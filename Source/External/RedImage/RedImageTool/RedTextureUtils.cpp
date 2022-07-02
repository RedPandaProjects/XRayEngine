#include "RedImage.hpp"
#include "ispc_texcomp/ispc_texcomp.h"
#include "Nvtt/nvimage/ColorBlock.h"
#include "Nvtt/nvimage/BlockDXT.h"
#include "Nvtt/nvtt/CompressorDX9.h"
#include "Nvtt/nvtt/QuickCompressDXT.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "StbImage/stb_image_resize.h"
using namespace RedImageTool;

template <typename T>
inline static T clamp(const T val, const T _low, const T _high) {
	if (val < _low) return _low;
	else if (val > _high)
		return _high;
	return val;
};

size_t RedTextureUtils::GetSizeWidth(size_t w, RedTexturePixelFormat format)
{
	if (isCompressor(format))
	{
		return GetSizeBlock(format)*GetCountBlock(w);
	}
	else
	{
		return GetSizePixel(format)*w;
	}
}

size_t RedTextureUtils::GetSizeDepth(size_t w, size_t h, RedTexturePixelFormat format)
{
	if (isCompressor(format))
	{
		return GetSizeBlock(format)*GetCountBlock(w)*GetCountBlock(h);
	}
	else
	{
		return GetSizePixel(format)*w*h;
	}
}

size_t RedTextureUtils::GetSizePixel(RedTexturePixelFormat format)
{
	switch (format)
	{
	case RedTexturePixelFormat::R8:
		return 1;
		break;
	case RedTexturePixelFormat::R8G8:
		return 2;
		break;
	case RedTexturePixelFormat::R8G8B8:
		return 3;
		break;
	case RedTexturePixelFormat::R8G8B8A8:
		return 4;
		break;
	case RedTexturePixelFormat::R32F:
		return 4;
		break;
	case RedTexturePixelFormat::R32G32F:
		return 8;
		break;
	case RedTexturePixelFormat::R32G32B32F:
		return 12;
		break;
	case RedTexturePixelFormat::R32G32B32A32F:
		return 16;
		break;
	default:
		assert(false);
		break;

	}
	return 0;
}

void RedTextureUtils::Append(u8*dst, size_t w_dst, size_t h_dst, size_t x_dst, size_t y_dst, u8*src, size_t w_src, size_t h_src, size_t squard_src_x, size_t squard_src_y, size_t squard_src_x1, size_t squard_src_y1, RedTexturePixelFormat dst_format, RedTexturePixelFormat src_format)
{
	if (isCompressor(dst_format))
	{
		RedTexturePixelFormat PixelFormatOut;
		u8*DestTemp = TempUncompressor(dst, w_dst, h_dst, dst_format, PixelFormatOut);
		Append(DestTemp, w_dst, h_dst, x_dst, y_dst, src, w_src, h_src, squard_src_x, squard_src_y, squard_src_x1, squard_src_y1, PixelFormatOut, src_format);
		TempCompress(DestTemp, dst, w_dst, h_dst, dst_format);
		return;
	}
	if (isCompressor(src_format))
	{
		RedTexturePixelFormat PixelFormatOut;
		u8*SourceTemp = TempUncompressor(src, w_src, h_src, src_format, PixelFormatOut);
		Append(dst, w_dst, h_dst, x_dst, y_dst, SourceTemp, w_src, h_src, squard_src_x, squard_src_y, squard_src_x1, squard_src_y1, dst_format, PixelFormatOut );
		TempCompress(SourceTemp, 0, w_src, h_src, src_format);
		return;
	}
	assert(w_dst >= squard_src_x1+x_dst);
	assert(h_dst >=  squard_src_y1+ y_dst);
	assert(w_src >= squard_src_x	+ squard_src_x1);
	assert(h_src >= squard_src_y + squard_src_y1);
	u8 SourceComp = GetCountComp(src_format);
	u8 DestComp = GetCountComp(dst_format);
	if (isFloatPixel(dst_format) && isFloatPixel(src_format))
	{
		for (size_t iy = 0; iy<squard_src_y1; iy++)
			for (size_t ix = 0; ix < squard_src_x1; ix++)
			{
				FloatPixelToFloat(GetPixelFloat(ix + x_dst, iy + y_dst, w_dst, DestComp, 0, dst), GetPixelFloat(ix+squard_src_x , iy + squard_src_y, w_src, SourceComp, 0, src), DestComp, SourceComp);
			}
	}
	else if (isFloatPixel(dst_format) )
	{
		for (size_t iy = 0; iy<squard_src_y1; iy++)
			for (size_t ix = 0; ix < squard_src_x1; ix++)
			{
				Uint8PixelToFloat(GetPixelFloat(ix + x_dst, iy + y_dst, w_dst, DestComp, 0, dst), GetPixelUint8(ix + squard_src_x, iy + squard_src_y, w_src, SourceComp, 0, src), DestComp, SourceComp);
			}
	}
	else if (isFloatPixel(src_format))
	{
		for (size_t iy = 0; iy<squard_src_y1; iy++)
			for (size_t ix = 0; ix < squard_src_x1; ix++)
			{
				FloatPixelToUint8(GetPixelUint8(ix + x_dst, iy + y_dst, w_dst, DestComp, 0, dst), GetPixelFloat(ix + squard_src_x, iy + squard_src_y, w_src, SourceComp, 0, src), DestComp, SourceComp);
			}
	}
	else
	{
		for (size_t iy = 0; iy<squard_src_y1; iy++)
			for (size_t ix = 0; ix < squard_src_x1; ix++)
			{
				Uint8PixelToUint8(GetPixelUint8(ix + x_dst, iy + y_dst, w_dst, DestComp, 0, dst), GetPixelUint8(ix + squard_src_x, iy + squard_src_y, w_src, SourceComp, 0, src), DestComp, SourceComp);
			}
	}
}


size_t RedTextureUtils::GetCountMips(size_t w, size_t h)
{
	size_t MaxSize = std::max(w, h);
	return static_cast<size_t>(log2f(static_cast<float>(MaxSize))+1);
}

size_t RedTextureUtils::GetMip(size_t w, size_t level)
{
	size_t Mip = static_cast<size_t>((w ) / pow(2, static_cast<double>(level)));
	return std::max(Mip, size_t(1));
}

u8 * RedTextureUtils::GetPixelUint8(size_t x, size_t y, size_t w, size_t comps, size_t comp_id, u8 * data)
{
	return  &data[(x + (y * w))*comps+comp_id];
}

float * RedTextureUtils::GetPixelFloat(size_t x, size_t y, size_t w, size_t comps, size_t comp_id, u8 * data)
{
	return (float*)&data[y*w*comps*4+x*4*comps+comp_id*4];
}

bool RedTextureUtils::isFloatPixel(RedTexturePixelFormat format)
{
	switch (format)
	{
	case RedTexturePixelFormat::R8:
	case RedTexturePixelFormat::R8G8:
	case RedTexturePixelFormat::R8G8B8:
	case RedTexturePixelFormat::R8G8B8A8:
		return false;;
		break;
	case RedTexturePixelFormat::R32F:
	case RedTexturePixelFormat::R32G32F:
	case RedTexturePixelFormat::R32G32B32F:
	case RedTexturePixelFormat::R32G32B32A32F:
		return true;
	case RedTexturePixelFormat::DXT_1:
	case RedTexturePixelFormat::DXT_1_Alpha:
	case RedTexturePixelFormat::DXT_3:
	case RedTexturePixelFormat::DXT_5:
	case RedTexturePixelFormat::BC4:
	case RedTexturePixelFormat::BC5:
	case RedTexturePixelFormat::BC6:
	case RedTexturePixelFormat::BC7:
		return false;

		break;
	default:
		assert(false);
	}
	return false;
}

bool RedTextureUtils::isCompressor(RedTexturePixelFormat format)
{
	switch (format)
	{
	case RedTexturePixelFormat::R8:
	case RedTexturePixelFormat::R8G8:
	case RedTexturePixelFormat::R8G8B8:
	case RedTexturePixelFormat::R8G8B8A8:
	case RedTexturePixelFormat::R32F:
	case RedTexturePixelFormat::R32G32F:
	case RedTexturePixelFormat::R32G32B32F:
	case RedTexturePixelFormat::R32G32B32A32F:
		return false;
		break;
	case RedTexturePixelFormat::DXT_1:
	case RedTexturePixelFormat::DXT_1_Alpha:
	case RedTexturePixelFormat::DXT_3:
	case RedTexturePixelFormat::DXT_5:
	case RedTexturePixelFormat::BC4:
	case RedTexturePixelFormat::BC5:
	case RedTexturePixelFormat::BC6:
	case RedTexturePixelFormat::BC7:
		return true;
	default:
		break;
	}
	return false;
}

void RedTextureUtils::Fill(u8 * data, size_t w, size_t h, size_t mip, const RedColor & color, RedTexturePixelFormat format)
{	
	if (isCompressor(format))
	{
		RedTexturePixelFormat  PixelFormatOut;
		size_t PixelSize = GetSizeBlock(format);
		for (size_t i = 0; i < mip; i++) 
		{

			size_t MipWidth = GetMip(w, i);
			size_t MipHeight = GetMip(h, i);
			u8*Temp = TempUncompressor(0, MipWidth, MipHeight, format, PixelFormatOut);

			if (isFloatPixel(PixelFormatOut))
			{
				FillFloat(Temp, MipWidth, MipHeight, color, PixelFormatOut);
			}
			else
			{
				FillUint8(Temp, MipWidth, MipHeight, color, PixelFormatOut);
			}
		
			TempCompress(Temp, data, MipWidth, MipHeight, format);
			data += PixelSize * GetCountBlock(MipWidth)*GetCountBlock(MipHeight);
		}

	}
	
	else if (isFloatPixel(format))
	{
		size_t PixelSize = GetSizePixel(format);
		for (size_t i = 0; i < mip; i++) {
			size_t MipWidth = GetMip(w, i);
			size_t MipHeight = GetMip(h, i);
			FillFloat(data, MipWidth, MipHeight, color, format);
			data += PixelSize * MipWidth*MipHeight;
		}
	}
	else
	{
		size_t PixelSize = GetSizePixel(format);
		for (size_t i = 0; i < mip; i++) {
			size_t MipWidth = GetMip(w, i);
			size_t MipHeight = GetMip(h, i);
			FillUint8(data, MipWidth, MipHeight, color, format);
			data += PixelSize * MipWidth*MipHeight;
		}
	}
}

size_t RedTextureUtils::GetSizeInMemory(size_t w, size_t h, size_t mips, RedTexturePixelFormat format)
{
	size_t Result = 0;
	if (!isCompressor(format))
	{
		size_t PixelSize = GetSizePixel(format);
		//Result += w * h * PixelSize;
		for (size_t i = 0; i < mips; i++)
		{
			size_t MipWidth = GetMip(w, i);
			size_t MipHeight = GetMip(h, i);
			Result += MipWidth * MipHeight * PixelSize;
		}
	}
	else
	{
		size_t size_block = GetSizeBlock(format);
	//	Result += GetCountBlock(w)*GetCountBlock(h)*size_block;
		for (size_t i = 0; i < mips; i++)
		{
			size_t MipWidth = GetMip(w, i);
			size_t MipHeight = GetMip(h, i);
			Result += GetCountBlock(MipWidth)*GetCountBlock(MipHeight)* size_block;
		}
	}
	return Result;
}

void RedTextureUtils::Convert(RedTexturePixelFormat dst_format, RedTexturePixelFormat src_format, u8 * dst, u8 * src, size_t w, size_t h)
{
	if (src_format == dst_format) { memcpy(dst, src, GetSizeInMemory(w, h, 1, dst_format)); return; }
	if (isFloatPixel(dst_format)&& isCompressor(src_format))
	{
		CompressorToFloat(dst, src, w, h, GetCountComp(dst_format), src_format);
	}
	else if (isFloatPixel(dst_format) && isFloatPixel(src_format))
	{
		FloatToFloat(dst, src, w, h, GetCountComp(dst_format), GetCountComp(src_format));
	}
	else if (isFloatPixel(dst_format))
	{
		Uint8ToFloat(dst, src, w, h, GetCountComp(dst_format), GetCountComp(src_format));
	}
	else if (isCompressor(dst_format) && isCompressor(src_format))
	{
		CompressorToCompressor(dst, src, w, h, dst_format, src_format);
	}
	else if (isCompressor(dst_format) && isFloatPixel(src_format))
	{
		FloatToCompressor(dst, src, w, h, dst_format, GetCountComp(src_format));
	}
	else  if (isCompressor(dst_format))
	{
		Uint8ToCompressor(dst, src, w, h, dst_format, GetCountComp(src_format));
	}
	else if (isCompressor(src_format))
	{
		CompressorToUint8(dst, src, w, h, GetCountComp(dst_format), src_format);
	}
	else if (isFloatPixel(src_format))
	{
		FloatToUint8(dst, src, w, h, GetCountComp(dst_format), GetCountComp(src_format));
	}
	else
	{
		Uint8ToUint8(dst, src, w, h, GetCountComp(dst_format), GetCountComp(src_format));
	}
}

size_t RedTextureUtils::GetCountBlock(size_t w)
{
	return (w + 3) / 4;
}

size_t RedTextureUtils::GetCountBlock(size_t w, RedTexturePixelFormat format)
{
	if (isCompressor(format))
	{
		return GetCountBlock(w);
	}
	else
	{
		return w;
	}
}

size_t RedTextureUtils::GetSizeBlock(RedTexturePixelFormat format)
{
	switch (format)
	{
	case RedTexturePixelFormat::DXT_1:
		return 8;
	case RedTexturePixelFormat::DXT_1_Alpha:
		return 8;
	case RedTexturePixelFormat::DXT_3:
		return 16;
	case RedTexturePixelFormat::DXT_5:
		return 16;
	case RedTexturePixelFormat::BC4:
		return 8;
		break;
	case RedTexturePixelFormat::BC5:
		return 16;
		break;
	case RedTexturePixelFormat::BC6:
		return 16;
		break;
	case RedTexturePixelFormat::BC7:
		return 16;
		break;
	default:
		assert(false);
	}
	return 0;
}	

u8 RedTextureUtils::GetCountComp(RedTexturePixelFormat format)
{
	switch (format)
	{
	case RedTexturePixelFormat::R8:
		return 1;
		break;
	case RedTexturePixelFormat::R8G8:
		return 2;
		break;
	case RedTexturePixelFormat::R8G8B8:
		return 3;
		break;
	case RedTexturePixelFormat::R8G8B8A8:
		return 4;
		break;
	case RedTexturePixelFormat::R32F:
		return 1;
		break;
	case RedTexturePixelFormat::R32G32F:
		return 2;
		break;
	case RedTexturePixelFormat::R32G32B32F:
		return 3;
		break;
	case RedTexturePixelFormat::R32G32B32A32F:
		return 4;
	default:
		assert(false);
	}
	return 0;
}

void RedTextureUtils::GenerateMip(u8 * dst, u8 * src, size_t w_src, size_t h_src, RedTexturePixelFormat format, RedResizeFilter filter)
{
	if (isCompressor(format))
	{
		RedTexturePixelFormat PixelFormatOut;
		u8*SourceTemp = TempUncompressor(src, w_src, h_src, format, PixelFormatOut);
		u8*DestTemp = TempUncompressor(0, w_src/2, h_src/2, format, PixelFormatOut);
		GenerateMip(DestTemp, SourceTemp, w_src, h_src, PixelFormatOut, filter);
		TempCompress(SourceTemp, 0, w_src, h_src, format);
		TempCompress(DestTemp, dst, w_src / 2, h_src / 2, format);
	}
	else if (isFloatPixel(format))
	{
		GenerateMipFloat(dst, src, w_src, h_src,GetCountComp(format), filter);
	}
	else
	{
		GenerateMipUint8(dst, src, w_src, h_src, GetCountComp(format), filter);
	}
}

void RedTextureUtils::Scale(u8 * dst, size_t w_dst, size_t h_dst, u8 * src, size_t w_src, size_t h_src, RedTexturePixelFormat format, RedResizeFilter filter)
{
	if (isCompressor(format))
	{
		RedTexturePixelFormat PixelFormatOut;
		u8*SourceTemp = TempUncompressor(src, w_src, h_src, format, PixelFormatOut);
		u8*DestTemp = TempUncompressor(0, w_dst, h_dst, format, PixelFormatOut);
		Scale(DestTemp, w_dst, h_dst, SourceTemp, w_src, h_src, PixelFormatOut,filter);
		TempCompress(SourceTemp, 0, w_src, h_src, format);
		TempCompress(DestTemp, dst, w_dst, h_dst, format);
	}
	else if (isFloatPixel(format))
	{
		ScaleFloat(dst, w_dst, h_dst, src, w_src, h_src, GetCountComp(format), filter);
	}
	else
	{
		ScaleUint8(dst, w_dst, h_dst, src, w_src, h_src, GetCountComp(format), filter);
	}
}

u8 * RedTextureUtils::GetImage(u8 * data, size_t w, size_t h, size_t mips, size_t depth, size_t mip, RedTexturePixelFormat format)
{
	return data + RedTextureUtils::GetSizeInMemory(w, h, mips, format)*depth + RedTextureUtils::GetSizeInMemory(w, h, mip, format);
}

void RedTextureUtils::GetPixel(RedColor & color,  u8*data, size_t x, size_t y, size_t depth, size_t w, size_t h, size_t mips, RedTexturePixelFormat format)
{
	color.SetAsFloat(0, 0, 0);
	u8*Image = data + RedTextureUtils::GetSizeInMemory(w, h, mips, format)*depth;
	if (isCompressor(format))
	{
		RedColor Colors[16];
		GetBlock(Colors, Image, w, h, x, y, format);
		color = Colors[x % 4 +( 4 * (y % 4))];
	}
	else if (isFloatPixel(format))
	{
		float* pixels = GetPixelFloat(x, y, w, GetCountComp(format), 0, data);
		switch ( GetCountComp(format))
		{
		case 1:
			color.SetAsFloat(  pixels[0],0,0);
			break;
		case 2:
			color.SetAsFloat(pixels[0], pixels[1], 0);
			break;
		case 3:
			color.SetAsFloat(pixels[0], pixels[1], pixels[2]);
			break;
		case 4:
			color.SetAsFloat(pixels[0], pixels[1], pixels[2], pixels[3]);
			break;
		default:
			assert(false);
			break;
		}
	}
	else 
	{
		u8* pixels = GetPixelUint8(x, y, w, GetCountComp(format), 0, data);
		switch (GetCountComp(format))
		{
		case 1:
			color.SetAsUint8(pixels[0], 0, 0);
			break;
		case 2:
			color.SetAsUint8(pixels[0], pixels[1], 0);
			break;
		case 3:
			color.SetAsUint8(pixels[0], pixels[1], pixels[2]);
			break;
		case 4:
			color.SetAsUint8(pixels[0], pixels[1], pixels[2], pixels[3]);
			break;
		default:
			assert(false);
			break;
		}
	}

	return;
}

void RedTextureUtils::SetPixel(const RedColor & color, u8 * data, size_t x, size_t y,size_t depth, size_t w, size_t h, size_t mips, RedTexturePixelFormat format)
{
	u8*Image = data + RedTextureUtils::GetSizeInMemory(w, h, mips, format)*depth;
	if (isCompressor(format))
	{
		RedColor Colors[16];
		GetBlock(Colors, Image, w, h, x, y, format);
		Colors[x % 4 + (4 * (y % 4))] = color;
		SetBlock(Colors, Image, w, h, x, y, format);
	}
	else if (isFloatPixel(format))
	{
		float* pixels = GetPixelFloat(x, y, w, GetCountComp(format), 0, data);
		switch (GetCountComp(format))
		{
		case 1:
			memcpy(pixels, color.R32G32B32A32, 1*sizeof(float));
			break;
		case 2:
			memcpy(pixels, color.R32G32B32A32,2 * sizeof(float));
			break;
		case 3:
			memcpy(pixels, color.R32G32B32A32, 3 * sizeof(float));
			break;
		case 4:
			memcpy(pixels, color.R32G32B32A32, 4 * sizeof(float));
			break;
		default:
			assert(false);
			break;
		}
	}
	else
	{
		u8* pixels = GetPixelUint8(x, y, w, GetCountComp(format), 0, data);
		switch (GetCountComp(format))
		{
		case 1:
			memcpy(pixels, &color.R8G8B8A8, 1);
			break;
		case 2:
			memcpy(pixels, &color.R8G8B8A8, 2);
			break;
		case 3:
			memcpy(pixels, &color.R8G8B8A8,3);
			break;
		case 4:
			memcpy(pixels, &color.R8G8B8A8, 4);
			break;
		default:
			assert(false);
			break;
		}
	}

	return;
}

void RedTextureUtils::ScaleFloat(u8 * dst, size_t w_dst, size_t h_dst, u8 * src, size_t w_src, size_t h_src, u8 comp, RedResizeFilter filter)
{
	stbir_filter Filter = STBIR_FILTER_DEFAULT;
	switch (filter)
	{
	case RedResizeFilter::Box:
		Filter = STBIR_FILTER_BOX;
		break;
	case RedResizeFilter::Triangle:
		Filter = STBIR_FILTER_TRIANGLE;
		break;
	case RedResizeFilter::Cubicbspline:
		Filter = STBIR_FILTER_CUBICBSPLINE;
		break;
	case RedResizeFilter::Catmullrom:
		Filter = STBIR_FILTER_CATMULLROM;
		break;
	case RedResizeFilter::Mitchell:
		Filter = STBIR_FILTER_MITCHELL;
		break;
	}
	stbir_resize(reinterpret_cast<void*>(src), static_cast<int>(w_src), static_cast<int>(h_src), 0, reinterpret_cast<void*>(dst), static_cast<int>(w_dst), static_cast<int>(h_dst), 0, STBIR_TYPE_FLOAT, comp, -1, 0, STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP, Filter, Filter, STBIR_COLORSPACE_LINEAR, NULL);
}

void RedTextureUtils::ScaleUint8(u8 * dst, size_t w_dst, size_t h_dst, u8 * src, size_t w_src, size_t h_src, u8 comp, RedResizeFilter filter)
{
	stbir_filter Filter = STBIR_FILTER_DEFAULT;
	switch (filter)
	{
	case RedResizeFilter::Box:
		Filter = STBIR_FILTER_BOX;
		break;
	case RedResizeFilter::Triangle:
		Filter = STBIR_FILTER_TRIANGLE;
		break;
	case RedResizeFilter::Cubicbspline:
		Filter = STBIR_FILTER_CUBICBSPLINE;
		break;
	case RedResizeFilter::Catmullrom:
		Filter = STBIR_FILTER_CATMULLROM;
		break;
	case RedResizeFilter::Mitchell:
		Filter = STBIR_FILTER_MITCHELL;
		break;
	}
	stbir_resize(reinterpret_cast<void*>(src), static_cast<int>(w_src), static_cast<int>(h_src), 0, reinterpret_cast<void*>(dst), static_cast<int>(w_dst), static_cast<int>(h_dst), 0, STBIR_TYPE_UINT8, comp, -1, 0, STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP, Filter, Filter, STBIR_COLORSPACE_LINEAR, NULL);

}



void RedTextureUtils::GenerateMipFloat(u8 * dst, u8 * src, size_t w_src, size_t h_src, u8 comps, RedResizeFilter filter)
{
	ScaleFloat(dst, w_src / 2, h_src / 2, src, w_src, h_src, comps, filter);
}

void RedTextureUtils::GenerateMipUint8(u8 * dst, u8 * src, size_t w_src, size_t h_src, u8 comps, RedResizeFilter filter)
{
	ScaleUint8(dst, w_src / 2, h_src / 2, src, w_src, h_src, comps, filter);
}

void RedTextureUtils::FloatPixelToUint8(u8 * dst, float * src, u8 comp_dst, u8 comp_src)
{
	for (u8 i = 0; i < comp_dst; i++)
	{
		if (i < comp_src)
		{
			* dst = static_cast<u8>(clamp(*src, 0.f, 1.f) *255.f);
			src++;
		}
		else if(i==3)
		{
			*dst = 255;
		}
		else
		{
			*dst = 0;
		}
		dst++;
	}
}

void RedTextureUtils::FloatPixelToFloat(float * dst, float * src, u8 comp_dst, u8 comp_src)
{
	for (u8 i = 0; i < comp_dst; i++)
	{
		if (i < comp_src)
		{
			*dst = *src;
			src++;
		}
		else if (i == 3)
		{
			*dst = 1.f;
		}
		else
		{
			*dst = 0.f;
		}
		dst++;
	}
}

void RedTextureUtils::Uint8PixelToFloat(float * dst, u8 * src, u8 comp_dst, u8 comp_src)
{
	for (u8 i = 0; i < comp_dst; i++)
	{
		if (i < comp_src)
		{
			*dst = static_cast<float>(*src/255.f);
			src++;
		}
		else if (i == 3)
		{
			*dst = 1.f;
		}
		else
		{
			*dst = 0.f;
		}
		dst++;
	}
}
void RedTextureUtils::Uint8PixelToUint8(u8 * dst, u8 * src, u8 comp_dst, u8 comp_src)
{
	for (u8 i = 0; i < comp_dst; i++)
	{
		if (i < comp_src)
		{
			*dst = *src;
			src++;
		}
		else if (i == 3)
		{
			*dst = 255;
		}
		else
		{
			*dst = 255;
		}
		dst++;
	}
}
u16 floatToHalf(float f)
{
	u32 fred_uint = *((u32*)&f);
	u16 a = ((fred_uint >> 16) & 0b1000000000000000) | ((((fred_uint & 0b1111111100000000000000000000000) - 0b111000000000000000000000000000) >> 13) & 0b111110000000000) | ((fred_uint >> 13) & 0b1111111111);
	if (f == 0)
		return 0;
	return a;
}

void RedTextureUtils::FloatPixelToHalf4(u16 * dst, float * src, u8 comp_src)
{
	for (u8 i = 0; i < 4; i++)
	{
		if (i < comp_src)
		{
			if(i==3)
				*dst = floatToHalf(1.f);
			else
				*dst = floatToHalf(*src);
			src++;
		}
		else if (i == 3)
		{
			*dst = floatToHalf(1.f);
		}
		else
		{
			*dst = floatToHalf(0.f);
		}
		dst++;
	}
}

void RedTextureUtils::Uint8PixelToHalf4(u16 * dst, u8 * src, u8 comp_src)
{
	for (u8 i = 0; i < 4; i++)
	{
		if (i < comp_src)
		{
			if (i == 3)
				*dst = floatToHalf(1.f);
			else
			*dst = floatToHalf(static_cast<float>(*src / 255.f));
			src++;
		}
		else if (i == 3)
		{
			*dst = floatToHalf(1.f);
		}
		else
		{
			*dst = floatToHalf(0.f);
		}
		dst++;
	}
}

void RedTextureUtils::SwapRB(u32 & color)
{
		u32 R = color & 0x000000FF;

		u32 B = color & 0x00FF0000;
		u32 G = color & 0x0000FF00;
		u32 A = color & 0xFF000000;
		color = (B >> 16) | G | (R << 16) | A;
}

void RedTextureUtils::FillUint8(u8 * data, size_t x, size_t y, const RedColor & color, RedTexturePixelFormat format)
{
	size_t SizePixel = GetSizePixel(format);
	for (size_t i = 0; i < x*y; i++)
	{
		memcpy(data + (SizePixel*i), &color.R8G8B8A8, SizePixel);
	}
}

void RedTextureUtils::FillFloat(u8 * data, size_t x, size_t y, const RedColor & color, RedTexturePixelFormat format)
{
	size_t SizePixel = GetSizePixel(format);
	for (size_t i = 0; i < x*y; i++)
	{
		memcpy(data + (SizePixel*i), color.R32G32B32A32, SizePixel);
	}
}

void RedTextureUtils::Uint8ToHalf4(u8 * dst, u8 * src, size_t w, size_t h, u8 comp_src)
{
	for (size_t i = 0; i < w*h; i++)
	{
		Uint8PixelToHalf4((u16*)dst, src, comp_src);
		dst += 4*2;
		src += comp_src;
	}
}

void RedTextureUtils::Uint8ToFloat(u8 * dst, u8 * src, size_t w, size_t h, u8 comp_dst, u8 comp_src)
{
	for (size_t i = 0; i < w*h; i++)
	{
		Uint8PixelToFloat((float*)dst, src, comp_dst, comp_src);
		dst += comp_dst * 4;
		src += comp_src;
	}
}

void RedTextureUtils::Uint8ToUint8(u8 * dst, u8 * src, size_t w, size_t h, u8 comp_dst, u8 comp_src)
{
	for (size_t i = 0; i < w*h; i++)
	{
		Uint8PixelToUint8(dst, src, comp_dst, comp_src);
		dst += comp_dst ;
		src += comp_src;
	}
}

void RedTextureUtils::Uint8ToCompressor(u8 * dst, u8 * src, size_t w, size_t h, RedTexturePixelFormat compressor, u8 comp_src)
{
	u8*In=(u8*)StartCompressor(compressor,w,h);
	switch (compressor)
	{
	case RedTexturePixelFormat::DXT_1:
		Uint8ToFloat(In, src, w, h, 3, comp_src);
		break;
	case RedTexturePixelFormat::DXT_1_Alpha:
		Uint8ToUint8(In, src, w, h, 4, comp_src);
		break;
	case RedTexturePixelFormat::DXT_3:
		Uint8ToUint8(In, src, w, h, 4, comp_src);
		break;
	case RedTexturePixelFormat::DXT_5:
		Uint8ToUint8(In, src, w, h, 4, comp_src);
		break;
	case RedTexturePixelFormat::BC4:
		Uint8ToUint8(In, src, w, h, 1, comp_src);
		break;
	case RedTexturePixelFormat::BC5:
		Uint8ToUint8(In, src, w, h, 2, comp_src);
		break;
	case RedTexturePixelFormat::BC6:
		Uint8ToHalf4(In, src, w, h,  comp_src);
		break;
	case RedTexturePixelFormat::BC7:
		Uint8ToUint8(In, src, w, h, 4, comp_src);
		break;
	default:
		assert(false);
	}

	EndCompressor(compressor, w, h, In, dst);
}

void RedTextureUtils::FloatToHalf4(u8 * dst, u8 * src, size_t w, size_t h, u8 comp_src)
{
	for (size_t i = 0; i < w*h; i++)
	{
		Uint8PixelToHalf4((u16*)dst, src, comp_src);
		dst += 4 * 2;
		src += comp_src*4;
	}
}

void RedTextureUtils::FloatToUint8(u8 * dst, u8 * src, size_t w, size_t h, u8 comp_dst, u8 comp_src)
{
	for (size_t i = 0; i < w*h; i++)
	{
		FloatPixelToUint8(dst, (float*)src, comp_dst, comp_src);
		dst += comp_dst ;
		src += comp_src * 4;
	}
}

void RedTextureUtils::FloatToFloat(u8 * dst, u8 * src, size_t w, size_t h, u8 comp_dst, u8 comp_src)
{
	for (size_t i = 0; i < w*h; i++)
	{
		FloatPixelToFloat((float*)dst, (float*)src, comp_dst, comp_src);
		dst += comp_dst * 4;
		src += comp_src * 4;
	}
}

void RedTextureUtils::FloatToCompressor(u8 * dst, u8 * src, size_t w, size_t h, RedTexturePixelFormat compressor, u8 comp_src)
{
	u8*in = (u8*)StartCompressor(compressor, w, h);
	switch (compressor)
	{
	case RedTexturePixelFormat::DXT_1:
		FloatToFloat(in, src, w, h, 3, comp_src);
		break;
	case RedTexturePixelFormat::DXT_1_Alpha:
		FloatToUint8(in, src, w, h, 4, comp_src);
		break;
	case RedTexturePixelFormat::DXT_3:
		FloatToUint8(in, src, w, h, 4, comp_src);
		break;
	case RedTexturePixelFormat::DXT_5:
		FloatToUint8(in, src, w, h, 4, comp_src);
		break;
	case RedTexturePixelFormat::BC4:
		FloatToUint8(in, src, w, h, 1, comp_src);
		break;
	case RedTexturePixelFormat::BC5:
		FloatToUint8(in, src, w, h, 2, comp_src);
		break;
	case RedTexturePixelFormat::BC6:
		FloatToHalf4(in, src, w, h, comp_src);
		break;
	case RedTexturePixelFormat::BC7:
		FloatToUint8(in, src, w, h, 4, comp_src);
		break;
	default:
		assert(false);
	}

	EndCompressor(compressor, w, h, in, dst);
}

void RedTextureUtils::CompressorToCompressor(u8 * dst, u8 * src, size_t w, size_t h, RedTexturePixelFormat compressor_dst, RedTexturePixelFormat compressor_src)
{

	u8*in_dst = (u8*)StartCompressor(compressor_dst, w, h);
	switch (compressor_dst)
	{
	case RedTexturePixelFormat::DXT_1:
		CompressorToFloat(in_dst, src, w, h, 3, compressor_src);
		break;
	case RedTexturePixelFormat::DXT_1_Alpha:
		CompressorToUint8(in_dst, src, w, h, 4, compressor_src);
		break;
	case RedTexturePixelFormat::DXT_3:
		CompressorToUint8(in_dst, src, w, h, 4, compressor_src);
		break;
	case RedTexturePixelFormat::DXT_5:
		CompressorToUint8(in_dst, src, w, h, 4, compressor_src);
		break;
	case RedTexturePixelFormat::BC4:
		CompressorToUint8(in_dst, src, w, h, 1, compressor_src);
		break;
	case RedTexturePixelFormat::BC5:
		CompressorToUint8(in_dst, src, w, h, 2, compressor_src);
		break;
	case RedTexturePixelFormat::BC6:
	{		
	float*temp = red_alloc<float>(w*h * 3);
	CompressorToFloat((u8*)temp, src, w, h, 3, compressor_src);
	FloatToHalf4(dst, (u8*)temp, w, h, 3);
	free(temp);
	}
		break;
	case RedTexturePixelFormat::BC7:
		CompressorToUint8(in_dst, src, w, h, 4, compressor_src);
		break;
	default:
		assert(false);
	}
	EndCompressor(compressor_dst, w, h, in_dst, dst);
}

void RedTextureUtils::CompressorToUint8(u8 * dst, u8 * src, size_t w, size_t h, u8 comp_dst, RedTexturePixelFormat compressor)
{
	u8*in=(u8*)StartDecompressor(compressor, w, h, src);
	switch (compressor)
	{
	case RedTexturePixelFormat::DXT_1:
	case RedTexturePixelFormat::DXT_1_Alpha:
		Uint8ToUint8(dst, in, w, h, comp_dst, 4);
		break;
	case RedTexturePixelFormat::DXT_3:
		Uint8ToUint8(dst, in, w, h, comp_dst, 4);
		break;
	case RedTexturePixelFormat::DXT_5:
		Uint8ToUint8(dst, in, w, h, comp_dst, 4);
		break;
	case RedTexturePixelFormat::BC4:
		Uint8ToUint8(dst, in, w, h, comp_dst, 1);
		break;
	case RedTexturePixelFormat::BC5:
		Uint8ToUint8(dst, in, w, h, comp_dst, 2);
		break;
	case RedTexturePixelFormat::BC6:
		FloatToUint8(dst, in, w, h, comp_dst, 3);
		break;
	case RedTexturePixelFormat::BC7:
		Uint8ToUint8(dst, in, w, h, comp_dst, 4);
		break;
	default:
		assert(false);
	}
	EndDecompressor(in);
}

void RedTextureUtils::CompressorToFloat(u8 * dst, u8 * src, size_t w, size_t h, u8 comp_dst, RedTexturePixelFormat compressor)
{
	u8*in = (u8*)StartDecompressor(compressor, w, h, src);
	switch (compressor)
	{
	case RedTexturePixelFormat::DXT_1:
	case RedTexturePixelFormat::DXT_1_Alpha:
		Uint8ToFloat(dst, in, w, h, comp_dst, 4);
		break;
	case RedTexturePixelFormat::DXT_3:
		Uint8ToFloat(dst, in, w, h, comp_dst, 4);
		break;
	case RedTexturePixelFormat::DXT_5:
		Uint8ToFloat(dst, in, w, h, comp_dst, 4);
		break;
	case RedTexturePixelFormat::BC4:
		Uint8ToFloat(dst, in, w, h, comp_dst, 1);
		break;
	case RedTexturePixelFormat::BC5:
		Uint8ToFloat(dst, in, w, h, comp_dst, 2);
		break;
	case RedTexturePixelFormat::BC6:
		FloatToFloat(dst, in, w, h, comp_dst, 3);
		break;
	case RedTexturePixelFormat::BC7:
		Uint8ToFloat(dst, in, w, h, comp_dst, 4);
		break;
	default:
		assert(false);
	}
	EndDecompressor(in);
}

void * RedTextureUtils::StartCompressor(RedTexturePixelFormat compressor, size_t w, size_t h)
{
	switch (compressor)
	{
	case RedTexturePixelFormat::DXT_1:
		return red_alloc<u8>(GetSizeWidth(w, RedTexturePixelFormat::R32G32B32F)*h);
	case RedTexturePixelFormat::DXT_1_Alpha:
		return red_alloc<u8>(GetSizeWidth(w, RedTexturePixelFormat::R8G8B8A8)*h);
	case RedTexturePixelFormat::DXT_3:
		return red_alloc<u8>(GetSizeWidth(w, RedTexturePixelFormat::R8G8B8A8)*h);
	case RedTexturePixelFormat::DXT_5:
		return red_alloc<u8>(GetSizeWidth(w, RedTexturePixelFormat::R8G8B8A8)*h);
	case RedTexturePixelFormat::BC4:
		return red_alloc<u8>(GetSizeWidth(w, RedTexturePixelFormat::R8)*h);
	case RedTexturePixelFormat::BC5:
		return red_alloc<u8>(GetSizeWidth(w, RedTexturePixelFormat::R8G8)*h);
	case RedTexturePixelFormat::BC6:
		return red_alloc<u8>(w*2*4*h);
	case RedTexturePixelFormat::BC7:
		return red_alloc<u8>(GetSizeWidth(w, RedTexturePixelFormat::R8G8B8A8)*h);
	default:
		assert(false);;
	}
	return 0;
}

void RedTextureUtils::EndCompressor(RedTexturePixelFormat compressor, size_t w, size_t h, void*in, void*out)
{

	switch (compressor)
	{
	case RedTexturePixelFormat::DXT_1:
	{
		nvtt::CompressionOptions CompOpt;
		CompOpt.setQuality(nvtt::Quality_Highest);
		CompOpt.setQuantization(false, false, true);

		CompOpt.setFormat(nvtt::Format_DXT1);

		const u32 BlockWidth =static_cast<u32>( (w + 3) / 4);
		const u32 BlockHeight = static_cast<u32>((h + 3) / 4);
		nv::CompressorDXT1 Compressor;
		for (u32 by = 0; by < BlockHeight; by++)
		{
			for (u32 bx = 0; bx < BlockWidth; bx++)
			{
				float wa[16];
				nv::Vector4 Block[16];
				for (size_t y = 0; y < std::min(size_t(4), h - 4 * by); y++)
				{
					for (size_t x = 0; x < std::min(size_t(4), w - 4 * bx); x++)
					{
						wa[x + y * 4] = 1.f;

						Block[x + y * 4].component[0] = *GetPixelFloat( bx * 4+x,  by * 4+y, w, 3,0,(u8*) in);
						Block[x + y * 4].component[1] = *(GetPixelFloat( bx * 4+x,  by * 4+y, w, 3, 1,(u8*)in));
						Block[x + y * 4].component[2] = *(GetPixelFloat( bx * 4+x,  by * 4+y, w, 3, 2,(u8*)in));
						Block[x + y * 4].component[3] = 1;


					}
				}
				Compressor.compressBlock(Block, wa, CompOpt.m, (u8*)out + 8 * (bx + (by * BlockWidth)));
			}
		}
	}
	break;
	case RedTexturePixelFormat::DXT_1_Alpha:
	{
		nvtt::CompressionOptions CompOpt;
		CompOpt.setQuality(nvtt::Quality_Highest);
		CompOpt.setQuantization(false, false, true);

		CompOpt.setFormat(nvtt::Format_DXT1a);

		const size_t BlockWidth = static_cast<size_t>((w + 3) / 4);
		const size_t BlockHeight = static_cast<size_t>((h + 3) / 4);
		nv::CompressorDXT1a Compressor;
		for (size_t by = 0; by < BlockHeight; by++)
		{
			for (size_t bx = 0; bx < BlockWidth; bx++)
			{
				nv::ColorBlock Block;
				for (size_t y = 0; y < std::min(size_t(4), h - 4 * by); y++)
				{
					for (size_t x = 0; x < std::min(size_t(4), w - 4 * bx); x++)
					{
						auto&color = Block.color(static_cast<uint>(x), static_cast<uint>(y));
						color.r = *GetPixelUint8(bx * 4 + x, by * 4 + y, w, 4, 0, (u8*)in);
						color.g = *GetPixelUint8(bx * 4 + x, by * 4 + y, w, 4, 1, (u8*)in);
						color.b = *GetPixelUint8(bx * 4 + x, by * 4 + y, w, 4, 2, (u8*)in);
						color.a = *GetPixelUint8(bx * 4 + x, by * 4 + y, w, 4, 3, (u8*)in);
						color.a = color.a > 127 ? 255 : 0;
					}
				}
				Compressor.compressBlock(Block, nvtt::AlphaMode_Transparency, CompOpt.m, (u8*)out + 8 * (bx + (by * BlockWidth)));
			}
		}
	}
	break;
	case RedTexturePixelFormat::DXT_3:
	{
		nvtt::CompressionOptions CompOpt;
		CompOpt.setQuality(nvtt::Quality_Highest);
		CompOpt.setQuantization(false, false, true);

		CompOpt.setFormat(nvtt::Format_DXT3);
		const size_t BlockWidth = static_cast<size_t>((w + 3) / 4);
		const size_t BlockHeight = static_cast<size_t>((h + 3) / 4);
		nv::CompressorDXT3 Compressor;
		for (size_t by = 0; by < BlockHeight; by++)
		{
			for (size_t bx = 0; bx < BlockWidth; bx++)
			{
				nv::ColorBlock Block;
				for (size_t y = 0; y < std::min(size_t(4), h - 4 * by); y++)
				{
					for (size_t x = 0; x < std::min(size_t(4), w - 4 * bx); x++)
					{
						auto& color = Block.color(static_cast<uint>(x), static_cast<uint>(y));
						color.r = *GetPixelUint8(bx * 4 + x, by * 4 + y, w, 4, 0, (u8*)in);
						color.g = *GetPixelUint8(bx * 4 + x, by * 4 + y, w, 4, 1, (u8*)in);
						color.b = *GetPixelUint8(bx * 4 + x, by * 4 + y, w, 4, 2, (u8*)in);
						color.a = *GetPixelUint8(bx * 4 + x, by * 4 + y, w, 4, 3, (u8*)in);

					}
				}
				Compressor.compressBlock(Block, nvtt::AlphaMode_Transparency, CompOpt.m, (u8*)out + 16 * (bx + by * BlockWidth));
			}
		}
	}
	break;
	case RedTexturePixelFormat::DXT_5:
	{
		nvtt::CompressionOptions CompOpt;
		CompOpt.setQuality(nvtt::Quality_Highest);
		CompOpt.setQuantization(false, false, true);

		CompOpt.setFormat(nvtt::Format_DXT5);

		const size_t BlockWidth = static_cast<size_t>((w + 3) / 4);
		const size_t BlockHeight = static_cast<size_t>((h + 3) / 4);
		nv::CompressorDXT5 Compressor;
		for (size_t by = 0; by < BlockHeight; by++)
		{
			for (size_t bx = 0; bx < BlockWidth; bx++)
			{
				nv::ColorBlock Block;
				for (size_t y = 0; y < std::min(size_t(4), h - 4 * by); y++)
				{
					for (size_t x = 0; x < std::min(size_t(4), w - 4 * bx); x++)
					{
						auto& color = Block.color(static_cast<uint>(x), static_cast<uint>(y));
						color.r = *GetPixelUint8(bx * 4 + x, by * 4 + y, w, 4, 0, (u8*)in);
						color.g = *GetPixelUint8(bx * 4 + x, by * 4 + y, w, 4, 1, (u8*)in);
						color.b = *GetPixelUint8(bx * 4 + x, by * 4 + y, w, 4, 2, (u8*)in);
						color.a = *GetPixelUint8(bx * 4 + x, by * 4 + y, w, 4, 3, (u8*)in);

					}
				}
				Compressor.compressBlock(Block, nvtt::AlphaMode_Transparency, CompOpt.m, (u8*)out + 16 * (bx + by * BlockWidth));
			}
		}
	}
	break;
	case RedTexturePixelFormat::BC4:
	{
		const size_t BlockWidth = static_cast<size_t>((w + 3) / 4);
		const size_t BlockHeight = static_cast<size_t>((h + 3) / 4);

		for (size_t by = 0; by < BlockHeight; by++)
		{
			for (size_t bx = 0; bx < BlockWidth; bx++)
			{
				nv::ColorBlock Block;
				nv::AlphaBlock4x4 Alpha1;
				nv::AlphaBlockDXT5 AlphaBlock1;
				for (size_t y = 0; y < std::min(size_t(4), h - 4 * by); y++)
				{
					for (size_t x = 0; x < std::min(size_t(4), w - 4 * bx); x++)
					{
						auto& color = Block.color(static_cast<uint>(x), static_cast<uint>(y));
						color.b = *GetPixelUint8(bx * 4 + x, by * 4 + y, w, 1, 0, (u8*)in);
					}
				}
				Alpha1.init(Block, 2);
				nv::QuickCompress::compressDXT5A(Alpha1, &AlphaBlock1, 8);
				memcpy((u8*)out + 8 * (bx + by * BlockWidth), &AlphaBlock1.u, 8);
			}
		}
		break;
	}
	case RedTexturePixelFormat::BC5:
	{
		const size_t BlockWidth = static_cast<size_t>((w + 3) / 4);
		const size_t BlockHeight = static_cast<size_t>((h + 3) / 4);

		for (size_t by = 0; by < BlockHeight; by++)
		{
			for (size_t bx = 0; bx < BlockWidth; bx++)
			{
				nv::ColorBlock Block;
				nv::AlphaBlock4x4 Alpha1, Alpha2;
				nv::AlphaBlockDXT5 AlphaBlock1, AlphaBlock2;
				for (size_t y = 0; y < std::min(size_t(4), h - 4 * by); y++)
				{
					for (size_t x = 0; x < std::min(size_t(4), w - 4 * bx); x++)
					{
						auto& color = Block.color(static_cast<uint>(x), static_cast<uint>(y));
						color.b = *GetPixelUint8(bx * 4 + x, by * 4 + y, w, 2, 0, (u8*)in);
						color.g = *GetPixelUint8(bx * 4 + x, by * 4 + y, w, 2,1, (u8*)in);
						//in = (u8*)in + 2;
					}
				}
				Alpha1.init(Block, 2);
				Alpha2.init(Block, 1);
				nv::QuickCompress::compressDXT5A(Alpha1, &AlphaBlock1, 8);
				nv::QuickCompress::compressDXT5A(Alpha2, &AlphaBlock2, 8);
				memcpy((u8*)out + 16 * (bx + by * BlockWidth), &AlphaBlock1.u, 8);
				memcpy((u8*)out + 16 * (bx + by * BlockWidth) + 8, &AlphaBlock2.u, 8);
			}
		}
	}
	break;
	case RedTexturePixelFormat::BC6:
	{
		rgba_surface Surface;
		Surface.height = static_cast<u32>(h);
		Surface.width = static_cast<u32>(w);
		Surface.ptr = (u8*)in;
		Surface.stride = static_cast<u32>(w * 2 * 4);
		bc6h_enc_settings str;
		GetProfile_bc6h_veryslow(&str);
		CompressBlocksBC6H(&Surface, (u8*)out, &str);
	}
	break;
	case RedTexturePixelFormat::BC7:
	{
		rgba_surface Surface;
		Surface.height = static_cast<u32>(h);
		Surface.width = static_cast<u32>(w);
		Surface.ptr = (u8*)in;
		Surface.stride = static_cast<u32>(w * 4);
		bc7_enc_settings str;
		GetProfile_alpha_slow(&str);
		CompressBlocksBC7(&Surface, (u8*)out, &str);
		break;
	}
	default:
		assert(false);
	}
	free(in);
}

void * RedTextureUtils::StartDecompressor(RedTexturePixelFormat Compressor, size_t w, size_t h, void * in)
{

	switch (Compressor)
	{
	case RedTexturePixelFormat::DXT_1_Alpha:
	case RedTexturePixelFormat::DXT_1:
	{

		u8*new_img = red_alloc<u8>(GetSizeWidth(w, RedTexturePixelFormat::R8G8B8A8)*h);
		const size_t BlockWidth = (w + 3) / 4;
		const size_t BlockHeight = (h + 3) / 4;

		for (size_t by = 0; by < BlockHeight; by++)
		{
			for (size_t bx = 0; bx < BlockWidth; bx++)
			{
				nv::BlockDXT1 BlockDXT;
				nv::ColorBlock ColorBlock;
				memcpy(&BlockDXT.col0.u, (u8*)in + 8 * (bx + (by*BlockWidth)), 2);
				memcpy(&BlockDXT.col1.u, (u8*)in + 8 * (bx + (by*BlockWidth)) + 2, 2);
				memcpy(&BlockDXT.indices, (u8*)in + 8 * (bx + (by*BlockWidth)) + 4, 4);
				BlockDXT.decodeBlock(&ColorBlock);

				for (u32 y = 0; y < std::min(size_t(4), h - 4 * by); y++)
				{
					for (u32 x = 0; x < std::min(size_t(4), w - 4 * bx); x++)
					{
						*GetPixelUint8(x + bx * 4, y + by * 4, w, 4, 0, new_img) = ColorBlock.color(x, y).r;
						*GetPixelUint8(x + bx * 4, y + by * 4, w, 4, 1, new_img) = ColorBlock.color(x, y).g;
						*GetPixelUint8(x + bx * 4, y + by * 4, w, 4, 2, new_img) = ColorBlock.color(x, y).b;
						*GetPixelUint8(x + bx * 4, y + by * 4, w, 4, 3, new_img) = ColorBlock.color(x, y).a;

					}
				}
			}
		}
		return new_img;
	}
	break;
	case RedTexturePixelFormat::DXT_3:
	{
		u8*new_img = red_alloc<u8>(GetSizeWidth(w, RedTexturePixelFormat::R8G8B8A8)*h);
		const size_t BlockWidth = (w + 3) / 4;
		const size_t BlockHeight = (h + 3) / 4;

		for (size_t by = 0; by < BlockHeight; by++)
		{
			for (size_t bx = 0; bx < BlockWidth; bx++)
			{
				nv::BlockDXT3 BlockDXT;
				nv::ColorBlock ColorBlock;
				memcpy(&BlockDXT.color.col0.u, (u8*)in + 16 * (bx + (by*BlockWidth)) + 8, 2);
				memcpy(&BlockDXT.color.col1.u, (u8*)in + 16 * (bx + (by*BlockWidth)) + 2 + 8, 2);
				memcpy(&BlockDXT.color.indices, (u8*)in + 16 * (bx + (by*BlockWidth)) + 4 + 8, 4);
				memcpy(&BlockDXT.alpha.row, (u8*)in + 16 * (bx + (by*BlockWidth)) , 8);
				BlockDXT.decodeBlock(&ColorBlock);

				for (u32 y = 0; y < std::min(size_t(4), h - 4 * by); y++)
				{
					for (u32 x = 0; x < std::min(size_t(4), w - 4 * bx); x++)
					{
						*GetPixelUint8(x + bx * 4, y + by * 4, w, 4, 0, new_img) = ColorBlock.color(x, y).r;
						*GetPixelUint8(x + bx * 4, y + by * 4, w, 4, 1, new_img) = ColorBlock.color(x, y).g;
						*GetPixelUint8(x + bx * 4, y + by * 4, w, 4, 2, new_img) = ColorBlock.color(x, y).b;
						*GetPixelUint8(x + bx * 4, y + by * 4, w, 4, 3, new_img) = ColorBlock.color(x, y).a;

					}
				}
			}
		}
		return new_img;
	}
	break;
	case RedTexturePixelFormat::DXT_5:
	{
		u8*new_img = red_alloc<u8>(GetSizeWidth(w, RedTexturePixelFormat::R8G8B8A8)*h);
		const size_t BlockWidth = (w + 3) / 4;
		const size_t BlockHeight = (h + 3) / 4;

		for (size_t by = 0; by < BlockHeight; by++)
		{
			for (size_t bx = 0; bx < BlockWidth; bx++)
			{
				nv::BlockDXT5 BlockDXT;
				nv::ColorBlock ColorBlock;
				memcpy(&BlockDXT.color.col0.u, (u8*)in + 16 * (bx + (by*BlockWidth)) + 8, 2);
				memcpy(&BlockDXT.color.col1.u, (u8*)in + 16 * (bx + (by*BlockWidth)) + 2 + 8, 2);
				memcpy(&BlockDXT.color.indices, (u8*)in + 16 * (bx + (by*BlockWidth)) + 4+ 8, 4);
				memcpy(&BlockDXT.alpha.u, (u8*)in + 16 * (bx + (by*BlockWidth)) , 8);
				BlockDXT.decodeBlock(&ColorBlock);

				for (u32 y = 0; y < std::min(size_t(4), h - 4 * by); y++)
				{
					for (u32 x = 0; x < std::min(size_t(4), w - 4 * bx); x++)
					{
						*GetPixelUint8(x + bx * 4, y + by * 4, w, 4, 0, new_img) = ColorBlock.color(x, y).r;
						*GetPixelUint8(x + bx * 4, y + by * 4, w, 4, 1, new_img) = ColorBlock.color(x, y).g;
						*GetPixelUint8(x + bx * 4, y + by * 4, w, 4, 2, new_img) = ColorBlock.color(x, y).b;
						*GetPixelUint8(x + bx * 4, y + by * 4, w, 4, 3, new_img) = ColorBlock.color(x, y).a;

					}
				}
			}
		}
		return new_img;
	}
	break;
	case RedTexturePixelFormat::BC4:
	{
		u8*new_img = red_alloc<u8>(GetSizeWidth(w, RedTexturePixelFormat::R8)*h);
		const size_t BlockWidth = (w + 3) / 4;
		const size_t BlockHeight = (h + 3) / 4;

		for (size_t by = 0; by < BlockHeight; by++)
		{
			for (size_t bx = 0; bx < BlockWidth; bx++)
			{
				nv::BlockATI1 ATI1;
				nv::ColorBlock ColorBlock;
				memcpy(&ATI1.alpha, (u8*)in + 8 * (bx + (by*BlockWidth)), 8);
				ATI1.decodeBlock(&ColorBlock);

				for (u32 y = 0; y < std::min(size_t(4), h - 4 * by); y++)
				{
					for (u32 x = 0; x < std::min(size_t(4), w - 4 * bx); x++)
					{
						*GetPixelUint8(x + bx * 4, y + by * 4, w, 1, 0, new_img) = ColorBlock.color(x, y).r;

					}
				}
			}
		}
		return new_img;
	}
	break;
	case RedTexturePixelFormat::BC5:
	{
		u8*new_img = red_alloc<u8>(GetSizeWidth(w, RedTexturePixelFormat::R8G8)*h);
		const size_t BlockWidth = (w + 3) / 4;
		const size_t BlockHeight = (h + 3) / 4;

		for (size_t by = 0; by < BlockHeight; by++)
		{
			for (size_t bx = 0; bx < BlockWidth; bx++)
			{
				nv::BlockATI2 ATI2;
				nv::ColorBlock ColorBlock;
				memcpy(&ATI2.x, (u8*)in + 16 * (bx + (by*BlockWidth)), 8);
				memcpy(&ATI2.y, (u8*)in + 16 * (bx + (by*BlockWidth)) + 8, 8);
				ATI2.decodeBlock(&ColorBlock);

				for (u32 y = 0; y < std::min(size_t(4), h - 4 * by); y++)
				{
					for (u32 x = 0; x < std::min(size_t(4), w - 4 * bx); x++)
					{
						*GetPixelUint8(x + bx * 4, y + by * 4, w, 2, 0, new_img) = ColorBlock.color(x, y).r;
						*GetPixelUint8(x + bx * 4, y + by * 4, w, 2, 1, new_img) = ColorBlock.color(x, y).g;
					}
				}
			}
		}
		return new_img;
	}
	break;
	case RedTexturePixelFormat::BC6:
	{
		u8*new_img = red_alloc<u8>(GetSizeWidth(w, RedTexturePixelFormat::R32G32B32F)*h);
		const size_t BlockWidth = (w + 3) / 4;
		const size_t BlockHeight = (h + 3) / 4;

		for (size_t by = 0; by < BlockHeight; by++)
		{
			for (size_t bx = 0; bx < BlockWidth; bx++)
			{
				nv::BlockBC6 BlockBC6;
				nv::Vector3 ColorBlock[16];
				memcpy(BlockBC6.data, (u8*)in + 16 * (bx + (by*BlockWidth)), 16);
				BlockBC6.decodeBlock(ColorBlock);

				for (size_t y = 0; y < std::min(size_t(4), h - 4 * by); y++)
				{
					for (size_t x = 0; x < std::min(size_t(4), w - 4 * bx); x++)
					{
						memcpy(GetPixelFloat(x + bx * 4, y + by * 4, w, 3, 0, new_img), ColorBlock[x + y * 4].component, 3);
						//swapRG(*reinterpret_cast<u32*>(getPixelComp((u8*)out, w, x + bx * 4, y + by * 4, out_comp, 0));
					}
				}
			}
		}
		return new_img;
	}
	break;
	case RedTexturePixelFormat::BC7:
	{
		u8*new_img = red_alloc<u8>(GetSizeWidth(w, RedTexturePixelFormat::R8G8B8A8)*h);
		const size_t BlockWidth = (w + 3) / 4;
		const size_t BlockHeight = (h + 3) / 4;

		for (size_t by = 0; by < BlockHeight; by++)
		{
			for (size_t bx = 0; bx < BlockWidth; bx++)
			{
				nv::BlockBC7 BlockBC7;
				nv::ColorBlock ColorBlock;
				memcpy(BlockBC7.data, (u8*)in + 16 * (bx + (by*BlockWidth)), 16);
				BlockBC7.decodeBlock(&ColorBlock);

				for (u32 y = 0; y < std::min(size_t(4), h - 4 * by); y++)
				{
					for (u32 x = 0; x < std::min(size_t(4), w - 4 * bx); x++)
					{
						SwapRB(*reinterpret_cast<u32*>((u8*)ColorBlock.color(x, y).component));
						memcpy(GetPixelUint8(x + bx * 4, y + by * 4, w, 4, 0, new_img), ColorBlock.color(x, y).component, 4);
					}
				}
			}
		}
		return new_img;
	}
	break;
	default:
		assert(false);
	}
	return 0;
}

void RedTextureUtils::EndDecompressor(void * in)
{
	free(in);
}

void RedTextureUtils::GetBlock(RedColor(&color)[16], u8 * data, size_t w, size_t h, size_t x_, size_t y, RedTexturePixelFormat px)
{

	u8*Block = data + ((x_ / 4) + ((w + 3) / 4)*(y / 4))*(px == RedTexturePixelFormat::BC1 || px == RedTexturePixelFormat::BC1a || px == RedTexturePixelFormat::BC4 ? 8 : 16);
	nv::ColorBlock ColorBlock;
	switch (px)
	{

	case RedTexturePixelFormat::BC1:
	{
		nv::BlockDXT1 BlockDXT;

		memcpy(&BlockDXT.col0, Block, 8);
		BlockDXT.decodeBlock(&ColorBlock);

		for (uint x = 0; x < 16; x++)
		{
			color[x].SetAsUint8(ColorBlock.color(x).r, ColorBlock.color(x).g, ColorBlock.color(x).b, ColorBlock.color(x).a);
		}
	}
	break;
	case RedTexturePixelFormat::BC1a:
	{
		nv::BlockDXT1 BlockDXT;

		memcpy(&BlockDXT.col0, Block, 8);
		BlockDXT.decodeBlock(&ColorBlock);
		for (uint x = 0; x < 16; x++)
		{
			color[x].SetAsUint8(ColorBlock.color(x).r, ColorBlock.color(x).g, ColorBlock.color(x).b, ColorBlock.color(x).a);
		}
	}
	break;
	case RedTexturePixelFormat::BC2:
	{
		nv::BlockDXT3 BlockDXT;

		memcpy(&BlockDXT.alpha, Block, 8);
		memcpy(&BlockDXT.color, Block + 8, 8);
		BlockDXT.decodeBlock(&ColorBlock);
		for (uint x = 0; x < 16; x++)
		{
			color[x].SetAsUint8(ColorBlock.color(x).r, ColorBlock.color(x).g, ColorBlock.color(x).b, ColorBlock.color(x).a);
		}
	}
	break;
	case RedTexturePixelFormat::BC3:
	{
		nv::BlockDXT5 BlockDXT;

		memcpy(&BlockDXT.alpha, Block, 8);
		memcpy(&BlockDXT.color, Block+8, 8);
		BlockDXT.decodeBlock(&ColorBlock);
		for (uint x = 0; x < 16; x++)
		{
			color[x].SetAsUint8(ColorBlock.color(x).r, ColorBlock.color(x).g, ColorBlock.color(x).b, ColorBlock.color(x).a);
		}
	}
	break;
	case RedTexturePixelFormat::BC4:
	{
		nv::BlockATI1 BlockATI;

		memcpy(&BlockATI.alpha, Block, 8);
		BlockATI.decodeBlock(&ColorBlock);
		for (uint x = 0; x < 16; x++)
		{
			color[x].SetAsUint8(ColorBlock.color(x).r, ColorBlock.color(x).g, ColorBlock.color(x).b, ColorBlock.color(x).a);
		}
	}
	break;
	case RedTexturePixelFormat::BC5:
	{
		nv::BlockATI2 BlockATI;

		memcpy(&BlockATI.x, Block, 8);
		memcpy(&BlockATI.y, Block+8, 8);
		BlockATI.decodeBlock(&ColorBlock);
		for (uint x = 0; x < 16; x++)
		{
			color[x].SetAsUint8(ColorBlock.color(x).r, ColorBlock.color(x).g, ColorBlock.color(x).b, ColorBlock.color(x).a);
		}
	}
	break;
	case RedTexturePixelFormat::BC6:
	{
		nv::BlockBC6 BlockBC6;
		nv::Vector3 ColorFloat[16];

		memcpy(&BlockBC6.data, Block, 16);
		BlockBC6.decodeBlock(ColorFloat);
		for (uint x = 0; x < 16; x++)
		{
			color[x].SetAsFloat(ColorFloat[x].x, ColorFloat[x].y, ColorFloat[x].z, 1.f);
		}
		return;
	}
	break;
	case RedTexturePixelFormat::BC7:
	{
		nv::BlockBC7 BlockBC7;

		memcpy(&BlockBC7.data, Block, 16);
		BlockBC7.decodeBlock(&ColorBlock);

		for (uint x = 0; x < 16; x++)
		{
			color[x].SetAsUint8(ColorBlock.color(x).r, ColorBlock.color(x).g, ColorBlock.color(x).b, ColorBlock.color(x).a);
		}
	}
	break;
	default:
		break;
	}
	
}

void RedTextureUtils::SetBlock(RedColor(&color)[16], u8 * data, size_t w, size_t h, size_t x_, size_t y, RedTexturePixelFormat px)
{
	u8*Block = data + ((x_ / 4) + ((w + 3) / 4)*(y / 4))*(px == RedTexturePixelFormat::BC1 || px == RedTexturePixelFormat::BC1a || px == RedTexturePixelFormat::BC4 ? 8 : 16);
	u8 ImagePixel16Uint8[16 * 4];
//	float imagePixel16Float[16 * 4];
	u16 ImagePixel16red_uint16[16 * 4];
	switch (px)
	{
	case RedTexturePixelFormat::BC1:
	{
		nv::CompressorDXT1 Compressor;
		nvtt::CompressionOptions CompOpt;
		CompOpt.setQuality(nvtt::Quality_Highest);
		CompOpt.setQuantization(false, false, false);

		CompOpt.setFormat(nvtt::Format_DXT1);

		float wa[16];
		nv::Vector4 blockColor[16];
		for (size_t x = 0; x < 16; x++)
		{
			wa[x] = 1.f;
			blockColor[x].x = color[x].R32F;
			blockColor[x].y = color[x].B32F;
			blockColor[x].z = color[x].G32F;
			blockColor[x].w = color[x].A32F;
		}
		Compressor.compressBlock(blockColor, wa, CompOpt.m, Block);
	}

	break;
	case RedTexturePixelFormat::BC1a:
	{
		nv::CompressorDXT1a Compressor;
		nvtt::CompressionOptions CompOpt;
		CompOpt.setQuality(nvtt::Quality_Highest);
		CompOpt.setQuantization(false, false, false);

		CompOpt.setFormat(nvtt::Format_DXT1a);

		nv::ColorBlock blockColor;
		for (uint x = 0; x < 16; x++)
		{
			blockColor.color(x).setRGBA(color[x].R8U, color[x].G8U, color[x].B8U, color[x].A8U );
		}
		Compressor.compressBlock(blockColor, nvtt::AlphaMode_Transparency, CompOpt.m, Block);
	}
	break;
	case RedTexturePixelFormat::BC2:
	{
		nv::CompressorDXT3 Compressor;
		nvtt::CompressionOptions CompOpt;
		CompOpt.setQuality(nvtt::Quality_Highest);
		CompOpt.setQuantization(false, false, false);

		CompOpt.setFormat(nvtt::Format_DXT1a);

		nv::ColorBlock blockColor;
		for (uint x = 0; x < 16; x++)
		{
			blockColor.color(x).setRGBA(color[x].R8U, color[x].G8U, color[x].B8U, color[x].A8U);
		}
		Compressor.compressBlock(blockColor, nvtt::AlphaMode_Transparency, CompOpt.m, Block);
	}
	break;
	case RedTexturePixelFormat::BC3:
	{
		nv::CompressorDXT5 Compressor;
		nvtt::CompressionOptions CompOpt;
		CompOpt.setQuality(nvtt::Quality_Highest);
		CompOpt.setQuantization(false, false, false);

		CompOpt.setFormat(nvtt::Format_DXT1a);

		nv::ColorBlock blockColor;
		for (uint x = 0; x < 16; x++)
		{
			blockColor.color(x).setRGBA(color[x].R8U, color[x].G8U, color[x].B8U, color[x].A8U);
		}
		Compressor.compressBlock(blockColor, nvtt::AlphaMode_Transparency, CompOpt.m, Block);
	}
	break;
	case RedTexturePixelFormat::BC4:
	{
		nv::ColorBlock blockColor;
		nv::AlphaBlock4x4 Alpha1;
		nv::AlphaBlockDXT5 AlphaBlock1;
		for (uint x = 0; x < 16; x++)
		{
			blockColor.color(x).setRGBA(color[x].R8U, color[x].G8U, color[x].B8U, color[x].A8U);
		}
		Alpha1.init(blockColor, 2);
		nv::QuickCompress::compressDXT5A(Alpha1, &AlphaBlock1, 8);
		memcpy(Block, &AlphaBlock1.u, 8);

	}
	break;
	case RedTexturePixelFormat::BC5:
	{
		nv::ColorBlock blockColor;
		nv::AlphaBlock4x4 Alpha1, Alpha2;
		nv::AlphaBlockDXT5 AlphaBlock1, AlphaBlock2;
		for (uint x = 0; x < 16; x++)
		{
			blockColor.color(x).setRGBA(color[x].R8U, color[x].G8U, color[x].B8U, color[x].A8U);
		}
		Alpha1.init(blockColor, 2);
		Alpha2.init(blockColor, 1);
		nv::QuickCompress::compressDXT5A(Alpha1, &AlphaBlock1, 8);
		nv::QuickCompress::compressDXT5A(Alpha2, &AlphaBlock2, 8);
		memcpy(Block, &AlphaBlock1.u, 8);
		memcpy(Block + 8, &AlphaBlock2.u, 8);
	}
	break;
	case RedTexturePixelFormat::BC6:
		for (uint x = 0; x < 16; x++)
		{
			ImagePixel16red_uint16[x * 4] = floatToHalf(color[x].R32F);
			ImagePixel16red_uint16[x * 4 + 1] = floatToHalf(color[x].G32F);
			ImagePixel16red_uint16[x * 4 + 2] = floatToHalf(color[x].B32F);
			ImagePixel16red_uint16[x * 4 + 3] = floatToHalf(color[x].A32F);

		}
		{

			rgba_surface Surface;
			Surface.height = static_cast<int32>(h);
			Surface.width = static_cast<int32>(w);
			Surface.ptr = (u8*)ImagePixel16red_uint16;
			/*for (size_t i = 0; i < w*h; i++)
			*(Image + i * 4) = 0;*/
			Surface.stride = static_cast<int32>(w) * 2 * 4;
			bc6h_enc_settings str;
			GetProfile_bc6h_veryslow(&str);
			CompressBlocksBC6H(&Surface, (u8*)Block, &str);
		}
		break;
	case RedTexturePixelFormat::BC7:
		for (size_t x = 0; x < 16; x++)
		{
			ImagePixel16Uint8[x * 4] = color[x].R8U;
			ImagePixel16Uint8[x * 4 + 1] = color[x].G8U;
			ImagePixel16Uint8[x * 4 + 2] = color[x].B8U;
			ImagePixel16Uint8[x * 4 + 3] = color[x].A8U;

		}
		{
			rgba_surface Surface;
			Surface.height = static_cast<int32>(h);
			Surface.width = static_cast<int32>(w);
			Surface.ptr = ImagePixel16Uint8;
			Surface.stride = static_cast<int32>(w) * 4;
			bc7_enc_settings str;
			if (GetCountComp(px) == 4)
				GetProfile_alpha_slow(&str);
			else
				GetProfile_slow(&str);
			CompressBlocksBC7(&Surface, (u8*)Block, &str);
		}
		break;
	default:
		break;
	}
}

bool RedTextureUtils::CompressorAsFloat(RedTexturePixelFormat px)
{
	switch (px)
	{
	case RedTexturePixelFormat::BC6:
		return true;
		break;
	}
	return false;
}

u8 * RedTextureUtils::TempUncompressor(u8 * data, size_t w, size_t h, RedTexturePixelFormat px, RedTexturePixelFormat & out_px)
{
	
	switch (px)
	{
	case RedTexturePixelFormat::BC1:
		out_px = RedTexturePixelFormat::R32G32B32F;
		break;
	case RedTexturePixelFormat::BC1a:
	case RedTexturePixelFormat::BC2:
	case RedTexturePixelFormat::BC3:
		out_px = RedTexturePixelFormat::R8G8B8A8;
		break;
	case RedTexturePixelFormat::BC4:
		out_px = RedTexturePixelFormat::R8;
		break;
	case RedTexturePixelFormat::BC5:
		out_px = RedTexturePixelFormat::R8G8;
		break;
	case RedTexturePixelFormat::BC6:
		out_px = RedTexturePixelFormat::R32G32B32F;
		break;
	case RedTexturePixelFormat::BC7:
		out_px = RedTexturePixelFormat::R8G8B8A8;
		break;
	default:
		break;
	}

	u8*data_out = red_alloc<u8>(GetSizeWidth(w, out_px)*h);
	if(data!=0)
	Convert(out_px, px, data_out,data , w, h);
	return data_out;
}

void RedTextureUtils::TempCompress(u8 * in, u8 * out, size_t w, size_t h, RedTexturePixelFormat px)
{
	RedTexturePixelFormat InPixelFormat;
	switch (px)
	{
	case RedTexturePixelFormat::BC1:
		InPixelFormat = RedTexturePixelFormat::R32G32B32F;
		break;
	case RedTexturePixelFormat::BC1a:
	case RedTexturePixelFormat::BC2:
	case RedTexturePixelFormat::BC3:
		InPixelFormat = RedTexturePixelFormat::R8G8B8A8;
		break;
	case RedTexturePixelFormat::BC4:
		InPixelFormat = RedTexturePixelFormat::R8;
		break;
	case RedTexturePixelFormat::BC5:
		InPixelFormat = RedTexturePixelFormat::R8G8;
		break;
	case RedTexturePixelFormat::BC6:
		InPixelFormat = RedTexturePixelFormat::R32G32B32F;
		break;
	case RedTexturePixelFormat::BC7:
		InPixelFormat = RedTexturePixelFormat::R8G8B8A8;
		break;
	default:
		InPixelFormat = RedTexturePixelFormat::R8;
		break;
	}
	if(out!=0)
		Convert(px, InPixelFormat,out, in, w, h);
	free(in);
}


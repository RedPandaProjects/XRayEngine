#include "RedImage.hpp"
using namespace RedImageTool;

RedImage::RedImage() :m_PixelFotmat(RedTexturePixelFormat::R8), m_Width(0), m_Height(0), m_Mips(0), m_Depth(0), m_ImageBuffer(0)
{

}

RedImage::RedImage(size_t w, size_t h, size_t mip, size_t depth, RedTexturePixelFormat px, bool cube) : m_PixelFotmat(RedTexturePixelFormat::R8), m_Width(0), m_Height(0), m_Mips(0), m_Depth(0), m_ImageBuffer(0)
{
	Create(w, h, mip, depth, px, cube);
}

void RedImage::Fill(const RedColor& color)
{
	if (Empty())return;
	size_t Size = RedTextureUtils::GetSizeInMemory(m_Width, m_Height, m_Mips, m_PixelFotmat);
	for (size_t i = 0; i < m_Depth; i++)
	{
		RedTextureUtils::Fill(m_ImageBuffer + (Size * i), m_Width, m_Height, m_Mips, color, m_PixelFotmat);
	}
}

void RedImage::Create(size_t w, size_t h, size_t mip, size_t depth, RedTexturePixelFormat px, bool cube)
{
	Clear();
	m_bCube = cube;
	m_Depth = depth;
	if (cube)m_Depth *= 6;
	if (RedTextureUtils::isCompressor(px))
	{
		assert(m_Width % 4 == 0 && m_Height % 4 == 0);
	}
	m_Width = w;
	m_Height = h;
	m_PixelFotmat = px;
	assert(m_Depth);
	if (mip)
		m_Mips = mip;
	else
		m_Mips = 1;
	m_ImageBuffer = red_alloc<u8>(GetSizeInMemory());
}

void RedImage::Append(size_t x, size_t y, const RedImage& Image, size_t x_src, size_t y_src, size_t w_src, size_t h_src, size_t dst_depth, size_t src_depth)
{
	if (Empty() || Image.Empty())
		return;
	assert(m_Width >= x + w_src && m_Height >= y + h_src);
	assert(Image.m_Width >= x_src + w_src && Image.m_Height >= y_src + h_src);
	assert(m_Depth > dst_depth);
	assert(m_Depth > src_depth);
	size_t DestSize = RedTextureUtils::GetSizeInMemory(m_Width, m_Height, m_Mips, m_PixelFotmat) * dst_depth;
	size_t SourceSize = RedTextureUtils::GetSizeInMemory(Image.m_Width, Image.m_Height, Image.m_Mips, Image.m_PixelFotmat) * src_depth;
	u8* DestData = m_ImageBuffer + DestSize;
	u8* src_data = Image.m_ImageBuffer + SourceSize;
	RedTextureUtils::Append(DestData, m_Width, m_Height, x, y, src_data, Image.m_Width, Image.m_Height, x_src, y_src, w_src, h_src, m_PixelFotmat, Image.m_PixelFotmat);
}

void RedImage::Append(size_t x, size_t y, const RedImage& Image, size_t dst_depth, size_t src_depth)
{
	if (Empty() || Image.Empty())
		return;
	Append(x, y, Image, 0, 0, Image.m_Width, Image.m_Height, dst_depth, src_depth);
}

void RedImage::Scale(size_t w, size_t h, RedResizeFilter filter)
{
	if (Empty())
		return;
	RedImage Image;
	size_t Mips = m_Mips;
	ClearMipLevels();
	Image.Create(w, h, 1, m_Depth, m_PixelFotmat);
	size_t SourceSize = RedTextureUtils::GetSizeInMemory(m_Width, m_Height, m_Mips, m_PixelFotmat);
	size_t DestSize = RedTextureUtils::GetSizeInMemory(Image.m_Width, Image.m_Height, Image.m_Mips, Image.m_PixelFotmat);
	for (size_t i = 0; i < m_Depth; i++)
	{

		u8* DestData = Image.m_ImageBuffer + DestSize * i;
		u8* src_data = m_ImageBuffer + SourceSize * i;
		RedTextureUtils::Scale(DestData, Image.m_Width, Image.m_Height, src_data, m_Width, m_Height, m_PixelFotmat, filter);
	}

	Swap(Image);
	if (Mips > 1) GenerateMipmap(filter);
}

void RedImage::ScaleCanvas(size_t w, size_t h)
{
	if (Empty())
		return;
	size_t Mips = m_Mips;
	RedImage Image;
	ClearMipLevels();
	Image.Create(w, h, m_Mips, m_Depth, m_PixelFotmat);
	Image.Fill(RedColor(0));
	for (size_t i = 0; i < m_Depth; i++)
	{
		Image.Append(0, 0, *this, 0, 0, std::min(m_Width, w), std::min(m_Height, h), i, i);
	}
	Swap(Image);
	if (Mips > 1) GenerateMipmap();

}


void RedImage::GenerateMipmap(size_t depth, RedResizeFilter filter)
{
	if (Empty())
		return;
	if (m_Mips == 1)
	{
		GenerateMipmap(filter);
	}
	else
	{

		for (size_t i = 0; i < m_Mips - 1; i++)
		{
			u8* cur_mip = RedTextureUtils::GetImage(m_ImageBuffer, m_Width, m_Height, m_Mips, depth, i, m_PixelFotmat);
			u8* new_mip = RedTextureUtils::GetImage(m_ImageBuffer, m_Width, m_Height, m_Mips, depth, i + 1, m_PixelFotmat);
			RedTextureUtils::GenerateMip(new_mip, cur_mip, RedTextureUtils::GetMip(m_Width, i), RedTextureUtils::GetMip(m_Height, i), m_PixelFotmat, filter);
		}

	}
}

void RedImage::GenerateMipmap(RedResizeFilter filter)
{
	if (Empty())
		return;
	m_Mips = RedTextureUtils::GetCountMips(m_Width, m_Height);
	m_ImageBuffer = red_realloc(m_ImageBuffer, RedTextureUtils::GetSizeInMemory(m_Width, m_Height, m_Mips, m_PixelFotmat) * m_Depth);
	size_t size = RedTextureUtils::GetSizeInMemory(m_Width, m_Height, 1, m_PixelFotmat);
	for (size_t i = m_Depth; i > 1; i--)
	{
		u8* images = RedTextureUtils::GetImage(m_ImageBuffer, m_Width, m_Height, 1, i - 1, 0, m_PixelFotmat);
		u8* images_new = RedTextureUtils::GetImage(m_ImageBuffer, m_Width, m_Height, m_Mips, i - 1, 0, m_PixelFotmat);
		memmove(images_new,images, size);
	}
	for (size_t i = 0; m_Depth > i; i++)
	{
		GenerateMipmap(i, filter);
	}
}


void RedImage::ClearMipLevels()
{
	if (Empty() || m_Mips == 1)return;
	size_t size = RedTextureUtils::GetSizeInMemory(m_Width, m_Height, 1, m_PixelFotmat);
	for (size_t i = m_Depth; i > 1; i--)
	{
		u8* images_new = RedTextureUtils::GetImage(m_ImageBuffer, m_Width, m_Height, 1, i - 1, 0, m_PixelFotmat);
		u8* images = RedTextureUtils::GetImage(m_ImageBuffer, m_Width, m_Height, m_Mips, i - 1, 0, m_PixelFotmat);
		memmove(images_new, images, size);
	}
	m_Mips = 1;
	m_ImageBuffer = red_realloc(m_ImageBuffer, RedTextureUtils::GetSizeInMemory(m_Width, m_Height, m_Mips, m_PixelFotmat) * m_Depth);
}

void RedImage::NormalizedSizeNotScale()
{
	size_t size = std::max(red_recommended_size(m_Width), red_recommended_size(m_Height));
	ScaleCanvas(size, size);
}

void RedImage::NormalizedSize(RedResizeFilter filter)
{
	size_t size = std::max(red_recommended_size(m_Width), red_recommended_size(m_Height));
	Scale(size, size);
}

RedColor RedImage::GetPixel(size_t x, size_t y, size_t d) const
{
	if (Empty())
		return RedColor();

	RedColor color;
	RedTextureUtils::GetPixel(color, m_ImageBuffer, x, y, d, m_Width, m_Height, m_Mips, m_PixelFotmat);
	return color;

}

void RedImage::SetPixel(const RedColor& color, size_t x, size_t y, size_t d)
{
	if (Empty())
		return;
	RedTextureUtils::SetPixel(color, m_ImageBuffer, x, y, d, m_Width, m_Height, m_Mips, m_PixelFotmat);
}

void RedImage::SwapRB()
{
	if (Empty())return;
	assert(m_Mips == 1);
	if (m_Mips != 1)return;
	if (m_PixelFotmat != RedTexturePixelFormat::R8G8B8A8)
	{
		for (size_t i = 0; i < m_Depth; i++)
		{
			for (size_t x = 0; x < m_Width * m_Height; x++)
			{
				for (size_t y = 0; y < m_Width * m_Height; y++)
				{
					RedColor Color = GetPixel(x, y, i);
					std::swap(Color.R8U, Color.B8U);
					std::swap(Color.R32F, Color.B32F);
					SetPixel(Color, x, y, i);
				}
			}
		}
	}
	else
	{
		for (size_t i = 0; i < m_Depth; i++)
		{
			for (size_t a = 0; a < m_Width * m_Height; a++)
			{
				u8* Pixel = m_ImageBuffer + i * (m_Width * m_Height) + a * 4;
				std::swap(Pixel[0], Pixel[2]);

			}
		}
	}
}

void RedImage::Clear()
{
	if (m_ImageBuffer)
		red_free(m_ImageBuffer);
	m_ImageBuffer = 0;
	m_Height = 0;
	m_Width = 0;
	m_Mips = 0;
	m_Depth = 0;
	m_PixelFotmat = RedTexturePixelFormat::R8;
}

RedImage::~RedImage()
{
	Clear();
}

bool RedImage::Empty() const
{
	return m_Depth == 0 || m_Mips == 0 || m_Width == 0 || m_Height == 0;
}

RedImage::RedImage(RedImage&& Image) :m_PixelFotmat(RedTexturePixelFormat::R8), m_Width(0), m_Height(0), m_Mips(0), m_Depth(0), m_ImageBuffer(0)
{
	Swap(Image);
}

RedImage::RedImage(const RedImage& Image) : m_PixelFotmat(RedTexturePixelFormat::R8), m_Width(0), m_Height(0), m_Mips(0), m_Depth(0), m_ImageBuffer(0)
{
	Copy(Image);
}

void RedImage::Copy(const RedImage& Image)
{
	Clear();
	if (Image.Empty())
	{
		return;
	}
	m_bCube = Image.m_bCube;
	m_Height = Image.m_Height;
	m_Width = Image.m_Width;
	m_Mips = Image.m_Mips;
	m_Depth = Image.m_Depth;
	m_PixelFotmat = Image.m_PixelFotmat;
	m_ImageBuffer = red_alloc<u8>(GetSizeInMemory());
	memcpy(m_ImageBuffer, Image.m_ImageBuffer, GetSizeInMemory());
}

void RedImage::Swap(RedImage& Image)
{
	std::swap(m_Height, Image.m_Height);
	std::swap(m_Width, Image.m_Width);
	std::swap(m_Mips, Image.m_Mips);
	std::swap(m_Depth, Image.m_Depth);
	std::swap(m_PixelFotmat, Image.m_PixelFotmat);
	std::swap(m_ImageBuffer, Image.m_ImageBuffer);
}

RedImage& RedImage::operator=(const RedImage& Image)
{
	Copy(Image);
	return*this;
}

RedImage& RedImage::operator=(RedImage&& Image)
{
	Swap(Image);
	return*this;
}

void RedImage::Convert(RedTexturePixelFormat format)
{
	if (Empty())return;
	RedImage Image(m_Width, m_Height, m_Mips, m_Depth, format);
	u8* src = (u8*)**this;
	u8* dst = (u8*)*Image;
	for (size_t i = 0; i < m_Depth; i++)
	{

		for (size_t a = 0; a < m_Mips; a++)
		{
			size_t w = RedTextureUtils::GetMip(m_Width, a);
			size_t h = RedTextureUtils::GetMip(m_Height, a);
			RedTextureUtils::Convert(format, m_PixelFotmat, dst, src, w, h);
			src += RedTextureUtils::GetSizeDepth(w, h, m_PixelFotmat);
			dst += RedTextureUtils::GetSizeDepth(w, h, format);
		}
	}
	Swap(Image);
}

RedTexturePixelFormat RedImage::GetFormat() const
{
	return m_PixelFotmat;
}

size_t RedImage::GetDepth() const
{
	return m_Depth;
}

size_t RedImage::GetMips()const
{
	return m_Mips;
}

void* RedImage::operator*()
{
	return m_ImageBuffer;
}

const void* RedImage::operator*() const
{
	return m_ImageBuffer;
}

size_t RedImage::GetSizeInMemory() const
{
	return RedTextureUtils::GetSizeInMemory(m_Width, m_Height, m_Mips, m_PixelFotmat) * m_Depth;
}

size_t RedImage::GetWidth() const
{
	return m_Width;
}

size_t RedImage::GetHeight() const
{
	return m_Height;
}


void RedImage::Resize(size_t w, size_t h, size_t depth, RedTexturePixelFormat px)
{
	if (m_Width != w || m_Height != h || m_PixelFotmat != px)
	{
		Create(w, h, m_Mips, depth + 1, px);
	}
	else
	{
		if (m_Depth < depth + 1)
			m_ImageBuffer = red_realloc(m_ImageBuffer, RedTextureUtils::GetSizeInMemory(m_Width, m_Height, m_Mips, m_PixelFotmat) * (depth + 1));
		m_Depth = depth + 1;
	}
}
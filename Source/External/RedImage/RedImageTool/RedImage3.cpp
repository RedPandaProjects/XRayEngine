#include "RedImage.hpp"
#include "dds.h"
#include "DXGIFormat.h"
using namespace RedImageTool;
inline void MaskShiftAndSize(size_t mask, u32* shift, u32* size)
{
	if (!mask)
	{
		*shift = 0;
		*size = 0;
		return;
	}

	*shift = 0;
	while ((mask & 1) == 0) {
		++(*shift);
		mask >>= 1;
	}

	*size = 0;
	while ((mask & 1) == 1) {
		++(*size);
		mask >>= 1;
	}
}

static u32 ConvertColor(u32 c, u32 inbits, u32 outbits)
{
	if (inbits == 0)
	{
		return 0;
	}
	else if (inbits >= outbits)
	{
		return c >> (inbits - outbits);
	}
	else
	{
		return (c << (outbits - inbits)) | ConvertColor(c, inbits, outbits - inbits);
	}
}
bool RedImage::LoadDDSFromMemory(void* pointer, size_t size)
{
	Clear();
	u8* data = reinterpret_cast<u8*>(pointer);
	if (*reinterpret_cast<u32*>(data) == MAKEFOURCC('D', 'D', 'S', ' '))
	{
		data += sizeof(u32);

		size_t size = *reinterpret_cast<u32*>(data);
		DDS_HEADER Header = {};
		if (size > sizeof(DDS_HEADER) || size < 4)
			return false;
		memcpy(&Header, data, size);
		data += size;
		if (!(Header.dwHeaderFlags&DDSD_WIDTH) || !(Header.dwHeaderFlags&DDSD_HEIGHT) || !(Header.dwHeaderFlags&DDSD_PIXELFORMAT))
			return false;
		m_Depth = Header.dwHeaderFlags&DDSD_DEPTH ? Header.dwDepth : 1;
		m_Mips = Header.dwHeaderFlags&DDSD_MIPMAPCOUNT ? Header.dwMipMapCount : 1;
		m_bCube = !!(Header.dwCubemapFlags & DDS_CUBEMAP_ALLFACES);
		m_Height = Header.dwHeight;
		m_Width = Header.dwWidth;
		if (m_Mips==0)
		{
			Clear();
			return false;
		}
		if (Header.ddspf.dwFlags != DDS_FOURCC)
		{
			bool Alpha = Header.ddspf.dwFlags&(DDS_RGBA&(~DDS_RGB));
			if (Header.ddspf.dwRGBBitCount % 8 || Header.ddspf.dwRGBBitCount > 64)
			{
				Clear();
				return false;
			}
			size_t ByteSizePixel = Header.ddspf.dwRGBBitCount / 8;
			u32 SizeBit[4], ShiftBit[4];
			for (size_t x = 0; x < 4; x++)
				MaskShiftAndSize(Header.ddspf.dwBitsMask[x], ShiftBit + x, SizeBit + x);
			if (Header.ddspf.dwRBitMask&&Header.ddspf.dwGBitMask&&Header.ddspf.dwBBitMask)
				m_PixelFotmat = Alpha ? RedTexturePixelFormat::R8G8B8A8 : RedTexturePixelFormat::R8G8B8;
			else	if (Header.ddspf.dwRBitMask&&Header.ddspf.dwGBitMask)
				m_PixelFotmat = RedTexturePixelFormat::R8G8;
			else
				m_PixelFotmat = RedTexturePixelFormat::R8;
			u8 coutComp =RedTextureUtils::GetCountComp(m_PixelFotmat);
			if (coutComp == 1 && Header.ddspf.dwBitsMask[3])
			{
				std::swap(SizeBit[0], SizeBit[3]);

				std::swap(ShiftBit[0], ShiftBit[3]);
				std::swap(Header.ddspf.dwBitsMask[0], Header.ddspf.dwBitsMask[3]);
			}

			u32 pixel = 0;
			Create(m_Width, m_Height, m_Mips, m_Depth, m_PixelFotmat,m_bCube);
			for (size_t d = 0; d < m_Depth; d++)
			{
				for (size_t m = 0; m < m_Mips; m++)
				{
					size_t h = RedTextureUtils::GetMip(m_Height, m);
					size_t w = RedTextureUtils::GetMip(m_Width, m);
					u8*data = RedTextureUtils::GetImage(m_ImageBuffer, m_Width, m_Height, m_Mips, d, m, m_PixelFotmat);
					for (size_t x = 0; x < w*h; x++)
					{
						memcpy(&pixel, data, ByteSizePixel);
						data += ByteSizePixel;
						for (size_t a = 0; a < coutComp; a++)
						{
							*RedTextureUtils::GetPixelUint8(x, 0, 0, coutComp, a, data)= static_cast<u8>(ConvertColor((pixel & Header.ddspf.dwBitsMask[a]) >> ShiftBit[a], SizeBit[a], 8));;
						}
					}
				}
			}
			return true;
		}
		else
		{

			
			if (Header.ddspf.dwFourCC == MAKEFOURCC('D', 'X', '1', '0'))
			{
				if (size + sizeof(DDSHeader10) == sizeof(DDS_HEADER))
				{
					data = reinterpret_cast<u8*>(pointer)+sizeof(u32);
					memcpy(&Header, data,size += sizeof(DDSHeader10));
				}
				switch (Header.Header10.dxgiFormat)
				{
				case DXGI_FORMAT_R32G32B32A32_FLOAT:
					m_PixelFotmat = RedTexturePixelFormat::R32G32B32A32F;
					break;
				case DXGI_FORMAT_R32G32B32_FLOAT:
					m_PixelFotmat = RedTexturePixelFormat::R32G32B32F;
					break;
				case DXGI_FORMAT_R32G32_FLOAT:
					m_PixelFotmat = RedTexturePixelFormat::R32G32F;
					break;
				case DXGI_FORMAT_R32_FLOAT:
					m_PixelFotmat = RedTexturePixelFormat::R32F;
					break;
				case DXGI_FORMAT_R8G8B8A8_UNORM:
					m_PixelFotmat = RedTexturePixelFormat::R8G8B8A8;
					break;
				case DXGI_FORMAT_R8G8_UNORM:
					m_PixelFotmat = RedTexturePixelFormat::R8G8;
					break;
				case DXGI_FORMAT_R8_UNORM:
					m_PixelFotmat = RedTexturePixelFormat::R8;
					break;
				case DXGI_FORMAT_BC1_UNORM:
					m_PixelFotmat = RedTexturePixelFormat::BC1;
					break;
				case DXGI_FORMAT_BC2_UNORM:
					m_PixelFotmat = RedTexturePixelFormat::BC2;
					break;
				case DXGI_FORMAT_BC3_UNORM:
					m_PixelFotmat = RedTexturePixelFormat::BC3;
					break;
				case DXGI_FORMAT_BC4_UNORM:
					m_PixelFotmat = RedTexturePixelFormat::BC4;
					break;
				case DXGI_FORMAT_BC5_UNORM:
					m_PixelFotmat = RedTexturePixelFormat::BC5;
					break;
				case DXGI_FORMAT_BC6H_SF16:
					m_PixelFotmat = RedTexturePixelFormat::BC6;
					break;
				case DXGI_FORMAT_BC7_UNORM:
					m_PixelFotmat = RedTexturePixelFormat::BC7;
					break;
				default:
					Clear();
					return false;
				}
			}
			else
			{
				switch (Header.ddspf.dwFourCC)
				{
				case  MAKEFOURCC('D', 'X', 'T', '1'):
					m_PixelFotmat = RedTexturePixelFormat::BC1;
					break;
				case  MAKEFOURCC('D', 'X', 'T', '2'):
				case  MAKEFOURCC('D', 'X', 'T', '3'):
					m_PixelFotmat = RedTexturePixelFormat::BC2;
					break;
				case  MAKEFOURCC('D', 'X', 'T', '4'):
				case  MAKEFOURCC('D', 'X', 'T', '5'):
					m_PixelFotmat = RedTexturePixelFormat::BC3;
					break;
				case  MAKEFOURCC('A', 'T', 'I', '1'):
					m_PixelFotmat = RedTexturePixelFormat::BC4;
					break;
				case  MAKEFOURCC('A', 'T', 'I', '2'):
					m_PixelFotmat = RedTexturePixelFormat::BC5;
					break;
				default:
					Clear();
					return false;
				}
			}
			Create(m_Width, m_Height, m_Mips , m_Depth, m_PixelFotmat, m_bCube);
			memcpy(m_ImageBuffer, data, GetSizeInMemory());
			return true;
		}
		
	}
	return false;
}


bool RedImage::SaveToDds(const char* name)
{
	FILE * file = fopen(name, "wb");
	if (!file)
		return false;
	fseek(file, 0, SEEK_SET);

	DDS_HEADER Header = {  };

	Header.dwSize = sizeof(Header) - sizeof(Header.Header10);
	Header.dwHeaderFlags = DDSD_CAPS|DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;// DDSD_MIPMAPCOUNT | DDSD_DEPTH;
	Header.dwWidth = static_cast<DWORD>(m_Width);
	Header.dwHeight = static_cast<DWORD>(m_Height);
	Header.dwSurfaceFlags = DDS_SURFACE_FLAGS_TEXTURE;
	if (m_Depth>1)
	{
		Header.dwDepth = static_cast<DWORD>(m_Depth);
		Header.dwHeaderFlags |= DDSD_DEPTH;
		
	}
	if (m_Mips>1)
	{
		Header.dwMipMapCount = static_cast<DWORD>(m_Mips);
		Header.dwHeaderFlags |= DDSD_MIPMAPCOUNT;
	}
	switch (m_PixelFotmat)
	{
	case RedTexturePixelFormat::R8:
	case RedTexturePixelFormat::R8G8:
	case RedTexturePixelFormat::R8G8B8:
	case RedTexturePixelFormat::R8G8B8A8:
	case RedTexturePixelFormat::R32F:
	case RedTexturePixelFormat::R32G32F:
	case RedTexturePixelFormat::R32G32B32F:
	case RedTexturePixelFormat::R32G32B32A32F:
		Header.dwHeaderFlags |= DDSD_LINEARSIZE;
		Header.dwPitchOrLinearSize = static_cast<DWORD>(RedTextureUtils::GetSizePixel(m_PixelFotmat) * m_Width);
		break;
	default:
		break;
	}
	char ddst[] = {'D','D','S',' '};
	if (!fwrite(ddst, 4, 1, file))
	{
		fclose(file);
		return false;
	}
	switch (m_PixelFotmat)
	{
	case RedTexturePixelFormat::R8:
		Header.ddspf = DDSPF_R8G8B8;
		Header.ddspf.dwFlags = 0x2;
		Header.ddspf.dwRBitMask = 0;
		Header.ddspf.dwBBitMask = 0;
		Header.ddspf.dwGBitMask = 0;
		Header.ddspf.dwABitMask = 255;
		Header.ddspf.dwRGBBitCount = 8;
		break;
	case RedTexturePixelFormat::R8G8:
		Header.ddspf = DDSPF_R8G8B8;
		Header.ddspf.dwBBitMask = 0;
		Header.ddspf.dwGBitMask = 0xFF<<8;
		Header.ddspf.dwRBitMask = 0xFF;
		Header.ddspf.dwRGBBitCount =16;
		break;
	case RedTexturePixelFormat::R8G8B8:
		Header.ddspf = DDSPF_R8G8B8;
		Header.ddspf.dwBBitMask = 0xFF << 16;
		Header.ddspf.dwGBitMask = 0xFF << 8;
		Header.ddspf.dwRBitMask = 0xFF;
		Header.ddspf.dwRGBBitCount = 24;
		break;
	case RedTexturePixelFormat::R8G8B8A8:
		Header.ddspf = DDSPF_A8R8G8B8;
		Header.ddspf.dwBBitMask = 0xFF << 16;
		Header.ddspf.dwGBitMask = 0xFF << 8;
		Header.ddspf.dwRBitMask = 0xFF;
		Header.ddspf.dwABitMask = DWORD(0xFF << 24);
		Header.ddspf.dwRGBBitCount = 32;
		break;
	case RedTexturePixelFormat::R32F:
		Header.ddspf = DDSPF_DX10;
		Header.Header10.dxgiFormat = DXGI_FORMAT_R32_FLOAT;
		Header.Header10.arraySize = static_cast<DWORD>(m_Depth);
		Header.Header10.resourceDimension = 3;
		break;
	case RedTexturePixelFormat::R32G32F:
		Header.ddspf = DDSPF_DX10;
		Header.Header10.dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
		Header.Header10.arraySize = static_cast<DWORD>(m_Depth);
		Header.Header10.resourceDimension = 3;
		break;
	case RedTexturePixelFormat::R32G32B32F:
		Header.ddspf = DDSPF_DX10;
		Header.Header10.dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		Header.Header10.arraySize = static_cast<DWORD>(m_Depth);
		Header.Header10.resourceDimension = 3;
		break;
	case RedTexturePixelFormat::R32G32B32A32F:
		Header.ddspf = DDSPF_DX10;
		Header.Header10.dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
		Header.Header10.arraySize = static_cast<DWORD>(m_Depth);
		Header.Header10.resourceDimension = 3;
		break;
	case RedTexturePixelFormat::DXT_1:
	case RedTexturePixelFormat::DXT_1_Alpha:
		Header.ddspf = DDSPF_DXT1;
		break;
	case RedTexturePixelFormat::DXT_3:
		Header.ddspf = DDSPF_DXT3;
		break;
	case RedTexturePixelFormat::DXT_5:
		Header.ddspf = DDSPF_DXT5;
		break;
	case RedTexturePixelFormat::BC4:
		Header.ddspf = DDSPF_ATI1;
		break;
	case RedTexturePixelFormat::BC5:
		Header.ddspf = DDSPF_ATI2;
		break;
	case RedTexturePixelFormat::BC6:
		Header.ddspf = DDSPF_DX10;			
		Header.Header10.dxgiFormat = DXGI_FORMAT_BC6H_UF16;
		Header.Header10.arraySize = static_cast<DWORD>(m_Depth);
		Header.Header10.resourceDimension = 3;
		break;
	case RedTexturePixelFormat::BC7:
		Header.ddspf = DDSPF_DX10;
		Header.Header10.dxgiFormat = DXGI_FORMAT_BC7_UNORM;
		Header.Header10.arraySize = static_cast<DWORD>(m_Depth);
		Header.Header10.resourceDimension = 3;
		break;
	default:
		return false;
	}
	if (!fwrite(&Header, Header.dwSize + (Header.ddspf.dwFourCC == MAKEFOURCC('D', 'X', '1', '0') ? sizeof(Header.Header10) : 0), 1, file) )
	{
		fclose(file);
		return false;
	}
	if (!fwrite(m_ImageBuffer, RedTextureUtils::GetSizeInMemory(m_Width, m_Height, m_Mips, m_PixelFotmat) * m_Depth,1,file))
	{
		fclose(file);
		return false;
	}
	fclose(file);
	return true;
}
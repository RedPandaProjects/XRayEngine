#include "RedImage.hpp"
#include "stbImage/stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

using namespace RedImageTool;
extern "C"
{
	void* Malloc(size_t a)
	{
		return red_alloc<u8>(a);
	}
	void Free(void* a)
	{
		if (!a)return;
		red_free(a);

	}
	void* Realloc(void* x, size_t n)
	{
		return  red_realloc<u8>((u8*)x, n);
	}
}
#include "StbImage/stb_image.h"
#include "StbImage/stb_image_write.h"
bool RedImage::LoadFromFile(const char * str)
{
	FILE*file = fopen(str, "rb");
	if (!file)
		return false;
	fseek(file, 0, SEEK_END);
	u32 Size = ftell(file);
	fseek(file, 0, SEEK_SET);
	u8* Data = red_alloc<u8>(Size);
	if (!fread(Data, Size, 1, file))
	{
		fclose(file);
		return false;
	}
	LoadFromMemory(Data, Size);
	red_free(Data);
	fclose(file);
	return true;
}

bool RedImage::LoadFromMemory(void* pointer, size_t size)
{
	if (LoadDDSFromMemory(pointer, size))
	{
		return true;
	}
	int w, h, comp;
	stbi_uc* data1 = stbi_load_from_memory((const stbi_uc*)pointer, (int)size, &w, &h, &comp, STBI_rgb_alpha);
	if (!data1) 
	{
		return false;
	}
	Create(w, h);
	memcpy(m_ImageBuffer, data1, RedTextureUtils::GetSizeInMemory(w, h, 1, m_PixelFotmat));
	red_free(data1);
	return true;
}


bool RedImage::LoadFromFile(size_t depth, const char * str)
{
	if (!Empty())
		return false;
	FILE* file = fopen(str, "rb");
	if (!file)
		return false;
	fseek(file, 0, SEEK_END);
	u32 Size = ftell(file);
	fseek(file, 0, SEEK_SET);
	u8* Data = red_alloc<u8>(Size);
	if (!fread(Data, Size, 1, file))
	{
		fclose(file);
		return false;
	}
	LoadFromMemory(depth,Data, Size);
	red_free(Data);
	fclose(file);
	return true;
}

bool RedImage::LoadFromMemory(size_t depth, void* pointer, size_t size)
{
	if (!Empty())
		return false;
	int w, h, comp;
	stbi_uc*data = stbi_load_from_memory((const stbi_uc*)pointer, (int)size, &w, &h, &comp, STBI_rgb_alpha);
	if (!data)
	{
		return false;
	}
	Resize(w, h, depth, RedTexturePixelFormat::R8G8B8A8);
	size_t size_image = RedTextureUtils::GetSizeInMemory(w, h, m_Mips, m_PixelFotmat);
	memcpy(m_ImageBuffer + (depth* size_image), data, RedTextureUtils::GetSizeInMemory(w, h, 1, m_PixelFotmat));
	red_free(data);
	return true;
}
bool RedImage::SaveToJpg(const char * name, size_t depth)
{
	assert(m_Depth > depth);
	assert(!RedTextureUtils::isCompressor(m_PixelFotmat));
	assert(!RedTextureUtils::isFloatPixel(m_PixelFotmat));
	return stbi_write_jpg(name, static_cast<int>(m_Width), static_cast<int>(m_Height), static_cast<int>(RedTextureUtils::GetCountComp(m_PixelFotmat)), m_ImageBuffer + depth * RedTextureUtils::GetSizeInMemory(m_Width, m_Height, m_Mips, m_PixelFotmat), 100);
}

bool RedImage::SaveToPng(const char * name, size_t depth)
{
	assert(m_Depth > depth);
	assert(!RedTextureUtils::isCompressor(m_PixelFotmat));
	assert(!RedTextureUtils::isFloatPixel(m_PixelFotmat));
	return stbi_write_png(name, static_cast<int>(m_Width), static_cast<int>(m_Height), static_cast<int>(RedTextureUtils::GetCountComp(m_PixelFotmat)), m_ImageBuffer + depth * RedTextureUtils::GetSizeInMemory(m_Width, m_Height, m_Mips, m_PixelFotmat), 0);
}

bool RedImage::SaveToBmp(const char * name, size_t depth)
{
	assert(m_Depth > depth);
	assert(!RedTextureUtils::isCompressor(m_PixelFotmat));
	assert(!RedTextureUtils::isFloatPixel(m_PixelFotmat));
	return stbi_write_bmp(name, static_cast<int>(m_Width), static_cast<int>(m_Height), static_cast<int>(RedTextureUtils::GetCountComp(m_PixelFotmat)), m_ImageBuffer + depth * RedTextureUtils::GetSizeInMemory(m_Width, m_Height, m_Mips, m_PixelFotmat));
}

bool RedImage::SaveToTga(const char * name, size_t depth)
{
	assert(m_Depth > depth);
	assert(!RedTextureUtils::isCompressor(m_PixelFotmat));
	assert(!RedTextureUtils::isFloatPixel(m_PixelFotmat));
	return stbi_write_tga(name, static_cast<int>(m_Width), static_cast<int>(m_Height), static_cast<int>(RedTextureUtils::GetCountComp(m_PixelFotmat)), m_ImageBuffer + depth * RedTextureUtils::GetSizeInMemory(m_Width, m_Height, m_Mips, m_PixelFotmat));

}
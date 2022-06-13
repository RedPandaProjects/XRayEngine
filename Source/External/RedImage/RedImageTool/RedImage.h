#pragma once 
namespace RedImageTool
{
	class  RedImage
	{
	public:
		RedImage();
		RedImage(size_t w, size_t h, size_t mip = 1, size_t depth = 1, RedTexturePixelFormat px = RedTexturePixelFormat::R8G8B8A8, bool cube = false);
		void Fill(const RedColor& color);
		void Create(size_t w, size_t h, size_t mip = 1, size_t depth = 1, RedTexturePixelFormat px = RedTexturePixelFormat::R8G8B8A8, bool cube = false);
		void Append(size_t x, size_t y, const RedImage& Image, size_t x_src, size_t y_src, size_t w_src, size_t h_src, size_t dst_depth, size_t src_depth);
		void Append(size_t x, size_t y, const RedImage& Image, size_t dst_depth, size_t src_depth);

		void Scale(size_t w, size_t h, RedResizeFilter filter = RedResizeFilter::Default);

		void ScaleCanvas(size_t w, size_t h);


		void GenerateMipmap(size_t depth, RedResizeFilter filter = RedResizeFilter::Default);
		void GenerateMipmap(RedResizeFilter filter = RedResizeFilter::Default);
		void ClearMipLevels();

		void NormalizedSizeNotScale();
		void NormalizedSize(RedResizeFilter filter = RedResizeFilter::Default);

		RedColor GetPixel(size_t x, size_t y, size_t d = 0)const;
		void SetPixel(const RedColor& color, size_t x, size_t y, size_t d = 0);
		void SwapRB();

		bool LoadFromFile(const char* str);
		bool LoadFromMemory(void*pointer,size_t size);

		bool LoadFromFile(size_t depth, const char* str);
		bool LoadFromMemory(size_t depth, void* pointer, size_t size);

		bool SaveToJpg(const char* name, size_t depth = 0);
		bool SaveToPng(const char* name, size_t depth = 0);
		bool SaveToBmp(const char* name, size_t depth = 0);
		bool SaveToTga(const char* name, size_t depth = 0);
		bool SaveToDds(const char* name);
		void* operator*();
		const void* operator*()const;

		void Clear();
		~RedImage();

		bool Empty() const;

		RedImage(RedImage&& Image);
		RedImage(const RedImage& Image);
		void Copy(const RedImage& Image);
		void Swap(RedImage& Image);
		RedImage& operator=(const RedImage& Image);
		RedImage& operator=(RedImage&& Image);
		void Convert(RedTexturePixelFormat format);

		RedTexturePixelFormat GetFormat()const;
		size_t GetDepth()const;
		size_t GetMips()const;
		size_t GetSizeInMemory()const;
		size_t GetWidth()const;
		size_t GetHeight()const;

		inline bool SetCubeMap() { m_bCube = m_Width == m_Height && (m_Depth % 6 == 0) && m_Depth; if (m_bCube)m_Depth = m_Depth / 6; return m_bCube; }
		inline bool IsCubeMap()const { return m_bCube; }
	private:
		void Resize(size_t w, size_t h, size_t depth, RedTexturePixelFormat px);
		RedTexturePixelFormat m_PixelFotmat;
		size_t m_Width, m_Height, m_Mips, m_Depth;
		bool m_bCube;
		u8* m_ImageBuffer;
	private:
		bool LoadDDSFromMemory(void* pointer, size_t size);
	};
}

#pragma once
namespace RedImageTool
{
	class RedTextureUtils
	{
	public:
		static void Fill(u8* data, size_t x, size_t y, size_t mip, const RedColor& color, RedTexturePixelFormat format);
		static void GenerateMip(u8* dst, u8* src, size_t w_src, size_t h_src, RedTexturePixelFormat format, RedResizeFilter filter);
		static void Scale(u8* dst, size_t w_dst, size_t h_dst, u8* src, size_t w_src, size_t h_src, RedTexturePixelFormat format, RedResizeFilter filter);
		static void Append(u8* dst, size_t w_dst, size_t h_dst, size_t x_dst, size_t y_dst, u8* src, size_t w_src, size_t h_src, size_t squard_src_x, size_t squard_src_y, size_t squard_src_x1, size_t squard_src_y1, RedTexturePixelFormat dst_format, RedTexturePixelFormat src_format);


		static size_t GetSizeWidth(size_t w, RedTexturePixelFormat format);
		static size_t GetSizeDepth(size_t w, size_t h, RedTexturePixelFormat format);
		static size_t GetSizePixel(RedTexturePixelFormat format);
		static size_t GetCountMips(size_t w, size_t h);
		static size_t GetMip(size_t w, size_t level);
		static u8* GetPixelUint8(size_t x, size_t y, size_t w, size_t comps, size_t comp_id, u8* data);
		static float* GetPixelFloat(size_t x, size_t y, size_t w, size_t comps, size_t comp_id, u8* data);
		static bool isFloatPixel(RedTexturePixelFormat format);
		static bool isCompressor(RedTexturePixelFormat format);
		static size_t GetSizeInMemory(size_t w, size_t h, size_t mips, RedTexturePixelFormat format);
		static void Convert(RedTexturePixelFormat dst_format, RedTexturePixelFormat src_format, u8* dst, u8* src, size_t w, size_t h);
		static size_t GetCountBlock(size_t w);
		static size_t GetCountBlock(size_t w, RedTexturePixelFormat format);
		static size_t GetSizeBlock(RedTexturePixelFormat format);
		static u8 GetCountComp(RedTexturePixelFormat format);
		static u8* GetImage(u8* data, size_t w, size_t h, size_t mips, size_t depth, size_t mip, RedTexturePixelFormat format);

		static void  GetPixel(RedColor& color, u8* data, size_t x, size_t y, size_t depth, size_t w, size_t h, size_t mips, RedTexturePixelFormat format);
		static void  SetPixel(const RedColor& color, u8* data, size_t x, size_t y, size_t depth, size_t w, size_t h, size_t mips, RedTexturePixelFormat format);
	private:
		static void ScaleFloat(u8* dst, size_t w_dst, size_t h_dst, u8* src, size_t w_src, size_t h_src, u8 comp, RedResizeFilter filter);
		static void ScaleUint8(u8* dst, size_t w_dst, size_t h_dst, u8* src, size_t w_src, size_t h_src, u8 comp, RedResizeFilter filter);


		static void GenerateMipFloat(u8* dst, u8* src, size_t w_src, size_t h_src, u8 comps, RedResizeFilter filter);
		static void GenerateMipUint8(u8* dst, u8* src, size_t w_src, size_t h_src, u8 comps, RedResizeFilter filter);

		static void FloatPixelToUint8(u8* dst, float* src, u8 comp_dst, u8 comp_src);
		static void FloatPixelToFloat(float* dst, float* src, u8 comp_dst, u8 comp_src);
		static void Uint8PixelToFloat(float* dst, u8* src, u8 comp_dst, u8 comp_src);
		static void Uint8PixelToUint8(u8* dst, u8* src, u8 comp_dst, u8 comp_src);

		static void FloatPixelToHalf4(u16* dst, float* src, u8 comp_src);
		static void Uint8PixelToHalf4(u16* dst, u8* src, u8 comp_src);

		static void SwapRB(u32& a);

		static void FillUint8(u8* data, size_t x, size_t y, const RedColor& color, RedTexturePixelFormat format);
		static void FillFloat(u8* data, size_t x, size_t y, const RedColor& color, RedTexturePixelFormat format);

		static void Uint8ToHalf4(u8* dst, u8* src, size_t w, size_t h, u8 comp_src);
		static void Uint8ToFloat(u8* dst, u8* src, size_t w, size_t h, u8 comp_dst, u8 comp_src);
		static void Uint8ToUint8(u8* dst, u8* src, size_t w, size_t h, u8 comp_dst, u8 comp_src);
		static void Uint8ToCompressor(u8* dst, u8* src, size_t w, size_t h, RedTexturePixelFormat compressor, u8 comp_src);

		static void FloatToHalf4(u8* dst, u8* src, size_t w, size_t h, u8 comp_src);
		static void FloatToUint8(u8* dst, u8* src, size_t w, size_t h, u8 comp_dst, u8 comp_src);
		static void FloatToFloat(u8* dst, u8* src, size_t w, size_t h, u8 comp_dst, u8 comp_src);
		static void FloatToCompressor(u8* dst, u8* src, size_t w, size_t h, RedTexturePixelFormat compressor, u8 comp_src);

		static void CompressorToCompressor(u8* dst, u8* src, size_t w, size_t h, RedTexturePixelFormat compressor_dst, RedTexturePixelFormat compressor_src);
		static void CompressorToUint8(u8* dst, u8* src, size_t w, size_t h, u8 comp_dst, RedTexturePixelFormat compressor);
		static void CompressorToFloat(u8* dst, u8* src, size_t w, size_t h, u8 comp_dst, RedTexturePixelFormat compressor);

		static void* StartCompressor(RedTexturePixelFormat compressor, size_t w, size_t h);
		static void EndCompressor(RedTexturePixelFormat compressor, size_t w, size_t h, void* in, void* out);

		static void* StartDecompressor(RedTexturePixelFormat compressor, size_t w, size_t h, void* in);
		static void EndDecompressor(void* in);

		static void GetBlock(RedColor(&color)[16], u8* data, size_t w, size_t h, size_t x, size_t y, RedTexturePixelFormat px);
		static void SetBlock(RedColor(&color)[16], u8* data, size_t w, size_t h, size_t x, size_t y, RedTexturePixelFormat px);
		static bool CompressorAsFloat(RedTexturePixelFormat px);
		static u8* TempUncompressor(u8* data, size_t w, size_t h, RedTexturePixelFormat px, RedTexturePixelFormat& out_px);
		static void TempCompress(u8* in, u8* out, size_t w, size_t h, RedTexturePixelFormat px);
	};
};
#pragma once 
namespace RedImageTool
{
	class  RedColor
	{
	public:
		RedColor() { SetAsUint32(0); }
		inline ~RedColor() { }

		RedColor(u32 color)
		{
			SetAsUint32(color);
		}
		RedColor(float R, float G, float B, float A = 1)
		{
			SetAsFloat(R, G, B, A);
		}

		RedColor(const RedColor& color) { Copy(color); }
		inline void Copy(const RedColor& color) { R8G8B8A8 = color.R8G8B8A8; memcpy(R32G32B32A32, color.R32G32B32A32, 4 * sizeof(float)); }
		inline void Swap(RedColor& color) { std::swap(R8G8B8A8, color.R8G8B8A8); std::swap(R32G32B32A32, color.R32G32B32A32); }
		inline RedColor& operator=(const RedColor& color)
		{
			Copy(color);
			return *this;
		}

		inline void SetAsUint32(u32 color)
		{
			R8G8B8A8 = color;
			R32F = static_cast<float>(R8U) / 255.f;
			G32F = static_cast<float>(G8U) / 255.f;
			B32F = static_cast<float>(B8U) / 255.f;
			A32F = static_cast<float>(A8U) / 255.f;
		}
		void SetAsUint8(u8 R, u8 G, u8 B, u8 A = 255)
		{
			R8U = R;
			G8U = G;
			B8U = B;
			A8U = A;
			R32F = static_cast<float>(R8U) / 255.f;
			G32F = static_cast<float>(G8U) / 255.f;
			B32F = static_cast<float>(B8U) / 255.f;
			A32F = static_cast<float>(A8U) / 255.f;
		}
		void SetAsFloat(float R, float G, float B, float A = 1.f)
		{
			R32F = R;
			G32F = G;
			B32F = B;
			A32F = A;
			R8U = static_cast<u8>(R32F * 255.f);
			G8U = static_cast<u8>(G32F * 255.f);
			B8U = static_cast<u8>(B32F * 255.f);
			A8U = static_cast<u8>(A32F * 255.f);
		}

		union
		{
			struct
			{
				float R32F;
				float G32F;
				float B32F;
				float A32F;
			};
			struct
			{
				float R32G32B32A32[4];
			};
		};
		union
		{
			struct
			{
				u8 R8U;
				u8 G8U;
				u8 B8U;
				u8 A8U;
			};
			struct
			{
				u32 R8G8B8A8;
			};
		};
	private:

	};
};
#pragma once
#pragma pack(push,4)
struct XRayGlobalUniform
{
	Fvector4        FogColor;
	Fvector4        FogPlane;
	Fvector4        FogParam;

	Fvector4		SunColor;
	Fvector4		SunDirW;
	Fvector4		SunDirE;

	Fvector4        HemiColor;
	Fvector4        AmbientColor;
	Fvector4        PosDecompressParams;
	Fvector4        EyePosition;
	Fvector4        Timer;
	Fvector4        BloomParams;
	Fvector4        BloomW0;
	Fvector4        BloomW1;
	float DetailTextureRange;
};
#pragma pack(pop)
#pragma pack(push,4)
struct XRayLocalUniform
{
	Fvector4 DetailParams;
};
#pragma pack(pop)
class XRayUniformAllocator
{

public:
	enum EUniformType
	{
		UT_Transformation,
		UT_Skinned,
		UT_Tree,
		UT_Local,
		UT_DetailScalar,
		UT_AlphaRef,
		UT_MeshInfo,
		UT_Count,
		UT_GlobalUniform,
	};
	enum EReserveTransformation
	{
		RT_Fidentity=0,
		RT_Sky,
		RT_Clouds,
		RT_Count,
	};
	XRayUniformAllocator();
	~XRayUniformAllocator();
	inline static size_t GetUniformElementSize(EUniformType type)
	{
		switch (type)
		{
		case XRayUniformAllocator::UT_MeshInfo:
			return sizeof(float) * 4;
		case XRayUniformAllocator::UT_Transformation:
			return sizeof(float) * 16 * 6;
			break;
		case XRayUniformAllocator::UT_Tree:
			return sizeof(float) * ((16 *2)+(4*5));
			break;
		case XRayUniformAllocator::UT_Skinned:
			return sizeof(float) * (4 * 256);
			break;
		case XRayUniformAllocator::UT_AlphaRef:
			return sizeof(float);
		case XRayUniformAllocator::UT_Local:
			return sizeof(XRayLocalUniform);
			break;
		case XRayUniformAllocator::UT_GlobalUniform:
			return sizeof(XRayGlobalUniform);
			break;
		case XRayUniformAllocator::UT_DetailScalar:
			return sizeof(float) * 4;
		}
		return 0;
	}
	void Reserve(EUniformType Type, size_t count);
	size_t Alloc(EUniformType Type);
	inline static size_t GetRegister(EUniformType Type)
	{
		switch (Type)
		{
		case XRayUniformAllocator::UT_MeshInfo:
			return 8;
		case XRayUniformAllocator::UT_AlphaRef:
			return 9;
		case XRayUniformAllocator::UT_DetailScalar:
			return 10;
		case XRayUniformAllocator::UT_Transformation:
			return 0;
		case XRayUniformAllocator::UT_Local:
			return 11;
		case XRayUniformAllocator::UT_GlobalUniform:
			return 12;
		case XRayUniformAllocator::UT_Tree:
		case XRayUniformAllocator::UT_Skinned:
			return 1;
		}
		return 0;
	}
	inline  size_t CalcOffset(EUniformType type, size_t id) 
	{
		return Uniform[type]->GetStride() * id;
	}
	inline void Flush()
	{
		for (size_t i = 0; i < XRayUniformAllocator::UT_Count; i++)
		{
			if (UT_DetailScalar == i)
				continue;
			m_Counter[i] = 0;
		}
	}
	BearFactoryPointer<BearRHI::BearRHIUniformBuffer> Uniform[UT_Count];
	BearFactoryPointer<BearRHI::BearRHIUniformBuffer> GlobalUniform;
private:
	size_t m_Size[UT_Count];
	size_t m_Counter[UT_Count];
	
};
extern XRayUniformAllocator* GUniformAllocator; 
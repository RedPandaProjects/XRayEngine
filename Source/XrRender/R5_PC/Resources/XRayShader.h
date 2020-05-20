#pragma once
enum ShaderTypeTransformation
{
	STT_Screen,
	STT_BloomScreen,
	STT_Matrix,
};
enum ShaderVertexDeclaration
{
#define ADD_SVD(x) SVD_ ## x,
#include "XRaySVDList.h"
#undef ADD_SVD
	SVD_Count,
};
enum ShaderSamplerState
{
	SSS_None,
	SSS_Default,
	SSS_NoFilter,
	SSS_Point,
	SSS_RTLinear,
	SSS_Linear,
};
enum	EShaderElement
{
	SE_NORMAL_HQ = 0,	// high quality/detail
	SE_NORMAL_LQ = 1,	// normal or low quality
	SE_LPOINT = 2,	// add: point light
	SE_LSPOT = 3,	// add:	spot light
	SE_LMODELS = 4,	// lighting info for models or shadowing from models
};
struct XRayPipeline :public XRayResource
{
	xr_map<size_t, BearFactoryPointer<BearRHI::BearRHIPipeline>>  Container;
};
using XRayPipelineRef = XRayResourceRef<XRayPipeline>;
class  XRayShaderElement
{
public:
	
	XRayShaderElement() { for (size_t i = 0; i < 16; i++) Textures[i] = 0; for (size_t i = 0; i < 16; i++)SamplerStates[i] = SSS_None; UseGlobalUniform = false; UseLocalUniform = false; DetailScalar = 0; AlphaRef = -1; StencilRef = -1; Sort = false; }
	~XRayShaderElement();
	void Clear();
	void Copy(const XRayShaderElement& right);
	void Swap(XRayShaderElement& right);
	
	XRayShaderElement(const XRayShaderElement& right)	{ for (size_t i = 0; i < 16; i++) Textures[i] = 0; Copy(right);}
	XRayShaderElement( XRayShaderElement&& right){ for (size_t i = 0; i < 16; i++) Textures[i] = 0; 	Swap(right);}
	XRayShaderElement& operator=(const XRayShaderElement& right) { Copy(right); return*this; }
	XRayShaderElement& operator=( XRayShaderElement&& right) { Swap(right); return*this; }

	XRayPipelineRef Pipeline;
	BearFactoryPointer<BearRHI::BearRHIRootSignature> RootSignature;
	BearFactoryPointer<BearRHI::BearRHIDescriptorHeap> DescriptorHeap;
	bool UseGlobalUniform;
	bool UseLocalUniform;
	size_t DetailScalar;
	int AlphaRef;
	int StencilRef;
	bool Sort;
	XRayLocalUniform LocalUniform;
	ShaderTypeTransformation TypeTransformation;
	XRayTexture* Textures[16];
	ShaderSamplerState SamplerStates[16];
	void CreateDescriptorHeap();
	bool Set(XRayObjectRender& item, u32 FVF);
	inline bool Set(BearFactoryPointer<BearRHI::BearRHIContext>& context,u32 FVF)
	{
		if (Pipeline.empty())return false;

		auto item =  Pipeline->Container.find(ToVertexDeclaration(FVF));
		if (item == Pipeline->Container.end())return false;
		if (item->second.empty())return false;
		context->SetPipeline(item->second);
		context->SetDescriptorHeap(DescriptorHeap);
		return true;
	}
private:
	
	inline ShaderVertexDeclaration ToVertexDeclaration(u32 FVF)
	{
		switch (FVF)
		{
#define ADD_SVD(x) case FVF::F_ ## x: return SVD_ ## x;
#include "XRaySVDList.h"
#undef ADD_SVD	
		default:
			R_ASSERT(0);
			break;
		};
		return SVD_Count;
	}
};
struct XRayShader:public XRayResource
{
	shared_str Name;
	bool MeshPipeline;
	XRayShaderElement E[2][6];
	inline void Clear()
	{
		for (size_t a = 0; a < 2; a++) for (size_t i = 0; i < 6; i++)E[a][i].Clear();
	}
};
using XRayShaderRef = XRayResourceRef<XRayShader>;
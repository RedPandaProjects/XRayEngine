#include "pch.h"

XRayBlenderModel::XRayBlenderModel()
{
	Description.ID = B_MODEL;
	Description.Version = 2;
	oTessellation.Count = 4;
	oTessellation.IDselected = 0;
	oAREF.value = 32;
	oAREF.min = 0;
	oAREF.max = 255;
	oBlend.value = FALSE;
}

XRayBlenderModel::~XRayBlenderModel()
{
}

LPCSTR XRayBlenderModel::getComment()
{
	return LPCSTR();
}

void XRayBlenderModel::Save(IWriter & fs)
{
}

void XRayBlenderModel::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	switch (version)
	{
	case 0:
		oAREF.value = 32;
		oAREF.min = 0;
		oAREF.max = 255;
		oBlend.value = FALSE;
		break;
	case 1:
	default:
		xrPREAD_PROP(fs, xrPID_BOOL, oBlend);
		xrPREAD_PROP(fs, xrPID_INTEGER, oAREF);
		break;
	}
	if (version > 1)
	{
		xrPREAD_PROP(fs, xrPID_TOKEN, oTessellation);
	}
}

void XRayBlenderModel::InitializeGraphics()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.Samplers[0].Shader = BearShaderType::Pixel;
	RootSignatureDescription.SRVResources[0].Shader = BearShaderType::Pixel;
	RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = BearShaderType::Vertex;
	RootSignature[1][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
	RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Skinned)].Shader = BearShaderType::Vertex;
	RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
	BearPipelineGraphicsDescription PipelineDescription;
	PipelineDescription.DepthStencilState.DepthEnable = true;
	PipelineDescription.DepthStencilState.StencillEnable = true;
	PipelineDescription.DepthStencilState.FrontFace.StencilPassOp = BearStencilOp::Replace;
	PipelineDescription.DepthStencilState.FrontFace.StencilTest =  BearCompareFunction::Always;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Base;
	if (oBlend.value)
	{
		PipelineDescription.RenderPass = GRenderTarget->RenderPass_Generic;
		PipelineDescription.BlendState.RenderTarget[0].Enable = true;
		PipelineDescription.BlendState.RenderTarget[0].ColorSrc = BearBlendFactor::SrcAlpha;
		PipelineDescription.BlendState.RenderTarget[0].ColorDst = BearBlendFactor::InvSrcAlpha;
	}
	//CreatePipeline(1, PipelineDescription, "default", "default", SVD_M);
	CreatePipeline(1, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("model\\model_0w", false , oBlend.value), GetPixelShaderName("model\\model", false, false, oBlend.value), SVD_0W);
	CreatePipeline(0, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("model\\model_1w", false, oBlend.value), GetPixelShaderName("model\\model", false, false, oBlend.value), SVD_1W);
	CreatePipeline(0, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("model\\model_2w", false, oBlend.value), GetPixelShaderName("model\\model", false, false, oBlend.value), SVD_2W);
	CreatePipeline(0, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("model\\model_3w", false, oBlend.value), GetPixelShaderName("model\\model", false, false, oBlend.value), SVD_3W);
	CreatePipeline(0, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("model\\model_4w", false, oBlend.value), GetPixelShaderName("model\\model", false, false, oBlend.value), SVD_4W);
}
void XRayBlenderModel::Compile(XRayShaderElement& shader)
{
	switch (IDShader)
	{
	case SE_NORMAL_HQ:
		SetTexture(shader, 0, "$base0");
		shader.SamplerStates[0] = SSS_Default;
		shader.TypeTransformation = STT_Matrix;
		shader.StencilRef = 1;
		shader.Sort = oBlend.value;
	};
}


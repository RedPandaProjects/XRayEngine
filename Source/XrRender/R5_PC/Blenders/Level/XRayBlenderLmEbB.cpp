#include "pch.h"

XRayBlenderLmEbB::XRayBlenderLmEbB()
{
	Description.ID = B_LmEbB;
	Description.Version = 0x1;
	xr_strcpy(oT2_Name, TEXT("$null"));
	xr_strcpy(oT2_xform, TEXT("$null"));
	oBlend.value = FALSE;
}

XRayBlenderLmEbB::~XRayBlenderLmEbB()
{
}

LPCSTR XRayBlenderLmEbB::getComment()
{
	return LPCSTR();
}

void XRayBlenderLmEbB::Save(IWriter & fs)
{
}

void XRayBlenderLmEbB::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	xrPREAD_MARKER(fs);
	xrPREAD_PROP(fs, xrPID_TEXTURE, oT2_Name);
	xrPREAD_PROP(fs, xrPID_MATRIX, oT2_xform);
	if (version >= 0x1) {
		xrPREAD_PROP(fs, xrPID_BOOL, oBlend);
	}
}

void XRayBlenderLmEbB::InitializeGraphics()
{
	{
		BearRootSignatureDescription RootSignatureDescription;
		RootSignatureDescription.Samplers[0].Shader = ST_Pixel;
		RootSignatureDescription.SRVResources[0].Shader = ST_Pixel;
		RootSignatureDescription.SRVResources[1].Shader = ST_Pixel;
		RootSignatureDescription.SRVResources[2].Shader = ST_Pixel;
		RootSignatureDescription.SRVResources[3].Shader = ST_Pixel;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = ST_Vertex;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform)].Shader = ST_ALL;

		RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
	}

	{
		BearPipelineGraphicsDescription PipelineDescription;
		{
			PipelineDescription.DepthStencilState.DepthEnable = true;
			PipelineDescription.DepthStencilState.StencillEnable = true;
			PipelineDescription.DepthStencilState.FrontFace.StencilPassOp = SO_REPLACE;
			PipelineDescription.DepthStencilState.FrontFace.StencilTest = CF_ALWAYS;
		}

		PipelineDescription.RenderPass = GRenderTarget->RenderPass_Base;
		if (oBlend.value)
		{
			PipelineDescription.RenderPass = GRenderTarget->RenderPass_Generic;
			PipelineDescription.BlendState.RenderTarget[0].Enable = true;
			PipelineDescription.BlendState.RenderTarget[0].ColorSrc = BF_SRC_ALPHA;
			PipelineDescription.BlendState.RenderTarget[0].ColorDst = BF_INV_SRC_ALPHA;
		}

		CreatePipeline(0, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmape",false, oBlend.value),GetPixelShaderName( "level\\lmap\\lmape",false,false, oBlend.value), SVD_R1LMap);
		CreatePipeline(0, SE_NORMAL_LQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmape", false, oBlend.value), GetPixelShaderName("level\\lmap\\lmape", false, false, oBlend.value), SVD_R1LMap);
	}
}

void XRayBlenderLmEbB::Compile(XRayShaderElement& shader)
{
	if (IDSlot == 0)
	{
		switch (IDShader)
		{
		case SE_NORMAL_LQ:
		case SE_NORMAL_HQ:
			SetTexture(shader, 0, "$base0");
			SetTexture(shader, 1, "$base1");
			SetTexture(shader, 2, "$base2");
			SetTexture(shader, 3, oT2_Name);
			shader.SamplerStates[0] = SSS_Default;
			shader.TypeTransformation = STT_Matrix;
			shader.UseGlobalUniform = true;
			shader.StencilRef = 1;
			shader.Sort = oBlend.value;
			break;
		default:
			break;
		}
	}

}


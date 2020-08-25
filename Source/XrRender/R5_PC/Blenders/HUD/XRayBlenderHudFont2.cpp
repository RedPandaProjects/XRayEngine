#include "pch.h"
#include "XRayBlenderHudFont2.h"

XRayBlenderHudFont2::XRayBlenderHudFont2()
{
}

XRayBlenderHudFont2::~XRayBlenderHudFont2()
{
}

void XRayBlenderHudFont2::InitializeGraphics()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.Samplers[0].Shader = BearShaderType::Pixel;
	RootSignatureDescription.SRVResources[0].Shader = BearShaderType::Pixel;
	RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = BearShaderType::Vertex;

	RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);

	BearPipelineGraphicsDescription PipelineDescription;
	PipelineDescription.BlendState.RenderTarget[0].Enable = true;
	PipelineDescription.BlendState.RenderTarget[0].ColorSrc = BearBlendFactor::SrcAlpha;
	PipelineDescription.BlendState.RenderTarget[0].ColorDst = BearBlendFactor::InvSrcAlpha;
	PipelineDescription.BlendState.RenderTarget[0].AlphaSrc = BearBlendFactor::SrcAlpha;
	PipelineDescription.BlendState.RenderTarget[0].AlphaDst = BearBlendFactor::DestAlpha;
	PipelineDescription.BlendState.RenderTarget[0].Alpha = BearBlendOp::Max;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Generic;
	CreatePipeline(0, 0, PipelineDescription, "hud\\notransform_tl", "hud\\font2", SVD_TL);
}

void XRayBlenderHudFont2::Compile(XRayShaderElement& shader)
{
	if (IDSlot == 0)
	{
		if (IDShader == 0)
		{
			SetTexture(shader, 0, "$base0");
			shader.SamplerStates[0] = SSS_Default;
			shader.TypeTransformation = STT_Screen;
		}
	}
}
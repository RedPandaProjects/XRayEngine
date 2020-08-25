#include "pch.h"
#include "XRayBlenderSun.h"

XRayBlenderSun::XRayBlenderSun()
{
}

XRayBlenderSun::~XRayBlenderSun()
{
}

void XRayBlenderSun::InitializeGraphics()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = BearShaderType::Vertex;

	RootSignatureDescription.SRVResources[0].Shader = BearShaderType::Pixel;
	RootSignatureDescription.Samplers[0].Shader = BearShaderType::Pixel;
	RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);

	BearPipelineGraphicsDescription PipelineDescription;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Generic;
	{

		PipelineDescription.BlendState.RenderTarget[0].Enable = true;
		PipelineDescription.BlendState.RenderTarget[0].ColorSrc = BearBlendFactor::SrcAlpha;
		PipelineDescription.BlendState.RenderTarget[0].ColorDst = BearBlendFactor::One;
	}
	CreatePipeline(0, 0, PipelineDescription, "effects\\sun", "hud\\hud", SVD_LIT);
	
}

void XRayBlenderSun::Compile(XRayShaderElement& shader)
{
	if (IDSlot == 0)
	{
		if (IDShader == 0)
		{
			SetTexture(shader, 0, "$base0");
			shader.UseGlobalUniform = true;

			shader.SamplerStates[0] = SSS_Default;
			shader.TypeTransformation = STT_Matrix;
		}
	}
}
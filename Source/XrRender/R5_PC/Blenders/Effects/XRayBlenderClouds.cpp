#include "pch.h"
#include "XRayBlenderClouds.h"

XRayBlenderClouds::XRayBlenderClouds()
{
}

XRayBlenderClouds::~XRayBlenderClouds()
{
}

void XRayBlenderClouds::InitializeGraphics()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = BearShaderType::Vertex;
	RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform)].Shader = BearShaderType::ALL;

	RootSignatureDescription.SRVResources[0].Shader = BearShaderType::Pixel;
	RootSignatureDescription.SRVResources[1].Shader = BearShaderType::Pixel;
	RootSignatureDescription.Samplers[0].Shader = BearShaderType::Pixel;
	RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);

	BearPipelineGraphicsDescription PipelineDescription;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Generic;

	PipelineDescription.BlendState.RenderTarget[0].Enable = true;
	PipelineDescription.BlendState.RenderTarget[0].ColorSrc = BearBlendFactor::SrcAlpha;
	PipelineDescription.BlendState.RenderTarget[0].ColorDst = BearBlendFactor::InvSrcAlpha;

	CreatePipeline(0, 0, PipelineDescription, "effects\\clouds", "effects\\clouds", SVD_CLOUDS);
	
}

void XRayBlenderClouds::Compile(XRayShaderElement& shader)
{
	if (IDSlot == 0)
	{
		if (IDShader == 0)
		{
			SetTexture(shader, 0, "$user\\clouds1");
			SetTexture(shader, 1, "$user\\clouds2");
			shader.UseGlobalUniform = true;

			shader.SamplerStates[0] = SSS_RTLinear;
			shader.TypeTransformation = STT_Matrix;
		}
	}
}
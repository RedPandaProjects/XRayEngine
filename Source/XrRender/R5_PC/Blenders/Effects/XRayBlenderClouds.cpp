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
	RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = ST_Vertex;
	RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform)].Shader = ST_ALL;

	RootSignatureDescription.SRVResources[0].Shader = ST_Pixel;
	RootSignatureDescription.SRVResources[1].Shader = ST_Pixel;
	RootSignatureDescription.Samplers[0].Shader = ST_Pixel;
	RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);

	BearPipelineGraphicsDescription PipelineDescription;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Generic;

	PipelineDescription.BlendState.RenderTarget[0].Enable = true;
	PipelineDescription.BlendState.RenderTarget[0].ColorSrc = BF_SRC_ALPHA;
	PipelineDescription.BlendState.RenderTarget[0].ColorDst = BF_INV_SRC_ALPHA;

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
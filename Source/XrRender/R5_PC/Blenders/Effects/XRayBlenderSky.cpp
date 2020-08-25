#include "pch.h"
#include "XRayBlenderSky.h"

XRayBlenderSky::XRayBlenderSky()
{
}

XRayBlenderSky::~XRayBlenderSky()
{
}

void XRayBlenderSky::InitializeGraphics()
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
	CreatePipeline(0, 0, PipelineDescription, "effects\\sky", "effects\\sky", SVD_SKY);
	
}

void XRayBlenderSky::Compile(XRayShaderElement& shader)
{
	if (IDSlot == 0)
	{
		if (IDShader == 0)
		{
			SetTexture(shader, 0, "$user\\sky1");
			SetTexture(shader, 1, "$user\\sky2");
			shader.UseGlobalUniform = true;

			shader.SamplerStates[0] = SSS_RTLinear;
			shader.TypeTransformation = STT_Matrix;
		}
	}
}
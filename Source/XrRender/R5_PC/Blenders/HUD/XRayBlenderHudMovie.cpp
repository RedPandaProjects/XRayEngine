#include "pch.h"
#include "XRayBlenderHudMovie.h"

XRayBlenderHudMovie::XRayBlenderHudMovie()
{
}

XRayBlenderHudMovie::~XRayBlenderHudMovie()
{
}

void XRayBlenderHudMovie::InitializeGraphics()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.Samplers[0].Shader = BearShaderType::Pixel;
	RootSignatureDescription.SRVResources[0].Shader = BearShaderType::Pixel;
	RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = BearShaderType::Vertex;
	
	RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);

	BearPipelineGraphicsDescription PipelineDescription;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Generic;
	CreatePipeline(0, 0,PipelineDescription, "hud\\notransform_tl", "hud\\yuv2rgb", SVD_TL);
}

void XRayBlenderHudMovie::Compile(XRayShaderElement& shader)
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

#include "pch.h"
#include "XRayBlenderViewportSet.h"

XRayBlenderViewportSet::XRayBlenderViewportSet()
{
}

XRayBlenderViewportSet::~XRayBlenderViewportSet()
{
}

void XRayBlenderViewportSet::InitializeGraphics()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.Samplers[0].Shader = BearShaderType::Pixel;
	RootSignatureDescription.SRVResources[0].Shader = BearShaderType::Pixel;
	RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = BearShaderType::Vertex;
	RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);

	BearPipelineGraphicsDescription PipelineDescription;
	PipelineDescription.RenderPass = HW->RenderPass_Viewport;
	CreatePipeline(0, 0,PipelineDescription, "hud\\notransform_tl", "hud\\viewport", SVD_TL);
}

void XRayBlenderViewportSet::Compile(XRayShaderElement& shader)
{
	if (IDSlot == 0)
	{
		if (IDShader == 0)
		{
			SetTexture(shader, 0, RT_GENERIC2);
			shader.SamplerStates[0] = SSS_Default;
			shader.TypeTransformation = STT_Screen;
		}
	}
}

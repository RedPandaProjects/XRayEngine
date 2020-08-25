#include "pch.h"
#include "XRayBlenderBloom.h"

XRayBlenderBloom::XRayBlenderBloom()
{
}

XRayBlenderBloom::~XRayBlenderBloom()
{
}

void XRayBlenderBloom::InitializeGraphics()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = BearShaderType::Vertex;
	RootSignatureDescription.SRVResources[0].Shader = BearShaderType::Pixel;
	RootSignatureDescription.Samplers[0].Shader = BearShaderType::Pixel;
	RootSignature[0][3] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
	RootSignature[0][4] = RootSignature[0][1];
	RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform)].Shader = BearShaderType::Pixel;
	RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
	RootSignature[0][1] = RootSignature[0][0];
	RootSignature[0][2] = RootSignature[0][1];
	BearPipelineGraphicsDescription PipelineDescription;



	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Bloom;
	CreatePipeline(0, 1, PipelineDescription, "hud\\notransform_filter", "effects\\bloom\\bloom_filter", SVD_V_FILTER);
	CreatePipeline(0, 2, PipelineDescription, "hud\\notransform_filter", "effects\\bloom\\bloom_filter", SVD_V_FILTER);
	CreatePipeline(0, 3, PipelineDescription, "hud\\notransform_build", "effects\\bloom\\bloom_filter_f", SVD_V_BUILD);
	CreatePipeline(0, 4, PipelineDescription, "hud\\notransform_build", "effects\\bloom\\bloom_filter_f", SVD_V_BUILD);
	{
		PipelineDescription.BlendState.RenderTarget[0].Enable = true;
		PipelineDescription.BlendState.RenderTarget[0].ColorSrc = BearBlendFactor::SrcAlpha ;
		PipelineDescription.BlendState.RenderTarget[0].ColorDst = BearBlendFactor::InvSrcAlpha;
	}
	CreatePipeline(0, 0, PipelineDescription, "hud\\notransform_build", "effects\\bloom\\bloom_build", SVD_V_BUILD);
	
}

void XRayBlenderBloom::Compile(XRayShaderElement& shader)
{
	if (IDSlot == 0)
	{
		
		shader.SamplerStates[0] = SSS_RTLinear;
		shader.TypeTransformation = STT_Screen;
		switch (IDShader)
		{
		case 0:
			shader.UseGlobalUniform = true;
			SetTexture(shader, 0, RT_BLOOM);
			break;
		case 1:
			shader.UseGlobalUniform = true;
			SetTexture(shader, 0, RT_BLOOM_1);
			break;
		case 2:
			shader.UseGlobalUniform = true;
			SetTexture(shader, 0, RT_BLOOM_2);
			break;
		case 3:
			SetTexture(shader, 0, RT_BLOOM_1);
			break;
		case 4:
			SetTexture(shader, 0, RT_BLOOM_2);
			break;
		}
		
	}
	
}
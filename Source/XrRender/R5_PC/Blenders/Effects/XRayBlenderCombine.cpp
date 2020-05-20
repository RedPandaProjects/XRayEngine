#include "pch.h"
#include "XRayBlenderCombine.h"

XRayBlenderCombine::XRayBlenderCombine()
{
}

XRayBlenderCombine::~XRayBlenderCombine()
{
}

void XRayBlenderCombine::InitializeGraphics()
{
	{
		BearRootSignatureDescription RootSignatureDescription;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = ST_Vertex;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform)].Shader = ST_Pixel;
		RootSignatureDescription.SRVResources[0].Shader = ST_Pixel;
		RootSignatureDescription.SRVResources[1].Shader = ST_Pixel;
		RootSignatureDescription.Samplers[0].Shader = ST_Pixel;
		RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
	}
	{
		BearRootSignatureDescription RootSignatureDescription;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = ST_Vertex;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform)].Shader = ST_Pixel;
		RootSignatureDescription.SRVResources[1].Shader = ST_Pixel;
		RootSignatureDescription.SRVResources[0].Shader = ST_Pixel;
		RootSignatureDescription.Samplers[0].Shader = ST_Pixel;
		RootSignatureDescription.Samplers[1].Shader = ST_Pixel;
		RootSignature[0][1] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
	}




	BearPipelineGraphicsDescription PipelineDescription;

	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Generic;
	CreatePipeline(0, 1, PipelineDescription, "hud\\notransform_tl", "effects\\combine\\combine_2", SVD_TL);
	{
		PipelineDescription.DepthStencilState.DepthEnable = true;
		PipelineDescription.DepthStencilState.EnableDepthWrite = false;
		PipelineDescription.DepthStencilState.DepthTest = CF_ALWAYS;
		PipelineDescription.DepthStencilState.StencilWriteMask = 0;
		PipelineDescription.DepthStencilState.StencillEnable = true;
		PipelineDescription.DepthStencilState.FrontFace.StencilTest = CF_EQUAL;
	}

	{
		PipelineDescription.BlendState.RenderTarget[0].Enable = true;
		PipelineDescription.BlendState.RenderTarget[0].ColorSrc = BF_SRC_ALPHA;
		PipelineDescription.BlendState.RenderTarget[0].ColorDst = BF_INV_SRC_ALPHA;
	}

	CreatePipeline(0,0, PipelineDescription, "hud\\notransform_tl", "effects\\combine\\combine_1", SVD_TL);
	
}

void XRayBlenderCombine::Compile(XRayShaderElement& shader)
{
	if (IDSlot == 0)
	{
		if (IDShader == 0)
		{
			SetTexture(shader, 0, RT_BASIC);
			SetTexture(shader, 1, RT_COMPUTE);
			shader.UseGlobalUniform = true;
			shader.SamplerStates[0] = SSS_NoFilter;
			shader.TypeTransformation = STT_Screen;
			shader.StencilRef = 1;
		}
		else if (IDShader == 1)
		{
			SetTexture(shader, 0, RT_GENERIC1);
			SetTexture(shader, 1, RT_BLOOM_3);
			shader.UseGlobalUniform = true;
			shader.SamplerStates[0] = SSS_NoFilter;
			shader.SamplerStates[1] = SSS_RTLinear;
			shader.TypeTransformation = STT_Screen;
		}
	}
	
}
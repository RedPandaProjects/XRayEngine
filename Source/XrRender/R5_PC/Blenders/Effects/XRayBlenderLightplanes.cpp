#include "pch.h"
#include "XRayBlenderLightplanes.h"

XRayBlenderLightplanes::XRayBlenderLightplanes()
{
}

XRayBlenderLightplanes::~XRayBlenderLightplanes()
{
}

void XRayBlenderLightplanes::InitializeGraphics()
{
	{
		BearRootSignatureDescription RootSignatureDescription;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = BearShaderType::Vertex;
		RootSignatureDescription.SRVResources[0].Shader = BearShaderType::Pixel;
		RootSignatureDescription.Samplers[0].Shader = BearShaderType::Pixel;
		RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
	}
	{
		BearPipelineGraphicsDescription PipelineDescription;
		PipelineDescription.RenderPass = GRenderTarget->RenderPass_Generic;
		{
			PipelineDescription.BlendState.RenderTarget[0].Enable = true;
			PipelineDescription.BlendState.RenderTarget[0].ColorSrc = BearBlendFactor::SrcAlpha;
			PipelineDescription.BlendState.RenderTarget[0].ColorDst = BearBlendFactor::InvSrcAlpha;
		}
		{
			PipelineDescription.DepthStencilState.DepthEnable = true;
			PipelineDescription.DepthStencilState.StencillEnable = true;
			PipelineDescription.DepthStencilState.FrontFace.StencilPassOp = BearStencilOp::Replace;
			PipelineDescription.DepthStencilState.FrontFace.StencilTest =  BearCompareFunction::Always;
		}
		CreatePipeline(0, 0, PipelineDescription, "effects\\lightplanes", "effects\\lightplanes", SVD_R1Vert);
	}
}

void XRayBlenderLightplanes::Compile(XRayShaderElement& shader)
{

	if (IDSlot == 0)
	{
		switch (IDShader)
		{

		case SE_NORMAL_LQ:
		case SE_NORMAL_HQ:
		{
			SetTexture(shader, 0, "$base0");
			shader.Sort = true;
			shader.SamplerStates[0] = SSS_Default;
			shader.TypeTransformation = STT_Matrix;
			shader.StencilRef = 1;
		}
		}
	};
}
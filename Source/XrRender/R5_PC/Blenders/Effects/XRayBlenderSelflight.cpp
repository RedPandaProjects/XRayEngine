#include "pch.h"
#include "XRayBlenderSelflight.h"

XRayBlenderSelflight::XRayBlenderSelflight()
{
}

XRayBlenderSelflight::~XRayBlenderSelflight()
{
}

void XRayBlenderSelflight::InitializeGraphics()
{
	{
		BearRootSignatureDescription RootSignatureDescription;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = BearShaderType::Vertex;
		RootSignatureDescription.SRVResources[0].Shader = BearShaderType::Pixel;
		RootSignatureDescription.Samplers[0].Shader = BearShaderType::Pixel;
		RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
		RootSignature[0][1] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
	}

	{
		BearPipelineGraphicsDescription PipelineDescription;
		PipelineDescription.RenderPass = GRenderTarget->RenderPass_Base;
		{
			PipelineDescription.DepthStencilState.DepthEnable = true;
			PipelineDescription.DepthStencilState.StencillEnable = true;
			PipelineDescription.DepthStencilState.FrontFace.StencilPassOp = BearStencilOp::Replace;
			PipelineDescription.DepthStencilState.FrontFace.StencilTest =  BearCompareFunction::Always;
		}
		CreatePipeline(0, SE_NORMAL_LQ, PipelineDescription, "level\\vertex\\vertex_simple", "effects\\selflight", SVD_R1Vert);
		CreatePipeline(0, SE_NORMAL_HQ, PipelineDescription, "level\\vertex\\vertex_simple", "effects\\selflight", SVD_R1Vert);
	}
}

void XRayBlenderSelflight::Compile(XRayShaderElement& shader)
{

	if (IDSlot == 0)
	{
		switch (IDShader)
		{

		case SE_NORMAL_LQ:
		case SE_NORMAL_HQ:
		{
			SetTexture(shader, 0, "$base0");
			shader.SamplerStates[0] = SSS_Default;
			shader.TypeTransformation = STT_Matrix;
			shader.StencilRef = 1;
		}
		}
	};
}
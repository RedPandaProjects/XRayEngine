#include "pch.h"
#include "XRayBlenderLOD.h"

XRayBlenderLOD::XRayBlenderLOD()
{
}

XRayBlenderLOD::~XRayBlenderLOD()
{
}

void XRayBlenderLOD::InitializeGraphics()
{
	{
		BearRootSignatureDescription RootSignatureDescription;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = BearShaderType::Vertex;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform)].Shader = BearShaderType::ALL;
		RootSignatureDescription.SRVResources[0].Shader = BearShaderType::Pixel;
		RootSignatureDescription.SRVResources[1].Shader = BearShaderType::Pixel;
		RootSignatureDescription.Samplers[0].Shader = BearShaderType::Pixel;
		RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
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
		CreatePipeline(0, 0, PipelineDescription, "level\\lod", "level\\lod", SVD_LOD);
	}
}

void XRayBlenderLOD::Compile(XRayShaderElement& shader)
{

	if (IDSlot == 0)
	{
		switch (IDShader)
		{

		case SE_NORMAL_LQ:
		case SE_NORMAL_HQ:
		{
			SetTexture(shader, 0, "$base0");
			SetTexture(shader, 1, "$base0", "_nm");
			shader.UseGlobalUniform = true;
			shader.SamplerStates[0] = SSS_Default;
			shader.TypeTransformation = STT_Matrix;
			shader.StencilRef = 1;
		}
		}
	};
}
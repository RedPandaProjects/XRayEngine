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
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = ST_Vertex;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform)].Shader = ST_ALL;
		RootSignatureDescription.SRVResources[0].Shader = ST_Pixel;
		RootSignatureDescription.SRVResources[1].Shader = ST_Pixel;
		RootSignatureDescription.Samplers[0].Shader = ST_Pixel;
		RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
	}

	{
		BearPipelineGraphicsDescription PipelineDescription;

		PipelineDescription.RenderPass = GRenderTarget->RenderPass_Base;
		{
			PipelineDescription.DepthStencilState.DepthEnable = true;
			PipelineDescription.DepthStencilState.StencillEnable = true;
			PipelineDescription.DepthStencilState.FrontFace.StencilPassOp = SO_REPLACE;
			PipelineDescription.DepthStencilState.FrontFace.StencilTest = CF_ALWAYS;
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
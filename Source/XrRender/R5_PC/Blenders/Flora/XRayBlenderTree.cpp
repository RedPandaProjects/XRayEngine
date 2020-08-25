#include "pch.h"

XRayBlenderTree::XRayBlenderTree()
{
	Description.ID = B_TREE;
	Description.Version = 1;
	oBlend.value = FALSE;
	oNotAnTree.value = FALSE;
}

XRayBlenderTree::~XRayBlenderTree()
{
}

LPCSTR XRayBlenderTree::getComment()
{
	return LPCSTR();
}

void XRayBlenderTree::Save(IWriter & fs)
{
}

void XRayBlenderTree::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	xrPREAD_PROP(fs, xrPID_BOOL, oBlend);
	if (version >= 1) {
		xrPREAD_PROP(fs, xrPID_BOOL, oNotAnTree);
	}
}

void XRayBlenderTree::InitializeGraphics()
{
	{
		BearRootSignatureDescription RootSignatureDescription;
		RootSignatureDescription.Samplers[0].Shader = BearShaderType::Pixel;
		RootSignatureDescription.SRVResources[0].Shader = BearShaderType::Pixel;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = BearShaderType::Vertex;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Tree)].Shader = BearShaderType::Vertex;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform)].Shader = BearShaderType::ALL;
		if (!oNotAnTree.value)
		{
			RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_AlphaRef)].Shader = BearShaderType::Pixel;
		}
		RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
		RootSignature[0][1] = RootSignature[0][0];
		RootSignature[1][1] = RootSignature[0][0];
		RootSignatureDescription.SRVResources[1].Shader = BearShaderType::Pixel;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_DetailScalar)].Shader = BearShaderType::Vertex;
		RootSignature[1][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);

	}
	{
		BearPipelineGraphicsDescription PipelineDescription;
		PipelineDescription.DepthStencilState.DepthEnable = true;
		PipelineDescription.DepthStencilState.StencillEnable = true;
		PipelineDescription.DepthStencilState.FrontFace.StencilPassOp = BearStencilOp::Replace;
		PipelineDescription.DepthStencilState.FrontFace.StencilTest =  BearCompareFunction::Always;
		PipelineDescription.RenderPass = GRenderTarget->RenderPass_Base;


		PipelineDescription.BlendState.RenderTarget[0].Enable = true;

		if (oNotAnTree.value)
		{
			CreatePipeline(0, SE_NORMAL_HQ, PipelineDescription, "flora\\tree", "level\\vertex\\vertex", SVD_MUModel);
			CreatePipeline(1, SE_NORMAL_HQ, PipelineDescription, "flora\\tree_dt", "level\\vertex\\vertex_dt", SVD_MUModel);

			CreatePipeline(0, SE_NORMAL_LQ, PipelineDescription, "flora\\tree", "level\\vertex\\vertex", SVD_MUModel);
			CreatePipeline(1, SE_NORMAL_LQ, PipelineDescription, "flora\\tree", "level\\vertex\\vertex", SVD_MUModel);
		}
		else
		{
		///	PipelineDescription.RasterizerState.RasterizerDiscard = true;
			CreatePipeline(0, SE_NORMAL_HQ, PipelineDescription, "flora\\tree_w", "level\\vertex\\vertex_aref", SVD_MUModel);
			CreatePipeline(1, SE_NORMAL_HQ, PipelineDescription, "flora\\tree_w_dt", "level\\vertex\\vertex_dt_aref", SVD_MUModel);

			CreatePipeline(0, SE_NORMAL_LQ, PipelineDescription, "flora\\tree", "level\\vertex\\vertex_aref", SVD_MUModel);
			CreatePipeline(1, SE_NORMAL_LQ, PipelineDescription, "flora\\tree", "level\\vertex\\vertex_aref", SVD_MUModel);
		}
		
	}

}

void XRayBlenderTree::Compile(XRayShaderElement& shader)
{
	if (IDSlot == 0)
	{
		switch (IDShader)
		{
		case SE_NORMAL_HQ:
		{
			
			SetTexture(shader, 0, "$base0");
			if (bDetail)
			{
				SetTexture(shader, 1, DetailName);
				shader.DetailScalar = DetailScalar;
			}
			shader.SamplerStates[0] = SSS_Default;
			shader.TypeTransformation = STT_Matrix;
			shader.UseGlobalUniform = true;
			if (!oNotAnTree.value)
			{
				shader.AlphaRef = 200;
			}
			shader.StencilRef = 1;
		}
		break;
		case SE_NORMAL_LQ:
		{
			SetTexture(shader, 0, "$base0");
			shader.SamplerStates[0] = SSS_Default;
			shader.TypeTransformation = STT_Matrix;
			shader.UseGlobalUniform = true;
			if (!oNotAnTree.value)
			{
				shader.AlphaRef = 200;
			}
			shader.StencilRef = 1;
		}

		break;
		default:
			break;
		}
	}

	
}

int XRayBlenderTree::GetSlotPipelineForShader(size_t slot)
{
	if (slot == 0)
	{
		if (bDetail)
			return 1;
		return 0;
	}
	return -1;
}


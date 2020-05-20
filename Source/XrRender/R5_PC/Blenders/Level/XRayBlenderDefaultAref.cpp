#include "pch.h"

XRayBlenderDefaultAref::XRayBlenderDefaultAref(bool vertex) :LVertex(vertex)
{
	Description.ID = B_DEFAULT_AREF;
	Description.Version = 1;
	oAREF.value = 32;
	oAREF.min = 0;
	oAREF.max = 255;
	oBlend.value = FALSE;
}

XRayBlenderDefaultAref::~XRayBlenderDefaultAref()
{
}

LPCSTR XRayBlenderDefaultAref::getComment()
{
	return LPCSTR();
}

void XRayBlenderDefaultAref::Save(IWriter & fs)
{
}

void XRayBlenderDefaultAref::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);

	switch (version)
	{
	case 0:
		xrPREAD_PROP(fs, xrPID_INTEGER, oAREF);
		oBlend.value = FALSE;
		break;
	case 1:
	default:
		xrPREAD_PROP(fs, xrPID_INTEGER, oAREF);
		xrPREAD_PROP(fs, xrPID_BOOL, oBlend);
		break;
	}
}

void XRayBlenderDefaultAref::InitializeGraphics()
{
	{
		BearRootSignatureDescription RootSignatureDescription;
		RootSignatureDescription.Samplers[0].Shader = ST_Pixel;
		RootSignatureDescription.SRVResources[0].Shader = ST_Pixel;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = ST_Vertex;
		if (!LVertex)
		{
			RootSignatureDescription.SRVResources[1].Shader = ST_Pixel;
			RootSignatureDescription.SRVResources[2].Shader = ST_Pixel;
		}
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform)].Shader = ST_ALL;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_AlphaRef)].Shader = ST_Pixel;
		RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
		RootSignature[0][1] = RootSignature[0][0];
		RootSignature[1][1] = RootSignature[0][0];
		if (!LVertex)
		{
			RootSignatureDescription.SRVResources[3].Shader = ST_Pixel;
		}
		else
		{
			RootSignatureDescription.SRVResources[1].Shader = ST_Pixel;
		}
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_DetailScalar)].Shader = ST_Vertex;
		RootSignature[1][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
	}

	BearPipelineGraphicsDescription PipelineDescription;
	PipelineDescription.DepthStencilState.DepthEnable = true;
	PipelineDescription.DepthStencilState.StencillEnable = true;
	PipelineDescription.DepthStencilState.FrontFace.StencilPassOp = SO_REPLACE;
	PipelineDescription.DepthStencilState.FrontFace.StencilTest = CF_ALWAYS;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Base;
	if (oBlend.value)
	{
		PipelineDescription.RenderPass = GRenderTarget->RenderPass_Generic;
		PipelineDescription.BlendState.RenderTarget[0].Enable = true;
		PipelineDescription.BlendState.RenderTarget[0].ColorSrc = BF_SRC_ALPHA;
		PipelineDescription.BlendState.RenderTarget[0].ColorDst = BF_INV_SRC_ALPHA;
	}

	if (LVertex)
	{
		CreatePipeline(0, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("level\\vertex\\vertex",false, oBlend.value), GetPixelShaderName("level\\vertex\\vertex", false,true, oBlend.value), SVD_R1Vert);
		CreatePipeline(1, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("level\\vertex\\vertex", true, oBlend.value), GetPixelShaderName("level\\vertex\\vertex", true, true, oBlend.value), SVD_R1Vert);

		CreatePipeline(0, SE_NORMAL_LQ, PipelineDescription,GetVertexShaderName("level\\vertex\\vertex", false, oBlend.value), GetPixelShaderName("level\\vertex\\vertex",false,true, oBlend.value), SVD_R1Vert);
		CreatePipeline(1, SE_NORMAL_LQ, PipelineDescription, GetVertexShaderName("level\\vertex\\vertex", false, oBlend.value), GetPixelShaderName("level\\vertex\\vertex", false, true, oBlend.value),  SVD_R1Vert);
	}
	else
	{
		CreatePipeline(0, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmap", false, oBlend.value), GetPixelShaderName("level\\lmap\\lmap", false, true, oBlend.value), SVD_R1LMap);
		CreatePipeline(1, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmap", true, oBlend.value), GetPixelShaderName("level\\lmap\\lmap", true, true, oBlend.value), SVD_R1LMap);

		CreatePipeline(0, SE_NORMAL_LQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmap", false, oBlend.value), GetPixelShaderName("level\\lmap\\lmap", false, true, oBlend.value), SVD_R1LMap);
		CreatePipeline(1, SE_NORMAL_LQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmap",false, oBlend.value), GetPixelShaderName("level\\lmap\\lmap", false, true, oBlend.value), SVD_R1LMap);
	}
}

void XRayBlenderDefaultAref::InitializeMesh()
{
	{
		BearRootSignatureDescription RootSignatureDescription;

		RootSignatureDescription.SRVResources[MSS_Vertices].Shader = ST_Mesh;
		RootSignatureDescription.SRVResources[MSS_Vertices].DescriptorType = DT_Buffer;
		RootSignatureDescription.SRVResources[MSS_Meshlets].Shader = ST_Mesh;
		RootSignatureDescription.SRVResources[MSS_Meshlets].DescriptorType = DT_Buffer;
		RootSignatureDescription.SRVResources[MSS_UniqueVertexIndices].Shader = ST_Mesh;
		RootSignatureDescription.SRVResources[MSS_UniqueVertexIndices].DescriptorType = DT_Buffer;
		RootSignatureDescription.SRVResources[MSS_PrimitiveIndices].Shader = ST_Mesh;
		RootSignatureDescription.SRVResources[MSS_PrimitiveIndices].DescriptorType = DT_Buffer;

		RootSignatureDescription.Samplers[0].Shader = ST_Pixel;
		RootSignatureDescription.SRVResources[0].Shader = ST_Pixel;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = ST_Mesh;
		if (!LVertex)
		{
			RootSignatureDescription.SRVResources[1].Shader = ST_Pixel;
			RootSignatureDescription.SRVResources[2].Shader = ST_Pixel;
		}
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform)].Shader = ST_ALL;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_AlphaRef)].Shader = ST_Pixel;
		RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
		RootSignature[0][1] = RootSignature[0][0];
		RootSignature[1][1] = RootSignature[0][0];
		if (!LVertex)
		{
			RootSignatureDescription.SRVResources[3].Shader = ST_Pixel;
		}
		else
		{
			RootSignatureDescription.SRVResources[1].Shader = ST_Pixel;
		}
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_DetailScalar)].Shader = ST_Mesh;
		RootSignature[1][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
	}

	BearPipelineMeshDescription PipelineDescription;
	PipelineDescription.DepthStencilState.DepthEnable = true;
	PipelineDescription.DepthStencilState.StencillEnable = true;
	PipelineDescription.DepthStencilState.FrontFace.StencilPassOp = SO_REPLACE;
	PipelineDescription.DepthStencilState.FrontFace.StencilTest = CF_ALWAYS;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Base;
	if (oBlend.value)
	{
		PipelineDescription.RenderPass = GRenderTarget->RenderPass_Generic;
		PipelineDescription.BlendState.RenderTarget[0].Enable = true;
		PipelineDescription.BlendState.RenderTarget[0].ColorSrc = BF_SRC_ALPHA;
		PipelineDescription.BlendState.RenderTarget[0].ColorDst = BF_INV_SRC_ALPHA;
	}

	if (LVertex)
	{
		CreatePipeline(0, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("level\\vertex\\vertex", false, oBlend.value), GetPixelShaderName("level\\vertex\\vertex", false, true, oBlend.value), SVD_R1Vert);
		CreatePipeline(1, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("level\\vertex\\vertex", true, oBlend.value), GetPixelShaderName("level\\vertex\\vertex", true, true, oBlend.value), SVD_R1Vert);

		CreatePipeline(0, SE_NORMAL_LQ, PipelineDescription, GetVertexShaderName("level\\vertex\\vertex", false, oBlend.value), GetPixelShaderName("level\\vertex\\vertex", false, true, oBlend.value), SVD_R1Vert);
		CreatePipeline(1, SE_NORMAL_LQ, PipelineDescription, GetVertexShaderName("level\\vertex\\vertex", false, oBlend.value), GetPixelShaderName("level\\vertex\\vertex", false, true, oBlend.value), SVD_R1Vert);
	}
	else
	{
		CreatePipeline(0, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmap", false, oBlend.value), GetPixelShaderName("level\\lmap\\lmap", false, true, oBlend.value), SVD_R1LMap);
		CreatePipeline(1, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmap", true, oBlend.value), GetPixelShaderName("level\\lmap\\lmap", true, true, oBlend.value), SVD_R1LMap);

		CreatePipeline(0, SE_NORMAL_LQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmap", false, oBlend.value), GetPixelShaderName("level\\lmap\\lmap", false, true, oBlend.value), SVD_R1LMap);
		CreatePipeline(1, SE_NORMAL_LQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmap", false, oBlend.value), GetPixelShaderName("level\\lmap\\lmap", false, true, oBlend.value), SVD_R1LMap);
	}
}

void XRayBlenderDefaultAref::Compile(XRayShaderElement& shader)
{
	if (IDSlot == 0)
	{
		switch (IDShader)
		{
		case SE_NORMAL_HQ:
		{

			SetTexture(shader, 0, "$base0");
			if (!LVertex)
			{
				SetTexture(shader, 1, "$base1");
				SetTexture(shader, 2, "$base2");
				if (bDetail)
				{
					SetTexture(shader, 3, DetailName);
					shader.DetailScalar = DetailScalar;
				}
			}
			else
			{
				if (bDetail)
				{
					SetTexture(shader, 1, DetailName);
					shader.DetailScalar = DetailScalar;
				}
			}
			shader.SamplerStates[0] = SSS_Default;
			shader.TypeTransformation = STT_Matrix;
			shader.UseGlobalUniform = true;
			shader.AlphaRef = oAREF.value;
			shader.Sort = oBlend.value;
			shader.StencilRef = 1;
		}
		break;
		case SE_NORMAL_LQ:
		{
			SetTexture(shader, 0, "$base0");
			if (!LVertex)
			{
				SetTexture(shader, 1, "$base1");
				SetTexture(shader, 2, "$base2");
			}
			shader.SamplerStates[0] = SSS_Default;
			shader.TypeTransformation = STT_Matrix;
			shader.UseGlobalUniform = true;
			shader.AlphaRef = oAREF.value;
			shader.Sort = oBlend.value;
			shader.StencilRef = 1;
		}

		break;
		default:
			break;
		}
	}
}

int XRayBlenderDefaultAref::GetSlotPipelineForShader(size_t slot)
{
	if (slot == 0)
	{
		if (bDetail)
			return 1;
		return 0;
	}
	return -1;
}

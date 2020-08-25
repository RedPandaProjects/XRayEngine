#include "pch.h"

XRayBlenderDefault::XRayBlenderDefault(bool vertex):LVertex(vertex)
{
	Description.ID = B_DEFAULT;
	Description.Version = 1;
	oTessellation.Count = 4;
	oTessellation.IDselected = 0;
}

XRayBlenderDefault::~XRayBlenderDefault()
{
}

LPCSTR XRayBlenderDefault::getComment()
{
	return LPCSTR();
}

void XRayBlenderDefault::Save(IWriter & fs)
{
}

void XRayBlenderDefault::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	if (version > 0)
	{
		xrPREAD_PROP(fs, xrPID_TOKEN, oTessellation);
	}
}

void XRayBlenderDefault::InitializeGraphics()
{
	{
		BearRootSignatureDescription RootSignatureDescription;
		RootSignatureDescription.Samplers[0].Shader = BearShaderType::Pixel;
		RootSignatureDescription.SRVResources[0].Shader = BearShaderType::Pixel;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = BearShaderType::Vertex;
		if (!LVertex)
		{
			RootSignatureDescription.SRVResources[1].Shader = BearShaderType::Pixel;
			RootSignatureDescription.SRVResources[2].Shader = BearShaderType::Pixel;
		}
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform)].Shader = BearShaderType::ALL;
		RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
		RootSignature[0][1] = RootSignature[0][0];
		RootSignature[1][1] = RootSignature[0][0];
		if (!LVertex)
		{
			RootSignatureDescription.SRVResources[3].Shader = BearShaderType::Pixel;
		}
		else
		{
			RootSignatureDescription.SRVResources[1].Shader = BearShaderType::Pixel;
		}
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_DetailScalar)].Shader = BearShaderType::Vertex;
		RootSignature[1][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
	}

	BearPipelineGraphicsDescription PipelineDescription;
	PipelineDescription.DepthStencilState.DepthEnable = true;
	PipelineDescription.DepthStencilState.StencillEnable = true;
	PipelineDescription.DepthStencilState.FrontFace.StencilPassOp = BearStencilOp::Replace;
	PipelineDescription.DepthStencilState.FrontFace.StencilTest =  BearCompareFunction::Always;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Base;

	if (LVertex)
	{
		CreatePipeline(0, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("level\\vertex\\vertex", false), GetPixelShaderName("level\\vertex\\vertex", false, false, false), SVD_R1Vert);
		CreatePipeline(1, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("level\\vertex\\vertex", true), GetPixelShaderName("level\\vertex\\vertex", true, false, false), SVD_R1Vert);

		CreatePipeline(0, SE_NORMAL_LQ, PipelineDescription, GetVertexShaderName("level\\vertex\\vertex", false), GetPixelShaderName("level\\vertex\\vertex", false, false, false), SVD_R1Vert);
		CreatePipeline(1, SE_NORMAL_LQ, PipelineDescription, GetVertexShaderName("level\\vertex\\vertex", false), GetPixelShaderName("level\\vertex\\vertex", false, false, false), SVD_R1Vert);
	}
	else
	{
		CreatePipeline(0, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmap", false), GetPixelShaderName("level\\lmap\\lmap", false, false, false), SVD_R1LMap);
		CreatePipeline(1, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmap", true), GetPixelShaderName("level\\lmap\\lmap", true, false, false), SVD_R1LMap);

		CreatePipeline(0, SE_NORMAL_LQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmap", false), GetPixelShaderName("level\\lmap\\lmap", false, false, false), SVD_R1LMap);
		CreatePipeline(1, SE_NORMAL_LQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmap", false), GetPixelShaderName("level\\lmap\\lmap", false, false, false), SVD_R1LMap);
	}
}

void XRayBlenderDefault::InitializeMesh()
{
	{
		BearRootSignatureDescription RootSignatureDescription;


		RootSignatureDescription.SRVResources[MSS_Vertices].Shader = BearShaderType::Mesh;
		RootSignatureDescription.SRVResources[MSS_Vertices].DescriptorType = BearSRVDescriptorType::Buffer;
		RootSignatureDescription.SRVResources[MSS_Meshlets].Shader = BearShaderType::Mesh;
		RootSignatureDescription.SRVResources[MSS_Meshlets].DescriptorType = BearSRVDescriptorType::Buffer;
		RootSignatureDescription.SRVResources[MSS_UniqueVertexIndices].Shader = BearShaderType::Mesh;
		RootSignatureDescription.SRVResources[MSS_UniqueVertexIndices].DescriptorType = BearSRVDescriptorType::Buffer;
		RootSignatureDescription.SRVResources[MSS_PrimitiveIndices].Shader = BearShaderType::Mesh;
		RootSignatureDescription.SRVResources[MSS_PrimitiveIndices].DescriptorType = BearSRVDescriptorType::Buffer;

		RootSignatureDescription.Samplers[0].Shader = BearShaderType::Pixel;
		RootSignatureDescription.SRVResources[0].Shader = BearShaderType::Pixel;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = BearShaderType::Mesh;
		if (!LVertex)
		{
			RootSignatureDescription.SRVResources[1].Shader = BearShaderType::Pixel;
			RootSignatureDescription.SRVResources[2].Shader = BearShaderType::Pixel;
		}
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform)].Shader = BearShaderType::ALL;
		RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
		RootSignature[0][1] = RootSignature[0][0];
		RootSignature[1][1] = RootSignature[0][0];
		if (!LVertex)
		{
			RootSignatureDescription.SRVResources[3].Shader = BearShaderType::Pixel;
		}
		else
		{
			RootSignatureDescription.SRVResources[1].Shader = BearShaderType::Pixel;
		}
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_DetailScalar)].Shader = BearShaderType::Mesh;
		RootSignature[1][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
	}

	BearPipelineMeshDescription PipelineDescription;
	PipelineDescription.DepthStencilState.DepthEnable = true;
	PipelineDescription.DepthStencilState.StencillEnable = true;
	PipelineDescription.DepthStencilState.FrontFace.StencilPassOp = BearStencilOp::Replace;
	PipelineDescription.DepthStencilState.FrontFace.StencilTest =  BearCompareFunction::Always;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Base;

	if (LVertex)
	{
		CreatePipeline(0, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("level\\vertex\\vertex", false), GetPixelShaderName("level\\vertex\\vertex", false, false, false), SVD_R1Vert);
		CreatePipeline(1, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("level\\vertex\\vertex", true), GetPixelShaderName("level\\vertex\\vertex", true, false, false), SVD_R1Vert);

		CreatePipeline(0, SE_NORMAL_LQ, PipelineDescription, GetVertexShaderName("level\\vertex\\vertex", false), GetPixelShaderName("level\\vertex\\vertex", false, false, false), SVD_R1Vert);
		CreatePipeline(1, SE_NORMAL_LQ, PipelineDescription, GetVertexShaderName("level\\vertex\\vertex", false), GetPixelShaderName("level\\vertex\\vertex", false, false, false), SVD_R1Vert);
	}
	else
	{
		CreatePipeline(0, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmap", false), GetPixelShaderName("level\\lmap\\lmap", false, false, false), SVD_R1LMap);
		CreatePipeline(1, SE_NORMAL_HQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmap", true), GetPixelShaderName("level\\lmap\\lmap", true, false, false), SVD_R1LMap);

		CreatePipeline(0, SE_NORMAL_LQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmap", false), GetPixelShaderName("level\\lmap\\lmap", false, false, false), SVD_R1LMap);
		CreatePipeline(1, SE_NORMAL_LQ, PipelineDescription, GetVertexShaderName("level\\lmap\\lmap", false), GetPixelShaderName("level\\lmap\\lmap", false, false, false), SVD_R1LMap);
	}
}

void XRayBlenderDefault::Compile(XRayShaderElement& shader)
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
			shader.StencilRef = 1;
		}

		break;
		default:
			break;
		}
	}
}
int XRayBlenderDefault::GetSlotPipelineForShader(size_t slot)
{
	if (slot == 0)
	{
		if (bDetail)
			return 1;
		return 0;
	}
	return -1;
}

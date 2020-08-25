#include "pch.h"

XRayBlenderBmmD::XRayBlenderBmmD()
{
	Description.ID = B_BmmD;
	Description.Version = 3;

	xr_strcpy(oT2_Name, TEXT("$null"));
	xr_strcpy(oT2_xform, TEXT("$null"));
	xr_strcpy(oR_Name, TEXT("detail\\detail_grnd_grass"));	//"$null");
	xr_strcpy(oG_Name, TEXT("detail\\detail_grnd_asphalt"));	//"$null");
	xr_strcpy(oB_Name, TEXT("detail\\detail_grnd_earth"));	//"$null");
	xr_strcpy(oA_Name, TEXT("detail\\detail_grnd_yantar"));	//"$null");
}

XRayBlenderBmmD::~XRayBlenderBmmD()
{
}

LPCSTR XRayBlenderBmmD::getComment()
{
	return LPCSTR();
}

void XRayBlenderBmmD::Save(IWriter & fs)
{
}

void XRayBlenderBmmD::Load(IReader & fs, u16 version)
{
	XRayBlenderCompiler::Load(fs, version);
	if (version < 3) {
		xrPREAD_MARKER(fs);
		xrPREAD_PROP(fs, xrPID_TEXTURE, oT2_Name);
		xrPREAD_PROP(fs, xrPID_MATRIX, oT2_xform);
	}
	else {
		xrPREAD_MARKER(fs);
		xrPREAD_PROP(fs, xrPID_TEXTURE, oT2_Name);
		xrPREAD_PROP(fs, xrPID_MATRIX, oT2_xform);
		xrPREAD_PROP(fs, xrPID_TEXTURE, oR_Name);
		xrPREAD_PROP(fs, xrPID_TEXTURE, oG_Name);
		xrPREAD_PROP(fs, xrPID_TEXTURE, oB_Name);
		xrPREAD_PROP(fs, xrPID_TEXTURE, oA_Name);
	}
}

void XRayBlenderBmmD::InitializeGraphics()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.Samplers[0].Shader = BearShaderType::Pixel;

	RootSignatureDescription.SRVResources[0].Shader = BearShaderType::Pixel;
	RootSignatureDescription.SRVResources[1].Shader = BearShaderType::Pixel;
	RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = BearShaderType::Vertex;
	RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_DetailScalar)].Shader = BearShaderType::Vertex;
	RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform)].Shader = BearShaderType::ALL;

	RootSignature[0][1] = GResourcesManager->CreateRootSignature(RootSignatureDescription);

	RootSignatureDescription.SRVResources[2].Shader = BearShaderType::Pixel;
	RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
/*	RootSignatureDescription.SRVResources[3].Shader = BearShaderType::Pixel;
	RootSignatureDescription.SRVResources[4].Shader = BearShaderType::Pixel;
	RootSignatureDescription.SRVResources[5].Shader = BearShaderType::Pixel;
	RootSignatureDescription.SRVResources[6].Shader = BearShaderType::Pixel;
	RootSignatureDescription.SRVResources[7].Shader = BearShaderType::Pixel*/
		;



	BearPipelineGraphicsDescription PipelineDescription;
	PipelineDescription.DepthStencilState.DepthEnable = true;
	PipelineDescription.DepthStencilState.StencillEnable = true;
	PipelineDescription.DepthStencilState.FrontFace.StencilPassOp = BearStencilOp::Replace;
	PipelineDescription.DepthStencilState.FrontFace.StencilTest =  BearCompareFunction::Always;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Base;

	CreatePipeline(0, 0, PipelineDescription, "level\\terrain", "level\\terrain_hq", SVD_R1LMap);
	CreatePipeline(0, 1, PipelineDescription, "level\\terrain", "level\\terrain_lq", SVD_R1LMap);
}

void XRayBlenderBmmD::InitializeMesh()
{
	{
		BearRootSignatureDescription RootSignatureDescription;
		RootSignatureDescription.Samplers[0].Shader = BearShaderType::Pixel;

		RootSignatureDescription.SRVResources[0].Shader = BearShaderType::Pixel;
		RootSignatureDescription.SRVResources[1].Shader = BearShaderType::Pixel;

		RootSignatureDescription.SRVResources[MSS_Vertices].Shader = BearShaderType::Mesh;
		RootSignatureDescription.SRVResources[MSS_Vertices].DescriptorType = BearSRVDescriptorType::Buffer;
		RootSignatureDescription.SRVResources[MSS_Meshlets].Shader = BearShaderType::Mesh;
		RootSignatureDescription.SRVResources[MSS_Meshlets].DescriptorType = BearSRVDescriptorType::Buffer;
		RootSignatureDescription.SRVResources[MSS_UniqueVertexIndices].Shader = BearShaderType::Mesh;
		RootSignatureDescription.SRVResources[MSS_UniqueVertexIndices].DescriptorType = BearSRVDescriptorType::Buffer;
		RootSignatureDescription.SRVResources[MSS_PrimitiveIndices].Shader = BearShaderType::Mesh;
		RootSignatureDescription.SRVResources[MSS_PrimitiveIndices].DescriptorType = BearSRVDescriptorType::Buffer;

		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = BearShaderType::Mesh;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_DetailScalar)].Shader = BearShaderType::Mesh;
		RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform)].Shader = BearShaderType::ALL;


		RootSignature[0][1] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
		RootSignatureDescription.SRVResources[2].Shader = BearShaderType::Pixel;
		RootSignature[0][0] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
	}
	{
		BearPipelineMeshDescription PipelineDescription;
		PipelineDescription.DepthStencilState.DepthEnable = true;
		PipelineDescription.DepthStencilState.StencillEnable = true;
		PipelineDescription.DepthStencilState.FrontFace.StencilPassOp = BearStencilOp::Replace;
		PipelineDescription.DepthStencilState.FrontFace.StencilTest =  BearCompareFunction::Always;
		PipelineDescription.RenderPass = GRenderTarget->RenderPass_Base;

		CreatePipeline(0, 0, PipelineDescription, "level\\terrain", "level\\terrain_hq", SVD_R1LMap);
		CreatePipeline(0, 1, PipelineDescription, "level\\terrain", "level\\terrain_lq", SVD_R1LMap);
	}
}

void XRayBlenderBmmD::Compile(XRayShaderElement& shader)
{
	if (IDSlot == 0)
	{
		switch (IDShader)
		{

		case SE_NORMAL_HQ:
		{
			R_ASSERT(bDetail);
			SetTexture(shader, 0, oTName);
			SetTexture(shader, 1, "$base1");
			SetTexture(shader, 2, oT2_Name);
			/*	SetTexture(shader, 3, oTName,"_mask");
				SetTexture(shader, 4, oR_Name);
				SetTexture(shader, 5, oG_Name);
				SetTexture(shader, 6, oB_Name);
				SetTexture(shader, 7, oA_Name);*/
			shader.SamplerStates[0] = SSS_Default;
			shader.TypeTransformation = STT_Matrix;
			shader.UseGlobalUniform = true;
			shader.DetailScalar = DetailScalar;
			shader.StencilRef = 1;
		}
		break;
		case SE_NORMAL_LQ:
		{
			R_ASSERT(bDetail);
			SetTexture(shader, 0, oTName);
			SetTexture(shader, 1, "$base1");
			shader.SamplerStates[0] = SSS_Default;
			shader.TypeTransformation = STT_Matrix;
			shader.UseGlobalUniform = true;
			shader.DetailScalar = DetailScalar;
			shader.StencilRef = 1;
		}

		break;
		default:
			break;
		}
	}
}


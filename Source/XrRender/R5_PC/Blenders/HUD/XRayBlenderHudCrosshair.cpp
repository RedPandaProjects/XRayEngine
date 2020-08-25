#include "pch.h"
#include "XRayBlenderHudCrosshair.h"

XRayBlenderHudCrosshair::XRayBlenderHudCrosshair()
{
}

XRayBlenderHudCrosshair::~XRayBlenderHudCrosshair()
{
}

void XRayBlenderHudCrosshair::InitializeGraphics()
{
	BearRootSignatureDescription RootSignatureDescription;
	RootSignatureDescription.UniformBuffers[XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation)].Shader = BearShaderType::Vertex;
	RootSignature[0][1] = GResourcesManager->CreateRootSignature(RootSignatureDescription);
	RootSignature[0][2] = RootSignature[0][1];
	RootSignature[0][3] = RootSignature[0][1];

	BearPipelineGraphicsDescription PipelineDescription;
	PipelineDescription.RenderPass = GRenderTarget->RenderPass_Generic;

	PipelineDescription.TopologyType = BearTopologyType::TriangleList;
	CreatePipeline(0, 1, PipelineDescription, "hud\\notransform_tl0uv", "hud\\simple_color", SVD_TL0uv);
	PipelineDescription.TopologyType = BearTopologyType::LintList;
	CreatePipeline(0,2, PipelineDescription, "hud\\notransform_tl0uv", "hud\\simple_color", SVD_TL0uv);
	PipelineDescription.TopologyType = BearTopologyType::LineStrip;
	CreatePipeline(0,3, PipelineDescription, "hud\\notransform_tl0uv", "hud\\simple_color", SVD_TL0uv);
}

void XRayBlenderHudCrosshair::Compile(XRayShaderElement& shader)
{
	shader.TypeTransformation = STT_Screen;
}
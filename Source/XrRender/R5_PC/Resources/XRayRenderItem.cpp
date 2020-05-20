#include "pch.h"

XRayObjectRender::~XRayObjectRender()
{
}


void XRayObjectRender::Copy(const XRayObjectRender& right)
{
	for (size_t i = 0; i < XRayUniformAllocator::UT_Count; i++) { UseUniform[i] = right.UseUniform[i]; }
	for (size_t i = 0; i < XRayUniformAllocator::UT_Count; i++) { UniformID[i] = right.UniformID[i]; }

	UseMWorld = right.UseMWorld;
	MWorld = right.MWorld;
	Visual = right.Visual;
	UseGlobalUniform = right.UseGlobalUniform;
	UseLocalUniform = right.UseLocalUniform;
	LocalUniform = right.LocalUniform;

	Pipeline = right.Pipeline;
	DescriptorHeap = right.DescriptorHeap;
	DetailScalar = right.DetailScalar;
	AlphaRef = right.AlphaRef;
	Sort = right.Sort;
	StencilRef = right.StencilRef;

	UseMeshPipeline = right.UseMeshPipeline;
	if (UseMeshPipeline)
	{
		MeshPipelineResource = right.MeshPipelineResource;
	}
	else
	{
		GraphicsPipelineResource = right.GraphicsPipelineResource;
	}

}

void XRayObjectRender::Swap(XRayObjectRender& right)
{
	for (size_t i = 0; i < XRayUniformAllocator::UT_Count; i++) { std::swap(UseUniform[i] , right.UseUniform[i]); }
	for (size_t i = 0; i < XRayUniformAllocator::UT_Count; i++) { std::swap(UniformID[i] , right.UniformID[i]); }
	std::swap(AlphaRef, right.AlphaRef);
	std::swap(DetailScalar , right.DetailScalar);
	std::swap(UseGlobalUniform, right.UseGlobalUniform);
	std::swap(UseLocalUniform, right.UseLocalUniform);
	std::swap(LocalUniform, right.LocalUniform);
	std::swap(UseMWorld, right.UseMWorld);
	std::swap(MWorld,  right.MWorld);
	std::swap(Visual , right.Visual);
	std::swap(StencilRef, right.StencilRef);
	std::swap(Sort,right.Sort);
	Pipeline .swap(right.Pipeline);
	DescriptorHeap.swap(right.DescriptorHeap);
	std::swap(UseMeshPipeline, right.UseMeshPipeline);
	std::swap(MeshPipelineResource, right.MeshPipelineResource);
	std::swap(GraphicsPipelineResource, right.GraphicsPipelineResource);
}

#pragma once
class XRayRenderVisual;
class  XRayObjectRender
{
public:
	XRayObjectRender() :UseMWorld(false), DetailScalar(0), AlphaRef(-1), StencilRef(-1), Sort(false), UseMeshPipeline(false) { for (size_t i = 0; i < XRayUniformAllocator::UT_Count; i++) { UseUniform[i] = false; } GraphicsPipelineResource = {}; MeshPipelineResource = {}; }
	 ~XRayObjectRender();
	 void Copy(const XRayObjectRender& right);
	 void Swap(XRayObjectRender& right);

	 XRayObjectRender(const XRayObjectRender& right)  { for (size_t i = 0; i < XRayUniformAllocator::UT_Count; i++) { UseUniform[i] = false; }  Copy(right); }
	 XRayObjectRender(XRayObjectRender&& right)  { for (size_t i = 0; i < XRayUniformAllocator::UT_Count; i++) { UseUniform[i] = false; }	Swap(right); }
	 XRayObjectRender& operator=(const XRayObjectRender& right) { Copy(right); return*this; }
	 XRayObjectRender& operator=(XRayObjectRender&& right) { Swap(right); return*this; }
	 
	 bool UseUniform[XRayUniformAllocator::UT_Count];
	 size_t UniformID[XRayUniformAllocator::UT_Count];
	 bool UseMWorld;
	 Fmatrix MWorld;
	 XRayRenderVisual *Visual;

	 BearFactoryPointer<BearRHI::BearRHIPipeline> Pipeline;
	 BearFactoryPointer<BearRHI::BearRHIDescriptorHeap> DescriptorHeap;

	 bool UseGlobalUniform;
	 bool UseLocalUniform;
	 size_t DetailScalar;
	 int AlphaRef;
	 int StencilRef;
	 bool Sort;

	 XRayLocalUniform LocalUniform;


	 struct SGraphicsPipelineResource
	 {
		 BearFactoryPointer<BearRHI::BearRHIIndexBuffer> IndexBuffer;
		 BearFactoryPointer<BearRHI::BearRHIVertexBuffer> VertexBuffer;
		 size_t CountIndex;
		 size_t OffsetIndex;
		 size_t OffsetVertex;
	 }GraphicsPipelineResource;
	 struct SMeshPipelineResource
	 {

		 BearFactoryPointer<BearRHI::BearRHIStructuredBuffer> VertexBufferAsStructured;
		 BearFactoryPointer<BearRHI::BearRHIStructuredBuffer> MeshBuffer;
		 size_t CountMeshlet;
		 size_t OffsetMeshlet;
		 size_t OffsetUniqueVertexIndices;
		 size_t OffsetPrimitiveIndices;

	 }MeshPipelineResource;



	 bool UseMeshPipeline;
	 inline  void Render(BearFactoryPointer<BearRHI::BearRHIContext>& context)
	 {
		 context->SetPipeline(Pipeline);
		 if (StencilRef >= 0)
			 context->SetStencilRef(static_cast<u32>(StencilRef));
		 if (UseMeshPipeline)
		 {
			 DescriptorHeap->SetShaderResource(MSS_Vertices, MeshPipelineResource.VertexBufferAsStructured);
			 DescriptorHeap->SetShaderResource(MSS_Meshlets, MeshPipelineResource.MeshBuffer, MeshPipelineResource.OffsetMeshlet);
			 DescriptorHeap->SetShaderResource(MSS_UniqueVertexIndices, MeshPipelineResource.MeshBuffer, MeshPipelineResource.OffsetUniqueVertexIndices);
			 DescriptorHeap->SetShaderResource(MSS_PrimitiveIndices, MeshPipelineResource.MeshBuffer, MeshPipelineResource.OffsetPrimitiveIndices);
			 context->SetDescriptorHeap(DescriptorHeap);
			 context->DispatchMesh(MeshPipelineResource.CountMeshlet, 1, 1);
		 }
		 else
		 {
			context->SetDescriptorHeap(DescriptorHeap);
			 context->SetVertexBuffer(GraphicsPipelineResource.VertexBuffer);
			 context->SetIndexBuffer(GraphicsPipelineResource.IndexBuffer);
			 context->DrawIndex(GraphicsPipelineResource.CountIndex, GraphicsPipelineResource.OffsetIndex, GraphicsPipelineResource.OffsetVertex);
		 }

	 }

};
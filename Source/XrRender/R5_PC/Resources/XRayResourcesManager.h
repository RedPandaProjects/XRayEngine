#pragma once
class XRayResourcesManager
{
public:
	XRayResourcesManager();
	~XRayResourcesManager();
	void LoadShaders(const char* name);
	void CompileBlender(XRayShader& Blender, const char* name, const char* textures);
	size_t GetStride(u32 VertexState);
	BearFactoryPointer<BearRHI::BearRHIShader> GetPixelShader (shared_str name);
	BearFactoryPointer<BearRHI::BearRHIShader> GetVertexShader(shared_str name);
	BearFactoryPointer<BearRHI::BearRHIShader> GetMeshShader(shared_str name);
	BearFactoryPointer<BearRHI::BearRHIPipeline> CreatePipeline(BearPipelineGraphicsDescription& Description);
	BearFactoryPointer<BearRHI::BearRHIPipeline> CreatePipeline(BearPipelineMeshDescription& Description);
	BearFactoryPointer<BearRHI::BearRHIRootSignature> CreateRootSignature(BearRootSignatureDescription& Description);

	XRayTexture* GetTexture(shared_str name);
	void Free(XRayTexture* texture);
	 

	BearFactoryPointer<BearRHI::BearRHISampler> SamplerDefault;
	BearFactoryPointer<BearRHI::BearRHISampler> SamplerNoFilter;
	BearFactoryPointer<BearRHI::BearRHISampler> SamplerPoint;
	BearFactoryPointer<BearRHI::BearRHISampler> SamplerRTLinear;
	BearFactoryPointer<BearRHI::BearRHISampler> SamplerLinear;

	XRayPipelineRef GetPipeline(XRayPipeline& Pipeline);
	/*
	smp_nofilter
	smp_point
	smp_rtlinear
	*/
private:
	void ReplaceBlender(shared_str name, XRayBlenderCompiler* new_blender);
	XRayBlenderCompiler* CreateBlender(CLASS_ID ID);
	xr_map< shared_str, XRayTexture*> m_Textures;
	xr_map<shared_str, XRayBlenderCompiler*> m_Blenders;
	xr_map< shared_str, BearFactoryPointer<BearRHI::BearRHIShader>> m_PShaders;
	xr_map< shared_str, BearFactoryPointer<BearRHI::BearRHIShader>> m_VShaders;
	xr_map< shared_str, BearFactoryPointer<BearRHI::BearRHIShader>> m_MShaders;
	xr_map< BearPipelineGraphicsDescription, BearFactoryPointer<BearRHI::BearRHIPipelineGraphics>> m_PipelinesGraphics;
	xr_map< BearPipelineMeshDescription, BearFactoryPointer<BearRHI::BearRHIPipelineMesh>> m_PipelinesMesh;
	xr_map< BearRootSignatureDescription, BearFactoryPointer<BearRHI::BearRHIRootSignature>> m_RootSignatures;
	xr_vector< XRayPipelineRef> m_PipelinesRef;
};
extern XRayResourcesManager* GResourcesManager;
#pragma once
#include "../../xrEngine/Properties.h"
#pragma pack(push,4)
struct XRayBlenderDescription
{
public:
	CLASS_ID		ID;
	string128	Name;
	string32	Computer;
	u32			Time;
	u16			Version;

	XRayBlenderDescription()
	{
		ID = CLASS_ID(0);
		Name[0] = 0;
		Computer[0] = 0;
		Time = 0;
		Version = 0;
	}
};
class  XRayBlenderCompiler:public CPropertyBase
{
	//BEAR_CLASS_NO_COPY(XRayBlender);
public:
	void Initialize();
	virtual void InitializeGraphics();
	virtual void InitializeMesh();
	virtual LPCSTR getName();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	XRayBlenderCompiler();
	virtual bool UseDetail() { return false; }
	virtual bool UseMeshShader() { return false; }
	virtual  ~XRayBlenderCompiler();
	inline const XRayBlenderDescription&GetDescription()const
	{
		return Description;
	}
	virtual void Compile(XRayShaderElement& shader);
	void Compile(XRayShader& shader,const char*textures=0);
protected:
	virtual int GetSlotPipelineForShader(size_t slot);
	void SetInputLayout(BearPipelineGraphicsDescription& Description, u32 VertexState);
	void SetTexture(XRayShaderElement& shader,size_t id,shared_str name);
	void SetTexture(XRayShaderElement& shader, size_t id, const char*name1,const char*name2);
	void CreatePipeline( size_t slot, size_t id_shader, BearPipelineGraphicsDescription& Description, shared_str name_vs, shared_str name_ps, ShaderVertexDeclaration FVFType);
	void CreatePipeline(size_t slot, size_t id_shader, BearPipelineMeshDescription& Description, shared_str name_ms, shared_str name_ps, ShaderVertexDeclaration FVFType);
	//;
	shared_str GetPixelShaderName(const char* name,bool detail=false,bool aref=false,bool blend=false);
	shared_str GetVertexShaderName(const char* name, bool detail=false, bool blend = false);
protected:
	shared_str GetTexture(shared_str name);
	XRayBlenderDescription			Description;
	xrP_Integer						oPriority;
	xrP_BOOL						oStrictSorting;
	string64					oTName;
	string64					oTXform;
	const char* DetailName;
	size_t		DetailScalar;
	bool        bDetail;
	size_t      IDShader;
	size_t      IDSlot;
	BearFactoryPointer < BearRHI::BearRHIRootSignature> RootSignature[4][5];
private:
	xr_vector<shared_str> m_textures;
private:
	XRayPipeline m_pipelines[4][5];
};
#pragma pack(pop)
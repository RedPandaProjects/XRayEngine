#pragma once
class  XRayBlenderDefault :public XRayBlenderCompiler
{
public:
	XRayBlenderDefault(bool vertex);
	virtual ~XRayBlenderDefault();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void InitializeGraphics();
	virtual void InitializeMesh();
	virtual void Compile(XRayShaderElement& shader);

	virtual int GetSlotPipelineForShader(size_t slot);
	virtual bool UseDetail() { return true; }
	virtual bool UseMeshShader() { return !LVertex; }
protected:
	bool LVertex;
	xrP_TOKEN	oTessellation;
};
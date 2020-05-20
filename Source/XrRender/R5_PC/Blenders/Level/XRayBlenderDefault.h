#pragma once
class  XRayBlenderDefaultAref :public XRayBlenderCompiler
{
public:
	XRayBlenderDefaultAref(bool vertex);
	virtual ~XRayBlenderDefaultAref();
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
	xrP_Integer	oAREF;
	xrP_BOOL	oBlend;
};
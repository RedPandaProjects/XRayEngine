#pragma once
class  XRayBlenderModel :public XRayBlenderCompiler
{
public:
	XRayBlenderModel();
	virtual ~XRayBlenderModel();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);

	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);

	virtual int GetSlotPipelineForShader(size_t slot) { return static_cast<int>(slot); }
protected:
	xrP_Integer	oAREF;
	xrP_BOOL	oBlend;
	xrP_TOKEN	oTessellation;
};
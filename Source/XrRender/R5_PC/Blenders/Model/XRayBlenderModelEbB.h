#pragma once
class  XRayBlenderModelEbB :public XRayBlenderCompiler
{
public:
	XRayBlenderModelEbB();
	virtual ~XRayBlenderModelEbB();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);

	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);

	virtual int GetSlotPipelineForShader(size_t slot) { return static_cast<int>(slot); }
protected:
	string64	oT2_Name;		// name of secondary texture
	string64	oT2_xform;		// xform for secondary texture
	xrP_BOOL	oBlend;
};
#pragma once
class  XRayBlenderLmEbB :public XRayBlenderCompiler
{
public:
	XRayBlenderLmEbB();
	virtual ~XRayBlenderLmEbB();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);
protected:
	string64	oT2_Name;		// name of secondary texture
	string64	oT2_xform;		// xform for secondary texture
	xrP_BOOL	oBlend;
};
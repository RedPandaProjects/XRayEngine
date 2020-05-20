#pragma once
class  XRayBlenderScreenSet :public XRayBlenderCompiler
{
public:
	XRayBlenderScreenSet();
	virtual ~XRayBlenderScreenSet();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);
	//virtual void Compile(XRayShader&shader);
protected:
	xrP_TOKEN	oBlend;
	xrP_Integer	oAREF;
	xrP_BOOL	oZTest;
	xrP_BOOL	oZWrite;
	xrP_BOOL	oLighting;
	xrP_BOOL	oFog;
	xrP_BOOL	oClamp;
private:
};
#pragma once
class  XRayBlenderParticle :public XRayBlenderCompiler
{
public:
	XRayBlenderParticle();
	virtual ~XRayBlenderParticle();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
protected:
	xrP_TOKEN	oBlend;
	xrP_Integer	oAREF;
	xrP_BOOL	oClamp;
};
#pragma once
class  XRayBlenderDetailStill :public XRayBlenderCompiler
{
public:
	XRayBlenderDetailStill();
	virtual ~XRayBlenderDetailStill();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
protected:
	xrP_BOOL	oBlend;
};
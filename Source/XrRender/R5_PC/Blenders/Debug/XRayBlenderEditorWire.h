#pragma once
class  XRayBlenderEditorWire :public XRayBlenderCompiler
{
public:
	XRayBlenderEditorWire();
	virtual ~XRayBlenderEditorWire();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
protected:
	string64	oT_Factor;
};
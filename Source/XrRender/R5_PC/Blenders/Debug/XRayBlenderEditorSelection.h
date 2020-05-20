#pragma once
class  XRayBlenderEditorSelection :public XRayBlenderCompiler
{
public:
	XRayBlenderEditorSelection();
	virtual ~XRayBlenderEditorSelection();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
protected:
	string64	oT_Factor;
};
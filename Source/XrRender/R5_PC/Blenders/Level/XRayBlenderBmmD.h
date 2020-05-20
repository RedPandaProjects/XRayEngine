#pragma once
class  XRayBlenderBmmD :public XRayBlenderCompiler
{

public:
	XRayBlenderBmmD();
	virtual ~XRayBlenderBmmD();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual void InitializeGraphics();
	virtual void InitializeMesh();
	virtual void Compile(XRayShaderElement& shader);
	virtual bool UseDetail() { return true; }
	virtual bool UseMeshShader() { return true; }
protected:
	string64	oT2_Name;	// name of secondary texture
	string64	oT2_xform;	// xform for secondary texture
	string64	oR_Name;	//. ����� �� �������
	string64	oG_Name;	//. ����� �� �������
	string64	oB_Name;	//. ����� �� �������
	string64	oA_Name;	//. ����� �� �������
};
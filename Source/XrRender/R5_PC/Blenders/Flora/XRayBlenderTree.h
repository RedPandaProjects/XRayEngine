#pragma once
class  XRayBlenderTree :public XRayBlenderCompiler
{
public:
	XRayBlenderTree();
	virtual 	~XRayBlenderTree();
	virtual LPCSTR getComment();
	virtual void Save(IWriter& fs);
	virtual void Load(IReader& fs, u16 version);
	virtual bool UseDetail() { return true; }
	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);
	virtual int GetSlotPipelineForShader(size_t slot);
protected:
	xrP_BOOL	oBlend;
	xrP_BOOL	oNotAnTree;
};
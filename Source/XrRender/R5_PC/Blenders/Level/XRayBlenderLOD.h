#pragma once
class  XRayBlenderLOD :public XRayBlenderCompiler
{
public:
	XRayBlenderLOD();
	virtual ~XRayBlenderLOD();
	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);
private:
};
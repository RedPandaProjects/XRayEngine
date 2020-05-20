#pragma once
class  XRayBlenderBloom :public XRayBlenderCompiler
{
public:
	XRayBlenderBloom();
	virtual ~XRayBlenderBloom();
	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);
private:
};
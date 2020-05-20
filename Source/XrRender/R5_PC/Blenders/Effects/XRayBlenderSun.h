#pragma once
class  XRayBlenderSun :public XRayBlenderCompiler
{
public:
	XRayBlenderSun();
	virtual ~XRayBlenderSun();
	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);
private:
};
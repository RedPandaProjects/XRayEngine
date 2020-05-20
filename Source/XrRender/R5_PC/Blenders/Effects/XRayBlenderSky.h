#pragma once
class  XRayBlenderSky :public XRayBlenderCompiler
{
public:
	XRayBlenderSky();
	virtual ~XRayBlenderSky();
	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);
private:
};
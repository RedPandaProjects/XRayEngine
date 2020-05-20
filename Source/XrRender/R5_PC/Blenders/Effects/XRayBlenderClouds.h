#pragma once
class  XRayBlenderClouds :public XRayBlenderCompiler
{
public:
	XRayBlenderClouds();
	virtual ~XRayBlenderClouds();
	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);
private:
};
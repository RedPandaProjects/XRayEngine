#pragma once
class  XRayBlenderLightplanes :public XRayBlenderCompiler
{
public:
	XRayBlenderLightplanes();
	virtual ~XRayBlenderLightplanes();
	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);
private:
};
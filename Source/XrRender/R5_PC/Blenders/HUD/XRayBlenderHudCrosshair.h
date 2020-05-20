#pragma once
class  XRayBlenderHudCrosshair :public XRayBlenderCompiler
{
public:
	XRayBlenderHudCrosshair();
	virtual ~XRayBlenderHudCrosshair();
	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);
private:
};
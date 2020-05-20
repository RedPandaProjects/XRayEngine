#pragma once
class  XRayBlenderHudFont :public XRayBlenderCompiler
{
public:
	XRayBlenderHudFont();
	virtual ~XRayBlenderHudFont();
	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);
private:
};
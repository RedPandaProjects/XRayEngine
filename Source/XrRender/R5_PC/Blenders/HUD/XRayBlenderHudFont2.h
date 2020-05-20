#pragma once
class  XRayBlenderHudFont2 :public XRayBlenderCompiler
{
public:
	XRayBlenderHudFont2();
	virtual ~XRayBlenderHudFont2();
	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);
private:
};
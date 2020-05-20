#pragma once
class  XRayBlenderHudMovie :public XRayBlenderCompiler
{
public:
	XRayBlenderHudMovie();
	virtual ~XRayBlenderHudMovie();
	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);
private:
};
#pragma once
class  XRayBlenderViewportSet :public XRayBlenderCompiler
{
public:
	XRayBlenderViewportSet();
	virtual ~XRayBlenderViewportSet();
	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);
private:
};
#pragma once
class  XRayBlenderSelflight :public XRayBlenderCompiler
{
public:
	XRayBlenderSelflight();
	virtual ~XRayBlenderSelflight();
	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);
private:
};
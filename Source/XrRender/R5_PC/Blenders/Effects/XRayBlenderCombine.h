#pragma once
class  XRayBlenderCombine :public XRayBlenderCompiler
{
public:
	XRayBlenderCombine();
	virtual ~XRayBlenderCombine();
	virtual void InitializeGraphics();
	virtual void Compile(XRayShaderElement& shader);
private:
};
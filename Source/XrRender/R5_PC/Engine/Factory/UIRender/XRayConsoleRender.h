#pragma once
class XRayConsoleRender :public IConsoleRender
{
public:
	XRayConsoleRender();
	virtual void Copy(IConsoleRender &_in);
	virtual void OnRender(bool bGame);
private:
	XRayShader m_Shader;
	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> m_VertexBuffer;
};
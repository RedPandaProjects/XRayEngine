#pragma once
class XRayFlareRender :public IFlareRender
{
public:
	XRayFlareRender();
	virtual ~XRayFlareRender();
	virtual void Copy(IFlareRender& _in);

	virtual void CreateShader(LPCSTR sh_name, LPCSTR tex_name);
	virtual void DestroyShader();
	XRayShaderRef Shader;
};
class XRayLensFlareRender:public ILensFlareRender
{
public:
	XRayLensFlareRender();
	virtual void Copy(ILensFlareRender &_in);


	virtual void Render(CLensFlare& owner, BOOL bSun, BOOL bFlares, BOOL bGradient);
	virtual void OnDeviceCreate();
	virtual void OnDeviceDestroy();
private:
	XRayShaderRef m_sun_shader;
	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> m_sun_vertex_buffer;
	
	XRayShaderRef m_flares_shader[24];
	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> m_flares_vertex_buffer[24];

	XRayShaderRef m_gradient_shader;
	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> m_gradient_vertex_buffer;
};
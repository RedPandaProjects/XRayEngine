#pragma once
class XRayEnvDescriptorMixerRender :public  IEnvDescriptorMixerRender
{
public:
	XRayEnvDescriptorMixerRender();
	virtual void Copy(IEnvDescriptorMixerRender& _in);

	virtual void Destroy();
	virtual void Clear();
	virtual void lerp(IEnvDescriptorRender* inA, IEnvDescriptorRender* inB);


	XRayTexture* sky_textures[2];
	XRayTexture* sky_texture_envs[2];
	XRayTexture* clouds_textures[2];
};


class XRayEnvDescriptorRender :public IEnvDescriptorRender
{
public:
	XRayEnvDescriptorRender();
	virtual void Copy(IEnvDescriptorRender& _in);

	virtual void OnDeviceCreate(IEnvDescriptor& owner);
	virtual void OnDeviceDestroy();
	XRayTexture* sky_texture;
	XRayTexture* sky_texture_env;
	XRayTexture* clouds_texture;
};


class XRayEnvironmentRender :public IEnvironmentRender
{
public:
	XRayEnvironmentRender();
	virtual ~XRayEnvironmentRender();
	virtual void	Copy(IEnvironmentRender& _in) ;
	virtual void	OnFrame(IEnvironment& env) ;
	virtual void	OnLoad() ;
	virtual void	OnUnload() ;
	virtual void	RenderSky(IEnvironment& env) ;
	virtual void	RenderClouds(IEnvironment& env) ;
	virtual void	OnDeviceCreate() ;
	virtual void	OnDeviceDestroy() ;
	virtual particles_systems::library_interface const& particles_systems_library();
private:
	XRayShader m_Sky_Shader;
	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> m_Sky_VertexBuffer;
	XRayTexture* m_Sky_Texture1;
	XRayTexture* m_Sky_Texture2;

	XRayShader m_Clouds_Shader;
	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> m_Clouds_VertexBuffer;
	BearFactoryPointer<BearRHI::BearRHIIndexBuffer> m_Clouds_IndexBuffer;
	XRayTexture* m_Clouds_Texture1;
	XRayTexture* m_Clouds_Texture2;
};
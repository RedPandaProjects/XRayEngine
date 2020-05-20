#include "pch.h"

XRayEnvDescriptorMixerRender::XRayEnvDescriptorMixerRender()
{
	sky_textures[0] = 0;
	sky_textures[1] = 0;
	sky_texture_envs[0] = 0;
	sky_texture_envs[1] = 0;
	clouds_textures[0] = 0;
	clouds_textures[1] = 0;
}

void XRayEnvDescriptorMixerRender::Copy(IEnvDescriptorMixerRender& _in)
{
	sky_textures[0] = static_cast<XRayEnvDescriptorMixerRender&>(_in).sky_textures[0];
	if (sky_textures[0])sky_textures[0]->Counter++;
	sky_textures[1] = static_cast<XRayEnvDescriptorMixerRender&>(_in).sky_textures[1];
	if (sky_textures[1])sky_textures[1]->Counter++;

	sky_texture_envs[0] = static_cast<XRayEnvDescriptorMixerRender&>(_in).sky_texture_envs[0];
	if (sky_texture_envs[0])sky_texture_envs[0]->Counter++;
	sky_texture_envs[1] = static_cast<XRayEnvDescriptorMixerRender&>(_in).sky_texture_envs[1];
	if (sky_texture_envs[1])sky_texture_envs[1]->Counter++;

	clouds_textures[0] = static_cast<XRayEnvDescriptorMixerRender&>(_in).clouds_textures[0];
	if (clouds_textures[0])clouds_textures[0]->Counter++;
	clouds_textures[1] = static_cast<XRayEnvDescriptorMixerRender&>(_in).clouds_textures[1];
	if (clouds_textures[1])clouds_textures[1]->Counter++;

}

void XRayEnvDescriptorMixerRender::Destroy()
{
	GResourcesManager->Free(sky_textures[0]);
	GResourcesManager->Free(sky_textures[1]);

	GResourcesManager->Free(sky_texture_envs[0]);
	GResourcesManager->Free(sky_texture_envs[1]);

	GResourcesManager->Free(clouds_textures[0]);
	GResourcesManager->Free(clouds_textures[1]);
	sky_textures[0] = 0;
	sky_textures[1] = 0;
	sky_texture_envs[0] = 0;
	sky_texture_envs[1] = 0;
	clouds_textures[0] = 0;
	clouds_textures[1] = 0;
}

void XRayEnvDescriptorMixerRender::Clear()
{
	Destroy();
}

void XRayEnvDescriptorMixerRender::lerp(IEnvDescriptorRender* inA, IEnvDescriptorRender* inB)
{
	Destroy();
	sky_textures[0] = static_cast<XRayEnvDescriptorRender&>(*inA).sky_texture;
	if (sky_textures[0])sky_textures[0]->Counter++;
	sky_texture_envs[0] = static_cast<XRayEnvDescriptorRender&>(*inA).sky_texture_env;
	if (sky_texture_envs[0])sky_texture_envs[0]->Counter++;
	clouds_textures[0] = static_cast<XRayEnvDescriptorRender&>(*inA).clouds_texture;
	if (clouds_textures[0])clouds_textures[0]->Counter++;

	sky_textures[1] = static_cast<XRayEnvDescriptorRender&>(*inB).sky_texture;
	if (sky_textures[1])sky_textures[1]->Counter++;
	sky_texture_envs[1] = static_cast<XRayEnvDescriptorRender&>(*inB).sky_texture_env;
	if (sky_texture_envs[1])sky_texture_envs[1]->Counter++;
	clouds_textures[1] = static_cast<XRayEnvDescriptorRender&>(*inB).clouds_texture;
	if (clouds_textures[1])clouds_textures[1]->Counter++;
}

XRayEnvDescriptorRender::XRayEnvDescriptorRender()
{
	sky_texture = 0;
	sky_texture_env = 0;
	clouds_texture = 0;
}

void XRayEnvDescriptorRender::Copy(IEnvDescriptorRender& _in)
{
	sky_texture = static_cast<XRayEnvDescriptorRender&>(_in).sky_texture;
	if (sky_texture)sky_texture->Counter++;
	sky_texture_env = static_cast<XRayEnvDescriptorRender&>(_in).sky_texture_env;
	if (sky_texture_env)sky_texture_env->Counter++;
	clouds_texture = static_cast<XRayEnvDescriptorRender&>(_in).clouds_texture;
	if (clouds_texture)clouds_texture->Counter++;
}

void XRayEnvDescriptorRender::OnDeviceCreate(CEnvDescriptor& owner)
{
	if (owner.sky_texture_name.size())
		sky_texture = GResourcesManager->GetTexture(owner.sky_texture_name.c_str());
	if (owner.sky_texture_env_name.size())
		sky_texture_env = GResourcesManager->GetTexture(owner.sky_texture_env_name.c_str());
	if (owner.clouds_texture_name.size())
		clouds_texture = GResourcesManager->GetTexture(owner.clouds_texture_name.c_str());
}

void XRayEnvDescriptorRender::OnDeviceDestroy()
{
	GResourcesManager->Free(sky_texture);
	GResourcesManager->Free(sky_texture_env);
	GResourcesManager->Free(clouds_texture);
	sky_texture = 0;
	sky_texture_env = 0;
	clouds_texture = 0;
}

#include "../../XrEngine/xr_efflensflare.h"
XRayEnvironmentRender::XRayEnvironmentRender()
{

}

XRayEnvironmentRender::~XRayEnvironmentRender()
{
}

void XRayEnvironmentRender::Copy(IEnvironmentRender & _in)
{
	R_ASSERT(0);
}

void XRayEnvironmentRender::OnFrame(CEnvironment& env)
{
}



void XRayEnvironmentRender::OnLoad()
{
}

void XRayEnvironmentRender::OnUnload()
{
}
#pragma pack(push,1)
struct v_skybox {
	Fvector3	p;
	u32			color;
	Fvector3	uv[2];

	void		set(Fvector3& _p, u32 _c, Fvector3& _tc)
	{
		p = _p;
		color = _c;
		uv[0] = _tc;
		uv[1] = _tc;
	}
};
#pragma pack(pop)

void XRayEnvironmentRender::RenderSky(CEnvironment& env)
{
	if (m_Sky_VertexBuffer.empty())OnDeviceCreate();

	XRayEnvDescriptorMixerRender& mixRen = *(XRayEnvDescriptorMixerRender*)&*env.CurrentEnv->m_pDescriptorMixer;


	if (!mixRen.sky_textures[0]||  mixRen.sky_textures[0]->GetType() != XRayTexture::TT_Cube)return;
	if (!mixRen.sky_textures[1] || mixRen.sky_textures[1]->GetType() != XRayTexture::TT_Cube)return;

	m_Sky_Texture1->Texture = mixRen.sky_textures[0]->Texture;
	m_Sky_Texture2->Texture = mixRen.sky_textures[1]->Texture;
	u32	C = color_rgba(iFloor(env.CurrentEnv->sky_color.x * 255.f), iFloor(env.CurrentEnv->sky_color.y * 255.f), iFloor(env.CurrentEnv->sky_color.z * 255.f), iFloor(env.CurrentEnv->weight * 255.f));

	BearContextEventLock Event(*HW->Context.get(), "RenderSky", BearColor::Red);
	v_skybox*pv = (v_skybox*)m_Sky_VertexBuffer->Lock();
	{
		static	Fvector3	hbox_verts[24] =
		{
			{ -1.f, -1.f, -1.f },	{ -1.f, -1.01f, -1.f }, // down
			{ 1.f, -1.f, -1.f },	{ 1.f, -1.01f, -1.f }, // down
			{ -1.f, -1.f, 1.f },	{ -1.f, -1.01f, 1.f }, // down
			{ 1.f, -1.f, 1.f },		{ 1.f, -1.01f, 1.f }, // down
			{ -1.f, 1.f, -1.f },	{ -1.f, 1.f, -1.f },
			{ 1.f, 1.f, -1.f },		{ 1.f, 1.f, -1.f },
			{ -1.f, 1.f, 1.f },		{ -1.f, 1.f, 1.f },
			{ 1.f, 1.f, 1.f },		{ 1.f, 1.f, 1.f },
			{ -1.f, 0.f, -1.f },	{ -1.f, -1.f, -1.f }, // half
			{ 1.f, 0.f, -1.f },		{ 1.f, -1.f, -1.f }, // half
			{ 1.f, 0.f, 1.f },		{ 1.f, -1.f, 1.f }, // half
			{ -1.f, 0.f, 1.f },		{ -1.f, -1.f, 1.f } // half
		};
		static	u16			hbox_faces[20 * 3] =
		{
			0,	 2,	 3,
			3,	 1,	 0,
			4,	 5,	 7,
			7,	 6,	 4,
			0,	 1,	 9,
			9,	 8,	 0,
			8,	 9,	 5,
			5,	 4,	 8,
			1,	 3,	10,
			10,	 9,	 1,
			9,	10,	 7,
			7,	 5,	 9,
			3,	 2,	11,
			11,	10,	 3,
			10,	11,	 6,
			6,	 7,	10,
			2,	 0,	 8,
			8,	11,	 2,
			11,	 8,	 4,
			4,	 6,	11
		};
		for (size_t i = 0; i < 60; i++)
		{
			u32 v = hbox_faces[i];
			{
				pv->set(hbox_verts[v * 2], C, hbox_verts[v * 2 + 1]);
			}
			pv++;
		}
	}
	m_Sky_VertexBuffer->Unlock();

	GRenderInterface.UpdateDescriptorHeap(m_Sky_Shader.E[0][0],true);
	m_Sky_Shader.E[0][0].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation), GUniformAllocator->Uniform[XRayUniformAllocator::UT_Transformation], XRayUniformAllocator::RT_Sky);
	m_Sky_Shader.E[0][0].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform), GUniformAllocator->GlobalUniform,0);
	m_Sky_Shader.E[0][0].Set(HW->Context, FVF::F_SKY);
	HW->Context->SetVertexBuffer(m_Sky_VertexBuffer);
	HW->Context->Draw(60);

	env.eff_LensFlare->Render(TRUE, FALSE, FALSE);
}
#pragma pack(push,1)
struct v_clouds {
	Fvector3	p;
	u32			color;
	u32			intensity;
	void		set(Fvector3& _p, u32 _c, u32 _i)
	{
		p = _p;
		color = _c;
		intensity = _i;
	}
};
#pragma pack(pop)

void XRayEnvironmentRender::RenderClouds(CEnvironment& env)
{
	BearContextEventLock Event(*HW->Context.get(), "RenderClouds", BearColor::Green);
	XRayEnvDescriptorMixerRender& mixRen = *(XRayEnvDescriptorMixerRender*)&*env.CurrentEnv->m_pDescriptorMixer;


	if (!mixRen.clouds_textures[0] || mixRen.clouds_textures[0]->GetType() == XRayTexture::TT_Cube)return;
	if (!mixRen.clouds_textures[1] || mixRen.clouds_textures[1]->GetType() == XRayTexture::TT_Cube)return;

	m_Clouds_Texture1->Texture = mixRen.clouds_textures[0]->Texture;
	m_Clouds_Texture2->Texture = mixRen.clouds_textures[1]->Texture;


	Fvector wd0, wd1;
	Fvector4 wind_dir;
	wd0.setHP(PI_DIV_4, 0);
	wd1.setHP(PI_DIV_4 + PI_DIV_8, 0);
	wind_dir.set(wd0.x, wd0.z, wd1.x, wd1.z).mul(0.5f).add(0.5f).mul(255.f);
	u32	i_offset, v_offset;
	u32	C0 = color_rgba(iFloor(wind_dir.x), iFloor(wind_dir.y), iFloor(wind_dir.w), iFloor(wind_dir.z));
	u32	C1 = color_rgba(iFloor(env.CurrentEnv->clouds_color.x * 255.f), iFloor(env.CurrentEnv->clouds_color.y * 255.f), iFloor(env.CurrentEnv->clouds_color.z * 255.f), iFloor(env.CurrentEnv->clouds_color.w * 255.f));

	// Fill index buffer

	if (m_Clouds_IndexBuffer->GetCount() < env.CloudsIndices.size())
	{
		m_Clouds_IndexBuffer->Create(env.CloudsIndices.size() * 2,true);
	}
	{
		u32* pib = m_Clouds_IndexBuffer->Lock();
		for (size_t i = 0; i < env.CloudsIndices.size(); i++)
		{
			pib[i] = env.CloudsIndices[i];
		}
		m_Clouds_IndexBuffer->Unlock();
	}
	if (m_Clouds_VertexBuffer->GetCount() < env.CloudsVerts.size())
	{
		m_Clouds_VertexBuffer->Create(GResourcesManager->GetStride(FVF::F_CLOUDS),env.CloudsVerts.size(), true);
	}
	{
		v_clouds* pv = (v_clouds*)m_Clouds_VertexBuffer->Lock();

		for (auto it = env.CloudsVerts.begin(); it != env.CloudsVerts.end(); it++, pv++) {
			pv->set(*it, C0, C1);
		}
		m_Clouds_VertexBuffer->Unlock();
	}
	GRenderInterface.UpdateDescriptorHeap(m_Clouds_Shader.E[0][0], true);
	m_Clouds_Shader.E[0][0].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation), GUniformAllocator->Uniform[XRayUniformAllocator::UT_Transformation], XRayUniformAllocator::RT_Clouds);
	m_Clouds_Shader.E[0][0].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform), GUniformAllocator->GlobalUniform, 0);
	m_Clouds_Shader.E[0][0].Set(HW->Context, FVF::F_CLOUDS);
	HW->Context->SetVertexBuffer(m_Clouds_VertexBuffer);
	HW->Context->SetIndexBuffer(m_Clouds_IndexBuffer);
	HW->Context->DrawIndex(m_Clouds_IndexBuffer->GetCount());
}


void XRayEnvironmentRender::OnDeviceCreate()
{
	GResourcesManager->CompileBlender(m_Sky_Shader, "sky","");
	GResourcesManager->CompileBlender(m_Clouds_Shader, "clouds", "");
	m_Sky_VertexBuffer = BearRenderInterface::CreateVertexBuffer();
	m_Sky_VertexBuffer->Create(GResourcesManager->GetStride(FVF::F_SKY), 20 * 3, true);
	m_Sky_Texture1 = GResourcesManager->GetTexture(TEXT("$user\\sky1"));
	m_Sky_Texture2 = GResourcesManager->GetTexture(TEXT("$user\\sky2"));

	m_Clouds_VertexBuffer = BearRenderInterface::CreateVertexBuffer();
	m_Clouds_IndexBuffer = BearRenderInterface::CreateIndexBuffer();
	m_Clouds_Texture1 = GResourcesManager->GetTexture(TEXT("$user\\clouds1"));
	m_Clouds_Texture2 = GResourcesManager->GetTexture(TEXT("$user\\clouds2"));
}

void XRayEnvironmentRender::OnDeviceDestroy()
{
	GResourcesManager->Free(m_Sky_Texture1);
	GResourcesManager->Free(m_Sky_Texture2);
	m_Sky_VertexBuffer.clear();
	m_Sky_Shader.Clear();
}
particles_systems::library_interface *null = 0;
particles_systems::library_interface const & XRayEnvironmentRender::particles_systems_library()
{
	return *null;
}

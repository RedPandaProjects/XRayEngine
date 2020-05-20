#include "pch.h"
XRayFlareRender::XRayFlareRender()
{
}

XRayFlareRender::~XRayFlareRender()
{
}

void XRayFlareRender::Copy(IFlareRender& _in)
{
	Shader = static_cast<XRayFlareRender&>(_in).Shader;
}

void XRayFlareRender::CreateShader(LPCSTR sh_name, LPCSTR tex_name)
{
	XRayShader shader;
	if (tex_name && tex_name[0])
		GResourcesManager->CompileBlender(shader, sh_name, tex_name);
	Shader = XRayShaderRef(shader);
}

void XRayFlareRender::DestroyShader()
{
	Shader.clear();
}

#include "../../XrEngine/xr_efflensflare.h"

XRayLensFlareRender::XRayLensFlareRender()
{
}

void XRayLensFlareRender::Copy(ILensFlareRender& _in)
{
	m_sun_shader = static_cast<XRayLensFlareRender&>(_in).m_sun_shader;
	for (size_t i = 0; i < 24; i++)
	{
		m_flares_shader[i] = static_cast<XRayLensFlareRender&>(_in).m_flares_shader[i];
	}
	m_gradient_shader = static_cast<XRayLensFlareRender&>(_in).m_gradient_shader;
}

#define FAR_DIST g_pGamePersistent->Environment().CurrentEnv->far_plane


void XRayLensFlareRender::Render(CLensFlare& owner, BOOL bSun, BOOL bFlares, BOOL bGradient)
{
	
	Fcolor dwLight;
	Fcolor color;
	Fvector vec, vecSx, vecSy;
	Fvector vecDx, vecDy;

	float fDistance = FAR_DIST * 0.75f;

	dwLight.set(owner.LightColor);
	if (bSun)
	{
		if (owner.m_Current->m_Flags.is(CLensFlareDescriptor::flSource))
		{
			BearContextEventLock Event(*HW->Context.get(), "RenderSun", BearColor::Yellow);
			vecSx.mul(owner.vecX, owner.m_Current->m_Source.fRadius * fDistance);
			vecSy.mul(owner.vecY, owner.m_Current->m_Source.fRadius * fDistance);

			if (owner.m_Current->m_Source.ignore_color)
				color.set(1.0f, 1.0f, 1.0f, 1.0f);
			else
				color.set(dwLight);

			color.a *= owner.m_StateBlend;
			u32 c = color.get();

			if (m_sun_vertex_buffer.empty())
			{
				m_sun_vertex_buffer = BearRenderInterface::CreateVertexBuffer();
				m_sun_vertex_buffer->Create(sizeof(FVF::LIT), 6, true);
			}

			FVF::LIT* pv = (FVF::LIT*) m_sun_vertex_buffer->Lock();
			pv->set(owner.vecLight.x + vecSx.x - vecSy.x, owner.vecLight.y + vecSx.y - vecSy.y, owner.vecLight.z + vecSx.z - vecSy.z, c, 0, 0); pv++;
			pv->set(owner.vecLight.x + vecSx.x + vecSy.x, owner.vecLight.y + vecSx.y + vecSy.y, owner.vecLight.z + vecSx.z + vecSy.z, c, 0, 1); pv++;
			pv->set(owner.vecLight.x - vecSx.x - vecSy.x, owner.vecLight.y - vecSx.y - vecSy.y, owner.vecLight.z - vecSx.z - vecSy.z, c, 1, 0); pv++;

			pv->set(owner.vecLight.x - vecSx.x + vecSy.x, owner.vecLight.y -vecSx.y + vecSy.y, owner.vecLight.z - vecSx.z + vecSy.z, c, 1, 1); pv++;
			pv->set(owner.vecLight.x - vecSx.x - vecSy.x, owner.vecLight.y - vecSx.y - vecSy.y, owner.vecLight.z - vecSx.z - vecSy.z, c, 1, 0); pv++;
			pv->set(owner.vecLight.x + vecSx.x + vecSy.x, owner.vecLight.y + vecSx.y + vecSy.y, owner.vecLight.z + vecSx.z + vecSy.z, c, 0, 1); pv++;
			m_sun_vertex_buffer->Unlock();

			m_sun_shader = ((XRayFlareRender*)&*owner.m_Current->m_Gradient.m_pRender)->Shader;

			GRenderInterface.UpdateDescriptorHeap(m_sun_shader->E[0][0], true);
			R_ASSERT(!m_sun_shader->E[0][0].DescriptorHeap.empty());
			m_sun_shader->E[0][0].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation), GUniformAllocator->Uniform[XRayUniformAllocator::UT_Transformation], XRayUniformAllocator::RT_Fidentity);
			m_sun_shader->E[0][0].Set(HW->Context, FVF::F_LIT);
			HW->Context->SetVertexBuffer(m_sun_vertex_buffer);
			HW->Context->Draw(6);
		}
	}
	
	if (owner.fBlend >= EPS_L)
	{
		// Flares
		if (bFlares)
		{
			vecDx.normalize(owner.vecAxis);
			vecDy.crossproduct(vecDx, owner.vecDir);
			if (owner.m_Current->m_Flags.is(CLensFlareDescriptor::flFlare))
			{
				BearContextEventLock Event(*HW->Context.get(), "RenderFlare");
				size_t index = 0;
				for (auto it = owner.m_Current->m_Flares.begin(); it != owner.m_Current->m_Flares.end(); ++it)
				{
					vec.mul(owner.vecAxis, (*it).fPosition);
					vec.add(owner.vecCenter);
					vecSx.mul(vecDx, (*it).fRadius * fDistance);
					vecSy.mul(vecDy, (*it).fRadius * fDistance);
					float cl = (*it).fOpacity * owner.fBlend * owner.m_StateBlend;
					color.set(dwLight);
					color.mul_rgba(cl);
					u32 c = color.get();

					if (m_flares_vertex_buffer[index].empty())
					{
						m_flares_vertex_buffer[index] = BearRenderInterface::CreateVertexBuffer();
						m_flares_vertex_buffer[index]->Create(sizeof(FVF::LIT), 6, true);
					}
					FVF::LIT* pv = (FVF::LIT*) m_flares_vertex_buffer[index]->Lock();
					pv->set(vec.x + vecSx.x - vecSy.x, vec.y + vecSx.y - vecSy.y, vec.z + vecSx.z - vecSy.z, c, 0, 0); pv++;
					pv->set(vec.x + vecSx.x + vecSy.x, vec.y + vecSx.y + vecSy.y, vec.z + vecSx.z + vecSy.z, c, 0, 1); pv++;
					pv->set(vec.x - vecSx.x - vecSy.x, vec.y - vecSx.y - vecSy.y, vec.z - vecSx.z - vecSy.z, c, 1, 0); pv++;

					pv->set(vec.x - vecSx.x + vecSy.x, vec.y - vecSx.y + vecSy.y, vec.z - vecSx.z + vecSy.z, c, 1, 1); pv++;
					pv->set(vec.x - vecSx.x - vecSy.x, vec.y - vecSx.y - vecSy.y, vec.z - vecSx.z - vecSy.z, c, 1, 0); pv++;
					pv->set(vec.x + vecSx.x + vecSy.x, vec.y + vecSx.y + vecSy.y, vec.z + vecSx.z + vecSy.z, c, 0, 1); pv++;

					m_flares_vertex_buffer[index]->Unlock();

					m_flares_shader[index] = ((XRayFlareRender*)&*it->m_pRender)->Shader;

					GRenderInterface.UpdateDescriptorHeap(m_flares_shader[index]->E[0][0], true);
					R_ASSERT(!m_flares_shader[index]->E[0][0].DescriptorHeap.empty());
					m_flares_shader[index]->E[0][0].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation), GUniformAllocator->Uniform[XRayUniformAllocator::UT_Transformation], XRayUniformAllocator::RT_Fidentity);
					m_flares_shader[index]->E[0][0].Set(HW->Context, FVF::F_LIT);
					HW->Context->SetVertexBuffer(m_flares_vertex_buffer[index]);
					HW->Context->Draw(6);
					index++;
				}
			}
		}
		if (bGradient && (owner.fGradientValue >= EPS_L))
		{
			BearContextEventLock Event(*HW->Context.get(), "RenderGradient");
			if (owner.m_Current->m_Flags.is(CLensFlareDescriptor::flGradient))
			{
				vecSx.mul(owner.vecX, owner.m_Current->m_Gradient.fRadius * owner.fGradientValue * fDistance);
				vecSy.mul(owner.vecY, owner.m_Current->m_Gradient.fRadius * owner.fGradientValue * fDistance);

				color.set(dwLight);
				color.mul_rgba(owner.fGradientValue * owner.m_StateBlend);

				u32 c = color.get();
				if (m_gradient_vertex_buffer.empty())
				{
					m_gradient_vertex_buffer = BearRenderInterface::CreateVertexBuffer();
					m_gradient_vertex_buffer->Create(sizeof(FVF::LIT), 6, true);
				}
				FVF::LIT* pv = (FVF::LIT*) m_gradient_vertex_buffer->Lock();
				pv->set(owner.vecLight.x + vecSx.x - vecSy.x, owner.vecLight.y + vecSx.y - vecSy.y, owner.vecLight.z + vecSx.z - vecSy.z, c, 0, 0); pv++;
				pv->set(owner.vecLight.x + vecSx.x + vecSy.x, owner.vecLight.y + vecSx.y + vecSy.y, owner.vecLight.z + vecSx.z + vecSy.z, c, 0, 1); pv++;
				pv->set(owner.vecLight.x - vecSx.x - vecSy.x, owner.vecLight.y - vecSx.y - vecSy.y, owner.vecLight.z - vecSx.z - vecSy.z, c, 1, 0); pv++;

				pv->set(owner.vecLight.x - vecSx.x + vecSy.x, owner.vecLight.y - vecSx.y + vecSy.y, owner.vecLight.z - vecSx.z + vecSy.z, c, 1, 1); pv++;
				pv->set(owner.vecLight.x - vecSx.x - vecSy.x, owner.vecLight.y - vecSx.y - vecSy.y, owner.vecLight.z - vecSx.z - vecSy.z, c, 1, 0); pv++;
				pv->set(owner.vecLight.x + vecSx.x + vecSy.x, owner.vecLight.y + vecSx.y + vecSy.y, owner.vecLight.z + vecSx.z + vecSy.z, c, 0, 1); pv++;

				m_gradient_vertex_buffer->Unlock();
				m_gradient_shader = ((XRayFlareRender*)&*owner.m_Current->m_Gradient.m_pRender)->Shader;

				GRenderInterface.UpdateDescriptorHeap(m_gradient_shader->E[0][0], true);
				R_ASSERT(!m_gradient_shader->E[0][0].DescriptorHeap.empty());
				m_gradient_shader->E[0][0].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation), GUniformAllocator->Uniform[XRayUniformAllocator::UT_Transformation], XRayUniformAllocator::RT_Fidentity);
				m_gradient_shader->E[0][0].Set(HW->Context, FVF::F_LIT);
				HW->Context->SetVertexBuffer(m_gradient_vertex_buffer);
				HW->Context->Draw(6);
			}
		}
	}

}

void XRayLensFlareRender::OnDeviceCreate()
{
}

void XRayLensFlareRender::OnDeviceDestroy()
{
	m_sun_shader.clear();
	m_sun_vertex_buffer.clear();
	for (size_t i = 0; i < 24; i++)
	{
		m_flares_shader[i].clear();
		m_flares_vertex_buffer[i].clear();
	}
	m_gradient_shader.clear();
	m_gradient_vertex_buffer.clear();
}

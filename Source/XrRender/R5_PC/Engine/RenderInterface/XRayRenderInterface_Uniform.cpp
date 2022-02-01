#include "pch.h"


void XRayRenderInterface::UniformCounter(size_t i, XRayObjectRender* b)
{
	if (i == XRayUniformAllocator::UT_DetailScalar)
	{
		b->UseUniform[i] = !!b->DetailScalar;
		if (b->UseUniform[i])
			b->UniformID[i] = b->DetailScalar - 1;
		return;
	}
	if (i == XRayUniformAllocator::UT_AlphaRef)
	{
		if (b->AlphaRef >= 0)
		{
			clampr(b->AlphaRef, 0, 255);
			b->UseUniform[i] = true;
			b->UniformID[i] = b->AlphaRef;
			return;
		}
	}
	if (i == XRayUniformAllocator::UT_Transformation)
	{
		if (b->UseMWorld == false)return;
	}
	if (i == XRayUniformAllocator::UT_Local)
	{
		b->UseUniform[i] = b->UseLocalUniform;
	}
	m_UniformCounter += b->UseUniform[i];
}

void XRayRenderInterface::UniformsCounter()
{

	for (size_t i = 0; i < XRayUniformAllocator::UT_Count; i++)
	{
		std::function<void(XRayObjectRender*)> function = std::bind(&XRayRenderInterface::UniformCounter, this, i, std::placeholders::_1);
		m_UniformCounter = 0;
		Rdsgraph_Genderal.RenderObjects.setup(function);
		Rdsgraph_Genderal.RenderHUDObjects.setup(function);
		Rdsgraph_Genderal.RenderSortObjects.setup(function);
		if (i == XRayUniformAllocator::UT_Transformation)m_UniformCounter += XRayUniformAllocator::RT_Count;
		GUniformAllocator->Reserve(static_cast<XRayUniformAllocator::EUniformType>(i), m_UniformCounter);
	}
}

void XRayRenderInterface::UniformUpdate(size_t i, XRayObjectRender* b)
{
	Fmatrix MWV, MVPW;
	if (b->UseUniform[i])
	{
		if (i == XRayUniformAllocator::UT_Transformation)
		{
			if (b->UseMWorld)
			{
				b->UniformID[i] = GUniformAllocator->Alloc(static_cast<XRayUniformAllocator::EUniformType>(i));
				u8* ptr = (u8*)m_UniformPtr + GUniformAllocator->CalcOffset(static_cast<XRayUniformAllocator::EUniformType>(i), b->UniformID[i]);

				SetWorld(b->MWorld);

				ContatMatrix* matrix = (ContatMatrix*)ptr;
				MWV.mul_43(m_MView, m_MWorld);
				MVPW.mul(m_MProject, MWV);
				matrix->View = m_MView;
				matrix->Project = m_MProject;
				matrix->World = m_MWorld;
				matrix->VP.mul(m_MProject, m_MView);
				matrix->VPW = MVPW;
				matrix->WV = MWV;
			}
			else
			{
				b->UniformID[i] = XRayUniformAllocator::RT_Fidentity;
			}
		}
		else if (i == XRayUniformAllocator::UT_Local)
		{
			b->UniformID[i] = GUniformAllocator->Alloc(static_cast<XRayUniformAllocator::EUniformType>(i));
			u8* ptr = (u8*)m_UniformPtr + GUniformAllocator->CalcOffset(static_cast<XRayUniformAllocator::EUniformType>(i), b->UniformID[i]);
			XRayLocalUniform* localu = (XRayLocalUniform*)ptr;
			*localu = b->LocalUniform;
		}
		else
		{

			b->UniformID[i] = GUniformAllocator->Alloc(static_cast<XRayUniformAllocator::EUniformType>(i));
			u8* ptr = (u8*)m_UniformPtr + GUniformAllocator->CalcOffset(static_cast<XRayUniformAllocator::EUniformType>(i), b->UniformID[i]);
			b->Visual->UpdateUniform(static_cast<XRayUniformAllocator::EUniformType>(i), ptr);
		}
	}
}

void XRayRenderInterface::UniformsUpdate()
{
	Fmatrix MWV, MVPW;
	{
		IEnvDescriptor& desc = *g_pGamePersistent->Environment().CurrentEnv;
		XRayGlobalUniform* Data = (XRayGlobalUniform*)GUniformAllocator->GlobalUniform->Lock();
		Data->AmbientColor.set(desc.ambient.x, desc.ambient.y, desc.ambient.z);
		Data->HemiColor.set(desc.hemi_color.x, desc.hemi_color.y, desc.hemi_color.z, desc.hemi_color.w);
		Data->SunColor.set(desc.sun_color.x, desc.sun_color.y, desc.sun_color.z, 1.0f);
		{
			Fvector D;
			Device->mView.transform_dir(D, desc.sun_dir);
			D.normalize();
			Data->SunDirE.set(D.x, D.y, D.z, 0);
		}
		Data->SunDirW.set(desc.sun_dir.x, desc.sun_dir.y, desc.sun_dir.z, 0.0f);
		{
			Fvector4 plane;
			Fmatrix& M = Device->mFullTransform;
			plane.x = -(M._14 + M._13);
			plane.y = -(M._24 + M._23);
			plane.z = -(M._34 + M._33);
			plane.w = -(M._44 + M._43);
			float denom = -1.0f / _sqrt(_sqr(plane.x) + _sqr(plane.y) + _sqr(plane.z));
			plane.mul(denom);

			// Near/Far
			
			float A = g_pGamePersistent->Environment().CurrentEnv->fog_near;
			float B = 1.0f / (g_pGamePersistent->Environment().CurrentEnv->fog_far - A);
			Data->FogPlane.set(-plane.x * B, -plane.y * B, -plane.z * B, 1.0f - (plane.w - A) * B); // view-plane
		}
		{
			float n = g_pGamePersistent->Environment().CurrentEnv->fog_near;
			float f = g_pGamePersistent->Environment().CurrentEnv->fog_far;
			float r = 1.0f / (f - n);
			Data->FogParam.set(-n * r, n, f, r);
		}
		{
			IEnvDescriptor& desc = *g_pGamePersistent->Environment().CurrentEnv;
			Data->FogColor.set(desc.fog_color.x, desc.fog_color.y, desc.fog_color.z, desc.fog_density);
		}
		{
			float VertTan = -1.0f * tanf(deg2rad(Device->fFOV / 2.0f));
			float HorzTan = -VertTan / Device->fASPECT;

			Data->PosDecompressParams.set(HorzTan, VertTan, (2.0f * HorzTan) / (float)Device->dwWidth, (2.0f * VertTan) / (float)Device->dwHeight);
		}
		{
			Data->EyePosition.set(Device->vCameraPosition.x, Device->vCameraPosition.y, Device->vCameraPosition.z, 1.f);
		}
		{
			float s = XRayRenderConsole::ps_r_bloom_threshold;								
			GRenderTarget->BloomFactor = .9f * GRenderTarget->BloomFactor + .1f *XRayRenderConsole::ps_r_bloom_speed * Device->fTimeDelta;
			Data->BloomParams.set( s, s, s, GRenderTarget->BloomFactor);
			
		}
		{
			Data->DetailTextureRange = 1.f / XRayRenderConsole::ps_r_detail_texture_range;
		}
		{
			float t = Device->fTimeGlobal;
			Data->Timer.set( t, t * 10.0f, t / 10.0f, _sin(t));
		}
		{
			auto CalcGauss_k7 = [](Fvector4& w0, Fvector4& w1, float r = 3.3f, float s_out = 1.f)
			{
				float				W[8];
				// calculate
				float mag = 0;
				for (int i = -7; i <= 0; i++)	W[-i] = expf(-float(i * i) / (2 * r * r));	// weight
				for (int i = 0; i < 8; i++)	mag += i ? 2 * W[i] : W[i];							// symmetrical weight
				for (int i = 0; i < 8; i++)	W[i] = s_out * W[i] / mag;

				// W[0]=0, W[7]=-7
				w0.set(W[1], W[2], W[3], W[4]);		// -1, -2, -3, -4
				w1.set(W[5], W[6], W[7], W[0]);		// -5, -6, -7, 0
			};
			auto	CalcGauss_wave = [&](Fvector4& w0, Fvector4& w1, float		r_base = 3.3f, float r_detail = 1.0f, float s_out = 1.f)
			{
				Fvector4	t0, t1;
				CalcGauss_k7(w0, w1, r_base, s_out);
				CalcGauss_k7(t0, t1, r_detail, s_out);
				w0.add(t0);
				w1.add(t1);
			};
			Fvector4	w0, w1;
			float		kernel = XRayRenderConsole::ps_r_bloom_kernel_g * float(Device->dwHeight) / float(Device->dwWidth);
			CalcGauss_wave(w0, w1, kernel, kernel / 3.f, XRayRenderConsole::ps_r_bloom_kernel_scale);
			Data->BloomW0 = w0;
			Data->BloomW1 = w1;
		}
		GUniformAllocator->GlobalUniform->Unlock();
	}
	for (size_t i = 0; i < XRayUniformAllocator::UT_Count; i++)
	{
		if (i == XRayUniformAllocator::UT_DetailScalar)continue;
		if (i == XRayUniformAllocator::UT_AlphaRef)continue;
		if (GUniformAllocator->Uniform[i].empty())continue;
		void* gptr = GUniformAllocator->Uniform[i]->Lock();
		m_UniformPtr = (u8*)gptr;
		if (i == XRayUniformAllocator::UT_Transformation)
		{
			{
				R_ASSERT(GUniformAllocator->Alloc(static_cast<XRayUniformAllocator::EUniformType>(i)) == XRayUniformAllocator::RT_Fidentity);
				u8* ptr = (u8*)gptr + GUniformAllocator->CalcOffset(static_cast<XRayUniformAllocator::EUniformType>(i), XRayUniformAllocator::RT_Fidentity);
				SetWorld(Fidentity);

				ContatMatrix* matrix = (ContatMatrix*)ptr;
				MWV.mul_43(m_MView, m_MWorld);
				MVPW.mul(m_MProject, MWV);
				matrix->View = m_MView;
				matrix->Project = m_MProject;
				matrix->World = m_MWorld;
				matrix->VP.mul(m_MProject, m_MView);
				matrix->VPW = MVPW;
				matrix->WV = MWV;
			}
			{
				R_ASSERT(GUniformAllocator->Alloc(static_cast<XRayUniformAllocator::EUniformType>(i)) == XRayUniformAllocator::RT_Sky);
				u8* ptr = (u8*)gptr + GUniformAllocator->CalcOffset(static_cast<XRayUniformAllocator::EUniformType>(i), XRayUniformAllocator::RT_Sky);


				Fmatrix mSky;
				mSky.rotateY(g_pGamePersistent->Environment().CurrentEnv->sky_rotation);
				mSky.translate_over(Device->vCameraPosition);
				SetWorld(mSky);

				ContatMatrix* matrix = (ContatMatrix*)ptr;
				MWV.mul_43(m_MView, m_MWorld);
				MVPW.mul(m_MProject, MWV);
				matrix->View = m_MView;
				matrix->Project = m_MProject;
				matrix->World = m_MWorld;
				matrix->VP.mul(m_MProject, m_MView);
				matrix->VPW = MVPW;
				matrix->WV = MWV;

			}
			{
				R_ASSERT(GUniformAllocator->Alloc(static_cast<XRayUniformAllocator::EUniformType>(i)) == XRayUniformAllocator::RT_Clouds);
				u8* ptr = (u8*)gptr + GUniformAllocator->CalcOffset(static_cast<XRayUniformAllocator::EUniformType>(i), XRayUniformAllocator::RT_Clouds);


				Fmatrix	mXFORM, mScale;
				mScale.scale(10, 0.4f, 10);

				mXFORM.rotateY(g_pGamePersistent->Environment().CurrentEnv->wind_direction); // wind_direction
				mXFORM.mulB_43(mScale);
				mXFORM.translate_over(Device->vCameraPosition);
				SetWorld(mXFORM);

				ContatMatrix* matrix = (ContatMatrix*)ptr;
				MWV.mul_43(m_MView, m_MWorld);
				MVPW.mul(m_MProject, MWV);
				matrix->View = m_MView;
				matrix->Project = m_MProject;
				matrix->World = m_MWorld;
				matrix->VP.mul(m_MProject, m_MView);
				matrix->VPW = MVPW;
				matrix->WV = MWV;
			}
		}
		std::function<void(XRayObjectRender*)> function = std::bind(&XRayRenderInterface::UniformUpdate, this, i, std::placeholders::_1);
		{
			Rdsgraph_Genderal.RenderObjects.setup(function);
			Rdsgraph_Genderal.RenderSortObjects.setup(function);
		}
		{
			extern ENGINE_API float		psHUD_FOV;
			Fmatrix Pold = Device->mProject;
			Fmatrix FTold = Device->mFullTransform;
			Device->mProject.build_projection(
				deg2rad(psHUD_FOV * Device->fFOV /* *Device->fASPECT*/),
				Device->fASPECT, VIEWPORT_NEAR,
				g_pGamePersistent->Environment().CurrentEnv->far_plane);

			Device->mFullTransform.mul(Device->mProject, Device->mView);
			SetProject(Device->mProject);
			Rdsgraph_Genderal.RenderHUDObjects.setup(function);

			Device->mProject = Pold;
			Device->mFullTransform = FTold;
			SetProject(Device->mProject);
		}
		GUniformAllocator->Uniform[i]->Unlock();
	}
}

void XRayRenderInterface::UniformSet(size_t i, XRayObjectRender* obj)
{
	if (i == XRayUniformAllocator::UT_GlobalUniform)
	{
		if (obj->UseGlobalUniform)
			obj->DescriptorHeap->SetUniformBuffer(GUniformAllocator->GetRegister(static_cast<XRayUniformAllocator::EUniformType>(i)), GUniformAllocator->GlobalUniform, 0);

	}
	else
	{
		if (obj->UseUniform[i])
			obj->DescriptorHeap->SetUniformBuffer(GUniformAllocator->GetRegister(static_cast<XRayUniformAllocator::EUniformType>(i)), GUniformAllocator->Uniform[i], obj->UniformID[i]);

	}
	
}

void XRayRenderInterface::UniformsSet()
{
	for (size_t i = 0; i < XRayUniformAllocator::UT_Count; i++)
	{
		std::function<void(XRayObjectRender*)> function = std::bind(&XRayRenderInterface::UniformSet, this, i, std::placeholders::_1);
		{
			Rdsgraph_Genderal.RenderObjects.setup(function);
			Rdsgraph_Genderal.RenderHUDObjects.setup(function);
			Rdsgraph_Genderal.RenderSortObjects.setup(function);
		}
	}
	{
		std::function<void(XRayObjectRender*)> function = std::bind(&XRayRenderInterface::UniformSet, this, XRayUniformAllocator::UT_GlobalUniform, std::placeholders::_1);
		{
			Rdsgraph_Genderal.RenderObjects.setup(function);
			Rdsgraph_Genderal.RenderHUDObjects.setup(function);
			Rdsgraph_Genderal.RenderSortObjects.setup(function);
		}
	}
}

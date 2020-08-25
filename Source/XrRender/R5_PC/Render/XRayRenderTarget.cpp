#include "pch.h"

XRayRenderTarget::XRayRenderTarget()
{
	BloomFactor = 0.5;
	m_bRenderHUD = false;

	{
		RTVBloom = GResourcesManager->GetTexture(RT_BLOOM);
		RTVBloom->Texture = BearRenderInterface::CreateTexture2D(RDEVICE.dwWidth, RDEVICE.dwHeight, BearRenderTargetFormat::R32G32B32A32F);

		RTVBloom1 = GResourcesManager->GetTexture(RT_BLOOM_1);
		RTVBloom1->Texture = BearRenderInterface::CreateTexture2D(RT_BLOOM_SIZE_X, RT_BLOOM_SIZE_Y, BearRenderTargetFormat::R32G32B32A32F);

		RTVBloom2 = GResourcesManager->GetTexture(RT_BLOOM_2);
		RTVBloom2->Texture = BearRenderInterface::CreateTexture2D(RT_BLOOM_SIZE_X, RT_BLOOM_SIZE_Y, BearRenderTargetFormat::R32G32B32A32F);

		RTVBloom3 = GResourcesManager->GetTexture(RT_BLOOM_3);
		RTVBloom3->Texture = BearRenderInterface::CreateTexture2D(RT_BLOOM_SIZE_X, RT_BLOOM_SIZE_Y, BearRenderTargetFormat::R32G32B32A32F);
	}


	RTVCompute = GResourcesManager->GetTexture(RT_COMPUTE);
	RTVCompute->Texture = BearRenderInterface::CreateTexture2D(RDEVICE.dwWidth, RDEVICE.dwHeight, BearRenderTargetFormat::R32G32B32A32F);

	RTVBasicColor = GResourcesManager->GetTexture(RT_BASIC);
	RTVBasicColor->Texture = BearRenderInterface::CreateTexture2D(RDEVICE.dwWidth, RDEVICE.dwHeight, BearRenderTargetFormat::R32G32B32A32F);
	DSVBasic = GResourcesManager->GetTexture(DS_BASIC);
	DSVBasic->Texture = BearRenderInterface::CreateTexture2D(RDEVICE.dwWidth, RDEVICE.dwHeight, BearDepthStencilFormat::Depth32FStencil8);

	RTVGeneric1 = GResourcesManager->GetTexture(RT_GENERIC1);
	RTVGeneric1->Texture = BearRenderInterface::CreateTexture2D(RDEVICE.dwWidth, RDEVICE.dwHeight, BearRenderTargetFormat::R32G32B32A32F);

	RTVGeneric2 = GResourcesManager->GetTexture(RT_GENERIC2);
	RTVGeneric2->Texture = BearRenderInterface::CreateTexture2D(RDEVICE.dwWidth, RDEVICE.dwHeight, BearRenderTargetFormat::R32G32B32A32F);

	{

		BearRenderPassDescription RenderPassDescription;
		RenderPassDescription.RenderTargets[0].Clear = true;
		RenderPassDescription.RenderTargets[0].Color = BearColor::Transparent;
		RenderPassDescription.RenderTargets[0].Format = BearRenderTargetFormat::R32G32B32A32F;
		RenderPassDescription.RenderTargets[1].Clear = true;
		RenderPassDescription.RenderTargets[1].Color = BearColor::Transparent;
		RenderPassDescription.RenderTargets[1].Format = BearRenderTargetFormat::R32G32B32A32F;
		RenderPassDescription.RenderTargets[2].Clear = true;
		RenderPassDescription.RenderTargets[2].Color = BearColor::Transparent;
		RenderPassDescription.RenderTargets[2].Format = BearRenderTargetFormat::R32G32B32A32F;
		RenderPassDescription.DepthStencil.Clear = true;
		RenderPassDescription.DepthStencil.Depth = 1.f;
		RenderPassDescription.DepthStencil.Stencil = 0;
		RenderPassDescription.DepthStencil.Format = BearDepthStencilFormat::Depth32FStencil8;
		RenderPass_Base = BearRenderInterface::CreateRenderPass(RenderPassDescription);


	}

	{
		BearFrameBufferDescription FrameBufferDescription;
		FrameBufferDescription.RenderPass = RenderPass_Base;
		FrameBufferDescription.RenderTargets[0] = RTVBasicColor->Texture;
		FrameBufferDescription.RenderTargets[1] = RTVBloom->Texture;
		FrameBufferDescription.RenderTargets[2] = RTVCompute->Texture;
		FrameBufferDescription.DepthStencil = DSVBasic->Texture;
		FrameBuffer_Base = BearRenderInterface::CreateFrameBuffer(FrameBufferDescription);
	}
	{

		BearRenderPassDescription RenderPassDescription;
		RenderPassDescription.RenderTargets[0].Clear = false;
		RenderPassDescription.RenderTargets[0].Color = BearColor::Transparent;
		RenderPassDescription.RenderTargets[0].Format = BearRenderTargetFormat::R32G32B32A32F;
		RenderPassDescription.DepthStencil.Clear = false;
		RenderPassDescription.DepthStencil.Format = BearDepthStencilFormat::Depth32FStencil8;
		RenderPass_Generic = BearRenderInterface::CreateRenderPass(RenderPassDescription);
	}
	{
		BearFrameBufferDescription FrameBufferDescription;
		FrameBufferDescription.RenderPass = RenderPass_Generic;
		FrameBufferDescription.RenderTargets[0] = RTVGeneric1->Texture;
		FrameBufferDescription.DepthStencil = DSVBasic->Texture;
		FrameBuffer_Generic1 = BearRenderInterface::CreateFrameBuffer(FrameBufferDescription);
	}
	{
		BearFrameBufferDescription FrameBufferDescription;
		FrameBufferDescription.RenderPass = RenderPass_Generic;
		FrameBufferDescription.RenderTargets[0] = RTVGeneric2->Texture;
		FrameBufferDescription.DepthStencil = DSVBasic->Texture;
		FrameBuffer_Generic2 = BearRenderInterface::CreateFrameBuffer(FrameBufferDescription);
	}
	{
		BearRenderPassDescription RenderPassDescription;
		RenderPassDescription.RenderTargets[0].Clear = true;
		RenderPassDescription.RenderTargets[0].Color = BearColor::Transparent;
		RenderPassDescription.RenderTargets[0].Format = BearRenderTargetFormat::R32G32B32A32F;
		RenderPass_Bloom = BearRenderInterface::CreateRenderPass(RenderPassDescription);
	}
	{
		BearFrameBufferDescription FrameBufferDescription;
		FrameBufferDescription.RenderPass = RenderPass_Bloom;
		FrameBufferDescription.RenderTargets[0] = RTVBloom1->Texture;
		FrameBuffer_Bloom_1 = BearRenderInterface::CreateFrameBuffer(FrameBufferDescription);
		
	}
	{
		BearFrameBufferDescription FrameBufferDescription;
		FrameBufferDescription.RenderPass = RenderPass_Bloom;
		FrameBufferDescription.RenderTargets[0] = RTVBloom2->Texture;
		FrameBuffer_Bloom_2 = BearRenderInterface::CreateFrameBuffer(FrameBufferDescription);

	}
	{
		BearFrameBufferDescription FrameBufferDescription;
		FrameBufferDescription.RenderPass = RenderPass_Bloom;
		FrameBufferDescription.RenderTargets[0] = RTVBloom3->Texture;
		FrameBuffer_Bloom_3 = BearRenderInterface::CreateFrameBuffer(FrameBufferDescription);

	}
	{
		m_ScreenSquare_VertexBuffer = BearRenderInterface::CreateVertexBuffer();
		m_ScreenSquare_VertexBuffer->Create(sizeof(FVF::TL), 6, true, 0);
		{
			FVF::TL* p = (FVF::TL*) m_ScreenSquare_VertexBuffer->Lock();
			p->set(0, 0, 0xFFFFFFFF, 0, 0); p++;
			p->set(int(RDEVICE.dwWidth), 0, 0xFFFFFFFF, 1, 0); p++;
			p->set(0, int(RDEVICE.dwHeight), 0xFFFFFFFF, 0, 1); p++;
			p->set(int(RDEVICE.dwWidth), 0, 0xFFFFFFFF, 1, 0); p++;
			p->set(int(RDEVICE.dwWidth), int(RDEVICE.dwHeight), 0xFFFFFFFF, 1, 1); p++;
			p->set(0, int(RDEVICE.dwHeight), 0xFFFFFFFF, 0, 1); p++;
			m_ScreenSquare_VertexBuffer->Unlock();
		}
	}
}

XRayRenderTarget::~XRayRenderTarget()
{

	GResourcesManager->Free(RTVBloom);
	GResourcesManager->Free(RTVBloom1);
	GResourcesManager->Free(RTVBloom2);
	GResourcesManager->Free(RTVBloom3);
	GResourcesManager->Free(RTVGeneric2);
	GResourcesManager->Free(RTVGeneric1);
	GResourcesManager->Free(RTVCompute);
	GResourcesManager->Free(RTVBasicColor);
	GResourcesManager->Free(DSVBasic);
}

void XRayRenderTarget::Flush()
{
	m_bRenderHUD = false;
}



void XRayRenderTarget::set_blur(float f)
{
}

void XRayRenderTarget::set_gray(float f)
{
}

void XRayRenderTarget::set_duality_h(float f)
{
}

void XRayRenderTarget::set_duality_v(float f)
{
}

void XRayRenderTarget::set_noise(float f)
{
}

void XRayRenderTarget::set_noise_scale(float f)
{
}

void XRayRenderTarget::set_noise_fps(float f)
{
}

void XRayRenderTarget::set_color_base(u32 f)
{
}

void XRayRenderTarget::set_color_gray(u32 f)
{
}

void XRayRenderTarget::set_color_add(const Fvector & f)
{
}

u32 XRayRenderTarget::get_width()
{
	return RDEVICE.dwWidth;
}

u32 XRayRenderTarget::get_height()
{
	return RDEVICE.dwHeight;
}

void XRayRenderTarget::set_cm_imfluence(float f)
{
}

void XRayRenderTarget::set_cm_interpolate(float f)
{
}

void XRayRenderTarget::set_cm_textures(const shared_str & tex0, const shared_str & tex1)
{
}


inline void RenderObject(XRayObjectRender* obj)
{
	obj->Render(HW->Context);
}

#include "../../xrEngine/CustomHUD.h"

void XRayRenderTarget::Render()
{
	m_bRenderHUD = false;
	HW->Context->Lock(GRenderTarget->FrameBuffer_Base);
	HW->Context->SetFrameBuffer(GRenderTarget->FrameBuffer_Base);
	{
		BearContextEventLock Event(*HW->Context.get(), "RenderGeom");
		RenderHUD();
		{
			BearContextEventLock Event(*HW->Context.get(), "RenderNotSort", BearColor::Green);
			GRenderInterface.Rdsgraph_Genderal.RenderObjects.setup(RenderObject);
		}
		{
			BearContextEventLock Event(*HW->Context.get(), "RenderLod");
			RenderLOD();
		}
		
		
	}
	
	HW->Context->ClearFrameBuffer();
	HW->Context->Unlock(GRenderTarget->FrameBuffer_Base);
	RenderCombine1();
	{
		BearContextEventLock Event(*HW->Context.get(), "RenderSort", BearColor::Blue);
		GRenderInterface.Rdsgraph_Genderal.RenderSortObjects.traverseRL(RenderObject);
	}
	RenderCombine2();
	m_bRenderHUD = true;
	HW->Context->BeginEvent("RenderGUI");
	if (g_pGamePersistent)	g_pGamePersistent->OnRenderPPUI_main();
}
void XRayRenderTarget::CompileShader()
{
	GResourcesManager->CompileBlender(m_Combine_Shader, TEXT("r5\\combine"), "");
	GResourcesManager->CompileBlender(m_Bloom_Shader, TEXT("r5\\bloom"),"");
}
void XRayRenderTarget::LoadLevel()
{
	GResourcesManager->CompileBlender(m_LOD_Shader, TEXT("details\\lod"), "level_lods");
}
void XRayRenderTarget::UnloadLevel()
{
	m_LOD_Shader.Clear();

}

extern float r_ssaLOD_A, r_ssaLOD_B;
inline bool  EqualShader(XRayShader&S1, XRayShader& S2)
{

	bool equal = true;
	for (size_t i = 0; i < 16; i++)
	{
		equal = equal && (S1.E[0][0].Textures[i] == S2.E[0][0].Textures[i]);
	}
	return equal&&(S1.Name == S2.Name);
}
void XRayRenderTarget::RenderLOD()
{
	if (GRenderInterface.Rdsgraph_Genderal.RenderLods.size() == 0)return;
	constexpr size_t Stride = sizeof(XRayFLOD::_hw);
	if (m_LOD_VertexBuffer.empty())
	{
		m_LOD_VertexBuffer = BearRenderInterface::CreateVertexBuffer();
		m_LOD_VertexBuffer->Create(Stride, 6 * bear_recommended_size(GRenderInterface.Rdsgraph_Genderal.RenderLods.size()), true);
	}
	else if (m_LOD_VertexBuffer->GetCount() < 6 * GRenderInterface.Rdsgraph_Genderal.RenderLods.size())
	{
		m_LOD_VertexBuffer->Create(Stride, 6 * bear_recommended_size(GRenderInterface.Rdsgraph_Genderal.RenderLods.size()), true);
	}
	XRayFLOD* First = 0;
	float ssaRange = r_ssaLOD_A - r_ssaLOD_B;
	if (ssaRange < EPS_S)	ssaRange = EPS_S;

	size_t offset = 0,count=0;
	HW->Context->SetVertexBuffer(m_LOD_VertexBuffer);

	XRayFLOD::_hw* pv = (XRayFLOD::_hw * )m_LOD_VertexBuffer->Lock();
	XRayFLOD::_hw* start_pv = pv;
	auto function = [&](XRayRdsgraphStructure::LodItem*item)->void 
	{
		if (First == nullptr)
		{
			if (!EqualShader(m_LOD_Shader, item->lod->Shader))
			{
				GRenderInterface.UpdateDescriptorHeap(m_LOD_Shader.E[0][0]);
				m_LOD_Shader.E[0][0].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation), GUniformAllocator->Uniform[XRayUniformAllocator::UT_Transformation], XRayUniformAllocator::RT_Fidentity);
				m_LOD_Shader.E[0][0].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform), GUniformAllocator->GlobalUniform, 0);
				if (m_LOD_Shader.E[0][0].Set(HW->Context, FVF::F_LOD))
				{
					if (m_LOD_Shader.E[0][0].StencilRef >= 0)
						HW->Context->SetStencilRef(m_LOD_Shader.E[0][0].StencilRef);
					HW->Context->Draw(count * 6, offset * 6);
				}
				First = item->lod;
				offset = count;
				count = 0;
			}
			
		}
		else
		{
			
			if (!EqualShader(First->Shader, item->lod->Shader))
			{
				GRenderInterface.UpdateDescriptorHeap(First->Shader.E[0][0]);
				First->Shader.E[0][0].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation), GUniformAllocator->Uniform[XRayUniformAllocator::UT_Transformation], XRayUniformAllocator::RT_Fidentity);
				First->Shader.E[0][0].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform), GUniformAllocator->GlobalUniform, 0);
				if (First->Shader.E[0][0].Set(HW->Context, FVF::F_LOD))
				{
					if (First->Shader.E[0][0].StencilRef >= 0)
						HW->Context->SetStencilRef(First->Shader.E[0][0].StencilRef);
					HW->Context->Draw(count * 6, offset * 6);
				}
				First = item->lod;
				offset = count;
				count = 0;

			}
		}
		float ssaDiff = item->ssa - r_ssaLOD_B;
		float scale = ssaDiff / ssaRange;
		int iA = iFloor((1 - scale) * 255.f);
		u32 uA = u32(clampr(iA, 0, 255));

		// calculate direction and shift
		XRayFLOD* lodV = item->lod;
		Fvector Ldir, shift;
		Ldir.sub(lodV->Vis.sphere.P, Device.vCameraPosition).normalize();
		shift.mul(Ldir, -.5f * lodV->Vis.sphere.R);

		// gen geometry
		XRayFLOD::_face* facets = lodV->facets;
		svector<std::pair<float, u32>, 8> selector;
		for (u32 s = 0; s < 8; s++)
		{
			selector.push_back(std::make_pair(Ldir.dotproduct(facets[s].N), s));
		}
		std::sort(selector.begin(), selector.end(), [](const std::pair<float, u32>& _1, const std::pair<float, u32>& _2) { return _1.first < _2.first; });

		float dot_best = selector[selector.size() - 1].first;
		float dot_next = selector[selector.size() - 2].first;
		float dot_next_2 = selector[selector.size() - 3].first;
		u32	  id_best = selector[selector.size() - 1].second;
		u32	  id_next = selector[selector.size() - 2].second;

		// Now we have two "best" planes, calculate factor, and approx normal
		float	fA = dot_best, fB = dot_next, fC = dot_next_2;
		float	alpha = 0.5f + 0.5f * (1 - (fB - fC) / (fA - fC));
		int		iF = iFloor(alpha * 255.5f);
		u32		uF = u32(clampr(iF, 0, 255));

		// Fill VB
		XRayFLOD::_face& FA = facets[id_best];
		XRayFLOD::_face& FB = facets[id_next];
		static int		vid[4] = { 3,0,2,1 };
		for (u32 vit = 0; vit < 4; vit++) {
			int			id = vid[vit];
			pv->p0.add(FB.v[id].v, shift);
			pv->p1.add(FA.v[id].v, shift);
			pv->n0 = FB.N;
			pv->n1 = FA.N;
			pv->sun_af = color_rgba(FB.v[id].c_sun, FA.v[id].c_sun, uA, uF);
			pv->t0 = FB.v[id].t;
			pv->t1 = FA.v[id].t;
			pv->rgbh0 = FB.v[id].c_rgb_hemi;
			pv->rgbh1 = FA.v[id].c_rgb_hemi;
			pv++;
		}
		memcpy(pv, pv - 2, Stride); pv++;
		memcpy(pv, pv - 4, Stride); pv++;
		count++;
	};


	GRenderInterface.Rdsgraph_Genderal.RenderLods.setup(function);
	m_LOD_VertexBuffer->Unlock();
	if (First == nullptr)
	{
		GRenderInterface.UpdateDescriptorHeap(m_LOD_Shader.E[0][0]);
		m_LOD_Shader.E[0][0].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation), GUniformAllocator->Uniform[XRayUniformAllocator::UT_Transformation], XRayUniformAllocator::RT_Fidentity);
		m_LOD_Shader.E[0][0].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform), GUniformAllocator->GlobalUniform, 0);
		if (m_LOD_Shader.E[0][0].Set(HW->Context, FVF::F_LOD))
		{
			if (m_LOD_Shader.E[0][0].StencilRef >= 0)
				HW->Context->SetStencilRef(m_LOD_Shader.E[0][0].StencilRef);
			HW->Context->Draw(count * 6, offset * 6);
		}
	}
	else
	{
		GRenderInterface.UpdateDescriptorHeap(First->Shader.E[0][0]);
		First->Shader.E[0][0].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation), GUniformAllocator->Uniform[XRayUniformAllocator::UT_Transformation], XRayUniformAllocator::RT_Fidentity);
		First->Shader.E[0][0].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform), GUniformAllocator->GlobalUniform, 0);
		if (First->Shader.E[0][0].Set(HW->Context, FVF::F_LOD))
		{
			if (First->Shader.E[0][0].StencilRef >= 0)
				HW->Context->SetStencilRef(First->Shader.E[0][0].StencilRef);
			HW->Context->Draw(count * 6, offset * 6);
		}

	}
}

void XRayRenderTarget::RenderHUD()
{
	BearContextEventLock Event(*HW->Context.get(), "RenderHUD", BearColor::Red);
	//	PIX_EVENT(r_dsgraph_render_hud);
	GRenderInterface.rmNear();
	GRenderInterface.Rdsgraph_Genderal.RenderHUDObjects.setup(RenderObject);
	if (g_hud && g_hud->RenderActiveItemUIQuery())
		g_hud->RenderActiveItemUI();
	GRenderInterface.rmNormal();
}





XRayRenderTarget* GRenderTarget = 0;
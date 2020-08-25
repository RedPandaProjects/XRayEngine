#include "pch.h"

XRayRenderDeviceRender::XRayRenderDeviceRender()
{

}

XRayRenderDeviceRender::~XRayRenderDeviceRender()
{

}

void XRayRenderDeviceRender::Copy(IRenderDeviceRender & _in)
{
	R_ASSERT(0);
}

float XRayRenderDeviceRender::GetGamma() const
{
	return 0.0f;
}

float XRayRenderDeviceRender::GetBrightness() const
{
	return 0.0f;
}

float XRayRenderDeviceRender::GetContrast() const
{
	return 0.0f;
}

Fvector XRayRenderDeviceRender::GetBalance() const
{
	return Fvector();
}

void XRayRenderDeviceRender::setGamma(float fGamma)
{
}

void XRayRenderDeviceRender::setBrightness(float fGamma)
{
}

void XRayRenderDeviceRender::setContrast(float fGamma)
{
}

void XRayRenderDeviceRender::setBalance(float r, float g, float b)
{
}

void XRayRenderDeviceRender::setBalance(Fvector& C)
{
}

void XRayRenderDeviceRender::updateGamma()
{
}




void XRayRenderDeviceRender::OnDeviceDestroy(BOOL bKeepTextures)
{
	Device.Statistic->OnDeviceDestroy();
	m_ViewportSet_Shader.Clear();
	m_ViewportSet_VertexBuffer.clear();
	GRenderInterface.destroy();
}

void XRayRenderDeviceRender::ValidateHW()
{
}

void XRayRenderDeviceRender::DestroyHW()
{
	/*xr_delete(GXRayRenderResource);*/
	xr_delete(HW);
	BearRenderInterface::Destroy();

	
}

void XRayRenderDeviceRender::Reset(HWND hWnd, u32& dwWidth, u32& dwHeight, float& fWidth_2, float& fHeight_2)
{
	HW->Update(hWnd,dwWidth, dwHeight, fWidth_2, fHeight_2);

	{

		FVF::TL* p = (FVF::TL*) m_ViewportSet_VertexBuffer->Lock();
		p->set(0, 0, 0xFFFFFFFF, 0, 0); p++;
		p->set(int(RDEVICE.dwWidth), 0, 0xFFFFFFFF, 1, 0); p++;
		p->set(0, int(RDEVICE.dwHeight), 0xFFFFFFFF, 0, 1); p++;
		p->set(int(RDEVICE.dwWidth), 0, 0xFFFFFFFF, 1, 0); p++;
		p->set(int(RDEVICE.dwWidth), int(RDEVICE.dwHeight), 0xFFFFFFFF, 1, 1); p++;
		p->set(0, int(RDEVICE.dwHeight), 0xFFFFFFFF, 0, 1); p++;
		m_ViewportSet_VertexBuffer->Unlock();
	}



	
}

void XRayRenderDeviceRender::SetupStates()
{
}

void XRayRenderDeviceRender::OnDeviceCreate(LPCSTR shName)
{
	GResourcesManager = xr_new<XRayResourcesManager>();
	GRenderInterface.create();
	GResourcesManager->LoadShaders(shName);
	GRenderTarget->CompileShader();
	m_ViewportSet_VertexBuffer = BearRenderInterface::CreateVertexBuffer();
	m_ViewportSet_VertexBuffer->Create(sizeof(FVF::TL), 6, true);
	{

		FVF::TL* p = (FVF::TL*) m_ViewportSet_VertexBuffer->Lock();
		p->set(0, 0, 0xFFFFFFFF, 0, 0); p++;

		p->set(int(RDEVICE.dwWidth), 0, 0xFFFFFFFF, 1, 0); p++;
		p->set(0, int(RDEVICE.dwHeight), 0xFFFFFFFF, 0, 1); p++;

		p->set(int(RDEVICE.dwWidth), 0, 0xFFFFFFFF, 1, 0); p++;
		p->set(int(RDEVICE.dwWidth), int(RDEVICE.dwHeight), 0xFFFFFFFF, 1, 1); p++;
		p->set(0, int(RDEVICE.dwHeight), 0xFFFFFFFF, 0, 1); p++;
		m_ViewportSet_VertexBuffer->Unlock();

	}


	GResourcesManager->CompileBlender(m_ViewportSet_Shader, "viewport\\set", RT_BASIC);
	Device.Statistic->OnDeviceCreate();
	/*GXRayRenderResource->LoadShaders(shName);*/

}

void XRayRenderDeviceRender::Create(HWND hWnd, u32& dwWidth, u32& dwHeight, float& fWidth_2, float& fHeight_2, bool)
{
	HW = xr_new<XRayHardware>();
	HW->Update(hWnd, dwWidth, dwHeight, fWidth_2, fHeight_2);
}




void XRayRenderDeviceRender::SetupGPU(BOOL bForceGPU_SW, BOOL bForceGPU_NonPure, BOOL bForceGPU_REF)
{
}

void XRayRenderDeviceRender::overdrawBegin()
{
}

void XRayRenderDeviceRender::overdrawEnd()
{
}

void XRayRenderDeviceRender::DeferredLoad(BOOL E)
{
}

void XRayRenderDeviceRender::ResourcesDeferredUpload()
{
}

void XRayRenderDeviceRender::ResourcesDestroyNecessaryTextures()
{
}

void XRayRenderDeviceRender::ResourcesStoreNecessaryTextures()
{
}


bool XRayRenderDeviceRender::HWSupportsShaderYUV2RGB()
{
	return true;
}

u32 XRayRenderDeviceRender::GetGPULoadPercent()
{
	return u32();
}

IRenderDeviceRender::DeviceState XRayRenderDeviceRender::GetDeviceState()
{
	return DeviceState();
}

BOOL XRayRenderDeviceRender::GetForceGPU_REF()
{
	return 0;
}

u32 XRayRenderDeviceRender::GetCacheStatPolys()
{
	return 0;
}

void XRayRenderDeviceRender::Begin()
{
	HW->Context->Reset();
	HW->Context->BeginEvent(TEXT("RenderBase"));
	HW->Context->SetViewport(0, 0, static_cast<float>(RDEVICE.dwWidth), static_cast<float>(RDEVICE.dwHeight));
	HW->Context->SetScissor(false, 0, 0, 0, 0);
	HW->Context->Lock(GRenderTarget->FrameBuffer_Generic2);
}

void XRayRenderDeviceRender::Clear()
{
	
}

void XRayRenderDeviceRender::End()
{
	if (GRenderTarget->IsRenderHUD())HW->Context->EndEvent();
	HW->Context->ClearFrameBuffer();
	HW->Context->Unlock(GRenderTarget->FrameBuffer_Generic2);
	
	HW->Context->Lock(HW->Viewport);
	HW->Context->SetViewportAsFrameBuffer(HW->Viewport);

	HW->Context->SetVertexBuffer(m_ViewportSet_VertexBuffer);
	GRenderInterface.UpdateDescriptorHeap(m_ViewportSet_Shader.E[0][0]);
	if (!m_ViewportSet_Shader.E[0][0].Set(HW->Context, FVF::F_TL)) { return; }
	HW->Context->Draw(6);

	HW->Context->Unlock(HW->Viewport);
	HW->Context->EndEvent();
	HW->Context->Flush(HW->Viewport,true);


	GUIRender.Flush();
	GRenderInterface.Flush();
	for (auto b = HW->FontRenders.begin(), e = HW->FontRenders.end(); b != e; b++)
	{
		(*b)->Flush();
	}
}

void XRayRenderDeviceRender::ClearTarget()
{
}

void XRayRenderDeviceRender::SetCacheXform( Fmatrix& mView,  Fmatrix& mProject)
{
	GRenderInterface.SetView(mView);
	GRenderInterface.SetProject(mProject);
}


void XRayRenderDeviceRender::OnAssetsChanged()
{
}

void XRayRenderDeviceRender::ResourcesDumpMemoryUsage()
{
}

void XRayRenderDeviceRender::ResourcesGetMemoryUsage(u32& m_base, u32& c_base, u32& m_lmaps, u32& c_lmaps)
{
}

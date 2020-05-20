#include "pch.h"
XRayHardware* HW = nullptr;
extern const char* GGraphicsAPI[];
size_t GCurrentApi = 0;
#include "../../xrEngine/XR_IOConsole.h"
XRayHardware::XRayHardware()
{
	R_ASSERT(BearRenderInterface::Initialize(GGraphicsAPI[GCurrentApi]));
	Context = BearRenderInterface::CreateContext();
	/*ContextHUD = BearRenderInterface::CreateContext();
	ContextCombine = BearRenderInterface::CreateContext();
	ContextSky = BearRenderInterface::CreateContext();*/
}

XRayHardware::~XRayHardware()
{
	RenderPass_Viewport.clear();
	Viewport.clear();
	Context.clear();
	BearRenderInterface::Destroy();
}

void XRayHardware::Update(HWND hWnd, u32& dwWidth, u32& dwHeight, float& fWidth_2, float& fHeight_2)
{
	if (psDeviceFlags.is(rsFullscreen))
	{
		dwWidth = psCurrentVidMode[0];
		dwHeight = psCurrentVidMode[1];
	}
	else
	{
	/*	string64 buff;
		xr_sprintf(buff, sizeof(buff), "%dx%d", psCurrentVidMode[0], psCurrentVidMode[1]);

		if (_ParseItem(buff, vid_mode_token) == u32(-1)) //not found
		{ //select safe
			xr_sprintf(buff, sizeof(buff), "vid_mode %s", vid_mode_token[0].name);
			Console->Execute(buff);
		}*/

		dwWidth = 0x400;
		dwHeight = 0x300;
	}

	if (Viewport.empty())
	{
		BearViewportDescription Description;
		Description.Clear = true;
		Description.ClearColor = BearColor::Black;
		Viewport = BearRenderInterface::CreateViewport(hWnd, dwWidth, dwHeight, psDeviceFlags.is(rsFullscreen), Description);
		BearRenderPassDescription RenderPassDescription;
		RenderPassDescription.RenderTargets[0].Format = Viewport->GetFormat();
		RenderPass_Viewport = BearRenderInterface::CreateRenderPass(RenderPassDescription);
	}
	else
	{
		Viewport->Resize(dwWidth, dwHeight);
		Viewport->SetFullScreen(psDeviceFlags.is(rsFullscreen));
	}
	

	fWidth_2 = float(dwWidth / 2);
	fHeight_2 = float(dwHeight / 2);
}

bool XRayHardware::MeshShaderSupport() const
{
	return  XRayRenderConsole::ps_r_mesh_shader_enable&&BearRenderInterface::MeshShaderSupport();
}

bool XRayHardware::DevOldShaderSupport() const
{
	return true;
}

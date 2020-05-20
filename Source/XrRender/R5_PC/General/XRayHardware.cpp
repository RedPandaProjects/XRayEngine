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
void selectResolution(u32& dwWidth, u32& dwHeight, BOOL bWindowed)
{

	if (bWindowed)
	{
		dwWidth = psCurrentVidMode[0];
		dwHeight = psCurrentVidMode[1];
	}
	else //check
	{
		string64					buff;
		xr_sprintf(buff, sizeof(buff), "%dx%d", psCurrentVidMode[0], psCurrentVidMode[1]);

		if (_ParseItem(buff, vid_mode_token) == u32(-1)) //not found
		{ //select safe
			xr_sprintf(buff, sizeof(buff), "vid_mode %s", vid_mode_token[0].name);
			Console->Execute(buff);
		}

		dwWidth = psCurrentVidMode[0];
		dwHeight = psCurrentVidMode[1];
	}
}
void XRayHardware::Update(HWND hWnd, u32& dwWidth, u32& dwHeight, float& fWidth_2, float& fHeight_2)
{
	BOOL	bWindowed = !psDeviceFlags.is(rsFullscreen);

	selectResolution(dwWidth, dwHeight, bWindowed);

	{
		u32		dwWindowStyle = 0;
		// Set window properties depending on what mode were in.
		if (bWindowed)
		{
			if (strstr(Core.Params, "-no_dialog_header"))
				SetWindowLong(hWnd, GWL_STYLE, dwWindowStyle = (WS_BORDER | WS_VISIBLE));
			else
				SetWindowLong(hWnd, GWL_STYLE, dwWindowStyle = (WS_BORDER | WS_DLGFRAME | WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX));
			// When moving from fullscreen to windowed mode, it is important to
			// adjust the window size after recreating the device rather than
			// beforehand to ensure that you get the window size you want.  For
			// example, when switching from 640x480 fullscreen to windowed with
			// a 1000x600 window on a 1024x768 desktop, it is impossible to set
			// the window size to 1000x600 until after the display mode has
			// changed to 1024x768, because windows cannot be larger than the
			// desktop.

			RECT			m_rcWindowBounds;
			BOOL			bCenter = TRUE;
			if (strstr(Core.Params, "-center_screen"))	bCenter = TRUE;

			if (bCenter)
			{
				RECT				DesktopRect;

				GetClientRect(GetDesktopWindow(), &DesktopRect);

				SetRect(&m_rcWindowBounds,
					(DesktopRect.right - dwWidth) / 2,
					(DesktopRect.bottom - dwHeight) / 2,
					(DesktopRect.right + dwWidth) / 2,
					(DesktopRect.bottom + dwHeight) / 2);
			}
			else
			{
				SetRect(&m_rcWindowBounds,
					0,
					0,
					dwWidth,
					dwHeight);
			}
			AdjustWindowRect(&m_rcWindowBounds, dwWindowStyle, FALSE);

			SetWindowPos(hWnd,
				HWND_NOTOPMOST,
				m_rcWindowBounds.left,
				m_rcWindowBounds.top,
				(m_rcWindowBounds.right - m_rcWindowBounds.left),
				(m_rcWindowBounds.bottom - m_rcWindowBounds.top),
				SWP_SHOWWINDOW | SWP_NOCOPYBITS | SWP_DRAWFRAME);
		}
		
		else
		{
			SetWindowLong(hWnd, GWL_STYLE, dwWindowStyle = (WS_POPUP | WS_VISIBLE));
		}

		ShowCursor(FALSE);
		SetForegroundWindow(hWnd);
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

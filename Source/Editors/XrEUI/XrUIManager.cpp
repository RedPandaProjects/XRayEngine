#include "stdafx.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "spectrum.h"


XrUIManager::XrUIManager()
{
}

XrUIManager::~XrUIManager()
{
}

void XrUIManager::Initialize(HWND hWnd, IDirect3DDevice9* device, const char* ini_path)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    xr_strcpy(m_name_ini, ini_path);
    io.IniFilename = m_name_ini;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    // Setup Dear ImGui style
    ImGui::StyleColorsClassic();
   // ImGui::Spectrum::LoadFont();
    ImGui::GetStyle().ItemSpacing = ImVec2(3,3);
    ImGui::GetStyle().FramePadding = ImVec2(3,1);
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX9_Init(device);

}

void XrUIManager::Destroy()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void XrUIManager::ResetBegin()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();

}

void XrUIManager::ResetEnd()
{
    ImGui_ImplDX9_CreateDeviceObjects();
}


void XrUIManager::OnDrawUI()
{
}

void XrUIManager::ApplyShortCut(DWORD Key)
{
    if ((ImGui::GetIO().WantTextInput))return;
	bool IsFail = true;
	if (Key >= 'A' && Key <= 'Z')
	{
		IsFail = false;
	}
	else if (Key >= '0' && Key <= '9')
	{
		IsFail = false;
	}
    else
    {
        switch (Key)
        {
        case VK_LEFT:
        case VK_RIGHT:
        case VK_UP:
        case VK_DOWN:
        case VK_NUMPAD0:
        case VK_NUMPAD1:
        case VK_NUMPAD2:
        case VK_NUMPAD3:
        case VK_NUMPAD4:
        case VK_NUMPAD5:
        case VK_NUMPAD6:
        case VK_NUMPAD7:
        case VK_NUMPAD8:
        case VK_NUMPAD9:
        case VK_F1:
        case VK_F2:
        case VK_F3:
        case VK_F4:
        case VK_F5:
        case VK_F6:
        case VK_F7:
        case VK_F8:
        case VK_F9:
        case VK_F10:
        case VK_F11:
        case VK_F12:
        case VK_DELETE:
        case VK_ADD:
        case VK_SUBTRACT:
        case VK_MULTIPLY:
        case VK_DIVIDE:
        case VK_OEM_PLUS:
        case VK_OEM_MINUS:
        case VK_OEM_1:
        case VK_OEM_COMMA:
        case VK_OEM_PERIOD:
        case VK_OEM_2:
        case VK_OEM_4:
        case VK_OEM_5:
        case VK_OEM_6:
        case VK_OEM_7:
        case VK_SPACE:
        case VK_CANCEL:
        case VK_RETURN:
            IsFail = false;
            break;
        default:
            break;
        }
    }
    if (IsFail)return;

    int ShiftState = ssNone;

    if (ImGui::GetIO().KeyShift)ShiftState |= ssShift;
    if (ImGui::GetIO().KeyCtrl)ShiftState |= ssCtrl;
    if (ImGui::GetIO().KeyAlt)ShiftState |= ssAlt;


    if (ImGui::IsMouseDown(ImGuiMouseButton_Left))ShiftState |= ssLeft;
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right))ShiftState |= ssRight;
    ApplyShortCut(Key, ShiftState);
}



void XrUIManager::Push(XrUI* ui, bool need_deleted)
{
	m_UIArray.push_back(ui);
	ui->Flags.set(!need_deleted, XrUI::F_NoDelete);
}

void XrUIManager::Draw()
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
   // ImGui::DockSpaceOverViewport();
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos( ImVec2(viewport->Pos.x, viewport->Pos.y+ UIToolBarSize));
        ImGui::SetNextWindowSize( ImVec2(viewport->Size.x, viewport->Size.y- UIToolBarSize));
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGuiWindowFlags window_flags = 0
            | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
            | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
            | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 2));
        ImGui::Begin("Master DockSpace", NULL, window_flags);
        ImGuiID dockMain = ImGui::GetID("MyDockspace");

        m_MenuBarHeight = ImGui::GetWindowBarHeight();
        // Save off menu bar height for later.

        ImGui::DockSpace(dockMain);
        ImGui::End();
        ImGui::PopStyleVar(4);

    }
	for (XrUI* ui : m_UIArray)
	{
		ui->Draw();
	}

    OnDrawUI();
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	for (size_t i = m_UIArray.size(); i > 0; i--)
	{
		if (m_UIArray[i - 1]->IsClosed())
		{
			if (!m_UIArray[i - 1]->Flags.test(XrUI::F_NoDelete))
			{
				xr_delete(m_UIArray[i - 1]);
			}
			m_UIArray.erase(m_UIArray.begin() + (i - 1));
			i = m_UIArray.size();
			if (i == 0)return;
		}
	}
}
static bool ImGui_ImplWin32_UpdateMouseCursor()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return false;

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
    {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        ::SetCursor(NULL);
    }
    else
    {
        // Show OS mouse cursor
        LPTSTR win32_cursor = IDC_ARROW;
        switch (imgui_cursor)
        {
        case ImGuiMouseCursor_Arrow:        win32_cursor = IDC_ARROW; break;
        case ImGuiMouseCursor_TextInput:    win32_cursor = IDC_IBEAM; break;
        case ImGuiMouseCursor_ResizeAll:    win32_cursor = IDC_SIZEALL; break;
        case ImGuiMouseCursor_ResizeEW:     win32_cursor = IDC_SIZEWE; break;
        case ImGuiMouseCursor_ResizeNS:     win32_cursor = IDC_SIZENS; break;
        case ImGuiMouseCursor_ResizeNESW:   win32_cursor = IDC_SIZENESW; break;
        case ImGuiMouseCursor_ResizeNWSE:   win32_cursor = IDC_SIZENWSE; break;
        case ImGuiMouseCursor_Hand:         win32_cursor = IDC_HAND; break;
        case ImGuiMouseCursor_NotAllowed:   win32_cursor = IDC_NO; break;
        }
        ::SetCursor(::LoadCursor(NULL, win32_cursor));
    }
    return true;
}
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif
#ifndef DBT_DEVNODES_CHANGED
#define DBT_DEVNODES_CHANGED 0x0007
#endif

LRESULT XrUIManager::WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        switch (wParam)
        {
        case VK_MENU:
        case VK_CONTROL:
        case VK_SHIFT:
            break;
        default:
            ApplyShortCut(wParam);
            break;
        }
    default:
        break;
    }
    return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);;
}
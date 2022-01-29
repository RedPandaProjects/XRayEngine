// LevelEditor.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"
#include "..\..\XrAPI\xrGameManager.h"
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    if(!IsDebuggerPresent()) Debug._initialize(false);
    const char* FSName = "fs.ltx";
    {
		if (strstr(GetCommandLine(), "-soc_14") || strstr(GetCommandLine(), "-soc_10004"))
		{
            FSName = "fs_soc.ltx";
		}
		else if (strstr(GetCommandLine(), "-soc"))
		{
            FSName = "fs_soc.ltx";
		}
		else if (strstr(GetCommandLine(), "-cs"))
		{
            FSName = "fs_cs.ltx";
		}
    }
    Core._initialize("LevelEditor", ELogCallback, 1, FSName, true);
    g_SEFactoryManager = xr_new<XrGameManager>();
    Tools = xr_new<CLevelTool>();
    LTools = (CLevelTool*)Tools;

    UI = xr_new<CLevelMain>();
    UI->RegisterCommands();
    LUI = (CLevelMain*)UI;

    Scene = xr_new<EScene>();

    UIMainForm* MainForm = xr_new< UIMainForm>();
    ::MainForm = MainForm;
    UI->Push(MainForm, false);
    while (MainForm->Frame())
    {
    }
    xr_delete(MainForm);
    xr_delete(g_SEFactoryManager);
    Core._destroy();
    return 0;
}

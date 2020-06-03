// LevelEditor.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Core._initialize("Actor", ELogCallback, 1, "fs.ltx", true);

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
    Core._destroy();
    return 0;
}

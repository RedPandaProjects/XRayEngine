// ActorEditor.cpp : Определяет точку входа для приложения.
//
#include "stdafx.h"
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    if (!IsDebuggerPresent()) Debug._initialize(false);
    Core._initialize("Actor", ELogCallback,1,"fs.ltx",true);

    Tools = xr_new<CActorTools>();
    ATools = (CActorTools*)Tools;
    UI = xr_new<CActorMain>();
    UI->RegisterCommands();

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

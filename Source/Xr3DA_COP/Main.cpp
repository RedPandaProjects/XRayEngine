#include "..\XrEngine\stdafx.h"
extern "C"
{
	DLL_API void __cdecl xrGameInitialize();
}
//ENGINE_API int EngineLaunch(EGamePath Game);
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,char* lpCmdLine,int       nCmdShow)
{
	xrGameInitialize();
	return		0;//EngineLaunch(EGamePath::COP_1602);
}
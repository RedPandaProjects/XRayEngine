#include "..\XrEngine\stdafx.h"

ENGINE_API int EngineLaunch(EGamePath Game);
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,char* lpCmdLine,int       nCmdShow)
{
	return		EngineLaunch(EGamePath::COP_1602);
}
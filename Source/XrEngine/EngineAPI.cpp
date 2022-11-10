// EngineAPI.cpp: implementation of the CEngineAPI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EngineAPI.h"
#include "../xrcdb/xrXRC.h"
#include "../XrAPI/xrGameManager.h"
#include "securom_api.h"
#include "..\XrAPI\xrGameManager.h"
#include "device.h"
extern xr_token* vid_quality_token;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __cdecl dummy		(void)	{
};
CEngineAPI::CEngineAPI	()
{
#ifndef SHIPPING
	hGame			= 0;
	hRender			= 0;
	hTuner			= 0;
#endif
	pCreate			= 0;
	pDestroy		= 0;
	tune_pause		= dummy	;
	tune_resume		= dummy	;
}

CEngineAPI::~CEngineAPI()
{
	// destroy quality token here
	if (vid_quality_token)
	{
		for( int i=0; vid_quality_token[i].name; i++ )
		{
			xr_free					(vid_quality_token[i].name);
		}
		xr_free						(vid_quality_token);
		vid_quality_token			= NULL;
	}
}

extern u32 renderer_value; //con cmd
ENGINE_API int g_current_renderer = 0;

ENGINE_API bool is_enough_address_space_available	()
{
	SYSTEM_INFO		system_info;

	SECUROM_MARKER_HIGH_SECURITY_ON(12)

	GetSystemInfo	( &system_info );

	SECUROM_MARKER_HIGH_SECURITY_OFF(12)

	return			(*(u32*)&system_info.lpMaximumApplicationAddress) > 0x90000000;	
}

#ifndef DEDICATED_SERVER

void CEngineAPI::InitializeNotDedicated()
{

	g_current_renderer = 2;
}
#endif // DEDICATED_SERVER

#ifdef SHIPPING

extern "C"  DLL_Pure * __cdecl xrFactory_Create(CLASS_ID clsid);
extern "C" void		__cdecl	xrFactory_Destroy(DLL_Pure * O);
extern "C" bool SupportsRendering();
extern "C" void InitializeRendering();
#else
extern "C" {
	typedef bool  SupportsRendering();
	typedef void  InitializeRendering();
	typedef void  xrGameInitialize();
};
#endif
void CEngineAPI::Initialize(void)
{
	//////////////////////////////////////////////////////////////////////////
	// render
#ifdef SHIPPING
	//InitializeNotDedicated();
	InitializeRendering();
	pCreate = &xrFactory_Create;
	pDestroy = &xrFactory_Destroy;

#else
#ifdef _DEBUG
	LPCSTR			r1_name	= "xrRender_R1_Debug.dll";
#else
	LPCSTR			r1_name = "xrRender_R1_Release.dll";
#endif
	#ifndef DEDICATED_SERVER
		InitializeNotDedicated();
	#endif // DEDICATED_SERVER



	//Device->ConnectToRender();
	// game	
	{
#ifdef _DEBUG
		LPCSTR			g_name = "XrGame_Debug.dll";
#else
		LPCSTR			g_name = "XrGame_Release.dll";
#endif
		
		switch (xrGameManager::GetGame())
		{
		case EGame::CS:
			g_name = "XrGameCS.dll";
			break;
		case EGame::SHOC:
			g_name = "XrGameSOC.dll";
			break;
		}

		Log				("Loading DLL:",g_name);
		hGame			= LoadLibrary	(g_name);
		if (0==hGame)	R_CHK			(GetLastError());
		R_ASSERT2		(hGame,"Game DLL raised exception during loading or there is no game DLL at all");
		pCreate			= (Factory_Create*)		GetProcAddress(hGame,"xrFactory_Create"		);	R_ASSERT(pCreate);
		pDestroy		= (Factory_Destroy*)		GetProcAddress(hGame,"xrFactory_Destroy"		);	R_ASSERT(pDestroy);
		xrGameInitialize* pxrGameInitialize = (xrGameInitialize*)	GetProcAddress(hGame,"xrGameInitialize"	);	R_ASSERT(pxrGameInitialize);
		pxrGameInitialize();
	}

	//////////////////////////////////////////////////////////////////////////
	// vTune
	tune_enabled		= FALSE;
	if (strstr(Core.Params,"-tune"))	{
		LPCSTR			g_name	= "vTuneAPI.dll";
		Log				("Loading DLL:",g_name);
		hTuner			= LoadLibrary	(g_name);
		if (0==hTuner)	R_CHK			(GetLastError());
		R_ASSERT2		(hTuner,"Intel vTune is not installed");
		tune_enabled	= TRUE;
		tune_pause		= (VTPause*)	GetProcAddress(hTuner,"VTPause"		);	R_ASSERT(tune_pause);
		tune_resume		= (VTResume*)	GetProcAddress(hTuner,"VTResume"	);	R_ASSERT(tune_resume);
	}
#endif
}

void CEngineAPI::Destroy	(void)
{
#ifndef SHIPPING
	if (hGame)				{ FreeLibrary(hGame);	hGame	= 0; }
	if (hRender)			{ FreeLibrary(hRender); hRender = 0; }
#endif
	pCreate					= 0;
	pDestroy				= 0;
	Engine->Event._destroy	();
	XRC.r_clear_compact		();
}



void CEngineAPI::CreateRendererList()
{
}
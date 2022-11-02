// xrCore.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#pragma hdrstop

#include <mmsystem.h>
#include <objbase.h>
#include "xrCore.h"
#include "..\XrAPI\xrGameManager.h"
#pragma comment(lib,"winmm.lib")

#ifdef DEBUG
#	include	<malloc.h>
#endif // DEBUG

XRCORE_API		xrCore	Core;
XRCORE_API		u32		build_id;
XRCORE_API		LPCSTR	build_date;

namespace CPU
{
	extern	void			Detect	();
};

static u32	init_counter	= 0;
extern XRayDebugInterface* GDebugInterface;
extern XRayLogInterface* GLogInterface;
XRAPI_API extern EGamePath GCurrentGame;
//. extern xr_vector<shared_str>*	LogFile;
extern xrCriticalSection* LogCS;
void xrCore::Initialize	(XRayMemoryInterface* InMemoryInterface, XRayLogInterface* InLogInterface, XRayDebugInterface* InDebugInterface, LPCSTR FSName, bool IsEditor ,EGamePath Game)
{

	MemoryInterface = InMemoryInterface;
	GLogInterface = InLogInterface;
	GDebugInterface = InDebugInterface;

	xr_strcpy					(ApplicationName,"XRay2UnrealEnigne");
	if (0==init_counter) 
	{
		g_pStringContainer = xr_new<str_container>();
		g_pSharedMemoryContainer = xr_new<smem_container>();
		LogCS = xr_new < xrCriticalSection>();
		if (Game == EGamePath::NONE)
		{
			if (strstr(GetCommandLine(), "-soc_14") || strstr(GetCommandLine(), "-soc_10004"))
			{
				GCurrentGame = EGamePath::SHOC_10004;
			}
			else if (strstr(GetCommandLine(), "-soc"))
			{
				GCurrentGame = EGamePath::SHOC_10006;
			}
			else if (strstr(GetCommandLine(), "-cs"))
			{
				GCurrentGame = EGamePath::CS_1510;
			}
			else
			{
				GCurrentGame = EGamePath::COP_1602;
			}
		}
		else
		{
			GCurrentGame = Game;
		}

		Editor = IsEditor;
#ifdef XRCORE_STATIC	
		_clear87	();
		_control87	( _PC_53,   MCW_PC );
		_control87	( _RC_CHOP, MCW_RC );
		_control87	( _RC_NEAR, MCW_RC );
		_control87	( _MCW_EM,  MCW_EM );
#endif
		// Init COM so we can use CoCreateInstance
//		HRESULT co_res = 
		if (!strstr(GetCommandLine(),"-editor"))
			CoInitializeEx	(NULL, COINIT_MULTITHREADED);
	
		xr_strcpy			(Params,sizeof(Params),GetCommandLine());
		_strlwr_s			(Params,sizeof(Params));

		string_path		fn,dr,di;

		// application path
        GetModuleFileName(GetModuleHandle(MODULE_NAME),fn,sizeof(fn));
        _splitpath		(fn,dr,di,0,0);
        strconcat		(sizeof(ApplicationPath),ApplicationPath,dr,di);

#if 0
		// working path
        if( strstr(Params,"-wf") )
        {
            string_path				c_name;
            sscanf					(strstr(Core.Params,"-wf ")+4,"%[^ ] ",c_name);
            SetCurrentDirectory     (c_name);
        }
#endif

		GetCurrentDirectory(sizeof(WorkingPath),WorkingPath);

		// User/Comp Name
		DWORD	sz_user		= sizeof(UserName);
		GetUserName			(UserName,&sz_user);

		DWORD	sz_comp		= sizeof(CompName);
		GetComputerName		(CompName,&sz_comp);

		// Mathematics & PSI detection
		CPU::Detect			();
		

		_initialize_cpu		();

//		Debug._initialize	();

		rtc_initialize		();

		if(IsEditor)
			xr_FS	= xr_new<ELocatorAPI>();
		else
			xr_FS	= xr_new<CLocatorAPI>();

		xr_EFS				= xr_new<EFS_Utils>		();
//.		R_ASSERT			(co_res==S_OK);
	}
	if (true)
	{
		u32 flags			= 0;
		if (0!=strstr(Params,"-build"))	 flags |= CLocatorAPI::flBuildCopy;
		if (0!=strstr(Params,"-ebuild")) flags |= CLocatorAPI::flBuildCopy|CLocatorAPI::flEBuildCopy;
#ifdef DEBUG
		if (strstr(Params,"-cache"))  flags |= CLocatorAPI::flCacheFiles;
		else flags &= ~CLocatorAPI::flCacheFiles;
#endif // DEBUG
#if 0 // for EDITORS - no cache
		flags 				&=~ CLocatorAPI::flCacheFiles;
#endif // _EDITOR
		flags |= CLocatorAPI::flScanAppRoot;

#if 1
	#ifndef ELocatorAPIH
		if (0!=strstr(Params,"-file_activity"))	 flags |= CLocatorAPI::flDumpFileActivity;
	#endif
#endif
		FS._initialize		(flags,0,FSName);
		Msg					("'%s' build %d, %s\n","xrCore",build_id, build_date);
		EFS._initialize		();
#ifdef DEBUG
    #if 1
		Msg					("CRT heap 0x%08x",_get_heap_handle());
		Msg					("Process heap 0x%08x",GetProcessHeap());
    #endif
#endif // DEBUG
	}
	SetLogCB				(nullptr);
	init_counter++;
}

#if 1
#include "compression_ppmd_stream.h"
extern compression::ppmd::stream	*trained_model;
#endif
void xrCore::Destroy		()
{
	--init_counter;
	if (0==init_counter){
		FS._destroy			();
		EFS._destroy		();
		xr_delete			(xr_FS);
		xr_delete			(xr_EFS);

#if 1
		if (trained_model) {
			void			*buffer = trained_model->buffer();
			xr_free			(buffer);
			xr_delete		(trained_model);
		}
#endif
		xr_delete(g_pStringContainer);
		xr_delete(g_pSharedMemoryContainer);
		xr_delete(LogCS);

	}
}

#ifndef SHIPPING

//. why ??? 
#if 0
	BOOL WINAPI DllEntryPoint(HINSTANCE hinstDLL, DWORD ul_reason_for_call, LPVOID lpvReserved)
#else
	BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD ul_reason_for_call, LPVOID lpvReserved)
#endif
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
		}
//.		LogFile.reserve		(256);
		break;
	case DLL_THREAD_ATTACH:
		//if (!strstr(GetCommandLine(),"-editor"))
			//CoInitializeEx	(NULL, COINIT_MULTITHREADED);
		timeBeginPeriod	(1);
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
#ifdef USE_MEMORY_MONITOR
		memory_monitor::flush_each_time	(true);
#endif // USE_MEMORY_MONITOR
		break;
	}
    return TRUE;
}
#endif // XRCORE_STATIC
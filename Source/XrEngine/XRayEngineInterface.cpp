//-----------------------------------------------------------------------------
// File: x_ray.cpp
//
// Programmers:
//	Oles		- Oles Shishkovtsov
//	AlexMX		- Alexander Maksimchuk
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "igame_level.h"
#include "igame_persistent.h"

#include "dedicated_server_only.h"
#include "no_single.h"
#include "../xrNetServer/NET_AuthCheck.h"

#include "xr_input.h"
#include "xr_ioconsole.h"
#include "XRayEngineInterface.h"
#include "std_classes.h"
#include "GameFont.h"
#include "resource.h"
#include "LightAnimLibrary.h"
#include "../xrcdb/ispatial.h"
#include "CopyProtection.h"
#include "Text_Console.h"
#include <process.h>
#include <locale.h>

#include "xrSash.h"

#include "securom_api.h"
#include "..\XrAPI\xrGameManager.h"
#include "GameMtlLib.h"
#include "xr_ioc_cmd.h"

//////////////////////////////////////////////////////////////////////////
// global variables
ENGINE_API	XRayEngineInterface* g_Engine = NULL;

//---------------------------------------------------------------------
ENGINE_API CInifile*		pGameIni		= NULL;
// computing build id
XRCORE_API extern	LPCSTR	build_date;
XRCORE_API extern u32		build_id;

#ifdef MASTER_GOLD
#	define NO_MULTI_INSTANCES
#endif // #ifdef MASTER_GOLD


static const char* month_id[12] = 
{
	"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
};

static int days_in_month[12] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static int start_day	= 02;	
static int start_month	= 11;	
static int start_year	= 2022;	

void compute_build_id	()
{
	build_date			= __DATE__;

	int					days;
	int					months = 0;
	int					years;
	string16			month;
	string256			buffer;
	xr_strcpy				(buffer,__DATE__);
	sscanf				(buffer,"%s %d %d",month,&days,&years);

	for (int i=0; i<12; i++) {
		if (_stricmp(month_id[i],month))
			continue;

		months			= i;
		break;
	}

	build_id			= (years - start_year)*365 + days - start_day;

	for (int i=0; i<months; ++i)
		build_id		+= days_in_month[i];

	for (int i=0; i<start_month-1; ++i)
		build_id		-= days_in_month[i];

	Msg("'%s' build %d, %s\n", "Stalker2UE", build_id, build_date);
}
//---------------------------------------------------------------------
// 2446363
// umbt@ukr.net
//////////////////////////////////////////////////////////////////////////
struct _SoundProcessor	: public pureFrame
{
	virtual void	_BCL	OnFrame	( )
	{
		//Msg							("------------- sound: %d [%3.2f,%3.2f,%3.2f]",u32(Device->dwFrame),VPUSH(Device->vCameraPosition));
		Device->Statistic->Sound.Begin();
		::Sound->update				(Device->vCameraPosition,Device->vCameraDirection,Device->vCameraTop);
		Device->Statistic->Sound.End	();
	}
}	SoundProcessor;

XRayEngineInterface::XRayEngineInterface()
{
	Engine = xr_new<CEngine>();
	// events
	eQuit						= Engine->Event.Handler_Attach("KERNEL:quit",this);
	eStart						= Engine->Event.Handler_Attach("KERNEL:start",this);
	eStartLoad					= Engine->Event.Handler_Attach("KERNEL:load",this);
	eDisconnect					= Engine->Event.Handler_Attach("KERNEL:disconnect",this);
	eConsole					= Engine->Event.Handler_Attach("KERNEL:console",this);
	eStartMPDemo				= Engine->Event.Handler_Attach("KERNEL:start_mp_demo",this);

}

XRayEngineInterface::~XRayEngineInterface()
{
	if (Console)
	{
		Console->Hide();
	}
	// events
	Engine->Event.Handler_Detach	(eConsole,this);
	Engine->Event.Handler_Detach	(eDisconnect,this);
	Engine->Event.Handler_Detach	(eStartLoad,this);
	Engine->Event.Handler_Detach	(eStart,this);
	Engine->Event.Handler_Detach	(eQuit,this);
	Engine->Event.Handler_Detach(eStartMPDemo, this);
	xr_delete(Engine);
	
}
void XRayEngineInterface::OnFrame()
{
	Engine->Event.OnFrame();
	g_SpatialSpace->update();
	g_SpatialSpacePhysic->update();
	if (g_pGameLevel)
	{
		g_pGameLevel->SoundEvent_Dispatch();
	}
	Device->seqFrame.Process(rp_Frame);
}

void XRayEngineInterface::RunGame			(const char* ServerParams, const char* ClientParams)
{
	R_ASSERT(0 == g_pGameLevel);
	R_ASSERT(0 != g_pGamePersistent);
	{
		Console->Execute("main_menu off");
		Console->Hide();
		g_pGamePersistent->PreStart(ServerParams);
		g_pGameLevel = (IGame_Level*)NEW_INSTANCE(CLSID_GAME_LEVEL);
		g_pGamePersistent->Start(ServerParams);
		g_pGameLevel->net_Start(ServerParams, ClientParams);
	}
}

void XRayEngineInterface::StopGame			()
{
	if (g_pGameLevel)
	{
		Console->Hide();
		g_pGameLevel->net_Stop();
		DEL_INSTANCE(g_pGameLevel);
		Console->Show();
	}
	R_ASSERT(0 != g_pGamePersistent);
	g_pGamePersistent->Disconnect();
}

bool XRayEngineInterface::IsRunningGame		()
{
	return g_pGameLevel!=nullptr;
}

shared_str XRayEngineInterface::GetGameVersion()
{
	xr_string Version = "";
	switch (xrGameManager::GetPath())
	{
	default:
		Version = "Unknown";
	case EGamePath::COP_1602:
		Version = "COP 1.6.02";
		break;
	case EGamePath::CS_1510:
		Version = "CS 1.5.10";
		break;
	case EGamePath::SHOC_10006:
		Version = "SOC 1.0006";
		break;
	case EGamePath::SHOC_10004:
		Version = "SOC 1.0004";
		break;
	}
	xr_string BuildData;BuildData.Printf(", build %d %s\n", build_id, build_date);
	Version += BuildData;
	Version += "(";
	Version += GetUnrealVersion().c_str();
	Version += ")";
	return Version.c_str();
}

void XRayEngineInterface::OnEvent(EVENT E, u64 P1, u64 P2)
{
	if (E==eQuit)
	{
		Exit();
	}
	else if(E==eStart) 
	{
		LPSTR		op_server		= LPSTR	(P1);
		LPSTR		op_client		= LPSTR	(P2);
		RunGame(op_server,op_client);
		xr_free							(op_server);
		xr_free							(op_client);
	} 
	else if (E==eDisconnect) 
	{
		StopGame();
		Console->ExecuteCommand		( "main_menu 0");
		if(P1==0)
		{
			LoadDefaultWorld();
		}
	}
	else if (E == eConsole)
	{
		LPSTR command				= (LPSTR)P1;
		Console->ExecuteCommand		( command, false );
		xr_free						(command);
	}
	else if (E == eStartMPDemo)
	{
		LPSTR demo_file				= LPSTR	(P1);

		R_ASSERT	(0==g_pGameLevel);
		R_ASSERT	(0!=g_pGamePersistent);

		Console->Execute("main_menu off");
		Console->Hide();
		Device->Reset					(false);

		g_pGameLevel					= (IGame_Level*)NEW_INSTANCE(CLSID_GAME_LEVEL);
		shared_str server_options		= g_pGameLevel->OpenDemoFile(demo_file);
		
		//-----------------------------------------------------------
		g_pGamePersistent->PreStart		(server_options.c_str());
		//-----------------------------------------------------------
		
		g_pGamePersistent->Start		("");//server_options.c_str()); - no prefetch !
		g_pGameLevel->net_StartPlayDemo	();

		xr_free						(demo_file);
	}
}

extern	ENGINE_API BOOL			g_appLoaded = FALSE;
void XRayEngineInterface::Initialize()
{
	compute_build_id();
	InitSettings();
	XRC = xr_new<xrXRC>();
	// Adjust player & computer name for Asian
	if (pSettings->line_exist("string_table", "no_native_input"))
	{
		xr_strcpy(Core.UserName, sizeof(Core.UserName), "Player");
		xr_strcpy(Core.CompName, sizeof(Core.CompName), "Computer");
	}

	{

		FPU::m24r();

		InitEngine();

		InitInput();

		InitConsole();


		//.		InitInput					( );
		Engine->External.Initialize();
		Console->Execute("stat_memory");
	}
	{
		InitSound1();
		execUserScript();
		InitSound2();

		// ...command line for auto start
		{
			LPCSTR	pStartup = strstr(Core.Params, "-start ");
			if (pStartup)				Console->Execute(pStartup + 1);
		}
		{
			LPCSTR	pStartup = strstr(Core.Params, "-load ");
			if (pStartup)				Console->Execute(pStartup + 1);
		}

		// Initialize APP
		LALib = xr_new<ELightAnimLibrary>();
		LALib->OnCreate();
		g_pGamePersistent = (IGame_Persistent*)NEW_INSTANCE(CLSID_GAME_PERSISTANT);
		g_SpatialSpace = xr_new<ISpatial_DB>();
		g_SpatialSpacePhysic = xr_new<ISpatial_DB>();
		g_loading_events = xr_new < xr_list<LOADING_EVENT>>();
	}
}

void XRayEngineInterface::Destroy()
{
	xr_delete(g_loading_events);
	xr_delete(g_SpatialSpacePhysic);
	xr_delete(g_SpatialSpace);
	DEL_INSTANCE(g_pGamePersistent);
	Engine->Event.Dump();

	destroySettings();

	LALib->OnDestroy();
	xr_delete(LALib);
	destroyConsole();

	destroySound();

	destroyEngine();
}

XRayLevelToBlueprint* XRayEngineInterface::GetLevelScript()
{
	if(g_pGameLevel)
	{
		return g_pGameLevel;
	}
	return nullptr;
}

extern XRCDB_API BOOL* cdb_bDebug;

void XRayEngineInterface::InitEngine()
{
	VERIFY(GameMaterialLibrary); 
	Engine->Initialize();

	Device->Statistic = xr_new<CStats>();

#ifdef	DEBUG
	cdb_clRAY = &Device->Statistic->clRAY;				// total: ray-testing
	cdb_clBOX = &Device->Statistic->clBOX;				// total: box query
	cdb_clFRUSTUM = &Device->Statistic->clFRUSTUM;			// total: frustum query
	cdb_bDebug = &bDebug;
#endif
}

void XRayEngineInterface::InitSettings()
{
#ifndef DEDICATED_SERVER
	//	Msg( "EH: %s\n" , ComputeModuleHash( szEngineHash ) );
#endif // DEDICATED_SERVER

	string_path					fname;
	FS.update_path(fname, "$game_config$", "system.ltx");
#ifdef DEBUG
	Msg("Updated path to system.ltx is %s", fname);
#endif // #ifdef DEBUG
	pSettings = xr_new<CInifile>(fname, TRUE);
	CHECK_OR_EXIT(0 != pSettings->section_count(), make_string("Cannot find file %s.\nReinstalling application may fix this problem.", fname));
	FS.update_path(fname, "$game_config$", "game.ltx");
	pGameIni = xr_new<CInifile>(fname, TRUE);
	CHECK_OR_EXIT(0 != pGameIni->section_count(), make_string("Cannot find file %s.\nReinstalling application may fix this problem.", fname));
}

void XRayEngineInterface::InitInput()
{
	BOOL bCaptureInput = !strstr(Core.Params, "-i");
	VERIFY(GXRayInput);
}

void XRayEngineInterface::InitConsole()
{
#ifdef DEDICATED_SERVER
	{
		Console = xr_new<CTextConsole>();
	}
#else
		//	else
	{
		extern void CCC_Register();
		CCC_Register();
	//	Console = xr_new<CConsole>();
	}
#endif
	//Console->Initialize();

	xr_strcpy(Console->ConfigFile, "user.ltx");
	if (strstr(Core.Params, "-ltx ")) {
		string64				c_name;
		sscanf(strstr(Core.Params, "-ltx ") + 5, "%[^ ] ", c_name);
		xr_strcpy(Console->ConfigFile, c_name);
	}
}

void XRayEngineInterface::InitSound1()
{
	CSound_manager_interface::_create(0);
}

void XRayEngineInterface::execUserScript()
{
	if (xrGameManager::GetGame() == EGame::SHOC)
	{
		string_path fname;
		FS.update_path(fname, "$game_config$", "default_controls.ltx");
		Console->ExecuteScript(fname);
	}
	else
	{
		Console->Execute("default_controls");
	}
	Console->ExecuteScript(Console->ConfigFile);
}

void XRayEngineInterface::InitSound2()
{
	CSound_manager_interface::_create(1); 
	Device->seqFrame.Add(&SoundProcessor);
}

void XRayEngineInterface::destroySettings()
{
	CInifile** s = (CInifile**)(&pSettings);
	xr_delete(*s);
	xr_delete(pGameIni);
}

void XRayEngineInterface::destroyConsole()
{
	Console->Execute("cfg_save");
	Console->Destroy();
}

void XRayEngineInterface::destroySound()
{
	CSound_manager_interface::_destroy();
	if (Device)
	{
		Device->seqFrameMT.Remove(&SoundProcessor);
		Device->seqFrame.Remove(&SoundProcessor);
	}
}

void XRayEngineInterface::destroyEngine()
{
	Engine->Destroy();
	xr_delete(Device->Statistic);
	xr_delete(XRC);
}


FACTORY_PTR_INSTANCIATE(FontRender)
FACTORY_PTR_INSTANCIATE(FlareRender)
FACTORY_PTR_INSTANCIATE(ThunderboltRender)
FACTORY_PTR_INSTANCIATE(ThunderboltDescRender)
FACTORY_PTR_INSTANCIATE(LensFlareRender)
FACTORY_PTR_INSTANCIATE(RainRender)
FACTORY_PTR_INSTANCIATE(EnvironmentRender)
FACTORY_PTR_INSTANCIATE(EnvDescriptorRender)
FACTORY_PTR_INSTANCIATE(EnvDescriptorMixerRender)
FACTORY_PTR_INSTANCIATE(UIShader)
FACTORY_PTR_INSTANCIATE(WallMarkArray)
FACTORY_PTR_INSTANCIATE(StatGraphRender)
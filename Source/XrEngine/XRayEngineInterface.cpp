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
//---------------------------------------------------------------------
ENGINE_API CInifile* pGameIni		= NULL;
// computing build id
XRCORE_API extern	LPCSTR	build_date;
XRCORE_API	extern u32		build_id;

#ifdef MASTER_GOLD
#	define NO_MULTI_INSTANCES
#endif // #ifdef MASTER_GOLD


static LPSTR month_id[12] = {
	"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
};

static int days_in_month[12] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static int start_day	= 31;	// 31
static int start_month	= 1;	// January
static int start_year	= 1999;	// 1999

// binary hash, mainly for copy-protection
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

//////////////////////////////////////////////////////////////////////////
// global variables
ENGINE_API	XRayEngineInterface*	g_Engine			= NULL;

// -------------------------------------------
// startup point

struct path_excluder_predicate
{
	explicit path_excluder_predicate(xr_auth_strings_t const * ignore) :
		m_ignore(ignore)
	{
	}
	bool xr_stdcall is_allow_include(LPCSTR path)
	{
		if (!m_ignore)
			return true;
		
		return allow_to_include_path(*m_ignore, path);
	}
	xr_auth_strings_t const *	m_ignore;
};

#include "xr_ioc_cmd.h"

LPCSTR _GetFontTexName (LPCSTR section)
{
	static char* tex_names[]={"texture800","texture","texture1600"};
	int def_idx		= 1;//default 1024x768
	int idx			= def_idx;

#if 0
	u32 w = Device->dwWidth;

	if(w<=800)		idx = 0;
	else if(w<=1280)idx = 1;
	else 			idx = 2;
#else
	u32 h = Device->dwHeight;

	if(h<=600)		idx = 0;
	else if(h<1024)	idx = 1;
	else 			idx = 2;
#endif

	while(idx>=0){
		if( pSettings->line_exist(section,tex_names[idx]) )
			return pSettings->r_string(section,tex_names[idx]);
		--idx;
	}
	return pSettings->r_string(section,tex_names[def_idx]);
}

void _InitializeFont(CGameFont*& F, LPCSTR section, u32 flags)
{
	LPCSTR FontName = pSettings->r_string(section, "font");
	float FontSize = pSettings->r_float(section, "size");
	if (!F)
		F = xr_new<CGameFont>(FontName, FontSize, flags);
	else
		F->Initialize(FontName, FontSize);

#ifdef DEBUG
	F->m_font_name = section;
#endif

}

XRayEngineInterface::XRayEngineInterface()
{
	ll_dwReference	= 0;

	max_load_stage = 0;

	Engine = xr_new<CEngine>();
	// events
	eQuit						= Engine->Event.Handler_Attach("KERNEL:quit",this);
	eStart						= Engine->Event.Handler_Attach("KERNEL:start",this);
	eStartLoad					= Engine->Event.Handler_Attach("KERNEL:load",this);
	eDisconnect					= Engine->Event.Handler_Attach("KERNEL:disconnect",this);
	eConsole					= Engine->Event.Handler_Attach("KERNEL:console",this);
	eStartMPDemo				= Engine->Event.Handler_Attach("KERNEL:start_mp_demo",this);

	// levels
	Level_Current				= u32(-1);
	pFontSystem = NULL;
	ls_header[0] = '\0';
	ls_tip_number[0] = '\0';
	ls_tip[0] = '\0';
	//if (Device->IsEditorMode())return;
	//Level_Scan					( );

	//// Font


	//// Register us
	//
	//if (psDeviceFlags.test(mtSound))	Device->seqFrameMT.Add		(&SoundProcessor);
	//else								

	//Console->Show				( );

	// App Title
//	app_title[ 0 ] = '\0';

}

XRayEngineInterface::~XRayEngineInterface()
{
	if (Console)
	{
		Console->Hide();
	}

	// font
	xr_delete					( pFontSystem		);

	


	// events
	Engine->Event.Handler_Detach	(eConsole,this);
	Engine->Event.Handler_Detach	(eDisconnect,this);
	Engine->Event.Handler_Detach	(eStartLoad,this);
	Engine->Event.Handler_Detach	(eStart,this);
	Engine->Event.Handler_Detach	(eQuit,this);
	Engine->Event.Handler_Detach(eStartMPDemo, this);
	xr_delete(Engine);
	
}

void XRayEngineInterface::OnEvent(EVENT E, u64 P1, u64 P2)
{
	if (E==eQuit)
	{

		PostQuitMessage	(0);
		
		for (u32 i=0; i<Levels.size(); i++)
		{
			xr_free(Levels[i].folder);
			xr_free(Levels[i].name);
		}
	}
	else if(E==eStart) 
	{
		LPSTR		op_server		= LPSTR	(P1);
		LPSTR		op_client		= LPSTR	(P2);
		Level_Current				= u32(-1);
		R_ASSERT	(0==g_pGameLevel);
		R_ASSERT	(0!=g_pGamePersistent);

#ifdef NO_SINGLE
		Console->Execute("main_menu on");
		if (	(op_server == NULL)			||
				(!xr_strlen(op_server))		||
				(
					(	strstr(op_server, "/dm")	|| strstr(op_server, "/deathmatch") ||
						strstr(op_server, "/tdm")	|| strstr(op_server, "/teamdeathmatch") ||
						strstr(op_server, "/ah")	|| strstr(op_server, "/artefacthunt") ||
						strstr(op_server, "/cta")	|| strstr(op_server, "/capturetheartefact")
					) && 
					!strstr(op_server, "/alife")
				)
			)
#endif // #ifdef NO_SINGLE
		{		
			Console->Execute("main_menu off");
			Console->Hide();
//!			this line is commented by Dima
//!			because I don't see any reason to reset device here
//!			Device->Reset					(false);
			//-----------------------------------------------------------
			g_pGamePersistent->PreStart		(op_server);
			//-----------------------------------------------------------
			g_pGameLevel					= (IGame_Level*)NEW_INSTANCE(CLSID_GAME_LEVEL);
			g_Engine->LoadBegin					(); 
			g_pGamePersistent->Start		(op_server);
			g_pGameLevel->net_Start			(op_server,op_client);
			g_Engine->LoadEnd					(); 
		}
		xr_free							(op_server);
		xr_free							(op_client);
	} 
	else if (E==eDisconnect) 
	{
		ls_header[0] = '\0';
		ls_tip_number[0] = '\0';
		ls_tip[0] = '\0';

		if (g_pGameLevel) 
		{
			Console->Hide			();
			g_pGameLevel->net_Stop	();
			DEL_INSTANCE			(g_pGameLevel);
			Console->Show			();
			
			if( (FALSE == Engine->Event.Peek("KERNEL:quit")) &&(FALSE == Engine->Event.Peek("KERNEL:start")) )
			{
				Console->Execute("main_menu off");
				Console->Execute("main_menu on");
			}
		}
		R_ASSERT			(0!=g_pGamePersistent);
		g_pGamePersistent->Disconnect();
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
		
		g_Engine->LoadBegin					(); 
		g_pGamePersistent->Start		("");//server_options.c_str()); - no prefetch !
		g_pGameLevel->net_StartPlayDemo	();
		g_Engine->LoadEnd					(); 

		xr_free						(demo_file);
	}
}

static	CTimer	phase_timer		;
extern	ENGINE_API BOOL			g_appLoaded = FALSE;

void XRayEngineInterface::LoadBegin	()
{
	ll_dwReference++;
	if (1==ll_dwReference)	{

		g_appLoaded			= FALSE;

#ifndef DEDICATED_SERVER
		_InitializeFont		(pFontSystem,"ui_font_letterica18_russian",0);

		//m_pRender->LoadBegin();
#endif
		phase_timer.Start	();
		load_stage			= 0;

	}
}

void XRayEngineInterface::LoadEnd		()
{
	ll_dwReference--;
	if (0==ll_dwReference)		{
		Msg						("* phase time: %d ms",phase_timer.GetElapsed_ms());
		Msg						("* phase cmem: %d K", MemoryInterface->mem_usage()/1024);
		Console->Execute		("stat_memory");
		g_appLoaded				= TRUE;
//		DUMP_PHASE;
	}
}

void XRayEngineInterface::destroy_loading_shaders()
{
	//m_pRender->destroy_loading_shaders();
	//hLevelLogo.destroy		();
	//sh_progress.destroy		();
//.	::Sound->mute			(false);
}

void XRayEngineInterface::Initialize()
{
	compute_build_id();
	InitSettings();

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
		LALib.OnCreate();
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

	// Destroying
//.	destroySound();
	destroyInput();

	destroySettings();

	LALib.OnDestroy();

	destroyConsole();

	destroySound();

	destroyEngine();
}

extern XRCDB_API BOOL* cdb_bDebug;

void XRayEngineInterface::InitEngine()
{
	GameMaterialLibrary = xr_new<CGameMtlLibrary>();
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

	xr_auth_strings_t			tmp_ignore_pathes;
	xr_auth_strings_t			tmp_check_pathes;
	fill_auth_check_params(tmp_ignore_pathes, tmp_check_pathes);

	path_excluder_predicate			tmp_excluder(&tmp_ignore_pathes);
	CInifile::allow_include_func_t	tmp_functor;
	tmp_functor.bind(&tmp_excluder, &path_excluder_predicate::is_allow_include);
	pSettingsAuth = xr_new<CInifile>(
		fname,
		TRUE,
		TRUE,
		FALSE,
		0,
		tmp_functor
		);

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

void XRayEngineInterface::destroyInput()
{

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
	xr_delete(Console);
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
	xr_delete(GameMaterialLibrary);
	xr_delete(Device->Statistic);
}

//u32 calc_progress_color(u32, u32, int, int);

 void XRayEngineInterface::LoadDraw		()
{
	if(g_appLoaded)				return;
	Device->dwFrame				+= 1;


	if(!Device->Begin () )		return;

	if	(g_dedicated_server)
		Console->OnRender			();
	else
		load_draw_internal			();

	Device->End					();
}

void XRayEngineInterface::LoadTitleInt(LPCSTR str1, LPCSTR str2, LPCSTR str3)
{
	xr_strcpy					(ls_header, str1);
	xr_strcpy					(ls_tip_number, str2);
	xr_strcpy					(ls_tip, str3);
//	LoadDraw					();
}
void XRayEngineInterface::LoadStage()
{
	load_stage++;
	VERIFY						(ll_dwReference);
	Msg							("* phase time: %d ms",phase_timer.GetElapsed_ms());	phase_timer.Start();
	Msg							("* phase cmem: %d K", MemoryInterface->mem_usage()/1024);
	
	if (g_pGamePersistent->GameType()==1 && strstr(Core.Params,"alife"))
		max_load_stage			= 17;
	else
		max_load_stage			= 14;
	LoadDraw					();
}
void XRayEngineInterface::LoadSwitch	()
{
}

// Sequential
void XRayEngineInterface::OnFrame	( )
{
	Engine->Event.OnFrame			();
	g_SpatialSpace->update			();
	g_SpatialSpacePhysic->update	();
	if (g_pGameLevel)				g_pGameLevel->SoundEvent_Dispatch	( );
	Device->seqFrame.Process(rp_Frame);
}

void XRayEngineInterface::Level_Append		(LPCSTR folder)
{
	string_path	N1,N2,N3,N4;
	strconcat	(sizeof(N1),N1,folder,"level");
	strconcat	(sizeof(N2),N2,folder,"level.ltx");
	strconcat	(sizeof(N3),N3,folder,"level.geom");
	strconcat	(sizeof(N4),N4,folder,"level.cform");
	if	(
		FS.exist("$game_levels$",N1)		&&
		FS.exist("$game_levels$",N2)		&&
		FS.exist("$game_levels$",N3)		&&
		FS.exist("$game_levels$",N4)	
		)
	{
		sLevelInfo			LI;
		LI.folder			= xr_strdup(folder);
		LI.name				= 0;
		Levels.push_back	(LI);
	}
}

void XRayEngineInterface::Level_Scan()
{

	for (u32 i=0; i<Levels.size(); i++)
	{ 
		xr_free(Levels[i].folder);
		xr_free(Levels[i].name);
	}
	Levels.clear	();

	Level_Append("labx8\\");
	Level_Append("zaton\\");
	Level_Append("test5\\");

	

}

void gen_logo_name(string_path& dest, LPCSTR level_name, int num)
{
	strconcat	(sizeof(dest), dest, "intro\\intro_", level_name);
	
	u32 len = xr_strlen(dest);
	if(dest[len-1]=='\\')
		dest[len-1] = 0;

	string16 buff;
	xr_strcat(dest, sizeof(dest), "_");
	xr_strcat(dest, sizeof(dest), itoa(num+1, buff, 10));
}

void XRayEngineInterface::Level_Set(u32 L)
{

	if (L>=Levels.size())	return;
	FS.get_path	("$level$")->_set	(Levels[L].folder);

	static string_path			path;

	if(Level_Current != L)
	{
		path[0]					= 0;

		Level_Current			= L;
		
		int count				= 0;
		while(true)
		{
			string_path			temp2;
			gen_logo_name		(path, Levels[L].folder, count);
			if(FS.exist(temp2, "$game_textures$", path, ".dds") || FS.exist(temp2, "$level$", path, ".dds"))
				count++;
			else
				break;
		}

		if(count)
		{
			int num				= ::Random.randI(count);
			gen_logo_name		(path, Levels[L].folder, num);
		}
	}

	/*if(path[0])
		m_pRender->setLevelLogo	(path);*/


}

int XRayEngineInterface::Level_ID(LPCSTR name, LPCSTR ver, bool bSet)
{
	int result = -1;
	
	Level_Scan							();
	
	string256		buffer;
	strconcat		(sizeof(buffer),buffer,name,"\\");
	for (u32 I=0; I<Levels.size(); ++I)
	{
		if (0==stricmp(buffer,Levels[I].folder))	
		{
			result = int(I);	
			break;
		}
	}

	if(bSet && result!=-1)
		Level_Set(result);

	g_pGamePersistent->OnAssetsChanged	();

	return result;
}

CInifile*  XRayEngineInterface::GetArchiveHeader(LPCSTR name, LPCSTR ver)
{
	return NULL;
}

void XRayEngineInterface::LoadAllArchives()
{
		Level_Scan							();
		g_pGamePersistent->OnAssetsChanged	();
}

void XRayEngineInterface::load_draw_internal()
{
	//m_pRender->load_draw_internal(*this);
}
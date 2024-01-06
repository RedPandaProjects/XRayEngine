#include "stdafx.h"
#include "igame_level.h"

//#include "xr_effgamma.h"
#include "xr_ioconsole.h"
#include "xr_ioc_cmd.h"
//#include "fbasicvisual.h"
#include "cameramanager.h"
#include "xr_input.h"
#include "CustomHUD.h"

#include "Render/RenderDeviceRender.h"

#include "xr_object.h"

xr_token*							vid_quality_token = NULL;

xr_token							vid_bpp_token							[ ]={
	{ "16",							16											},
	{ "32",							32											},
	{ 0,							0											}
};
//-----------------------------------------------------------------------

void IConsole_Command::add_to_LRU( shared_str const& arg )
{
	if ( arg.size() == 0 || bEmptyArgsHandled )
	{
		return;
	}
	
	bool dup = ( std::find( m_LRU.begin(), m_LRU.end(), arg ) != m_LRU.end() );
	if ( !dup )
	{
		m_LRU.push_back( arg );
		if ( m_LRU.size() > LRU_MAX_COUNT )
		{
			m_LRU.erase( m_LRU.begin() );
		}
	}
}

void  IConsole_Command::add_LRU_to_tips( vecTips& tips )
{
	vecLRU::reverse_iterator	it_rb = m_LRU.rbegin();
	vecLRU::reverse_iterator	it_re = m_LRU.rend();
	for ( ; it_rb != it_re; ++it_rb )
	{
		tips.push_back( (*it_rb) );
	}
}

// =======================================================

class CCC_Quit : public IConsole_Command
{
public:
	CCC_Quit(LPCSTR N) : IConsole_Command(N)  { bEmptyArgsHandled = TRUE; };
	virtual void Execute(LPCSTR args) {
//		TerminateProcess(GetCurrentProcess(),0);
		Console->Hide();
		Engine->Event.Defer("KERNEL:disconnect");
		Engine->Event.Defer("KERNEL:quit");
	}
};
//-----------------------------------------------------------------------
#ifdef DEBUG_MEMORY_MANAGER
class CCC_MemStat : public IConsole_Command
{
public:
	CCC_MemStat(LPCSTR N) : IConsole_Command(N)  { bEmptyArgsHandled = TRUE; };
	virtual void Execute(LPCSTR args) {
		string_path fn;
		if (args&&args[0])	xr_sprintf	(fn,sizeof(fn),"%s.dump",args);
		else				strcpy_s_s	(fn,sizeof(fn),"x:\\$memory$.dump");
		MemoryInterface->mem_statistic				(fn);
//		g_pStringContainer->dump			();
//		g_pSharedMemoryContainer->dump		();
	}
};
#endif // DEBUG_MEMORY_MANAGER

#ifdef DEBUG_MEMORY_MANAGER
class CCC_DbgMemCheck : public IConsole_Command
{
public:
	CCC_DbgMemCheck(LPCSTR N) : IConsole_Command(N)  { bEmptyArgsHandled = TRUE; };
	virtual void Execute(LPCSTR args) { if (MemoryInterface->debug_mode){ MemoryInterface->dbg_check();}else{Msg("~ Run with -mem_debug options.");} }
};
#endif // DEBUG_MEMORY_MANAGER

class CCC_DbgStrCheck : public IConsole_Command
{
public:
	CCC_DbgStrCheck(LPCSTR N) : IConsole_Command(N)  { bEmptyArgsHandled = TRUE; };
	virtual void Execute(LPCSTR args) { g_pStringContainer->test(); }
};

class CCC_DbgStrDump : public IConsole_Command
{
public:
	CCC_DbgStrDump(LPCSTR N) : IConsole_Command(N)  { bEmptyArgsHandled = TRUE; };
	virtual void Execute(LPCSTR args) { g_pStringContainer->dump();}
};

//-----------------------------------------------------------------------
class CCC_MotionsStat : public IConsole_Command
{
public:
	CCC_MotionsStat(LPCSTR N) : IConsole_Command(N)  { bEmptyArgsHandled = TRUE; };
	virtual void Execute(LPCSTR args) {
		//g_pMotionsContainer->dump();
		//	TODO: move this console commant into renderer
		VERIFY(0);
	}
};
class CCC_TexturesStat : public IConsole_Command
{
public:
	CCC_TexturesStat(LPCSTR N) : IConsole_Command(N)  { bEmptyArgsHandled = TRUE; };
	virtual void Execute(LPCSTR args) {
		//Device->Resources->_DumpMemoryUsage();
		//	TODO: move this console commant into renderer
		//VERIFY(0);
	}
};
//-----------------------------------------------------------------------
class CCC_E_Dump : public IConsole_Command
{
public:
	CCC_E_Dump(LPCSTR N) : IConsole_Command(N)  { bEmptyArgsHandled = TRUE; };
	virtual void Execute(LPCSTR args) {
		Engine->Event.Dump();
	}
};
class CCC_E_Signal : public IConsole_Command
{
public:
	CCC_E_Signal(LPCSTR N) : IConsole_Command(N)  { };
	virtual void Execute(LPCSTR args) {
		char	Event[128],Param[128];
		Event[0]=0; Param[0]=0;
		sscanf	(args,"%[^,],%s",Event,Param);
		Engine->Event.Signal	(Event,(u64)Param);
	}
};



//-----------------------------------------------------------------------
class CCC_Help : public IConsole_Command
{
public:
	CCC_Help(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
	virtual void Execute(LPCSTR args) {
		Log("- --- Command listing: start ---");
		CConsole::vecCMD_IT it;
		for (it=Console->Commands.begin(); it!=Console->Commands.end(); it++)
		{
			IConsole_Command &C = *(it->second);
			TStatus _S; C.Status(_S);
			TInfo	_I;	C.Info	(_I);
			
			Msg("%-20s (%-10s) --- %s",	C.Name(), _S, _I);
		}
		Log("Key: Ctrl + A         === Select all ");
		Log("Key: Ctrl + C         === Copy to clipboard ");
		Log("Key: Ctrl + V         === Paste from clipboard ");
		Log("Key: Ctrl + X         === Cut to clipboard ");
		Log("Key: Ctrl + Z         === Undo ");
		Log("Key: Ctrl + Insert    === Copy to clipboard ");
		Log("Key: Shift + Insert   === Paste from clipboard ");
		Log("Key: Shift + Delete   === Cut to clipboard ");
		Log("Key: Insert           === Toggle mode <Insert> ");
		Log("Key: Back / Delete          === Delete symbol left / right ");

		Log("Key: Up   / Down            === Prev / Next command in tips list ");
		Log("Key: Ctrl + Up / Ctrl + Down === Prev / Next executing command ");
		Log("Key: Left, Right, Home, End {+Shift/+Ctrl}       === Navigation in text ");
		Log("Key: PageUp / PageDown      === Scrolling history ");
		Log("Key: Tab  / Shift + Tab     === Next / Prev possible command from list");
		Log("Key: Enter  / NumEnter      === Execute current command ");
		
		Log("- --- Command listing: end ----");
	}
};


XRCORE_API void _dump_open_files(int mode);
class CCC_DumpOpenFiles : public IConsole_Command
{
public:
	CCC_DumpOpenFiles(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = FALSE; };
	virtual void Execute(LPCSTR args) {
		int _mode			= atoi(args);
		_dump_open_files	(_mode);
	}
};

//-----------------------------------------------------------------------
class CCC_SaveCFG : public IConsole_Command
{
public:
	CCC_SaveCFG(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
	virtual void Execute(LPCSTR args) 
	{
		string_path			cfg_full_name;
		xr_strcpy			(cfg_full_name, (xr_strlen(args)>0)?args:Console->ConfigFile);

		bool b_abs_name = xr_strlen(cfg_full_name)>2 && cfg_full_name[1]==':';

		if(!b_abs_name)
			FS.update_path	(cfg_full_name, "$app_data_root$", cfg_full_name);

		if (strext(cfg_full_name))	
			*strext(cfg_full_name) = 0;
		xr_strcat			(cfg_full_name,".ltx");
		
		BOOL b_allow = TRUE;
		if ( FS.exist(cfg_full_name) )
			b_allow = SetFileAttributes(cfg_full_name,FILE_ATTRIBUTE_NORMAL);

		if ( b_allow ){
			IWriter* F			= FS.w_open(cfg_full_name);
				CConsole::vecCMD_IT it;
				for (it=Console->Commands.begin(); it!=Console->Commands.end(); it++)
					it->second->Save(F);
				FS.w_close			(F);
				Msg("Config-file [%s] saved successfully",cfg_full_name);
		}else
			Msg("!Cannot store config file [%s]", cfg_full_name);
	}
};
CCC_LoadCFG::CCC_LoadCFG(LPCSTR N) : IConsole_Command(N) 
{};

void CCC_LoadCFG::Execute(LPCSTR args) 
{
		Msg("Executing config-script \"%s\"...",args);
		string_path						cfg_name;

		xr_strcpy							(cfg_name, args);
		if (strext(cfg_name))			*strext(cfg_name) = 0;
		xr_strcat							(cfg_name,".ltx");

		string_path						cfg_full_name;

		FS.update_path					(cfg_full_name, "$app_data_root$", cfg_name);
		
		if( NULL == FS.exist(cfg_full_name) )
			FS.update_path					(cfg_full_name, "$fs_root$", cfg_name);
			
		if( NULL == FS.exist(cfg_full_name) )
			xr_strcpy						(cfg_full_name, cfg_name);
		
		IReader* F						= FS.r_open(cfg_full_name);
		
		string1024						str;
		if (F!=NULL) {
			while (!F->eof()) {
				F->r_string				(str,sizeof(str));
				if(allow(str))
					Console->Execute	(str);
			}
			FS.r_close(F);
			Msg("[%s] successfully loaded.",cfg_full_name);
		} else {
			Msg("! Cannot open script file [%s]",cfg_full_name);
		}
}

CCC_LoadCFG_custom::CCC_LoadCFG_custom(LPCSTR cmd)
:CCC_LoadCFG(cmd)
{
	xr_strcpy(m_cmd, cmd);
};
bool CCC_LoadCFG_custom::allow(LPCSTR cmd)
{
	return (cmd == strstr(cmd, m_cmd) );
};

//-----------------------------------------------------------------------
class CCC_Start : public IConsole_Command
{
	void	parse		(LPSTR dest, LPCSTR args, LPCSTR name)
	{
		dest[0]	= 0;
		if (strstr(args,name))
			sscanf(strstr(args,name)+xr_strlen(name),"(%[^)])",dest);
	}

	void	protect_Name_strlwr( LPSTR str )
	{
 		string4096	out;
		xr_strcpy( out, sizeof(out), str );
		strlwr( str );

		LPCSTR name_str = "name=";
		LPCSTR name1 = strstr( str, name_str );
		if ( !name1 || !xr_strlen( name1 ) )
		{
			return;
		}
		int begin_p = xr_strlen( str ) - xr_strlen( name1 ) + xr_strlen( name_str );
		if ( begin_p < 1 )
		{
			return;
		}

		LPCSTR name2 = strchr( name1, '/' );
		int end_p = xr_strlen( str ) - ((name2)? xr_strlen(name2) : 0);
		if ( begin_p >= end_p )
		{
			return;
		}
		for ( int i = begin_p; i < end_p;++i )
		{
			str[i] = out[i];
		}
	}
public:
	CCC_Start(LPCSTR N) : IConsole_Command(N)	{ 	  bLowerCaseArgs = false; };
	virtual void Execute(LPCSTR args)
	{
/*		if (g_pGameLevel)	{
			Log		("! Please disconnect/unload first");
			return;
		}
*/
		string4096	op_server,op_client,op_demo;
		op_server[0] = 0;
		op_client[0] = 0;
		
		parse		(op_server,args,"server");	// 1. server
		parse		(op_client,args,"client");	// 2. client
		parse		(op_demo, args,	"demo");	// 3. demo
		
		strlwr( op_server );
		protect_Name_strlwr( op_client );

		if(!op_client[0] && strstr(op_server,"single"))
			xr_strcpy(op_client, "localhost");

		if ((0==xr_strlen(op_client)) && (0 == xr_strlen(op_demo)))
		{
			Log("! Can't start game without client. Arguments: '%s'.",args);
			return;
		}
		if (g_pGameLevel)
			Engine->Event.Defer("KERNEL:disconnect");
		
		if (xr_strlen(op_demo))
		{
			Engine->Event.Defer	("KERNEL:start_mp_demo",u64(xr_strdup(op_demo)),0);
		} else
		{
			Engine->Event.Defer	("KERNEL:start",u64(xr_strlen(op_server)?xr_strdup(op_server):0),u64(xr_strdup(op_client)));
		}
	}
};

class CCC_Disconnect : public IConsole_Command
{
public:
	CCC_Disconnect(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
	virtual void Execute(LPCSTR args) {
		Engine->Event.Defer("KERNEL:disconnect");
	}
};
//-----------------------------------------------------------------------
float	ps_gamma=1.f,ps_brightness=1.f,ps_contrast=1.f;


//-----------------------------------------------------------------------
/*
#ifdef	DEBUG
extern  INT	g_bDR_LM_UsePointsBBox;
extern	INT	g_bDR_LM_4Steps;
extern	INT g_iDR_LM_Step;
extern	Fvector	g_DR_LM_Min, g_DR_LM_Max;

class CCC_DR_ClearPoint : public IConsole_Command
{
public:
	CCC_DR_ClearPoint(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; };
	virtual void Execute(LPCSTR args) {
		g_DR_LM_Min.x = 1000000.0f;
		g_DR_LM_Min.z = 1000000.0f;

		g_DR_LM_Max.x = -1000000.0f;
		g_DR_LM_Max.z = -1000000.0f;

		Msg("Local BBox (%f, %f) - (%f, %f)", g_DR_LM_Min.x, g_DR_LM_Min.z, g_DR_LM_Max.x, g_DR_LM_Max.z);
	}
};

class CCC_DR_TakePoint : public IConsole_Command
{
public:
	CCC_DR_TakePoint(LPCSTR N) : IConsole_Command(N)	{ bEmptyArgsHandled = TRUE; };
	virtual void Execute(LPCSTR args) {
		Fvector CamPos =  Device->vCameraPosition;

		if (g_DR_LM_Min.x > CamPos.x)	g_DR_LM_Min.x = CamPos.x;
		if (g_DR_LM_Min.z > CamPos.z)	g_DR_LM_Min.z = CamPos.z;

		if (g_DR_LM_Max.x < CamPos.x)	g_DR_LM_Max.x = CamPos.x;
		if (g_DR_LM_Max.z < CamPos.z)	g_DR_LM_Max.z = CamPos.z;

		Msg("Local BBox (%f, %f) - (%f, %f)", g_DR_LM_Min.x, g_DR_LM_Min.z, g_DR_LM_Max.x, g_DR_LM_Max.z);
	}
};

class CCC_DR_UsePoints : public CCC_Integer
{
public:
	CCC_DR_UsePoints(LPCSTR N, int* V, int _min=0, int _max=999) : CCC_Integer(N, V, _min, _max)	{};
	virtual void	Save	(IWriter *F)	{};
};
#endif
*/

//void fill_render_mode_list();
//void free_render_mode_list();

class ENGINE_API CCC_HideConsole : public IConsole_Command
{
public		:
	CCC_HideConsole(LPCSTR N) : IConsole_Command(N)
	{
		bEmptyArgsHandled	= true;
	}

	virtual void	Execute	(LPCSTR args)
	{
		Console->Hide	();
	}
	virtual void	Status	(TStatus& S)
	{
		S[0]			= 0;
	}
	virtual void	Info	(TInfo& I)
	{	
		xr_sprintf		(I,sizeof(I),"hide console");
	}
};


ENGINE_API float	psHUD_FOV=0.45f;

//extern int			psSkeletonUpdate;
extern int			rsDVB_Size;
extern int			rsDIB_Size;
extern int			psNET_ClientUpdate;
extern int			psNET_ClientPending;
extern int			psNET_ServerUpdate;
extern int			psNET_ServerPending;
extern int			psNET_DedicatedSleep;
extern char			psNET_Name[32];
extern Flags32		psEnvFlags;
//extern float		r__dtex_range;

extern int			g_ErrorLineCount;




ENGINE_API int			ps_r__Supersample			= 1;
void CCC_Register()
{
	// General
	CMD1(CCC_Help,		"help"					);
	CMD1(CCC_Quit,		"quit"					);
	CMD1(CCC_Start,		"start"					);
	CMD1(CCC_Disconnect,"disconnect"			);
	CMD1(CCC_SaveCFG,	"cfg_save"				);
	CMD1(CCC_LoadCFG,	"cfg_load"				);

#ifdef DEBUG
	CMD1(CCC_MotionsStat,	"stat_motions"		);
	CMD1(CCC_TexturesStat,	"stat_textures"		);
#endif // DEBUG

#ifdef DEBUG_MEMORY_MANAGER
	CMD1(CCC_MemStat,		"dbg_mem_dump"		);
	CMD1(CCC_DbgMemCheck,	"dbg_mem_check"		);
#endif // DEBUG_MEMORY_MANAGER

#ifdef DEBUG
	CMD3(CCC_Mask,		"mt_particles",			&psDeviceFlags,			mtParticles);

	CMD1(CCC_DbgStrCheck,	"dbg_str_check"		);
	CMD1(CCC_DbgStrDump,	"dbg_str_dump"		);

	CMD3(CCC_Mask,		"mt_sound",				&psDeviceFlags,			mtSound);
	CMD3(CCC_Mask,		"mt_physics",			&psDeviceFlags,			mtPhysics);
	CMD3(CCC_Mask,		"mt_network",			&psDeviceFlags,			mtNetwork);
	
	// Events
	CMD1(CCC_E_Dump,	"e_list"				);
	CMD1(CCC_E_Signal,	"e_signal"				);


	CMD3(CCC_Mask,		"rs_detail",			&psDeviceFlags,		rsDetails	);
	//CMD4(CCC_Float,		"r__dtex_range",		&r__dtex_range,		5,		175	);

//	CMD3(CCC_Mask,		"rs_constant_fps",		&psDeviceFlags,		rsConstantFPS			);
#endif

	// Render device states
	CMD4(CCC_Integer,	"r__supersample",		&ps_r__Supersample,			1,		4		);

;
	CMD3(CCC_Mask,		"rs_stats",				&psDeviceFlags,		rsStatistic				);

	CMD3(CCC_Mask,		"rs_cam_pos",			&psDeviceFlags,		rsCameraPos				);
#ifdef DEBUG
	//CMD4(CCC_Integer,	"rs_skeleton_update",	&psSkeletonUpdate,	2,		128	);
#endif // DEBUG

//	CMD4(CCC_Integer,	"rs_vb_size",			&rsDVB_Size,		32,		4096);
//	CMD4(CCC_Integer,	"rs_ib_size",			&rsDIB_Size,		32,		4096);

	// Texture manager	
	CMD4(CCC_Integer,	"net_dedicated_sleep",	&psNET_DedicatedSleep,		0,	64	);


	#ifdef DEBUG

	CMD4(CCC_Integer,	"error_line_count",		&g_ErrorLineCount,	6,	1024	);
#endif // DEBUG

	// Mouse
	CMD3(CCC_Mask,		"mouse_invert",			&psMouseInvert,1);
	psMouseSens			= 0.12f;
	CMD4(CCC_Float,		"mouse_sens",			&psMouseSens,		0.001f, 0.6f);

	// Camera
	CMD2(CCC_Float,		"cam_inert",			&psCamInert);
	CMD2(CCC_Float,		"cam_slide_inert",		&psCamSlideInert);

	//CMD1(CCC_r2,		"renderer"				);


	extern	int	g_Dump_Export_Obj;
	extern	int	g_Dump_Import_Obj;
	CMD4(CCC_Integer,	"net_dbg_dump_export_obj",	&g_Dump_Export_Obj, 0, 1);
	CMD4(CCC_Integer,	"net_dbg_dump_import_obj",	&g_Dump_Import_Obj, 0, 1);

#ifdef DEBUG	
	CMD1(CCC_DumpOpenFiles,		"dump_open_files");
#endif

	extern int g_svTextConsoleUpdateRate;
	CMD4(CCC_Integer, "sv_console_update_rate", &g_svTextConsoleUpdateRate, 1, 100);


	CMD1(CCC_HideConsole,		"hide");

#ifdef	DEBUG
	extern BOOL debug_destroy;
	CMD4(CCC_Integer, "debug_destroy", &debug_destroy, FALSE, TRUE );
#endif
	
};
 

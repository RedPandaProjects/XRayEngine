#include "stdafx.h"
#include "HUDmanager.h"
#include "PHdynamicdata.h"
#include "Physics.h"
#include "level.h"
#include "../XrEngine/igame_persistent.h"
#include "PhysicsGamePars.h"
#include "ai_space.h"

extern	pureFrame*				g_pNetProcessor;

BOOL CLevel::net_Start_client	( LPCSTR options )
{
	return FALSE;
}
#include "string_table.h"
bool	CLevel::net_start_client1				()
{
	string64					name_of_server = "";
	if (strchr(*m_caClientOptions, '/'))
		strncpy(name_of_server,*m_caClientOptions, strchr(*m_caClientOptions, '/')-*m_caClientOptions);
	
	string256					temp;
	sprintf_s						(temp,"%s %s",
								CStringTable().translate("st_client_connecting_to").c_str(), name_of_server);

	g_pGamePersistent->LoadTitle				(temp);
	return true;
}

#include "xrServer.h"

bool	CLevel::net_start_client2				()
{
	if(psNET_direct_connect)
	{
		Server->create_direct_client();
	}
	connected_to_server = Connect2Server(*m_caClientOptions);
	return true;
}

bool	CLevel::net_start_client3				()
{
	if(connected_to_server)
	{
		LPCSTR					level_name = NULL;
		LPCSTR					level_ver  = NULL;

		if(psNET_direct_connect)
		{
			level_name	= ai().get_alife() ? *name() : Server->level_name( Server->GetConnectOptions() ).c_str();
		}
		else
		{
			level_name	= ai().get_alife() ? *name() : net_SessionName	();
		}

		shared_str const& server_options = Server->GetConnectOptions();
		level_name = name().c_str();//Server->level_name		(server_options).c_str();
		level_ver = "1.0";

		m_name					= level_name;
		// Load level
		//
		g_pGamePersistent->LoadTitle("st_loading_map");
		g_Engine->LoadWorld(level_name);

	}
	return true;
}

bool	CLevel::net_start_client4				()
{
	
	if(connected_to_server)
	{

		if(g_Engine->GetWorldStatus() == ERBMKWorldStatus::Loading)
		{
			return false;
		}
		R_ASSERT(g_Engine->GetWorldStatus() != ERBMKWorldStatus::None);
		if (g_Engine->GetWorldStatus() == ERBMKWorldStatus::Failure)
		{
			Disconnect();
			connected_to_server = FALSE;
			return true;
		}
		Load();
		g_pGamePersistent->LoadTitle("st_client_spawning");
		// Send physics to single or multithreaded mode
		LoadPhysicsGameParams				();
		ph_world							= xr_new<CPHWorld>();
		ph_world->Create					();

		// Send network to single or multithreaded mode
		// *note: release version always has "mt_*" enabled
		Device->seqFrameMT.Remove			(g_pNetProcessor);
		Device->seqFrame.Remove				(g_pNetProcessor);
		if (psDeviceFlags.test(mtNetwork))	Device->seqFrameMT.Add	(g_pNetProcessor,REG_PRIORITY_HIGH	+ 2);
		else								Device->seqFrame.Add		(g_pNetProcessor,REG_PRIORITY_LOW	- 2);

		if(!psNET_direct_connect)
		{
			// Waiting for connection/configuration completition
			CTimer	timer_sync	;	timer_sync.Start	();
			while	(!net_isCompleted_Connect())	Sleep	(5);
			Msg		("* connection sync: %d ms", timer_sync.GetElapsed_ms());
			while	(!net_isCompleted_Sync())	{ ClientReceive(); Sleep(5); }
		}

		while(!game_configured)			
		{ 
			ClientReceive(); 
			if(Server)
				Server->Update()	;
			Sleep(5); 
		}
	}
	return true;
}

bool	CLevel::net_start_client5				()
{
	if(connected_to_server)
	{
		// HUD

		// Textures
		if	(!g_dedicated_server)
		{
			HUD().Load							();
			LL_CheckTextures					();
		}
	}
	return true;
}

bool	CLevel::net_start_client6				()
{
	if(connected_to_server)
	{
		// Sync
		if(g_hud)
			g_hud->OnConnected				();

		g_pGamePersistent->LoadTitle("st_client_synchronising");
		Device->PreCache						(30,true,true);
		net_start_result_total				= TRUE;
	}
	else
	{
		net_start_result_total				= FALSE;
	}
	return true;
}
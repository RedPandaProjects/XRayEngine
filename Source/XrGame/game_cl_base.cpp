#include "pch_script.h"
#include "game_cl_base.h"
#include "level.h"
#include "GamePersistent.h"
#include "UIGameCustom.h"
#include "script_engine.h"
#include "xr_Level_controller.h"
#include "ui/UIMainIngameWnd.h"
#include "UI/UIGameTutorial.h"
#include "UI/UIMessagesWindow.h"
#include "UI/UIDialogWnd.h"
#include "string_table.h"

void game_cl_GameState::signal_Syncronize()
{
	sv_force_sync = TRUE;
}

// Network
void game_cl_GameState::net_Export_State(NET_Packet& P, ClientID to)
{
	// Generic
	P.w_u16(m_phase);
	P.w_u32(m_start_time);

	// Players
	net_Export_GameTime(P);
}

void game_cl_GameState::net_Export_Update(NET_Packet& P, ClientID id_to, ClientID id)
{
	net_Export_GameTime(P);
};

void game_cl_GameState::net_Export_GameTime(NET_Packet& P)
{
	//Syncronize GameTime 
	P.w_u64(GetGameTime());
	P.w_float(GetGameTimeFactor());

	//Syncronize EnvironmentGameTime 
	P.w_u64(GetEnvironmentGameTime());
	P.w_float(GetEnvironmentGameTimeFactor());
};

void game_cl_GameState::OnPlayerConnect(ClientID /**id_who/**/)
{
	signal_Syncronize();
}

void game_cl_GameState::Create(shared_str& options)
{
	// loading scripts
	ai().script_engine().remove_script_process(ScriptEngine::eScriptProcessorGame);
	string_path S;
	FS.update_path(S, "$game_config$", "script.ltx");
	CInifile* l_tpIniFile = new CInifile(S);
	R_ASSERT(l_tpIniFile);

	if (l_tpIniFile->section_exist("single"))
	{
		if (l_tpIniFile->r_string("single", "script"))
			ai().script_engine().add_script_process(ScriptEngine::eScriptProcessorGame, new CScriptProcess("game", l_tpIniFile->r_string("single", "script")));
		else
			ai().script_engine().add_script_process(ScriptEngine::eScriptProcessorGame, new CScriptProcess("game", ""));
	}
	xr_delete(l_tpIniFile);

	if (strstr(*options, "/alife"))
		m_alife_simulator = new CALifeSimulator(Level().Server, &options);

	switch_Phase(GAME_PHASE_INPROGRESS);
}

void game_cl_GameState::Update()
{
	if (Level().game)
	{
		CScriptProcess* script_process = ai().script_engine().script_process(ScriptEngine::eScriptProcessorGame);
		if (script_process)
			script_process->update();
	}
}

game_cl_GameState::game_cl_GameState() : m_alife_simulator(nullptr)
{
	VERIFY(g_pGameLevel);
	m_type = eGameIDSingle;
}

game_cl_GameState::~game_cl_GameState()
{
	ai().script_engine().remove_script_process(ScriptEngine::eScriptProcessorGame);
	m_alife_simulator->destroy();
	xr_delete(m_alife_simulator);
	m_alife_simulator = nullptr;
}

bool game_cl_GameState::change_level(NET_Packet& net_packet)
{
	if (ai().get_alife())
		return (alife().change_level(net_packet));
	else
		return (true);
}

bool game_cl_GameState::load_game(NET_Packet& net_packet)
{
	if (!ai().get_alife())
		return true;

	shared_str game_name;
	net_packet.r_stringZ(game_name);
	return (alife().load_game(*game_name, true));
}

void game_cl_GameState::OnEvent(NET_Packet& tNetPacket, u16 type, u32 time, ClientID sender)
{
	if (type == GAME_EVENT_ON_HIT)
	{
		u16 id_dest = tNetPacket.r_u16();
		u16 id_src = tNetPacket.r_u16();
		ISE_Abstract* e_src = Level().Server->ID_to_entity(id_src);

		if (e_src)
			Level().Server->SendBroadcast(tNetPacket);
	}
}

void game_cl_GameState::teleport_object(NET_Packet& packet, u16 id)
{
	if (!ai().get_alife())
		return;

	GameGraph::_GRAPH_ID		game_vertex_id;
	u32						level_vertex_id;
	Fvector					position;

	packet.r(&game_vertex_id, sizeof(game_vertex_id));
	packet.r(&level_vertex_id, sizeof(level_vertex_id));
	packet.r_vec3(position);

	alife().teleport_object(id, game_vertex_id, level_vertex_id, position);
}

void game_cl_GameState::add_restriction(RestrictionSpace::ERestrictorTypes type, u16 restriction_id, u16 id)
{
	if (ai().get_alife())
	{
		alife().add_restriction(id, restriction_id, type);
	}
}

void game_cl_GameState::remove_restriction(RestrictionSpace::ERestrictorTypes type, u16 restriction_id, u16 id)
{
	if (ai().get_alife())
	{
		alife().remove_restriction(id, restriction_id, type);
	}
}

void game_cl_GameState::remove_all_restrictions(RestrictionSpace::ERestrictorTypes type, u16 id)
{
	if (ai().get_alife())
	{
		alife().remove_all_restrictions(id, type);
	}
}

shared_str game_cl_GameState::level_name(const shared_str& server_options) const
{
	if (!ai().get_alife())
	{
		string64 l_name = "";
		VERIFY(_GetItemCount(*server_options, '/'));
		return (_GetItem(*server_options, 0, l_name, '/'));
	}

	return (alife().level_name());
}

constexpr const char* default_map_version = "1.0";
constexpr const char* map_ver_string = "ver=";

shared_str game_cl_GameState::parse_level_version(const shared_str& server_options)
{
	const char* map_ver = strstr(server_options.c_str(), map_ver_string);
	string128	result_version;
	if (map_ver)
	{
		map_ver += sizeof(map_ver_string);
		if (strchr(map_ver, '/'))
			strncpy_s(result_version, map_ver, strchr(map_ver, '/') - map_ver);
		else
			xstr::strcpy(result_version, map_ver);
	}
	else xstr::strcpy(result_version, default_map_version);

	return shared_str(result_version);
}

void game_cl_GameState::on_death(CSE_Abstract* e_dest, CSE_Abstract* e_src)
{
	CSE_ALifeCreatureAbstract* creature = smart_cast<CSE_ALifeCreatureAbstract*>(e_dest);
	if (!creature)
		return;

	VERIFY(creature->get_killer_id() == ALife::_OBJECT_ID(-1));
	creature->set_killer_id(e_src->ID);

	if (!ai().get_alife())
		return;

	alife().on_death(e_dest, e_src);
}

void game_cl_GameState::OnCreate(u16 id_who)
{
	if (!ai().get_alife())
		return;

	ISE_Abstract* e_who = Level().Server->ID_to_entity(id_who);
	VERIFY(e_who);
	if (!e_who->m_bALifeControl)
		return;

	CSE_ALifeObject* alife_object = smart_cast<CSE_ALifeObject*>(e_who);
	if (!alife_object)
		return;

	alife_object->m_bOnline = true;

	if (alife_object->ID_Parent != 0xffff)
	{
		CSE_ALifeDynamicObject* parent = ai().alife().objects().object(alife_object->ID_Parent, true);
		if (parent)
		{
			CSE_ALifeTraderAbstract* trader = smart_cast<CSE_ALifeTraderAbstract*>(parent);
			if (trader)
				alife().create(alife_object);
			else
			{
				CSE_ALifeInventoryBox* const box = smart_cast<CSE_ALifeInventoryBox*>(parent);
				if (box)
					alife().create(alife_object);
				else
					alife_object->m_bALifeControl = false;
			}
		}
		else alife_object->m_bALifeControl = false;
	}
	else alife().create(alife_object);
}

void game_cl_GameState::restart_simulator(const char* saved_game_name)
{
	shared_str options = GamePersistent().GetServerOption();

	m_alife_simulator->destroy();
	xr_delete(m_alife_simulator);
	m_alife_simulator = nullptr;

	Level().Server->clear_ids();

	xstr::strcpy(g_pGamePersistent->m_game_params.m_game_or_spawn, saved_game_name);
	xstr::strcpy(g_pGamePersistent->m_game_params.m_new_or_load, "load");

	pApp->SetLoadingScreen(new UILoadingScreen());
	pApp->LoadBegin();
	m_alife_simulator = new CALifeSimulator(Level().Server, &options);
	g_pGamePersistent->SetLoadStageTitle("st_client_synchronising");
	pApp->LoadForceFinish();
	g_pGamePersistent->LoadTitle();
	Device->PreCache(60, true, true);
	pApp->LoadEnd();
}

void game_cl_GameState::OnTouch(u16 eid_who, u16 eid_what)
{
	ISE_Abstract* e_who = Level().Server->ID_to_entity(eid_who);		VERIFY(e_who);
	ISE_Abstract* e_what = Level().Server->ID_to_entity(eid_what);	VERIFY(e_what);

	if (ai().get_alife())
	{
		CSE_ALifeInventoryItem* l_tpALifeInventoryItem = smart_cast<CSE_ALifeInventoryItem*>	(e_what);
		CSE_ALifeDynamicObject* l_tpDynamicObject = smart_cast<CSE_ALifeDynamicObject*>	(e_who);

		if (l_tpALifeInventoryItem && l_tpDynamicObject &&
			ai().alife().graph().level().object(l_tpALifeInventoryItem->base()->ID, true) &&
			ai().alife().objects().object(e_who->ID, true) &&
			ai().alife().objects().object(e_what->ID, true))
			alife().graph().attach(*(CSE_Abstract*)e_who, l_tpALifeInventoryItem, l_tpDynamicObject->m_tGraphID, false, false);
	}
}

void game_cl_GameState::OnDetach(u16 eid_who, u16 eid_what)
{
	if (ai().get_alife())
	{
		ISE_Abstract* e_who = Level().Server->ID_to_entity(eid_who);		VERIFY(e_who);
		ISE_Abstract* e_what = Level().Server->ID_to_entity(eid_what);	VERIFY(e_what);

		CSE_ALifeInventoryItem* l_tpALifeInventoryItem = smart_cast<CSE_ALifeInventoryItem*>(e_what);
		if (!l_tpALifeInventoryItem)
			return;

		CSE_ALifeDynamicObject* l_tpDynamicObject = smart_cast<CSE_ALifeDynamicObject*>(e_who);
		if (!l_tpDynamicObject)
			return;

		const CALifeObjectRegistry& refAlifeObj = ai().alife().objects();

		if (refAlifeObj.object(e_who->ID, true) && refAlifeObj.object(e_what->ID, true) &&
			!ai().alife().graph().level().object(l_tpALifeInventoryItem->base()->ID, true))
		{
			alife().graph().detach(*(CSE_Abstract*)e_who, l_tpALifeInventoryItem, l_tpDynamicObject->m_tGraphID, false, false);
		}
		else if (!refAlifeObj.object(e_what->ID, true))
		{
			u16 id = l_tpALifeInventoryItem->base()->ID_Parent;
			l_tpALifeInventoryItem->base()->ID_Parent = 0xffff;

			CSE_ALifeDynamicObject* dynamic_object = smart_cast<CSE_ALifeDynamicObject*>(e_what);
			VERIFY(dynamic_object);
			dynamic_object->m_tNodeID = l_tpDynamicObject->m_tNodeID;
			dynamic_object->m_tGraphID = l_tpDynamicObject->m_tGraphID;
			dynamic_object->m_bALifeControl = true;
			dynamic_object->m_bOnline = true;
			alife().create(dynamic_object);
			l_tpALifeInventoryItem->base()->ID_Parent = id;
		}
	}
}

u64 game_cl_GameState::GetStartGameTime()
{
	if (ai().get_alife() && ai().alife().initialized())
		return(ai().alife().time_manager().start_game_time());
	else
		return(inherited::GetStartGameTime());
}

u64 game_cl_GameState::GetGameTime()
{
	if (ai().get_alife() && ai().alife().initialized())
		return(ai().alife().time_manager().game_time());
	else
		return(inherited::GetGameTime());
}

float game_cl_GameState::GetGameTimeFactor()
{
	if (ai().get_alife() && ai().alife().initialized())
		return(ai().alife().time_manager().time_factor());
	else
		return(inherited::GetGameTimeFactor());
}

void game_cl_GameState::SetGameTimeFactor(const float fTimeFactor)
{
	if (ai().get_alife() && ai().alife().initialized())
		return(alife().time_manager().set_time_factor(fTimeFactor));
	else
		return(inherited::SetGameTimeFactor(fTimeFactor));
}

u64 game_cl_GameState::GetEnvironmentGameTime()
{
	if (ai().get_alife() && ai().alife().initialized())
		return(alife().time_manager().game_time());
	else
		return(inherited::GetGameTime());
}

float game_cl_GameState::GetEnvironmentGameTimeFactor()
{
	return(inherited::GetGameTimeFactor());
}

void game_cl_GameState::SetEnvironmentGameTimeFactor(const float fTimeFactor)
{
	return(inherited::SetGameTimeFactor(fTimeFactor));
}

void game_cl_GameState::sls_default()
{
	alife().update_switch();
}

//  [7/5/2005]
#ifdef DEBUG
extern	Flags32	dbg_net_Draw_Flags;

void		game_cl_GameState::OnRender()
{
};
#endif

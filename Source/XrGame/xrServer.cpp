// xrServer.cpp: implementation of the xrServer class.
//
//////////////////////////////////////////////////////////////////////

#include "pch_script.h"
#include "xrServer.h"
#include "xrMessages.h"
#include "xrServer_Objects_ALife_All.h"
#include "level.h"
#include "game_cl_base.h"
#include "game_cl_base_weapon_usage_statistic.h"
#include "ai_space.h"
#include "../xrEngine/IGame_Persistent.h"
#include "string_table.h"
#include "../XrEngine/object_broker.h"

#include "../xrEngine/XR_IOConsole.h"
#include "ui/UIInventoryUtilities.h"

#include "alife_simulator_base.h"
#include "alife_object_registry.h"
#include "alife_simulator.h"

xrServer::xrServer()
{
	Game = smart_cast<game_sv_GameState*>(NEW_INSTANCE(TEXT2CLSID("SV_SINGL")));
}
xrServer::~xrServer()
{
	xr_delete(Game);
}

u32 xrServer::OnMessage(NET_Packet& P)
{
	u16 type;
	P.r_begin(type);

	switch (type)
	{
	case M_UPDATE:
	{
		ProcessUpdate(P);						// No broadcast
	}break;
	case M_SPAWN:
	{
		ProcessSpawn(P,nullptr);
	}break;
	case M_EVENT:
	{
		ProcessEvent(P);
	}break;
	case M_EVENT_PACK:
	{
		NET_Packet	tmpP;
		while (!P.r_eof())
		{
			tmpP.B.count = P.r_u8();
			P.r(tmpP.B.data, tmpP.B.count);

			OnMessage(tmpP);
		};
	}break;
	//-------------------------------------------------------------------
	case M_CHANGE_LEVEL:
	{
		if (Game->change_level(P))
		{
			SendBroadcast(P);
		}
	}break;
	case M_LOAD_GAME:
	{
		Game->load_game(P);
		SendBroadcast(P);
	}break;
	case M_SAVE_PACKET:
	{
		ProcessSave(P);
	}break;
	}

	return IPureServer::OnMessage(P);
}

ISE_Abstract* xrServer::CreateEntity(const char* Name)
{
	return F_entity_Create(Name);
}

void xrServer::DestroyEntity(ISE_Abstract* ServerEntity)
{
	IPureServer::DestroyEntity(ServerEntity);
	if (!ai().get_alife() || !ServerEntity->m_bALifeControl)
	{
		F_entity_Destroy(ServerEntity);
	}
}

void xrServer::OnCreateEntity(ISE_Abstract* Entity, ISE_Abstract* ParentEntity)
{
	Game->OnCreate(Entity->ID);

	if (0xffff != Entity->ID_Parent)
	{
		R_ASSERT(ParentEntity);

		Game->OnTouch(Entity->ID_Parent, Entity->ID);

		ParentEntity->children.push_back(Entity->ID);
	}
}

void xrServer::OnDestroyEntity(ISE_Abstract*InEntity)
{
	F_entity_Destroy(InEntity);
}

void xrServer::ProcessEvent(NET_Packet& P)
{

	auto ProcessEventActivate = [this](NET_Packet& P, const u16 id_parent, const u16 id_entity)
	{
		// Parse message
		ISE_Abstract* e_parent = GetEntityFromID(id_parent);
		ISE_Abstract* e_entity = GetEntityFromID(id_entity);

		R_ASSERT(e_parent);
		R_ASSERT(e_entity);

		xr_delete(e_parent);

		if (0xffff != e_entity->ID_Parent)
		{
			// Signal to everyone (including sender)
			SendBroadcast(P);
		}
	};
	auto ProcessEventOwnership = [this](NET_Packet& P, u16 ID)
	{

		u16 id_parent = ID;
		u16 id_entity = P.r_u16();

		CSE_Abstract* e_parent = (CSE_Abstract*)GetEntityFromID(id_parent);
		CSE_Abstract* e_entity = (CSE_Abstract*)GetEntityFromID(id_entity);

		auto CheckGameObjectByID = [](u16 EntityID)->bool
		{
			if (EntityID == 0)
				return false;

			CObject* TryCheckObj = Level().Objects.net_Find(EntityID);
			if (TryCheckObj == nullptr)
				return false;

			CGameObject* WeCanCastToGameObject = smart_cast<CGameObject*>(TryCheckObj);
			if (WeCanCastToGameObject == nullptr || WeCanCastToGameObject->getDestroy())
				return false;

			return !WeCanCastToGameObject->object_removed();
		};

		if (!CheckGameObjectByID(id_parent))
		{
			Msg("! ERROR on ownership: parent object not found or is not valid. parent_id = [%d], entity_id = [%d], frame = [%d].", id_parent, id_entity, Device->dwFrame);
			return;
		}
		if (!CheckGameObjectByID(id_entity))
		{
			Msg("! ERROR on ownership: entity object not found is not valid on sv client. parent_id = [%d], entity_id = [%d], frame = [%d]", id_parent, id_entity, Device->dwFrame);
			return;
		}

		if (0xffff != e_entity->ID_Parent)
			return;

		// Game allows ownership of entity
		Game->OnTouch(id_parent, id_entity);

		// Rebuild parentness
		e_entity->ID_Parent = id_parent;
		e_parent->children.push_back(id_entity);

		// Signal to everyone (including sender)
		SendBroadcast(P);
	};

	VERIFY(VerifyEntities());

	u32 timestamp;
	u16 type;
	u16 destination;

	// correct timestamp with server-unique-time (note: direct message correction)
	P.r_u32(timestamp);

	// read generic info
	P.r_u16(type);
	P.r_u16(destination);

	CSE_Abstract* receiver = (CSE_Abstract*)GetEntityFromID(destination);
	if (receiver)
	{
		R_ASSERT(receiver->owner);
		receiver->OnEvent(P, type, timestamp, ServerClient->ID);
	}

	switch (type)
	{
	case GEG_PLAYER_DISABLE_SPRINT:
	case GEG_PLAYER_ACTIVATE_SLOT:
	case GEG_PLAYER_ITEM_EAT:			Level().OnMessage(P.B.data, (u32)P.B.count); break;
	case GE_TELEPORT_OBJECT: 			Game->teleport_object(P, destination); break;
	case GE_INFO_TRANSFER:
	case GE_WPN_STATE_CHANGE:
	case GE_ZONE_STATE_CHANGE:
	case GEG_PLAYER_ATTACH_HOLDER:
	case GEG_PLAYER_DETACH_HOLDER:
	case GEG_PLAYER_ITEM2SLOT:
	case GEG_PLAYER_ITEM2BELT:
	case GEG_PLAYER_ITEM2RUCK:
	case GE_ADDON_ATTACH:
	case GE_ADDON_DETACH:
	case GE_GRENADE_EXPLODE:			SendBroadcast(P); break;
	case GEG_PLAYER_ACTIVATEARTEFACT: 	ProcessEventActivate(P, destination, P.r_u16()); break;
	case GE_INV_ACTION: 				if (ServerClient) Level().OnMessage(P.B.data, (u32)P.B.count); break;
	case GE_TRADE_BUY:
	case GE_OWNERSHIP_TAKE: 			ProcessEventOwnership(P, destination); break;
	case GE_TRADE_SELL:
	case GE_OWNERSHIP_REJECT:
	case GE_LAUNCH_ROCKET: 				ProcessEventReject(P, timestamp, destination, P.r_u16()); break;
	case GE_DESTROY: 					ProcessEventDestroy(P, timestamp, destination, nullptr); break;
	case GE_TRANSFER_AMMO:
	{
		u16 id_entity;
		P.r_u16(id_entity);

		// кто забирает (для своих нужд)
		CSE_Abstract* e_parent = receiver;
		// кто отдает
		CSE_Abstract* e_entity = (CSE_Abstract*)GetEntityFromID(id_entity);
		if (!e_entity) break;

		// this item already taken
		if (0xffff != e_entity->ID_Parent) break;

		// Signal to everyone (including sender)
		SendBroadcast(P);

		// Perfrom real destroy
		ISE_Abstract* CompilerHack = e_entity;
		DestroyEntity(CompilerHack);
	}
	break;
	case GE_HIT:
	case GE_HIT_STATISTIC:
	{
		P.r_pos -= 2;
		if (type == GE_HIT_STATISTIC)
		{
			P.B.count -= 4;
			P.w_u32(ServerClient->ID.value());
		};
		u16 id_src = P.r_u16();
		CSE_Abstract* e_src = (CSE_Abstract*)GetEntityFromID(id_src);

		if (e_src)
			SendBroadcast(P);
	} break;
	case GE_ASSIGN_KILLER:
	{
		u16 id_src;
		P.r_u16(id_src);

		// кто умер
		CSE_Abstract* e_dest = receiver;
		// this is possible when hit event is sent before destroy event
		if (!e_dest) break;

		CSE_ALifeCreatureAbstract* creature = smart_cast<CSE_ALifeCreatureAbstract*>(e_dest);
		if (creature)
			creature->set_killer_id(id_src);
	} break;
	case GE_CHANGE_VISUAL:
	{
		CSE_Visual* visual = smart_cast<CSE_Visual*>(receiver); VERIFY(visual);
		string256 tmp;
		P.r_stringZ(tmp);
		visual->set_visual(tmp);
	} break;
	case GE_DIE:
	{
		// Parse message
		u16 id_dest = destination, id_src;
		P.r_u16(id_src);

		// кто умер
		CSE_Abstract* e_dest = receiver;
		// this is possible when hit event is sent before destroy event
		if (!e_dest) break;

		CSE_Abstract* e_src = (CSE_Abstract*)GetEntityFromID(id_src);	// кто убил

		if (!e_src)
		{
			Msg("! ERROR: SV: src killer not exist.");
			return;
		}

		Game->on_death(e_dest, e_src);

		// клиент, чей юнит убил
		XRayClient* c_src = e_src->owner;

		if (c_src->owner->ID == id_src)
		{
			// Main unit
			P.w_begin(M_EVENT);
			P.w_u32(timestamp);
			P.w_u16(type);
			P.w_u16(destination);
			P.w_u16(id_src);
			P.w_clientID(c_src->ID);
		}

		SendBroadcast(P);
		//////////////////////////////////////////////////////////////////////////
		P.w_begin(M_EVENT);
		P.w_u32(timestamp);
		P.w_u16(GE_KILL_SOMEONE);
		P.w_u16(id_src);
		P.w_u16(destination);
		Level().OnMessage(P.B.data, (u32)P.B.count);
		//////////////////////////////////////////////////////////////////////////

		VERIFY(VerifyEntities());
	} break;
	case GE_INSTALL_UPGRADE:
	{
		shared_str upgrade_id;
		P.r_stringZ(upgrade_id);

		CSE_ALifeInventoryItem* iitem = smart_cast<CSE_ALifeInventoryItem*>(receiver);
		if (iitem)
			iitem->add_upgrade(upgrade_id);
	} break;
	//	case GE_INV_BOX_STATUS:
	case GE_INV_OWNER_STATUS:
	{
		u8 can_take, closed;
		P.r_u8(can_take);
		P.r_u8(closed);

		CSE_ALifeTraderAbstract* iowner = smart_cast<CSE_ALifeTraderAbstract*>(receiver);
		if (iowner)
		{
			iowner->m_deadbody_can_take = (can_take == 1);
			iowner->m_deadbody_closed = (closed == 1);
		}
	} break;
	case GE_MONEY:
	{
		CSE_Abstract* e_dest = receiver;
		CSE_ALifeTraderAbstract* pTa = smart_cast<CSE_ALifeTraderAbstract*>(e_dest);
		pTa->m_dwMoney = P.r_u32();
	} break;
	case GE_INV_OWNER_SETNAME:
	{
		shared_str name;
		P.r_stringZ(name);

		CSE_ALifeTraderAbstract* iowner = smart_cast<CSE_ALifeTraderAbstract*>(receiver);
		if (iowner)
			iowner->m_character_name = name.c_str();
	} break;
	default: VERIFY2(0, "Game Event not implemented!!!"); break;
	}
}

bool xrServer::ProcessEventReject(NET_Packet& P, const u32& time, const u16 id_parent, const u16 id_entity, bool send_message /*= true*/)
{
	// Parse message
	ISE_Abstract* e_parent = GetEntityFromID(id_parent);
	ISE_Abstract* e_entity = GetEntityFromID(id_entity);

	if (!e_entity)
	{
		Msg("[ERROR on rejecting]: entity not found. parent_id = [%d], entity_id = [%d], frame = [%d].", id_parent, id_entity, Device->dwFrame);
		return false;
	}

	if (!e_parent)
	{
		Msg("[ERROR on rejecting]: parent not found. parent_id = [%d], entity_id = [%d], frame = [%d].", id_parent, id_entity, Device->dwFrame);
		return false;
	}
	else if (0xffff == e_entity->ID_Parent)
		return false;

	auto& C = e_parent->children;
	auto c = std::find(C.begin(), C.end(), id_entity);
	if (c == C.end())
	{
		Msg("[ERROR] SV: can't find children [%d] of parent [%d]", id_entity, e_parent);
		return false;
	}


	if (e_entity->ID_Parent != id_parent)
	{
		Msg("[ERROR] e_entity->ID_Parent = [%d]  parent = [%d][%s]  entity_id = [%d]  frame = [%d]",
			e_entity->ID_Parent, id_parent, e_parent->name_replace(), id_entity, Device->dwFrame);
		//it can't be !!!
	}

	Game->OnDetach(id_parent, id_entity);

	e_entity->ID_Parent = 0xffff;
	C.erase(c);

	// Signal to everyone (including sender) then is alife started
	if (send_message)
		SendBroadcast(P);

	return true;
}

void xrServer::ProcessEventDestroy(NET_Packet& P, const u32& time, u16 ID, NET_Packet* pEPack)
{
	// Parse message
	u16 id_dest = ID;

	// кто должен быть уничтожен
	CSE_Abstract* e_dest = (CSE_Abstract*)GetEntityFromID(id_dest);
	if (!e_dest)
	{
		return;
	};
	u16 parent_id = e_dest->ID_Parent;

	//---------------------------------------------
	NET_Packet	P2, * pEventPack = pEPack;
	P2.w_begin(M_EVENT_PACK);
	//---------------------------------------------
	// check if we have children 
	if (!e_dest->children.empty())
	{
		if (!pEventPack) pEventPack = &P2;

		while (!e_dest->children.empty())
			ProcessEventDestroy(P, time, *e_dest->children.begin(), pEventPack);
	}

	if (0xffff == parent_id && nullptr == pEventPack)
	{
		SendBroadcast(P);
	}
	else
	{
		NET_Packet	tmpP;
		if (0xffff != parent_id && ProcessEventReject(P, time, parent_id, ID, false))
		{
			CGameObject::u_EventGen(tmpP, GE_OWNERSHIP_REJECT, parent_id);
			tmpP.w_u16(id_dest);
			tmpP.w_u8(1);

			if (!pEventPack) pEventPack = &P2;

			pEventPack->w_u8(u8(tmpP.B.count));
			pEventPack->w(tmpP.B.data, tmpP.B.count);
		};

		CGameObject::u_EventGen(tmpP, GE_DESTROY, id_dest);

		pEventPack->w_u8(u8(tmpP.B.count));
		pEventPack->w(tmpP.B.data, tmpP.B.count);
	}

	if (!pEPack && pEventPack)
	{
		SendBroadcast(*pEventPack);
	}
	// Everything OK, so perform entity-destroy
	if (e_dest->m_bALifeControl && ai().get_alife())
	{
		if (ai().alife().objects().object(id_dest, true))
			ai().alife().release(e_dest, false);
	}

	Level().OnDestroyObject(e_dest->ID);

	ISE_Abstract* CompilerHack = e_dest;
	DestroyEntity(CompilerHack);
}

void xrServer::SpawnClient()
{
	IPureServer::SpawnClient();
	Game->OnPlayerConnect(ServerClient->ID);
}

void xrServer::PerformGameExport()
{
	if (!ServerClient->net_Accepted)
		return;
	NET_Packet P;
	P.w_begin(M_SV_CONFIG_GAME);
	Game->net_Export_State(P, ServerClient->ID);
	g_pGameLevel->OnMessage(P.B.data, (u32)P.B.count);
	Game->sv_force_sync = FALSE;
}
void xrServer::Update()
{
	if (Game->sv_force_sync)
		PerformGameExport();
	IPureServer::Update();

}
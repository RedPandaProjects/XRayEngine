#include "stdafx.h"
#include "net_server.h"
#include "IGame_Level.h"

XRayClient::XRayClient()
{
	flags.bConnected = FALSE;
	Clear();
}

XRayClient::~XRayClient() {}

void XRayClient::Clear()
{
	owner = nullptr;
	net_Accepted = FALSE;
}


IPureServer::IPureServer()
{
	ServerClient = NULL;
	CountIndexes = 0;
}

IPureServer::~IPureServer() 
{
	VERIFY(CountIndexes-FreeIndexes.size()==0);
	ServerClient = NULL;
}

void IPureServer::SendBroadcast(NET_Packet& P)
{
	if (!ServerClient || !ServerClient->flags.bConnected)
		return;

	if (ServerClient->net_Accepted)
	{
		g_pGameLevel->OnMessage(P.B.data, (u32)P.B.count);
	}
}

u32 IPureServer::OnMessage(NET_Packet& P)
{
	VERIFY(VerifyEntities());
	return 0;
}

void IPureServer::SpawnClient()
{
	if (!ServerClient)
	{
		Msg("! ERROR: Player state not created - incorrect message sequence!");
		return;
	}

	ServerClient->net_Accepted = TRUE;

	// Export Game Type
	R_ASSERT(g_pGameLevel->Load_GameSpecific_After());
	// end

	PerformGameExport();

	NET_Packet P;
	xr_set<u16> IsSpawned;

	// Replicate current entities on to this client
	for (auto [ID, Entity] : Entities)
	{
		PerformConnectSpawn(Entity, P, IsSpawned);
	}
	//Perform_game_export();
	//SendConnectionData(SV_Client);
	//OnPlayerConnected();
}

void IPureServer::Update()
{
	VerifyEntities();
}

void IPureServer::ProcessUpdate(NET_Packet& P)
{
	if (!ServerClient) 
	{
		return;
	}

	// while has information
	while (!P.r_eof())
	{
		// find entity
		u16 ID = P.r_u16();
		u8 size = P.r_u8();

		u32	_pos = P.r_tell();
		ISE_Abstract* pSEAbstract = GetEntityFromID(ID);

		if (pSEAbstract)
		{
			pSEAbstract->UPDATE_Read(P);

			if ((P.r_tell() - _pos) != size)
			{
				string16 tmp;
				CLSID2TEXT(pSEAbstract->m_tClassID, tmp);
				Debug.fatal(DEBUG_INFO, "Beer from the creator of '%s'; initiator: 0x%08x, r_tell() = %d, pos = %d, size = %d, objectID = %d",tmp, ServerClient->ID.value(), P.r_tell(), _pos, size, pSEAbstract->ID);
			}
		}
		else P.r_advance(size);
	}
}

void IPureServer::ProcessSave(NET_Packet& P)
{
	R_ASSERT(ServerClient);

	// while has information
	while (!P.r_eof())
	{
		// find entity
		u16 ID = P.r_u16();
		u16 size = P.r_u16();

		u32 _pos_start = P.r_tell();
		ISE_Abstract* pSEAbstract = GetEntityFromID(ID);

		if (pSEAbstract)
		{
			pSEAbstract->load(P);
		}
		else P.r_advance(size);

		const u32 _pos_end = P.r_tell();

		if (size != (_pos_end - _pos_start))
		{
			Msg("! load/save mismatch, object: '%s'", pSEAbstract ? pSEAbstract->name_replace() : "unknown");
			// Rollback pos
			P.r_seek(_pos_start + size);
		}
	}
}

void IPureServer::ProcessEvent(NET_Packet& P)
{
	R_ASSERT(!"not_impl");
}

ISE_Abstract* IPureServer::ProcessSpawn(NET_Packet& P, ISE_Abstract* InServerEntity)
{
	ISE_Abstract* ServerEntity = InServerEntity;
	if (!ServerEntity)
	{
		// read spawn information
		string64		EntityName;
		P.r_stringZ(EntityName);
		ServerEntity = CreateEntity(EntityName);
		R_ASSERT3(ServerEntity, "Can't create entity.", EntityName);
		ServerEntity->Spawn_Read(P);
	}
	else 
	{
		VERIFY(ServerEntity->m_bALifeControl);
	}

	ISE_Abstract* EntityParent = nullptr;
	if (ServerEntity->ID_Parent != 0xffff) 
	{
		EntityParent = GetEntityFromID(ServerEntity->ID_Parent);
		if (!EntityParent)
		{
			R_ASSERT(!InServerEntity);
			OnDestroyEntity(ServerEntity);
			return			NULL;
		}
	}


	// check for respawn-capability and create phantom as needed
	if (ServerEntity->RespawnTime && (0xffff == ServerEntity->ID_Phantom))
	{
		// Create phantom
		ISE_Abstract* Phantom = CreateEntity(*ServerEntity->s_name); R_ASSERT(Phantom);
		Phantom->Spawn_Read(P);
		Phantom->ID = GenerateIndex(0xffff);
		Phantom->ID_Phantom = Phantom->ID;						// Self-linked to avoid phantom-breeding
		Phantom->owner = NULL;
		Entities.insert(mk_pair(Phantom->ID, Phantom));

		Phantom->s_flags.set(M_SPAWN_OBJECT_PHANTOM, TRUE);

		// Spawn entity
		ServerEntity->ID = GenerateIndex(ServerEntity->ID);
		ServerEntity->ID_Phantom = Phantom->ID;
		ServerEntity->owner = ServerClient;
		Entities.insert(mk_pair(ServerEntity->ID, ServerEntity));
	}
	else
	{
		if (ServerEntity->s_flags.is(M_SPAWN_OBJECT_PHANTOM))
		{
			// Clone from Phantom
			ServerEntity->ID = GenerateIndex(0xffff);
			ServerEntity->owner = ServerClient;//		= SelectBestClientToMigrateTo	(ServerEntity);
			ServerEntity->s_flags.set(M_SPAWN_OBJECT_PHANTOM, FALSE);
			Entities.insert(mk_pair(ServerEntity->ID, ServerEntity));
		}
		else
		{
			ServerEntity->ID = GenerateIndex(ServerEntity->ID);
			ServerEntity->owner = ServerClient;
			Entities.insert(mk_pair(ServerEntity->ID, ServerEntity));
		}
	}

	// PROCESS NAME; Name this entity
	if (ServerClient && (ServerEntity->s_flags.is(M_SPAWN_OBJECT_ASPLAYER)))
	{
		ServerClient->owner = ServerEntity;
	}
	// Parent-Connect
	if (!InServerEntity)
	{
		OnCreateEntity(ServerEntity, EntityParent);
	}

	// create packet and broadcast packet to everybody
	NET_Packet				NewPacket;
	if (ServerClient)
	{
		// For local ONLY
		EntityParent->Spawn_Write(NewPacket, TRUE);
		if (EntityParent->s_flags.is(M_SPAWN_UPDATE))
		{
			EntityParent->UPDATE_Write(NewPacket);
		}

		g_pGameLevel->OnMessage(NewPacket.B.data, (u32)NewPacket.B.count);
	}

	return ServerEntity;
}

ISE_Abstract* IPureServer::CreateEntity(const char* Name)
{
	R_ASSERT(!"not_impl");
	return nullptr;
}

void IPureServer::DestroyEntity(ISE_Abstract* ServerEntity)
{
	R_ASSERT(ServerEntity);

	Entities.erase(ServerEntity->ID);
	FreeIndex(ServerEntity->ID);

	if (ServerEntity->owner && ServerEntity->owner->owner == ServerEntity)
		ServerEntity->owner->owner = NULL;

	ServerEntity->owner = nullptr;
}

void IPureServer::OnCreateEntity(ISE_Abstract* Entity, ISE_Abstract* ParentEntity)
{
	R_ASSERT(!"not_impl");
}
void IPureServer::OnDestroyEntity(ISE_Abstract*)
{
	R_ASSERT(!"not_impl");
}

u16 IPureServer::GenerateIndex(u16 CurrentIndex /*= 0xFFFF*/)
{
	if (CurrentIndex != 0xFFFF)
	{
		return CurrentIndex;
	}
	if (FreeIndexes.size())
	{
		u16 Index = FreeIndexes.back();
		FreeIndexes.pop_back();
		return Index;
	}
	VERIFY(CountIndexes<0xFFFF);
	return CountIndexes++;
}


void IPureServer::FreeIndex(u16 CurrentIndex)
{
	FreeIndexes.push_back(CurrentIndex);
}

void IPureServer::ClearIndexes()
{
	CountIndexes = 0;
	FreeIndexes.clear();
}

ISE_Abstract* IPureServer::GetEntityFromID(u16 Index) const
{
	if (Index == 0xFFFF)
	{
		return nullptr;
	}
	auto Iterator =  Entities.find(Index);
	if (Iterator == Entities.end())
	{
		return nullptr;
	}
	return Iterator->second;
}

void IPureServer::PerformGameExport()
{

}

bool IPureServer::VerifyEntities() const
{
#ifdef DEBUG
	static bool IsInitialized = false;
	static bool IsUse = false;
	if (!IsInitialized)
	{
		IsInitialized = true;
		if (strstr(Core.Params, "-verify_entities"))	IsUse = true;
	}
	if (!IsUse)						return true;

	auto		I = Entities.begin();
	auto		E = Entities.end();
	for (; I != E; ++I) {
		VERIFY2((*I).first != 0xffff, "SERVER : Invalid entity id as a map key - 0xffff");
		VERIFY2((*I).second, "SERVER : Null entity object in the map");
		VERIFY3((*I).first == (*I).second->ID, "SERVER : ID mismatch - map key doesn't correspond to the real entity ID", (*I).second->name_replace());
		VerifyEntity((*I).second);
	}
#endif
	return								(true);
}

void IPureServer::VerifyEntity(const ISE_Abstract* entity) const
{
	VERIFY(entity->m_wVersion != 0);
	if (entity->ID_Parent != 0xffff) {
		auto	J = Entities.find(entity->ID_Parent);
		if(J == Entities.end())
		{
			Debug.fatal(__FILE__,__LINE__,__FUNCTION__,"SERVER : Cannot find parent in the map [%s][%s]", entity->name_replace(),entity->name());
		}
			
		VERIFY3((*J).second, "SERVER : Null entity object in the map", entity->name_replace());
		VERIFY3((*J).first == (*J).second->ID, "SERVER : ID mismatch - map key doesn't correspond to the real entity ID", (*J).second->name_replace());
		VERIFY3(std::find((*J).second->children.begin(), (*J).second->children.end(), entity->ID) != (*J).second->children.end(), "SERVER : Parent/Children relationship mismatch - Object has parent, but corresponding parent doesn't have children", (*J).second->name_replace());
	}

	auto	I = entity->children.begin();
	auto		E = entity->children.end();
	for (; I != E; ++I) {
		VERIFY3(*I != 0xffff, "SERVER : Invalid entity children id - 0xffff", entity->name_replace());
		auto	J = Entities.find(*I);
		VERIFY3(J != Entities.end(), "SERVER : Cannot find children in the map", entity->name_replace());
		VERIFY3((*J).second, "SERVER : Null entity object in the map", entity->name_replace());
		VERIFY3((*J).first == (*J).second->ID, "SERVER : ID mismatch - map key doesn't correspond to the real entity ID", (*J).second->name_replace());
		VERIFY3((*J).second->ID_Parent == entity->ID, "SERVER : Parent/Children relationship mismatch - Object has children, but children doesn't have parent", (*J).second->name_replace());
	}
}

void IPureServer::PerformConnectSpawn(ISE_Abstract* Entity, NET_Packet& P, xr_set<u16>& IsSpawned)
{
	P.B.count = 0;
	if (IsSpawned.contains(Entity->ID))
		return;

	IsSpawned.insert(Entity->ID);

	if (Entity->s_flags.is(M_SPAWN_OBJECT_PHANTOM))	return;

	// Connectivity order
	ISE_Abstract* Parent = GetEntityFromID(Entity->ID_Parent);
	if (Parent)
	{
		PerformConnectSpawn(Parent, P, IsSpawned);
	}

	// Process
	Flags16 save = Entity->s_flags;
	//-------------------------------------------------
	Entity->s_flags.set(M_SPAWN_UPDATE, true);
	if (!Entity->owner)
	{
		// PROCESS NAME; Name this entity
		if (Entity->s_flags.is(M_SPAWN_OBJECT_ASPLAYER))
		{
			ServerClient->owner = Entity;
			Entity->set_name_replace("");
		}

		// Associate
		Entity->owner = ServerClient;
		Entity->Spawn_Write(P, TRUE);
		Entity->UPDATE_Write(P);

		if (!Entity->keep_saved_data_anyway())
			Entity->client_data.clear();
	}
	else
	{
		Entity->Spawn_Write(P, FALSE);
		Entity->UPDATE_Write(P);
	}
	//-----------------------------------------------------
	Entity->s_flags = save;
	g_pGameLevel->OnMessage(P.B.data, (u32)P.B.count);
}

#pragma once

enum
{
	M_SPAWN_OBJECT_LOCAL = (1 << 0),	// after spawn it becomes local (authorative)
	M_SPAWN_OBJECT_HASUPDATE = (1 << 2),	// after spawn info it has update inside message
	M_SPAWN_OBJECT_ASPLAYER = (1 << 3),	// after spawn it must become viewable
	M_SPAWN_OBJECT_PHANTOM = (1 << 4),	// after spawn it must become viewable
	M_SPAWN_VERSION = (1 << 5),	// control version
	M_SPAWN_UPDATE = (1 << 6),	// + update packet
	M_SPAWN_TIME = (1 << 7),	// + spawn time
	M_SPAWN_DENIED = (1 << 8),	// don't spawn entity with this flag

	M_SPAWN_OBJECT_FORCEDWORD = u32(-1)
};

enum {
	M_UPDATE = 0,	// DUAL: Update state
	M_SPAWN,					// DUAL: Spawning, full state

	M_SV_CONFIG_NEW_CLIENT,
	M_SV_CONFIG_GAME,

	M_EVENT,					// Game Event

	M_CHANGE_LEVEL,				// changing level
	M_LOAD_GAME,
	M_SAVE_PACKET,

	M_SWITCH_DISTANCE,
	M_EVENT_PACK,					// Pack of M_EVENT

	MSG_FORCEDWORD = u32(-1)
};

enum {
	GE_OWNERSHIP_TAKE,			// DUAL: Client request for ownership of an item
	GE_OWNERSHIP_REJECT,		// DUAL: Client request ownership rejection
	GE_TRANSFER_AMMO,			// DUAL: Take ammo out of weapon for our weapon
	GE_HIT,						//
	GE_DIE,						//
	GE_ASSIGN_KILLER,			//
	GE_DESTROY,					// authorative client request for entity-destroy
	GE_DESTROY_REJECT,			// GE_DESTROY + GE_OWNERSHIP_REJECT
	GE_TELEPORT_OBJECT,
	GE_INV_OWNER_SETNAME,

	GE_BUY,
	GE_INFO_TRANSFER,			//transfer _new_ info on PDA

	GE_TRADE_SELL,
	GE_TRADE_BUY,

	GE_WPN_AMMO_ADD,
	GE_WPN_STATE_CHANGE,

	GE_ADDON_ATTACH,
	GE_ADDON_DETACH,
	GE_ADDON_CHANGE,
	GE_INSTALL_UPGRADE,

	GE_GRENADE_EXPLODE,
	GE_INV_ACTION,				//a action beign taken on inventory
	GE_INV_BOX_STATUS,
	GE_INV_OWNER_STATUS,

	GE_ZONE_STATE_CHANGE,

	GE_MOVE_ACTOR,				//move actor to desired position instantly
	GE_ACTOR_MAX_POWER,
	GE_ACTOR_MAX_HEALTH,

	GE_CHANGE_POS,

	GE_GAME_EVENT,

	GE_CHANGE_VISUAL,
	GE_MONEY,

	GEG_PLAYER_ACTIVATE_SLOT,
	GEG_PLAYER_ITEM2SLOT,
	GEG_PLAYER_ITEM2BELT,
	GEG_PLAYER_ITEM2RUCK,
	GEG_PLAYER_ITEM_EAT,
	GEG_PLAYER_ACTIVATEARTEFACT,

	GEG_PLAYER_WEAPON_HIDE_STATE,
	GEG_PLAYER_DISABLE_SPRINT,

	GEG_PLAYER_ATTACH_HOLDER,
	GEG_PLAYER_DETACH_HOLDER,

	//-------------------------------------
	GE_HIT_STATISTIC,
	//-------------------------------------
	GE_KILL_SOMEONE,
	GE_LAUNCH_ROCKET,

	GE_FORCEDWORD = u32(-1)
};



class IPureServer;
class ENGINE_API XRayClient
{
public:
    XRayClient();
    virtual ~XRayClient();
	struct Flags
	{
		u32 bConnected : 1;
	};
	ISE_Abstract*   owner;
	BOOL            net_Ready;
	BOOL            net_Accepted;
    ClientID        ID;
    Flags           flags; // local/host/normal
	virtual void    Clear();
};
class CServerInfo;

class ENGINE_API IPureServer
{


public:
                                        IPureServer         ();
    virtual                             ~IPureServer        ();

    virtual void                        SendBroadcast       (NET_Packet& P);
    virtual u32                         OnMessage           (NET_Packet& P); // Non-Zero means broadcasting with "flags" as returned
    virtual void                        SpawnClient			();
	virtual void						Update				();

	virtual void						ProcessUpdate		(NET_Packet& P);
	virtual void						ProcessSave			(NET_Packet& P);
	virtual void						ProcessEvent		(NET_Packet& P);
    virtual ISE_Abstract*               ProcessSpawn        (NET_Packet& P,ISE_Abstract*ServerEntity);

    virtual ISE_Abstract*               CreateEntity        (const char* Name);
    virtual void                        DestroyEntity       (ISE_Abstract* ServerEntity);
    
    virtual void                        OnCreateEntity      (ISE_Abstract* Entity, ISE_Abstract* ParentEntity);
    virtual void                        OnDestroyEntity     (ISE_Abstract*);

	XRayClient*                         GetServerClient     () { return ServerClient; };
	const shared_str&                   GetConnectOptions   () const { return connect_options; }

    u16                                 GenerateIndex       (u16 CurrentIndex = 0xFFFF);
    void                                FreeIndex           (u16 CurrentIndex);
    void                                ClearIndexes        ();
    ISE_Abstract*                       GetEntityFromID     (u16 Index) const;



protected:
	virtual void						PerformGameExport	();
	bool								VerifyEntities		() const;
	void								VerifyEntity		(const ISE_Abstract* Entity) const;

	shared_str                          connect_options;
    XRayClient*                         ServerClient;
    x_unordered_map<u16, ISE_Abstract*> Entities;
private:
	void								PerformConnectSpawn(ISE_Abstract* Entity, NET_Packet& P,xr_set<u16>&IsSpawned);
	xr_vector<u16>                      FreeIndexes;
	u16                                 CountIndexes;

};
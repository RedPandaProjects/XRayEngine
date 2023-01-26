#pragma once
// xrServer.h: interface for the xrServer class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "../xrEngine/net_server.h"
#include "game_sv_base.h"
#include "id_generator.h"
#include "../xrEngine/mp_logging.h"
#include "xrClientsPool.h"

#ifdef DEBUG
//. #define SLOW_VERIFY_ENTITIES
#endif

class CSE_Abstract;

const u32 NET_Latency = 50; // time in (ms)

// t-defs
typedef x_unordered_map<u16, CSE_Abstract*> xrS_entities;


// main
struct svs_respawn {
    u32 timestamp;
    u16 phantom;
};
IC bool operator<(const svs_respawn& A, const svs_respawn& B) { return A.timestamp < B.timestamp; }

class xrServer : public IPureServer 
{

public:
                    xrServer            ();
                    ~xrServer           ();
    void            SpawnClient         () override;
    void            Update              () override;
    u32             OnMessage           (NET_Packet& P) override;
    ISE_Abstract*   CreateEntity        (const char* Name) override;
    void            DestroyEntity       (ISE_Abstract* ServerEntity) override;
    void            OnCreateEntity      (ISE_Abstract* Entity, ISE_Abstract* ParentEntity) override;
	void            OnDestroyEntity     (ISE_Abstract*) override;

	void            ProcessEvent        (NET_Packet& P) override;
	bool			ProcessEventReject  (NET_Packet& P, const u32& time, const u16 id_parent, const u16 id_entity, bool send_message = true);
	void	        ProcessEventDestroy (NET_Packet& P, const u32& time, u16 ID, NET_Packet* pEPack);
    game_sv_GameState* Game;
protected:
    void            PerformGameExport   () override;

};

#ifdef DEBUG
enum e_dbg_net_Draw_Flags {

    dbg_draw_actor_alive = (1 << 0),
    dbg_draw_actor_dead = (1 << 1),
    dbg_draw_customzone = (1 << 2),
    dbg_draw_teamzone = (1 << 3),
    dbg_draw_invitem = (1 << 4),
    dbg_draw_actor_phys = (1 << 5),
    dbg_draw_customdetector = (1 << 6),
    dbg_destroy = (1 << 7),
    dbg_draw_autopickupbox = (1 << 8),
    dbg_draw_rp = (1 << 9),
    dbg_draw_climbable = (1 << 10),
    dbg_draw_skeleton = (1 << 11)
};
extern Flags32 dbg_net_Draw_Flags;
#endif
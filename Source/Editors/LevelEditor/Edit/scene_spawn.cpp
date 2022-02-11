#include "stdafx.h"
#include "../AI/game_spawn_constructor.h"
#pragma hdrstop



void EScene::BuildSpawn()
{
    m_Spawn.clear();
    CGameSpawnConstructor SpawnConstructor("editor", m_Spawn, "editor", true);

}
#include "stdafx.h"
#include "../AI/game_spawn_constructor.h"

void EScene::BuildSpawn()
{
    m_Spawn.clear();
    CGameSpawnConstructor SpawnConstructor(Scene->m_LevelOp.m_FNLevelPath.c_str(), m_Spawn, Scene->m_LevelOp.m_FNLevelPath.c_str(), true);
}
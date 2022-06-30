#include "stdafx.h"
#include "../AI/game_spawn_constructor.h"

bool EScene::BuildSpawn()
{
	if (m_cfrom_builder.empty())
	{
		if (!BuildCForm())
		{
			ELog.DlgMsg(mtError, mbOK, "CFrom is empty!\nSee log.");
			return false;
		}
	}
	if (m_level_graph.empty())
	{
		if (!BuildAIMap())
		{
			ELog.DlgMsg(mtError, mbOK, "AIMap is empty!\nSee log.");
			return false;
		}
	}
	if (m_game_graph.empty())
	{
		if (!BuildGameGraph())
		{
			ELog.DlgMsg(mtError, mbOK, "GameGraph is empty!\nSee log.");
			return false;
		}
	}
	UI->ShowConsole();
	m_spawn_data.clear();
	CGameSpawnConstructor SpawnConstructor;
	if (!SpawnConstructor.build(Scene->m_LevelOp.m_FNLevelPath.c_str(), m_spawn_data, Scene->m_LevelOp.m_FNLevelPath.c_str(), true))
	{
		ELog.DlgMsg(mtError, mbOK, "! Failed build spawn! \nSee log.");
		UI->CloseConsole();
		return false;
	}
	UI->CloseConsole();
	return true;
}
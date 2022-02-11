#include "stdafx.h"
#pragma hdrstop


ILevelGraph* EScene::GetLevelGraph()
{
	return &m_level_graph;
}

void EScene::BuildAIMap()
{
	m_level_graph.build();

}

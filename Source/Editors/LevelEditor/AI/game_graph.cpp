#include "stdafx.h"
#include "game_graph.h"
#include "game_graph_builder.h"

CGameGraph::CGameGraph(CGameGraphBuilder* Builder)
{
	m_header = *Builder->graph_header();
	m_nodes = Builder->graph_vertex();
	m_edge = (BYTE*)Builder->graph_edges();
	m_current_level_some_vertex_id = _GRAPH_ID(-1);
	m_enabled.assign(header().vertex_count(), true);
	u8* temp = (u8*)(m_nodes + header().vertex_count());
	temp += header().edge_count() * sizeof(CGameGraph::CEdge);
	/*m_cross_tables = (u32*)(((CLevelPoint*)temp) + header().death_point_count());*/
	m_current_level_cross_table = &Builder->cross();
}
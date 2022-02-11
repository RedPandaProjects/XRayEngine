#include "stdafx.h"
#include "game_graph_editor.h"

CGameGraphEditor::CGameGraphEditor()
{
	m_current_level_cross_table = nullptr;
	m_nodes = nullptr;
	m_edges = nullptr;
}

CGameGraphEditor::~CGameGraphEditor()
{
	xr_free(m_nodes);
	xr_free(m_edges);
	xr_delete(m_current_level_cross_table);
}

void CGameGraphEditor::set_current_level(u32 level_id)
{
	R_ASSERT(level_id == 0);
}

void CGameGraphEditor::realloc(const CHeader& new_header)
{
	m_header = new_header;
	m_nodes = (CVertex*)xr_realloc(m_nodes, sizeof(CVertex) * new_header.m_vertex_count);
	if (new_header.m_edge_count)
	{
		m_edges = (BYTE*)xr_realloc(m_edges, sizeof(CEdge) * new_header.m_edge_count);
	}
	else
	{
		xr_free(m_edges);
	}
	m_current_level_some_vertex_id = _GRAPH_ID(-1);
	m_enabled.assign(header().vertex_count(), true);
}

void CGameGraphEditor::set_cross_table(IGameLevelCrossTable* cross_table)
{
	xr_delete(m_current_level_cross_table);
	m_current_level_some_vertex_id = 0;
	m_current_level_cross_table = cross_table;
}

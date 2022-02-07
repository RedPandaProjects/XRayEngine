////////////////////////////////////////////////////////////////////////////
//	Module 		: game_level_cross_table.h
//	Created 	: 20.02.2003
//  Modified 	: 13.11.2003
//	Author		: Dmitriy Iassenev
//	Description : Cross table between game and level graphs
////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef AI_COMPILER
#include "../../xrGame/xrLevel.h"
#else // AI_COMPILER
#	include "../xrEngine/xrLevel.h"
#endif // AI_COMPILER

#include "alife_space.h"
#include "..\xrEngine\game_graph_space.h"

#define CROSS_TABLE_NAME					"level.gct"

#define CROSS_TABLE_CHUNK_VERSION			0
#define CROSS_TABLE_CHUNK_DATA				1


class CGameLevelCrossTable
{
	friend class CLevelGameGraph;
	friend class CCrossTableBuilder;
	friend class CRenumbererConverter;
	friend class CGameGraphBuilder;

public:
#pragma pack(push,2)
	class CHeader
	{
		u32					dwVersion;
		u32					dwNodeCount;
		u32					dwGraphPointCount;
		xrGUID				m_level_guid;
		xrGUID				m_game_guid;

	public:
		IC	u32				version					() const;
		IC	u32				level_vertex_count		() const;
		IC	u32				game_vertex_count		() const;
		IC	const xrGUID	&level_guid				() const;
		IC	const xrGUID	&game_guid				() const;
		friend class CLevelGameGraph;
		friend class CCrossTableBuilder;
		friend class CRenumbererConverter;
		friend class CGameGraphBuilder;
	};
	
	class  CCell {
		GameGraph::_GRAPH_ID	tGraphIndex;
		float				fDistance;
	public:
		IC	GameGraph::_GRAPH_ID game_vertex_id			() const;
		IC	float			distance				() const;
		friend class CLevelGameGraph;
		friend class CCrossTableBuilder;
		friend class CRenumbererConverter;
		friend class CGameGraphBuilder;
	};
#pragma pack(pop)

private:
	CCell					*m_tpaCrossTable;
	CHeader					m_Header;

public:
	IC						CGameLevelCrossTable	(const CHeader&Header);

public:
	IC virtual				~CGameLevelCrossTable	();
	IC		const CCell		&vertex					(u32 level_vertex_id) const;
	IC			  CCell     &vertex					(u32 level_vertex_id) ;
	IC		const CHeader	&header					() const;
};

#include "game_level_cross_table_inline.h"
#include "game_graph_space.h"
#include "game_graph.h"
////////////////////////////////////////////////////////////////////////////
//	Module 		: game_graph_inline.h
//	Created 	: 18.02.2003
//  Modified 	: 13.11.2003
//	Author		: Dmitriy Iassenev
//	Description : Game graph inline functions
////////////////////////////////////////////////////////////////////////////

IC const IGameGraph::CHeader &IGameGraph::header					() const
{
	return						(m_header);
}

IC	bool IGameGraph::mask											(const svector<_LOCATION_ID,GameGraph::LOCATION_TYPE_COUNT> &M, const _LOCATION_ID E[GameGraph::LOCATION_TYPE_COUNT]) const
{
	for (int i=0; i<GameGraph::LOCATION_TYPE_COUNT; ++i)
		if ((M[i] != E[i]) && (255 != M[i]))
			return(false);
	return(true);
}

IC	bool IGameGraph::mask											(const _LOCATION_ID M[GameGraph::LOCATION_TYPE_COUNT], const _LOCATION_ID E[GameGraph::LOCATION_TYPE_COUNT]) const
{
	for (int i=0; i<GameGraph::LOCATION_TYPE_COUNT; ++i)
		if ((M[i] != E[i]) && (255 != M[i]))
			return(false);
	return(true);
}

IC	float IGameGraph::distance										(const _GRAPH_ID tGraphID0, const _GRAPH_ID tGraphID1) const
{
	IGameGraph::const_iterator				i, e;
	begin						(tGraphID0,i,e);
	for ( ; i != e; ++i)
		if (value(tGraphID0,i) == tGraphID1)
			return				(edge_weight(i));
	R_ASSERT2					(false,"There is no proper graph point neighbour!");
	return						(_GRAPH_ID(-1));
}

IC	bool IGameGraph::accessible										(u32 const vertex_id) const
{
	VERIFY						(valid_vertex_id(vertex_id));
	return						(m_enabled[vertex_id]);
}

IC	void IGameGraph::accessible										(u32 const vertex_id, bool value) const
{
	VERIFY						(valid_vertex_id(vertex_id));
	m_enabled[vertex_id]		= value;
}

IC	bool IGameGraph::valid_vertex_id								(u32 const vertex_id) const
{
	return						(vertex_id < header().vertex_count());
}

IC	void IGameGraph::begin											(u32 const vertex_id, const_iterator &start, const_iterator &end) const
{
	start	= get_edges()	+ vertex(_GRAPH_ID(vertex_id))->edge_offset();
	end		= start			+ vertex(_GRAPH_ID(vertex_id))->edge_count();
}

IC	const IGameGraph::_GRAPH_ID &IGameGraph::value					(u32 const vertex_id, const_iterator &i) const
{
	return						(i->vertex_id());
}

IC	const float &IGameGraph::edge_weight							(const_iterator i) const
{
	return						(i->distance());
}

IC	const IGameGraph::CVertex *IGameGraph::vertex					(u32 const vertex_id) const
{
	return						(get_nodes() + vertex_id);
}

IC	const u8 &IGameGraph::CHeader::version							() const
{
	return						(m_version);
}

IC	GameGraph::_LEVEL_ID GameGraph::CHeader::level_count			() const
{
	VERIFY						(m_levels.size() < (u32(1) << (8*sizeof(GameGraph::_LEVEL_ID))));
	return						((GameGraph::_LEVEL_ID)m_levels.size());
}

IC	const GameGraph::_GRAPH_ID &GameGraph::CHeader::vertex_count	() const
{
	return						(m_vertex_count);
}

IC	const u32 &GameGraph::CHeader::edge_count						() const
{
	return						(m_edge_count);
}

IC	const u32 &GameGraph::CHeader::death_point_count				() const
{
	return						(m_death_point_count);
}

IC	const GameGraph::LEVEL_MAP &GameGraph::CHeader::levels			() const
{
	return						(m_levels);
}

IC	const GameGraph::SLevel &GameGraph::CHeader::level				(const _LEVEL_ID &id) const
{
	LEVEL_MAP::const_iterator	I = levels().find(id);
	R_ASSERT2					(I != levels().end(),make_string("there is no specified level in the game graph : %d",id));
	return						((*I).second);
}

IC	const GameGraph::SLevel &GameGraph::CHeader::level				(LPCSTR level_name) const
{
	LEVEL_MAP::const_iterator	I = levels().begin();
	LEVEL_MAP::const_iterator	E = levels().end();
	for ( ; I != E; ++I)
		if (!xr_strcmp((*I).second.name(),level_name))
			return				((*I).second);
	
#ifdef DEBUG
	Msg							("! There is no specified level %s in the game graph!",level_name);
	return						(levels().begin()->second);
#else
	R_ASSERT3					(false,"There is no specified level in the game graph!",level_name);
	NODEFAULT;
#endif
}

IC	const GameGraph::SLevel *GameGraph::CHeader::level				(LPCSTR level_name, bool) const
{
	LEVEL_MAP::const_iterator	I = levels().begin();
	LEVEL_MAP::const_iterator	E = levels().end();
	for ( ; I != E; ++I)
		if (!xr_strcmp((*I).second.name(),level_name))
			return				(&(*I).second);
	
	return						(0);
}

IC	const xrGUID &IGameGraph::CHeader::guid							() const
{
	return						(m_guid);
}

IC	const Fvector &GameGraph::CVertex::level_point					() const
{
	return						(tLocalPoint);
}

IC	const Fvector &GameGraph::CVertex::game_point					() const
{
	return						(tGlobalPoint);
}

IC	GameGraph::_LEVEL_ID GameGraph::CVertex::level_id				() const
{
	return						(tLevelID);
}

IC	u32 GameGraph::CVertex::level_vertex_id							() const
{
	return						(tNodeID);
}

IC	const u8 *GameGraph::CVertex::vertex_type						() const
{
	return						(tVertexTypes);
}

IC	const u8 &GameGraph::CVertex::edge_count						() const
{
	return						(tNeighbourCount);
}

IC	const u8 &GameGraph::CVertex::death_point_count					() const
{
	return						(tDeathPointCount);
}

IC	const u32 &GameGraph::CVertex::edge_offset						() const
{
	return						(dwEdgeOffset);
}

IC	const u32 &GameGraph::CVertex::death_point_offset				() const
{
	return						(dwPointOffset);
}

IC	const GameGraph::_GRAPH_ID &GameGraph::CEdge::vertex_id			() const
{
	return						(m_vertex_id);
}

IC	const float &GameGraph::CEdge::distance							() const
{
	return						(m_path_distance);
}

IC	void IGameGraph::begin_spawn									(u32 const vertex_id, const_spawn_iterator &start, const_spawn_iterator &end) const
{
	const CVertex				*object = vertex(vertex_id);
	start						= get_level_points() + object->death_point_offset();
	end							= start				 + object->death_point_count();
}

IC	void IGameGraph::set_invalid_vertex								(_GRAPH_ID &vertex_id) const
{
	vertex_id					= _GRAPH_ID(-1);
	VERIFY						(!valid_vertex_id(vertex_id));
}

IC	GameGraph::_GRAPH_ID IGameGraph::vertex_id						(const IGameGraph::CVertex *vertex) const
{
	VERIFY						(valid_vertex_id(_GRAPH_ID(vertex - get_nodes())));
	return						(_GRAPH_ID(vertex - get_nodes()));
}

IC	const GameGraph::_GRAPH_ID &IGameGraph::current_level_vertex	() const
{
	VERIFY						(valid_vertex_id(m_current_level_some_vertex_id));
	return						(m_current_level_some_vertex_id);
}

IC	void GameGraph::SLevel::load									(IReader *reader)
{
	reader->r_stringZ			(m_name);
	reader->r_fvector3			(m_offset);
	reader->r					(&m_id,sizeof(m_id));
	reader->r_stringZ			(m_section);
	reader->r					(&m_guid,sizeof(m_guid));
}

IC	void GameGraph::SLevel::save									(IWriter *writer)
{
	writer->w_stringZ			(m_name);
	writer->w_fvector3			(m_offset);
	writer->w					(&m_id,sizeof(m_id));
	writer->w_stringZ			(m_section);
	writer->w					(&m_guid,sizeof(m_guid));
}

IC	void GameGraph::CHeader::load									(IReader *reader)
{
	reader->r					(&m_version,			sizeof(m_version));
	reader->r					(&m_vertex_count,		sizeof(m_vertex_count));
	reader->r					(&m_edge_count,			sizeof(m_edge_count));
	reader->r					(&m_death_point_count,	sizeof(m_death_point_count));
	reader->r					(&m_guid,				sizeof(m_guid));
	
	u32							level_count = reader->r_u8();

	m_levels.clear				();
	for (u32 i=0; i<level_count; ++i) {
		SLevel					l_tLevel;
		l_tLevel.load			(reader);
		m_levels.insert			(mk_pair(l_tLevel.id(),l_tLevel));
	}
}

IC	void GameGraph::CHeader::save									(IWriter *writer)
{
	writer->w					(&m_version,			sizeof(m_version));
	writer->w					(&m_vertex_count,		sizeof(m_vertex_count));
	writer->w					(&m_edge_count,			sizeof(m_edge_count));
	writer->w					(&m_death_point_count,	sizeof(m_death_point_count));
	writer->w					(&m_guid,				sizeof(m_guid));
	
	VERIFY						(m_levels.size() < u32((1) << (8*sizeof(u8))));
	writer->w_u8				((u8)m_levels.size());

	LEVEL_MAP::iterator			I = m_levels.begin();
	LEVEL_MAP::iterator			E = m_levels.end();
	for ( ; I != E; ++I)
		(*I).second.save		(writer);
}


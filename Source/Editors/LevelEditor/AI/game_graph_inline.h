


IC CGameGraph::CGameGraph()
{
}

IC CGameGraph::~CGameGraph()
{
	xr_delete(m_current_level_cross_table);
#ifdef AI_COMPILER
	FS.r_close(m_reader);
#endif // AI_COMPILER
}

IC const CGameGraph::CHeader& CGameGraph::header() const
{
	return						(m_header);
}

IC	bool CGameGraph::mask(const svector<_LOCATION_ID, GameGraph::LOCATION_TYPE_COUNT>& M, const _LOCATION_ID E[GameGraph::LOCATION_TYPE_COUNT]) const
{
	for (int i = 0; i < GameGraph::LOCATION_TYPE_COUNT; ++i)
		if ((M[i] != E[i]) && (255 != M[i]))
			return(false);
	return(true);
}

IC	bool CGameGraph::mask(const _LOCATION_ID M[GameGraph::LOCATION_TYPE_COUNT], const _LOCATION_ID E[GameGraph::LOCATION_TYPE_COUNT]) const
{
	for (int i = 0; i < GameGraph::LOCATION_TYPE_COUNT; ++i)
		if ((M[i] != E[i]) && (255 != M[i]))
			return(false);
	return(true);
}

IC	float CGameGraph::distance(const _GRAPH_ID tGraphID0, const _GRAPH_ID tGraphID1) const
{
	const_iterator				i, e;
	begin(tGraphID0, i, e);
	for (; i != e; ++i)
		if (value(tGraphID0, i) == tGraphID1)
			return				(edge_weight(i));
	R_ASSERT2(false, "There is no proper graph point neighbour!");
	return						(_GRAPH_ID(-1));
}

IC	bool CGameGraph::accessible(u32 const vertex_id) const
{
	VERIFY(valid_vertex_id(vertex_id));
	return						(m_enabled[vertex_id]);
}

IC	void CGameGraph::accessible(u32 const vertex_id, bool value) const
{
	VERIFY(valid_vertex_id(vertex_id));
	m_enabled[vertex_id] = value;
}

IC	bool CGameGraph::valid_vertex_id(u32 const vertex_id) const
{
	return						(vertex_id < header().vertex_count());
}

IC	void CGameGraph::begin(u32 const vertex_id, const_iterator& start, const_iterator& end) const
{
	end = (start = (const CEdge*)((BYTE*)m_nodes + vertex(_GRAPH_ID(vertex_id))->edge_offset())) + vertex(_GRAPH_ID(vertex_id))->edge_count();
}

IC	const CGameGraph::_GRAPH_ID& CGameGraph::value(u32 const vertex_id, const_iterator& i) const
{
	return						(i->vertex_id());
}

IC	const float& CGameGraph::edge_weight(const_iterator i) const
{
	return						(i->distance());
}

IC	const CGameGraph::CVertex* CGameGraph::vertex(u32 const vertex_id) const
{
	return						(m_nodes + vertex_id);
}

IC	const u8& CGameGraph::CHeader::version() const
{
	return						(m_version);
}

IC	GameGraph::_LEVEL_ID GameGraph::CHeader::level_count() const
{
	VERIFY(m_levels.size() < (u32(1) << (8 * sizeof(GameGraph::_LEVEL_ID))));
	return						((GameGraph::_LEVEL_ID)m_levels.size());
}

IC	const GameGraph::_GRAPH_ID& GameGraph::CHeader::vertex_count() const
{
	return						(m_vertex_count);
}

IC	u32 const& GameGraph::CHeader::edge_count() const
{
	return						(m_edge_count);
}

IC	u32 const& GameGraph::CHeader::death_point_count() const
{
	return						(m_death_point_count);
}

IC	const xrGUID& CGameGraph::CHeader::guid() const
{
	return						(m_guid);
}

IC	const Fvector& GameGraph::CVertex::level_point() const
{
	return						(tLocalPoint);
}

IC	const Fvector& GameGraph::CVertex::game_point() const
{
	return						(tGlobalPoint);
}

IC	GameGraph::_LEVEL_ID GameGraph::CVertex::level_id() const
{
	return						(tLevelID);
}

IC	u32 GameGraph::CVertex::level_vertex_id() const
{
	return						(tNodeID);
}

IC	const u8* GameGraph::CVertex::vertex_type() const
{
	return						(tVertexTypes);
}

IC	const u8& GameGraph::CVertex::edge_count() const
{
	return						(tNeighbourCount);
}

IC	const u8& GameGraph::CVertex::death_point_count() const
{
	return						(tDeathPointCount);
}

IC	u32 const& GameGraph::CVertex::edge_offset() const
{
	return						(dwEdgeOffset);
}

IC	u32 const& GameGraph::CVertex::death_point_offset() const
{
	return						(dwPointOffset);
}

IC	const GameGraph::_GRAPH_ID& GameGraph::CEdge::vertex_id() const
{
	return						(m_vertex_id);
}

IC	const float& GameGraph::CEdge::distance() const
{
	return						(m_path_distance);
}

IC	void CGameGraph::begin_spawn(u32 const vertex_id, const_spawn_iterator& start, const_spawn_iterator& end) const
{
	const CVertex* object = vertex(vertex_id);
	start = (const_spawn_iterator)((u8*)m_nodes + object->death_point_offset());
	end = start + object->death_point_count();
}

IC	void CGameGraph::set_invalid_vertex(_GRAPH_ID& vertex_id) const
{
	vertex_id = _GRAPH_ID(-1);
	VERIFY(!valid_vertex_id(vertex_id));
}

IC	GameGraph::_GRAPH_ID CGameGraph::vertex_id(const CGameGraph::CVertex* vertex) const
{
	VERIFY(valid_vertex_id(_GRAPH_ID(vertex - m_nodes)));
	return						(_GRAPH_ID(vertex - m_nodes));
}

IC	const GameGraph::_GRAPH_ID& CGameGraph::current_level_vertex() const
{
	VERIFY(valid_vertex_id(m_current_level_some_vertex_id));
	return						(m_current_level_some_vertex_id);
}

IC	void GameGraph::SLevel::load(IReader* reader)
{
	reader->r_stringZ(m_name);
	reader->r_fvector3(m_offset);
	reader->r(&m_id, sizeof(m_id));
	reader->r_stringZ(m_section);
	reader->r(&m_guid, sizeof(m_guid));
}

IC	void GameGraph::SLevel::save(IWriter* writer)
{
	writer->w_stringZ(m_name);
	writer->w_fvector3(m_offset);
	writer->w(&m_id, sizeof(m_id));
	writer->w_stringZ(m_section);
	writer->w(&m_guid, sizeof(m_guid));
}

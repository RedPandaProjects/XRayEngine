#pragma once
class ISE_ALifeGraphPoint
{
public:
	virtual class ISE_Abstract*		CastAbstract				() = 0;
	shared_str						m_caConnectionLevelName;
	shared_str						m_caConnectionPointName;
	u8								m_tLocations[GameGraph::LOCATION_TYPE_COUNT];
};
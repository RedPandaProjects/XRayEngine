#pragma once
namespace RestrictionSpace 
{
	enum ERestrictorTypes 
	{
		eDefaultRestrictorTypeNone = u8(0),
		eDefaultRestrictorTypeOut = u8(1),
		eDefaultRestrictorTypeIn = u8(2),
		eRestrictorTypeNone = u8(3),
		eRestrictorTypeIn = u8(4),
		eRestrictorTypeOut = u8(5),
	};
};

class ISE_ALifeSpaceRestrictor
{
public:
	u8				m_space_restrictor_type;
	virtual class	ISE_Shape* shape() = 0;
	virtual class ISE_ALifeObject* CastALifeObject() = 0;
	virtual class ISE_Abstract* CastAbstract() = 0;
}; 
class ISE_ALifeCustomZone
{
public:
	u32								m_enabled_time;
	u32								m_disabled_time;
	u32								m_start_time_shift;
};
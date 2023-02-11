#pragma once
#include "alife_space.h"

class ISE_ALifeItemWeapon
{
public:
	typedef	ALife::EWeaponAddonStatus	EWeaponAddonStatus;

	//текущее состояние аддонов


	EWeaponAddonStatus				m_scope_status;
	EWeaponAddonStatus				m_silencer_status;
	EWeaponAddonStatus				m_grenade_launcher_status;

	enum EWeaponAddonState
	{
		eWeaponAddonScope = 0x01,
		eWeaponAddonGrenadeLauncher = 0x02,
		eWeaponAddonSilencer = 0x04
	};
	u8								ammo_type;
	u16								a_elapsed;
	Flags8							m_addon_flags;
};
class ISE_ALifeItemArtefact
{
public:
	float							m_fAnomalyValue;
};
class ISE_ALifeItemAmmo
{
public:
	u16								a_elapsed;
};

class ISE_ALifeItemDocument
{
public:
	shared_str							m_wDoc;
};
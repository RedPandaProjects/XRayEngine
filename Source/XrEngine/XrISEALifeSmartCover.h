#pragma once


class ISE_SmartCover
{
public:
	shared_str						m_description;
	float							m_hold_position_time;
	float							m_enter_min_enemy_distance;
	float							m_exit_min_enemy_distance;
	BOOL							m_is_combat_cover;
	BOOL							m_can_fire;

	xr_map<shared_str,bool>			EditorLoopholes;

	virtual void 					OnChangeDescription	() = 0;
	virtual void					OnChangeLoopholes	() = 0;
	virtual void					OnFillProperties	() = 0;
};
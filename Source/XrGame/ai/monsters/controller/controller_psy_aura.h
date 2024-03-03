#pragma once
////////////////////////////////////////////////////////////////////////
// Effector controlling class
////////////////////////////////////////////////////////////////////////
#include "../../../pp_effector_custom.h"

class CController;



class CPPEffectorControllerAura : public CPPEffectorCustom {
	typedef CPPEffectorCustom inherited;

	enum {eStateFadeIn, eStateFadeOut, eStatePermanent} m_effector_state;

	u32				m_time_state_started;
	u32				m_time_to_fade;
	
	FRBMKSoundSourceRef		m_snd;

public:
					CPPEffectorControllerAura	(const SPPInfo &ppi, u32 time_to_fade, const FRBMKSoundSourceRef &snd);
	virtual BOOL	update						();
	void			switch_off					();
};

class CControllerAura : public CPPEffectorCustomController<CPPEffectorControllerAura>{
	typedef CPPEffectorCustomController<CPPEffectorControllerAura> inherited;

	CController			*m_object;
	u32					m_time_last_update;

	FRBMKSoundSourceRef	aura_sound;
	float				aura_radius;
	
	u32					m_time_fake_aura;

	u32					m_time_fake_aura_duration;
	u32					m_time_fake_aura_delay;
	float				m_fake_max_add_dist;
	float				m_fake_min_add_dist;

	u32					m_time_started;


public:
					CControllerAura			(CController *monster) : m_object(monster){}
	virtual void	load					(LPCSTR section);

			void	on_death				();
			void	update_schedule			();
			void	update_frame			();
};



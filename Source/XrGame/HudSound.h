#pragma once
#include "ai_sounds.h"


struct HUD_SOUND_ITEM
{
	HUD_SOUND_ITEM():m_activeSnd(NULL),m_b_exclusive(false)		{}

	static void		LoadSound		(	LPCSTR section, LPCSTR line,
										FRBMKSoundSourceRef& hud_snd,
										int type = SOUND_TYPE_FROM_SOURCE,
										float* volume = NULL,
										float* delay = NULL);

	static void		LoadSound		(	LPCSTR section, 
										LPCSTR line,
										HUD_SOUND_ITEM& hud_snd,  
										int type = SOUND_TYPE_FROM_SOURCE);

	static void		DestroySound	(	HUD_SOUND_ITEM& hud_snd);

	static void		PlaySound		(	HUD_SOUND_ITEM& snd,
										const Fvector& position,
										const CObject* parent,
										bool hud_mode,
										bool looped = false,
										u8 index=u8(-1));

	static void		StopSound		(	HUD_SOUND_ITEM& snd);

	ICF BOOL		playing			()
	{
		if (m_activeSnd)	return	m_activeSnd->snd.IsPlaying();
		else				return	FALSE;
	}

	ICF void		set_position	(	const Fvector& pos)
	{
		if(m_activeSnd)
		{ 
			if (m_activeSnd->snd.IsValid()&&!m_activeSnd->snd.IsRelative())	
			{
				m_activeSnd->snd.SetPosition(pos);
			}
			else
			{
				m_activeSnd	= nullptr;
			}
		}
	}

	struct SSnd		{
		FRBMKSoundSourceRef	snd;
		float		delay;		//задержка перед проигрыванием
		float		volume;		//громкость
	};
	shared_str		m_alias;
	SSnd*			m_activeSnd;
	bool			m_b_exclusive;
	xr_vector<SSnd> sounds;

	bool operator == (LPCSTR alias) const{return 0==stricmp(m_alias.c_str(),alias);}
};

class HUD_SOUND_COLLECTION
{
	xr_vector<HUD_SOUND_ITEM>	m_sound_items;
	HUD_SOUND_ITEM*				FindSoundItem	(	LPCSTR alias, bool b_assert);
public:
								~HUD_SOUND_COLLECTION();
	void						PlaySound		(	LPCSTR alias, 
													const Fvector& position,
													const CObject* parent,
													bool hud_mode,
													bool looped = false,
													u8 index=u8(-1));

	void						StopSound		(	LPCSTR alias);

	void						LoadSound		(	LPCSTR section, 
													LPCSTR line,
													LPCSTR alias,
													bool exclusive = false,
													int type = SOUND_TYPE_FROM_SOURCE);

	void						SetPosition		(	LPCSTR alias, 	const Fvector& pos);
	void						StopAllSounds	();
};

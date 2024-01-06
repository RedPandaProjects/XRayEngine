////////////////////////////////////////////////////////////////////////////
//	Module 		: script_sound.h
//	Created 	: 06.02.2004
//  Modified 	: 06.02.2004
//	Author		: Dmitriy Iassenev
//	Description : XRay Script sound class
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "script_export_space.h"
#include "ai_sounds.h"

class CScriptGameObject;

class CScriptSound {
	mutable FRBMKSoundSourceRef	m_sound;
	shared_str					m_caSoundToPlay;

	friend class CScriptSoundAction;
public:

								CScriptSound		(LPCSTR caSoundName, ESoundTypes sound_type = SOUND_TYPE_NO_SOUND);
	virtual						~CScriptSound		();
	IC		u32					Length				();
	IC		void				Play				(CScriptGameObject *object);
	IC		void				Play				(CScriptGameObject *object, float delay);
			void				Play				(CScriptGameObject *object, float delay, int flags);
	IC		void				PlayAtPos			(CScriptGameObject *object, const Fvector &position);
	IC		void				PlayAtPos			(CScriptGameObject *object, const Fvector &position, float delay);
			void				PlayAtPos			(CScriptGameObject *object, const Fvector &position, float delay, int flags);
			void				PlayNoFeedback		(CScriptGameObject *object,	int flags/*!< Looping */, float delay/*!< Delay */, Fvector pos, float vol);
	IC		void				Stop				();
	IC		void				StopDeffered		();
	IC		void				SetPosition			(const Fvector &position);
	IC		void				SetVolume			(float volume);
			void				SetMinDistance		(const float fMinDistance);
	IC		void				SetMaxDistance		(const float fMaxDistance);
			Fvector				GetPosition			() const;
	IC		const float			GetMinDistance		() const;
	IC		const float			GetMaxDistance		() const;
	IC		const float			GetVolume			() const;
	IC		bool				IsPlaying			() const;

	DECLARE_SCRIPT_REGISTER_FUNCTION
};
add_to_type_list(CScriptSound)
#undef script_type_list
#define script_type_list save_type_list(CScriptSound)

#include "script_sound_inline.h"
////////////////////////////////////////////////////////////////////////////
//	Module 		: script_sound.cpp
//	Created 	: 06.02.2004
//  Modified 	: 06.02.2004
//	Author		: Dmitriy Iassenev
//	Description : XRay Script sound class
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "script_sound.h"
#include "script_game_object.h"
#include "gameobject.h"
#include "ai_space.h"
#include "script_engine.h"

CScriptSound::CScriptSound				(LPCSTR caSoundName, ESoundTypes sound_type)
{
	m_caSoundToPlay			= caSoundName;
	if (g_Engine->GetSoundManager()->ExistSoundWave(caSoundName))
		m_sound = g_Engine->GetSoundManager()->CreateSource(caSoundName,sound_type);
	else
		ai().script_engine().script_log	(ScriptStorage::eLuaMessageTypeError,"File not found \"%s\"!",caSoundName);
}

CScriptSound::~CScriptSound		()
{
	THROW3					(!m_sound.IsPlaying(),"playing sound is not completed, but is destroying",m_sound.IsValid() ? (m_sound.GetName().c_str()) : "unknown");
}

Fvector CScriptSound::GetPosition() const
{
	VERIFY				(m_sound.IsValid());
	return m_sound.GetWorldPosition();
}

void CScriptSound::Play			(CScriptGameObject *object, float delay, int flags)
{
	THROW3(m_sound.IsValid(),"There is no sound",*m_caSoundToPlay);
	VERIFY((flags&0x2) == 0x2);

	m_sound.Play(&object->object(),(flags&0x1) == 0x1,delay);
}

void CScriptSound::PlayAtPos		(CScriptGameObject *object, const Fvector &position, float delay, int flags)
{
	THROW3						(m_sound.IsValid(),"There is no sound",*m_caSoundToPlay);
	if((flags&0x2) == 0x2)
	{
		VERIFY(fis_zero( position.square_magnitude()));
		m_sound.Play(&object->object(),(flags&0x1) == 0x1,delay);
	}
	else
	{
		m_sound.Play(&object->object(), position,(flags&0x1) == 0x1,delay);
	}
}

void CScriptSound::PlayNoFeedback	(CScriptGameObject *object,	u32 flags/*!< Looping */, float delay/*!< Delay */, Fvector pos, float vol)
{
	THROW3						(m_sound.IsValid(),"There is no sound",*m_caSoundToPlay);
	if((flags&0x2) == 0x2)
	{
		VERIFY(fis_zero( pos.square_magnitude()));
		m_sound.PlayWithoutFeedback(&object->object(),(flags&0x1) == 0x1,delay,&vol);
	}
	else
	{
		m_sound.PlayWithoutFeedback(&object->object(), pos,(flags&0x1) == 0x1,delay,&vol);
	}
}
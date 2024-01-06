////////////////////////////////////////////////////////////////////////////
//	Module 		: script_sound_inline.h
//	Created 	: 06.02.2004
//  Modified 	: 06.02.2004
//	Author		: Dmitriy Iassenev
//	Description : XRay Script sound class inline functions
////////////////////////////////////////////////////////////////////////////

#pragma once

IC	u32	CScriptSound::Length				()
{
	return static_cast<u32>(m_sound.GetDuration() * 1000.f);
}

IC	void CScriptSound::Play					(CScriptGameObject *object)
{
	Play					(object,0.f,0);
}

IC	void CScriptSound::Play					(CScriptGameObject *object, float delay)
{
	Play					(object,delay,0);
}
		
IC	void CScriptSound::PlayAtPos			(CScriptGameObject *object, const Fvector &position)
{
	PlayAtPos				(object,position,0.f,0);
}

IC	void CScriptSound::PlayAtPos			(CScriptGameObject *object, const Fvector &position, float delay)
{
	PlayAtPos				(object,position,delay,0);
}

IC	void CScriptSound::SetMinDistance		(const float fMinDistance)
{
	VERIFY				(m_sound.IsValid());
	m_sound.SetRange(fMinDistance,GetMaxDistance());
}

IC	void CScriptSound::SetMaxDistance		(const float fMaxDistance)
{
	VERIFY				(m_sound.IsValid());
	m_sound.SetRange(GetMinDistance(),fMaxDistance);
}


IC	const float CScriptSound::GetMinDistance() const
{
	VERIFY				(m_sound.IsValid());
	return				(m_sound.GetMinDistance());
}

IC	const float CScriptSound::GetMaxDistance() const
{
	VERIFY				(m_sound.IsValid());
	return				(m_sound.GetMaxDistance());
}

IC	const float	CScriptSound::GetVolume		() const
{
	VERIFY				(m_sound.IsValid());
	return				(m_sound.GetVolume());
}

IC	bool CScriptSound::IsPlaying			() const
{
	VERIFY				(m_sound.IsValid());
	return				m_sound.IsPlaying();
}

IC	void CScriptSound::Stop					()
{
	VERIFY(m_sound.IsValid());
	m_sound.Stop();
}

IC	void CScriptSound::StopDeffered			()
{
	VERIFY(m_sound.IsValid());
	m_sound.StopWithFade();
}

IC	void CScriptSound::SetPosition			(const Fvector &position)
{
	VERIFY(m_sound.IsValid());
	m_sound.SetPosition(position);
}

IC	void CScriptSound::SetVolume			(float volume)
{
	VERIFY(m_sound.IsValid());
	m_sound.SetVolume	(volume);
}


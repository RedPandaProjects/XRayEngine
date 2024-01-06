#pragma once
#include "RBMKSoundSource.h"
#include "RBMKSoundActorListener.h"
ENGINE_API class IRBMKSoundManager
{
public:
	virtual IRBMKSoundSource*	CreateSource				(const char*Name) = 0;
	virtual IRBMKSoundSource*	CreateSource				(const char*Name,s32 CustomFlags) = 0;
	virtual IRBMKSoundSource*	AddReference				(IRBMKSoundSource*SoundSource) = 0;
	virtual void				Destroy						(IRBMKSoundSource*SoundSource) = 0;
	virtual IRBMKSoundSource*	Duplicate					(IRBMKSoundSource*SoundSource) = 0;
	virtual IRBMKSoundSource*	Duplicate					(IRBMKSoundSource*SoundSource,s32 CustomFlags) = 0;
	virtual void				Destroyed					(IRBMKSoundActorListener*SoundActorListener) = 0;
	virtual void				Destroyed					(CObject*Object) = 0;
	virtual void				SetShockEffectorVolume		(float NewVolume) = 0;
	virtual bool				ExistSoundWave				(const char*Name) const= 0;
};

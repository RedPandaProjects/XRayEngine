#include "stdafx.h"
#include "RBMKSoundSource.h"

#include "../Core/RBMKEngine.h"

FRBMKSoundSourceRef::FRBMKSoundSourceRef():SoundSource(nullptr)
{
}

FRBMKSoundSourceRef::FRBMKSoundSourceRef(FRBMKSoundSourceRef&& Right):SoundSource(Right.SoundSource)
{
	Right.SoundSource = nullptr;
}

FRBMKSoundSourceRef::FRBMKSoundSourceRef(const FRBMKSoundSourceRef& Right):SoundSource(nullptr)
{
	SoundSource = g_Engine->GetSoundManager()->AddReference(Right.SoundSource);
}

FRBMKSoundSourceRef::FRBMKSoundSourceRef(IRBMKSoundSource* InSource):SoundSource(InSource)
{
}

FRBMKSoundSourceRef::~FRBMKSoundSourceRef()
{
	Reset();
}

FRBMKSoundSourceRef& FRBMKSoundSourceRef::operator=(IRBMKSoundSource* InSource)
{
	Reset();
	SoundSource = InSource;
	return *this;
}

FRBMKSoundSourceRef& FRBMKSoundSourceRef::operator=(const FRBMKSoundSourceRef& Right)
{
	Reset();
	SoundSource = g_Engine->GetSoundManager()->AddReference(Right.SoundSource);
	return *this;
}


void FRBMKSoundSourceRef::Create(const char* Name)
{
	Reset();
	SoundSource = g_Engine->GetSoundManager()->CreateSource(Name);
	
}

void FRBMKSoundSourceRef::Create(const char* Name, s32 Flags)
{
	Reset();
	SoundSource = g_Engine->GetSoundManager()->CreateSource(Name,Flags);
}

void FRBMKSoundSourceRef::Reset()
{
	if(SoundSource)
	{
		g_Engine->GetSoundManager()->Destroy(SoundSource);
	}
	SoundSource = nullptr;
}

bool FRBMKSoundSourceRef::IsValid() const
{
	return SoundSource!=nullptr;
}

bool FRBMKSoundSourceRef::IsPlaying() const
{
	if(SoundSource)
	{
		return SoundSource->IsPlaying();
	}
	return false;
}

// void FRBMKSoundSourceRef::Pause(bool Paused)
// {
// 	if(SoundSource)
// 	{
// 		SoundSource->Pause(Paused);
// 	}
// }

void FRBMKSoundSourceRef::SetVolume(float NewVolume)
{
	if(SoundSource)
	{
		SoundSource->SetVolume(NewVolume);
	}
}

void FRBMKSoundSourceRef::SetFrequency(float NewFrequency)
{
	if(SoundSource)
	{
		SoundSource->SetFrequency(NewFrequency);
	}
}

void FRBMKSoundSourceRef::SetPosition(const Fvector& InPosition)
{
	if(SoundSource)
	{
		SoundSource->SetPosition(InPosition);
	}
}

void FRBMKSoundSourceRef::SetRange(float MinDistance, float MaxDistance)
{
	if(SoundSource)
	{
		SoundSource->SetRange(MinDistance,MaxDistance);
	}
}

bool FRBMKSoundSourceRef::IsRelative()
{
	if(SoundSource)
	{
		return SoundSource->IsRelative();
	}
	return false;
}

float FRBMKSoundSourceRef::GetDuration() const
{
	if(SoundSource)
	{
		return SoundSource->GetDuration();
	}
	return 0;
}

Fvector FRBMKSoundSourceRef::GetWorldPosition() const
{
	if(SoundSource)
	{
		return SoundSource->GetWorldPosition();
	}
	return Fvector(0,0,0);
}

float FRBMKSoundSourceRef::GetMinDistance() const
{
	if(SoundSource)
	{
		return SoundSource->GetMinDistance();
	}
	return 0;
}

float FRBMKSoundSourceRef::GetMaxDistance() const
{
	if(SoundSource)
	{
		return SoundSource->GetMaxDistance();
	}
	return 0;
}

float FRBMKSoundSourceRef::GetVolume() const
{
	if(SoundSource)
	{
		return SoundSource->GetVolume();
	}
	return 0;
}

shared_str FRBMKSoundSourceRef::GetName() const
{
	if(SoundSource)
	{
		return SoundSource->GetName();
	}
	return "unkown";
}


void FRBMKSoundSourceRef::Dublicate(const FRBMKSoundSourceRef& Right,s32 NewFlags) 
{
	Reset();
	SoundSource = g_Engine->GetSoundManager()->Duplicate(Right.SoundSource,NewFlags);
}

FRBMKSoundSourceRef::operator bool() const
{
	return SoundSource != nullptr;
}

void FRBMKSoundSourceRef::Play(CObject* ActorSource, bool Looping, float Delay)
{
	if(SoundSource)
	{
		SoundSource->Play(ActorSource,Looping,Delay);
	}
}

void FRBMKSoundSourceRef::Play(CObject* ActorSource, const Fvector& InPosition, bool Looping, float Delay)
{
	if(SoundSource)
	{
		SoundSource->Play(ActorSource,InPosition,Looping,Delay);
	}
}

void FRBMKSoundSourceRef::PlayWithoutFeedback(CObject* ActorSource, bool Looping, float Delay, float* Volume)
{
	if(SoundSource)
	{
		SoundSource->PlayWithoutFeedback(ActorSource,Looping,Delay);
	}
}

void FRBMKSoundSourceRef::PlayWithoutFeedback(CObject* ActorSource, const Fvector& InPosition, bool Looping,float Delay, float* Volume)
{
	if(SoundSource)
	{
		SoundSource->PlayWithoutFeedback(ActorSource,InPosition,Looping,Delay);
	}
}

void FRBMKSoundSourceRef::Stop()
{
	if(SoundSource)
	{
		SoundSource->Stop();
	}
}

void FRBMKSoundSourceRef::StopWithFade()
{
	if(SoundSource)
	{
		SoundSource->StopWithFade();
	}
}


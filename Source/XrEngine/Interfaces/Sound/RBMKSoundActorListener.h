#pragma once

class ENGINE_API IRBMKSoundActorListener
{
public:
	virtual			~IRBMKSoundActorListener	();
	virtual void	ListenSound					(CObject* InSourceActor, s32 SoundFlags,const Fvector& InPosition, float InPower,CSound_UserDataPtr UserData) = 0;
};

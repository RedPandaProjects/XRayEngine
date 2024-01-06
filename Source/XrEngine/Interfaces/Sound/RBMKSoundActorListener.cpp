#include "stdafx.h"
#include "RBMKSoundActorListener.h"
#include "..\Core\RBMKEngine.h"

IRBMKSoundActorListener::~IRBMKSoundActorListener()
{
	g_Engine->GetSoundManager()->Destroyed(this);
}

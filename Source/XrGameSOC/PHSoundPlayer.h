#pragma once
#include "../XrEngine/GameMtlLib.h"

class CPhysicsShellHolder;

class CPHSoundPlayer
{
		FRBMKSoundSourceRef					m_sound																		;
		CPhysicsShellHolder			*m_object;
public:
		void						Play					(SGameMtlPair* mtl_pair,const Fvector& pos)			;
									CPHSoundPlayer			(CPhysicsShellHolder *m_object)									;
virtual								~CPHSoundPlayer			()													;


private:
};
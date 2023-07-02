//---------------------------------------------------------------------------
#include "stdafx.h"
#pragma hdrstop

#include "particle_actions.h"

void PAPI::ParticleActions::clear()
{
	R_ASSERT(!m_bLocked);
	for (PAVecIt it = actions.begin(); it != actions.end(); it++)
		xr_delete(*it);
	actions.clear();
}
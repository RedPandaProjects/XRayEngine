//----------------------------------------------------
// file: PSObject.cpp
//----------------------------------------------------
#include "stdafx.h"
#pragma hdrstop

#include "ParticlesObject.h"
#include "../xrEngine/defines.h"
#include "../XrEngine/Render/RenderVisual.h"
#include "../XrEngine/Render/ParticleCustom.h"
#include "../xrEngine/render.h"
#include "../xrEngine/IGame_Persistent.h"
#include "../xrEngine/environment.h"

const Fvector zero_vel		= {0.f,0.f,0.f};

CParticlesObject::CParticlesObject	(LPCSTR p_name, BOOL bAutoRemove, bool destroy_on_game_load) :
	inherited				(destroy_on_game_load)
{
	Init					(p_name,0,bAutoRemove);
}

void CParticlesObject::Init	(LPCSTR p_name, IRender_Sector* S, BOOL bAutoRemove)
{
	m_bLooped				= false;
	m_bStopping				= false;
	m_bAutoRemove			= bAutoRemove;

	if(!g_dedicated_server)
	{
		// create visual
		renderable.visual		= Render->model_CreateParticles(p_name);
		renderable.visual	->Lock(this);
#ifndef MASTER_GOLD
		if (renderable.visual)
		{
#endif
			VERIFY(renderable.visual);
			IParticleCustom* V =renderable.visual->dcast_ParticleCustom();  VERIFY(V);

#ifndef MASTER_GOLD
		}
		else
		{
#endif
		}
	}else
	{
	}

	if (renderable.visual &&renderable.visual->dcast_ParticleCustom()&&renderable.visual->dcast_ParticleCustom()->IsLooping())
	{
		if (bAutoRemove)
		{
			R_ASSERT3(!m_bAutoRemove, "Can't set auto-remove flag for looped particle system.", p_name);
		}
		else
		{
			m_bLooped = true;
		}
	}



	// spatial
	spatial.type			= 0;
	spatial.sector			= S;
	
	// sheduled
	shedule.t_min			= 20;
	shedule.t_max			= 50;
	shedule_register		();

}

//----------------------------------------------------
CParticlesObject::~CParticlesObject()
{
	if(renderable.visual)
	{
		renderable.visual->Unlock(this);
	}
//	we do not need this since CPS_Instance does it
//	shedule_unregister		();
}

void CParticlesObject::UpdateSpatial()
{
	if(g_dedicated_server)		return;
	#ifndef MASTER_GOLD
	if (!renderable.visual)return;
#endif
	// spatial	(+ workaround occasional bug inside particle-system)
	vis_data &vis = renderable.visual->getVisData();
	if (_valid(vis.sphere))
	{
		Fvector	P;	float	R;
		renderable.xform.transform_tiny	(P,vis.sphere.P);
		R								= vis.sphere.R;
		if (0==spatial.type)	{
			// First 'valid' update - register
			spatial.type			= STYPE_RENDERABLE;
			spatial.sphere.set		(P,R);
			spatial_register		();
		} else {
			BOOL	bMove			= FALSE;
			if		(!P.similar(spatial.sphere.P,EPS_L*10.f))		bMove	= TRUE;
			if		(!fsimilar(R,spatial.sphere.R,0.15f))			bMove	= TRUE;
			if		(bMove)			{
				spatial.sphere.set	(P, R);
				spatial_move		();
			}
		}
	}
}

const shared_str CParticlesObject::Name()
{
	if(g_dedicated_server)	return "";
#ifndef MASTER_GOLD
	if (!renderable.visual)return "";
#endif
	IParticleCustom* V	= renderable.visual->dcast_ParticleCustom(); VERIFY(V);
	return (V) ? V->Name() : "";
}

//----------------------------------------------------
void CParticlesObject::Play		(bool bHudMode)
{
	if(g_dedicated_server)		return;
#ifndef MASTER_GOLD
	if (!renderable.visual)return ;
#endif
	IParticleCustom* V			= renderable.visual->dcast_ParticleCustom(); VERIFY(V);
	if(bHudMode)
		V->SetHudMode			(bHudMode);

	V->Play						();
	UpdateSpatial				();
	m_bStopping					= false;
}

void CParticlesObject::play_at_pos(const Fvector& pos, BOOL xform)
{
	if(g_dedicated_server)		return;
#ifndef MASTER_GOLD
	if (!renderable.visual)return ;
#endif
	IParticleCustom* V			=renderable.visual->dcast_ParticleCustom(); VERIFY(V);
	Fmatrix m; m.translate		(pos); 
	V->UpdateParent				(m,zero_vel,xform);
	V->Play();
	UpdateSpatial();
	m_bStopping					= false;
}

void CParticlesObject::Stop		(BOOL bDefferedStop)
{
	if(g_dedicated_server)		return;
#ifndef MASTER_GOLD
	if (!renderable.visual)return ;
#endif
	IParticleCustom* V			= renderable.visual->dcast_ParticleCustom(); VERIFY(V);
	V->Stop						(bDefferedStop);
	m_bStopping					= true;
}

void CParticlesObject::shedule_Update	(u32 _dt)
{
	inherited::shedule_Update		(_dt);

	if(g_dedicated_server)		return;
#ifndef MASTER_GOLD
	if (!renderable.visual)return ;
#endif
	// Update
	if (m_bDead)					return;
	UpdateSpatial					();
}

void CParticlesObject::SetXFORM			(const Fmatrix& m)
{
	if(g_dedicated_server)		return;
#ifndef MASTER_GOLD
	if (!renderable.visual)return ;
#endif
	IParticleCustom* V	= renderable.visual->dcast_ParticleCustom(); VERIFY(V);
	V->UpdateParent		(m,zero_vel,TRUE);
	renderable.xform.set(m);
	UpdateSpatial		();
}

void CParticlesObject::UpdateParent		(const Fmatrix& m, const Fvector& vel)
{
	if(g_dedicated_server)		return;
#ifndef MASTER_GOLD
	if (!renderable.visual)return ;
#endif
	IParticleCustom* V	=renderable.visual->dcast_ParticleCustom(); VERIFY(V);
	V->UpdateParent		(m,vel,FALSE);
	UpdateSpatial		();
}

Fvector& CParticlesObject::Position		()
{
	if(g_dedicated_server) 
	{
		static Fvector _pos = Fvector().set(0,0,0);
		return _pos;
	}
#ifndef MASTER_GOLD
	if (!renderable.visual)
	{
		static Fvector _pos = Fvector().set(0,0,0);
		return _pos;
	}
#endif
	vis_data &vis = renderable.visual->getVisData();
	return vis.sphere.P;
}

float CParticlesObject::shedule_Scale		()	
{ 
	if(g_dedicated_server)		return 5.0f;
#ifndef MASTER_GOLD
	if (!renderable.visual)    return 5.0f;
#endif
	return Device->vCameraPosition.distance_to(Position())/200.f; 
}

void CParticlesObject::renderable_Render	()
{
#ifndef MASTER_GOLD
	if (!renderable.visual)return;
#endif
	VERIFY					(renderable.visual);

}

bool CParticlesObject::IsAutoRemove			()
{
	if(m_bAutoRemove) return true;
	else return false;
}
void CParticlesObject::SetAutoRemove		(bool auto_remove)
{
	VERIFY(m_bStopping || !IsLooped());
	m_bAutoRemove = auto_remove;
}

//�������� �� ��������, ���������� �� PSI_Alive, ��� ��� �����
//��������� Stop �������� ����� ��� ���������� �������� IsPlaying = true
bool CParticlesObject::IsPlaying()
{
	if(g_dedicated_server)		return false;
#ifndef MASTER_GOLD
	if (!renderable.visual)    return false;
#endif
	IParticleCustom* V	= renderable.visual->dcast_ParticleCustom(); 
	VERIFY(V);
	return !!V->IsPlaying();
}

BOOL CParticlesObject::PSI_alive()
{
	if (!renderable.visual)return false;
	IParticleCustom* V = renderable.visual->dcast_ParticleCustom(); VERIFY(V);
	return V ? V->Alive() : false;
}
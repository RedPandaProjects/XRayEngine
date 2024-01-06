////////////////////////////////////////////////////////////////////////////
//	Module 		: material_manager.cpp
//	Created 	: 27.12.2003
//  Modified 	: 27.12.2003
//	Author		: Dmitriy Iassenev
//	Description : Material manager
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "material_manager.h"
#include "..\xrEngine\alife_space.h"
#include "phmovementcontrol.h"
#include "entity_alive.h"
#include "CharacterPhysicsSupport.h"
#include "../XrEngine/Render/KinematicsAnimated.h"

CMaterialManager::CMaterialManager	(CObject *object, CPHMovementControl *movement_control)
{
	VERIFY					(object);
	m_object				= object;

	VERIFY					(movement_control);
	m_movement_control		= movement_control;

	m_my_material_idx		= GAMEMTL_NONE_IDX;
	m_run_mode				= false;
}

CMaterialManager::~CMaterialManager	()
{
}

void CMaterialManager::Load			(LPCSTR section)
{
	R_ASSERT3				(pSettings->line_exist(section,"material"),"Material not found in the section ",*(m_object->cNameSect()));
	m_my_material_idx		= GameMaterialLibrary->GetMaterialIdx(pSettings->r_string(section,"material"));
}

void CMaterialManager::reinit		()
{
	m_last_material_idx		= GameMaterialLibrary->GetMaterialIdx("default");
	m_step_id				= 0;
	m_run_mode				= false;

	CEntityAlive			*entity_alive = smart_cast<CEntityAlive*>(m_object);
	if (entity_alive) {
		if(entity_alive->character_physics_support()->movement()->CharacterExist())
			entity_alive->character_physics_support()->movement()->SetPLastMaterialIDX	(&m_last_material_idx);

//		if (entity_alive->use_simplified_visual()) {
//			IKinematics			*kinematics = CastToIKinematics(entity_alive->Visual());
//			m_my_material_idx	= kinematics->GetBoneData(kinematics->LL_GetBoneRoot()).game_mtl_idx;
//		}

		entity_alive->character_physics_support()->movement()->SetMaterial		(m_my_material_idx);
	}
}

void CMaterialManager::reload		(LPCSTR section)
{
}

void CMaterialManager::update		(float time_delta, float volume, float step_time, bool standing)
{
	VERIFY					(GAMEMTL_NONE_IDX != m_my_material_idx);
	VERIFY					(GAMEMTL_NONE_IDX != m_last_material_idx);
	SGameMtlPair			*mtl_pair = GameMaterialLibrary->GetMaterialPair(m_my_material_idx,m_last_material_idx);
	VERIFY3					(mtl_pair,"Undefined material pair: ", *GameMaterialLibrary->GetMaterialByIdx(m_last_material_idx)->m_Name);
	Fvector					position = m_object->Position();
	if(m_movement_control->CharacterExist())
	{
		position.y				+= m_movement_control->FootRadius(); 
	}
	
	// FRBMKSoundSourceContainer  step
	if (!standing) {
		if (m_time_to_step < 0) {
			xr_vector<shared_str>* snd_array = &mtl_pair->StepSoundsNames;
			
			if(m_run_mode && mtl_pair->BreakingSoundsNames.size() >0)
				snd_array = &mtl_pair->BreakingSoundsNames;

			if (snd_array->size() >0){
				m_step_id								= ::Random.randI(0, snd_array->size());
				m_time_to_step							= step_time;

				m_step_sound[m_step_id].Create((*snd_array)[m_step_id].c_str());
				m_step_sound[m_step_id].Play	(m_object,position);
			}
		}
		m_time_to_step								-= time_delta;
	}
	else
		m_time_to_step								= 0;


	for(int i=0; i<4; i++)
		if (m_step_sound[i].IsPlaying())		{
			m_step_sound[i].SetPosition	(position    );
			m_step_sound[i].SetVolume		( volume);
		}
}

void CMaterialManager::set_run_mode			(bool run_mode)
{
	m_run_mode			= run_mode;
}

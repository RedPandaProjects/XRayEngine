#include "stdafx.h"
#include "tushkano.h"
#include "tushkano_state_manager.h"

#include "../control_animation_base.h"
#include "../control_direction_base.h"
#include "../control_movement_base.h"
#include "../control_path_builder_base.h"

#include "../states/monster_state_rest.h"
#include "../states/monster_state_eat.h"
#include "../states/monster_state_attack.h"
#include "../states/monster_state_panic.h"
#include "../states/monster_state_hear_danger_sound.h"
#include "../states/monster_state_hitted.h"
#include "../states/monster_state_controlled.h"
#include "../states/monster_state_help_sound.h"

#include "../../../entitycondition.h"


CStateManagerTushkano::CStateManagerTushkano(CTushkano *obj) : inherited(obj)
{
	this->add_state(eStateRest,				xr_new<CStateMonsterRest<CTushkano> >				(obj));
	this->add_state(eStateAttack,				xr_new<CStateMonsterAttack<CTushkano> >				(obj));
	this->add_state(eStateEat,				xr_new<CStateMonsterEat<CTushkano> >				(obj));
	this->add_state(eStateHearDangerousSound,	xr_new<CStateMonsterHearDangerousSound<CTushkano> >	(obj));
	this->add_state(eStatePanic,				xr_new<CStateMonsterPanic<CTushkano> >				(obj));
	this->add_state(eStateHitted,				xr_new<CStateMonsterHitted<CTushkano> >				(obj));
	this->add_state(eStateControlled,			xr_new<CStateMonsterControlled<CTushkano> >			(obj));
	this->add_state(eStateHearHelpSound,		xr_new<CStateMonsterHearHelpSound<CTushkano> >		(obj));
}

CStateManagerTushkano::~CStateManagerTushkano()
{
}

void CStateManagerTushkano::execute()
{
	u32 state_id = u32(-1);

	if (!this->object->is_under_control())
	{
		const CEntityAlive* enemy	= this->object->EnemyMan.get_enemy();

		if (enemy) {
			switch (this->object->EnemyMan.get_danger_type()) {
				case eStrong:	state_id = eStatePanic; break;
				case eWeak:		state_id = eStateAttack; break;
			}
		} else if (this->object->HitMemory.is_hit()) {
			state_id = eStateHitted;
		} else if (check_state(eStateHearHelpSound)) {
			state_id = eStateHearHelpSound;
		} else if (this->object->hear_interesting_sound || this->object->hear_dangerous_sound) {
			state_id = eStateHearDangerousSound;
		} else {
			if (can_eat())	state_id = eStateEat;
			else 			state_id = eStateRest;
		}
	} else state_id = eStateControlled;

	// установить текущее состояние
	this->select_state(state_id);

	// выполнить текущее состояние
	this->get_state_current()->execute();

	this->prev_substate = this->current_substate;
}


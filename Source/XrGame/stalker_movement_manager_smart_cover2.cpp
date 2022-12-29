#include "stdafx.h"
#include "pch_script.h"
#define SMART_CAST

#include "script_game_object.h"
#include "script_game_object_impl.h"
#include "InventoryOwner.h"
#include "Pda.h"
#include "xrMessages.h"
#include "character_info.h"
#include "gametask.h"
#include "actor.h"
#include "level.h"
#include "date_time.h"
#include "uigamesp.h"
#include "restricted_object.h"
#include "script_engine.h"
#include "attachable_item.h"
#include "script_entity.h"
#include "string_table.h"
#include "alife_registry_wrappers.h"
#include "relation_registry.h"
#include "custommonster.h"
#include "actorcondition.h"
#include "level_graph.h"
#include "huditem.h"
#include "ui/UItalkWnd.h"
#include "inventory.h"
#include "infoportion.h"
#include "AI/Monsters/BaseMonster/base_monster.h"
#include "weaponmagazined.h"
#include "ai/stalker/ai_stalker.h"
#include "agent_manager.h"
#include "agent_member_manager.h"
#include "stalker_animation_manager.h"
#include "CameraFirstEye.h"
#include "stalker_movement_manager_smart_cover.h"
#include "script_callback_ex.h"
#include "memory_manager.h"
#include "enemy_manager.h"
#include "ai/stalker/ai_stalker_impl.h"
#include "smart_cover_object.h"
#include "smart_cover.h"
#include "customdetector.h"
#include "doors_manager.h"
#include "doors_door.h"
#include "Torch.h"
#include "physicobject.h"

namespace smart_cover {
	shared_str	transform_vertex(shared_str const& vertex_id, bool const& in);
} // namespace smart_cover


void stalker_movement_manager_smart_cover::loophole_path(smart_cover::cover const& cover, shared_str const& source_raw, shared_str const& target_raw, LoopholePath& path) const
{
	shared_str				source = smart_cover::transform_vertex(source_raw, true);
	shared_str				target = smart_cover::transform_vertex(target_raw, false);

	typedef GraphEngineSpace::CBaseParameters	CBaseParameters;
	CBaseParameters			parameters(u32(-1), u32(-1), u32(-1));
	path.clear();
	R_ASSERT2(
		ai().graph_engine().search(
			cover.description()->transitions(),
			source,
			target,
			&path,
			parameters
		),
		make_string(
			"cannot build path via loopholes [%s] -> [%s] (cover %s)",
			source_raw.c_str(),
			target_raw.c_str(),
			cover.description()->table_id().c_str()
		)
	);
}
#if 1

bool stalker_movement_manager_smart_cover::exit_transition()
{
	VERIFY(m_current.cover());

	try_actualize_path();

	VERIFY(!m_path.empty());
	VERIFY(m_path.size() > 1);

	return					(m_path[1]._get() == smart_cover::transform_vertex("", false)._get());
}

void stalker_movement_manager_smart_cover::bind_global_selector()
{
	CStalkerAnimationManager& animation = object().animation();

	animation.global_selector(CStalkerAnimationManager::AnimationSelector(&animation_selector(), &smart_cover::animation_selector::select_animation));
	animation.global_callback(CStalkerAnimationManager::AnimationCallback(&animation_selector(), &smart_cover::animation_selector::on_animation_end));
#ifdef DEBUG
	animation.global_modifier(CStalkerAnimationManager::AnimationModifier(&animation_selector(), &smart_cover::animation_selector::modify_animation));
#endif // #ifdef DEBUG

	if (!m_current.cover())
		return;

	Fvector								position = m_current.cover()->fov_position(*m_current.cover_loophole());
	Fvector								direction = m_current.cover()->enter_direction(*m_current.cover_loophole());
	object().animation().global().target_matrix(position, direction);
}

void  stalker_movement_manager_smart_cover::unbind_global_selector()
{
	CStalkerAnimationManager& animation = object().animation();

	animation.global_selector(CStalkerAnimationManager::AnimationSelector());
	animation.global_callback(CStalkerAnimationManager::AnimationCallback());
#ifdef DEBUG
	animation.global_modifier(CStalkerAnimationManager::AnimationModifier());
#endif // #ifdef DEBUG

	object().animation().global().target_matrix();
}

stalker_movement_manager_smart_cover::transition_action const& stalker_movement_manager_smart_cover::current_transition()
{
#ifdef DEBUG
	Msg(
		"m_current_transition guard: [%s][%s] -> [%s][%s], [%d]",
		m_current.cover() ? m_current.cover()->id().c_str() : "<world>",
		m_current.cover() ? m_current.cover_loophole()->id().c_str() : "<no loophole>",
		m_target.cover() ? m_target.cover()->id().c_str() : "<world>",
		m_target.cover() ? m_target.cover_loophole()->id().c_str() : "<no loophole>",
		m_path.size()
	);
#endif // #ifdef DEBUG

	VERIFY(
		(m_current.cover() != m_target.cover()) ||
		!m_current.cover() ||
		(m_current.cover_loophole() != m_target.cover_loophole())
	);

	try_actualize_path();

	VERIFY2(
		m_current_transition,
		make_string(
			"[%s][%s] -> [%s][%s], [%d]",
			m_current.cover() ? m_current.cover()->id().c_str() : "<world>",
			m_current.cover() ? m_current.cover_loophole()->id().c_str() : "<no loophole>",
			m_target.cover() ? m_target.cover()->id().c_str() : "<world>",
			m_target.cover() ? m_target.cover_loophole()->id().c_str() : "<no loophole>",
			m_path.size()
		)
	);
	return					(*m_current_transition);
}

#include "level_path_manager.h"
#include "smart_cover_planner_target_selector.h"
void stalker_movement_manager_smart_cover::cleanup_after_animation_selector()
{
	level_path().make_inactual();
	detail().make_inactual();
}

void stalker_movement_manager_smart_cover::target_selector(CScriptCallbackEx<void> const& callback)
{
	VERIFY(m_target_selector);
	m_target_selector->callback(callback);
}

void stalker_movement_manager_smart_cover::target_idle()
{
	//	if (!m_current.cover()) {
	//		Msg								("! Cannot set target idle. Bad or absent smart_cover.");
	//		return;
	//	}

	//	if (!m_current.cover_loophole()->is_action_available("idle")) {
	//		Msg								("! Cannot set target idle. Loophole has no such action.");
	//		return;
	//	}

	m_target_selector->object().target(StalkerDecisionSpace::eWorldPropertyLoopholeIdle);
}

void stalker_movement_manager_smart_cover::target_lookout()
{
	//	if (!m_current.cover()) {
	//		Msg								("! Cannot set target lookout. Bad or absent smart_cover.");
	//		return;
	//	}

	//	if (!m_current.cover_loophole()->is_action_available("lookout")) {
	//		Msg								("! Cannot set target lookout. Loophole has no such action.");
	//		return;
	//	}

	m_target_selector->object().target(StalkerDecisionSpace::eWorldPropertyLookedOut);
}

void stalker_movement_manager_smart_cover::target_fire()
{
	//	if (!m_current.cover()) {
	//		Msg								("! Cannot set target fire. Bad or absent smart_cover.");
	//		return;
	//	}

	//	if (!m_current.cover_loophole()->is_action_available("fire")) {
	//		Msg								("! Cannot set target fire. Loophole has no such action.");
	//		return;
	//	}

	//	if (!enemy_in_fov()) {
	//		Msg								("! Cannot set target fire. Enemy is not in current loophole's fov.");
	//		return;
	//	}

	m_target_selector->object().target(StalkerDecisionSpace::eWorldPropertyLoopholeFire);
}

void stalker_movement_manager_smart_cover::target_fire_no_lookout()
{
	//	if (!current_params().cover()) {
	//		Msg								("! Cannot set target fire_no_lookout. Bad or absent smart_cover.");
	//		return;
	//	}

	//	if (!current_params().cover_loophole()->is_action_available("fire_no_lookout")) {
	//		Msg								("! Cannot set target fire_no_lookout. Loophole has no such action.");
	//		return;
	//	}

	m_target_selector->object().target(StalkerDecisionSpace::eWorldPropertyLoopholeFireNoLookout);
}

void stalker_movement_manager_smart_cover::target_default(bool const& value)
{
	//	if (!current_params().cover()) {
	//		Msg								("! Cannot set target fire_no_lookout. Bad or absent smart_cover.");
	//		return;
	//	}

	//	if (!current_params().cover_loophole()->is_action_available("fire_no_lookout")) {
	//		Msg								("! Cannot set target fire_no_lookout. Loophole has no such action.");
	//		return;
	//	}

	m_default_behaviour = value;
}

bool stalker_movement_manager_smart_cover::default_behaviour() const
{
	VERIFY(m_current.cover());
	VERIFY(m_current.cover_loophole());

	VERIFY(m_target_selector);
	if (m_target_selector->callback())
		return						(m_default_behaviour);

	if (m_current.cover_fire_object())
		return						(true);

	if (m_current.cover_fire_position())
		return						(true);

	return							(false);
}

bool stalker_movement_manager_smart_cover::in_smart_cover() const
{
	if (current_params().cover())
		return						(true);

	if (entering_smart_cover_with_animation())
		return						(true);

	return							(false);
}

void stalker_movement_manager_smart_cover::remove_links(CObject* object)
{
	inherited::remove_links(object);

	if (m_target.cover_fire_object() == object)
		m_target.cover_fire_object(0);

	if (m_current.cover_fire_object() == object)
		m_current.cover_fire_object(0);
}

#endif
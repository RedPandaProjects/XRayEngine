#include "stdafx.h"
#include "ai_monster_utils.h"
#include "../../entity.h"
#include "../../ai_object_location.h"
#include "ai_space.h"

#include "../../../XrEngine/Render/Kinematics.h"

// ���������, ��������� �� ������ entity �� ����
// ���������� ������� �������, ���� �� ��������� �� ����, ��� ����� ��� ����
Fvector get_valid_position(const CEntity *entity, const Fvector &actual_position) 
{
	if (
		ai().level_graph().valid_vertex_id(entity->ai_location().level_vertex_id()) &&
		ai().level_graph().valid_vertex_position(entity->Position()) && 
		ai().level_graph().inside(entity->ai_location().level_vertex_id(), entity->Position())
		)
		return			(actual_position);
	else
		return			(ai().level_graph().vertex_position(entity->ai_location().level_vertex()));
}

// ���������� true, ���� ������ entity ��������� �� ����
bool object_position_valid(const CEntity *entity)
{
	return				(
		ai().level_graph().valid_vertex_id(entity->ai_location().level_vertex_id()) &&
		ai().level_graph().valid_vertex_position(entity->Position()) && 
		ai().level_graph().inside(entity->ai_location().level_vertex_id(), entity->Position())
		);
}

Fvector get_bone_position	(CObject *object, LPCSTR bone_name)
{
	u16 bone_id			= CastToIKinematics(object->Visual())->LL_BoneID				(bone_name);
	IBoneInstance &bone = CastToIKinematics(object->Visual())->LL_GetBoneInstance	(bone_id);

	Fmatrix	global_transform;
	global_transform.mul	(object->XFORM(),bone.GetTransform());

	return	(global_transform.c);
}

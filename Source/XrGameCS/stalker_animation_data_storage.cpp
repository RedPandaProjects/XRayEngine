////////////////////////////////////////////////////////////////////////////
//	Module 		: stalker_animation_data_storage.cpp
//	Created 	: 13.10.2005
//  Modified 	: 13.10.2005
//	Author		: Dmitriy Iassenev
//	Description : Stalker animation data storage
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stalker_animation_data_storage.h"
#include "stalker_animation_data.h"
#include "../xrEngine/object_broker.h"
#include "../xrRender/Public/Kinematics.h"

CStalkerAnimationDataStorage	*g_stalker_animation_data_storage = 0;

CStalkerAnimationDataStorage::~CStalkerAnimationDataStorage()
{
	clear();
}

void CStalkerAnimationDataStorage::clear()
{
	while (!m_objects.empty()) {
		xr_delete(m_objects.back().second);
		m_objects.pop_back();
	}
}

const CStalkerAnimationData* CStalkerAnimationDataStorage::object(IKinematicsAnimated* skeleton_animated)
{
	OBJECTS::const_iterator	I = std::find_if(m_objects.begin(), m_objects.end(), [skeleton_animated](const OBJECT& obj) {return obj.first == skeleton_animated->GetNameData(); });
	if (I != m_objects.end())
		return				((*I).second);

	m_objects.push_back(std::make_pair(skeleton_animated->GetNameData(), xr_new<CStalkerAnimationData>(skeleton_animated)));
	return					(m_objects.back().second);
}

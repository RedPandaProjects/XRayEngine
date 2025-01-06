///////////////////////////////////
// class CUIOptionsManager
///////////////////////////////////

#include "StdAfx.h"
#include "UIOptionsManager.h"
#include "UIOptionsItem.h"
#include "../XrEngine/Xr_ioconsole.h"

void CUIOptionsManager::RegisterItem(CUIOptionsItem* item, const char* group){
	groups_it it = m_groups.find(group);

	if (m_groups.end() != it)
	{
		(*it).second.push_back(item);
	}
	else
	{
		group_name gr_name = group;
		items_list list;

		list.push_back(item);
		m_groups.insert(mk_pair(gr_name, list));
	}
}

void CUIOptionsManager::UnRegisterGroup(const char* group){
	groups_it it = m_groups.find(group);

	if (it != m_groups.end())
		m_groups.erase(it);
}
void CUIOptionsManager::UnRegisterItem(CUIOptionsItem* item){
	groups_it it;
	for (it = m_groups.begin(); it!= m_groups.end(); it++){
		for (u32 i = 0; i < (*it).second.size(); i++)
			if ((*it).second[i] == item){
				(*it).second.erase((*it).second.begin() + i); return;}
	}
}

void CUIOptionsManager::SendMessage2Group(const char* group, const char* message){
	groups_it it = m_groups.find(group);

	R_ASSERT2(m_groups.end() != it, "invalid group name");

	for (u32 i = 0; i < (*it).second.size(); i++)
		(*it).second[i]->OnMessage(message);
}

void CUIOptionsManager::SeveBackupValues(const char* group)
{
	groups_it it = m_groups.find(group);

	R_ASSERT3(m_groups.end() != it, "invalid group name",group);

	for (u32 i = 0; i < (*it).second.size(); i++){
		(*it).second[i]->SeveBackUpValue();
	}
}

void CUIOptionsManager::SetCurrentValues(const char* group){
	groups_it it = m_groups.find(group);

	R_ASSERT3(m_groups.end() != it, "invalid group name",group);

	for (u32 i = 0; i < (*it).second.size(); i++){
		(*it).second[i]->SetCurrentValue();
//.		(*it).second[i]->SeveBackUpValue();
	}
}

void CUIOptionsManager::SaveValues(const char* group, bool need_post_save){
	groups_it it = m_groups.find(group);

	R_ASSERT3(m_groups.end() != it, "invalid group name",group);

	for (u32 i = 0; i < (*it).second.size(); i++)
	{
		if (need_post_save)
			(*it).second[i]->SaveUeValue();
		else
		{
			if ((*it).second[i]->IsChanged())
				(*it).second[i]->SaveValue();
		}
	}
}

bool CUIOptionsManager::IsGroupChanged(const char* group){
	groups_it it = m_groups.find(group);	
	R_ASSERT2(m_groups.end() != it, "invalid group name");

	for (u32 i = 0; i < (*it).second.size(); i++)
	{
		if ((*it).second[i]->IsChanged())
			return true;
	}

	return false;
}

void CUIOptionsManager::UndoGroup(const char* group){
	groups_it it = m_groups.find(group);	
	R_ASSERT2(m_groups.end() != it, "invalid group name");

	for (u32 i = 0; i < (*it).second.size(); i++){
		if ((*it).second[i]->IsChanged())
            (*it).second[i]->Undo();
	}
}






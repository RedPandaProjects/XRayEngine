///////////////////////////////////
// class CUIOptionsManager
///////////////////////////////////

#pragma once

class CUIOptionsItem;

class CUIOptionsManager{
	friend class CUIOptionsItem;
public:	
	CUIOptionsManager() {};

	void SeveBackupValues(const char* group);
	void SetCurrentValues(const char* group);
	void SaveValues(const char* group, bool need_post_save);
	bool IsGroupChanged(const char* group);
	void UndoGroup(const char* group);

	void SendMessage2Group(const char* group, const char* message);

protected:	
	void RegisterItem(CUIOptionsItem* item, const char* group);
	void UnRegisterGroup(const char* group);
	void UnRegisterItem(CUIOptionsItem* item);


	typedef	xr_string									group_name;
	typedef xr_vector<CUIOptionsItem*>					items_list;
    typedef xr_map<group_name, items_list>				groups;
	typedef xr_map<group_name, items_list>::iterator	groups_it;

	groups	m_groups;
};
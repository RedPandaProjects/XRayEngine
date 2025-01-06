#include "pch_script.h"
#include "UIOptionsItem.h"
#include "UIOptionsManagerScript.h"

using namespace luabind;

void CUIOptionsManagerScript::SetCurrentValues(const char* group){
	CUIOptionsItem::GetOptionsManager()->SetCurrentValues(group);
}

void CUIOptionsManagerScript::SaveBackupValues(const char* group){
	CUIOptionsItem::GetOptionsManager()->SeveBackupValues(group);
}

void CUIOptionsManagerScript::SaveValues(const char* group, bool need_post_save){
	CUIOptionsItem::GetOptionsManager()->SaveValues(group, need_post_save);
}

LPCSTR CUIOptionsManagerScript::GetCurrentResolution()
{
	u32 w, h;
	g_Engine->GetCurrentResolution(w, h);
	string128 str;
	xr_sprintf(str, "%dx%d", w, h);

	return str;
}

LPCSTR CUIOptionsManagerScript::SetResolution(bool next)
{
	std::vector<LPCSTR> ResVec;
	//g_Engine->GetResolutions(ResVec);
	LPCSTR curr = GetCurrentResolution();
	LPCSTR next_resolution;


	for (int i = 1; i < ResVec.size() + 1; i++)
	{
		if (next)
		{
			if (strcmp(curr, ResVec.at(i)) == 0)
			{
				if (i < ResVec.size())
					next_resolution = ResVec.at(i + 1);
				else
					next_resolution = ResVec.at(1);

				break;
			}
		}
		else
		{
			if (strcmp(curr, ResVec.at(i)) == 0)
			{
				if (i == 1)
				{
					next_resolution = ResVec.back();
				}
				else
				{
					next_resolution = ResVec.at(i - 1);
				}
				break;
			}
		}
	}

	u32 w, h;

	if (sscanf(next_resolution, "%dx%d", &w, &h) == 2)
	{
		g_Engine->SetResolution(w, h);
	}

	return next_resolution;
}

bool CUIOptionsManagerScript::IsGroupChanged(const char* group){
	return CUIOptionsItem::GetOptionsManager()->IsGroupChanged(group);
}

void CUIOptionsManagerScript::UndoGroup(const char* group) {
	CUIOptionsItem::GetOptionsManager()->UndoGroup(group);
}

void CUIOptionsManagerScript::SendMessage2Group(const char* group, const char* message){
	CUIOptionsItem::GetOptionsManager()->SendMessage2Group(group, message);
}


void CUIOptionsManagerScript::script_register(lua_State *L)
{
	module(L)
		[
			class_<CUIOptionsManagerScript>("COptionsManager")
			.def(						constructor<>())
			.def("SaveBackupValues",	&CUIOptionsManagerScript::SaveBackupValues )
			.def("SetCurrentValues",	&CUIOptionsManagerScript::SetCurrentValues )
			.def("SaveValues",			&CUIOptionsManagerScript::SaveValues )
			.def("GetCurrentResolution",&CUIOptionsManagerScript::GetCurrentResolution)
			.def("SetResolution", &CUIOptionsManagerScript::SetResolution)
			.def("IsGroupChanged",		&CUIOptionsManagerScript::IsGroupChanged )
			.def("UndoGroup",			&CUIOptionsManagerScript::UndoGroup )
			.def("SendMessage2Group",	&CUIOptionsManagerScript::SendMessage2Group )
			
		];
}
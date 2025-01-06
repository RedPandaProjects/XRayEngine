#include "StdAfx.h"
#include "UIOptionsItem.h"
#include "UIOptionsManager.h"
#include "../XrEngine/Xr_ioconsole.h"
#include "UIComboBox.h"

CUIOptionsManager CUIOptionsItem::m_optionsManager;

CUIOptionsItem::~CUIOptionsItem()
{
	m_optionsManager.UnRegisterItem(this);
}

void CUIOptionsItem::Register(const char* entry, const char* group, int UeSetting)
{
	m_optionsManager.RegisterItem	(this, group);
	m_entry							= entry;	
	UeSettingIndex = UeSetting;
}

void CUIOptionsItem::SendMessage2Group(const char* group, const char* message)
{
	m_optionsManager.SendMessage2Group(group,message);
}

void CUIOptionsItem::OnMessage(const char* message)
{
	// do nothing
}

LPCSTR CUIOptionsItem::GetOptStringValue()
{
	return Console->GetString(m_entry.c_str());
}

void CUIOptionsItem::SaveOptStringValue(const char* val)
{
	xr_string command	= m_entry;
	command				+= " ";
	command				+= val;
	Console->Execute	(command.c_str());
}

void CUIOptionsItem::GetOptIntegerValue(int& val, int& min, int& max, bool IsUe)
{
	if (IsUe)
	{
		val = g_Engine->GetSettingInt(UeSettingIndex, min, max);
	}
	else
		val = Console->GetInteger(m_entry.c_str(),  min, max);
}

void CUIOptionsItem::SaveOptIntegerValue(int val, bool IsUe)
{
	if (IsUe)
	{
		UeSettingsPairInt.first = UeSettingIndex;
		UeSettingsPairInt.second = val;
	}
	else
	{
		string512			command;
		sprintf_s(command, "%s %d", m_entry.c_str(), val);
		Console->Execute(command);
	}
}

void CUIOptionsItem::GetOptFloatValue(float& val, float& min, float& max, bool IsUe)
{
	if (IsUe)
	{
		val = g_Engine->GetSettingFloat(UeSettingIndex, min, max);
	}
	else
		val = Console->GetFloat(m_entry.c_str(), min, max);
}

void CUIOptionsItem::SaveOptFloatValue(float val, bool IsUe)
{
	if (IsUe)
	{
		UeSettingsPairFloat.first = UeSettingIndex;
		UeSettingsPairFloat.second = val;
	}
	else
	{
		string512			command;
		sprintf_s(command, "%s %f", m_entry.c_str(), val);
		Console->Execute(command);
	}
}

bool CUIOptionsItem::GetOptBoolValue()
{
	BOOL val;
	val = Console->GetBool(m_entry.c_str());
	return val ? true : false;
}

void CUIOptionsItem::SaveOptBoolValue(bool val)
{
	string512			command;
	sprintf_s				(command, "%s %s", m_entry.c_str(), (val)?"on":"off");
	Console->Execute	(command);
}

char* CUIOptionsItem::GetOptTokenValue()
{
	return (char*)Console->GetToken(m_entry.c_str());
}

xr_token* CUIOptionsItem::GetOptToken()
{
	return Console->GetXRToken(m_entry.c_str());
}

void CUIOptionsItem::SaveOptTokenValue(const char* val){
	SaveOptStringValue(val);
}

void CUIOptionsItem::SaveUeValue()
{
		g_Engine->ChangeUeSettingsInt(UeSettingsPairInt);
		g_Engine->ChangeUeSettingsFloat(UeSettingsPairFloat);

		CUIComboBox* combo = smart_cast<CUIComboBox*>(this);
		if (combo)
		{
			LPCSTR Current = combo->GetText();
			u32 w = 0;
			u32 h = 0;
			sscanf(Current, "%dx%d", &w, &h);

			g_Engine->SetResolution(w, h);
		}
}


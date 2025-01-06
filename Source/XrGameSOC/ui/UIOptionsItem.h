#pragma once
#include "UIOptionsManager.h"

class CUIOptionsItem
{
	friend class CUIOptionsManager;
public:
	virtual					~CUIOptionsItem		();
	virtual void			Register			(const char* entry, const char* group, int UeSetting);
	static CUIOptionsManager* GetOptionsManager	() {return &m_optionsManager;}
	void					SaveUeValue();

protected:
	virtual void			SetCurrentValue		()	=0;	
	virtual void			SaveValue() {};

	virtual bool			IsChanged			()			=0;
	virtual void			SeveBackUpValue		()	{};
	virtual void			Undo				()				{SetCurrentValue();};
			
			void			SendMessage2Group	(const char* group, const char* message);
	virtual	void			OnMessage			(const char* message);


			// string
			LPCSTR			GetOptStringValue	();
			void			SaveOptStringValue	(const char* val);
			// integer
			void			GetOptIntegerValue	(int& val, int& min, int& max, bool IsUe = false);
			void			SaveOptIntegerValue	(int val, bool IsUe = false);
			// float
			void			GetOptFloatValue	(float& val, float& min, float& max, bool IsUe = false);
			void			SaveOptFloatValue	(float val, bool IsUe = false);
			// bool
			bool			GetOptBoolValue		();
			void			SaveOptBoolValue	(bool val);
			// token
			char*			GetOptTokenValue	();
			xr_token*		GetOptToken			();
			void			SaveOptTokenValue	(const char* val);

	xr_string		m_entry;
	int UeSettingIndex;
	std::pair<int, int> UeSettingsPairInt;
	std::pair<int, float> UeSettingsPairFloat;
	static CUIOptionsManager m_optionsManager;
};

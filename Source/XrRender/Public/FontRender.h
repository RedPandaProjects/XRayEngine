#ifndef FontRender_included
#define FontRender_included
#pragma once

class CGameFont;

class IFontRender
{
public:
	virtual ~IFontRender() {;}

	virtual void Initialize(LPCSTR font, float size) = 0;
	virtual void OnRender(CGameFont &owner) = 0;

	virtual float					GetTextSize(LPCSTR s) =0;
	virtual float					GetTextSize(const wide_char* wsStr) = 0;
	virtual float					GetTextSize(const char cChar) = 0;
};

#endif	//	FontRender_included
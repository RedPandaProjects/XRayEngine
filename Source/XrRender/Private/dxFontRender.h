#ifndef dxFontRender_included
#define dxFontRender_included
#pragma once

#include "FontRender.h"
#include "../../xrEngine/GameFont.h"

class dxFontRender : public IFontRender
{
public:
	dxFontRender();
	virtual ~dxFontRender();

	virtual void Initialize(LPCSTR cShader, LPCSTR cTexture);
	virtual void OnRender(CGameFont &owner);


	void Initialize(LPCSTR font, float size) override;


	float GetTextSize(LPCSTR s) override;


	float GetTextSize(const wide_char* wsStr) override;


	float GetTextSize(const char cChar) override;

private:
	ref_shader				pShader;
	ref_geom				pGeom;
};

#endif	//	FontRender_included
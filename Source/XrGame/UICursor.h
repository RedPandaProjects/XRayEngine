#pragma once

#include "ui_base.h"
class CUIStatic;

class CUICursor:	public pureRenderUI, 
					public pureScreenResolutionChanged
{
	bool			bVisible;
	Fvector2		vPos;
	Fvector2		vPrevPos;
	CUIStatic*		m_static;
	void			InitInternal				();
public:
					CUICursor					();
	virtual			~CUICursor					();
	virtual void	OnRenderUI					();
	
	Fvector2		GetCursorPositionDelta		();

	Fvector2		GetCursorPosition			();
	void			SetUICursorPosition			(Fvector2 pos);
	void			UpdateCursorPosition		(int _dx, int _dy);
	virtual void	OnScreenResolutionChanged	();

	bool			IsVisible					() {return bVisible;}
	void			Show						() {bVisible = true;}
	void			Hide						() {bVisible = false;}
};

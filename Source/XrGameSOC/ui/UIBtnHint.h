#pragma once
#include "UIWindow.h"

class CUIStatic;
class CUIFrameLineWnd;

class CUIButtonHint :public CUIWindow, public pureRenderUI
{
	CUIWindow*			m_ownerWnd;

	CUIStatic*			m_text;
	CUIFrameLineWnd*	m_border;
	bool				m_enabledOnFrame;
public:
					CUIButtonHint	();
	virtual			~CUIButtonHint	();
	CUIWindow*		Owner			()	{return m_ownerWnd;}
	void			Discard			()	{m_ownerWnd=NULL;};
	virtual void	OnRenderUI		() override;
	void			Draw_			()	{m_enabledOnFrame = true;};
	void			SetHintText		(CUIWindow* w, LPCSTR text);
};

extern CUIButtonHint* g_btnHint; 

#include "stdafx.h"

#include "UIEditBoxEx.h"
#include "UIFrameWindow.h"



CUIEditBoxEx::CUIEditBoxEx()
{
	m_pFrameWindow = xr_new<CUIFrameWindow>();
	AttachChild(m_pFrameWindow);
	m_lines.SetTextComplexMode(true);
	m_lines.SetCutWordsMode(true);
	m_lines.SetUseNewLineMode(false);
	m_lines.SetVTextAlignment(valTop);		
}

CUIEditBoxEx::~CUIEditBoxEx()
{
	xr_delete(m_pFrameWindow);
}	

void CUIEditBoxEx::Init(float x, float y, float width, float height){
	m_pFrameWindow->Init(0,0,width,height);
	CUICustomEdit::Init(x,y,width,height);
}

void CUIEditBoxEx::InitTexture(const char* texture){
	m_pFrameWindow->InitTexture(texture);
}

void CUIEditBoxEx::InitTextureEx(const char* texture, const char* shader)
{
	m_pFrameWindow->InitTextureEx(texture,shader);
}

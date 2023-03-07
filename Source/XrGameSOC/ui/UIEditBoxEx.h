#pragma once

#include "UICustomEdit.h"

class CUIFrameWindow;

class CUIEditBoxEx : public CUIMultiTextureOwner, public CUICustomEdit {
public:
	CUIEditBoxEx();
	virtual ~CUIEditBoxEx();

	// CUIWindow
	virtual void	Init(float x, float y, float width, float heigt);

	// CUIMultiTextureOwner
	virtual void	InitTexture(const char* texture);

	void InitTextureEx(const char* texture, const char* shader) override;

protected:
	CUIFrameWindow*	m_pFrameWindow;
};

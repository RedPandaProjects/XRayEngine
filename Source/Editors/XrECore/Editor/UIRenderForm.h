#pragma once
class ECORE_API UIRenderForm :public XrUI
{
public:
	UIRenderForm();
	virtual ~UIRenderForm();
	virtual void Draw();
	IC Ivector2 GetMousePos()const { return m_mouse_position; }
private:
	Ivector2	m_mouse_position;
	ref_texture m_rt;
};
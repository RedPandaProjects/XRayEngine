#pragma once
class ECORE_API UIRenderForm :public XrUI
{
public:
	UIRenderForm();
	virtual ~UIRenderForm();
	virtual void Draw();
private:
	ref_texture m_rt;
};
#pragma once
class UILeftBarForm :
	public XrUI
{
public:
	enum RenderMode
	{
		Render_Editor,
		Render_Engine
	};
	UILeftBarForm();
	virtual ~UILeftBarForm();
	virtual void Draw();
	IC RenderMode GetRenderMode() {return m_RenderMode;}
	void SetRenderMode(bool bEngineNode);
private:
	RenderMode m_RenderMode;
};


#pragma once
class UIMainForm :public XrUI
{
public:
	UIMainForm();
	virtual ~UIMainForm();
	virtual void Draw();
	void Frame();
	IC UILeftBarForm* GetLeftBarForm() {return m_LeftBar;}
	IC UITopBarForm* GetTopBarForm() { return m_TopBar; }
	IC UIKeyForm* GetKeyForm() { return m_KeyForm; }
private:
	UITopBarForm *m_TopBar;
	UIRenderForm* m_Render;
	UIMainMenuForm* m_MainMenu;
	UILeftBarForm* m_LeftBar;
	UIKeyForm* m_KeyForm;
};
extern UIMainForm* MainForm;

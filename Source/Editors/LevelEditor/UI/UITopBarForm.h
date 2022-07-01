#pragma once
class UITopBarForm :public XrUI
{
public:
	UITopBarForm();
	virtual ~UITopBarForm();
	virtual void Draw();
	void RefreshBar();
	bool VerifySpaceRestrictors()const {return m_VerifySpaceRestrictors;}
private:
	void ClickLevelPlay();
	void ClickLevelSimulate();

	void ClickUndo(); ref_texture m_tUndo; u32 m_timeUndo;
	void ClickRedo(); ref_texture m_tRedo; u32 m_timeRedo;
	void ClickCForm();  ref_texture m_tCForm;
	void ClickAIMap();  ref_texture m_tAIMap;
	void ClickGGraph();  ref_texture m_tGGraph;

	bool m_VerifySpaceRestrictors;
};
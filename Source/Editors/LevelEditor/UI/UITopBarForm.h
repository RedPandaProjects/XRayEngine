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

	void ClickUndo(); ref_texture m_tUndo; u32 m_timeUndo;
	void ClickRedo(); ref_texture m_tRedo; u32 m_timeRedo;

	void ClickNew(); ref_texture m_tNew;
	void ClickOpen(); ref_texture m_tOpen;
	void ClickSave(); ref_texture m_tSave;

	void ClickCForm();  ref_texture m_tCForm;
	void ClickAIMap();  ref_texture m_tAIMap;
	void ClickGGraph();  ref_texture m_tGGraph;
	void ClickPlayInEditor();  ref_texture m_tPlayInEditor;
	void ClickPlayPC();  ref_texture m_tPlayPC;
	PROCESS_INFORMATION m_GameProcess;
	bool m_VerifySpaceRestrictors;
};
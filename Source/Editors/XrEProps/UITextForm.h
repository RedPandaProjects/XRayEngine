#pragma once
class XREPROPS_API UITextForm :public XrUI
{
public:
	UITextForm(xr_string*str);
	virtual ~UITextForm();
	virtual void Draw();
private:
	void CLBOk();
	void CLBApply();
	void CLBCancel();

	void CLBLoad();
	void CLBSave();
	void CLBClear();
private:
	xr_string& m_Text;
	string4096 m_EditText;
};


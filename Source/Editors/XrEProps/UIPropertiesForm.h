#pragma once
class XREPROPS_API UIPropertiesForm :public XrUI, private FolderHelper<PropItem,true>
{
public:
	void SetModifiedEvent(TOnModifiedEvent modif = 0) { OnModifiedEvent = modif; }
	UIPropertiesForm();
	virtual ~UIPropertiesForm();
	virtual void Draw();
	void AssignItems(PropItemVec& items);
	PropItem* FindItem(const char* path);
	PropItem* FindItemOfName(shared_str name);
	void ClearProperties();
	IC void SetReadOnly(bool enable) { m_Flags.set(plReadOnly, enable); }
	IC bool IsModified() { return m_bModified;}
	IC bool Empty() { return m_Items.size() == 0; }
public:
	enum {
		plReadOnly = (1 << 0),
	};
	Flags32 m_Flags;
private:
	virtual void DrawItem(Node* Node);
	virtual void DrawItem(const char*name,PropItem* Node);
	virtual bool IsDrawFloder(Node* Node);
	virtual void DrawAfterFloderNode(bool is_open, Node* Node = 0);
private:
	PropItemVec m_Items;
	Node m_GeneralNode;
	PropItem* m_EditChooseValue;
	PropItem* m_EditTextureValue;
	PropItem* m_EditShortcutValue;
private:
	TOnModifiedEvent 	OnModifiedEvent;
	bool m_bModified;
	void Modified() { m_bModified = true; if (!OnModifiedEvent.empty()) OnModifiedEvent(); }
private:
	PropItem* m_EditTextValue;
	char* m_EditTextValueData;
	int m_EditTextValueDataSize;
	void DrawEditText();
	int  DrawEditText_Callback(ImGuiInputTextCallbackData* data);
private:
	GameTypeChooser m_EditGameTypeChooser;
	PropItem* m_EditGameTypeValue;
	void DrawEditGameType();
private:
	void RemoveMixed(Node* Node);
};


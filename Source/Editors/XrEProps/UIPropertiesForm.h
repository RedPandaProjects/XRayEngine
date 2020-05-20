#pragma once
class XREPROPS_API UIPropertiesForm :public XrUI, private FolderHelper<PropItem>
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
public:
	enum {
		plReadOnly = (1 << 0),
		plMultiSelect = (1 << 1),
	};
	Flags32 m_Flags;
private:
	virtual void DrawItem(Node* Node);
	virtual bool IsDrawFloder(Node* Node);
	virtual void DrawAfterFloderNode();
private:
	PropItemVec m_Items;
	Node m_GeneralNode;
	PropItem* m_EditChooseValue;
	PropItem* m_EditShortcutValue;
private:
	TOnModifiedEvent 	OnModifiedEvent;
	bool m_bModified;
	void Modified() { m_bModified = true; if (!OnModifiedEvent.empty()) OnModifiedEvent(); }
};


#pragma once
class XREPROPS_API UIItemListForm :
	public XrUI, private FolderHelper<ListItem,true>
{
	TOnILItemFocused	OnItemFocusedEvent;
	TOnItemRemove	OnItemRemoveEvent;
public:
	UIItemListForm();
	virtual ~UIItemListForm();
public:
	virtual void Draw();
	void ClearList();
	void RemoveSelectItem();
	void ClearSelected();
	void AssignItems(ListItemsVec& items);
	IC const ListItemsVec& GetItems()const {return m_Items;}
	bool GetSelected(RStringVec& items)const;
public:
	IC void		SetOnItemFocusedEvent(TOnILItemFocused e) { OnItemFocusedEvent = e; }
	IC void		SetOnItemRemoveEvent(TOnItemRemove e) { OnItemRemoveEvent = e; }
private:
	virtual void DrawItem(Node* Node);
	virtual bool IsDrawFloder(Node* Node);
	virtual void IsItemClicked(Node* Node);
	virtual bool IsFloderBullet(Node* Node);
	Node m_GeneralNode;
	ListItemsVec m_Items;
	ListItem* m_SelectedItem;
};


#pragma once
class XREPROPS_API UITreeItem
{
public:
	UITreeItem(shared_str Name);
	virtual ~UITreeItem();

	UITreeItem* AppendItem(const char* Path);
	UITreeItem* FindItem(const char* Path);
	xr_vector<UITreeItem*> Items;
	shared_str Name;
protected:
	virtual UITreeItem* CreateItem(shared_str Name);
};


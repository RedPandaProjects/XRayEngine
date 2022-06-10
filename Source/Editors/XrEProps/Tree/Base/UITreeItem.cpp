#include "stdafx.h"

UITreeItem::UITreeItem(shared_str NewName):Name(NewName)
{
}

UITreeItem::~UITreeItem()
{
	for (UITreeItem* Item : Items)
	{
		xr_delete(Item);
	}
}

UITreeItem* UITreeItem::AppendItem(const char* Path)
{
	VERIFY(Path && *Path);
	if (strchr(Path, '\\'))
	{
		string_path Name;
		xr_strcpy(Name, Path);
		strchr(Name, '\\')[0] = 0;
		UITreeItem* Item = FindItem(Name);
		if (!Item)
		{
			Items.push_back(CreateItem(Name));
			Item = Items.back();
		}
		return Item->AppendItem(strchr(Path, '\\') + 1);
	}
	else
	{
		UITreeItem* Item = FindItem(Path);
		if (!Item)
		{
			Items.push_back(CreateItem(Path));
			Item = Items.back();
		}
		return Item;
	}
}

UITreeItem* UITreeItem::FindItem(const char* Path)
{
	if (strchr(Path, '\\'))
	{
		string_path Name;
		xr_strcpy(Name, Path);
		strchr(Name, '\\')[0] = 0;
		UITreeItem* Item = FindItem(Name);
		if (Item)
		{
			return Item->FindItem(strchr(Path, '\\') + 1);
		}
	}
	else
	{
		shared_str FName = Path;
		for (UITreeItem* Item : Items)
		{
			if (Item->Name == FName)
			{
				return Item;
			}
		}
	}
	return nullptr;
}

UITreeItem* UITreeItem::CreateItem(shared_str Name)
{
	return xr_new< UITreeItem>(Name);
}


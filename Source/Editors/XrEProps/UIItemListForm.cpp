#include "stdafx.h"


UIItemListForm::UIItemListForm()
{
	m_SelectedItem = nullptr;
}

UIItemListForm::~UIItemListForm()
{
	ClearList();
}

void UIItemListForm::Draw()
{
	DrawNode(&m_GeneralNode);
}

void UIItemListForm::ClearList()
{
	m_SelectedItem = nullptr;
	for (ListItem* item : m_Items)
	{
		xr_delete(item);
	}
	m_Items.clear();

}

void UIItemListForm::RemoveSelectItem()
{
	if (m_SelectedItem == nullptr)return;
	for (auto b = m_Items.begin(), e = m_Items.end(); b != e; b++)
	{
		if (*b == m_SelectedItem)
		{
			m_Items.erase(b);
			break;
		}
	}
	m_GeneralNode = Node();
	for (ListItem* item : m_Items)
	{
		Node* N = AppendObject(&m_GeneralNode, item->Key());
		VERIFY(N);
		N->Object = item;
	}
}

void UIItemListForm::ClearSelected()
{
	m_SelectedItem = nullptr;
	if (!OnItemFocusedEvent.empty())
		OnItemFocusedEvent(m_SelectedItem);
}
bool UIItemListForm::GetSelected(RStringVec& items) const
{
	if (m_SelectedItem == nullptr)
		return false;
	items.push_back(m_SelectedItem->key);
	return true;
}
void UIItemListForm::AssignItems(ListItemsVec& items)
{
	ClearList();
	m_Items = items;
	m_GeneralNode = Node();
	for (ListItem* item : m_Items)
	{
		Node*N=	AppendObject(&m_GeneralNode, item->Key());
		VERIFY(N);
		N->Object = item;
	}
}

void UIItemListForm::DrawItem(Node* Node)
{
	if (!Node->Object->Visible())return;
	ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	if (m_SelectedItem == Node->Object)
		Flags |= ImGuiTreeNodeFlags_Bullet;
	ImGui::TreeNodeEx(Node->Name.c_str(), Flags);
	if (ImGui::IsItemClicked())
	{
		if (!OnItemFocusedEvent.empty())
			OnItemFocusedEvent(Node->Object);
		m_SelectedItem = Node->Object;
	}
}

bool UIItemListForm::IsDrawFloder(Node* node)
{

	if (node->Object )return node->Object->Visible();
	bool result = false;
	for (Node& N : node->Nodes)
	{
		result = result | IsDrawFloder(&N);
	}
	return result;
}

void UIItemListForm::IsItemClicked(Node* Node)
{
	m_SelectedItem = Node->Object;
	if (!OnItemFocusedEvent.empty())
		OnItemFocusedEvent(Node->Object);
}

bool UIItemListForm::IsFloderBullet(Node* Node)
{
	if (Node->Object != nullptr)
		return m_SelectedItem == Node->Object;
	return false;
}

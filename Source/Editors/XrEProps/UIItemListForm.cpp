#include "stdafx.h"


UIItemListForm::UIItemListForm()
{
	m_SelectedItem = nullptr;
	m_Flags.zero();
	m_UseMenuEdit = false;
}

UIItemListForm::~UIItemListForm()
{
	ClearList();
}

void UIItemListForm::Draw()
{

	m_UseMenuEdit = false;

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
	DrawMenuEdit();
	DrawNode(&m_GeneralNode);
	ImGui::PopStyleVar(2);
	if (!m_UseMenuEdit)
	{
		m_edit_node = nullptr;
	}

}

void UIItemListForm::ClearList()
{
	m_GeneralNode = Node();
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

void UIItemListForm::SelectItem(const char* name)
{
	if (name == nullptr)return;
	Node* N = SelectObject(&m_GeneralNode, name);
	if (N)m_SelectedItem = N->Object;
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
int UIItemListForm::GetSelected(LPCSTR pref, ListItemsVec& items, bool bOnlyObject)
{
	ListItem* prop = m_SelectedItem;


	if (prop && (!bOnlyObject || (bOnlyObject && prop->m_Object)))
	{
		xr_string key = *prop->key;
		if (pref) {
			if (0 == key.find(pref))
				items.push_back(prop);
		}
		else
			items.push_back(prop);
	}
	return items.size();
}
void UIItemListForm::AssignItems(ListItemsVec& items, const char* name_selection, bool clear_floder)
{
	ClearList();
	m_Items = items;
	
	if (!clear_floder)
	{
		ClearObject(&m_GeneralNode);
	}
	else
	{
		m_GeneralNode = Node();
	}


	for (ListItem* item : m_Items)
	{
		Node*N=	AppendObject(&m_GeneralNode, item->Key());
		VERIFY(N);
		N->Object = item;
	}
	if (name_selection)
	{
		Node*N =  SelectObject(&m_GeneralNode, name_selection);
		if (N)m_SelectedItem = N->Object;
	}
}

void UIItemListForm::DrawMenuEdit()
{
	if (ImGui::BeginPopupContextItem("MenuEdit"))
	{
		ImGui::PopStyleVar(2);
		m_UseMenuEdit = true;

		if (!OnItemCreateEvent.empty())
		{
			if (ImGui::MenuItem("Create"))
			{
				for (int i = 0; i < 256; i++)
				{
					string4096  name;
					if (i == 0)
						xr_strcpy(name, "new");
					else
						xr_sprintf(name, "new_%d", i);

					Node* N = m_edit_node == 0 ? &m_GeneralNode : m_edit_node;
					string_path path; path[0] = 0;
					if (N->Path.c_str() && N->Path.c_str()[0])
					{
						xr_strcpy(path, N->Path.c_str());
					}
					if (N->IsFloder()&& N->Name.c_str()&& N->Name.c_str()[0])
					{
						if (path[0])
							xr_strcat(path, "\\");
						xr_strcat(path, N->Name.c_str());
					}
					if (path[0])
						xr_strcat(path, "\\");
					xr_strcat(path, name);
					if (!Find(&m_GeneralNode, path))
					{
						OnItemCreateEvent(path);
						ImGui::CloseCurrentPopup();
						m_edit_node = nullptr;
						break;
					}
				}
			}
		}
		if (!OnItemCloneEvent.empty()&&m_edit_node&&m_edit_node->IsObject())
		{
			if (ImGui::MenuItem("Clone"))
			{
				string_path parent_path; parent_path[0] = 0;
				Node* N = m_edit_node == 0 ? &m_GeneralNode : m_edit_node;
				GetFullPath(N, parent_path);
				for (int i = 0; i < 256; i++)
				{
					string4096  name;
					if (i == 0)
						xr_sprintf(name, "%s_clone", m_edit_node->Name.c_str());
					else
						xr_sprintf(name, "%s_clone_%d", m_edit_node->Name.c_str(), i);

					Node* N = m_edit_node == 0 ? &m_GeneralNode : m_edit_node;
					string_path path; path[0] = 0;
					if (N->Path.c_str() && N->Path.c_str()[0])
					{
						xr_strcpy(path, N->Path.c_str());
					}
					if (m_edit_node->IsFloder())
					{
						if (path[0])
							xr_strcat(path, "\\");
						xr_strcat(path, m_edit_node->Name.c_str());
					}
					if (path[0])
						xr_strcat(path, "\\");
					xr_strcat(path, name);
					if (!Find(&m_GeneralNode, path))
					{

						OnItemCloneEvent(parent_path,path);
						ImGui::CloseCurrentPopup();
						m_edit_node = nullptr;
						break;
					}
				}

			}
			if (!OnItemCreateEvent.empty() || !OnItemCloneEvent.empty())
			{
				ImGui::Separator();
			}
		}
		
		if (ImGui::MenuItem("Create Floder"))
		{
			Node* N = m_edit_node == 0 ? &m_GeneralNode : m_edit_node;
			for (int i = 0; i < 256; i++)
			{
				string4096 full_path,name;
				if(i==0)
					xr_strcpy(name, "new_floder");
				else
					xr_sprintf(name, "new_floder_%d",i);

				if (N->Path.c_str() && N->Path.c_str()[0])
					xr_strcpy(full_path, N->Path.c_str());
				else
					full_path[0] = 0;
				if (N->IsFloder()&& N->Name.c_str() && N->Name.c_str()[0])
				{
					if (full_path[0])
						xr_strcat(full_path, "\\");
					xr_strcat(full_path, N->Name.c_str());
				}
				if (full_path[0])
					xr_strcat(full_path, "\\");
				xr_strcat(full_path, name);

				if (!FindFloder(&m_GeneralNode, full_path)&&AppendFloder(&m_GeneralNode, full_path))
				{
					ImGui::CloseCurrentPopup();
					m_edit_node = nullptr;
					break;
				} 
			}
		}
		if (m_edit_node &&m_edit_node != &m_GeneralNode)
		{
			ImGui::Separator();
			if (ImGui::BeginMenu("Rename"))
			{
				ImGui::InputText("New Name", m_edit_name, sizeof(m_edit_name));
				if (ImGui::Button("Ok"))
				{
					string4096 full_path;
					if (m_edit_node->Path.c_str() && m_edit_node->Path.c_str()[0])
						xr_strcpy(full_path, m_edit_node->Path.c_str());
					else
						full_path[0] = 0;

					if (full_path[0])
						xr_strcat(full_path, "\\");
					xr_strcat(full_path, m_edit_name);
					if (Move(&m_GeneralNode, m_edit_node, full_path))
					{
						ImGui::CloseCurrentPopup();
						m_edit_node = nullptr;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Move"))
			{
				ImGui::InputText("New Path", m_edit_path, sizeof(m_edit_path));
				if (ImGui::Button("Ok"))
				{
					string4096 full_path;
					xr_strcpy(full_path, m_edit_path);
					if (m_edit_path[0])
						xr_strcat(full_path, "\\");
					xr_strcat(full_path, m_edit_node->Name.c_str());
					if (Move(&m_GeneralNode, m_edit_node, full_path))
					{
						ImGui::CloseCurrentPopup();
						m_edit_node = nullptr;
					}
				}ImGui::SameLine();
				if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Delete"))
			{
				Remove(&m_GeneralNode, m_edit_node, true);
				ImGui::CloseCurrentPopup();
				m_edit_node = nullptr;
			}
		}
		
		ImGui::EndPopup();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
	}

	
}

void UIItemListForm::DrawAfterFloderNode(bool is_open, Node* Node)
{
	if (m_Flags.is(fMenuEdit))
	{
		if (ImGui::OpenPopupOnItemClick("MenuEdit", 1))
		{
			m_UseMenuEdit = true;
			m_edit_node = Node;
			xr_strcpy(m_edit_path, Node->Path.c_str());
			xr_strcpy(m_edit_name, Node->Name.c_str());
		}
	}
	if (is_open&& m_Flags.is(fMenuEdit))
	{
		DrawMenuEdit();
	}
}

void UIItemListForm::DrawItem(Node* Node)
{
	if (!Node->Object->Visible())return;
	ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	if (m_SelectedItem == Node->Object)
		Flags |= ImGuiTreeNodeFlags_Bullet;
	if (m_edit_node == Node)
		Flags |= ImGuiTreeNodeFlags_Selected;
	ImGui::TreeNodeEx(Node->Name.c_str(), Flags);

	if (m_Flags.is(fMenuEdit))
	{
		if (ImGui::OpenPopupOnItemClick("MenuEdit", 1))
		{
			m_UseMenuEdit = true;
			m_edit_node = Node;
			xr_strcpy(m_edit_path, Node->Path.c_str());
			xr_strcpy(m_edit_name,Node->Name.c_str());
		}
	}

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
	bool result = m_Flags.test(fMenuEdit);;
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

bool UIItemListForm::IsFloderSelected(Node* Node)
{
	return Node == m_edit_node;
}

void UIItemListForm::EventRenameNode(Node* Node, const char* old_path, const char* new_path)
{
	EItemType type = TYPE_FOLDER;
	if (Node->IsObject())
	{
		type = TYPE_OBJECT;
		Node->Object->key = new_path;

	}
	if(!OnItemRenameEvent.empty())OnItemRenameEvent(old_path, new_path, type);
}

void UIItemListForm::EventRemoveNode(Node* Node, const char* path)
{
	EItemType type = TYPE_FOLDER;
	if (Node->IsObject())
	{
		type = TYPE_OBJECT;
	}
	if (!OnItemRemoveEvent.empty())OnItemRemoveEvent(path, type);
}

void UIItemListForm::ClearObject(Node* N)
{
	for (int i = N->Nodes.size()-1; i >= 0; i--)
	{
		if (N->Nodes[i].IsObject())
		{
			N->Nodes.erase(N->Nodes.begin() + i);
		}
		else
		{
			ClearObject(&N->Nodes[i]);
		}
	}
}

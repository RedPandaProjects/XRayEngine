#include "stdafx.h"
#include "UIKeyPressForm.h"
UIPropertiesForm::UIPropertiesForm()
{
	m_bModified = false;
	m_EditChooseValue = nullptr;
	m_EditShortcutValue = nullptr;
	m_Flags.zero();
}

UIPropertiesForm::~UIPropertiesForm()
{
	ClearProperties();
}

void UIPropertiesForm::Draw()
{
	{
		if (m_EditChooseValue)
		{
			shared_str result;
			bool is_result;
			if (UIChooseForm::GetResult(is_result,result))
			{
				if (is_result)
				{
					if (m_EditChooseValue->AfterEdit<ChooseValue, shared_str>(result))
						if (m_EditChooseValue->ApplyValue<ChooseValue, shared_str>(result))
						{
							Modified();
						}
				}
				m_EditChooseValue = nullptr;
			}

			UIChooseForm::Update();
		}
		if (m_EditShortcutValue)
		{
			xr_shortcut result;
			if (UIKeyPressForm::GetResult(result))
			{
				if (m_EditShortcutValue->AfterEdit<ShortcutValue, xr_shortcut>(result))
					if (m_EditShortcutValue->ApplyValue<ShortcutValue, xr_shortcut>(result))
					{
						Modified();
					}
				m_EditShortcutValue = nullptr;
			}
		}
	}
	if (m_GeneralNode.Nodes.size())
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
	
		ImGui::Columns(2);
		ImGui::Separator();
		DrawNode(&m_GeneralNode);
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar(2);
	}
}


void UIPropertiesForm::AssignItems(PropItemVec& items)
{
	m_GeneralNode = Node();
	m_Items = items;
	for (PropItem* item : items)
	{
		item->m_Owner = this;
		Node*N = AppendObject(&m_GeneralNode,item->Key());
		N->Object = item;
	}
}

PropItem* UIPropertiesForm::FindItemOfName(shared_str name)
{
	for (PropItem* I : m_Items)
	{
		const char* key = I->Key();
		if (strrchr(key, '\\'))
		{
			key = strrchr(key, '\\') + 1;
		}
		if (name == key)
		{
			return I;
		}
	}
	return nullptr;
}

void UIPropertiesForm::ClearProperties()
{
	VERIFY(!m_EditChooseValue);
	for (PropItem* I : m_Items)
	{
		xr_delete(I);
	}
	m_GeneralNode = Node();
	m_Items.clear();
}

PropItem* UIPropertiesForm::FindItem(const char* name)
{
	Node* N = FindObject(&m_GeneralNode, name);
	if (N)return N->Object;
	return nullptr;
}

void UIPropertiesForm::DrawItem(Node* node)
{
	
	EPropType type = node->Object->Type();
	switch (type)
	{
	case PROP_CANVAS:
	{	
		ImGui::Text(node->Name.c_str());
		ImGui::NextColumn();
		if (node->Object->m_Flags.test(PropItem::flMixed))
		{
			ImGui::Text(node->Object->GetDrawText().c_str());

		}
		else
		{
			ImGui::PushItemWidth(-1);
			CanvasValue* val = dynamic_cast<CanvasValue*>(node->Object->GetFrontValue()); R_ASSERT(val);
			if (!val->OnDrawCanvasEvent.empty())
				val->OnDrawCanvasEvent(val);
		}
		ImGui::NextColumn();
	}
	break;
	case PROP_BUTTON:
		ImGui::Text(node->Name.c_str());
		ImGui::NextColumn();
		if (node->Object->m_Flags.test(PropItem::flMixed))
		{
			ImGui::Text(node->Object->GetDrawText().c_str());
			
		}
		else
		{
			bool bRes = false;
			bool bSafe = false;
			ButtonValue* V = dynamic_cast<ButtonValue*>(node->Object->GetFrontValue()); R_ASSERT(V);
			if (!V->value.empty())
			{
				ImGui::PushItemWidth(-1);
				float size = float(ImGui::CalcItemWidth());
				float dx = floorf(size / float(V->value.size()));
				float offset = size -( dx * V->value.size());
				V->btn_num = V->value.size();
				for (RStringVecIt it = V->value.begin(); it != V->value.end(); it++)
				{
				
					int k = it - V->value.begin();
					if (ImGui::Button(it->c_str(), ImVec2(dx+ offset, 0)))
					{
						V->btn_num = k;

						bRes |= V->OnBtnClick(bSafe);
					}
					offset = 0;
					ImGui::SameLine(0,2);
				}
			}
			else
			{
				ImGui::Text("");
			}
		}
		ImGui::NextColumn();
		break;
	case PROP_WAVE:
	case PROP_UNDEF:
		break;
	case PROP_CAPTION:
	{
		ImGui::Text(node->Name.c_str());
		ImGui::NextColumn();
		ImGui::Text(node->Object->GetDrawText().c_str());
		ImGui::NextColumn();
	}
	break;

	default:
		ImGui::PushID(node->Name.c_str());
		if (m_Flags.is(plReadOnly))
		{
			ImGui::Text(node->Name.c_str());
			ImGui::NextColumn();
			ImGui::Text(node->Object->GetDrawText().c_str());
			ImGui::NextColumn();
		}
		else if (node->Object->m_Flags.test(PropItem::flMixed)&&!node->Object->m_Flags.test(PropItem::flIgnoreMixed))
		{
			ImGui::Text(node->Name.c_str());
			ImGui::NextColumn();
			if (ImGui::Button("(Mixed)", ImVec2(-1, 0)))
			{
				node->Object->m_Flags.set(PropItem::flIgnoreMixed, 1);
			}
			ImGui::NextColumn();
		}
		else
		{
			ImGui::Text(node->Name.c_str());
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);
			DrawItem(node->Name.c_str(),node->Object);
			ImGui::NextColumn();
		}
		ImGui::PopID();
		break;
	}
}

bool UIPropertiesForm::IsDrawFloder(Node* Node)
{
	return true;
}

void UIPropertiesForm::DrawAfterFloderNode()
{
	ImGui::NextColumn();
	ImGui::Text("");
	ImGui::NextColumn();
}


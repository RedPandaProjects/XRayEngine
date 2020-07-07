#include "stdafx.h"
#include "UIKeyPressForm.h"
UIPropertiesForm::UIPropertiesForm()
{
	m_bModified = false;
	m_EditChooseValue = nullptr;
	m_EditShortcutValue = nullptr;
	m_EditTextureValue = nullptr;
	m_EditTextValueData = nullptr;
	m_Flags.zero();
}

UIPropertiesForm::~UIPropertiesForm()
{
	if (m_EditTextValueData)xr_delete(m_EditTextValueData);
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
		if (m_EditTextureValue)
		{
			shared_str result;
			bool is_result;
			if (UIChooseForm::GetResult(is_result, result))
			{
				if (is_result)
				{
					if (result.c_str() == nullptr)
					{
						xr_string result_as_str = "$null";
						if (m_EditTextureValue->AfterEdit<CTextValue, xr_string>(result_as_str))
							if (m_EditTextureValue->ApplyValue<CTextValue, LPCSTR>(result_as_str.c_str()))
							{
								Modified();
							}
					}
					else
					{
						xr_string result_as_str = result.c_str();
						if (m_EditTextureValue->AfterEdit<CTextValue, xr_string>(result_as_str))
							if (m_EditTextureValue->ApplyValue<CTextValue, LPCSTR>(result_as_str.c_str()))
							{
								Modified();
							}
					}
					
				}
				m_EditTextureValue = nullptr;
			}
			UIChooseForm::Update();
		}
		if (m_EditShortcutValue)
		{
			xr_shortcut result;
			bool ok;
			if (UIKeyPressForm::GetResult(ok,result))
			{
				if (ok)
				{
					if (m_EditShortcutValue->AfterEdit<ShortcutValue, xr_shortcut>(result))
						if (m_EditShortcutValue->ApplyValue<ShortcutValue, xr_shortcut>(result))
						{
							Modified();
						}
				}
				m_EditShortcutValue = nullptr;
			}
		}
	}
	if (m_GeneralNode.Nodes.size())
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 5);
		ImGui::Columns(2);
		ImGui::Separator();
		DrawNode(&m_GeneralNode);
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar(3);
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
		VERIFY(N);
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
	//
	switch (type)
	{
	case PROP_CANVAS:
	{
		ImGui::Text(node->Name.c_str());
		ImGui::NextColumn();
		if (node->Object->m_Flags.test(PropItem::flMixed))
		{
			ImGui::TextDisabled(node->Object->GetDrawText().c_str());

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
			ImGui::TextDisabled(node->Object->GetDrawText().c_str());

		}
		else
		{
			ImGui::PushID(node->Name.c_str());
			bool bRes = false;
			bool bSafe = false;
			ButtonValue* V = dynamic_cast<ButtonValue*>(node->Object->GetFrontValue()); R_ASSERT(V);
			if (!V->value.empty())
			{
				ImGui::PushItemWidth(-1);
				float size = float(ImGui::CalcItemWidth());
				float dx = floorf(size / float(V->value.size()));
				float offset = size - (dx * V->value.size());
				V->btn_num = V->value.size();
				for (RStringVecIt it = V->value.begin(); it != V->value.end(); it++)
				{

					int k = it - V->value.begin();
					if (ImGui::Button(it->c_str(), ImVec2(dx + offset, 0)))
					{
						V->btn_num = k;

						bRes |= V->OnBtnClick(bSafe);
					}
					offset = 0;
					ImGui::SameLine(0, 2);
				}
			}
			else
			{
				ImGui::Text("");
			}
			ImGui::PopID();
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
		ImGui::TextDisabled(node->Object->GetDrawText().c_str());
		ImGui::NextColumn();
	}
	break;

	default:
		ImGui::PushID(node->Name.c_str());
		if (m_Flags.is(plReadOnly))
		{
			ImGui::Text(node->Name.c_str());
			ImGui::NextColumn();
			if (type == PROP_BOOLEAN)
			{
				FlagValueCustom* V = dynamic_cast<FlagValueCustom*>(node->Object->GetFrontValue()); VERIFY(V);
				ImGui::TextDisabled(V->GetValueEx() ? "true" : "false");
			}
			else
			{
				ImGui::TextDisabled(node->Object->GetDrawText().c_str());
			}
			ImGui::NextColumn();
		}
		else if (node->Object->m_Flags.test(PropItem::flMixed) && !node->Object->m_Flags.test(PropItem::flIgnoreMixed))
		{
			ImGui::Text(node->Name.c_str());
			ImGui::NextColumn();
			if (ImGui::Button("(Mixed)", ImVec2(-1, 0)))
			{
				RemoveMixed(node);
			}
			ImGui::NextColumn();
		}
		else
		{
			if (node->Object->m_Flags.test(PropItem::flShowCB))
			{
				if (ImGui::CheckboxFlags("##value", &node->Object->m_Flags.flags, PropItem::flCBChecked))
				{
					node->Object->OnChange();
					Modified();
				} ImGui::SameLine(0, 2);
			}
			ImGui::Text(node->Name.c_str());
			ImGui::NextColumn();
			if (node->Object->m_Flags.test(PropItem::flDisabled))
			{
				if (type == PROP_FLAG)
				{
					FlagValueCustom* V = dynamic_cast<FlagValueCustom*>(node->Object->GetFrontValue()); VERIFY(V);
					ImGui::TextDisabled(V->GetValueEx()?"true":"false");
				}
				else
				{
					ImGui::TextDisabled(node->Object->GetDrawText().c_str());
				}
			}
			else
			{
				ImGui::PushItemWidth(-1);
				DrawItem(node->Name.c_str(), node->Object);
			}
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

void UIPropertiesForm::DrawAfterFloderNode(bool is_open, Node* node )
{
	if (!node->Object)
	{
		ImGui::NextColumn();
		ImGui::Text("");
		ImGui::NextColumn();
		return;
	}
	EPropType type = node->Object->Type();
	//
	switch (type)
	{
	case PROP_CANVAS:
	{
		ImGui::NextColumn();
		ImGui::Text("");
		ImGui::NextColumn();
	}
	break;
	case PROP_BUTTON:
		ImGui::PushID(node->Name.c_str());
		ImGui::NextColumn();
		if (node->Object->m_Flags.test(PropItem::flMixed))
		{
			ImGui::TextDisabled(node->Object->GetDrawText().c_str());

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
				float offset = size - (dx * V->value.size());
				V->btn_num = V->value.size();
				for (RStringVecIt it = V->value.begin(); it != V->value.end(); it++)
				{

					int k = it - V->value.begin();
					if (ImGui::Button(it->c_str(), ImVec2(dx + offset, 0)))
					{
						V->btn_num = k;

						bRes |= V->OnBtnClick(bSafe);
					}
					offset = 0;
					ImGui::SameLine(0, 2);
				}
			}
			else
			{
				ImGui::Text("");
			}
		}
		ImGui::NextColumn();
		ImGui::PopID();
		break;
	case PROP_WAVE:
	case PROP_UNDEF:
		break;
	case PROP_CAPTION:
	{
		ImGui::NextColumn();
		ImGui::TextDisabled(node->Object->GetDrawText().c_str());
		ImGui::NextColumn();
	}
	break;

	default:
		ImGui::PushID(node->Name.c_str());
		if (m_Flags.is(plReadOnly))
		{
			ImGui::NextColumn();
			ImGui::TextDisabled(node->Object->GetDrawText().c_str());
			ImGui::NextColumn();
		}
		else if (node->Object->m_Flags.test(PropItem::flMixed) && !node->Object->m_Flags.test(PropItem::flIgnoreMixed))
		{
			ImGui::NextColumn();
			if (ImGui::Button("(Mixed)", ImVec2(-1, 0)))
			{
				node->Object->m_Flags.set(PropItem::flIgnoreMixed, 1);
			}
			ImGui::NextColumn();
		}
		else
		{
			
			ImGui::NextColumn();
			if (node->Object->m_Flags.test(PropItem::flDisabled))
			{
				if (type == PROP_FLAG)
				{
					FlagValueCustom* V = dynamic_cast<FlagValueCustom*>(node->Object->GetFrontValue()); VERIFY(V);
					ImGui::TextDisabled(V->GetValueEx() ? "true" : "false");
				}
				else
				{
					ImGui::TextDisabled(node->Object->GetDrawText().c_str());
				}
			}
			else
			{
				ImGui::PushItemWidth(-1);
				DrawItem(node->Name.c_str(), node->Object);
			}
			ImGui::NextColumn();
		}
		ImGui::PopID();
		break;
	}
}

void UIPropertiesForm::DrawEditText()
{

	if (ImGui::BeginPopupContextItem("EditText", 0))
	{
		R_ASSERT(m_EditTextValueData);
		ImGui::PopStyleVar(3);

		ImGui::BeginGroup();
		if (ImGui::Button("Ok"))
		{
			CTextValue* V1 = dynamic_cast<CTextValue*>(m_EditTextValue->GetFrontValue());
			if (V1)
			{
				xr_string out = m_EditTextValueData;
				if (m_EditTextValue->AfterEdit<CTextValue, xr_string>(out))
				{
					if (m_EditTextValue->ApplyValue<CTextValue, LPCSTR>(out.c_str()))
					{
						xr_delete(m_EditTextValueData);
						Modified();
						ImGui::CloseCurrentPopup();
					}
				}
			}
			else
			{
				RTextValue* V2 = dynamic_cast<RTextValue*>(m_EditTextValue->GetFrontValue());
				if (V2)
				{
					shared_str out = m_EditTextValueData;
					if (m_EditTextValue->AfterEdit<RTextValue, shared_str>(out))
					{
						if (m_EditTextValue->ApplyValue<RTextValue, shared_str>(out))
						{
							xr_delete(m_EditTextValueData);
							Modified();
							ImGui::CloseCurrentPopup();
						}
					}
				}
				else
				{
					STextValue* V3 = dynamic_cast<STextValue*>(m_EditTextValue->GetFrontValue());
					if (V3)
					{
						xr_string out = m_EditTextValueData;
						if (m_EditTextValue->AfterEdit<STextValue, xr_string>(out))
						{
							if (m_EditTextValue->ApplyValue<STextValue, xr_string>(out))
							{
								xr_delete(m_EditTextValueData);
								Modified();
								ImGui::CloseCurrentPopup();
							}
						}
					}
					else
					{
						R_ASSERT(false);
					}
				}
			}
		}ImGui::SameLine(0);
		if (ImGui::Button("Cancel"))
		{
			xr_delete(m_EditTextValueData);
			ImGui::CloseCurrentPopup();
		} ImGui::SameLine(0);
		if (ImGui::Button("Apply"))
		{
			CTextValue* V1 = dynamic_cast<CTextValue*>(m_EditTextValue->GetFrontValue());
			if (V1)
			{
				xr_string out = m_EditTextValueData;
				if (m_EditTextValue->AfterEdit<CTextValue, xr_string>(out))
				{
					if (m_EditTextValue->ApplyValue<CTextValue, LPCSTR>(out.c_str()))
					{
						Modified();
					}
				}
			}
			else
			{
				RTextValue* V2 = dynamic_cast<RTextValue*>(m_EditTextValue->GetFrontValue());
				if (V2)
				{
					shared_str out = m_EditTextValueData;
					if (m_EditTextValue->AfterEdit<RTextValue, shared_str>(out))
					{
						if (m_EditTextValue->ApplyValue<RTextValue, shared_str>(out))
						{
							Modified();
						}
					}
				}
				else
				{
					STextValue* V3 = dynamic_cast<STextValue*>(m_EditTextValue->GetFrontValue());
					if (V3)
					{
						xr_string out = m_EditTextValueData;
						if (m_EditTextValue->AfterEdit<STextValue, xr_string>(out))
						{
							if (m_EditTextValue->ApplyValue<STextValue, xr_string>(out))
							{
								Modified();
							}
						}
					}
					else
					{
						R_ASSERT(false);
					}
				}
			}
		}ImGui::SameLine(150);

		if (ImGui::Button("Load"))
		{
			xr_string fn;
			if (EFS.GetOpenName(0, "$import$", fn, false, NULL, 2)) {
				xr_string		buf;
				IReader* F = FS.r_open(fn.c_str());
				F->r_stringZ(buf);
				xr_delete(m_EditTextValueData);
				m_EditTextValueData = xr_strdup(buf.c_str());
				m_EditTextValueDataSize = xr_strlen(m_EditTextValueData)+1;
				FS.r_close(F);
			}
		}
		
		ImGui::SameLine(0);
		if (ImGui::Button("Save"))
		{
			xr_string fn;
			if (EFS.GetSaveName("$import$", fn, NULL, 2)) {
				CMemoryWriter F;
				F.w_stringZ(m_EditTextValueData);
				if (!F.save_to(fn.c_str()))
					Log("!Can't save text file:", fn.c_str());
			}
		}
		
		ImGui::SameLine(0);
		if (ImGui::Button("Clear")) { m_EditTextValueData[0] = 0; }
		ImGui::EndGroup();
		if(m_EditTextValueData)
		ImGui::InputTextMultiline("", m_EditTextValueData, m_EditTextValueDataSize, ImVec2(500, 200) ,ImGuiInputTextFlags_CallbackResize, [](ImGuiInputTextCallbackData* data)->int {return reinterpret_cast<UIPropertiesForm*>(data->UserData)->DrawEditText_Callback(data); }, reinterpret_cast<void*>(this));

		
		ImGui::EndPopup();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 5);
	}
}

int UIPropertiesForm::DrawEditText_Callback(ImGuiInputTextCallbackData* data)
{
	m_EditTextValueData =(char*) xr_realloc(m_EditTextValueData, data->BufSize);
	m_EditTextValueDataSize = data->BufSize;
	data->Buf = m_EditTextValueData;
	return 0;
}

void UIPropertiesForm::DrawEditGameType()
{
	if (ImGui::BeginPopupContextItem("EditGameType", 0))
	{
		R_ASSERT(m_EditGameTypeValue);

		bool test = false;
		ImGui::PopStyleVar(3);
		{
			ImGui::BeginGroup();
			{
				bool cheked = m_EditGameTypeChooser.MatchType(eGameIDSingle);
				if (ImGui::Checkbox("Single", &cheked))
				{
					m_EditGameTypeChooser.m_GameType.set(eGameIDSingle, cheked);
				}
			}
			{
				bool cheked = m_EditGameTypeChooser.MatchType(eGameIDDeathmatch);
				if (ImGui::Checkbox("DM", &cheked))
				{
					m_EditGameTypeChooser.m_GameType.set(eGameIDDeathmatch, cheked);
				}
			}
			{
				bool cheked = m_EditGameTypeChooser.MatchType(eGameIDTeamDeathmatch);
				if (ImGui::Checkbox("TDM", &cheked))
				{
					m_EditGameTypeChooser.m_GameType.set(eGameIDTeamDeathmatch, cheked);
				}
			}
			{
				bool cheked = m_EditGameTypeChooser.MatchType(eGameIDArtefactHunt);
				if (ImGui::Checkbox("ArtefactHunt", &cheked))
				{
					m_EditGameTypeChooser.m_GameType.set(eGameIDArtefactHunt, cheked);
				}
			}
			{
				bool cheked = m_EditGameTypeChooser.MatchType(eGameIDCaptureTheArtefact);
				if (ImGui::Checkbox("CTA", &cheked))
				{
					m_EditGameTypeChooser.m_GameType.set(eGameIDCaptureTheArtefact, cheked);
				}
			}
			ImGui::EndGroup(); ImGui::SameLine();
		}
		{
			ImGui::BeginGroup();
			if (ImGui::Button("Ok", ImVec2(ImGui::GetFrameHeight() * 6, 0)))
			{
				if (m_EditGameTypeValue->AfterEdit<GameTypeValue, GameTypeChooser>(m_EditGameTypeChooser))
					if (m_EditGameTypeValue->ApplyValue<GameTypeValue, GameTypeChooser>(m_EditGameTypeChooser))
					{
						Modified();
					}
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::Button("Cancel", ImVec2(ImGui::GetFrameHeight() * 6, 0)))
			{
				m_EditGameTypeValue = nullptr;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndGroup();
		}
		ImGui::EndPopup();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 5);
	}
}


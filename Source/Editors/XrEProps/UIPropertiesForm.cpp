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
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();
		DrawNode(&m_GeneralNode);
		ImGui::Separator();
		ImGui::PopStyleVar();
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

template<typename T>
inline bool DrawNumeric(const char* name,PropItem*item, bool& change, bool read_only)
{
	change = false;
	NumericValue<T>* V = dynamic_cast<NumericValue<T>*>(item->GetFrontValue());
	if (!V)					return false;
	int data =int(*V->value);
	ImGui::Text(name);
	ImGui::NextColumn();
	shared_str str;
	str.printf("##value_%s", name);
	change =  ImGui::InputInt(str.c_str(), &data, read_only ? ImGuiInputTextFlags_ReadOnly : 0);
	ImGui::NextColumn();
	if (int(V->lim_mn) > data)
		data = int(V->lim_mn);
	if (int(V->lim_mx) < data)
		data = int(V->lim_mx);
	T temp = T(data);
	if (change && item->AfterEdit< NumericValue<T>, T>(temp) && !read_only)
	{
		change = item->ApplyValue< NumericValue<T>, T>(temp);
	}
	return true;
}
template<>
inline bool DrawNumeric<float>(const char* name, PropItem* item,bool&change,bool read_only)
{
	change = false;
	NumericValue<float>* V = dynamic_cast<NumericValue<float>*>(item->GetFrontValue());
	if (!V)					return false;
	ImGui::Text(name);
	ImGui::NextColumn();
	shared_str str;
	str.printf("##value_%s", name);
	float temp = *V->value;
	ImGui::SetNextItemWidth(-1);
	change = ImGui::InputFloat(str.c_str(),&temp,0.01,0.1,V->dec, read_only?ImGuiInputTextFlags_ReadOnly:0);
	ImGui::NextColumn();
	if (V->lim_mn > temp)
		temp = V->lim_mn;
	if (V->lim_mx < temp)
		temp = V->lim_mx;
	if (change&&item->AfterEdit< NumericValue<float>, float>(temp)&& !read_only)
	{
		change = item->ApplyValue< NumericValue<float>, float>(temp);
	}

	return true;
}

template <class T>
BOOL TokenGetValue(PropItem* prop, int&result)
{
	TokenValue<T>* V = dynamic_cast<TokenValue<T>*>(prop->GetFrontValue());
	if (!V)					return FALSE;
	result = V->GetValue();
	return TRUE;
}

template <class T>
BOOL TokenOnEdit(PropItem* prop, u32 _new_val, bool& change)
{
	TokenValue<T>* V = dynamic_cast<TokenValue<T>*>(prop->GetFrontValue());
	if (!V)					return FALSE;
	T new_val = _new_val;
	if (prop->AfterEdit<TokenValue<T>, T>(new_val))
		change = prop->ApplyValue<TokenValue<T>, T>(new_val);
	return TRUE;
}
template <class T>
BOOL FlagOnEdit(const char* name, PropItem* prop, bool& change)
{
	FlagValue<_flags<T> >* V = dynamic_cast<FlagValue<_flags<T> >*>(prop->GetFrontValue());
	if (!V)					return FALSE;
	_flags<T> new_val = V->GetValue();
	prop->BeforeEdit<FlagValue<_flags<T> >, _flags<T> >(new_val);
	u32 u = new_val.get();

	shared_str str;
	str.printf("##value_%s", name);
	if (ImGui::CheckboxFlags(str.c_str(), &u, V->mask))
	{
		new_val.assign(u);
		if (prop->AfterEdit<FlagValue<_flags<T> >, _flags<T> >(new_val))
			change = prop->ApplyValue<FlagValue<_flags<T> >, _flags<T> >(new_val);
	}
	return TRUE;
}
void UIPropertiesForm::DrawItem(Node* node)
{
	EPropType type = node->Object->Type();
	switch (type)
	{
	case PROP_NUMERIC:
	{
		bool change = false;
		if (!DrawNumeric<u32>(node->Name.c_str(), node->Object, change, m_Flags.test(plReadOnly)))
			if (!DrawNumeric<float>(node->Name.c_str(), node->Object, change, m_Flags.test(plReadOnly)))
				if (!DrawNumeric<u8>(node->Name.c_str(), node->Object, change, m_Flags.test(plReadOnly)))
					if (!DrawNumeric<s8>(node->Name.c_str(), node->Object, change, m_Flags.test(plReadOnly)))
						if (!DrawNumeric<u16>(node->Name.c_str(), node->Object, change, m_Flags.test(plReadOnly)))
							if (!DrawNumeric<u16>(node->Name.c_str(), node->Object, change, m_Flags.test(plReadOnly)))
								if (!DrawNumeric<s32>(node->Name.c_str(), node->Object, change, m_Flags.test(plReadOnly)))
									R_ASSERT(false);
		if (change)Modified();
	}
	break;
	case PROP_FLAG:
	{
		ImGui::Text(node->Name.c_str());
		ImGui::NextColumn();
		if (m_Flags.test(plReadOnly))
		{
			FlagValueCustom* V = dynamic_cast<FlagValueCustom*>(node->Object->GetFrontValue()); R_ASSERT(V);
			if (V->HaveCaption())	ImGui::Text(node->Object->GetDrawText().c_str());
			else
			{
				bool value = V->m_Flags.is(FlagValueCustom::flInvertedDraw) ? !V->GetValueEx() : V->GetValueEx();
				ImGui::Text(value ? "true" : "false");
			}
		}
		else
		{
			bool change = false;
			if(!FlagOnEdit<u8>(node->Name.c_str(), node->Object, change))
				if (!FlagOnEdit<u16>(node->Name.c_str(), node->Object, change))
					if (!FlagOnEdit<u32>(node->Name.c_str(), node->Object, change))
						R_ASSERT(false);
			if (change)Modified();
		}

		ImGui::NextColumn();
	}
	break;
	case PROP_BOOLEAN:
	{

		BOOLValue* V = dynamic_cast<BOOLValue*>(node->Object); VERIFY(V);
		bool new_val = V->GetValue();
		if (m_Flags.test(plReadOnly))
		{
			ImGui::Text(new_val ? "True" : "False");
		}
		else
		{
			ImGui::SetNextItemWidth(-1);
			if (ImGui::Checkbox(node->Name.c_str(), &new_val))Modified();
			*V->value = new_val;
		}

	}
	break;

	case PROP_CAPTION:
	{
		ImGui::Text(node->Name.c_str());
		ImGui::NextColumn();
		ImGui::Text(node->Object->GetDrawText().c_str());
		ImGui::NextColumn();
	}
	break;
	case PROP_CHOOSE:
	{
		ImGui::Text(node->Name.c_str());

		ImGui::NextColumn();
		ImGui::SetNextItemWidth(-1);
		xr_string text = node->Object->GetDrawText().c_str();
		if (!text[0])text = "<none>";
		if (ImGui::Button(text.c_str()) && !m_Flags.test(plReadOnly))
		{
			PropItem* prop = node->Object;

			ChooseValue* V = dynamic_cast<ChooseValue*>(prop->GetFrontValue()); VERIFY(V);
			shared_str	edit_val = V->GetValue();
			if (!edit_val.size()) 	edit_val = V->m_StartPath;
			prop->BeforeEdit<ChooseValue, shared_str>(edit_val);
			//
			ChooseItemVec			items;
			if (!V->OnChooseFillEvent.empty())
			{
				V->m_Items = &items;
				V->OnChooseFillEvent(V);
			}
			UIChooseForm::SelectItem(V->m_ChooseID, V->subitem, edit_val.c_str(), 0, V->m_FillParam, 0, items.size() ? &items : 0, V->m_ChooseFlags);
			m_EditChooseValue = prop;

		}

		ImGui::NextColumn();
	}
	break;
	case PROP_TOKEN:
	{
		ImGui::Text(node->Name.c_str());
		ImGui::NextColumn();
		if (m_Flags.test(plReadOnly))
		{
			ImGui::Text(node->Object->GetDrawText().c_str());
		}
		else
		{
			const char* InTokens[256];
			int index = 0;
			if (!TokenGetValue<u8>(node->Object, index))
				if (!TokenGetValue<u16>(node->Object, index))
					if (!TokenGetValue<u32>(node->Object, index))
						R_ASSERT(false);

			TokenValueCustom* T = dynamic_cast<TokenValueCustom*>(node->Object->GetFrontValue()); R_ASSERT(T);
			xr_token* token_list = T->token;
			int i = 0;
			for (; token_list[i].name; i++)
			{
				InTokens[i] = token_list[i].name;
			}
			shared_str str;
			str.printf("##value_%s", node->Name.c_str());

			bool change = false;
			ImGui::SetNextItemWidth(-1);
			if(ImGui::Combo(str.c_str(), &index, InTokens, i))
			if (!TokenOnEdit<u8>(node->Object, index, change))
				if (!TokenOnEdit<u16>(node->Object, index, change))
					if (!TokenOnEdit<u32>(node->Object, index, change))
						R_ASSERT(false);
			if (change)Modified();
		}

		ImGui::NextColumn();
	}
	break;

	case PROP_BUTTON:
	{
		ImGui::Text(node->Name.c_str());
		ImGui::NextColumn();
		bool bRes = false;
		bool bSafe = false;
		ButtonValue* V = dynamic_cast<ButtonValue*>(node->Object->GetFrontValue()); R_ASSERT(V);
		if (!V->value.empty())
		{
			ImGui::SetNextItemWidth(-1);
			float dx = floorf(float(ImGui::CalcItemWidth()) / float(V->value.size()));
			V->btn_num = V->value.size();
			for (RStringVecIt it = V->value.begin(); it != V->value.end(); it++)
			{
				int k = it - V->value.begin();
				
				if (ImGui::Button(it->c_str(), ImVec2(dx - 4, 0)))
				{
					V->btn_num = k;

					bRes |= V->OnBtnClick(bSafe);
				}
				ImGui::SameLine();
			}
		}
		else
		{
			ImGui::Text("");
		}
		if (bRes)Modified();
		ImGui::NextColumn();
	}

	break;

	case PROP_CANVAS:
	{
		ImGui::Text(node->Name.c_str());
		ImGui::NextColumn();
		ImGui::SetNextItemWidth(-1);
		CanvasValue* val = dynamic_cast<CanvasValue*>(node->Object->GetFrontValue()); R_ASSERT(val);
		if (!val->OnDrawCanvasEvent.empty())
			val->OnDrawCanvasEvent(val);
		ImGui::NextColumn();
	}

	break;
	case PROP_COLOR:
	{
		ImGui::Text(node->Name.c_str());
		ImGui::NextColumn();
		U32Value* V = dynamic_cast<U32Value*>(node->Object->GetFrontValue()); R_ASSERT(V);
		u32 edit_val = V->GetValue();
		node->Object->BeforeEdit<U32Value, u32>(edit_val);
		u32 a = color_get_A(edit_val);
		shared_str str;
		str.printf("##value_%s", node->Name.c_str());
		float color[3] = { color_get_R(edit_val) / 255.f, color_get_G(edit_val) / 255.f, color_get_B(edit_val) / 255.f };
		ImGui::SetNextItemWidth(-1);
		if (ImGui::ColorEdit3(str.c_str(), color))
		{
			edit_val = color_rgba_f(color[0], color[1], color[2], 1.f);
			edit_val = subst_alpha(edit_val, a);
			if (node->Object->AfterEdit<U32Value, u32>(edit_val))
				if (node->Object->ApplyValue<U32Value, u32>(edit_val)) {
					Modified();
				}
		}
		ImGui::NextColumn();
	}
	break;
	case PROP_SHORTCUT:
	{
		ImGui::Text(node->Name.c_str());
		ImGui::NextColumn();
		ShortcutValue* V = dynamic_cast<ShortcutValue*>(node->Object->GetFrontValue()); R_ASSERT(V);
		if (ImGui::Button(V->GetDrawText(0).c_str(),ImVec2(128,0)))
		{
			UIKeyPressForm::Show();
			m_EditShortcutValue = node->Object;
		}ImGui::SameLine();

		if (ImGui::Button("X", ImVec2(0, 0)))
		{
			xr_shortcut val;
			V->ApplyValue(val);
		}
		ImGui::NextColumn();
	}
	break;
	case PROP_RTOKEN:
	case PROP_SH_TOKEN:
	case PROP_RLIST:
	case PROP_CLIST:
	case PROP_TEXTURE2:
	case PROP_UNDEF:
	case PROP_VECTOR:
	
	case PROP_FCOLOR:
	case PROP_VCOLOR:
	case PROP_RTEXT:
	case PROP_STEXT:
	case PROP_WAVE:
	case PROP_TIME:
	case PROP_CTEXT:

	case PROP_GAMETYPE:
		not_implemented();
		break;
	default:
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


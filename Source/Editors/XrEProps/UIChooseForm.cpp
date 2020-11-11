#include "stdafx.h"
#include "UIChooseForm.h"
UIChooseForm::EventsMap	UIChooseForm::m_Events;
UIChooseForm* UIChooseForm::Form = 0;
xr_string UIChooseForm::m_LastSelection;
ImTextureID   UIChooseForm::NullTexture = nullptr;
void UIChooseForm::DrawItem(Node* Node)
{
    if (m_Filter.PassFilter(Node->Name.c_str()))
    {
        if (Node->Selected)
        {
            ImGui::SetScrollHereY();
            m_SelectedItem = Node->Object;
            UpdateTexture();
            if (!E.on_sel.empty() && !m_Flags.is(cfMultiSelect))
            {
                PropItemVec Vec;
                E.on_sel(m_SelectedItem, Vec);
                m_Props->AssignItems(Vec);
            }
            m_ClickItem = Node->Object;
        }
        ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        for (auto& item : m_SelectedItems)
        {
            if (item == Node->Object)
            {
                Flags |= ImGuiTreeNodeFlags_Bullet;
                break;
            }
        }
        if (m_SelectedItem ==  Node->Object)
        {
            Flags |= ImGuiTreeNodeFlags_Selected;
        }
           
        ImGui::TreeNodeEx(Node->Name.c_str(), Flags);
        if (ImGui::IsItemClicked())
        {
            if ( E.flags.test(SChooseEvents::flClearTexture))
            {
                if (m_Texture)
                    m_Texture->Release();
                m_Texture = 0;
            }
            m_SelectedItem = Node->Object;
            UpdateTexture();
            if (!E.on_sel.empty() && !m_Flags.is(cfMultiSelect))
            {
                PropItemVec Vec;
                E.on_sel(m_SelectedItem, Vec);
                m_Props->AssignItems(Vec);
            }
            if (ImGui::GetIO().KeyCtrl||!m_Flags.is(cfMultiSelect))
                m_ClickItem = Node->Object;
        }
    }

}
bool UIChooseForm::IsDrawFloder(Node*node)
{
    bool result = false;
    for (Node& N : node->Nodes)
    {
        if (N.IsFloder())
        {
            result = result | IsDrawFloder(&N);
        }
        else  if (N.IsObject())
        {
            result = result | m_Filter.PassFilter(N.Name.c_str());
        }
    }
    return result;
}
void UIChooseForm::AppendItem(SChooseItem& item)
{
   Node*node =  AppendObject(&m_GeneralNode, item.name.c_str());
   VERIFY(node);
   node->Object = &item;
}
void UIChooseForm::UpdateTexture()
{
    if (m_SelectedItem == &m_ItemNone)
    {
        if (m_Texture)m_Texture->Release();
        m_Texture = nullptr;
        m_SelectedItems.clear();
    }
    else  if (m_SelectedItem)
    {

        if (m_SelectedItem && E.flags.test(SChooseEvents::flClearTexture))
        {
            if (m_Texture)
                m_Texture->Release();
            m_Texture = 0;
        }
        if (!E.on_get_texture.empty())
            E.on_get_texture(m_SelectedItem->name.c_str(), m_Texture);
    }

}
void UIChooseForm::FillItems(u32 choose_id)
{
    m_ChooseID = choose_id;
    m_GeneralNode = Node();
    u32 ss = m_Items.size();
    ChooseItemVecIt  it = m_Items.begin();
    ChooseItemVecIt  _E = m_Items.end();

    //SPBItem* pb	= UI->ProgressStart(ss,"Fill items..."); //sky to all: no idea how to do it

 //   bool b_check_duplicate = (choose_id != 15);// smSkeletonAnimations is already unique set !!!
    for (; it != _E; it++)
    {
        AppendItem(*it);
    }

    //UI->ProgressEnd(pb);

    if (m_Flags.is(cfAllowNone) && !m_Flags.is(cfMultiSelect))
    {
        m_ItemNone.name = NONE_CAPTION;
        AppendItem(m_ItemNone);
    }
   /* form->tvItems->Sort(true);
    form->tvItems->IsUpdating = false;
    if (m_Flags.is(cfFullExpand))*
        form->tvItems->FullExpand();*/
}

UIChooseForm::UIChooseForm():m_Texture(nullptr),m_ClickItem(nullptr), iSelectedInList(-1), m_SelectedItem(nullptr)
{
    m_Props = xr_new<UIPropertiesForm>();
   // m_Props->AsGroup();
}
UIChooseForm::~UIChooseForm()
{
    if (m_Texture)
        m_Texture->Release();
    if (!E.on_close.empty())
    {
        E.on_close();
    }
    xr_delete(m_Props);
}
void UIChooseForm::Draw()
{
 
    if (!m_LastSelection.empty())
    {
        Node* N = FindObject(&m_GeneralNode, m_LastSelection.c_str());
        if (N)SelectObject(&m_GeneralNode, m_LastSelection.c_str());
        m_LastSelection.clear();
    }
    if (m_SelectedItem&& E.flags.test(SChooseEvents::flAnimated))
    {
        if (!E.on_get_texture.empty())E.on_get_texture(m_SelectedItem->name.c_str(), m_Texture);
    }
    ImGui::Columns(2);
    {
        {
            ImGui::Text("Find:");
            ImGui::SameLine();
            m_Filter.Draw("",-1);
            if (ImGui::BeginChild("Left", ImVec2(0, 0),false,ImGuiWindowFlags_HorizontalScrollbar))
            {
                DrawNode(&m_GeneralNode);
            }
            
            ImGui::EndChild();
        }
        ImGui::NextColumn();
        {
            ImGui::BeginChild("Right", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false);
            {
                if (NullTexture || m_Texture)
                {
                    if (m_Texture)
                    {
                        ImGui::Image(m_Texture, ImVec2(192, 192));
                    }
                    else
                    {
                        ImGui::Image(NullTexture, ImVec2(192, 192));
                    }
                }
                else
                {
                    ImGui::InvisibleButton("Image", ImVec2(192, 192));
                }
                //   ImGui::Image
                ImGui::Separator();
                if (m_SelectedItem == nullptr)
                {
                    ImGui::Text("Name:");
                    ImGui::Text("Hit:");
                }
                else
                {
                    ImGui::Text("Name:%s", m_SelectedItem->name.c_str());
                    ImGui::Text("Hit:%s", m_SelectedItem->hint.c_str());
                }
                ImGui::Separator();
            }
            if (!E.on_sel.empty() && !m_Flags.is(cfMultiSelect))
            {
                if (m_SelectedItem)
                {
                    if (ImGui::BeginChild("Props", ImVec2(0,0)))
                    {
                        m_Props->Draw();
                    }
                    ImGui::EndChild();
                }
            }
            else if (m_Flags.is(cfMultiSelect))
            {
                if (ImGui::BeginChild("List", ImVec2(0,0), true, ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar))
                {
                    int i = 0;

                    int HereY = -iSelectedInList - 2;
                    if (HereY >= 0)iSelectedInList = HereY;
                    for (auto& item : m_SelectedItems)
                    {
                        if (HereY == i)
                            ImGui::SetScrollHereY();
                        if (ImGui::Selectable(item->name.c_str(), iSelectedInList == i))
                        {
                            iSelectedInList = i;
                        }
                        i++;
                    }
                }
                ImGui::EndChild();
                if (ImGui::Button("Up")) { if (iSelectedInList > 0) { std::swap(m_SelectedItems[iSelectedInList - 1], m_SelectedItems[iSelectedInList]); iSelectedInList = -(iSelectedInList - 1) - 2; } } ImGui::SameLine();
                if (ImGui::Button("Down")) { if (m_SelectedItems.size() > 1 && iSelectedInList < m_SelectedItems.size() - 1) { std::swap(m_SelectedItems[iSelectedInList], m_SelectedItems[iSelectedInList + 1]); iSelectedInList = -(iSelectedInList + 1) - 2; } }  ImGui::SameLine();
                if (ImGui::Button("Del")) { if (m_SelectedItems.size() && iSelectedInList >= 0) { m_SelectedItems.erase(m_SelectedItems.begin() + iSelectedInList); iSelectedInList = -1; } } ImGui::SameLine();
                if (ImGui::Button("Clear List")) { m_SelectedItems.clear(); iSelectedInList = -1;/*  if (E.flags.test(SChooseEvents::flClearTexture) ){ if (m_Texture)m_Texture->Release(); m_Texture = 0; } */ImGui::SameLine(); }
            }
           
            ImGui::EndChild();
            if (ImGui::Button("Ok", ImVec2(100, 0)))
            {
                m_Result = R_Ok;
                bOpen = false;
            }
            ImGui::SameLine(0);
            if (ImGui::Button("Cancel", ImVec2(100, 0)))
            {

                m_Result = R_Cancel;
                bOpen = false;
            }
        }
    }
  
      
    
    if (m_ClickItem)
    {
       
        {
           

            if (!m_Flags.is(cfMultiSelect))
            {
                m_SelectedItems.clear();
            }
            else
            {
                for (auto b = m_SelectedItems.begin(), e = m_SelectedItems.end(); b != e; b++)
                {
                    if (*b == m_ClickItem)
                    {
                        iSelectedInList = -1;
                        m_SelectedItems.erase(b);
                        m_ClickItem = nullptr;
                        return;
                    }
                }
            }
            if (m_SelectedItems.size() != iMultiSelLimit)
            {
                m_SelectedItems.push_back(m_ClickItem);
                iSelectedInList = -1;
            }

            m_ClickItem = nullptr;
        }

    }
}

void UIChooseForm::SetNullTexture(ImTextureID Texture)
{
    NullTexture = Texture;
}
void UIChooseForm::Update()
{
    // ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings
    if (Form&& !Form->IsClosed())
    {
        ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
        if (ImGui::BeginPopupModal("ChooseForm", nullptr,0,true))
        {
            Form->Draw();
            ImGui::EndPopup();
        }
    }

}
bool UIChooseForm::IsActive()
{
    return Form;
}

bool UIChooseForm::GetResult(bool& change, shared_str& result)
{
    if (!Form)return false;
    if (!Form->bOpen)
    {
        if (Form->m_Result == R_Ok)
        {
            if (!Form->m_Flags.test(cfMultiSelect))
            {
                VERIFY(Form->GetSelectedItem());
            }
            xr_string reuslt_temp;
            int i = 0;
            for (auto& item : Form->m_SelectedItems)
            {
                if (i != 0)
                {
                    reuslt_temp.append(",");
                }
                reuslt_temp.append(item->name.c_str());
                i++;
            }
            result = reuslt_temp.c_str();
            change = true;
            xr_delete(Form);
            return true;
        }
        change = false;
        xr_delete(Form);
        return true;
   }
      
	return false;
}
bool UIChooseForm::GetResult(bool& change, xr_string& result)
{
    if (!Form->bOpen)
    {
        if (Form->m_Result == R_Ok)
        {
            if (!Form->m_Flags.test(cfMultiSelect))
            {
                VERIFY(Form->GetSelectedItem());
            }
            int i = 0;
            for (auto& item : Form->m_SelectedItems)
            {
                if (i != 0)
                {
                    result.append(",");
                }
                result.append(item->name.c_str());
                i++;
            }
            change = true;
            xr_delete(Form);
            return true;
        }
        change = false;
        xr_delete(Form);
        return true;
    }

    return false;
}
bool UIChooseForm::GetResult(bool& change, xr_vector<xr_string>& result)
{
    if (!Form->bOpen)
    {
        if (Form->m_Result == R_Ok)
        {
            if (!Form->m_Flags.test(cfMultiSelect))
            {
                VERIFY(Form->GetSelectedItem());
            }
            int i = 0;
            for (auto& item : Form->m_SelectedItems)
            {
               
                result.push_back(item->name.c_str());
                i++;
            }
            change = true;
            xr_delete(Form);
            return true;
        }
        change = false;
        xr_delete(Form);
        return true;
    }

    return false;
}
void UIChooseForm::SelectItem(u32 choose_ID, int sel_cnt, LPCSTR init_name, TOnChooseFillItems item_fill, void* fill_param, TOnChooseSelectItem item_select, ChooseItemVec* items, u32 mask)
{
    VERIFY(!Form);

    Form = xr_new<UIChooseForm>();
    Form->m_Flags.assign(mask);
    Form->m_Flags.set(cfMultiSelect, sel_cnt > 1);
    Form->iMultiSelLimit = sel_cnt;

    // init
   
    if(sel_cnt <= 1)
    {
        if (init_name && init_name[0])
            m_LastSelection = init_name;
    }
    //Form->tvItems->Selected = 0;

    // fill items
    if (items) 
    {
        VERIFY2(item_fill.empty(), "ChooseForm: Duplicate source.");
        Form->m_Items = *items;
        Form->E.Set("Select Item", 0, item_select, 0, 0, 0);
    }
    else if (!item_fill.empty()) 
    {
        // custom
        Form->E.Set("Select Item", item_fill, item_select, 0, 0, 0);
    }
    else
    {
        SChooseEvents* e = GetEvents(choose_ID); VERIFY2(e, "Can't find choose event.");
        Form->E = *e;
    }
    // set & fill


    Form->m_Title = Form->E.caption.c_str();
    if (!Form->E.on_fill.empty())
        Form->E.on_fill(Form->m_Items, fill_param);

    Form->FillItems(choose_ID);

    if (sel_cnt > 1)
    {
        int cnt = _GetItemCount(init_name);
        xr_string result;
        for (int i = 0; i < cnt; i++)
        {
            _GetItem(init_name, i, result);
            for (auto& item : Form->m_Items)
            {
                if (item.name.c_str() == result)
                {
                    Form->m_SelectedItems.push_back(&item);
                }
            }
        }
    }
    /*ImGui::GetBack*/
    //.	Form->paItemsCount->Caption		= AnsiString(" Items in list: ")+AnsiString(Form->tvItems->Items->Count);

        // show
   /* bool bRes = (Form->ShowModal() == mrOk);
    dest = 0;
    if (bRes) {
        int item_cnt = _GetItemCount(select_item.c_str(), ',');
        dest = (select_item == NONE_CAPTION) ? 0 : select_item.c_str();
        m_LastSelection = select_item;
        return 						item_cnt ? item_cnt : 1;
    }*/
}

void UIChooseForm::AppendEvents(u32 choose_ID, LPCSTR caption, TOnChooseFillItems on_fill, TOnChooseSelectItem on_sel, TGetTexture on_thm, TOnChooseClose on_close, u32 flags)
{
	EventsMapIt it = m_Events.find(choose_ID); VERIFY(it == m_Events.end());
	m_Events.insert(std::make_pair(choose_ID, SChooseEvents(caption, on_fill, on_sel, on_thm, on_close, flags)));

}

void UIChooseForm::ClearEvents()
{
    NullTexture->Release();
    m_Events.clear();
}

SChooseEvents* UIChooseForm::GetEvents(u32 choose_ID)
{
	EventsMapIt it = m_Events.find(choose_ID);
	if (it != m_Events.end()) {
		return &it->second;
	}
	else return 0;
}

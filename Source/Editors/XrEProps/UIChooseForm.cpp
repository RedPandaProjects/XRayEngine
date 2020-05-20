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
            if (m_SelectedItem && E.flags.test(SChooseEvents::flClearTexture))
            {
                if(m_Texture)
                m_Texture->Release();
                m_Texture = 0;
            }
            if (!E.on_get_texture.empty())E.on_get_texture(Node->Object->name.c_str(), m_Texture);
            m_ClickItem = Node->Object;
        }
        ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        if (m_SelectedItem == Node->Object)
            Flags |= ImGuiTreeNodeFlags_Bullet;
        ImGui::TreeNodeEx(Node->Name.c_str(), Flags);
        if (ImGui::IsItemClicked())
        {
            if (m_SelectedItem && E.flags.test(SChooseEvents::flClearTexture))
            {
                if (m_Texture)
                    m_Texture->Release();
                m_Texture = 0;
            }
            if (!E.on_get_texture.empty())E.on_get_texture(Node->Object->name.c_str(), m_Texture);
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
UIChooseForm::UIChooseForm():m_Texture(nullptr),m_SelectedItem(nullptr),m_ClickItem(nullptr)
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

    {
        {
            if (ImGui::BeginChild("Left", ImVec2(200, 400)))
            {
                DrawNode(&m_GeneralNode);
            }
            ImGui::EndChild();
            ImGui::SameLine();
        }
        {
            ImGui::BeginChild("Right", ImVec2(200, 400), true);
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
            if (!E.on_sel.empty())
            {
                if (m_SelectedItem)
                {
                    if (ImGui::BeginChild("Props"))
                    {
                        m_Props->Draw();
                    }
                    ImGui::EndChild();
                }
            }
            ImGui::EndChild();
        }
        {
            ImGui::SetNextItemWidth(200 - ImGui::CalcTextSize("  Find").x);
            m_Filter.Draw("Find");
            ImGui::SameLine(210);
            if (ImGui::Button("Ok", ImVec2(100, 0)))
            {
                m_Result = R_Ok;
                bOpen= false;
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
        if (m_ClickItem == &m_ItemNone)
        {
            if (m_Texture)m_Texture->Release();
            m_Texture = nullptr;
            m_SelectedItem = nullptr;
        }
        else
        {
            if (!E.on_sel.empty())
            {
                PropItemVec Vec;
                E.on_sel(m_ClickItem, Vec);
                m_Props->AssignItems(Vec);
            }

            m_SelectedItem = m_ClickItem;
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
        if (ImGui::BeginPopupModal("ChooseForm", nullptr, ImGuiWindowFlags_AlwaysAutoResize| ImGuiWindowFlags_NoResize,true))
        {
            Form->Draw();
            ImGui::EndPopup();
        }
    }

}
bool UIChooseForm::GetResult(bool& change, shared_str& result)
{
    if (!Form->bOpen)
    {
        if (Form->m_Result == R_Ok)
        {
            VERIFY(Form->m_SelectedItem);
            result = Form->m_SelectedItem->name;
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
	if (init_name&&init_name[0]) 
        m_LastSelection = init_name;
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

#pragma once
class XREPROPS_API UIChooseForm :
    public XrUI,private FolderHelper<SChooseItem>
{
    virtual void DrawItem(Node* Node);
    virtual bool IsDrawFloder(Node* Node);
    void AppendItem(SChooseItem& item);
    void UpdateTexture();
    Node m_GeneralNode;
    ImTextureID m_Texture;
    ImGuiTextFilter m_Filter;
private:
    int 			iMultiSelLimit;
    int             iSelectedInList;
    UIPropertiesForm* m_Props;
    Flags32  m_Flags;
    xr_string m_Title;
    xr_vector<SChooseItem*>m_SelectedItems;

    inline SChooseItem* GetSelectedItem() { if (m_SelectedItems.size()) { return m_SelectedItems.back(); }return nullptr; }
    SChooseItem* m_ClickItem;
    SChooseItem* m_SelectedItem;
    SChooseItem m_ItemNone;
    ChooseItemVec	m_Items;
    u32 m_ChooseID;
    void  FillItems(u32 choose_id);
protected:

    DEFINE_MAP(u32, SChooseEvents, EventsMap, EventsMapIt);
    static EventsMap			m_Events;
private:
    static xr_string 			m_LastSelection;
    SChooseEvents 	E;
    static UIChooseForm* Form;
    static ImTextureID NullTexture;

public:
    enum Result
    {
        R_Ok,
        R_Cancel
    };
private:
        Result m_Result;
public:
    UIChooseForm();
    virtual ~UIChooseForm();
    virtual void Draw();
    static void SetNullTexture(ImTextureID Texture);
    static void Update();
    static bool IsActive();
    static bool GetResult(bool&change,shared_str&result);
    static bool GetResult(bool& change, xr_string& result);
    static bool GetResult(bool& change, xr_vector<xr_string>& result);
    static void SelectItem(u32 choose_ID,  int sel_cnt = 1, LPCSTR init_name = 0, TOnChooseFillItems item_fill = 0, void* fill_param = 0, TOnChooseSelectItem item_select = 0, ChooseItemVec* items = 0, u32 flags = cfAllowNone);
    static void AppendEvents(u32 choose_ID, LPCSTR caption, TOnChooseFillItems on_fill, TOnChooseSelectItem on_sel, TGetTexture on_thm, TOnChooseClose on_close, u32 flags);
    static void ClearEvents();
    static SChooseEvents* GetEvents(u32 choose_ID);
};


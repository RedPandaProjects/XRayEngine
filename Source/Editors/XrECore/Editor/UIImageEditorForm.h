//---------------------------------------------------------------------------

#ifndef ImageLibH
#define ImageLibH
class UIImageEditorForm :public XrUI
{
public:
	UIImageEditorForm();
	virtual ~UIImageEditorForm();
	virtual void Draw();
public:
	static void Update();
	static void Show(bool bImport);
	static void ImportTextures();
private:
	DEFINE_VECTOR(ETextureThumbnail*, THMVec, THMIt);
	DEFINE_MAP(shared_str, ETextureThumbnail*, THMMap, THMMapIt);
	THMMap m_THM_Used;
	THMVec m_THM_Current;
	UIItemListForm* m_ItemList;
	UIPropertiesForm* m_ItemProps;
	FS_FileSet	texture_map;
	FS_FileSet	modif_map;
	bool bImportMode;
	bool bReadonlyMode;
	static UIImageEditorForm* Form;
	ImTextureID m_Texture;
	ImTextureID m_TextureRemove;
private:
	ETextureThumbnail* FindUsedTHM(const shared_str& name);
	void RegisterModifiedTHM();
	void OnCubeMapBtnClick(ButtonValue* value, bool& bModif, bool& bSafe);
	void OnTypeChange(PropValue* prop);
	void InitItemList();
	void HideLib();
	void UpdateLib();
	void OnItemsFocused(ListItem* item);
	void SaveUsedTHM();
private:
	bool m_bFilterImage;
	bool m_bFilterTerrain;
	bool m_bFilterBump;
	bool m_bFilterNormal;
	bool m_bFilterCube;
	void FilterUpdate();
};
//---------------------------------------------------------------------------
/*#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>

#include "ElXPThemedControl.hpp"
#include "ExtBtn.hpp"
#include "mxPlacemnt.hpp"
#include <ImgList.hpp>

#include "MXCtrls.hpp"

//---------------------------------------------------------------------------
// refs
class ETextureThumbnail;
class TProperties;
//---------------------------------------------------------------------------

class TfrmImageLib : public TForm
{
__published:	// IDE-managed Components
	TPanel *paRight;
	TFormStorage *fsStorage;
	TPanel *paCommand;
	TExtBtn *ebOk;
	TBevel *Bevel1;
	TPanel *paProperties;
	TPanel *paItems;
	TSplitter *Splitter1;
	TBevel *Bevel2;
	TImageList *ImageList;
	TExtBtn *ebCancel;
	TExtBtn *ebRemoveTexture;
	TBevel *Bevel5;
	TPanel *Panel1;
	TMxPanel *paImage;
	TPanel *paFilter;
	TExtBtn *ttImage;
	TExtBtn *ttBumpMap;
	TExtBtn *ttNormalMap;
	TExtBtn *ttCubeMap;
	TExtBtn *ttTerrain;
	TPanel *Panel2;
	TExtBtn *btFilter;
	TExtBtn *ExtBtn1;
    void  ebOkClick(TObject *Sender);
    void  FormShow(TObject *Sender);
    void  FormClose(TObject *Sender, TCloseAction &Action);
    void  FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void  fsStorageRestorePlacement(TObject *Sender);
	void  fsStorageSavePlacement(TObject *Sender);
	void  FormCreate(TObject *Sender);
	void  ebCancelClick(TObject *Sender);
	void  ebRemoveTextureClick(TObject *Sender);
	void  FormDestroy(TObject *Sender);
	void  paImagePaint(TObject *Sender);
	void  ttImageClick(TObject *Sender);
	void  btFilterClick(TObject *Sender);
	void  ExtBtn1Click(TObject *Sender);
private:
// list functions
    void  		RemoveTexture		(LPCSTR fname, EItemType type, bool& res);

    void 				InitItemsList		();
	void   	OnItemsFocused		(ListItemsVec& items);

    void   	OnCubeMapBtnClick	(ButtonValue* value, bool& bModif, bool& bSafe);
    
	enum{
    	flUpdateProperties = (1<<0),
    };    
    static Flags32		m_Flags;
private:	// User declarations
	static TfrmImageLib* form;

    DEFINE_VECTOR		(ETextureThumbnail*,THMVec,THMIt);
    DEFINE_MAP			(shared_str,ETextureThumbnail*,THMMap,THMMapIt);
    THMMap				m_THM_Used;
    THMVec				m_THM_Current;
    TItemList*			m_ItemList;
    TProperties* 		m_ItemProps;

    ETextureThumbnail*	FindUsedTHM			(const shared_str& name);
    void				SaveUsedTHM			();
    void				DestroyUsedTHM		();

	void  	RegisterModifiedTHM	();
    
    void 				OnModified			();
    static FS_FileSet	texture_map;
    static FS_FileSet	modif_map;
    bool 				bImportMode;
    bool 				bReadonlyMode;
    void  	UpdateLib			();
    static bool 		bFormLocked;
    static void 		LockForm			(){ bFormLocked = true;	form->paProperties->Enabled = false; 	form->paItems->Enabled = false; }
    static void 		UnlockForm			(){ bFormLocked = false;form->paProperties->Enabled = true; 	form->paItems->Enabled = true; 	}

    void 		OnTypeChange		(PropValue* prop);
    void 				SortList			(ETextureThumbnail* thm, xr_vector<xr_string>& sel_str_vec);
    void bool  HideLib			();
public:		// User declarations
     			TfrmImageLib		(TComponent* Owner);
// static function
    static void  ImportTextures	();
    static void  EditLib			(xr_string& title, bool bImport=false);
    static bool  Visible			(){return !!form;}
    static void 		OnFrame				();
    static void			UpdateProperties	(){m_Flags.set(flUpdateProperties,TRUE);}
};
//---------------------------------------------------------------------------*/
#endif

//----------------------------------------------------
// file: Library.h
//----------------------------------------------------

#ifndef LibraryH
#define LibraryH

//----------------------------------------------------
class CEditableObject;

DEFINE_MAP_PRED(xr_string,CEditableObject*,EditObjMap,EditObjPairIt,astr_pred);
//----------------------------------------------------
class ECORE_API XRayObjectLibrary//:	public pureDeviceCreate, public pureDeviceDestroy
{
	bool				m_bReady;
	EditObjMap			m_EditObjects;

    CEditableObject*	LoadEditObject		(LPCSTR full_name);
    void				UnloadEditObject	(LPCSTR full_name);
public:
						XRayObjectLibrary			();
	virtual 			~XRayObjectLibrary			();
                    
    void  		RemoveObject		(LPCSTR fname, EItemType type, bool& res);
    void  		RenameObject		(LPCSTR fn0, LPCSTR fn1, EItemType type);

	void 				OnCreate			();
	void 				OnDestroy			();
	void 				Save				(FS_FileSet* modif_map=0);
    
    void 				ReloadObjects		();
    void 				CleanLibrary		();
    void 				ReloadObject		(LPCSTR name);

    CEditableObject*	CreateEditObject	(LPCSTR name);
    void				RemoveEditObject	(CEditableObject*& object);

    int					GetObjects			(FS_FileSet& files);
    int					ObjectCount			(){return m_EditObjects.size();}

    void				EvictObjects		();

	virtual		void	OnDeviceCreate		();
	virtual		void	OnDeviceDestroy		();
public:
    float               AngleSmooth      = 75;
    EGame               LoadAsGame       = EGame::NONE;
};

extern ECORE_API XRayObjectLibrary* GXRayObjectLibrary;
//----------------------------------------------------
#endif /*_INCDEF_Library_H_*/


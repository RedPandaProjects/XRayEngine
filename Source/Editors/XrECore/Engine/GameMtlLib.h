#pragma once
#include "../../../XrEngine/XrGameMaterialLibraryInterface.h"
class ECORE_API CGameMtlLibrary:public XrGameMaterialLibraryInterface
{
	int					material_index;
	int					material_pair_index;
	BENCH_SEC_SCRAMBLEMEMBER1

	GameMtlVec			materials;
    GameMtlPairVec		material_pairs;

    u32					material_count;
    GameMtlPairVec		material_pairs_rt;
public:
	CGameMtlLibrary		();
	virtual ~CGameMtlLibrary();
	void				Unload			() override;
    // material routine
	GameMtlIt 		GetMaterialIt	(LPCSTR name) 
    {
        for (GameMtlIt it=materials.begin(); materials.end() != it; ++it)
            if (0==strcmpi(*(*it)->m_Name,name)) return it;
        return materials.end();
    }
	GameMtlIt 		GetMaterialIt	(shared_str& name) 
    {
        for (GameMtlIt it=materials.begin(); materials.end() != it; ++it)
            if (name.equal((*it)->m_Name)) return it;
        return materials.end();
    }
	GameMtlIt 		GetMaterialItByID(int id) 
    {
        for (GameMtlIt it=materials.begin(); materials.end() != it; ++it)
            if ((*it)->ID==id) return it;
        return materials.end();
    }
	u32				GetMaterialID	(LPCSTR name) override
    {
    	GameMtlIt it	= GetMaterialIt	(name);
        return (it==materials.end())?GAMEMTL_NONE_ID:(*it)->ID;
    }
	// game
	SGameMtl*		GetMaterialByID (s32 id)		override {return GetMaterialByIdx(GetMaterialIdx(id));}
	u16				GetMaterialIdx	(int ID)		override {GameMtlIt it=GetMaterialItByID(ID);VERIFY(materials.end() != it); return (u16)(it-materials.begin());}
	u16				GetMaterialIdx	(LPCSTR name)	override {GameMtlIt it=GetMaterialIt(name);VERIFY(materials.end() != it); return (u16)(it-materials.begin());}
	SGameMtl*		GetMaterialByIdx(u16 idx)		override {VERIFY(idx<(u16)materials.size()); return materials[idx];}


	GameMtlIt		FirstMaterial	(){return materials.begin();}
	GameMtlIt		LastMaterial	(){return materials.end();}
	u32				CountMaterial	() override {return materials.size();}

	// game
	SGameMtlPair*	GetMaterialPair	 (u16 idx0, u16 idx1) override {R_ASSERT((idx0<material_count)&&(idx1<material_count)); return material_pairs_rt[idx1*material_count+idx0];}

	GameMtlPairIt	FirstMaterialPair	()   {return material_pairs.begin();}
	GameMtlPairIt	LastMaterialPair	() {return material_pairs.end();}

	// IO routines
	void				Load			() override;
	void				Load			(const char*FileName) ;
	bool				Save			()override { R_ASSERT(false); return false; }
};





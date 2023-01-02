#include "stdafx.h"
#include "GameMtlLib.h"
CGameMtlLibrary::CGameMtlLibrary()
{
    material_index = 0;
    material_pair_index = 0;
    material_count = 0;
}

CGameMtlLibrary::~CGameMtlLibrary()
{
}


void CGameMtlLibrary::Unload()
{
	material_count = 0;
	material_pairs_rt.clear();
	for (GameMtlIt m_it = materials.begin(); materials.end() != m_it; ++m_it)
		xr_delete(*m_it);
	materials.clear();
	for (GameMtlPairIt p_it = material_pairs.begin(); material_pairs.end() != p_it; ++p_it)
		xr_delete(*p_it);
	material_pairs.clear();
}

void CGameMtlLibrary::Load()
{
	string_path			name;
	if (!FS.exist(name,	_game_data_,GAMEMTL_FILENAME)){
    	Log				("! Can't find game material file: ",name);
    	return;
    }

    Load(name);
	
}

void CGameMtlLibrary::Load(const char* FileName)
{
	R_ASSERT(material_pairs.empty());
	R_ASSERT(materials.empty());
    IReader* F = FS.r_open(FileName);
    IReader& fs = *F;

    R_ASSERT(fs.find_chunk(GAMEMTLS_CHUNK_VERSION));
    u16 version = fs.r_u16();
    if (GAMEMTL_CURRENT_VERSION != version) {
        Log("CGameMtlLibrary: invalid version. Library can't load.");
        FS.r_close(F);
        return;
    }

    R_ASSERT(fs.find_chunk(GAMEMTLS_CHUNK_AUTOINC));
    material_index = fs.r_u32();
    material_pair_index = fs.r_u32();

    materials.clear();
    material_pairs.clear();

    IReader* OBJ = fs.open_chunk(GAMEMTLS_CHUNK_MTLS);
    if (OBJ) {
        u32				count;
        for (IReader* O = OBJ->open_chunk_iterator(count); O; O = OBJ->open_chunk_iterator(count, O)) {
            SGameMtl* M = xr_new<SGameMtl>();
            M->Load(*O);
            materials.push_back(M);
        }
        OBJ->close();
    }

    OBJ = fs.open_chunk(GAMEMTLS_CHUNK_MTLS_PAIR);
    if (OBJ) {
        u32				count;
        for (IReader* O = OBJ->open_chunk_iterator(count); O; O = OBJ->open_chunk_iterator(count, O)) {
            SGameMtlPair* M = xr_new<SGameMtlPair>(this);
            M->Load(*O);
            material_pairs.push_back(M);
        }
        OBJ->close();
    }

#ifndef _EDITOR
    material_count = (u32)materials.size();
    material_pairs_rt.resize(material_count * material_count, 0);
    for (GameMtlPairIt p_it = material_pairs.begin(); material_pairs.end() != p_it; ++p_it) {
        SGameMtlPair* S = *p_it;
        int idx0 = GetMaterialIdx(S->mtl0) * material_count + GetMaterialIdx(S->mtl1);
        int idx1 = GetMaterialIdx(S->mtl1) * material_count + GetMaterialIdx(S->mtl0);
        material_pairs_rt[idx0] = S;
        material_pairs_rt[idx1] = S;
    }
#endif

    /*
        for (GameMtlPairIt p_it=material_pairs.begin(); material_pairs.end() != p_it; ++p_it){
            SGameMtlPair* S	= *p_it;
            for (int k=0; k<S->StepSounds.size(); k++){
                Msg("%40s - 0x%x", S->StepSounds[k].handle->file_name(), S->StepSounds[k].g_type);
            }
        }
    */
    FS.r_close(F);
}


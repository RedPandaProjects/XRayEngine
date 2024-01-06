#include "stdafx.h"
#include "XrGameMaterialLibraryInterface.h"

#include "Interfaces/Core/RBMKEngine.h"

SGameMtl::SGameMtl()
{
    ID = -1;
    m_Name = "unknown";
    Flags.zero();
    // factors
    fFlotationFactor = 1.f;
    fShootFactor = 0.f;
    fShootFactorMP = 0.f;
    fBounceDamageFactor = 1.f;
    fInjuriousSpeed = 0.f;
    fVisTransparencyFactor = 0.f;
    fSndOcclusionFactor = 0.f;
    // physics
    fPHFriction = 1.f;
    fPHDamping = 1.f;
    fPHSpring = 1.f;
    fPHBounceStartVelocity = 0.f;
    fPHBouncing = 0.1f;
    fDensityFactor = 0.0f;
}

SGameMtl::~SGameMtl()
{
}

static void DestroySounds(xr_vector<IRBMKSoundSource*>& Sources)
{
	for(IRBMKSoundSource*Source:Sources)
	{
	  g_Engine->GetSoundManager()->Destroy(Source);
	}
	Sources.clear();
}


void CreateSounds(xr_vector<shared_str>& lst, LPCSTR buf)
{
    string128 tmp;
    int cnt = _GetItemCount(buf);	R_ASSERT(cnt <= GAMEMTL_SUBITEM_COUNT + 2);
    lst.resize(cnt);
    for (int k = 0; k < cnt; ++k)
        lst[k] = shared_str(_GetItem(buf, k, tmp));
}

void CreateMarks(xr_vector<shared_str>&Out, LPCSTR buf)
{
    string256	tmp;
    int cnt = _GetItemCount(buf);	R_ASSERT(cnt <= GAMEMTL_SUBITEM_COUNT);
    for (int k = 0; k < cnt; ++k)
      Out.emplace_back(_GetItem(buf, k, tmp));
}


void CreatePSs(PSVec& lst, LPCSTR buf)
{
    string256 tmp;
    int cnt = _GetItemCount(buf);	R_ASSERT(cnt <= GAMEMTL_SUBITEM_COUNT);
    for (int k = 0; k < cnt; ++k)
        lst.push_back(_GetItem(buf, k, tmp));
}

SGameMtlPair::~SGameMtlPair()
{
    // destroy all media
}


void SGameMtlPair::Load(IReader& fs)
{
    shared_str				buf;

    R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_PAIR));
    mtl0 = fs.r_u32();
    mtl1 = fs.r_u32();
    ID = fs.r_u32();
    ID_parent = fs.r_u32();
    OwnProps.assign(fs.r_u32());

    R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_BREAKING));
    fs.r_stringZ(buf); 		CreateSounds(BreakingSoundsNames, *buf);

    R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_STEP));
    fs.r_stringZ(buf);		CreateSounds(StepSoundsNames, *buf);

    R_ASSERT(fs.find_chunk(GAMEMTLPAIR_CHUNK_COLLIDE));
    fs.r_stringZ(buf);		CreateSounds(CollideSoundsNames, *buf);
    fs.r_stringZ(buf);		CreatePSs(CollideParticles, *buf);
    fs.r_stringZ(buf);
    CreateMarks(CollideMarksLegacy, *buf);
}

void SGameMtlPair::Save(IWriter& fs)
{
    R_ASSERT(0);
}
void SGameMtl::Load(IReader& fs)
{
    R_ASSERT(fs.find_chunk(GAMEMTL_CHUNK_MAIN));
    ID = fs.r_u32();
    fs.r_stringZ(m_Name);

    if (fs.find_chunk(GAMEMTL_CHUNK_DESC)) {
        fs.r_stringZ(m_Desc);
    }

    R_ASSERT(fs.find_chunk(GAMEMTL_CHUNK_FLAGS));
    Flags.assign(fs.r_u32());

    R_ASSERT(fs.find_chunk(GAMEMTL_CHUNK_PHYSICS));
    fPHFriction = fs.r_float();
    fPHDamping = fs.r_float();
    fPHSpring = fs.r_float();
    fPHBounceStartVelocity = fs.r_float();
    fPHBouncing = fs.r_float();

    R_ASSERT(fs.find_chunk(GAMEMTL_CHUNK_FACTORS));
    fShootFactor = fs.r_float();
    fBounceDamageFactor = fs.r_float();
    fVisTransparencyFactor = fs.r_float();
    fSndOcclusionFactor = fs.r_float();


    if (fs.find_chunk(GAMEMTL_CHUNK_FACTORS_MP))
        fShootFactorMP = fs.r_float();
    else
        fShootFactorMP = fShootFactor;

    if (fs.find_chunk(GAMEMTL_CHUNK_FLOTATION))
        fFlotationFactor = fs.r_float();

    if (fs.find_chunk(GAMEMTL_CHUNK_INJURIOUS))
        fInjuriousSpeed = fs.r_float();

    if (fs.find_chunk(GAMEMTL_CHUNK_DENSITY))
        fDensityFactor = fs.r_float();
}

void SGameMtl::Save(IWriter& fs)
{
    R_ASSERT(0);
}

XrGameMaterialLibraryInterface::XrGameMaterialLibraryInterface()
{
}

XrGameMaterialLibraryInterface::~XrGameMaterialLibraryInterface()
{
}
#ifdef DEBUG
LPCSTR SGameMtlPair::dbg_Name()
{
    static string256 nm;
    SGameMtl* M0 = GameMaterialLibrary->GetMaterialByID(GetMtl0());
    SGameMtl* M1 = GameMaterialLibrary->GetMaterialByID(GetMtl1());
    xr_sprintf(nm, sizeof(nm), "Pair: %s - %s", *M0->m_Name, *M1->m_Name);
    return nm;
}
#endif

SGameMtlPair::SGameMtlPair(XrGameMaterialLibraryInterface* owner)
{

    mtl0 = -1;
    mtl1 = -1;
    ID = -1;
    ID_parent = -1;
    m_Owner = owner;
    OwnProps.one();
}

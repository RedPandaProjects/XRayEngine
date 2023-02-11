////////////////////////////////////////////////////////////////////////////
//	Module 		: xrSE_Factory.cpp
//	Created 	: 18.06.2004
//  Modified 	: 18.06.2004
//	Author		: Dmitriy Iassenev
//	Description : Precompiled header creatore
////////////////////////////////////////////////////////////////////////////

#include "pch_script.h"
#include "xrSE_Factory.h"
#include "ai_space.h"
#include "script_engine.h"
#include "object_factory.h"
#include "..\Editors\Public\xrEProps.h"
#include "xrSE_Factory_import_export.h"
#include "script_properties_list_helper.h"

#include "character_info.h"
#include "specific_character.h"
//#include "character_community.h"
//#include "monster_community.h"
//#include "character_rank.h"
//#include "character_reputation.h"

#include <lua/library_linkage.h>
#include "xrServer_Objects_ALife.h"


extern CSE_Abstract *F_entity_Create	(LPCSTR section);

extern CScriptPropertiesListHelper	*g_property_list_helper;


extern SFillPropData			fp_data;
void setup_luabind_allocator();
XRaySEFactory GSEFactory;
extern "C" 
{
	FACTORY_API	XRaySEFactoryInterface* __cdecl GetSEFactoryInterface()
	{
		return &GSEFactory;
	}

}
//typedef void DUMMY_STUFF (const void*,const u32&,void*);
//XRCORE_API DUMMY_STUFF	*g_temporary_stuff;


//#define TRIVIAL_ENCRYPTOR_DECODER
//#include UP(xrEngine/trivial_encryptor.h)


void _destroy_item_data_vector_cont(T_VECTOR* vec)
{
	T_VECTOR::iterator it		= vec->begin();
	T_VECTOR::iterator it_e		= vec->end();

	xr_vector<CUIXml*>			_tmp;	
	for(;it!=it_e;++it)
	{
		xr_vector<CUIXml*>::iterator it_f = std::find(_tmp.begin(), _tmp.end(), (*it)._xml);
		if(it_f==_tmp.end())
			_tmp.push_back	((*it)._xml);
	}
	delete_data	(_tmp);
}

XRaySEFactory::XRaySEFactory()
{

}

XRaySEFactory::~XRaySEFactory()
{

}

void XRaySEFactory::Create()
{
	string_path					SYSTEM_LTX;
	FS.update_path(SYSTEM_LTX, "$game_config$", "system.ltx");
	pSettings = xr_new<CInifile>(SYSTEM_LTX);

	setup_luabind_allocator();

	CCharacterInfo::InitInternal();
	CSpecificCharacter::InitInternal();
	fp_data.inc();
}

void XRaySEFactory::Destroy()
{
	fp_data.dec();
	R_ASSERT(fp_data.counter == 0);
	CCharacterInfo::DeleteSharedData();
	CCharacterInfo::DeleteIdToIndexData();
	CSpecificCharacter::DeleteSharedData();
	CSpecificCharacter::DeleteIdToIndexData();


	xr_delete(g_object_factory);
	CInifile** s = (CInifile**)(&pSettings);
	xr_delete(*s);
	xr_delete(g_property_list_helper);
	xr_delete(g_ai_space);
	xr_delete(g_object_factory);
}

class ISE_Abstract* XRaySEFactory::CreateEntity(const char* SectionName)
{
	return					(F_entity_Create(SectionName));
}

void XRaySEFactory::DestroyEntity(ISE_Abstract* Entity)
{
	CSE_Abstract* object = smart_cast<CSE_Abstract*>(Entity);
	F_entity_Destroy(object);
}

const RTokenVec& XRaySEFactory::GetLocations(int ID)
{
	return fp_data.locations[ID];
}

const RStringVec& XRaySEFactory::GetLevelIDS()
{
	return fp_data.level_ids;
}

const RTokenVec& XRaySEFactory::GetStoryNames()
{
	return fp_data.story_names;
}

const RTokenVec& XRaySEFactory::GetSpawnStoryNames()
{
	return fp_data.spawn_story_names;
}

const RStringVec& XRaySEFactory::GetCharacterProfiles()
{
	return fp_data.character_profiles;
}

const RStringVec& XRaySEFactory::GetSmartCovers()
{
	return fp_data.smart_covers;
}

const xr_map<shared_str, u32>& XRaySEFactory::GetLocationColors()
{
	return fp_data.location_colors;
}

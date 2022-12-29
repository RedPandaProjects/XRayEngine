#include "stdafx.h"
#pragma hdrstop

#include "EditObject.h"
#include "EditMesh.h"
#include "../Public/PropertiesListHelper.h"
#include "motion.h"
#include "bone.h"

void CEditableObject::OnChangeShader(PropValue*)
{
    OnDeviceDestroy	();
}
//---------------------------------------------------------------------------

void CEditableObject::FillSurfaceProps(CSurface* SURF, LPCSTR pref, PropItemVec& items)
{
   
}
//---------------------------------------------------------------------------

xr_token ECORE_API eo_type_token[]={
	{ "Static",					0},
	{ "Dynamic",				CEditableObject::eoDynamic},
	{ "Progressive Dynamic",	CEditableObject::eoDynamic|CEditableObject::eoProgressive},
	{ "HOM",					CEditableObject::eoHOM},
	{ "Multiple Usage",			CEditableObject::eoMultipleUsage|CEditableObject::eoUsingLOD},
	{ "Sound Occluder",			CEditableObject::eoSoundOccluder},
	{ 0,						0}
};

void CEditableObject::FillBasicProps(LPCSTR pref, PropItemVec& items)
{
   
}
//---------------------------------------------------------------------------

void CEditableObject::FillSummaryProps(LPCSTR pref, PropItemVec& items)
{
   
}
//---------------------------------------------------------------------------

ECORE_API xr_string MakeFullBoneName(CBone* bone)
{
	if (bone->Parent()){
    	return MakeFullBoneName(bone->Parent())+"\\"+bone->Name().c_str();
    }else{
    	return bone->Name().c_str();
    }
}

xr_string MakeFullBonePath(CBone* bone)
{
	if (bone->Parent()){
	   	return MakeFullBoneName(bone->Parent());
	}else{
    	return "";
	}
}

void CEditableObject::FillSurfaceList(LPCSTR pref, ListItemsVec& items, int modeID)
{
  
}
//---------------------------------------------------------------------------

void CEditableObject::FillBoneList(LPCSTR pref, ListItemsVec& items, int modeID)
{
  
}

void CEditableObject::FillMotionList(LPCSTR pref, ListItemsVec& items, int modeID)
{

}



 
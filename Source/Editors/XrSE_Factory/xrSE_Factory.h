////////////////////////////////////////////////////////////////////////////
//	Module 		: xrSE_Factory.h
//	Created 	: 18.06.2004
//  Modified 	: 18.06.2004
//	Author		: Dmitriy Iassenev
//	Description : Precompiled header creatore
////////////////////////////////////////////////////////////////////////////
#pragma once
#include "../XrEngine/XRaySEFactoryInterface.h"

class XRaySEFactory:public XRaySEFactoryInterface
{

public:
									XRaySEFactory			();
									~XRaySEFactory			();
	void							Create					() override;
	void							Destroy					() override;
	class ISE_Abstract*				CreateEntity			(const char* SectionName) override;
	void							DestroyEntity			(ISE_Abstract* Entity) override;
	const RTokenVec&				GetLocations			(int ID) override;
	const RStringVec&				GetLevelIDS				() override;
	const RTokenVec&				GetStoryNames			() override;
	const RTokenVec&				GetSpawnStoryNames		() override;
	const RStringVec&				GetCharacterProfiles	() override;
	const RStringVec&				GetSmartCovers			() override;
	const xr_map<shared_str, u32>&	GetLocationColors		() override;

};
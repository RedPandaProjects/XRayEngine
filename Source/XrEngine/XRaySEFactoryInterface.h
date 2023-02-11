#pragma once
class ENGINE_API XRaySEFactoryInterface
{
public:
											XRaySEFactoryInterface	();
	virtual									~XRaySEFactoryInterface	();
	virtual	void							Create					() = 0;
	virtual	void							Destroy					() = 0;
	virtual class ISE_Abstract*				CreateEntity			(const char* SectionName) = 0;
	virtual void							DestroyEntity			(ISE_Abstract*Entity) = 0;
	virtual const RTokenVec&				GetLocations			(int ID) = 0;
	virtual const RStringVec&				GetLevelIDS				() = 0;
	virtual const RTokenVec&				GetStoryNames			() = 0;
	virtual const RTokenVec&				GetSpawnStoryNames		() = 0;
	virtual const RStringVec&				GetCharacterProfiles	() = 0;
	virtual const RStringVec&				GetSmartCovers			() = 0;
	virtual const xr_map<shared_str, u32>&	GetLocationColors		() = 0;
};
#pragma once
#include "XRayLevelToBlueprint.h"
enum class EXRayWorldStatus
{
	None,
	Ready,
	Loading,
	Failure
};
class ENGINE_API XRayEngineInterface	:public IEventReceiver
{
public:
											XRayEngineInterface			();
	virtual									~XRayEngineInterface		();

	virtual void							Initialize					();
	virtual void							Destroy						();
	 
	virtual	void							OnEvent						(EVENT E, u64 P1, u64 P2);
	virtual void							OnFrame						();

	virtual void							RunGame						(const char* ServerParams = "unreal/single/alife/new" ,const char*ClientParams = "localhost");
	virtual void							StopGame					();
	virtual void							LoadDefaultWorld			() = 0;
	virtual bool							IsRunningGame				();
	shared_str								GetGameVersion				();
	virtual shared_str						GetUnrealVersion			() = 0;
	virtual class XRayUnrealProxyInterface*	CreateUnrealProxy			() = 0;
	virtual class XRayUnrealProxyInterface* GetUnrealPlayerCharacter	() = 0;
	virtual void							Destroy						(class XRayUnrealProxyInterface*) = 0;
	virtual void							LoadCFormFormCurrentWorld	(class CObjectSpace&ObjectSpace,CDB::build_callback build_callback)= 0 ;
	virtual class ILevelGraph*				GetLevelGraph				(const char* Name) = 0;
	virtual class IGameGraph*				GetGameGraph				() = 0;
	virtual IReader							GetGameSpawn				() = 0;
	virtual bool							LoadWorld					(const char*Name) = 0;
	virtual EXRayWorldStatus				GetWorldStatus				() = 0;
			XRayLevelToBlueprint*			GetLevelScript				();
private:
	void									InitEngine					();
	void									InitSettings				();
	void									InitInput					();
	void									InitConsole					();
	void									InitSound1					();
	void									InitSound2					();

	void									execUserScript				();

	void									destroySettings				();
	void									destroyConsole				();
	void									destroySound				();
	void									destroyEngine				();

private:
	EVENT									eQuit;
	EVENT									eStart;
	EVENT									eStartLoad;
	EVENT									eDisconnect;
	EVENT									eConsole;
	EVENT									eStartMPDemo;
};

extern ENGINE_API	XRayEngineInterface*	g_Engine;


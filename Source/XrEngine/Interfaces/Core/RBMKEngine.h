#pragma once
#include "RBMKLevelToBlueprint.h"
#include "../Sound/RBMKSoundManager.h"
#include "../Environment/RBMKEnvironment.h"
enum class ERBMKWorldStatus
{
	None,
	Ready,
	Loading,
	Failure
};
class ENGINE_API IRBMKEngine	:public IEventReceiver
{
public:
											IRBMKEngine			();
	virtual									~IRBMKEngine		();

	virtual void							Initialize					();
	virtual void							Destroy						();
	 
	virtual	void							OnEvent						(EVENT E, u64 P1, u64 P2);
	virtual void							OnFrame						();

	virtual void							Exit						() = 0;
	virtual void							RunGame						(const char* ServerParams = "unreal/single/alife/new" ,const char*ClientParams = "localhost");
	virtual void							StopGame					();
	virtual void							LoadDefaultWorld			() = 0;
	virtual bool							IsRunningGame				();
			shared_str						GetGameVersion				();
	virtual shared_str						GetUnrealVersion			() = 0;
	virtual class IRBMKUnrealProxy*			CreateUnrealProxy			() = 0;
	virtual class IRBMKUnrealProxy*			GetUnrealPlayerCharacter	() = 0;
	virtual void							Destroy						(class IRBMKUnrealProxy*) = 0;
	virtual void							LoadCFormFormCurrentWorld	(class CObjectSpace&ObjectSpace,CDB::build_callback build_callback)= 0 ;
	virtual class ILevelGraph*				GetLevelGraph				(const char* Name) = 0;
	virtual class IGameGraph*				GetGameGraph				() = 0;
	virtual IReader							GetGameSpawn				() = 0;
	virtual bool							LoadWorld					(const char*Name) = 0;
	virtual ERBMKWorldStatus				GetWorldStatus				() = 0;

			IRBMKLevelToBlueprint*			GetLevelScript				();

	virtual IRBMKSoundManager*				GetSoundManager				() = 0;
	virtual IRBMKEnvironment*				GetEnvironment				() = 0;
			IRBMKEnvironment*				GetEnvironmentCheck			() {IRBMKEnvironment* Environment = GetEnvironment();R_ASSERT(Environment); return Environment;};
private:
	void									InitEngine					();
	void									InitSettings				();
	void									InitInput					();
	void									InitConsole					();

	void									execUserScript				();

	void									destroySettings				();
	void									destroyConsole				();
	void									destroyEngine				();

	EVENT									eQuit;
	EVENT									eStart;
	EVENT									eStartLoad;
	EVENT									eDisconnect;
	EVENT									eConsole;
	EVENT									eStartMPDemo;
};

extern ENGINE_API	IRBMKEngine*	g_Engine;


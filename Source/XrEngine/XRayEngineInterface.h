#ifndef __X_RAY_H__
#define __X_RAY_H__

// refs
class ENGINE_API CGameFont;

#include "../XrRender/Public/FactoryPtr.h"
#include "../XrRender/Public/ApplicationRender.h"

// definition
class ENGINE_API XRayEngineInterface	:
	public IEventReceiver
{
	friend class dxApplicationRender;

	// levels
	struct					sLevelInfo
	{
		char*				folder;
		char*				name;
	};
public:
	string2048				ls_header;
	string2048				ls_tip_number;
	string2048				ls_tip;
private:

	int		max_load_stage;

	int						load_stage;

	u32						ll_dwReference;
private:
	EVENT					eQuit;
	EVENT					eStart;
	EVENT					eStartLoad;
	EVENT					eDisconnect;
	EVENT					eConsole;
	EVENT					eStartMPDemo;

	void					Level_Append		(LPCSTR lname);
public:
	CGameFont*				pFontSystem;

	// Levels
	xr_vector<sLevelInfo>	Levels;
	u32						Level_Current;
	virtual void					Level_Scan			();
	virtual int						Level_ID			(LPCSTR name, LPCSTR ver, bool bSet);
	virtual void					Level_Set			(u32 ID);
	virtual void					LoadAllArchives		();
	virtual CInifile*				GetArchiveHeader	(LPCSTR name, LPCSTR ver);

	// Loading
	virtual void					LoadBegin			();
	virtual void					LoadEnd				();
	virtual void					LoadTitleInt		(LPCSTR str1, LPCSTR str2, LPCSTR str3);
	virtual void					LoadStage			();
	virtual void					LoadSwitch			();
	virtual void					LoadDraw			();

	virtual	void			OnEvent				(EVENT E, u64 P1, u64 P2);

	// Other
							XRayEngineInterface		();
	virtual					~XRayEngineInterface		();

	virtual void			OnFrame				();
	virtual	void			load_draw_internal	();
	virtual	void			destroy_loading_shaders();
	virtual void					Initialize();
	virtual void					Destroy();

	virtual class XRayUnrealProxyInterface*	CreateUnrealProxy	(class CObject*) = 0;
	virtual void							Destroy				(class XRayUnrealProxyInterface*) = 0;
	virtual void			LoadCFormFormCurrentWorld(class CObjectSpace&ObjectSpace,CDB::build_callback build_callback)= 0 ;
private:
	void InitEngine();
	void InitSettings();
	void InitInput();
	void InitConsole();
	void InitSound1();
	void execUserScript();
	void InitSound2();

	void destroyInput();
	void destroySettings();
	void destroyConsole();
	void destroySound();
	void destroyEngine();
};

extern ENGINE_API	XRayEngineInterface*	g_Engine;

#endif //__XR_BASE_H__

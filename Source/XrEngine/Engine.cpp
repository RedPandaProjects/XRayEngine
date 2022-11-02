// Engine->cpp: implementation of the CEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Engine.h"
#include "dedicated_server_only.h"

CEngine			*	Engine = nullptr;;
xrDispatchTable		*PSGP = nullptr;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEngine::CEngine()
{
	
}

CEngine::~CEngine()
{
	
}

extern	void msCreate		(LPCSTR name);

 void CEngine::Initialize	(void)
{
#ifdef _DEBUG
	 hPSGP = LoadLibrary("xrCPU_Pipe_Debug.dll");
#else
	 hPSGP = LoadLibrary("xrCPU_Pip_Release.dll");
#endif
	// Bind PSGP
	
	R_ASSERT	(hPSGP);
	xrBinder*	bindCPU	= (xrBinder*)	GetProcAddress(hPSGP,"xrBind_PSGP");	R_ASSERT(bindCPU);
	PSGP = xr_new<xrDispatchTable>();
	bindCPU		(PSGP, CPU::ID.feature);

	// Other stuff
	Engine->Sheduler.Initialize			( );
	// 
#ifdef DEBUG
	msCreate							("game");
#endif
}

typedef void __cdecl ttapi_Done_func(void);

void CEngine::Destroy	()
{
	Engine->Sheduler.Destroy				( );
#ifdef DEBUG_MEMORY_MANAGER
	extern void	dbg_dump_leaks_prepare	( );
	if (MemoryInterface->debug_mode)				dbg_dump_leaks_prepare	();
#endif // DEBUG_MEMORY_MANAGER
	Engine->External.Destroy				( );
	
	if (hPSGP)	
	{ 
		ttapi_Done_func*  ttapi_Done = (ttapi_Done_func*) GetProcAddress(hPSGP,"ttapi_Done");	R_ASSERT(ttapi_Done);
		if (ttapi_Done)
			ttapi_Done();

		FreeLibrary	(hPSGP); 
		hPSGP		=0; 
		ZeroMemory	(&PSGP,sizeof(PSGP));
		xr_delete(PSGP);
	}
}

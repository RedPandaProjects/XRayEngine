#include "stdafx.h"

#ifdef DEBUG
	ENGINE_API BOOL bDebug	= FALSE;
	
#endif

// Video
//. u32			psCurrentMode		= 1024;
// release version always has "mt_*" enabled
ENGINE_API Flags32		psDeviceFlags		= {
	rsDetails|mtPhysics|mtSound|mtNetwork};

// textures 
ENGINE_API int			psTextureLOD		= 1;

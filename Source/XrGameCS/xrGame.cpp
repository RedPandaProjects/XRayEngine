////////////////////////////////////////////////////////////////////////////
//	Module 		: xrGame.cpp
//	Created 	: 07.01.2001
//  Modified 	: 27.05.2004
//	Author		: Aleksandr Maksimchuk and Oles' Shyshkovtsov
//	Description : Defines the entry point for the DLL application.
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "object_factory.h"
#include "ui/xrUIXmlParser.h"
#include "xr_level_controller.h"
#include "profiler.h"

#include "lua/library_linkage.h"

FACTORY_PTR_INSTANCIATE(FontRender)
FACTORY_PTR_INSTANCIATE(FlareRender)
FACTORY_PTR_INSTANCIATE(ThunderboltRender)
FACTORY_PTR_INSTANCIATE(ThunderboltDescRender)
FACTORY_PTR_INSTANCIATE(LensFlareRender)
FACTORY_PTR_INSTANCIATE(RainRender)
FACTORY_PTR_INSTANCIATE(EnvironmentRender)
FACTORY_PTR_INSTANCIATE(EnvDescriptorRender)
FACTORY_PTR_INSTANCIATE(EnvDescriptorMixerRender)
FACTORY_PTR_INSTANCIATE(UIShader)
FACTORY_PTR_INSTANCIATE(WallMarkArray)
FACTORY_PTR_INSTANCIATE(StatGraphRender)
FACTORY_PTR_INSTANCIATE(UISequenceVideoItem)

void CCC_RegisterCommands();
void setup_luabind_allocator();

extern "C" {
	DLL_API DLL_Pure*	__cdecl xrFactory_Create		(CLASS_ID clsid)
	{
		static bool bIsInitilize = false;
		if (!bIsInitilize)
		{
			CCC_RegisterCommands();
			// keyboard binding
			CCC_RegisterInput();

			setup_luabind_allocator();

#ifdef DEBUG
			g_profiler = xr_new<CProfiler>();
#endif

			bIsInitilize = true;
		}

		DLL_Pure			*object = object_factory().client_object(clsid);
#ifdef DEBUG
		if (!object)
			return			(0);
#endif
		object->CLS_ID		= clsid;
		return				(object);
	}

	DLL_API void		__cdecl	xrFactory_Destroy		(DLL_Pure* O)
	{
		xr_delete			(O);
	}
};





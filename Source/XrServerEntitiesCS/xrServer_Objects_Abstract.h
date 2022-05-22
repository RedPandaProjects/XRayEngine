#pragma once
#include "xrServer_Space.h"
#	include "../xrCDB/xrCDB.h"
#include "../XrEngine/ShapeData.h"
#include "../XrEngine/gametype_chooser.h"

class NET_Packet;
class CDUInterface;

#ifndef _EDITOR
    #ifndef XRGAME_EXPORTS
		#include "../xrSound/Sound.h"
    #endif
#endif


#include "..\Editors\Public\xrEProps.h"

#ifndef XRGAME_EXPORTS
	#include "..\..\xrRender/Public\DrawUtils.h"
#else
	#include "..\xrRender/Public\DrawUtils.h"
#endif
#pragma warning(push)
#pragma warning(disable:4005)
#include "..\XrServerEntities\xrServer_Objects_Abstract_Base.h"
#pragma warning(pop)
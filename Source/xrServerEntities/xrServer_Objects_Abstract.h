////////////////////////////////////////////////////////////////////////////
//	Module 		: xrServer_Objects_Abstract.h
//	Created 	: 19.09.2002
//  Modified 	: 18.06.2004
//	Author		: Oles Shyshkovtsov, Alexander Maksimchuk, Victor Reutskiy and Dmitriy Iassenev
//	Description : Server objects
////////////////////////////////////////////////////////////////////////////
#pragma once
#pragma pack(push,4)
#include "xrServer_Space.h"
#	include "../xrCDB/xrCDB.h"

class NET_Packet;
class CDUInterface;




#ifndef XRGAME_EXPORTS
	#include "../../XrEngine/Render/DrawUtils.h"
#else
	#include "../XrEngine/Render/DrawUtils.h"
#endif
#pragma warning(push)
#pragma warning(disable:4005)


#include "xrServer_Objects_Abstract_Base.h"
#pragma warning(pop)

#pragma pack(pop)
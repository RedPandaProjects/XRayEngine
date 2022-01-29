////////////////////////////////////////////////////////////////////////////
//	Module 		: xrServer_Objects_Abstract.h
//	Created 	: 19.09.2002
//  Modified 	: 18.06.2004
//	Author		: Oles Shyshkovtsov, Alexander Maksimchuk, Victor Reutskiy and Dmitriy Iassenev
//	Description : Server objects
////////////////////////////////////////////////////////////////////////////



#pragma pack(push,4)
#include "xrServer_Space.h"
#ifdef XRGAME_EXPORTS
#	include "../../xrCDB/xrCDB.h"
#else // XRGAME_EXPORTS
#	include "../xrCDB/xrCDB.h"
#endif // XRGAME_EXPORTS
#include "ShapeData.h"

class NET_Packet;
class CDUInterface;

#ifndef XRGAME_EXPORTS
#	include "Sound.h"
#endif

#include "xrEProps.h"
#include "DrawUtils.h"

#pragma warning(push)
#pragma warning(disable:4005)
#include "..\..\xrServerEntities\xrServer_Objects_Abstract_Base.h"
#pragma warning(pop)

#pragma pack(pop)
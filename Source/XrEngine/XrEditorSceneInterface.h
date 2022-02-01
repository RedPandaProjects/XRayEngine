#pragma once
#include "..\XrCDB\xrCDB.h"
class CObjectSpace;
class ENGINE_API XrEditorSceneInterface
{
public:
	XrEditorSceneInterface();
	virtual ~XrEditorSceneInterface();
	virtual void LoadSpawn(xr_vector<NET_Packet>& Ps) = 0;
	virtual	void LoadCFrom(CObjectSpace*Space, CDB::build_callback)=0;
};
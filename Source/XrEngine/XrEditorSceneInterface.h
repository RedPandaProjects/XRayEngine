#pragma once
#include "..\XrCDB\xrCDB.h"
#include "game_graph.h"
class CObjectSpace;
class ENGINE_API XrEditorSceneInterface
{
public:
	XrEditorSceneInterface();
	virtual ~XrEditorSceneInterface();
	virtual IReader* LoadSpawn() = 0;
	virtual	void LoadCFrom(CObjectSpace*Space, CDB::build_callback)=0;
	virtual IGameGraph* GetGameGraph() = 0;
	virtual ILevelGraph* GetLevelGraph() = 0;

};
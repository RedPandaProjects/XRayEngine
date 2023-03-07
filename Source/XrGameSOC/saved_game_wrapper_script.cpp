////////////////////////////////////////////////////////////////////////////
//	Module 		: saved_game_wrapper_script.cpp
//	Created 	: 21.02.2006
//  Modified 	: 21.02.2006
//	Author		: Dmitriy Iassenev
//	Description : saved game wrapper class script export
////////////////////////////////////////////////////////////////////////////

#include "pch_script.h"
#include "saved_game_wrapper.h"
#include "ai_space.h"
#include "game_graph.h"
#include "xr_time.h"
#include "../XrEngine/XRayEngineInterface.h"

using namespace luabind;

xrTime CSavedGameWrapper__game_time		(const CSavedGameWrapper *self)
{
	return			(xrTime(self->game_time()));
}

LPCSTR CSavedGameWrapper__level_name	(const CSavedGameWrapper *self)
{
	if (g_Engine->GetGameGraph()&&self->level_id()>=0)
	{
		return g_Engine->GetGameGraph()->header().level(self->level_id()).name().c_str();
	}
	return "Unknown";
}


void CSavedGameWrapper::script_register	(lua_State *L)
{
	module(L)
	[
		class_<CSavedGameWrapper>("CSavedGameWrapper")
			.def(constructor<LPCSTR>())
			.def("game_time",		&CSavedGameWrapper__game_time)
			.def("level_name",		&CSavedGameWrapper__level_name)
			.def("level_id",		&CSavedGameWrapper::level_id)
			.def("actor_health",	&CSavedGameWrapper::actor_health),

		def("valid_saved_game",		(bool (*)(LPCSTR))(&valid_saved_game))
	];
}

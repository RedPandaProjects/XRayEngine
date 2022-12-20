#include "pch_script.h"
#include "bloodsucker.h"

using namespace luabind;


void CAI_Bloodsucker::script_register(lua_State *L)
{
	module(L)
	[
		class_<CAI_Bloodsucker,CGameObject>("CAI_Bloodsucker")
			.def(constructor<>())
	];
}

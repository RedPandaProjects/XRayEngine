#include "pch_script.h"
#include "burer.h"

using namespace luabind;


void CBurer::script_register(lua_State *L)
{
	module(L)
	[
		class_<CBurer,CGameObject>("CBurer")
			.def(constructor<>())
	];
}

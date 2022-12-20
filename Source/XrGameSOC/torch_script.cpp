#include "pch_script.h"
#include "torch.h"

using namespace luabind;


void CTorch::script_register	(lua_State *L)
{
	module(L)
	[
		class_<CTorch,CGameObject>("CTorch")
			.def(constructor<>())
	];
}

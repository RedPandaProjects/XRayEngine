#include "pch_script.h"
#include "console_registrator.h"
#include "../XrEngine/xr_ioconsole.h"

using namespace luabind;

XRayConsoleInterface*	console()
{
	return Console;
}

int get_console_integer(XRayConsoleInterface* c, LPCSTR cmd)
{
	int val=0,min=0,max=0;
	max = c->GetInteger ( cmd, val, min);
	return val;
}

float get_console_float(XRayConsoleInterface* c, LPCSTR cmd)
{
	float val=0,min=0,max=0;
	max = c->GetFloat ( cmd, val, min);
	return val;
}

bool get_console_bool(XRayConsoleInterface* c, LPCSTR cmd)
{
	BOOL val;
	val = c->GetBool (cmd);
	return !!val;
}

#pragma optimize("s",on)
void console_registrator::script_register(lua_State *L)
{
	module(L)
	[
		def("get_console",					&console),
		class_<XRayConsoleInterface>("CConsole")
		.def("execute",						&XRayConsoleInterface::Execute)
		.def("execute_script",				&XRayConsoleInterface::ExecuteScript)
		.def("show",						&XRayConsoleInterface::Show)
		.def("hide",						&XRayConsoleInterface::Hide)
//		.def("save",						&XRayConsoleBase::Save)
		.def("get_string",					&XRayConsoleInterface::GetString)
		.def("get_integer",					&get_console_integer)
		.def("get_bool",					&get_console_bool)
		.def("get_float",					&get_console_float)
		.def("get_token",					&XRayConsoleInterface::GetToken)
//		.def("",				&XRayConsoleBase::)

	];
}
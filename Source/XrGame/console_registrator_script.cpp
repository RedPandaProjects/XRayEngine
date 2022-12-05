#include "pch_script.h"
#include "console_registrator.h"
#include "../xrEngine/xr_ioconsole.h"

using namespace luabind;

ConsoleBase*	console()
{
	return Console;
}

int get_console_integer(ConsoleBase* c, LPCSTR cmd )
{
	int min = 0, max = 0;
	int val = c->GetInteger ( cmd, min, max );
	return val;
}

float get_console_float(ConsoleBase* c, LPCSTR cmd )
{
	float min = 0.0f, max = 0.0f;
	float val = c->GetFloat ( cmd, min, max );
	return val;
}

bool get_console_bool(ConsoleBase* c, LPCSTR cmd )
{
	return c->GetBool( cmd );
}

void execute_console_command_deferred	(ConsoleBase* c, LPCSTR string_to_execute)
{
	Engine->Event.Defer	("KERNEL:console", size_t(xr_strdup(string_to_execute)) );
}

#pragma optimize("s",on)
void console_registrator::script_register(lua_State *L)
{
	module(L)
	[
		def("get_console",					&console),

		class_<ConsoleBase>("CConsole")
			.def("execute",					&ConsoleBase::Execute)
			.def("execute_script",			&ConsoleBase::ExecuteScript)
			.def("show",					&ConsoleBase::Show)
			.def("hide",					&ConsoleBase::Hide)

			.def("get_string",				&ConsoleBase::GetString)
			.def("get_integer",				&get_console_integer)
			.def("get_bool",				&get_console_bool)
			.def("get_float",				&get_console_float)
			.def("get_token",				&ConsoleBase::GetToken)
			.def("execute_deferred",		&execute_console_command_deferred)
	];
}
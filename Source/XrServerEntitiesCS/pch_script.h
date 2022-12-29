////////////////////////////////////////////////////////////////////////////
//	Module 		: pch_script.h
//	Created 	: 23.05.2007
//  Modified 	: 10.08.2007
//	Author		: Dmitriy Iassenev
//	Description : precompiled header for lua and luabind users
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_SCRIPT_H
#define PCH_SCRIPT_H

#include "stdafx.h"

extern "C" {
	#include <lua/lua.h>
	#include <lua/lualib.h>
	#include <lua/lauxlib.h>
};

#include <luabind/boost_legacy.h>
#include <luabind/luabind.hpp>

#include <luabind/object.hpp>
#include <luabind/functor.hpp>
#include <luabind/operator.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/return_reference_to_policy.hpp>
#include <luabind/out_value_policy.hpp>
#include <luabind/iterator_policy.hpp>

#endif // PCH_SCRIPT_H
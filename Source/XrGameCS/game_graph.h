////////////////////////////////////////////////////////////////////////////
//	Module 		: game_graph.h
//	Created 	: 18.02.2003
//  Modified 	: 13.11.2003
//	Author		: Dmitriy Iassenev
//	Description : Game graph class
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../xrEngine/game_graph.h"
#include "script_export_space.h"

class CGameGraphScript
{
public:
	DECLARE_SCRIPT_REGISTER_FUNCTION
};
add_to_type_list(CGameGraphScript)
#undef script_type_list
#define script_type_list save_type_list(CGameGraphScript)

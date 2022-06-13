////////////////////////////////////////////////////////////////////////////
//	Module 		: luabind_memory.h
//	Created 	: 24.06.2005
//  Modified 	: 23.04.2008
//	Author		: Dmitriy Iassenev
//	Description : luabind memory
////////////////////////////////////////////////////////////////////////////

#ifndef LUABIND_MEMORY_H_INCLUDED
#define LUABIND_MEMORY_H_INCLUDED


#include "luabind_types.h"
#include <boost/type_traits/is_polymorphic.hpp>

namespace luabind {
	extern LUABIND_API	memory_allocation_function_pointer		allocator;
	extern LUABIND_API	memory_allocation_function_parameter	allocator_parameter;

	inline void* call_allocator	(void const* buffer, size_t const size)
	{
		return			(allocator(allocator_parameter, buffer, size));
	}
} // namespace luabind

#include "luabind_delete.h"
#include "luabind_auto_ptr.h"

#define LUABIND_MEMORY_MANAGER_GENERATOR_MAX_ARITY	6
#include "luabind_memory_manager_generator.h"

#endif // #ifndef LUABIND_MEMORY_H_INCLUDED
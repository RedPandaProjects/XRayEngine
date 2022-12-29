#include "pch.h"
#include "luabind/luabind_memory.h"
luabind::memory_allocation_function_pointer		luabind::allocator = 0;
luabind::memory_allocation_function_parameter	luabind::allocator_parameter = 0;
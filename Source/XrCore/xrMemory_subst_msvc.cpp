#include "stdafx.h"
#pragma hdrstop

#include "xrMemory_align.h"
#include "xrMemory_pure.h"


#ifndef DEBUG_MEMORY_MANAGER
#	define	debug_mode 0
#endif // DEBUG_MEMORY_MANAGER

#ifdef DEBUG_MEMORY_MANAGER
	XRCORE_API void*	g_globalCheckAddr = NULL;
#endif // DEBUG_MEMORY_MANAGER

#ifdef DEBUG_MEMORY_MANAGER
	extern void save_stack_trace	();
#endif // DEBUG_MEMORY_MANAGER

MEMPOOL		mem_pools			[mem_pools_count];

#ifdef PURE_ALLOC
bool	g_use_pure_alloc		= false;
#endif // PURE_ALLOC

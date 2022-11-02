#include "stdafx.h"
#pragma hdrstop

#include	"xrsharedmem.h"
#include	"xrMemory_pure.h"

#include	<malloc.h>

BOOL		mem_initialized	= FALSE;
bool		shared_str_initialized	= false;

//fake fix of memory corruptions in multiplayer game :(
XRCORE_API	bool	g_allow_heap_min = true;

// Processor specific implementations
extern		pso_MemCopy		xrMemCopy_MMX;
extern		pso_MemCopy		xrMemCopy_x86;
extern		pso_MemFill		xrMemFill_x86;
extern		pso_MemFill32	xrMemFill32_MMX;
extern		pso_MemFill32	xrMemFill32_x86;

#ifdef DEBUG_MEMORY_MANAGER
XRCORE_API void dump_phase		()
{
	if (!MemoryInterface->debug_mode)
		return;

	static int					phase_counter = 0;

	string256					temp;
	xr_sprintf					(temp,sizeof(temp),"x:\\$phase$%d.dump",++phase_counter);
	MemoryInterface->mem_statistic		(temp);
}
#endif // DEBUG_MEMORY_MANAGER

// xr_strdup
char*			xr_strdup		(const char* string)
{	
	VERIFY	(string);
	u32		len			= u32(xr_strlen(string))+1	;
	char *	memory		= (char*)	MemoryInterface->mem_alloc( len
#ifdef DEBUG_MEMORY_NAME
		, "strdup"
#endif // DEBUG_MEMORY_NAME
	);
	CopyMemory		(memory,string,len);
	return	memory;
}

XRCORE_API		BOOL			is_stack_ptr		( void* _ptr)
{
	int			local_value		= 0;
	void*		ptr_refsound	= _ptr;
	void*		ptr_local		= &local_value;
	ptrdiff_t	difference		= (ptrdiff_t)_abs(s64(ptrdiff_t(ptr_local) - ptrdiff_t(ptr_refsound)));
	return		(difference < (512*1024));
}

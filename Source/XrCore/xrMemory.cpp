#include "stdafx.h"
#pragma hdrstop

#include	"xrsharedmem.h"
#include	"xrMemory_pure.h"

#include	<malloc.h>

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

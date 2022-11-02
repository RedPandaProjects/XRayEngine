#pragma once
#include "xrMemory_pso.h"
#include "xrMemory_POOL.h"
#include "Interface/XRayMemoryInterface.h"
#include "xrMemory_subst_msvc.h"

	template <class T>
	IC T*		xr_alloc	(u32 count)				{	return  (T*)MemoryInterface->mem_alloc(count*sizeof(T));	}
	template <class T>
	IC void		xr_free		(T* &P)					{	if (P) { MemoryInterface->mem_free((void*)P); P=NULL;	};	}
	IC void*	xr_malloc	(size_t size)			{	return	MemoryInterface->mem_alloc(size);					}
	IC void*	xr_realloc	(void* P, size_t size)	{	return MemoryInterface->mem_realloc(P,size);				}

XRCORE_API	char* 	xr_strdup	(const char* string);
#if !__UNREAL__
IC void* operator new		(size_t size) { R_ASSERT(false);return nullptr; }
IC void		operator delete		(void* p) { R_ASSERT(false); }
IC void* operator new[](size_t size) {	R_ASSERT(false);	return nullptr;	}
IC void		operator delete[](void* p) {	R_ASSERT(false);}
#endif
// POOL-ing
const		u32			mem_pools_count			=	54;
const		u32			mem_pools_ebase			=	16;
const		u32			mem_generic				=	mem_pools_count+1;
extern		MEMPOOL		mem_pools				[mem_pools_count];
extern		BOOL		mem_initialized;

XRCORE_API void vminfo			(size_t *_free, size_t *reserved, size_t *committed);
XRCORE_API void log_vminfo		();
XRCORE_API u32	mem_usage_impl	(HANDLE heap_handle, u32* pBlocksUsed, u32* pBlocksFree);

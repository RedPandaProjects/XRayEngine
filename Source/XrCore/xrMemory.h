#pragma once
#include "xrMemory_pso.h"
#include "xrMemory_POOL.h"
#include "Interface/XRayMemoryInterface.h"
#include "xrMemory_subst_msvc.h"
#ifndef SHIPPING
	IC void*	xr_malloc	(size_t size)			{	return	MemoryInterface->mem_alloc(size);}
	IC void		xr_mfree		(void*P)				{	return	MemoryInterface->mem_free(P);}
	IC void*	xr_realloc	(void* P, size_t size)	{	return MemoryInterface->mem_realloc(P,size); }
#else
	extern void*xr_malloc	(size_t size);
	extern void	xr_mfree		(void*P);
	extern void*xr_realloc	(void* P, size_t size);
#endif

	template <class T>
	IC void		xr_free		(T* &P)					{	if (P) { xr_mfree(const_cast<void*>(reinterpret_cast<const void*>(P))); P=NULL; }; }
	template <class T>
	IC T*		xr_alloc	(u32 count)				{	return reinterpret_cast<T*>(xr_malloc(count * sizeof(T))); }

	XRCORE_API	char* 	xr_strdup	(const char* string);
#if !__UNREAL__
#ifdef SHIPPING
extern  void*	operator new(size_t size);
extern void	operator delete	(void* p);
extern void*	operator new[](size_t size);
extern void	operator delete[](void* p);
#else
ICF void* operator new		(size_t size) { return MemoryInterface->mem_alloc(size); }
ICF void		operator delete		(void* p) { return MemoryInterface->mem_free(p); }
ICF void* operator new[](size_t size) {		return MemoryInterface->mem_alloc(size);	}
ICF void		operator delete[](void* p) {	return MemoryInterface->mem_free(p); }
#endif
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

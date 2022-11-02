#pragma once
class XRCORE_API XRayMemoryInterface
{
public:
	virtual				~XRayMemoryInterface	();
	virtual u32			mem_usage				() = 0;
	IC void				mem_counter_set			(u32 _val) { stat_counter = _val; }
	IC u32				mem_counter_get			() { return stat_counter; }

	virtual void*		mem_alloc				(size_t	size) = 0;
	virtual void*		mem_realloc				(void* p, size_t size) = 0;
	virtual void		mem_free				(void* p) = 0;

	u32					stat_calls;
	s32					stat_counter;
};
namespace RedImageTool
{
	typedef void* (* red_memory_allocation_function) (void* pointer,  size_t size);
	extern red_memory_allocation_function MemoryAllocationFunction;
	template<typename T>
	inline T* red_alloc(size_t count)
	{
		return (T*)MemoryAllocationFunction(nullptr, sizeof(T) * count);
	}
	template<typename T>
	inline T* red_realloc(T* pointer, size_t count)
	{
		return (T*)MemoryAllocationFunction(pointer, sizeof(T) * count);
	}
	template<typename T>
	inline void red_free(T*pointer)
	{
		MemoryAllocationFunction(pointer, 0);
	}
	inline size_t red_recommended_size(size_t size)
	{
		size_t out = 1;
		while (out <= size)
		{
			out <<= 1;
		}
		return out;
	}

}
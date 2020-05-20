#pragma once
class XRayRenderFastAllocator
{
public:
	static const size_t size_block = 1024;
	static const size_t count_block = 128;

	XRayRenderFastAllocator();
	~XRayRenderFastAllocator();
	void* Alloc();
	void FlushStart();
	void FlushEnd();
	void Free(void*data);
private:
	class Buffer
	{
		uint8_t data[size_block * count_block];

	public:

		Buffer*  next;

		Buffer(Buffer* next):next(next)
		{
		}

		void* getBlock(std::size_t index)
		{
			return reinterpret_cast<void*>(&data[size_block * index]);
		}
	};
	struct Block
	{
		Block* next;
	};
	Buffer* AllocaBuffer();
	Block* m_firstFreeBlock = nullptr;
	Buffer* m_firstBuffer = nullptr;
	Buffer* m_firstFreeBuffer = nullptr;
	std::size_t m_bufferedBlocks = count_block;
	bool m_flush;
};
extern XRayRenderFastAllocator* GRenderFastAllocator;

template <typename T>
class XRayRenderFastAlloca
{
public:
	typedef size_t size_type;
	typedef  std::ptrdiff_t difference_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;

public:
	template<class _Other>
	struct rebind { typedef XRayRenderFastAlloca<_Other> other; };
public:
	pointer address(reference _Val) const { return (&_Val); }
	const_pointer address(const_reference _Val) const { return (&_Val); }
	XRayRenderFastAlloca() { }
	XRayRenderFastAlloca(const XRayRenderFastAlloca<T>&) { }
	template<class _Other> XRayRenderFastAlloca(const XRayRenderFastAlloca<_Other>&) { }
	template<class _Other> XRayRenderFastAlloca<T>& operator= (const XRayRenderFastAlloca<_Other>&) { return (*this); }
	bool operator== (const XRayRenderFastAlloca&) { return true; }
	bool operator!= (const XRayRenderFastAlloca&) { return false; }
	template<class _Other>
	bool operator== (const XRayRenderFastAlloca<_Other>&) { return false; }
	template<class _Other>
	bool operator!= (const XRayRenderFastAlloca<_Other>&) { return true; }
	pointer allocate(size_type n, const void* p = 0) const { static_assert(XRayRenderFastAllocator::size_block >= sizeof(T),"sizeof(T) is bigggg!!!");   return (pointer)GRenderFastAllocator->Alloc(); }
	char* _charalloc(size_type n) { return (char*)allocate(n); }
	void deallocate(pointer p, size_type n) const { GRenderFastAllocator->Free(p); }
	void deallocate(void* p, size_type n) const { GRenderFastAllocator->Free(p); }
	template<class C, class U>
	void construct(C* p, const U& _Val) { ::new ((void*)p) C(_Val); }
	template<class C>
	void destroy(C* p) { p->~C(); }
	size_type max_size() const { size_type _Count = (size_type)(-1) / sizeof(T); return (0 < _Count ? _Count : 1); }


};
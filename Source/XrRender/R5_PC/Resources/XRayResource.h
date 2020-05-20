#pragma once
class XRayResource
{
public:

	XRayResource() :Counter(0) {}
	size_t Counter;
};
class XRayResourceNamed:public XRayResource
{
public:

	XRayResourceNamed() {}
	shared_str GetName() { return Name; }
protected:
	shared_str Name;
};
template<typename T>
class XRayResourceRef
{
public:
	XRayResourceRef(const T& right) { m_ref = xr_new<T>(right); m_ref->Counter = 1; }
	XRayResourceRef() :m_ref(0) {}
	inline ~XRayResourceRef() { clear(); }
	inline void clear() { if (m_ref) { if (--m_ref->Counter == 0) { xr_delete(m_ref); }m_ref = 0; } }
	inline bool empty()const { return m_ref == nullptr; }
	inline void copy(const XRayResourceRef& right) { clear(); m_ref = right.m_ref; if (m_ref)m_ref->Counter++; }
	inline void swap(XRayResourceRef& right) { std::swap(m_ref, right.m_ref); }

	XRayResourceRef(const XRayResourceRef& right) { m_ref = 0; copy(right); }
	XRayResourceRef(XRayResourceRef&& right) { m_ref = 0; swap(right); }
	inline XRayResourceRef& operator=(const XRayResourceRef& right) { copy(right);  return *this; }
	inline XRayResourceRef& operator=(XRayResourceRef&& right) { swap(right);  return *this; }

	T* operator->() { VERIFY(m_ref); return m_ref; }
	const T* operator->()const { VERIFY(m_ref); return m_ref; }

private:
	T* m_ref;
};
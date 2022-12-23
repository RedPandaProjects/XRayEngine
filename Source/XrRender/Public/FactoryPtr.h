#pragma once

#include "RenderFactory.h"
#include "../xrAPI/xrAPI.h"

#define FACTORY_PTR_INSTANCIATE(Class) \
	void FactoryPtr<I##Class>::CreateObject(void) \
{ \
	m_pObject = RenderFactory->Create##Class(); \
} \
	void FactoryPtr<I##Class>::DestroyObject(void) \
{ \
	RenderFactory->Destroy##Class(m_pObject); \
	m_pObject = NULL; \
}
	

template<class T> 
class FactoryPtr
{
public:
	FactoryPtr() { CreateObject();}
	~FactoryPtr() { DestroyObject();}

	FactoryPtr(const FactoryPtr<T> &_in)
	{
		CreateObject();
		m_pObject->Copy(*_in.m_pObject);
	}

	FactoryPtr& operator=( const FactoryPtr &_in)
	{
		m_pObject->Copy(*_in.m_pObject);
		return *this;
	}

	T& operator*() const {return *m_pObject;}
	T* operator->() const {return m_pObject;}

	// unspecified bool type
	typedef T const * (FactoryPtr::*unspecified_bool_type) () const;
	operator unspecified_bool_type () const	{return (!m_pObject ? 0 : &FactoryPtr::get);}
	bool operator!	() const { return m_pObject == 0;}

private:
	void CreateObject();
	void DestroyObject();
	T const* get() const { return m_pObject; }
private:
	T*					m_pObject;
};
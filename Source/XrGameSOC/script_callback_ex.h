////////////////////////////////////////////////////////////////////////////
//	Module 		: script_callback_ex.h
//	Created 	: 06.02.2004
//  Modified 	: 11.01.2005
//	Author		: Sergey Zhemeitsev and Dmitriy Iassenev
//	Description : Script callbacks with return value
////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ai_space.h"
#include "luabind/luabind.hpp"
#include "script_engine.h"

IC bool compare_safe(const luabind::object& o1, const luabind::object& o2) {
	if ((o1.type() == LUA_TNIL) && (o2.type() == LUA_TNIL))
		return (true);

	return (o1 == o2);
}

template <typename return_type>
class CScriptCallbackEx_
{
private:
	using functor_type = luabind::functor<return_type>;
	using object_type = luabind::object;
	using unspecified_bool_type = bool (CScriptCallbackEx_::*)() const;

protected:
	functor_type m_functor;
	object_type m_object;

private:
	IC bool empty() const { return (!!m_functor.lua_state()); }

public:
	CScriptCallbackEx_() = default;
	CScriptCallbackEx_(const CScriptCallbackEx_& callback) {
		clear();
		*this = callback;
	}
	virtual ~CScriptCallbackEx_() = default;
	CScriptCallbackEx_& operator=(const CScriptCallbackEx_& callback) {
		clear();

		if (callback.m_functor.is_valid() && callback.m_functor.lua_state())
			m_functor = callback.m_functor;

		if (callback.m_object.is_valid() && callback.m_object.lua_state())
			m_object = callback.m_object;

		return (*this);
	}

	bool operator==(const CScriptCallbackEx_& callback) const {
		return compare_safe(m_object, (callback.m_object)) && m_functor == (callback.m_functor);
	}

	bool operator==(const object_type& object) const { return compare_safe(m_object, object); }
	void set(const functor_type& functor) {
		clear();
		m_functor = functor;
	}

	void set(const functor_type& functor, const object_type& object) {
		clear();

		m_functor = functor;
		m_object = object;
	}

	void clear() {
		m_functor.~functor_type();
		new (&m_functor) functor_type();

		m_object.~object_type();
		new (&m_object) object_type();
	}

	operator unspecified_bool_type() const {
		return (!m_functor.is_valid() ? 0 : &CScriptCallbackEx_::empty);
	}
};

template <typename return_type>
class CScriptCallbackEx : public CScriptCallbackEx_<return_type> {
	template <typename T>
	static void do_return(T&& value, std::true_type /*is_void*/) {
		(void)value;
	}

	template <typename T>
	static return_type do_return(T&& value, std::false_type /*is_void*/) {
		return return_type(std::forward<T>(value));
	}

	template <typename T>
	static decltype(auto) do_return(T&& value) {
		return do_return(std::forward<T>(value), std::is_void<return_type>());
	}
public:
	template <typename... Args>
	return_type operator()(Args&&... args) const
	{
		try
		{
			try
			{
				if (this->m_functor)
				{
					VERIFY(this->m_functor.is_valid());
					if (this->m_object.is_valid())
					{
						VERIFY(this->m_object.is_valid());
						return do_return(this->m_functor(this->m_object, std::forward<Args>(args)...));
					}
					else
						return do_return(this->m_functor(std::forward<Args>(args)...));
				}
			}
			catch (std::exception&)
			{
				//    CVMLua::PrintOut(ai().script_engine().lua(), "", 2);
			}
		}
		catch (...) {
			const_cast<CScriptCallbackEx<return_type>*>(this)->clear();
		}
		return do_return(0);
	}

	template <typename... Args>
	return_type operator()(Args&&... args)
	{
		try
		{
			try
			{
				if (this->m_functor)
				{
					VERIFY(this->m_functor.is_valid());
					if (this->m_object.is_valid()) {
						VERIFY(this->m_object.is_valid());
						return do_return(this->m_functor(this->m_object, std::forward<Args>(args)...));
					}
					else
						return do_return(this->m_functor(std::forward<Args>(args)...));
				}
			}
			catch (std::exception&)
			{
				//	CVMLua::PrintOut(ai().script_engine().lua(), "", 2);
			}
		}
		catch (...) {
			const_cast<CScriptCallbackEx<return_type>*>(this)->clear();
		}
		return do_return(0);
	}
};
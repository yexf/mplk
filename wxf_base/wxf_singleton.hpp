#pragma once

#include "wxf_lock.h"

namespace noncopyable_  // protection from unintended ADL
{
	class noncopyable
	{
	protected:
		noncopyable() {}
		~noncopyable() {}
	private:  // emphasize the following members are private
		noncopyable( const noncopyable& );
		const noncopyable& operator=( const noncopyable& );
	};
}

namespace wxf
{
	typedef noncopyable_::noncopyable noncopyable;

	template <typename T>
	class singleton_wrapper : public T
	{
	};

	template <typename T, bool threadsafe = true>
	class singleton : public noncopyable
	{
	private:
		static wxf_lock mtx;
		static bool initialized;
		static T &get_instance()
		{
			bool locked = false;
			if (threadsafe && !initialized)
			{
				mtx.lock();
				locked = true;
			}
			static singleton_wrapper<T> t;
			initialized = true;
			if (locked)
			{
				mtx.unlock();
			}
			return static_cast<T &>(t);
		}

	public:
		static T &get_mutable_instance()
		{
			return get_instance();
		}

		static T const &get_const_instance()
		{
			return get_instance();
		}
	};


	template<typename T, bool lock>
	wxf_lock singleton<T, lock>::mtx;

	template<typename T, bool lock>
	bool singleton<T, lock>::initialized = false;

	// non-threadsafe version
	template<typename T>
	class singleton<T, false> : public noncopyable
	{
	private:
		static T &get_instance()
		{
			static T t;
			return t;
		}

	public:
		static T &get_mutable_instance()
		{
			return get_instance();
		}

		static T const &get_const_instance()
		{
			return get_instance();
		}
	};

}
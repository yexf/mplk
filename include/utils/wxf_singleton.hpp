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


class wxf_delay
{
public:
	wxf_delay():m_hWnd(NULL),m_uMsg(0),m_wParam(0),m_lParam(0),m_uCount(0)
	{

	}
	~wxf_delay()
	{
		m_hWnd = NULL;
	}
public:
	void SetWnd(HWND hWnd)
	{
		m_hWnd = hWnd;
	}
	void SetMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		m_uMsg = uMsg;
		m_wParam = wParam;
		m_lParam = lParam;
	}
	void SetCount(UINT uCount)
	{
		m_uCount = uCount;
	}
	UINT DoTime()
	{
		if (m_uCount)
		{
			m_uCount--;
			if (m_uCount == 0 && m_hWnd != NULL)
			{
				::SendMessage(m_hWnd, m_uMsg, m_wParam, m_lParam);
			}
		}
		return m_uCount;
	}
private:
	HWND m_hWnd;
	UINT m_uMsg;
	WPARAM m_wParam;
	LPARAM m_lParam;
	UINT m_uCount;
};

class wxf_delaylist
{
public:
	wxf_delaylist():m_hWnd(NULL)
	{

	}
	~wxf_delaylist()
	{
		m_hWnd = NULL;
	}
public:
	void SetWnd(HWND hWnd)
	{
		m_hWnd = hWnd;
	}
	bool AddDelay(UINT uCount, UINT uMsg, HWND hWnd = NULL,WPARAM wParam = 0, LPARAM lParam = 0)
	{
		if (hWnd == NULL)
		{
			hWnd = m_hWnd;
		}
		if (hWnd == NULL)
		{
			return false;
		}
		wxf_delay oDelay;
		oDelay.SetWnd(hWnd);
		oDelay.SetCount(uCount);
		oDelay.SetMsg(uMsg,wParam,lParam);

		m_oLock.lock();
		m_qDelay.push_back(oDelay);
		m_oLock.unlock();

		return true;
	}
	void DoTime()
	{
		std::list<wxf_delay>::iterator itDelay = m_qDelay.begin();

		while (itDelay != m_qDelay.end())
		{
			if (!itDelay->DoTime())
			{
				m_oLock.lock();
				itDelay = m_qDelay.erase(itDelay);
				m_oLock.unlock();
			}
			else
			{
				itDelay++; 
			}
			
		}
	}
public:
	HWND m_hWnd;
	std::list<wxf_delay> m_qDelay;
	wxf_lock m_oLock;
};

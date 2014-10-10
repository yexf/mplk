#pragma warning(disable : 4275)
#pragma warning(disable : 4996)
#include "wxf_base.h"
#include "wxf_lock.h"


//////////////////////////////////////////////////////////////////////////
//                  Class  wxf_lock   实现                                    
//////////////////////////////////////////////////////////////////////////

/*************************************************************
 函 数 名 : wxf_lock()
 功能概要 : 构造函数
 返 回 值 : 无 
 参    数 : 
            char * phost : 标志是那个模块在使用该互斥体 
*************************************************************/
wxf_lock::wxf_lock(bool bInit)
{
	if (bInit)
	{
		init(NULL);
	}
}

/*************************************************************
 函 数 名 : wxf_lock()
 功能概要 : 析构函数
 返 回 值 : 无 
 参    数 : 无
*************************************************************/
wxf_lock::~wxf_lock()
{
	if(bInit)
		del();
}

/*************************************************************
 函 数 名 : init()
 功能概要 : 初始化互斥体对象
 返 回 值 : BOOL 
 参    数 : 
            char * phost : 标志是那个模块在使用该互斥体
*************************************************************/
//##ModelId=4519E3D00375
bool wxf_lock::init(char * phost)
{
	int n =0;

	//初始化ch_host
	if( (phost == NULL) ||
		(strlen(phost) > WXF_MAX_NAME) )
		strcpy(ch_host,"wxf_lock");
	else
		strcpy(ch_host,phost);

	//init windows mutex
	//CRITICAL_SECTION 默认支持递归访问 
	::InitializeCriticalSection(&m_mutex);

	bInit = true;
	return bInit;
}

/*************************************************************
 函 数 名 : del()
 功能概要 : 删除互斥体对象
 返 回 值 : BOOL
 参    数 : 无
*************************************************************/
//##ModelId=4519E3E701F2
bool wxf_lock::del()
{
	//del windows mutex
	::DeleteCriticalSection(&m_mutex);

	bInit = false;
	return true;
}

/*************************************************************
 函 数 名 : lock()
 功能概要 : 阻塞等待获得拥有权
 返 回 值 : void 
 参    数 : 无
*************************************************************/
//##ModelId=4519E3F40088
bool wxf_lock::lock()
{
	int n =0;

	//get ownership blocking
	::EnterCriticalSection(&m_mutex);

	return true;
}

/*************************************************************
 函 数 名 : trylock()
 功能概要 : 无论是否取得拥有权,调用立即返回(但WINDOWS下还是阻塞)
 返 回 值 : bool 
            TRUE :  成功获得用有权
			FALSE:  互斥体被其它线程锁定,获得失败
 参    数 : 无
*************************************************************/
bool wxf_lock::trylock()
{
	int ret = 0;

    ret = ::TryEnterCriticalSection(&m_mutex);
	if (ret) return true;
	else return false;
}

/*************************************************************
 函 数 名 : unlock()
 功能概要 : 让出拥有权
 返 回 值 : BOOL
 参    数 : 无
*************************************************************/
//##ModelId=4519E40E003F
bool wxf_lock::unlock()
{
	int n =0;

	//release ownership 
	::LeaveCriticalSection(&m_mutex);
		
	return true;
}
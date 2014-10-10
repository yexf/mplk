#pragma warning(disable : 4275)
#pragma warning(disable : 4996)
#include "wxf_base.h"
#include "wxf_lock.h"


//////////////////////////////////////////////////////////////////////////
//                  Class  wxf_lock   ʵ��                                    
//////////////////////////////////////////////////////////////////////////

/*************************************************************
 �� �� �� : wxf_lock()
 ���ܸ�Ҫ : ���캯��
 �� �� ֵ : �� 
 ��    �� : 
            char * phost : ��־���Ǹ�ģ����ʹ�øû����� 
*************************************************************/
wxf_lock::wxf_lock(bool bInit)
{
	if (bInit)
	{
		init(NULL);
	}
}

/*************************************************************
 �� �� �� : wxf_lock()
 ���ܸ�Ҫ : ��������
 �� �� ֵ : �� 
 ��    �� : ��
*************************************************************/
wxf_lock::~wxf_lock()
{
	if(bInit)
		del();
}

/*************************************************************
 �� �� �� : init()
 ���ܸ�Ҫ : ��ʼ�����������
 �� �� ֵ : BOOL 
 ��    �� : 
            char * phost : ��־���Ǹ�ģ����ʹ�øû�����
*************************************************************/
//##ModelId=4519E3D00375
bool wxf_lock::init(char * phost)
{
	int n =0;

	//��ʼ��ch_host
	if( (phost == NULL) ||
		(strlen(phost) > WXF_MAX_NAME) )
		strcpy(ch_host,"wxf_lock");
	else
		strcpy(ch_host,phost);

	//init windows mutex
	//CRITICAL_SECTION Ĭ��֧�ֵݹ���� 
	::InitializeCriticalSection(&m_mutex);

	bInit = true;
	return bInit;
}

/*************************************************************
 �� �� �� : del()
 ���ܸ�Ҫ : ɾ�����������
 �� �� ֵ : BOOL
 ��    �� : ��
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
 �� �� �� : lock()
 ���ܸ�Ҫ : �����ȴ����ӵ��Ȩ
 �� �� ֵ : void 
 ��    �� : ��
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
 �� �� �� : trylock()
 ���ܸ�Ҫ : �����Ƿ�ȡ��ӵ��Ȩ,������������(��WINDOWS�»�������)
 �� �� ֵ : bool 
            TRUE :  �ɹ��������Ȩ
			FALSE:  �����屻�����߳�����,���ʧ��
 ��    �� : ��
*************************************************************/
bool wxf_lock::trylock()
{
	int ret = 0;

    ret = ::TryEnterCriticalSection(&m_mutex);
	if (ret) return true;
	else return false;
}

/*************************************************************
 �� �� �� : unlock()
 ���ܸ�Ҫ : �ó�ӵ��Ȩ
 �� �� ֵ : BOOL
 ��    �� : ��
*************************************************************/
//##ModelId=4519E40E003F
bool wxf_lock::unlock()
{
	int n =0;

	//release ownership 
	::LeaveCriticalSection(&m_mutex);
		
	return true;
}
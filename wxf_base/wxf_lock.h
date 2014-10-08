/************************************************************************************** 
** Copyright (C) 2011 Multak,Inc. All rights reserved
**
** Filename : wxf_lock.h
** Revision : 1.00
**
** Author :  yexf
** Date :  2013/08/20
**
** Description:
**************************************************************************************/ 
#ifndef _wxf_lock_h_
#define _wxf_lock_h_



class wxf_lock
{
public:
	//构造函数
	wxf_lock(bool bInit = true);

	//析构函数
	~wxf_lock();

	//初始化锁
	//##ModelId=4519E3D00375
	bool init(char * phost = NULL);

	//删除锁
	//##ModelId=4519E3E701F2
	bool del();

	//阻塞等待直到取得拥有权
	//##ModelId=4519E3F40088
	bool lock();

	//无论是否取得拥有权,调用马上返回
	bool trylock();

	//让出拥有权
	//##ModelId=4519E40E003F
	bool unlock();

private:
	//##ModelId=4519E338018C
	wxf_mutex m_mutex;

	//该mutex所属的宿主
	char  ch_host[WXF_MAX_NAME];

	//初始化标志
	bool  bInit;
};

#endif /*_wxf_lock_h_*/

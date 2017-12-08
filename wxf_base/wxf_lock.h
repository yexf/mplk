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
	//���캯��
	wxf_lock(bool bInit = true);

	//��������
	~wxf_lock();

	//��ʼ����
	//##ModelId=4519E3D00375
	bool init(char * phost = NULL);

	//ɾ����
	//##ModelId=4519E3E701F2
	bool del();

	//�����ȴ�ֱ��ȡ��ӵ��Ȩ
	//##ModelId=4519E3F40088
	bool lock();

	//�����Ƿ�ȡ��ӵ��Ȩ,�������Ϸ���
	bool trylock();

	//�ó�ӵ��Ȩ
	//##ModelId=4519E40E003F
	bool unlock();

private:
	//##ModelId=4519E338018C
	wxf_mutex m_mutex;

	//��mutex����������
	char  ch_host[WXF_MAX_NAME];

	//��ʼ����־
	bool  bInit;
};

#endif /*_wxf_lock_h_*/

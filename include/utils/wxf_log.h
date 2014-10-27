/************************************************************************************** 
** Copyright (C) 2011 Multak,Inc. All rights reserved
**
** Filename : wxf_log.h
** Revision : 1.00
**
** Author :  yexf
** Date :  2013/08/20
**
** Description:
**************************************************************************************/ 
#ifndef _wxf_log_h_
#define _wxf_log_h_

#include "wxf_lock.h"
#include "wxf_file.h"

/** @brief             ��־�ļ�Ĭ������(��λ:�ֽ�)*/
#define  MAX_LOG_FILE_SIZE        0x800000

/** @brief              ÿһ��д����־������ֽ���*/
#define EVERY_LOG_MAX_LENGTH		100000


class wxf_log
{
public:

	wxf_log();
	virtual ~wxf_log();

	enum log_level 
	{
		nolog     =      0,          //����¼��־���ļ�
		error     =      1,          // ������־
		warning   =      2,          // ������־
		trace     =      3,          // ������־
	};
	//���������־����
	virtual bool Clear();

	//������־����
	virtual bool SetLogLevel(int nLevel);

	//�����־����
	virtual int  GetLogLevel();

	//������־���·��
	virtual bool SetLogPath(const char * pszPath);

	//������־�ļ���С(������λ:Kb)
	virtual bool SetLogSize(long nSize);


	//���ļ�
	virtual bool Open();
	virtual bool Open(const char * pszFileName);

	//�ر��ļ�
	virtual bool Close();

	//����ָ���ļ����ʽ������
	virtual int  FormatAdd(int nCurLevel,const char* formatString, ...);

	virtual int  Format(int nCurLevel,const char * formatString, va_list argList);
	//����ָ���ļ��������ַ���
	virtual void Add(const char * pchLog,int nCurLevel= wxf_log::trace);

	//����ָ���ĳ��ȼ�¼ָ��������������
	virtual void AddFixLen(const char * pchLog,int nLen,int nCurLevel = wxf_log::trace);


private:
	wxf_file	*m_File;
	void		Flush( const char* data,int nCurLevel);
	void        Flush( const char* data,int nLen,int nCurLevel);

	//����־�������
	int m_nLevel;

	//�ļ���
	char m_FileName[WXF_MAX_NAME];

	//�ļ�·��
	char m_szLogPath[WXF_MAX_NAME];

	//������
	wxf_lock  m_Lock;

	//�ļ���ǰ��С(��λ:�ֽ�)
	long      m_nCurrenSize;

	//�ļ�ָ�����Դﵽ�����ֵ(��λ:�ֽ�)
	long      m_nMaxSize;
};


#ifdef USE_GLOBE_LOG
#include "wxf_singleton.hpp"
class wxf_slog:public wxf::singleton<wxf_slog>
{
public:
	wxf_slog &debug()
	{
		m_iCurLevel = wxf_log::warning;
		return *this;
	}
	wxf_slog &error()
	{
		m_iCurLevel = wxf_log::error;
		return *this;
	}
	wxf_slog &trace()
	{
		m_iCurLevel = wxf_log::trace;
		return *this;
	}
	void add(const char * formatString)
	{
		m_oLog.Add(formatString,m_iCurLevel);
	}
	wxf_log &get_log()
	{
		return m_oLog;
	}
private:
	int m_iCurLevel;
	wxf_log m_oLog;
};

#define GLOG		(wxf_slog::get_mutable_instance())

#endif

#endif /*_wxf_log_h_*/

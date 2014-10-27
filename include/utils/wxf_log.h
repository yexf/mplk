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

/** @brief             日志文件默认容量(单位:字节)*/
#define  MAX_LOG_FILE_SIZE        0x800000

/** @brief              每一次写入日志的最大字节数*/
#define EVERY_LOG_MAX_LENGTH		100000


class wxf_log
{
public:

	wxf_log();
	virtual ~wxf_log();

	enum log_level 
	{
		nolog     =      0,          //不记录日志到文件
		error     =      1,          // 错误日志
		warning   =      2,          // 警告日志
		trace     =      3,          // 跟踪日志
	};
	//清楚所有日志内容
	virtual bool Clear();

	//设置日志级别
	virtual bool SetLogLevel(int nLevel);

	//获得日志级别
	virtual int  GetLogLevel();

	//设置日志存放路径
	virtual bool SetLogPath(const char * pszPath);

	//设置日志文件大小(参数单位:Kb)
	virtual bool SetLogSize(long nSize);


	//打开文件
	virtual bool Open();
	virtual bool Open(const char * pszFileName);

	//关闭文件
	virtual bool Close();

	//按照指定的级别格式化输入
	virtual int  FormatAdd(int nCurLevel,const char* formatString, ...);

	virtual int  Format(int nCurLevel,const char * formatString, va_list argList);
	//按照指定的级别输入字符串
	virtual void Add(const char * pchLog,int nCurLevel= wxf_log::trace);

	//按照指定的长度记录指定缓冲区的内容
	virtual void AddFixLen(const char * pchLog,int nLen,int nCurLevel = wxf_log::trace);


private:
	wxf_file	*m_File;
	void		Flush( const char* data,int nCurLevel);
	void        Flush( const char* data,int nLen,int nCurLevel);

	//该日志输出级别
	int m_nLevel;

	//文件名
	char m_FileName[WXF_MAX_NAME];

	//文件路径
	char m_szLogPath[WXF_MAX_NAME];

	//互斥体
	wxf_lock  m_Lock;

	//文件当前大小(单位:字节)
	long      m_nCurrenSize;

	//文件指定可以达到的最大值(单位:字节)
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

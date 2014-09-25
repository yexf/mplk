//
// define.h
//
// This is a define and include file
//
// Copyright (C) 2006 Jiangjianlin
// Version 1.0
//

#pragma once

#pragma warning(disable : 4786)//屏蔽4786错误

#ifndef __cplusplus
#error "wxf_base 是C++库，不能在C语言中引用"
#endif


// c_runtime lib
#include <stdio.h>  
#include <stdlib.h>  
#include <time.h>
#include <signal.h>

#include <errno.h>
#include <string.h>  
#include <ctype.h>
#include <stddef.h>
#include <stdarg.h>

//standard c++ lib
#include <iostream>
#include <strstream>
#include <string>
#include <iterator>
#include <cassert>
#include <fstream>
#include <list>
#include <deque>
#include <map>
#include <numeric>            
#include <set>
#include <stack>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include <queue>
#include <complex>
#include <cctype>             
#include <new>
#include <cstdarg>             
#include <utility>
#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <cerrno>
#include <csignal>
#include <ctime>

using namespace std;

#ifdef _WIN32
	#include <Winsock2.h>
	#include <process.h>
	#include <conio.h>
	#include <Windows.h>
	#include <io.h>
	#include <direct.h>
	#include <sys/stat.h>
#endif

#include "wxf_define.h"

typedef unsigned int(WXF_STDCALL WXF_THREAD_ROUTINE)(void *);
typedef WXF_THREAD_ROUTINE *			wxf_thdrt;

//socket define
typedef int								sock_len;       //windows下socket的一些api中采用 int
#define SOCKET_SEND_FLAG				0

//lock
typedef CRITICAL_SECTION				wxf_mutex;      //windows下的互斥体类型

//semaphore define
typedef HANDLE							hsem;			//信号量句柄
typedef HANDLE							hnsem;			//无名信号量句柄
	
//handle define
typedef HANDLE							wxf_hdl;		//句柄



//数据类型定义
typedef unsigned char                   uchar;
typedef unsigned short                  ushort;
typedef unsigned int                    uint;
typedef unsigned long					ulong;

typedef char							int8;
typedef unsigned char					uint8;
typedef short							int16;
typedef unsigned short					uint16;
typedef int								int32;
typedef unsigned int					uint32;
typedef long long						int64;
typedef unsigned long long				uint64;
typedef float							float32;
typedef double							float64;



//socket缓冲区结构
typedef struct _buffer
{
	ushort   len;                                           //报文长度
	uchar     buf[MAX_MSGLEN];
}wxf_buffer;

//系统时间结构
typedef struct 
{
	ushort wYear; 
	ushort wMonth; 
	ushort wDayOfWeek; 
	ushort wDayOfYear;
	ushort wDay; 
	ushort wHour; 
	ushort wMinute; 
	ushort wSecond; 
	ushort wMilliseconds; 
}systime;

typedef SYSTEMTIME wxf_time;	

//net 地址结构
typedef struct _netaddr
{
	char	IpAddr[40];                                    //IP地址
	ushort	nPort;                                         //端口号
}netaddr;



/*  全局函数声明  */
extern void GetSysTime(systime & curTime);
extern void CreateSavePath(char * szPath);
extern void GetIniKey(const char *filename,const char *title,const char *key,char *chResult);
extern void EndThread();
extern string GetFormatPath(string sourcePath);
extern string GetFileNameFromFullFileName(string sFullFileName);
extern string GetFilePathFromFullFileName(string sFullFileName);
extern bool PingDestination(const string pIp);
extern WORD ReverseWordEndian(WORD pSourse);
extern int ReverseIntEndian(int pSourse);
extern float ReverseFloatEndian(float pSourse);
extern string FormatGatewayList(string pGatewayA, string pGatewayB);
extern void GetGatewayFromList(string pGatewayList, string& pGatewayA, string& pGatewayB);
extern 	bool GetFileExFileName(const string& pSrcFileName,string& strExtName);


wxf_dir_t win_opendir(const char* dirname);
int win_closedir(wxf_dir_t dir);
wxf_dirent_t win_readdir(wxf_dir_t dir);
int win_entry_is_dir(wxf_dirent_t dirent);
char* win_get_entry_name(void *dirent);
unsigned int win_fsize(wxf_file_t fp);
bool win_fexist(const char *path);

//thread
int wxf_thread_create(wxf_hdl *phandle, ulong *pid, wxf_thdrt routine, void *param);
int wxf_thread_detach(wxf_hdl phandle);
int wxf_thread_join(wxf_hdl phandle, void **pretcode);
void wxf_thread_exit(unsigned int retval);

//mutex
int wxf_mutex_init(wxf_mutex *mutex);
int wxf_mutex_lock(wxf_mutex *mutex);
int wxf_mutex_trylock(wxf_mutex *mutex);
int wxf_mutex_unlock(wxf_mutex *mutex);
int wxf_mutex_destroy(wxf_mutex *mutex);

//no name semaphore
int wxf_noname_sem_init(hnsem *sem, unsigned int value);
int wxf_noname_sem_destroy(hnsem *sem);
int wxf_noname_sem_wait(hnsem *sem);
int wxf_noname_sem_post(hnsem *sem);
int wxf_noname_sem_getvalue(hnsem *sem);

//dynamic linking loader
wxf_hdl wxf_load_library(const char *filename);
void *wxf_get_addr(wxf_hdl handle, const char *module);
int wxf_free_library(wxf_hdl handle);
char *wxf_get_liberror();

//some useful functions
int wxf_sleep(unsigned int milliseconds);
int wxf_getlocaltime(wxf_time *psystime);
int wxf_setlocaltime(const wxf_time *psystime);
int wxf_pathfile_exist(const char *pathfile);
int wxf_pathfile_exec(const char *pathfile);
int wxf_pathfile_rw(const char *pathfile);
int wxf_get_file_size(const char *file_name);
int wxf_create_local_dir(char *szPath);
int wxf_msg_box(const char *fmt,...);

bool wxf_isutf8(const char *str); 
bool IsUTF8(const char * pzInfo);
bool IsGB2312(const char *pzInfo );
int IsGB(char *pText);
bool IsChinese(const char *pzInfo);

#include "wxf_str.h"
#include "wxf_lock.h"
#include "wxf_file.h"
#include "wxf_log.h"
#include "wxf_icov.h"

#ifndef WXF_LIB
#pragma comment(lib,"../lib/wxf_base.lib")
#pragma comment(lib,"ws2_32.lib")
#endif

#ifdef WXF_VLD
#include "vld/vld.h"
#pragma comment(lib,"vld.lib")
#endif
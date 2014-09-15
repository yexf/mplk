/************************************************************************************** 
** Copyright (C) 2011 Multak,Inc. All rights reserved
**
** Filename : wxf_define.h
** Revision : 1.00
**
** Author :  yexf
** Date :  2013/08/22
**
** Description:
**************************************************************************************/ 
#ifndef _wxf_define_h_
#define _wxf_define_h_

#define wxf_err									(-1)
#define wxf_succ								(0)

#ifdef _WIN32
	#define WIN32
	#define WXF_IMPORT								__declspec(dllimport)
	#define WXF_EXPORT								__declspec(dllexport)
	#define WXF_STDCALL								__stdcall		//标准回调
	#define WXF_STDCD								__cdecl			//标准C调用	
#endif

#ifdef WIN32

typedef struct _finddata_t						win_dirent;
typedef struct {
	win_dirent dirent;
	intptr_t dir;
}wxf_dir;

/* TODO: FILE/DIR/DIRENT TYPE DEFINE */
typedef FILE*									wxf_file_t;
typedef wxf_dir*								wxf_dir_t;
typedef win_dirent*								wxf_dirent_t;
/* TODO: END */

/* TODO: SEEK PARAMETER */
#define WXF_SEEK_SET							SEEK_SET
#define WXF_SEEK_END							SEEK_END
#define WXF_SEEK_CUR							SEEK_CUR
/* TODO: END */

/* TODO: FILE OPEN MODE */
#define WXF_OM_READ								"rb"
#define WXF_OM_READWRITE						"rb+"
#define WXF_OM_WRITE							"wb"
#define WXF_OM_CREATE							"wb+"
/* TODO: END */

/* TODO: ERROR RETURN CODE OF FILE OPEN AND DIR OPEN  */
#define WXF_ERR_FILE							(wxf_file_t)(NULL)
#define WXF_ERR_DIR								(wxf_dir_t)(NULL)
#define WXF_ERR_DIRENT							(wxf_dirent_t)(NULL)
#define WXF_EOF									EOF
/* TODO: END */
#endif /* WIN32 */


#ifdef WIN32

#define wxf_printf(...)							printf(__VA_ARGS__)
#define wxf_scanf(...)							scanf(__VA_ARGS__)
#define wxf_assert(ab) 							if (!(ab)){ wxf_printf("assert:");	\
												wxf_printf(" file \"%s\",", __FILE__); \
												wxf_printf(" line %d\n", __LINE__);\
												exit(-1);}


#define wxf_cat(i,j)							i##j
#define wxf_cat3(i,j,k)							i##j##k

#define wxf_max(a,b)							((a)>(b)?(a):(b))
#define wxf_min(a,b)							((a)<(b)?(a):(b))
#define wxf_max3(a,b,c)							wxf_max(wxf_max(a,b),c)
#define wxf_min3(a,b,c)							wxf_min(wxf_min(a,b),c)
#define wxf_anum(arr)							(sizeof(arr) / sizeof(*arr))
#define wxf_bzero(ptr,n)						memset(ptr,0,n)


/* TODO: FILE SYSTEM API */
#define wxf_fopen(fn, om)						fopen((const char *)fn, om)
#define wxf_fcreate(fn)							fopen((const char *)fn, WXF_OM_CREATE)
#define wxf_fclose(fp)							fclose(fp)
#define wxf_fread								fread
#define wxf_fread_succ(fp, buf, size)			(fread(buf, 1, size, (fp)) == size)
#define wxf_fwrite								fwrite
#define wxf_fwrite_succ(fp, buf, size)			(fwrite(buf, 1, size, (fp)) == size)
#define wxf_fseek(fp, offset, opr)				fseek(fp, offset, opr)
#define wxf_ftell(fp)							ftell(fp)
#define wxf_feof(fp)							feof(fp)
#define wxf_opendir(dirname)					win_opendir((const char *)dirname)
#define wxf_closedir(dir)						win_closedir(dir)
#define wxf_readdir(dir)						win_readdir(dir)
#define wxf_isdir(dirent)						win_entry_is_dir(dirent)
#define wxf_getname(dirent)						win_get_entry_name(dirent)
#define wxf_fsize(fp)							win_fsize(fp)
#define wxf_fexist(filename)					win_fexist(filename)

#define wxf_mkdir(dir)							_mkdir(dir)
#define wxf_mkdir_succ(dir)						(_mkdir(dir) == 0)
#define wxf_rmdir(dir)							_rmdir(dir)
#define wxf_rmdir_succ(dir)						(_rmdir(dir) == 0)
#define wxf_fremove(fn)							remove(fn)
#define wxf_fremove_succ(fn)					(remove(fn) == 0)
#define wxf_frename(on, nn)						rename(on, nn)
#define wxf_frename_succ(on, nn)				(rename(on, nn) == 0)


#define wxf_fputc(ch,fp)						fputc((int)ch,fp)
#define wxf_fgetc(fp)							fgetc(fp)
#define wxf_fgets(buf, size, fp)				fgets(buf, size, fp)
#define wxf_fputs(buf, fp)						fputs(buf, fp)
#define wxf_fscanf								fscanf
#define wxf_fprintf								fprintf
#define wxf_sprintf								sprintf
/* TODO: END */

/* TODO: ANSI C API */
#define wxf_strlen(a)							strlen(a)
#define wxf_strchr(a,b)							strchr(a,b)
#define wxf_strcpy(a,b)							strcpy(a,b)
#define wxf_strncpy(a,b,c)						strncpy(a,b,c)
#define wxf_stricmp(a,b)						_stricmp(a,b)
#define wxf_strncmp(a,b,c)						strncmp(a,b,c)
#define wxf_strcmp(a,b)							strcmp(a,b)
#define wxf_strstr(a,b)							strstr(a,b)
#define wxf_strcat(a,b)							strcat(a,b)
#define wxf_sprintf								sprintf

#define wxf_rand(a)								(a * rand() / RAND_MAX)
#define wxf_abs(a)								abs(a)
#define wxf_memset(a,b,c)						memset(a,b,c)
#define wxf_memcpy(a,b,c)						memcpy(a,b,c)
#define wxf_memmove(a,b,c)						memmove(a,b,c)
#define wxf_memcmp(a,b,c)						memcmp(a,b,c)
#define wxf_malloc(a)							malloc(a)
#define wxf_realloc(a,b)						realloc(a,b)
#define wxf_free(a)								free(a)
#define wxf_atoi(a)								atoi(a)
#define wxf_itoa(i,a,f)							itoa(i,a,f)
#define wxf_qsort(b,n,s,f)						qsort(buf,num,size,pf)
/* TODO: END */



#endif /* WIN32 */


#ifdef _BIG_ENDIAN_CPU_
#define SWAP_16(b)\
{\
	unsigned short&a =  *(unsigned short*)(&(b));\
	a = (a>>8)|(a<<8);\
}
#define SWAP_32(b)\
{\
	unsigned &a = *(unsigned *)(&(b));\
	a = (a>>24)|((a>>8)&0xFF00)|((a<<8)&0xFF0000)|(a<<24);\
}
#else
#define SWAP_16(b)
#define SWAP_32(b)
#endif  //_BIG_ENDIAN_CPU_




//msg define
#define MAX_MSGLEN               0x2800                    //SOCKET数据缓冲区最大长度 10240(10K)
#define MAX_SESSION              255                       //最大的会话资源数
#define CREATE_THREAD_NUM        10                        //第一次启动创建个数
#define MAX_LINE_LENGTH          1024                      //文件中一行的最大长度
#define FILE_ERROR_MAX_LEN       255                       //操作文件错误时,错误描述长度
#define FILE_NAME_MAX_LEN        255                       //文件名最大长度

#define MAX_MSGLENGTH            512                       //消息的最大长度
#define MAX_FIELD_NAME_LEN       255                       //表名最大长度
#define MAX_FIELD_VALE_LEN       255                       //字段值最大长度
#define MAX_CONDITION_LEN        512                       //条件最大长度


#define WXF_MAX_NAME			 FILE_NAME_MAX_LEN


/****************************net--socket*********************************/

#define LOCAL_ACT_AS_SERVER	     0                         //Socket作为服务器监听
#define LOCAL_ACT_AS_CLIENT	     1                         //socket作为客户端连接
#define LOCAL_ACT_AS_UNKNOW	    -1                         //socket功能为定义

#define ID_OUTPUT_SEND           0                         //发送
#define ID_OUTPUT_READ           1                         //接收

#define RECVTIME                 1                         //接收时间
#define SENDTIME                 2                         //发送时间
#define BLOCKING                 3                         //阻塞方式
#define UNBLOCKING               4                         //非阻塞方式
#define CONNTIME                 5                         //连接超时时间
#define BROADCAST                6						   //广播方式
#define REUSEADDR				 7						   //端口共用

#define ID_NET                   0                         //网络
#define ID_COM                   1                         //串口
#define ID_RAS                   2                         //modem 
#define ID_OTHERS                3                         //其它

#define TIMEOUT                 -2                         //从socket接收数据超时时返回值




#endif /*_wxf_define_h_*/

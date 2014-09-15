// File.cpp: implementation of the wxf_file class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4275)
#pragma warning(disable : 4996)
#include "wxf_base.h"
#include "wxf_file.h"
//////////////////////////////////////////////////////////////////////////
//                  Class  wxf_file   实现                                    
//////////////////////////////////////////////////////////////////////////
wxf_file::wxf_file()
{
	pfile = NULL;

	memset(m_chError,0,WXF_MAX_NAME);

	memset(m_chName,0,WXF_MAX_NAME);
}

wxf_file::~wxf_file()
{
	if(pfile != NULL)
		fclose(pfile);

	pfile = NULL;
}

/*************************************************************
 函 数 名：Open()
 功能概要：打开文件
 返 回 值: bool  TRUE:成功 false:失败
 参    数：const char * name   文件名称
           uint  nOpenFlags    打开标志
		   #define modeCreate                0          //0000 0000 
           #define modeRead                  1          //0000 0001
           #define modeWrite                 2          //0000 0010
           #define modeReadWrite             4          //0000 0100
		   #define modeNoTruncate            8          //0000 1000
***************************************************************/
bool wxf_file::Open(const char *name, uint nOpenFlags)
{
	if(name == NULL)
	{
		memset(m_chError,0,WXF_MAX_NAME);
		strcpy(m_chError,"File Name Is NULL,Can't Open");
        printf("%s\n",m_chError);
		return false;
	}
	
	//save file name
	if( sizeof(name) > WXF_MAX_NAME)
	{
		memset(m_chError,0,WXF_MAX_NAME);
		sprintf(m_chError,"file Name len =%d,too leng",sizeof(name));
        printf("%s\n",m_chError);
		return false;
	}
	wxf_sprintf(m_chName,"%s",name);
    
	if(pfile != NULL)
	{
		memset(m_chError,0,WXF_MAX_NAME);
		sprintf(m_chError,"the File '%s' has been Opend,please close it Firstly",name);
		printf("%s\n",m_chError);
		return false;
	}
    
	switch(nOpenFlags)
	{
	case 0:    
		/*Opens an empty file for both reading and writing. If the given file 
		  exists, its contents are destroyed; creates the file first if it 
		  doesn’t exist. */
		pfile = fopen(name,"w+b");
		break;
	case 1:      
		/*Opens for reading. If the file does not exist or cannot be found, 
		  the fopen call fails*/
		pfile = fopen(name,"rb");
		break;
	case 2:
	case 8:
		/*Opens for writing at the end of the file (appending) without removing 
		  the EOF marker before writing new data to the file; creates the file 
		  first if it doesn’t exist*/
		pfile = fopen(name,"ab");
		break;
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		/*Opens for both reading and writing. (The file must exist.) */
		pfile = fopen(name,"r+b");
		break;
	default:
		/*Opens for reading and appending; the appending operation includes the 
		  removal of the EOF marker before new data is written to the file and 
		  the EOF marker is restored after writing is complete; creates the file 
		  first if it doesn’t exist*/
		pfile = fopen(name,"a+b");
		break;
	}

	if(pfile == NULL)
	{
		memset(m_chError,0,WXF_MAX_NAME);
		sprintf(m_chError,"open %s failed,reason:%s(errno:%d)",name,strerror(errno),errno);
        printf("%s\n",m_chError);
		return false;
	}
	return true;
}

/*************************************************************
 函 数 名：Read()
 功能概要：从文件中读取指定的字节数目
 返 回 值: uint 实际读取到的字节数
 参    数：void * lpbuf  存放读取到数据的缓冲区
           uint   nCount 读取的字节数,实际返回值可能小于该值

***************************************************************/
uint wxf_file::Read(void * lpbuf, uint nCount)
{
	uint nRead = 0;
	if(pfile == NULL) 
	{
        memset(m_chError,0,WXF_MAX_NAME);
		sprintf(m_chError,"read from %s failed because the file handler is NULL",m_chName);
		printf("%s\n",m_chError);
		return 0;
	}

	if(!feof(pfile))
	{
		//if( fgets((char*)lpbuf,nCount,pfile) == NULL)
		nRead = fread((char*)lpbuf,sizeof(char),nCount,pfile);
		if(ferror(pfile))
		{
			sprintf(m_chError,"read from %s error,reason:%s",m_chName,strerror(errno));
			printf("%s\n",m_chError);
			Flush();
			return nRead;
		}
		
		//读与写进行转换时需要增加一个fflush或fseek或fsetpos
		//Flush();
		return  nRead;//(strlen((char*)lpbuf)+1);	
	}
    
	//file pointer is end
	sprintf(m_chError,"the file %s's pointer has been eof,can't read",m_chName);
	printf("%s\n",m_chError);
	return 0;
}

/*************************************************************
 函 数 名：Write()
 功能概要：向文件中写入指定数目的数据
 返 回 值: uint 实际写入文件中的数据数目
 参    数：const void * lpbuf  要写入的数据缓存区
           uint  nCount        要写入的数据的数目
***************************************************************/
uint wxf_file::Write(const void * lpBuf,uint nCount)
{
	uint nwrite = 0;

	if(pfile == NULL) 
	{
        memset(m_chError,0,WXF_MAX_NAME);
		sprintf(m_chError,"write to %s failed because the file handler is NULL",m_chName);
		printf("%s\n",m_chError);
		return 0;
	}

	//write data
    nwrite = fwrite(lpBuf,sizeof(char),nCount,pfile);
	if( int n=ferror(pfile) )
	{
		sprintf(m_chError,"write to %s error,reason:%s",m_chName,strerror(errno));
	    printf("%s\n",m_chError);
		Flush();
		return nwrite;
	}
		
	//读与写进行转换时需要增加一个fflush或fseek或fsetpos
	Flush();
	return nwrite;	
} 

/*************************************************************
 函 数 名：Flush()
 功能概要：强制将缓存数据写入文件
 返 回 值: bool
 参    数：无
***************************************************************/
bool wxf_file::Flush()
{
	if(pfile == NULL) 
	{
		sprintf(m_chError,"fflush %s failed because the file handler is NULL",m_chName);
		printf("%s\n",m_chError);
		return false;
	}

	if(fflush(pfile) == 0)
		return TRUE;
	else{
	    sprintf(m_chError,"fflush %s error,reason:%s",m_chName,strerror(errno));
		printf("%s\n",m_chError);
	}
	return false;
}

/*************************************************************
 函 数 名：Close()
 功能概要：关闭文件
 返 回 值: bool
 参    数：无
 修改历史：
***************************************************************/
bool wxf_file::Close()
{
	if(pfile == NULL) 
	{
		return TRUE;
	}

	if(fclose(pfile) == 0)
	{
		pfile = NULL;
		return TRUE;
	}
	else{
	    sprintf(m_chError,"fclose %s error,reason:%s",m_chName,strerror(errno));
		printf("%s\n",m_chError);
	}
	return false;
}

/*************************************************************
 函 数 名：Seek()
 功能概要：移动文件指针到指定的位置
 返 回 值: bool  成功或失败
 参    数：long lOff   偏移量
           uint nFrom  偏移起始地址
***************************************************************/
bool wxf_file::Seek(long lOff,uint nFrom)
{
	int nFlag = 0;
	if(pfile == NULL) 
	{
		sprintf(m_chError,"fseek %s failed because the file handler is NULL",m_chName);
		printf("%s\n",m_chError);
		return false;
	}
    
	//判断position
	switch(nFrom)
	{
	case wxf_file::begin:
		nFlag = SEEK_SET;
		break;
	case wxf_file::current:
		nFlag = SEEK_CUR;
		break;
	case wxf_file::end:
		nFlag = SEEK_END;
		break;
	default:
		sprintf(m_chError,"fseek %s failed because the second argument is error",m_chName);
		printf("%s\n",m_chError);
		return false;
	}
      
	//fseek
	if(fseek(pfile,lOff,nFlag) == 0)
		return TRUE;
	else{
		sprintf(m_chError,"fseek %s %ld failed,reason:%s",m_chName,lOff,strerror(errno));
        printf("%s\n",m_chError);
	}
	return false;
}

/*************************************************************
 函 数 名：SeekToBegin()
 功能概要：移动文件指针到文件的起始地址
 返 回 值: bool
 参    数：无
***************************************************************/
bool wxf_file::SeekToBegin()
{
	if(pfile == NULL) 
	{
		sprintf(m_chError,"fseek to begien %s failed because the file handler is NULL",m_chName);
		printf("%s\n",m_chError);
		return false;
	}

	//set file position to begin
    if(fseek(pfile,0,SEEK_SET) == 0)
		return TRUE;
	else{
		sprintf(m_chError,"fseek to begin %s error,reason:%s",m_chName,strerror(errno));
        printf("%s\n",m_chError);
	}
	return false;
}

/*************************************************************
 函 数 名：SeekToEnd()
 功能概要：移动文件指针到文件结束位置
 返 回 值: bool
 参    数：无
***************************************************************/
bool wxf_file::SeekToEnd()
{
	if(pfile == NULL) 
	{
		sprintf(m_chError,"fseek to end %s failed because the file handler is NULL",m_chName);
		printf("%s\n",m_chError);
		return false;
	}

	//set file position to end
    if(fseek(pfile,0,SEEK_END) == 0)
		return TRUE;
	else{
		sprintf(m_chError,"fseek to end %s error,reason:%s",m_chName,strerror(errno));
        printf("%s\n",m_chError);
	}

	return false;
}

/*************************************************************
 函 数 名：GetLength()
 功能概要：获得文件总长度
 返 回 值: long 文件的总字节数
 参    数：无
***************************************************************/
long wxf_file::GetLength()
{
	long num = 0;

	int r=0;

	//用于保存文件当前位置的变量
	fpos_t  curpos;

	if(pfile == NULL) 
	{
		wxf_sprintf(m_chError,"get %s length failed because the file handler is NULL",m_chName);
		printf("%s\n",m_chError);
		return -1;
	}
    
	//保存当前文件位置
	if( fgetpos( pfile, &curpos ) != 0 )
	{
		wxf_sprintf(m_chError,"get file cur position failed when get %s's length,\
               rason:%s",m_chName,strerror(errno));
		printf("%s\n",m_chError);
		return -1;		    
	}

    //移动文件到首部
	if( !SeekToBegin() )
	{
		return -1;
	}

	// 获得首位置偏移
	long n_begin = ftell(pfile);

	// 移动到文件尾部
	if( !SeekToEnd() )
	{
		fsetpos(pfile,&curpos);

		return -1;
	}

	// 获得尾位置偏移
	long n_end = ftell(pfile);

	// 获得文件长度
	num = n_end - n_begin;

	//移动文件指针到原来位置
	if( fsetpos(pfile,&curpos) != 0)
	{
		sprintf(m_chError,"set file cur position failed when get %s's length,\
               rason:%s",m_chName,strerror(errno));
		printf("%s\n",m_chError);
		return num;		  
	}

	return num;
}	

/*************************************************************
 函 数 名：GetError()
 功能概要：获得错误原因
 返 回 值: char * 错误描述
 参    数：无
***************************************************************/
char *wxf_file::GetError()
{
	return m_chError;
}

#pragma once

#include <stdio.h>

class wxf_file
{
public:
	wxf_file(void);
	~wxf_file(void);

	enum OpenFlags{
		//file mode
		modeCreate         =       0  ,         //0000 0000
		modeRead           =       1  ,         //0000 0001
		modeWrite          =       2  ,         //0000 0010
		modeReadWrite      =       4  ,         //0000 0100
		modeNoTruncate     =       8  ,         //0000 1000
		shareDenyNone      =       16 ,         //0001 0000
		shareDenyWrite     =       32 ,         //0010 0000
		shareDenyRead      =       64 ,         //0100 0000
		shareExclusive     =       128,         //1000 0000
	};

	enum PosFlags{
		//file position 
		begin                  =   0,          //文件开始
		current                =   1,          //文件当前位置
		end                    =   2,         //文件尾部
	};

	/* Input/Output */

	//打开文件
	bool Open(const char *, uint nOpenFlags);

	//读文件
	uint Read(void * lpbuf, uint nCount);

	//写文件
	uint Write(const void * lpBuf,uint nCount);

	//强制写入
	bool Flush();

	//关闭文件
	bool Close();

	/*position */

	bool Seek(long lOff,uint nFrom);

	bool SeekToBegin();

	bool SeekToEnd();

	long GetLength();

	/* 错误描述 */
	char * GetError();

protected:

private:

	//文件指针
	FILE * pfile ;

	char   m_chError[WXF_MAX_NAME];

	char   m_chName[WXF_MAX_NAME];
};


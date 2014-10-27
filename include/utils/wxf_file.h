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
		begin                  =   0,          //�ļ���ʼ
		current                =   1,          //�ļ���ǰλ��
		end                    =   2,         //�ļ�β��
	};

	/* Input/Output */

	//���ļ�
	bool Open(const char *, uint nOpenFlags);

	//���ļ�
	uint Read(void * lpbuf, uint nCount);

	//д�ļ�
	uint Write(const void * lpBuf,uint nCount);

	//ǿ��д��
	bool Flush();

	//�ر��ļ�
	bool Close();

	/*position */

	bool Seek(long lOff,uint nFrom);

	bool SeekToBegin();

	bool SeekToEnd();

	long GetLength();

	/* �������� */
	char * GetError();

protected:

private:

	//�ļ�ָ��
	FILE * pfile ;

	char   m_chError[WXF_MAX_NAME];

	char   m_chName[WXF_MAX_NAME];
};


#pragma warning(disable : 4275)
#pragma warning(disable : 4996)
#include "wxf_base.h"

#ifdef WIN32

#define wxf_get_entry_name				wxf_getname
#define wxf_entryisdir					wxf_isdir
#define wxf_app_free					wxf_free
#define wxf_app_malloc					wxf_malloc

#define WXF_ASSERT						wxf_assert

/********************************************************************************************* 
** Function name  : win_opendir
** Arguments      : dirname - 
** Return         : wxf_dir_t 
** Date&Time      : 2012-09-05  10:05:14
** Description    : 
*********************************************************************************************/ 
wxf_dir_t win_opendir(const char* dirname)
{
	wxf_dir_t ret;

	if(_chdir(dirname))
	{
		printf("Unable to locate the directory: %s\n", dirname);
		return NULL;
	}
	else 
	{
		ret = (wxf_dir_t)malloc(sizeof(wxf_dir));
		/* Find first in the current directory */
		ret->dir =  _findfirst("*.*", &(ret->dirent)); 
		return ret;
	}
}

/********************************************************************************************* 
** Function name  : win_closedir
** Arguments      : dir - 
** Return         : int 
** Date&Time      : 2012-09-05  10:05:17
** Description    : 
*********************************************************************************************/ 
int win_closedir(wxf_dir_t dir)
{
	_findclose(dir->dir); 
	free(dir);
	return 0;
}

/********************************************************************************************* 
** Function name  : win_readdir
** Arguments      : dir - 
** Return         : wxf_dirent_t 
** Date&Time      : 2012-09-05  10:05:18
** Description    : 
*********************************************************************************************/ 
wxf_dirent_t win_readdir(wxf_dir_t dir)
{	
	if (_findnext(dir->dir, &(dir->dirent)) == 0)
		return &(dir->dirent);
	else
		return NULL;
}

/********************************************************************************************* 
** Function name  : win_entry_is_dir
** Arguments      : dirent - 
** Return         : int 
** Date&Time      : 2012-09-05  10:05:20
** Description    : 
*********************************************************************************************/ 
int win_entry_is_dir(wxf_dirent_t dirent)
{
	struct _finddata_t* entry = (struct _finddata_t*)dirent;

	if (entry->attrib & _A_SUBDIR)
		return true;
	else
		return false;
}

/********************************************************************************************* 
** Function name  : win_get_entry_name
** Arguments      : dirent - 
** Return         : char* 
** Date&Time      : 2012-09-05  10:05:21
** Description    : 
*********************************************************************************************/ 
char* win_get_entry_name(void *dirent)
{
#ifdef _WIN32
	struct _finddata_t* entry = (struct _finddata_t*)dirent;
	return entry->name;
#else
	return dirent->d_name;
#endif
}

/********************************************************************************************* 
** Function name  : win_fsize
** Arguments      : fp - 
** Return         : unsigned int 
** Date&Time      : 2012-09-05  10:05:23
** Description    : 
*********************************************************************************************/ 
unsigned int win_fsize(wxf_file_t fp)
{
	unsigned int size,temp;
	unsigned int pos = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	temp = ftell(fp); 
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, pos, SEEK_SET);
	return size - temp;
}
int win_fexist(const char *path)
{
	wxf_dir_t dir;
	wxf_file_t fp;

	fp = wxf_fopen(path,WXF_OM_READ);
	if (fp != WXF_ERR_FILE)
	{
		wxf_fclose(fp);
		return true;
	}
	dir = wxf_opendir(path);
	if (dir != WXF_ERR_DIR)
	{
		wxf_closedir(dir);
		return true;
	}
	return false;
}

#endif


#ifdef WIN32

//thread

//0 success; -1 error
int wxf_thread_create(wxf_hdl *phandle, ulong *pid, wxf_thdrt routine, void *param)
{
	unsigned int dwId;
	
	wxf_hdl h;
	
	h = (wxf_hdl)_beginthreadex(0, 0, routine, param, 0, &dwId);
	
	if(h == (wxf_hdl)-1)
		return -1;

	if(phandle)
	{
		*phandle = h;
		if (pid)
			*pid = dwId;

		return 0;
	}
	else
	{
		CloseHandle(h);

		return -1;
	}
}

//0 success; -1 error
int wxf_thread_detach(wxf_hdl phandle)
{
	CloseHandle(phandle);
	return 0;
}

//0 success; -1 error
int wxf_thread_join(wxf_hdl phandle, void **pretcode)
{
	if(WaitForSingleObject(phandle, INFINITE) == WAIT_OBJECT_0)
	{
		if(pretcode)
			GetExitCodeThread(phandle, (unsigned long*)pretcode);
		
		return 0;
	}

	return -1;
}

void wxf_thread_exit(unsigned int retval)
{
	_endthreadex(retval);
}


//mutex

//0 success; other error
int wxf_mutex_init(wxf_mutex *mutex)
{
	InitializeCriticalSection(mutex);
	return 0;
}

//0 success; other error
int wxf_mutex_lock(wxf_mutex *mutex)
{
	EnterCriticalSection(mutex);
	return 0;
}

//0 success; other error
int wxf_mutex_trylock(wxf_mutex *mutex)
{
	TryEnterCriticalSection(mutex);
	return 0;
}

//0 success; other error
int wxf_mutex_unlock(wxf_mutex *mutex)
{
	LeaveCriticalSection(mutex);
	return 0;
}

//0 success; other error
int wxf_mutex_destroy(wxf_mutex *mutex)
{
	DeleteCriticalSection(mutex);
	return 0;
}


//no name semaphore

//0 success; -1 error
int wxf_noname_sem_init(hnsem *sem, unsigned int value)
{
	*sem = CreateSemaphore(0, value, 0x7FFFFFFF, NULL);
	if(*sem == NULL)
		return -1;

	return 0;
}

//0 success; -1 error
int wxf_noname_sem_destroy(hnsem *sem)
{
	if(CloseHandle(*sem))
		return 0;
	return -1;
}

//0 success; -1 error
int wxf_noname_sem_wait(hnsem *sem)
{
	int ret = WaitForSingleObject(*sem, INFINITE);

	if(ret == WAIT_OBJECT_0)
		return 0;
	else
		return -1;
}

//0 success; -1 error
int wxf_noname_sem_post(hnsem *sem)
{
	if(ReleaseSemaphore(*sem, 1, 0))
		return 0;
	
	return -1;
}

//0 success; -1 error
int wxf_noname_sem_getvalue(hnsem *sem)
{
	/*
	long value=0;

	if(ReleaseSemaphore(*sem, 0, &value))
		return -1;
	else
		return value;
		*/
	return 0;
}

//dynamic linking loader

//0 success; -1 error
wxf_hdl wxf_load_library(const char *filename)
{
	wxf_hdl handle;

	if (filename == NULL)
	{
		return NULL;
	}

	handle=LoadLibrary(filename); 

	return handle;
}

//0 success; -1 error
void *wxf_get_addr(wxf_hdl handle, const char *module)
{
	void *ptr=NULL; 
    
    if (handle==NULL || module==NULL)
    { 
        return NULL; 
    }
    
    ptr=(void *)GetProcAddress((HINSTANCE)handle, module); 

	return ptr;
}

//0 success; -1 error
int wxf_free_library(wxf_hdl handle)
{
	if (handle == NULL)
	{
		return -1;
	}
	
    FreeLibrary((HINSTANCE)handle);

	return 0;
}

//NULL�޴���,�����д���
char *wxf_get_liberror()
{
	/*
	* ----- commented by qingch   4/8/2009 ------
  	return NULL;
	*/
	return strerror(errno);
}

//some useful functions

//0 success; other error
int wxf_sleep(unsigned int milliseconds)
{
	Sleep(milliseconds);
	return 0;
}

//0 success; -1 error
int wxf_getlocaltime(wxf_time *psystime)
{
	GetLocalTime(psystime);
	return 0;
}

//0 success; -1 error
int wxf_setlocaltime(const SYSTEMTIME *psystime)
{
	if(SetLocalTime(psystime))
		return 0;

	return -1;
}

//0 yes; -1 no
int wxf_pathfile_exist(const char *pathfile)
{
	if( _access(pathfile,0) != 0)
		return -1;
	
	return 0;
}

//0 yes; -1 no
int wxf_pathfile_exec(const char *pathfile)
{
	if( _access(pathfile,2) != 0)
		return -1;
	
	return 0;
}

//0 yes; -1 no
int wxf_pathfile_rw(const char *pathfile)
{
	if( _access(pathfile,6) != 0)
		return -1;

	return 0;
}

int wxf_get_file_size(const char *file_name)
{
	struct _stat file_desc;

	if (_stat(file_name, &file_desc) == 0)
	{
		return file_desc.st_size;
	}

	return -1;
}

int wxf_create_local_dir(char *szPath)  
{
	int			iIndex1=0,iIndex2=0;
	string		strFilePath;
	string		strFullFilePath;

	strFilePath = szPath;
	if (strFilePath.at(0) == '.')
	{
		iIndex2=strFilePath.find('\\', iIndex1+1);

		if(iIndex2==-1)
		{
			return FALSE;
		}

		iIndex1=iIndex2;
		strFullFilePath=".";
	}
	else 
	{
		if (wxf_pathfile_exist(strFilePath.c_str())==0)
		{
			return TRUE;
		}
	}

	while(iIndex1!=-1)
	{
		iIndex2=strFilePath.find('\\', iIndex1+1);

		if (iIndex2>0)
		{
			strFullFilePath += strFilePath.substr(iIndex1, iIndex2-iIndex1);

			if (wxf_pathfile_exist(strFullFilePath.c_str())!=0)
			{
				if (_mkdir((const char *)strFullFilePath.c_str())<0)
				{
					return FALSE;
				}
			}
		}
		else if(iIndex2==-1)
		{
			if((unsigned int)iIndex1!=strFilePath.size()-1)
			{
				strFullFilePath += strFilePath.substr(iIndex1, strFilePath.size()-1);
					
				if (wxf_pathfile_exist(strFullFilePath.c_str())!=0)
				{
					if (_mkdir((const char *)strFullFilePath.c_str())<0)
					{
						return FALSE;
					}
				}
			}
		}

		iIndex1=iIndex2;
	}

	return TRUE;
}
int wxf_msg_box(const char *fmt,...)
{
	// if the format string is NULL ,return 
	if (fmt == NULL)
	{
		return -1;
	}
		
	va_list argList;

	// Set va_list to the beginning of optional arguments
	va_start(argList, fmt);

	const char * ptr = fmt;
	char * str = NULL;

	//save the max len of the formatstring
	int    nMaxLen = 0;

	while(*ptr != '\0')
	{
		str = NULL;

		if(*ptr == '%')
		{
			switch(*(ptr+1))
			{
			case 's':
			case 'S':
				str = va_arg(argList,char*);

				if( NULL == str)
					nMaxLen ++;
				else
					nMaxLen += strlen(str);
				ptr++;
				break;

			case 'c':
			case 'C':
				va_arg(argList,char);
				nMaxLen +=2;
				ptr++;
				break;

			case 'd':
			case 'D':
				va_arg(argList, int);
				nMaxLen +=11;
				ptr++;
				break;

			case 'u':
			case 'U':
				va_arg(argList, unsigned int);
				nMaxLen +=10;
				ptr++;
				break;

			case 'l':
			case 'L':
				ptr++;
				if(*(ptr+1) == 'd')
				{
					va_arg(argList, long);
					nMaxLen +=11;
				}
				else if(*(ptr+1) == 'u')
				{
					va_arg(argList, unsigned long);
					nMaxLen +=10;
				}
				ptr++;
				break;
			case 'f':
			case 'F':
				va_arg(argList, double);
				nMaxLen += 31;
				ptr++;
				break;
			case 'x':
			case 'X':
				va_arg(argList, void*);
				nMaxLen += 2*sizeof(void*);
				ptr++;
				break;
			default:
				nMaxLen+=1;
			}
		} //  if(*ptr == '%')
		else
		{
			nMaxLen +=1;
		}
		// Increment pointer..
		ptr++;
	}

	// end va_list
	va_end(argList);


	nMaxLen += 255;       // ��ֹ����������ȼ������
    char *buf = new char[nMaxLen]; 


	va_start(argList, fmt);  	
	vsprintf(buf, fmt, argList);	
	va_end(argList);
	
	MessageBox(NULL,buf,"",MB_OK);

	return nMaxLen;
}
#endif//OS_WINDOW


#ifdef WIN32


/*  ȫ�ֺ���ʵ��  */

/*************************************************************
 �� �� ����GetSysTime()
 ���ܸ�Ҫ�����ϵͳ��ǰʱ��
 �� �� ֵ: void
 ��    ����SYSTIME& curTime
***************************************************************/
void GetSysTime(systime & curTime)
{
	//��ű���ʱ��
	time_t ltime=0;

	//tm �ṹ
	struct tm *today = NULL;

	//ȡ�ñ���ʱ��
	time(&ltime);

	//ʱ��ת��Ϊtm����
	today=localtime(&ltime); 
	
    //ȡֵ
	curTime.wYear      = today->tm_year + 1900;  //tm ��year ����1900
	curTime.wMonth     = today->tm_mon +1 ;      //tm ��month ����0
	curTime.wDay       = today->tm_mday;
	curTime.wHour      = today->tm_hour;
	curTime.wMinute    = today->tm_min ;
	curTime.wSecond    = today->tm_sec ;
	curTime.wDayOfWeek = today->tm_wday ;        //sunday is 0 ,monday is 1 ,and so on
    curTime.wDayOfYear = today->tm_yday + 1;     //tm�л���0��ʼ

	//��֧��ȥ����,��0
	curTime.wMilliseconds = 0;
}
/*************************************************************
 �� �� ����CreateSavePath()
 ���ܸ�Ҫ: ����·��
 �� �� ֵ: void
 ��    ����char * szPath   ·����
***************************************************************/
void CreateSavePath(char * szPath)  
{
	wxf_create_local_dir(szPath);
}
/*************************************************************
 �� �� ����GetIniKey()
 ���ܸ�Ҫ: ���ָ�������ļ���ָ���ؼ��ֵ�ֵ
 �� �� ֵ: void
 ��    ����const char * filename  �����ļ�����
           const * title     ��Ŀ����
		   const * key       �ؼ�������
		   const * chResult  ȡ�õĽ��
***************************************************************/
void GetIniKey(const char *filename, const char *title,
				  const char *key,char *chResult)
{
	if(chResult == NULL)
	{
		fprintf(stderr,"the last parameter is NULL in GetIniKey()");
		return ;
	}

	//�ļ�ָ��
	FILE * fp = NULL;

	//��ʱ����
	char tmpLine[1024]="";         // ÿ������ַ���
	char every_line[1024]="";
	char tempKey[255]="";          // ������ʱ�ؼ���
	char tmpTitle[255]="";         // ������ʷ��Ŀ����
	char * pEveryLine = NULL;
   	char * pResult = NULL;

	//��ȡ�ļ�����ֵ
	char * pReturn = NULL;

	int i=0,j=0,k=0,nLen = 0;

	//��־һ����Ŀ�µ�key�Ƿ��Ѿ�ɨ����
	int flag = 0;

	char * tmp=NULL;
	
	//���ļ�
	if ((fp = fopen( filename, "r")) == NULL )
	{
		fprintf(stderr,"Open File %s Failed,Reason:%s \n",filename,strerror(errno));
		return;
	}

	//��ȡ����
	while (!feof(fp))
	{
		memset(tmpLine,0,1024);
		memset(every_line,0,1024);

		pReturn = fgets(every_line,1023,fp );    //��ȡһ��

		if ( pReturn == NULL )                //�ļ�������������
		{  
			break; 
		}
		
		nLen = strlen(every_line);
        pEveryLine = every_line;
        j = 0;

		// ȥ�����׿ո�(����һ����Ϊ�ո��ַ�ǰ�����пո�ȥ��,)
		for(i =0; i<nLen; i++)
		{
			if(every_line[i] == 0x20)
			{
				j++;         // �ո���Ŀ
			}
			else             // ���ֲ�Ϊ�ո�ʱ���˳���ѭ��
				break;
		}
        
		memcpy(tmpLine,pEveryLine+j,nLen-j);  // ���Ƴ��ո�����ַ�
        nLen = nLen - j;                      // �������ó���

		// ����ע���л����
		if( ( tmpLine[0] == ';') ||                           // ';'��ע�� 
		    ((tmpLine[0] == '/') && (tmpLine[1] == '/')) ||   // '//'ע��
		    ((tmpLine[0] == '/') && (tmpLine[1] == '*')) ||   // '/*'ע��
			((tmpLine[0] == '-') && (tmpLine[1] == '-')) ||   // '--'ע��
			((tmpLine[0] == '\r')&& (tmpLine[1] == '\n'))||   // windows �»��з�(�޿ո�Ŀ���)
			 (tmpLine[0] =='\n') )                            // linux/unix�»��з� (�޿ո�Ŀ���)
		{
			continue;
		}	

        // ȥ�����з�
		if( (nLen >= 1) && (tmpLine[nLen-1] == '\n') )      // windows\linux�»��з����Ϊ'\n'
			tmpLine[nLen-1] = 0;
		if( (nLen >= 2) && (tmpLine[nLen-2] == '\r') )      //  windows�»��з�Ϊ��'\n'֮ǰ���и�'\r'
			tmpLine[nLen-2] = 0;

		tmp = strchr(tmpLine, '=');
		
		//��һ�ζ��� '=' Ӧ���� ��Ŀ����
		if (( tmp != NULL )&&(flag == 1))
		{
			i= j = 0;
			memset(tempKey,0,255);

			//ȡ���ؼ���
			while(tmpLine[j] != '=')
			{
				if(tmpLine[j] != 0x20)                        //ȥ���ո�
				{
					tempKey[i]=tmpLine[j];
					i++;

					if(i > 254)
						break;
				}
				j++;
			}
			
	    	//����ؼ�����ͬ��ȡ��ֵ (ȥ������еĿո�)
			if(strcmp(tempKey,key) == 0)
			{
				nLen = strlen(tmp) - 1;
				tmp = tmp + 1;          // ���� '='��
                pResult = chResult;

				while( nLen > 0)
				{
					if( (*tmp) != 0x20 )
					{
						*pResult = *tmp;
						pResult ++;
					}

                    tmp ++;
					nLen --;
				}

				fclose ( fp );        // �ر��ļ�

				return ;
			}
		}
		else
		{
			//���flag=1,��˵��ǰһ��title�µ�key�Ѿ�����
			if( flag == 1)
			{
				fprintf(stderr,"Section=[%s],Key=%s not exist in %s\n", \
					   title,key,filename);
				wxf_bzero(chResult,strlen(chResult));
				fclose(fp);
				return ;
			}
			memset(tmpTitle,0,255);

			// ������Ŀ����
			strcpy(tmpTitle,"[");
			strcat(tmpTitle,title);
			strcat(tmpTitle,"]");
			
			// ȷ�϶�������Ϣ�Ƿ�����Ŀ����
			if( strstr(tmpLine,tmpTitle) != NULL )
			{
				flag = 1;   // ��ʶ�ҵ�title
			}
		}
	}

	fclose ( fp );

	fprintf(stderr,"Section=[%s],Key=%s not exist in %s\n",\
		    title,key,filename);
	wxf_bzero(chResult,strlen(chResult));
	return;
}

/*************************************************************
 �� �� ����EndThread()
 ���ܸ�Ҫ: �����߳�
 �� �� ֵ: void
 ��    ������
***************************************************************/
void EndThread()
{
	unsigned long code=0;
	ExitThread(code);
}

/*************************************************************
 �� �� ��:   GetFormatPath
 ���ܸ�Ҫ:   ��ø�ʽ�����ļ�·���������е�б��ͳһΪ'/',���û��'/'
			ʱ����'/'
 �� �� ֵ:   std::string ��ʽ�����·��
 ��    ��:   string sourcePath ���ʽ����·��
**************************************************************/
string GetFormatPath(string sourcePath)
{
	string::size_type stPos(0);
	//
	while(string::npos != (stPos = sourcePath.find('\\',stPos))){
		sourcePath.replace(stPos,1,"/");
	}

	if((!sourcePath.empty()) && ((*(sourcePath.end()-1) != '/')))
	{
		sourcePath += '/';
  	}
	return sourcePath;
}

/*************************************************************
 �� �� ��:   GetFileNameFromFullFileName
 ���ܸ�Ҫ:   �Ӵ�·�����ļ����л�ȡ�ļ���
 �� �� ֵ:   std::string	����·�����ļ���
 ��    ��:   string sFullFileName	��·�����ļ���
**************************************************************/
string GetFileNameFromFullFileName(string sFullFileName)
{
	string::size_type n = sFullFileName.find_last_of('\\', sFullFileName.size());
	if (string::npos != n){
		sFullFileName.erase(0, n+1);
	}

	n = sFullFileName.find_last_of('/', sFullFileName.size());
	if (string::npos != n){
		sFullFileName.erase(0, n+1 );
	}
	return sFullFileName;
}

/*************************************************************
 �� �� ��:   GetFilePathFromFullFileName
 ���ܸ�Ҫ:   ��ȫ�ļ�������·���ģ��л�ȡ�ļ�·������ͳһΪ'/'
 �� �� ֵ:   std::string	�������ļ�·��
 ��    ��:   string sFullFileName	ȫ�ļ���
**************************************************************/
string GetFilePathFromFullFileName(string sFullFileName)
{
	string sPath;
	string tFullFileName(sFullFileName);
	//��ʽ�� ��'\'�滻Ϊ'/'
	string::size_type stPos(0);
	while(string::npos != (stPos = tFullFileName.find('\\',stPos))){
		tFullFileName.replace(stPos,1,"/");
	}
	
	string::size_type n = tFullFileName.find_last_of('/', tFullFileName.size());
	if (string::npos != n){
		sPath.assign(sFullFileName,0,n+1);
	}

	return sPath;
}

bool PingDestination(const string pIp)
{
    char chPingFile[256];
    memset(chPingFile,0,256);
    sprintf(chPingFile,"ping(%s).txt",pIp.c_str());
    
    char chBuff[256] = "";
#ifdef OS_WINDOWS
    sprintf(chBuff,"ping  %s -n 3 -w 5000>%s",pIp.c_str(),chPingFile);
    system(chBuff);
#endif

#ifdef OS_LINUX
    FILE   *stream;
    FILE   *wstream;
    char   buf[8192];
    
#ifdef OS_SUNUNIX
    sprintf(chBuff,"ping -s -I 1 %s 1024 3",pIp.c_str());   
#elif defined OS_AIX
    sprintf(chBuff,"ping -c 3 -w 5 %s",pIp.c_str());  
#elif defined OS_HPUNIX
    sprintf(chBuff,"ping %s -n 3 -m 5",pIp.c_str());  	
#else
    sprintf(chBuff,"ping %s -c 3",pIp.c_str());  
#endif
    
    memset( buf, '\0', sizeof(buf) );//��ʼ��buf,�������д�����뵽�ļ���
    stream = popen( chBuff, "r" ); //����ls ��l���������� ͨ���ܵ���ȡ����r����������FILE* stream
    if (stream==NULL)  
	{
		return false;
    }
	wstream = fopen(chPingFile, "w+"); //�½�һ����д���ļ�
    if (wstream==NULL)  
	{
		pclose(stream);
		return false;
    }	
    fread( buf, sizeof(char), sizeof(buf), stream); //���ո�FILE* stream����������ȡ��buf��
    fwrite( buf, 1, sizeof(buf), wstream );//��buf�е�����д��FILE    *wstream��Ӧ�����У�Ҳ��д���ļ���
    pclose( stream );  
    fclose( wstream );
#endif
    FILE   *pFile=NULL;
    pFile = fopen(chPingFile, "r");
    if (pFile == NULL)
    {
        return false;
    }
    
    char szTempBuff[256];
    int nNum=0;
    while (fgets(szTempBuff, 256, pFile) != NULL)
    {
        if (strstr(szTempBuff, "time=")!=NULL||
			strstr(szTempBuff, "time<")!=NULL||
			strstr(szTempBuff, "ʱ��=")!=NULL||
			strstr(szTempBuff, "ʱ��<")!=NULL )
        {
            nNum=nNum+1;
        }
    }
    fclose(pFile);
    pFile=NULL;
    remove(chPingFile);
    if(nNum>=2)
    {
        return true;
    }
    else
    {
        return false;
	}
}

WORD ReverseWordEndian(WORD pSourse)
{
    SWAP_16(pSourse);
    return pSourse;
}

int ReverseIntEndian(int pSourse)
{
    SWAP_32(pSourse);
    return pSourse;
}

float ReverseFloatEndian(float pSourse)
{
    SWAP_32(pSourse);
    return pSourse;
}

string FormatGatewayList(string pGatewayA, string pGatewayB)
{
    if(pGatewayA.size()>15) 
        pGatewayA.resize(15);
    if(pGatewayB.size()>15) 
        pGatewayB.resize(15);

    string strGatewayList(pGatewayA);
    strGatewayList += ",";
    strGatewayList += pGatewayB;

    return strGatewayList;
}

void GetGatewayFromList(string pGatewayList, string& pGatewayA, string& pGatewayB)
{
    pGatewayA.resize(0);
    pGatewayB.resize(0);
    string::size_type st_pos = pGatewayList.find_first_of(',');
    if (st_pos != string::npos)
    {
        pGatewayA.assign(pGatewayList,0,st_pos);
        pGatewayB.assign(pGatewayList,st_pos+1,pGatewayList.size()-st_pos-1);
        if(pGatewayA.size()>15) 
            pGatewayA.resize(15);
        if(pGatewayB.size()>15) 
            pGatewayB.resize(15);
    }
}

bool GetFileExFileName(const string& pSrcFileName,string& strExtName)
{
	bool bReturn(false);
	string::size_type n = pSrcFileName.find_last_of('.');
	if (string::npos != n)	{
		strExtName = pSrcFileName.substr(n,pSrcFileName.size());
		bReturn = true;
	}
	return bReturn;
}

#endif
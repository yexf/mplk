#pragma once
#include "wxf_base.h"
/*************************************************************
 �� �� ����socketsInit()
 ���ܸ�Ҫ����ʼ��socket
           windows����Ҫ�ڳ�����ʹ��Ws2_32.dll
 �� �� ֵ���ɹ�true,ʧ��false 
 ��    ������
***************************************************************/
inline bool socket_init()
{
#ifdef _WIN32

	WSADATA wsadata;

	//��ʼ����SOCKET2.0�ĵ���
	if (WSAStartup(MAKEWORD(2,1), &wsadata) == 0)
		return true;
	else
		return false;
#endif
	// Linux/Unix do not require any initialization
	return true;
}

/*************************************************************
 �� �� ����socketsEnd()
 ���ܸ�Ҫ��������Ws2_32.dll�ĵ���
 �� �� ֵ���� 
 ��    ������
 �޸���ʷ��
***************************************************************/
inline void socket_deinit()
{
#ifdef _WIN32

	//�����׽��ֵĵ���
	WSACleanup();
#endif
}


//ͨѶ�� (������)
class wxf_common
{
private:

	//ͨѶ���� 0-���� 1-���� 2-���� 3-����
	int       m_nType;                           

protected:

	//���ͻ�����
    wxf_buffer    m_SendBuff;                     

	//���ջ�����
	wxf_buffer    m_RecvBuff;                           
	
	//������
	wxf_lock     m_Lock;

	//��־��¼
	wxf_log  m_LogFile;

public:

	//���캯��
	wxf_common(int nType=ID_NET/*Ĭ��Ϊ����*/);

    //��������
	~wxf_common();

	//��ý��շ��ͻ�����
	void *      GetSendBuffer();

	void *      GetRecvBuffer();

	//������־�������
	void        SetLogLevel(int nLevel);  

	//�����־�������
	int         GetLogLevel();

	//������־·��
	void        SetLogPath(char *pPath);
	
	//��ʼ����־�ļ�
	bool        InitLogFile(char * pFileName);

//����Ϊ���麯��,����������

    //��ͨѶ��
    virtual bool open()=0;                                        

	//���ͱ��ط��ͻ�����ָ����Ŀ������(���������ݽṹ)
    virtual int  write(int nLen = MAX_MSGLEN)=0;           
	
	//����ָ����������ָ�����ȵ�����
	virtual int  write(void * pBuf,int nLen) =0;
    
	//����ָ����Ŀ�����ݵ����ؽ��ջ�����(���������ݽṹ)
	virtual int  read(int nLen = MAX_MSGLEN)=0;   
	
	//����ָ����Ŀ�����ݵ�ָ���Ļ�����
	virtual int  read(void * pBuf,int nLen) =0;
	
	//�ر�ͨѶ��
    virtual bool close()=0;                                       
    
	//����ͨѶ��ѡ��
    virtual bool set_options(int nFlag,int nTime,int Reserve=0)=0;               

	//��������������
    //virtual int  CopyTo(wxf_common *target);                       
};

enum sockstate_type {INITIALIZED, BINDED, LISTENING, CONNECTED, CLOSED};

class wxf_net: public wxf_common
{
private:
	//socket handler
	//##ModelId=45190CD801B7
	SOCKET			m_hSocket; 

	//�� AF_INET/PF_INET
	//##ModelId=45190CD801B8
	int				m_nDomain;  

	//socket������ TCP-SOCK_STREAM FTP-SOCK_DGRAM
	int				m_Type;      

	//Э������
	//##ModelId=45190CD801C1
	int				m_nProtocol;

	//���ؽ�ɫ��0--������, 1--�ͻ���, -1--��δ��ʼ��
	//##ModelId=45190CD801C2
	int				m_nLocalRole;	            

	/* ------ Added by qingch   3/12/2009 ----- */	
	//���ܳ�ʱ ��λ��MS
	//##ModelId=45190CD801C1
	int				m_nRecvTimes;


	//���ͳ�ʱ ��λ��MS
	//##ModelId=45190CD801C1
	int				m_nSendTimes;
	/* --------------------------------------- */

	//socket״̬
	//##ModelId=45190CD801C4
	sockstate_type	m_eState;                   

	//�Զ˵�ַ
	//##ModelId=45190CD801CB
	netaddr         m_OpponentAddr;       

	//���ص�ַ
	netaddr         m_LocalAddr;   

	//�鲥��ַ
	netaddr         m_MultiCastAddr;  

public:
	//���캯��
	//##ModelId=45190CD801CC
	wxf_net(int type = SOCK_STREAM, int domain = AF_INET, int protocol = IPPROTO_IP);

	//##ModelId=45190CD801D6
	wxf_net(const netaddr* addr,int domain = AF_INET,int type = SOCK_STREAM, int protocol = IPPROTO_IP);

	//��������
	//##ModelId=45190CD801DB
	virtual ~wxf_net();

	//��ý�ɫ
	int GetLocalRole() const {return m_nLocalRole;}
	//���ý�ɫ
	void SetLocalRole(int nRole);
	//���socket���
	SOCKET GetHandler() const {return m_hSocket;}
	//����socket���
	void SetHandler(SOCKET handler) {close();m_hSocket=handler;}
	//���õ�ǰsocket״̬
	void SetState(sockstate_type state);
	//ȡ�õ�ǰsocket״̬
	sockstate_type GetState() const { return m_eState; }
	//���öԶ˵�ַ
	void SetOpponentAddr(const netaddr * addr);    
	void SetLocalAddr(const netaddr *addr);
	void SetMultiCastAddr(const netaddr *addr);
	//��öԶ˵�ַ
	void GetOpponentAddr(netaddr * addr);    
	void GetLocalAddr(netaddr * addr); 
	void GetMultiCastAddr(netaddr * addr); 
	int GetSocketType()const { return m_Type;}

public:
	//����wxf_common�麯��

	//����Socket
	bool open();    
	bool open(int flag); 

	//�ر�socket
	bool close();                                         

	//��������
	int write(int nLen = MAX_MSGLEN);     
	int write(void * pBuf,int nLen);

	//��������
	int read(int nLen = MAX_MSGLEN);    
	int read(void * pBuf,int nLen);

	//����ѡ��
	bool set_options(int nFlag,int nTime,int Reserve = 0);  

//�����Ա����
protected:
	int	write_udp(int nLen = MAX_MSGLEN);
	int write_udp(void *pBuf, int nLen);
	int	read_udp(int nLen = MAX_MSGLEN);
	int	read_udp(void *pBuf, int nLen);

public:	
	//�������˹���(�󶨲���)
	bool Bind (u_short port); 
	bool Bind (const char* host,u_short port);
	//��������,SERVER
	bool Listen (int queue = SOMAXCONN);                   
	//��������,SERVER
	bool Accept(wxf_net &sock);  	
	
	//�������󶨡���������,SERVER
	bool ListenServer(u_short port,const char * host=NULL,int nqueue=SOMAXCONN);
	//�ͻ��˹���(���ӷ�����)

	//�ͻ��˹���(���ӷ�����)
	bool ConnectServerB(const char * ip=NULL,int port=-1,int nLocalPort=-1,int nTimeout=-1);


	bool ConnectServer(const char * ip=NULL,int port=-1,int nLocalPort=-1,int nTimeout=-1);

	//�����鲥
	bool AddToMultiCast();
	//��������������
	int CopyTo(const wxf_net & net,int nFlag=2); 
	//����socketѡ��
	bool SetSocketOption(int nOptionName,const char* lpOptionValue,int nOptionLen,int nLevel);
	//ȡ��socket������Ϣ
	void GetSocketErrorMsg(char * cError);
	//ȡ�ñ���IP��ַ
	bool GetLocalComputerIp(char *ipAddr);
};


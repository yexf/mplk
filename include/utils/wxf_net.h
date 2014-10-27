#pragma once
#include "wxf_base.h"
/*************************************************************
 函 数 名：socketsInit()
 功能概要：初始化socket
           windows下需要在程序中使用Ws2_32.dll
 返 回 值：成功true,失败false 
 参    数：无
***************************************************************/
inline bool socket_init()
{
#ifdef _WIN32

	WSADATA wsadata;

	//初始化对SOCKET2.0的调用
	if (WSAStartup(MAKEWORD(2,1), &wsadata) == 0)
		return true;
	else
		return false;
#endif
	// Linux/Unix do not require any initialization
	return true;
}

/*************************************************************
 函 数 名：socketsEnd()
 功能概要：结束对Ws2_32.dll的调用
 返 回 值：无 
 参    数：无
 修改历史：
***************************************************************/
inline void socket_deinit()
{
#ifdef _WIN32

	//结束套接字的调用
	WSACleanup();
#endif
}


//通讯类 (纯虚类)
class wxf_common
{
private:

	//通讯类型 0-网络 1-串口 2-拨号 3-其它
	int       m_nType;                           

protected:

	//发送缓冲区
    wxf_buffer    m_SendBuff;                     

	//接收缓冲区
	wxf_buffer    m_RecvBuff;                           
	
	//互斥体
	wxf_lock     m_Lock;

	//日志记录
	wxf_log  m_LogFile;

public:

	//构造函数
	wxf_common(int nType=ID_NET/*默认为网络*/);

    //析构函数
	~wxf_common();

	//获得接收发送缓冲区
	void *      GetSendBuffer();

	void *      GetRecvBuffer();

	//设置日志输出级别
	void        SetLogLevel(int nLevel);  

	//获得日志输出级别
	int         GetLogLevel();

	//设置日志路径
	void        SetLogPath(char *pPath);
	
	//初始化日志文件
	bool        InitLogFile(char * pFileName);

//以下为纯虚函数,供子类重载

    //打开通讯口
    virtual bool open()=0;                                        

	//发送本地发送缓冲区指定数目的数据(不关心内容结构)
    virtual int  write(int nLen = MAX_MSGLEN)=0;           
	
	//发送指定缓冲区中指定长度的数据
	virtual int  write(void * pBuf,int nLen) =0;
    
	//接收指定数目的数据到本地接收缓冲区(不关心内容结构)
	virtual int  read(int nLen = MAX_MSGLEN)=0;   
	
	//接收指定数目的数据到指定的缓冲区
	virtual int  read(void * pBuf,int nLen) =0;
	
	//关闭通讯口
    virtual bool close()=0;                                       
    
	//设置通讯口选项
    virtual bool set_options(int nFlag,int nTime,int Reserve=0)=0;               

	//拷贝缓冲区数据
    //virtual int  CopyTo(wxf_common *target);                       
};

enum sockstate_type {INITIALIZED, BINDED, LISTENING, CONNECTED, CLOSED};

class wxf_net: public wxf_common
{
private:
	//socket handler
	//##ModelId=45190CD801B7
	SOCKET			m_hSocket; 

	//域 AF_INET/PF_INET
	//##ModelId=45190CD801B8
	int				m_nDomain;  

	//socket流类型 TCP-SOCK_STREAM FTP-SOCK_DGRAM
	int				m_Type;      

	//协议类型
	//##ModelId=45190CD801C1
	int				m_nProtocol;

	//本地角色：0--服务器, 1--客户端, -1--尚未初始化
	//##ModelId=45190CD801C2
	int				m_nLocalRole;	            

	/* ------ Added by qingch   3/12/2009 ----- */	
	//接受超时 单位：MS
	//##ModelId=45190CD801C1
	int				m_nRecvTimes;


	//发送超时 单位：MS
	//##ModelId=45190CD801C1
	int				m_nSendTimes;
	/* --------------------------------------- */

	//socket状态
	//##ModelId=45190CD801C4
	sockstate_type	m_eState;                   

	//对端地址
	//##ModelId=45190CD801CB
	netaddr         m_OpponentAddr;       

	//本地地址
	netaddr         m_LocalAddr;   

	//组播地址
	netaddr         m_MultiCastAddr;  

public:
	//构造函数
	//##ModelId=45190CD801CC
	wxf_net(int type = SOCK_STREAM, int domain = AF_INET, int protocol = IPPROTO_IP);

	//##ModelId=45190CD801D6
	wxf_net(const netaddr* addr,int domain = AF_INET,int type = SOCK_STREAM, int protocol = IPPROTO_IP);

	//析构函数
	//##ModelId=45190CD801DB
	virtual ~wxf_net();

	//获得角色
	int GetLocalRole() const {return m_nLocalRole;}
	//设置角色
	void SetLocalRole(int nRole);
	//获得socket句柄
	SOCKET GetHandler() const {return m_hSocket;}
	//设置socket句柄
	void SetHandler(SOCKET handler) {close();m_hSocket=handler;}
	//设置当前socket状态
	void SetState(sockstate_type state);
	//取得当前socket状态
	sockstate_type GetState() const { return m_eState; }
	//设置对端地址
	void SetOpponentAddr(const netaddr * addr);    
	void SetLocalAddr(const netaddr *addr);
	void SetMultiCastAddr(const netaddr *addr);
	//获得对端地址
	void GetOpponentAddr(netaddr * addr);    
	void GetLocalAddr(netaddr * addr); 
	void GetMultiCastAddr(netaddr * addr); 
	int GetSocketType()const { return m_Type;}

public:
	//重载wxf_common虚函数

	//创建Socket
	bool open();    
	bool open(int flag); 

	//关闭socket
	bool close();                                         

	//发送数据
	int write(int nLen = MAX_MSGLEN);     
	int write(void * pBuf,int nLen);

	//接收数据
	int read(int nLen = MAX_MSGLEN);    
	int read(void * pBuf,int nLen);

	//设置选项
	bool set_options(int nFlag,int nTime,int Reserve = 0);  

//自身成员函数
protected:
	int	write_udp(int nLen = MAX_MSGLEN);
	int write_udp(void *pBuf, int nLen);
	int	read_udp(int nLen = MAX_MSGLEN);
	int	read_udp(void *pBuf, int nLen);

public:	
	//服务器端工具(绑定操作)
	bool Bind (u_short port); 
	bool Bind (const char* host,u_short port);
	//监听操作,SERVER
	bool Listen (int queue = SOMAXCONN);                   
	//接受连接,SERVER
	bool Accept(wxf_net &sock);  	
	
	//创建、绑定、监听操作,SERVER
	bool ListenServer(u_short port,const char * host=NULL,int nqueue=SOMAXCONN);
	//客户端工具(连接服务器)

	//客户端工具(连接服务器)
	bool ConnectServerB(const char * ip=NULL,int port=-1,int nLocalPort=-1,int nTimeout=-1);


	bool ConnectServer(const char * ip=NULL,int port=-1,int nLocalPort=-1,int nTimeout=-1);

	//设置组播
	bool AddToMultiCast();
	//拷贝缓冲区数据
	int CopyTo(const wxf_net & net,int nFlag=2); 
	//设置socket选项
	bool SetSocketOption(int nOptionName,const char* lpOptionValue,int nOptionLen,int nLevel);
	//取得socket错误消息
	void GetSocketErrorMsg(char * cError);
	//取得本地IP地址
	bool GetLocalComputerIp(char *ipAddr);
};


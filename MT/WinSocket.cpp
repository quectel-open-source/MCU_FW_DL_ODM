#include "StdAfx.h"
#include "WinSocket.h"

CWinSocket::CWinSocket( void )
	: m_csError_Info( _T( "" ) )
	, m_bDebug_Log_File( false )
	, m_bConn_Running_Enable( false )
{
	m_uiSocket_Main = INVALID_SOCKET;
	m_bConn_Running_Enable = false;
}

CWinSocket::~CWinSocket(void)
{
	UNINIT_WIN_SOCK_API();//释放为该程序分配的资源，终止对winsock动态库的使用
}

void CWinSocket::Save_Trace_Log(const wchar_t * wcp_Trace_Log)
{
//	::SendMessage(this->m_hWnd_View, WM_TRACE_LOG_MSG, (WPARAM)wcp_Trace_Log, (LPARAM)1);
}

bool CWinSocket::INIT_WIN_SOCK_API(void)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int iRet;

	//
	try
	{
		//第一步，加载套接字。WinSock初始化
		wVersionRequested = MAKEWORD(2, 2);//希望使用的WinSock DLL的版本

		//加载套接字库
		iRet = WSAStartup(wVersionRequested, &wsaData);

		//判定返回值
		if(iRet != 0)
		{
			this->m_csError_Info.Format(_T("FAIL, INIT_WIN_SOCK_API, WSAStartup(), iRet = %d;"), iRet);
			throw(0);
		}


		//确认WinSock DLL支持版本2.2
		if((LOBYTE(wsaData.wVersion) != 2) || (HIBYTE(wsaData.wVersion) != 2))
		{
			this->m_csError_Info.Format(_T("FAIL, INIT_WIN_SOCK_API, Invalid_WinSock_Version(%d.%d)!"),
				LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));
			throw(0);
		}
	}
	catch(...)
	{
		this->UNINIT_WIN_SOCK_API();//释放为该程序分配的资源，终止对winsock动态库的使用

		//fail
		return false;
	}

	//pass
	return true;
}

void CWinSocket::UNINIT_WIN_SOCK_API(void)
{
	WSACleanup();//释放为该程序分配的资源，终止对winsock动态库的使用
}

bool CWinSocket::Create_Socket(int i_Mode_TCP_UDP)
{
	int iType, iProtocol;

	try
	{
		//确定参数
		switch(i_Mode_TCP_UDP)
		{
		case MODE_TCP:
			iType = SOCK_STREAM;
			iProtocol = IPPROTO_TCP;
			break;
		default:
			this->m_csError_Info.Format(_T("ERROR, Create_Socket, Mode(%d) is wrong!"), i_Mode_TCP_UDP);
			throw(0);
		}


		//第二步，创建套接字。创建Socket,使用TCP协议
		this->m_uiSocket_Main = socket(AF_INET, iType, iProtocol);

		//判定返回值
		if(this->m_uiSocket_Main == INVALID_SOCKET)
		{
			this->m_csError_Info.Format(_T("FAIL, Create_Socket, uiSocket_HD = %d;"), this->m_uiSocket_Main);
			throw(0);
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool CWinSocket::Create_Client(const wchar_t * wcp_IP_Addr, int i_Port)
{
	struct sockaddr_in strSock_Addr_In;//服务器地址信息
	int iRet;
	char * cpIP_Addr = NULL;


	try
	{
		//分配char内存
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpIP_Addr, (wcslen(wcp_IP_Addr) * 2 + 8));

		//IP地址转换
		this->m_cla_GFC.WideChar_To_MultiByte(wcp_IP_Addr, cpIP_Addr, &iRet);


		//
		memset(&strSock_Addr_In, 0, sizeof(strSock_Addr_In));

		//构建服务器地址信息
		strSock_Addr_In.sin_family = AF_INET; //地址家族
		strSock_Addr_In.sin_port = htons((unsigned short)i_Port);//htons(SERVER_PORT);//注意转化为网络节序
		strSock_Addr_In.sin_addr.S_un.S_addr = htonl(ntohl(inet_addr(cpIP_Addr)));//inet_addr("192.168.1.10");

/*		//第二步、异步消息处理机制
		iRet = WSAAsyncSelect(this->m_uiSocket_Main, this->m_hWnd_View, WM_SOCKET, FD_READ | FD_CLOSE);
		if (iRet == SOCKET_ERROR)
		{
			this->m_csError_Info.Format(_T("FAIL, WSAAsyncSelect(), iRet = %d, Error_code = %d;"), iRet, WSAGetLastError());
			throw(0);
		}*/
		

		//第三步，连接服务器。
		iRet = connect(this->m_uiSocket_Main, (struct sockaddr *)&strSock_Addr_In, sizeof(strSock_Addr_In));
		//
		if(iRet == SOCKET_ERROR)
		{
			this->m_csError_Info.Format(_T("FAIL, Create_Client(IP_Addr = %s, Port = %d), iRet = %d;"), wcp_IP_Addr, i_Port, iRet);
			throw(0);
		}

		iRet = WSAAsyncSelect(this->m_uiSocket_Main, this->m_hWnd_View, WM_SOCKET, FD_READ | FD_CLOSE);
		//释放char内存
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpIP_Addr);
	}
	catch(...)
	{
		//关闭套接字
		this->CLOSE_SOCKET_MAIN();

		//释放char内存
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpIP_Addr);

		//
		return false;
	}

	return true;
}

void CWinSocket::CLOSE_SOCKET_MAIN(void)
{
	//关闭“主控”连接套接字
	this->CLOSE_SOCKET_SEL( &this->m_uiSocket_Main );
}

bool CWinSocket::SEND_CHAR_MAIN(const char * cp_Send_Byte, int i_Byte_Quan)
{
	return this->SEND_CHAR_SEL(this->m_uiSocket_Main, cp_Send_Byte, i_Byte_Quan);
}

bool CWinSocket::READ_CHAR_MAIN(char * cp_Read_Byte, int * ip_Recv_Byte_Size, int i_Byte_Max_Limit)
{
	return this->READ_CHAR_SEL(this->m_uiSocket_Main, cp_Read_Byte, ip_Recv_Byte_Size, i_Byte_Max_Limit);
}

bool CWinSocket::CONN_Client(int i_Mode_TCP_UDP, const wchar_t * wcp_IP_Addr, int i_Port)
{
	try
	{
		//检查Client是否已连接
		if( this->m_bConn_Running_Enable == true )
		{
			//Client已连接
			return true;
		}

		//第二步，创建套接字。创建Socket,使用TCP、UDP协议
		if(this->Create_Socket(i_Mode_TCP_UDP) == false)
		{
			throw(0);
		}

		//第三步，连接服务器。
		if(this->Create_Client(wcp_IP_Addr, i_Port) == false)
		{
			throw(0);
		}

		//设定Client连接已建立标识
		this->m_bConn_Running_Enable = true;
	}
	catch(...)
	{
		//fail
		return false;
	}

	//pass
	return true;
}

void CWinSocket::DISCONN_Client(void)
{
	if( this->m_bConn_Running_Enable == true )
	{
		this->CLOSE_SOCKET_MAIN();//关闭主套接字
	}
}

bool CWinSocket::START_Server(int i_Mode_TCP_UDP, const wchar_t * wcp_IP_Addr, int i_Port)
{
	try
	{
		//第二步，创建监听套接字，sListen=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);仍然使用的是socket函数。
		if( this->Create_Socket( i_Mode_TCP_UDP ) == false )
		{
			throw(0);
		}

		//第三步，绑定。使用bind函数，该函数的作用是将一个创建好的套接字绑定到本地的某个地址和端口上。
		//第四步，监听连接。使用listen函数，该函数是将指定的套接字设置为监听模式，如ret = listen(sListen, 5)。
		if( this->Create_Server( wcp_IP_Addr, i_Port ) == false )
		{
			throw(0);
		}

		//设定Server连接已建立标识
		this->m_bConn_Running_Enable = true;
	}
	catch( ... )
	{
		//fail
		return false;
	}

	//pass
	return true;
}

void CWinSocket::STOP_Server(void)
{
	//关闭主套接字
	this->CLOSE_SOCKET_MAIN();
	//
	WSACleanup();
}

bool CWinSocket::Create_Server(const wchar_t * wcp_IP_Addr, int i_Port)
{
	struct sockaddr_in strSock_Addr_In_Server;
	int iRet;
	char * cpIP_Addr = NULL;

	try
	{
		//分配char内存
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpIP_Addr, (wcslen(wcp_IP_Addr) * 2 + 8));

		//IP地址转换
		this->m_cla_GFC.WideChar_To_MultiByte(wcp_IP_Addr, cpIP_Addr, &iRet);

		//
		memset(&strSock_Addr_In_Server, 0, sizeof(strSock_Addr_In_Server));

		//构建本地地址信息
		strSock_Addr_In_Server.sin_family = AF_INET; //地址家族
		strSock_Addr_In_Server.sin_port = htons((unsigned short)i_Port);//htons(SERVER_PORT); //注意转化为网络字节序
		strSock_Addr_In_Server.sin_addr.S_un.S_addr = htonl(ADDR_ANY);//htonl(ntohl(inet_addr(cpIP_Addr)));//htonl(INADDR_ANY);使用INADDR_ANY 指示任意地址


		//第三步，绑定。使用bind函数，该函数的作用是将一个创建好的套接字绑定到本地的某个地址和端口上。
		iRet = bind(this->m_uiSocket_Main, (struct sockaddr *)&strSock_Addr_In_Server, sizeof(strSock_Addr_In_Server));
		if(iRet == SOCKET_ERROR)
		{
			this->m_csError_Info.Format(_T("FAIL, bind(IP_Addr = %s, Port = %d), iRet = %d, Error_code = %d;"), wcp_IP_Addr, i_Port, iRet, WSAGetLastError());
			throw(0);
		}


		//第四步，监听连接。使用listen函数，该函数是将指定的套接字设置为监听模式，如ret = listen(sListen, 5)。
		iRet = listen(this->m_uiSocket_Main, 12);//第二个参数，是等待连接队列的最大的长度。
		if(iRet == SOCKET_ERROR)
		{
			this->m_csError_Info.Format(_T("FAIL, listen(), iRet = %d, Error_code = %d;"), iRet, WSAGetLastError());
			throw(0);
		}


		//TCP连接接入, 消息处理机制
		iRet = WSAAsyncSelect(this->m_uiSocket_Main, this->m_hWnd_View, WM_SOCKET_SERVER, FD_ACCEPT | FD_READ| FD_WRITE | FD_CLOSE );
		if(iRet == SOCKET_ERROR) 
		{
			this->m_csError_Info.Format(_T("FAIL, WSAAsyncSelect(), iRet = %d, Error_code = %d;"), iRet, WSAGetLastError());
			throw(0);
		}


		//释放char内存
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpIP_Addr);
	}
	catch(...)
	{
		//关闭套接字
		this->CLOSE_SOCKET_MAIN();
		//
		WSACleanup();

		//释放char内存
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpIP_Addr);

		//
		return false;
	}

	return true;
}

SOCKET CWinSocket::ACCEPT(sockaddr_in * strp_Addr_In)
{
	int iLength, iRet;
	SOCKET uiRec_Socket = INVALID_SOCKET;//连接套接字

	try
	{
		iLength = sizeof(*strp_Addr_In);//第三个参数，也是一个返回值，指向整型的变量，保存了返回的地址信息的长度。

		//第五步，接受客户端的连接请求。使用accept函数接受客户端发送的连接请求。
		//accept函数返回值是一个客户端和服务器连接的SOCKET类型的描述符，在服务器端标识着这个客户端。
		uiRec_Socket = accept(this->m_uiSocket_Main, (struct sockaddr *)strp_Addr_In, &iLength);
		//
		if(uiRec_Socket == INVALID_SOCKET)
		{
			this->m_csError_Info.Format(_T("FAIL, accept(), strRec_Socket = %d, Error_code = %d;"), uiRec_Socket, WSAGetLastError());
			throw(0);
		}



		//建立“读、写、关闭”消息处理
		iRet = WSAAsyncSelect(uiRec_Socket, this->m_hWnd_View, WM_SOCKET_SERVER, (FD_READ | FD_WRITE | FD_CLOSE));
		//
		if(iRet == SOCKET_ERROR)
		{
			this->m_csError_Info.Format(_T("FAIL, WSAAsyncSelect( FD_READ FD_WRITE FD_CLOSE ), iRet = %d, Error_code = %d;"), iRet, WSAGetLastError());
			closesocket(uiRec_Socket);
			throw(0);
		}
	}
	catch(...)
	{
		return INVALID_SOCKET;
	}

	return uiRec_Socket;
}

CString CWinSocket::IP_NetworkByteOrder_To_CString(in_addr str_sin_addr)
{
	CString csIP;
	int iTemp;
	char crIP[128];
	wchar_t wcrIP[128];

	sprintf_s(crIP, "%s", inet_ntoa(str_sin_addr));

	this->m_cla_GFC.MultiByte_To_WideChar(crIP, wcrIP, &iTemp);

	csIP.Format(_T("%s"), wcrIP);

	return csIP;
}

CString CWinSocket::Port_NetworkByteOrder_To_CString(unsigned short us_sin_port)
{
	CString csPort;

	csPort.Format(_T("%d"), ntohs(us_sin_port));

	return csPort;
}

void CWinSocket::CLOSE_SOCKET_SEL(SOCKET * strp_Sel_Socket)
{
	//关闭“选中的”连接套接字

	if( *strp_Sel_Socket != INVALID_SOCKET )
	{
		int iRet;

		iRet = closesocket( *strp_Sel_Socket );
		//
		if( iRet == SOCKET_ERROR )
		{
			this->m_csError_Info.Format( _T( "FAIL, closesocket( %d ), iRet = %d, Error_code = %d;" ), *strp_Sel_Socket, iRet, WSAGetLastError() );
			::AfxMessageBox( this->m_csError_Info );
		}

		//句柄恢复“无效”
		*strp_Sel_Socket = INVALID_SOCKET;

		//连接已断开标识
		this->m_bConn_Running_Enable = false;
	}
}

bool CWinSocket::SEND_CHAR_SEL(SOCKET ui_Sel_Socket, const char * cp_Send_Byte, int i_Byte_Quan)
{
	int iRet;

	try
	{
		//第四步，发送。
		iRet = send(ui_Sel_Socket, cp_Send_Byte, i_Byte_Quan, 0);//send(sClient, (char *)&sendMessage, sizeof(sendMessage), 0);

		//
		if(iRet == SOCKET_ERROR)
		{
			this->m_csError_Info.Format(_T("FAIL, SEND_CHAR, iRet = %d;"), iRet);
			throw(0);
		}
	}
	catch(...)
	{
		//
		return false;
	}

	return true;
}

bool CWinSocket::READ_CHAR_SEL(SOCKET ui_Sel_Socket, char * cp_Read_Byte, int * ip_Recv_Byte_Size, int i_Byte_Max_Limit)
{
	int iRet;

	try
	{
		//第五步，接收。
		iRet = recv(ui_Sel_Socket, cp_Read_Byte, i_Byte_Max_Limit, 0);

		//判定+处理读取数量和结果
		switch(iRet)
		{
		case SOCKET_ERROR://TCP连接的接收BUFF中无数据了，可以不再读了
			*ip_Recv_Byte_Size = 0;
			break;
		case 0://连接若断开，则返回的是“0”，If the connection has been gracefully closed, the return value is zero
			*ip_Recv_Byte_Size = 0;//客户端已经关闭连接
			throw(0);
		default:
			{
				if(iRet < i_Byte_Max_Limit)
				{
					//“实际读到数据量”小于“期望读取量”，全读完了，后面无数据了
				}
				else
				{
					//“实际读到数据量”大于等于“期望读取量”，没有读完了，后面可能还有数据
					//需要继续读取+处理
				}

				//
				*ip_Recv_Byte_Size = iRet;
			}
			break;
		}
	}
	catch(...)
	{
		//
		return false;
	}

	return true;
}

bool CWinSocket::Set_Send_And_Recv_Timeout(int i_Send_Timeout_mS, int i_Recv_Timeout_mS)
{
	int iRet;


	try
	{
		//发送时限
		iRet = setsockopt(this->m_uiSocket_Main, SOL_SOCKET, SO_SNDTIMEO, (char *)&i_Send_Timeout_mS, sizeof(i_Send_Timeout_mS));
		//
		if(iRet == SOCKET_ERROR)
		{
			this->m_csError_Info.Format(_T("FAIL, setsockopt, SO_SNDTIMEO, iRet = %d;"), iRet);
			throw(0);
		}


		//接收时限
		iRet = setsockopt(this->m_uiSocket_Main, SOL_SOCKET, SO_RCVTIMEO, (char *)&i_Recv_Timeout_mS, sizeof(i_Recv_Timeout_mS));
		//
		if(iRet == SOCKET_ERROR)
		{
			this->m_csError_Info.Format(_T("FAIL, setsockopt, SO_RCVTIMEO, iRet = %d;"), iRet);
			throw(0);
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}

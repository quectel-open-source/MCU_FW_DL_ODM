#pragma once
#include "gfc.h"
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")

#define MODE_TCP					0
#define MODE_UDP					1

#define WM_WS_TCP_ACCEPT_MSG_EXT		WM_USER + 8000//客户端TCP接入消息
#define WM_WS_TCP_OP_MSG_EXT			WM_USER + 8001//客户端TCP读、写、关操作消息
#define WM_SOCKET       			WM_USER + 8002//socket消息
#define WM_SOCKET_SERVER       			WM_USER + 8003//socket消息

class CWinSocket
{
public:
	CWinSocket(void);
	~CWinSocket(void);
	HWND m_hWnd_View;
	CGFC m_cla_GFC;
	CString m_csError_Info;
	bool m_bDebug_Log_File;
	void Save_Trace_Log(const wchar_t * wcp_Trace_Log);
	bool INIT_WIN_SOCK_API(void);
	void UNINIT_WIN_SOCK_API(void);
	SOCKET m_uiSocket_Main;//“主控”连接套接字
	bool Create_Socket(int i_Mode_TCP_UDP);
	bool Create_Client(const wchar_t * wcp_IP_Addr, int i_Port);
	void CLOSE_SOCKET_MAIN(void);
	bool SEND_CHAR_MAIN(const char * cp_Send_Byte, int i_Byte_Quan);
	bool READ_CHAR_MAIN(char * cp_Read_Byte, int * ip_Recv_Byte_Size, int i_Byte_Max_Limit);
	bool CONN_Client(int i_Mode_TCP_UDP, const wchar_t * wcp_IP_Addr, int i_Port);
	void DISCONN_Client(void);
	bool START_Server(int i_Mode_TCP_UDP, const wchar_t * wcp_IP_Addr, int i_Port);
	void STOP_Server(void);
	bool Create_Server(const wchar_t * wcp_IP_Addr, int i_Port);
	SOCKET ACCEPT(sockaddr_in * strp_Addr_In);
	CString IP_NetworkByteOrder_To_CString(in_addr str_sin_addr);
	CString Port_NetworkByteOrder_To_CString(unsigned short us_sin_port);
	void CLOSE_SOCKET_SEL(SOCKET * strp_Sel_Socket);
	bool SEND_CHAR_SEL(SOCKET ui_Sel_Socket, const char * cp_Send_Byte, int i_Byte_Quan);
	bool READ_CHAR_SEL(SOCKET ui_Sel_Socket, char * cp_Read_Byte, int * ip_Recv_Byte_Size, int i_Byte_Max_Limit);
	bool Set_Send_And_Recv_Timeout(int i_Send_Timeout_mS, int i_Recv_Timeout_mS);
	bool m_bConn_Running_Enable;//连接已建立、运行中的标识
};

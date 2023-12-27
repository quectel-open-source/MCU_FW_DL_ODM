#pragma once
#include "gfc.h"

class CCom_Port
{
public:
	CCom_Port(void);
	~CCom_Port(void);
	bool m_bOpen;
	bool Port_Open(int i_Read_TimeOut);
	int m_iPort;
	int m_iBaudRate;
	CString m_csError_Info;
	void Port_Close(void);
	HANDLE m_hCommHandle;
	COMMPROP CommProps;
	DCB CommDcb;
	COMMTIMEOUTS commTimeOuts;
	bool Write(const wchar_t * wcpSend_Data);
	bool Read(wchar_t * wcpRead_Data, int * ipRec_Numb, const int iRec_TimeOut);
	bool Query(const wchar_t * wcpSend_Data, wchar_t * wcpRead_Data, int * ipRec_Bits, const int iRec_TimeOut);
	bool Write_Byte(char * cp_Send_Data, unsigned long ul_Data_Quan);
	bool Read_Byte(char * cp_Read_Buff, unsigned long ul_Expect_Read_Bytes, unsigned long * ulp_Real_Rec_Bytes);
	void Clear_Rx_Buffer(void);
	void Clear_Tx_Buffer(void);
	unsigned long Get_Com_Rx_Buff_Valid(void);
	bool Send_Byte_Data(const wchar_t * wcp_Send_Data, int i_Numb);
	bool Read_Byte_Data(wchar_t * wcp_Read_Data, int * ip_Rec_Bytes, const int i_Rec_TimeOut);
	CGFC m_cla_GFC;
	PKT_BUFF_S m_str_Com_Rx_Buff;
	bool Read_Byte_Sync_V1(void);
	bool Read_Byte_Sync_V2(unsigned long ul_Exp_Rec_Byte_Quan, int i_TimeOut);
	bool Query_Tgt_CHAR(const wchar_t * wcp_Send_Data, const wchar_t * wcp_Tgt_Data, int i_TimeOut);
	void GET_RX_PKT_BUFF_DATA(CString * csp_Rx_Data);
	bool GET_BUFF_BYTE(unsigned char ** ucp2_Buff_Pointer, unsigned long * ulp_Data_Length);
	void DEL_BUFF_BYTE(unsigned long ul_Del_Data_Len);
	bool Empty_Com_And_Rx_Buff(void);
	DWORD m_dwDTR_CTRL;
	DWORD m_dwRTS_CTRL;
};

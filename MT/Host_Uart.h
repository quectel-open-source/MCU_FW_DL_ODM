#pragma once

enum CHFlowMode
{
	CHFLOW_UNVALID  = -1,
    CHFLOW_NONE     = 0,
    CHFLOW_HARDWARE = 1,
    CHFLOW_XONXOFF  = 2
};

class CHost_Uart
{
public:
	CHost_Uart(void);
	~CHost_Uart(void);
	int m_iCom_Port_Numb;
	int m_iCom_Baud_Rate;
	int m_iCom_Flow_Mode;
	bool m_bOpened;
	HANDLE m_hComDev;
	bool Is_Port_Opened(void);
	void Port_Close(void);
	bool Port_Open(void);
	CString m_csError_Info;
	DCB m_dcb;
	char m_cXonChar;
	char m_cXoffChar;
	bool Write_Byte(unsigned char * ucp_Send_Data, unsigned long ul_Data_Quan);
	bool Read_Byte(unsigned char * ucp_Read_Buff, unsigned long ul_Expect_Read_Bytes, unsigned long * ulp_Real_Rec_Bytes);
	unsigned long Get_Rx_Buff_Valid(void);
	void Clear_Rx_Buffer(void);
};

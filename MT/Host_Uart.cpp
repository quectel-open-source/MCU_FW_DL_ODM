#include "StdAfx.h"
#include "Host_Uart.h"

CHost_Uart::CHost_Uart(void)
: m_iCom_Port_Numb(0)
, m_iCom_Baud_Rate(0)
, m_iCom_Flow_Mode(0)
, m_bOpened(false)
, m_csError_Info(_T(""))
, m_cXonChar(0)
, m_cXoffChar(0)
{
	m_hComDev = INVALID_HANDLE_VALUE;
}

CHost_Uart::~CHost_Uart(void)
{
}

bool CHost_Uart::Is_Port_Opened(void)
{
	return ((this->m_bOpened == true) || (this->m_hComDev != INVALID_HANDLE_VALUE));
}

void CHost_Uart::Port_Close(void)
{
	if(this->Is_Port_Opened() == true)
	{
		CloseHandle(this->m_hComDev);
		this->m_hComDev = INVALID_HANDLE_VALUE;
		this->m_bOpened = false;
	}
}

bool CHost_Uart::Port_Open(void)
{
	if(this->m_bOpened == true)
	{
		return true;
	}

	try
	{
		this->m_csError_Info.Format(_T(""));

		CString csPort;
		csPort.Format(_T("\\\\.\\COM%d"), this->m_iCom_Port_Numb);

		this->m_hComDev = CreateFile(csPort,
			GENERIC_READ | GENERIC_WRITE,	//允许读和写
			NULL,							//独占方式 don't share port
			NULL,							//SecurityAttributes - none in Win95
			OPEN_EXISTING,					//打开而不是创建 how to create
			0,								//同步方式 file attributes
			NULL							// handle of file with attributes to copy
			);

		/*
		char szPort[32];
		memset(szPort,0,sizeof(szPort));
		if(this->m_iCom_Port_Numb < 10)
		{
			sprintf(szPort, "COM%d", this->m_iCom_Port_Numb);
		}
		else
		{
			sprintf(szPort, "\\\\.\\COM%d", this->m_iCom_Port_Numb);
		}

		this->m_hComDev = CreateFileA(szPort, 
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL, 
			OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
			NULL);
		*/

		if(this->m_hComDev == INVALID_HANDLE_VALUE) 
		{	
			this->m_csError_Info.Format(_T("FAIL, Open com port %d failed!"), this->m_iCom_Port_Numb);
			throw(0);
		}

		
		//DCB
		GetCommState(this->m_hComDev, &this->m_dcb);

		this->m_dcb.DCBlength = sizeof(DCB);
		this->m_dcb.BaudRate        = this->m_iCom_Baud_Rate;
		this->m_dcb.fBinary         = TRUE;
		this->m_dcb.fParity         = FALSE;
		this->m_dcb.fOutxCtsFlow    = (this->m_iCom_Flow_Mode == CHFLOW_HARDWARE) ? TRUE:FALSE; 
		this->m_dcb.fOutxDsrFlow    = FALSE;
		this->m_dcb.fDtrControl     = DTR_CONTROL_DISABLE;
		this->m_dcb.fDsrSensitivity = FALSE;
		this->m_dcb.fOutX           = FALSE;
		this->m_dcb.fInX            = FALSE;
		this->m_dcb.fNull           = FALSE;
		this->m_dcb.fRtsControl     = (this->m_iCom_Flow_Mode == CHFLOW_HARDWARE) ? RTS_CONTROL_ENABLE:RTS_CONTROL_DISABLE;
		this->m_dcb.ByteSize        = 8;
		this->m_dcb.Parity          = NOPARITY;
		this->m_dcb.StopBits        = ONESTOPBIT;

		if(this->m_iCom_Flow_Mode == CHFLOW_XONXOFF)
		{
			this->m_dcb.fInX    = TRUE;
			this->m_dcb.fOutX   = TRUE;
			this->m_dcb.XonChar = this->m_cXonChar;//0x11;
			this->m_dcb.XoffChar = this->m_cXoffChar;//0x13;
		}
		else
		{
			this->m_dcb.fOutX = FALSE;
			this->m_dcb.fInX  = FALSE;
		}

		if(SetCommState(this->m_hComDev, &this->m_dcb) == FALSE)
		{
			this->m_csError_Info.Format(_T("FAIL, Set comm state fail!"));
			throw(0);
		}

		if(SetupComm(this->m_hComDev, 4096, 4096) == FALSE)
		{
			this->m_csError_Info.Format(_T("FAIL, Setup comm fail!"));
			throw(0);
		}

		//串口超时设置
		COMMTIMEOUTS CommTimeOuts;
		// read timeout is 1bit time + nb_bytes * bytetime
		CommTimeOuts.ReadIntervalTimeout = MAXDWORD;  
		CommTimeOuts.ReadTotalTimeoutMultiplier = 0;       //(11000/m_nBaud)+1 ; 
		CommTimeOuts.ReadTotalTimeoutConstant = 0;         //(READ_TIMEOUT/m_nBaud) +1;  
		// write timeout is 1bit time + nb bytes * byte time
		CommTimeOuts.WriteTotalTimeoutMultiplier = 1;      //(100000/m_nBaud)+1;
		CommTimeOuts.WriteTotalTimeoutConstant = 1000;     //(WRITE_TIMEOUT/m_nBaud) +1;

		if(SetCommTimeouts( this->m_hComDev, &CommTimeOuts ) == FALSE)
		{
			this->m_csError_Info.Format(_T("FAIL, Set comm timeouts fail!"));
			throw(0);
		}

		if(SetCommMask(this->m_hComDev, EV_RXCHAR|EV_CTS|EV_DSR) == FALSE)
		{
			this->m_csError_Info.Format(_T("FAIL, Set comm mask fail!"));
			throw(0);
		}	

		PurgeComm(this->m_hComDev, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

		//串口打开标识设定
		this->m_bOpened = true;
	}
	catch(...)
	{
		this->Port_Close();
		return false;
	}

	return true;
}

bool CHost_Uart::Write_Byte(unsigned char * ucp_Send_Data, unsigned long ul_Data_Quan)
{
	//发送单char（BYTE）数据，例如FW下载
	DWORD dwdTotalBytesSent = 0;
	int iRtn;

	//写
	iRtn = WriteFile(this->m_hComDev, ucp_Send_Data, ul_Data_Quan, &dwdTotalBytesSent, NULL);

	if(iRtn == 0)//If the function fails, the return value is zero
	{
		this->m_csError_Info.Format(_T("FATL, WriteFile Error = %d"), GetLastError());
		return false;
	}
	else
	{
		return true;
	}
}

bool CHost_Uart::Read_Byte(unsigned char * ucp_Read_Buff, unsigned long ul_Expect_Read_Bytes, unsigned long * ulp_Real_Rec_Bytes)
{
	//读取单char（BYTE）数据，例如FW下载
	DWORD dw_Bytes_Rec = 0;
	int iResult;

	//读数据前不再检测BUFF中数据量，而是依照函数参数中规定的数据量读取。
	//因为BUFF中数据量会不断增大，有可能将读取BUFF撑爆，造成内存溢出。
//	DWORD dw_Numb_Read = this->Get_Rx_Buff_Valid();

	//读
	iResult = ReadFile(this->m_hComDev, ucp_Read_Buff, ul_Expect_Read_Bytes, &dw_Bytes_Rec, NULL);

	if(iResult == 0)
	{
		*ulp_Real_Rec_Bytes = 0;
		this->m_csError_Info.Format(_T("FAIL, ReadFile Error = %d"), GetLastError());
		return false;
	}
	else
	{
		*ulp_Real_Rec_Bytes = dw_Bytes_Rec;//实际读出位数
		return true;
	}
}

unsigned long CHost_Uart::Get_Rx_Buff_Valid(void)
{
	COMSTAT ComStat;
	DWORD dwErrorFlags;

	ComStat.cbInQue = 0;

	if(!ClearCommError(this->m_hComDev, &dwErrorFlags, &ComStat))
	{
		return 0;
	}

	if(dwErrorFlags != 0)//有报错后，ComStat.cbInQue的值很荒唐
	{
		return 0;
	}

	return ComStat.cbInQue;
}

void CHost_Uart::Clear_Rx_Buffer(void)
{
	PurgeComm(this->m_hComDev, (PURGE_RXABORT | PURGE_RXCLEAR));
}

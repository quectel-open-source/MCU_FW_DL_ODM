#include "StdAfx.h"
#include "Com_Port.h"

/*
Ver1.04 (2014-5-12)

2014-3-12，xjli
1.添加了一些Read函数的“功能注释”，以辨别Read函数之间的功能区别。

2014-3-17，xjli
1.添加了“Query_Tgt_CHAR”和“GET_RX_PKT_BUFF_DATA”函数。

2014-3-29，xjli
1.添加了“Clear_Tx_Buffer”、“GET_BUFF_BYTE”、“DEL_BUFF_BYTE”和“Empty_Com_And_Rx_Buff”函数。

2014-5-12, xjli
1.发送AT的“Write”函数由“\r”结尾改为“\r\n”结尾。

2014-6-11, xjli
1.增加了DTR和RTS管脚的变量控制。
*/

CCom_Port::CCom_Port(void)
: m_bOpen(false)
, m_iPort(0)
, m_iBaudRate(0)
, m_csError_Info(_T(""))
, m_dwDTR_CTRL(DTR_CONTROL_ENABLE)
, m_dwRTS_CTRL(RTS_CONTROL_ENABLE)
{
	m_bOpen = false;
	m_hCommHandle = INVALID_HANDLE_VALUE;
	m_dwDTR_CTRL = DTR_CONTROL_ENABLE;
	m_dwRTS_CTRL = RTS_CONTROL_ENABLE;

	m_str_Com_Rx_Buff.ucp_Buff = NULL;
	m_str_Com_Rx_Buff.ul_Total_Size = 0;
	m_str_Com_Rx_Buff.ul_Data_Length = 0;
}

CCom_Port::~CCom_Port(void)
{
	if(m_str_Com_Rx_Buff.ucp_Buff != NULL)
	{
		free(m_str_Com_Rx_Buff.ucp_Buff);

		m_str_Com_Rx_Buff.ucp_Buff = NULL;
		m_str_Com_Rx_Buff.ul_Total_Size = 0;
		m_str_Com_Rx_Buff.ul_Data_Length = 0;
	}
}

bool CCom_Port::Port_Open(int i_Read_TimeOut)
{
	if(this->m_bOpen == true)
	{
		return true;
	}

	CString port;
	port.Format(_T("\\\\.\\COM%d"), this->m_iPort);

	this->m_hCommHandle = CreateFile(port,
		GENERIC_READ | GENERIC_WRITE,	//允许读和写
		NULL,							//独占方式 don't share port
		NULL,							//SecurityAttributes - none in Win95
		OPEN_EXISTING,					//打开而不是创建 how to create
		0,								//同步方式 file attributes
		NULL							// handle of file with attributes to copy
		);

	if(this->m_hCommHandle == INVALID_HANDLE_VALUE)//无效句柄
	{
		this->m_csError_Info.Format(_T("FAIL, Open com port %d failed!"), this->m_iPort);
		return false;
	}

	//有效句柄
	this->CommProps.wPacketLength = sizeof(COMMPROP);
	GetCommProperties(this->m_hCommHandle, &this->CommProps);

	//SERIAL_SET_QUEUE_SIZE
	SetupComm(this->m_hCommHandle, 4096, 4096);


	//设置Mask
	SetCommMask(this->m_hCommHandle, EV_RXCHAR|EV_ERR|EV_BREAK);


	//DCB
	GetCommState(this->m_hCommHandle, &this->CommDcb);

	/*
	this->CommDcb.DCBlength = sizeof(DCB);
	this->CommDcb.fBinary = TRUE;
	this->CommDcb.fParity = TRUE;
	this->CommDcb.fOutxCtsFlow = TRUE;
	this->CommDcb.fOutxDsrFlow = FALSE;
	this->CommDcb.fDtrControl = DTR_CONTROL_ENABLE;
	this->CommDcb.fDsrSensitivity = FALSE;
	this->CommDcb.fTXContinueOnXoff = TRUE;
	this->CommDcb.fOutX = FALSE;
	this->CommDcb.fInX = FALSE;
	this->CommDcb.fErrorChar = FALSE;
	this->CommDcb.fNull = FALSE;
	this->CommDcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
	this->CommDcb.fAbortOnError = FALSE;
	this->CommDcb.ByteSize	= 8;
	this->CommDcb.Parity	= NOPARITY;
	this->CommDcb.StopBits	= ONESTOPBIT;
	this->CommDcb.BaudRate = this->m_iBaudRate;
	this->CommDcb.EvtChar = 0x7E;
	*/

	this->CommDcb.DCBlength = sizeof(DCB);
	this->CommDcb.BaudRate = this->m_iBaudRate;// commDeviceInfo.clockRates[1].baudSetting[COMM_9600_BAUD_INDEX]; // 9600

	this->CommDcb.ByteSize = 8;
	this->CommDcb.fParity  = FALSE;
	this->CommDcb.StopBits = ONESTOPBIT;

	this->CommDcb.fBinary           = TRUE;
	this->CommDcb.fOutxCtsFlow      = FALSE;
	this->CommDcb.fOutxDsrFlow      = FALSE;

	this->CommDcb.fDtrControl = this->m_dwDTR_CTRL;

	EscapeCommFunction(this->m_hCommHandle, SETDTR);

	this->CommDcb.fRtsControl = this->m_dwRTS_CTRL;

	EscapeCommFunction(this->m_hCommHandle, SETRTS);

	this->CommDcb.fDsrSensitivity   = FALSE;
	this->CommDcb.fTXContinueOnXoff = FALSE;
	this->CommDcb.fOutX             = FALSE;
	this->CommDcb.fInX              = FALSE;
	this->CommDcb.fErrorChar        = FALSE;
	this->CommDcb.fNull             = FALSE;
	this->CommDcb.fAbortOnError     = FALSE;

	//为“高通平台”后加
//	this->CommDcb.EvtChar = 0x7e;//事件字符。当接收到此字符的时候，会产生一个事件。
	this->CommDcb.XonLim = 0;
	this->CommDcb.XoffLim = 0;


	//SERIAL_SET_RTS
	//SERIAL_SET_DTR
	//SERIAL_SET_LINE_CONTROL
	//SERIAL_SET_CHAR
	//SERIAL_SET_HANDFLOW
	if(SetCommState(this->m_hCommHandle, &this->CommDcb) == FALSE)
	{
		this->Port_Close();
		this->m_csError_Info.Format(_T("FAIL, Could not set comm state!"));
		return false;
	}



	//读间隔超时
	this->commTimeOuts.ReadIntervalTimeout         = -1;
	//读时间系数
	this->commTimeOuts.ReadTotalTimeoutMultiplier  = -1;
	//读时间常量
	this->commTimeOuts.ReadTotalTimeoutConstant    = i_Read_TimeOut;//500;
	//写时间系数
	this->commTimeOuts.WriteTotalTimeoutMultiplier = 1;
	//写时间常量
	this->commTimeOuts.WriteTotalTimeoutConstant   = 500;

	//SERIAL_SET_TIMEOUTS
	if(SetCommTimeouts(this->m_hCommHandle, &this->commTimeOuts) == FALSE)
	{
		this->Port_Close();
		this->m_csError_Info.Format(_T("FAIL, Could not set comm timeout!"));
		return false;
	}


	this->Clear_Tx_Buffer();
	this->Clear_Rx_Buffer();


	//分配存放串口接收到的数据的“串口BUFF”内存
	this->m_cla_GFC.PKT_BUFF_CALLOC(&this->m_str_Com_Rx_Buff, 6144);

	this->m_bOpen = true;
	return true;
}

void CCom_Port::Port_Close(void)
{
	if((this->m_hCommHandle != INVALID_HANDLE_VALUE) || (this->m_bOpen == true))
	{
		CloseHandle(this->m_hCommHandle);
		this->m_hCommHandle = INVALID_HANDLE_VALUE;

		this->m_cla_GFC.PKT_BUFF_FREE(&this->m_str_Com_Rx_Buff);
		this->m_bOpen = false;
	}
}

bool CCom_Port::Write(const wchar_t * wcpSend_Data)
{
	//发送wchar_t类型的“指令数据”，例如普通的AT指令

	char * cpWrite_Buff = NULL;
	DWORD dwdTotalBytesSent = 0;
	int iTemp;
	CString csWrite;

	//“字符类型指令”需要用“\r\n”结尾
	csWrite.Format(_T("%s\r\n"), wcpSend_Data);

	//分配内存
	iTemp = csWrite.GetLength() * 2 + 8;
	this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpWrite_Buff, iTemp);

	//加载数据
	this->m_cla_GFC.WideChar_To_MultiByte(csWrite, cpWrite_Buff, &iTemp);

	//写
	iTemp = WriteFile(this->m_hCommHandle, cpWrite_Buff, strlen(cpWrite_Buff), &dwdTotalBytesSent, NULL);

	//释放内存
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpWrite_Buff);

	if(iTemp == 0)
	{
		this->m_csError_Info.Format(_T("WriteFile Error = %d"), GetLastError());
		return false;
	}
	else
	{
		return true;
	}
}

bool CCom_Port::Read(wchar_t * wcpRead_Data, int * ipRec_Numb, const int iRec_TimeOut)
{
	//读取wchar_t类型的“指令数据”，例如普通的AT指令
	//检查在COM Rx Buff中的数据量不增加时，做“一次”读COM操作，放到wcpRead_Data中。

	char * cpRead_Buff = NULL;
	DWORD dw_Bytes_Rec = 0;
	DWORD dw_Numb_Read = 0;
	DWORD dw_Last_Read = 0;
	int iResult;
	time_t start, end;
	double dTimeOut = 2.0;

	try
	{
		//开始计时
		time(&start);

		do
		{
			time(&end);
			if(difftime(end, start) >= dTimeOut)
			{
				throw(0);//超时
			}

			//检查COM的BUFF里有多少数据
			dw_Numb_Read = this->Get_Com_Rx_Buff_Valid();

			if(dw_Numb_Read > 0)//有数据
			{
				if(dw_Numb_Read == dw_Last_Read)//新状态数据量没有增加，就可以读数据了
				{
					//方式可读数据量超过BUFF尺寸
					if(dw_Numb_Read > iRec_TimeOut)
					{
						dw_Numb_Read = iRec_TimeOut;
					}

					//分配内存
					this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, (dw_Numb_Read + 8));

					memset(cpRead_Buff, 0, (dw_Numb_Read + 8));

					//
					iResult = ReadFile(this->m_hCommHandle, cpRead_Buff, dw_Numb_Read, &dw_Bytes_Rec, NULL);

					if((dw_Bytes_Rec <= 0) || (iResult == FALSE))
					{
						throw(0);//读失败
					}
					else
					{
						*(cpRead_Buff + dw_Bytes_Rec) = '\0';

						//串口读出的数据需要转化为wchar_t的双字符才能用于界面显示
						this->m_cla_GFC.MultiByte_To_WideChar(cpRead_Buff, wcpRead_Data, ipRec_Numb);

						//释放内存
						this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

						*ipRec_Numb = *ipRec_Numb - 1;//去掉终止位
						return true;//成功
					}
				}//if(dw_Numb_Read == dw_Last_Read
				else//为防止后续还有数据，防止数据断裂，再搜索一次数据量后再确定是否读串口
				{
					dw_Last_Read = dw_Numb_Read;
					Sleep(50);
					continue;
				}
			}//if(dw_Numb_Read > 0
			else
			{
				Sleep(50);
				continue;
			}
		}
		while(true);
	}
	catch(...)
	{
		//释放内存
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

		return false;
	}

	return false;
}

bool CCom_Port::Query(const wchar_t * wcpSend_Data, wchar_t * wcpRead_Data, int * ipRec_Bits, const int iRec_TimeOut)
{
	if(this->Write(wcpSend_Data))
	{
		return this->Read(wcpRead_Data, ipRec_Bits, iRec_TimeOut);
	}
	else
	{
		return false;
	}
}

bool CCom_Port::Write_Byte(char * cp_Send_Data, unsigned long ul_Data_Quan)
{
	//发送单char数据，例如高通FW下载

	DWORD dwdTotalBytesSent = 0;
	int iRtn;

	//写
	iRtn = WriteFile(this->m_hCommHandle, cp_Send_Data, ul_Data_Quan, &dwdTotalBytesSent, NULL);

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

bool CCom_Port::Read_Byte(char * cp_Read_Buff, unsigned long ul_Expect_Read_Bytes, unsigned long * ulp_Real_Rec_Bytes)
{
	//读取单char数据，例如高通FW下载
	//按“读数据量参数 Expect_Read_Bytes”的要求做“一次”读COM操作。

	DWORD dw_Bytes_Rec = 0;
	int iResult;

	//读数据前不再检测BUFF中数据量，而是依照函数参数中规定的数据量读取。
	//因为BUFF中数据量会不断增大，有可能将读取BUFF撑爆，造成内存溢出。
//	DWORD dw_Numb_Read = this->Get_Rx_Buff_Valid();

	//读
	iResult = ReadFile(this->m_hCommHandle, cp_Read_Buff, ul_Expect_Read_Bytes, &dw_Bytes_Rec, NULL);

	if(iResult == 0)
	{
		*ulp_Real_Rec_Bytes = 0;
		this->m_csError_Info.Format(_T("FAIL, ReadFile Error = %d"), GetLastError());
		return false;
	}

	*ulp_Real_Rec_Bytes = dw_Bytes_Rec;//实际读出位数
	return true;
}

void CCom_Port::Clear_Rx_Buffer(void)
{
	//清空硬件COM口的RX_BUFF数据
	PurgeComm(this->m_hCommHandle, (PURGE_RXABORT | PURGE_RXCLEAR));
}

void CCom_Port::Clear_Tx_Buffer(void)
{
	//清空硬件COM口的TX_BUFF数据
	PurgeComm(this->m_hCommHandle, (PURGE_TXCLEAR | PURGE_TXABORT));
}

unsigned long CCom_Port::Get_Com_Rx_Buff_Valid(void)
{
	//检查COM口的RX缓存中有多少数据（可以读取）

	COMSTAT ComStat;
	DWORD dwErrorFlags;

	ComStat.cbInQue = 0;

	if(!ClearCommError(this->m_hCommHandle, &dwErrorFlags, &ComStat))
	{
		return 0;
	}

	if(dwErrorFlags != 0)//有报错后，ComStat.cbInQue的值很荒唐
	{
		return 0;
	}

	return ComStat.cbInQue;
}

bool CCom_Port::Send_Byte_Data(const wchar_t * wcp_Send_Data, int i_Numb)
{
	//用于发送wchar_t类型的单byte（0x00FF）数据，例如规避直播星开机指令
	char * cpWrite_Buff = NULL;
	DWORD dwdTotalBytesSent = 0;
	int i;

	PurgeComm(this->m_hCommHandle, (PURGE_TXCLEAR | PURGE_RXCLEAR));

	//分配内存
	this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpWrite_Buff, (i_Numb + 8));

	//加载数据，这类数据一般不用于显示，只是数值传递
	for(i=0; i<i_Numb; i++)
	{
		*(cpWrite_Buff + i) = *(wcp_Send_Data + i) & 0x00FF;
	}
	*(cpWrite_Buff + i) = '\0';//终止位

	//写
	i = WriteFile(this->m_hCommHandle, cpWrite_Buff, i_Numb, &dwdTotalBytesSent, NULL);

	//释放内存
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpWrite_Buff);

	if(i == 0)
	{
		this->m_csError_Info.Format(_T("WriteFile Error = %d"), GetLastError());
		return false;
	}
	else
	{
		return true;
	}
}

bool CCom_Port::Read_Byte_Data(wchar_t * wcp_Read_Data, int * ip_Rec_Bytes, const int i_Rec_TimeOut)
{
	//用于读取wchar_t类型的单byte（0x00FF）数据，例如规避直播星开机指令

	char * cpRead_Buff = NULL;
	DWORD dwdBytesReceived = 0;
	int iResult, i;

	//分配内存
	this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpRead_Buff, (i_Rec_TimeOut + 8));

	memset(cpRead_Buff, 0, (i_Rec_TimeOut + 8));

	//读
	iResult = ReadFile(this->m_hCommHandle, cpRead_Buff, (DWORD)i_Rec_TimeOut, &dwdBytesReceived, NULL);

	//加载数据，把读到的单字节数据加载双字节BUFF中。这类数据一般不用于显示，只是数值传递
	for(i=0; i<dwdBytesReceived; i++)
	{
		*(wcp_Read_Data + i) = *(cpRead_Buff + i) & 0x00FF;
	}
	*(wcp_Read_Data + i) = '\0';//终止位

	//释放内存
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpRead_Buff);

	if(iResult == 0)
	{
		*ip_Rec_Bytes = 0;
		this->m_csError_Info.Format(_T("FAIL, Read_Byte_Data, ReadFile Error = %d"), GetLastError());
		return false;
	}

	*ip_Rec_Bytes = dwdBytesReceived;//实际读出位数
	return true;
}

bool CCom_Port::Read_Byte_Sync_V1(void)
{
	//同步模式下，做“一次”读COM的操作（读空COM口），把一次读到的数据并存入PKT_BUFF格式的“总COM Rx Buff”中。
	//先会检查COM_RX内有多少数据，若有数据则做一次读操作。

	unsigned long ulExp_Read_Com, ulValid_Quan;
	unsigned char * ucpData = NULL;	//把数据从“串口BUFF”提取后存放的“临时内存”

	try
	{
		//检查COM Rx Buff中的数据量
		ulExp_Read_Com = this->Get_Com_Rx_Buff_Valid();

		if(ulExp_Read_Com <= 0)//串口BUFF中无数据可读
		{
			return true;
		}

		//串口中有新数据便读取
		//分配“待解析数据”的“临时内存”
		this->m_cla_GFC.BUFF_BYTE_CALLOC(&ucpData, (ulExp_Read_Com + 8));

		//读串口数据
		//注意，读的数据量必须小于你的BUFF，存入前一定测量一下实际数量，否则很容易溢出
		//串口中的“待读数据量”时刻在发生变化，先前探测的数据量很可能过了几毫秒不准了
		ulValid_Quan = 0;
		this->Read_Byte((char *)ucpData, ulExp_Read_Com, &ulValid_Quan);

		if(ulExp_Read_Com != ulValid_Quan)
		{
			this->m_csError_Info.Format(_T("ERROR, Read_Byte_Sync, Read byte fail, E = %d, R = %d!"), ulExp_Read_Com, ulValid_Quan);
			throw(0);
		}

		if(ulValid_Quan <= 0)
		{
			this->m_csError_Info.Format(_T("FAIL, Read_Byte_Sync, COM port read byte error!"));
			throw(0);
		}

		//添加到COM BUFF中
		if(this->m_cla_GFC.PKT_BUFF_ADD_DATA(&this->m_str_Com_Rx_Buff, ucpData, ulValid_Quan) == false)
		{
			this->m_csError_Info.Format(_T("FAIL, Read_Byte_Sync, COM buff add byte error!"));
			throw(0);
		}
	}
	catch(...)
	{
		//释放内存
		this->m_cla_GFC.BUFF_BYTE_FREE(&ucpData);
		return false;
	}

	//释放内存
	this->m_cla_GFC.BUFF_BYTE_FREE(&ucpData);
	return true;
}

bool CCom_Port::Read_Byte_Sync_V2(unsigned long ul_Exp_Rec_Byte_Quan, int i_TimeOut)
{
	//同步模式下，在超时时间内，读取“期望byte数量的数据”。
	//事先检查COM_RX内有多少数据。

	time_t t_Start, t_End;
	unsigned long ulActual_Valid_Quan;	//单次实际读出byte数量
	unsigned long ulExp_Read_Quan;		//单次期望读出byte数量
	unsigned long ulRec_Total_Quan = 0;	//读出说有byte数量统计
	unsigned char * ucpData = NULL;		//把数据从“串口BUFF”提取后存放的“临时内存”


	try
	{
		//计时开始
		time(&t_Start);

		//循环发送指令，并读取响应
		do
		{
			//预判
			ulExp_Read_Quan = this->Get_Com_Rx_Buff_Valid();
			//
			if(ulExp_Read_Quan <= 0)
			{
				ulExp_Read_Quan = 1;
			}


			//先释放内存
			this->m_cla_GFC.BUFF_BYTE_FREE(&ucpData);
			//分配“待解析数据”的“临时内存”
			this->m_cla_GFC.BUFF_BYTE_CALLOC(&ucpData, (ulExp_Read_Quan + 8));


			//读操作
			ulActual_Valid_Quan = 0;
			//
			if(this->Read_Byte((char *)ucpData, ulExp_Read_Quan, &ulActual_Valid_Quan) == true)
			{
				//读出总数统计
				ulRec_Total_Quan += ulActual_Valid_Quan;

				//读到数据，便加入Rx buff中
				if(ulActual_Valid_Quan > 0)
				{
					//添加到COM BUFF中
					if(this->m_cla_GFC.PKT_BUFF_ADD_DATA(&this->m_str_Com_Rx_Buff, ucpData, ulActual_Valid_Quan) == false)
					{
						this->m_csError_Info.Format(_T("FAIL, Read_Byte_Sync_V2, COM buff add byte error!"));
						throw(0);
					}
				}
			}


			//超时检测
			time(&t_End);
			//
			if(difftime(t_End, t_Start) >= i_TimeOut)
			{
				this->m_csError_Info.Format(_T("FAIL, Read_Byte_Sync_V2 timeout"));
				throw(0);
			}
		}
		while(ulRec_Total_Quan < ul_Exp_Rec_Byte_Quan);



		//释放内存
		this->m_cla_GFC.BUFF_BYTE_FREE(&ucpData);
	}
	catch(...)
	{
		//释放内存
		this->m_cla_GFC.BUFF_BYTE_FREE(&ucpData);

		return false;
	}

	return true;
}

bool CCom_Port::Query_Tgt_CHAR(const wchar_t * wcp_Send_Data, const wchar_t * wcp_Tgt_Data, int i_TimeOut)
{
	//针对“可视字符”类通信，发送指令后，检查是否有期望的应答，超时便报fail
	//需要把界面使用的wchar_t转换为串口通信用的char来核对

	time_t t_Start, t_End;
	char * cpTgt = NULL;
	int iTemp;

	 wchar_t wcp_Data[500];

	try
	{
		//在开始发命令前，清空总Rx Buff
		this->Empty_Com_And_Rx_Buff();

		//分配内存
		this->m_cla_GFC.BUFF_CHAR_CALLOC(&cpTgt, (wcslen(wcp_Tgt_Data) * 2 + 16));

		//转换目标字符
		this->m_cla_GFC.WideChar_To_MultiByte(wcp_Tgt_Data, cpTgt, &iTemp);

		//开始计时
		time(&t_Start);

		//发指令
		this->Write(wcp_Send_Data);

		do
		{
			//发指令
//			this->Write(wcp_Send_Data);

			Sleep(5);

			//读到“总buff”中
			this->Read_Byte_Sync_V1();

			//从“总buff”中核对目标数据
			if(strstr((char *)this->m_str_Com_Rx_Buff.ucp_Buff, cpTgt) != NULL)
			{
				break;
			}
			else
			{
				int len;
				this->m_cla_GFC.MultiByte_To_WideChar((char *)this->m_str_Com_Rx_Buff.ucp_Buff,wcp_Data,&len);
				//AfxMessageBox(wcp_Data);

			}

			//超时检测
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= i_TimeOut)//超时，报fail
			{
				this->m_csError_Info.Format(_T("FAIL, Query_Tgt_CHAR, Time out!"));
				throw(0);
			}
		}
		while(true);
	}
	catch(...)
	{
		//释放内存
		this->m_cla_GFC.BUFF_CHAR_FREE(&cpTgt);

		return false;
	}

	//释放内存
	this->m_cla_GFC.BUFF_CHAR_FREE(&cpTgt);

	return true;
}

void CCom_Port::GET_RX_PKT_BUFF_DATA(CString * csp_Rx_Data)
{
	//针对“可视字符”类通信，把“总Rx Buff”中的数据转换后载入CString

	wchar_t * wcpTemp_Data = NULL;
	int iLen;

	this->m_cla_GFC.BUFF_WCHAR_CALLOC(&wcpTemp_Data, (this->m_str_Com_Rx_Buff.ul_Data_Length + 16));

	this->m_cla_GFC.MultiByte_To_WideChar((char *)this->m_str_Com_Rx_Buff.ucp_Buff, wcpTemp_Data, &iLen);

	csp_Rx_Data->Format(_T("%s"), wcpTemp_Data);

	this->m_cla_GFC.BUFF_WCHAR_FREE(&wcpTemp_Data);
}

bool CCom_Port::GET_BUFF_BYTE(unsigned char ** ucp2_Buff_Pointer, unsigned long * ulp_Data_Length)
{
	//读串口，返回串口实际接收到数据的BUFF的地址和BUFF中的byte数据量
	bool bRtn = false;

	bRtn = this->Read_Byte_Sync_V1();

	*ucp2_Buff_Pointer = this->m_str_Com_Rx_Buff.ucp_Buff;
	*ulp_Data_Length = this->m_str_Com_Rx_Buff.ul_Data_Length;

	return bRtn;
}

void CCom_Port::DEL_BUFF_BYTE(unsigned long ul_Del_Data_Len)
{
	this->m_cla_GFC.PKT_BUFF_DEL_DATA(&this->m_str_Com_Rx_Buff, ul_Del_Data_Len);
}

bool CCom_Port::Empty_Com_And_Rx_Buff(void)
{
	bool ret =false;
	//清空COM缓存
	this->Clear_Rx_Buffer();
	//清空接收COM所有数据的BUFF
	ret = this->m_cla_GFC.PKT_BUFF_DEL_ALL(&this->m_str_Com_Rx_Buff);
	return ret;
}

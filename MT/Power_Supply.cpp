#include "StdAfx.h"
#include "Power_Supply.h"
#ifdef FW_MULTI
#ifndef FW_UPGRADE

/*
Ver1.0.4 (2020-6-22)

2020-5-16，xjli
1.添加了“AG_E3610X”系列电源的定义和处理。
2.修改了“AG_E36XX”系列电源的定义和处理。
3.添加了“LAN网线程控电源”的支持。

2020-5-19，xjli
1.添加了“KS_PMX”系列电源的定义和处理。

2020-6-5, Kite.hong
1.添加了“DH_17XX”系列电源的定义和处理。
2.添加了“GW_PFR100L”系列电源的定义和处理。

2020-6-22, xjli
1.电源定义“GW_PFR100L”变更为“GW_PFR_PSW”。因为支持了固伟PSW系列电源。
*/


//
CPower_Supply::CPower_Supply( void )
	: m_csError_Info( _T( "" ) )
	, m_csIDN( _T( "" ) )
	, m_iGPIB_Numb( 0 )
	, m_iGPIB_Addr( 0 )
	, m_iCtrl_Type_GPIB_Or_LAN( 0 )
	, m_ul_PS_Handle( NULL )
	, m_iPS_Type( 0 )
	, m_iPS_Model( 0 )
	, m_d_Init_Volt( 0 )
	, m_d_Init_Curr( 0 )
	, m_bCtrl_PS( false )
	, m_csIP_Instru( _T( "" ) )
{
	m_vise_DefaultRM = NULL;
	m_ul_PS_Handle = NULL;
}

CPower_Supply::~CPower_Supply( void )
{
}

int CPower_Supply::INIT_PS( void )
{
	wchar_t * wcpData = NULL;

	//
	try
	{
		//清空
		this->m_csError_Info.Format( _T( "" ) );
		this->m_csIDN.Format( _T( "" ) );


		//先释放电源句柄
		//放在“阶梯式下电”之后会有问题
		//NI函数“ibonl”（释放句柄操作）放到电源和仪器获取句柄之前。因为电源“阶梯下电”中有延迟时间，其中的ibwrt操作实际上会运行在ibonl之后，造成“阶梯下电”指令会发送失败。
		this->FREE_ALL();


		//------------------------------------- 打开、创建仪器句柄 ------------------------------------- 
		if( this->m_iCtrl_Type_GPIB_Or_LAN == GPIB_NI )// NI GPIB 卡
		{
			this->m_ul_PS_Handle = ibdev( this->m_iGPIB_Numb, this->m_iGPIB_Addr, 0, 13, 1, 0 );
			//
			if( ibsta & ERR )
			{
				this->m_csError_Info.Format( _T( "Power Supply ibdev Fail!" ) );
				throw(0);
			}

			//
			ibclr( this->m_ul_PS_Handle );
		}
		else if( (this->m_iCtrl_Type_GPIB_Or_LAN == GPIB_VISA) || (this->m_iCtrl_Type_GPIB_Or_LAN == GPIB_VISA_TCPIP) )// LAN 网线通信
		{
			char crCmd[260];
			int iTemp;
			CString csData;

			//Connect IQxstream 用NI Visa驱动, 如 viOpen & viOpenRM
			this->m_vist_Status = viOpenDefaultRM( &this->m_vise_DefaultRM );
			//判断返回值
			if( this->m_vist_Status != VI_SUCCESS )
			{
				this->m_csError_Info.Format( _T( "FAIL, INIT_PS, viOpenDefaultRM fail!" ) );
				throw(0);
			}

			//打包visa指令
			csData.Format( _T( "TCPIP::%s::inst0::INSTR" ), this->m_csIP_Instru );

			//清空
			memset( crCmd, 0, sizeof( crCmd ) );

			//转换单字符字串
			this->WideChar_To_MultiByte( csData, crCmd, &iTemp );

			//open仪器（第一次尝试）
			this->m_vist_Status = viOpen( this->m_vise_DefaultRM, crCmd, VI_NULL, VI_NULL, &this->m_ul_PS_Handle );
			//判断open仪器的返回值
			if( this->m_vist_Status != VI_SUCCESS )
			{
				int a = 0;
				int irPS_Port[4] = { 1026, 1790, 5025, 2268 };//不同电源的固定 LAN Port

				//继续尝试其他 LAN Port
				for( a = 0; a < 4; a++ )
				{
					//打包visa指令
					csData.Format( _T( "TCPIP::%s::%d::SOCKET" ), this->m_csIP_Instru, irPS_Port[a] );

					//清空
					memset( crCmd, 0, sizeof( crCmd ) );

					//转换单字符字串
					this->WideChar_To_MultiByte( csData, crCmd, &iTemp );

					//open仪器
					this->m_vist_Status = viOpen( this->m_vise_DefaultRM, crCmd, VI_NULL, VI_NULL, &this->m_ul_PS_Handle );

					if( this->m_vist_Status != VI_SUCCESS )
					{
						//还是失败，继续下一条指令
					}
					else
					{
						//open success
						break;
					}
				}//for( a = 0; a < 4; a++
				//判断循环尝试结果
				if( a >= 4 )
				{
					this->m_csError_Info.Format( _T( "FAIL, INIT_PS, viOpen fail!" ) );
					//fail
					throw(0);
				}
			}//if( this->m_vist_Status != VI_SUCCESS


			//设置VISA 属性: VI_ATTR_TERMCHAR_EN, VI_FALSE, VI_ATTR_SUPPRESS_END_EN, VI_FALSE
			viSetAttribute( this->m_ul_PS_Handle, VI_ATTR_TERMCHAR_EN, VI_FALSE );
			viSetAttribute( this->m_ul_PS_Handle, VI_ATTR_SUPPRESS_END_EN, VI_FALSE );
			viSetAttribute( this->m_ul_PS_Handle, VI_ATTR_TMO_VALUE, ViAttrState( 10000 ) );
		}
		else
		{
			//fail
			throw(0);
		}
		//-------------------------------------


		//------------------------------------- 识别电源型号 ------------------------------------- 
		//分配内存
		wcpData = (wchar_t *)calloc( 128, sizeof( wchar_t ) );

		//查询电源型号
		if( this->Query( _T( "*IDN?\n" ), wcpData ) != RTN_SUCCESS )
		{
			throw(0);
		}

		//加载数据
		this->m_csIDN.Format( _T( "%s" ), wcpData );

		//判断电源型号
		if( wcsstr( wcpData, _T( "KEITHLEY" ) ) != NULL )
		{
			this->m_iPS_Model = KEITHLEY;

			if( wcsstr( wcpData, _T( "2306" ) ) != NULL )
			{
				this->m_iPS_Type = TWO_CH;
			}
			else
			{
				this->m_iPS_Type = ONE_CH;
			}
		}
		else if( wcsstr( wcpData, _T( "E3610" ) ) != NULL )//E36100系列
		{
			this->m_iPS_Model = AG_E3610X;

			this->m_iPS_Type = ONE_CH;
		}
		else if( wcsstr( wcpData, _T( "E36" ) ) != NULL )//E3640系列
		{
			this->m_iPS_Model = AG_E36XX;

			this->m_iPS_Type = ONE_CH;
		}
		else if( (wcsstr( wcpData, _T( "KIKUSUI" ) ) != NULL) && (wcsstr( wcpData, _T( "PMX" ) ) != NULL) )
		{
			this->m_iPS_Model = KS_PMX;

			this->m_iPS_Type = ONE_CH;
		}
		else if( (wcsstr( wcpData, _T( "BJDH" ) ) != NULL) && (wcsstr( wcpData, _T( "DH17" ) ) != NULL) )
		{
			this->m_iPS_Model = DH_17XX;

			this->m_iPS_Type = ONE_CH;
		}
		else if( (wcsstr( wcpData, _T( "GW-INSTEK" ) ) != NULL) && (wcsstr( wcpData, _T( "PSW3" ) ) != NULL) )
		{
			//PSW30-36
			this->m_iPS_Model = GW_PFR_PSW;

			this->m_iPS_Type = ONE_CH;
		}
		else if( (wcsstr( wcpData, _T( "GW-INSTEK" ) ) != NULL) && (wcsstr( wcpData, _T( "PFR-100L" ) ) != NULL) )
		{
			//PFR-100L
			this->m_iPS_Model = GW_PFR_PSW;

			this->m_iPS_Type = ONE_CH;
		}
		else if( wcsstr( wcpData, _T( "IT61" ) ) != NULL )
		{
			this->m_iPS_Model = IT6100B;

			this->m_iPS_Type = ONE_CH;
		}
		else if (wcsstr(wcpData, _T("GPP")) != NULL)
		{
			this->m_iPS_Model = GW_GPP;

			this->m_iPS_Type = TWO_CH;
		}
		else
		{
			this->m_iPS_Model = AG_66;

			if( (wcsstr( wcpData, _T( "66309" ) ) != NULL) || (wcsstr( wcpData, _T( "66319" ) ) != NULL) )
			{
				this->m_iPS_Type = TWO_CH;
			}
			else
			{
				this->m_iPS_Type = ONE_CH;
			}
		}



		//恢复出厂设置
		if (GW_GPP == this->m_iPS_Model)
		{
			this->Write(_T("*RST\n"));
		}
		else
		{
			this->Query(_T("*CLS;*RST;*OPC?\n"), wcpData);
		}
		//
		Sleep( 1000 );



		//先关闭全部输出
		this->Power_Off( 1 );
		this->Power_Off( 2 );



		//------------------------------------- 初始化设置电源 ------------------------------------- 
		switch( this->m_iPS_Model )
		{
		case KEITHLEY:
			this->INIT_PS_KEITHLEY();
			break;

		case AG_66:
			this->INIT_PS_Agilent();
			break;

		case AG_E36XX:
		case AG_E3610X:
			this->INIT_PS_Agilent();
			break;

		case KS_PMX:
			this->INIT_PS_KS_PMX();
			break;

		case DH_17XX:
			this->INIT_PS_DH_17XX();
			break;

		case GW_PFR_PSW:
			this->INIT_PS_GW_PFR_PSW();
			break;

		case IT6100B:
			this->INIT_PS_IT6100B();
			break;

		case GW_GPP:
			this->INIT_PS_GW_GPP();
			break;

		default:
			this->INIT_PS_Agilent();
			break;
		}//switch( this->m_iPS_Model
		//-------------------------------------


		//先关闭全部输出
		this->Power_Off( 1 );
		this->Power_Off( 2 );


		//电源初始化完毕后获取电源系统错误
		if( false == this->CHECK_PS_SYSTEM_ERROR() )
		{
			throw(0);
		}

	}
	catch( ... )
	{
		//释放内存
		if( wcpData != NULL )
		{
			free( wcpData );
		}

		//fail
		return RTN_FAIL;
	}

	//释放内存
	if( wcpData != NULL )
	{
		free( wcpData );
	}

	//pass
	return RTN_SUCCESS;
}

int CPower_Supply::Write( const wchar_t * wcpSend_Data )
{
	char * cpWrite_Buff = NULL;
	int iTemp, iWrite_Size;

	//
	try
	{
		//测量发送数据的长度
		iWrite_Size = wcslen( wcpSend_Data );

		//分配内存
		cpWrite_Buff = (char *)calloc( (iWrite_Size * 2), sizeof( char ) );

		//加载数据，双char数据转为单char数据
		if( this->WideChar_To_MultiByte( wcpSend_Data, cpWrite_Buff, &iTemp ) == false )
		{
			throw(0);
		}

		//------------------------------------- 发送数据
		if( this->m_iCtrl_Type_GPIB_Or_LAN == GPIB_NI )
		{
			if( ibwrt( this->m_ul_PS_Handle, cpWrite_Buff, strlen( cpWrite_Buff ) ) & ERR )
			{
				ibclr( this->m_ul_PS_Handle );
				this->m_csError_Info.Format( _T( "Power Supply ibwrt Fail!" ) );
				throw(0);
			}
		}
		else if( (this->m_iCtrl_Type_GPIB_Or_LAN == GPIB_VISA) || (this->m_iCtrl_Type_GPIB_Or_LAN == GPIB_VISA_TCPIP) )
		{
			this->m_vist_Status = viPrintf( this->m_ul_PS_Handle, (ViString)cpWrite_Buff );
			if( this->m_vist_Status != VI_SUCCESS )
			{
				this->m_csError_Info.Format( _T( "Power Supply viPrintf Fail!" ) );
				throw(0);
			}
		}
		else
		{
			throw(0);
		}
	}
	catch( ... )
	{
		//释放内存
		if( cpWrite_Buff != NULL )
		{
			free( cpWrite_Buff );
			cpWrite_Buff = NULL;
		}

		//fail
		return RTN_FAIL;
	}

	//释放内存
	if( cpWrite_Buff != NULL )
	{
		free( cpWrite_Buff );
		cpWrite_Buff = NULL;
	}

	//pass
	return RTN_SUCCESS;
}

int CPower_Supply::Read( wchar_t * wcpRead_Data )
{
	char * cpRead_Buff = NULL;
	int iRead_Size = 512;//512个字符对于电源是足够的
	int iTemp;

	try
	{
		//分配内存
		cpRead_Buff = (char *)calloc( (iRead_Size + 8), sizeof( char ) );
		memset( cpRead_Buff, 0, (iRead_Size + 8) );

		//------------------------------------- 读数据
		if( this->m_iCtrl_Type_GPIB_Or_LAN == GPIB_NI )
		{
			if( ibrd( this->m_ul_PS_Handle, cpRead_Buff, iRead_Size ) & ERR )
			{
				ibclr( this->m_ul_PS_Handle );
				this->m_csError_Info.Format( _T( "Power Supply ibrd Fail!" ) );
				throw(0);
			}

			//NI读时，最后会自动添加‘\n’字符
			iTemp = strlen( cpRead_Buff ) - 1;
			if( *(cpRead_Buff + iTemp) == '\n' )
			{
				*(cpRead_Buff + iTemp) = '\0';
			}
		}
		else if( (this->m_iCtrl_Type_GPIB_Or_LAN == GPIB_VISA) || (this->m_iCtrl_Type_GPIB_Or_LAN == GPIB_VISA_TCPIP) )
		{
			this->m_vist_Status = viRead( this->m_ul_PS_Handle, (ViPBuf)cpRead_Buff, (ViUInt32)iRead_Size, (ViPUInt32)(&iTemp) );

			if( this->m_vist_Status != VI_SUCCESS )
			{
				this->m_csError_Info.Format( _T( "Power Supply viRead Fail!" ) );
				throw(0);
			}
			cpRead_Buff[iTemp - 1] = '\0';
		}
		else
		{
			throw(0);
		}


		//串口读出的数据需要转化为wchar_t的双字符才能用于界面显示
		this->MultiByte_To_WideChar( cpRead_Buff, wcpRead_Data, &iTemp );

	}
	catch( ... )
	{
		//释放内存
		if( cpRead_Buff != NULL )
		{
			free( cpRead_Buff );
			cpRead_Buff = NULL;
		}

		//fail
		return RTN_FAIL;
	}

	//释放内存
	if( cpRead_Buff != NULL )
	{
		free( cpRead_Buff );
		cpRead_Buff = NULL;
	}

	//pass
	return RTN_SUCCESS;
}

bool CPower_Supply::WideChar_To_MultiByte( const wchar_t * wcp_WCHAR_16, char * cp_Char_8, int * ip_Len_8 )
{
	//Unicode，wchar_t* ---> utf8编码，char*

	int iSize = WideCharToMultiByte( CP_ACP, 0, wcp_WCHAR_16, -1, NULL, 0, NULL, NULL );

	//
	if( !WideCharToMultiByte( CP_ACP, 0, wcp_WCHAR_16, -1, cp_Char_8, iSize, NULL, NULL ) )
	{
		//fail
		return false;
	}

	//
	*ip_Len_8 = iSize;

	//pass
	return true;
}

void CPower_Supply::MultiByte_To_WideChar( const char * cp_Char_8, wchar_t * wcp_WCHAR_16, int * ip_Len_16 )
{
	//utf8编码，char*  ---> Unicode，wchar_t*

	int nLen = strlen( cp_Char_8 ) + 1;
	int nwLen = MultiByteToWideChar( CP_ACP, 0, cp_Char_8, nLen, NULL, 0 );

	MultiByteToWideChar( CP_ACP, 0, cp_Char_8, nLen, wcp_WCHAR_16, nwLen );//CP_UTF8(中文字无法显示)

	*ip_Len_16 = nwLen - 1;//注意：nwLen这个长度（字符数）是包含了终止符（\0）的

	*(wcp_WCHAR_16 + nwLen - 1) = '\0';
}

int CPower_Supply::Query( const wchar_t * wcpSend_Data, wchar_t * wcpRead_Data )
{
	if( this->Write( wcpSend_Data ) == RTN_SUCCESS )
	{
		Sleep( 200 );

		return this->Read( wcpRead_Data );
	}
	else
	{
		return RTN_FAIL;
	}
}

int CPower_Supply::INIT_PS_KEITHLEY( void )
{
	this->Set_Volt( 1, this->m_d_Init_Volt );

	this->Set_Curr( 1, this->m_d_Init_Curr );

	this->Set_Test_Point( 5 );

	return RTN_SUCCESS;
}

int CPower_Supply::INIT_PS_Agilent( void )
{
	this->Set_Volt( 1, this->m_d_Init_Volt );

	this->Set_Curr( 1, this->m_d_Init_Curr );

	this->Set_Test_Point( 500 );

	//针对Agilent 66309、66319两个输出端口的独立工作
	if( this->m_iPS_Type == TWO_CH )
	{
		this->Write( _T( "INST:COUP:OUTP:STAT NONE\n" ) );
	}

	//返回
	return RTN_SUCCESS;
}

int CPower_Supply::INIT_PS_GW_GPP(void)
{
	this->Set_Volt(1, this->m_d_Init_Volt);

	this->Set_Curr(1, this->m_d_Init_Curr);

	//	this->Set_Test_Point(5);

	return RTN_SUCCESS;
}
void CPower_Supply::Set_Volt( int iCH, double dVolt )
{
	CString csCmd;

	//
	switch( this->m_iPS_Type )
	{
	case TWO_CH:
		{
			if( iCH <= 2 )
			{
				if( this->m_iPS_Model == KEITHLEY )
				{
					csCmd.Format( _T( "SOUR%d:VOLT %.3f\n" ), iCH, dVolt );
				}
				else if( this->m_iPS_Model == AG_66 )
				{
					csCmd.Format( _T( "SOUR:VOLT%d %.3f\n" ), iCH, dVolt );
				}
				else if (this->m_iPS_Model == GW_GPP)
				{
					csCmd.Format(_T(":VSET%d:%.3f\n"), iCH, dVolt);
				}
				else
				{
					csCmd.Format( _T( "SOUR:VOLT%d %.3f\n" ), iCH, dVolt );
				}

				break;
			}
			else
			{
				return;
			}
		}
		//
		break;

	case ONE_CH:
		{
			if( iCH == 1 )
			{
				switch( this->m_iPS_Model )
				{
				case KEITHLEY:
					csCmd.Format( _T( "VOLT %.3f\n" ), dVolt );
					break;
				case AG_66:
					csCmd.Format( _T( "VOLT %.3f\n" ), dVolt );
					break;
				case KS_PMX:
					csCmd.Format( _T( "VOLTage %.3f\n" ), dVolt );
					break;
				case DH_17XX:
					csCmd.Format( _T( "VOLTage %.3f\n" ), dVolt );
					break;
				case GW_PFR_PSW:
					csCmd.Format( _T( "VOLTage %.3f\n" ), dVolt );
					break;
				case IT6100B:
					csCmd.Format( _T( "SOURce:VOLTage:LEVel %.3fV\n" ), dVolt );//命令语法：[SOURce:]VOLTage[:LEVel] <NRf>
					break;
				default:
				//E3460A大于8V要切换20V档位
				if ( dVolt > 8.0 )
				{
					csCmd.Format ( _T ( "SOUR:VOLT:RANG HIGH\n" )  );
					this->Write ( csCmd );
				}
					csCmd.Format( _T( "VOLT %.3f\n" ), dVolt );
					break;
				}

				//
				break;
			}
			else
			{
				return;
			}
		}
		//
		break;

	default:
		return;
	}//switch( this->m_iPS_Type


	//发送指令
	this->Write( csCmd );
}

void CPower_Supply::Set_Curr( int iCH, double dCurr )
{
	CString csCmd;

	switch( this->m_iPS_Type )
	{
	case TWO_CH:
		if( iCH <= 2 )
		{
			if( this->m_iPS_Model == KEITHLEY )
			{
				csCmd.Format( _T( ":SOUR%d:CURR:LIM %.3f\n" ), iCH, dCurr );
			}
			else if( this->m_iPS_Model == AG_66 )
			{
				csCmd.Format( _T( "SOUR:CURR%d %.3f\n" ), iCH, dCurr );
			}
			else if (this->m_iPS_Model == GW_GPP)
			{
				csCmd.Format(_T(":ISET%d:%.3f\n"), iCH, dCurr);
			}
			else
			{
				csCmd.Format( _T( "SOUR:CURR%d %.3f\n" ), iCH, dCurr );
			}

			break;
		}
		else
		{
			return;
		}
	case ONE_CH:
		if( iCH == 1 )
		{
			switch( this->m_iPS_Model )
			{
			case KEITHLEY:
				csCmd.Format( _T( ":SOUR:CURR:LIM %.3f\n" ), dCurr );
				break;
			case AG_66:
				csCmd.Format( _T( "CURR %.3f\n" ), dCurr );
				break;
			case KS_PMX:
				csCmd.Format( _T( "CURRent %.3f\n" ), dCurr );
				break;
			case DH_17XX:
				csCmd.Format( _T( "CURRent %.3f\n" ), dCurr );
				break;
			case GW_PFR_PSW:
				csCmd.Format( _T( "CURRent %.3f\n" ), dCurr );
				break;
			case IT6100B:
				csCmd.Format( _T( "SOURce:CURRent:LEVel %.3fA\n" ), dCurr );//命令语法：[SOURce:]CURRent [:LEVel] <NRf>
				break;
			default:

				//E3640A 20V档位限流最大1.5A
				if ( this->m_d_Init_Volt > 8.0 && dCurr > 1.5 )
				{
					csCmd.Format ( _T ( "CURR %.3f\n" ) , 1.500 );
				}
				else
				{
					csCmd.Format ( _T ( "CURR %.3f\n" ) , dCurr );
				}
				break;
			}

			//
			break;
		}
		else
		{
			return;
		}
	default:
		return;
	}

	//发送指令
	this->Write( csCmd );
}

void CPower_Supply::Set_Test_Point( int iPoint )
{
	CString csCmd;

	switch( this->m_iPS_Model )
	{
	case KEITHLEY:
		csCmd.Format( _T( "SENS:AVER %d\n" ), iPoint );
		break;
	case AG_66:
		csCmd.Format( _T( "SENS:SWE:POIN %d\n" ), iPoint );
		break;
	case GW_GPP:
		csCmd.Format(_T("SENS:SWE:POIN %d\n"), iPoint);
		break;
	case IT6100B:
		csCmd.Format( _T( "SENSe:AVERage:COUNt %d\n" ), iPoint );//命令语法：SENSe:AVERage:COUNt <NR1 | MIN | MAX | DEF>
		break;
	default:
		return;
	}

	//
	this->Write( csCmd );
}

void CPower_Supply::Power_On( int iCH )
{
	CString csCmd;

	switch( this->m_iPS_Type )
	{
	case TWO_CH:
		if( iCH <= 2 )
		{
			if( this->m_iPS_Model == KEITHLEY )
			{
				csCmd.Format( _T( "OUTP%d ON\n" ), iCH );
			}
			else if( this->m_iPS_Model == AG_66 )
			{
				csCmd.Format( _T( "OUTP%d ON\n" ), iCH );
			}
			else if (this->m_iPS_Model == GW_GPP)  //cls.Write(":OUTPut1:STATe ON");   
			{
				csCmd.Format(_T(":OUTPut%d:STATe ON\n"), iCH);
			}
			else
			{
				csCmd.Format( _T( "OUTP%d ON\n" ), iCH );
			}

			break;
		}
		else
		{
			return;
		}

	case ONE_CH:
		if( iCH == 1 )
		{
			switch( this->m_iPS_Model )
			{
			case KEITHLEY:
				csCmd.Format( _T( "OUTP ON\n" ) );
				break;

			case AG_66:
			case AG_E36XX:
			case AG_E3610X:
				csCmd.Format( _T( "OUTP ON\n" ) );
				break;

			case KS_PMX:
			case DH_17XX:
			case GW_PFR_PSW:
				csCmd.Format( _T( "OUTPut ON\n" ) );
				break;

			case IT6100B:
				csCmd.Format( _T( "OUTPut:STATe ON\n" ) );//命令语法：OUTPut[:STATe] <bool>
				break;

			default:
				csCmd.Format( _T( "OUTP ON\n" ) );
				break;
			}

			//
			break;
		}
		else
		{
			return;
		}

	default:
		return;
	}

	//发送指令
	this->Write( csCmd );
}

void CPower_Supply::Power_Off( int iCH )
{
	CString csCmd;

	switch( this->m_iPS_Type )
	{
	case TWO_CH:
		if( iCH <= 2 )
		{
			if( this->m_iPS_Model == KEITHLEY )
			{
				csCmd.Format( _T( "OUTP%d OFF\n" ), iCH );
			}
			else if( this->m_iPS_Model == AG_66 )
			{
				csCmd.Format( _T( "OUTP%d OFF\n" ), iCH );
			}
			else if (this->m_iPS_Model == GW_GPP)
			{
				csCmd.Format(_T(":OUTPut%d:STATe OFF\n"), iCH);//:OUTPut1:STATe ON
			}
			else
			{
				csCmd.Format( _T( "OUTP%d OFF\n" ), iCH );
			}

			break;
		}
		else
		{
			return;
		}

	case ONE_CH:
		if( iCH == 1 )
		{
			switch( this->m_iPS_Model )
			{
			case KEITHLEY:
				csCmd.Format( _T( "OUTP OFF\n" ) );
				break;

			case AG_66:
			case AG_E36XX:
			case AG_E3610X:
				csCmd.Format( _T( "OUTP OFF\n" ) );
				break;

			case KS_PMX:
			case DH_17XX:
			case GW_PFR_PSW:
				csCmd.Format( _T( "OUTPut OFF\n" ) );//OUTPut 0(0表示关闭)
				break;

			case IT6100B:
				csCmd.Format( _T( "OUTPut:STATe OFF\n" ) );//命令语法：OUTPut[:STATe] <bool>
				break;

			default:
				csCmd.Format( _T( "OUTP OFF\n" ) );
				break;
			}

			//
			break;
		}
		else
		{
			return;
		}

	default:
		return;
	}


	//发送指令
	this->Write( csCmd );
}

bool CPower_Supply::Meas_Volt( int iCH, double * dpVolt )
{
	wchar_t * wcpData = NULL;
	CString csCmd;

	try
	{
		wcpData = (wchar_t *)calloc( 128, sizeof( wchar_t ) );

		switch( this->m_iPS_Type )
		{
		case TWO_CH:
			if( iCH <= 2 )
			{
				if( this->m_iPS_Model == KEITHLEY )
				{
					csCmd.Format( _T( "MEAS%d:VOLT?\n" ), iCH );
					this->Query( csCmd, wcpData );
					*dpVolt = _wtof( wcpData );
				}
				else if( this->m_iPS_Model == AG_66 )
				{
					this->Write( _T( "SENS:FUNC \"VOLT\"\n" ) );
					csCmd.Format( _T( "MEAS:VOLT%d?\n" ), iCH );
					this->Query( csCmd, wcpData );
					*dpVolt = _wtof( wcpData );
				}
				else if (this->m_iPS_Model == GW_GPP)
				{
					csCmd.Format(_T(":MEAS%d:VOLT?\n"), iCH);
					this->Query(csCmd, wcpData);
					*dpVolt = _wtof(wcpData);
				}
				else
				{
					csCmd.Format( _T( "MEAS:VOLT%d?\n" ), iCH );
					this->Query( csCmd, wcpData );
					*dpVolt = _wtof( wcpData );
				}

				break;
			}
			else
			{
				throw(0);
			}

		case ONE_CH:
			if( iCH == 1 )
			{
				switch( this->m_iPS_Model )
				{
				case KEITHLEY:
					this->Query( _T( "MEAS:VOLT?\n" ), wcpData );
					*dpVolt = _wtof( wcpData );
					break;
				case AG_66:
					this->Write( _T( "SENS:FUNC \"VOLT\"\n" ) );
					this->Query( _T( "MEAS:VOLT?\n" ), wcpData );
					*dpVolt = _wtof( wcpData );
					break;
				case IT6100B:
					this->Query( _T( "MEASure:VOLTage:DC?\n" ), wcpData );//命令语法： MEASure:VOLTage[:DC]?
					*dpVolt = _wtof( wcpData );
					break;
				case KS_PMX:
					this->Query( _T( "MEASure:VOLTage?\n" ), wcpData );
					*dpVolt = _wtof( wcpData );
					break;
				case DH_17XX:
					this->Query( _T( "MEASure:VOLTage?\n" ), wcpData );
					*dpVolt = _wtof( wcpData );
					break;
				case GW_PFR_PSW:
					this->Query( _T( "MEASure:VOLTage?\n" ), wcpData );
					*dpVolt = _wtof( wcpData );
					break;
				default:
					this->Query( _T( "MEAS:VOLT?\n" ), wcpData );
					*dpVolt = _wtof( wcpData );
					break;
				}

				//
				break;
			}
			else
			{
				throw(0);
			}

		default:
			throw(0);
		}
	}
	catch( ... )
	{
		//释放内存
		if( wcpData != NULL )
		{
			free( wcpData );
		}

		//fail
		return false;
	}

	//释放内存
	if( wcpData != NULL )
	{
		free( wcpData );
	}

	//pass
	return true;
}

bool CPower_Supply::Meas_Curr( int iCH, double * dpCurr )
{
	wchar_t * wcpData = NULL;
	CString csCmd;

	//
	try
	{
		wcpData = (wchar_t *)calloc( 128, sizeof( wchar_t ) );

		switch( this->m_iPS_Type )
		{
		case TWO_CH:
			if( iCH <= 2 )
			{
				if( this->m_iPS_Model == KEITHLEY )
				{
					csCmd.Format( _T( "MEAS%d:CURR?\n" ), iCH );
					this->Query( csCmd, wcpData );
					*dpCurr = _wtof( wcpData );
				}
				else if( this->m_iPS_Model == AG_66 )
				{
					this->Write( _T( "SENS:CURR:DET DC\n" ) );
					this->Write( _T( "SENS:FUNC \"CURR\"\n" ) );
					csCmd.Format( _T( "MEAS:CURR%d?\n" ), iCH );
					this->Query( csCmd, wcpData );
					*dpCurr = _wtof( wcpData );
				}
				else if (this->m_iPS_Model == GW_GPP)
				{
					csCmd.Format(_T(":MEAS%d:CURR?\n"), iCH);
					this->Query(csCmd, wcpData);
					*dpCurr = _wtof(wcpData);
				}
				else
				{
					csCmd.Format( _T( "MEAS:CURR%d?\n" ), iCH );
					this->Query( csCmd, wcpData );
					*dpCurr = _wtof( wcpData );
				}

				//
				break;
			}
			else
			{
				throw(0);
			}

		case ONE_CH:
			if( iCH == 1 )
			{
				switch( this->m_iPS_Model )
				{
				case KEITHLEY:
					this->Query( _T( "MEAS:CURR?\n" ), wcpData );
					*dpCurr = _wtof( wcpData );
					break;
				case AG_66:
					this->Write( _T( "SENS:CURR:DET DC\n" ) );
					this->Write( _T( "SENS:FUNC \"CURR\"\n" ) );
					this->Query( _T( "MEAS:CURR?\n" ), wcpData );
					*dpCurr = _wtof( wcpData );
					break;
				case IT6100B:
					this->Query( _T( "MEASure:CURRent:DC?\n" ), wcpData );//命令语法： MEASure:CURRent[:DC]?
					*dpCurr = _wtof( wcpData );
					break;
				case KS_PMX:
					this->Query( _T( "MEASure:CURRent?\n" ), wcpData );
					*dpCurr = _wtof( wcpData );
					break;
				case DH_17XX:
					this->Query( _T( "MEASure:CURRent?\n" ), wcpData );
					*dpCurr = _wtof( wcpData );
					break;
				case GW_PFR_PSW:
					this->Query( _T( "MEASure:CURRent?\n" ), wcpData );
					*dpCurr = _wtof( wcpData );
					break;
				default:
					this->Query( _T( "MEAS:CURR?\n" ), wcpData );
					*dpCurr = _wtof( wcpData );
					break;
				}

				//
				break;
			}
			else
			{
				throw(0);
			}

		default:
			throw(0);
		}
	}
	catch( ... )
	{
		//释放内存
		if( wcpData != NULL )
		{
			free( wcpData );
		}

		//fail
		return false;
	}

	//释放内存
	if( wcpData != NULL )
	{
		free( wcpData );
	}

	//pass
	return true;
}

void CPower_Supply::Close_VISA( void )
{
	if( this->m_vise_DefaultRM != NULL )
	{
		viClear( this->m_ul_PS_Handle );
		viClose( this->m_ul_PS_Handle );
		viClose( this->m_vise_DefaultRM );

		//清空句柄
		this->m_ul_PS_Handle = NULL;
		this->m_vise_DefaultRM = NULL;
	}
}

int CPower_Supply::INIT_PS_IT6100B( void )
{
	this->Set_Volt( 1, this->m_d_Init_Volt );

	this->Set_Curr( 1, this->m_d_Init_Curr );

	this->Set_Test_Point( 50 );

	//这条命令用来设置SOURCE METER 为远程控制模式
	this->Write( _T( "SYSTem:REMote\n" ) );

	return RTN_SUCCESS;
}

void CPower_Supply::POWER_ON_V1( int i_CH )
{
	//阶段式上电、打开电源输出

	int i;
	double drVolt_List[4] = { 0.0, 1.0, 2.0 };//电压升到2.4V和3.0V时, 有可能开机就会出现问题

	//
	for( i = 0; i < 3; i++ )
	{
		//设置电压值
		this->Set_Volt( i_CH, drVolt_List[i] );

		//打开输出（只一次）
		if( i == 0 )
		{
			//打开电源输出
			this->Power_On( i_CH );
		}

		//延迟
		Sleep( 330 );
	}

	//最后切到“实际输出电压”
	this->Set_Volt( i_CH, this->m_d_Init_Volt );

	//延迟
	Sleep( 330 );

	//再次设置“实际输出电压”，防止GPIB丢数据
	this->Set_Volt( i_CH, this->m_d_Init_Volt );
}

void CPower_Supply::POWER_OFF_V1( int i_CH )
{
	int i;
	double drVolt_List[4] = { 2.0, 1.0, 0.0 };


	//
	for( i = 0; i < 3; i++ )
	{
		//设置电压值
		this->Set_Volt( i_CH, drVolt_List[i] );

		//延迟
		Sleep( 300 );
	}


	//关闭输出
	this->Power_Off( i_CH );
}


bool CPower_Supply::CHECK_PS_SYSTEM_ERROR()
{
	wchar_t * wcpData = NULL;
	CString csData;

	//
	try
	{
		//分配内存
		wcpData = (wchar_t *)calloc( 128, sizeof( wchar_t ) );

		//
		switch( this->m_iPS_Model )
		{
		case KEITHLEY:
			this->Query( _T( ":SYSTem:ERRor?\n" ), wcpData );
			break;

		case AG_66:
		case AG_E36XX:
		case IT6100B:
		case AG_E3610X:
		case KS_PMX:
		case DH_17XX:
		case GW_PFR_PSW:
			this->Query( _T( "SYSTem:ERRor?\n" ), wcpData );
			break;

		default:
			Sleep(1000);
			this->Query( _T( "SYSTem:ERRor?\n" ), wcpData );
			break;
		}

		//转换大写
		csData.Format( _T( "%s" ), wcpData );
		csData.MakeUpper();


		//判定返回结果
		if( NULL == _tcsstr( csData, _T( "NO ERROR" ) ) )
		{
			//没找到"NO ERROR"关键词，有错误发生
			//保存错误信息
			this->m_csError_Info.Format( _T( "ERROR, PS has error(%s)!" ), wcpData );
			//fail
			throw(0);
		}


		//释放内存
		if( wcpData != NULL )
		{
			free( wcpData );
		}

	}
	catch( ... )
	{
		//释放内存
		if( wcpData != NULL )
		{
			free( wcpData );
		}

		//fail
		return false;
	}

	//pass
	return true;
}


void CPower_Supply::FREE_ALL()
{
	switch( this->m_iCtrl_Type_GPIB_Or_LAN )
	{
	case GPIB_NI://NI API
		this->Close_NI_Device();
		//
		break;

	case GPIB_VISA://VISA API
	case GPIB_VISA_TCPIP:
		this->Close_VISA();
		//
		break;

	default:
		//
		break;
	}//switch( this->m_iGPIB_Type
}


void CPower_Supply::Close_NI_Device()
{
	//NI
	if( this->m_ul_PS_Handle != NULL )
	{
		//Place the device or interface online or offline.
		//if v is zero, the device or interface is placed offline.
		//If v is non-zero, the device or interface is left operational, or online.
		ibonl( this->m_ul_PS_Handle, 0 );

		//清空句柄
		this->m_ul_PS_Handle = NULL;
		this->m_vise_DefaultRM = NULL;
	}
}


void CPower_Supply::INIT_PS_KS_PMX()
{
	this->Set_Volt( 1, this->m_d_Init_Volt );

	this->Set_Curr( 1, this->m_d_Init_Curr );
}

void CPower_Supply::INIT_PS_DH_17XX()
{
	this->Set_Volt( 1, this->m_d_Init_Volt );

	this->Set_Curr( 1, this->m_d_Init_Curr );
}

void CPower_Supply::INIT_PS_GW_PFR_PSW()
{
	this->Set_Volt( 1, this->m_d_Init_Volt );

	this->Set_Curr( 1, this->m_d_Init_Curr );
}

#endif
#endif



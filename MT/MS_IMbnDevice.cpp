#include "stdafx.h"
#include "MS_IMbnDevice.h"


CMS_IMbnDevice::CMS_IMbnDevice ()
{

}


CMS_IMbnDevice::~CMS_IMbnDevice ()
{

}

bool CMS_IMbnDevice::Open_IMbn_Device(BOOL bIsSleep)
{
	CComPtr<IMbnInterface>  pInf = NULL;
	MBN_INTERFACE_CAPS interfaceCaps;
	SAFEARRAY *psa = NULL;
	HRESULT hr = E_FAIL;
	LONG lLower;
	LONG lUpper;
	BSTR InterfaceID;
	CString csDeviceID;
	unsigned long _req_id;


	try
	{
		//清空
		this->_mbn_dev_svc_mgr = NULL;
		this->_mnb_dev_svc_cxt = NULL;
		this->_mbn_dev_svc = NULL;
		this->g_InterfaceMgr = NULL;


		//INIT COM组件
		hr = CoInitializeEx ( NULL , COINIT_MULTITHREADED );
		if ( FAILED ( hr ) )
		{
			TRACE(_T("INIT COM组件 fail\n"));
			throw(0);
		}


		//创建IMbnDeviceServicesManager
		hr = CoCreateInstance ( CLSID_MbnDeviceServicesManager ,
			NULL ,
			CLSCTX_ALL ,
			IID_IMbnDeviceServicesManager ,
			( void** ) &_mbn_dev_svc_mgr );
		if ( FAILED ( hr ) )
		{
			TRACE(_T("创建IMbnDeviceServicesManager fail\n"));
			throw(0);
		}

		//创建IMBN Interface Manager
		hr = CoCreateInstance ( CLSID_MbnInterfaceManager ,
			NULL ,
			CLSCTX_ALL ,
			IID_IMbnInterfaceManager ,
			( void** ) &g_InterfaceMgr );
		if ( FAILED ( hr ) )
		{
			TRACE(_T("创建IMBN Interface Manager fail\n"));
			throw(0);
		}

		if (bIsSleep)
		{
			Sleep(3000);
		}

		//通过IMBN Interface Manager获取设备信息
		hr = g_InterfaceMgr->GetInterfaces ( &psa );
		if ( FAILED ( hr ) )
		{
			TRACE(_T("通过IMBN Interface Manager获取设备信息 fail\n"));
			throw(0);
		}

		//获取设备LBound
		hr = SafeArrayGetLBound ( psa , 1 , &lLower );
		if ( FAILED ( hr ) )
		{

			TRACE(_T("获取设备LBound fail\n"));
			throw(0);
		}

		//获取设备UBound
		hr = SafeArrayGetUBound ( psa , 1 , &lUpper );
		if ( FAILED ( hr ) )
		{
			TRACE(_T("获取设备UBound fail\n"));
			throw(0);
		}


		if ( lLower <= lLower )
		{
			//（单设备lLower和lUpper相等）
			hr = SafeArrayGetElement ( psa , &lLower , ( void* ) ( &pInf ) );
			if ( FAILED ( hr ) )
			{
				TRACE(_T("（单设备lLower和lUpper相等） fail\n"));
				throw(0);
			}

			//通过IMbnInterface获取ID
			hr = pInf->get_InterfaceID ( &InterfaceID );
			if ( FAILED ( hr ) )
			{
				TRACE(_T("通过IMbnInterface获取ID fail\n"));
				throw(0);
			}

			//通过InterfaceID获取MBN_INTERFACE_CAPS
			hr = pInf->GetInterfaceCapability ( &interfaceCaps );
			if ( FAILED ( hr ) )
			{
				TRACE(_T("通过InterfaceID获取MBN_INTERFACE_CAPS fail\n"));
				throw(0);
			}

			//通过MBN_INTERFACE_CAPS获取DeviceID
			csDeviceID = interfaceCaps.deviceID;


			//通过DeviceServicesManager + InterfaceID获取DeviceServicesContext
			hr = _mbn_dev_svc_mgr->GetDeviceServicesContext ( InterfaceID , &_mnb_dev_svc_cxt );
			if ( FAILED ( hr ) )
			{
				TRACE(_T("通过DeviceServicesManager + InterfaceID获取DeviceServicesContext fail\n"));
				throw(0);
			}


			//通过ID创建IMbnDeviceService
			// "{6427015F-579D-48F5-8C54-F43ED1E76F83}" ==> EM06-J
			hr = _mnb_dev_svc_cxt->GetDeviceService ( _T ( "{6427015F-579D-48F5-8C54-F43ED1E76F83}" ) , &_mbn_dev_svc );
			if ( FAILED ( hr ) )
			{
				TRACE(_T("通过ID创建IMbnDeviceService fail\n"));
				throw(0);
			}

			//打开DeviceService数据传输
			hr = _mbn_dev_svc->OpenCommandSession ( &_req_id );
			if ( FAILED ( hr ) )
			{
				TRACE(_T("打开DeviceService数据传输 fail\n"));
				throw(0);
			}

			//等待稳定(必须)
			Sleep ( 2000 );
		}
		else
		{
			//没有设备
			TRACE(_T("没有设备 fail\n"));
			throw(0);
		}

	}
	catch ( ... )
	{
		return false;
	}

	return true;
}

bool CMS_IMbnDevice::Open_Debug_Port ()
{
	unsigned char data[2] = { 0x11 , 0x00 };
	unsigned char cid = 0x05;

	try
	{
		//打开IMbn设备
		if ( false == Open_IMbn_Device () )
		{
			throw( 0 );
		}

		//发送指定消息
		if ( false == Send_specific_cid_command ( cid , data ) )
		{
			throw( 0 );
		}

	}
	catch ( ... )
	{
		//Uninit COM组件
		CoUninitialize ();
		return false;
	}

	//Uninit COM组件
	CoUninitialize ();
	return true;
}

bool CMS_IMbnDevice::Close_Debug_Port ()
{
	unsigned char data[2] = { 0x00 , 0x11 };
	unsigned char cid = 0x05;
	try
	{
		//打开IMbn设备
		if ( false == Open_IMbn_Device (TRUE) )
		{
			TRACE(_T("pen_IMbn_Device fail\n"));
			throw( 0 );
		}

		//发送指定消息
		if ( false == Send_specific_cid_command ( cid , data ) )
		{
			TRACE(_T("Send_specific_cid_command fail\n"));
			throw(0);
		}

	}
	catch ( ... )
	{
		//Uninit COM组件
		CoUninitialize ();
		return false;
	}

	//Uninit COM组件
	CoUninitialize ();
	return true;
}

bool CMS_IMbnDevice::Open_QDloader_Port ()
{
	unsigned char data[2] = { 0x00 , 0x00 };
	unsigned char cid = 0x04;
	try
	{
		//打开IMbn设备
		if ( false == Open_IMbn_Device () )
		{
			throw( 0 );
		}

		//发送指定消息
		if ( false == Send_specific_cid_command ( cid , data ) )
		{
			throw( 0 );
		}

	}
	catch ( ... )
	{
		//Uninit COM组件
		CoUninitialize ();
		return false;
	}

	//Uninit COM组件
	CoUninitialize ();
	return true;
}

bool CMS_IMbnDevice::Send_specific_cid_command ( unsigned char cid , unsigned char data[] )
{
	HRESULT hr = E_FAIL;
	unsigned long _req_id_Ex;

	//定义CComSafeArray ATL模板库
	CComSafeArray<BYTE> csa ( 2 , 0 );

	try
	{
		//数据载入CComSafeArray ATL模块库
		csa.SetAt ( 0 , data[0] );
		csa.SetAt ( 1 , data[1] );

		//CComSafeArray转换LPSAFEARRAY
		LPSAFEARRAY *psa = csa.GetSafeArrayPtr ();

		//执行发送
		hr = _mbn_dev_svc->SetCommand ( cid , *psa , &_req_id_Ex );
		if ( FAILED ( hr ) )
		{
			throw( 0 );
		}
	}
	catch ( ... )
	{
		return false;
	}
	return true;
}
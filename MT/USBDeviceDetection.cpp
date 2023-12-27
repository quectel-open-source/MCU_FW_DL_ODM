#include "stdafx.h"
#include "USBDeviceDetection.h"

CUSBDeviceDetection::CUSBDeviceDetection()
{

}

CUSBDeviceDetection::~CUSBDeviceDetection()
{

}

CString CUSBDeviceDetection::GetParentDevicePhysicalAddress(CString devicePhysicalAddress)
{
	return devicePhysicalAddress.Left(devicePhysicalAddress.ReverseFind('#'));            
}

BOOL  CUSBDeviceDetection::UnicodeToMultiByte(LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,NULL,0,NULL,FALSE) - 1;
	if(dwSize < dwMinSize)
	{
		return FALSE;
	}
	WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,lpszStr,dwSize,NULL,FALSE);
	return TRUE;
}


int CUSBDeviceDetection::GetConnectionIndex(CString DevicePath, CString DriverKeyName)
{
	int ConnectionIndex = 0;
	HDEVINFO deviceInfoSet = INVALID_HANDLE_VALUE;

	while(deviceInfoSet == INVALID_HANDLE_VALUE)
	{
		DeleteFile(DevicePath);
		deviceInfoSet = CreateFile(DevicePath, GENERIC_WRITE, FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
	}

	if (deviceInfoSet != INVALID_HANDLE_VALUE)
	{
		int HubPortCount = 0;
		DWORD nBytesReturned;  
		USB_NODE_INFORMATION Buffer;
		if (DeviceIoControl(deviceInfoSet,
			IOCTL_USB_GET_NODE_INFORMATION,
			&Buffer,
			sizeof(Buffer),
			&Buffer,
			sizeof(Buffer),
			&nBytesReturned,
			0))
		{
			if (Buffer.NodeType == UsbHub)
			{
				HubPortCount = Buffer.u.HubInformation.HubDescriptor.bNumberOfPorts;         // Port Number
			}
			else
			{
				HubPortCount = 0;
			}

			// loop thru all of the ports on the hub
			// BTW: Ports are numbered starting at 1
			for (int i = 1; i <= HubPortCount; i++)
			{
				USB_NODE_CONNECTION_DRIVERKEY_NAME NodeKeyName;
				NodeKeyName.ConnectionIndex = i;
				if (DeviceIoControl(deviceInfoSet,
					IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME,
					&NodeKeyName,
					sizeof(NodeKeyName),
					&NodeKeyName,
					sizeof(NodeKeyName),
					&nBytesReturned,
					0))
				{

					if(NodeKeyName.DriverKeyName == DriverKeyName)

					{
						ConnectionIndex = NodeKeyName.ConnectionIndex;
					}

				}
			}
		}

		CloseHandle(deviceInfoSet);
	}
	return ConnectionIndex;
}  

/*
This method is used on WinXP
*/
CString CUSBDeviceDetection::GetCMPropertyString(DEVINST ptrDevice, ULONG cmProperty)
{
	CString strCMProperty = _T("");
	ULONG RequiredSize = 0;
	TCHAR ptrBuf[BUFFER_SIZE];
	ULONG pulRegDataType;

	CM_Get_DevNode_Registry_PropertyW(ptrDevice, cmProperty, &pulRegDataType, ptrBuf, &RequiredSize, 0);
	if (CM_Get_DevNode_Registry_PropertyW(ptrDevice, cmProperty, &pulRegDataType, ptrBuf, &RequiredSize, 0) == CR_SUCCESS)
	{
#ifdef _UNICODE
		strCMProperty = ptrBuf;
#else
		TCHAR ptrBufA[BUFFER_SIZE];
		UnicodeToMultiByte((LPCWSTR)ptrBuf, (LPSTR)ptrBufA, BUFFER_SIZE);
		strCMProperty = ptrBufA;
#endif
	}
	return strCMProperty;
}

VOID CUSBDeviceDetection::GetDevicePropertyInfo(HDEVINFO DeviceInfoSet, SP_DEVINFO_DATA DeviceInfoData,  USB_PROPERTY_INFORMATION *DeviceProperty)
{
	HDEVINFO deviceInfoSet = DeviceInfoSet;
	SP_DEVINFO_DATA deviceInfoData = DeviceInfoData;
	TCHAR PropertyBuffer[BUFFER_SIZE] = {0};
	DWORD RequiredSize = 0;
	DWORD RegType = REG_SZ;
	CString KeyName = _T("");
	try
	{
		//Get Description
		if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_FRIENDLYNAME, &RegType, (BYTE *)PropertyBuffer, BUFFER_SIZE, &RequiredSize))
		{
			KeyName = PropertyBuffer;
		}
		else if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_DEVICEDESC, &RegType, (BYTE *)PropertyBuffer, BUFFER_SIZE, &RequiredSize))
		{
			KeyName = PropertyBuffer;
		}
		DeviceProperty->strDeviceDescription = KeyName;
		//Get Service
		if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_SERVICE, &RegType, (BYTE *)PropertyBuffer, BUFFER_SIZE, &RequiredSize))
		{
			KeyName = PropertyBuffer;
		}
		DeviceProperty->strService = KeyName;
		//Get Device Instance Path
		if (SetupDiGetDeviceInstanceId(deviceInfoSet, &deviceInfoData,(TCHAR *)PropertyBuffer, BUFFER_SIZE, &RequiredSize))
		{
			KeyName = PropertyBuffer;
		}
		DeviceProperty->strDeviceInstancePath = KeyName;
	}
	catch (CException *e)
	{
		//Fail
		TCHAR szError[1024] = {0};
		e->GetErrorMessage(szError, 1024);
		DeviceProperty->strDeviceDescription = _T("");
		DeviceProperty->strService = _T("");
	}
}


CString CUSBDeviceDetection::GetPhysicalAddress(HDEVINFO DeviceInfoSet, SP_DEVINFO_DATA DeviceInfoData)
{
	//Get windows version
	HDEVINFO deviceInfoSet = DeviceInfoSet;
	SP_DEVINFO_DATA deviceInfoData = DeviceInfoData;
	TCHAR PropertyBuffer[BUFFER_SIZE] = {0};
	DWORD RequiredSize = 0;
	DWORD RegType = REG_SZ;
	CString thisPhysicalAddress = _T("");
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(osvi);
	GetVersionEx(&osvi);
	double WinVer = osvi.dwMajorVersion + osvi.dwMinorVersion * 0.1;
/*
Windows 7                   6.1
Windows Server 2008 R2      6.1
Windows Server 2008         6.0
Windows Vista               6.0
Windows Server 2003 R2      5.2
Windows Server 2003         5.2
Windows XP 64-Bit Edition   5.2
Windows XP                  5.1
Windows 2000                5.0

Windows Version       WinUSB support
Windows 10 and later    Yes
Windows 7               Yes
Windows Server 2008     Yes
Windows Vista           Yes
Windows Server 2003     No
Windows XP				Yes since Windows XP SP2
Windows 2000            No

WinUSB support is native since Windows XP SP2

Windows XP SP3 is 5.1.2600
*/	
	if (WinVer >= 5.1)
	{
		//For Windows Server 2003 or later support location path
		//Get the location path
		if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_LOCATION_PATHS, &RegType, (BYTE *)PropertyBuffer, BUFFER_SIZE, &RequiredSize))
		{
			thisPhysicalAddress = (TCHAR *)PropertyBuffer;

			//Get the location information
			if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_LOCATION_INFORMATION, &RegType, (BYTE *)PropertyBuffer, BUFFER_SIZE, &RequiredSize))
			{
				CString strLocationInformation = (TCHAR *)PropertyBuffer;
				// The location information should be Port_#XXXX.Hub_#XXXX for a usb device and
				// xxxx.xxxx.xxxx.xxxx.xxxx for a interface, so we check the string format here to determine
				// if it's a interface of a device
				try
				{
					/*					
					Match match = Regex.Match(strLocationInformation, "Port_#[0-9|A-F]{4}.Hub_#[0-9|A-F]{4}");
					if (match.Success == false)
					{
						//It's a interface get the parent device' device location path
						thisPhysicalAddress = GetParentDevicePhysicalAddress(thisPhysicalAddress);
					}
					*/
				}
				catch(CException *e)
				{
					TCHAR szError[1024] = {0};
					e->GetErrorMessage(szError, 1024);
					thisPhysicalAddress = GetParentDevicePhysicalAddress(thisPhysicalAddress);
				}
			}                                      
		}
		else
		{
			//This part for win XP or earlier, these doesn't support location path information
			//Need to link with XP's cfgmgr32.lib and setupapi.lib from WDK, otherwise it will not work on XP
			if(CM_Get_Device_ID(deviceInfoData.DevInst, (TCHAR *)PropertyBuffer, BUFFER_SIZE, 0) == CR_SUCCESS)
			{
				CString DeviceID = (TCHAR *)PropertyBuffer;
				DEVINST ptrParent = (DEVINST)INVALID_HANDLE_VALUE;
				INT32 cr = CR_SUCCESS;
				DEVINST ptrDevice = (DEVINST)INVALID_HANDLE_VALUE;
				try
				{
					/*Match match = Regex.Match(DeviceID, "VID_[0-9|A-F]{4}&PID_[0-9|A-F]{4}&MI_[0-9|A-F]{2}");
					// If device contains MI_XX, it's a interface*/
					if ((DeviceID.Find(_T("VID_"))) > 0 &&
						(DeviceID.Find(_T("VID_")) < DeviceID.Find(_T("PID_"))) &&
						(DeviceID.Find(_T("PID_")) < DeviceID.Find(_T("MI_"))))
					{
						//It's a interface, get the parent device
						cr = CM_Get_Parent(&ptrDevice, deviceInfoData.DevInst, 0);
					}
					else
					{
						//Not a interface
						ptrDevice = deviceInfoData.DevInst;
					}
				}
				catch(CException *e)
				{
					TCHAR szError[1024] = {0};
					e->GetErrorMessage(szError, 1024);
					ptrDevice = (DEVINST)deviceInfoData.DevInst;
				}
				//Get parent device's device instance which is a hub port
				if (ptrDevice != (DEVINST)INVALID_HANDLE_VALUE &&
					CM_Get_Parent(&ptrParent, ptrDevice, 0) == CR_SUCCESS && 
					ptrParent != (DEVINST)INVALID_HANDLE_VALUE)
				{
					if (CM_Get_Device_ID(ptrParent, (TCHAR *)PropertyBuffer, BUFFER_SIZE, 0) == CR_SUCCESS)
					{

						CString parentPhysicalAddress = (TCHAR *)PropertyBuffer;
						//Get Hub Port Index
						CString guid = GUID_HUB_DRIVER; //HUB driver GUID
						CString ParentPNPDevicePath = _T("");
						parentPhysicalAddress.Replace('\\', '#');
						ParentPNPDevicePath.Format(_T("\\\\.\\%s#%s"), parentPhysicalAddress, guid);
						//Get the driver key of present device.
						CString DriverKeyName = GetCMPropertyString(ptrDevice, CM_DRP_DRIVER);
						if (DriverKeyName != _T(""))
						{
							//Go through the driver key of every hub port and get the connection index of our devices
							//The ConnectIndex is the USB Port number
							int ConnectIndex = GetConnectionIndex(ParentPNPDevicePath, DriverKeyName);
							if (ConnectIndex > 0)
							{
								thisPhysicalAddress.Format(_T("%s&%d"), parentPhysicalAddress, ConnectIndex);
							}
						}                       
					}
				}
			}
		}
	}
	else
	{
		CString strMsg;
		strMsg.Format(_T("Error: Get the WinVer as %lf, winUSB is not supported"),WinVer);
		AfxMessageBox(strMsg);
	}
	//ASSERT( thisPhysicalAddress.GetLength() != 0 );
	return thisPhysicalAddress;
}

CString CUSBDeviceDetection::GetDevicePositionInfo(HDEVINFO DeviceInfoSet, SP_DEVINFO_DATA DeviceInfoData)
{
	HDEVINFO deviceInfoSet = DeviceInfoSet;
	SP_DEVINFO_DATA deviceInfoData = DeviceInfoData;
	TCHAR PropertyBuffer[BUFFER_SIZE] = { 0 };
	DWORD RequiredSize = 0;
	DWORD RegType = REG_SZ;
	CString KeyName = _T("");
	try
	{
		if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_LOCATION_INFORMATION, &RegType, (BYTE *)PropertyBuffer, BUFFER_SIZE, &RequiredSize))
		{
			KeyName = PropertyBuffer;
		}
	}
	catch (CException *e)
	{
		//Fail
		TCHAR szError[1024] = { 0 };
		e->GetErrorMessage(szError, 1024);
		KeyName = _T("");
	}

	return KeyName;
}

VOID CUSBDeviceDetection::GetUsbPhysicalAddressList(CStringArray &strPhysicalAddress, CStringArray &strDeviceInstancePath, UINT &uDeviceCount)
{
	strPhysicalAddress.RemoveAll();
	strDeviceInstancePath.RemoveAll();
	CString DevEnum = REGSTR_KEY_USB;
	CString strServiceName = _T("WINUSB");
	CString strServiceName_Ext = _T("MUSBSer");
	UINT uCount = 0;
	CString thisPhysicalAddress, strService;
	// Use the "enumerator form" of the SetupDiGetClassDevs API
	// to generate a list of all USB devices
	HDEVINFO deviceInfoSet = SetupDiGetClassDevs(0, DevEnum, 0, (DIGCF_PRESENT | DIGCF_ALLCLASSES));
	if (deviceInfoSet != INVALID_HANDLE_VALUE)
	{
		BOOL Success = TRUE;
		UINT deviceIndex = 0;
		while (Success)
		{
			// create a Device Interface Data structure
			SP_DEVINFO_DATA deviceInfoData;
			deviceInfoData.cbSize = (UINT)sizeof(deviceInfoData);

			// start the enumeration
			Success = SetupDiEnumDeviceInfo(deviceInfoSet, deviceIndex, &deviceInfoData);
			if (Success)
			{
				USB_PROPERTY_INFORMATION DeviceProperty;
				GetDevicePropertyInfo(deviceInfoSet, deviceInfoData, &DeviceProperty);
				// is it a match?
				if (DeviceProperty.strService != _T(""))
				{
					strService = DeviceProperty.strService;
					strService.MakeUpper();

					CString strDeviceDes;
					strDeviceDes = DeviceProperty.strDeviceDescription;
					strDeviceDes.MakeUpper();

					if (( strService.Find(strServiceName.GetBuffer(0))>=0)
						&& ((0 == wcscmp(strDeviceDes,_T("QUECTEL USB DOWNLOAD PORT")))//获取下载端口的物理地址
						    ||  (NULL != wcsstr(strDeviceDes,_T("ADB")) ))//EC200H 华为4G平台 已经下载过usbloader.bin
							||  (NULL != wcsstr(strDeviceDes,_T("HUAWEI MOBILE CONNECT - DOWNLOAD PORT")) ))//EC200H 华为4G平台 短接或者裸片
					{
						thisPhysicalAddress = GetPhysicalAddress(deviceInfoSet, deviceInfoData);
						if (_T("") != thisPhysicalAddress)
						{
							if (0 == strPhysicalAddress.GetCount())
							{
								strPhysicalAddress.Add(thisPhysicalAddress);
								strDeviceInstancePath.Add(DeviceProperty.strDeviceInstancePath);
							}
							int i = 0;
							for (i = 0; i < strPhysicalAddress.GetCount(); i++)
							{
								if (thisPhysicalAddress == strPhysicalAddress.GetAt(i))
									break;
							}

							if (i == strPhysicalAddress.GetCount())
							{
								strPhysicalAddress.Add(thisPhysicalAddress);
								strDeviceInstancePath.Add(DeviceProperty.strDeviceInstancePath);
							}
						}

					
					}
					else
					{
						//20191107
						if ((DeviceProperty.strService.Find(strServiceName_Ext.GetBuffer(0))>=0)
							&& ((NULL != wcsstr(DeviceProperty.strDeviceDescription,_T("Modem Device")))//获取下载端口的物理地址
							|| (NULL != wcsstr(strDeviceDes,_T("DOWNLOAD DEVICE")))
							|| (NULL != wcsstr(strDeviceDes,_T("QUECTEL DOWNLOAD PORT")))
							)
							)//获取下载端口的物理地址
						{
							thisPhysicalAddress = GetDevicePositionInfo(deviceInfoSet, deviceInfoData);
							strPhysicalAddress.Add(thisPhysicalAddress);

							CString strComInfo;
							strComInfo = DeviceProperty.strDeviceDescription;
							int pos = strComInfo.Find(_T("("));
							strComInfo  = strComInfo.Mid(pos,strComInfo.GetLength());
							strComInfo.Replace(_T("("),_T(""));
							strComInfo.Replace(_T(")"),_T(""));
							strDeviceInstancePath.Add(strComInfo);
						}
						else
						{
							//...
						}

					}
				}
				
				else
				{
					//...
				}

			}
			deviceIndex++;
		}
		SetupDiDestroyDeviceInfoList(deviceInfoSet);
		uCount = deviceIndex;
	}
	uDeviceCount = uCount;
}

//获取端口 硬件ID中的VID参数
//eg: USB\VID_12D1&PID_1443
VOID CUSBDeviceDetection::GetUsbPhysicalVID(CString &strVID)
{
	CString DevEnum = REGSTR_KEY_USB;
	CString strServiceName = _T("WINUSB");
	CString strServiceName_Ext = _T("MUSBSer");
	UINT uCount = 0;
	CString thisPhysicalAddress, strService;
	// Use the "enumerator form" of the SetupDiGetClassDevs API
	// to generate a list of all USB devices
	HDEVINFO deviceInfoSet = SetupDiGetClassDevs(0, DevEnum, 0, (DIGCF_PRESENT | DIGCF_ALLCLASSES));
	if (deviceInfoSet != INVALID_HANDLE_VALUE)
	{
		BOOL Success = TRUE;
		UINT deviceIndex = 0;
		while (Success)
		{
			// create a Device Interface Data structure
			SP_DEVINFO_DATA deviceInfoData;
			deviceInfoData.cbSize = (UINT)sizeof(deviceInfoData);

			// start the enumeration
			Success = SetupDiEnumDeviceInfo(deviceInfoSet, deviceIndex, &deviceInfoData);
			if (Success)
			{
				USB_PROPERTY_INFORMATION DeviceProperty;
				GetDevicePropertyInfo(deviceInfoSet, deviceInfoData, &DeviceProperty);
				// is it a match?
				if (DeviceProperty.strService != _T(""))
				{
					strService = DeviceProperty.strService;
					strService.MakeUpper();

					CString strDeviceDes;
					strDeviceDes = DeviceProperty.strDeviceDescription;
					strDeviceDes.MakeUpper();

					if (( strService.Find(strServiceName.GetBuffer(0))>=0)
						&& ((0 == wcscmp(strDeviceDes,_T("QUECTEL USB DOWNLOAD PORT")))//获取下载端口的物理地址
						||  (NULL != wcsstr(strDeviceDes,_T("ADB")) ))//EC200H 华为4G平台 已经下载过usbloader.bin
						||  (NULL != wcsstr(strDeviceDes,_T("HUAWEI MOBILE CONNECT - DOWNLOAD PORT")) ))//EC200H 华为4G平台 短接或者裸片
					{

						CString strDevicePath;
						strDevicePath = DeviceProperty.strDeviceInstancePath;
						strDevicePath.MakeUpper();//USB\VID_12D1&PID_1443

						int iPos2 = -1, iPos1 = -1;
						iPos2 = strDevicePath.Find(_T("&"));
						iPos1 = strDevicePath.Find(_T("VID_"));

						if ((iPos2-4-iPos1>0)&&(iPos1>0))
						{
							strVID = strDevicePath.Mid(iPos1+4,iPos2-4-iPos1);
							return;
						}

						

					}
					else
					{
						//20191107
						if ((DeviceProperty.strService.Find(strServiceName_Ext.GetBuffer(0))>=0)
							&& ((NULL != wcsstr(DeviceProperty.strDeviceDescription,_T("Modem Device")))//获取下载端口的物理地址
							|| (NULL != wcsstr(strDeviceDes,_T("QUECTEL DOWNLOAD PORT")))))//获取下载端口的物理地址
						{
							
						}
						else
						{
							//...
						}

					}
				}

				else
				{
					//...
				}

			}
			deviceIndex++;
		}
		SetupDiDestroyDeviceInfoList(deviceInfoSet);
		uCount = deviceIndex;
	}

}

VOID CUSBDeviceDetection::GetUsbAT_PortList(CStringArray &strAT_PortList, UINT &uDeviceCount)
{
	strAT_PortList.RemoveAll();

	CString DevEnum = REGSTR_KEY_USB;
	UINT uCount = 0;
	// Use the "enumerator form" of the SetupDiGetClassDevs API
	// to generate a list of all USB devices
	HDEVINFO deviceInfoSet = SetupDiGetClassDevs(0, DevEnum, 0, (DIGCF_PRESENT | DIGCF_ALLCLASSES));
	if (deviceInfoSet != INVALID_HANDLE_VALUE)
	{
		BOOL Success = TRUE;
		UINT deviceIndex = 0;
		while (Success)
		{
			// create a Device Interface Data structure
			SP_DEVINFO_DATA deviceInfoData;
			deviceInfoData.cbSize = (UINT)sizeof(deviceInfoData);

			// start the enumeration
			Success = SetupDiEnumDeviceInfo(deviceInfoSet, deviceIndex, &deviceInfoData);
			if (Success)
			{
				USB_PROPERTY_INFORMATION DeviceProperty;
				GetDevicePropertyInfo(deviceInfoSet, deviceInfoData, &DeviceProperty);
				// is it a match?
				if (DeviceProperty.strService != _T(""))
				{
					CString strDeviceDes;
					strDeviceDes = DeviceProperty.strDeviceDescription;
					strDeviceDes.MakeUpper();

					if (NULL != wcsstr(strDeviceDes,_T("AT PORT")))//获取下载端口的物理地址
					{
						strAT_PortList.Add(strDeviceDes);
					}
					else
					{
					}
				}

				else
				{
					//...
				}

			}
			deviceIndex++;
		}
		SetupDiDestroyDeviceInfoList(deviceInfoSet);
		uCount = deviceIndex;
	}
	uDeviceCount = uCount;
}


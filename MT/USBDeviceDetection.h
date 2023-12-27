#include "StdAfx.h"
#include "cfgmgr32.h"
#include <initguid.h>
#include "Setupapi.h"
#include "usb100.h"

//Get USB physical address
const CString REGSTR_KEY_USB = _T("USB");
const int BUFFER_SIZE = 2048;

//const INT32 MAXIMUM_USB_STRING_LENGTH = 255;
const INT32 IOCTL_USB_GET_NODE_INFORMATION = 0x220408;  
const INT32 IOCTL_USB_GET_NODE_CONNECTION_INFORMATION_EX = 0x220448;
const INT32 IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME = 0x220420;

const CString GUID_HUB_DRIVER = _T("{f18a0e88-c30c-11d0-8815-00a0c906bed8}");

//added new functions for USB detection
enum USB_HUB_NODE  
{  
	UsbHub,  
	UsbMIParent  
};

//typedef struct _USB_HUB_DESCRIPTOR  
//{  
//	BYTE bDescriptorLength;  
//	BYTE bDescriptorType;
//	BYTE bNumberOfPorts;
//	INT16 wHubCharacteristics;
//	BYTE bPowerOnToPowerGood;
//	BYTE bHubControlCurrent;
//	BYTE bRemoveAndPowerMask[64];
//} USB_HUB_DESCRIPTOR;

typedef struct _USB_HUB_INFORMATION
{  
	USB_HUB_DESCRIPTOR HubDescriptor;  
	BYTE HubIsBusPowered;  
} USB_HUB_INFORMATION; 

typedef struct _USB_MI_PARENT_INFORMATION  
{  
	INT32 NumberOfInterfaces;  
} USB_MI_PARENT_INFORMATION;  

typedef struct _UsbNodeUnion  
{  
	USB_HUB_INFORMATION HubInformation; 
	USB_MI_PARENT_INFORMATION MiParentInformation;  
} UsbNodeUnion;

typedef struct _USB_NODE_INFORMATION  
{  
	USB_HUB_NODE NodeType;  
	UsbNodeUnion u;  
} USB_NODE_INFORMATION;

typedef struct _USB_NODE_CONNECTION_DRIVERKEY_NAME
{
	int ConnectionIndex;
	int ActualLength;
	WCHAR DriverKeyName[MAXIMUM_USB_STRING_LENGTH];
} USB_NODE_CONNECTION_DRIVERKEY_NAME;

typedef struct{
	CString strService;
	CString strDeviceDescription;
	CString strDeviceInstancePath;
}USB_PROPERTY_INFORMATION, *PUSB_PROPERTY_INFORMATION;

class CUSBDeviceDetection
{
public:
	CUSBDeviceDetection();           // protected constructor used by dynamic creation
	virtual ~CUSBDeviceDetection();

public:
	int GetConnectionIndex(CString DevicePath, CString DriverKeyName);
	CString GetParentDevicePhysicalAddress(CString devicePhysicalAddress);
	CString GetPhysicalAddress(HDEVINFO DeviceInfoSet, SP_DEVINFO_DATA DeviceInfoData);
	CString GetCMPropertyString(DEVINST ptrDevice, ULONG cmProperty);
	VOID GetDevicePropertyInfo(HDEVINFO DeviceInfoSet, SP_DEVINFO_DATA DeviceInfoData,  USB_PROPERTY_INFORMATION *DeviceProperty);
	BOOL  UnicodeToMultiByte(LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize);

public:
	VOID GetUsbPhysicalAddressList(CStringArray &strPhysicalAddress, CStringArray &strDeviceInstancePath, UINT &uDeviceCount);
	CString GetDevicePositionInfo(HDEVINFO DeviceInfoSet, SP_DEVINFO_DATA DeviceInfoData);
	//获取端口 硬件ID中的VID参数
	//eg: USB\VID_12D1&PID_1443
	VOID GetUsbPhysicalVID(CString &strVID);

	VOID GetUsbAT_PortList(CStringArray &strAT_PortList, UINT &uDeviceCount);
};
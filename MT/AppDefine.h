
#pragma once

#include "ListProgress.h"
#define LISTBKCOLOR             RGB(170,177,193)
#define TEXTCOLOR               RGB(0,0,0)
#define LISTTEXTCOLOR           RGB(179,179,255)
#define EREASEALLCOLOR			RGB(255,244,244)
#define UPDATECOLOR				RGB(245,254,243)
#define PROGRESSCOLOR           RGB(50,199,248)
#define PROGRESS_PASS_COLOR     RGB(0,206,0)
#define PROGRESS_FAILED_COLOR   RGB(255,0,0)

#define DEVICE_USB_PORT       _T("USB Port")
#define WM_CALLBACK	          WM_USER+1
#define WM_UPDATE_SUCCESS_RATE WM_USER+2
#define WM_SHOW_PLATFORM_INFO WM_USER+3
#define INI_FILE_NAME         _T("MPMDownloader.ini")
#define PACKAGE_SETTING       _T("Package_Setting")
#define PACKAGE_TYPE          _T("DownloadPackage_Type")
#define PACKAGE_FILE          _T("Package_File")
#define BOOTROM_USB_SETTING     _T("BootRom_USB_Settings")
#define BOOTLOADER_USB_SETTING  _T("BootLoader_USB_Settings")
#define TOTAL_USB_PORT			_T("Total_USB_Port_Num")
#define USB_PORT_ADDRESS		_T("USB_PortAddress")
#define IDS_DBG_LOG_MSG			_T("Target Debug Message : \\%s\\")
#define PERMISSON_SETTING		_T("User_Permission")
#define ADMIN_USER				_T("Admin_UserName")
#define ADMIN_PSWD				_T("Admin_PassWord")
#define USER_INFO_EN            _T("User_Info_Settings")
#define WORK_STATION_INFO_EN    _T("Current_WorkStation_Info")
#define PRODUCT_INFO_EN         _T("Product_Info")
#define TMP_DIR					_T("temp")
#define USERGUIDE_SETTING		_T("UserGuide_Setting")
#define USERGUIDE_DOC			_T("UserGuideDoc")
#define SWDMODE					_T("SWD_MODE")					

#define USER_INFO_CHN             _T("用户信息设置")
#define WORK_STATION_INFO_CHN     _T("当前工作站")
#define PRODUCT_INFO_CHN          _T("产品信息")

#define LIST_COLUMN_WITH_SETTING	_T("ColumnWith_Settings")
#define DEVICESTATE_COLUMN_WITH		_T("DeviceState_ColumnWith")
#define DEVICEINDEX_COLUMN_WITH		_T("DeviceIndex_ColumnWith")
#define DEVICEADDRESS_COLUMN_WITH	_T("DeviceAddress_ColumnWith")
#define PROGESSBAR_COLUMN_WITH		_T("DownloadProgress_ColumnWith")
#define ELAPSEDTIME_COLUMN_WITH		_T("ElapsedTime_ColumnWith")
#define STATUSMSG_COLUMN_WITH		_T("StatusMsg_ColumnWith")

#define EXCEL_FILE_NAME             _T("ResultStatistic.csv")

#define DEFAULT_PROCESS_TIMEOUT  500
#define MAX_ZIP_PROCESS_TIMEOUT  600000		// 10 minutes
#define IDS_EL_TIME            _T( "%H:%M:%S")


#define FBFFILE_NAME			_T("fbf.bin")
#define BLF_FILE_SUFFIX			_T(".blf")
#define PLATFORMINFO_FILENAME	_T("PlatformInfo.ini")
#define TEMP_PACKAGE			_T("TempPackage")
#define RELIABLEDATA_IMAGE_ID	_T("RBLI")

#define JSON_FILE_SUFFIX			_T(".json")
#define ASR3601_FILE			_T(".zip")


#define NUM_DEVICE_VIEW_COLUMNS		6
#define MAX_FILE_LENGTH				1024


static int		nColumnWidthFactors[NUM_DEVICE_VIEW_COLUMNS] = { 70, 70, 140 , 400, 80,300};
enum EColumnID
{
	kStateCtrl,
	kDeviceIndex,
	kDeviceAddress,
	kUSBProgess,
	kElapsedTime,
	kComment,
	kColumnNum,
};


enum EWorkInfoColumnID
{
	kItem,
	kItemInfo,
	kColCount,
};

enum EWorkInfoRowID
{
	kRowHeader,
	kCurrentUser,
	kCurrentStation,
	kProductInfo,
	kLoadTotalNum,
	kLoadSuccessNum,
	kLoadSeccessRate,
	kDefaultRowCount,
};

typedef enum _EDeviceType
{
	kNone			= 0,
	kBootRom		= 1,
	kBootLoader		= 2,
}EDeviceType;

typedef enum _ESWDMode
{
	kUpgrade,
	kEraseAll,
	kFormalRelease,
}ESWDMode;

typedef struct _USBDEVICE
{
	CListProgress* m_pListProgr;
	int            m_USBIndex;      


}USBDEVICE, *PUSBDEVICE;

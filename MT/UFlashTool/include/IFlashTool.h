#ifndef __IFLASHTOOL_H__
#define __IFLASHTOOL_H__

#ifdef FLASHTOOL_API_EXPORTS
#define FLASHTOOL_API extern "C" __declspec(dllexport)
#else
#define FLASHTOOL_API extern "C" __declspec(dllimport)
#endif

#pragma pack(push,1)

const COLORREF CLR_DOWNLOADING	= RGB(0, 0, 0);
const COLORREF CLR_DL_PASS		= RGB(0, 0, 255);
const COLORREF CLR_DL_FAIL		= RGB(255, 0, 0);
/////////////////////////////UIInterface BEGIN//////////////////////////////////
enum EUI_TEXT_ID
{
    eUI_TEXT_INDEX = 0,
    eUI_TEXT_COM,
    eUI_TEXT_IMEI,
    eUI_TEXT_PROGRESS,
    eUI_TEXT_INFOR,
	eUI_TEXT_ERROR,
    eUI_TEXT_LAST
};

enum EUI_DATA_ID
{
	eUI_DATA_PHASECHECK		= 0,
	eUI_DATA_DEVICE_IMEI1	= 1,
	eUI_DATA_DEVICE_IMEI2	= 2,
	eUI_DATA_DEVICE_IMEI3	= 3,
	eUI_DATA_DEVICE_IMEI4	= 4,
	eUI_DATA_CUST_PARTITION	= 5,
    eUI_DATA_DL_FILES_MAP   = 6,
    eUI_DATA_DL_STATES_MAP  = 7,
	
	eUI_DATA_LAST
};

typedef void (__stdcall *IUI_Update_Progress)(UINT nProgress, UINT nPosID, LPVOID pUserData);
typedef void (__stdcall *IUI_Output_Text)(LPCWSTR lpszStr, COLORREF clr, UINT nPosID, EUI_TEXT_ID eUITextID, LPVOID pUserData);
typedef BOOL (__stdcall *IUI_Output_Data)(LPBYTE lpData, UINT nLen, UINT nPosID, EUI_DATA_ID eUIDataID, LPVOID pUserData);

struct IUIInterface
{
    IUIInterface()
    {
		pIUI_Output_Data		= NULL;
        pIUI_Output_Text 		= NULL;
        pIUI_Update_Progress 	= NULL;
        pUserData 				= NULL;
    }
	IUI_Output_Data			pIUI_Output_Data;
    IUI_Output_Text			pIUI_Output_Text;
    IUI_Update_Progress	    pIUI_Update_Progress;
    LPVOID 					pUserData;
};

typedef CMap<CString, LPCTSTR, CString, LPCTSTR>   MAP_DL_FILE;
typedef CMap<CString, LPCTSTR, BOOL, BOOL>         MAP_DL_STATE;

typedef struct _UnpacProgress_Arg_T
{
	DWORD	dwRcvHandle;//GUI HWND or ThreadID
	BOOL	bThread;	//TRUE: receive by GUI, FALSE : receive by thread
	_UnpacProgress_Arg_T()
	{
		memset(this,0,sizeof(_UnpacProgress_Arg_T));
	}
}UnpacProgress_Arg;


#define MAX_PARTITION_ID_LEN          (36)  //wchar length, total 72 bytes
typedef struct _BLOCK_INFO_T_
{
	unsigned __int64 llBase;
	unsigned __int64 llSize;
	WCHAR szPartitionID[MAX_PARTITION_ID_LEN];

	_BLOCK_INFO_T_()
	{
		Clear();
	}
	void Clear()
	{
		memset(this,0,sizeof(_BLOCK_INFO_T_));
	}
}BLOCK_INFO_T, *PBLOCK_INFO_PTR;

#define MAX_ID_LENGTH 30
#define MAX_FILE_NUM  40
typedef struct _DL_FILE_INFO_T_
{
    WCHAR szFileID[MAX_ID_LENGTH];
    WCHAR szFilePath[MAX_PATH];
    int bSelected;

    _DL_FILE_INFO_T_()
    {
        Clear();
    }
    void Clear()
    {
        memset(this,0,sizeof(_DL_FILE_INFO_T_));
    }
}DL_FILE_INFO_T, *PDL_FILE_INFO_PTR;

typedef struct _DL_FILE_LIST_T_
{
    DL_FILE_INFO_T fileList[MAX_FILE_NUM];
    int fileNum;

    _DL_FILE_LIST_T_()
    {
        Clear();
    }
    void Clear()
    {
        memset(this,0,sizeof(_DL_FILE_LIST_T_));
    }
}DL_FILE_LIST_T, *PDL_FILE_LIST_PTR;

/////////////////////////////UIInterface END//////////////////////////////////

typedef enum
{
    ePOS_STATE_IDLE,         
    ePOS_STATE_DOWNLOADING,  
    ePOS_STATE_DLD_SUCCESS,  
    ePOS_STATE_DLD_FAILED 
} EPOS_STATE;

typedef enum
{
	E_DOWNLOAD_MODE,         
	E_UPGRADE_MODE,
	E_READBACK_DEVICE_INFO,
	E_DL_MODE_UNKNOW
} DOWNLOAD_MODE;

typedef enum 
{
    E_PROR_DEBUG_MODE		= 0,
    E_PROR_LOG_PATH			= 1,
    E_PROR_LOG_LEVEL		= 2,
    E_PROR_UNPAC_PATH		= 3,
    E_PROR_RESET_DEV		= 4,
	E_PROR_DOWNLOAD_MODE	= 5,
	E_PROR_POWEROFF_DEV		= 6,
    E_PROR_ERASE_ALL		= 7,
    E_PROP_CRC_CFG_FILE		= 8,
    E_PROP_NAND_BLOCK_INFO	= 9,
	E_PROR_PARTIAL_UPGRADE 	= 10,	
	E_PROR_END_OF_DOWNLOAD 	= 11,	
	E_PROR_BAUDRATE			= 12,
	E_PROR_UNPAC_PROGRESS_RECEIVER	= 13,
	E_PROR_SUPPORT_CUSTOMIZED		= 14,
	E_PROP_READBACK_CUST_PARTITION	= 15,
    E_PROP_ERASE_CUST_PARTITION	    = 16,
    E_PROP_DL_FILE_LIST     = 17,
    E_PROR_UNKNOW
}FT_PROPERTY_ID_E;

typedef enum 
{
	E_VALUE_PHASECHECK		= 0,
	E_VALUE_UNKNOW
}FT_VALUE_TYPE_ID_E;

typedef enum 
{
	E_NONE_ACTION			= 0,
	E_POWER_OFF				= 1,
	E_RESET_TO_NORMAL		= 2,
    E_RESET_TO_GSM_CALI     = 3,
    E_RESET_TO_WCDMA_CALI   = 4,
    E_RESET_TO_LTE_CALI     = 5,
    E_RESET_TO_BBAT         = 6
}FT_END_OF_DOWNLOAD_TYPE_E;

#pragma pack(pop)

class IFlashCtrlHandler
{
public:
    virtual ~IFlashCtrlHandler(){;}
	
	/** SetProperty/GetProperty
	*
	* @param dwPropertyID	: FT_PROPERTY_ID_E type
	* @param lpValue		: point to buffer
	* @param u32Bytes		: the size of buffer
	* @return: true,if set successfully;false,otherwise
	*/
    virtual  BOOL       SetProperty(DWORD dwPropertyID, LPCVOID pPropertyValue) = 0;/* dwPropertyID*/
    virtual  BOOL       GetProperty(DWORD dwPropertyID, LPVOID*  lpPropertyValue) = 0;


	/** InitDownload
	*
	* @param lpszPacketName	: pac file
	* @return: 0,if init successfully; otherwise,init fail
	*/
    virtual  DWORD      InitDownload(LPCWSTR lpszPacketName) = 0;

	/** SetVelue/GetValue
	*
	* @param dwPort		: port number
	* @param dwValueID	: FT_VALUE_TYPE_ID_E type
	* @param lpValue	: point to buffer
	* @param u32Bytes	: the size of buffer
	* @return: true,if set successfully;false,otherwise
	*/
	virtual  BOOL		SetValue(DWORD dwPort, DWORD dwValueID, const void* lpValue, UINT32 u32Bytes) = 0;
	virtual  BOOL		GetValue(DWORD dwPort, DWORD dwValueID, void* lpValue, UINT32 u32Bytes) = 0;

    virtual  DWORD	    StartOnePortWork(DWORD dwPort) = 0;
    virtual  void       StopOnePortWork(DWORD dwPort) = 0;
    virtual  HANDLE     GetDownloadThreadHandle(DWORD dwPort) = 0;
    virtual  DWORD      GetDownloadState(DWORD dwPort) = 0;

    virtual  void       GetOprErrorCodeDescription( DWORD dwErrorCode,LPSTR lpszErrorDescription, int nSize,DWORD dwPort=0 ) = 0;
    virtual  void       RegisterUIInterface(IUIInterface *pUIInterface) = 0; 
};

FLASHTOOL_API BOOL CreateFlashToolObject( IFlashCtrlHandler ** pFlashCtrlObj);
FLASHTOOL_API void ReleaseFlashToolObject( IFlashCtrlHandler * pFlashCtrlObj);

#endif  //__IFLASHTOOL_H__
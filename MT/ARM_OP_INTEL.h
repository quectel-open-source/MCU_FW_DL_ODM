#pragma once
#include "gfc.h"
#include "Com_Port.h"

//.fls文件的index
typedef enum tagIDX_FLS_FILE_E
{
	PSI_FLS_FILE_I	= 0,
	SLB_FLS_FILE_I	= 1,
	FW_FLS_FILE_I	= 2,
	MOD_FLS_FILE_I	= 3,
	MSY_FLS_FILE_I	= 4,
	MAX_FLS_FILE	//最多下载的普通.fls文件
}IDX_FLS_FILE_E;

typedef struct tagFLS_FILE_S
{
	FILE_BASE_S str_File_Base;
	bool b_Enable_DL;
}FLS_FILE_S;

#define  INTEL_SCAN_USB_DEV_FAIL    0x03001001
#define  INTEL_CREATE_PROCESS_FAIL 0x03001002
#define  INTEL_DOWNLOAD_ERROR_FAIL		0x03001003
#define  INTEL_DOWNLOAD_TIMEOUT_FAIL		0x03001004
#define  INTEL_DOWNLOAD_RESET_USB_DEV_FAIL		0x03001005

#define  ERR_Detect_AT_PORT_Error	0x03001006
#define  ERR_OPEN_AT_PORT_Error		0x03001007
#define  ERR_Write_IMEI_Error		0x03001008
#define  ERR_Write_SN_Error			0x03001009
#define  ERR_Read_IMEI_Error		0x0300100A
#define  ERR_Confirm_IMEI_Error		0x0300100B
#define  ERR_Read_SN_Error			0x0300100C
#define  ERR_Confirm_SN_Error		0x0300100D
#define  ERR_Write_SN_LEN_Error		0x0300100E
#define  ERR_Write_IMEI_LEN_Error   0x0300100F


class CARM_OP_INTEL
{
public:
	CARM_OP_INTEL(void);
	~CARM_OP_INTEL(void);
	HWND m_hWnd_View;
	bool m_bRuning;
	int m_iStop_Flag;
	CString m_csError_Code;
	int m_iError_Code;
	int m_iClass_Idx;
	CString m_csUSB_Detect_PID_VID;
	CGFC m_cla_GFC;
	FLS_FILE_S m_strr_FLS_File_List[MAX_FLS_FILE];
	bool Scan_Active_Usb_Ch(int * ip_Act_USB_CH, CString * csp_Act_USB_DEV, int i_TimeOut);
	void Show_Status(const wchar_t * wcp_Status);
	int Do_Execute(void);
	bool Get_FLS_File_Index(CString cs_FLS_Name, int * ip_Index);
	bool LOAD_FW_FILE(const wchar_t * wcp_FW_File_Path);
	int m_iOP_TYPE;
	bool DL_FLS_FILE_IN_EXE();
	void Save_Trace_Log(const wchar_t * wcp_Trace_Log);
	void Show_Progress(double d_Pgr_Percent);
	bool m_bGet_Debug_Log;
	bool Scan_Usb_Dev(void);
	CString m_csrUsb_Dev_List[64];
	int m_iUsb_Dev_Quan;
	bool Read_Reg_Usb_Dev_Ch(const wchar_t * wcp_USB_PID_VID, CString * csp_USB_Dev_List, int i_USB_Dev_Quan, int * ip_USB_Ch_List);
	bool Set_Reg_Usb_Dev_Ch_Value(const wchar_t * wcp_USB_PID_VID, const wchar_t * wcp_USB_Dev, const wchar_t * wcp_USB_Ch);
	int m_iCTRL_DL_TYPE;
	CString m_csCurr_Exe_Path;


	bool m_bPLC_Flag;//带PLC控制的夹具
	int m_iPLC_Port;
	void Show_Out(const wchar_t * wcp_Status);

	//读写IMEI、SN
	CCom_Port m_cla_Com_Port;
	int m_iAt_Port;

	bool _W_Write_Pri_IMEI_Intel( const wchar_t * wcp_IMEI );
	bool _W_Write_SN_Intel( const wchar_t * wcp_SN );

	bool _Read_Pri_IMEI_Intel( wchar_t * wcp_IMEI );
	bool _Read_SN_Intel( wchar_t * wcp_SN );

	bool _W_C_IMEI_SN_Intel( const wchar_t * wcp_IMEI, const wchar_t * wcp_SN);

	bool Detect_Com_Port(int i_Tgt_Com, int i_TimeOut);

};

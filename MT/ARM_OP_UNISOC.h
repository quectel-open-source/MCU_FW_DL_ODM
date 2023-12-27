#pragma once
#include "gfc.h"
#include "Com_Port.h"
#include "Gen_Header.h"

#define WM_UNISOC_STATUS_MSG                    WM_USER + 5001

struct PORT_DATA
{
	int  iIndex;
	bool bRuning;
	PORT_DATA()
	{
		iIndex = 0;
		bRuning = false;
	}
};

#define DL_FW_MCU_FAIL1  1096
#define DL_FW_MCU_FAIL2  1097
#define DL_FW_MCU_FAIL3  1098
#define DL_FW_MCU_FAIL4  1099
#define DL_FW_MCU_FAIL5  1100

typedef struct tagChannel_Port_Info
{
	int iAT_Port;        // AT口
	int iQDLoader_Port;  // 紧急下载口
	BOOL bIsRunning;     // 是否正在下载

	~tagChannel_Port_Info()
	{
		iAT_Port = 0;
		iQDLoader_Port = 0;
		bIsRunning = FALSE;
	}
}Channel_Port_Info;

typedef enum tagBB_CHIP_UNISOC_TYPE
{
	SL8541 = 0,   // SC200L\SC100L
	UIS8581 = 1,  // SC200K uis8581e2h10_XXXXXXX.pac
	UDX710  = 2,  // RG500U udx710_XXXXXXXXXXXXX.pac
	UIS8910 = 3,   // EC200U 8915DM_XXXXXXXXXXXXX.pac
	UIS8811 = 4    //BC95GU  BC95GUSAR01A01.pac
}BB_CHIP_UNISOC_TYPE;
class CARM_OP_UNISOC
{
public:
	CARM_OP_UNISOC(void);
	~CARM_OP_UNISOC(void);

	HWND m_hWnd_View;
	bool m_bRuning;
	CCom_Port m_cla_Com_Port;
	CGFC m_cla_GFC;
	int m_iStop_Flag;
	CString m_csError_Code;
	int m_iError_Code;
	int m_iClass_Idx;
	int m_iCom_Port;
	int m_iCom_Port_Auto_Sec;
	CString m_csCurr_Exe_Path;
	int m_iBaud_Rate;
    bool m_bUnisoc_Reset_after_DL;
	static Channel_Port_Info m_arrCh_Port_Info[DL_CHANNEL_NUM];
	BB_CHIP_UNISOC_TYPE m_eu_BB_Chip;

	void Show_Bar_IMEI(const wchar_t * wcp_Status);
	void Show_Progress(double d_Pgr_Percent);
	void Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN);
	void Show_DL_Port(const int wip_port);
	void Show_Status(const wchar_t * wcp_Status);
	void Show_Out(const wchar_t * wcp_Status);

	void Save_Trace_Log(const wchar_t * wcp_Trace_Log);
	void Show_COMPORT(const wchar_t * wcp_Status);
	bool LOAD_BB_INFO(const wchar_t * wcp_FW_File_Path);  // 得到芯片的类型
	bool GetDownloadPort(int &iDLPort);
	bool Confirm_Dynamic_Com_Port(const int i_Tgt_Com_Port, int i_TimeOut);
	bool Read_Reg_USB_VID_PID_QDLoader_Port_Of_QUD(int * ip_QDLoader_Port_List, int * ip_Port_Quan);
	bool Switch_EDL(int * ip_New_QDLoader_Port);
	BOOL Switch_EDL_Sub(CArray<int, int&> &arrOldQDLoadPort);
	bool Confirm_Dynamic_Com_Port_N(const int i_Tgt_Com_Port, int i_TimeOut);
	int Do_Execute_MCU_JLink(CString str_FolderName);
	bool CreatQDM536_Jlink_CMD();
	bool Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size);
	bool Creat_Jlink_CMD(CString str_EraseAndFlash_File, CString str_DeviceName, CString str_FWAllPath);
	CString GetExeFilePath();
	CString m_csOC;
};

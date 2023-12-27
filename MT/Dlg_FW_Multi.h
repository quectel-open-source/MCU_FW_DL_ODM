#pragma once
#include "Gen_Header.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "inifile.h"
#include "flash_op.h"
#include "arm_op_qcm.h"
#include "arm_op_rda.h"
#include "arm_op_intel.h"
#include "ARM_OP_SILABS.h"

#include "dlg_config_multi.h"
#include "com_thr_port.h"
#include "gfc.h"
#include "Dlg_Password.h"
#include "sql_db.h"
#include <afxinet.h> 
#include "ts_lan.h"
#include "flash_test.h"
#include "write_imei.h"
#include "write_imei.h"
#include "ocp.h"
#include "ARM_OP_HUAWEI.h"
#include "ARM_OP_ASR.h"
#include "ARM_OP_EIGEN.h"
#include "WriteFlashId.h"
#include "WorkNum.h"
#include "ARM_OP_BEKEN.h"

#include "WinSocket.h"

#ifdef FW_MULTI
#ifndef FW_UPGRADE
#include "PipeCommObj00.h"
#endif
#endif

#include <vector>



#include "Flash_OP_NB.h"
#include "Flash_OP_GK.h"
#include "Flash_OP_5G.h"
#include "Flash_OP_AG.h"
#include "ARM_OP_UNISOC.h"
#include "ARM_OP_XINYI.h"
#include "ARM_OP_ALTAIR.h"
#include "UFlashTool\include\IFlashTool.h"
#ifdef FW_MULTI
#ifndef FW_UPGRADE
#include "Power_Supply.h"
#endif
#endif

#include "IFL_SWD\IFL_SWDAPI.h"
#include "WtptpDownLoad\ParaDefine.h"
#include "AppDefine.h"
#include "WtptpDownLoad\WtptpDownLoad.h"

#include "json.h"


#define IFLSWDDLL_NAME            _T("IFL_SWD.dll")
#define WTPTPDOWNLOADDLL_NAME     _T("WtptpDownLoad.dll")

//机型
//BC25-PA;BC25B8-PA;BC25B5-PA;BC32-RA;BC32B8-RA;BC25-PB;BC25B8-PB;BC25B5-PB;BC32-RB;BC32B8-RB;
#define PRO_BC25_NO_USE			0
#define PRO_BC25_PA				1
#define PRO_BC25B8_PA			2
#define PRO_BC25B5_PA			3
#define PRO_BC32_RA				4
#define PRO_BC32B8_RA			5
#define PRO_BC25_PB				6
#define PRO_BC25B8_PB			7
#define PRO_BC25B5_PB			8
#define PRO_BC32_RB				9
#define PRO_BC32B8_RB			10
#define PRO_NUM 11

#define FAIL_MO_NOT_MATCH_DB			0x03002001
#define FAIL_IMEI_STATUS_FAIL			0x03002002
#define FAIL_TOBETEST					0x03002003
#define FAIL_BLUETOOTH_ADD_ERR			0x03002004

#define FAIL_IMEI2_IN_DB_ERR			0x03002005
#define FAIL_IMEI_NOT_IN_DB_ERR			0x03002006

#define FAIL_BLUETOOTH_ADD_NOT_BAR		0x03002008
#define ERROR_Flash_Test				0x03002009

#define FAIL_Upload_DL_Status_Link_DB_Fail				0x0300200B
#define FAIL_Upload_DL_Status_Update_SQL_Net_Tag_Fail	0x0300200c
#define FAIL_Upload_DL_Status_To_SQL_DB					0x0300200d
#define FAIL_CHECK_SQL_IMEI_INFO_V1						0x0300200e
#define FAIL_DO_WRITE_IMEI								0x0300200f
#define Reload_Fixture_Fail								0x03002018

#define FAIL_Upload_QDE007_PCBA_DATA_Link_DB_Fail				0x03003000



#define	FAIL_QISDA_SMT_NG                     0x03002010
#define	FAIL_QISDA_FENBAN_RES_NG			  0x03002011

#define	FAIL_QISDA_SMT_LINK_DB                     0x03002012

#define FAIL_QISDA_SMT_NG_NOT_FIND     0x03002013
#define FAIL_QISDA_FENBAN_RES_NG_NOT_FIND   0x03002014
#define  FAIL_SMT_RES  0x03002015
#define  FAIL_FENBAN_RES  0x03002016

#define  Update_MOB_SN_Status_Fail  0x03002017

#define  FAIL_QISDA_FENBAN_Data_Not_Find  0x03002019

#define  FAIL_SAVE_FTP_TEST_LOG    0x0300201A

#define  TestFlow_Update_MOB_SN_Status_Fail  0x0300201B

#define  FAIL_NO_TEST  0x03FFFFFFF

#define  FT_FAIL_DETECT_PORT					0x03002109
#define  FT_FAIL_OPEN_PORT						0x03002209
#define  FT_FAIL_READ_MODULE_ID					0x03002309
#define  FT_FAIL_MODULE_ID_EXCEPTION			0x03002409
#define  FT_FAIL_GET_KEY_TYPE					0x03002509
#define  FT_FAIL_KEY_TYPE_INIT					0x03002609
#define  FT_FAIL_CREATE_TXT_FILE_LICENSE		0x03002709
#define  FT_FAIL_WRITE_MODULE_LICENSE			0x03002809
#define  FT_FAIL_CHECK_FLASH_DATA				0x03002909
#define  FT_FAIL_READ_MODULE_LICENSE			0x03002A09

#define  WI_FAIL_DETECT_PORT	    0x0300210f
#define  WI_FAIL_OPEN_PORT			0x0300220f
#define  WI_FAIL_CHECK_FW_VER		0x0300230f
#define  WI_FAIL_PRI_IMEI_LEN		0x0300240f
#define  WI_FAIL_PRI_IMEI_WRITE		0x0300250f
#define  WI_FAIL_SEC_IMEI_LEN		0x0300260f
#define  WI_FAIL_SEC_IMEI_WRITE		0x0300270f
#define  WI_FAIL_SN_LEN				0x0300280f
#define  WI_FAIL_SN_WRITE			0x0300290f
#define  WI_FAIL_BT_ADDR_LEN		0x03002a0f
#define  WI_FAIL_BT_ADDR_WRITE		0x03002b0f
#define  WI_FAIL_BT_ADDR_READ       0x03002c0f
#define  WI_FAIL_BT_ADDR_CONFIRM    0x03002d0f
#define  WI_FAIL_MAC_ADDR_LEN       0x03002e0f
#define  WI_FAIL_MAC_ADDR_WRITE     0x03002f0f
#define  WI_FAIL_MEID_LEN           0x03002f1f
#define  WI_FAIL_MEID_WRITE         0x03002f2f
  




//主窗口显示标题名
extern CString g_csMain_Title_Name;

typedef struct tagABSTRACT_RECORD_S
{
	int iID;	//int	Unchecked	自增ID，无需关注
	CString csIMEI;	//nvarchar(50)	Unchecked	IMEI
	CString csSN;	//nvarchar(50)	Unchecked	SN
	CString csStationName;	//nvarchar(50)	Unchecked	测试工位名
	CString csTSNum;	//nvarchar(50)	Unchecked	测试台号
	int iTestResult;	//int	Unchecked	"0 -- FAIL 1 -- PASS"
	CString csTestTool;	//nvarchar(50)	Unchecked	工具软件名（含版本）
	int iErrCode;	//int	Unchecked	结果返回码
	CString csErrInfo;	//nvarchar(50)	Unchecked	PASS/FAIL的具体信息
	CString csMO;	//nvarchar(50)	Unchecked	工单号
	CString csTestPC;	//nvarchar(50)	Unchecked	测试电脑名称
	CString csTestPCIp;	//nvarchar(50)	Unchecked	测试电脑IP
	CString csTestDate;	//datetime	Unchecked	测试日期时间
	CString csLine;	//nvarchar(50)	Checked	线体名称
	CString csOperator;	//nvarchar(50)	Unchecked	操作人员
	int    iIsSync;//	是否被同步，用于同步工厂接口，0-未同步 1-已同步
}ABSTRACT_RECORD;


//单条流程记录结构体
typedef struct tagCUSTOM_TEST_FLOW_S
{
	int i_Id;//从“1”开始
	CString cs_Station;
	CString cs_Status;
	CString cs_SNFlag;
	CString cs_TestTool;

}CUSTOM_TEST_FLOW_S;



//自定义流程标志位体系
typedef struct tagCUSTOM_TEST_FLOW_FLAG_S
{
	CStringArray csaAllow_Status;//允许的网络标志位
	CStringArray csaAllow_SNFlag;//允许的SN标志位
	CString cs_Write_Status;//需要写入的网络标志位
	CString cs_Write_SNFlag;//需要写入的本地SN标志位
}CUSTOM_TEST_FLOW_FLAG_S;
// CDlg_FW_Multi dialog

class CDlg_FW_Multi : public CDialog
{
	DECLARE_DYNAMIC(CDlg_FW_Multi)

public:
	CDlg_FW_Multi(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_FW_Multi();

// Dialog Data
	enum { IDD = IDD_DIALOG_FW_MULTI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	void INIT(void);
	void DO_DL_OP(int i_TSCH_Numb);//下载入口

	CCom_Port m_Pow_Com_Port;
	CCom_Port m_Pow_Com_Port_S[11];
	CCom_Thr_Port m_cla_Robot_Port;//三轴机器人的控制COM口
	CCom_Thr_Port m_cla_PLC_OL_Port;//半自动夹具的PLC的控制COM口
	
	BOOL CheckPower(void);
	BOOL PowOpen(void);
	BOOL PowClose(void);

	CString m_csCurr_Exe_Path;
	CString m_csDebug_Log_File_Path_1;
	CString m_csDebug_Log_File_Path_2;
	CString m_csDebug_Log_File_Path_3;
	CString m_csDebug_Log_File_Path_4;
	CString m_csDebug_Log_File_Path_5;
	CString m_csDebug_Log_File_Path_6;
	CString m_csDebug_Log_File_Path_7;
	CString m_csDebug_Log_File_Path_8;
	CString m_csDebug_Log_File_Path_9;
	CString m_csDebug_Log_File_Path_10;


	CString  m_csRunning_Log_File_Path_1;
	CString  m_csRunning_Log_File_Path_2;
	CString  m_csRunning_Log_File_Path_3;
	CString  m_csRunning_Log_File_Path_4;
	CString  m_csRunning_Log_File_Path_5;
	CString  m_csRunning_Log_File_Path_6;
	CString  m_csRunning_Log_File_Path_7;
	CString  m_csRunning_Log_File_Path_8;
	CString  m_csOrg_Log_File_Path;
	CString	 m_csScat_Cfg_File_Path;
	CString  m_csDA_File_Path;
	CString m_csLocal_PC_Name;
	CString m_strStatusTest[10];
	CString m_strFW_SW_Version_DB;
	CString m_csFwMO_Name;
	CString m_csFwFac_Name;
	CString m_csServer;
	CString m_csOc_Name;
	CString m_csDlFileInfo;
	CString m_csrBarcode_In_Code[11];	//完整的二维码数据
	CString m_csrIMEI_In_Code[11];		//二维码中提取到的“IMEI”数据
	CString m_csrSN_In_Code[11];		//二维码中提取到的“SN”数据
	CString m_strIMEI_Current[11];//当前对象所使用的IMEI
	CString m_csrBT_In_Code[11];//二维码中的蓝牙地址
	CString m_strMCU_Ver;
	CString  m_strLog_Level;
	CString m_csSW_DL;
	CString m_csSW_Label;
	CString m_csrError_Info[11];//错误信息
	CString m_strB1;
	CString m_strB2;
	CString m_strB3;
	CString m_strB4;
	CString m_strB5;
	CString m_strB6;
	CString m_strB7;
	CString m_strB8;
	CString m_strB9;
	CString m_strB10;
	CString m_csrBTAddr_In_SQL[11];
	CString m_csrIMEI_2_In_SQL[11];
	CString m_csLastMO_Name;
	CString m_strAutoFN_Name1;
	CString m_strAutoFN_Name2;
	CString m_strAutoFN_Name3;
	CString m_strAutoFN_Name4;
	CString m_strAutoFN_Name5;
	CString m_strAutoFN_Name6;
	CString m_strAutoFN_Name7;
	CString m_strAutoFN_Name8;
	CString m_strAutoFN_Name9;
	CString m_strAutoFN_Name10;
	CString  m_strTest_Firmware_Name; //该字段主要用可生产性测试的FIRMWARE（可通过配置文件去配置）

	//20210705
	CString m_str_2_Test_Fixture_ID;
	CString m_str_2_Test_Fixture_ID_TIME;
	BOOL    m_b_2_JTAG_DL;
	CString	m_str_2_JTAG_DL_TIME;


	int m_iCom_Power_Idx;
	int m_iCom_1_Idx;
	int m_iCom_2_Idx;
	int m_iCom_3_Idx;
	int m_iCom_4_Idx;
	int m_iCom_5_Idx;
	int m_iCom_6_Idx;
	int m_iCom_7_Idx;
	int m_iCom_8_Idx;
	int m_iCom_9_Idx;
	int m_iCom_10_Idx;
	int m_iRunning_Log_Handle_1;
	int m_iRunning_Log_Handle_2;
	int m_iRunning_Log_Handle_3;
	int m_iRunning_Log_Handle_4;
	int m_iRunning_Log_Handle_5;
	int m_iRunning_Log_Handle_6;
	int m_iRunning_Log_Handle_7;
	int m_iRunning_Log_Handle_8;


	int m_iDebug_Log_Handle_1;
	int m_iDebug_Log_Handle_2;
	int m_iDebug_Log_Handle_3;
	int m_iDebug_Log_Handle_4;
	int m_iDebug_Log_Handle_5;
	int m_iDebug_Log_Handle_6;
	int m_iDebug_Log_Handle_7;
	int m_iDebug_Log_Handle_8;
	int m_iDebug_Log_Handle_9;
	int m_iDebug_Log_Handle_10;

	int m_iPlatform_Curr;
	int m_iBBchip_Curr;//增加芯片类型参数
	int m_iBBchip_Curr_OCP;//增加芯片类型参数
	
	int m_iOrg_Log_Handle;
	int m_iDownHandle[10];//下载句柄，芯翼平台
	int m_iXYDLFileNum;
	int m_irPgr_Last[16];	
	int m_iNv_Type;
	int m_iCtrl_Dl_Type;
	int m_iDelay_Time;//找口延时
	int m_iOCPU_Name_M;

	//统计PASS，FAIL,FPY次数
	int iPASS[10];
	int iFAIL[10];
	int iFPY[10];
	int m_iAutoTest;
	int m_AUTO_TEST_TIME;
	int m_bFtpFwCompFlag; //0:初始化值 1：成功  2：失败
	int m_iDlFileProgress;
	int m_irDL_OBJ_State[11];//0=未激活-未选中-不使用的端口；1=被选中-未开始-无模块；2=下载中-运行中；3=下载pass；4=下载fail；
	int m_iGetInfoStatus[4]; //获取到4个端口的是否获取到二维码信息 1:获取到 0:未获取
	// 	int m_iPlatform_Solution;
	// 	int m_iBB_Chip;
	int m_iCustomize_OP;//6310标签后打印标签
	int m_irTest_Time[11];//测试时间
	int m_iDEBUG_LOG;
	int m_irTS_Test_Pass_Quan[11];
	int m_irTS_Test_Fail_Quan[11];
	int m_iAutoDL_Flag;//Multi_Config.ini  Auto_DL_Flag=0
	int m_iAutoSaveLog;//Multi_Config.ini  Auto_SAVE_FLAG=0
	int m_irFX_Status[8];
	


	bool m_bProdu_Landi;
	bool m_bProdu_WiFi;
	bool m_bWrite_PRI_IMEI;
	bool m_bWrite_SEC_IMEI;
	bool m_bWrite_BT_Addr;
	bool m_bWrite_MAC_Addr;
	bool m_bWrite_MEID;
	bool m_bLaserMark;
	bool m_bBurnFlag;//是否需要Burn In
	bool m_bMCU_VerFlag;//是否需要下载MCU的版本
	bool m_bLog_LevelFlag;
	bool m_bLogDB_Flag;
	bool m_bPortDL1;
	bool m_bPortDL2;
	bool m_bPortDL3;
	bool m_bPortDL4;
	bool m_bPortDL5;
	bool m_bPortDL6;
	bool m_bPortDL7;
	bool m_bPortDL8;
	bool m_bPortDL9;
	bool m_bPortDL10;
	bool m_bSaveLogFile;
	bool m_brTF_Closed_Work[11];

	bool m_bLoad_Bin_List;
	bool m_bInside_Flash;
	bool m_bINIT_FW;
	bool m_bStop_Flag;
	bool m_bRobot_Running;

	COLORREF m_colorr_Edit_Text[16];
	COLORREF m_colorr_Edit_Text_L[16];
	COLORREF m_color_ZIP_INF;

	ABSTRACT_RECORD Abstract_Log[10];

	CStdioFile m_cla_Org_Log_File;

	CButton m_ctr_btnSet_Scat_Cfg_File;
	CButton m_ctr_btnStart_All;
	CButton m_ctr_btnStop_All;

	CComboBox m_ctr_cbxCom_1;
	CComboBox m_ctr_cbxCom_2;
	CComboBox m_ctr_cbxCom_3;
	CComboBox m_ctr_cbxCom_4;
	CComboBox m_ctr_cbxCom_5;
	CComboBox m_ctr_cbxCom_6;
	CComboBox m_ctr_cbxCom_7;
	CComboBox m_ctr_cbxCom_8;
	CComboBox m_ctr_cbxCom_9;
	CComboBox m_ctr_cbxCom_10;
	CComboBox m_iCom_Power_Ctrl;

	CButton m_ctr_btnStart_1;
	CButton m_ctr_btnStart_2;
	CButton m_ctr_btnStart_3;
	CButton m_ctr_btnStart_4;
	CButton m_ctr_btnStart_5;
	CButton m_ctr_btnStart_6;
	CButton m_ctr_btnStart_7;
	CButton m_ctr_btnStart_8;
	CButton m_ctr_btnStart_9;
	CButton m_ctr_btnStart_10;

	CProgressCtrl m_ctr_pgrProgress_Port_1;
	CProgressCtrl m_ctr_pgrProgress_Port_2;
	CProgressCtrl m_ctr_pgrProgress_Port_3;
	CProgressCtrl m_ctr_pgrProgress_Port_4;
	CProgressCtrl m_ctr_pgrProgress_Port_5;
	CProgressCtrl m_ctr_pgrProgress_Port_6;
	CProgressCtrl m_ctr_pgrProgress_Port_7;
	CProgressCtrl m_ctr_pgrProgress_Port_8;
	CProgressCtrl m_ctr_pgrProgress_Port_9;
	CProgressCtrl m_ctr_pgrProgress_Port_10;
	CProgressCtrl m_ctr_pgrProgress_Auto_DL;

	CEdit m_ctr_edtStatus_Port_1;
	CEdit m_ctr_edtStatus_Port_2;
	CEdit m_ctr_edtStatus_Port_3;
	CEdit m_ctr_edtStatus_Port_4;
	CEdit m_ctr_edtStatus_Port_5;
	CEdit m_ctr_edtStatus_Port_6;
	CEdit m_ctr_edtStatus_Port_7;
	CEdit m_ctr_edtStatus_Port_8;
	CEdit m_ctr_edtStatus_Port_9;
	CEdit m_ctr_edtStatus_Port_10;
	CEdit m_ctr_edtTime_1;
	CEdit m_ctr_edtTime_2;
	CEdit m_ctr_edtTime_3;
	CEdit m_ctr_edtTime_4;
	CEdit m_ctr_edtTime_5;
	CEdit m_ctr_edtTime_6;
	CEdit m_ctr_edtTime_7;
	CEdit m_ctr_edtTime_8;
	CEdit m_ctr_edtTime_9;
	CEdit m_ctr_edtTime_10;
	CEdit m_CtrlPort1;
	CEdit m_CtrlPort2;
	CEdit m_CtrlPort3;
	CEdit m_CtrlPort4;
	CEdit m_CtrlPort5;
	CEdit m_CtrlPort6;
	CEdit m_CtrlPort7;
	CEdit m_CtrlPort8;
	CEdit m_CtrlPort9;
	CEdit m_CtrlPort10;
	CEdit m_ctrB1;
	CEdit m_ctrB2;
	CEdit m_ctrB3;
	CEdit m_ctrB4;
	CEdit m_ctrB5;
	CEdit m_ctrB6;
	CEdit m_ctrB7;
	CEdit m_ctrB8;
	CEdit m_ctrB9;
	CEdit m_ctrB10;

	CRichEditCtrl m_ctrlOut1;
	CRichEditCtrl m_ctrlOut2;
	CRichEditCtrl m_ctrlOut3;
	CRichEditCtrl m_ctrlOut4;
	CRichEditCtrl m_ctrlOut5;
	CRichEditCtrl m_ctrlOut6;
	CRichEditCtrl m_ctrlOut7;
	CRichEditCtrl m_ctrlOut8;
	CRichEditCtrl m_ctrlOut9;
	CRichEditCtrl m_ctrlOut10;
	CListCtrl m_ctr_lscBin_List_Multi;

	CIniFile m_cla_iniFile;
	CIniFile m_cla_iniParams;

	CIniFile m_cla_iniFile_SPD;//展锐平台的配置文件

	CStatic m_CtlEn;
	CStatic m_CtrlLN;
	CStdioFile m_cla_Debug_Log_Stdio_File_1;
	CStdioFile m_cla_Debug_Log_Stdio_File_2;
	CStdioFile m_cla_Debug_Log_Stdio_File_3;
	CStdioFile m_cla_Debug_Log_Stdio_File_4;
	CStdioFile m_cla_Debug_Log_Stdio_File_5;
	CStdioFile m_cla_Debug_Log_Stdio_File_6;
	CStdioFile m_cla_Debug_Log_Stdio_File_7;
	CStdioFile m_cla_Debug_Log_Stdio_File_8;
	CStdioFile m_cla_Debug_Log_Stdio_File_9;
	CStdioFile m_cla_Debug_Log_Stdio_File_10;

	CFont m_cla_fontBtn_1;
	CFont m_cla_fontBtn_2;
	CFont m_cla_fontEdt_1;
	CFont m_cla_fontEdt_2;
	CFont m_cla_fontEdt_3;

	time_t m_time_Start_1;
	time_t m_time_Start_2;
	time_t m_time_Start_3;
	time_t m_time_Start_4;
	time_t m_time_Start_5;
	time_t m_time_Start_6;
	time_t m_time_Start_7;
	time_t m_time_Start_8;
	time_t m_time_Start_9;
	time_t m_time_Start_10;

	CFlash_OP m_cla_Flash_OP_1;
	CFlash_OP m_cla_Flash_OP_2;
	CFlash_OP m_cla_Flash_OP_3;
	CFlash_OP m_cla_Flash_OP_4;
	CFlash_OP m_cla_Flash_OP_5;
	CFlash_OP m_cla_Flash_OP_6;
	CFlash_OP m_cla_Flash_OP_7;
	CFlash_OP m_cla_Flash_OP_8;
	CFlash_OP m_cla_Flash_OP_9;
	CFlash_OP m_cla_Flash_OP_10;

	CFlash_OP_NB m_cla_Flash_OP_NB_1;
	CFlash_OP_NB m_cla_Flash_OP_NB_2;
	CFlash_OP_NB m_cla_Flash_OP_NB_3;
	CFlash_OP_NB m_cla_Flash_OP_NB_4;
	CFlash_OP_NB m_cla_Flash_OP_NB_5;
	CFlash_OP_NB m_cla_Flash_OP_NB_6;
	CFlash_OP_NB m_cla_Flash_OP_NB_7;
	CFlash_OP_NB m_cla_Flash_OP_NB_8;
	CFlash_OP_NB m_cla_Flash_OP_NB_9;
	CFlash_OP_NB m_cla_Flash_OP_NB_10;

	CFlash_OP_5G m_cla_Flash_OP_5G_1;
	CFlash_OP_5G m_cla_Flash_OP_5G_2;
	CFlash_OP_5G m_cla_Flash_OP_5G_3;
	CFlash_OP_5G m_cla_Flash_OP_5G_4;
	CFlash_OP_5G m_cla_Flash_OP_5G_5;
	CFlash_OP_5G m_cla_Flash_OP_5G_6;
	CFlash_OP_5G m_cla_Flash_OP_5G_7;
	CFlash_OP_5G m_cla_Flash_OP_5G_8;
	CFlash_OP_5G m_cla_Flash_OP_5G_9;
	CFlash_OP_5G m_cla_Flash_OP_5G_10;

	CFlash_OP_AG m_cla_Flash_OP_AG_1;
	CFlash_OP_AG m_cla_Flash_OP_AG_2;
	CFlash_OP_AG m_cla_Flash_OP_AG_3;
	CFlash_OP_AG m_cla_Flash_OP_AG_4;
	CFlash_OP_AG m_cla_Flash_OP_AG_5;
	CFlash_OP_AG m_cla_Flash_OP_AG_6;
	CFlash_OP_AG m_cla_Flash_OP_AG_7;
	CFlash_OP_AG m_cla_Flash_OP_AG_8;
	CFlash_OP_AG m_cla_Flash_OP_AG_9;
	CFlash_OP_AG m_cla_Flash_OP_AG_10;

	CARM_OP_QCM m_cla_ARM_OP_QCM_1;
	CARM_OP_QCM m_cla_ARM_OP_QCM_2;
	CARM_OP_QCM m_cla_ARM_OP_QCM_3;
	CARM_OP_QCM m_cla_ARM_OP_QCM_4;
	CARM_OP_QCM m_cla_ARM_OP_QCM_5;
	CARM_OP_QCM m_cla_ARM_OP_QCM_6;
	CARM_OP_QCM m_cla_ARM_OP_QCM_7;
	CARM_OP_QCM m_cla_ARM_OP_QCM_8;
	CARM_OP_QCM m_cla_ARM_OP_QCM_9;
	CARM_OP_QCM m_cla_ARM_OP_QCM_10;

	CFlash_OP_GK m_cla_Flash_GK_1;
	CFlash_OP_GK m_cla_Flash_GK_2;
	CFlash_OP_GK m_cla_Flash_GK_3;
	CFlash_OP_GK m_cla_Flash_GK_4;
	CFlash_OP_GK m_cla_Flash_GK_5;
	CFlash_OP_GK m_cla_Flash_GK_6;
	CFlash_OP_GK m_cla_Flash_GK_7;
	CFlash_OP_GK m_cla_Flash_GK_8;
	CFlash_OP_GK m_cla_Flash_GK_9;
	CFlash_OP_GK m_cla_Flash_GK_10;

	CARM_OP_EIGEN m_cla_ARM_OP_EIGEN_1;
	CARM_OP_EIGEN m_cla_ARM_OP_EIGEN_2;
	CARM_OP_EIGEN m_cla_ARM_OP_EIGEN_3;
	CARM_OP_EIGEN m_cla_ARM_OP_EIGEN_4;
	CARM_OP_EIGEN m_cla_ARM_OP_EIGEN_5;
	CARM_OP_EIGEN m_cla_ARM_OP_EIGEN_6;
	CARM_OP_EIGEN m_cla_ARM_OP_EIGEN_7;
	CARM_OP_EIGEN m_cla_ARM_OP_EIGEN_8;
	CARM_OP_EIGEN m_cla_ARM_OP_EIGEN_9;
	CARM_OP_EIGEN m_cla_ARM_OP_EIGEN_10;

	CARM_OP_BEKEN m_cla_ARM_OP_BEKEN_1;
	CARM_OP_BEKEN m_cla_ARM_OP_BEKEN_2;
	CARM_OP_BEKEN m_cla_ARM_OP_BEKEN_3;
	CARM_OP_BEKEN m_cla_ARM_OP_BEKEN_4;
	CARM_OP_BEKEN m_cla_ARM_OP_BEKEN_5;
	CARM_OP_BEKEN m_cla_ARM_OP_BEKEN_6;
	CARM_OP_BEKEN m_cla_ARM_OP_BEKEN_7;
	CARM_OP_BEKEN m_cla_ARM_OP_BEKEN_8;
	CARM_OP_BEKEN m_cla_ARM_OP_BEKEN_9;
	CARM_OP_BEKEN m_cla_ARM_OP_BEKEN_10;

	CARM_OP_UNISOC m_cla_ARM_OP_UNISOC_1;
	CARM_OP_UNISOC m_cla_ARM_OP_UNISOC_2;
	CARM_OP_UNISOC m_cla_ARM_OP_UNISOC_3;
	CARM_OP_UNISOC m_cla_ARM_OP_UNISOC_4;
	CARM_OP_UNISOC m_cla_ARM_OP_UNISOC_5;
	CARM_OP_UNISOC m_cla_ARM_OP_UNISOC_6;
	CARM_OP_UNISOC m_cla_ARM_OP_UNISOC_7;
	CARM_OP_UNISOC m_cla_ARM_OP_UNISOC_8;
	CARM_OP_UNISOC m_cla_ARM_OP_UNISOC_9;
	CARM_OP_UNISOC m_cla_ARM_OP_UNISOC_10;

	CARM_OP_XINYI m_cla_ARM_OP_XINYI_1;
	CARM_OP_XINYI m_cla_ARM_OP_XINYI_2;
	CARM_OP_XINYI m_cla_ARM_OP_XINYI_3;
	CARM_OP_XINYI m_cla_ARM_OP_XINYI_4;
	CARM_OP_XINYI m_cla_ARM_OP_XINYI_5;
	CARM_OP_XINYI m_cla_ARM_OP_XINYI_6;
	CARM_OP_XINYI m_cla_ARM_OP_XINYI_7;
	CARM_OP_XINYI m_cla_ARM_OP_XINYI_8;
	CARM_OP_XINYI m_cla_ARM_OP_XINYI_9;
	CARM_OP_XINYI m_cla_ARM_OP_XINYI_10;

	CARM_OP_ALTAIR m_cla_ARM_OP_ALTAIR_1;
	CARM_OP_ALTAIR m_cla_ARM_OP_ALTAIR_2;
	CARM_OP_ALTAIR m_cla_ARM_OP_ALTAIR_3;
	CARM_OP_ALTAIR m_cla_ARM_OP_ALTAIR_4;
	CARM_OP_ALTAIR m_cla_ARM_OP_ALTAIR_5;
	CARM_OP_ALTAIR m_cla_ARM_OP_ALTAIR_6;
	CARM_OP_ALTAIR m_cla_ARM_OP_ALTAIR_7;
	CARM_OP_ALTAIR m_cla_ARM_OP_ALTAIR_8;
	CARM_OP_ALTAIR m_cla_ARM_OP_ALTAIR_9;
	CARM_OP_ALTAIR m_cla_ARM_OP_ALTAIR_10;

	CARM_OP_RDA m_cla_ARM_OP_RDA_1;
	CARM_OP_RDA m_cla_ARM_OP_RDA_2;
	CARM_OP_RDA m_cla_ARM_OP_RDA_3;
	CARM_OP_RDA m_cla_ARM_OP_RDA_4;
	CARM_OP_RDA m_cla_ARM_OP_RDA_5;
	CARM_OP_RDA m_cla_ARM_OP_RDA_6;
	CARM_OP_RDA m_cla_ARM_OP_RDA_7;
	CARM_OP_RDA m_cla_ARM_OP_RDA_8;
	CARM_OP_RDA m_cla_ARM_OP_RDA_9;
	CARM_OP_RDA m_cla_ARM_OP_RDA_10;

	CARM_OP_INTEL m_cla_ARM_OP_INTEL_1;
	CARM_OP_INTEL m_cla_ARM_OP_INTEL_2;
	CARM_OP_INTEL m_cla_ARM_OP_INTEL_3;
	CARM_OP_INTEL m_cla_ARM_OP_INTEL_4;
	CARM_OP_INTEL m_cla_ARM_OP_INTEL_5;
	CARM_OP_INTEL m_cla_ARM_OP_INTEL_6;
	CARM_OP_INTEL m_cla_ARM_OP_INTEL_7;
	CARM_OP_INTEL m_cla_ARM_OP_INTEL_8;
	CARM_OP_INTEL m_cla_ARM_OP_INTEL_9;
	CARM_OP_INTEL m_cla_ARM_OP_INTEL_10;

	CARM_OP_HUAWEI m_cla_ARM_OP_HUAWEI_1;
	CARM_OP_HUAWEI m_cla_ARM_OP_HUAWEI_2;
	CARM_OP_HUAWEI m_cla_ARM_OP_HUAWEI_3;
	CARM_OP_HUAWEI m_cla_ARM_OP_HUAWEI_4;
	CARM_OP_HUAWEI m_cla_ARM_OP_HUAWEI_5;
	CARM_OP_HUAWEI m_cla_ARM_OP_HUAWEI_6;
	CARM_OP_HUAWEI m_cla_ARM_OP_HUAWEI_7;
	CARM_OP_HUAWEI m_cla_ARM_OP_HUAWEI_8;
	CARM_OP_HUAWEI m_cla_ARM_OP_HUAWEI_9;
	CARM_OP_HUAWEI m_cla_ARM_OP_HUAWEI_10;

	CARM_OP_ASR m_cla_ARM_OP_ASR_1;
	CARM_OP_ASR m_cla_ARM_OP_ASR_2;
	CARM_OP_ASR m_cla_ARM_OP_ASR_3;
	CARM_OP_ASR m_cla_ARM_OP_ASR_4;
	CARM_OP_ASR m_cla_ARM_OP_ASR_5;
	CARM_OP_ASR m_cla_ARM_OP_ASR_6;
	CARM_OP_ASR m_cla_ARM_OP_ASR_7;
	CARM_OP_ASR m_cla_ARM_OP_ASR_8;
	CARM_OP_ASR m_cla_ARM_OP_ASR_9;
	CARM_OP_ASR m_cla_ARM_OP_ASR_10;

	CARM_OP_SILABS m_cla_ARM_OP_SILABS_1;
	CARM_OP_SILABS m_cla_ARM_OP_SILABS_2;
	CARM_OP_SILABS m_cla_ARM_OP_SILABS_3;
	CARM_OP_SILABS m_cla_ARM_OP_SILABS_4;
	CARM_OP_SILABS m_cla_ARM_OP_SILABS_5;
	CARM_OP_SILABS m_cla_ARM_OP_SILABS_6;
	CARM_OP_SILABS m_cla_ARM_OP_SILABS_7;
	CARM_OP_SILABS m_cla_ARM_OP_SILABS_8;
	CARM_OP_SILABS m_cla_ARM_OP_SILABS_9;
	CARM_OP_SILABS m_cla_ARM_OP_SILABS_10;
	

	//------------------------------------------------------
	void Reset_Progress_Ctrl(int i_Index);
	bool Load_FW_Files(void);
	void Clear_View_Bin_List(void);
	bool Get_Platform(const wchar_t * wcp_FW_File_Path, int * ip_Platform);
	void Close_Trace_Log_File_all(void);
	void Save_Trace_Log_File(const wchar_t * wcp_Data, int index);
	void Close_Trace_Log_File_1(void);
	void Close_Trace_Log_File_2(void);
	void Close_Trace_Log_File_3(void);
	void Close_Trace_Log_File_4(void);
	void Close_Trace_Log_File_5(void);
	void Close_Trace_Log_File_6(void);
	void Close_Trace_Log_File_7(void);
	void Close_Trace_Log_File_8(void);
	void Close_Trace_Log_File_9(void);
	void Close_Trace_Log_File_10(void);
	void Set_View_Ctrl_Font(void);
	void Set_View_Ctrl_State(int i_State, int i_Index);
	void Read_Config(void);
	void Save_Config(void);
	void Show_Out(const wchar_t * wcp_Status, int i_Index);
	void Show_Status(const wchar_t * wcp_Status, int i_Index);
	void Show_COMPORT(const wchar_t * wcp_Status, int i_Index);
	void Show_Time(const wchar_t * wcp_Time, int i_Index);
	void Reset_View(int i_Index);
	void Ctrl_Colour(int i_Colour, int i_Index);
	void Load_Com_Port_Numb(int i_Index);
	void Load_Flash_Option_Params(int i_Index);
	void Get_Com_Port_Ctrl_Numb(int i_Index, int * ip_Com_Port_Numb);
	void Set_Com_Port_Cla_Numb(int i_Index, int i_Com_Port_Numb);
	void Set_Baud_Rate_Cla_Numb(int i_Index, int i_BR_Index);
	void Set_Debug_Log_Cla_Enable(int i_Index, bool b_Enable);
	void Load_Com_Cla_Params_QCMM(int i_Index);
	void Load_Com_Cla_Params_RDA(int i_Index);
	bool Load_All_DL_Files_MTK(void);
	bool Load_All_DL_Files_QCMM(void);
	void Load_Flash_Product_Params(const wchar_t * wcp_Product);
	void Add_ListCtrl_Bin_List_MTK(const ROM_INFO_EX str_Rom_Info_Ex);
	void Load_View_Bin_List_MTK(void);
	void Load_Params_QCMM(const wchar_t * wcp_Product);
	void Load_View_Bin_List_QCMM(void);
	void Add_ListCtrl_Bin_List_Data(const wchar_t * wcp_Data_1, const wchar_t * wcp_Data_2, bool b_Check);
	void Init_Progress_Ctrl(int i_Platform, int i_OP_TYPE);
	bool INIT_META_HANDLE(int i_Index);
	bool Load_Com_Cla_NV_Retain_File_List(int i_Index);
	void Update_Com_Ctrl_Option(int i_Index);
	void Delete_Com_Ctrl_Option(CComboBox * clap_cbxCom_Port);
	void Load_Com_Ctrl_Option(CComboBox * clap_cbxCom_Port, int * ip_Com_Port_Idx);
	void Set_Progress(CProgressCtrl * clap_pgrProgress, int i_Pgr_Percent, int i_Index);

	//------------------------------------------------------
	LRESULT OnGetMsg_Trace_Log_Msg(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Show_Status(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Unisoc_Show_Status(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Show_COMPORT(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Out_Show_Status(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Ctrl_Colour(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Show_Progress(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Show_Progress_Entire(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Show_Progress_Auto_DL(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Show_ZIP_Auto_DL(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Show_FTP_Time(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_DL_Info(WPARAM wParam, LPARAM lParam);
	LRESULT OnPowerOn_SPC(WPARAM wParam, LPARAM lParam);
	LRESULT OnPowerOn_SPC_Only(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Scan_DL_Start(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Scan_DL_Start_Four(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Scan_Start_Four(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Process_TCP_Accept(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Process_TCP_OP(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Get_LAN_MO_Name(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Get_LAN_Barcode(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Clear_TS_FPY(WPARAM wParam, LPARAM lParam);
	LRESULT OnMsg_Socket_TCP(WPARAM wParam, LPARAM lParam);
	LRESULT OnMsg_Socket_TCP_SERVER(WPARAM wParam, LPARAM lParam);


	//------------------------------------------------------
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnItemchangedListBinListMulti(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSetScatFile();
	afx_msg void OnCbnSelchangeComboBrMulti();
	afx_msg void OnCbnDropdownComboCom1();
	afx_msg void OnCbnSelchangeComboCom1();
	afx_msg void OnCbnDropdownComboCom2();
	afx_msg void OnCbnSelchangeComboCom2();
	afx_msg void OnCbnDropdownComboCom3();
	afx_msg void OnCbnDropdownComboCom4();
	afx_msg void OnCbnDropdownComboCom5();
	afx_msg void OnCbnDropdownComboCom6();
	afx_msg void OnCbnDropdownComboCom7();
	afx_msg void OnCbnDropdownComboCom8();
	afx_msg void OnCbnDropdownComboCom9();
	afx_msg void OnCbnDropdownComboCom10();
	afx_msg void OnCbnSelchangeComboCom3();
	afx_msg void OnCbnSelchangeComboCom4();
	afx_msg void OnCbnSelchangeComboCom5();
	afx_msg void OnCbnSelchangeComboCom6();
	afx_msg void OnCbnSelchangeComboCom7();
	afx_msg void OnCbnSelchangeComboCom8();
	afx_msg void OnCbnSelchangeComboCom9();
	afx_msg void OnCbnSelchangeComboCom10();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStart1();
	afx_msg void OnBnClickedButtonStart2();
	afx_msg void OnBnClickedButtonStart3();
	afx_msg void OnBnClickedButtonStart4();
	afx_msg void OnBnClickedButtonStart5();
	afx_msg void OnBnClickedButtonStart6();
	afx_msg void OnBnClickedButtonStart7();
	afx_msg void OnBnClickedButtonStart8();
	afx_msg void OnBnClickedButtonStart9();
	afx_msg void OnBnClickedButtonStart10();
	afx_msg void OnBnClickedButtonStopAll();
	afx_msg void OnBnClickedButtonStartAll();
	afx_msg void OnCbnSelchangeComboCtrlDlType();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeComboOcpuModelM();
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnEditCopy();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeB1();
	afx_msg void OnEnChangeB2();
	afx_msg void OnEnChangeB3();
	afx_msg void OnEnChangeB4();
	afx_msg void OnEnChangeB5();
	afx_msg void OnEnChangeB6();
	afx_msg void OnEnChangeB7();
	afx_msg void OnEnChangeB8();
	afx_msg void OnEnChangeB9();
	afx_msg void OnEnChangeB10();
	afx_msg void OnEnChangeEP1();
	afx_msg void OnEnChangeEP2();
	afx_msg void OnEnChangeEP3();
	afx_msg void OnEnChangeEP4();
	afx_msg void OnEnChangeEP5();
	afx_msg void OnEnChangeEP6();
	afx_msg void OnEnChangeEP7();
	afx_msg void OnEnChangeEP8();
	afx_msg void OnEnChangeEP9();
	afx_msg void OnEnChangeEP10();
	afx_msg void OnEnHscrollRichedit2ShowInfo();
	afx_msg void OnEnVscrollRichedit2ShowInfo();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnUpdateRichedit2ShowInfo();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//RSA
	afx_msg LRESULT OnCallbackFunc_RSA(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButDlUnlock();
	//------------------------------------------------------
	HANDLE m_hThread_Handle_1;
	HANDLE m_hThread_Handle_2;
	HANDLE m_hThread_Handle_3;
	HANDLE m_hThread_Handle_4;
	HANDLE m_hThread_Handle_5;
	HANDLE m_hThread_Handle_6;
	HANDLE m_hThread_Handle_7;
	HANDLE m_hThread_Handle_8;
	HANDLE m_hThread_Handle_9;
	HANDLE m_hThread_Handle_10;
	DWORD m_dwThread_ID_1;
	DWORD m_dwThread_ID_2;
	DWORD m_dwThread_ID_3;
	DWORD m_dwThread_ID_4;
	DWORD m_dwThread_ID_5;
	DWORD m_dwThread_ID_6;
	DWORD m_dwThread_ID_7;
	DWORD m_dwThread_ID_8;
	DWORD m_dwThread_ID_9;
	DWORD m_dwThread_ID_10;

	HANDLE m_hThread_Handle_1_AT;
	HANDLE m_hThread_Handle_2_AT;
	HANDLE m_hThread_Handle_3_AT;
	HANDLE m_hThread_Handle_4_AT;
	HANDLE m_hThread_Handle_5_AT;
	HANDLE m_hThread_Handle_6_AT;
	HANDLE m_hThread_Handle_7_AT;
	HANDLE m_hThread_Handle_8_AT;
	HANDLE m_hThread_Handle_9_AT;
	HANDLE m_hThread_Handle_10_AT;
	DWORD m_dwThread_ID_1_AT;
	DWORD m_dwThread_ID_2_AT;
	DWORD m_dwThread_ID_3_AT;
	DWORD m_dwThread_ID_4_AT;
	DWORD m_dwThread_ID_5_AT;
	DWORD m_dwThread_ID_6_AT;
	DWORD m_dwThread_ID_7_AT;
	DWORD m_dwThread_ID_8_AT;
	DWORD m_dwThread_ID_9_AT;
	DWORD m_dwThread_ID_10_AT;
	

	HANDLE m_hThread_Auto_SPC_PowON_Handle;
	HANDLE m_hThread_Auto_SPC_PowReset_Handle;
	DWORD m_dwThread_Auto_SPC_PowON_ID;
	DWORD m_dwThread_Auto_SPC_PowReset_ID;


	HANDLE m_hThread_Auto_PLC_Pow_Handle_1;
	DWORD  m_dwThread_Auto_PLC_Pow_ID_1;
	HANDLE m_hThread_Auto_PLC_Pow_Handle_2;
	DWORD  m_dwThread_Auto_PLC_Pow_ID_2;
	HANDLE m_hThread_Auto_PLC_Pow_Handle_3;
	DWORD  m_dwThread_Auto_PLC_Pow_ID_3;
	HANDLE m_hThread_Auto_PLC_Pow_Handle_4;
	DWORD  m_dwThread_Auto_PLC_Pow_ID_4;	
	HANDLE m_hThread_Auto_PLC_Pow_Handle_5;
	DWORD  m_dwThread_Auto_PLC_Pow_ID_5;	
	HANDLE m_hThread_Auto_PLC_Pow_Handle_6;
	DWORD  m_dwThread_Auto_PLC_Pow_ID_6;	
	HANDLE m_hThread_Auto_PLC_Pow_Handle_7;
	DWORD  m_dwThread_Auto_PLC_Pow_ID_7;	
	HANDLE m_hThread_Auto_PLC_Pow_Handle_8;
	DWORD  m_dwThread_Auto_PLC_Pow_ID_8;	
	HANDLE m_hThread_Auto_PLC_Pow_Handle_9;
	DWORD  m_dwThread_Auto_PLC_Pow_ID_9;	
	HANDLE m_hThread_Auto_PLC_Pow_Handle_10;
	DWORD  m_dwThread_Auto_PLC_Pow_ID_10;

	HANDLE m_hThread_Auto_ON_Handle_1;
	DWORD  m_dwThread_Auto_ON_ID_1;
	HANDLE m_hThread_Auto_ON_Handle_2;
	DWORD  m_dwThread_Auto_ON_ID_2;
	HANDLE m_hThread_Auto_ON_Handle_3;
	DWORD  m_dwThread_Auto_ON_ID_3;
	HANDLE m_hThread_Auto_ON_Handle_4;
	DWORD  m_dwThread_Auto_ON_ID_4;
	HANDLE m_hThread_Auto_ON_Handle_5;
	DWORD  m_dwThread_Auto_ON_ID_5;
	HANDLE m_hThread_Auto_ON_Handle_6;
	DWORD  m_dwThread_Auto_ON_ID_6;
	HANDLE m_hThread_Auto_ON_Handle_7;
	DWORD  m_dwThread_Auto_ON_ID_7;
	HANDLE m_hThread_Auto_ON_Handle_8;
	DWORD  m_dwThread_Auto_ON_ID_8;
	HANDLE m_hThread_Auto_ON_Handle_9;
	DWORD  m_dwThread_Auto_ON_ID_9;
	HANDLE m_hThread_Auto_ON_Handle_10;
	DWORD  m_dwThread_Auto_ON_ID_10;

	HANDLE m_hThread_PLC_Pow_Handle_1;
	HANDLE m_hThread_PLC_Pow_Handle_2;
	HANDLE m_hThread_PLC_Pow_Handle_3;
	HANDLE m_hThread_PLC_Pow_Handle_4;

	HANDLE m_hThread_Auto_SPC_Pow_Handle_1;
	DWORD  m_dwThread_Auto_SPC_Pow_ID_1;
	HANDLE m_hThread_Auto_SPC_Pow_Handle_2;
	DWORD  m_dwThread_Auto_SPC_Pow_ID_2;
	HANDLE m_hThread_Auto_SPC_Pow_Handle_3;
	DWORD  m_dwThread_Auto_SPC_Pow_ID_3;
	HANDLE m_hThread_Auto_SPC_Pow_Handle_4;
	DWORD  m_dwThread_Auto_SPC_Pow_ID_4;
	HANDLE m_hThread_Auto_SPC_PowON_Handle_1;
	DWORD  m_dwThread_Auto_SPC_PowON_ID_1;
	HANDLE m_hThread_Auto_SPC_PowON_Handle_2;
	DWORD  m_dwThread_Auto_SPC_PowON_ID_2;
	HANDLE m_hThread_Auto_SPC_PowON_Handle_3;
	DWORD  m_dwThread_Auto_SPC_PowON_ID_3;
	HANDLE m_hThread_Auto_SPC_PowON_Handle_4;
	DWORD  m_dwThread_Auto_SPC_PowON_ID_4;

	HANDLE m_hThread_PLC_PowOn_Handle_1;
	HANDLE m_hThread_PLC_PowOn_Handle_2;
	HANDLE m_hThread_PLC_PowOn_Handle_3;
	HANDLE m_hThread_PLC_PowOn_Handle_4;
	

	HANDLE m_hThread_PLC_Auto_Handle_1;
	HANDLE m_hThread_PLC_Auto_Handle_2;
	HANDLE m_hThread_PLC_Auto_Handle_3;
	HANDLE m_hThread_PLC_Auto_Handle_4;

	HANDLE m_hLenovoHandle;

	DWORD m_dwThread_PLC_Pow_ID_1;
	DWORD m_dwThread_PLC_Pow_ID_2;
	DWORD m_dwThread_PLC_Pow_ID_3;
	DWORD m_dwThread_PLC_Pow_ID_4;

	DWORD m_dwThread_PLC_PowOn_ID_1;
	DWORD m_dwThread_PLC_PowOn_ID_2;
	DWORD m_dwThread_PLC_PowOn_ID_3;
	DWORD m_dwThread_PLC_PowOn_ID_4;
	

	DWORD  m_dwThread_Auto_ID_1;
	DWORD  m_dwThread_Auto_ID_2;
	DWORD  m_dwThread_Auto_ID_3;
	DWORD  m_dwThread_Auto_ID_4;


	HANDLE m_hThread_BC39_DL_QGNSSC_ON_Handle_1;
	HANDLE m_hThread_BC39_DL_QGNSSC_ON_Handle_2;
	HANDLE m_hThread_BC39_DL_QGNSSC_ON_Handle_3;
	HANDLE m_hThread_BC39_DL_QGNSSC_ON_Handle_4;

	DWORD m_dwThread_BC39_DL_QGNSSC_ON_1;
	DWORD m_dwThread_BC39_DL_QGNSSC_ON_2;
	DWORD m_dwThread_BC39_DL_QGNSSC_ON_3;
	DWORD m_dwThread_BC39_DL_QGNSSC_ON_4;
	

	CComboBox m_ctr_cbxCtrl_EDL_Type_M;
	CComboBox m_ctr_cbxOCPU_Name_M;
	CStatic m_ctr_staCtrl_EDL_Type_M;
	CStatic m_ctr_staOCPU_Name_M;



	
	void Start_Timer(int i_Timer_Number);
	void Set_Ctrl_DL_Type(int i_Index, int i_Ctrl_Dl_Type,int iDelay_Time);
	void Load_Advanced_Config_QCMM(int i_Ctrl_OP_Option);
	bool Judge_Ctrl_OP_Option_QCMM(void);
	bool Get_FW_File_Size_QCMM(void);
	bool Load_FW_File_QCMM(const wchar_t * wcp_FW_File_Path);
	void Load_Nv_Def_File_QCMM(const wchar_t * wcp_Nv_Def_File_Name);
	void Set_Ctrl_DLoad_Port(int i_Index, int i_Ctrl_DLoad_Port);
	bool Load_All_DL_Files_RDA(void);
	void Load_View_Bin_List_RDA(void);
	void COPY_STR_LOD_FILE(LOD_FILE_S * strp_New, const LOD_FILE_S str_Src);
	bool Load_All_DL_Files_Intel(void);
	bool Load_All_DL_Files_EIGEN(void);
	bool Load_All_DL_Files_BEKEN(void);
	bool Load_All_DL_Files_SILABS(void);
	bool Load_All_DL_Files_UNISOC(void);
	bool Load_All_DL_Files_XINYI(void);
	bool Load_All_DL_Files_ALTAIR(void);
	void COPY_CLA_ARM_OP_INTEL(CARM_OP_INTEL * clap_New, CARM_OP_INTEL * clap_Src);
	void Load_View_Bin_List_Intel(void);
	void Load_Params_Intel(const wchar_t * wcp_Product);
	void Load_Temp_FW_File_QCMM(const wchar_t * wcp_Cur_Exe_File_Path);
	bool Load_BB_Info_QCMM(const wchar_t * wcp_FW_File_Path);	
	void Load_View_Static_Value(void);
	void Load_Robot_Option_Params(void);
	void Start_Test_3_Axis_Robot_V1(void);
	bool Do_Detect_3_Axis_Robot_V1(void);

	CButton m_ctr_btnConfig;
	CDlg_Config_Multi m_dlg_Config_Multi;
	CRichEditCtrl m_ctr_redtShow_Info;
	CGFC m_cla_GFC;
	HANDLE m_hThread_Hd_Robot;
	DWORD m_dwThread_ID_Robot;
	HANDLE m_hThread_Hd_Robot_Four;
	DWORD m_dwThread_ID_Robot_Four;
	PKT_BUFF_S m_str_Pkt_Send;
	SQL_RECORD_SET_S m_str_MO_STD;
	SQL_RECORD_SET_S m_str_O_MO_STD;//

	void Show_REdit_Info(const wchar_t * wcp_Msg, bool b_RN);
	//LRESULT OnGetMsg_Show_REdit_Info(WPARAM wParam, LPARAM lParam);
	bool Get_Robot_Rsp_Useful_Data(char * cp_Buff_Pointer, unsigned long ul_Buff_Data_Len, int i_Rsp_Type, CString * csp_Get_Useful_Data, int * ip_Get_Quan);
	bool Detect_Robot_Rsp_Data_Type(char * cp_Buff_Pointer, unsigned long ul_Buff_Data_Len, int * ip_Rsp_Type);
	bool Decode_Robot_Rsp_Data_V1(int * ip_Rsp_Cmd_Type, const wchar_t * wcp_Rsp_Data, int * ip_Info_Numb, CString * csp_Barcode);
	bool Decode_Robot_Rsp_Data_1(const wchar_t * wcp_Rsp_Data, int * ip_State_Numb);
	bool Decode_Robot_Rsp_Data_2(const wchar_t * wcp_Rsp_Data, int * ip_Gasbin_Numb, int * ip_Detail_Rsp_Type);
	bool Decode_Robot_Rsp_Data_3_V1(const wchar_t * wcp_Rsp_Data, int * ip_Put_Numb, int * ip_Detail_Rsp_Type, CString * csp_Barcode);
	bool Decode_Robot_Rsp_Data_4(const wchar_t * wcp_Rsp_Data, int * ip_Fail_Numb);
	bool Process_3_Axis_Robot_Data_V1(int i_Rsp_Cmd_Type, int i_Info_Numb, const wchar_t * wcp_Barcode);
	bool Process_Robot_Data_1(int i_State_Numb);
	bool Process_Robot_Data_21_V1(int i_Gasbin_Up_Numb);
	bool Process_Robot_Data_22_V1(int i_Gasbin_Down_Numb);
	bool Process_Robot_Data_3_V1(int i_Put_Numb, const wchar_t * wcp_Barcode);
	bool Process_Robot_Data_4(int i_Fail_Numb);
	void Process_Robot_Stop_And_Red_LED(void);
	void Init_PLC_Of_Semi_Fixture(void);	
	void PKT_PLC_CMD(PKT_BUFF_S * strp_Pkt_Cmd, int i_Ch, int i_Cmd_Code);
	bool UnPKT_PLC_RSP(char * cp_PLC_Rsp, int * ip_Ch, int * ip_Rsp_Code);
	bool Read_PLC_Rsp(int * ip_Ch, int * ip_Rsp_Code, int i_TimeOut);
	void GET_PORT_RX_BUFF(char ** cp2_Buff_Pointer, unsigned long * ulp_Data_Length);
	void Open_Fixture(int i_Fixture_Numb);
	void Close_Fixture(int i_Fixture_Numb);
	void Query_Fixture(int i_Fixture_Numb);
	void Start_Test_Semi_Fixture(void);
	bool Do_Detect_PLC_Semi_Fixture(void);
	void Start_Test_Semi_Fixture_Four(void);
	bool Do_Detect_PLC_Semi_Fixture_Four(void);
	bool Scan_Barcode(int i_Index, CString * csp_Barcode);
	void Load_Com_Port_Data(int i_Index);
	void Read_Server_Config(void);
	bool Read_Server_Config_Ext(void);
	void Get_Com_Port_Ctrl_View(int i_Index, CString * csp_Com_Port_View);
	bool Upload_DL_Result_To_SQL_DB(const wchar_t * wcp_Key_IMEI, bool b_Result, const wchar_t * wcp_Err_Info);
	void Start_Test_3_Axis_Robot_V2(void);
	bool Do_Detect_3_Axis_Robot_V2(void);
	bool Decode_Robot_Rsp_Data_V2(int * ip_Rsp_Cmd_Type, const wchar_t * wcp_Rsp_Data, int * ip_Info_Numb, CString * csp_Barcode_List);
	bool Decode_Robot_Rsp_Data_3_V2(const wchar_t * wcp_Rsp_Data, int * ip_Put_Numb, int * ip_Detail_Rsp_Type, CString * csp_Barcode_List);
	bool Process_3_Axis_Robot_Data_V2(int i_Rsp_Cmd_Type, int i_Info_Numb, CString * csp_Barcode_List);
	bool Process_Robot_Data_31_V2(int i_Put_Numb, CString * csp_Barcode_List);
	bool Process_Robot_Data_32_V2(int i_Put_Numb);
	bool Process_Robot_Data_21_V2(int i_Gasbin_Up_Numb);
	bool Process_Robot_Data_22_V2(int i_Gasbin_Down_Numb);
	void Save_Server_Config(void);
	bool Get_TS_Index(int i_TS_Number, int * ip_TS_CH);
	void DO_DL_OP_Test(void);
	bool OpenConnection(CString server, CString strUser, CString strPass);
	void DownloadFile();
	int GetMultipleFile(CStringArray *remoteArray,
		CStringArray *localArray,
		int number_file, int iType);//增加类型设置参数  1:MD5  2:Firmware File
	bool CloseConnection();

	HANDLE m_hThread_Handle_test;
	DWORD m_dwThread_test_ID;
	CInternetSession* pInternetSession;
	CFtpConnection*  pFtpConnection;
	COLORREF m_color_Text;
	CIniFile m_cla_IniStatistics;
	HANDLE  m_DwFw_Thread_Handle;
	DWORD m_dwThread_DwFw_ID;
	HANDLE m_hThread_Handle_TS_Server;
	DWORD m_dwThread_ID_TS_Server;
	QTI_PROG_ARG m_str_QTI_ARG;

	bool Find_Create_Fw_Folder(const wchar_t * wcp_FW_File_Path,CString strFolderName);
	bool Find_Fw_Folder(CString strFirPath, CString strLocalMd5, CString strLocalFw,CString strLocalFw_RAR,int* pFindMD5,int* pFindFW,int* pRAR_Flag);
	bool IsDirectoryExists(CString const& path);
	CString Find_Fw_File(const wchar_t * wcp_FW_File_Path,int iPlatform_Solution);
	bool Load_All_DL_Files_HUAWEI(void);
	bool Load_All_DL_Files_ASR(void);	
	void Load_ARG_ALL(void);
	void Send_Back_Init_Result(void);
	void Get_Com_Port_From_Ctrl_View(int i_Index, int * ip_Com_Port_Numb);
	bool Load_MO(SQL_RECORD_SET_S str_MO_STD,bool bSecond);
	bool Decode_Every_SW(const wchar_t * wcp_SW_Ver, CString * csp_SW_DL, CString * csp_SW_Label);
	bool Load_Params_ALL_TS_CONFIG(void);
	bool INIT_DLL_ALL(void);
	int DO_WRITE_IMEI(int i_TSCH_Numb, int i_COM_AT_Port);
	bool Get_TS_Number(int i_TS_CH, int * ip_TS_Number);
	void Save_Result_Statistics_Data(int i_TSCH_Numb, int i_TS_Numb, bool b_Result);
	void Reset_Statistics_Data(int i_TSCH_Numb, int i_TS_Numb);
	void Setup_TRD_String(int i_TSCH_Numb, bool b_Test_Result, CString * csp_TRD_String);
	bool CHECK_SQL_IMEI_INFO_V1(int i_TSCH_Numb, const wchar_t * wcp_Key_IMEI);	
	bool DO_WRITE_IMEI_BC95(int  i_TSCH_Numb);
	void Set_Port_Numb_Display(int i_Index);
	bool Upload_DL_Status_To_SQL_DB(int i_TSCH_Numb,const wchar_t * wcp_Key_IMEI, int iStatusFlag);
	bool Upload_FTP_Daily_Record_Excel_File(int i_TSCH_Numb,const wchar_t * wcp_Excel_File_Path);
#ifdef FW_MULTI
#ifndef FW_UPGRADE
CPower_Supply m_cla_PS;

CPower_Supply m_cla_PS_1;
CPower_Supply m_cla_PS_2;
CPower_Supply m_cla_PS_3;
CPower_Supply m_cla_PS_4;
#endif
#endif

	BOOL m_UpdateDataFlag;
	SQL_RECORD_SET_S m_str_Abstract_Log_STD;

	//打开一拖四气动夹具
	BOOL OpenAutoQiDong(int Index);
	//关闭一拖四气动夹具
	BOOL CloseAutoQiDong(int Index);
	//打开其中一路PLC的电源
	BOOL OpenPLC_Power_Path(int Index);
	//关闭其中一路PLC的电源
	BOOL ClosePLC_Power_Path(int Index);

	CWinSocket m_Socket; //客户端
	void OnClientSend( CString csInfo);

	CWinSocket m_SocketServer;//服务器端
	SOCKET m_socketAccept;//客户端socket
	SOCKADDR_IN m_addrClient;//用于保存客户端套接字信息

	int m_iFourInitStatus; //初始化OK:1
	int m_iFourBARCODE_Flag;//二维码状态

	bool m_bMINI_REP_Flag;
	bool m_bMo_REP_Flag;//REP工单
    bool m_bMo_RMA;//RMA工单标志

	//检查MC20/MC60是否在老的Firmware里面
	void Is_MCXX_OldName( CString srcDB_Name);
	bool Load_All_DL_Files_MTK_NB(void);
	bool FindBinFile(CString lpPath);
	void Load_Other_Params(void);

	//设置一拖四气动夹具半自动模式 Index 1：对应的控制箱 通道1和通道2  2：对应的控制箱 通道3和通道4
	BOOL SetAutoMode(int Index);

	//设置一拖四气动夹具读码器模式 Index 1：对应的控制箱 通道1和通道2  2：对应的控制箱 通道3和通道4
	BOOL SetScanMode(int Index);
	//设置一拖四气动夹具初始化
	bool Init_PLC_Of_Semi_Fixture_Four(void);
	//显示下载结果
	void Display_Result(int i_TSCH_Numb,bool bDL_Result);
	void SetFont_S(int i_TSCH_Numb);//大字体
	void SetFont(int i_TSCH_Numb);//小字体
	void Detect_Module(int iRsp_Code);
	void Msg_Scan_Start_Four_Proc(int iMulti_DL_Index);

	//BC95 写号
	bool DO_WRITE_IMEI_BC95_Ext(int  i_TSCH_Numb);
	void Show_Bar_IMEI(const wchar_t * wcp_Status, int i_Index);
	LRESULT OnGetMsg_Bar_IMEI_Show_Status(WPARAM wParam, LPARAM lParam);



	void AddDevice(NotifyStatus* pNtStat);
	BOOL AddDevice(int iUSBPort,int nDeviceType,CString sUSBAddress);
	BOOL SetProgress(NotifyStatus* pNtStat);
	LRESULT OnProgressToView(LPARAM lParam);

	CString  m_strUSBPortAddress[4];
	CString  m_strUSBPortPath[4];
	int m_iUSBPortUseIndex;

	CMapStringToPtr   m_IndexProgressMap;
	void Show_Progress(int i_Pgr_Percent, int i_Index);

	void Set_PowerOn_SPC(int i_Index);

	BOOL              m_bIsInit;
	PInstanceParams  m_lPInstanceParams;
	HMODULE         m_gSWDhModule;
	CString          m_sExePath;   
	CString          m_sConfigFile;
	CString          m_sUserInfo;
	
	ESWDMode		 m_eSWDMode;
	TCHAR m_szBlfFilename[MAX_FILE_LENGTH];
	TCHAR m_szFlasherFileName[MAX_FILE_LENGTH];
	TCHAR m_szFBFFilename[MAX_FILE_LENGTH];

	bool             m_bIsAdminAuthorized;
	bool m_bIsStart;
	bool m_bInitializeBL;
	bool m_bIsPackageReady;
	HMODULE m_gWtptphModule;
	PInstanceParams GetInstanceParam();
	HMODULE GetModule();
	int RSA_DL_Run();
	bool StopDownload();
	ESWDMode GetSWDMode(){return m_eSWDMode;}
	void Save_FPY(int i_TSCH_Numb, CString*   p_strPASS, CString*   p_strFAIL, CString*   p_strFPY);
	//bool Update_SQL_IMEI_INFO_FW(int i_TSCH_Numb, const wchar_t * wcp_Key_IMEI);
	void Show_Progress_AutoDL(double d_Pgr_Percent);
	void Show_ZIP_DL_MSG(const wchar_t * wcp_Status);
	void Show_FTP_Time_MSG(const wchar_t * wcp_Status);
	bool  DeleteDirectory(CString DirName);
	CString m_strDB_FW;//主要是将该字段存入IMEI_2017表SW字段中
	CStatic m_ctrlFTP_Time;
	CStatic m_crlZipInfo;
	CString  m_csDB_MO_Name;
	CString m_csFactory_Info_Name;
	CString  m_csDB_FW_VER_Name;//

	CString  m_csPN_Name;
	bool m_b120M_Flag;

	void Quit(void);

	void Clear_Msg_Out_Show_Status(int index);

	CCom_Port m_cla_Com_PLC_Power;

	LRESULT OnGetMsg_PLC_Open_PATH(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_PLC_Close_PATH(WPARAM wParam, LPARAM lParam);

	//
	bool DO_BC39_Set_QGNSSC_PowerOn(int  i_TSCH_Numb);
	bool DO_BC39_Set_QGNSSC_PowerOFF(int  i_TSCH_Numb);

	bool m_bBC39_GPS_Support;//仅BC39 JA的支持GPS功能
	CString  m_strOC;

	//20190212
	//BC39 国科微GPS打开GPS使能的线程
	void DO_BC39_DL_QGNSSC_ON(int i_TSCH_Numb);

	bool m_bDisPlag4Port;//部分平台仅显示4个端口
	CButton m_ctrlWriteFlash;


	BOOL m_bHasFW_Verion_DB;
	BOOL m_bBurnIn_DB;

	CString m_csTest_Line_Numb_Tz;
	CString m_cs_Operator_ID_Tz;//从总控那获取的线体号和操作员号

	bool GetO_Mo(CString*  csp_OMO);

	CString m_strProType[PRO_NUM];

	double m_dVoltage;
	double m_dCurrent;

	bool Init_Half_PLC_Control();

	//部分机型初始化PLC的电压
	//iVoLType 1:表示低电压 3.3V
	//iVoLType 2:表示低电压 3.8V
	BOOL Init_Half_PLC_Voltage(int iVoLType);

	//20190424 获取第二个口
	void Get_Sec_Com_Port(int i_Index, int * ip_Com_Port_Numb);

	//全自动机器人向总控程序返回“测试结果”以及断电
	void Return_TotalControl_Val(int i_TSCH_Numb, int iTS_Numb, bool bDL_Result);

	//摘要日志结果保存
	int Save_Abstract_Log(int i_TSCH_Numb, int iTS_Numb, bool bDL_Result,CString csError_Code);

	//测试LOG保存
	bool Save_Test_Log(int i_TSCH_Numb, CString csError_Code, bool bDL_Result,CString strPASS, CString strFAIL, CString strFPY);

	//通过配置文件中参数设置
	void Set_ASR_USB_PORT_Init();
	void Set_ASR_USB_PORT_Init_3601();

	//
	HANDLE m_hWaitTestFin[4];

	HANDLE m_HASR_StopHandle;
	bool m_bASR_StopFlag;

	bool m_bWaitTestEvent[4];

	//等待开始测试
	HANDLE m_hWaitTestStart[4];
	bool m_bWaitTestEventStart[4];

	//通过配置文件中参数设置
	bool AddDevice_INI(int i_TSCH_Numb,int nDeviceType,CString strUSBAddress);

	//海思平台下载接口
	int Do_Execute_HUAWEI( int i_TSCH_Numb);

	//INTEL平台下载接口
	int Do_Execute_INTEL( int i_TSCH_Numb);

	//RDA平台下载接口
	int Do_Execute_RDA( int i_TSCH_Numb);

	//高通平台下载接口
	int Do_Execute_QCM( int i_TSCH_Numb);

	//MTK_NB平台下载接口
	int Do_Execute_MTK_NB( int i_TSCH_Numb);

	//MTK_GSM平台下载接口
	int Do_Execute_MTK( int i_TSCH_Numb);

	//ASR平台下载接口
	int Do_Execute_ASR( int i_TSCH_Numb);

	//EIGEN平台下载接口
	int Do_Execute_EIGEN(int i_TSCH_Numb);
	//BEKEN平台下载接口
	int Do_Execute_BEKEN(int i_TSCH_Numb);
	//UNISOC平台下载接口
	int Do_Execute_Unisoc(int i_TSCH_Numb);
	//芯翼XINYI平台下载接口
	int Do_Execute_XINYI(int i_TSCH_Numb);
	//索尼ALTAIR平台下载
	int Do_Execute_ALTAIR(int i_TSCH_Numb);
	void SetErrCode(const int &i_Index, const int& iErrCode, const CString& strErrCode);
	//ASR 错误消息的获取
	void ReportError(NotifyStatus* pNtStat);

	bool DeleteFolder(const TCHAR* pszFolderPath);

	bool DeleteEntireDir(const TCHAR * sPath,bool bDelDir = true);

	BOOL ExactZipPackage();

	CString m_sExtractPackageFile;

	CStringArray m_sFileNameArray;

	BOOL PrepareDownloadFile();

	bool BrowseDirectory(CString sFileName,CString sRootDir);

	BOOL CheckReliableDataBinFile(CString strBlfFilePath, BOOL *bEnable);

	bool SupportCurentBlfVersion(CString strBlfFilePath);

	LRESULT OnKill_SSB_EXE(WPARAM wParam, LPARAM lParam);

	//获取设置界面中的对应的夹具编号信息
	CString Get_FN_Name(int i_TSCH_Numb);

	LRESULT OnKill_SSB_EXE_MSG(WPARAM wParam, LPARAM lParam);

	void Kill_SSB_EXE(int iIndex);

	//二维码解析功能函数
	bool Extract_Useful_Data_From_Barcode(const int i_TSCH_Numb, const wchar_t * wcp_Barcode, CString * csp_IMEI_1_Key, CString * csp_IMEI_2, CString * csp_SN, CString * csp_MAC, CString * csp_MEID, CString * csp_ICCID, CString * csp_IMSI, CString * csp_BT_ADDR, CString * csp_OC,CString * csp_Landi_SN_2);


	bool Check_Barcode_IMEI_1(const int i_TSCH_Numb,const wchar_t * wcp_IMEI_1, const wchar_t * wcp_Item_Info, bool b_Write_PRI_IMEI);
	bool Check_Barcode_IMEI_2(const int i_TSCH_Numb,const wchar_t * wcp_IMEI_2, const wchar_t * wcp_Item_Info, bool b_Write_SEC_IMEI);
	bool Check_Barcode_SN(const int i_TSCH_Numb,const wchar_t * wcp_SN, const wchar_t * wcp_Item_Info);
	bool Check_Barcode_MAC(const int i_TSCH_Numb,const wchar_t * wcp_MAC, const wchar_t * wcp_Item_Info, bool b_Write_MAC);
	bool Check_Barcode_MEID(const int i_TSCH_Numb,const wchar_t * wcp_MEID, const wchar_t * wcp_Item_Info, bool b_Write_MEID);
	bool Check_Barcode_BT_Addr(const int i_TSCH_Numb,const wchar_t * wcp_BT_Addr, const wchar_t * wcp_Item_Info, bool b_Write_BT_Addr);
	bool Check_Barcode_OC(const int i_TSCH_Numb,const wchar_t * wcp_OC, const wchar_t * wcp_Item_Info);

	//20190731
	//	查询分版结果
	//	执行查询：SELECT Result FROM [IMEI].[dbo].[Qisda_Fenban_Result] where IMEI = '181663330190702'
	//	返回结果：1表OK，0或无记录表示NG
	bool CHECK_SQL_QISDA_FENBAN_RES(int i_TSCH_Numb, const wchar_t * wcp_Key_IMEI);

	//	检查模块SMT结果是否成功
	//	苏州佳世达工厂
	//	查询佳世达SMT结果
	//	执行查询：exec usp_CheckProcessForYiYuan ‘864506031257422’
	//	返回结果：0表示OK，-1表示NG
	bool CHECK_SQL_QISDA_SMT_RES(int i_TSCH_Numb, const wchar_t * wcp_Key_IMEI);




	LRESULT OnGetMsg_Get_LAN_Fixture_Code(WPARAM wParam, LPARAM lParam);

	LRESULT OnGetMsg_Get_LAN_Query_MO(WPARAM wParam, LPARAM lParam);

	void Load_LAN_TEST_INFO(void);

	//获取总控提供中的对应的夹具编号信息
	CString Get_Auto_FN_Name(int i_TSCH_Numb);


	//****************************************************
	//test
	//****************************************************
	bool CHECK_SQL_TEST_PROC(int i_TSCH_Numb, const wchar_t * wcp_Key_IMEI);

	//保存每个通道下的详细log
	void OnSaveOut1();
	void OnSaveOut2();
	void OnSaveOut3();
	void OnSaveOut4();

	void OnEXP_CONF_File();

	void OnIMP_CONF_File();

	//PLC控制reset
	void PLC_DC_REON(const int i_TSCH_Numb);

	//一拖四的PLC的下载夹具增加RDA平台的FLASH TEST加密处理
	int RDA_Flash_Test_PLC(const int  i_TSCH_Numb);

	//全自动机器人模式增加RDA平台的FLASH TEST加密处理
	int RDA_Flash_Test_AUTO_ROBOT(const int  i_TSCH_Numb);

	//全自动机器人模式增加MTK平台(非NB)的FLASH TEST加密处理
	int MTK_Flash_Test_AUTO_ROBOT(const int  i_TSCH_Numb);

	//全自动机器人模式增加MTK平台(非NB)的写号处理
	int MTK_Write_IMEI_AUTO_ROBOT(const int  i_TSCH_Numb);

	//全自动机器人模式增加INTER平台的FLASH TEST加密处理(单口)
	int INTER_Flash_Test_AUTO_ROBOT(const int  i_TSCH_Numb);

	//海思平台参数清空
	void Huawei_Parameter_Clear(const int  i_TSCH_Numb);

	//全自动机器人模式增加海思平台的写号处理
	int HUAWEI_Write_IMEI_AUTO_ROBOT(const int  i_TSCH_Numb);

	bool Check_Barcode_Landi_SN_2(const int i_TSCH_Numb,const wchar_t * wcp_Landi_SN_2, const wchar_t * wcp_Item_Info, bool b_Produ_Landi);

	//检测端口有没有为空的场景
	int Check_Port_Null(const int i_TSCH_Numb);

	//下载前的延时和上下电reset
	bool DL_Prepare( const int i_TSCH_Numb);

	//检查IMEI在数据库中的数据以及设置下载前的网络标志位
	int Check_Set_SQL_IMEI( const int  i_TSCH_Numb);

	//下载结果上传SQL数据库
	int Upload_DL_Status_Flag(const int i_TSCH_Numb, const int iLast_Rtn);

	//MTK NB 海尔定制项目HAIERSTOP 9600波特率
	void MTK_NB_OPEN_PORT_AFTER_DL_HR(const int   i_TSCH_Numb,const int  iLastRtn);

	//获取工厂名称
	CString GetFactoryName();

	BOOL m_cla_Flash_GK_LoadInit;

	void  Init_Load_Manual();
	void  Init_Load_Auto_Robot();
	void  Init_Load_Total_Four();
	void  Init_LOAD_4_AUTO_DL_Vison();
	void  Init_Load_4_AUTO_DL();
	//识别当前指定的是Firehose还是Sahara协议
	void  Identify_DL_Method();
	void  GetSQL_DB_Data();

	//比较数据中工单的时间有没有超过2019年10月1日
	int TimeCmpMo(CString strTimeSrc);

	//全自动机器人模式增加MTK平台NB)的FLASH TEST加密处理
	//
	int MTK_Flash_Test_AUTO_ROBOT_NB(const int  i_TSCH_Numb);

	CString m_strFactory;
	CString m_strFTPdir;//从工厂信息表中获取工厂名称和FTP路径

	//20191024 检查夹具编号是否合法，端口是否下载
	void SetPortDL_Enable(void);

	//检测RDA平台模块是否开机
	int RDA_Check_Module_Power_ON( int i_TSCH_Numb);

	void  Init_Load_Auto_Robot_SPC();

	//每片模块下载前都必须给每个通道的程控电源上电
	void  Init_Power(int iIndex);

	//每片模块下载前都必须给每个通道的程控电源断电并释放资源
	void  Del_Power(int iIndex);

	//PLC控制 断电
	void PLC_DC_OFF(const int i_TSCH_Numb);

	bool Create_Fw_Folder(int i_TSCH_Numb, int  y ,CString strMo, CString strRes, CString strFileName,CString strSrcFile);

	BOOL ASR_Get_BB_Chip();

	CStringArray m_csaPortInfo;  //端口

	CStringArray m_csaPortInfo_Path;  //端口path

	BOOL fnGetPortInfo();

	int m_iDisableGroupNum;
	int m_iDisableGroupName[15];

	BOOL  m_bDisableGroupInit;

	BOOL GetEraseUserDataGroupInfo(const char* partitionName1, const char* partitionName2, const char* partitionName3, int &iDisableGroupNum, int* iDisableGroupName);

	BOOL system_hide(TCHAR* CommandLine, char* ReturnValue, int timeout, bool timeshot);

	void ConvertTCHARToChar(TCHAR* tcIn, char* cOut, int nOutLength);

	BOOL  m_bASR_DownLoadInit1;
	BOOL  m_bASR_DownLoadInit2;
	BOOL  m_bASR_DownLoadInit3;
	BOOL  m_bASR_DownLoadInit4;


	void Set_Progress_Entire(int iIndex, int iPos);

	bool  m_b_aboot_engine_init_flag;

	void Out_Show_Status(CString strParam,int Index);//直接到控件上显示
	void Msg_Show_Status(CString strParam,int  Index);

	CString  m_str_ASR3601_FW;
	CString  m_str_ASR3601_FW_Last;//用以记录上一次的固件包全路径

	void  aboot_download_init_Ext(void);

	bool  m_ASR_DL_OK1;
	bool  m_ASR_DL_OK2;
	bool  m_ASR_DL_OK3;
	bool  m_ASR_DL_OK4;

	CEdit m_ctrl_FX_1;
	CEdit m_ctrl_FX_2;
	CEdit m_ctrl_FX_3;
	CEdit m_ctrl_FX_4;
	CEdit m_ctrl_FX_5;
	CEdit m_ctrl_FX_6;
	CEdit m_ctrl_FX_7;
	CEdit m_ctrl_FX_8;

	//
	CString m_csMO_Create_Year;
	bool IsNeedFlashTest(void);//BC32\BC92部分机型需要自动线做Flash Test

	bool m_bNeedFlashTest;
	BOOL m_bSaharaOnly;//界面菜单中设置的SaharaOnly参数

	BOOL m_bRdaReboot;

	int  m_iBaud_Rate;

	//工具初始化时候检测每个通道的程控电源是否OK
	int Init_Power_Check_All(void);

	CString  m_strPro_Name;

	//PLC控制on
	void PLC_DC_ON(const int i_TSCH_Numb);

	void Display_FN_Params(void);
    //UNISOC平台
	CMap<int,int,PORT_DATA,PORT_DATA> m_PortMap; // UNISOC平台COM口通道信息
	IFlashCtrlHandler* m_pDlCctrl;
	IUIInterface m_cbUIInterface;
	static TCHAR m_szSATES[MAX_PATH];
	static HWND m_hwndUI;
	static void __stdcall Progress_Update(UINT nProgress, UINT nPosID, LPVOID pUserData);
	static void __stdcall Text_Output(LPCTSTR lpszStr, COLORREF clr, UINT nPosID, EUI_TEXT_ID eUITextID, LPVOID pUserData);
	void SetUnisocRunningState(int iCh, bool bState);

	//保存摘要日志LOG
	int SaveLog_Abstract(const int i_TSCH_Numb,const int  iTS_Numb,bool  bDL_Result, CString csError_Code, CString strPASS,CString  strFAIL,CString  strFPY);

	//保存测试日志的log
	bool SaveLog_Test(const int i_TSCH_Numb,const int  iTS_Numb,bool  bDL_Result, CString csError_Code, CString strPASS,CString  strFAIL,CString  strFPY);

	//检查MOB_SN在数据库中的数据以及设置下载前的网络标志位
	int Check_Set_SQL_MOB_SN( const int  i_TSCH_Numb);

	bool CHECK_SQL_MOB_SN_INFO_V1(int i_TSCH_Numb, const wchar_t * wcp_Key_IMEI);

	//下载结果上传SQL数据库--MOB
	int Upload_DL_Status_Flag_MOB(const int i_TSCH_Numb, const int iLast_Rtn);

	//更新网络标志位 MOB
	bool Update_SQL_MOB_Status(int i_TSCH_Numb, const wchar_t * wcp_Key_MobSn);

	void Set_View_Ctrl_Font_Ext(void);


	bool   m_bUnlockFlag;
	CButton m_ctrlDL_UnLock;

	BOOL ExactZipPackage_Auto(const CString   strLocalFw,const CString   strLocalFwPath);

	HWND m_hWnd_log_View;
	CRichEditCtrl m_ctrl_LocFW;

	void OnWrite_Flash(void);

	void OnDL_Unlock(void);//工厂下载授权

	void OnBn_M_LOAD_FW();
	void OnM_LOAD_FW(void);//手动加载固件授权

	bool Search_First_Product_SQL_DB(int i_TSCH_Numb,const wchar_t * wcp_Key_IMEI);

	bool m_bFirst_Product;//判断首件标志

	bool m_iNotNeedFenBanFlag;//判断是否需要卡控分板不良标志

	void  Open_Power_SPC(int iIndex);

	int  m_iHS_DL_MODE;

	//全自动机器人-程控电源-模式增加海思平台的写号处理
	int HUAWEI_Write_IMEI_AUTO_SPC(const int  i_TSCH_Numb);

	//全自动机器人-程控电源模式增加RDA平台的FLASH TEST加密处理
	int RDA_Flash_Test_AUTO_SPC(const int  i_TSCH_Numb);

	//2020518 获取ASR的原始的log
	void Save_Org_Log(const wchar_t * wcp_Data);

	void Close_Org_Log_Handle(void);

	void aboot_download_init_Ext_2(void);

	//3601平台 再次加载固件需要将原先的删掉
	BOOL ASR_Get_New_FW_ZIP();

	//3601平台将原先Bin文件夹下的ZIP临时文件删掉
	BOOL ASR_Delete_Bin_ZIP();

    //20200522
	void SetOut1Clear();

	//ASR 3601 平台判断下载的结果状态
	void ASR_judgeDownloadStatus(const CString& csJson,int iPortIndex);
	BOOL EditRegistry(IN HKEY hKey,IN LPCWSTR lpSubKey,IN REGSAM RegAccess,IN LPCWSTR lpValueName);
	//一拖四的PLC的下载夹具增加MTK平台的FLASH TEST加密处理
	int MTK_Flash_Test_PLC(const int  i_TSCH_Numb);
	//程控电源SPC控制reset
	void SPC_DC_REON(const int i_TSCH_Numb);
	//自动化线PLC控制reset
	void AUTO_PLC_DC_REON(const int i_TSCH_Numb);
	//设置是否支持Firehose 属性
	void  Set_DB_Firhose_Flag();
	bool Check_SQL_Tool_Version(const CString cs_MO_Name, CString & oj_cs_Tool_Version);
	void  Close_Power_SPC(int iIndex);
	//全自动合站模式增加MTK平台的FLASH TEST加密处理
	//
	int MTK_Flash_Test_AUTO_SPC(const int  i_TSCH_Numb);
	void  Open_Power_SPC_Ext(int iIndex);
	//程控电源SPC控制ON
	void SPC_DC_ON(const int i_TSCH_Numb);
	//每片模块下载前都必须给每个通道的程控电源下电
	void Init_Power_OFF(int iIndex);
	//工具初始化时候检测程控电源是否OK
	int  Init_Power_SCAN_BCODE_Check_All(void);
	//就一个程控电源的场景
	//程控电源SPC控制ON
	void SPC_DC_ON_OnlyOne(void);
	void SPC_DC_REON_OnlyOne(void);
	//单个程控电源
	void  Init_Power_OFF_OnlyOne(void);
	//20200905 EM120和EM160 7个OC的EID需要将取出写入到数据库中
	bool Update_SQL_IMEI_EID(int i_TSCH_Numb, const wchar_t * wcp_Key_IMEI);
	bool Update_User_Ver_2017(int i_TSCH_Numb, const wchar_t * wcp_Key_IMEI, const wchar_t * wcp_UserVer);
	bool Get_SQL_ATT_Custom_Test_Flow(const CString cs_MO_Name,CString & oj_cs_ATT_Custom_Test_Flow);
// 
 	bool Analysis_Custom_Test_Flow ( const CString csCustomTestFlow , std::vector<CUSTOM_TEST_FLOW_S> &vtCustomTestFlow );
	bool Check_CustomTestFlow(const CString csTool_Name1, const CString csTool_Name2);
	//上传自定义流程的网络标志位到SQL数据库
	int Upload_DL_Status_Flag_CustomTestFlow(const int i_TSCH_Numb, const int iLast_Rtn);
	bool Update_NetFlag_2017_CustomTestFlow(int i_TSCH_Numb, const wchar_t * wcp_Key_IMEI, const wchar_t * wcp_Write_Status);

	//刷新设备管理器
	bool Devcon_Rescan_Device();
#ifdef FW_MULTI
#ifndef FW_UPGRADE
	CPipeCommObj00 m_PipeComm;			//管道类	
#endif
#endif
	bool Devcon_Check_PCIE_Device_Detect();
	bool Confirm_NO_PCIE_Port(int i_TimeOut);
	bool _W_PCIE_Rescan(int i_TimeOut);
	//下载前上传自定义流程的前一个网络标志位到SQL数据库
	bool Upload_DL_Status_Flag_CustomTestFlow_Before_DL(const int i_TSCH_Numb);
	//
	bool Load_All_DL_Files_MTK_5G(void);
	void Load_View_Bin_List_MTK_5G(void);
	//MTK_5G平台下载接口
	int Do_Execute_MTK_5G(int i_TSCH_Numb);
	void QDM01_Pow_Ctrl_OFF(const int i_TSCH_Numb);
	bool GetOcpVal(const CString OC,const bool bSecond);
	//一拖四的PLC的下载夹具增加高通平台的PCIE EFUSE
	int QUALCOMM_PCIE_EFUSE_PLC(const int  i_TSCH_Numb);
	//打开其中一路PLC的5V电源
	BOOL OpenPLC_Power_Path_5V(int Index);
	//关闭其中一路PLC的5V电源
	BOOL ClosePLC_Power_Path_5V(int Index);
	bool Read_PLC_Rsp_Ext(int * ip_Ch, int * ip_Rsp_Code, int i_TimeOut);
	void GET_PORT_RX_BUFF_Ext(char ** cp2_Buff_Pointer, unsigned long * ulp_Data_Length);
	//
	bool Load_All_DL_Files_MTK_AG(void);
	void Load_View_Bin_List_MTK_AG(void);
	//MTK_AG平台下载接口
	int Do_Execute_MTK_AG(int i_TSCH_Numb);
	void FindAT_Switch_DL(int index);
	void OnMenu_ODM_SPC_NOT_DL(void);//飞天QDC506加密部分固件不下载授权
	void OnBn_ODM_SPC_NOT_DL();

	void ASR_Enable_Device_1(void);
	void ASR_Enable_Device_2(void);
	void ASR_Enable_Device_3(void);
	void ASR_Enable_Device_4(void);

	CString m_strPortTracePath;
	CString m_csCustomTestFlow;
	CString m_strCustomTestFlow_LastStatus;

	std::vector<CUSTOM_TEST_FLOW_S> m_vtCustom_Test_Flow;
	CUSTOM_TEST_FLOW_FLAG_S m_stu_CustomTestFlow_Flag;

	bool m_bAG_Flag;//AG机型标志
	bool m_bQDM002_flag;
	bool m_bTestFlow;//自定义流程标志
	bool m_bTestFlowFind;
	bool m_bAtFallback;
	bool m_bReboot;
	bool m_bAotoReboot;
	bool m_bAT_UseFlag;
	bool m_bAT_UseFlag_QDM;
	bool m_bHoneywell_A4_flag;

	int m_iAT_Port1;
	int m_iAT_Port2;
	int m_iAT_Port3;
	int m_iAT_Port4;


	CBrush   m_bkBrush; //背景刷
	CStatic m_ctrl_L1;
	CStatic m_ctrl_L2;
	CStatic m_ctrl_L3;
	CStatic m_ctrl_L4;
	CStatic m_ctrl_L5;
	CStatic m_ctrl_L6;
	CStatic m_ctrl_L7;
	CStatic m_ctrl_L8;
	

	HBRUSH m_brBrush1;
	HBRUSH m_brBrush2;
	HBRUSH m_brBrush3;
	HBRUSH m_brBrush4;
	HBRUSH m_brBrush5;
	HBRUSH m_brBrush6;
	HBRUSH m_brBrush7;
	HBRUSH m_brBrush8;

	CString m_strEID[11];//
	CString m_strQW_MAC[11];
	vector<CString> m_Total_Info_V;
	vector<CString> m_Module_Info_V;

	//PipeCom Init
	bool Init_PipeComm(int iTimeOut);
	//_Terminate_Process
	bool Terminate_PipComm(int iTimeOut);

	//---------------------------------------mount使用的5条指令-----------------------------------------
	bool Mount_Persist();

	int Do_Execute_QDM512_MCU(int i_TSCH_Numb);
	int Do_Execute_ZG100S_MCU(int i_TSCH_Numb);

	bool Find_Create_Fw_Folder_ASR(const wchar_t * wcp_FW_File_Path, CString strFolderName);
	//自动化线PLC控制延时上电
	void AUTO_PLC_DC_ON(const int i_TSCH_Numb);

	bool Upload_QDE007_PCBA_DATA(int i_TSCH_Numb, int Index, const wchar_t * wcp_Key_IMEI, const wchar_t * wcp_Specified_Item, const wchar_t * wcp_New_Data);

	bool Update_MCU_Ver_2017(int i_TSCH_Numb, const wchar_t * wcp_Key_IMEI, const wchar_t * wcp_UserVer);
	int Do_Execute_MCU(int i_TSCH_Numb, CString str_FolderName);
	bool Read_Server_Config_Fixed_Parameter();
	bool Read_Server_Config_Ext_Fixed_Parameter();
	bool Read_Server_Config_Ext_Fixed_Parameter_OldDB();
	int iFTP_Debug_Switch;
	void TraversFolderFile_ESP32(CString str_TargetPath, CString str_FileMark, CString & strFilePath);
	void TraversFolderFile(CString str_TargetPath, CString str_FileMark, CString & strFilePath);
	int QDE017_Meas_Curr(int i_TSCH_Numb, bool bInitNormalPower, double dPower_Voltage, double dLower_Limit, double dUpper_Limit, CString & strCurr);

	CString GetExeFilePath();
	bool Upload_FTP_Daily_Record_Log_File(CString strTargetFolder, CString wcp_Log_File_Path, int i_TSCH_Numb);
	bool WriteCurrTxt(CString strLocalPath, CString strFileName, CString str_Nofw_Curr, CString str_5V_Curr, CString str_8V_Curr);
	void GetTimeStr(CString* strTime);
	int iTraversFolderFileCount;
	void TraversFolderFile_All(CString str_TargetPath, CString str_FileMark, CString & strFilePath);
	CString str_TraversFolderFile_AllFile;
	// 数据库2019表里存放Wifi MAC的R3
	CString str_DB_R3_MAC;
	CString m_strMCU_Ver2;
};
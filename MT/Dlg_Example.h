#pragma once
#include "Gen_Header.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "inifile.h"
#include "flash_op.h"
#include "arm_op_qcm.h"
#include "arm_op_rda.h"
#include "arm_op_intel.h"
#include "afx.h"
#include "ARM_OP_HUAWEI.h"
#include "ARM_OP_ASR.h"
#include "ARM_OP_BEKEN.h"

#include "Flash_OP_NB.h"
#include "Flash_OP_GK.h"
#include "ARM_OP_EIGEN.h"
#include "Flash_OP_5G.h"

#include "ARM_OP_UNISOC.h"
#include "UFlashTool\include\IFlashTool.h"

#include "IFL_SWD\IFL_SWDAPI.h"
#include "WtptpDownLoad\ParaDefine.h"
#include "AppDefine.h"
#include "WtptpDownLoad\WtptpDownLoad.h"

#include "ActivateMbnOpName.h"





#define IFLSWDDLL_NAME            _T("IFL_SWD.dll")
#define WTPTPDOWNLOADDLL_NAME     _T("WtptpDownLoad.dll")

// CDlg_Example dialog

class CDlg_Example : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Example)

public:
	CDlg_Example(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Example();

// Dialog Data
	enum { IDD = IDD_DIALOG_EXAMPLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int m_iQFLASH_CMD;
	int m_iCom_Main_Idx;
	CComboBox m_ctr_cbxCom_Main;
	int m_iBR_Main_Idx;
	CComboBox m_ctr_cbxBR_Main;
	CString m_csScat_Cfg_File_Path;
	CString m_csDA_File_Path;
	CButton m_ctr_btnSet_Scat_Cfg_File;
	CButton m_ctr_btnStart;
	CButton m_ctr_btnStop;
	CListCtrl m_ctr_lscBin_List;
	CIniFile m_cla_iniFile;
	void Read_Config(void);
	void Save_Config(void);
	afx_msg void OnCbnSelchangeComboComMain();
	void Update_Com_Ctrl_Option();
	afx_msg void OnCbnSelchangeComboBrMain();
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedListBinList(NMHDR *pNMHDR, LRESULT *pResult);
	CFlash_OP m_cla_Flash_OP_Main;
	afx_msg void OnBnClickedButtonSetScatFile();
	void Add_ListCtrl_Bin_List_MTK(const ROM_INFO_EX str_Rom_Info_Ex);
	void Load_View_Bin_List_MTK(void);
	bool Load_All_DL_Files_MTK(void);
	CEdit m_ctr_edtStatus_Main;
	CEdit m_ctr_edtStatus_Main2;
	LRESULT OnGetMsg_Show_Status(WPARAM wParam, LPARAM lParam);
	void Show_Status(const wchar_t * wcpStatus);

	LRESULT OnGetMsg_Show_Dos(WPARAM wParam, LPARAM lParam);
	void Show_Dos(const wchar_t * wcpStatus);

	LRESULT OnGetMsg_Show_Status2(WPARAM wParam, LPARAM lParam);
	void Show_Status2(const wchar_t * wcpStatus);

	void Load_Flash_Option_Params(void);
	afx_msg void OnBnClickedButtonStart();
	CIniFile m_cla_iniParams;
	void Load_Flash_Product_Params(const wchar_t * wcp_Product);
	HANDLE m_hThread_Handle;
	DWORD m_dwThread_ID;

	HANDLE m_hThread_Erase_Handle;
	DWORD m_dwThread_Erase_ID;

	void Set_View_Ctrl_State(int i_State);
	afx_msg void OnBnClickedButtonStop();
	CString m_csCurr_Exe_Path;
	int m_iNv_Type;
	LRESULT OnGetMsg_Show_Progress(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Show_Progress_Entire(WPARAM wParam, LPARAM lParam);
	CProgressCtrl m_ctr_pgrProgress_Main;
	void Reset_Progress_Ctrl(void);
	void Init_Progress_Ctrl(int i_Platform, int i_OP_TYPE);
	CFont m_cla_fontBtn_1;
	CFont m_cla_fontEdt_1;
	CFont m_cla_fontEdt_2;
	void Set_View_Ctrl_Font(void);
	int m_iPgr_Last;
	void Clear_View_Bin_List(void);
	bool m_bInside_Flash;
	CEdit m_ctr_edtTime_Main;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	COLORREF m_colorEditText;
	afx_msg void OnDestroy();
	LRESULT OnGetMsg_Ctrl_Colour(WPARAM wParam, LPARAM lParam);
	void Ctrl_Colour(int i_Colour);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	time_t m_time_Start_1;
	void Show_Time(const wchar_t * wcp_Time);
	void Reset_View(void);
	bool m_bLoad_Bin_List;
	int m_iPlatform_Curr;
	int m_iBBchip_Curr;//增加芯片类型参数
	

	bool Get_Platform(const wchar_t * wcp_FW_File_Path, int * ip_Platform);
	bool Load_FW_Files(void);
	bool Load_All_DL_Files_QCMM(void);
	void Add_ListCtrl_Bin_List_Data(const wchar_t * wcp_Data_1, const wchar_t * wcp_Data_2, bool b_Check);
	afx_msg void OnCbnDropdownComboComMain();
	CARM_OP_QCM m_cla_ARM_OP_QCM_Main;
	void Load_View_Bin_List_QCMM(void);
	void DO_DL_OP(void);
	void Start_Timer(int i_Timer_Number);
	void Load_Params_QCMM(const wchar_t * wcp_Product);
	LRESULT OnGetMsg_DL_Info(WPARAM wParam, LPARAM lParam);
	void INIT(void);
	CStatic m_ctr_staConfig_Adv;
	int m_iCtrl_Dl_Type_S;
	CComboBox m_ctr_cbxCtrl_Dl_Type_S;
	int m_iCtrl_OP_Option_S;
	CComboBox m_ctr_cbxCtrl_OP_Option_S;
	BOOL m_bGet_Debug_Log;
	CButton m_ctr_cekGet_Debug_Log;
	CStatic m_ctr_staCtrl_Dl_Type_S;
	CStatic m_ctr_staCtrl_OP_Option_S;
	bool Load_Advanced_Config_MTK(const wchar_t * wcp_Product);
	bool Load_Advanced_Config_QCMM();
	bool m_bEnable_Advanced_Option;
	bool m_bEnable_Advanced_Option_1;
	bool  m_bEnable_Com_Method;

	LRESULT OnGetMsg_Hot_Key(WPARAM wParam , LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	LRESULT OnGetMsg_Hot_Key_DLoad(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Hot_Key_Option(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Hot_Key_Option_1(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Hot_Key_Option_Method(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Hot_Key_Get_Debug_Log(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Hot_Key_Firehose(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Hot_Key_NB_BC95_PARA(WPARAM wParam, LPARAM lParam);
	void Load_View_Option(int i_Platform);
	afx_msg void OnCbnSelchangeComboCtrlDlTypeS();
	afx_msg void OnCbnSelchangeComboCtrlOpOptionS();
	afx_msg void OnBnClickedCheckGetDebugLog();
	CString m_csQCN_Temp_File_Path;
	bool m_bEnable_Emerg_DLoad;
	bool m_bEnable_Get_Debug_Log;
	void Get_Advanced_Config_Value(int i_Platform, const wchar_t * wcp_Product);
	CComboBox m_ctr_cbxCtrl_DLoad_Port_S;
	int m_iCtrl_DLoad_Port_S;
	CStatic m_ctr_staCtrl_DLoad_Port_S;
	LRESULT OnGetMsg_Hot_Key_DLoad_Port(WPARAM wParam, LPARAM lParam);
	bool m_bEnable_DLoad_Port;
	afx_msg void OnCbnSelchangeComboCtrlDlPortS();
	LRESULT OnGetMsg_Hot_Key_Update_Gen(WPARAM wParam, LPARAM lParam);
	CARM_OP_RDA m_cla_ARM_OP_RDA_Main;
	bool Load_All_DL_Files_RDA(void);
	void Load_View_Bin_List_RDA(void);
	bool Load_Advanced_Config_RDA(void);
	CARM_OP_INTEL m_cla_ARM_OP_INTEL_Main;
	bool Load_All_DL_Files_Intel(void);
	void Load_View_Bin_List_Intel(void);
	bool Load_Advanced_Config_Intel(void);
	LRESULT OnGetMsg_Trace_Log_Msg(WPARAM wParam, LPARAM lParam);
	void Save_Trace_Log_File(const wchar_t * wcp_Data);
	void Close_Trace_Log_File(void);
	int m_iDebug_Log_Handle;
	CStdioFile m_cla_Debug_Log_Stdio_File;
	CString m_csDebug_Log_File_Path;
	void Load_Params_Intel(const wchar_t * wcp_Product);
	int m_iOCPU_Name_S;
	int m_iComMethod;
	int m_iResetFlag;
	CComboBox m_ctr_cbxOCPU_Name_S;
	afx_msg void OnCbnSelchangeComboOcpuModelS();
	CStatic m_ctr_staOCPU_Name_S;
	void Load_View_Static_Value(void);

	bool Load_Advanced_Config_HUAWEI(void);
	CARM_OP_HUAWEI m_cla_ARM_OP_HUAWEI_Main;
	bool Load_All_DL_Files_HUAWEI(void);

	bool Load_All_DL_Files_EIGEN(void);
	CFlash_OP_NB m_cla_ARM_OP_NB_Main;

	CARM_OP_ASR m_cla_ARM_OP_ASR_Main;

	CFlash_OP_5G m_cla_ARM_OP_5G_Main;

	CFlash_OP_GK m_cla_ARM_OP_GK_Main;//BC39-GK gps
	CARM_OP_EIGEN m_cla_ARM_OP_EIGEN_Main; // BC260
	CARM_OP_BEKEN m_cla_ARM_OP_BEKEN_Main;
	CARM_OP_UNISOC m_cla_ARM_OP_SPREADTRUM_Main;

	bool Load_All_DL_Files_ASR(void);
	bool Load_Advanced_Config_ASR(void);

	int iCmdComPort;
	int iCmdBd;
	CString strCmdPath;
	CGFC m_cla_GFC;

	BOOL m_bFirehsoeFlag;
	BOOL m_bNB_BC95_Flag;

	CButton m_ctrlFirehoseReset;
	BOOL m_bFirehoseReset;
	CButton m_ctrlSaharaDL;
	BOOL m_bSaharaOnly;//界面菜单中设置的SaharaOnly参数
	afx_msg void OnBnClickedFirehoseReset();
	afx_msg void OnBnClickedSaharaDl();
	CButton m_ctrlNoBaudSwitch;
	BOOL m_bNoBaudSwitch;
	afx_msg void OnBnClickednobaudswitch();
	CButton m_ctrlSSB_Update;
	BOOL m_bSSB_Update;
	CButton m_ctrlWrCon;
	BOOL m_blWrCon;
	BOOL m_bWriteKV;

	int  m_iHS_DL_MODE;

	afx_msg void OnBnClickedWrCon();
	afx_msg void OnBnClickedSsbUp();
	afx_msg void OnBnClickedWriteKV();

	bool Load_All_DL_Files_MTK_NB(void);

	
	bool Load_Advanced_Config_MTK_NB(void);

	bool FindBinFile(CString lpPath);
	afx_msg void OnBnClickedErase();
	CButton m_ctrlErase;
	void DO_Erase_OP(void);
	afx_msg void OnBnClickedRVersion();
	afx_msg void OnBnClickedRImei();
	afx_msg void OnBnClickedWImei();
	CEdit m_ctrlEditImei;
	CString m_strImei;
	//通过查找Firmware同目录下是否有BootLoader_XXXXX.bin文件来区分是APP还是STD
	bool FindBootLoaderFile(CString lpPath);



	//RSA
	afx_msg LRESULT OnCallbackFunc_RSA(WPARAM wParam, LPARAM lParam);

	void AddDevice(NotifyStatus* pNtStat);
	BOOL AddDevice(int iUSBPort,int nDeviceType,CString sUSBAddress);
	BOOL SetProgress(NotifyStatus* pNtStat);
	LRESULT OnProgressToView(LPARAM lParam);

	CString  m_strUSBPortAddress[4];
	int m_iUSBPortUseIndex;

	CMapStringToPtr   m_IndexProgressMap;
	void Show_Progress(int i_Pgr_Percent);
	void Show_Progress_ENTIRE(int i_Pgr_Percent);

	BOOL              m_bIsInit;
	PInstanceParams  m_lPInstanceParams;
	HMODULE         m_gSWDhModule;
	CString          m_sExePath;   
	CString          m_sConfigFile;
	CString          m_sUserInfo;
	bool             m_bIsAdminAuthorized;
	ESWDMode		 m_eSWDMode;
	TCHAR m_szBlfFilename[MAX_FILE_LENGTH];
	TCHAR m_szFlasherFileName[MAX_FILE_LENGTH];
	TCHAR m_szFBFFilename[MAX_FILE_LENGTH];

	bool m_bIsStart;
	bool m_bInitializeBL;
	bool m_bIsPackageReady;
	HMODULE m_gWtptphModule;
	PInstanceParams GetInstanceParam();
	HMODULE GetModule();

	int RSA_DL_Run();
	bool StopDownload();
	ESWDMode GetSWDMode(){return m_eSWDMode;}

	int m_irDL_OBJ_State;//0=未激活-未选中-不使用的端口；1=被选中-未开始-无模块；2=下载中-运行中；3=下载pass；4=下载fail；
	bool Get_File_Size(const wchar_t * wcp_Path, unsigned long * ulp_Size);

	bool DeleteFolder(const TCHAR* pszFolderPath);

	bool DeleteEntireDir(const TCHAR * sPath,bool bDelDir=true);

	BOOL ExactZipPackage();

	bool BrowseDirectory(CString sFileName,CString sRootDir);

	bool SupportCurentBlfVersion(CString strBlfFilePath);

	BOOL PrepareDownloadFile();

	BOOL CheckReliableDataBinFile(CString strBlfFilePath, BOOL *bEnable);


	CString m_sExtractPackageFile;

	CStringArray m_sFileNameArray;

	//
	HANDLE m_hWaitTestFin_Main;

	bool m_bWaitTestEvent_Main;

	void ReportError(NotifyStatus* pNtStat);

	CButton m_ctrlWKV;
	CComboBox m_ctr_ComMethod;
	afx_msg void OnCbnSelchangeComboCommMethod();
	CStatic m_ctrl_Dl_Type;
	CButton m_ctrlFEC;
	BOOL m_bWriteFEC;
	afx_msg void OnBnClickedFec();
	CButton m_CtrlClearNv;
	BOOL m_bClearNv;
	afx_msg void OnBnClickedClearnv();

	int  m_iClearNVFlag;

	BOOL m_bDisableGroupInit;
	int m_iDisableGroupNum;
	int m_iDisableGroupName[15];

	BOOL ASR_Get_BB_Chip();

	CString  m_str_ASR3601_FW;

	BOOL GetEraseUserDataGroupInfo(const char* partitionName1, const char* partitionName2, const char* partitionName3, int &iDisableGroupNum, int* iDisableGroupName);

	BOOL system_hide(TCHAR* CommandLine, char* ReturnValue, int timeout, bool timeshot);

	void  aboot_download_init_Ext(void);

	bool  m_b_aboot_engine_init_flag;

	CString m_strUSBPortPath;

	bool  m_ASR_DL_OK;

	bool Copy_FW_Bin();

	BOOL EditRegistry(IN HKEY hKey,IN LPCWSTR lpSubKey,IN REGSAM RegAccess,IN LPCWSTR lpValueName);


	int m_iAT_Port;
	afx_msg void OnBnClickedSetDlMode();

	bool Confirm_Dynamic_Com_Port(const int i_Tgt_Com_Port, int i_TimeOut);

	HANDLE m_hLenovoHandle;


	bool m_bHoneywell_A4_flag;
	bool m_bQDM002_flag;

	//UNISOC平台
	CMap<int,int,PORT_DATA,PORT_DATA> m_PortMap; // UNISOC平台COM口通道信息
	IFlashCtrlHandler* m_pDlCctrl;
	IUIInterface m_cbUIInterface;
	static TCHAR m_szSATES[MAX_PATH];
	static HWND m_hwndUI;
	static void __stdcall Progress_Update(UINT nProgress, UINT nPosID, LPVOID pUserData);
	static void __stdcall Text_Output(LPCTSTR lpszStr, COLORREF clr, UINT nPosID, EUI_TEXT_ID eUITextID, LPVOID pUserData);
	void SetUnisocRunningState(int iCh, bool bState);

	//UNISOC平台下载
	int Do_Execute_Unisoc(void);
	bool Load_All_DL_Files_UNISOC(void);
	bool Load_All_DL_Files_BEKEN(void);
	LRESULT OnGetMsg_Unisoc_Show_Status(WPARAM wParam, LPARAM lParam);

	//
	bool Load_All_DL_Files_MTK_5G(void);
};

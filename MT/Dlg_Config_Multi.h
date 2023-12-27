#pragma once
#include "afxwin.h"
#include "inifile.h"
#include "gfc.h"


// CDlg_Config_Multi dialog

class CDlg_Config_Multi : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Config_Multi)

public:
	CDlg_Config_Multi(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Config_Multi();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONFIG_MULTI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_iOP_Robot_Multi;
	CComboBox m_ctr_cbxRobot_COM;
	int m_iRobot_COM_Idx;

	CComboBox m_ctr_FLASH_TYPE;
	int m_iFLASH_TYPE_Idx;

	int m_iRobot_BR_Idx;
	CIniFile m_cla_iniFile;
	void Read_Setting(void);
	void Save_Setting(void);
	afx_msg void OnCbnDropdownComboRobotComMulti();
	void Delete_Com_Ctrl_Option(CComboBox * clap_cbxCom_Port);
	void Load_Com_Ctrl_Option(CComboBox * clap_cbxCom_Port, int * ip_Com_Port_Idx);
	CGFC m_cla_GFC;
	void Update_Com_Ctrl_Option(void);
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CComboBox m_ctr_cbxRobot_BR;
	int m_irCom_Port_List[64];
	int m_iCom_Port_Quan;
	BOOL m_bShow_Auto_Log;
	CString m_csMO_Name;
	CString m_csMO_Name_Old;
	CEdit m_ctr_edtMO_Name;
	void Read_Scan_Cfg(int i_Index, CString * csp_Scan_IP, int * ip_Scan_Port);
	void Read_Scan_Angle(int i_Index, int * ip_Angle);
	BOOL m_bEnable_Conn_Sql_Server;
	CString m_csSQL_Server_Name;
	CEdit m_ctr_edtSQL_Server_Name;
	int m_iTS_FIX_Option_Idx_1;//1~16测试台
	int m_iTS_FIX_Option_Idx_2;
	int m_iTS_FIX_Option_Idx_3;
	int m_iTS_FIX_Option_Idx_4;
	int m_iTS_FIX_Option_Idx_5;
	int m_iTS_FIX_Option_Idx_6;
	int m_iTS_FIX_Option_Idx_7;
	int m_iTS_FIX_Option_Idx_8;
	int m_iTS_FIX_Option_Idx_9;
	int m_iTS_FIX_Option_Idx_10;
	CComboBox m_ctr_edtTS_FIX_Option_1;
	CComboBox m_ctr_edtTS_FIX_Option_2;
	CComboBox m_ctr_edtTS_FIX_Option_3;
	CComboBox m_ctr_edtTS_FIX_Option_4;
	CComboBox m_ctr_edtTS_FIX_Option_5;
	CComboBox m_ctr_edtTS_FIX_Option_6;
	CComboBox m_ctr_edtTS_FIX_Option_7;
	CComboBox m_ctr_edtTS_FIX_Option_8;
	CComboBox m_ctr_edtTS_FIX_Option_9;
	CComboBox m_ctr_edtTS_FIX_Option_10;
	CString m_csTS_LAN_IP;
	CString m_csMain_Ctrl_PC_LAN_IP;

	CString m_csServerName_Main;
	CString m_csDatabaseName_Main;
	CString m_csUserName_Main;
	CString m_csPassword_Main;


	CString m_csServerName_Abstract;
	CString m_csDatabaseName_Abstract;
	CString m_csUserName_Abstract;
	CString m_csPassword_Abstract;

	CString m_csFTP_LOG_Server;
	CString m_csFTP_LOG_User_Name;
	CString m_csFTP_LOG_Password;
	int m_iFTP_LOG_Port;
	int m_iSecPort1;
	int m_iSecPort2;
	int m_iSecPort3;
	int m_iSecPort4;

	int m_iAT_Port1;
	int m_iAT_Port2;
	int m_iAT_Port3;
	int m_iAT_Port4;


	CButton m_CtrlPowFlag;
	BOOL m_bPowFlag;
	BOOL m_bFirehose_Reset_after_DL;//Firehsoe下载完是否需要Reset

	BOOL m_bDL_Provision;
	BOOL m_bPCIE_DL;

	CButton m_ctrlSSB_BIN;
	BOOL m_bSSB_BIN;

	CButton m_ctrlAoto_DL;
	BOOL m_bAoto_DL;

	CButton m_ctrlREP_NO_SSB;
	BOOL  m_bREP_NO_SSB;




	int m_iGpibCard;
	CEdit m_ctrlGpibCard;
	int m_iGpibAddress;
	CEdit m_ctrlGpibAddress;
	CEdit m_ctrlRobotComPort;
	int m_iRobotComPort;
	CButton m_ctrlNoBaudSwitch;
	BOOL m_bNoBaudSwitch;

	CButton m_ctrlErase_Switch;
	BOOL m_bErase_Switch;

	int m_iReset_Time_Interval;
	int m_iASR_DL_TIMEOUT;

	CButton m_ctrlCheckSSB;
	BOOL m_bCheckSSB;
	BOOL m_bWirte_RF_Configure;
	afx_msg void OnBnClickedcheckSsb();
	afx_msg void OnBnClickednobaudswitch();
	afx_msg void OnBnClickedSsbBin();
	afx_msg void OnBnClickedEraseSwitch();
	afx_msg void OnBnClickedDlFwAuto();;
	int m_iPLC_Com_Port;
	afx_msg void OnEnChangePlcCom();

	int m_iModule_Tpye;
	afx_msg void OnBnClickedDetPort1();
	afx_msg void OnBnClickedDetPort2();
	afx_msg void OnBnClickedDetPort3();
	afx_msg void OnBnClickedDetPort4();
	CString m_strDet_Port1;
	CString m_strDet_Port2;
	CString m_strDet_Port3;
	CString m_strDet_Port4;

	CString m_strDet_Port_Path1;
	CString m_strDet_Port_Path2;
	CString m_strDet_Port_Path3;
	CString m_strDet_Port_Path4;
	

	BOOL ClosePLC_Power_Path_Conf(int Index);
	BOOL OpenPLC_Power_Path_Conf(int Index);

	CString m_strFN_Name1;
	CString m_strFN_Name2;
	CString m_strFN_Name3;
	CString m_strFN_Name4;
	CString m_strFN_Name5;
	CString m_strFN_Name6;
	CString m_strFN_Name7;
	CString m_strFN_Name8;
	CString m_strFN_Name9;
	CString m_strFN_Name10;
	int m_iTS_LAN_PORT;
	CEdit m_ctrlGpibCard_1;
	CEdit m_ctrlGpibCard_2;
	CEdit m_ctrlGpibCard_3;
	CEdit m_ctrlGpibCard_4;
	int m_iGpibCard_1;
	int m_iGpibCard_2;
	int m_iGpibCard_3;
	int m_iGpibCard_4;
	CEdit m_ctrlGpibAddress_1;
	CEdit m_ctrlGpibAddress_2;
	CEdit m_ctrlGpibAddress_3;
	CEdit m_ctrlGpibAddress_4;
	int m_iGpibAddress_1;
	int m_iGpibAddress_2;
	int m_iGpibAddress_3;
	int m_iGpibAddress_4;
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
	int m_iPort1;
	int m_iPort2;
	int m_iPort3;
	int m_iPort4;
	int m_iPort5;
	int m_iPort6;
	int m_iPort7;
	int m_iPort8;
	int m_iPort9;
	int m_iPort10;
	int m_iBR_Multi_Idx;
	int m_iQDPort1;
	int m_iQDPort2;
	int m_iQDPort3;
	int m_iQDPort4;

	int m_iSdx55_Port1;
	int m_iSdx55_Port2;
	int m_iSdx55_Port3;
	int m_iSdx55_Port4;

	int m_iAuto_Reset_After_DL;
	afx_msg void OnBnClickedRepNoSsb();

	void GetUsbPyAddr(int iIndex);
	afx_msg void OnEnChangeEditSdx55Port1();

	BOOL m_bAtFallback;
	BOOL m_bReboot;
	BOOL m_bRdaReboot;

	CString m_strGpib_IP_Addr;
	int m_iGpib_Type;
	BOOL m_bEraseAll;

	int m_iDEBUG_LOG;

	CString m_strGpib_IP_Addr1;
	int m_iGpib_Type1;
	CString m_strGpib_IP_Addr2;
	int m_iGpib_Type2;
	CString m_strGpib_IP_Addr3;
	int m_iGpib_Type3;
	CString m_strGpib_IP_Addr4;
	int m_iGpib_Type4;
	int m_iASR_Timeout;
	BOOL m_bODM_SPC_Not_DL;
	CString m_csSQL_FTP_Name;
	int m_iUse_DB_IKOTEK_IMEI;
	afx_msg void OnBnClickedCheckDbIkotekImei();
	void LoadFactoryInfo();
	CComboBox m_ComBox_Factory;
	afx_msg void OnCbnDropdownComboFactory();
	void Get_Factory_IPs();
};

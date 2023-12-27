// Dlg_Config_Multi.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "Dlg_Config_Multi.h"

#include "USBDeviceDetection.h"

#include "Com_Port.h"
extern CCriticalSection g_PLC_Pow_Ctrl;
extern CCom_Port g_cla_Com_PLC_Power;//针对一拖四的半自动夹具控制



// CDlg_Config_Multi dialog

IMPLEMENT_DYNAMIC(CDlg_Config_Multi, CDialog)

CDlg_Config_Multi::CDlg_Config_Multi(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Config_Multi::IDD, pParent)
	, m_iOP_Robot_Multi(0)
	, m_iRobot_COM_Idx(0)
	, m_iRobot_BR_Idx(0)
	, m_iCom_Port_Quan(0)
	, m_bShow_Auto_Log(FALSE)
	, m_csMO_Name(_T(""))
	, m_bEnable_Conn_Sql_Server(TRUE)
	, m_csSQL_Server_Name(_T(""))
	, m_iTS_FIX_Option_Idx_1(0)
	, m_iTS_FIX_Option_Idx_2(0)
	, m_iTS_FIX_Option_Idx_3(0)
	, m_iTS_FIX_Option_Idx_4(0)
	, m_iTS_FIX_Option_Idx_5(0)
	, m_iTS_FIX_Option_Idx_6(0)
	, m_iTS_FIX_Option_Idx_7(0)
	, m_iTS_FIX_Option_Idx_8(0)
	, m_iTS_FIX_Option_Idx_9(0)
	, m_iTS_FIX_Option_Idx_10(0)
	, m_csTS_LAN_IP(_T(""))
	, m_csMain_Ctrl_PC_LAN_IP(_T(""))
	, m_iSecPort1(0)
	, m_iSecPort2(0)
	, m_iSecPort3(0)
	, m_iSecPort4(0)
	, m_bPowFlag(FALSE)
	, m_bFirehose_Reset_after_DL(FALSE)
	, m_bDL_Provision(FALSE)
	, m_bPCIE_DL(FALSE)
	, m_bSSB_BIN(FALSE)
	, m_bREP_NO_SSB(FALSE)
	, m_bAoto_DL(TRUE)
	, m_iGpibCard(0)
	, m_iGpibAddress(0)
	, m_iRobotComPort(0)
	, m_bNoBaudSwitch(false)
	, m_bCheckSSB(false)
	, m_bWirte_RF_Configure(false)
	, m_bErase_Switch(false)
	, m_iReset_Time_Interval(0)
	, m_iASR_DL_TIMEOUT(0)
	, m_csMO_Name_Old(_T(""))
	, m_iPLC_Com_Port(0)
	, m_iModule_Tpye(0)
	, m_strDet_Port1(_T(""))
	, m_strDet_Port2(_T(""))
	, m_strDet_Port3(_T(""))
	, m_strDet_Port4(_T(""))

	, m_strFN_Name1(_T(""))
	, m_strFN_Name2(_T(""))
	, m_strFN_Name3(_T(""))
	, m_strFN_Name4(_T(""))
	, m_strFN_Name5(_T(""))
	, m_strFN_Name6(_T(""))
	, m_strFN_Name7(_T(""))
	, m_strFN_Name8(_T(""))
	, m_strFN_Name9(_T(""))
	, m_strFN_Name10(_T(""))
	, m_iTS_LAN_PORT(5000)
	, m_iGpibCard_1(0)
	, m_iGpibCard_2(0)
	, m_iGpibCard_3(0)
	, m_iGpibCard_4(0)
	, m_iGpibAddress_1(0)
	, m_iGpibAddress_2(0)
	, m_iGpibAddress_3(0)
	, m_iGpibAddress_4(0)
	, m_iPort1(0)
	, m_iPort2(0)
	, m_iPort3(0)
	, m_iPort4(0)
	, m_iPort5(0)
	, m_iPort6(0)
	, m_iPort7(0)
	, m_iPort8(0)
	, m_iPort9(0)
	, m_iPort10(0)
	, m_iBR_Multi_Idx(0)
	, m_iQDPort1(0)
	, m_iQDPort2(0)
	, m_iQDPort3(0)
	, m_iQDPort4(0)
	, m_iAuto_Reset_After_DL(0)

	, m_iSdx55_Port1(0)
	, m_iSdx55_Port2(0)
	, m_iSdx55_Port3(0)
	, m_iSdx55_Port4(0)

	, m_bAtFallback(TRUE)
	, m_bReboot(FALSE)
	, m_bRdaReboot(FALSE)

	, m_iAT_Port1(0)
	, m_iAT_Port2(0)
	, m_iAT_Port3(0)
	, m_iAT_Port4(0)
	
	, m_strGpib_IP_Addr(_T(""))
	, m_iGpib_Type(0)

	, m_strGpib_IP_Addr1(_T(""))
	, m_iGpib_Type1(0)

	, m_strGpib_IP_Addr2(_T(""))
	, m_iGpib_Type2(0)

	, m_strGpib_IP_Addr3(_T(""))
	, m_iGpib_Type3(0)

	, m_strGpib_IP_Addr4(_T(""))
	, m_iGpib_Type4(0)

	, m_bEraseAll(FALSE)
	, m_iASR_Timeout(0)

	, m_iDEBUG_LOG(0)
	, m_iFLASH_TYPE_Idx(0)
	, m_bODM_SPC_Not_DL(FALSE)
	, m_csSQL_FTP_Name(_T(""))
	, m_iUse_DB_IKOTEK_IMEI(0)
	{

}

CDlg_Config_Multi::~CDlg_Config_Multi()
{
}

void CDlg_Config_Multi::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_OP_MULTI, m_iOP_Robot_Multi);
	DDX_Control(pDX, IDC_COMBO_ROBOT_COM_MULTI, m_ctr_cbxRobot_COM);
	DDX_CBIndex(pDX, IDC_COMBO_ROBOT_COM_MULTI, m_iRobot_COM_Idx);
	DDX_CBIndex(pDX, IDC_COMBO_ROBOT_BR_MULTI, m_iRobot_BR_Idx);
	DDX_Control(pDX, IDC_COMBO_ROBOT_BR_MULTI, m_ctr_cbxRobot_BR);
	DDX_Check(pDX, IDC_CHECK_SHOW_AUTO_LOG, m_bShow_Auto_Log);
	DDX_Text(pDX, IDC_EDIT_MO_NAME, m_csMO_Name);
	DDX_Control(pDX, IDC_EDIT_MO_NAME, m_ctr_edtMO_Name);
	DDX_Check(pDX, IDC_CHECK_CONN_SQL_SERVER, m_bEnable_Conn_Sql_Server);
	//  	DDX_Text(pDX, IDC_EDIT_SERVER_NAME, m_csSQL_Server_Name);
	//  	DDX_Control(pDX, IDC_EDIT_SERVER_NAME, m_ctr_edtSQL_Server_Name);
	DDX_CBIndex(pDX, IDC_COMBO_TS_FIX_OPTION_1, m_iTS_FIX_Option_Idx_1);
	DDX_CBIndex(pDX, IDC_COMBO_TS_FIX_OPTION_2, m_iTS_FIX_Option_Idx_2);
	DDX_CBIndex(pDX, IDC_COMBO_TS_FIX_OPTION_3, m_iTS_FIX_Option_Idx_3);
	DDX_CBIndex(pDX, IDC_COMBO_TS_FIX_OPTION_4, m_iTS_FIX_Option_Idx_4);
	DDX_CBIndex(pDX, IDC_COMBO_TS_FIX_OPTION_5, m_iTS_FIX_Option_Idx_5);
	DDX_CBIndex(pDX, IDC_COMBO_TS_FIX_OPTION_6, m_iTS_FIX_Option_Idx_6);
	DDX_CBIndex(pDX, IDC_COMBO_TS_FIX_OPTION_7, m_iTS_FIX_Option_Idx_7);
	DDX_CBIndex(pDX, IDC_COMBO_TS_FIX_OPTION_8, m_iTS_FIX_Option_Idx_8);
	DDX_CBIndex(pDX, IDC_COMBO_TS_FIX_OPTION_9, m_iTS_FIX_Option_Idx_9);
	DDX_CBIndex(pDX, IDC_COMBO_TS_FIX_OPTION_10, m_iTS_FIX_Option_Idx_10);
	DDX_Control(pDX, IDC_COMBO_TS_FIX_OPTION_1, m_ctr_edtTS_FIX_Option_1);
	DDX_Control(pDX, IDC_COMBO_TS_FIX_OPTION_2, m_ctr_edtTS_FIX_Option_2);
	DDX_Control(pDX, IDC_COMBO_TS_FIX_OPTION_3, m_ctr_edtTS_FIX_Option_3);
	DDX_Control(pDX, IDC_COMBO_TS_FIX_OPTION_4, m_ctr_edtTS_FIX_Option_4);
	DDX_Control(pDX, IDC_COMBO_TS_FIX_OPTION_5, m_ctr_edtTS_FIX_Option_5);
	DDX_Control(pDX, IDC_COMBO_TS_FIX_OPTION_6, m_ctr_edtTS_FIX_Option_6);
	DDX_Control(pDX, IDC_COMBO_TS_FIX_OPTION_7, m_ctr_edtTS_FIX_Option_7);
	DDX_Control(pDX, IDC_COMBO_TS_FIX_OPTION_8, m_ctr_edtTS_FIX_Option_8);
	DDX_Control(pDX, IDC_COMBO_TS_FIX_OPTION_9, m_ctr_edtTS_FIX_Option_9);
	DDX_Control(pDX, IDC_COMBO_TS_FIX_OPTION_10, m_ctr_edtTS_FIX_Option_10);
	DDX_Text(pDX, IDC_EDIT_TS_LAN_IP, m_csTS_LAN_IP);
	DDX_Text(pDX, IDC_EDIT_MAIN_CTRL_PC_LAN_IP, m_csMain_Ctrl_PC_LAN_IP);
	DDX_Check(pDX, IDC_Firehose_Reset_after_DL, m_bFirehose_Reset_after_DL);
	DDX_Check(pDX, IDC_SC66_POR, m_bDL_Provision);
	DDX_Check(pDX, IDC_PCIE_DL, m_bPCIE_DL);



	DDX_Text(pDX, IDC_EDIT_SEC_PORT1, m_iSecPort1);
	DDX_Text(pDX, IDC_EDIT_SEC_PORT2, m_iSecPort2);
	DDX_Text(pDX, IDC_EDIT_SEC_PORT3, m_iSecPort3);
	DDX_Text(pDX, IDC_EDIT_SEC_PORT4, m_iSecPort4);


	DDX_Text(pDX, IDC_EDIT_AT_PORT1, m_iAT_Port1);
	DDX_Text(pDX, IDC_EDIT_AT_PORT2, m_iAT_Port2);
	DDX_Text(pDX, IDC_EDIT_AT_PORT3, m_iAT_Port3);
	DDX_Text(pDX, IDC_EDIT_AT_PORT4, m_iAT_Port4);


	DDX_Control(pDX, IDC_CHECK_POW, m_CtrlPowFlag);
	DDX_Check(pDX, IDC_CHECK_POW, m_bPowFlag);
	DDX_Control(pDX, IDC_SSB_BIN, m_ctrlSSB_BIN);
	DDX_Check(pDX, IDC_SSB_BIN, m_bSSB_BIN);
	DDX_Control(pDX, IDC_REP_NO_SSB, m_ctrlREP_NO_SSB);
	DDX_Check(pDX, IDC_REP_NO_SSB, m_bREP_NO_SSB);


	DDX_Control(pDX, IDC_DL_FW_AUTO, m_ctrlAoto_DL);
	DDX_Check(pDX, IDC_DL_FW_AUTO, m_bAoto_DL);

	DDX_Text(pDX, IDC_GPIB_CARD, m_iGpibCard);
	DDV_MinMaxInt(pDX, m_iGpibCard, 0, 100);
	DDX_Control(pDX, IDC_GPIB_CARD, m_ctrlGpibCard);
	DDX_Text(pDX, IDC_GPIB_ADDRESS, m_iGpibAddress);
	DDX_Control(pDX, IDC_GPIB_ADDRESS, m_ctrlGpibAddress);
	DDX_Control(pDX, IDC_ROBOT_COM_MULTI, m_ctrlRobotComPort);
	DDX_Text(pDX, IDC_ROBOT_COM_MULTI, m_iRobotComPort);
	DDX_Control(pDX, IDC_noBaudSwitch, m_ctrlNoBaudSwitch);
	DDX_Check(pDX, IDC_noBaudSwitch, m_bNoBaudSwitch);
	DDX_Control(pDX, IDC_check_SSB, m_ctrlCheckSSB);
	DDX_Check(pDX, IDC_check_SSB, m_bCheckSSB);

	DDX_Control(pDX, IDC_Erase_Switch, m_ctrlErase_Switch);
	DDX_Check(pDX, IDC_Erase_Switch, m_bErase_Switch);

	DDX_Text(pDX, IDC_Reset_Time_Interval, m_iReset_Time_Interval);
	DDX_Text(pDX, IDC_ASR_DL_TIMEOUT, m_iASR_DL_TIMEOUT);



	DDX_Text(pDX, IDC_PLC_COM, m_iPLC_Com_Port);
	DDX_CBIndex(pDX, IDC_COMBO_MOD_TYPE, m_iModule_Tpye);
	DDX_Text(pDX, IDC_DET_PORT1_INFO, m_strDet_Port1);
	DDX_Text(pDX, IDC_DET_PORT2_INFO, m_strDet_Port2);
	DDX_Text(pDX, IDC_DET_PORT3_INFO, m_strDet_Port3);
	DDX_Text(pDX, IDC_DET_PORT4_INFO, m_strDet_Port4);
	DDX_Text(pDX, IDC_EDIT_FN1, m_strFN_Name1);
	DDX_Text(pDX, IDC_EDIT_FN2, m_strFN_Name2);
	DDX_Text(pDX, IDC_EDIT_FN3, m_strFN_Name3);
	DDX_Text(pDX, IDC_EDIT_FN4, m_strFN_Name4);
	DDX_Text(pDX, IDC_EDIT_FN5, m_strFN_Name5);
	DDX_Text(pDX, IDC_EDIT_FN6, m_strFN_Name6);
	DDX_Text(pDX, IDC_EDIT_FN7, m_strFN_Name7);
	DDX_Text(pDX, IDC_EDIT_FN8, m_strFN_Name8);
	DDX_Text(pDX, IDC_EDIT_FN9, m_strFN_Name9);
	DDX_Text(pDX, IDC_EDIT_FN10, m_strFN_Name10);
	DDX_Text(pDX, IDC_EDIT_TS_LAN_PORT, m_iTS_LAN_PORT);
	DDX_Control(pDX, IDC_GPIB_CARD_1, m_ctrlGpibCard_1);
	DDX_Control(pDX, IDC_GPIB_CARD_2, m_ctrlGpibCard_2);
	DDX_Control(pDX, IDC_GPIB_CARD_3, m_ctrlGpibCard_3);
	DDX_Control(pDX, IDC_GPIB_CARD_4, m_ctrlGpibCard_4);
	DDX_Text(pDX, IDC_GPIB_CARD_1, m_iGpibCard_1);
	DDX_Text(pDX, IDC_GPIB_CARD_2, m_iGpibCard_2);
	DDX_Text(pDX, IDC_GPIB_CARD_3, m_iGpibCard_3);
	DDX_Text(pDX, IDC_GPIB_CARD_4, m_iGpibCard_4);
	DDX_Control(pDX, IDC_GPIB_ADDRESS_1, m_ctrlGpibAddress_1);
	DDX_Control(pDX, IDC_GPIB_ADDRESS_2, m_ctrlGpibAddress_2);
	DDX_Control(pDX, IDC_GPIB_ADDRESS_3, m_ctrlGpibAddress_3);
	DDX_Control(pDX, IDC_GPIB_ADDRESS_4, m_ctrlGpibAddress_4);
	DDX_Text(pDX, IDC_GPIB_ADDRESS_1, m_iGpibAddress_1);
	DDX_Text(pDX, IDC_GPIB_ADDRESS_2, m_iGpibAddress_2);
	DDX_Text(pDX, IDC_GPIB_ADDRESS_3, m_iGpibAddress_3);
	DDX_Text(pDX, IDC_GPIB_ADDRESS_4, m_iGpibAddress_4);
	DDX_Control(pDX, IDC_EDIT_PORT1, m_CtrlPort1);
	DDX_Control(pDX, IDC_EDIT_PORT2, m_CtrlPort2);
	DDX_Control(pDX, IDC_EDIT_PORT3, m_CtrlPort3);
	DDX_Control(pDX, IDC_EDIT_PORT4, m_CtrlPort4);
	DDX_Control(pDX, IDC_EDIT_PORT5, m_CtrlPort5);
	DDX_Control(pDX, IDC_EDIT_PORT6, m_CtrlPort6);
	DDX_Control(pDX, IDC_EDIT_PORT7, m_CtrlPort7);
	DDX_Control(pDX, IDC_EDIT_PORT8, m_CtrlPort8);
	DDX_Control(pDX, IDC_EDIT_PORT9, m_CtrlPort9);
	DDX_Control(pDX, IDC_EDIT_PORT10, m_CtrlPort10);
	DDX_Text(pDX, IDC_EDIT_PORT1, m_iPort1);
	DDX_Text(pDX, IDC_EDIT_PORT2, m_iPort2);
	DDX_Text(pDX, IDC_EDIT_PORT3, m_iPort3);
	DDX_Text(pDX, IDC_EDIT_PORT4, m_iPort4);
	DDX_Text(pDX, IDC_EDIT_PORT5, m_iPort5);
	DDX_Text(pDX, IDC_EDIT_PORT6, m_iPort6);
	DDX_Text(pDX, IDC_EDIT_PORT7, m_iPort7);
	DDX_Text(pDX, IDC_EDIT_PORT8, m_iPort8);
	DDX_Text(pDX, IDC_EDIT_PORT9, m_iPort9);
	DDX_Text(pDX, IDC_EDIT_PORT10, m_iPort10);
	DDX_CBIndex(pDX, IDC_COMBO_BR_MULTI, m_iBR_Multi_Idx);
	DDX_Text(pDX, IDC_EDIT_QD_PORT1, m_iQDPort1);
	DDX_Text(pDX, IDC_EDIT_QD_PORT2, m_iQDPort2);
	DDX_Text(pDX, IDC_EDIT_QD_PORT3, m_iQDPort3);
	DDX_Text(pDX, IDC_EDIT_QD_PORT4, m_iQDPort4);

	DDX_Text(pDX, IDC_EDIT_SDX55_PORT1, m_iSdx55_Port1);
	DDX_Text(pDX, IDC_EDIT_SDX55_PORT2, m_iSdx55_Port2);
	DDX_Text(pDX, IDC_EDIT_SDX55_PORT3, m_iSdx55_Port3);
	DDX_Text(pDX, IDC_EDIT_SDX55_PORT4, m_iSdx55_Port4);

	DDX_Check(pDX, IDC_ASR_AT_Fallback, m_bAtFallback);
	DDX_Check(pDX, IDC_ASR_Reboot, m_bReboot);

	DDX_Check(pDX, IDC_RDA_Reboot, m_bRdaReboot);

	DDX_Check(pDX, IDC_DEBUG_LOG, m_iDEBUG_LOG);


	DDX_Text(pDX, IDC_GPIB_IP_ADDR, m_strGpib_IP_Addr);
	DDX_CBIndex(pDX, IDC_COMBO_GPIB_TYPE, m_iGpib_Type);
	DDX_Check(pDX, IDC_ERASE_ALL, m_bEraseAll);
	//==================
	DDX_Text(pDX, IDC_GPIB_IP_ADDR11, m_strGpib_IP_Addr1);
	DDX_CBIndex(pDX, IDC_COMBO_GPIB_TYPE11, m_iGpib_Type1);

	DDX_Text(pDX, IDC_GPIB_IP_ADDR22, m_strGpib_IP_Addr2);
	DDX_CBIndex(pDX, IDC_COMBO_GPIB_TYPE22, m_iGpib_Type2);

	DDX_Text(pDX, IDC_GPIB_IP_ADDR33, m_strGpib_IP_Addr3);
	DDX_CBIndex(pDX, IDC_COMBO_GPIB_TYPE33, m_iGpib_Type3);

	DDX_Text(pDX, IDC_GPIB_IP_ADDR44, m_strGpib_IP_Addr4);
	DDX_CBIndex(pDX, IDC_COMBO_GPIB_TYPE44, m_iGpib_Type4);
	//==================
	DDX_Text(pDX, IDC_ASR_DL_TIMEOUT, m_iASR_Timeout);

	DDX_Control(pDX, IDC_COMBO_FLASH_TYPE, m_ctr_FLASH_TYPE);
	DDX_CBIndex(pDX, IDC_COMBO_FLASH_TYPE, m_iFLASH_TYPE_Idx);
	//DDX_Check(pDX, IDC_ODM_SPC_Not_DL, m_bODM_SPC_Not_DL);
	//DDX_Text(pDX, IDC_EDIT_FTP_NAME, m_csSQL_FTP_Name);
	DDX_Control(pDX, IDC_COMBO_FACTORY, m_ComBox_Factory);
}


BEGIN_MESSAGE_MAP(CDlg_Config_Multi, CDialog)
	ON_CBN_DROPDOWN(IDC_COMBO_ROBOT_COM_MULTI, &CDlg_Config_Multi::OnCbnDropdownComboRobotComMulti)
	ON_BN_CLICKED(IDOK, &CDlg_Config_Multi::OnBnClickedOk)
	ON_BN_CLICKED(IDC_check_SSB, &CDlg_Config_Multi::OnBnClickedcheckSsb)
	ON_BN_CLICKED(IDC_noBaudSwitch, &CDlg_Config_Multi::OnBnClickednobaudswitch)
	ON_BN_CLICKED(IDC_SSB_BIN, &CDlg_Config_Multi::OnBnClickedSsbBin)
	ON_BN_CLICKED(IDC_Erase_Switch, &CDlg_Config_Multi::OnBnClickedEraseSwitch)

	ON_BN_CLICKED(IDC_DL_FW_AUTO, &CDlg_Config_Multi::OnBnClickedDlFwAuto)
	ON_EN_CHANGE(IDC_PLC_COM, &CDlg_Config_Multi::OnEnChangePlcCom)
	ON_BN_CLICKED(IDC_DET_PORT1, &CDlg_Config_Multi::OnBnClickedDetPort1)
	ON_BN_CLICKED(IDC_DET_PORT2, &CDlg_Config_Multi::OnBnClickedDetPort2)
	ON_BN_CLICKED(IDC_DET_PORT3, &CDlg_Config_Multi::OnBnClickedDetPort3)
	ON_BN_CLICKED(IDC_DET_PORT4, &CDlg_Config_Multi::OnBnClickedDetPort4)
	ON_BN_CLICKED(IDC_REP_NO_SSB, &CDlg_Config_Multi::OnBnClickedRepNoSsb)
	ON_EN_CHANGE(IDC_EDIT_SDX55_PORT1, &CDlg_Config_Multi::OnEnChangeEditSdx55Port1)
	ON_BN_CLICKED(IDC_CHECK_DB_IKOTEK_IMEI, &CDlg_Config_Multi::OnBnClickedCheckDbIkotekImei)
	ON_CBN_DROPDOWN(IDC_COMBO_FACTORY, &CDlg_Config_Multi::OnCbnDropdownComboFactory)
END_MESSAGE_MAP()


// CDlg_Config_Multi message handlers

void CDlg_Config_Multi::Read_Setting(void)
{
	this->m_iOP_Robot_Multi = this->m_cla_iniFile.ReadInt(_T("Main"), _T("OP_Robot_Multi"), 0);

	this->m_iFLASH_TYPE_Idx = this->m_cla_iniFile.ReadInt(_T("Main"), _T("Flash_Type"), 0);

	//this->m_iRobot_COM_Idx = this->m_cla_iniFile.ReadInt(_T("Main"), _T("Robot_COM"), 0);
	this->m_iRobotComPort = this->m_cla_iniFile.ReadInt(_T("Main"), _T("Robot_COM"), 0);
	this->m_iRobot_BR_Idx = this->m_cla_iniFile.ReadInt(_T("Main"), _T("Robot_BR"), 1);
	this->m_bShow_Auto_Log = this->m_cla_iniFile.ReadB(_T("Main"), _T("Show_Auto_Log"), false);
#ifdef FW_MULTI
#ifndef FW_UPGRADE
	this->m_bEnable_Conn_Sql_Server = this->m_cla_iniFile.ReadB(_T("Main"), _T("Conn_Sql_Server"), true);
#else
	this->m_bEnable_Conn_Sql_Server =FALSE;
#endif
#endif

	
	
	this->m_bPCIE_DL = this->m_cla_iniFile.ReadB(_T("Main"), _T("PCIE_DL"), false);

	this->m_bDL_Provision=this->m_cla_iniFile.ReadB(_T("Main"), _T("DL_Provision"), false);

	this->m_bFirehose_Reset_after_DL=this->m_cla_iniFile.ReadB(_T("Main"), _T("Firehose_Reset_after_DL"), false);

	this->m_bSSB_BIN=this->m_cla_iniFile.ReadB(_T("Main"), _T("Bcg_ssb_com"), true);

	this->m_bREP_NO_SSB=this->m_cla_iniFile.ReadB(_T("Main"), _T("REP_MO_NO_SSB"), false);

	//this->m_bAoto_DL=this->m_cla_iniFile.ReadB(_T("Main"), _T("Auto_DL_Flag"), true);//默认被选上

	this->m_bPowFlag=this->m_cla_iniFile.ReadB(_T("Main"), _T("Pow_Flag"), true);

	

	this->m_bNoBaudSwitch=this->m_cla_iniFile.ReadB(_T("Main"), _T("BC95_Baud_Switch"), false);

	this->m_bErase_Switch=this->m_cla_iniFile.ReadB(_T("Main"), _T("BC95_Erase_Switch"), false);

	this->m_iReset_Time_Interval = this->m_cla_iniFile.ReadInt(_T("Main"), _T("Time_Interval"), 0);

	this->m_iASR_DL_TIMEOUT = this->m_cla_iniFile.ReadInt(_T("Main"), _T("ASR_DL_TIMEOUT"), 180000);
	

	this->m_bCheckSSB=this->m_cla_iniFile.ReadB(_T("Main"), _T("BC95_Check_SSB"), true);

	this->m_bWirte_RF_Configure=this->m_cla_iniFile.ReadB(_T("Main"), _T("Wirte_RF_Configure"), true);
	
	//MO
	this->m_csMO_Name = this->m_cla_iniFile.ReadStr(_T("Main"), _T("MO_Name"), _T(""));

	//本机 TS IP
	this->m_csTS_LAN_IP = this->m_cla_iniFile.ReadStr(_T("Main"), _T("TS_LAN_IP"), _T(""));

	//本机 TS Port
	this->m_iTS_LAN_PORT= this->m_cla_iniFile.ReadInt(_T("Main"), _T("TS_LAN_PORT"), 5000);

	//本机 TS IP
	this->m_csMain_Ctrl_PC_LAN_IP = this->m_cla_iniFile.ReadStr(_T("Main"), _T("Main_Ctrl_PC_LAN_IP"), _T(""));

	//GPIB Card Nnm
	this->m_iGpibCard = this->m_cla_iniFile.ReadInt(_T("Main"), _T("GPIB_Card_Num"), 1);
	//GPIB Card Address
	this->m_iGpibAddress = this->m_cla_iniFile.ReadInt(_T("Main"), _T("GPIB_Card_Address"), 5);

	this->m_strGpib_IP_Addr = this->m_cla_iniFile.ReadStr(_T("Main"), _T("Gpib_IP_Addr"), _T(""));
	this->m_iGpib_Type = this->m_cla_iniFile.ReadInt(_T("Main"), _T("Gpib_Type"), 0);
	//-------------------------------------------------------------------------------------------
	this->m_strGpib_IP_Addr1 = this->m_cla_iniFile.ReadStr(_T("Main"), _T("Gpib_IP_Addr1"), _T(""));
	this->m_iGpib_Type1 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("Gpib_Type1"), 0);
	this->m_strGpib_IP_Addr2 = this->m_cla_iniFile.ReadStr(_T("Main"), _T("Gpib_IP_Addr2"), _T(""));
	this->m_iGpib_Type2 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("Gpib_Type2"), 0);
	this->m_strGpib_IP_Addr3 = this->m_cla_iniFile.ReadStr(_T("Main"), _T("Gpib_IP_Addr3"), _T(""));
	this->m_iGpib_Type3 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("Gpib_Type3"), 0);
	this->m_strGpib_IP_Addr4 = this->m_cla_iniFile.ReadStr(_T("Main"), _T("Gpib_IP_Addr4"), _T(""));
	this->m_iGpib_Type4 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("Gpib_Type4"), 0);
	//-------------------------------------------------------------------------------------------
	//GPIB Path1 Card Nnm
	this->m_iGpibCard_1 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("GPIB_Card_Num_1"), 1);
	//GPIB Card Address
	this->m_iGpibAddress_1 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("GPIB_Card_Address_1"), 5);
	//-------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------
	//GPIB Path2 Card Nnm
	this->m_iGpibCard_2 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("GPIB_Card_Num_2"), 1);
	//GPIB Card Address
	this->m_iGpibAddress_2 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("GPIB_Card_Address_2"), 5);
	//-------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------
	//GPIB Path3 Card Nnm
	this->m_iGpibCard_3 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("GPIB_Card_Num_3"), 1);
	//GPIB Card Address
	this->m_iGpibAddress_3 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("GPIB_Card_Address_3"), 5);
	//-------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------
	//GPIB Path4 Card Nnm
	this->m_iGpibCard_4 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("GPIB_Card_Num_4"), 1);
	//GPIB Card Address
	this->m_iGpibAddress_4 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("GPIB_Card_Address_4"), 5);
	//-------------------------------------------------------------------------------------------

	//
	this->m_iTS_FIX_Option_Idx_1 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("TS_FIX_Idx_1"), 0);
	this->m_iTS_FIX_Option_Idx_2 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("TS_FIX_Idx_2"), 0);
	this->m_iTS_FIX_Option_Idx_3 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("TS_FIX_Idx_3"), 0);
	this->m_iTS_FIX_Option_Idx_4 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("TS_FIX_Idx_4"), 0);
	this->m_iTS_FIX_Option_Idx_5 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("TS_FIX_Idx_5"), 0);
	this->m_iTS_FIX_Option_Idx_6 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("TS_FIX_Idx_6"), 0);
	this->m_iTS_FIX_Option_Idx_7 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("TS_FIX_Idx_7"), 0);
	this->m_iTS_FIX_Option_Idx_8 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("TS_FIX_Idx_8"), 0);
	this->m_iTS_FIX_Option_Idx_9 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("TS_FIX_Idx_9"), 0);
	this->m_iTS_FIX_Option_Idx_10 = this->m_cla_iniFile.ReadInt(_T("Main"), _T("TS_FIX_Idx_10"), 0);


	//增加夹具编号的配置文件中读取
	this->m_strFN_Name1 = this->m_cla_iniFile.ReadStr(_T("Main"), _T("TS_FIX_NAME_1"), _T(""));
	this->m_strFN_Name2 = this->m_cla_iniFile.ReadStr(_T("Main"), _T("TS_FIX_NAME_2"), _T(""));
	this->m_strFN_Name3 = this->m_cla_iniFile.ReadStr(_T("Main"), _T("TS_FIX_NAME_3"), _T(""));
	this->m_strFN_Name4 = this->m_cla_iniFile.ReadStr(_T("Main"), _T("TS_FIX_NAME_4"), _T(""));
	this->m_strFN_Name5 = this->m_cla_iniFile.ReadStr(_T("Main"), _T("TS_FIX_NAME_5"), _T(""));
	this->m_strFN_Name6 = this->m_cla_iniFile.ReadStr(_T("Main"), _T("TS_FIX_NAME_6"), _T(""));
	this->m_strFN_Name7 = this->m_cla_iniFile.ReadStr(_T("Main"), _T("TS_FIX_NAME_7"), _T(""));
	this->m_strFN_Name8 = this->m_cla_iniFile.ReadStr(_T("Main"), _T("TS_FIX_NAME_8"), _T(""));
	this->m_strFN_Name9 = this->m_cla_iniFile.ReadStr(_T("Main"), _T("TS_FIX_NAME_9"), _T(""));
	this->m_strFN_Name10 = this->m_cla_iniFile.ReadStr(_T("Main"), _T("TS_FIX_NAME_10"), _T(""));

	

	this->m_iPLC_Com_Port= this->m_cla_iniFile.ReadInt(_T("Main"), _T("PLC_PORT"),0 );

	this->m_iSecPort1= this->m_cla_iniFile.ReadInt(_T("Main"), _T("Second_Port1"), 0);
	this->m_iSecPort2= this->m_cla_iniFile.ReadInt(_T("Main"), _T("Second_Port2"), 0);
	this->m_iSecPort3= this->m_cla_iniFile.ReadInt(_T("Main"), _T("Second_Port3"), 0);
	this->m_iSecPort4= this->m_cla_iniFile.ReadInt(_T("Main"), _T("Second_Port4"), 0);


	this->m_iQDPort1= this->m_cla_iniFile.ReadInt(_T("Main"), _T("QDload_Port1"), 0);
	this->m_iQDPort2= this->m_cla_iniFile.ReadInt(_T("Main"), _T("QDload_Port2"), 0);
	this->m_iQDPort3= this->m_cla_iniFile.ReadInt(_T("Main"), _T("QDload_Port3"), 0);
	this->m_iQDPort4= this->m_cla_iniFile.ReadInt(_T("Main"), _T("QDload_Port4"), 0);

	this->m_iSdx55_Port1= this->m_cla_iniFile.ReadInt(_T("Main"), _T("QSdx55_Port1"), 0);
	this->m_iSdx55_Port2= this->m_cla_iniFile.ReadInt(_T("Main"), _T("QSdx55_Port2"), 0);
	this->m_iSdx55_Port3= this->m_cla_iniFile.ReadInt(_T("Main"), _T("QSdx55_Port3"), 0);
	this->m_iSdx55_Port4= this->m_cla_iniFile.ReadInt(_T("Main"), _T("QSdx55_Port4"), 0);


	this->m_iPort1 = this->m_cla_iniFile.ReadInt(_T("Multi"), _T("PORT_1"), 0);
	this->m_iPort2 = this->m_cla_iniFile.ReadInt(_T("Multi"), _T("PORT_2"), 0);
	this->m_iPort3 = this->m_cla_iniFile.ReadInt(_T("Multi"), _T("PORT_3"), 0);
	this->m_iPort4 = this->m_cla_iniFile.ReadInt(_T("Multi"), _T("PORT_4"), 0);
	this->m_iPort5 = this->m_cla_iniFile.ReadInt(_T("Multi"), _T("PORT_5"), 0);
	this->m_iPort6 = this->m_cla_iniFile.ReadInt(_T("Multi"), _T("PORT_6"), 0);
	this->m_iPort7 = this->m_cla_iniFile.ReadInt(_T("Multi"), _T("PORT_7"), 0);
	this->m_iPort8 = this->m_cla_iniFile.ReadInt(_T("Multi"), _T("PORT_8"), 0);
	this->m_iPort9 = this->m_cla_iniFile.ReadInt(_T("Multi"), _T("PORT_9"), 0);
	this->m_iPort10 = this->m_cla_iniFile.ReadInt(_T("Multi"), _T("PORT_10"), 0);

	this->m_iBR_Multi_Idx = this->m_cla_iniFile.ReadInt(_T("Multi"), _T("BR_Multi"), 6);



	this->m_iModule_Tpye = this->m_cla_iniFile.ReadInt(_T("Main"), _T("Module_Tpye"), 0);

	this->m_strDet_Port1 = this->m_cla_iniFile.ReadStr(_T("Multi"), _T("ASR_PORT_1"), _T(""));
	this->m_strDet_Port2 = this->m_cla_iniFile.ReadStr(_T("Multi"), _T("ASR_PORT_2"), _T(""));
	this->m_strDet_Port3 = this->m_cla_iniFile.ReadStr(_T("Multi"), _T("ASR_PORT_3"), _T(""));
	this->m_strDet_Port4 = this->m_cla_iniFile.ReadStr(_T("Multi"), _T("ASR_PORT_4"), _T(""));

	this->m_strDet_Port_Path1 = this->m_cla_iniFile.ReadStr(_T("Multi"), _T("ASR_PORT_PATH1"), _T(""));
	this->m_strDet_Port_Path2 = this->m_cla_iniFile.ReadStr(_T("Multi"), _T("ASR_PORT_PATH2"), _T(""));
	this->m_strDet_Port_Path3 = this->m_cla_iniFile.ReadStr(_T("Multi"), _T("ASR_PORT_PATH3"), _T(""));
	this->m_strDet_Port_Path4 = this->m_cla_iniFile.ReadStr(_T("Multi"), _T("ASR_PORT_PATH4"), _T(""));


	//20200311
	this->m_iAuto_Reset_After_DL = this->m_cla_iniFile.ReadInt(_T("Main"), _T("Auto_Reset_After_DL"), 0);

	this->m_bAtFallback = this->m_cla_iniFile.ReadB(_T("Main"), _T("ASR_AT_Fallback"), true);
	this->m_bReboot = this->m_cla_iniFile.ReadB(_T("Main"), _T("ASR_Reboot"), false);

	this->m_bRdaReboot = this->m_cla_iniFile.ReadB(_T("Main"), _T("RDA_Reboot"), false);
	

	this->m_bEraseAll = this->m_cla_iniFile.ReadB(_T("Main"), _T("BC95_First_Erase"), false);


	this->m_iAT_Port1 = this->m_cla_iniFile.ReadInt(_T("Multi"), _T("AT_Port1"), 0);
	this->m_iAT_Port2 = this->m_cla_iniFile.ReadInt(_T("Multi"), _T("AT_Port2"), 0);
	this->m_iAT_Port3 = this->m_cla_iniFile.ReadInt(_T("Multi"), _T("AT_Port3"), 0);
	this->m_iAT_Port4 = this->m_cla_iniFile.ReadInt(_T("Multi"), _T("AT_Port4"), 0);

	this->m_iDEBUG_LOG = this->m_cla_iniFile.ReadInt(_T("Main"), _T("DEBUG_LOG"), 0);


	this->m_csSQL_FTP_Name = this->m_cla_iniFile.ReadStr(_T("IKOTEK_DB_INFO"), _T("FTP_IP"), _T(""));

	this->m_csSQL_Server_Name = this->m_cla_iniFile.ReadStr(_T("IKOTEK_DB_INFO"), _T("SQL_IP"), _T(""));

	this->m_iUse_DB_IKOTEK_IMEI = this->m_cla_iniFile.ReadInt(_T("IKOTEK_DB_INFO"), _T("DB_SWITCH"), 1);
}

void CDlg_Config_Multi::Save_Setting(void)
{
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("OP_Robot_Multi"), this->m_iOP_Robot_Multi);

	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Flash_Type"), this->m_iFLASH_TYPE_Idx);

	//this->m_cla_iniFile.StoreInt(_T("Main"), _T("Robot_COM"), this->m_iRobot_COM_Idx);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Robot_COM"), this->m_iRobotComPort);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Robot_BR"), this->m_iRobot_BR_Idx);
	this->m_cla_iniFile.StoreB(_T("Main"), _T("Show_Auto_Log"), this->m_bShow_Auto_Log);
	//this->m_cla_iniFile.StoreB(_T("Main"), _T("Conn_Sql_Server"), this->m_bEnable_Conn_Sql_Server);
	this->m_cla_iniFile.StoreB(_T("Main"), _T("Firehose_Reset_after_DL"), this->m_bFirehose_Reset_after_DL);
	this->m_cla_iniFile.StoreB(_T("Main"), _T("DL_Provision"), this->m_bDL_Provision);
	this->m_cla_iniFile.StoreB(_T("Main"), _T("PCIE_DL"), this->m_bPCIE_DL);


	this->m_cla_iniFile.StoreB(_T("Main"), _T("Bcg_ssb_com"), this->m_bSSB_BIN);
	this->m_cla_iniFile.StoreB(_T("Main"), _T("Auto_DL_Flag"), this->m_bAoto_DL);
	this->m_cla_iniFile.StoreB(_T("Main"), _T("Pow_Flag"), this->m_bPowFlag);

	this->m_cla_iniFile.StoreB(_T("Main"), _T("REP_MO_NO_SSB"), this->m_bREP_NO_SSB);

	this->m_cla_iniFile.StoreInt(_T("Main"), _T("PLC_PORT"), this->m_iPLC_Com_Port);

	this->m_cla_iniFile.StoreB(_T("Main"), _T("BC95_Baud_Switch"), this->m_bNoBaudSwitch);
	this->m_cla_iniFile.StoreB(_T("Main"), _T("BC95_Check_SSB"), this->m_bCheckSSB);
	this->m_cla_iniFile.StoreB(_T("Main"), _T("BC95_Erase_Switch"), this->m_bErase_Switch);

	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Time_Interval"), this->m_iReset_Time_Interval);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("ASR_DL_TIMEOUT"), this->m_iASR_DL_TIMEOUT);

	//MO
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("MO_Name"), this->m_csMO_Name);

	//本机 TS IP
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("TS_LAN_IP"), this->m_csTS_LAN_IP);

	//本机 TS Port
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("TS_LAN_PORT"), this->m_iTS_LAN_PORT);

	//本机 TS IP
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("Main_Ctrl_PC_LAN_IP"), this->m_csMain_Ctrl_PC_LAN_IP);

	//GPIB Card Nnm
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("GPIB_Card_Num"), this->m_iGpibCard);
	//GPIB Card Address
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("GPIB_Card_Address"), this->m_iGpibAddress);

	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Gpib_Type"), this->m_iGpib_Type);
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("Gpib_IP_Addr"), this->m_strGpib_IP_Addr);
//----------------------------------------------------------------------------------------
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Gpib_Type1"), this->m_iGpib_Type1);
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("Gpib_IP_Addr1"), this->m_strGpib_IP_Addr1);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Gpib_Type2"), this->m_iGpib_Type2);
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("Gpib_IP_Addr2"), this->m_strGpib_IP_Addr2);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Gpib_Type3"), this->m_iGpib_Type3);
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("Gpib_IP_Addr3"), this->m_strGpib_IP_Addr3);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Gpib_Type4"), this->m_iGpib_Type4);
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("Gpib_IP_Addr4"), this->m_strGpib_IP_Addr4);
//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//GPIB Path1 Card Nnm
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("GPIB_Card_Num_1"), this->m_iGpibCard_1);
	//GPIB Path1 Card Address
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("GPIB_Card_Address_1"), this->m_iGpibAddress_1);
//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//GPIB Path2 Card Nnm
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("GPIB_Card_Num_2"), this->m_iGpibCard_2);
	//GPIB Path1 Card Address
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("GPIB_Card_Address_2"), this->m_iGpibAddress_2);
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//GPIB Path3 Card Nnm
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("GPIB_Card_Num_3"), this->m_iGpibCard_3);
	//GPIB Path1 Card Address
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("GPIB_Card_Address_3"), this->m_iGpibAddress_3);
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//GPIB Path4 Card Nnm
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("GPIB_Card_Num_4"), this->m_iGpibCard_4);
	//GPIB Path1 Card Address
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("GPIB_Card_Address_4"), this->m_iGpibAddress_4);
	//----------------------------------------------------------------------------------------



	//
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("TS_FIX_Idx_1"), this->m_iTS_FIX_Option_Idx_1);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("TS_FIX_Idx_2"), this->m_iTS_FIX_Option_Idx_2);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("TS_FIX_Idx_3"), this->m_iTS_FIX_Option_Idx_3);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("TS_FIX_Idx_4"), this->m_iTS_FIX_Option_Idx_4);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("TS_FIX_Idx_5"), this->m_iTS_FIX_Option_Idx_5);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("TS_FIX_Idx_6"), this->m_iTS_FIX_Option_Idx_6);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("TS_FIX_Idx_7"), this->m_iTS_FIX_Option_Idx_7);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("TS_FIX_Idx_8"), this->m_iTS_FIX_Option_Idx_8);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("TS_FIX_Idx_9"), this->m_iTS_FIX_Option_Idx_9);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("TS_FIX_Idx_10"), this->m_iTS_FIX_Option_Idx_10);

	//增加夹具编号的配置文件中保存
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("TS_FIX_NAME_1"), this->m_strFN_Name1);
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("TS_FIX_NAME_2"), this->m_strFN_Name2);
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("TS_FIX_NAME_3"), this->m_strFN_Name3);
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("TS_FIX_NAME_4"), this->m_strFN_Name4);
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("TS_FIX_NAME_5"), this->m_strFN_Name5);
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("TS_FIX_NAME_6"), this->m_strFN_Name6);
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("TS_FIX_NAME_7"), this->m_strFN_Name7);
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("TS_FIX_NAME_8"), this->m_strFN_Name8);
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("TS_FIX_NAME_9"), this->m_strFN_Name9);
	this->m_cla_iniFile.StoreStr(_T("Main"), _T("TS_FIX_NAME_10"), this->m_strFN_Name10);


	this->m_cla_iniFile.StoreInt(_T("Multi"), _T("PORT_1"), this->m_iPort1);
	this->m_cla_iniFile.StoreInt(_T("Multi"), _T("PORT_2"), this->m_iPort2);
	this->m_cla_iniFile.StoreInt(_T("Multi"), _T("PORT_3"), this->m_iPort3);
	this->m_cla_iniFile.StoreInt(_T("Multi"), _T("PORT_4"), this->m_iPort4);
	this->m_cla_iniFile.StoreInt(_T("Multi"), _T("PORT_5"), this->m_iPort5);
	this->m_cla_iniFile.StoreInt(_T("Multi"), _T("PORT_6"), this->m_iPort6);
	this->m_cla_iniFile.StoreInt(_T("Multi"), _T("PORT_7"), this->m_iPort7);
	this->m_cla_iniFile.StoreInt(_T("Multi"), _T("PORT_8"), this->m_iPort8);
	this->m_cla_iniFile.StoreInt(_T("Multi"), _T("PORT_9"), this->m_iPort9);
	this->m_cla_iniFile.StoreInt(_T("Multi"), _T("PORT_10"), this->m_iPort10);
	this->m_cla_iniFile.StoreInt(_T("Multi"), _T("BR_Multi"), this->m_iBR_Multi_Idx);


	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Second_Port1"), this->m_iSecPort1);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Second_Port2"), this->m_iSecPort2);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Second_Port3"), this->m_iSecPort3);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Second_Port4"), this->m_iSecPort4);

	this->m_cla_iniFile.StoreInt(_T("Main"), _T("QDload_Port1"), this->m_iQDPort1);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("QDload_Port2"), this->m_iQDPort2);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("QDload_Port3"), this->m_iQDPort3);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("QDload_Port4"), this->m_iQDPort4);

	this->m_cla_iniFile.StoreInt(_T("Main"), _T("QSdx55_Port1"), this->m_iSdx55_Port1);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("QSdx55_Port2"), this->m_iSdx55_Port2);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("QSdx55_Port3"), this->m_iSdx55_Port3);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("QSdx55_Port4"), this->m_iSdx55_Port4);


	this->m_cla_iniFile.StoreInt(_T("Main"), _T("Module_Tpye"), this->m_iModule_Tpye);

	this->m_cla_iniFile.StoreB(_T("Main"), _T("ASR_AT_Fallback"), this->m_bAtFallback);
	this->m_cla_iniFile.StoreB(_T("Main"), _T("ASR_Reboot"), this->m_bReboot);

	this->m_cla_iniFile.StoreB(_T("Main"), _T("RDA_Reboot"), this->m_bRdaReboot);
	

	this->m_cla_iniFile.StoreB(_T("Main"), _T("BC95_First_Erase"), this->m_bEraseAll);

	this->m_cla_iniFile.StoreInt(_T("Multi"), _T("AT_Port1"), this->m_iAT_Port1);
	this->m_cla_iniFile.StoreInt(_T("Multi"), _T("AT_Port2"), this->m_iAT_Port2);
	this->m_cla_iniFile.StoreInt(_T("Multi"), _T("AT_Port3"), this->m_iAT_Port3);
	this->m_cla_iniFile.StoreInt(_T("Multi"), _T("AT_Port4"), this->m_iAT_Port4);	

	this->m_cla_iniFile.StoreInt(_T("Main"), _T("DEBUG_LOG"), this->m_iDEBUG_LOG);

	this->m_cla_iniFile.StoreStr(_T("IKOTEK_DB_INFO"), _T("FTP_IP"), this->m_csSQL_FTP_Name);

	this->m_cla_iniFile.StoreStr(_T("IKOTEK_DB_INFO"), _T("SQL_IP"), this->m_csSQL_Server_Name);
}

void CDlg_Config_Multi::OnCbnDropdownComboRobotComMulti()
{
	this->Update_Com_Ctrl_Option();
}

void CDlg_Config_Multi::Delete_Com_Ctrl_Option(CComboBox * clap_cbxCom_Port)
{
	while(clap_cbxCom_Port->GetCount())
	{
		clap_cbxCom_Port->DeleteString(0);
	}
}

void CDlg_Config_Multi::Load_Com_Ctrl_Option(CComboBox * clap_cbxCom_Port, int * ip_Com_Port_Idx)
{
	CString csData;
	int i = 0;


	//搜索PC上所有COM口
	this->m_iCom_Port_Quan = 0;
	memset(this->m_irCom_Port_List, 0, sizeof(this->m_irCom_Port_List));
	this->m_cla_GFC.Get_Reg_Com_Port_List(this->m_irCom_Port_List, &this->m_iCom_Port_Quan);

	//载入COM口列表
	while(i < this->m_iCom_Port_Quan)
	{
		csData.Format(_T("%d"), this->m_irCom_Port_List[i]);
		clap_cbxCom_Port->AddString(csData);
		i++;
	}

	//
	if(*ip_Com_Port_Idx >= this->m_iCom_Port_Quan)
	{
		*ip_Com_Port_Idx = 0;
	}

	//设定当前值
	clap_cbxCom_Port->SetCurSel(*ip_Com_Port_Idx);
}

void CDlg_Config_Multi::Update_Com_Ctrl_Option(void)
{
	this->Delete_Com_Ctrl_Option(&this->m_ctr_cbxRobot_COM);
	this->Load_Com_Ctrl_Option(&this->m_ctr_cbxRobot_COM, &this->m_iRobot_COM_Idx);	
}

void CDlg_Config_Multi::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	this->m_csMO_Name_Old = this->m_csMO_Name;

	Get_Factory_IPs();

	this->UpdateData(TRUE);

	OnOK();
}

BOOL CDlg_Config_Multi::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	//加载COM口数据
	this->Update_Com_Ctrl_Option();

	this->Read_Setting();

	UpdateData(FALSE);

	//=====[IKOTEK_DB_INFO]
	if (1 == m_iUse_DB_IKOTEK_IMEI)
	{
		((CButton *)GetDlgItem(IDC_CHECK_DB_IKOTEK_IMEI))->SetCheck(TRUE);
		//GetDlgItem(IDC_EDIT_FTP_NAME)->EnableWindow(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECK_DB_IKOTEK_IMEI))->SetCheck(FALSE);
		//GetDlgItem(IDC_EDIT_FTP_NAME)->EnableWindow(FALSE);
	}

	LoadFactoryInfo();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_Config_Multi::Read_Scan_Cfg(int i_Index, CString * csp_Scan_IP, int * ip_Scan_Port)
{
	CString csSection_Name;

	csSection_Name.Format(_T("SCAN_%d"), i_Index);

	//
	*csp_Scan_IP = this->m_cla_iniFile.ReadStr(csSection_Name, _T("IP"), _T(""));
	*ip_Scan_Port = this->m_cla_iniFile.ReadInt(csSection_Name, _T("Port"), 0);
}

void CDlg_Config_Multi::Read_Scan_Angle(int i_Index, int * ip_Angle)
{
	CString csSection_Name;

	csSection_Name.Format(_T("SCAN_%d"), i_Index);

	//
	*ip_Angle = this->m_cla_iniFile.ReadInt(csSection_Name, _T("Angle"), 0);
}

void CDlg_Config_Multi::OnBnClickedcheckSsb()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	this->m_cla_iniFile.StoreB(_T("Main"), _T("BC95_Check_SSB"), this->m_bCheckSSB);
}

void CDlg_Config_Multi::OnBnClickednobaudswitch()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	this->m_cla_iniFile.StoreB(_T("Main"), _T("BC95_Baud_Switch"), this->m_bNoBaudSwitch);
}

void CDlg_Config_Multi::OnBnClickedSsbBin()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	this->m_cla_iniFile.StoreB(_T("Main"), _T("Bcg_ssb_com"), this->m_bSSB_BIN);
}

void CDlg_Config_Multi::OnBnClickedEraseSwitch()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	this->m_cla_iniFile.StoreB(_T("Main"), _T("BC95_Erase_Switch"), this->m_bErase_Switch);
}

void CDlg_Config_Multi::OnBnClickedDlFwAuto()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	this->m_cla_iniFile.StoreB(_T("Main"), _T("Auto_DL_Flag"), this->m_bAoto_DL);	
}


void CDlg_Config_Multi::OnEnChangePlcCom()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	this->m_cla_iniFile.StoreInt(_T("Main"), _T("PLC_PORT"), this->m_iPLC_Com_Port);
}

void CDlg_Config_Multi::GetUsbPyAddr(int iIndex)
{
	CUSBDeviceDetection usbDeviceDetect;
	CStringArray strPhysicalAddress,strDeviceInstancePath;
	UINT uDeviceCount;

	time_t t_Start, t_End;
	int iTimeout = 15;

	//开始计时
	time(&t_Start);

	try
	{

		while (1)
		{
			usbDeviceDetect.GetUsbPhysicalAddressList(strPhysicalAddress,strDeviceInstancePath,uDeviceCount);
			if (strPhysicalAddress.GetSize()>0)
			{
				break;
			}
			//超时检测
			time(&t_End);
			//
			if((int)difftime(t_End, t_Start) >= iTimeout)//超时，报fail
			{
				break;
			}

		}

		switch(iIndex)
		{
			case 1:
			{
				if (strPhysicalAddress.GetSize()<=0)
				{
					//
					GetDlgItem(IDC_DET_PORT1_INFO2)->SetWindowText(_T("Get USB addresss 超时，请将模块重启再进行获取"));  
					GetDlgItem(IDC_DET_PORT1_INFO)->SetWindowText(_T("")); 
					this->m_cla_iniFile.StoreStr(_T("Multi"), _T("ASR_PORT_1"), _T(""));

				} 
				else
				{
					//
					m_strDet_Port1= strPhysicalAddress.GetAt(0);
					this->m_cla_iniFile.StoreStr(_T("Multi"), _T("ASR_PORT_1"), m_strDet_Port1);

					GetDlgItem(IDC_DET_PORT1_INFO)->SetWindowText(m_strDet_Port1);  

					//20191108
					m_strDet_Port_Path1= strDeviceInstancePath.GetAt(0);
					this->m_cla_iniFile.StoreStr(_T("Multi"), _T("ASR_PORT_PATH1"), m_strDet_Port_Path1);

					GetDlgItem(IDC_DET_PORT1_INFO2)->SetWindowText(_T("Get USB addresss Pass"));  


				}
			}
			break;
			case 2:
			{
				if (strPhysicalAddress.GetSize()<=0)
				{
					//
					GetDlgItem(IDC_DET_PORT2_INFO2)->SetWindowText(_T("Get USB addresss 超时，请将模块重启再进行获取"));  
					GetDlgItem(IDC_DET_PORT2_INFO)->SetWindowText(_T("")); 
					this->m_cla_iniFile.StoreStr(_T("Multi"), _T("ASR_PORT_2"), _T(""));

				} 
				else
				{
					//
					m_strDet_Port2= strPhysicalAddress.GetAt(0);
					this->m_cla_iniFile.StoreStr(_T("Multi"), _T("ASR_PORT_2"), m_strDet_Port2);

					GetDlgItem(IDC_DET_PORT2_INFO)->SetWindowText(m_strDet_Port2);  

					//20191108
					m_strDet_Port_Path2= strDeviceInstancePath.GetAt(0);
					this->m_cla_iniFile.StoreStr(_T("Multi"), _T("ASR_PORT_PATH2"), m_strDet_Port_Path2);

					GetDlgItem(IDC_DET_PORT2_INFO2)->SetWindowText(_T("Get USB addresss Pass"));  


				}
			}
			break;
			case 3:
			{
				if (strPhysicalAddress.GetSize()<=0)
				{
					//
					GetDlgItem(IDC_DET_PORT3_INFO2)->SetWindowText(_T("Get USB addresss 超时，请将模块重启再进行获取"));  
					GetDlgItem(IDC_DET_PORT3_INFO)->SetWindowText(_T("")); 
					this->m_cla_iniFile.StoreStr(_T("Multi"), _T("ASR_PORT_3"), _T(""));

				} 
				else
				{
					//
					m_strDet_Port3= strPhysicalAddress.GetAt(0);
					this->m_cla_iniFile.StoreStr(_T("Multi"), _T("ASR_PORT_3"), m_strDet_Port3);

					GetDlgItem(IDC_DET_PORT3_INFO)->SetWindowText(m_strDet_Port3);  

					//20191108
					m_strDet_Port_Path3= strDeviceInstancePath.GetAt(0);
					this->m_cla_iniFile.StoreStr(_T("Multi"), _T("ASR_PORT_PATH3"), m_strDet_Port_Path3);

					GetDlgItem(IDC_DET_PORT3_INFO2)->SetWindowText(_T("Get USB addresss Pass")); 


				}
			}
			break;
			case 4:
			{
				if (strPhysicalAddress.GetSize()<=0)
				{
					//
					GetDlgItem(IDC_DET_PORT4_INFO2)->SetWindowText(_T("Get USB addresss 超时，请将模块重启再进行获取"));  
					GetDlgItem(IDC_DET_PORT4_INFO)->SetWindowText(_T("")); 
					this->m_cla_iniFile.StoreStr(_T("Multi"), _T("ASR_PORT_4"), _T(""));

				} 
				else
				{
					//
					m_strDet_Port4= strPhysicalAddress.GetAt(0);
					this->m_cla_iniFile.StoreStr(_T("Multi"), _T("ASR_PORT_4"), m_strDet_Port4);

					GetDlgItem(IDC_DET_PORT4_INFO)->SetWindowText(m_strDet_Port4);  

					//20191108
					m_strDet_Port_Path4= strDeviceInstancePath.GetAt(0);
					this->m_cla_iniFile.StoreStr(_T("Multi"), _T("ASR_PORT_PATH4"), m_strDet_Port_Path4);

					GetDlgItem(IDC_DET_PORT4_INFO2)->SetWindowText(_T("Get USB addresss Pass")); 


				}
			}
			break;
			default:break;



		}
	}
	catch(...)
	{

	}

}


void GetUsbAddr_1(LPVOID x)
{
	CDlg_Config_Multi *pSv = (CDlg_Config_Multi *)x;
	//
	pSv->GetUsbPyAddr(1);

}
void GetUsbAddr_2(LPVOID x)
{
	CDlg_Config_Multi *pSv = (CDlg_Config_Multi *)x;
	//
	pSv->GetUsbPyAddr(2);

}
void GetUsbAddr_3(LPVOID x)
{
	CDlg_Config_Multi *pSv = (CDlg_Config_Multi *)x;
	//
	pSv->GetUsbPyAddr(3);

}
void GetUsbAddr_4(LPVOID x)
{
	CDlg_Config_Multi *pSv = (CDlg_Config_Multi *)x;
	//
	pSv->GetUsbPyAddr(4);

}



void Multi_PLC_Pow_Con_1(LPVOID x)
{
	CDlg_Config_Multi *pSv = (CDlg_Config_Multi *)x;

	//关闭其中一路PLC的电源
	pSv->ClosePLC_Power_Path_Conf(1);
	Sleep(2000);
	pSv->OpenPLC_Power_Path_Conf(1);

}
void Multi_PLC_Pow_Con_2(LPVOID x)
{
	CDlg_Config_Multi *pSv = (CDlg_Config_Multi *)x;

	//关闭其中一路PLC的电源
	pSv->ClosePLC_Power_Path_Conf(2);
	Sleep(2000);
	pSv->OpenPLC_Power_Path_Conf(2);

}
void Multi_PLC_Pow_Con_3(LPVOID x)
{
	CDlg_Config_Multi *pSv = (CDlg_Config_Multi *)x;

	//关闭其中一路PLC的电源
	pSv->ClosePLC_Power_Path_Conf(3);
	Sleep(2000);
	pSv->OpenPLC_Power_Path_Conf(3);

}
void Multi_PLC_Pow_Con_4(LPVOID x)
{
	CDlg_Config_Multi *pSv = (CDlg_Config_Multi *)x;

	//关闭其中一路PLC的电源
	pSv->ClosePLC_Power_Path_Conf(4);
	Sleep(2000);
	pSv->OpenPLC_Power_Path_Conf(4);

}
//打开其中一路PLC的电源
BOOL CDlg_Config_Multi::OpenPLC_Power_Path_Conf(int Index)
{
	time_t t_Start, t_End;
	unsigned long ulIndex=0;
	int  i_TimeOut=3;
	unsigned char ucOpenCmd[4],ucrTgt_Key[4];
	CString csData;
	char cp_Read_Buff[8];
	int ulTgt_Key_Quan=0;
	unsigned long ul_Expect_Read_Bytes=5;
	unsigned long ulp_Real_Rec_Bytes=0;

	try
	{	
		g_PLC_Pow_Ctrl.Lock();

		g_cla_Com_PLC_Power.m_iPort = this->m_iPLC_Com_Port;
		g_cla_Com_PLC_Power.m_iBaudRate = 119200;

		g_cla_Com_PLC_Power.m_dwDTR_CTRL = FALSE;
		g_cla_Com_PLC_Power.m_dwRTS_CTRL = FALSE;

		if (false == g_cla_Com_PLC_Power.Port_Open(500))
		{
			throw(0);
		}

		//发送一次指令
		ucOpenCmd[0]=0xFF;
		ucOpenCmd[1]=Index;
		ucOpenCmd[2]=0x1A;
		ucOpenCmd[3]=0x00;

		//期望回值
		ucrTgt_Key[0] = 0xFF;
		ucrTgt_Key[1] = Index;
		ucrTgt_Key[2] = 0x1B;
		ucrTgt_Key[3] = 0x00;
		ulTgt_Key_Quan = 4;


		//清空接收BUFF
		if (false == g_cla_Com_PLC_Power.Empty_Com_And_Rx_Buff())
		{
			throw(0);
		}


		//循环
		time(&t_Start);
		//
		do
		{
			g_cla_Com_PLC_Power.Write_Byte((char*)ucOpenCmd,4);

			//期望值为：0xFF 01 1B 00 
			memset(&cp_Read_Buff,0,sizeof(cp_Read_Buff));
			Sleep(20);
			g_cla_Com_PLC_Power.Read_Byte(cp_Read_Buff,ul_Expect_Read_Bytes,&ulp_Real_Rec_Bytes);

			if(ulp_Real_Rec_Bytes!=0)
			{
				if(this->m_cla_GFC.mem_uc_str((unsigned char*)cp_Read_Buff, ulp_Real_Rec_Bytes, ucrTgt_Key, ulTgt_Key_Quan, &ulIndex) == true)
				{
					g_PLC_Pow_Ctrl.Unlock();
					return TRUE;
				}			
			}

			//超时检测
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= i_TimeOut)
			{
				throw(0);
			}
		}
		while(true);
	}
	catch(...)
	{
		g_PLC_Pow_Ctrl.Unlock();
		return FALSE;
	}
	return TRUE;
}

//关闭其中一路PLC的电源
BOOL CDlg_Config_Multi::ClosePLC_Power_Path_Conf(int Index)
{
	time_t t_Start, t_End;
	unsigned long ulIndex=0;
	int  i_TimeOut=3;
	unsigned char ucOpenCmd[4],ucrTgt_Key[4];
	CString csData;
	char cp_Read_Buff[8];
	int ulTgt_Key_Quan=0;
	unsigned long ul_Expect_Read_Bytes=5;
	unsigned long ulp_Real_Rec_Bytes=0;

	try
	{
		g_PLC_Pow_Ctrl.Lock();

		g_cla_Com_PLC_Power.m_iPort = this->m_iPLC_Com_Port;
		g_cla_Com_PLC_Power.m_iBaudRate = 119200;

		g_cla_Com_PLC_Power.m_dwDTR_CTRL = FALSE;
		g_cla_Com_PLC_Power.m_dwRTS_CTRL = FALSE;

		if (false == g_cla_Com_PLC_Power.Port_Open(500))
		{
			throw(0);
		}


		//发送一次指令
		ucOpenCmd[0]=0xFF;
		ucOpenCmd[1]=Index;
		ucOpenCmd[2]=0x1C;
		ucOpenCmd[3]=0x00;

		//期望回值
		ucrTgt_Key[0] = 0xFF;
		ucrTgt_Key[1] = Index;
		ucrTgt_Key[2] = 0x1D;
		ucrTgt_Key[3] = 0x00;
		ulTgt_Key_Quan = 4;


		//清空接收BUFF
		//清空接收BUFF
		if (false == g_cla_Com_PLC_Power.Empty_Com_And_Rx_Buff())
		{
			throw(0);
		}

		//循环
		time(&t_Start);
		//
		do
		{
			g_cla_Com_PLC_Power.Write_Byte((char*)ucOpenCmd,4);

			//期望值为：0xFF 01 1B 00 
			memset(&cp_Read_Buff,0,sizeof(cp_Read_Buff));
			Sleep(20);
			g_cla_Com_PLC_Power.Read_Byte(cp_Read_Buff,ul_Expect_Read_Bytes,&ulp_Real_Rec_Bytes);

			if(ulp_Real_Rec_Bytes!=0)
			{
				if(this->m_cla_GFC.mem_uc_str((unsigned char*)cp_Read_Buff, ulp_Real_Rec_Bytes, ucrTgt_Key, ulTgt_Key_Quan, &ulIndex) == true)
				{
					g_PLC_Pow_Ctrl.Unlock();
					return TRUE;
				}			
			}

			//超时检测
			time(&t_End);
			if((int)difftime(t_End, t_Start) >= i_TimeOut)
			{
				throw(0);
			}
		}
		while(true);
	}
	catch(...)
	{
		g_PLC_Pow_Ctrl.Unlock();
		return FALSE;
	}
}

void CDlg_Config_Multi::OnBnClickedDetPort1()
{
	// TODO: Add your control notification handler code here

	GetDlgItem(IDC_DET_PORT1_INFO2)->SetWindowText(_T("Get USB addresss..."));  



	if (TRUE == this->m_bPowFlag)
	{
		//利用线程对改通道上电
		CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)Multi_PLC_Pow_Con_1,
			(void *)this,
			0,
			NULL);

		Sleep(5000);
	}

	//建立找口线程
	CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)GetUsbAddr_1,
		(void *)this,
		0,
		NULL);

}

void CDlg_Config_Multi::OnBnClickedDetPort2()
{
	// TODO: Add your control notification handler code here

	GetDlgItem(IDC_DET_PORT2_INFO2)->SetWindowText(_T("Get USB addresss..."));  
	if (TRUE == this->m_bPowFlag)
	{
		//利用线程对改通道上电
		CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)Multi_PLC_Pow_Con_2,
			(void *)this,
			0,
			NULL);

		Sleep(5000);
	}

	//建立找口线程
	CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)GetUsbAddr_2,
		(void *)this,
		0,
		NULL);



}

void CDlg_Config_Multi::OnBnClickedDetPort3()
{
	// TODO: Add your control notification handler code here

	GetDlgItem(IDC_DET_PORT3_INFO2)->SetWindowText(_T("Get USB addresss..."));  
	if (TRUE == this->m_bPowFlag)
	{
		//利用线程对改通道上电
		CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)Multi_PLC_Pow_Con_3,
			(void *)this,
			0,
			NULL);

		Sleep(5000);
	}

	//建立找口线程
	CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)GetUsbAddr_3,
		(void *)this,
		0,
		NULL);

}

void CDlg_Config_Multi::OnBnClickedDetPort4()
{
	// TODO: Add your control notification handler code here

	GetDlgItem(IDC_DET_PORT4_INFO2)->SetWindowText(_T("Get USB addresss..."));  

	if (TRUE == this->m_bPowFlag)
	{
		//利用线程对改通道上电
		CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)Multi_PLC_Pow_Con_4,
			(void *)this,
			0,
			NULL);

		Sleep(5000);
	}

	//建立找口线程
	CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)GetUsbAddr_4,
		(void *)this,
		0,
		NULL);

}



void CDlg_Config_Multi::OnBnClickedRepNoSsb()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	this->m_cla_iniFile.StoreB(_T("Main"), _T("REP_MO_NO_SSB"), this->m_bREP_NO_SSB);
}

void CDlg_Config_Multi::OnEnChangeEditSdx55Port1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CDlg_Config_Multi::OnBnClickedCheckDbIkotekImei()
{
	// TODO:  在此添加控件通知处理程序代码
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_DB_IKOTEK_IMEI);
	int i_State = pBtn->GetCheck();
	//上面的也可以绑定一个变量做处理

	m_cla_iniFile.StoreInt(_T("IKOTEK_DB_INFO"), _T("DB_SWITCH"), i_State);


}


void CDlg_Config_Multi::LoadFactoryInfo()
{
	CStdioFile fileAccount;
	if (fileAccount.Open(_T(".\\FactoryInfo.txt"), CFile::typeText | CFile::modeRead))
	{
		CString strValue(_T("\0"));

		// 处理UNICODE下【中文乱码】异常
		char* pOldLocale = _strdup(setlocale(LC_CTYPE, NULL));
		setlocale(LC_CTYPE, "chs");

		while (fileAccount.ReadString(strValue))
		{
			m_ComBox_Factory.AddString(strValue);
			strValue.Empty();
		}

		// 处理完毕后,释放资源
		setlocale(LC_CTYPE, pOldLocale);
		free(pOldLocale);
	}

	fileAccount.Close();



	if (m_csSQL_Server_Name.IsEmpty())
	{
		m_ComBox_Factory.SetCurSel(0);
	}
	else
	{
		CString str_ListName = _T("");
		int index = m_ComBox_Factory.GetCount();
		for (int i = 0; i < index; i++)
		{
			m_ComBox_Factory.GetLBText(i, str_ListName);
			if (str_ListName.Find(m_csSQL_Server_Name) != -1)
			{
				m_ComBox_Factory.SetCurSel(i);
				break;
			}
		}
	}
}


void CDlg_Config_Multi::OnCbnDropdownComboFactory()
{
	// TODO:  在此添加控件通知处理程序代码
	CClientDC dc(this);
	int nWitdh = 10;
	int nSaveDC = dc.SaveDC();

	//获取字体信息，
	dc.SelectObject(GetFont());

	//计算最大的显示长度
	for (int i = 0; i < m_ComBox_Factory.GetCount(); i++)
	{
		CString strLable = _T("");
		m_ComBox_Factory.GetLBText(i, strLable);

		nWitdh = max(nWitdh, dc.GetTextExtent(strLable).cx);
	}

	//多增加的冗余宽度
	nWitdh += 20;

	//设置下拉列表宽度
	m_ComBox_Factory.SetDroppedWidth(nWitdh);
	//恢复实际dc
	dc.RestoreDC(nSaveDC);
}


void CDlg_Config_Multi::Get_Factory_IPs()
{
	CString csValue;
	int index = m_ComBox_Factory.GetCurSel();
	m_ComBox_Factory.GetLBText(index, csValue);//选项的值保存到csValues中

	m_csSQL_Server_Name = (_T(""));
	m_csSQL_FTP_Name = (_T(""));


	CString strSrc = csValue;
	CStringArray strResult;

	CString strGap = _T(" ");
	int nPos = strSrc.Find(strGap);

	CString strLeft = _T("");
	while (0 <= nPos)
	{
		strLeft = strSrc.Left(nPos);
		if (!strLeft.IsEmpty())
			strResult.Add(strLeft);

		strSrc = strSrc.Right(strSrc.GetLength() - nPos - 1);
		nPos = strSrc.Find(strGap);
	}

	if (!strSrc.IsEmpty())
	{
		strResult.Add(strSrc);
	}

	m_csSQL_Server_Name = strResult.GetAt(1);
	m_csSQL_FTP_Name = strResult.GetAt(2);
}


// MTView.h : interface of the CMTView class
//

#pragma once
#include "Gen_Header.h"
#include "afxwin.h"
#include "MsgBox.h"
#include "InfoWindow.h"
#include "afxcmn.h"
#include "atltypes.h"
#include "dlg_example.h"
#include "dlg_fw_multi.h"
#include "logview.h"
#include "sql_db.h"
//#include "inifile.h"
#include "UpdateConfig.h"
#include "Ftp.h"
#include "Tool_Update_Config.h"


//主窗口显示标题名
extern CString g_csMain_Title_Name;

class CMTView : public CFormView
{
protected: // create from serialization only
	CMTView();
	DECLARE_DYNCREATE(CMTView)

public:
	enum{ IDD = IDD_MT_FORM };

// Attributes
public:
	CMTDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CMTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	// 获取当前路径，并初始化Config文件的路径
	void Init_Config_File_Path(void);
	wchar_t m_wcrCurr_Exe_Path[MAX_PATH];
	wchar_t m_wcrLog_File_Path[MAX_PATH];
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void Show_MsgBox(const wchar_t * wcpTitle, const wchar_t * wcpMsg, int iFlag, bool bClose, int iTime);
	CInfoWindow * m_clap_ShowInfoDia;
	LRESULT OnGetMsg_Show_Info(WPARAM wParam, LPARAM lParam);
	void Show_Info(const wchar_t * wcpMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	CTabCtrl m_ctr_tabMain;
	void Create_Dlg_View(void);
	CRect m_rect_tab;
	CRect m_rect_main;
	void Ctrl_Dlg_View();
	LRESULT OnGetMsg_Log_In(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Log_Out(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMsg_Log_Change(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	LRESULT OnGetMsg_Change_Password(WPARAM wParam, LPARAM lParam);
	CDlg_Example m_dlg_Upgrade_FW_Single;
	CDlg_FW_Multi m_dlg_Upgrade_FW_Multi;
	void Read_Params_First_Basic(void);
	CIniFile m_cla_IniFile_Basic;
	afx_msg void OnMenu_Reset_Stat_Tsch_1();
	afx_msg void OnMenu_Reset_Stat_Tsch_2();
	afx_msg void OnMenu_Reset_Stat_Tsch_3();
	afx_msg void OnMenu_Reset_Stat_Tsch_4();
	afx_msg void OnMenu_Reset_Stat_Tsch_5();
	afx_msg void OnMenu_Reset_Stat_Tsch_6();
	afx_msg void OnMenu_Reset_Stat_Tsch_7();
	afx_msg void OnMenu_Reset_Stat_Tsch_8();
	afx_msg void OnMenu_Reset_Stat_Tsch_9();
	afx_msg void OnMenu_Reset_Stat_Tsch_10();

	afx_msg void OnMenu_EXP_CONF();//导出配置
	afx_msg void OnMenu_IMP_CONF();//导入配置

	afx_msg void OnMenu_Write_Flash();//MTK 写合法ID授权
	afx_msg void OnMenu_DL_Unlock();//工厂下载授权

	afx_msg void OnMenu_M_LOAD_FW();//手动加载固件授权
	afx_msg void OnMenu_ODM_SPC_NOT_DL();//飞天QDC506加密部分固件不下载授权




	
		

	afx_msg void OnUpdate_Dll(void);
	afx_msg void OnUpdate_Tool(void);//ID_UPDATE_TOOL

	UpdateConfig m_UpdateConfig;
	CString m_csUpdateDLL_EXE_File_Path;
	afx_msg void OnPaint();

	CFtp m_cls_Ftp_Interface;//FTP对象 
	Tool_Update_Config m_cls_Tool_Update_Config;//自动升级配置类
	bool Auto_Update_Tool (); 
	void DoExcute_Update_Tool (); 
	bool Compare_Tool_Local_and_FTP_Version ( const CString csLocal_Version , const CString csFTP_Version ); 

	CString   m_csLocal_Tool_Version;//当前这个工具的版本号 VX.XX.XX
	afx_msg void OnSaharaonly();
	afx_msg void OnUpdateSaharaonly(CCmdUI *pCmdUI);

	BOOL   m_bSHOWMESH;

	logview  m_logviewDlg;

	

};

#ifndef _DEBUG  // debug version in MTView.cpp
inline CMTDoc* CMTView::GetDocument() const
   { return reinterpret_cast<CMTDoc*>(m_pDocument); }
#endif
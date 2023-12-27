
// MT.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "MT.h"
#include "MainFrm.h"

#include "MTDoc.h"
#include "MTView.h"
#include "afxwin.h"

// #include <io.h>
// 
// #include <fcntl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



//主窗口显示标题名
CString g_csMain_Title_Name = _T("");


// CMTApp

BEGIN_MESSAGE_MAP(CMTApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMTApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	
END_MESSAGE_MAP()


// CMTApp construction

CMTApp::CMTApp()
{

	m_bHiColorIcons = TRUE;

	m_bExitFlag = false;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CMTApp object

CMTApp theApp;


// CMTApp initialization

BOOL CMTApp::InitInstance()
{
	//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	

	//------检查是否僵尸进程，不允许重复打开------------------------------------------------------------------
	//当前程序路径
	m_bExitFlag = false;
#ifdef FW_MULTI

	wchar_t wcrCurr_Exe_Path[MAX_PATH];
	wmemset(wcrCurr_Exe_Path, 0, MAX_PATH);
	::GetCurrentDirectory(MAX_PATH, wcrCurr_Exe_Path);

	int iLong = wcslen(wcrCurr_Exe_Path);

	if(wcrCurr_Exe_Path[iLong - 1] == '\\')
	{
		wcrCurr_Exe_Path[iLong - 1] = '\0';
	}

	CIniFile cla_IniFile_Basic;
	cla_IniFile_Basic.m_csIniFileName.Format(_T("%s\\Params\\eco.ini"), wcrCurr_Exe_Path);
	::g_csMain_Title_Name = cla_IniFile_Basic.ReadStr(_T("MAIN_WINDOWS"), _T("NAME"), _T(""));
#else
	::g_csMain_Title_Name = _T("QFlash_V4.21");
#endif

	
	CreateMutex ( NULL , true , ::g_csMain_Title_Name);
	if ( GetLastError () == ERROR_ALREADY_EXISTS )
	{
		AfxMessageBox ( _T ( "应用程序不可以重复启动" ) , MB_OK | MB_APPLMODAL | MB_ICONSTOP );
		m_bExitFlag = true;
		return FALSE;
	}

	//------------------------------------------------------------------------
	AfxInitRichEdit();//xjli


	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();


	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
#ifdef FW_MULTI
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMTDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMTView));
#else
	pDocTemplate = new CSingleDocTemplate(
		IDR_MENU_QFLASH,
		RUNTIME_CLASS(CMTDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMTView));
#endif
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
#ifdef FW_MULTI

#ifdef FW_UPGRADE//多口 ＋ 升级操作
	m_pMainWnd->SetWindowText( ::g_csMain_Title_Name );
#else//多口工具 + 下载 ＋ 格式化操作
	m_pMainWnd->SetWindowText( ::g_csMain_Title_Name );
#endif

#else//#elif 单口下载

	::g_csMain_Title_Name = _T("QFlash_V4.21");
	
	m_pMainWnd->SetWindowText( ::g_csMain_Title_Name );
#endif


// 	AfxGetApp()->ParseCommandLine (cmdInfo);
// 	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
// 	{
// 		//Don't show the main window if Application 
// 		//was run with /Embedding or /Automation.
// 		
// 		return TRUE;
// 	}

	m_pMainWnd->ShowWindow(SW_SHOW);


	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_ctr_staAbout_Name;
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_NAME, m_ctr_staAbout_Name);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CMTApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMTApp customization load/save methods

void CMTApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CMTApp::LoadCustomState()
{
}

void CMTApp::SaveCustomState()
{
}

// CMTApp message handlers




int CMTApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class

	if (false == m_bExitFlag)
	{
		AfxOleTerm(FALSE);
		this->CleanState();
	}
	 
	return CWinAppEx::ExitInstance();
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

#ifdef FW_MULTI

#ifdef FW_UPGRADE//多口 ＋ 升级操作
	this->m_ctr_staAbout_Name.SetWindowTextW( ::g_csMain_Title_Name );
#else//多口工具 + 下载 ＋ 格式化操作
	this->m_ctr_staAbout_Name.SetWindowTextW( ::g_csMain_Title_Name );
#endif

#else//#elif 单口下载
	this->m_ctr_staAbout_Name.SetWindowTextW( _T("QFlash_V4.21") );
#endif
	char str[]="\x0A9\x00\x00\x00";
	::SetWindowTextW(GetDlgItem(IDC_STATIC_VS)->GetSafeHwnd(), (LPCWSTR)str);
	
	CString strVer;
    GetDlgItem(IDC_STATIC_VS)->GetWindowText(strVer);
	strVer+=_T("Quectel 2020 All rights reserved");
	::SetWindowTextW(GetDlgItem(IDC_STATIC_VS)->GetSafeHwnd(), strVer);





	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

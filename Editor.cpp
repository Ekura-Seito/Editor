// Editor.cpp : �A�v���P�[�V�����p�N���X�̋@�\��`���s���܂��B
//

#include "stdafx.h"
#include <Afxtempl.h>			//�l�e�b�̂b�l�������g�p����

#define MAIN

#include <afxadv.h>				//for CRecentFileList
#include <shlobj.h>				//for CreateShortCut
#include <afxdisp.h>			//for AfxOleInit
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include <locale.h>
#include <direct.h>
#include <io.h>					// for _access
#include <dos.h>
#include <ddeml.h>				// for DDE
#include <winsock.h>
#include <Dlgs.h>
#include <CDERR.H>				// for CommDlgExtendedError
#include <mmsystem.h>			// for wav file play
#include "Editor_Def.h"
#include "StatusBarDisp.h"		//�X�e�[�^�X�o�[�\�����\����
#include "ColorManager.h"
#include "Line.h"
#include "LineManager.h"
#include "Editor.h"
#include "BitmapClient.h"
#include "Operation.h"
#include "OperationManager.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "UndoItem.h"
#include "UndoManager.h"
#include "EditorDoc.h"
#include "EditorView.h"
#include "Hyperlink.h"
#include "Label.h"
#include "AboutDlg.h"
#include "SetLicenseDlg.h"
#include "Mymd5.h"
#include "Myfeal.h"
#include "Splash.h"
#include "CheckExecPasswdDlg.h"
#include "DdeServer.h"
#include "KeyBind.h"
#include "KeyBindManager.h"
#include "FileDialogEx.h"
#include "DdeClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorApp

BEGIN_MESSAGE_MAP(CEditorApp, CWinApp)
	//{{AFX_MSG_MAP(CEditorApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_ALLSAVE, OnFileAllsave)
	ON_UPDATE_COMMAND_UI(ID_FILE_ALLSAVE, OnUpdateFileAllsave)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPENONEXIT, OnUpdateFileOpenonexit)
	ON_COMMAND(ID_FILE_OPENONEXIT, OnFileOpenonexit)
	ON_COMMAND(ID_HELP_LICENSE, OnHelpLicense)
	ON_COMMAND(ID_HELP_CONTENT, OnHelpContent)
	ON_UPDATE_COMMAND_UI(ID_HELP_CONTENT, OnUpdateHelpContent)
	ON_COMMAND(ID_ALLSAVE_EXIT, OnAllsaveExit)
	ON_UPDATE_COMMAND_UI(ID_ALLSAVE_EXIT, OnUpdateAllsaveExit)
	ON_COMMAND(ID_HELP_CONTENT2, OnHelpContent2)
	ON_UPDATE_COMMAND_UI(ID_HELP_CONTENT2, OnUpdateHelpContent2)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	// �W���̈���Z�b�g�A�b�v �R�}���h
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorApp �N���X�̍\�z

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CEditorApp::CEditorApp()
{
	m_bIsDriveRoot = FALSE;
	m_unOrgCaretBlinkTime=0;
	m_nPrintOrientation=0;
	m_nMaxMRU = 0;
	m_bIsOpenNewFile = 0;
	m_bIsChdirToOpenFile = 0;
	m_bShowSplash = 0;
	m_fpLogging = NULL;
	m_cBuf4MyReadString = 0;
	m_pobjDdeServer = NULL;
	m_hRasDllInstance = NULL;
	m_hSockDllInstance = NULL;
	m_hRegexDllInstance = NULL;
	m_lpfnRasFunc1 = NULL;
	m_lpfnRasFunc2 = NULL;
	m_lpfnRasFunc3 = NULL;
	m_lpfnRasFunc4 = NULL;
	m_lpfnRasFunc5 = NULL;
	m_lpfnSckFunc1 = NULL;
	m_lpfnSckFunc2 = NULL;
	m_lpfnSckFunc3 = NULL;
	m_lpfnSckFunc4 = NULL;
	m_lpfnSckFunc5 = NULL;
	m_lpfnSckFunc6 = NULL;
	m_lpfnSckFunc7 = NULL;
	m_lpfnSckFunc8 = NULL;
	m_lpfnSckFunc9 = NULL;
	m_lpfnSckFunc10 = NULL;
	m_lpfnRegFunc1 = NULL;
	m_lpfnRegFunc2 = NULL;
	m_lpfnRegFunc3 = NULL;
	m_lpfnRegFunc4 = NULL;
	m_lpfnRegFunc5 = NULL;
	m_hMutex = NULL;
	m_pobjKeyBindManager = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEditorApp �I�u�W�F�N�g

CEditorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEditorApp �N���X�̏�����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�C���X�^���X������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::InitInstance()
{
	//�n�k�d�Q�D�O������
	if(!AfxOleInit())
		return FALSE;

#ifdef _AFXDLL
	Enable3dControls();			// ���L DLL �̒��� MFC ���g�p����ꍇ�ɂ͂������Ăяo���Ă��������B
#else
	Enable3dControlsStatic();	// MFC �ƐÓI�Ƀ����N���Ă���ꍇ�ɂ͂������Ăяo���Ă��������B
#endif

	// �ݒ肪�ۑ�����鉺�̃��W�X�g�� �L�[��ύX���܂��B
	// ��Ж��܂��͏����ȂǁA�K�؂ȕ������
	// �ύX���Ă��������B
	SetRegistryKey(_T("S.Ekura"));
	m_nMaxMRU = GetProfileInt("CEditorApp","nMaxMRU",4);
	LoadStdProfileSettings(m_nMaxMRU);  // �W���� INI �t�@�C���̃I�v�V���������[�ނ��܂� (MRU ���܂�)

	//�N���p�X���[�h�`�F�b�N
	if( GetProfileInt("CEditorApp","bIsExecPasswd",0) != FALSE )
	{
		CCheckExecPasswdDlg	objCheckExecPasswdDlg;
		objCheckExecPasswdDlg.DoModal();
		if(objCheckExecPasswdDlg.IsPasswdOK() == FALSE)
			return FALSE;
	}

	// CG: ���̃u���b�N�́u�X�v���b�V�� �X�N���[���v�R���|�[�l���g�ɂ��ǉ�����Ă��܂��B
	m_bShowSplash = GetProfileInt("CEditorApp","bShowSplash",TRUE);
	if(m_bShowSplash == FALSE && m_bShowSplash != FALSE)
		m_bShowSplash = TRUE;
	{
//		CCommandLineInfo cmdInfo;
//		ParseCommandLine(cmdInfo);
//		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
		CSplashWnd::EnableSplashScreen(m_bShowSplash);
	}

	// �N���f�B���N�g�����
	getMyDirectory();

	//�]���������`�F�b�N
	IsExpired(FALSE);

	// �L�����b�g�_�ŊԊu�I���W�i���l�擾
	m_unOrgCaretBlinkTime = ::GetCaretBlinkTime();

	//�A�v���P�[�V�����̓�d�N���h�~�̂��߂̏���
	BOOL bIsNoDoubleExecute = GetProfileInt("CEditorApp","bIsNoDoubleExecute",0);
	if( bIsNoDoubleExecute != 0 )
	{
		m_hMutex = ::CreateMutex(NULL,TRUE,m_pszExeName);
		if(GetLastError()==ERROR_ALREADY_EXISTS)
		{
			//�O�̃C���X�^���X�̃��C���E�C���h�E��T��
			CWnd*	pPrevWnd=CWnd::GetDesktopWindow()->GetWindow(GW_CHILD);
			while(pPrevWnd)
			{
				//�u�ŏ��̃C���X�^���X�^�O�v������E�C���h�E��
				if(::GetProp(pPrevWnd->GetSafeHwnd(),m_pszExeName))
				{
					//�E�C���h�E��T���A�t�H�[�J�X���E�C���h�E�ɐݒ肷��
					//�E�C���h�E�����݃A�C�R��������Ă���ꍇ�́A���̃T�C�Y�ɖ߂�
					if(pPrevWnd->IsIconic())
						pPrevWnd->ShowWindow(SW_RESTORE);

					//�t�H�[�J�X�����C���E�C���h�E�ɐݒ肷��
//					pPrevWnd->SetForegroundWindow();
					MySetForegroundWindow(pPrevWnd->m_hWnd);

					//�E�C���h�E�Ƀ|�b�v�A�b�v�E�C���h�E������ꍇ�́A
					//�t�H�[�J�X���|�b�v�A�b�v�ɐݒ肷��
//					pPrevWnd->GetLastActivePopup()->SetForegroundWindow();
					MySetForegroundWindow(pPrevWnd->GetLastActivePopup()->m_hWnd);

					//�c�c�d�Ńt�@�C���I�[�v��
					if(__argc >= 2)
					{
						if( (_access( __argv[1], 0 )) != -1 )
							TryDDE( __argv[1] );
					}

					return FALSE;
				}
				//�E�C���h�E��������Ȃ���΁A���X�g�ɂ��鎟�̃E�C���h�E��\������
				pPrevWnd=pPrevWnd->GetWindow(GW_HWNDNEXT);
			}
			return FALSE;
		}
	}

	// �A�v���P�[�V�����p�̃h�L�������g �e���v���[�g��o�^���܂��B�h�L�������g �e���v���[�g
	//  �̓h�L�������g�A�t���[�� �E�B���h�E�ƃr���[���������邽�߂ɋ@�\���܂��B
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_EDITORTYPE,
		RUNTIME_CLASS(CEditorDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CEditorView));
	AddDocTemplate(pDocTemplate);

	// ���C�� MDI �t���[�� �E�B���h�E���쐬
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// This code replaces the MFC created menus with the Ownerdrawn versions 
	pDocTemplate->m_hMenuShared=pMainFrame->NewMenu();
	pMainFrame->m_hMenuDefault=pMainFrame->NewDefaultMenu();

	// This simulates a window being opened if you don't have
	// a default window displayed at startup
	pMainFrame->OnUpdateFrameMenu(pMainFrame->m_hMenuDefault);

	// �h���b�O/�h���b�v �̃I�[�v���������܂�
	m_pMainWnd->DragAcceptFiles();

	// DDE Execute open ���g�p�\�ɂ��܂��B
	EnableShellOpen();
//	RegisterShellFileTypes(TRUE);

	// DDE�Afile open �ȂǕW���̃V�F�� �R�}���h�̃R�}���h���C������͂��܂��B
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	m_bIsOpenNewFile = GetProfileInt("CEditorApp","bIsOpenNewFile",1);
	if( m_bIsOpenNewFile != 1 )
	{
		//�N�����ɐV�K�q�E�C���h�E��\�����Ȃ�
		m_bIsOpenNewFile = 0;
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	}

	// �R�}���h���C���Ńf�B�X�p�b�` �R�}���h���w�肵�܂��B
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���܂��B
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// �N���p�����[�^���
	paramAnalyze(cmdInfo);

	//�T�E���h
	if(GetProfileInt("Sound","IsPlayOnXerxesStart",0) != 0)
	{
		//Wave�t�@�C���p�X�擾
		CString strPath;
		strPath.Format("%s\\%s",m_strMediaDirPath,(LPCTSTR)GetProfileString("Sound","WaveFileOnXerxesStart",""));
		if(_access((LPCTSTR)strPath,0) != -1)
			MyPlaySound(strPath,FALSE);
	}

	// ���P�[���ݒ�i���{����܂ޕ����t�@�C���̃I�[�v���p�j
//	setlocale(LC_ALL,"jpn");

	// �ݒ�擾
	m_strOpenFileDirectory = (LPCTSTR)GetProfileString("CEditorApp","OpenFileDirectory","");
	m_bIsChdirToOpenFile = GetProfileInt("CEditorApp","IsChdirToOpenFile",0);
	if( m_bIsChdirToOpenFile != 0 )
		m_bIsChdirToOpenFile = 1;
	if( m_bIsChdirToOpenFile != 0 )
	{
		if( !m_strOpenFileDirectory.IsEmpty() )
			::chdir((LPCTSTR)m_strOpenFileDirectory);
	}
	else
	{
		if( !m_strExePath.IsEmpty() )
			::chdir((LPCTSTR)m_strExePath);
	}

	//�^�O�ƃE�C���h�E���֘A�t���A��ŒT����悤�ɂ���
	::SetProp(m_pMainWnd->GetSafeHwnd(),m_pszExeName,(HANDLE)1);

#ifdef _DEBUG
//	//���O�t�@�C���I�[�v��
//	m_fpLogging = fopen("Editor.log","w");
//	m_fpLogging = fopen("Editor.log","a");
//	if(m_fpLogging!=NULL)
//	{
//		fputs("\r\n",m_fpLogging);
//		fputs("-----------------------------------------------------------------------\r\n",m_fpLogging);
//		fputs("\r\n",m_fpLogging);
//	}
#endif

	//����]���̃}�[�W���ݒ�
	CRect		objMarginRect(0,0,0,0);
	PRINTDLG	stPrintDlg;
	if(GetPrinterDeviceDefaults(&stPrintDlg)!=FALSE) //�v�����^�C���X�g�[���ς�
	{
		CPageSetupDialog objPageSetupDialog(PSD_MARGINS|PSD_INTHOUSANDTHSOFINCHES|PSD_RETURNDEFAULT);
		objPageSetupDialog.DoModal(); //�f�t�H���g�}�[�W���擾
		objPageSetupDialog.GetMargins(&objMarginRect,NULL);
		objMarginRect.NormalizeRect();
	}
	m_objPrintMarginRect.left   = GetProfileInt("CEditorApp","PrintMarginRectLeft",  objMarginRect.left);
	m_objPrintMarginRect.top    = GetProfileInt("CEditorApp","PrintMarginRectTop",   objMarginRect.top);
	m_objPrintMarginRect.right  = GetProfileInt("CEditorApp","PrintMarginRectRight", objMarginRect.right);
	m_objPrintMarginRect.bottom = GetProfileInt("CEditorApp","PrintMarginRectBottom",objMarginRect.bottom);

	//����p�������ݒ�
	m_nPrintOrientation = GetProfileInt("CEditorApp","PrintOrientation",PRINTORIENTATION_TATE);
	if(m_nPrintOrientation!=PRINTORIENTATION_TATE && m_nPrintOrientation!=PRINTORIENTATION_YOKO)
		m_nPrintOrientation = PRINTORIENTATION_TATE;
	SetPrintOrientation(m_nPrintOrientation);

	//�V�F���G�N�X�e���V�����c�k�k�̓o�^
	if(GetProfileInt("CEditorApp","bIsEnableShellOpen",0) != 0)
		RegisterShellExtDll();

#ifdef _DEBUG
//	//���C�Z���X�L�[�쐬
//	GenerateLisenceKeys();
#endif

	//for DDE
	m_pobjDdeServer = new CDdeServer();
	m_pobjDdeServer->SetMainWnd(m_pMainWnd);

	//�q�`�r�c�k�k�̖����I�����N
	HINSTANCE hDll = ::LoadLibrary("rasapi32.dll");
	if(hDll != NULL) //�_�C�A���A�b�v�l�b�g���[�N�C���X�g�[���ς�
	{
		::FreeLibrary(hDll);
		m_hRasDllInstance = ::LoadLibrary("ras_dll.dll");
	}

	//�L�[�o�C���h
	m_pobjKeyBindManager = new CKeyBindManager();

	//�h�d�փ��j���[�o�^
	SetMenuOfIE(TRUE);

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�C���X�^���X�I��														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorApp::ExitInstance() 
{
	//�T�E���h
	if(GetProfileInt("Sound","IsPlayOnXerxesEnd",0) != 0)
	{
		//Wave�t�@�C���p�X�擾
		CString strPath;
		strPath.Format("%s\\%s",m_strMediaDirPath,(LPCTSTR)GetProfileString("Sound","WaveFileOnXerxesEnd",""));
		if(_access((LPCTSTR)strPath,0) != -1)
			MyPlaySound((LPCTSTR)strPath,TRUE);
	}

	//�ݒ�ۑ�
	WriteProfileInt("CEditorApp","nMaxMRU",m_nMaxMRU);
	WriteProfileString("CEditorApp","OpenFileDirectory",m_strOpenFileDirectory);
	WriteProfileInt("CEditorApp","bIsOpenNewFile",m_bIsOpenNewFile);
	WriteProfileInt("CEditorApp","IsChdirToOpenFile",m_bIsChdirToOpenFile);
	WriteProfileInt("CEditorApp","bShowSplash",m_bShowSplash);
	WriteProfileInt("CEditorApp","PrintOrientation",m_nPrintOrientation);
	WriteProfileInt("CEditorApp","PrintMarginRectLeft",  m_objPrintMarginRect.left);
	WriteProfileInt("CEditorApp","PrintMarginRectTop",   m_objPrintMarginRect.top);
	WriteProfileInt("CEditorApp","PrintMarginRectRight", m_objPrintMarginRect.right);
	WriteProfileInt("CEditorApp","PrintMarginRectBottom",m_objPrintMarginRect.bottom);

	//�c�c�d�I��
	if(m_pobjDdeServer != NULL)
		delete m_pobjDdeServer;

	// �L�����b�g�_�ŊԊu�I���W�i���l����
	if(m_unOrgCaretBlinkTime != 0)
		::SetCaretBlinkTime(m_unOrgCaretBlinkTime);

	//�q�`�r�c�k�k�A�������N
	if(m_hRasDllInstance != NULL)
		::FreeLibrary(m_hRasDllInstance);

	//�\�P�b�g�c�k�k�A�������N
	if(m_hSockDllInstance != NULL)
		::FreeLibrary(m_hSockDllInstance);

	//���K�\���c�k�k�A�������N
	if(m_hRegexDllInstance != NULL)
		::FreeLibrary(m_hRegexDllInstance);

	if(m_hMutex != NULL)
		::CloseHandle(m_hMutex);
	m_hMutex = NULL;

	//�L�[�o�C���h�I��
	if(m_pobjKeyBindManager != NULL)
		delete m_pobjKeyBindManager;

#ifdef _DEBUG
//	//���O�t�@�C���N���[�Y
//	if(m_fpLogging!=NULL)
//		fclose(m_fpLogging);
#endif

	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CEditorApp ���b�Z�[�W�n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���b�Z�[�W���f�B�X�p�b�`�����O�ɂ�т�����鏈��					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::PreTranslateMessage(MSG* pMsg)
{
	CSplashWnd::PreTranslateAppMessage(pMsg);
	return CWinApp::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorApp �R�}���h

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���|�V�K�쐬�v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::OnFileNew() 
{
	//�T�E���h
	if(GetProfileInt("Sound","IsPlayOnFileNew",0) != 0)
	{
		//Wave�t�@�C���p�X�擾
		CString strPath;
		strPath.Format("%s\\%s",m_strMediaDirPath,(LPCTSTR)GetProfileString("Sound","WaveFileOnFileNew",""));
		if(_access((LPCTSTR)strPath,0) != -1)
			MyPlaySound((LPCTSTR)strPath,FALSE);
	}

	CWinApp::OnFileNew();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���|�J���v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::OnFileOpen() 
{
	int nRet=0;
	CString objStrFName=_T(""), objMsg;
	CString strPath;
	char szBuf[_MAX_PATH];
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();

	CString	objFilter = _T("");
	objFilter.LoadString(IDS_FILTER_ALLFILE);
	objFilter += _T("Text (*.txt)|*.txt|");
	objFilter += _T("HTML (*.htm;*.html;*.shtml;*.shtm)|*.htm;*.html;*.shtml;*.shtm|");
	objFilter += _T("C/C++ Source (*.c;*.cpp)|*.c;*.cpp|");
	objFilter += _T("C/C++ Header (*.h)|*.h|");
	objFilter += _T("Assembler Source (*.asm)|*.asm|");
	objFilter += _T("Active Server Page (*.asp)|*.asp|");
	objFilter += _T("VB Form (*.frm)|*.frm|");
	objFilter += _T("VB Module (*.bas)|*.bas|");
	objFilter += _T("Oracle ProC/C++ (*.pc)|*.pc|");
	objFilter += _T("Log (*.log)|*.log|");
	objFilter += _T("Data (*.dat)|*.dat|");
	objMsg.LoadString(IDS_FILTER_BATFILE); 
	objFilter += objMsg;
	objMsg.LoadString(IDS_FILTER_INIFILE);
	objFilter += objMsg;
	objMsg.LoadString(IDS_FILTER_MAKFILE);
	objFilter += objMsg;
	objFilter += _T("Java (*.java;*.h)|*.java;*.h|");
	objFilter += _T("Yacc/Lex (*.y;*.l)|*.y;*.l|");
	objFilter += _T("VRML (*.wrl)|*.wrl||");

	//�f�B���N�g���ړ�
	if( m_bIsChdirToOpenFile != 0 )
	{
		if( !m_strOpenFileDirectory.IsEmpty() )
			::chdir((LPCTSTR)m_strOpenFileDirectory);
	}

	//�����t�@�C���I�[�v�����\�ɂ���
	DWORD dwFlags = ( OFN_ALLOWMULTISELECT |
					  OFN_OVERWRITEPROMPT  |
					  OFN_EXPLORER		   |
					  OFN_LONGNAMES        |
					  OFN_FILEMUSTEXIST );

	//�ߋ��ɊJ�����t�@�C�����擾
	strPath = (LPCTSTR)GetProfileString("CEditorApp","OpenFileName","");
	if(_access((LPCTSTR)strPath,0) == -1)
		strPath = "";

	//�t�@�C���I�[�v���R�����_�C�A���O�\��
	CFileDialog		objFileDialog(	TRUE,				//OPEN
									NULL,				//�f�t�H���g�t�@�C���g���q
									NULL,				//�f�t�H���g�t�@�C����
									dwFlags,			//�I�[�v���t���O
									objFilter,			//�t�B���^
									NULL );				//�e�E�C���h�E�n���h��
	char*	szFileNameBuf = new char[65535];
	memset(szFileNameBuf,0,65535);
	strcpy(szFileNameBuf,(LPCTSTR)strPath);
	objFileDialog.m_ofn.lpstrFile = szFileNameBuf;
	objFileDialog.m_ofn.nMaxFile = 65535;
	nRet = objFileDialog.DoModal();
	if(nRet != IDOK)
	{
		if(szFileNameBuf != NULL)
			delete[] szFileNameBuf;
		return;
	}

	POSITION	pos = objFileDialog.GetStartPosition();
	for(; pos!=0; )
	{
		//�I�������t�@�C�����I�[�v��
		objStrFName = objFileDialog.GetNextPathName(pos);
		OpenDocumentFile((char*)(LPCTSTR)objStrFName);

		if(objFileDialog.GetReadOnlyPref())//�ǂݎ���p�Ƃ��ĊJ��
		{
			POSITION pos1 = GetFirstDocTemplatePosition();
			while(pos1 != 0)
			{
				CDocTemplate* pTempl = GetNextDocTemplate(pos1);
				POSITION pos2 = pTempl->GetFirstDocPosition();
				while(pos2 != 0)
				{
					CEditorDoc* pDoc = (CEditorDoc*)pTempl->GetNextDoc(pos2);
					if( pDoc != 0 )
					{
						memset(szBuf,0,sizeof(szBuf));
						pDoc->MyGetFilePath(szBuf);
						if( !objStrFName.CompareNoCase(szBuf) )
						{
							pDoc->SetReadOnly2();
							pWnd->SetReadOnlyMode(TRUE);
						}
					}
				}
			}
		}
	}

	if(szFileNameBuf != NULL)
		delete[] szFileNameBuf;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���|���ׂĕۑ��v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::OnFileAllsave() 
{
	POSITION pos1 = GetFirstDocTemplatePosition();
	while(pos1 != 0)
	{
		CDocTemplate* pTempl = GetNextDocTemplate(pos1);
		POSITION pos2 = pTempl->GetFirstDocPosition();
		while(pos2 != 0)
		{
			CEditorDoc* pDoc = (CEditorDoc*)pTempl->GetNextDoc(pos2);
			if( (pDoc != 0) && !pDoc->IsReadOnly() )
			{
				pDoc->OnFileSave();
			}
		}
	}
}
void CEditorApp::OnUpdateFileAllsave(CCmdUI* pCmdUI) 
{
	BOOL bExistNotReadOnly=TRUE; //�����݉̃t�@�C�����P�͂���

	POSITION pos1 = GetFirstDocTemplatePosition();
	if(pos1 != 0)
	{
		while(pos1 != 0)
		{
			CDocTemplate* pTempl = GetNextDocTemplate(pos1);
			POSITION pos2 = pTempl->GetFirstDocPosition();
			while(pos2 != 0)
			{
				CEditorDoc* pDoc = (CEditorDoc*)pTempl->GetNextDoc(pos2);
				if( (pDoc != 0) && !pDoc->IsReadOnly() )
				{
					pCmdUI->Enable(TRUE);
					return;
				}
			}
		}
		pCmdUI->Enable(FALSE);
		return;
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���|�O��I������̧�ق��J���v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::OnFileOpenonexit() 
{
	int nCnt=0;
	int nFileCnt = GetProfileInt("CEditorApp","FCountOnExit",0);
	char	szEntry[32];
	CString strPath;

	for(nCnt=0;nCnt<nFileCnt;nCnt++)
	{
		sprintf(szEntry,"FnameOnExit%d",nCnt);
		strPath = (LPCTSTR)GetProfileString("CEditorApp",szEntry,"");
		if( (_access( (LPCTSTR)strPath, 0 )) != -1 )
		{
			OpenDocumentFile((LPCTSTR)strPath);
		}
	}
}
void CEditorApp::OnUpdateFileOpenonexit(CCmdUI* pCmdUI) 
{
	int nCnt=0;
	int nFileCnt = GetProfileInt("CEditorApp","FCountOnExit",0);
	char	szEntry[32];
	CString strPath;

	if( nFileCnt == 0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	for(nCnt=0;nCnt<nFileCnt;nCnt++)
	{
		sprintf(szEntry,"FnameOnExit%d",nCnt);
		strPath = (LPCTSTR)GetProfileString("CEditorApp",szEntry,"");
		if( (_access( (LPCTSTR)strPath, 0 )) != -1 )
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}

	pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���|���ׂĕۑ����I���v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::OnAllsaveExit() 
{
	//�t�@�C�������ׂĕۑ�
	DoFileAllSave();

	//�I��
	AfxGetMainWnd()->PostMessage(WM_CLOSE);
}
void CEditorApp::OnUpdateAllsaveExit(CCmdUI* pCmdUI) 
{
}

///////////////////////////////////////////////////////////////////////////////////
// �w���v�@�|�@���C�Z���X���֘A

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�w���v�@�|�@�ڎ��v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::OnHelpContent() 
{
	CString strBuf="";
	CString strPath;

	//�w���v�t�@�C���̑��݊m�F
	if(!IsDriveRoot())
		strBuf = "\\";
	strPath.Format("%s%s%s.hlp",m_strExePath,strBuf,m_pszAppName);
	if( _access( (LPCTSTR)strPath, 0 ) == -1 )
		return;

	//�w���v�t�@�C���I�[�v��
	::WinHelp( NULL,			//�e�E�C���h�E�n���h��
			   (LPCTSTR)strPath,//�w���v�t�@�C����
			   HELP_FINDER,		//�R�}���h
			   0 );
}
void CEditorApp::OnUpdateHelpContent(CCmdUI* pCmdUI) 
{
	CString strBuf="";
	CString strPath;

	//�w���v�t�@�C���̑��݊m�F
	if(!IsDriveRoot())
		strBuf = "\\";
	strPath.Format("%s%s%s.hlp",m_strExePath,strBuf,m_pszAppName);
	if( _access( (LPCTSTR)strPath, 0 ) != -1 )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�g�s�l�k�w���v�@�|�@�ڎ��v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::OnHelpContent2() 
{
	CString strBuf="";
	CString strPath;
	CString objMsg;

	//�w���v�t�@�C���̑��݊m�F
	if(!IsDriveRoot())
		strBuf = "\\";
	strPath.Format("%s%s%s.chm",m_strExePath,strBuf,m_pszAppName);
	if( _access( (LPCTSTR)strPath, 0 ) == -1 )
		return;

	//�w���v�t�@�C���I�[�v��
	HINSTANCE result = ShellExecute( NULL, "open", (LPCTSTR)strPath, NULL, (LPCTSTR)m_strExePath, SW_SHOW );
	if((UINT)result <= HINSTANCE_ERROR)
	{
		objMsg.LoadString(IDS_HTMLHELP_OPEN_ERR);
		MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
	}
}
void CEditorApp::OnUpdateHelpContent2(CCmdUI* pCmdUI) 
{
	CString strBuf="";
	CString strPath;

	//�w���v�t�@�C���̑��݊m�F
	if(!IsDriveRoot())
		strBuf = "\\";
	strPath.Format("%s%s%s.chm",m_strExePath,strBuf,m_pszAppName);
	if( _access( (LPCTSTR)strPath, 0 ) != -1 )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�w���v�@�|�@�o�[�W�������v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;

//	if(IsLicenseRegistered() == FALSE) //���C�Z���X���o�^
//	{
//		aboutDlg.m_objKaisya = "���C�Z���X���o�^";
//		if(IsExpired(FALSE) == FALSE) //���p�������؂�Ă���
//			aboutDlg.m_objKaisya += "  ���p�����؂�";
//		aboutDlg.m_objUser   = _T("");
//	}
//	else //���C�Z���X�o�^�ς�
//	{
		aboutDlg.m_objKaisya = GetProfileString("LICENSE","KAISYA","");
		aboutDlg.m_objUser   = GetProfileString("LICENSE","USER","");
//	}

	//�o�[�W�������_�C�A���O�\��
	aboutDlg.DoModal();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�w���v�v�|�u���C�Z���X�o�^�v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::OnHelpLicense() 
{
//	if(IsLicenseRegistered() == FALSE) //���C�Z���X���o�^
//	{
		CSetLicenseDlg	objSetLicenseDlg;
		objSetLicenseDlg.DoModal();
//	}
//	else //���C�Z���X�o�^�ς�
//	{
//		CString		objUser=_T("");
//		objUser = GetProfileString("LICENSE","USER","");
//		CString	objMsg;
//		objMsg = "���̃\�t�g�E�G�A��";
//		objMsg += objUser;
//		objMsg += "����Ƀ��C�Z���X����Ă��܂��B";
//		MyMessageBox((LPCTSTR)objMsg,(MB_OK|MB_ICONINFORMATION),0);
//	}
}

/////////////////////////////////////////////////////////////////////////////
// CEditorApp �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w��E�C���h�E��Foreground����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::MySetForegroundWindow(HWND hWnd)
{
	HWND hWndNow=0;
	DWORD dwPid=0;
	DWORD dwTidNow=0;
	DWORD dwTidMe=0;

	hWndNow = ::GetForegroundWindow();
	dwTidNow = GetWindowThreadProcessId(hWndNow, &dwPid);
	dwTidMe = GetCurrentThreadId();
	AttachThreadInput(dwTidMe, dwTidNow, TRUE);
	::SetForegroundWindow(hWnd);
	AttachThreadInput(dwTidNow, dwTidMe, FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�\���I�v�V�����̑S�E�C���h�E���f����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::SetFlag2AllWindow
	(	CString&	objFileExt,
		BOOL&		bDispLineNo,
		BOOL&		bDispRuler,
		BOOL&		bDispCRLF,
		BOOL&		bDispTab,
		BOOL&		bDispZenkakuSpace,
		BOOL&		bDispUnderLine
	)
{
	CString objExt;
	POSITION pos1 = GetFirstDocTemplatePosition();
	while(pos1 != 0)
	{
		CDocTemplate* pTempl = GetNextDocTemplate(pos1);
		POSITION pos2 = pTempl->GetFirstDocPosition();
		while(pos2 != 0)
		{
			CEditorDoc* pDoc = (CEditorDoc*)pTempl->GetNextDoc(pos2);
			if(pDoc == NULL)
				continue;

			objExt = pDoc->MyGetFileExt();
			if(objFileExt.IsEmpty())
			{
				if(!objExt.IsEmpty())
					continue;
			}
			else if(!objFileExt.CompareNoCase("txt"))
			{
				if(objExt.CompareNoCase("txt"))
					continue;
			}
			else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
					!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
			{
				if(objExt.CompareNoCase("htm")   && objExt.CompareNoCase("html") &&
				   objExt.CompareNoCase("shtml") && objExt.CompareNoCase("shtm") )
					continue;
			}
			else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
			{
				if(objExt.CompareNoCase("c") && objExt.CompareNoCase("cpp"))
					continue;
			}
			else if(!objFileExt.CompareNoCase("h"))
			{
				if(objExt.CompareNoCase("h"))
					continue;
			}
			else if(!objFileExt.CompareNoCase("asm"))
			{
				if(objExt.CompareNoCase("asm"))
					continue;
			}
			else if(!objFileExt.CompareNoCase("asp"))
			{
				if(objExt.CompareNoCase("asp"))
					continue;
			}
			else if(!objFileExt.CompareNoCase("frm"))
			{
				if(objExt.CompareNoCase("frm"))
					continue;
			}
			else if(!objFileExt.CompareNoCase("bas"))
			{
				if(objExt.CompareNoCase("bas"))
					continue;
			}
			else if(!objFileExt.CompareNoCase("pc"))
			{
				if(objExt.CompareNoCase("pc"))
					continue;
			}
			else if(!objFileExt.CompareNoCase("log"))
			{
				if(objExt.CompareNoCase("log"))
					continue;
			}
			else if(!objFileExt.CompareNoCase("dat"))
			{
				if(objExt.CompareNoCase("dat"))
					continue;
			}
			else if(!objFileExt.CompareNoCase("bat"))
			{
				if(objExt.CompareNoCase("bat"))
					continue;
			}
			else if(!objFileExt.CompareNoCase("ini"))
			{
				if(objExt.CompareNoCase("ini"))
					continue;
			}
			else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
			{
				if(objExt.CompareNoCase("mak") && objExt.CompareNoCase("mk"))
					continue;
			}
			else if(!objFileExt.CompareNoCase("java"))
			{
				if(objExt.CompareNoCase("java"))
					continue;
			}
			else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
			{
				if(objExt.CompareNoCase("y") && objExt.CompareNoCase("l"))
					continue;
			}
			else if(!objFileExt.CompareNoCase("wrl"))
			{
				if(objExt.CompareNoCase("wrl"))
					continue;
			}

			POSITION pos3 = pDoc->GetFirstViewPosition();
			while(pos3 != 0)
			{
				CEditorView* pView = (CEditorView*)pDoc->GetNextView(pos3);
				if(pView!=NULL)
				{
					pView->SetFlagDispLineNo(bDispLineNo);
					pView->SetFlagDispRuler(bDispRuler);
					pView->SetFlagDispCRLF(bDispCRLF);
					pView->SetFlagDispTab(bDispTab);
					pView->SetFlagDispZenkakuSpace(bDispZenkakuSpace);
					pView->SetFlagDispUnderLine(bDispUnderLine);
				}
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�g���q�ʃt�H���g�ݒ�̑S�E�C���h�E���f����							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::SetFontSettings2AllWindow
	(	CEditorDoc*	pActiveDoc
	)
{
	CString objKey;
	CString objFileExt;
	long lDispFontPoint=0;
	CString objDispFontName;
	long lPrintFontPoint=0;
	CString objPrintFontName;
	CString objMsg;

	POSITION pos1 = GetFirstDocTemplatePosition();
	while(pos1 != 0)
	{
		CDocTemplate* pTempl = GetNextDocTemplate(pos1);
		POSITION pos2 = pTempl->GetFirstDocPosition();
		while(pos2 != 0)
		{
			CEditorDoc* pDoc = (CEditorDoc*)pTempl->GetNextDoc(pos2);
			if(pDoc == NULL)
				continue;
			if(pDoc == pActiveDoc)
				continue;

			objFileExt = pDoc->MyGetFileExt();
			if(objFileExt.IsEmpty())
				objKey = "FileExt_none";
			else if(!objFileExt.CompareNoCase("txt"))
				objKey = "FileExt_txt";
			else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
					!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
				objKey = "FileExt_htm html shtml shtm";
			else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
				objKey = "FileExt_c cpp";
			else if(!objFileExt.CompareNoCase("h"))
				objKey = "FileExt_h";
			else if(!objFileExt.CompareNoCase("asm"))
				objKey = "FileExt_asm";
			else if(!objFileExt.CompareNoCase("asp"))
				objKey = "FileExt_asp";
			else if(!objFileExt.CompareNoCase("frm"))
				objKey = "FileExt_frm";
			else if(!objFileExt.CompareNoCase("bas"))
				objKey = "FileExt_bas";
			else if(!objFileExt.CompareNoCase("pc"))
				objKey = "FileExt_pc";
			else if(!objFileExt.CompareNoCase("log"))
				objKey = "FileExt_log";
			else if(!objFileExt.CompareNoCase("dat"))
				objKey = "FileExt_dat";
			else if(!objFileExt.CompareNoCase("bat"))
				objKey = "FileExt_bat";
			else if(!objFileExt.CompareNoCase("ini"))
				objKey = "FileExt_ini";
			else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
				objKey = "FileExt_mak mk";
			else if(!objFileExt.CompareNoCase("java"))
				objKey = "FileExt_java";
			else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
				objKey = "FileExt_y l";
			else if(!objFileExt.CompareNoCase("wrl"))
				objKey = "FileExt_wrl";
			else
				objKey = "FileExt_other";

			objMsg.LoadString(IDS_FONTNAME_DISP);
			lDispFontPoint   = GetProfileInt(objKey,"DispFontPoint",120);
			objDispFontName  = GetProfileString(objKey,"DispFontName",objMsg);
			objMsg.LoadString(IDS_FONTNAME_PRINT);
			lPrintFontPoint  = GetProfileInt(objKey,"PrintFontPoint",80);
			objPrintFontName = GetProfileString(objKey,"PrintFontName",objMsg);

			POSITION pos3 = pDoc->GetFirstViewPosition();
			while(pos3 != 0)
			{
				CEditorView* pView = (CEditorView*)pDoc->GetNextView(pos3);
				if(pView!=NULL)
				{
					//�\���t�H���g�ݒ�
					pView->SetDispFontPoint(lDispFontPoint);
					pView->SetDispFontName(objDispFontName);
					//�󎚃t�H���g�ݒ�
					pView->SetPrintFontPoint(lPrintFontPoint);
					pView->SetPrintFontName(objPrintFontName);
				}
			}
		}
	}

	objFileExt = pActiveDoc->MyGetFileExt();
	if(objFileExt.IsEmpty())
		objKey = "FileExt_none";
	else if(!objFileExt.CompareNoCase("txt"))
		objKey = "FileExt_txt";
	else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
			!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
		objKey = "FileExt_htm html shtml shtm";
	else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
		objKey = "FileExt_c cpp";
	else if(!objFileExt.CompareNoCase("h"))
		objKey = "FileExt_h";
	else if(!objFileExt.CompareNoCase("asm"))
		objKey = "FileExt_asm";
	else if(!objFileExt.CompareNoCase("asp"))
		objKey = "FileExt_asp";
	else if(!objFileExt.CompareNoCase("frm"))
		objKey = "FileExt_frm";
	else if(!objFileExt.CompareNoCase("bas"))
		objKey = "FileExt_bas";
	else if(!objFileExt.CompareNoCase("pc"))
		objKey = "FileExt_pc";
	else if(!objFileExt.CompareNoCase("log"))
		objKey = "FileExt_log";
	else if(!objFileExt.CompareNoCase("dat"))
		objKey = "FileExt_dat";
	else if(!objFileExt.CompareNoCase("bat"))
		objKey = "FileExt_bat";
	else if(!objFileExt.CompareNoCase("ini"))
		objKey = "FileExt_ini";
	else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
		objKey = "FileExt_mak mk";
	else if(!objFileExt.CompareNoCase("java"))
		objKey = "FileExt_java";
	else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
		objKey = "FileExt_y l";
	else if(!objFileExt.CompareNoCase("wrl"))
		objKey = "FileExt_wrl";
	else
		objKey = "FileExt_other";

	objMsg.LoadString(IDS_FONTNAME_DISP);
	lDispFontPoint   = GetProfileInt(objKey,"DispFontPoint",120);
	objDispFontName  = GetProfileString(objKey,"DispFontName",objMsg);
	objMsg.LoadString(IDS_FONTNAME_PRINT);
	lPrintFontPoint  = GetProfileInt(objKey,"PrintFontPoint",80);
	objPrintFontName = GetProfileString(objKey,"PrintFontName",objMsg);

	POSITION pos3 = pActiveDoc->GetFirstViewPosition();
	while(pos3 != 0)
	{
		CEditorView* pView = (CEditorView*)pActiveDoc->GetNextView(pos3);
		if(pView!=NULL)
		{
			//�\���t�H���g�ݒ�
			pView->SetDispFontPoint(lDispFontPoint);
			pView->SetDispFontName(objDispFontName);
			//�󎚃t�H���g�ݒ�
			pView->SetPrintFontPoint(lPrintFontPoint);
			pView->SetPrintFontName(objPrintFontName);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�g���q�ʐF�ݒ�̑S�E�C���h�E���f����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::SetColorSettings2AllWindow()
{
	CString objKey;
	CString objFileExt;
	char szKeyNo1Fname[_MAX_FNAME],szKeyNo2Fname[_MAX_FNAME];
	char szKeyNo3Fname[_MAX_FNAME],szKeyNo4Fname[_MAX_FNAME];

	POSITION pos1 = GetFirstDocTemplatePosition();
	while(pos1 != 0)
	{
		CDocTemplate* pTempl = GetNextDocTemplate(pos1);
		POSITION pos2 = pTempl->GetFirstDocPosition();
		while(pos2 != 0)
		{
			CEditorDoc* pDoc = (CEditorDoc*)pTempl->GetNextDoc(pos2);
			if(pDoc == NULL)
				continue;

			objFileExt = pDoc->MyGetFileExt();
			if(objFileExt.IsEmpty())
				objKey = "FileExt_none";
			else if(!objFileExt.CompareNoCase("txt"))
				objKey = "FileExt_txt";
			else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
					!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
				objKey = "FileExt_htm html shtml shtm";
			else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
				objKey = "FileExt_c cpp";
			else if(!objFileExt.CompareNoCase("h"))
				objKey = "FileExt_h";
			else if(!objFileExt.CompareNoCase("asm"))
				objKey = "FileExt_asm";
			else if(!objFileExt.CompareNoCase("asp"))
				objKey = "FileExt_asp";
			else if(!objFileExt.CompareNoCase("frm"))
				objKey = "FileExt_frm";
			else if(!objFileExt.CompareNoCase("bas"))
				objKey = "FileExt_bas";
			else if(!objFileExt.CompareNoCase("pc"))
				objKey = "FileExt_pc";
			else if(!objFileExt.CompareNoCase("log"))
				objKey = "FileExt_log";
			else if(!objFileExt.CompareNoCase("dat"))
				objKey = "FileExt_dat";
			else if(!objFileExt.CompareNoCase("bat"))
				objKey = "FileExt_bat";
			else if(!objFileExt.CompareNoCase("ini"))
				objKey = "FileExt_ini";
			else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
				objKey = "FileExt_mak mk";
			else if(!objFileExt.CompareNoCase("java"))
				objKey = "FileExt_java";
			else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
				objKey = "FileExt_y l";
			else if(!objFileExt.CompareNoCase("wrl"))
				objKey = "FileExt_wrl";
			else
				objKey = "FileExt_other";

			COLORREF FontColor       = GetProfileInt(objKey,"FontColor",RGB(255,255,255));
			COLORREF BackColor       = GetProfileInt(objKey,"BackColor",RGB(0,0,0));
			COLORREF InvertFontColor = GetProfileInt(objKey,"InvertFontColor",RGB(0,0,0));
			COLORREF InvertBackColor = GetProfileInt(objKey,"InvertBackColor",RGB(255,255,255));
			COLORREF KeyNo1Color     = GetProfileInt(objKey,"KeyNo1Color",RGB(0,255,255));
			COLORREF KeyNo2Color     = GetProfileInt(objKey,"KeyNo2Color",RGB(0,255,255));
			COLORREF KeyNo3Color     = GetProfileInt(objKey,"KeyNo3Color",RGB(0,255,255));
			COLORREF KeyNo4Color     = GetProfileInt(objKey,"KeyNo4Color",RGB(0,255,255));
			COLORREF MarkLineColor   = GetProfileInt(objKey,"MarkLineColor",RGB(128,0,128));
			BOOL     bComment1Clr    = GetProfileInt(objKey,"bComment1Clr",1);
			COLORREF CommentColor    = GetProfileInt(objKey,"CommentColor",RGB(0,255,0));
			BOOL     bComment2Clr    = GetProfileInt(objKey,"bComment2Clr",0);
			COLORREF CommentColor2   = GetProfileInt(objKey,"CommentColor2",RGB(0,255,0));
			BOOL     bComment3Clr    = GetProfileInt(objKey,"bComment3Clr",0);
			COLORREF CommentColor3   = GetProfileInt(objKey,"CommentColor3",RGB(0,255,0));
			BOOL     bComment4Clr    = GetProfileInt(objKey,"bComment4Clr",0);
			COLORREF CommentColor4   = GetProfileInt(objKey,"CommentColor4",RGB(0,255,0));
			BOOL     bDispLineNo     = GetProfileInt(objKey,"bDispLineNo",1);
			COLORREF LineNoColor     = GetProfileInt(objKey,"LineNoColor",RGB(0,255,255));
			COLORREF LineNoBackColor = GetProfileInt(objKey,"LineNoBackColor",RGB(0,0,0));
			BOOL     bDispRuler	     = GetProfileInt(objKey,"bDispRuler",1);
			COLORREF RulerColor	     = GetProfileInt(objKey,"RulerColor",RGB(0,0,0));
			COLORREF RulerBackColor  = GetProfileInt(objKey,"RulerBackColor",RGB(192,192,192));
			BOOL     bDispCRLF		 = GetProfileInt(objKey,"bDispCRLF",1);
			COLORREF CRLFColor		 = GetProfileInt(objKey,"CRLFColor",RGB(255,255,0));
			BOOL     bDispTab		 = GetProfileInt(objKey,"bDispTab",0);
			COLORREF TabColor		 = GetProfileInt(objKey,"TabColor",RGB(255,255,255));
			BOOL     bDispZenkakuSpace = GetProfileInt(objKey,"bDispZenkakuSpace",0);
			COLORREF ZenkakuSpaceColor = GetProfileInt(objKey,"ZenkakuSpaceColor",RGB(255,255,255));
			BOOL     bDispUnderLine  = GetProfileInt(objKey,"bDispUnderLine",1);
			COLORREF UnderLineColor  = GetProfileInt(objKey,"UnderLineColor",RGB(255,255,0));
			BOOL     bDispUrl		 = GetProfileInt(objKey,"bDispUrl",1);
			COLORREF UrlColor		 = GetProfileInt(objKey,"UrlColor",RGB(255,0,255));
			BOOL     bDispEMail	     = GetProfileInt(objKey,"bDispEMail",1);
			COLORREF EMailColor	     = GetProfileInt(objKey,"EMailColor",RGB(255,0,255));
			strcpy(szKeyNo1Fname,(LPCTSTR)GetProfileString(objKey,"Keyword_no1_filename",""));
			strcpy(szKeyNo2Fname,(LPCTSTR)GetProfileString(objKey,"Keyword_no2_filename",""));
			strcpy(szKeyNo3Fname,(LPCTSTR)GetProfileString(objKey,"Keyword_no3_filename",""));
			strcpy(szKeyNo4Fname,(LPCTSTR)GetProfileString(objKey,"Keyword_no4_filename",""));

			pDoc->SetFontColor(FontColor);
			pDoc->DeleteAllKeyWord();
			pDoc->SetKeyNo1Color(KeyNo1Color,szKeyNo1Fname);
			pDoc->SetKeyNo2Color(KeyNo2Color,szKeyNo2Fname);
			pDoc->SetKeyNo3Color(KeyNo3Color,szKeyNo3Fname);
			pDoc->SetKeyNo4Color(KeyNo4Color,szKeyNo4Fname);
			pDoc->SetFlagComment1Clr(bComment1Clr);
			pDoc->SetCommentColor(CommentColor);
			pDoc->SetFlagComment2Clr(bComment2Clr);
			pDoc->SetCommentColor2(CommentColor2);
			pDoc->SetFlagComment3Clr(bComment3Clr);
			pDoc->SetCommentColor3(CommentColor3);
			pDoc->SetFlagComment4Clr(bComment4Clr);
			pDoc->SetCommentColor4(CommentColor4);
			pDoc->SetFlagUrlColor(bDispUrl);
			pDoc->SetUrlColor(UrlColor);
			pDoc->SetFlagEMailColor(bDispEMail);
			pDoc->SetEMailColor(EMailColor);
			POSITION pos3 = pDoc->GetFirstViewPosition();
			while(pos3 != 0)
			{
				CEditorView* pView = (CEditorView*)pDoc->GetNextView(pos3);
				if(pView!=NULL)
				{
					pView->SetFontColor(FontColor);
					pView->SetBackColor(BackColor);
					pView->SetInvertFontColor(InvertFontColor);
					pView->SetInvertBackColor(InvertBackColor);
					pView->SetMarkLineColor(MarkLineColor);
					pView->SetFlagDispLineNo(bDispLineNo);
					pView->SetLineNoColor(LineNoColor);
					pView->SetLineNoBackColor(LineNoBackColor);
					pView->SetFlagDispRuler(bDispRuler);
					pView->SetRulerColor(RulerColor);
					pView->SetRulerBackColor(RulerBackColor);
					pView->SetFlagDispCRLF(bDispCRLF);
					pView->SetCRLFColor(CRLFColor);
					pView->SetFlagDispTab(bDispTab);
					pView->SetTabColor(TabColor);
					pView->SetFlagDispZenkakuSpace(bDispZenkakuSpace);
					pView->SetZenkakuSpaceColor(ZenkakuSpaceColor);
					pView->SetFlagDispUnderLine(bDispUnderLine);
					pView->SetUnderLineColor(UnderLineColor);
				}
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�V���[�g�J�b�g�쐬����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::MakeXerxesShortcut
	(	BOOL	bIsMakeShortcutDsktop
	)
{
	//�f�X�N�g�b�v�̃f�B���N�g�����擾
	CString objMsg;
	HKEY hKey;
	DWORD lpType;
	ULONG ulSize=256;
	TCHAR key[256];
	memset(key,0,sizeof(key));
	if(::RegOpenKeyEx( HKEY_CURRENT_USER, 
					   "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
					   0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
		::RegQueryValueEx( hKey,"Desktop", NULL, &lpType, (unsigned char*)&key, &ulSize);
		::RegCloseKey(hKey);

		lstrcat(key,"\\Xerxes.lnk");
		if(bIsMakeShortcutDsktop)
		{
			if( _access( key, 0 ) == -1 ) // �Ȃ�
			{
				objMsg.LoadString(IDS_SHORTCUTNAME_TOXERXES);
				CreateShortCut(m_strModuleFileName,key,(char*)(LPCTSTR)objMsg);
			}
		}
		else
		{
			if( _access( key, 0 ) != -1 )
				::DeleteFile( key );
		}
		return TRUE;
	}
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�V���[�g�J�b�g�쐬����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::CreateShortCut
	(	const char*	szPathObj, 		//�V���[�g�J�b�g���̃t�@�C����
		char*	szPathLink,		//�V���[�g�J�b�g�̃t�@�C����
		char*	szDesc			//�V���[�g�J�b�g�̖��O
	)
{
	HRESULT hres;
	IShellLink* psl=NULL; 		//IShellLink�C���^�[�t�F�[�X
	IPersistFile* ppf=NULL; 	//IPersistFile�C���^�[�t�F�[�X
	WORD wsz[MAX_PATH];

//	hres = CoInitialize(NULL);

	//IShellLink�C���^�[�t�F�[�X�𓾂�
	hres = CoCreateInstance( CLSID_ShellLink, 
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IShellLink, 
							 (void**)&psl );
	if(hres != S_OK)
		return FALSE;

	//IPersistFile�C���^�[�t�F�[�X��QueryInterface
	hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf); 
    if(hres != S_OK)
	{
		psl->Release();
//		CoUninitialize();
		return FALSE;
	}

	//�V���[�g�J�b�g���̃t�@�C��
	hres = psl->SetPath(szPathObj); 
	if(hres != NOERROR)
	{
		if(ppf != NULL)
			ppf->Release();
		psl->Release();
//		CoUninitialize();
		return FALSE;
	}
   	//�V���[�g�J�b�g�̂���
	hres = psl->SetDescription(szDesc); 
	if(hres != NOERROR)
	{
		if(ppf != NULL)
			ppf->Release();
		psl->Release();
//		CoUninitialize();
		return FALSE;
	}

   	//�V���[�g�J�b�g���̂̃t�@�C������Unicode��
	MultiByteToWideChar(CP_ACP, 0, szPathLink, -1, (LPWSTR)wsz, MAX_PATH); 

	//�쐬
	hres = ppf->Save((LPCOLESTR)wsz, TRUE); 
	if(hres != S_OK)
	{
		if(ppf != NULL)
			ppf->Release();
		psl->Release();
//		CoUninitialize();
		return FALSE;
	}

	if(ppf != NULL)
		ppf->Release();
	psl->Release();
//	CoUninitialize();
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���W�X�g���L�[�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LONG CEditorApp::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
	HKEY hkey;
	LONG retval	= RegOpenKeyEx(key,	subkey,	0, KEY_QUERY_VALUE,	&hkey);

	if (retval == ERROR_SUCCESS) 
	{
		long datasize =	MAX_PATH;
		TCHAR data[MAX_PATH];
		RegQueryValue(hkey,	NULL, data,	&datasize);
		lstrcpy(retdata,data);
		RegCloseKey(hkey);
	}

	return retval;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w���v�g�s�b�N�̃I�[�v������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::OpenHelpTopix
	(	ULONG	ulID		//�w���v�g�s�b�N�̂h�c
	)
{
	CString strBuf="";
	CString strPath;

	//�w���v�t�@�C���̑��݊m�F
	if(!IsDriveRoot())
		strBuf = "\\";
	strPath.Format("%s%s%s.hlp",m_strExePath,strBuf,m_pszAppName);
	if( _access( (LPCTSTR)strPath, 0 ) == -1 )
		return;

	//�w���v�t�@�C���I�[�v��
	::WinHelp( NULL,			//�e�E�C���h�E�n���h��
			   (LPCTSTR)strPath,//�w���v�t�@�C����
			   HELP_CONTEXT,	//�R�}���h
			   ulID );			//�w���v�g�s�b�N�h�c
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�S�t�@�C���ۑ����s����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::DoFileAllSave()
{
	POSITION pos1 = GetFirstDocTemplatePosition();
	while(pos1 != 0)
	{
		CDocTemplate* pTempl = GetNextDocTemplate(pos1);
		POSITION pos2 = pTempl->GetFirstDocPosition();
		while(pos2 != 0)
		{
			CEditorDoc* pDoc = (CEditorDoc*)pTempl->GetNextDoc(pos2);
			if( (pDoc != 0) && !pDoc->IsReadOnly() )
			{
				pDoc->OnFileSave();
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���ύX�`�F�b�N����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::CheckFileChange()
{
	POSITION pos1 = GetFirstDocTemplatePosition();
	while(pos1 != 0)
	{
		CDocTemplate* pTempl = GetNextDocTemplate(pos1);
		POSITION pos2 = pTempl->GetFirstDocPosition();
		while(pos2 != 0)
		{
			CEditorDoc* pDoc = (CEditorDoc*)pTempl->GetNextDoc(pos2);
			if( pDoc != 0 )
			{
				if( pDoc->IsNeedReload() )
				{
					//�_�C�A���O�m�F����
					pDoc->ReloadDocument(TRUE);
				}
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�Y���b�Z�[�W�z������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::MessagePump()
{
	MSG		msg;
	if(::PeekMessage(&msg,0,0,0,PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�V�K�q�E�C���h�E�쐬����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::CreateNewMDIChdWnd
	(	CDocument**	pDocument,				//�쐬�����V�K�h�L�������g
		CFrameWnd**	pFrame					//�쐬�����V�K�q�E�C���h�E
	)
{
	POSITION pos1 = GetFirstDocTemplatePosition();
	if(pos1 == 0)
		return FALSE;

	CDocTemplate* pTempl = GetNextDocTemplate(pos1);
	(*pDocument) = pTempl->CreateNewDocument();
	(*pFrame) = pTempl->CreateNewFrame( (*pDocument), NULL );
	pTempl->InitialUpdateFrame( (*pFrame), (*pDocument), TRUE );

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w��s�W�����v���b�Z�[�W���M����										|*/
/*|	���	�F�����Ŏw�肳�ꂽ�h�L�������g�����u�h�d�v�ɃW�����v�l�r�f�𑗐M����	|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::SendJumpMsg
	(	const char*	szFilePath,			// ���b�Z�[�W���M��h�L�������g�̃t�@�C����
		long	lDestLine			// �W�����v��s�ԍ�
	)
{
	char	szBuf[_MAX_PATH];

	POSITION pos1 = GetFirstDocTemplatePosition();
	while(pos1 != 0)
	{
		CDocTemplate* pTempl = GetNextDocTemplate(pos1);
		POSITION pos2 = pTempl->GetFirstDocPosition();
		while(pos2 != 0)
		{
			CEditorDoc* pDoc = (CEditorDoc*)pTempl->GetNextDoc(pos2);
			if( pDoc != 0)
			{
				memset(szBuf,0,sizeof(szBuf));
				pDoc->MyGetFilePath(szBuf);
				if( !strcmp(szBuf,szFilePath) ) //���b�Z�[�W���M�攭��
				{
					pDoc->SendJumpMsg(lDestLine,TRUE);
				}
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�l�q�t�t�@�C�����擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorApp::GetMaxMRU()
{
	return m_nMaxMRU;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�l�q�t�t�@�C�����ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::SetMaxMRU(int nMaxMRU)
{
	m_nMaxMRU = nMaxMRU;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�v���b�V���X�N���[���\����������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::IsShowSplash()
{
	return m_bShowSplash;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�v���b�V���X�N���[���\���ݒ菈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::SetShowSplash
	(	BOOL	bShowSplash
	)
{
	m_bShowSplash = bShowSplash;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�N�����V�K�q�E�C���h�E�쐬��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::IsOpenNewFile()
{
	return m_bIsOpenNewFile;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�N�����V�K�q�E�C���h�E�쐬�ݒ菈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::SetOpenNewFile(BOOL bIsOpenNewFile)
{
	m_bIsOpenNewFile = bIsOpenNewFile;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�[�v���t�@�C���f�B���N�g���ړ���������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::IsChdirToOpenFile()
{
	return m_bIsChdirToOpenFile;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�[�v���t�@�C���f�B���N�g���ړ��ݒ菈��								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::SetChdirToOpenFile
	(	BOOL	bIsChdirToOpenFile
	)
{
	m_bIsChdirToOpenFile = bIsChdirToOpenFile;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����ReadString�̏���													|*/
/*|	���	�F																		|*/
/*|	���l	�FMyReadString���ĂԎ��A�K���ŏ��ɖ{�֐����P�񂾂��R�[�����邱��		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::PrepareMyReadString()
{
	// ����A�[�J�C�u���͗p�����ꎞ�o�b�t�@���N���A
	m_cBuf4MyReadString = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����ReadString														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorApp::MyReadString	//�O�F�d�n�e�I���A�P�F���s�I��
	(	CArchive&	ar,
		CString&	objStr,
		long*		plCRCount,	//CR�̐�(out)
		long*		plLFCount	//LF�̐�(out)
	)
{
	CString	objBufStr = _T("");
	long lCnt=0;
	char ch=0;
	int	nStage=0;

	(*plCRCount) = 0;
	(*plLFCount) = 0;

	TRY
	{
		for(lCnt=0;;lCnt++)
		{
			if(lCnt==0 && m_cBuf4MyReadString != 0)
			{
				ch = m_cBuf4MyReadString;
				m_cBuf4MyReadString = 0;
			}
			else
			{
				nStage=0;
				ar >> (BYTE&)ch;
			}

			if(IsDBCSLeadByteEx(0,ch)) //�Q�o�C�g����
			{
				objBufStr += ch;
				ar >> (BYTE&)ch;
				objBufStr += ch;
			}
			else //�P�o�C�g����
			{
				if(ch=='\r')
				{
					(*plCRCount)++;
					break;
				}
				else if(ch=='\n')
				{
					(*plLFCount)++;
					break;
				}
				else
				{
					objBufStr += ch;
				}
			}
		}

		//////////////////////////////////////////
		//	���s�R�[�h�iCR+LF, CR, LF�j�̔���	//
		//////////////////////////////////////////
		nStage=1;
		ar >> (BYTE&)ch;

		if(IsDBCSLeadByteEx(0,ch)) //�Q�o�C�g����
		{
			m_cBuf4MyReadString = ch;
			objStr = objBufStr;
			return 1;
		}
		else //�P�o�C�g����
		{
			if(ch=='\r')
			{
				m_cBuf4MyReadString = ch;
				objStr = objBufStr;
				return 1;
			}
			else if(ch=='\n') //���s
			{
				if( (*plCRCount)==0 ) //LF
				{
					m_cBuf4MyReadString = ch;
					objStr = objBufStr;
					return 1;
				}
				else //CR + LF
				{
					(*plLFCount)++;
					m_cBuf4MyReadString = 0;
					objStr = objBufStr;
					return 1;
				}
			}
			else
			{
				m_cBuf4MyReadString = ch;
				objStr = objBufStr;
				return 1;
			}
		}
	}
	CATCH_ALL(e)
	{
		if(nStage==0)
		{
			objStr = objBufStr;
			return 0;
		}
		else
		{
			m_cBuf4MyReadString = 0;
			objStr = objBufStr;
			return 1;
		}
	}
	END_CATCH_ALL

	objStr = objBufStr;
	return 1;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���A�N�Z�X�G���[���b�Z�[�W�擾									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::GetFileErrMessage(CString& objStr, CFileException& e)
{
	CString objMsg;

	switch(e.m_cause)
	{
	case CFileException::none: 					//"�G���[�Ȃ�"
		objMsg.LoadString(IDS_FILEEXCEPTION_NONE);
		break;
	case CFileException::generic:				//"����`�G���["
		objMsg.LoadString(IDS_FILEEXCEPTION_GENGERIC);
		break;
	case CFileException::fileNotFound:			//"�t�@�C����������Ȃ�"
		objMsg.LoadString(IDS_FILEEXCEPTION_NOTFOUND);
		break;
	case CFileException::badPath:				//"�s���p�X"
		objMsg.LoadString(IDS_FILEEXCEPTION_BADPATH);
		break;
	case CFileException::tooManyOpenFiles:		//"�I�[�v�� �t�@�C��������"
		objMsg.LoadString(IDS_FILEEXCEPTION_TOOMANY);
		break;
	case CFileException::accessDenied:			//"�t�@�C�� �A�N�Z�X�֎~"
		objMsg.LoadString(IDS_FILEEXCEPTION_ACCESSDENY);
		break;
	case CFileException::invalidFile:			//"�s���t�@�C�� �n���h���g�p"
		objMsg.LoadString(IDS_FILEEXCEPTION_INVALIDFILE);
		break;
	case CFileException::removeCurrentDir:		//"��ƒ��̃J�����g �f�B���N�g���̍폜�s��"
		objMsg.LoadString(IDS_FILEEXCEPTION_REMOVECURRDIR);
		break;
	case CFileException::directoryFull:			//"�f�B���N�g�� �t��"
		objMsg.LoadString(IDS_FILEEXCEPTION_DIRFULL);
		break;
	case CFileException::badSeek:				//"�t�@�C�� �|�C���^�ݒ�G���["
		objMsg.LoadString(IDS_FILEEXCEPTION_BADSEEK);
		break;
	case CFileException::hardIO:				//"�n�[�h�E�G�A �G���["
		objMsg.LoadString(IDS_FILEEXCEPTION_HARDIO);
		break;
	case CFileException::sharingViolation:		//"SHARE.EXE �����[�h�܂��͋��L�̈惍�b�N�ς�"
		objMsg.LoadString(IDS_FILEEXCEPTION_SHAREVIOLATION);
		break;
	case CFileException::lockViolation:			//"���b�N�ςݗ̈�̃��b�N�v��"
		objMsg.LoadString(IDS_FILEEXCEPTION_LOCKVIOLATION);
		break;
	case CFileException::diskFull:				//"�f�B�X�N �t��"
		objMsg.LoadString(IDS_FILEEXCEPTION_DISKFULL);
		break;
	case CFileException::endOfFile:				//"�t�@�C���̏I���ɓ��B"
		objMsg.LoadString(IDS_FILEEXCEPTION_ENDOFFILE);
		break;
	default:
		break;
	}

	objStr += objMsg;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�h���C�u���[�g�f�B���N�g����������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::IsDriveRoot()
{
	return m_bIsDriveRoot;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�h���C�u���[�g�f�B���N�g����������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::IsDriveRoot(const char* dir)
{
	if(strlen(dir)==3 && dir[1]==':' && dir[2]=='\\')
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���O�o�͏���															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::ExecuteLogging
	(	const char*	szMsg
	)
{
	CString strBuf;

	if(m_fpLogging==NULL)
		return;

	//���ݎ���
	CTime	objNow = CTime::GetCurrentTime();
	CString	objStr = objNow.Format("%Y/%m/%d %H:%M:%S --- ");

	strBuf = (LPCTSTR)objStr;
	strBuf += szMsg;
	strBuf += "\n";

	fputs((LPCTSTR)strBuf,m_fpLogging);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����p�������擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorApp::GetPrintOrientation()
{
	return m_nPrintOrientation;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����p�������ݒ菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::SetPrintOrientation
	(	int	nPrintOrientation
	)
{
	if(nPrintOrientation!=PRINTORIENTATION_TATE && nPrintOrientation!=PRINTORIENTATION_YOKO)
		return;

	//�����o�ϐ��֕ێ�
	m_nPrintOrientation = nPrintOrientation;

	//��������ݒ�
	PRINTDLG	stPrintDlg;
	if(GetPrinterDeviceDefaults(&stPrintDlg)==FALSE)
		return;
	HANDLE hDevMode = stPrintDlg.hDevMode;
	DEVMODE* pDevMode = (DEVMODE*)::GlobalLock(hDevMode);
	if(pDevMode != NULL)
	{
		if(m_nPrintOrientation==PRINTORIENTATION_TATE)
			pDevMode->dmOrientation = DMORIENT_PORTRAIT;
		else if(m_nPrintOrientation==PRINTORIENTATION_YOKO)
			pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
		::GlobalUnlock(hDevMode);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����]���}�[�W���擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::GetPrintMargin
	(	CRect&	rect
	)
{
	rect = m_objPrintMarginRect;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����]���}�[�W���ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::SetPrintMargin
	(	CRect&	rect
	)
{
	m_objPrintMarginRect = rect;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w��t�@�C���I�[�v������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::OpenFile
	(	CString&	objStr
	)
{
	//�f�B���N�g���ړ�
	if( m_bIsChdirToOpenFile != 0 )
	{
		if( !m_strOpenFileDirectory.IsEmpty() )
			::chdir((LPCTSTR)m_strOpenFileDirectory);
	}

	OpenDocumentFile((char*)(LPCTSTR)objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g���ړ�����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::ChangeDirectory
	(	const char*	pszDirectory
	)
{
	::chdir(pszDirectory);
	m_strOpenFileDirectory = pszDirectory;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w��t�q�k�̃u���E�Y����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::BrowseUrl
	(	CString&	objUrl			//�t�q�k�A�h���X
	)
{
	CWaitCursor	objCursor;
	TCHAR key[MAX_PATH + MAX_PATH];
	CString objMsg;

	HINSTANCE result = ::ShellExecute(NULL, _T("open"), (LPCTSTR)objUrl, NULL,NULL, SW_SHOW);
	if((UINT)result <= HINSTANCE_ERROR)
	{
		if(GetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS)
		{
			lstrcat(key, _T("\\shell\\open\\command"));
			if(GetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS)
			{
				TCHAR *pos;
				pos	= _tcsstr(key, _T("\"%1\""));
				if (pos	== NULL)					   // No quotes	found
				{
					pos	= strstr(key, _T("%1"));	   // Check	for	%1,	without	quotes
					if (pos	== NULL)				   // No parameter at all...
						pos	= key+lstrlen(key)-1;
					else
						*pos = '\0';				   // Remove the parameter
				}
				else
					*pos = '\0';					   // Remove the parameter

				lstrcat(pos, _T(" "));
				lstrcat(pos, (LPCTSTR)objUrl);
				result = (HINSTANCE)::WinExec(key,SW_SHOW);
			}
		}
	}
	if((UINT)result <= HINSTANCE_ERROR)
	{
		objMsg.LoadString(IDS_BROUSERKICK_ERR);
		MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	else
		return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w�肅�|���������A�h���X�Ń��[���[���N��								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::ExecMailer
	(	CString&	objEMail			//���|���������A�h���X
	)
{
	CString objBuf;
	CWaitCursor	objCursor;
	TCHAR key[MAX_PATH + MAX_PATH];
	CString objMsg;

	objBuf = "mailto:";
	objBuf += objEMail;

	HINSTANCE result = ::ShellExecute(NULL, _T("open"), (LPCTSTR)objBuf, NULL,NULL, SW_SHOWNORMAL);
	if((UINT)result <= HINSTANCE_ERROR)
	{
		if(GetRegKey(HKEY_CLASSES_ROOT, _T("mailto\\shell\\open\\command"), key) == ERROR_SUCCESS)
		{
			TCHAR *pos;
			pos	= _tcsstr(key, _T("\"%1\""));
			if (pos	== NULL)					   // No quotes	found
			{
				pos	= strstr(key, _T("%1"));	   // Check	for	%1,	without	quotes
				if (pos	== NULL)				   // No parameter at all...
					pos	= key+lstrlen(key)-1;
				else
					*pos = '\0';				   // Remove the parameter
			}
			else
				*pos = '\0';					   // Remove the parameter

			lstrcat(pos, _T(" "));
			lstrcat(pos, (LPCTSTR)objBuf);
			result = (HINSTANCE)::WinExec(key,SW_SHOW);
		}
	}
	if((UINT)result <= HINSTANCE_ERROR)
	{
		objMsg.LoadString(IDS_MAILERKICK_ERR);
		MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	else
		return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[�o�C���h��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CKeyBind* CEditorApp::FindKeyBind
	(	UINT	nChar,
		BOOL	bIsCtrl,
		BOOL	bIsShift,
		BOOL	bIsAlt
	)
{
	if(m_pobjKeyBindManager == NULL)
		return NULL;
	return m_pobjKeyBindManager->FindKeyBind(nChar,bIsCtrl,bIsShift,bIsAlt);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[�o�C���h��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CKeyBind* CEditorApp::FindKeyBind
	(	DWORD dwID
	)
{
	if(m_pobjKeyBindManager == NULL)
		return NULL;
	return m_pobjKeyBindManager->FindKeyBind(dwID);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[�o�C���h�o�^����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::addKeyBind
	(	CKeyBind& objKeyBind
	)
{
	if(m_pobjKeyBindManager == NULL)
		return;
	m_pobjKeyBindManager->addKeyBind(objKeyBind);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[�o�C���h���Z�b�g����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::resetKeyBind()
{
	if(m_pobjKeyBindManager == NULL)
		return;
	m_pobjKeyBindManager->resetKeyBind();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[�o�C���h�������ݏ���												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::writeKeyBind()
{
	if(m_pobjKeyBindManager == NULL)
		return;
	m_pobjKeyBindManager->writeKeyBind();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[�o�C���h�ǂݍ��ݏ���												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::readKeyBind()
{
	if(m_pobjKeyBindManager == NULL)
		return;
	m_pobjKeyBindManager->readKeyBind();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�`�u�t�@�C���o�k�`�x����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::MyPlaySound(UINT nIDS, BOOL bIsSync)
{
	LPTSTR lpszSound = MAKEINTRESOURCE(nIDS);
	if(bIsSync)
		return ::PlaySound(lpszSound,AfxGetResourceHandle(),SND_SYNC|SND_NODEFAULT|SND_RESOURCE);
	else
		return ::PlaySound(lpszSound,AfxGetResourceHandle(),SND_ASYNC|SND_NODEFAULT|SND_RESOURCE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�`�u�t�@�C���o�k�`�x����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::MyPlaySound(const char* szFilePath, BOOL bIsSync)
{
	if(bIsSync)
		return ::PlaySound(szFilePath,NULL,SND_SYNC|SND_NODEFAULT|SND_FILENAME);
	else
		return ::PlaySound(szFilePath,NULL,SND_ASYNC|SND_NODEFAULT|SND_FILENAME);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FMessageBox�\������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorApp::MyMessageBox(LPCTSTR szText, UINT nType, UINT nIDHelp)
{
	CString strPath;

	if((nType & MB_ICONINFORMATION) != 0) //���_�C�A���O�\��
	{
		if(GetProfileInt("Sound","IsPlayOnMessageInfo",0) != 0)
		{
			//Wave�t�@�C���p�X�擾
			strPath.Format("%s\\%s",m_strMediaDirPath,(LPCTSTR)GetProfileString("Sound","WaveFileOnMessageInfo",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				MyPlaySound((LPCTSTR)strPath,FALSE);
		}
	}
	else if((nType & MB_ICONSTOP) != 0) //�x���_�C�A���O�\��
	{
		if(GetProfileInt("Sound","IsPlayOnMessageStop",0) != 0)
		{
			//Wave�t�@�C���p�X�擾
			strPath.Format("%s\\%s",m_strMediaDirPath,(LPCTSTR)GetProfileString("Sound","WaveFileOnMessageStop",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				MyPlaySound((LPCTSTR)strPath,FALSE);
		}
	}
	else if((nType & MB_ICONQUESTION) != 0) //�₢���킹�_�C�A���O�\��
	{
		if(GetProfileInt("Sound","IsPlayOnMessageQuestion",0) != 0)
		{
			//Wave�t�@�C���p�X�擾
			strPath.Format("%s\\%s",m_strMediaDirPath,(LPCTSTR)GetProfileString("Sound","WaveFileOnMessageQuestion",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				MyPlaySound((LPCTSTR)strPath,FALSE);
		}
	}
	else if((nType & MB_ICONEXCLAMATION) != 0) //���Ӄ_�C�A���O�\��
	{
		if(GetProfileInt("Sound","IsPlayOnMessageExclamation",0) != 0)
		{
			//Wave�t�@�C���p�X�擾
			strPath.Format("%s\\%s",m_strMediaDirPath,(LPCTSTR)GetProfileString("Sound","WaveFileOnMessageExclamation",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				MyPlaySound((LPCTSTR)strPath,FALSE);
		}
	}

	return AfxMessageBox(szText,nType,nIDHelp);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w��t�@�C�����l�q�t���X�g����폜									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::RemoveRecentFileList(const char* szFilePath)
{
	int nCnt=0;
	CString objFilePath;
	if(m_pRecentFileList == NULL)
		return;
	for(nCnt=0; nCnt<m_pRecentFileList->GetSize(); nCnt++)
	{
		objFilePath = (*m_pRecentFileList)[nCnt];
		if(!objFilePath.CompareNoCase(szFilePath))
		{
			m_pRecentFileList->Remove(nCnt);
			break;
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�V�F���G�N�X�e���V�����c�k�k�o�^����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::RegisterShellExtDll()
{
	CString objMsg;

	if(GetProfileInt("CEditorApp","IsShellExtensionRegistered",0) == 0) //���o�^
	{
		//�V�F���G�N�X�e���V�����c�k�k�̓o�^
		if( (_access( (LPCTSTR)m_strShelExtDllPath, 0 )) == -1 )
		{
			objMsg.LoadString(IDS_SHELLDLL_NOTFOUND);
			MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}

		//regsvr32�œo�^
		char szCmdLine[_MAX_PATH];
		sprintf(szCmdLine,"Regsvr32 /s %c%s%c",0x22, (LPCTSTR)m_strShelExtDllPath,0x22);
		::WinExec(szCmdLine,SW_HIDE);
		//�o�^�ς݃}�[�N
		WriteProfileInt("CEditorApp","IsShellExtensionRegistered",1);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�V�F���G�N�X�e���V�����c�k�k�o�^��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::UnRegisterShellExtDll()
{
	CString objMsg;

	if(GetProfileInt("CEditorApp","IsShellExtensionRegistered",0) != 0) //�o�^�ς�
	{
		//�V�F���G�N�X�e���V�����c�k�k�̓o�^����
		if( (_access( (LPCTSTR)m_strShelExtDllPath, 0 )) == -1 )
		{
			objMsg.LoadString(IDS_SHELLDLL_NOTFOUND);
			MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}

		//regsvr32�œo�^����
		char szCmdLine[_MAX_PATH];
		sprintf(szCmdLine,"Regsvr32 /u /s %c%s%c",0x22, (LPCTSTR)m_strShelExtDllPath,0x22);
		::WinExec(szCmdLine,SW_HIDE);
		//�o�^�����}�[�N
		WriteProfileInt("CEditorApp","IsShellExtensionRegistered",0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�h�d�Ɂu�g�s�l�k�\�[�X��Xerxes�ŕҏW�v���j���[�ǉ��E�폜				|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::SetMenuOfIE(BOOL bIsAdd)
{
	LONG	lRet=0;
	DWORD	dwDisposition=0,dwError=0;
	HKEY	hKey=0, hKey1=0, hKey2=0, hKey3=0, hKey4=0;
	BOOL	bCreated=FALSE,bOpened=FALSE;
	char	szBuf[_MAX_PATH];

	if(bIsAdd)// "c:\program files\xerxes\xerxes.exe %1"��ҏW
		sprintf(szBuf,"%s %%1", (LPCTSTR)m_strModuleFileName);
	else
		memset(szBuf,0,sizeof(szBuf));

	//HKEY_CLASSES_ROOT\.htm\OpenWithList\Xerxes.exe\shell\edit\command\�i�W���j
	//�� "c:\program files\xerxes\xerxes.exe %1"��ݒ肷��
	lRet = ::RegCreateKeyEx( HKEY_CLASSES_ROOT,
							 ".htm\\OpenWithList\\Xerxes.exe",
							 0,
							 NULL,
							 REG_OPTION_NON_VOLATILE,
							 KEY_ALL_ACCESS,
							 NULL,
							 &hKey,
							 &dwDisposition );
	if(lRet == ERROR_SUCCESS)
	{
		lRet = ::RegCreateKeyEx( hKey,
								 "shell",
								 0,
								 NULL,
								 REG_OPTION_NON_VOLATILE,
								 KEY_ALL_ACCESS,
								 NULL,
								 &hKey2,
								 &dwDisposition );
		if(lRet == ERROR_SUCCESS)
		{
			lRet = ::RegCreateKeyEx( hKey2,
									 "edit",
									 0,
									 NULL,
									 REG_OPTION_NON_VOLATILE,
									 KEY_ALL_ACCESS,
									 NULL,
									 &hKey3,
									 &dwDisposition );
			if(lRet == ERROR_SUCCESS)
			{
				lRet = ::RegCreateKeyEx( hKey3,
										 "command",
										 0,
										 NULL,
										 REG_OPTION_NON_VOLATILE,
										 KEY_ALL_ACCESS,
										 NULL,
										 &hKey4,
										 &dwDisposition );
				if(lRet == ERROR_SUCCESS)
				{
					lRet = ::RegSetValueEx( hKey4,
											"",
											0,
											REG_SZ,
											(const BYTE*)szBuf,
											strlen(szBuf)+1 );
					::RegCloseKey(hKey4);
				}
				::RegCloseKey(hKey3);
			}
			::RegCloseKey(hKey2);
		}
		::RegCloseKey(hKey);
	}

	//HKEY_CLASSES_ROOT\Applications\xerxes.exe\shell\edit\command\�i�W���j
	//�� "c:\program files\xerxes\xerxes.exe %1"��ݒ肷��
	lRet = ::RegCreateKeyEx( HKEY_CLASSES_ROOT,
							 "Applications\\Xerxes.exe",
							 0,
							 NULL,
							 REG_OPTION_NON_VOLATILE,
							 KEY_ALL_ACCESS,
							 NULL,
							 &hKey,
							 &dwDisposition );
	if(lRet != ERROR_SUCCESS)
	{
		lRet = ::RegCreateKeyEx( hKey,
								 "shell",
								 0,
								 NULL,
								 REG_OPTION_NON_VOLATILE,
								 KEY_ALL_ACCESS,
								 NULL,
								 &hKey2,
								 &dwDisposition );
		if(lRet == ERROR_SUCCESS)
		{
			lRet = ::RegCreateKeyEx( hKey2,
									 "edit",
									 0,
									 NULL,
									 REG_OPTION_NON_VOLATILE,
									 KEY_ALL_ACCESS,
									 NULL,
									 &hKey3,
									 &dwDisposition );
			if(lRet == ERROR_SUCCESS)
			{
				lRet = ::RegCreateKeyEx( hKey3,
										 "command",
										 0,
										 NULL,
										 REG_OPTION_NON_VOLATILE,
										 KEY_ALL_ACCESS,
										 NULL,
										 &hKey4,
										 &dwDisposition );
				if(lRet != ERROR_SUCCESS)
				{
					lRet = ::RegSetValueEx( hKey4,
											"",
											0,
											REG_SZ,
											(const BYTE*)szBuf,
											strlen(szBuf)+1 );
					::RegCloseKey(hKey4);
				}
				::RegCloseKey(hKey3);
			}
			::RegCloseKey(hKey2);
		}
		::RegCloseKey(hKey);
	}

	//HKEY_LOCAL_MACHINE\Software\CLASSES\.htm\OpenWithList\Xerxes.exe\shell\edit\command\�i�W���j
	//�� "c:\program files\xerxes\xerxes.exe %1"��ݒ肷��
	lRet = ::RegCreateKeyEx( HKEY_LOCAL_MACHINE,
							 "Software\\CLASSES\\.htm\\OpenWithList\\Xerxes.exe",
							 0,
							 NULL,
							 REG_OPTION_NON_VOLATILE,
							 KEY_ALL_ACCESS,
							 NULL,
							 &hKey,
							 &dwDisposition );
	if(lRet == ERROR_SUCCESS)
	{
		lRet = ::RegCreateKeyEx( hKey,
								 "shell",
								 0,
								 NULL,
								 REG_OPTION_NON_VOLATILE,
								 KEY_ALL_ACCESS,
								 NULL,
								 &hKey2,
								 &dwDisposition );
		if(lRet == ERROR_SUCCESS)
		{
			lRet = ::RegCreateKeyEx( hKey2,
									 "edit",
									 0,
									 NULL,
									 REG_OPTION_NON_VOLATILE,
									 KEY_ALL_ACCESS,
									 NULL,
									 &hKey3,
									 &dwDisposition );
			if(lRet == ERROR_SUCCESS)
			{
				lRet = ::RegCreateKeyEx( hKey3,
										 "command",
										 0,
										 NULL,
										 REG_OPTION_NON_VOLATILE,
										 KEY_ALL_ACCESS,
										 NULL,
										 &hKey4,
										 &dwDisposition );
				if(lRet == ERROR_SUCCESS)
				{
					lRet = ::RegSetValueEx( hKey4,
											"",
											0,
											REG_SZ,
											(const BYTE*)szBuf,
											strlen(szBuf)+1 );
					::RegCloseKey(hKey4);
				}
				::RegCloseKey(hKey3);
			}
			::RegCloseKey(hKey2);
		}
		::RegCloseKey(hKey);
	}

	//HKEY_LOCAL_MACHINE\Software\CLASSES\Applications\xerxes.exe\shell\edit\command\�i�W���j
	//�� "c:\program files\xerxes\xerxes.exe %1"��ݒ肷��
	lRet = ::RegCreateKeyEx( HKEY_LOCAL_MACHINE,
							 "Software\\CLASSES\\Applications\\Xerxes.exe",
							 0,
							 NULL,
							 REG_OPTION_NON_VOLATILE,
							 KEY_ALL_ACCESS,
							 NULL,
							 &hKey,
							 &dwDisposition );
	if(lRet != ERROR_SUCCESS)
	{
		lRet = ::RegCreateKeyEx( hKey,
								 "shell",
								 0,
								 NULL,
								 REG_OPTION_NON_VOLATILE,
								 KEY_ALL_ACCESS,
								 NULL,
								 &hKey2,
								 &dwDisposition );
		if(lRet == ERROR_SUCCESS)
		{
			lRet = ::RegCreateKeyEx( hKey2,
									 "edit",
									 0,
									 NULL,
									 REG_OPTION_NON_VOLATILE,
									 KEY_ALL_ACCESS,
									 NULL,
									 &hKey3,
									 &dwDisposition );
			if(lRet == ERROR_SUCCESS)
			{
				lRet = ::RegCreateKeyEx( hKey3,
										 "command",
										 0,
										 NULL,
										 REG_OPTION_NON_VOLATILE,
										 KEY_ALL_ACCESS,
										 NULL,
										 &hKey4,
										 &dwDisposition );
				if(lRet != ERROR_SUCCESS)
				{
					lRet = ::RegSetValueEx( hKey4,
											"",
											0,
											REG_SZ,
											(const BYTE*)szBuf,
											strlen(szBuf)+1 );
					::RegCloseKey(hKey4);
				}
				::RegCloseKey(hKey3);
			}
			::RegCloseKey(hKey2);
		}
		::RegCloseKey(hKey);
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�c�c�d�ʐM�g���C����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::TryDDE(const char* szPath)
{
	BOOL bRet=FALSE;
	CDdeClient	objDdeClient;

	//�T�[�o�[�ւ̐ڑ�
	bRet = objDdeClient.Connect();
	if(bRet == FALSE)
		return FALSE;

	//�w��t�@�C���I�[�v���̃��N�G�X�g���s
	bRet = objDdeClient.Request(szPath);
	if(bRet == FALSE)
		return FALSE;

	//�T�[�o�[����̐ؒf
	objDdeClient.DisConnect();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CEditorApp �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����͏���													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::getMyDirectory()
{
	char szBuf[_MAX_PATH];

	//���W���[�����擾
	GetModuleFileName( AfxGetInstanceHandle(), szBuf, _MAX_PATH);
	m_strModuleFileName = szBuf;
	WriteProfileString("CEditorApp","ExeName",m_strModuleFileName);

	//�N�����f�B���N�g���擾
	CString strDir = szBuf;
	int nROffset = strDir.ReverseFind('\\');
	int nFOffset = strDir.Find('\\');
	if( nROffset != nFOffset )
	{
		m_bIsDriveRoot = FALSE;
		strDir.SetAt(nROffset,0);
	}
	else
	{
		m_bIsDriveRoot = TRUE;
		strDir.SetAt(nROffset+1,0);
	}
	m_strExePath = strDir;

	if(m_bIsDriveRoot)
	{
		//�V�F���G�N�X�e���V�����c�k�k���擾
		m_strShelExtDllPath.Format("%sx_shelext.dll",m_strExePath);

		//��M�ς݃��[���ۑ���p�X
		m_strRecvMailPath.Format("%sRecvMail",m_strExePath);
		m_strRecvMailAttachPath.Format("%sAttach",m_strExePath);
		if( _access((LPCTSTR)m_strRecvMailPath,0) == -1 )
			::CreateDirectory((LPCTSTR)m_strRecvMailPath,NULL);
		if( _access((LPCTSTR)m_strRecvMailAttachPath,0) == -1 )
			::CreateDirectory((LPCTSTR)m_strRecvMailAttachPath,NULL);

		//HTML �L���b�V���p�p�X
		m_strCashePath.Format("%sCashe",m_strExePath);
		if( _access((LPCTSTR)m_strCashePath,0) == -1 )
			::CreateDirectory((LPCTSTR)m_strCashePath,NULL);
	}
	else
	{
		//�V�F���G�N�X�e���V�����c�k�k���擾
		m_strShelExtDllPath.Format("%s\\x_shelext.dll",m_strExePath);

		//��M�ς݃��[���ۑ���p�X
		m_strRecvMailPath.Format("%s\\RecvMail",m_strExePath);
		m_strRecvMailAttachPath.Format("%s\\Attach",m_strExePath);
		if( _access((LPCTSTR)m_strRecvMailPath,0) == -1 )
			::CreateDirectory((LPCTSTR)m_strRecvMailPath,NULL);
		if( _access((LPCTSTR)m_strRecvMailAttachPath,0) == -1 )
			::CreateDirectory((LPCTSTR)m_strRecvMailAttachPath,NULL);

		//HTML �L���b�V���p�p�X
		m_strCashePath.Format("%s\\Cashe",m_strExePath);
		if( _access((LPCTSTR)m_strCashePath,0) == -1 )
			::CreateDirectory((LPCTSTR)m_strCashePath,NULL);
	}

	//Media�p�X
	if(::GetWindowsDirectory(szBuf,_MAX_PATH) != 0)
		m_strMediaDirPath.Format("%s\\Media",szBuf);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�N���p�����[�^��͏���												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::paramAnalyze
	(	CCommandLineInfo&	cmdInfo
	)
{
	if(__argc >= 2)
	{
		if( (_access( __argv[1], 0 )) != -1 )
		{
			//���p�����[�^�Ŏw�肳�ꂽ�t�@�C�����I�[�v��
			OpenDocumentFile( __argv[1] );
			if(__argc >= 3)
			{
				//���p�����[�^�Ɏw�肳�ꂽ�s�ԍ��փW�����v
				long lLine = atol(__argv[2]);
				SendJumpMsg(__argv[1], lLine);
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���C�Z���X�o�^�ς݃`�F�b�N����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::IsLicenseRegistered()
{
	//���W�X�g���ǂݍ���
	CString objLicenseKey = GetProfileString("LICENSE","LICENSEKEY","");

	//���C�Z���X�L�[�`�F�b�N
	for(long lCnt=0; lCnt<LISENCEKEY_COUNT; lCnt++)
	{
		if(objLicenseKey.Compare(g_szLisenceKeys[lCnt])==0) //OK
			return TRUE;
	}
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�]���������`�F�b�N����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorApp::IsExpired				//TRUE : ���v�AFALSE : ���p�����؂�
	(	BOOL	bDispDialog				//���p�����؂�_�C�A���O��\�����邩�H
	)
{
	CString objMsg;

	//�v���O�����N���֘A�̐ݒ�
	WriteProfileInt("EXECUTE","EXEC_COUNT",(GetProfileInt("EXECUTE","EXEC_COUNT",0)+1));

	//�\�t�g�E�G�A����N���`�F�b�N
	int nRet = GetProfileInt("AAAA","BBBB",24);
	if(nRet==24)//����
	{
		//�v���O�����N���֘A�̐ݒ�
		WriteProfileInt("EXECUTE","EXEC_COUNT",1);

		//���W�X�g���ݒ�
		WriteProfileInt("AAAA","BBBB",27);

		//�N������������
		CTime objTime = CTime::GetCurrentTime();
		WriteProfileInt("AAAA","ER",objTime.GetYear()*5);
		WriteProfileInt("AAAA","TH",objTime.GetMonth()*4);
		WriteProfileInt("AAAA","AY",objTime.GetDay()*3);

		return TRUE;
	}
	else //�N������ȍ~
	{
		if(IsLicenseRegistered()) //���C�Z���X�o�^�ς�
		{
			return TRUE;
		}
		else //���C�Z���X���o�^
		{
			CTime objTime = CTime::GetCurrentTime();
			int nYear,nMonth,nDay;
			nYear  = GetProfileInt("AAAA","ER",objTime.GetYear());
			nMonth = GetProfileInt("AAAA","TH",objTime.GetMonth());
			nDay   = GetProfileInt("AAAA","AY",objTime.GetDay());
			nYear  /= 5;
			nMonth /= 4;
			nDay   /= 3;
			CTime objFirstTime(nYear,nMonth,nDay,0,0,0);
			CTimeSpan objDiff = objTime - objFirstTime;
			CTimeSpan objKijyun(30,0,0,0); //30��
			if( objDiff > objKijyun )
			{
				//���p���Ԑ؂ꃁ�b�Z�[�W�\��
				if(bDispDialog)
				{
					objMsg.LoadString(IDS_EXPIRED);
					MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
				}
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���C�Z���X�L�[�쐬����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorApp::GenerateLisenceKeys()
{
	char	szMD5Output[64];
	char	szBuf[128];

	FILE* fp=NULL;
	fp = fopen("key.txt","w");
	if(fp==NULL)
		return;

	for(long lCnt=0; lCnt<10; lCnt++)
	{
		Sleep(2000);
		CTime	objTime = CTime::GetCurrentTime();
		CString	objStr = objTime.Format("%Y/%m/%d %H:%M:%S");
		memset(szBuf,0,sizeof(szBuf));
		strcpy(szBuf,(LPCTSTR)objStr);

		//�l�c�T�_�C�W�F�X�g����
		CMd5	objMd5;
		memset(szMD5Output,0,sizeof(szMD5Output));
		objMd5.MD5String(szBuf,szMD5Output);

		//�t�@�C���o��
		fprintf(fp,"%s\r\n",szMD5Output);
	}

	fclose(fp);
}


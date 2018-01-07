// MyPropertyPage1.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "Editor.h"
#include "MyPropertyPage1.h"
#include "ExecPasswdDlg.h"
#include "Mymd5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMyPropertyPage1, CPropertyPage)

BEGIN_MESSAGE_MAP(CMyPropertyPage1, CPropertyPage)
	//{{AFX_MSG_MAP(CMyPropertyPage1)
	ON_BN_CLICKED(IDC_CHECK_CHDIRTOOPENFILE, OnCheckChdirtoopenfile)
	ON_BN_CLICKED(IDC_CHECK_MAXCLDWND, OnCheckMaxcldwnd)
	ON_BN_CLICKED(IDC_CHECK_OPENNEWFILE, OnCheckOpennewfile)
	ON_BN_CLICKED(IDC_CHECK_SAVEWINDOWPLACE, OnCheckSavewindowplace)
	ON_EN_CHANGE(IDC_EDIT_MAXMRU, OnChangeEditMaxmru)
	ON_BN_CLICKED(IDC_CHECK_FULLTITLE, OnCheckFulltitle)
	ON_BN_CLICKED(IDC_CHECK_DISPEOFMARK, OnCheckDispeofmark)
	ON_BN_CLICKED(IDC_CHECK_NODOUBLEEXECUTE, OnCheckNodoubleexecute)
	ON_BN_CLICKED(IDC_CHECK_SHOWSPLASH, OnCheckShowsplash)
	ON_EN_CHANGE(IDC_EDIT_SPLASHTIME, OnChangeEditSplashtime)
	ON_BN_CLICKED(IDC_CHECK_EXECPASSWD, OnCheckExecpasswd)
	ON_BN_CLICKED(IDC_SETPASSWDBTN, OnSetpasswdbtn)
	ON_BN_CLICKED(IDC_CHECK_ENABLESHELLOPEN, OnCheckEnableshellopen)
	ON_BN_CLICKED(IDC_CHECK_MAKESHORTCUTDSKTOP, OnCheckMakeshortcutdsktop)
	ON_BN_CLICKED(IDC_CHECK_IMMAUTOOPEN, OnCheckImmautoopen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage1 プロパティ ページ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage1::CMyPropertyPage1() : CPropertyPage(CMyPropertyPage1::IDD)
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsChanged = FALSE;								//変更発生フラグ
	m_lSplashTime = 0;
	m_objExecPasswdStr = _T("");

	//{{AFX_DATA_INIT(CMyPropertyPage1)
	m_bShowSplash = FALSE;
	m_bIsOpenNewFile = FALSE;
	m_bIsChdirToOpenFile = FALSE;
	m_bMaximizeWindow = FALSE;
	m_bIsSaveWindowPlace = FALSE;
	m_nMaxMRU = 0;
	m_bIsFullTitle = FALSE;
	m_bIsDispEOFMark = FALSE;
	m_bIsNoDoubleExecute = FALSE;
	m_bIsExecPasswd = FALSE;
	m_bIsEnableShellOpen = FALSE;
	m_bIsMakeShotcutDsktop = FALSE;
	m_bIsImmAutoOpen = FALSE;
	//}}AFX_DATA_INIT
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage1::~CMyPropertyPage1()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＤＤＸデータ交換														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage1)
	DDX_Check(pDX, IDC_CHECK_SHOWSPLASH, m_bShowSplash);
	DDX_Control(pDX, IDC_EDIT_SPLASHTIME, m_objSplashTimeEdit);
	DDX_Check(pDX, IDC_CHECK_OPENNEWFILE, m_bIsOpenNewFile);
	DDX_Check(pDX, IDC_CHECK_CHDIRTOOPENFILE, m_bIsChdirToOpenFile);
	DDX_Check(pDX, IDC_CHECK_MAXCLDWND, m_bMaximizeWindow);
	DDX_Check(pDX, IDC_CHECK_SAVEWINDOWPLACE, m_bIsSaveWindowPlace);
	DDX_Text(pDX, IDC_EDIT_MAXMRU, m_nMaxMRU);
	DDV_MinMaxInt(pDX, m_nMaxMRU, 4, 16);
	DDX_Check(pDX, IDC_CHECK_FULLTITLE, m_bIsFullTitle);
	DDX_Check(pDX, IDC_CHECK_DISPEOFMARK, m_bIsDispEOFMark);
	DDX_Check(pDX, IDC_CHECK_NODOUBLEEXECUTE, m_bIsNoDoubleExecute);
	DDX_Check(pDX, IDC_CHECK_EXECPASSWD, m_bIsExecPasswd);
	DDX_Control(pDX, IDC_SETPASSWDBTN, m_objSetPasswdBtn);
	DDX_Check(pDX, IDC_CHECK_ENABLESHELLOPEN, m_bIsEnableShellOpen);
	DDX_Check(pDX, IDC_CHECK_MAKESHORTCUTDSKTOP, m_bIsMakeShotcutDsktop);
	DDX_Check(pDX, IDC_CHECK_IMMAUTOOPEN, m_bIsImmAutoOpen);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage1 パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：変更発生調査処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage1::IsChanged() 
{
	return m_bIsChanged;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：変更発生フラグクリア処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::ClearChangeFlag()
{
	m_bIsChanged = FALSE;
	SetModified(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage1 メッセージハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage1::OnInitDialog() 
{
	//規定クラスのハンドラコール
	CPropertyPage::OnInitDialog();

	//フォント設定
	m_objSplashTimeEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	CWnd* pWnd = GetDlgItem(IDC_EDIT_MAXMRU);
	pWnd->SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//スプラッシュスクリーン表示時間エディット有効／無効設定
	if( m_bShowSplash )
		m_objSplashTimeEdit.EnableWindow(TRUE);
	else
		m_objSplashTimeEdit.EnableWindow(FALSE);

	//スプラッシュスクリーン表示時間エディット初期化
	char	szTemp[32];
	sprintf(szTemp,"%d",m_lSplashTime);
	m_objSplashTimeEdit.SetWindowText(szTemp);
	m_objSplashTimeEdit.UpdateData(TRUE);

	//起動パスワード設定ボタン有効／無効設定
	if( m_bIsExecPasswd )
		m_objSetPasswdBtn.EnableWindow(TRUE);
	else
		m_objSetPasswdBtn.EnableWindow(FALSE);

	//メンバ変数－＞コントロール
	UpdateData(FALSE);

	//変更発生フラグクリア
	m_bIsChanged = FALSE;

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「スプラッシュスクリーン表示」チェック時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckShowsplash() 
{
	m_bShowSplash = (m_bShowSplash!=FALSE) ? FALSE : TRUE;

	m_bIsChanged = TRUE;
	SetModified();

	//スプラッシュスクリーン表示時間エディット有効／無効設定
	if( m_bShowSplash )
		m_objSplashTimeEdit.EnableWindow(TRUE);
	else
		m_objSplashTimeEdit.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：スプラッシュスクリーン表示時間エディット変更時処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnChangeEditSplashtime() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「複数起動防止」チェック時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckNodoubleexecute() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「オープンファイルディレクトリ移動」チェック時処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckChdirtoopenfile() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「子ウインドウ作成時最大化」チェック時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckMaxcldwnd() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「起動時新規子ウインドウオープン」チェック時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckOpennewfile() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「メインフレームウインドウ位置サイズ保存」チェック時処理				|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckSavewindowplace() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイルタイトルフルパス表示」チェック時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckFulltitle() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ＥＯＦ表示」チェック時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckDispeofmark() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「起動時にパスワードチェックする」チェック時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckExecpasswd() 
{
	m_bIsExecPasswd = (m_bIsExecPasswd!=FALSE) ? FALSE : TRUE;

	m_bIsChanged = TRUE;
	SetModified();

	//起動パスワード設定ボタン有効／無効設定
	if( m_bIsExecPasswd )
		m_objSetPasswdBtn.EnableWindow(TRUE);
	else
		m_objSetPasswdBtn.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：起動時パスワード設定ボタン押下時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnSetpasswdbtn() 
{
	CString		objNewPasswd, objNewPasswdMD5, objOldPasswdMD5;
	char		szNewPasswd[1024], szNewPasswdMD5[1024];

	//パスワード設定ダイアログ表示
	CExecPasswdDlg	objExecPasswdDlg;
	if(objExecPasswdDlg.DoModal() != IDOK)
		return;

	//新パスワード取得
	objNewPasswd.Empty();
	objNewPasswd = objExecPasswdDlg.m_objExecPasswdStr;
	memset(szNewPasswd,0,sizeof(szNewPasswd));
	strcpy(szNewPasswd,(LPCTSTR)objNewPasswd);

	//新パスワードのＭＤ５を取得
	CMd5	objMd5;
	memset(szNewPasswdMD5,0,sizeof(szNewPasswdMD5));
	objMd5.MD5String(szNewPasswd,szNewPasswdMD5);
	objNewPasswdMD5 = szNewPasswdMD5;

	//パスワードが変わったかどうかをチェック
	objOldPasswdMD5 = m_pApp->GetProfileString("CEditorApp","ExecPasswd","");
	if(objOldPasswdMD5.Compare(objNewPasswdMD5) != 0) //違う
	{
		m_bIsChanged = TRUE;
		SetModified();

		//ＭＤ５変換後の新パスワード保持
		m_objExecPasswdStr = objNewPasswdMD5;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「エクスプローラから開けるようにする」チェック時処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckEnableshellopen() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「デスクトップ上にXerxesへのショートカット作成」チェック時処理		|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckMakeshortcutdsktop() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：最新ファイル数エディットボックス変更時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnChangeEditMaxmru() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「IMM自動オープン」チェック時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckImmautoopen() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ＯＫ」ボタン押下時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnOK() 
{
	CString objMsg;

	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	if(m_bShowSplash)
	{
		CString objStr;
		m_objSplashTimeEdit.GetWindowText(objStr);
		m_lSplashTime = atoi((char*)(LPCTSTR)objStr);

		//スプラッシュスクリーン表示時間の妥当性チェック
		if(m_lSplashTime < 1 || m_lSplashTime > 10)
		{
			objMsg.LoadString(IDS_SPLASHTIME_ERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONEXCLAMATION),0);
		}
	}
	if(m_bIsExecPasswd)
	{
		if(m_objExecPasswdStr.IsEmpty())
		{
			objMsg.LoadString(IDS_SET_PASSWD);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		}
	}

	CPropertyPage::OnOK();
}



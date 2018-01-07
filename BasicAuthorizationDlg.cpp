// BasicAuthorizationDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "Editor_Def.h"
#include "AutoResizer.h"
#include "BasicAuthorizationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBasicAuthorizationDlg クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CBasicAuthorizationDlg::CBasicAuthorizationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBasicAuthorizationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBasicAuthorizationDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_objAuthSite = _T("");
	m_objAuthRealm = _T("");
	m_objAuthUser = _T("");
	m_objAuthPasswd = _T("");
	m_bIsSaveThisPassword = FALSE;
	m_hIcon = m_pApp->LoadIcon(IDR_MAINFRAME);
	m_hIconS = (HICON)::LoadImage(m_pApp->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME),
			IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＤＤＸデータ交換														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBasicAuthorizationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBasicAuthorizationDlg)
	DDX_Control(pDX, IDC_EDIT_AUTH_USER, m_objAuthUserEdit);
	DDX_Control(pDX, IDC_EDIT_AUTH_PASSWD, m_objAuthPasswdEdit);
	DDX_Check(pDX, IDC_CHECK_SAVETHISPASSWD, m_bIsSaveThisPassword);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBasicAuthorizationDlg, CDialog)
	//{{AFX_MSG_MAP(CBasicAuthorizationDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBasicAuthorizationDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CBasicAuthorizationDlg::OnInitDialog()
{
	CString objMsg,objKey;
	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//オートリサイズ
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_AUTH_SITE,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_AUTH_REALM,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_EDIT_AUTH_USER,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_EDIT_AUTH_PASSWD,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_CHECK_SAVETHISPASSWD,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDOK,CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDCANCEL,CP_MOVE_HORIZONTAL);

	//ウインドウ位置復元
	WINDOWPLACEMENT		wp;
	if(LoadWindowPlacement(&wp))
		SetWindowPlacement(&wp);

	//オートリサイズ起動
	m_objAutoResizer.MoveControls();

	//ユーザＩＤ、パスワード保存値復元
	objKey.Format("%s_%s_bIsSaveThisPassword",m_objAuthSite,m_objAuthRealm);
	m_bIsSaveThisPassword = m_pApp->GetProfileInt("CBasicAuthorizationDlg",objKey,FALSE);
	if(m_bIsSaveThisPassword)
	{
		objKey.Format("%s_%s_userid",m_objAuthSite,m_objAuthRealm);
		m_objAuthUser = m_pApp->GetProfileString("CBasicAuthorizationDlg",objKey,"");
		objKey.Format("%s_%s_passwd",m_objAuthSite,m_objAuthRealm);
		m_objAuthPasswd = m_pApp->GetProfileString("CBasicAuthorizationDlg",objKey,"");
	}
	else
	{
		m_objAuthUser = "";
		m_objAuthPasswd = "";
	}

	//サイト、領域情報、ユーザＩＤ、パスワードを表示
	SetDlgItemText(IDC_AUTH_SITE, m_objAuthSite);
	SetDlgItemText(IDC_AUTH_REALM, m_objAuthRealm);
	m_objAuthUserEdit.SetWindowText(m_objAuthUser);
	m_objAuthUserEdit.UpdateData(TRUE);
	m_objAuthPasswdEdit.SetWindowText(m_objAuthPasswd);
	m_objAuthPasswdEdit.UpdateData(TRUE);

	//メンバ変数－＞コントロール
	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＳＩＺＥ処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBasicAuthorizationDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	//オートリサイズ起動
	m_objAutoResizer.MoveControls();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＯＫボタン押下時処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBasicAuthorizationDlg::OnOK() 
{
	CString objMsg,objKey;

	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	//データ取得
	m_objAuthUserEdit.GetWindowText(m_objAuthUser);
	m_objAuthPasswdEdit.GetWindowText(m_objAuthPasswd);

	//ブランクチェック
	if(m_objAuthUser.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_USERID);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if(m_objAuthPasswd.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_PASSWORD);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//ユーザＩＤとパスワードを保存
	if(m_bIsSaveThisPassword)
	{
		objKey.Format("%s_%s_bIsSaveThisPassword",m_objAuthSite,m_objAuthRealm);
		m_pApp->WriteProfileInt("CBasicAuthorizationDlg",objKey,m_bIsSaveThisPassword);
		objKey.Format("%s_%s_userid",m_objAuthSite,m_objAuthRealm);
		m_pApp->WriteProfileString("CBasicAuthorizationDlg",objKey,m_objAuthUser);
		objKey.Format("%s_%s_passwd",m_objAuthSite,m_objAuthRealm);
		m_pApp->WriteProfileString("CBasicAuthorizationDlg",objKey,m_objAuthPasswd);
	}
	else
	{
		objKey.Format("%s_%s_bIsSaveThisPassword",m_objAuthSite,m_objAuthRealm);
		m_pApp->WriteProfileInt("CBasicAuthorizationDlg",objKey,m_bIsSaveThisPassword);
		objKey.Format("%s_%s_userid",m_objAuthSite,m_objAuthRealm);
		m_pApp->WriteProfileString("CBasicAuthorizationDlg",objKey,"");
		objKey.Format("%s_%s_passwd",m_objAuthSite,m_objAuthRealm);
		m_pApp->WriteProfileString("CBasicAuthorizationDlg",objKey,"");
	}

	//ウインドウの状態と位置を記憶させる
	WINDOWPLACEMENT		wp;
	if(GetWindowPlacement(&wp))
	{
		if(IsZoomed())
			wp.flags |= WPF_RESTORETOMAXIMIZED;
		SaveWindowPlacement(&wp);
	}

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CBasicAuthorizationDlg プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウ位置ロード処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CBasicAuthorizationDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CBasicAuthorizationDlg","WindowPos");
	if(strBuffer.IsEmpty())
		return FALSE;

	int cRead = _stscanf(strBuffer,"%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
					&pwp->flags,
					&pwp->showCmd,
					&pwp->ptMinPosition.x, &pwp->ptMinPosition.y,
					&pwp->ptMaxPosition.x, &pwp->ptMaxPosition.y,
					&pwp->rcNormalPosition.left,  &pwp->rcNormalPosition.top,
					&pwp->rcNormalPosition.right, &pwp->rcNormalPosition.bottom);
	if(cRead != 10)
		return FALSE;
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウ位置セーブ処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBasicAuthorizationDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	//ウインドウの位置を保存する
	CString	strBuffer;
	strBuffer.Format("%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
					pwp->flags,
					pwp->showCmd,
					pwp->ptMinPosition.x, pwp->ptMinPosition.y,
					pwp->ptMaxPosition.x, pwp->ptMaxPosition.y,
					pwp->rcNormalPosition.left,  pwp->rcNormalPosition.top,
					pwp->rcNormalPosition.right, pwp->rcNormalPosition.bottom);
	m_pApp->WriteProfileString("CBasicAuthorizationDlg","WindowPos",strBuffer);
}


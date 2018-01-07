// CheckExecPasswdDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "Editor_Def.h"
#include "CheckExecPasswdDlg.h"
#include "Mymd5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckExecPasswdDlg クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CCheckExecPasswdDlg::CCheckExecPasswdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckExecPasswdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckExecPasswdDlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT

	m_bIsPasswdOK = FALSE;
	m_pApp = (CEditorApp*)AfxGetApp();
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
void CCheckExecPasswdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckExecPasswdDlg)
	DDX_Control(pDX, IDC_EDIT_EXECPASSWD, m_objExecPasswdEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckExecPasswdDlg, CDialog)
	//{{AFX_MSG_MAP(CCheckExecPasswdDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckExecPasswdDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CCheckExecPasswdDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//フォント設定
	m_objExecPasswdEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//デフォルトデータ設定
	m_objExecPasswdEdit.SetWindowText(_T(""));
	m_objExecPasswdEdit.UpdateData(TRUE);

	m_bIsPasswdOK = FALSE;

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//メンバ変数－＞コントロール
	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＯＫボタン押下時処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CCheckExecPasswdDlg::OnOK() 
{
	CString	objTypedPasswd, objTypedPasswdMD5, objOldPasswdMD5;
	char	szTypedPasswd[1024], szTypedPasswdMD5[1024];

	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	//データ取得
	objTypedPasswd.Empty();
	m_objExecPasswdEdit.GetWindowText(objTypedPasswd);
	memset(szTypedPasswd,0,sizeof(szTypedPasswd));
	strcpy(szTypedPasswd,(LPCTSTR)objTypedPasswd);

	//入力されたパスワードのＭＤ５を取得
	CMd5	objMd5;
	memset(szTypedPasswdMD5,0,sizeof(szTypedPasswdMD5));
	objMd5.MD5String(szTypedPasswd,szTypedPasswdMD5);
	objTypedPasswdMD5 = szTypedPasswdMD5;

	//パスワードチェック
	objOldPasswdMD5 = m_pApp->GetProfileString("CEditorApp","ExecPasswd","");
	if(objOldPasswdMD5.Compare(objTypedPasswdMD5) != 0) //違う
	{
		CString objMsg;
		objMsg.LoadString(IDS_PASSWORD_NG);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		m_bIsPasswdOK = FALSE;
		return;
	}
	else
	{
		m_bIsPasswdOK = TRUE;
		CDialog::OnOK();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＣＡＮＣＥＬボタン押下時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CCheckExecPasswdDlg::OnCancel() 
{
	m_bIsPasswdOK = FALSE;

	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// CCheckExecPasswdDlg パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：パスワードチェック結果取得											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CCheckExecPasswdDlg::IsPasswdOK()
{
	return m_bIsPasswdOK;
}


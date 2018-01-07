// EncriptStatusDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "StaticFilespec.h"
#include "EncriptStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEncriptStatusDlg ダイアログ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CEncriptStatusDlg::CEncriptStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEncriptStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEncriptStatusDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsCanceled = 0;
	m_lEncriptFileCount = 0;
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
void CEncriptStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEncriptStatusDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEncriptStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CEncriptStatusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEncriptStatusDlg パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャンセルチェック													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEncriptStatusDlg::IsCanceled()
{
	return m_bIsCanceled;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル種別メッセージ設定											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEncriptStatusDlg::SetFileKind
	(	CString&	objStr
	)
{
	SetDlgItemText(IDC_FILEKIND,objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：フォルダメッセージ設定												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEncriptStatusDlg::SetFolder
	(	CString&		objStr
	)
{
	m_wndStaticFolder.SetText(objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイルメッセージ設定												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEncriptStatusDlg::SetFile
	(	CString&	objStr
	)
{
	SetDlgItemText(IDC_FILE, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：暗号化ファイル件数カウントアップ処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEncriptStatusDlg::IncEncriptFileCount()
{
	m_lEncriptFileCount++;
	CString	objStr;
	objStr.Format("%ld",m_lEncriptFileCount);
	SetDlgItemText(IDC_FILECOUNT, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：暗号化ファイル件数取得												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEncriptStatusDlg::GetEncriptFileCount()
{
	return m_lEncriptFileCount;
}

/////////////////////////////////////////////////////////////////////////////
// CEncriptStatusDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEncriptStatusDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//暗号化ファイル件数初期化
	CString	objStr;
	objStr.Format("%ld",m_lEncriptFileCount);
	SetDlgItemText(IDC_FILECOUNT, objStr);

	//フォルダ表示サブクラス化
	m_wndStaticFolder.SubclassDlgItem(IDC_FOLDER, this);
	m_wndStaticFolder.SetPath(TRUE);

	//メンバ変数－＞コントロール
	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャンセルボタン押下時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEncriptStatusDlg::OnCancel() 
{
	m_bIsCanceled = TRUE;
	CDialog::OnCancel();
}


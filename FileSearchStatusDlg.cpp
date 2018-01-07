// FileSearchStatusDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "StaticFilespec.h"
#include "FileSearchStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileSearchStatusDlg ダイアログ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFileSearchStatusDlg::CFileSearchStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileSearchStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileSearchStatusDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsCanceled = 0;
	m_lMatchCount = 0;
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
void CFileSearchStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileSearchStatusDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileSearchStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CFileSearchStatusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileSearchStatusDlg パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャンセルチェック													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileSearchStatusDlg::IsCanceled()
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
void CFileSearchStatusDlg::SetFileKind
	(	CString&	objStr
	)
{
	SetDlgItemText(IDC_FILEKIND,objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：検索フォルダメッセージ設定											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileSearchStatusDlg::SetFolder
	(	CString&		objStr
	)
{
	m_wndStaticFolder.SetText(objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：検索ファイルメッセージ設定											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileSearchStatusDlg::SetFile
	(	CString&	objStr
	)
{
	SetDlgItemText(IDC_FILE, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：該当件数カウントアップ処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileSearchStatusDlg::IncMatchCount()
{
	m_lMatchCount++;
	CString	objStr;
	objStr.Format("%ld",m_lMatchCount);
	SetDlgItemText(IDC_MATCHCOUNT, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：該当件数取得処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CFileSearchStatusDlg::GetMatchCount()
{
	return m_lMatchCount;
}

/////////////////////////////////////////////////////////////////////////////
// CFileSearchStatusDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileSearchStatusDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//フォルダ
	m_wndStaticFolder.SubclassDlgItem(IDC_FOLDER, this);
	m_wndStaticFolder.SetPath(TRUE);

	//該当件数初期化
	CString	objStr;
	objStr.Format("%ld",m_lMatchCount);
	SetDlgItemText(IDC_MATCHCOUNT, objStr);

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
void CFileSearchStatusDlg::OnCancel() 
{
	m_bIsCanceled = TRUE;
	CDialog::OnCancel();
}


// DirCompareStatusDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "StaticFilespec.h"
#include "DirCompareStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirCompareStatusDlg ダイアログ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDirCompareStatusDlg::CDirCompareStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDirCompareStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDirCompareStatusDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsCanceled = FALSE;
	m_lCompareFileCount = 0;
	m_lAddFileCount = 0;
	m_lDelFileCount = 0;
	m_lDiffFileCount = 0;
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
void CDirCompareStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDirCompareStatusDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDirCompareStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CDirCompareStatusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirCompareStatusDlg パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャンセルチェック													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDirCompareStatusDlg::IsCanceled()
{
	return m_bIsCanceled;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：比較第一ディレクトリ設定												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareStatusDlg::SetDir1
	(	CString&	objDir
	)
{
	m_wndStaticDirName1.SetText(objDir);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：比較第二ディレクトリ設定												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareStatusDlg::SetDir2
	(	CString&	objDir
	)
{
	m_wndStaticDirName2.SetText(objDir);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：比較状況設定															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareStatusDlg::SetStatus
	(	char*	szMsg
	)
{
	CString objStr = szMsg;
	SetDlgItemText(IDC_STATUS, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：比較状況設定															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareStatusDlg::SetStatus
	(	CString& objStr
	)
{
	SetDlgItemText(IDC_STATUS, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：比較ファイル数カウントアップ											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareStatusDlg::IncCompareFileCount()
{
	m_lCompareFileCount++;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：比較ファイル数取得													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CDirCompareStatusDlg::GetCompareFileCount()
{
	return m_lCompareFileCount;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：追加ファイル数カウントアップ											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareStatusDlg::IncAddFileCount()
{
	m_lAddFileCount++;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：追加ファイル数取得													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CDirCompareStatusDlg::GetAddFileCount()
{
	return m_lAddFileCount;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：削除ファイル数カウントアップ											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareStatusDlg::IncDelFileCount()
{
	m_lDelFileCount++;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：削除ファイル数取得													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CDirCompareStatusDlg::GetDelFileCount()
{
	return m_lDelFileCount;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：相違ファイル数カウントアップ											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareStatusDlg::IncDiffFileCount()
{
	m_lDiffFileCount++;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：相違ファイル数取得													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CDirCompareStatusDlg::GetDiffFileCount()
{
	return m_lDiffFileCount;
}

/////////////////////////////////////////////////////////////////////////////
// CDirCompareStatusDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDirCompareStatusDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//第一ディレクトリ
	m_wndStaticDirName1.SubclassDlgItem(IDC_DIR1, this);
	m_wndStaticDirName1.SetPath(TRUE);
	//第二ディレクトリ
	m_wndStaticDirName2.SubclassDlgItem(IDC_DIR2, this);
	m_wndStaticDirName2.SetPath(TRUE);

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
void CDirCompareStatusDlg::OnCancel() 
{
	m_bIsCanceled = TRUE;
	CDialog::OnCancel();
}


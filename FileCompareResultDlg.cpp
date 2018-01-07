// FileCompareResultDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "StaticFilespec.h"
#include "FileCompareResultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileCompareResultDlg ダイアログ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFileCompareResultDlg::CFileCompareResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileCompareResultDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileCompareResultDlg)
	m_objAddLineCountStr = _T("");
	m_objDelLineCountStr = _T("");
	m_objStatusStr = _T("");
	m_objTimeStr = _T("");
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_objFile1Name = _T("");
	m_objFile2Name = _T("");
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
void CFileCompareResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileCompareResultDlg)
	DDX_Text(pDX, IDC_ADDLINE_CNT, m_objAddLineCountStr);
	DDX_Text(pDX, IDC_DELLINE_CNT, m_objDelLineCountStr);
	DDX_Text(pDX, IDC_COMPARE_STATUS, m_objStatusStr);
	DDX_Text(pDX, IDC_TIME, m_objTimeStr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileCompareResultDlg, CDialog)
	//{{AFX_MSG_MAP(CFileCompareResultDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileCompareResultDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileCompareResultDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//第一ファイル名表示
	m_wndStaticFileName1.SubclassDlgItem(IDC_FILE1_NAME, this);
	m_wndStaticFileName1.SetPath(TRUE);
	SetDlgItemText(IDC_FILE1_NAME, m_objFile1Name);
	//第二ファイル名表示
	m_wndStaticFileName2.SubclassDlgItem(IDC_FILE2_NAME, this);
	m_wndStaticFileName2.SetPath(TRUE);
	SetDlgItemText(IDC_FILE2_NAME, m_objFile2Name);

	//メンバ変数－＞コントロール
	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


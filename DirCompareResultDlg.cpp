// DirCompareResultDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "Editor_Def.h"
#include "StaticFilespec.h"
#include "DirCompareResultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirCompareResultDlg ダイアログ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDirCompareResultDlg::CDirCompareResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDirCompareResultDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDirCompareResultDlg)
	m_objStatusStr = _T("");
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_objDir1Name = _T("");
	m_objDir2Name = _T("");
	m_lCompareFileCount = 0;
	m_lAddFileCount = 0;
	m_lDelFileCount = 0;
	m_lDiffFileCount = 0;
	m_lTime = 0;
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
void CDirCompareResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDirCompareResultDlg)
	DDX_Text(pDX, IDC_COMPARE_STATUS, m_objStatusStr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDirCompareResultDlg, CDialog)
	//{{AFX_MSG_MAP(CDirCompareResultDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirCompareResultDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDirCompareResultDlg::OnInitDialog() 
{
	CString objStr = _T("");
	CString objMsg;

	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//第一ディレクトリ名表示
	m_wndStaticDirName1.SubclassDlgItem(IDC_DIR1_NAME, this);
	m_wndStaticDirName1.SetPath(TRUE);
	SetDlgItemText(IDC_DIR1_NAME, m_objDir1Name);
	//第二ディレクトリ名表示
	m_wndStaticDirName2.SubclassDlgItem(IDC_DIR2_NAME, this);
	m_wndStaticDirName2.SetPath(TRUE);
	SetDlgItemText(IDC_DIR2_NAME, m_objDir2Name);

	//比較ファイル総数
	objStr.Format("%6ld",m_lCompareFileCount);
	SetDlgItemText(IDC_COMPAREFILE_CNT,objStr);
	//削除ファイル数
	objStr.Format("%6ld",m_lAddFileCount);
	SetDlgItemText(IDC_ADDFILE_CNT,objStr);
	//追加ファイル数
	objStr.Format("%6ld",m_lDelFileCount);
	SetDlgItemText(IDC_DELFILE_CNT,objStr);
	//相違ファイル数
	objStr.Format("%6ld",m_lDiffFileCount);
	SetDlgItemText(IDC_DIFFILE_CNT,objStr);

	//所要時間
	objMsg.LoadString(IDS_TIME);
	objStr.Format((LPCTSTR)objMsg, m_lTime);
	SetDlgItemText(IDC_TIME,objStr);

	//メンバ変数－＞コントロール
	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


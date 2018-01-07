// HtmlOpenStatusDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "HtmlOpenStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHtmlOpenStatusDlg ダイアログ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CHtmlOpenStatusDlg::CHtmlOpenStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHtmlOpenStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHtmlOpenStatusDlg)
	//}}AFX_DATA_INIT

	m_bIsCanceled = FALSE;
	m_pApp = (CEditorApp*)AfxGetApp();
	m_hIcon = m_pApp->LoadIcon(IDR_MAINFRAME);
	m_hIconS = (HICON)::LoadImage(m_pApp->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME),
			IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	m_nProgressLower = 0;
	m_nProgressUpper = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＤＤＸデータ交換														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CHtmlOpenStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHtmlOpenStatusDlg)
	DDX_Control(pDX, IDC_PROGRESS_BYTE, m_objProgressBar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHtmlOpenStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CHtmlOpenStatusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmlOpenStatusDlg パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャンセルチェック													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CHtmlOpenStatusDlg::IsCanceled()
{
	return m_bIsCanceled;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスメッセージ設定												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CHtmlOpenStatusDlg::SetStatus
	(	char*	pszMsg
	)
{
	if(IsCanceled())
		return;

	CString objStr = pszMsg;
	SetDlgItemText(IDC_STATUS, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスメッセージ設定												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CHtmlOpenStatusDlg::SetStatus
	(	CString& objStr
	)
{
	if(IsCanceled())
		return;
	SetDlgItemText(IDC_STATUS, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：プログレスバー範囲設定処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CHtmlOpenStatusDlg::SetRange
	(	int nLower,		//範囲の下限
		int nUpper		//範囲の上限
	)
{
	if(IsCanceled())
		return;

	//プログレスバーの範囲設定
	m_objProgressBar.SetRange(nLower,nUpper);
	m_nProgressLower = nLower;
	m_nProgressUpper = nUpper;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：プログレスバー位置設定処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CHtmlOpenStatusDlg::SetPos
	(	int nPos		//位置（SetRangeで設定した範囲内であること）
	)
{
	if(IsCanceled())
		return;
	if(nPos < m_nProgressLower || nPos > m_nProgressUpper)
		return;

	//プログレスバーの現在値設定
	m_objProgressBar.SetPos(nPos);

	//バイト数情報設定
	CString objBuf;
	float percent=0;
	if(m_nProgressUpper != 0)
		percent = (float)((nPos * 100) / m_nProgressUpper);
	objBuf.Format("%5d / %5d ( %3.1f %% )", nPos, m_nProgressUpper, percent);

	SetDlgItemText(IDC_BYTE, objBuf);
}

/////////////////////////////////////////////////////////////////////////////
// CHtmlOpenStatusDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CHtmlOpenStatusDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	SetRange(0,0);
	SetPos(0);
	SetDlgItemText(IDC_BYTE, "");

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
void CHtmlOpenStatusDlg::OnCancel() 
{
	m_bIsCanceled = TRUE;
	CDialog::OnCancel();
}


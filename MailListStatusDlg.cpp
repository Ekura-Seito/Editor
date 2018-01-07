// MailListStatusDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "MailListStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailListStatusDlg ダイアログ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMailListStatusDlg::CMailListStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMailListStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMailListStatusDlg)
	//}}AFX_DATA_INIT

	m_bIsCanceled = FALSE;
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
void CMailListStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailListStatusDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMailListStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CMailListStatusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailListStatusDlg パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャンセルチェック													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMailListStatusDlg::IsCanceled()
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
void CMailListStatusDlg::SetStatus
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
void CMailListStatusDlg::SetStatus
	(	CString& objStr
	)
{
	if(IsCanceled())
		return;
	SetDlgItemText(IDC_STATUS, objStr);
}

/////////////////////////////////////////////////////////////////////////////
// CMailListStatusDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMailListStatusDlg::OnInitDialog() 
{
	int nCurrentAccountNo=0;
	CString objMsg;
	char szSection[64];
	CWnd*	pWnd=NULL;

	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	nCurrentAccountNo = m_pApp->GetProfileInt("Mail","CurrentAccountNo",0);
	if(nCurrentAccountNo < 0 || nCurrentAccountNo > 8)
		nCurrentAccountNo = 0;
	sprintf(szSection,"Mail_Account%d",nCurrentAccountNo);

	//アカウント名設定
	objMsg.LoadString(IDS_MAILACCOUNT_0 + nCurrentAccountNo);
	pWnd = GetDlgItem(IDC_LISTACCOUNT);
	if(pWnd != NULL)
		pWnd->SetWindowText(objMsg);

	//ＰＯＰ３サーバ名設定
	objMsg = m_pApp->GetProfileString(szSection,"PopServer","");
	pWnd = GetDlgItem(IDC_LISTSERVER);
	if(pWnd != NULL)
		pWnd->SetWindowText(objMsg);

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
void CMailListStatusDlg::OnCancel() 
{
	m_bIsCanceled = TRUE;
	CDialog::OnCancel();
}


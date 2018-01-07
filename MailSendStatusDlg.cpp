// MailSendStatusDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "MailSendStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailSendStatusDlg ダイアログ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMailSendStatusDlg::CMailSendStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMailSendStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMailSendStatusDlg)
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
void CMailSendStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailSendStatusDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMailSendStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CMailSendStatusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailSendStatusDlg パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャンセルチェック													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMailSendStatusDlg::IsCanceled()
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
void CMailSendStatusDlg::SetStatus
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
void CMailSendStatusDlg::SetStatus
	(	CString&	objStr
	)
{
	if(IsCanceled())
		return;
	SetDlgItemText(IDC_STATUS, objStr);
}

/////////////////////////////////////////////////////////////////////////////
// CMailSendStatusDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMailSendStatusDlg::OnInitDialog() 
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
	pWnd = GetDlgItem(IDC_MSENDACCOUNT);
	if(pWnd != NULL)
		pWnd->SetWindowText(objMsg);

	//ＳＭＴＰサーバ名設定
	objMsg = m_pApp->GetProfileString(szSection,"SmtpServer","");
	pWnd = GetDlgItem(IDC_MSENDSERVER);
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
void CMailSendStatusDlg::OnCancel() 
{
	m_bIsCanceled = TRUE;
	CDialog::OnCancel();
}


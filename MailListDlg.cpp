// MailListDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <io.h>				//for _access
#pragma warning(disable:4786)
#include "editor_def.h"
#include "editor.h"
#include "ListCtrlEx.h"
#include "AutoResizer.h"
#include "MailListDlg.h"
#include "Pop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailListDlg ダイアログ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMailListDlg::CMailListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMailListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMailListDlg)
	m_objPopServerStr = _T("");
	m_objPopUserStr = _T("");
	//}}AFX_DATA_INIT

	m_pPop = NULL;
	m_pApp = (CEditorApp*)AfxGetApp();
	m_nMailCount = 0;
	m_objPopPasswdStr = _T("");
	//イメージリスト作成
	m_AttachImageList.Create(IDB_MAILATTACH, 16, 1, RGB(255, 255, 255));
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
void CMailListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailListDlg)
	DDX_Control(pDX, IDC_LIST_MAIL, m_objListMail);
	DDX_Text(pDX, IDC_POPSERVER, m_objPopServerStr);
	DDX_Text(pDX, IDC_POPUSER, m_objPopUserStr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMailListDlg, CDialog)
	//{{AFX_MSG_MAP(CMailListDlg)
	ON_BN_CLICKED(IDC_RECVBTN, OnRecvbtn)
	ON_BN_CLICKED(IDC_DELBTN, OnDelbtn)
	ON_BN_CLICKED(IDC_REFRESHBTN, OnRefreshbtn)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailListDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMailListDlg::OnInitDialog() 
{
	//基底クラスのハンドラコール
	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//イメージリストセット
	m_objListMail.SetImageList(&m_AttachImageList, LVSIL_SMALL);

	//リストヘッダ
	m_objListMail.InsertColumn(0, "№", LVCFMT_LEFT, 45);
	m_objListMail.InsertColumn(1, "Subject", LVCFMT_LEFT, 180);
	m_objListMail.InsertColumn(2, "From", LVCFMT_LEFT, 160);
	m_objListMail.InsertColumn(3, "Date", LVCFMT_LEFT, 170);
	m_objListMail.InsertColumn(4, "Size", LVCFMT_RIGHT, 60);

	//メールリスト作成
	DispMailList();

	//レポートビュー
	m_objListMail.ModifyStyle( LVS_TYPEMASK, LVS_REPORT);

	//受信済みメール保存先ディレクトリ存在チェック
	if( _access(m_pApp->m_strRecvMailPath,0) == -1 )
		GetDlgItem(IDC_RECVBTN)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_RECVBTN)->EnableWindow(TRUE);

	//オートリサイズ
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_POPSERVER,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_POPUSER,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_MAILCOUNT,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_LIST_MAIL,CP_RESIZE_HORIZONTAL|CP_RESIZE_VERTICAL);
	m_objAutoResizer.AddControl(IDC_RECVBTN,CP_MOVE_HORIZONTAL|CP_MOVE_VERTICAL);
	m_objAutoResizer.AddControl(IDC_DELBTN,CP_MOVE_HORIZONTAL|CP_MOVE_VERTICAL);
	m_objAutoResizer.AddControl(IDC_REFRESHBTN,CP_MOVE_HORIZONTAL|CP_MOVE_VERTICAL);
	m_objAutoResizer.AddControl(IDOK,CP_MOVE_HORIZONTAL|CP_MOVE_VERTICAL);

	//ウインドウ位置復元
	WINDOWPLACEMENT		wp;
	if(LoadWindowPlacement(&wp))
		SetWindowPlacement(&wp);

	//オートリサイズ起動
	m_objAutoResizer.MoveControls();

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
void CMailListDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	//オートリサイズ起動
	m_objAutoResizer.MoveControls();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「メール受信」ボタン押下時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMailListDlg::OnRecvbtn() 
{
	int 	nCnt=0,nSelectCnt=0,nRecvMailCnt=0;
	LV_ITEM lvi;
	BOOL	bRet=FALSE;
	time_t	t_now=0;
	struct tm* ptm=NULL;
	char	szMailPath[_MAX_PATH],szAttachPath[_MAX_PATH],szBuf[1024];
	CString objMsg;

	//コントロール－＞メンバ変数
	UpdateData(TRUE);

	//現在時刻取得
	t_now = time(NULL);
	ptm = localtime(&t_now);

	for(nCnt=0; nCnt<m_nMailCount; nCnt++)
	{
		//選択状態チェック
		memset(&lvi,0,sizeof(LV_ITEM));
		lvi.mask = LVIF_STATE;
		lvi.iItem = nCnt;
		lvi.iSubItem = 0;
		lvi.stateMask = 0xFFFF;
		m_objListMail.GetItem(&lvi);
		if(lvi.state & LVIS_SELECTED)
		{
			//メール保存先パス編集
			nSelectCnt++;
			sprintf(szMailPath,"%s\\Mail%04d%02d%02d_%02d%02d%02d_%d.txt",
				(LPCTSTR)m_pApp->m_strRecvMailPath,
				ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,
				ptm->tm_hour,ptm->tm_min,ptm->tm_sec,nSelectCnt);

			//添付ファイル保存先パス編集
			sprintf(szAttachPath,"%s\\%04d%02d%02d_%02d%02d%02d_%d",
				(LPCTSTR)m_pApp->m_strRecvMailAttachPath,
				ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,
				ptm->tm_hour,ptm->tm_min,ptm->tm_sec,nSelectCnt);

			//１メール受信＆オープン
			bRet = m_pPop->RecvOneMail(nCnt+1,szMailPath,szAttachPath);
			if(bRet)
			{
				nRecvMailCnt++;
				if( (_access( szMailPath, 0 )) != -1 )
					m_pApp->OpenDocumentFile(szMailPath);
			}
			else
			{
				objMsg.LoadString(IDS_MAILRECVERR);
				sprintf(szBuf,(LPCTSTR)objMsg,nCnt+1);
				m_pApp->MyMessageBox(szBuf,(MB_OK|MB_ICONSTOP),0);
			}
		}
	}

	if(nSelectCnt == 0)
	{
		objMsg.LoadString(IDS_POP_SELECTLIST);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONEXCLAMATION),0);
	}
	else
	{
		//メールリスト再作成
		DispMailList();

		//受信終了通知
		if(nRecvMailCnt>0)
		{
			objMsg.LoadString(IDS_MAILRECVEND);
			sprintf(szBuf,(LPCTSTR)objMsg,nRecvMailCnt);
			m_pApp->MyMessageBox(szBuf,(MB_OK|MB_ICONINFORMATION),0);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「メール削除」ボタン押下時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMailListDlg::OnDelbtn() 
{
	int 	nCnt=0,nSelectCnt=0;
	LV_ITEM lvi;
	BOOL	bRet=FALSE;
	CString objMsg;

	//コントロール－＞メンバ変数
	UpdateData(TRUE);

	for(nCnt=0; nCnt<m_nMailCount; nCnt++)
	{
		//選択状態チェック
		memset(&lvi,0,sizeof(LV_ITEM));
		lvi.mask = LVIF_STATE;
		lvi.iItem = nCnt;
		lvi.iSubItem = 0;
		lvi.stateMask = 0xFFFF;
		m_objListMail.GetItem(&lvi);
		if(lvi.state & LVIS_SELECTED)
		{
			nSelectCnt++;
			//１メール削除
			bRet = m_pPop->DelOneMail(nCnt+1);
		}
	}

	if(nSelectCnt == 0)
	{
		objMsg.LoadString(IDS_POP_SELECTLIST);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONEXCLAMATION),0);
	}
	else
	{
		//メールリスト再作成
		DispMailList();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：表示更新ボタン押下時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMailListDlg::OnRefreshbtn() 
{
	CWaitCursor	objWaitCursor;

	char			szSection[64];
	int				nAccountNo = m_pApp->GetProfileInt("Mail","CurrentAccountNo",0);
	if(nAccountNo < 0 || nAccountNo > 8)
		nAccountNo = 0;
	sprintf(szSection,"Mail_Account%d",nAccountNo);

	int nAuthorizationType = m_pApp->GetProfileInt(szSection,"nAuthorizationType",POP3_AUTHENTICATE_STANDARD);

	m_pPop->Disconnect();
	BOOL bRet = m_pPop->Connect( m_objPopServerStr,
								 m_objPopUserStr,
								 m_objPopPasswdStr,
								 nAuthorizationType );
	if(bRet)
	{
		int nMailCount = m_pPop->GetMailCount();
		if(nMailCount >= 0)
			m_nMailCount = nMailCount;

		//メールリスト再作成
		DispMailList();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＯＫボタン押下時処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMailListDlg::OnOK() 
{
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
// CMailListDlg プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：メールリスト作成														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMailListDlg::DispMailList()
{
	int	nCnt=0,nIndex=0,nMailCount=0,nSize=0;
	CString objStr, objMsg;
	char	szNumber[32],szSubject[1024],szFrom[1024],szDate[1024];
	BOOL	bRet=FALSE,bRet2=FALSE,bIsMultiPart=FALSE;
	LV_ITEM lvi;

	//リストをまず全部クリア
	m_objListMail.DeleteAllItems();

	//メールリスト作成
	for(nIndex=0,nCnt=0; nCnt<m_nMailCount; nCnt++)
	{
		//件名、差出人、日付等情報取得
		memset(szSubject,0,sizeof(szSubject));
		memset(szFrom,0,sizeof(szFrom));
		memset(szDate,0,sizeof(szDate));
		bRet  = m_pPop->GetMailInfo(nCnt+1,szSubject,szFrom,szDate,&bIsMultiPart);
		//サイズ情報取得
		bRet2 = m_pPop->GetMailSize(nCnt+1,&nSize);
		if(bRet || bRet2)
		{
			sprintf(szNumber,"%02d",nIndex+1);

			memset(&lvi,0,sizeof(LV_ITEM));
			lvi.mask = LVIF_TEXT | LVIF_IMAGE;
			lvi.iItem = nIndex;
			lvi.iSubItem = 0;
			lvi.pszText = szNumber;
			lvi.iImage = (bIsMultiPart ? 1 : 0);
			m_objListMail.InsertItem(&lvi);

			m_objListMail.SetItemText(nIndex,1,szSubject);
			m_objListMail.SetItemText(nIndex,2,szFrom);
			m_objListMail.SetItemText(nIndex,3,szDate);
			sprintf(szNumber,"%7d",nSize);
			m_objListMail.SetItemText(nIndex,4,szNumber);

			nIndex++;
		}
	}

	//メール総数
	nMailCount = m_pPop->GetMailCount();
	if(nMailCount >= 0)
		m_nMailCount = nMailCount;
	objMsg.LoadString(IDS_MAILCOUNT);
	objStr.Format(objMsg, m_nMailCount );
	SetDlgItemText(IDC_MAILCOUNT, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウ位置ロード処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMailListDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CMailListDlg","WindowPos");
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
void CMailListDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CMailListDlg","WindowPos",strBuffer);
}


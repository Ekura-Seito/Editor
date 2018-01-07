// MailSendDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <afxtempl.h>
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "MyComboBoxEx.h"
#include "AutoResizer.h"
#include "MailSendDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailSendDlg ダイアログ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMailSendDlg::CMailSendDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMailSendDlg::IDD, pParent)
{
	m_pApp = (CEditorApp*)AfxGetApp();

	//{{AFX_DATA_INIT(CMailSendDlg)
	m_objSubject = _T("");
	m_objCc = _T("");
	m_objBcc = _T("");
	m_objReplyTo = _T("");
	m_objReference = _T("");
	m_bIsReqMsgDispsition = FALSE;
	//}}AFX_DATA_INIT

	for(int nCnt=0; nCnt<20; nCnt++)
		m_objPastTo[nCnt] = _T("");

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
void CMailSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailSendDlg)
	DDX_Control(pDX, IDC_COMBO_TO, m_objComboTo);
	DDX_CBString(pDX, IDC_COMBO_TO, m_objTo);
	DDX_Text(pDX, IDC_EDIT_SUBJECT, m_objSubject);
	DDX_Text(pDX, IDC_EDIT_CC, m_objCc);
	DDX_Text(pDX, IDC_EDIT_BCC, m_objBcc);
	DDX_Text(pDX, IDC_EDIT_REPLYTO, m_objReplyTo);
	DDX_Text(pDX, IDC_EDIT_REFERENCE, m_objReference);
	DDX_Control(pDX, IDC_LIST_ATTACHFILE, m_objListAttachFiles);
	DDX_Check(pDX, IDC_CHECK_DISPOSITION, m_bIsReqMsgDispsition);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMailSendDlg, CDialog)
	//{{AFX_MSG_MAP(CMailSendDlg)
	ON_BN_CLICKED(IDC_FILEATTACH, OnFileattach)
	ON_BN_CLICKED(IDC_FILEDETACH, OnFiledetach)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailSendDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMailSendDlg::OnInitDialog() 
{
	CString objMsg;
	CEditorApp* pApp = (CEditorApp*)AfxGetApp();
	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	char			szSection[64];
	int				nAccountNo = m_pApp->GetProfileInt("Mail","CurrentAccountNo",0);
	if(nAccountNo < 0 || nAccountNo > 8)
		nAccountNo = 0;
	sprintf(szSection,"Mail_Account%d",nAccountNo);

	//フォント設定
	m_objComboTo.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	GetDlgItem(IDC_EDIT_SUBJECT)->SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	GetDlgItem(IDC_EDIT_CC)->SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	GetDlgItem(IDC_EDIT_BCC)->SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	GetDlgItem(IDC_EDIT_REPLYTO)->SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	GetDlgItem(IDC_EDIT_REFERENCE)->SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//宛先コンボボックスへ過去データ設定
	CString	objStr;
	char	szKey[32];
	int nIndex=0;
	int nCnt=0,nCnt2=0;
	BOOL bFind=FALSE;
	if(!m_objTo.IsEmpty())
	{
		m_objPastTo[nIndex] = m_objTo;
		m_objComboTo.AddString(m_objTo);
		nIndex++;
	}
	for(nCnt=0; nCnt<20; nCnt++)
	{
		if(nIndex >= 20)
			break;
		sprintf(szKey,"ToStr%d",nCnt);
		objStr = pApp->GetProfileString(szSection,szKey,"");
		if( !objStr.IsEmpty() )
		{
			for(bFind=FALSE,nCnt2=0; nCnt2<nIndex; nCnt2++)
			{
				if(m_objPastTo[nCnt2].Compare(objStr) == 0)
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind == FALSE)
			{
				m_objPastTo[nIndex] = objStr;
				m_objComboTo.AddString(objStr);
				nIndex++;
			}
		}
	}
	m_objComboTo.SetCurSel(0);

	//添付ファイルリストにシステムイメージリストを設定
	HIMAGELIST hSysImageList;
	SHFILEINFO shfi;
	CImageList ImageList;
	hSysImageList = (HIMAGELIST)SHGetFileInfo( "", 
												0, 
												&shfi,
												sizeof(SHFILEINFO), 
												SHGFI_SYSICONINDEX | SHGFI_LARGEICON);
	ImageList.Attach(hSysImageList);
	m_objListAttachFiles.SetImageList(&ImageList, LVSIL_NORMAL);
	ImageList.Detach();

	//カラム設定
	TCHAR title[][20] = { _T("File Name") };
	int width[] = {200};
	LV_COLUMN listcol;
	listcol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	for (int i=0; i<sizeof(title)/(sizeof(TCHAR)*20); i++){
		if (i != 1)
			listcol.fmt = LVCFMT_LEFT;
		else
			listcol.fmt = LVCFMT_RIGHT;
		listcol.pszText = title[i];
		listcol.cx = width[i];
		m_objListAttachFiles.InsertColumn(i, &listcol);
	}

	//アイコンビュー
	m_objListAttachFiles.ModifyStyle( LVS_TYPEMASK, LVS_ICON|LVS_SHAREIMAGELISTS);

	//ツールヒント
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	objMsg.LoadString(IDS_SMTP_COMMASEPARATE);
//	m_tooltip.AddTool(GetDlgItem(IDC_COMBO_TO), objMsg);
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_CC), objMsg);
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_BCC), objMsg);

	//オートリサイズ
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_COMBO_TO,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_EDIT_SUBJECT,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_EDIT_CC,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_EDIT_BCC,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_EDIT_REPLYTO,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_EDIT_REFERENCE,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_LIST_ATTACHFILE,CP_RESIZE_VERTICAL|CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDOK,CP_MOVE_VERTICAL|CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDCANCEL,CP_MOVE_VERTICAL|CP_MOVE_HORIZONTAL);

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
void CMailSendDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	//オートリサイズ起動
	m_objAutoResizer.MoveControls();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：メッセージ処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMailSendDlg::PreTranslateMessage(MSG* pMsg)
{
	m_tooltip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル添付」ボタン押下時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMailSendDlg::OnFileattach() 
{
	LV_ITEM			listitem;
	SHFILEINFO 		shfi;
	CString 		objFilePath;
	DWORD dwFlags = ( OFN_ALLOWMULTISELECT |
					  OFN_HIDEREADONLY     | 
					  OFN_OVERWRITEPROMPT  |
					  OFN_LONGNAMES        |
					  OFN_FILEMUSTEXIST );
	CString			objMsg;

	//ファイルオープンコモンダイアログ表示
	CFileDialog	objFileDialog(	TRUE,				//OPEN
								NULL,				//デフォルトファイル拡張子
								NULL,				//デフォルトファイル名
								dwFlags,			//オープンフラグ
								NULL,				//フィルタ
								NULL );				//親ウインドウハンドル
	char	szFileNameBuf[4096];
	memset(szFileNameBuf,0,sizeof(szFileNameBuf));
	objMsg.LoadString(IDS_SELECTATTACHFILE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	objFileDialog.m_ofn.lpstrFile = szFileNameBuf;
	objFileDialog.m_ofn.nMaxFile = sizeof(szFileNameBuf);
	if( objFileDialog.DoModal() != IDOK )
		return;
	POSITION	pos = objFileDialog.GetStartPosition();
	for( ; pos != 0; )
	{
		objFilePath = objFileDialog.GetNextPathName(pos);

		m_objAttachFiles.Add(objFilePath);

		//添付ファイル情報の取得
		::SHGetFileInfo( objFilePath, 
						 0, 
						 &shfi, 
						 sizeof(SHFILEINFO),
						 SHGFI_SYSICONINDEX | SHGFI_LARGEICON | SHGFI_DISPLAYNAME | SHGFI_TYPENAME);
		//リストへ追加
		listitem.mask = LVIF_TEXT | LVIF_IMAGE;
		listitem.pszText = shfi.szDisplayName;
		listitem.iImage = shfi.iIcon;
		listitem.iItem = m_objListAttachFiles.GetItemCount();
		listitem.iSubItem = 0;
		m_objListAttachFiles.InsertItem(&listitem);	//アイテムの追加
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「添付解除」ボタン押下時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMailSendDlg::OnFiledetach() 
{
	//コントロール－＞メンバ変数
	UpdateData(TRUE);

	int nCnt=0;
	LV_ITEM lvi;
	char szBuf[_MAX_FNAME];
	CString objStr;

	for(nCnt=0; nCnt<m_objListAttachFiles.GetItemCount(); )
	{
		memset(szBuf,0,sizeof(szBuf));
		memset(&lvi,0,sizeof(LV_ITEM));

		lvi.mask = LVIF_STATE | LVIF_TEXT;
		lvi.iItem = nCnt;
		lvi.pszText = szBuf;
		lvi.iSubItem = 0;
		lvi.stateMask = 0xFFFF;         // get all state flags
		m_objListAttachFiles.GetItem(&lvi);
		if(lvi.state & LVIS_SELECTED)
		{
			objStr = m_objAttachFiles.GetAt(nCnt);
			m_objAttachFiles.RemoveAt(nCnt);
			m_objListAttachFiles.DeleteItem(nCnt);
		}
		else
		{
			nCnt++;
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ＯＫ」ボタン押下時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMailSendDlg::OnOK() 
{
	int	nPos=0;
	CString objStr=_T("");
	CEditorApp* pApp = (CEditorApp*)AfxGetApp();
	CString objMsg;

	char			szSection[64];
	int				nAccountNo = m_pApp->GetProfileInt("Mail","CurrentAccountNo",0);
	if(nAccountNo < 0 || nAccountNo > 8)
		nAccountNo = 0;
	sprintf(szSection,"Mail_Account%d",nAccountNo);

	//コントロール－＞メンバ変数
	UpdateData(TRUE);

	//宛先ブランクチェック
	if(m_objTo.IsEmpty())
	{
		objMsg.LoadString(IDS_SMTP_NOTO);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//複数の宛先切り分け
	for(;;)
	{
		nPos = m_objTo.Find(',');
		if(nPos == -1)
		{
			m_objTo.TrimLeft();
			m_objTo.TrimRight();
			if(!m_objTo.IsEmpty())
				m_objToArray.Add(m_objTo);
			break;
		}
		else
		{
			objStr = m_objTo.Left(nPos);
			objStr.TrimLeft();
			objStr.TrimRight();
			m_objToArray.Add(objStr);
			m_objTo = m_objTo.Right( m_objTo.GetLength() - nPos - 1 );
		}
	}

	//複数のＣＣ切り分け
	for(;;)
	{
		nPos = m_objCc.Find(',');
		if(nPos == -1)
		{
			m_objCc.TrimLeft();
			m_objCc.TrimRight();
			if(!m_objCc.IsEmpty())
				m_objCcArray.Add(m_objCc);
			break;
		}
		else
		{
			objStr = m_objCc.Left(nPos);
			objStr.TrimLeft();
			objStr.TrimRight();
			m_objCcArray.Add(objStr);
			m_objCc = m_objCc.Right( m_objCc.GetLength() - nPos - 1 );
		}
	}

	//複数のＢＣＣ切り分け
	for(;;)
	{
		nPos = m_objBcc.Find(',');
		if(nPos == -1)
		{
			m_objBcc.TrimLeft();
			m_objBcc.TrimRight();
			if(!m_objBcc.IsEmpty())
				m_objBccArray.Add(m_objBcc);
			break;
		}
		else
		{
			objStr = m_objBcc.Left(nPos);
			objStr.TrimLeft();
			objStr.TrimRight();
			m_objBccArray.Add(objStr);
			m_objBcc = m_objBcc.Right( m_objBcc.GetLength() - nPos - 1 );
		}
	}

	//宛先履歴データ更新
	char	szKey[32];
	pApp->WriteProfileString(szSection,"ToStr0",m_objTo);
	int nIndex=0;
	int nCnt=0,nCnt2=0;
	BOOL bFind=FALSE;
	for(nIndex=0; nIndex<20; nIndex++)
	{
		sprintf(szKey,"ToStr%d",nCnt+1);
		if( !m_objPastTo[nIndex].IsEmpty() &&
			m_objTo != m_objPastTo[nIndex]
		)
		{
			for(bFind=FALSE,nCnt2=0; nCnt2<nIndex; nCnt2++)
			{
				if(m_objPastTo[nCnt2].Compare(m_objPastTo[nIndex]) == 0)
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind==FALSE)
			{
				pApp->WriteProfileString(szSection,szKey,m_objPastTo[nIndex]);
				nCnt++;
			}
		}
	}

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
// CMailSendDlg プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウ位置ロード処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMailSendDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CMailSendDlg","WindowPos");
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
void CMailSendDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CMailSendDlg","WindowPos",strBuffer);
}


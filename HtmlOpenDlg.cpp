// HtmlOpenDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "MyComboBoxEx.h"
#include "AutoResizer.h"
#include "HtmlOpenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHtmlOpenDlg クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CHtmlOpenDlg::CHtmlOpenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHtmlOpenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHtmlOpenDlg)
	m_bIsUseProxy = FALSE;
	m_objRequestPath = _T("");
	//}}AFX_DATA_INIT

	m_objProxyIPAddress = _T("");
	m_pApp = (CEditorApp*)AfxGetApp();
	m_objSite = _T("");
	m_objHost = _T("");
	m_objPath = _T("");
	m_objFileName = _T("");
	m_objFileExt = _T("");
	for(int nCnt=0; nCnt<20; nCnt++)
		m_objPastRequestPath[nCnt] = _T("");

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
void CHtmlOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHtmlOpenDlg)
	DDX_Control(pDX, IDC_COMBO_REQUESTPATH, m_objComboRequestPath);
	DDX_CBString(pDX, IDC_COMBO_REQUESTPATH, m_objRequestPath);
	DDX_Check(pDX, IDC_CHECK_USEPROXY, m_bIsUseProxy);
	DDX_Control(pDX, IDC_EDIT_PROXYIPADDRESS, m_objEditProxyIPAddress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHtmlOpenDlg, CDialog)
	//{{AFX_MSG_MAP(CHtmlOpenDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_USEPROXY, OnCheckUseproxy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmlOpenDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CHtmlOpenDlg::OnInitDialog() 
{
	CString objMsg;
	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//デフォルトデータ設定
	m_objEditProxyIPAddress.SetWindowText(m_objProxyIPAddress);
	m_objEditProxyIPAddress.UpdateData(TRUE);

	//ＨＴＭＬパスコンボボックスへ過去データ設定
	CString	objStr;
	char	szKey[32];
	int nIndex=0;
	int nCnt=0,nCnt2=0;
	BOOL bFind=FALSE;
	if(!m_objRequestPath.IsEmpty())
	{
		m_objPastRequestPath[nIndex] = m_objRequestPath;
		m_objComboRequestPath.AddString(m_objRequestPath);
		nIndex++;
	}
	for(nCnt=0; nCnt<20; nCnt++)
	{
		if(nIndex >= 20)
			break;
		sprintf(szKey,"RequestPath%d",nCnt);
		objStr = m_pApp->GetProfileString("CHtmlOpenDlg",szKey,"");
		if( !objStr.IsEmpty() )
		{
			for(bFind=FALSE,nCnt2=0; nCnt2<nIndex; nCnt2++)
			{
				if(m_objPastRequestPath[nCnt2].Compare(objStr) == 0)
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind == FALSE)
			{
				m_objPastRequestPath[nIndex] = objStr;
				m_objComboRequestPath.AddString(objStr);
				nIndex++;
			}
		}
	}
	m_objComboRequestPath.SetCurSel(0);

	//オートリサイズ
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_COMBO_REQUESTPATH,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_EDIT_PROXYIPADDRESS,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDOK,CP_MOVE_HORIZONTAL|CP_MOVE_VERTICAL);
	m_objAutoResizer.AddControl(IDCANCEL,CP_MOVE_HORIZONTAL|CP_MOVE_VERTICAL);

	//ウインドウ位置復元
	WINDOWPLACEMENT		wp;
	if(LoadWindowPlacement(&wp))
		SetWindowPlacement(&wp);

	//オートリサイズ起動
	m_objAutoResizer.MoveControls();

	//プロキシサーバを使う場合にはプロキシサーバアドレス欄を有効化
	if(m_bIsUseProxy)
		m_objEditProxyIPAddress.EnableWindow(TRUE);
	else
		m_objEditProxyIPAddress.EnableWindow(FALSE);

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
void CHtmlOpenDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	//オートリサイズ起動
	m_objAutoResizer.MoveControls();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「プロキシサーバを使う」チェック時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CHtmlOpenDlg::OnCheckUseproxy() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	if(m_bIsUseProxy)
		m_objEditProxyIPAddress.EnableWindow(TRUE);
	else
		m_objEditProxyIPAddress.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＯＫボタン押下時処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CHtmlOpenDlg::OnOK() 
{
	int	nPos=0;
	long lCnt=0;
	CString objMsg, objHostPath;

	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	//データ取得
	m_objEditProxyIPAddress.GetWindowText(m_objProxyIPAddress);

	//ブランクチェック
	if(m_objRequestPath.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_REQUESTPATH);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if(m_bIsUseProxy && m_objProxyIPAddress.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_PROXYIPADDRESS);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//////////////////////////////
	//	フォーマットチェック	//
	//////////////////////////////

	//要求パスはhttp://www.asahi-net.or.jp/~ga5s-ekr/index.htmのフルパス形式
	if(m_objRequestPath.Left(7) != "http://")
	{
		objMsg.LoadString(IDS_HTTP_INVALIDFORMAT);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	for(lCnt=0; lCnt<m_objRequestPath.GetLength(); lCnt++)
	{
		if(IsDBCSLeadByteEx(0,m_objRequestPath.GetAt(lCnt)))//２ﾊﾞｲﾄ文字の第一ﾊﾞｲﾄ
		{
			objMsg.LoadString(IDS_HTTP_ASCIONLY);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//////////////////////////////////////////
	//	接続先ホスト　＆　要求パス取りだし	//
	//////////////////////////////////////////

	//要求するパスからホストパスを切り出し
	//例：objHostPath <-- http://www.asahi-net.or.jp/~ga5s-ekr/index.htm
	objHostPath = m_objRequestPath.Right( m_objRequestPath.GetLength() - strlen("http://") );

	//ホストとパスの区切りを調べる
	//例：m_objHost <-- www.asahi-net.or.jp,   objPath <-- /~ga5s-ekr/index.htm
	nPos = objHostPath.Find('/');
	if(nPos == -1)
	{
		m_objHost = objHostPath;
		m_objPath = "/";
	}
	else
	{
		m_objHost = objHostPath.Left(nPos);
		m_objPath = objHostPath.Right( objHostPath.GetLength() - nPos );
	}

	//ホスト部にポート番号が含まれているか調べる
	nPos = m_objHost.Find(':');
	if(nPos != -1)
		m_objHost.SetAt(nPos,0);

	if(m_objHost.IsEmpty() || m_objPath.IsEmpty())
	{
		objMsg.LoadString(IDS_HTTP_INVALIDFORMAT);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	m_objSite = m_objHost;

	if(m_bIsUseProxy)
	{
		m_objHost = m_objProxyIPAddress;
		m_objPath = m_objRequestPath;
	}


	//要求パスからファイル名を作成
	nPos = m_objPath.ReverseFind('/');
	if(nPos == m_objPath.GetLength()-1)
		m_objFileName = _T("index.html");
	else
		m_objFileName = m_objPath.Right( m_objPath.GetLength() - nPos - 1 );

	//ファイル名から拡張子を取得
	nPos = m_objFileName.ReverseFind('.');
	if(nPos != -1)
		m_objFileExt = m_objFileName.Right( m_objFileName.GetLength() - nPos - 1 );
	else
		m_objFileExt = _T("html");

	//履歴データ更新
	char	szKey[32];
	m_pApp->WriteProfileString("CHtmlOpenDlg","RequestPath0",m_objRequestPath);
	int nIndex=0;
	int nCnt=0,nCnt2=0;
	BOOL bFind=FALSE;
	for(nIndex=0; nIndex<20; nIndex++)
	{
		sprintf(szKey,"RequestPath%d",nCnt+1);
		if( !m_objPastRequestPath[nIndex].IsEmpty() &&
			m_objRequestPath != m_objPastRequestPath[nIndex]
		)
		{
			for(bFind=FALSE,nCnt2=0; nCnt2<nIndex; nCnt2++)
			{
				if(m_objPastRequestPath[nCnt2].Compare(m_objPastRequestPath[nIndex]) == 0)
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind==FALSE)
			{
				m_pApp->WriteProfileString("CHtmlOpenDlg",szKey,m_objPastRequestPath[nIndex]);
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
// CHtmlOpenDlg プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウ位置ロード処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CHtmlOpenDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CHtmlOpenDlg","WindowPos");
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
void CHtmlOpenDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CHtmlOpenDlg","WindowPos",strBuffer);
}


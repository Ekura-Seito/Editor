// GrepDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "Editor.h"
#include "MyComboBoxEx.h"
#include "AutoResizer.h"
#include "GrepDlg.h"
#include "DirDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrepDlg クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CGrepDlg::CGrepDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGrepDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGrepDlg)
	m_bIsMatchCaseGrep = FALSE;
	m_bIsMatchWholeWordGrep = FALSE;
	m_bIsSearchSubFolderGrep = FALSE;
	m_bIsUseRegularExpression = FALSE;
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_objFolder = _T("");					// ＧＲＥＰ実行フォルダ
	m_objGrepString = _T("");				// 検索文字列
	m_objFileKind = _T("");					// 検索対象のファイル種別

	for(int nCnt=0; nCnt<20; nCnt++)
		m_objPastGrepString[nCnt] = _T("");

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
void CGrepDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGrepDlg)
	DDX_Control(pDX, IDC_COMBO_GREPSTR, m_objComboGrepString);
	DDX_CBString(pDX, IDC_COMBO_GREPSTR, m_objGrepString);
	DDX_Control(pDX, IDC_EDIT_FOLDER, m_objEditFolder);
	DDX_Control(pDX, IDC_EDIT_FILEKIND, m_objEditFileKind);
	DDX_Check(pDX, IDC_CHECK_MATCHCASE, m_bIsMatchCaseGrep);
	DDX_Check(pDX, IDC_CHECK_MATCHWHOLEWORD, m_bIsMatchWholeWordGrep);
	DDX_Check(pDX, IDC_CHECK_SUBFOLDER, m_bIsSearchSubFolderGrep);
	DDX_Check(pDX, IDC_CHECK_REGEX, m_bIsUseRegularExpression);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGrepDlg, CDialog)
	//{{AFX_MSG_MAP(CGrepDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_REGEX, OnCheckRegex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrepDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CGrepDlg::OnInitDialog() 
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

	//文字数制限
	m_objComboGrepString.LimitText(255);

	//デフォルトデータ設定
	m_objEditFolder.SetWindowText(m_objFolder);
	m_objEditFolder.UpdateData(TRUE);
	m_objEditFileKind.SetWindowText(m_objFileKind);
	m_objEditFileKind.UpdateData(TRUE);

	//検索文字列コンボボックスへ過去検索データ設定
	CString	objStr;
	char	szKey[32];
	int nIndex=0;
	int nCnt=0,nCnt2=0;
	BOOL bFind=FALSE;
	if(!m_objGrepString.IsEmpty())
	{
		m_objPastGrepString[nIndex] = m_objGrepString;
		m_objComboGrepString.AddString(m_objGrepString);
		nIndex++;
	}
	for(nCnt=0; nCnt<20; nCnt++)
	{
		if(nIndex >= 20)
			break;
		sprintf(szKey,"GrepStr%d",nCnt);
		objStr = m_pApp->GetProfileString("CGrepDlg",szKey,"");
		if( !objStr.IsEmpty() )
		{
			for(bFind=FALSE,nCnt2=0; nCnt2<nIndex; nCnt2++)
			{
				if(m_objPastGrepString[nCnt2].Compare(objStr) == 0)
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind == FALSE)
			{
				m_objPastGrepString[nIndex] = objStr;
				m_objComboGrepString.AddString(objStr);
				nIndex++;
			}
		}
	}
	m_objComboGrepString.SetCurSel(0);

	//ツールヒント
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	objMsg.LoadString(IDS_BROWSE);
	m_tooltip.AddTool(GetDlgItem(IDC_BROWSE), objMsg);
	objMsg.LoadString(IDS_FILEKIND);
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_FILEKIND), objMsg);

	//オートリサイズ
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_COMBO_GREPSTR,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_EDIT_FILEKIND,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_EDIT_FOLDER,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDOK,CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDCANCEL,CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_BROWSE,CP_MOVE_HORIZONTAL);

	//ウインドウ位置復元
	WINDOWPLACEMENT		wp;
	if(LoadWindowPlacement(&wp))
		SetWindowPlacement(&wp);

	//オートリサイズ起動
	m_objAutoResizer.MoveControls();

	//regex_dll.dll検索
	if(m_pApp->m_hRegexDllInstance == NULL)
	{
		CWaitCursor	objWaitCursor;
		m_pApp->m_hRegexDllInstance = ::LoadLibrary("regex_dll.dll");
		if(m_pApp->m_hRegexDllInstance == NULL)//regex_dll.dllがない
		{
			m_pApp->m_lpfnRegFunc1 = NULL;
			m_pApp->m_lpfnRegFunc2 = NULL;
			m_pApp->m_lpfnRegFunc3 = NULL;
			m_pApp->m_lpfnRegFunc4 = NULL;
			m_pApp->m_lpfnRegFunc5 = NULL;
			GetDlgItem(IDC_CHECK_REGEX)->EnableWindow(FALSE);
			m_bIsUseRegularExpression = FALSE;
		}
	}
	if(m_pApp->m_hRegexDllInstance)
	{
		if( m_pApp->m_lpfnRegFunc1 != NULL && m_pApp->m_lpfnRegFunc2 != NULL &&
			m_pApp->m_lpfnRegFunc3 != NULL && m_pApp->m_lpfnRegFunc4 != NULL &&
			m_pApp->m_lpfnRegFunc5 != NULL )
		{
			GetDlgItem(IDC_CHECK_REGEX)->EnableWindow(TRUE);
		}
		else
		{
			m_pApp->m_lpfnRegFunc1 = (LPFNREGFUNC1)::GetProcAddress(m_pApp->m_hRegexDllInstance,"myRegcomp");
			m_pApp->m_lpfnRegFunc2 = (LPFNREGFUNC2)::GetProcAddress(m_pApp->m_hRegexDllInstance,"myRegexec");
			m_pApp->m_lpfnRegFunc3 = (LPFNREGFUNC3)::GetProcAddress(m_pApp->m_hRegexDllInstance,"myRegerror");
			m_pApp->m_lpfnRegFunc4 = (LPFNREGFUNC4)::GetProcAddress(m_pApp->m_hRegexDllInstance,"myRegfree");
			m_pApp->m_lpfnRegFunc5 = (LPFNREGFUNC5)::GetProcAddress(m_pApp->m_hRegexDllInstance,"myIsNeedRegcomp");
			if( m_pApp->m_lpfnRegFunc1 == NULL || m_pApp->m_lpfnRegFunc2 == NULL ||
				m_pApp->m_lpfnRegFunc3 == NULL || m_pApp->m_lpfnRegFunc4 == NULL || //ＤＬＬ関数アドレス取得できない
				m_pApp->m_lpfnRegFunc5 == NULL )
			{
				::FreeLibrary(m_pApp->m_hRegexDllInstance);
				m_pApp->m_hRegexDllInstance = NULL;
				m_pApp->m_lpfnRegFunc1 = NULL;
				m_pApp->m_lpfnRegFunc2 = NULL;
				m_pApp->m_lpfnRegFunc3 = NULL;
				m_pApp->m_lpfnRegFunc4 = NULL;
				m_pApp->m_lpfnRegFunc5 = NULL;
				GetDlgItem(IDC_CHECK_REGEX)->EnableWindow(FALSE);
				m_bIsUseRegularExpression = FALSE;
			}
			else
				GetDlgItem(IDC_CHECK_REGEX)->EnableWindow(TRUE);
		}
	}

	//正規表現を使う場合には単語単位オプション無効
	if(m_bIsUseRegularExpression)
		GetDlgItem(IDC_CHECK_MATCHWHOLEWORD)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_CHECK_MATCHWHOLEWORD)->EnableWindow(TRUE);

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
void CGrepDlg::OnSize(UINT nType, int cx, int cy) 
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
BOOL CGrepDlg::PreTranslateMessage(MSG* pMsg)
{
	m_tooltip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「正規表現を使う」チェック時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CGrepDlg::OnCheckRegex() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	if(m_bIsUseRegularExpression)
		GetDlgItem(IDC_CHECK_MATCHWHOLEWORD)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_CHECK_MATCHWHOLEWORD)->EnableWindow(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：フォルダブラウズボタン押下時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CGrepDlg::OnBrowse() 
{
	CDirDialog		objDirDialog;
	objDirDialog.m_strTitle = "  ";
	if( objDirDialog.DoBrowse() != FALSE )
	{
		m_objEditFolder.SetWindowText(objDirDialog.m_strPath);
		m_objEditFolder.UpdateData(TRUE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＯＫボタン押下時処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CGrepDlg::OnOK() 
{
	int cflags=0, ret=0;
	char szMsg[256];
	CString objMsg,objBuf;
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	//データ取得
	m_objEditFolder.GetWindowText(m_objFolder);
	m_objEditFileKind.GetWindowText(m_objFileKind);

	//ブランクチェック
	if(m_objFolder.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_FOLDER);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if(m_objFileKind.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_FILEKIND);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if(m_objGrepString.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_FINDSTRING);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//正規表現コンパイル
	if(m_bIsUseRegularExpression)
	{
		if( m_pApp->m_lpfnRegFunc1 == NULL || m_pApp->m_lpfnRegFunc2 == NULL ||
			m_pApp->m_lpfnRegFunc3 == NULL || m_pApp->m_lpfnRegFunc4 == NULL || //ＤＬＬ関数アドレス取得できない
			m_pApp->m_lpfnRegFunc5 == NULL )
		{
			m_bIsUseRegularExpression=FALSE;
		}
	}
	if(m_bIsUseRegularExpression)
	{
		m_bIsMatchWholeWordGrep=FALSE;

		cflags = REG_EXTENDED;
		if(m_bIsMatchCaseGrep==FALSE)//大小文字を区別しない
			cflags |= REG_ICASE;
		//regcomp
		ret = m_pApp->m_lpfnRegFunc1((LPCTSTR)m_objGrepString,cflags);
		if(ret != 0) //error
		{
			//regerror
			memset(szMsg,0,sizeof(szMsg));
			m_pApp->m_lpfnRegFunc3(ret,szMsg,sizeof(szMsg));
			objMsg.LoadString(IDS_REGEX_COMPERR);
			objBuf.Format("%s\r\n%s",objMsg,szMsg);
			m_pApp->MyMessageBox(objBuf,(MB_OK|MB_ICONSTOP),0);
			//regfree
			m_pApp->m_lpfnRegFunc4();
			return;
		}
	}

	//履歴データ更新
	char	szKey[32];
	m_pApp->WriteProfileString("CGrepDlg","GrepStr0",m_objGrepString);
	int nIndex=0;
	int nCnt=0,nCnt2=0;
	BOOL bFind=FALSE;
	for(nIndex=0; nIndex<20; nIndex++)
	{
		sprintf(szKey,"GrepStr%d",nCnt+1);
		if( !m_objPastGrepString[nIndex].IsEmpty() &&
			m_objGrepString != m_objPastGrepString[nIndex]
		)
		{
			for(bFind=FALSE,nCnt2=0; nCnt2<nIndex; nCnt2++)
			{
				if(m_objPastGrepString[nCnt2].Compare(m_objPastGrepString[nIndex]) == 0)
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind==FALSE)
			{
				m_pApp->WriteProfileString("CGrepDlg",szKey,m_objPastGrepString[nIndex]);
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
// CGrepDlg プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウ位置ロード処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CGrepDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CGrepDlg","WindowPos");
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
void CGrepDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CGrepDlg","WindowPos",strBuffer);
}



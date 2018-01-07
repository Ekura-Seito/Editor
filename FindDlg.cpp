// FindDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <afxtempl.h>			//for CTypedPtrList
#include <afxole.h>				//for COleDataSource, COleDropTarget
#pragma warning(disable:4786)
#include "editor.h"
#include "Editor_Def.h"			//共通ＤＥＦＩＮＥ
#include "StatusBarDisp.h"		//ステータスバー表示情報構造体
#include "MyComboBoxEx.h"
#include "Line.h"
#include "BitmapClient.h"
#include "Operation.h"
#include "OperationManager.h"
#include "MainFrm.h"
#include "EditorView.h"
#include "AutoResizer.h"
#include "FindDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFindDlg ダイアログ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFindDlg::CFindDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindDlg::IDD, pParent)
{
	m_pApp = (CEditorApp*)AfxGetApp();

	//{{AFX_DATA_INIT(CFindDlg)
	m_objFindString = _T("");
	m_bIsMatchWholeWordFind = FALSE;
	m_bIsMatchCaseFind = FALSE;
	m_bIsSearchDownFind = -1;
	m_bIsUseRegularExpression = FALSE;
	//}}AFX_DATA_INIT

	m_pobjMainFrame = NULL;
	m_pobjEditorView = NULL;
	for(int nCnt=0; nCnt<20; nCnt++)
		m_objPastFindString[nCnt] = _T("");
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
void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindDlg)
	DDX_Control(pDX, IDC_COMBO_FINDSTRING, m_objComboFindString);
	DDX_CBString(pDX, IDC_COMBO_FINDSTRING, m_objFindString);
	DDX_Check(pDX, IDC_CHECK_WHOLEWORD, m_bIsMatchWholeWordFind);
	DDX_Check(pDX, IDC_CHECK_MATCHCASE, m_bIsMatchCaseFind);
	DDX_Radio(pDX, IDC_RADIO_UP, m_bIsSearchDownFind);
	DDX_Check(pDX, IDC_CHECK_REGEX, m_bIsUseRegularExpression);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindDlg, CDialog)
	//{{AFX_MSG_MAP(CFindDlg)
	ON_BN_CLICKED(IDC_FIND, OnFind)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_REGEX, OnCheckRegex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFindDlg::OnInitDialog() 
{
	CEditorApp* pApp = (CEditorApp*)AfxGetApp();

	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

//	//フォント設定
//	m_objComboFindString.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//文字数制限
	m_objComboFindString.LimitText(255);

	//検索文字列コンボボックスへ過去検索データ設定
	CString	objStr;
	char	szKey[32];
	int nIndex=0;
	int nCnt=0,nCnt2=0;
	BOOL bFind=FALSE;
	if(!m_objFindString.IsEmpty())
	{
		m_objPastFindString[nIndex] = m_objFindString;
		m_objComboFindString.AddString(m_objFindString);
		nIndex++;
	}
	for(nCnt=0; nCnt<20; nCnt++)
	{
		if(nIndex >= 20)
			break;
		sprintf(szKey,"FindStr%d",nCnt);
		objStr = pApp->GetProfileString("CFindDlg",szKey,"");
		if( !objStr.IsEmpty() )
		{
			for(bFind=FALSE,nCnt2=0; nCnt2<nIndex; nCnt2++)
			{
				if(m_objPastFindString[nCnt2].Compare(objStr) == 0)
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind == FALSE)
			{
				m_objPastFindString[nIndex] = objStr;
				m_objComboFindString.AddString(objStr);
				nIndex++;
			}
		}
	}
	m_objComboFindString.SetCurSel(0);

	//オートリサイズ
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_COMBO_FINDSTRING,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_FIND,CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDCANCEL,CP_MOVE_HORIZONTAL);

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
		GetDlgItem(IDC_CHECK_WHOLEWORD)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_CHECK_WHOLEWORD)->EnableWindow(TRUE);

	//メンバ変数－＞コントロール
	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「正規表現を使う」チェック時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFindDlg::OnCheckRegex() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	if(m_bIsUseRegularExpression)
		GetDlgItem(IDC_CHECK_WHOLEWORD)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_CHECK_WHOLEWORD)->EnableWindow(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＳＩＺＥ処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFindDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	//オートリサイズ起動
	m_objAutoResizer.MoveControls();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：次を検索選択時処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFindDlg::OnFind() 
{
	int cflags=0, ret=0;
	char szMsg[256];
	CString objBuf;
	BOOL bRet=FALSE;
	CString objMsgStr, objMsg;

	//コントロール－＞メンバ
	UpdateData(TRUE);

	if(m_pobjEditorView == NULL)
		return;

	//ブランクチェック
	if(m_objFindString.IsEmpty())
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
		m_bIsMatchWholeWordFind=FALSE;

		cflags = REG_EXTENDED;
		if(m_bIsMatchCaseFind==FALSE)//大小文字を区別しない
			cflags |= REG_ICASE;
		//myIsNeedRegcomp
		if(m_pApp->m_lpfnRegFunc5((LPCTSTR)m_objFindString,cflags))
		{
			//regcomp
			ret = m_pApp->m_lpfnRegFunc1((LPCTSTR)m_objFindString,cflags);
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
	}

	//マウスカーソル砂時計
	CWaitCursor	objWaitCursor;

	//検索実行
	bRet = m_pobjEditorView->DoFind( m_objFindString,
									 m_bIsMatchCaseFind,
									 m_bIsMatchWholeWordFind,
									 m_bIsSearchDownFind,
									 m_bIsUseRegularExpression);
	if( bRet == FALSE ) // 検索終了
	{
		objMsg.LoadString(IDS_FINDENDMSG);
		objMsgStr.Format(objMsg,m_objFindString);
		m_pApp->MyMessageBox((LPCTSTR)objMsgStr,(MB_OK|MB_ICONINFORMATION),0);
	}

	//検索パラメータ
	m_pobjMainFrame->SetFindParameter( m_objFindString,
									   m_bIsMatchCaseFind,
									   m_bIsMatchWholeWordFind,
									   m_bIsSearchDownFind,
									   m_bIsUseRegularExpression );
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャンセル処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFindDlg::OnCancel() 
{
	//コントロール－＞メンバ
	UpdateData(TRUE);

	CEditorApp* pApp = (CEditorApp*)AfxGetApp();

	//履歴データ更新
	char	szKey[32];
	pApp->WriteProfileString("CFindDlg","FindStr0",m_objFindString);
	int nIndex=0;
	int nCnt=0,nCnt2=0;
	BOOL bFind=FALSE;
	for(nIndex=0; nIndex<20; nIndex++)
	{
		sprintf(szKey,"FindStr%d",nCnt+1);
		if( !m_objPastFindString[nIndex].IsEmpty() &&
			m_objFindString != m_objPastFindString[nIndex]
		)
		{
			for(bFind=FALSE,nCnt2=0; nCnt2<nIndex; nCnt2++)
			{
				if(m_objPastFindString[nCnt2].Compare(m_objPastFindString[nIndex]) == 0)
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind==FALSE)
			{
				pApp->WriteProfileString("CFindDlg",szKey,m_objPastFindString[nIndex]);
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

	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// CFindDlg プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウ位置ロード処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFindDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CFindDlg","WindowPos");
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
void CFindDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CFindDlg","WindowPos",strBuffer);
}



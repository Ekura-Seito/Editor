// ReplaceDlg.cpp : インプリメンテーション ファイル
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
#include "ReplaceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReplaceDlg ダイアログ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CReplaceDlg::CReplaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReplaceDlg::IDD, pParent)
{
	m_pApp = (CEditorApp*)AfxGetApp();

	//{{AFX_DATA_INIT(CReplaceDlg)
	m_objFindString = _T("");
	m_objReplaceString = _T("");
	m_bIsMatchCaseReplace = FALSE;
	m_bIsMatchWholeWordReplace = FALSE;
	m_bIsUseRegularExpression = FALSE;
	//}}AFX_DATA_INIT

	m_pobjMainFrame = NULL;
	m_pobjEditorView = NULL;
	for(int nCnt=0; nCnt<20; nCnt++)
	{
		m_objPastFindString[nCnt] = _T("");
		m_objPastReplaceString[nCnt] = _T("");
	}
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
void CReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReplaceDlg)
	DDX_Control(pDX, IDC_COMBO_FINDSTRING, m_objComboFindString);
	DDX_CBString(pDX, IDC_COMBO_FINDSTRING, m_objFindString);
	DDX_Control(pDX, IDC_COMBO_REPLACESTRING, m_objComboReplaceString);
	DDX_CBString(pDX, IDC_COMBO_REPLACESTRING, m_objReplaceString);
	DDX_Check(pDX, IDC_CHECK_MATCHCASE, m_bIsMatchCaseReplace);
	DDX_Check(pDX, IDC_CHECK_WHOLEWORD, m_bIsMatchWholeWordReplace);
	DDX_Check(pDX, IDC_CHECK_REGEX, m_bIsUseRegularExpression);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReplaceDlg, CDialog)
	//{{AFX_MSG_MAP(CReplaceDlg)
	ON_BN_CLICKED(IDC_FIND, OnFind)
	ON_BN_CLICKED(IDC_REPLACEALL, OnReplaceall)
	ON_BN_CLICKED(IDC_REPLACENEXT, OnReplacenext)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_REGEX, OnCheckRegex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReplaceDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CReplaceDlg::OnInitDialog() 
{
	CString	objStr=_T("");
	char	szKey[32];
	int nIndex=0;
	int nCnt=0,nCnt2=0;
	BOOL bFind=FALSE;
	CEditorApp* pApp = (CEditorApp*)AfxGetApp();

	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//文字数制限
	m_objComboFindString.LimitText(255);

//	//フォント設定
//	m_objComboFindString.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
//	m_objComboReplaceString.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//検索文字列コンボボックスへ過去検索データ設定
	nIndex = nCnt = 0;
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
		objStr = pApp->GetProfileString("CReplaceDlg",szKey,"");
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

	//置換文字列コンボボックスへ過去検索データ設定
	nIndex = nCnt = 0;
	if(!m_objReplaceString.IsEmpty())
	{
		m_objPastReplaceString[nIndex] = m_objReplaceString;
		m_objComboReplaceString.AddString(m_objReplaceString);
		nIndex++;
	}
	for(nCnt=0; nCnt<20; nCnt++)
	{
		if(nIndex >= 20)
			break;
		sprintf(szKey,"ReplaceStr%d",nCnt);
		objStr = pApp->GetProfileString("CReplaceDlg",szKey,"");
		if( !objStr.IsEmpty() )
		{
			for(bFind=FALSE,nCnt2=0; nCnt2<nIndex; nCnt2++)
			{
				if(m_objPastReplaceString[nCnt2].Compare(objStr) == 0)
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind == FALSE)
			{
				m_objPastReplaceString[nIndex] = objStr;
				m_objComboReplaceString.AddString(objStr);
				nIndex++;
			}
		}
	}
	m_objComboReplaceString.SetCurSel(0);

	//オートリサイズ
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_COMBO_FINDSTRING,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_COMBO_REPLACESTRING,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_FIND,CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_REPLACENEXT,CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_REPLACEALL,CP_MOVE_HORIZONTAL);
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
void CReplaceDlg::OnCheckRegex() 
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
void CReplaceDlg::OnSize(UINT nType, int cx, int cy) 
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
void CReplaceDlg::OnFind() 
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
		m_bIsMatchWholeWordReplace=FALSE;

		cflags = REG_EXTENDED;
		if(m_bIsMatchCaseReplace==FALSE)//大小文字を区別しない
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
									 m_bIsMatchCaseReplace,
									 m_bIsMatchWholeWordReplace,
									 TRUE,						//文末方向のみ
									 m_bIsUseRegularExpression);
	if( bRet == FALSE ) // 検索終了
	{
		objMsg.LoadString(IDS_FINDENDMSG);
		objMsgStr.Format(objMsg, m_objFindString);
		m_pApp->MyMessageBox((LPCTSTR)objMsgStr,(MB_OK|MB_ICONINFORMATION),0);
	}

	//検索パラメータ
	m_pobjMainFrame->SetReplaceParameter( m_objFindString,
										  m_objReplaceString,
										  m_bIsMatchCaseReplace,
										  m_bIsMatchWholeWordReplace,
										  m_bIsUseRegularExpression );
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：置換して次に選択時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CReplaceDlg::OnReplacenext() 
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
		m_bIsMatchWholeWordReplace=FALSE;

		cflags = REG_EXTENDED;
		if(m_bIsMatchCaseReplace==FALSE)//大小文字を区別しない
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

	if( !m_pobjEditorView->IsSelected() )
	{
		//検索実行
		bRet = m_pobjEditorView->DoFind( m_objFindString,
										 m_bIsMatchCaseReplace,
										 m_bIsMatchWholeWordReplace,
										 TRUE,						//文末方向のみ
										 m_bIsUseRegularExpression);
		if( bRet == FALSE ) // 検索終了
		{
			objMsg.LoadString(IDS_REPLACEENDMSG);
			objMsgStr.Format(objMsg, m_objFindString);
			m_pApp->MyMessageBox((LPCTSTR)objMsgStr,(MB_OK|MB_ICONINFORMATION),0);
			return;
		}
	}

	//置換実行
	m_pobjEditorView->DoReplace( m_objReplaceString );

	//検索実行
	bRet = m_pobjEditorView->DoFind( m_objFindString,
									 m_bIsMatchCaseReplace,
									 m_bIsMatchWholeWordReplace,
									 TRUE,						//文末方向のみ
									 m_bIsUseRegularExpression);
	if( bRet == FALSE ) // 検索終了
	{
		objMsg.LoadString(IDS_REPLACEENDMSG);
		objMsgStr.Format(objMsg, m_objFindString);
		m_pApp->MyMessageBox((LPCTSTR)objMsgStr,(MB_OK|MB_ICONINFORMATION),0);
	}

	//検索パラメータ
	m_pobjMainFrame->SetReplaceParameter( m_objFindString,
										  m_objReplaceString,
										  m_bIsMatchCaseReplace,
										  m_bIsMatchWholeWordReplace,
										  m_bIsUseRegularExpression );
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：すべて置換選択時処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CReplaceDlg::OnReplaceall() 
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
		m_bIsMatchWholeWordReplace=FALSE;

		cflags = REG_EXTENDED;
		if(m_bIsMatchCaseReplace==FALSE)//大小文字を区別しない
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

	for(;;)
	{
		// ウインドウズメッセージ処理
		m_pApp->MessagePump();

		if( !m_pobjEditorView->IsSelected() )
		{
			//検索実行
			bRet = m_pobjEditorView->DoFind( m_objFindString,
											 m_bIsMatchCaseReplace,
											 m_bIsMatchWholeWordReplace,
											 TRUE,						//文末方向のみ
											 m_bIsUseRegularExpression);
			if( bRet == FALSE ) // 検索終了
			{
				objMsg.LoadString(IDS_REPLACEENDMSG);
				objMsgStr.Format(objMsg, m_objFindString);
				m_pApp->MyMessageBox((LPCTSTR)objMsgStr,(MB_OK|MB_ICONINFORMATION),0);
				break;
			}
		}

		//置換実行
		m_pobjEditorView->DoReplace( m_objReplaceString );

		//検索実行
		bRet = m_pobjEditorView->DoFind( m_objFindString,
										 m_bIsMatchCaseReplace,
										 m_bIsMatchWholeWordReplace,
										 TRUE,						//文末方向のみ
										 m_bIsUseRegularExpression);
		if( bRet == FALSE ) // 検索終了
		{
			objMsg.LoadString(IDS_REPLACEENDMSG);
			objMsgStr.Format(objMsg, m_objFindString);
			m_pApp->MyMessageBox((LPCTSTR)objMsgStr,(MB_OK|MB_ICONINFORMATION),0);
			break;
		}
	}

	//検索パラメータ
	m_pobjMainFrame->SetReplaceParameter( m_objFindString,
										  m_objReplaceString,
										  m_bIsMatchCaseReplace,
										  m_bIsMatchWholeWordReplace,
										  m_bIsUseRegularExpression );
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャンセル処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CReplaceDlg::OnCancel() 
{
	CEditorApp* pApp = (CEditorApp*)AfxGetApp();
	char	szKey[32];
	int nIndex=0;
	int nCnt=0,nCnt2=0;
	BOOL bFind=FALSE;

	//コントロール－＞メンバ
	UpdateData(TRUE);

	//検索文字列履歴データ更新
	nIndex = nCnt = 0;
	pApp->WriteProfileString("CReplaceDlg","FindStr0",m_objFindString);
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
				pApp->WriteProfileString("CReplaceDlg",szKey,m_objPastFindString[nIndex]);
				nCnt++;
			}
		}
	}

	//置換文字列履歴データ更新
	nIndex = nCnt = 0;
	pApp->WriteProfileString("CReplaceDlg","ReplaceStr0",m_objReplaceString);
	for(nIndex=0; nIndex<20; nIndex++)
	{
		sprintf(szKey,"ReplaceStr%d",nCnt+1);
		if( !m_objPastReplaceString[nIndex].IsEmpty() &&
			m_objReplaceString != m_objPastReplaceString[nIndex]
		)
		{
			for(bFind=FALSE,nCnt2=0; nCnt2<nIndex; nCnt2++)
			{
				if(m_objPastReplaceString[nCnt2].Compare(m_objPastReplaceString[nIndex]) == 0)
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind==FALSE)
			{
				pApp->WriteProfileString("CReplaceDlg",szKey,m_objPastReplaceString[nIndex]);
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
// CReplaceDlg プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウ位置ロード処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CReplaceDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CReplaceDlg","WindowPos");
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
void CReplaceDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CReplaceDlg","WindowPos",strBuffer);
}


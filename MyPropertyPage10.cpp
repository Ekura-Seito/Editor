// MyPropertyPage10.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "MyPropertyPage10.h"
#include "KeyBind.h"
#include "ListCtrlEx.h"
#include "AutoResizer.h"
#include "KeyBindListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//グローバル変数
extern KEY_FUNC g_stKeyFunc[];
extern KEY g_stKeys[];
extern char* g_szFunc[];

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage10 プロパティ ページ

IMPLEMENT_DYNCREATE(CMyPropertyPage10, CPropertyPage)

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage10::CMyPropertyPage10() : CPropertyPage(CMyPropertyPage10::IDD)
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsChanged = FALSE;								//変更発生フラグ

	//{{AFX_DATA_INIT(CMyPropertyPage10)
	m_bIsCtrl = FALSE;
	m_bIsShift = FALSE;
	m_objMenuExplain = _T("");
	m_bIsAlt = FALSE;
	m_objCmdID = _T("");
	//}}AFX_DATA_INIT
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage10::~CMyPropertyPage10()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＤＤＸデータ交換														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage10::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage10)
	DDX_Control(pDX, IDC_BUTTON_ASSIGN, m_objAssignBtn);
	DDX_Control(pDX, IDC_BUTTON_KAIJYO, m_objKaijyoBtn);
	DDX_Control(pDX, IDC_LIST_CMD, m_objCmdList);
	DDX_Control(pDX, IDC_COMBO_FUNC, m_objFuncCombo);
	DDX_Control(pDX, IDC_LIST_KEY, m_objKeyList);
	DDX_Check(pDX, IDC_CHECK_CTRL, m_bIsCtrl);
	DDX_Check(pDX, IDC_CHECK_SHIFT, m_bIsShift);
	DDX_Text(pDX, IDC_SETUMEI, m_objMenuExplain);
	DDX_Check(pDX, IDC_CHECK_ALT, m_bIsAlt);
	DDX_Text(pDX, IDC_CMDID, m_objCmdID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPropertyPage10, CPropertyPage)
	//{{AFX_MSG_MAP(CMyPropertyPage10)
	ON_BN_CLICKED(IDC_CHECK_CTRL, OnCheckCtrl)
	ON_BN_CLICKED(IDC_CHECK_SHIFT, OnCheckShift)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNC, OnSelchangeComboFunc)
	ON_LBN_SELCHANGE(IDC_LIST_CMD, OnSelchangeListCmd)
	ON_LBN_SELCHANGE(IDC_LIST_KEY, OnSelchangeListKey)
	ON_BN_CLICKED(IDC_BUTTON_ASSIGN, OnButtonAssign)
	ON_BN_CLICKED(IDC_BUTTON_KAIJYO, OnButtonKaijyo)
	ON_BN_CLICKED(IDC_BUTTON_LIST, OnButtonList)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, OnButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON_READ, OnButtonRead)
	ON_BN_CLICKED(IDC_CHECK_ALT, OnCheckAlt)
	ON_BN_CLICKED(IDC_BUTTON_HELP, OnButtonHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage10 パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：変更発生調査処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage10::IsChanged() 
{
	return m_bIsChanged;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：変更発生フラグクリア処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage10::ClearChangeFlag()
{
	m_bIsChanged = FALSE;
	SetModified(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage10 メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage10::OnInitDialog() 
{
	int i=0;
	//基底クラスのハンドラコール
	CPropertyPage::OnInitDialog();

	//キーリスト登録
	for(i=0; ; i++)
	{
		if(g_stKeys[i].nChar==0)
			break;
		if(g_stKeys[i].bIsCtrl==FALSE && g_stKeys[i].bIsShift==FALSE && g_stKeys[i].bIsAlt==FALSE)
			m_objKeyList.AddString(g_stKeys[i].szKeyName);
	}

	//機能コンボボックス登録
	for(i=0; ; i++)
	{
		if(!strlen(g_szFunc[i]))
			break;
		m_objFuncCombo.AddString(g_szFunc[i]);
	}
	m_objFuncCombo.SetCurSel(0);

	//コマンドリストボックスへファイル系メニューを登録
	for(i=0; ; i++)
	{
		if(g_stKeyFunc[i].dwMenuID==0)
			break;
		if(!strcmp(g_stKeyFunc[i].szFuncName,g_szFunc[0]))
			m_objCmdList.AddString(g_stKeyFunc[i].szMenuName);
	}
	m_objCmdList.SelectString(-1,g_stKeyFunc[0].szMenuName);

	//コマンドＩＤ
	m_objCmdID.Format("%d",g_stKeyFunc[0].dwMenuID);
	//説明文
	m_objMenuExplain = g_stKeyFunc[0].szMenuExplain;

	//メンバ変数－＞コントロール
	UpdateData(FALSE);
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「Ctrl」チェック押下時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage10::OnCheckCtrl() 
{
	int i=0;

	//コントロール－＞メンバ変数
	UpdateData(TRUE);

	//キーリストを一度すべてクリア
	for(;;)
	{
		if(m_objKeyList.DeleteString(0) <= 0)
			break;
	}

	for(i=0; ; i++)
	{
		if(g_stKeys[i].nChar==0)
			break;
		if( g_stKeys[i].bIsCtrl==m_bIsCtrl   && 
			g_stKeys[i].bIsShift==m_bIsShift &&
			g_stKeys[i].bIsAlt==m_bIsAlt
		)
		{
			m_objKeyList.AddString(g_stKeys[i].szKeyName);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「Shift」チェック押下時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage10::OnCheckShift() 
{
	int i=0;

	//コントロール－＞メンバ変数
	UpdateData(TRUE);

	//キーリストを一度すべてクリア
	for(;;)
	{
		if(m_objKeyList.DeleteString(0) <= 0)
			break;
	}

	for(i=0; ; i++)
	{
		if(g_stKeys[i].nChar==0)
			break;
		if( g_stKeys[i].bIsCtrl==m_bIsCtrl   && 
			g_stKeys[i].bIsShift==m_bIsShift &&
			g_stKeys[i].bIsAlt==m_bIsAlt
		)
		{
			m_objKeyList.AddString(g_stKeys[i].szKeyName);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「Alt」チェック押下時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage10::OnCheckAlt() 
{
	int i=0;

	//コントロール－＞メンバ変数
	UpdateData(TRUE);

	//キーリストを一度すべてクリア
	for(;;)
	{
		if(m_objKeyList.DeleteString(0) <= 0)
			break;
	}

	for(i=0; ; i++)
	{
		if(g_stKeys[i].nChar==0)
			break;
		if( g_stKeys[i].bIsCtrl==m_bIsCtrl   && 
			g_stKeys[i].bIsShift==m_bIsShift &&
			g_stKeys[i].bIsAlt==m_bIsAlt
		)
		{
			m_objKeyList.AddString(g_stKeys[i].szKeyName);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーリスト選択変更時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage10::OnSelchangeListKey() 
{
	BOOL bFind=FALSE;
	CKeyBind* pobjKeyBind=NULL;
	DWORD dwMenuID=0;
	char szKeyName[64];
	int i=0,j=0;
	int nIndex = m_objKeyList.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//コントロール－＞メンバ変数
	UpdateData(TRUE);

	m_objCmdID = "";
	m_objMenuExplain = "";

	//キー割り当て登録済みかどうかチェック
	for(i=0; ;i++)
	{
		if(g_stKeys[i].nChar==0)
			break;
		if( g_stKeys[i].bIsCtrl==m_bIsCtrl   && 
			g_stKeys[i].bIsShift==m_bIsShift &&
			g_stKeys[i].bIsAlt==m_bIsAlt
		)
		{
			pobjKeyBind = m_pApp->FindKeyBind( g_stKeys[i+nIndex].nChar,
											   g_stKeys[i+nIndex].bIsCtrl,
											   g_stKeys[i+nIndex].bIsShift,
											   g_stKeys[i+nIndex].bIsAlt);
			strcpy(szKeyName,g_stKeys[i+nIndex].szKeyName);
			//Alt+F4, Ctrl+F4, Alt+F6, Ctrl+F6, Shift+Alt+F6は解除できないようにする
			if( g_stKeys[i+nIndex].nChar==VK_F4    &&
				g_stKeys[i+nIndex].bIsCtrl==FALSE  &&
				g_stKeys[i+nIndex].bIsShift==FALSE &&
				g_stKeys[i+nIndex].bIsAlt==TRUE
			)
			{
				m_objAssignBtn.EnableWindow(FALSE);
				m_objKaijyoBtn.EnableWindow(FALSE);
			}
			else if( g_stKeys[i+nIndex].nChar==VK_F4    &&
					 g_stKeys[i+nIndex].bIsCtrl==TRUE  &&
					 g_stKeys[i+nIndex].bIsShift==FALSE &&
					 g_stKeys[i+nIndex].bIsAlt==FALSE
			)
			{
				m_objAssignBtn.EnableWindow(FALSE);
				m_objKaijyoBtn.EnableWindow(FALSE);
			}
			else if( g_stKeys[i+nIndex].nChar==VK_F6    &&
					 g_stKeys[i+nIndex].bIsCtrl==TRUE  &&
					 g_stKeys[i+nIndex].bIsShift==FALSE &&
					 g_stKeys[i+nIndex].bIsAlt==FALSE
			)
			{
				m_objAssignBtn.EnableWindow(FALSE);
				m_objKaijyoBtn.EnableWindow(FALSE);
			}
			else if( g_stKeys[i+nIndex].nChar==VK_F6    &&
					 g_stKeys[i+nIndex].bIsCtrl==FALSE  &&
					 g_stKeys[i+nIndex].bIsShift==FALSE &&
					 g_stKeys[i+nIndex].bIsAlt==TRUE
			)
			{
				m_objAssignBtn.EnableWindow(FALSE);
				m_objKaijyoBtn.EnableWindow(FALSE);
			}
			else if( g_stKeys[i+nIndex].nChar==VK_F6    &&
					 g_stKeys[i+nIndex].bIsCtrl==FALSE  &&
					 g_stKeys[i+nIndex].bIsShift==TRUE &&
					 g_stKeys[i+nIndex].bIsAlt==TRUE
			)
			{
				m_objAssignBtn.EnableWindow(FALSE);
				m_objKaijyoBtn.EnableWindow(FALSE);
			}
			else
			{
				m_objAssignBtn.EnableWindow(TRUE);
				m_objKaijyoBtn.EnableWindow(TRUE);
			}
			break;
		}
	}
	//メニューＩＤ取り出し
	if(pobjKeyBind==NULL)
	{
		//メンバ変数－＞コントロール
		UpdateData(FALSE);
		return;
	}
	else if(pobjKeyBind->m_dwMenuID==0)
	{
		//メンバ変数－＞コントロール
		UpdateData(FALSE);
		return;
	}
	dwMenuID = pobjKeyBind->m_dwMenuID;

	//コマンドリストを一度すべてクリア
	for(;;)
	{
		if(m_objCmdList.DeleteString(0) <= 0)
			break;
	}

	for(i=0;;i++)
	{
		if(g_stKeyFunc[i].dwMenuID == 0)
			break;
		if(g_stKeyFunc[i].dwMenuID == dwMenuID)
		{
			m_objFuncCombo.SelectString(-1,g_stKeyFunc[i].szFuncName);
			for(j=0; ; j++)
			{
				if(g_stKeyFunc[j].dwMenuID==0)
					break;
				if(!strcmp(g_stKeyFunc[i].szFuncName,g_stKeyFunc[j].szFuncName))
					m_objCmdList.AddString(g_stKeyFunc[j].szMenuName);
			}
			m_objCmdList.SelectString(-1,g_stKeyFunc[i].szMenuName);
			m_objCmdID.Format("%d",g_stKeyFunc[i].dwMenuID);
			m_objMenuExplain = g_stKeyFunc[i].szMenuExplain;
			//メンバ変数－＞コントロール
			UpdateData(FALSE);
			return;
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：機能コンボボックス選択変更時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage10::OnSelchangeComboFunc() 
{
	int i=0,nPos=-1;
	char szFuncName[64];
	int nIndex = m_objFuncCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;
	if(m_objFuncCombo.GetLBText(nIndex,szFuncName)==CB_ERR)
		return;

	//コマンドリストを一度すべてクリア
	for(;;)
	{
		if(m_objCmdList.DeleteString(0) <= 0)
			break;
	}

	for(nPos=-1,i=0; ; i++)
	{
		if(g_stKeyFunc[i].dwMenuID==0)
			break;
		if(!strcmp(g_stKeyFunc[i].szFuncName,szFuncName))
		{
			if(nPos == -1)
				nPos = i;
			m_objCmdList.AddString(g_stKeyFunc[i].szMenuName);
		}
	}
	if(nPos != -1)
	{
		m_objCmdList.SelectString(-1,g_stKeyFunc[nPos].szMenuName);
		m_objCmdID.Format("%d",g_stKeyFunc[nPos].dwMenuID);
		m_objMenuExplain = g_stKeyFunc[nPos].szMenuExplain;
		//メンバ変数－＞コントロール
		UpdateData(FALSE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コマンドリスト選択変更時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage10::OnSelchangeListCmd() 
{
	char szFuncName[64];
	int nIndex=0, nIndex2=0, nPos=0, i=0;
	CKeyBind* pobjKeyBind=NULL;
	nIndex = m_objFuncCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;
	if(m_objFuncCombo.GetLBText(nIndex,szFuncName)==CB_ERR)
		return;
	nIndex2 = m_objCmdList.GetCurSel();
	if(nIndex2 == CB_ERR)
		return;

	for(i=0; ;i++)
	{
		if(g_stKeyFunc[i].dwMenuID==0)
			break;
		if(!strcmp(g_stKeyFunc[i].szFuncName,szFuncName))
		{
			m_objCmdID.Format("%d",g_stKeyFunc[i+nIndex2].dwMenuID);
			m_objMenuExplain = g_stKeyFunc[i+nIndex2].szMenuExplain;
			//メンバ変数－＞コントロール
			UpdateData(FALSE);
			return;
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「割り当て」ボタン押下時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage10::OnButtonAssign() 
{
	UINT nChar=0;
	DWORD dwMenuID=0;
	int i=0,nIndex=0,nIndex2=0,nIndex3=0;
	char szKeyName[64],szFuncName[64],szMenuName[128],szMsg[256];
	CString objMsg;

	//コントロール－＞メンバ変数
	UpdateData(TRUE);

	nIndex = m_objKeyList.GetCurSel();
	if(nIndex == CB_ERR)
		return;
	nIndex2 = m_objFuncCombo.GetCurSel();
	if(nIndex2 == CB_ERR)
		return;
	if(m_objFuncCombo.GetLBText(nIndex2,szFuncName)==CB_ERR)
		return;
	nIndex3 = m_objCmdList.GetCurSel();
	if(nIndex3 == CB_ERR)
		return;

	//キー名称取得
	for(i=0; ;i++)
	{
		if(g_stKeys[i].nChar==0)
			return;
		if( g_stKeys[i].bIsCtrl==m_bIsCtrl   && 
			g_stKeys[i].bIsShift==m_bIsShift &&
			g_stKeys[i].bIsAlt==m_bIsAlt
		)
		{
			nChar = g_stKeys[i+nIndex].nChar;
			strcpy(szKeyName,g_stKeys[i+nIndex].szKeyName);
			break;
		}
	}

	//機能名称取得
	for(i=0; ;i++)
	{
		if(g_stKeyFunc[i].dwMenuID==0)
			return;
		if(!strcmp(g_stKeyFunc[i].szFuncName,szFuncName))
		{
			strcpy(szMenuName,g_stKeyFunc[i+nIndex3].szMenuName);
			dwMenuID = g_stKeyFunc[i+nIndex3].dwMenuID;
			break;
		}
	}
	objMsg.LoadString(IDS_KEYBIND_ASSIGN);
	sprintf(szMsg,(LPCTSTR)objMsg,szKeyName,szFuncName,szMenuName);
	int nRet = m_pApp->MyMessageBox(szMsg,(MB_OKCANCEL|MB_ICONINFORMATION),0);
	if(nRet != IDOK)
		return;

	m_pApp->addKeyBind(CKeyBind(nChar,dwMenuID,m_bIsCtrl,m_bIsShift,m_bIsAlt));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「解除」ボタン押下時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage10::OnButtonKaijyo() 
{
	DWORD dwMenuID=0;
	int i=0,nIndex=0;
	char szKeyName[64],szMsg[256];
	CKeyBind* pobjKeyBind=NULL;
	CString objMsg;

	//コントロール－＞メンバ変数
	UpdateData(TRUE);

	nIndex = m_objKeyList.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//キー割り当て登録済みかどうかチェック
	for(i=0; ;i++)
	{
		if(g_stKeys[i].nChar==0)
			break;
		if( g_stKeys[i].bIsCtrl==m_bIsCtrl   && 
			g_stKeys[i].bIsShift==m_bIsShift && 
			g_stKeys[i].bIsAlt==m_bIsAlt
		)
		{
			pobjKeyBind = m_pApp->FindKeyBind( g_stKeys[i+nIndex].nChar,
											   g_stKeys[i+nIndex].bIsCtrl,
											   g_stKeys[i+nIndex].bIsShift,
											   g_stKeys[i+nIndex].bIsAlt);
			strcpy(szKeyName,g_stKeys[i+nIndex].szKeyName);
			break;
		}
	}

	if(pobjKeyBind==NULL)
	{
		objMsg.LoadString(IDS_KEYBIND_NOTASSIGNED);
		sprintf(szMsg,(LPCTSTR)objMsg,szKeyName);
		m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONEXCLAMATION),0);
		return;
	}
	else if(pobjKeyBind->m_dwMenuID==0)
	{
		objMsg.LoadString(IDS_KEYBIND_NOTASSIGNED);
		sprintf(szMsg,(LPCTSTR)objMsg,szKeyName);
		m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONEXCLAMATION),0);
		return;
	}

	objMsg.LoadString(IDS_KEYBIND_DEASSIGN);
	sprintf(szMsg,(LPCTSTR)objMsg,szKeyName);
	int nRet = m_pApp->MyMessageBox(szMsg,(MB_OKCANCEL|MB_ICONINFORMATION),0);
	if(nRet != IDOK)
		return;

	CKeyBind objKeyBind((*pobjKeyBind));
	objKeyBind.m_dwMenuID = 0;
	m_pApp->addKeyBind(objKeyBind);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「一覧表示」ボタン押下時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage10::OnButtonList() 
{
	CKeyBindListDlg objKeyBindListDlg;
	objKeyBindListDlg.DoModal();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「リセット」ボタン押下時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage10::OnButtonReset() 
{
	CString objMsg;
	objMsg.LoadString(IDS_KEYBIND_RESET);
	int nRet = m_pApp->MyMessageBox(objMsg,(MB_OKCANCEL|MB_ICONINFORMATION),0);
	if(nRet != IDOK)
		return;
	m_pApp->resetKeyBind();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル書き出し」ボタン押下時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage10::OnButtonWrite() 
{
	m_pApp->writeKeyBind();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル読み込み」ボタン押下時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage10::OnButtonRead() 
{
	m_pApp->readKeyBind();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ヘルプ」ボタン押下時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage10::OnButtonHelp() 
{
	m_pApp->OpenHelpTopix(HELPTOPIC_KEYBIND);

//test
//	FILE* fp = fopen("aaa","w");
//	for(int i=0; ;i++)
//	{
//		if(g_stKeyFunc[i].dwMenuID==0)
//			break;
//		fprintf(fp,"   %s    %d     %s\n",g_stKeyFunc[i].szFuncName,
//								g_stKeyFunc[i].dwMenuID,
//								g_stKeyFunc[i].szMenuName);
//	}
//	fclose(fp);
//test end
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ＯＫ」ボタン押下時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage10::OnOK() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);
	CPropertyPage::OnOK();
}



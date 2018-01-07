// KeyBindListDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "ListCtrlEx.h"
#include "AutoResizer.h"
#include "KeyBindListDlg.h"
#include "KeyBind.h"

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
// CKeyBindListDlg ダイアログ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CKeyBindListDlg::CKeyBindListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyBindListDlg::IDD, pParent)
{
	m_pApp = (CEditorApp*)AfxGetApp();

	//{{AFX_DATA_INIT(CKeyBindListDlg)
	//}}AFX_DATA_INIT

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
void CKeyBindListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyBindListDlg)
	DDX_Control(pDX, IDC_LIST_KEYBIND, m_objKeyBindList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeyBindListDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyBindListDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyBindListDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CKeyBindListDlg::OnInitDialog() 
{
	int i=0,j=0,nCnt=0;
	LV_ITEM	lvi;
	BOOL bFind=FALSE;
	char szKeyName[64],szFuncName[64],szMenuName[128];
	CKeyBind* pobjKeyBind=NULL;
	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//カラム挿入
	m_objKeyBindList.InsertColumn( 0, "Key", LVCFMT_LEFT, 120 );
	m_objKeyBindList.InsertColumn( 1, "Func", LVCFMT_LEFT, 100 );
	m_objKeyBindList.InsertColumn( 2, "Command", LVCFMT_LEFT, 250 );

	//レポートビュー
	m_objKeyBindList.ModifyStyle( LVS_TYPEMASK, LVS_REPORT);

	//キーリストをたどりながらアイテム挿入
	for(i=0;;i++)
	{
		if(g_stKeys[i].nChar==0)
			break;
		pobjKeyBind=m_pApp->FindKeyBind( g_stKeys[i].nChar,
										 g_stKeys[i].bIsCtrl,
										 g_stKeys[i].bIsShift,
										 g_stKeys[i].bIsAlt);
		if(pobjKeyBind == NULL)
			continue;
		strcpy(szKeyName,g_stKeys[i].szKeyName);

		bFind=FALSE;
		for(j=0;;j++)
		{
			if(g_stKeyFunc[j].dwMenuID == 0)
				break;
			if(g_stKeyFunc[j].dwMenuID == pobjKeyBind->m_dwMenuID)
			{
				strcpy(szFuncName,g_stKeyFunc[j].szFuncName);
				strcpy(szMenuName,g_stKeyFunc[j].szMenuName);
				bFind=TRUE;
				break;
			}
		}
		if(bFind)
		{
			nCnt = m_objKeyBindList.GetItemCount();
			memset(&lvi,0,sizeof(LV_ITEM));
			lvi.mask = LVIF_TEXT;
			lvi.iItem = nCnt;
			lvi.iSubItem = 0;
			lvi.pszText = szKeyName;
			m_objKeyBindList.InsertItem(&lvi);
			m_objKeyBindList.SetItemText( nCnt, 1, szFuncName);
			m_objKeyBindList.SetItemText( nCnt, 2, szMenuName);
			continue;
		}
	}

	//オートリサイズ
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_LIST_KEYBIND,CP_RESIZE_HORIZONTAL|CP_RESIZE_VERTICAL);
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
void CKeyBindListDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	//オートリサイズ起動
	m_objAutoResizer.MoveControls();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＯＫボタン押下時処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyBindListDlg::OnOK() 
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
// CKeyBindListDlg プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウ位置ロード処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CKeyBindListDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CKeyBindListDlg","WindowPos");
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
void CKeyBindListDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CKeyBindListDlg","WindowPos",strBuffer);
}


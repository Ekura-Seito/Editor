// KeyOpeDispDlg.cpp : CKeyOpeDispDlg クラスの動作の定義を行います。
//

#include "stdafx.h"
#include <afxtempl.h>			//for CTypedPtrList
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "Editor.h"
#include "Operation.h"
#include "OperationManager.h"
#include "StaticFilespec.h"
#include "ListCtrlEx.h"
#include "AutoResizer.h"
#include "KeyOpeDispDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyOpeListDlg クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CKeyOpeDispDlg::CKeyOpeDispDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyOpeDispDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyOpeDispDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_pobjOperationManager = NULL;
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
void CKeyOpeDispDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyOpeDispDlg)
	DDX_Control(pDX, IDC_EDIT_OPERATIONNAME, m_objOperatinNameEdit);
	DDX_Control(pDX, IDC_KEYOPE_LIST, m_objKeyOpeList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKeyOpeDispDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyOpeDispDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyOpeDispDlg パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：初期化																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyOpeDispDlg::Initialize
	(	COperationManager*	pobjOperationManager
	)
{
	m_pobjOperationManager = pobjOperationManager;
}

/////////////////////////////////////////////////////////////////////////////
// CKeyOpeDispDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＩＮＩＴＤＩＡＬＯＧ処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CKeyOpeDispDlg::OnInitDialog() 
{
	long	lCnt=0;
	COperation* pobjOperation = NULL;
	char	szNo[16];
	char	szBuf[256];

	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//名前欄のフォント設定
	m_objOperatinNameEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//名前欄エディット初期化
	CString objOperationName = _T("");
	m_pobjOperationManager->GetOperationName(objOperationName);
	m_objOperatinNameEdit.SetWindowText(objOperationName);
	UpdateData(FALSE);

	//キー操作ファイル名初期化
	CString objKeyOpeFname = _T("");
	m_pobjOperationManager->GetOperationFileName(objKeyOpeFname);
	if(objKeyOpeFname.IsEmpty())
		objKeyOpeFname = "unsaved";
	m_wndStaticFileTitle.SubclassDlgItem(IDC_KEYOPE_FNAME, this);
	m_wndStaticFileTitle.SetPath(TRUE);
	SetDlgItemText(IDC_KEYOPE_FNAME, objKeyOpeFname);

	//リストヘッダ設定
	m_objKeyOpeList.InsertColumn(0, "No.", LVCFMT_LEFT, 36);
	m_objKeyOpeList.InsertColumn(1, "Kind", LVCFMT_LEFT, 512-32);

	//レポートビュー
	m_objKeyOpeList.ModifyStyle( LVS_TYPEMASK, LVS_REPORT);

	if(m_pobjOperationManager != NULL)
	{
		//オペレーション順次検索
		m_pobjOperationManager->SearchStartOperation();
		for(lCnt=0;;lCnt++)
		{
			pobjOperation = m_pobjOperationManager->SearchGetOperation();
			if(pobjOperation == NULL)
				break;

			sprintf(szNo,"%02d",(lCnt+1));
			m_objKeyOpeList.InsertItem(lCnt,szNo);

			//オペレーション内容説明テキスト取得
			memset(szBuf,0,sizeof(szBuf));
			pobjOperation->GetOperationString(szBuf);

			//リストへ表示
			m_objKeyOpeList.SetItemText(lCnt,1,szBuf);
		}
		m_pobjOperationManager->SearchEndOperation();
	}

	//オートリサイズ
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_EDIT_OPERATIONNAME,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_KEYOPE_FNAME,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_KEYOPE_LIST,CP_RESIZE_HORIZONTAL|CP_RESIZE_VERTICAL);
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
void CKeyOpeDispDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	//オートリサイズ起動
	m_objAutoResizer.MoveControls();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ＯＫ」ボタン押下時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyOpeDispDlg::OnOK() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	//名前欄取得
	CString objOperationName=_T("");
	m_objOperatinNameEdit.GetWindowText(objOperationName);
	m_pobjOperationManager->SetOperationName(objOperationName);

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
// CKeyOpeDispDlg プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウ位置ロード処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CKeyOpeDispDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CKeyOpeDispDlg","WindowPos");
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
void CKeyOpeDispDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CKeyOpeDispDlg","WindowPos",strBuffer);
}


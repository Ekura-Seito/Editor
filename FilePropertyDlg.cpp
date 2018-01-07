// FilePropertyDlg.cpp : CFilePropertyDlg クラスの動作の定義を行います。
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "StaticFilespec.h"
#include "AutoResizer.h"
#include "FilePropertyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilePropertyDlg クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFilePropertyDlg::CFilePropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFilePropertyDlg::IDD, pParent)
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_objTitle = _T("");

	//{{AFX_DATA_INIT(CFilePropertyDlg)
	m_objFileName = _T("");
	m_objCTime = _T("");
	m_objMTime = _T("");
	m_objATime = _T("");
	m_objSize = _T("");
	m_objAttribute = _T("");
	m_objKaigyo = _T("");
	m_objCode = _T("");
	m_objLineCnt = _T("");
	m_objTabSize = _T("");
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
void CFilePropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilePropertyDlg)
	DDX_Text(pDX, IDC_FILENAME, m_objFileName);
	DDX_Text(pDX, IDC_CTIME, m_objCTime);
	DDX_Text(pDX, IDC_MTIME, m_objMTime);
	DDX_Text(pDX, IDC_ATIME, m_objATime);
	DDX_Text(pDX, IDC_LOGSIZE, m_objSize);
	DDX_Text(pDX, IDC_ATTR, m_objAttribute);
	DDX_Text(pDX, IDC_KAIGYO, m_objKaigyo);
	DDX_Text(pDX, IDC_CODE, m_objCode);
	DDX_Text(pDX, IDC_LINECNT, m_objLineCnt);
	DDX_Text(pDX, IDC_TABSIZE, m_objTabSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilePropertyDlg, CDialog)
	//{{AFX_MSG_MAP(CFilePropertyDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilePropertyDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFilePropertyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//タイトル表示
	m_wndStaticFileTitle.SubclassDlgItem(IDC_TITLE, this);
	m_wndStaticFileTitle.SetPath(TRUE);
	SetDlgItemText(IDC_TITLE, m_objTitle);

	//オートリサイズ
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_TITLE,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_FILENAME,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_CTIME,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_MTIME,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_ATIME,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_LOGSIZE,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_ATTR,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_KAIGYO,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_CODE,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_LINECNT,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_TABSIZE,CP_RESIZE_HORIZONTAL);
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
void CFilePropertyDlg::OnSize(UINT nType, int cx, int cy) 
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
void CFilePropertyDlg::OnOK() 
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
// CFilePropertyDlg プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウ位置ロード処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFilePropertyDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CFilePropertyDlg","WindowPos");
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
void CFilePropertyDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CFilePropertyDlg","WindowPos",strBuffer);
}


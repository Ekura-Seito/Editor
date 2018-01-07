// ExecPasswdDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "Editor_Def.h"
#include "ExecPasswdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExecPasswdDlg クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CExecPasswdDlg::CExecPasswdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExecPasswdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExecPasswdDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_objExecPasswdStr = _T("");				// 起動パスワード
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
void CExecPasswdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExecPasswdDlg)
	DDX_Control(pDX, IDC_EDIT_EXECPASSWD, m_objExecPasswdEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExecPasswdDlg, CDialog)
	//{{AFX_MSG_MAP(CExecPasswdDlg)
	ON_EN_CHANGE(IDC_EDIT_EXECPASSWD, OnChangeEditExecpasswd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExecPasswdDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CExecPasswdDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//フォント設定
	m_objExecPasswdEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//デフォルトデータ設定
	m_objExecPasswdStr = _T("");
	m_objExecPasswdEdit.SetWindowText(m_objExecPasswdStr);
	m_objExecPasswdEdit.UpdateData(TRUE);

	//メンバ変数－＞コントロール
	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：パスワードエディットボックス変更時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CExecPasswdDlg::OnChangeEditExecpasswd() 
{
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで EM_SETEVENTMASK
	// メッセージをコントロールへ送るために CDialog::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。

	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＯＫボタン押下時処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CExecPasswdDlg::OnOK() 
{
	CString objMsg;
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	//データ取得
	m_objExecPasswdEdit.GetWindowText(m_objExecPasswdStr);
	if(m_objExecPasswdStr.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_PASSWD);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	CDialog::OnOK();
}


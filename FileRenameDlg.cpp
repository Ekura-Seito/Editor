// FileRenameDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "FileRenameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileRenameDlg クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFileRenameDlg::CFileRenameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileRenameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileRenameDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	memset(m_szFromDir,0,sizeof(m_szFromDir));
	memset(m_szFromName,0,sizeof(m_szFromName));
	memset(m_szFromPath,0,sizeof(m_szFromPath));
	m_objToFile = _T("");
	memset(m_szToPath,0,sizeof(m_szToPath));
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
void CFileRenameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileRenameDlg)
	DDX_Control(pDX, IDC_EDIT_TOFILE, m_objEditToFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileRenameDlg, CDialog)
	//{{AFX_MSG_MAP(CFileRenameDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileRenameDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileRenameDlg::OnInitDialog() 
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
	m_objEditToFile.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//ファイル名とディレクトリ取得
	strcpy(m_szFromDir,m_szFromPath);
	long lPos=0;
	long lCnt=0;
	for(lCnt=0; lCnt<(signed)strlen(m_szFromDir); )
	{
		if(IsDBCSLeadByteEx(0,m_szFromDir[lCnt]))//２ﾊﾞｲﾄ文字の第一ﾊﾞｲﾄ
		{
			lCnt+=2;
		}
		else
		{
			if(m_szFromDir[lCnt]=='\\')
			{
				//'\'位置保持
				lPos = lCnt;
			}
			lCnt++;
		}
	}
	strcpy(m_szFromName,&m_szFromDir[lPos+1]);
	m_szFromDir[lPos+1] = 0;

	//元ファイル名表示
	CString objStr = m_szFromName;
	SetDlgItemText(IDC_FROMFILE, objStr);

	//デフォルトデータ設定
	m_objEditToFile.SetWindowText(m_objToFile);
	m_objEditToFile.UpdateData(TRUE);

	//メンバ変数－＞コントロール
	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＯＫボタン押下時処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileRenameDlg::OnOK() 
{
	CString objMsg;
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	//データ取得
	m_objEditToFile.GetWindowText(m_objToFile);

	//ブランクチェック
	if(m_objToFile.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_AFTERNAME);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	//名前重複チェック
	if(m_objToFile.CompareNoCase(m_szFromName) == 0)
	{
		objMsg.LoadString(IDS_INPUT_DIFFNAME);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	//変更後ファイル名妥当性チェック
	for(long lCnt=0; lCnt<m_objToFile.GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,m_objToFile.GetAt(lCnt)))//２ﾊﾞｲﾄ文字の第一ﾊﾞｲﾄ
		{
			lCnt+=2;
		}
		else
		{
			if( m_objToFile.GetAt(lCnt) == '\\' )
			{
				objMsg.LoadString(IDS_INVALIDCHAR_AFTERNAME);
				m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
				return;
			}
			lCnt++;
		}
	}

	//変更後のファイルパスを編集
	sprintf(m_szToPath,"%s%s",m_szFromDir,(LPCTSTR)m_objToFile);

	CDialog::OnOK();
}


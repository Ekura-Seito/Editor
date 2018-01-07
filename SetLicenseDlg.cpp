// SetLicenseDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <io.h>
#include <direct.h>
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "SetLicenseDlg.h"
#include "Mymd5.h"
#include "Myfeal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CSetLicenseDlg::CSetLicenseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetLicenseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetLicenseDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
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
void CSetLicenseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetLicenseDlg)
	DDX_Control(pDX, IDC_EDIT_KAISYA, m_objKaisyaEdit);
	DDX_Control(pDX, IDC_EDIT_USER, m_objUserEdit);
	DDX_Control(pDX, IDC_EDIT_LICENSEKEY, m_objLicenseKeyEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetLicenseDlg, CDialog)
	//{{AFX_MSG_MAP(CSetLicenseDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetLicenseDlg メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CSetLicenseDlg::OnInitDialog() 
{
	CString		objKaisya=_T("");
	CString		objUser=_T("");
	CString		objLicenseKey=_T("");

	CDialog::OnInitDialog();

	//アイコン設定
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//レジストリ読み込み
	objKaisya     = m_pApp->GetProfileString("LICENSE","KAISYA","");
	objUser       = m_pApp->GetProfileString("LICENSE","USER","");
	objLicenseKey = m_pApp->GetProfileString("LICENSE","LICENSEKEY","");

	//フォント設定
	m_objKaisyaEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	m_objUserEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	m_objLicenseKeyEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//ダイアログへ表示
	m_objKaisyaEdit.SetWindowText(objKaisya);
	m_objUserEdit.SetWindowText(objUser);
	m_objLicenseKeyEdit.SetWindowText(objLicenseKey);

	//メンバ変数－＞コントロール
	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ＯＫ」ボタン押下時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSetLicenseDlg::OnOK()
{
	char*		pPtr=NULL;
	BOOL		bLicenseOK=FALSE;
	CString		objKaisya=_T("");
	CString		objUser=_T("");
	CString		objLicenseKey=_T("");
	CString		objMsg;

	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	m_objKaisyaEdit.GetWindowText(objKaisya);
	m_objUserEdit.GetWindowText(objUser);
	m_objLicenseKeyEdit.GetWindowText(objLicenseKey);

	if(objKaisya.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_ORGANIZATION);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if(objUser.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_USERNAME);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	if(!objLicenseKey.IsEmpty()) //ライセンスキー入力済み
	{
		//ライセンスキーチェック
		BOOL bFind=FALSE;
		for(long lCnt=0; lCnt<LISENCEKEY_COUNT; lCnt++)
		{
			if(objLicenseKey.Compare(g_szLisenceKeys[lCnt])==NULL) //OK
			{
				bFind=TRUE;
				break;
			}
		}
		if(bFind != FALSE) //OK
		{
			objMsg.LoadString(IDS_LICENSE_OK);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
			bLicenseOK=TRUE;
		}
		else //NG
		{
			objMsg.LoadString(IDS_LICENSE_NG);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//レジストリ書込み
	m_pApp->WriteProfileString("LICENSE","KAISYA",objKaisya);
	m_pApp->WriteProfileString("LICENSE","USER",objUser);
	if(bLicenseOK)
		m_pApp->WriteProfileString("LICENSE","LICENSEKEY",objLicenseKey);

	//基底ハンドラコール
	CDialog::OnOK();
}


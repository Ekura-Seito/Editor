// MyPropertyPage4.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "Line.h"					//１行クラス
#include "LineManager.h"
#include "CodeConvManager.h"
#include "MyPropertyPage4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMyPropertyPage4, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage4 プロパティ ページ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage4::CMyPropertyPage4() : CPropertyPage(CMyPropertyPage4::IDD)
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsChanged = FALSE;								// 変更発生フラグ

	//{{AFX_DATA_INIT(CMyPropertyPage4)
	m_nKaigyoKind = -1;
	m_bIsAutoSave = FALSE;
	m_bDelEndSpacesOnSave = FALSE;
	m_bSpace2TabOnSave = FALSE;
	//}}AFX_DATA_INIT
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage4::~CMyPropertyPage4()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage4 パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：変更発生調査処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage4::IsChanged() 
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
void CMyPropertyPage4::ClearChangeFlag()
{
	m_bIsChanged = FALSE;
	SetModified(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＤＤＸデータ交換														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage4)
	DDX_Control(pDX, IDC_EDIT_SAVEINTERVAL, m_objSaveIntervalEdit);
	DDX_Radio(pDX, IDC_RADIO_CRLF, m_nKaigyoKind);
	DDX_Check(pDX, IDC_CHECK_AUTOSAVE, m_bIsAutoSave);
	DDX_Check(pDX, IDC_CHECK_DELENDSPACES, m_bDelEndSpacesOnSave);
	DDX_Check(pDX, IDC_CHECK_SPACE2TAB, m_bSpace2TabOnSave);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPropertyPage4, CPropertyPage)
	//{{AFX_MSG_MAP(CMyPropertyPage4)
	ON_BN_CLICKED(IDC_RADIO_CRLF, OnRadioCrlf)
	ON_BN_CLICKED(IDC_RADIO_LF, OnRadioLf)
	ON_BN_CLICKED(IDC_RADIO_CR, OnRadioCr)
	ON_BN_CLICKED(IDC_CHECK_AUTOSAVE, OnCheckAutosave)
	ON_EN_CHANGE(IDC_EDIT_SAVEINTERVAL, OnChangeEditSaveinterval)
	ON_BN_CLICKED(IDC_CHECK_DELENDSPACES, OnCheckDelendspacesonsave)
	ON_BN_CLICKED(IDC_CHECK_SPACE2TAB, OnCheckSpace2tab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage4 メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ初期化処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage4::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	//フォント設定
	m_objSaveIntervalEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//オートセーブ間隔エディット有効／無効設定
	if( m_bIsAutoSave )
		m_objSaveIntervalEdit.EnableWindow(TRUE);
	else
		m_objSaveIntervalEdit.EnableWindow(FALSE);

	//オートセーブ間隔エディット初期化
	char	szTemp[32];
	sprintf(szTemp,"%d",m_lSaveInterval);
	m_objSaveIntervalEdit.SetWindowText(szTemp);
	UpdateData(FALSE);

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＣＲ／ＬＦ選択時処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage4::OnRadioCrlf() 
{
	if(m_nKaigyoKind != KAIGYO_CRLF)
	{
		m_nKaigyoKind = KAIGYO_CRLF;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＬＦ選択時処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage4::OnRadioLf() 
{
	if(m_nKaigyoKind != KAIGYO_LF)
	{
		m_nKaigyoKind = KAIGYO_LF;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＣＲ選択時処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage4::OnRadioCr() 
{
	if(m_nKaigyoKind != KAIGYO_CR)
	{
		m_nKaigyoKind = KAIGYO_CR;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「オートセーブを行う」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage4::OnCheckAutosave() 
{
	m_bIsAutoSave = (m_bIsAutoSave!=FALSE) ? FALSE : TRUE;

	m_bIsChanged = TRUE;
	SetModified();

	//オートセーブ間隔エディット有効／無効設定
	if( m_bIsAutoSave )
		m_objSaveIntervalEdit.EnableWindow(TRUE);
	else
		m_objSaveIntervalEdit.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オートセーブ間隔エディットボックス変更時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage4::OnChangeEditSaveinterval() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「保存時に行末の空白削除」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage4::OnCheckDelendspacesonsave() 
{
	m_bDelEndSpacesOnSave = (m_bDelEndSpacesOnSave!=FALSE) ? FALSE : TRUE;

	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「保存時に空白をタブへ変換」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage4::OnCheckSpace2tab() 
{
	m_bSpace2TabOnSave = (m_bSpace2TabOnSave!=FALSE) ? FALSE : TRUE;

	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ＯＫ」ボタン押下時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage4::OnOK() 
{
	CString objMsg;

	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	if(m_bIsAutoSave)
	{
		CString objStr;
		m_objSaveIntervalEdit.GetWindowText(objStr);
		m_lSaveInterval = atoi((char*)(LPCTSTR)objStr);
	
		//オートセーブ間隔妥当性チェック
		if(m_lSaveInterval < 1 || m_lSaveInterval > 60)
		{
			objMsg.LoadString(IDS_AUTOSAVETIME_ERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONEXCLAMATION),0);
		}
	}

	CPropertyPage::OnOK();
}


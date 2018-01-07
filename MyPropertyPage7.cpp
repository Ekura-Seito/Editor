// MyPropertyPage7.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "MyPropertyPage7.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMyPropertyPage7, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage7 プロパティ ページ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage7::CMyPropertyPage7() : CPropertyPage(CMyPropertyPage7::IDD)
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_unLogoPosX = 0;
	m_unLogoPosY = 0;
	m_bIsChanged = FALSE;								// 変更発生フラグ
	m_objBmpFilePath = _T("");

	//{{AFX_DATA_INIT(CMyPropertyPage7)
	m_bDispLogoBmp = FALSE;
	m_nLogoBmpKind = -1;
	m_nLogoPosKind = -1;
	//}}AFX_DATA_INIT
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage7::~CMyPropertyPage7()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage7 パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：変更発生調査処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage7::IsChanged() 
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
void CMyPropertyPage7::ClearChangeFlag()
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
void CMyPropertyPage7::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage7)
	DDX_Control(pDX, IDC_REF, m_objRefBtn);
	DDX_Check(pDX, IDC_CHECK_LOGODISP, m_bDispLogoBmp);
	DDX_Radio(pDX, IDC_RADIO_XERXES, m_nLogoBmpKind);
	DDX_Control(pDX, IDC_EDIT_BMPFILEPATH, m_objBmpFilePathEdit);
	DDX_Control(pDX, IDC_EDIT_LOGOPOSX, m_objLogoPosXEdit);
	DDX_Control(pDX, IDC_EDIT_LOGOPOSY, m_objLogoPosYEdit);
	DDX_Radio(pDX, IDC_RADIO_TILE, m_nLogoPosKind);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPropertyPage7, CPropertyPage)
	//{{AFX_MSG_MAP(CMyPropertyPage7)
	ON_BN_CLICKED(IDC_REF, OnRef)
	ON_BN_CLICKED(IDC_CHECK_LOGODISP, OnCheckLogodisp)
	ON_BN_CLICKED(IDC_RADIO_XERXES, OnRadioXerxesLogo)
	ON_BN_CLICKED(IDC_RADIO_FAVARITE, OnRadioFavariteLogo)
	ON_EN_CHANGE(IDC_EDIT_BMPFILEPATH, OnChangeEditBmpFilePath)
	ON_EN_CHANGE(IDC_EDIT_LOGOPOSX, OnChangeEditLogoposx)
	ON_EN_CHANGE(IDC_EDIT_LOGOPOSY, OnChangeEditLogoposy)
	ON_BN_CLICKED(IDC_RADIO_TILE, OnRadioTile)
	ON_BN_CLICKED(IDC_RADIO_POSSITEI, OnRadioPossitei)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage7 メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ初期化処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage7::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	//フォント設定
	m_objBmpFilePathEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	m_objLogoPosXEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	m_objLogoPosYEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	if( m_bDispLogoBmp )
	{
		//ロゴビットマップ区分　有効／無効設定
		GetDlgItem(IDC_RADIO_XERXES)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_FAVARITE)->EnableWindow(TRUE);
		if( m_nLogoBmpKind == LOGO_XERXES )
		{
			m_objRefBtn.EnableWindow(FALSE);
			m_objBmpFilePathEdit.EnableWindow(FALSE);
		}
		else if( m_nLogoBmpKind == LOGO_FAVARITE )
		{
			m_objRefBtn.EnableWindow(TRUE);
			m_objBmpFilePathEdit.EnableWindow(TRUE);
		}

		//ロゴ表示区分　有効／無効設定
		GetDlgItem(IDC_RADIO_TILE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_POSSITEI)->EnableWindow(TRUE);
		if( m_nLogoPosKind == LOGO_POS_TILE )
		{
			m_objLogoPosXEdit.EnableWindow(FALSE);
			m_objLogoPosYEdit.EnableWindow(FALSE);
		}
		else if( m_nLogoPosKind == LOGO_POS_SITEI )
		{
			m_objLogoPosXEdit.EnableWindow(TRUE);
			m_objLogoPosYEdit.EnableWindow(TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_RADIO_XERXES)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FAVARITE)->EnableWindow(FALSE);
		m_objRefBtn.EnableWindow(FALSE);
		m_objBmpFilePathEdit.EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_POSSITEI)->EnableWindow(FALSE);
		m_objLogoPosXEdit.EnableWindow(FALSE);
		m_objLogoPosYEdit.EnableWindow(FALSE);
	}

	//ＢＭＰファイルパスエディット初期化
	m_objBmpFilePathEdit.SetWindowText(m_objBmpFilePath);
	//ロゴ位置エディット初期化
	char	szTemp[32];
	sprintf(szTemp,"%d",m_unLogoPosX);
	m_objLogoPosXEdit.SetWindowText(szTemp);
	sprintf(szTemp,"%d",m_unLogoPosY);
	m_objLogoPosYEdit.SetWindowText(szTemp);

	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ロゴを表示」選択時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnCheckLogodisp() 
{
	m_bDispLogoBmp = (m_bDispLogoBmp!=FALSE) ? FALSE : TRUE;

	m_bIsChanged = TRUE;
	SetModified();

	//ロゴ位置エディット有効／無効設定
	if( m_bDispLogoBmp )
	{
		//ロゴビットマップ区分　有効／無効設定
		GetDlgItem(IDC_RADIO_XERXES)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_FAVARITE)->EnableWindow(TRUE);
		if( m_nLogoBmpKind == LOGO_XERXES )
		{
			m_objRefBtn.EnableWindow(FALSE);
			m_objBmpFilePathEdit.EnableWindow(FALSE);
		}
		else if( m_nLogoBmpKind == LOGO_FAVARITE )
		{
			m_objRefBtn.EnableWindow(TRUE);
			m_objBmpFilePathEdit.EnableWindow(TRUE);
		}

		//ロゴ表示区分　有効／無効設定
		GetDlgItem(IDC_RADIO_TILE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_POSSITEI)->EnableWindow(TRUE);
		if( m_nLogoPosKind == LOGO_POS_TILE )
		{
			m_objLogoPosXEdit.EnableWindow(FALSE);
			m_objLogoPosYEdit.EnableWindow(FALSE);
		}
		else if( m_nLogoPosKind == LOGO_POS_SITEI )
		{
			m_objLogoPosXEdit.EnableWindow(TRUE);
			m_objLogoPosYEdit.EnableWindow(TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_RADIO_XERXES)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FAVARITE)->EnableWindow(FALSE);
		m_objRefBtn.EnableWindow(FALSE);
		m_objBmpFilePathEdit.EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_POSSITEI)->EnableWindow(FALSE);
		m_objLogoPosXEdit.EnableWindow(FALSE);
		m_objLogoPosYEdit.EnableWindow(FALSE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デフォルトのXerxesロゴ選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnRadioXerxesLogo() 
{
	if(m_nLogoBmpKind != LOGO_XERXES)
	{
		m_nLogoBmpKind = LOGO_XERXES;
		m_objRefBtn.EnableWindow(FALSE);
		m_objBmpFilePathEdit.EnableWindow(FALSE);

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：好きなＢＭＰロゴ選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnRadioFavariteLogo() 
{
	if(m_nLogoBmpKind != LOGO_FAVARITE)
	{
		m_nLogoBmpKind = LOGO_FAVARITE;
		m_objRefBtn.EnableWindow(TRUE);
		m_objBmpFilePathEdit.EnableWindow(TRUE);

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＢＭＰファイルパスエディットボックス変更時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnChangeEditBmpFilePath() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：参照ボタン押下時処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnRef() 
{
	CString	objFilter = _T("BMP file (*.bmp)|*.bmp|");
	DWORD dwFlags = ( OFN_HIDEREADONLY     | 
					  OFN_OVERWRITEPROMPT  |
					  OFN_LONGNAMES        |
					  OFN_FILEMUSTEXIST );
	CFileDialog	objFileDialog(	TRUE,				//OPEN
								NULL,				//デフォルトファイル拡張子
								NULL,				//デフォルトファイル名
								dwFlags,			//オープンフラグ
								objFilter,			//フィルタ
								NULL );				//親ウインドウハンドル
	if( objFileDialog.DoModal() == IDOK )
	{
		m_objBmpFilePathEdit.SetWindowText(objFileDialog.GetPathName());
		m_objBmpFilePathEdit.UpdateData(TRUE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ロゴをタイル表示する選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnRadioTile() 
{
	if(m_nLogoPosKind != LOGO_POS_TILE)
	{
		m_nLogoPosKind = LOGO_POS_TILE;
		m_objLogoPosXEdit.EnableWindow(FALSE);
		m_objLogoPosYEdit.EnableWindow(FALSE);

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ロゴ表示位置を指定する選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnRadioPossitei() 
{
	if(m_nLogoPosKind != LOGO_POS_SITEI)
	{
		m_nLogoPosKind = LOGO_POS_SITEI;
		m_objLogoPosXEdit.EnableWindow(TRUE);
		m_objLogoPosYEdit.EnableWindow(TRUE);

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ロゴ位置Ｘエディットボックス変更時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnChangeEditLogoposx() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ロゴ位置Ｙエディットボックス変更時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnChangeEditLogoposy() 
{
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
void CMyPropertyPage7::OnOK() 
{
	CString		objStr=_T("");
	char*		pPtr=NULL;
	CString		objMsg;

	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	//ＢＭＰファイルパス取得
	m_objBmpFilePathEdit.GetWindowText(m_objBmpFilePath);

	//ロゴ位置Ｘの妥当性チェック
	m_objLogoPosXEdit.GetWindowText(objStr);
	m_unLogoPosX = (UINT)atoi((char*)(LPCTSTR)objStr);
	if(m_unLogoPosX < 0 || m_unLogoPosX > 100)
	{
		objMsg.LoadString(IDS_LOGOX_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONEXCLAMATION),0);
	}

	//ロゴ位置Ｙの妥当性チェック
	m_objLogoPosYEdit.GetWindowText(objStr);
	m_unLogoPosY = (UINT)atoi((char*)(LPCTSTR)objStr);
	if(m_unLogoPosY < 0 || m_unLogoPosY > 100)
	{
		objMsg.LoadString(IDS_LOGOY_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONEXCLAMATION),0);
	}

	CPropertyPage::OnOK();
}


// MyPropertyPage2.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "Editor_Def.h"
#include "MyPropertyPage2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMyPropertyPage2, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage2 プロパティ ページ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage2::CMyPropertyPage2() : CPropertyPage(CMyPropertyPage2::IDD)
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsChanged = FALSE;								// 変更発生フラグ
	m_objFileExt = _T("");

	//{{AFX_DATA_INIT(CMyPropertyPage2)
	m_nTabSize = -1;
	//}}AFX_DATA_INIT
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage2::~CMyPropertyPage2()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：変更発生調査処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage2::IsChanged() 
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
void CMyPropertyPage2::ClearChangeFlag()
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
void CMyPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage2)
	DDX_Control(pDX, IDC_COMBO_FILEEXT, m_objFileExtCombo);
	DDX_Control(pDX, IDC_COMBO_DISPFONTNAME, m_objDispFontNameCombo);
	DDX_Control(pDX, IDC_COMBO_DISPFONTSIZE, m_objDispFontSizeCombo);
	DDX_Control(pDX, IDC_COMBO_PRINTFONTNAME, m_objPrintFontNameCombo);
	DDX_Control(pDX, IDC_COMBO_PRINTFONTSIZE, m_objPrintFontSizeCombo);
	DDX_Radio(pDX, IDC_RADIO_2TAB, m_nTabSize);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyPropertyPage2, CPropertyPage)
	//{{AFX_MSG_MAP(CMyPropertyPage2)
	ON_BN_CLICKED(IDC_BUTTON_STSBARFONT, OnButtonStsbarfont)
	ON_BN_CLICKED(IDC_RADIO_4TAB, OnRadio4tab)
	ON_BN_CLICKED(IDC_RADIO_8TAB, OnRadio8tab)
	ON_BN_CLICKED(IDC_RADIO_2TAB, OnRadio2tab)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPFONTNAME, OnSelchangeComboDispfontname)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPFONTSIZE, OnSelchangeComboDispfontsize)
	ON_CBN_SELCHANGE(IDC_COMBO_FILEEXT, OnSelchangeComboFileext)
	ON_CBN_SELCHANGE(IDC_COMBO_PRINTFONTNAME, OnSelchangeComboPrintfontname)
	ON_CBN_SELCHANGE(IDC_COMBO_PRINTFONTSIZE, OnSelchangeComboPrintfontsize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage2 メッセージハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage2::OnInitDialog() 
{
	int nCnt=0,nIndex=0;
	char szBuf[32];
	CString objKey, objMsg;
	char *Size[] = {"8","9","10","11","12","14","16","18","20","22","24","26","28","36","48" };

	CPropertyPage::OnInitDialog();

	//ファイル拡張子コンボボックス設定
	m_objFileExtCombo.AddString("none");
	m_objFileExtCombo.AddString("txt");
	m_objFileExtCombo.AddString("htm html shtml shtm");
	m_objFileExtCombo.AddString("c cpp");
	m_objFileExtCombo.AddString("h");
	m_objFileExtCombo.AddString("asm");
	m_objFileExtCombo.AddString("asp");
	m_objFileExtCombo.AddString("frm");
	m_objFileExtCombo.AddString("bas");
	m_objFileExtCombo.AddString("pc");
	m_objFileExtCombo.AddString("log");
	m_objFileExtCombo.AddString("dat");
	m_objFileExtCombo.AddString("bat");
	m_objFileExtCombo.AddString("ini");
	m_objFileExtCombo.AddString("mak mk");
	m_objFileExtCombo.AddString("java");
	m_objFileExtCombo.AddString("y l");
	m_objFileExtCombo.AddString("wrl");
	m_objFileExtCombo.AddString("other");

	m_objFileExt.MakeLower();
	if(m_objFileExt.IsEmpty())
	{
		objKey = "FileExt_none";
		m_objFileExtCombo.SetCurSel(0);
	}
	else if(!m_objFileExt.CompareNoCase("txt"))
	{
		objKey = "FileExt_txt";
		m_objFileExtCombo.SetCurSel(1);
	}
	else if(!m_objFileExt.CompareNoCase("htm")   || !m_objFileExt.CompareNoCase("html") ||
			!m_objFileExt.CompareNoCase("shtml") || !m_objFileExt.CompareNoCase("shtm")
	)
	{
		objKey = "FileExt_htm html shtml shtm";
		m_objFileExtCombo.SetCurSel(2);
	}
	else if(!m_objFileExt.CompareNoCase("c") || !m_objFileExt.CompareNoCase("cpp"))
	{
		objKey = "FileExt_c cpp";
		m_objFileExtCombo.SetCurSel(3);
	}
	else if(!m_objFileExt.CompareNoCase("h"))
	{
		objKey = "FileExt_h";
		m_objFileExtCombo.SetCurSel(4);
	}
	else if(!m_objFileExt.CompareNoCase("asm"))
	{
		objKey = "FileExt_asm";
		m_objFileExtCombo.SetCurSel(5);
	}
	else if(!m_objFileExt.CompareNoCase("asp"))
	{
		objKey = "FileExt_asp";
		m_objFileExtCombo.SetCurSel(6);
	}
	else if(!m_objFileExt.CompareNoCase("frm"))
	{
		objKey = "FileExt_frm";
		m_objFileExtCombo.SetCurSel(7);
	}
	else if(!m_objFileExt.CompareNoCase("bas"))
	{
		objKey = "FileExt_bas";
		m_objFileExtCombo.SetCurSel(8);
	}
	else if(!m_objFileExt.CompareNoCase("pc"))
	{
		objKey = "FileExt_pc";
		m_objFileExtCombo.SetCurSel(9);
	}
	else if(!m_objFileExt.CompareNoCase("log"))
	{
		objKey = "FileExt_log";
		m_objFileExtCombo.SetCurSel(10);
	}
	else if(!m_objFileExt.CompareNoCase("dat"))
	{
		objKey = "FileExt_dat";
		m_objFileExtCombo.SetCurSel(11);
	}
	else if(!m_objFileExt.CompareNoCase("bat"))
	{
		objKey = "FileExt_bat";
		m_objFileExtCombo.SetCurSel(12);
	}
	else if(!m_objFileExt.CompareNoCase("ini"))
	{
		objKey = "FileExt_ini";
		m_objFileExtCombo.SetCurSel(13);
	}
	else if(!m_objFileExt.CompareNoCase("mak") || !m_objFileExt.CompareNoCase("mk"))
	{
		objKey = "FileExt_mak mk";
		m_objFileExtCombo.SetCurSel(14);
	}
	else if(!m_objFileExt.CompareNoCase("java"))
	{
		objKey = "FileExt_java";
		m_objFileExtCombo.SetCurSel(15);
	}
	else if(!m_objFileExt.CompareNoCase("y") || !m_objFileExt.CompareNoCase("l"))
	{
		objKey = "FileExt_y l";
		m_objFileExtCombo.SetCurSel(16);
	}
	else if(!m_objFileExt.CompareNoCase("wrl"))
	{
		objKey = "FileExt_wrl";
		m_objFileExtCombo.SetCurSel(17);
	}
	else
	{
		objKey = "FileExt_other";
		m_objFileExtCombo.SetCurSel(18);
	}

	//ファイル拡張子に対応する設定取得
	m_lDispFontPoint  = m_pApp->GetProfileInt(objKey,"DispFontPoint",120);
	objMsg.LoadString(IDS_FONTNAME_DISP);
	m_objDispFontName = m_pApp->GetProfileString(objKey,"DispFontName",objMsg);
	m_lPrintFontPoint = m_pApp->GetProfileInt(objKey,"PrintFontPoint",80);
	objMsg.LoadString(IDS_FONTNAME_PRINT);
	m_objPrintFontName= m_pApp->GetProfileString(objKey,"PrintFontName",objMsg);
	//ファイル拡張子に対応する設定保存
	m_pApp->WriteProfileInt(objKey,"DispFontPoint",m_lDispFontPoint);
	m_pApp->WriteProfileString(objKey,"DispFontName",m_objDispFontName);
	m_pApp->WriteProfileInt(objKey,"PrintFontPoint",m_lPrintFontPoint);
	m_pApp->WriteProfileString(objKey,"PrintFontName",m_objPrintFontName);

	//フォント名称コンボボックス設定
	objMsg.LoadString(IDS_FONTNAME_DISP);
	if(!objMsg.IsEmpty())
		m_objDispFontNameCombo.AddString(objMsg);
	objMsg.LoadString(IDS_FONTNAME_DISP2);
	if(!objMsg.IsEmpty())
		m_objDispFontNameCombo.AddString(objMsg);
	objMsg.LoadString(IDS_FONTNAME_DISP3);
	if(!objMsg.IsEmpty())
		m_objDispFontNameCombo.AddString(objMsg);
	objMsg.LoadString(IDS_FONTNAME_DISP4);
	if(!objMsg.IsEmpty())
		m_objDispFontNameCombo.AddString(objMsg);

	objMsg.LoadString(IDS_FONTNAME_PRINT);
	if(!objMsg.IsEmpty())
		m_objPrintFontNameCombo.AddString(objMsg);
	objMsg.LoadString(IDS_FONTNAME_PRINT2);
	if(!objMsg.IsEmpty())
		m_objPrintFontNameCombo.AddString(objMsg);
	objMsg.LoadString(IDS_FONTNAME_PRINT3);
	if(!objMsg.IsEmpty())
		m_objPrintFontNameCombo.AddString(objMsg);
	objMsg.LoadString(IDS_FONTNAME_PRINT4);
	if(!objMsg.IsEmpty())
		m_objPrintFontNameCombo.AddString(objMsg);

//	CClientDC dc(NULL);
//	LOGFONT logFont;
//	memset(&logFont,0,sizeof(LOGFONT));
//	logFont.lfCharSet = SHIFTJIS_CHARSET;
//	strcpy(logFont.lfFaceName,"");
//	logFont.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
//	::EnumFontFamiliesEx( dc.GetSafeHdc(),
//						  &logFont,
//						 (FONTENUMPROC)EnumFontFamiliesCallBack,
//						 (LPARAM)this,0);

	//フォントサイズコンボボックス設定
	for(nCnt=0; nCnt<15; nCnt++)
	{
		m_objDispFontSizeCombo.AddString(Size[nCnt]);
		m_objPrintFontSizeCombo.AddString(Size[nCnt]);
	}

	//フォント名称コンボボックス選択
	nIndex = m_objDispFontNameCombo.FindStringExact(-1,(LPCTSTR)m_objDispFontName);
	if(nIndex != CB_ERR)
		m_objDispFontNameCombo.SetCurSel(nIndex);
	nIndex = m_objPrintFontNameCombo.FindStringExact(-1,(LPCTSTR)m_objPrintFontName);
	if(nIndex != CB_ERR)
		m_objPrintFontNameCombo.SetCurSel(nIndex);
	//フォントサイズコンボボックス選択
	sprintf(szBuf,"%d",m_lDispFontPoint/10);
	nIndex = m_objDispFontSizeCombo.FindStringExact(-1,szBuf);
	if(nIndex != CB_ERR)
		m_objDispFontSizeCombo.SetCurSel(nIndex);
	sprintf(szBuf,"%d",m_lPrintFontPoint/10);
	nIndex = m_objPrintFontSizeCombo.FindStringExact(-1,szBuf);
	if(nIndex != CB_ERR)
		m_objPrintFontSizeCombo.SetCurSel(nIndex);

	//メンバ変数－＞コントロール
	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル拡張子コンボボックス選択変更時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage2::OnSelchangeComboFileext() 
{
	CString objMsg;
	char szBuf[32];
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//ファイル拡張子コンボボックス文字列取得
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//ファイル拡張子に対応する設定取得
	m_lDispFontPoint  = m_pApp->GetProfileInt(objStr,"DispFontPoint",120);
	objMsg.LoadString(IDS_FONTNAME_DISP);
	m_objDispFontName = m_pApp->GetProfileString(objStr,"DispFontName",objMsg);
	m_lPrintFontPoint = m_pApp->GetProfileInt(objStr,"PrintFontPoint",80);
	objMsg.LoadString(IDS_FONTNAME_PRINT);
	m_objPrintFontName= m_pApp->GetProfileString(objStr,"PrintFontName",objMsg);

	//フォント名称コンボボックス選択
	nIndex = m_objDispFontNameCombo.FindStringExact(-1,(LPCTSTR)m_objDispFontName);
	if(nIndex != CB_ERR)
		m_objDispFontNameCombo.SetCurSel(nIndex);
	nIndex = m_objPrintFontNameCombo.FindStringExact(-1,(LPCTSTR)m_objPrintFontName);
	if(nIndex != CB_ERR)
		m_objPrintFontNameCombo.SetCurSel(nIndex);
	//フォントサイズコンボボックス選択
	sprintf(szBuf,"%d",m_lDispFontPoint/10);
	nIndex = m_objDispFontSizeCombo.FindStringExact(-1,szBuf);
	if(nIndex != CB_ERR)
		m_objDispFontSizeCombo.SetCurSel(nIndex);
	sprintf(szBuf,"%d",m_lPrintFontPoint/10);
	nIndex = m_objPrintFontSizeCombo.FindStringExact(-1,szBuf);
	if(nIndex != CB_ERR)
		m_objPrintFontSizeCombo.SetCurSel(nIndex);

	//メンバ変数－＞コントロール
	UpdateData(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：表示フォント名称コンボボックス選択変更時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage2::OnSelchangeComboDispfontname() 
{
	int nCnt=0;
	char *Size[] = {"8","9","10","11","12","14","16","18","20","22","24","26","28","36","48" };
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//ファイル拡張子コンボボックス文字列取得
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//表示フォント名称取得
	nIndex = m_objDispFontNameCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;
	m_objDispFontNameCombo.GetLBText(nIndex,objStr2);
	if(m_objDispFontName.Compare(objStr2))
	{
		m_objDispFontName = objStr2;

		//設定保存
		m_pApp->WriteProfileString(objStr,"DispFontName",m_objDispFontName);

		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：表示フォントサイズコンボボックス選択変更時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage2::OnSelchangeComboDispfontsize() 
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//ファイル拡張子コンボボックス文字列取得
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//表示フォントサイズ取得
	nIndex = m_objDispFontSizeCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;
	m_objDispFontSizeCombo.GetLBText(nIndex,objStr2);
	int nSize = atoi((LPCTSTR)objStr2);
	if(m_lDispFontPoint != nSize*10)
	{
		m_lDispFontPoint = nSize*10;

		//設定保存
		m_pApp->WriteProfileInt(objStr,"DispFontPoint",m_lDispFontPoint);

		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷フォント名称コンボボックス選択変更時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage2::OnSelchangeComboPrintfontname() 
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//ファイル拡張子コンボボックス文字列取得
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//印刷フォント名称取得
	nIndex = m_objPrintFontNameCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;
	m_objPrintFontNameCombo.GetLBText(nIndex,objStr2);
	if(m_objPrintFontName.Compare(objStr2))
	{
		m_objPrintFontName = objStr2;

		//設定保存
		m_pApp->WriteProfileString(objStr,"PrintFontName",m_objPrintFontName);

		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷フォントサイズコンボボックス選択変更時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage2::OnSelchangeComboPrintfontsize() 
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//ファイル拡張子コンボボックス文字列取得
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//印刷フォントサイズ取得
	nIndex = m_objPrintFontSizeCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;
	m_objPrintFontSizeCombo.GetLBText(nIndex,objStr2);
	int nSize = atoi((LPCTSTR)objStr2);
	if(m_lPrintFontPoint != nSize*10)
	{
		m_lPrintFontPoint = nSize*10;

		//設定保存
		m_pApp->WriteProfileInt(objStr,"PrintFontPoint",m_lPrintFontPoint);

		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスバーフォント設定ボタン押下時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage2::OnButtonStsbarfont() 
{
	LOGFONT	stOrgStatusBarLogFont;
	memcpy(&stOrgStatusBarLogFont,&m_stStatusBarLogFont,sizeof(LOGFONT));

	CFontDialog	objFontDlg(&m_stStatusBarLogFont,CF_SCREENFONTS);
	int nRet = objFontDlg.DoModal();
	if(nRet == IDOK)
	{
		if(	memcmp(&stOrgStatusBarLogFont,&m_stStatusBarLogFont,sizeof(LOGFONT)) )
		{
			//変更された
			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：２タブ選択時処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage2::OnRadio2tab() 
{
	if(m_nTabSize != 0)
	{
		m_nTabSize = 0;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：４タブ選択時処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage2::OnRadio4tab() 
{
	if(m_nTabSize != 1)
	{
		m_nTabSize = 1;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：８タブ選択時処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage2::OnRadio8tab() 
{
	if(m_nTabSize != 2)
	{
		m_nTabSize = 2;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage2 コールバック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：フォント列挙用コールバック											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CALLBACK EnumFontFamiliesCallBack
	(	ENUMLOGFONT*	lpLf,
		NEWTEXTMETRIC*	lpMetric,
		int 			FontType, 
		LPARAM 			lParam
	)
{
	CMyPropertyPage2 *pPage = (CMyPropertyPage2*)lParam;
	ASSERT(pPage);

	if(	FontType == TRUETYPE_FONTTYPE &&					//TrueType
		lpLf->elfLogFont.lfCharSet == SHIFTJIS_CHARSET &&   //日本語フォント
		lpLf->elfLogFont.lfFaceName[0] != '@'				//横書きフォント
	)
	{
		pPage->m_objDispFontNameCombo.AddString(lpLf->elfLogFont.lfFaceName);
		pPage->m_objPrintFontNameCombo.AddString(lpLf->elfLogFont.lfFaceName);
	}

	return 1;
}



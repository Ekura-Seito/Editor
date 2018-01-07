// MyPropertyPage6.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "MyPropertyPage6.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMyPropertyPage6, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage6 プロパティ ページ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage6::CMyPropertyPage6() : CPropertyPage(CMyPropertyPage6::IDD)
{
	m_bIsChanged = FALSE;								// 変更発生フラグ

	//{{AFX_DATA_INIT(CMyPropertyPage6)
	m_nCaretType = -1;
	m_unCaretBlinkTime = 0;
	m_nCaretBehave = -1;
	m_nCaretMoveType = -1;
	m_nMultValueOfCaretAccelarete = -1;
	m_nSetCaretToEndOnPaste = -1;
	//}}AFX_DATA_INIT
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage6::~CMyPropertyPage6()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage6 パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：変更発生調査処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage6::IsChanged() 
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
void CMyPropertyPage6::ClearChangeFlag()
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
void CMyPropertyPage6::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage6)
	DDX_Radio(pDX, IDC_RADIO_CARET_IBEAM, m_nCaretType);
	DDX_Text(pDX, IDC_EDIT_BLINKTIME, m_unCaretBlinkTime);
	DDV_MinMaxUInt(pDX, m_unCaretBlinkTime, 200, 800);
	DDX_Radio(pDX, IDC_RADIO_CARETMOVE, m_nCaretBehave);
	DDX_Radio(pDX, IDC_RADIO_WORDTYPE, m_nCaretMoveType);
	DDX_Radio(pDX, IDC_RADIO_ACCEL_1, m_nMultValueOfCaretAccelarete);
	DDX_Radio(pDX, IDC_RADIO_PASTETOP, m_nSetCaretToEndOnPaste);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyPropertyPage6, CPropertyPage)
	//{{AFX_MSG_MAP(CMyPropertyPage6)
	ON_BN_CLICKED(IDC_RADIO_CARET_IBEAM, OnRadioCaretIbeam)
	ON_BN_CLICKED(IDC_RADIO_CARET_SQUARE, OnRadioCaretSquare)
	ON_EN_CHANGE(IDC_EDIT_BLINKTIME, OnChangeEditBlinktime)
	ON_BN_CLICKED(IDC_RADIO_CARETMOVE, OnRadioCaretmove)
	ON_BN_CLICKED(IDC_RADIO_CARETSTAY, OnRadioCaretstay)
	ON_BN_CLICKED(IDC_RADIO_WORDTYPE, OnRadioWordtype)
	ON_BN_CLICKED(IDC_RADIO_NOTEPADTYPE, OnRadioNotepadtype)
	ON_BN_CLICKED(IDC_RADIO_ACCEL_1, OnRadioAccel1)
	ON_BN_CLICKED(IDC_RADIO_ACCEL_2, OnRadioAccel2)
	ON_BN_CLICKED(IDC_RADIO_ACCEL_3, OnRadioAccel3)
	ON_BN_CLICKED(IDC_RADIO_ACCEL_4, OnRadioAccel4)
	ON_BN_CLICKED(IDC_RADIO_ACCEL_5, OnRadioAccel5)
	ON_BN_CLICKED(IDC_RADIO_PASTETOP, OnRadioPastetop)
	ON_BN_CLICKED(IDC_RADIO_PASTEEND, OnRadioPasteend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage6 メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage6::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	//フォント設定
	CWnd* pWnd = GetDlgItem(IDC_EDIT_BLINKTIME);
	pWnd->SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット形状：縦棒選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioCaretIbeam() 
{
	if(m_nCaretType != 0)
	{
		m_nCaretType = 0;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット形状：箱型選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioCaretSquare() 
{
	if(m_nCaretType != 1)
	{
		m_nCaretType = 1;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット点滅間隔エディットボックス変更時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnChangeEditBlinktime() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：スクロール時のキャレット動作：「行位置を変更」選択時処理				|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioCaretmove() 
{
	if(m_nCaretBehave != 0)
	{
		m_nCaretBehave = 0;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：スクロール時のキャレット動作：「行位置を保持」選択時処理				|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioCaretstay() 
{
	if(m_nCaretBehave != 1)
	{
		m_nCaretBehave = 1;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット移動時の動作：「ワープロ風」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioWordtype() 
{
	if(m_nCaretMoveType != 0)
	{
		m_nCaretMoveType = 0;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット移動時の動作：「メモ帳風」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioNotepadtype() 
{
	if(m_nCaretMoveType != 1)
	{
		m_nCaretMoveType = 1;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット移動時の加速：「１倍速」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioAccel1() 
{
	if(m_nMultValueOfCaretAccelarete != 0)
	{
		m_nMultValueOfCaretAccelarete = 0;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット移動時の加速：「２倍速」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioAccel2() 
{
	if(m_nMultValueOfCaretAccelarete != 1)
	{
		m_nMultValueOfCaretAccelarete = 1;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット移動時の加速：「３倍速」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioAccel3() 
{
	if(m_nMultValueOfCaretAccelarete != 2)
	{
		m_nMultValueOfCaretAccelarete = 2;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット移動時の加速：「４倍速」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioAccel4() 
{
	if(m_nMultValueOfCaretAccelarete != 3)
	{
		m_nMultValueOfCaretAccelarete = 3;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット移動時の加速：「５倍速」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioAccel5() 
{
	if(m_nMultValueOfCaretAccelarete != 4)
	{
		m_nMultValueOfCaretAccelarete = 4;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：貼り付け後のキャレット位置：「先頭」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioPastetop() 
{
	if(m_nSetCaretToEndOnPaste != 0)
	{
		m_nSetCaretToEndOnPaste = 0;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：貼り付け後のキャレット位置：「最後」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioPasteend() 
{
	if(m_nSetCaretToEndOnPaste != 1)
	{
		m_nSetCaretToEndOnPaste = 1;

		//変更された
		m_bIsChanged = TRUE;
		SetModified();
	}
}


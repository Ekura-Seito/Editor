// MyPropertyPage8.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "StatusBarDisp.h"				//ステータスバー表示情報構造体
#include "MyPropertyPage8.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMyPropertyPage8, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage8 プロパティ ページ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage8::CMyPropertyPage8() : CPropertyPage(CMyPropertyPage8::IDD)
{
	m_bIsChanged = FALSE;								// 変更発生フラグ
	memset(&m_stStatusBarDisp,0,sizeof(STATUSBARDISP));	// ステータスバー表示情報構造体
	m_lStatusDayStyle = 0;			// ステータスバーへ表示する日付表示スタイル
	m_lStatusHourStyle = 0;			// ステータスバーへ表示する時刻表示スタイル
	m_lStatusLineStyle = 0;			// ステータスバーへ表示する行、列表示スタイル

	//{{AFX_DATA_INIT(CMyPropertyPage8)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage8::~CMyPropertyPage8()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage8 パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：変更発生調査処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage8::IsChanged() 
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
void CMyPropertyPage8::ClearChangeFlag()
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
void CMyPropertyPage8::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage8)
	DDX_Control(pDX, IDC_COMBO_LINESTYLE, m_objLineStyleCombo);
	DDX_Control(pDX, IDC_COMBO_HOURSTYLE, m_objHourStyleCombo);
	DDX_Control(pDX, IDC_COMBO_DAYSTYLE, m_objDayStyleCombo);
	DDX_Control(pDX, IDC_LIST_CURRENTITEM, m_objCurrentItemListBox);
	DDX_Control(pDX, IDC_LIST_ADDITEM, m_objAddItemListBox);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_objDelButton);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_objAddButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPropertyPage8, CDialog)
	//{{AFX_MSG_MAP(CMyPropertyPage8)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_CBN_SELCHANGE(IDC_COMBO_DAYSTYLE, OnSelchangeComboDaystyle)
	ON_CBN_SELCHANGE(IDC_COMBO_HOURSTYLE, OnSelchangeComboHourstyle)
	ON_CBN_SELCHANGE(IDC_COMBO_LINESTYLE, OnSelchangeComboLinestyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage8 メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ初期化処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage8::OnInitDialog() 
{
	int	nCnt=0;
	BOOL bFind=FALSE;
	CString objMsg;

	CPropertyPage::OnInitDialog();

	//現在の表示項目リストボックス
	for(nCnt=0; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		if(m_stStatusBarDisp.m_nCurrentItems[nCnt] == ID_INDICATOR_GYORETU)
			objMsg.LoadString(IDS_INDICATOR_GYORETU);
		else if(m_stStatusBarDisp.m_nCurrentItems[nCnt] == ID_INDICATOR_BYTE)
			objMsg.LoadString(IDS_INDICATOR_BYTE);
		else if(m_stStatusBarDisp.m_nCurrentItems[nCnt] == ID_INDICATOR_INS)
			objMsg.LoadString(IDS_INDICATOR_INS);
		else if(m_stStatusBarDisp.m_nCurrentItems[nCnt] == ID_INDICATOR_READONLY)
			objMsg.LoadString(IDS_INDICATOR_READONLY);
		else if(m_stStatusBarDisp.m_nCurrentItems[nCnt] == ID_INDICATOR_TIME)
			objMsg.LoadString(IDS_INDICATOR_TIME);
		else if(m_stStatusBarDisp.m_nCurrentItems[nCnt] == ID_INDICATOR_CHARCODE)
			objMsg.LoadString(IDS_INDICATOR_CHARCODE);
		else if(m_stStatusBarDisp.m_nCurrentItems[nCnt] == ID_INDICATOR_KANA)
			objMsg.LoadString(IDS_INDICATOR_KANA);
		else if(m_stStatusBarDisp.m_nCurrentItems[nCnt] == ID_INDICATOR_CAPS)
			objMsg.LoadString(IDS_INDICATOR_CAPS);
		else if(m_stStatusBarDisp.m_nCurrentItems[nCnt] == ID_INDICATOR_NUM)
			objMsg.LoadString(IDS_INDICATOR_NUM);
		else if(m_stStatusBarDisp.m_nCurrentItems[nCnt] == ID_INDICATOR_SCRL)
			objMsg.LoadString(IDS_INDICATOR_SCRL);
		else 
			continue;

		m_objCurrentItemListBox.AddString(objMsg);
	}

	//追加項目リストボックス
	for(nCnt=0; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		if(m_stStatusBarDisp.m_nAddItems[nCnt] == ID_INDICATOR_GYORETU)
			objMsg.LoadString(IDS_INDICATOR_GYORETU);
		else if(m_stStatusBarDisp.m_nAddItems[nCnt] == ID_INDICATOR_BYTE)
			objMsg.LoadString(IDS_INDICATOR_BYTE);
		else if(m_stStatusBarDisp.m_nAddItems[nCnt] == ID_INDICATOR_INS)
			objMsg.LoadString(IDS_INDICATOR_INS);
		else if(m_stStatusBarDisp.m_nAddItems[nCnt] == ID_INDICATOR_READONLY)
			objMsg.LoadString(IDS_INDICATOR_READONLY);
		else if(m_stStatusBarDisp.m_nAddItems[nCnt] == ID_INDICATOR_TIME)
			objMsg.LoadString(IDS_INDICATOR_TIME);
		else if(m_stStatusBarDisp.m_nAddItems[nCnt] == ID_INDICATOR_CHARCODE)
			objMsg.LoadString(IDS_INDICATOR_CHARCODE);
		else if(m_stStatusBarDisp.m_nAddItems[nCnt] == ID_INDICATOR_KANA)
			objMsg.LoadString(IDS_INDICATOR_KANA);
		else if(m_stStatusBarDisp.m_nAddItems[nCnt] == ID_INDICATOR_CAPS)
			objMsg.LoadString(IDS_INDICATOR_CAPS);
		else if(m_stStatusBarDisp.m_nAddItems[nCnt] == ID_INDICATOR_NUM)
			objMsg.LoadString(IDS_INDICATOR_NUM);
		else if(m_stStatusBarDisp.m_nAddItems[nCnt] == ID_INDICATOR_SCRL)
			objMsg.LoadString(IDS_INDICATOR_SCRL);
		else 
			continue;

		m_objAddItemListBox.AddString(objMsg);
	}

	//「追加」、「削除」ボタン有効化設定
	if(m_objCurrentItemListBox.GetCount() == 0)
		m_objDelButton.EnableWindow(FALSE);
	else 
		m_objDelButton.EnableWindow(TRUE);

	if(m_objAddItemListBox.GetCount() == 0)
		m_objAddButton.EnableWindow(FALSE);
	else
		m_objAddButton.EnableWindow(TRUE);

	// 日付表示スタイルのコンボボックス
	m_objDayStyleCombo.AddString("None");
	m_objDayStyleCombo.AddString("YYYY/MM/DD");
	m_objDayStyleCombo.AddString("'YY/MM/DD");
	m_objDayStyleCombo.AddString("MM/DD/YYYY");
	m_objDayStyleCombo.AddString("MM/DD/'YY");
	m_objDayStyleCombo.AddString("MM/DD");
	m_objDayStyleCombo.AddString("DD/MM/YYYY");
	m_objDayStyleCombo.AddString("DD/MM/'YY");
	m_objDayStyleCombo.AddString("DD/MM");
	objMsg.LoadString(IDS_DAYSTYLE_YYYY_MM_DD2);
	m_objDayStyleCombo.AddString(objMsg);
	objMsg.LoadString(IDS_DAYSTYLE_YY_MM_DD2);
	m_objDayStyleCombo.AddString(objMsg);
	objMsg.LoadString(IDS_DAYSTYLE_MM_DD2);
	m_objDayStyleCombo.AddString(objMsg);
	if(m_lStatusDayStyle >= 0 && m_lStatusDayStyle < m_objDayStyleCombo.GetCount() )
		m_objDayStyleCombo.SetCurSel(m_lStatusDayStyle);
	bFind=FALSE;
	for(nCnt=0; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		if(m_stStatusBarDisp.m_nCurrentItems[nCnt] == ID_INDICATOR_TIME)
		{
			bFind=TRUE;
			break;
		}
	}
	if(bFind != FALSE)
		m_objDayStyleCombo.EnableWindow(TRUE);
	else
		m_objDayStyleCombo.EnableWindow(FALSE);

	//時刻表示スタイルのコンボボックス
	m_objHourStyleCombo.AddString("None");
	m_objHourStyleCombo.AddString("HH:MM");
	m_objHourStyleCombo.AddString("AM/PM HH:MM");
	objMsg.LoadString(IDS_HOURSTYLE_HH_MM2);
	m_objHourStyleCombo.AddString(objMsg);
	objMsg.LoadString(IDS_HOURSTYLE_AMPM_HH_MM2);
	m_objHourStyleCombo.AddString(objMsg);
	if( m_lStatusHourStyle >= 0 && m_lStatusHourStyle < m_objHourStyleCombo.GetCount() )
		m_objHourStyleCombo.SetCurSel(m_lStatusHourStyle);
	bFind=FALSE;
	for(nCnt=0; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		if(m_stStatusBarDisp.m_nCurrentItems[nCnt] == ID_INDICATOR_TIME)
		{
			bFind=TRUE;
			break;
		}
	}
	if(bFind != FALSE)
		m_objHourStyleCombo.EnableWindow(TRUE);
	else
		m_objHourStyleCombo.EnableWindow(FALSE);

	//行、列表示スタイルのコンボボックス
	objMsg.LoadString(IDS_LINESTYLE_LINE_OFFS_ALL2);
	m_objLineStyleCombo.AddString(objMsg);
	objMsg.LoadString(IDS_LINESTYLE_LINE_OFFS2);
	m_objLineStyleCombo.AddString(objMsg);
	objMsg.LoadString(IDS_LINESTYLE_LINE_ALL2);
	m_objLineStyleCombo.AddString(objMsg);
	objMsg.LoadString(IDS_LINESTYLE_ALL2);
	m_objLineStyleCombo.AddString(objMsg);
	objMsg.LoadString(IDS_LINESTYLE_LINE2);
	m_objLineStyleCombo.AddString(objMsg);
	objMsg.LoadString(IDS_LINESTYLE_OFFS2);
	m_objLineStyleCombo.AddString(objMsg);
	if( m_lStatusLineStyle >= 0 && m_lStatusLineStyle < m_objLineStyleCombo.GetCount() )
		m_objLineStyleCombo.SetCurSel(m_lStatusLineStyle);
	bFind=FALSE;
	for(nCnt=0; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		if(m_stStatusBarDisp.m_nCurrentItems[nCnt] == ID_INDICATOR_GYORETU)
		{
			bFind=TRUE;
			break;
		}
	}
	if(bFind != FALSE)
		m_objLineStyleCombo.EnableWindow(TRUE);
	else
		m_objLineStyleCombo.EnableWindow(FALSE);

	//メンバ変数－＞コントロール
	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「追加」ボタン押下時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage8::OnButtonAdd() 
{
	int nCnt=0;
	BOOL bFind=FALSE;
	CString objMsg;

	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	int nIndex = m_objAddItemListBox.GetCurSel();
	if(nIndex == LB_ERR)
		return;
	if(nIndex < 0 || nIndex >= STS_INDICATORS_COUNT)
		return;

	//リストボックス更新
	m_objAddItemListBox.DeleteString(nIndex);
	if(m_stStatusBarDisp.m_nAddItems[nIndex] == ID_INDICATOR_GYORETU)
		objMsg.LoadString(IDS_INDICATOR_GYORETU);
	else if(m_stStatusBarDisp.m_nAddItems[nIndex] == ID_INDICATOR_BYTE)
		objMsg.LoadString(IDS_INDICATOR_BYTE);
	else if(m_stStatusBarDisp.m_nAddItems[nIndex] == ID_INDICATOR_INS)
		objMsg.LoadString(IDS_INDICATOR_INS);
	else if(m_stStatusBarDisp.m_nAddItems[nIndex] == ID_INDICATOR_READONLY)
		objMsg.LoadString(IDS_INDICATOR_READONLY);
	else if(m_stStatusBarDisp.m_nAddItems[nIndex] == ID_INDICATOR_TIME)
		objMsg.LoadString(IDS_INDICATOR_TIME);
	else if(m_stStatusBarDisp.m_nAddItems[nIndex] == ID_INDICATOR_CHARCODE)
		objMsg.LoadString(IDS_INDICATOR_CHARCODE);
	else if(m_stStatusBarDisp.m_nAddItems[nIndex] == ID_INDICATOR_KANA)
		objMsg.LoadString(IDS_INDICATOR_KANA);
	else if(m_stStatusBarDisp.m_nAddItems[nIndex] == ID_INDICATOR_CAPS)
		objMsg.LoadString(IDS_INDICATOR_CAPS);
	else if(m_stStatusBarDisp.m_nAddItems[nIndex] == ID_INDICATOR_NUM)
		objMsg.LoadString(IDS_INDICATOR_NUM);
	else if(m_stStatusBarDisp.m_nAddItems[nIndex] == ID_INDICATOR_SCRL)
		objMsg.LoadString(IDS_INDICATOR_SCRL);
	else 
		return;

	m_objCurrentItemListBox.AddString(objMsg);

	// ステータスバー表示情報構造体更新
	m_stStatusBarDisp.m_nCurrentItems[m_stStatusBarDisp.m_nCurrentItemCount]
		= m_stStatusBarDisp.m_nAddItems[nIndex];
	for(nCnt=nIndex; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		if(nCnt<(STS_INDICATORS_COUNT-1))
			m_stStatusBarDisp.m_nAddItems[nCnt] = m_stStatusBarDisp.m_nAddItems[nCnt+1];
		else
			m_stStatusBarDisp.m_nAddItems[nCnt] = 0;
	}
	m_stStatusBarDisp.m_nAddItemCount--;
	m_stStatusBarDisp.m_nCurrentItemCount++;

	//追加、削除ボタン更新
	if(m_objCurrentItemListBox.GetCount() == 0)
		m_objDelButton.EnableWindow(FALSE);
	else 
		m_objDelButton.EnableWindow(TRUE);

	if(m_objAddItemListBox.GetCount() == 0)
		m_objAddButton.EnableWindow(FALSE);
	else
		m_objAddButton.EnableWindow(TRUE);

	//日付、時刻表示スタイルコンボボックス更新
	bFind=FALSE;
	for(nCnt=0; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		if(m_stStatusBarDisp.m_nCurrentItems[nCnt] == ID_INDICATOR_TIME)
		{
			bFind=TRUE;
			m_objDayStyleCombo.EnableWindow(TRUE);
			m_objHourStyleCombo.EnableWindow(TRUE);
			break;
		}
	}
	if(bFind==FALSE)
	{
		m_objDayStyleCombo.EnableWindow(FALSE);
		m_objHourStyleCombo.EnableWindow(FALSE);
	}

	//行、列表示スタイルコンボボックス更新
	bFind=FALSE;
	for(nCnt=0; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		if(m_stStatusBarDisp.m_nCurrentItems[nCnt] == ID_INDICATOR_GYORETU)
		{
			bFind=TRUE;
			m_objLineStyleCombo.EnableWindow(TRUE);
			break;
		}
	}
	if(bFind==FALSE)
		m_objLineStyleCombo.EnableWindow(FALSE);


	//メンバ変数－＞コントロール
	UpdateData(FALSE);

	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「削除」ボタン押下時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage8::OnButtonDel() 
{
	int nCnt=0;
	BOOL bFind=FALSE;
	CString objMsg;

	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	int nIndex = m_objCurrentItemListBox.GetCurSel();
	if(nIndex == LB_ERR)
		return;
	if(nIndex < 0 || nIndex >= STS_INDICATORS_COUNT)
		return;

	//リストボックス更新
	m_objCurrentItemListBox.DeleteString(nIndex);
	if(m_stStatusBarDisp.m_nCurrentItems[nIndex] == ID_INDICATOR_GYORETU)
		objMsg.LoadString(IDS_INDICATOR_GYORETU);
	else if(m_stStatusBarDisp.m_nCurrentItems[nIndex] == ID_INDICATOR_BYTE)
		objMsg.LoadString(IDS_INDICATOR_BYTE);
	else if(m_stStatusBarDisp.m_nCurrentItems[nIndex] == ID_INDICATOR_INS)
		objMsg.LoadString(IDS_INDICATOR_INS);
	else if(m_stStatusBarDisp.m_nCurrentItems[nIndex] == ID_INDICATOR_READONLY)
		objMsg.LoadString(IDS_INDICATOR_READONLY);
	else if(m_stStatusBarDisp.m_nCurrentItems[nIndex] == ID_INDICATOR_TIME)
		objMsg.LoadString(IDS_INDICATOR_TIME);
	else if(m_stStatusBarDisp.m_nCurrentItems[nIndex] == ID_INDICATOR_CHARCODE)
		objMsg.LoadString(IDS_INDICATOR_CHARCODE);
	else if(m_stStatusBarDisp.m_nCurrentItems[nIndex] == ID_INDICATOR_KANA)
		objMsg.LoadString(IDS_INDICATOR_KANA);
	else if(m_stStatusBarDisp.m_nCurrentItems[nIndex] == ID_INDICATOR_CAPS)
		objMsg.LoadString(IDS_INDICATOR_CAPS);
	else if(m_stStatusBarDisp.m_nCurrentItems[nIndex] == ID_INDICATOR_NUM)
		objMsg.LoadString(IDS_INDICATOR_NUM);
	else if(m_stStatusBarDisp.m_nCurrentItems[nIndex] == ID_INDICATOR_SCRL)
		objMsg.LoadString(IDS_INDICATOR_SCRL);
	else 
		return;

	m_objAddItemListBox.AddString(objMsg);

	// ステータスバー表示情報構造体更新
	m_stStatusBarDisp.m_nAddItems[m_stStatusBarDisp.m_nAddItemCount]
		= m_stStatusBarDisp.m_nCurrentItems[nIndex];
	for(nCnt=nIndex; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		if(nCnt<(STS_INDICATORS_COUNT-1))
			m_stStatusBarDisp.m_nCurrentItems[nCnt] = m_stStatusBarDisp.m_nCurrentItems[nCnt+1];
		else
			m_stStatusBarDisp.m_nCurrentItems[nCnt] = 0;
	}
	m_stStatusBarDisp.m_nCurrentItemCount--;
	m_stStatusBarDisp.m_nAddItemCount++;

	//追加、削除ボタン更新
	if(m_objCurrentItemListBox.GetCount() == 0)
		m_objDelButton.EnableWindow(FALSE);
	else 
		m_objDelButton.EnableWindow(TRUE);

	if(m_objAddItemListBox.GetCount() == 0)
		m_objAddButton.EnableWindow(FALSE);
	else
		m_objAddButton.EnableWindow(TRUE);

	//日付、時刻表示スタイルコンボボックス更新
	bFind=FALSE;
	for(nCnt=0; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		if(m_stStatusBarDisp.m_nCurrentItems[nCnt] == ID_INDICATOR_TIME)
		{
			bFind=TRUE;
			m_objDayStyleCombo.EnableWindow(TRUE);
			m_objHourStyleCombo.EnableWindow(TRUE);
			break;
		}
	}
	if(bFind==FALSE)
	{
		m_objDayStyleCombo.EnableWindow(FALSE);
		m_objHourStyleCombo.EnableWindow(FALSE);
	}

	//行、列表示スタイルコンボボックス更新
	bFind=FALSE;
	for(nCnt=0; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		if(m_stStatusBarDisp.m_nCurrentItems[nCnt] == ID_INDICATOR_GYORETU)
		{
			bFind=TRUE;
			m_objLineStyleCombo.EnableWindow(TRUE);
			break;
		}
	}
	if(bFind==FALSE)
		m_objLineStyleCombo.EnableWindow(FALSE);

	//メンバ変数－＞コントロール
	UpdateData(FALSE);

	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：日付表示スタイル変更時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage8::OnSelchangeComboDaystyle() 
{
	int nIndex = m_objDayStyleCombo.GetCurSel();
	if(nIndex != CB_ERR)
		m_lStatusDayStyle = nIndex;

	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：時刻表示スタイル変更時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage8::OnSelchangeComboHourstyle() 
{
	int nIndex = m_objHourStyleCombo.GetCurSel();
	if(nIndex != CB_ERR)
		m_lStatusHourStyle = nIndex;

	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行、列表示スタイル変更時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage8::OnSelchangeComboLinestyle() 
{
	int nIndex = m_objLineStyleCombo.GetCurSel();
	if(nIndex != CB_ERR)
		m_lStatusLineStyle = nIndex;

	m_bIsChanged = TRUE;
	SetModified();
}


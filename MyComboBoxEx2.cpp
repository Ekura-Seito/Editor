// MyComboBoxEx2.cpp : implementation file
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "MyComboBoxEx2.h"
#include "AutoResizer.h"
#include "CodeConvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyComboBoxEx2

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyComboBoxEx2::CMyComboBoxEx2()
{
	m_bAutoComplete = TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyComboBoxEx2::~CMyComboBoxEx2()
{
}


BEGIN_MESSAGE_MAP(CMyComboBoxEx2, CComboBox)
	//{{AFX_MSG_MAP(CMyComboBoxEx2)
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, OnEditUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyComboBoxEx2 message handlers

BOOL CMyComboBoxEx2::PreTranslateMessage(MSG* pMsg)
{
	// Need to check for backspace/delete. These will modify the text in
	// the edit box, causing the auto complete to just add back the text
	// the user has just tried to delete. 

	if (pMsg->message == WM_KEYDOWN)
	{
		m_bAutoComplete = FALSE;
	}
	else
	{
		m_bAutoComplete = TRUE;
	}
	return CComboBox::PreTranslateMessage(pMsg);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：更新時処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyComboBoxEx2::OnEditUpdate() 
{
	// if we are not to auto update the text, get outta here
	if (!m_bAutoComplete) return;

	//エディットボックスのテキスト取得
	CString str;
	CString euc, sjis, jis;
	euc.LoadStringA(IDS_CODECONV_TO_EUC);
	sjis.LoadStringA(IDS_CODECONV_TO_SJIS);
	jis.LoadStringA(IDS_CODECONV_TO_JIS);

	GetWindowText(str);
	if( str == euc  ||		//OK
		str == sjis ||
		str == jis
	)
	{
		SetWindowText(str);
	}
	else //NG
	{
		SetWindowText(CODECONV_TO_EUC);
	}
}


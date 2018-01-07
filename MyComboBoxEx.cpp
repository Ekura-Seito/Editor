// MyComboBoxEx.cpp : implementation file
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "MyComboBoxEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyComboBoxEx

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyComboBoxEx::CMyComboBoxEx()
{
	m_bAutoComplete = TRUE;
	m_bIsCompareNoCase = FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyComboBoxEx::~CMyComboBoxEx()
{
}


BEGIN_MESSAGE_MAP(CMyComboBoxEx, CComboBox)
	//{{AFX_MSG_MAP(CMyComboBoxEx)
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, OnEditUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyComboBoxEx パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：大文字、小文字を区別するか？設定										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyComboBoxEx::SetFlagCompareNoCase(BOOL bFlg)
{
	m_bIsCompareNoCase = bFlg;
}

/////////////////////////////////////////////////////////////////////////////
// CMyComboBoxEx message handlers

BOOL CMyComboBoxEx::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		m_bAutoComplete = TRUE;

		int nVirtKey = (int) pMsg->wParam;
		if (nVirtKey == VK_DELETE || nVirtKey == VK_BACK)
			m_bAutoComplete = FALSE;
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
void CMyComboBoxEx::OnEditUpdate() 
{
	if (!m_bAutoComplete) return;

	CString str;
	GetWindowText(str);
	int nLength = str.GetLength();

	if(m_bIsCompareNoCase)
		str.MakeLower();

	int nIndex = SelectString(-1,str);
	int nPreFoundIndex = -1;
	for(;;)
	{
		if(nIndex == CB_ERR) 
		{
			SetWindowText(str);
			break;
		}
		if( nPreFoundIndex==-1)
			nPreFoundIndex = nIndex;
		else if( nPreFoundIndex == nIndex ) //未発見のまま一巡した
		{
			SetWindowText(str);
			break;
		}

		//検索結果取得
		CString	objFoundStr;
		GetWindowText(objFoundStr);
		CString objLeft = objFoundStr.Left(nLength);
		if( str == objLeft ) //大文字、小文字区別を含めて正確に一致
		{
			break;
		}
		else
		{
			//元に戻す
			SetWindowText(str);
			//検索続行
			nIndex = SelectString(nIndex,str);
		}
	}

	SetEditSel(nLength,-1);
}


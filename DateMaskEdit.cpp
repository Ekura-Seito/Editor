// DateMaskEdit.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <afxdisp.h>	//For COleDateTime
#pragma warning(disable:4786)
#include "editor.h"
#include "Editor_Def.h"
#include "DateMaskEdit.h"

#define DELIMITER_CHARACTER		'/'		//デリミタ文字

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDateMaskEdit

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDateMaskEdit::CDateMaskEdit()
{
	m_bMaskKeyInProgress = FALSE;

	//日付フォーマット初期化
	m_objDateFormatString = _T("YYYY/MM/DD");
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDateMaskEdit::~CDateMaskEdit()
{
}


BEGIN_MESSAGE_MAP(CDateMaskEdit, CEdit)
	//{{AFX_MSG_MAP(CDateMaskEdit)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDateMaskEdit パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：初期化処理　日付設定													|*/
/*|	解説	：																		|*/
/*|	備考	：本メンバ関数は、OnInitDialog()から呼ぶこと							|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDateMaskEdit::Init()
{
	//ｳｲﾝﾄﾞｳ作成済み？
	ASSERT(::IsWindow(m_hWnd));

	//現在時刻取得＆設定
	CTime objTime;
	objTime = CTime::GetCurrentTime();
	m_objDateString = objTime.Format("%Y/%m/%d");
	SetWindowText(m_objDateString);
	UpdateData(TRUE);

	//メンバ変数へ保持
	m_nYear = (UINT)objTime.GetYear();
	m_nMonth = (UINT)objTime.GetMonth();
	m_nDay = (UINT)objTime.GetDay();

	//フォント設定
	SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：年設定処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDateMaskEdit::SetYear(UINT nYear)
{
	if(!(nYear < 9999))
		return;

	m_nYear = nYear;
	m_objDateString.Format("%04d/%02d/%02d",m_nYear,m_nMonth,m_nDay);
	SetWindowText(m_objDateString);
	UpdateData(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：月設定処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDateMaskEdit::SetMonth(UINT nMonth)
{
	if(!(nMonth >= 1 && nMonth <= 12))
		return;

	m_nMonth = nMonth;
	m_objDateString.Format("%04d/%02d/%02d",m_nYear,m_nMonth,m_nDay);
	SetWindowText(m_objDateString);
	UpdateData(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：日設定処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDateMaskEdit::SetDay(UINT nDay)
{
	if(!(nDay >= 1 && nDay <= 31))
		return;

	m_nDay = nDay;
	m_objDateString.Format("%04d/%02d/%02d",m_nYear,m_nMonth,m_nDay);
	SetWindowText(m_objDateString);
	UpdateData(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：年取得処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CDateMaskEdit::GetYear()
{
	return m_nYear;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：月取得処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CDateMaskEdit::GetMonth()
{
	return m_nMonth;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：日取得処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CDateMaskEdit::GetDay()
{
	return m_nDay;
}

/////////////////////////////////////////////////////////////////////////////
// CDateMaskEdit メッセージ ハンドラ
	
/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＫＥＹＤＯＵＮメッセージ処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDateMaskEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch(nChar) //DELETEｷｰ、INSERTｷｰ、BACK SPACEｷｰを無視
	{
    case VK_DELETE:
    case VK_INSERT: 
	case VK_BACK:
	case VK_CONTROL:
		  return;
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＣＨＡＲメッセージ処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDateMaskEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int startPos, endPos;

	switch(nChar) //DELETEｷｰ、INSERTｷｰ、BACK SPACEｷｰを無視
	{
    case VK_DELETE:
    case VK_INSERT: 
	case VK_BACK:
	case VK_CONTROL:
	case 0x01: //Ctrl+A
	case 0x03: //Ctrl+C
	case 0x16: //Ctrl+V
	case 0x18: //Ctrl+X
		MessageBeep((UINT)-1);
		return;
	}

	///////////////////////////////////////////
	//	フォーマットに合うかチェック
	if(!m_bMaskKeyInProgress)//ﾃﾞﾘﾐﾀ文字 '/' 用にWM_CHARﾒｯｾｰｼﾞ発行中でない
	{
		//押下された文字がフォーマットにあうかチェック
		if(!CheckChar(nChar))
		{
			MessageBeep((UINT)-1);//フォーマットに合わない
			return;
		}
	}

	////////////////////////////////////////////
	//	挿入モードで文字上書き
	if (isprint(nChar))//印字可能
	{
		GetSel(startPos, endPos);//何文字目でキーが押されたか取得
		SetSel(startPos, endPos+1);
		Clear();//エディットの選択解除
		m_objDateString.SetAt(endPos, nChar);//文字を入れ替え
	}

	//派生元クラスのハンドラをコール
	CEdit::OnChar(nChar, nRepCnt, nFlags);

	/////////////////////////////////////////////
	//	デリミタ文字を自動的にスキップ
	if(!m_bMaskKeyInProgress)//ﾃﾞﾘﾐﾀ文字 '/' 用にWM_CHARﾒｯｾｰｼﾞ発行中でない
	{
		GetSel(startPos, endPos);
		if(endPos < m_objDateFormatString.GetLength())
		{
			UINT c = m_objDateFormatString.GetAt(endPos);
			if (c == DELIMITER_CHARACTER)//次の文字がデリミタ
			{
				//自動的にスキップするように自分でWM_CHARメッセージを発行する
				SendChar(c);
			}
		}
	}

	/////////////////////////////////////////
	//	年、月、日更新
	char* pPtr = m_objDateString.GetBuffer(36);
	char	szBuf[36];
	strncpy(szBuf,pPtr,4);//年
	szBuf[4] = NULL;
	m_nYear = atoi(szBuf);
	strncpy(szBuf,(pPtr+5),2);//月
	szBuf[2] = NULL;
	m_nMonth = atoi(szBuf);
	strncpy(szBuf,(pPtr+8),2);//日
	szBuf[2] = NULL;
	m_nDay = atoi(szBuf);
	m_objDateString.ReleaseBuffer();
}

///////////////////////////////////////////////////////////////////
//	プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：フォーマットチェック処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDateMaskEdit::CheckChar(UINT nChar)//ＴＲＵＥ：ＯＫ、ＦＡＬＳＥ：ＮＧ
{
	UINT	c;
	int		startPos, endPos;
	char	szBuf[36];
	char	*pPtr;

	///////////////////////////////////////////
	//	押下キーチェック
	if(!isprint(nChar))//コントロールキーはＯＫ
		return TRUE;

	GetSel(startPos,endPos);
	if(endPos >= m_objDateFormatString.GetLength())//フォーマット文字列より長くなればはじく
		return FALSE;

	c = m_objDateFormatString.GetAt(endPos);
	if (c == DELIMITER_CHARACTER)	//デリミタ位置ではデリミタ文字以外ダメ
		if(nChar != c)
			return FALSE;
	else							//デリミタ位置以外では、数字以外ダメ
		if(!isdigit(nChar))
			return FALSE;

	///////////////////////////////////////////
	//	修正後文字取得
	CString objStr = m_objDateString;
	GetSel(startPos, endPos);
	objStr.SetAt(endPos,nChar);

	//年、月、日取り出し
	pPtr = objStr.GetBuffer(36);

	strncpy(szBuf,pPtr,4);//年
	szBuf[4] = NULL;
	int		nYear = atoi(szBuf);

	strncpy(szBuf,(pPtr+5),2);//月
	szBuf[2] = NULL;
	int		nMonth = atoi(szBuf);

	strncpy(szBuf,(pPtr+8),2);//日
	szBuf[2] = NULL;
	int		nDay = atoi(szBuf);

	objStr.ReleaseBuffer();

//日付妥当性チェックはCFileTouchDlg::OnOKにて行う
//	////////////////////////////////////////
//	//	日付妥当性チェック
//	COleDateTime	objOleDateTime(nYear,nMonth,nDay,0,0,0);
//	if(objOleDateTime.GetStatus() == COleDateTime::valid)//正当
//		return TRUE;
//	else
//		return FALSE;
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＣＨＡＲメッセージ発行処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDateMaskEdit::SendChar(UINT nChar)
{
	m_bMaskKeyInProgress = TRUE;//処理中
#ifdef WIN32
	AfxCallWndProc(this, m_hWnd, WM_CHAR, nChar, 1);
#else
	SendMessage(WM_CHAR, nChar, 1);
#endif
	m_bMaskKeyInProgress = FALSE;//終了
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：メッセージ前処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDateMaskEdit::PreTranslateMessage(MSG* pMsg) 
{
//	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
//	if( pMsg->message == WM_KEYDOWN ) 
//	{
//		if( pMsg->wParam == 'V' ) 
//		{
//			if( GetAsyncKeyState(VK_CONTROL) < 0 ) //Ctrl+Vを無視
//				return TRUE;
//		}
//	}
	return CEdit::PreTranslateMessage(pMsg);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＣＯＮＴＥＸＴＭＥＮＵメッセージ発行処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDateMaskEdit::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	//何もしないことで、マウス右クリックによるコンテキスト
	//メニューが表示されないようにする
}


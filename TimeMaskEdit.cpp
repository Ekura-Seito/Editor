// TimeMaskEdit.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <afxdisp.h>	//For COleDateTime
#pragma warning(disable:4786)
#include "editor.h"
#include "Editor_Def.h"
#include "TimeMaskEdit.h"

#define DELIMITER_CHARACTER		':'		//デリミタ文字

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimeMaskEdit

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CTimeMaskEdit::CTimeMaskEdit()
{
	m_bMaskKeyInProgress = FALSE;

	//時間フォーマット初期化
	m_objTimeFormatString = _T("HH:MM:SS");
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CTimeMaskEdit::~CTimeMaskEdit()
{
}


BEGIN_MESSAGE_MAP(CTimeMaskEdit, CEdit)
	//{{AFX_MSG_MAP(CTimeMaskEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDateMaskEdit パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：初期化処理　時刻設定													|*/
/*|	解説	：																		|*/
/*|	備考	：本メンバ関数は、OnInitDialog()から呼ぶこと							|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CTimeMaskEdit::Init()
{
	//ｳｲﾝﾄﾞｳ作成済み？
	ASSERT(::IsWindow(m_hWnd));

	//現在時刻取得＆設定
	CTime objTime;
	objTime = CTime::GetCurrentTime();
	m_objTimeString = objTime.Format("%H:%M:%S");
	SetWindowText(m_objTimeString);
	UpdateData(TRUE);

	//メンバ変数へ保持
	m_nHour   = (UINT)objTime.GetHour();
	m_nMinute = (UINT)objTime.GetMinute();
	m_nSecond = (UINT)objTime.GetSecond();

	//フォント設定
	SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：時間設定処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CTimeMaskEdit::SetHour(UINT nHour)
{
	if(!(nHour <= 23))
		return;

	m_nHour = nHour;
	m_objTimeString.Format("%02d:%02d:%02d",m_nHour,m_nMinute,m_nSecond);
	SetWindowText(m_objTimeString);
	UpdateData(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：分設定処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CTimeMaskEdit::SetMinute(UINT nMinute)
{
	if(!(nMinute <= 59))
		return;

	m_nMinute = nMinute;
	m_objTimeString.Format("%02d:%02d:%02d",m_nHour,m_nMinute,m_nSecond);
	SetWindowText(m_objTimeString);
	UpdateData(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：秒設定処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CTimeMaskEdit::SetSecond(UINT nSecond)
{
	if(!(nSecond <= 59))
		return;

	m_nSecond = nSecond;
	m_objTimeString.Format("%02d:%02d:%02d",m_nHour,m_nMinute,m_nSecond);
	SetWindowText(m_objTimeString);
	UpdateData(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：時間取得処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CTimeMaskEdit::GetHour()
{
	return m_nHour;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：分取得処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CTimeMaskEdit::GetMinute()
{
	return m_nMinute;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：秒取得処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CTimeMaskEdit::GetSecond()
{
	return m_nSecond;
}


/////////////////////////////////////////////////////////////////////////////
// CTimeMaskEdit メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＫＥＹＤＯＵＮメッセージ処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.06.20 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CTimeMaskEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
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
void CTimeMaskEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
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
	if(!m_bMaskKeyInProgress)//ﾃﾞﾘﾐﾀ文字 ':' 用にWM_CHARﾒｯｾｰｼﾞ発行中でない
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
		m_objTimeString.SetAt(endPos, nChar);//文字を入れ替え
	}

	//派生元クラスのハンドラをコール
	CEdit::OnChar(nChar, nRepCnt, nFlags);

	/////////////////////////////////////////////
	//	デリミタ文字を自動的にスキップ
	if(!m_bMaskKeyInProgress)//ﾃﾞﾘﾐﾀ文字 ':' 用にWM_CHARﾒｯｾｰｼﾞ発行中でない
	{
		GetSel(startPos, endPos);
		if(endPos < m_objTimeFormatString.GetLength())
		{
			UINT c = m_objTimeFormatString.GetAt(endPos);
			if (c == DELIMITER_CHARACTER)//次の文字がデリミタ
			{
				//自動的にスキップするように自分でWM_CHARメッセージを発行する
				SendChar(c);
			}
		}
	}

	////////////////////////////////////////////
	//	時間、分、秒更新
	char* pPtr = m_objTimeString.GetBuffer(36);
	char	szBuf[36];
	strncpy(szBuf,pPtr,2);//時間
	szBuf[2] = NULL;
	m_nHour = atoi(szBuf);
	strncpy(szBuf,(pPtr+3),2);//分
	szBuf[2] = NULL;
	m_nMinute = atoi(szBuf);
	strncpy(szBuf,(pPtr+6),2);//秒
	szBuf[2] = NULL;
	m_nSecond = atoi(szBuf);
	m_objTimeString.ReleaseBuffer();
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
BOOL CTimeMaskEdit::CheckChar(UINT nChar)	//ＴＲＵＥ：ＯＫ、ＦＡＬＳＥ：ＮＧ
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
	if(endPos >= m_objTimeFormatString.GetLength())//フォーマット文字列より長くなればはじく
		return FALSE;

	c = m_objTimeFormatString.GetAt(endPos);
	if (c == DELIMITER_CHARACTER)	//デリミタ位置ではデリミタ文字以外ダメ
		if(nChar != c)
			return FALSE;
	else							//デリミタ位置以外では、数字以外ダメ
		if(!isdigit(nChar))
			return FALSE;

	///////////////////////////////////////
	//	修正後文字取得
	CString objStr = m_objTimeString;
	GetSel(startPos, endPos);
	objStr.SetAt(endPos,nChar);

	//時間、分、秒取り出し
	pPtr = objStr.GetBuffer(36);

	strncpy(szBuf,pPtr,2);//時間
	szBuf[2] = NULL;
	int		nHour = atoi(szBuf);

	strncpy(szBuf,(pPtr+3),2);//分
	szBuf[2] = NULL;
	int		nMinute = atoi(szBuf);

	strncpy(szBuf,(pPtr+6),2);//秒
	szBuf[2] = NULL;
	int		nSecond = atoi(szBuf);

	objStr.ReleaseBuffer();

//日付妥当性チェックはCFileTouchDlg::OnOKにて行う
//	///////////////////////////////////////////
//	//	時刻妥当性チェック
//	COleDateTime	objOleDateTime(1998,6,20,nHour,nMinute,nSecond);//日付はダミー
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
void CTimeMaskEdit::SendChar(UINT nChar)
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
BOOL CTimeMaskEdit::PreTranslateMessage(MSG* pMsg) 
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
void CTimeMaskEdit::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	//何もしないことで、マウス右クリックによるコンテキスト
	//メニューが表示されないようにする
}


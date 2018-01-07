// EditorView2.cpp : CEditorView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include <Afxtempl.h>			//ＭＦＣのＣＭａｐを使用する
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include <afxadv.h>				//for CSharedFile
#include <mbstring.h>			//for OnLButtonDblClk
#include <io.h>					//for _access
#include "Editor_Def.h"
#include "StatusBarDisp.h"		//ステータスバー表示情報構造体
#include "ColorManager.h"
#include "CodeConvManager.h"
#include "BitmapClient.h"
#include "Operation.h"
#include "OperationManager.h"
#include "MainFrm.h"
#include "Editor.h"
#include "Line.h"
#include "LineManager.h"
#include "UndoItem.h"
#include "UndoManager.h"
#include "EditorDoc.h"
#include "EditorView.h"
#include "LineJumpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorView パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：範囲選択中チェック処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::IsSelected()
{
	if( (m_lSelectStartLine   != -1) && (m_lSelectEndLine   != -1) &&
		(m_lSelectStartOffset != -1) && (m_lSelectEndOffset != -1)
	)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：選択行数取得処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorView::GetSelectLineCount()
{
	if(!IsSelected())
		return 0;
	if(m_lSelectEndLine > m_lSelectStartLine)
		return (m_lSelectEndLine - m_lSelectStartLine + 1);
	else if(m_lSelectEndLine < m_lSelectStartLine)
		return (m_lSelectStartLine - m_lSelectEndLine + 1);
	else
		return 1;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：選択範囲解除処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::ClearSelectArea()
{
	//範囲選択中であれば解除する
	if( (m_lSelectStartLine != -1) || (m_lSelectStartOffset != -1) ||
		(m_lSelectEndLine   != -1) || (m_lSelectEndOffset   != -1)
	)
	{
		//選択範囲解除
		m_lSelectStartLine   = -1;
		m_lSelectStartOffset = -1;
		m_lSelectEndLine     = -1;
		m_lSelectEndOffset   = -1;

		//再描画
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);
	}
	m_bIsShiftKeyDown=FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：検索実行処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::DoFind					//ＴＲＵＥ：発見終了、ＦＡＬＳＥ：未発見終了
	(	CString&	objFindStr,				//検索対象文字列
		BOOL		bIsMatchCase,			//大文字、小文字を区別するか？（ＴＲＵＥ：する、ＦＡＬＳＥ：しない）
		BOOL		bIsMatchWholeWord,		//単語全体が一致するもの？（ＴＲＵＥ：全体、ＦＡＬＳＥ：一部）
		BOOL		bIsSearchDown,			//文末方向への検索？（ＴＲＵＥ：文末方向、ＦＡＬＳＥ：文頭方向）
		BOOL		bIsUseRegularExpression	//正規表現を使うか？
	)
{
	long lMatchLength = 0;
	long lMatchStartOffset = -1;

	//////////////////////////////////////////////
	//	現在のキャレット位置の行、列番号取得	//
	//////////////////////////////////////////////

	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	long lMaxLine = pDoc->GetLineCount();
	long lStartLine = 0;
	long lStartOffset = 0;
	if( CaretToLineOffset(&lStartLine,&lStartOffset) == FALSE )
		return FALSE;

	//範囲選択中であれば解除する
	ClearSelectArea();

	//検索対象文字列が全て英数字から成るかチェック
	BOOL bAllAlphaNum = TRUE;
	for(long lCnt=0; lCnt<objFindStr.GetLength(); lCnt++)
	{
		if( IsDBCSLeadByteEx(0,objFindStr.GetAt(lCnt)) )
		{
			bAllAlphaNum = FALSE;
			break;
		}
		else if( (!isAlnum(objFindStr.GetAt(lCnt))) &&
				 (objFindStr.GetAt(lCnt) != '_')
		)
		{
			bAllAlphaNum = FALSE;
			break;
		}
	}

	//////////////////////
	//	検索ループ		//
	//////////////////////
	long lCurrentLine   = lStartLine;
	long lCurrentOffset = lStartOffset;
	for(;;)
	{
		//１行取得
		CLine* pobjLine = pDoc->GetOneLine( lCurrentLine );
		if( lCurrentLine != lStartLine )
		{
			if( bIsSearchDown ) //文末方向検索
				lCurrentOffset = 0;
			else //文頭方向検索
				lCurrentOffset = pobjLine->GetLength();
		}
		lMatchStartOffset = -1;
		lMatchLength = 0;
		if( pobjLine != NULL )
		{
			//検索
			if( bIsSearchDown ) //文末方向検索
			{
				lMatchStartOffset = pobjLine->DoFind( lCurrentOffset,		//検索開始オフセット
													  pobjLine->GetLength(),//検索終了オフセット
													  objFindStr,			//検索対象文字列
													  bAllAlphaNum,			//検索対象文字列がすべて英数字であるか？
													  bIsMatchCase,			//大文字、小文字を区別するか？
													  bIsMatchWholeWord,	//単語全体が一致するもの？
													  bIsUseRegularExpression,
													  &lMatchLength );
			}
			else //文頭方向検索
			{
				lMatchStartOffset = pobjLine->DoReverseFind( 0,				//検索開始オフセット
													  lCurrentOffset,		//検索終了オフセット
													  objFindStr,			//検索対象文字列
													  bAllAlphaNum,			//検索対象文字列がすべて英数字であるか？
													  bIsMatchCase,			//大文字、小文字を区別するか？
													  bIsMatchWholeWord,	//単語全体が一致するもの？
													  bIsUseRegularExpression,
													  &lMatchLength );
			}
		}
		if( lMatchStartOffset != -1 ) //見つかった
		{
			//選択範囲設定
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lMatchStartOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lMatchStartOffset + lMatchLength;

			//発見位置へジャンプ
			if( bIsSearchDown ) //文末方向検索
			{
				Jump2LineOffsetEx(m_lSelectEndLine,m_lSelectEndOffset);
			}
			else //文頭方向検索
			{
				Jump2LineOffsetEx(m_lSelectStartLine,m_lSelectStartOffset);
			}
			//再描画
			OnUpdate(NULL,NULL,NULL);
			Invalidate(FALSE);

			return TRUE;
		}
		else //見つからなかった
		{
			//検索位置の更新
			if( bIsSearchDown ) //文末方向検索
			{
				if( (lCurrentLine+1) >= lMaxLine ) //最終行到達
					break;
				else
					lCurrentLine++;
			}
			else if( bIsSearchDown == FALSE ) //文頭方向検索
			{
				if( lCurrentLine == 0 ) //先頭行到達
					break;
				else
					lCurrentLine--;
			}
		}
	}

	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：インクリメンタル検索実行処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::DoIncrimentalSearch		//ＴＲＵＥ：発見終了、ＦＡＬＳＥ：未発見終了
	(	long		lStartLine,				// 検索開始行
		long		lStartOffset,			// 検索開始オフセット
		CString&	objFindStr,				// 検索文字列
		BOOL		bIsSearchDown			// 文末方向への検索？（ＴＲＵＥ：文末方向、ＦＡＬＳＥ：文頭方向）
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	long lMaxLine = pDoc->GetLineCount();
	BOOL		bIsMatchCase=TRUE;			// 大文字、小文字を区別するか？（ＴＲＵＥ：する、ＦＡＬＳＥ：しない）
	BOOL		bIsMatchWholeWord=FALSE;	// 単語全体が一致するもの？（ＴＲＵＥ：全体、ＦＡＬＳＥ：一部）
	long	lMatchLength=0;
	long	lMatchStartOffset=-1;


	//範囲選択中であれば解除する
	ClearSelectArea();

	//検索対象文字列が全て英数字から成るかチェック
	BOOL bAllAlphaNum = TRUE;
	for(long lCnt=0; lCnt<objFindStr.GetLength(); lCnt++)
	{
		if( IsDBCSLeadByteEx(0,objFindStr.GetAt(lCnt)) )
		{
			bAllAlphaNum = FALSE;
			break;
		}
		else if( (!isAlnum(objFindStr.GetAt(lCnt))) &&
				 (objFindStr.GetAt(lCnt) != '_')
		)
		{
			bAllAlphaNum = FALSE;
			break;
		}
	}

	//////////////////////
	//	検索ループ		//
	//////////////////////
	long lCurrentLine   = lStartLine;
	long lCurrentOffset = lStartOffset;
	for(;;)
	{
		//１行取得
		CLine* pobjLine = pDoc->GetOneLine( lCurrentLine );
		if( lCurrentLine != lStartLine )
		{
			if( bIsSearchDown ) //文末方向検索
				lCurrentOffset = 0;
			else //文頭方向検索
				lCurrentOffset = pobjLine->GetLength();
		}
		lMatchStartOffset = -1;
		lMatchLength = 0;
		if( pobjLine != NULL )
		{
			//検索
			if( bIsSearchDown ) //文末方向検索
			{
				lMatchStartOffset = pobjLine->DoFind( lCurrentOffset,		//検索開始オフセット
													  pobjLine->GetLength(),//検索終了オフセット
													  objFindStr,			//検索対象文字列
													  bAllAlphaNum,			//検索対象文字列がすべて英数字であるか？
													  bIsMatchCase,			//大文字、小文字を区別するか？
													  bIsMatchWholeWord,	//単語全体が一致するもの？
													  FALSE,
													  &lMatchLength);
			}
			else //文頭方向検索
			{
				lMatchStartOffset = pobjLine->DoReverseFind( 0,				//検索開始オフセット
													  lCurrentOffset,		//検索終了オフセット
													  objFindStr,			//検索対象文字列
													  bAllAlphaNum,			//検索対象文字列がすべて英数字であるか？
													  bIsMatchCase,			//大文字、小文字を区別するか？
													  bIsMatchWholeWord,	//単語全体が一致するもの？
													  FALSE,
													  &lMatchLength);
			}
		}
		if( lMatchStartOffset != -1 ) //見つかった
		{
			//選択範囲設定
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lMatchStartOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lMatchStartOffset + objFindStr.GetLength();

			//発見位置へジャンプ
			if( bIsSearchDown ) //文末方向検索
			{
				Jump2LineOffsetEx(m_lSelectEndLine,m_lSelectEndOffset);
			}
			else //文頭方向検索
			{
				Jump2LineOffsetEx(m_lSelectStartLine,m_lSelectStartOffset);
			}
			//再描画
			OnUpdate(NULL,NULL,NULL);
			Invalidate(FALSE);

			return TRUE;
		}
		else //見つからなかった
		{
			//検索位置の更新
			if( bIsSearchDown ) //文末方向検索
			{
				if( (lCurrentLine+1) >= lMaxLine ) //最終行到達
					break;
				else
					lCurrentLine++;
			}
			else if( bIsSearchDown == FALSE ) //文頭方向検索
			{
				if( lCurrentLine == 0 ) //先頭行到達
					break;
				else
					lCurrentLine--;
			}
		}
	}

	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：置換実行処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::DoReplace
	(	CString&	objReplaceStr			//置換対象文字列
	)
{
	if( !IsSelected() )
		return;

	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//範囲選択位置調整
	AdjustSelectArea();

	//置換処理
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessReplaceSelectWord(
						  objReplaceStr,		//置換後文字列
						  lCurrentLine,			//キャレット位置の行
						  lCurrentOffset,		//キャレット位置のオフセット
						  m_lSelectStartLine,	//選択開始行
						  m_lSelectStartOffset,	//選択開始オフセット
						  m_lSelectEndLine,		//選択終了行
						  m_lSelectEndOffset,	//選択終了オフセット
						  m_caretPos,			//キャレット位置
						  objScrollPosLog,		//スクロール位置
						  m_bDispRuler,
						  m_bDispLineNo );

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：表示フォントのＬＯＧＦＯＮＴ構造体取得処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetLogfontOfDisplay
	(	LOGFONT*	pstLogFont
	)
{
	if( m_objDispFont.GetLogFont(pstLogFont) != 0 )
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印字フォントのＬＯＧＦＯＮＴ構造体取得処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetLogfontOfPrint
	(	LOGFONT*	pstLogFont
	)
{
	//未実装
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：表示フォントのポイント取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorView::GetDispFontPoint()
{
	return m_lDispFontPoint;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：表示フォントのポイント設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetDispFontPoint(long lDispFontPoint)
{
	if(m_lDispFontPoint == lDispFontPoint)
		return;

	//新しいポイントを保持
	m_lDispFontPoint = lDispFontPoint;

	//表示フォント作成しなおし
	m_objDispFont.DeleteObject();
	m_objDispFontUnderLine.DeleteObject();

	CClientDC	dc(this);
	CPoint ptVpOrg(0, 0);
	ptVpOrg.x = -1 * GetScrollPosLogX();
	dc.SetViewportOrg(ptVpOrg);

	//表示フォント作成
	m_objDispFont.CreatePointFont(m_lDispFontPoint,m_objDispFontName,&dc);
	//下線付き表示フォント作成
	LOGFONT logfont;
	memset(&logfont,0,sizeof(LOGFONT));
	m_objDispFont.GetLogFont(&logfont);
	logfont.lfUnderline = TRUE;
	m_objDispFontUnderLine.CreateFontIndirect(&logfont);

	CFont* oldFont = dc.SelectObject(&m_objDispFont);
	TEXTMETRIC	tm;
	dc.GetTextMetrics(&tm);
	m_lfHeight = tm.tmHeight + tm.tmExternalLeading;
	m_lfWidth  = tm.tmAveCharWidth + tm.tmOverhang;
	dc.SelectObject(oldFont);

	//一度キャレット消去(1999.03.16 Add)
	if(m_bDispUnderLine)
		MyEraseUnderLine(&dc);
	HideCaret();

	//行番号表示関連初期化
	if( m_bDispLineNo )
	{
		m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		m_LineNoPos = CPoint(m_lLineNoWidth,0);
	}
	else
	{
		m_lLineNoWidth = 0;
		m_LineNoPos = CPoint(0,0);
	}

	//ルーラ表示関連初期化
	if( m_bDispRuler )
		m_RulerPos = CPoint(0,(m_lfHeight+m_lLineInterval));
	else
		m_RulerPos = CPoint(0,0);

	//キャレット位置初期化
	if(m_bInsertKey)
		CreateSolidCaret(m_lfWidth/4, m_lfHeight); //挿入キャレット作成
	else
		CreateSolidCaret(m_lfWidth, m_lfHeight); //上書きキャレット作成
	ShowCaret();					//キャレット表示
	MySetCaretPos(CPoint(m_LineNoPos.x,m_RulerPos.y));		//キャレット位置設定

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：表示フォント名取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CString CEditorView::GetDispFontName()
{
	return m_objDispFontName;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：表示フォント名設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetDispFontName(CString& objDispFontName)
{
	if(!m_objDispFontName.CompareNoCase(objDispFontName))
		return;

	//新しいフォント名保持
	m_objDispFontName = objDispFontName;

	//表示フォント作成しなおし
	m_objDispFont.DeleteObject();
	m_objDispFontUnderLine.DeleteObject();

	CClientDC	dc(this);
	CPoint ptVpOrg(0, 0);
	ptVpOrg.x = -1 * GetScrollPosLogX();
	dc.SetViewportOrg(ptVpOrg);

	//表示フォント作成
	m_objDispFont.CreatePointFont(m_lDispFontPoint,m_objDispFontName,&dc);
	//下線付き表示フォント作成
	LOGFONT logfont;
	memset(&logfont,0,sizeof(LOGFONT));
	m_objDispFont.GetLogFont(&logfont);
	logfont.lfUnderline = TRUE;
	m_objDispFontUnderLine.CreateFontIndirect(&logfont);
	
	CFont* oldFont = dc.SelectObject(&m_objDispFont);
	TEXTMETRIC	tm;
	dc.GetTextMetrics(&tm);
	m_lfHeight = tm.tmHeight + tm.tmExternalLeading;
	m_lfWidth  = tm.tmAveCharWidth + tm.tmOverhang;
	dc.SelectObject(oldFont);

	//一度キャレット消去(1999.03.16 Add)
	if(m_bDispUnderLine)
		MyEraseUnderLine(&dc);
	HideCaret();

	//行番号表示関連初期化
	if( m_bDispLineNo )
	{
		m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		m_LineNoPos = CPoint(m_lLineNoWidth,0);
	}
	else
	{
		m_lLineNoWidth = 0;
		m_LineNoPos = CPoint(0,0);
	}

	//ルーラ表示関連初期化
	if( m_bDispRuler )
		m_RulerPos = CPoint(0,(m_lfHeight+m_lLineInterval));
	else
		m_RulerPos = CPoint(0,0);

	//キャレット位置初期化
	if(m_bInsertKey)
		CreateSolidCaret(m_lfWidth/4, m_lfHeight); //挿入キャレット作成
	else
		CreateSolidCaret(m_lfWidth, m_lfHeight); //上書きキャレット作成
	ShowCaret();					//キャレット表示
	MySetCaretPos(CPoint(m_LineNoPos.x,m_RulerPos.y));		//キャレット位置設定

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印字フォントのポイント取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorView::GetPrintFontPoint()
{
	return m_lPrintFontPoint;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印字フォントのポイント設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetPrintFontPoint(long lPrintFontPoint)
{
	//新しいポイントを保持
	m_lPrintFontPoint = lPrintFontPoint;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印字フォント名取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CString CEditorView::GetPrintFontName()
{
	return m_objPrintFontName;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印字フォント名設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetPrintFontName(CString& objPrintFontName)
{
	//新しいフォント名保持
	m_objPrintFontName = objPrintFontName;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：通常の文字色取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetFontColor()
{
	return m_FontColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：通常の文字色設定処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFontColor(COLORREF color)
{
	m_FontColor = color;

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：通常の背景色取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetBackColor()
{
	return m_BackColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：通常の背景色設定処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetBackColor(COLORREF color)
{
	m_BackColor = color;

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：反転時の文字色取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetInvertFontColor()
{
	return m_InvertFontColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：反転時の文字色設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetInvertFontColor(COLORREF color)
{
	m_InvertFontColor = color;

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：反転時の背景色設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetInvertBackColor()
{
	return m_InvertBackColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：反転時の背景色設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetInvertBackColor(COLORREF color)
{
	m_InvertBackColor = color;

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：改行マーク表示フラグ取得処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetFlagDispCRLF()
{
	return m_bDispCRLF;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：改行マーク表示フラグ設定処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagDispCRLF
	(	BOOL	bDispCRLF
	)
{
	m_bDispCRLF = bDispCRLF;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：改行マーク色取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetCRLFColor()
{
	return m_CRLFColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：改行マーク色設定処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetCRLFColor
	(	COLORREF	color
	)
{
	m_CRLFColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行番号表示フラグ取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetFlagDispLineNo()
{
	return m_bDispLineNo;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行番号表示フラグ設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagDispLineNo
	(	BOOL	bDispLineNo
	)
{
	if( m_bDispLineNo == bDispLineNo ) //フラグに変化なし
		return;

	m_bDispLineNo = bDispLineNo;
	if( m_bDispLineNo )
	{
		m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		m_LineNoPos = CPoint(m_lLineNoWidth,0);
		CPoint objPoint = m_caretPos + m_LineNoPos;
		MySetCaretPos(objPoint);
	}
	else
	{
		m_lLineNoWidth = 0;
		if(m_caretPos.x >= m_LineNoPos.x)
		{
			CPoint objPoint = m_caretPos - m_LineNoPos;
			m_LineNoPos = CPoint(0,0);
			MySetCaretPos(objPoint);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行番号色取得処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetLineNoColor()
{
	return m_LineNoColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行番号色設定処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetLineNoColor
	(	COLORREF	color
	)
{
	m_LineNoColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行番号の背景の色取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetLineNoBackColor()
{
	return m_LineNoBackColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行番号の背景の色設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetLineNoBackColor
	(	COLORREF	color
	)
{
	m_LineNoBackColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ルーラ表示フラグ取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetFlagDispRuler()
{
	return m_bDispRuler;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ルーラ表示フラグ設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagDispRuler
	(	BOOL	bDispRuler
	)
{
	if( m_bDispRuler == bDispRuler ) //フラグに変化なし
		return;

	m_bDispRuler = bDispRuler;
	if( m_bDispRuler )
	{
		m_RulerPos = CPoint(0,(m_lfHeight+m_lLineInterval));
		CPoint objPoint = m_caretPos + m_RulerPos;
		MySetCaretPos(objPoint);
	}
	else
	{
		CPoint objPoint = m_caretPos - m_RulerPos;
		m_RulerPos = CPoint(0,0);
		MySetCaretPos(objPoint);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ルーラ色取得処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetRulerColor()
{
	return m_RulerColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ルーラ色設定処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetRulerColor
	(	COLORREF	color
	)
{
	m_RulerColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ルーラの背景の色取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetRulerBackColor()
{
	return m_RulerBackColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ルーラの背景の色設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetRulerBackColor
	(	COLORREF	color
	)
{
	m_RulerBackColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：全角スペース表示フラグ取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetFlagDispZenkakuSpace()
{
	return m_bDispZenkakuSpace;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：全角スペース表示フラグ設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagDispZenkakuSpace
	(	BOOL	bDispZenkakuSpace
	)
{
	m_bDispZenkakuSpace = bDispZenkakuSpace;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：全角スペース色取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetZenkakuSpaceColor()
{
	return m_ZenkakuSpaceColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：全角スペース色設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetZenkakuSpaceColor
	(	COLORREF	color
	)
{
	m_ZenkakuSpaceColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：タブコード表示フラグ取得処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetFlagDispTab()
{
	return m_bDispTab;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：タブコード表示フラグ設定処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagDispTab
	(	BOOL	bDispTab
	)
{
	m_bDispTab = bDispTab;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：タブコード色取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetTabColor()
{
	return m_TabColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：タブコード色設定処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetTabColor
	(	COLORREF	color
	)
{
	m_TabColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行番号印刷フラグ取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetFlagPrintLineNo()
{
	return m_bPrintLineNo;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行番号印刷フラグ設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagPrintLineNo
	(	BOOL	bPrintLineNo
	)
{
	m_bPrintLineNo = bPrintLineNo;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：２段組み印刷フラグ取得処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetFlagPrint2Dangumi()
{
	return m_bPrint2Dangumi;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：２段組み印刷フラグ設定処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagPrint2Dangumi
	(	BOOL	bPrint2Dangumi
	)
{
	m_bPrint2Dangumi = bPrint2Dangumi;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷ヘッダレイアウト位置取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::GetPrintHeaderLayoutPos()
{
	return m_nHeaderLayout;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷ヘッダレイアウト位置設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetPrintHeaderLayoutPos
	(	int	nHeaderLayout
	)
{
	if( nHeaderLayout>=0 && nHeaderLayout<=2 )
	{
		m_nHeaderLayout = nHeaderLayout;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷フッタレイアウト位置取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::GetPrintFooterLayoutPos()
{
	return m_nFooterLayout;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷フッタレイアウト位置設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetPrintFooterLayoutPos
	(	int	nFooterLayout
	)
{
	if( nFooterLayout>=0 && nFooterLayout<=2 )
	{
		m_nFooterLayout = nFooterLayout;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷ヘッダフォーマット取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetStrHeaderFormat
	(	CString&	objStr
	)
{
	objStr = m_objStrHeaderFormat;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷ヘッダフォーマット設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetStrHeaderFormat
	(	CString&	objStr
	)
{
	m_objStrHeaderFormat = objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷フッタフォーマット取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetStrFooterFormat
	(	CString&	objStr
	)
{
	objStr = m_objStrFooterFormat;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷フッタフォーマット設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetStrFooterFormat
	(	CString&	objStr
	)
{
	m_objStrFooterFormat = objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行間取得処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorView::GetLineInterval()
{
	return m_lLineInterval;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行間設定処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetLineInterval
	(	long	lLineInterval
	)
{
	if(m_lLineInterval == lLineInterval)
		return;
	if(lLineInterval<0 || lLineInterval>2)
		return;

	//行間設定
	m_lLineInterval = lLineInterval;

	MyScrollToPosition(CPoint(0,0));
	MySetCaretPos(CPoint(m_LineNoPos.x,m_RulerPos.y));

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：アンダーライン表示フラグ取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetFlagDispUnderLine()
{
	return m_bDispUnderLine;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：アンダーライン表示フラグ設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagDispUnderLine
	(	BOOL	bDispUnderLine
	)
{
	m_bDispUnderLine = bDispUnderLine;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：アンダーライン色取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetUnderLineColor()
{
	return m_UnderLineColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：アンダーライン色設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetUnderLineColor
	(	COLORREF	color
	)
{
	m_UnderLineColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット形状取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::GetCaretType()
{
	return m_nCaretType;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット形状設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetCaretType
	(	int		nCaretType
	)
{
	if(m_nCaretType == nCaretType) //すでに同じ
		return;
	if( (nCaretType!=CARET_IBEAM) && (nCaretType!=CARET_SQUARE) )
		return;

	//キャレット形状保持
	m_nCaretType = nCaretType;

	if(!m_bInsertKey) //挿入モードでは反映させない
		return;

	if(m_nCaretType==CARET_IBEAM) //縦型キャレット
	{
		HideCaret();								//キャレット非表示
		CreateSolidCaret(m_lfWidth/4, m_lfHeight); 	//縦棒キャレット作成
		ShowCaret();								//キャレット表示
		MySetCaretPos(m_caretPos);					//キャレット位置設定
	}
	else if(m_nCaretType==CARET_SQUARE) //箱型キャレット
	{
		HideCaret();								//キャレット非表示
		CreateSolidCaret(m_lfWidth, m_lfHeight/2); 	//箱型キャレット作成
		ShowCaret();								//キャレット表示
		MySetCaretPos(m_caretPos);					//キャレット位置設定
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット点滅間隔保持処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::HoldCaretBlinkTime
	(	UINT	unCaretBlinkTime
	)
{
	m_unCaretBlinkTime = unCaretBlinkTime;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレットの振る舞い取得処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::GetCaretBehave()
{
	return m_nCaretBehave;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレットの振る舞い設定処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetCaretBehave
	(	int		nCaretBehave
	)
{
	if((nCaretBehave!=CARET_BEHAVE_MOVE)&&(nCaretBehave!=CARET_BEHAVE_STAY))
		return;

	m_nCaretBehave = nCaretBehave;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット移動タイプ取得処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::GetCaretMoveType()
{
	return m_nCaretMoveType;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット移動タイプ設定処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetCaretMoveType
	(	int		nCaretMoveType
	)
{
	if(nCaretMoveType != CARET_MOVE_WORDTYPE && nCaretMoveType != CARET_MOVE_NOTEPADTYPE)
		return;

	m_nCaretMoveType = nCaretMoveType;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＥＯＦマーク表示フラグ設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagDispEOFMark
	(	BOOL	bIsDispEOFMark
	)
{
	BOOL bOrg = m_pApp->GetProfileInt("CEditorView","bIsDispEOFMark",1);
	m_pApp->WriteProfileInt("CEditorView","bIsDispEOFMark",bIsDispEOFMark);

	if(bOrg != bIsDispEOFMark)
	{
		//再描画
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：選択範囲テキスト取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLine* CEditorView::MyGetSelectedText
	(	long*	plSelectLineCnt			// 選択範囲行数返却用バッファポインタ
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(!IsSelected())
	{
		(*plSelectLineCnt) = 0;
		return NULL;
	}
	else
	{
		if(m_lSelectStartLine == m_lSelectEndLine)
			(*plSelectLineCnt) = 1;
		else if(m_lSelectStartLine < m_lSelectEndLine)
			(*plSelectLineCnt) = m_lSelectEndLine - m_lSelectStartLine + 1;
		else if(m_lSelectStartLine > m_lSelectEndLine)
			(*plSelectLineCnt) = m_lSelectStartLine - m_lSelectEndLine + 1;

		return pDoc->MyGetSelectedText( m_lSelectStartLine,
										m_lSelectStartOffset,
										m_lSelectEndLine,
										m_lSelectEndOffset );
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：論理スクロール位置Ｘ取得処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::GetScrollPosLogX()
{
	SCROLLINFO	hinfo;
	memset(&hinfo,0,sizeof(SCROLLINFO));
	CScrollBar* pobjHScrollBar = GetScrollBarCtrl(SB_HORZ);
	pobjHScrollBar->GetScrollInfo(&hinfo,SIF_ALL);
	if( (hinfo.nPos%m_lfWidth) != 0 )
	{
		AdjustHScrollPos(&hinfo);
		pobjHScrollBar->SetScrollInfo(&hinfo,TRUE);
	}
	return hinfo.nPos;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：論理スクロール位置Ｙ取得処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::GetScrollPosLogY()
{
	SCROLLINFO	vinfo;
	memset(&vinfo,0,sizeof(SCROLLINFO));
	CScrollBar* pobjVScrollBar = GetScrollBarCtrl(SB_VERT);
	pobjVScrollBar->GetScrollInfo(&vinfo,SIF_ALL);
	if( (vinfo.nPos%(m_lfHeight+m_lLineInterval)) != 0 )
	{
		AdjustVScrollPos(&vinfo);
		pobjVScrollBar->SetScrollInfo(&vinfo,TRUE);
	}
	return vinfo.nPos;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：水平スクロール位置調整処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::AdjustHScrollPos
	(	SCROLLINFO*	pInfo
	)
{
	if( (pInfo->nPos%m_lfWidth) == 0)
		return;

	int nAmari = pInfo->nPos%m_lfWidth;
	int nSyo   = pInfo->nPos/m_lfWidth;
	if(nAmari >= m_lfWidth/2)
		pInfo->nPos = (nSyo+1)*m_lfWidth;
	else
		pInfo->nPos = nSyo * m_lfWidth;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：垂直スクロール位置調整処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::AdjustVScrollPos
	(	SCROLLINFO*	pInfo
	)
{
	if( (pInfo->nPos%(m_lfHeight+m_lLineInterval)) == 0)
		return;

	int nAmari = pInfo->nPos%(m_lfHeight+m_lLineInterval);
	int nSyo   = pInfo->nPos/(m_lfHeight+m_lLineInterval);
	if(nAmari >= ((m_lfHeight+m_lLineInterval)/2))
		pInfo->nPos = (nSyo+1)*(m_lfHeight+m_lLineInterval);
	else
		pInfo->nPos = nSyo * (m_lfHeight+m_lLineInterval);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：マーク行色取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetMarkLineColor()
{
	return m_MarkLineColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：マーク行色設定処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetMarkLineColor
	(	COLORREF	MarkLineColor
	)
{
	m_MarkLineColor = MarkLineColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル比較用、削除行色取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetDiffDelLineColor()
{
	return m_DiffDelLineColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル比較用、削除行色設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetDiffDelLineColor
	(	COLORREF	color
	)
{
	m_DiffDelLineColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル比較用、追加行色取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetDiffAddLineColor()
{
	return m_DiffAddLineColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル比較用、追加行色設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetDiffAddLineColor
	(	COLORREF	color
	)
{
	m_DiffAddLineColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ比較用、相違ファイル名色取得処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetDirDiffLineColor()
{
	return m_DirDiffLineColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ比較用、相違ファイル名色設定処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetDirDiffLineColor
	(	COLORREF	color
	)
{
	m_DirDiffLineColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：インクリメンタルサーチ開始行、オフセットクリア処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::ClearIncrimentalSearchLineOffset()
{
	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) != FALSE )
	{
		m_lIncrimentalSearchStartLine = lCurrentLine;
		m_lIncrimentalSearchStartOffset = lCurrentOffset;
	}
	else
	{
		m_lIncrimentalSearchStartLine = 0;
		m_lIncrimentalSearchStartOffset = 0;
	}
	m_objIncrimentalSearchLine.Set((const unsigned char*)"");
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：インクリメンタルサーチ文字列取得処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetIncrimentalSearchString
	(	CString&	objStr
	)
{
	objStr = m_objIncrimentalSearchLine.String();
}

/////////////////////////////////////////////////////////////////////////////
//	CEditorView プロテクト関数（スクロール関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：スクロールサイズ設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MySetScrollSizes
	(	int		nMapMode,		// マッピングモード
		SIZE	sizeTotal,		// トータルサイズ
		const SIZE&	sizeVPage,	// 垂直スクロールバーシャフトクリックでのスクロールサイズ
		const SIZE& sizeVLine,	// 垂直スクロールバー矢印クリックでのスクロールサイズ
		const SIZE& sizeHPage,	// 水平スクロールバーシャフトクリックでのスクロールサイズ
		const SIZE& sizeHLine	// 水平スクロールバー矢印クリックでのスクロールサイズ
	)
{
	BOOL bRet;

	// メンバ変数へ保持
	m_nMapMode = nMapMode;				// マッピングモード
	m_totalLog = sizeTotal;				// 論理単位でのトータルサイズ
	m_VPageLog = sizeVPage;				// 垂直スクロールバーの論理単位での１ページあたりスクロールサイズ
	m_VLineLog = sizeVLine;				// 垂直スクロールバーの論理単位での１行あたりスクロールサイズ
	m_HPageLog = sizeHPage;				// 水平スクロールバーの論理単位での１ページあたりスクロールサイズ
	m_HLineLog = sizeHLine;				// 水平スクロールバーの論理単位での１行あたりスクロールサイズ

	{
		CWindowDC dc(NULL);
		dc.SetMapMode(m_nMapMode);			// マッピングモード設定
	}

	// 垂直スクロールバー更新
	SCROLLINFO		vinfo;
	memset(&vinfo,0,sizeof(SCROLLINFO));
	CScrollBar*	pobjVScrollBar = GetScrollBarCtrl(SB_VERT);
	pobjVScrollBar->GetScrollInfo(&vinfo,SIF_ALL);
	vinfo.fMask = SIF_ALL;
	vinfo.nMin  = 0;
	vinfo.nMax  = m_totalLog.cy;
	vinfo.nPage = m_VPageLog.cy;
	if( (vinfo.nPos%(m_lfHeight+m_lLineInterval)) != 0 )
		AdjustVScrollPos(&vinfo);
	bRet = pobjVScrollBar->SetScrollInfo(&vinfo,TRUE);

	// 水平ｽｸﾛｰﾙﾊﾞｰ更新
	SCROLLINFO		hinfo;
	memset(&hinfo,0,sizeof(SCROLLINFO));
	CScrollBar*	pobjHScrollBar = GetScrollBarCtrl(SB_HORZ);
	pobjHScrollBar->GetScrollInfo(&hinfo,SIF_ALL);
	hinfo.fMask = SIF_ALL;
	hinfo.nMin  = 0;
	hinfo.nMax  = m_totalLog.cx;
	hinfo.nPage = m_HPageLog.cx;
	if( (hinfo.nPos%m_lfWidth) != 0 )
		AdjustHScrollPos(&hinfo);
	bRet = pobjHScrollBar->SetScrollInfo(&hinfo,TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：スクロール位置設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyScrollToPosition
	(	CPoint&	point			//論理座標値
	)
{
	if(m_bDispUnderLine) //アンダーラインを描画する場合
	{
		//スクロールする前にアンダーラインを消去しておく
		CClientDC dc(this);
		CPoint ptVpOrg(0, 0);
		ptVpOrg.x = -1 * GetScrollPosLogX();
		dc.SetViewportOrg(ptVpOrg);
		MyEraseUnderLine(&dc);
	}

	/////////////////////////////////
	// 垂直ｽｸﾛｰﾙﾊﾞｰ更新
	/////////////////////////////////
	SCROLLINFO		vinfo;
	memset(&vinfo,0,sizeof(SCROLLINFO));
	CScrollBar*	pobjVScrollBar = GetScrollBarCtrl(SB_VERT);
	pobjVScrollBar->GetScrollInfo(&vinfo,SIF_ALL);
	int yOrig = vinfo.nPos;	//前回位置
	vinfo.nPos = point.y;	//今回位置
	if(vinfo.nPos<0)
		vinfo.nPos = 0;

	//ｽｸﾛｰﾙﾊﾞｰ現在位置更新
	if( (vinfo.nPos%(m_lfHeight+m_lLineInterval)) != 0 )
		AdjustVScrollPos(&vinfo);
	pobjVScrollBar->SetScrollInfo(&vinfo,TRUE);

	//////////////////////////////////////
	// 水平ｽｸﾛｰﾙﾊﾞｰ更新
	//////////////////////////////////////
	SCROLLINFO		hinfo;
	memset(&hinfo,0,sizeof(SCROLLINFO));
	CScrollBar*	pobjHScrollBar = GetScrollBarCtrl(SB_HORZ);
	pobjHScrollBar->GetScrollInfo(&hinfo,SIF_ALL);
	int xOrig = hinfo.nPos;	//前回位置
	hinfo.nPos = point.x;	//今回位置
	if(hinfo.nPos<0)
		hinfo.nPos=0;

	//ｽｸﾛｰﾙﾊﾞｰ現在位置更新
	if( (hinfo.nPos%m_lfWidth) != 0 )
		AdjustHScrollPos(&hinfo);
	pobjHScrollBar->SetScrollInfo(&hinfo,TRUE);

	if( (xOrig!=hinfo.nPos) || (yOrig!=vinfo.nPos) )
	{
#if 1
		if(yOrig != vinfo.nPos)
		{
			int nAmount = -(vinfo.nPos - yOrig);
			MyScrollWindow(2,nAmount);
		}
		if(xOrig != hinfo.nPos)
		{
			int nAmount = -(hinfo.nPos - xOrig);
			MyScrollWindow(1,nAmount);
		}
#else
		//再描画
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);
#endif
	}

	//ｷｬﾚｯﾄ位置
	MySetCaretPos(m_caretPos);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ｳｲﾝﾄﾞｳｽｸﾛｰﾙ処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyScrollWindow
	(	BOOL	bHorzVert,			//ｽｸﾛｰﾙ方向( 1 : 水平方向、2 : 垂直方向）
		int		nAmount				//ｽｸﾛｰﾙ量
	)
{
	CRect	clientRect, rect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();
	rect = clientRect;

	if( bHorzVert == 1 )// 水平ｽｸﾛｰﾙ
	{
#if 1
		InvalidateRect(&clientRect,FALSE);
		UpdateWindow();
		return;
#endif

		if(nAmount > 0)//左へ
		{
			clientRect.right = nAmount;
			if( m_bDispLineNo )
			{
				clientRect.left -= m_lLineNoWidth;
				clientRect.right += (m_lLineNoWidth + m_lfWidth*4);
			}
		}
		else //右へ
		{
			clientRect.left = clientRect.right - (clientRect.right % m_lfWidth);
			clientRect.left += nAmount;
		}
		ScrollWindow( nAmount, 0 );
		if(m_pobjInvalidRect)
			delete m_pobjInvalidRect;
		m_pobjInvalidRect = new CRect();
		(*m_pobjInvalidRect) = clientRect;
		m_bUseClipRgn = TRUE; //クリップされた領域のみ描画更新する
		InvalidateRect(&clientRect,FALSE);
//		if( m_bDispLineNo )
//		{
//			CRect linenoRect(0,0,m_lLineNoWidth,clientRect.bottom);
//			m_bUseClipRgn = TRUE; //クリップされた領域のみ描画更新する
//			InvalidateRect(&linenoRect,FALSE);
//		}
		//すぐに描画
		UpdateWindow();
	}
	else // 垂直ｽｸﾛｰﾙ
	{
		if(nAmount > 0)//上へ
		{
			clientRect.bottom = nAmount;
			if( m_bDispRuler )
			{
				clientRect.top += (m_lfHeight+m_lLineInterval);
				clientRect.bottom += (m_lfHeight+m_lLineInterval);
				//スクロール時にルーラをうごかさないよう、スクロール領域からルーラ領域を除く
				rect.top += (m_lfHeight+m_lLineInterval);
			}
		}
		else //下へ
		{
			clientRect.top = clientRect.bottom - (clientRect.bottom % (m_lfHeight+m_lLineInterval));
			clientRect.top += nAmount;
			if( m_bDispRuler )
			{
				//スクロール時にルーラがちらつかないよう、スクロール領域からルーラ領域を除く
				int nScrollLineCount = ((-1)*nAmount)/(m_lfHeight+m_lLineInterval);
				rect.top += (m_lfHeight+m_lLineInterval)*(1+nScrollLineCount);
			}
		}
		ScrollWindow( 0, nAmount, rect );
		if(m_pobjInvalidRect)
			delete m_pobjInvalidRect;
		m_pobjInvalidRect = new CRect();
		(*m_pobjInvalidRect) = clientRect;
		m_bUseClipRgn = TRUE; //クリップされた領域のみ描画更新する
		InvalidateRect(&clientRect,FALSE);
		//すぐに描画
		UpdateWindow();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オートスクロール処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::AutoScroll()
{
	MSG		msg;
	CEditorDoc*	pDoc = (CEditorDoc*)GetDocument();
	CPoint	tempPos(0,0);
	CRect	clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	memset(&msg,0,sizeof(MSG));
	msg.message	= 0;
	for(;;)
	{
		//メッセージポンプ
		if(::PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		if(msg.message == WM_LBUTTONUP)
		{
			break;
		}

		//マウス位置取得
		POINT	stCursorPos;
		::GetCursorPos(&stCursorPos);
		ScreenToClient(&stCursorPos);

		//オートスクロール
		if(stCursorPos.y < 0) //上
		{
			OnVScroll(SB_LINEUP,NULL,GetScrollBarCtrl(SB_VERT));
			continue;
		}
		else if(stCursorPos.y >	clientRect.Height()) //下
		{
			OnVScroll(SB_LINEDOWN,NULL,GetScrollBarCtrl(SB_VERT));
			continue;
		}
		else if(stCursorPos.x <	0) //左
		{
			OnHScroll(SB_LINEUP,NULL,GetScrollBarCtrl(SB_HORZ));
			continue;
		}
		else if(stCursorPos.x >	clientRect.Width())//右
		{
			OnHScroll(SB_LINEDOWN,NULL,GetScrollBarCtrl(SB_HORZ));
			continue;
		}

//		if(stCursorPos.x < m_LineNoPos.x)
//			continue;
		if(stCursorPos.y < m_RulerPos.y)
			continue;

		//マウス位置　－＞　行、列番号変換
		long lLineBuf=0;
		long lOffsetBuf=0;
		tempPos.x =	0;
		tempPos.y =	0;
		CPoint objCursorPos(stCursorPos.x,stCursorPos.y);
		long lRet =	MouseToLineOffset( objCursorPos, &lLineBuf,	&lOffsetBuf, &tempPos );
		if(lRet	== 0)
		{
			continue;
		}
		else if(lRet ==	1)
		{
			//キャレット位置を現在のマウス位置に設定
			MySetCaretPos(tempPos);
		}
		else if(lRet ==	2)
		{
			//キャレットをマウス位置行の最後に位置づけ
			CLine* pobjLine	= pDoc->GetOneLine(lLineBuf);
			if(pobjLine	!= NULL)
				lOffsetBuf = pobjLine->GetLength();
			MySetCaretToLineOffset(lLineBuf,lOffsetBuf);
		}

		// 選択範囲の再描画処理
		RedrawSelectArea(m_lPreMoveCaretLine,m_lPreMoveCaretOffset);

		//再設定
		m_lPreMoveCaretLine	= lLineBuf;
		m_lPreMoveCaretOffset =	lOffsetBuf;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	CEditorView プロテクト関数（描画関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オフスクリーンバッファ作成処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::CreateOffScreenBuf()
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	//最大幅計算
	m_nOffBufferWidth = 0;
	if( ((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth) > clientRect.Width() )
		m_nOffBufferWidth = (ULONG)((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth);
	else
		m_nOffBufferWidth = clientRect.Width();

	//オフスクリーンバッファ作成
	CDC* pDC = GetDC();
	m_objOffBufferDC.CreateCompatibleDC(pDC);
	m_objOffBufferBmp.CreateCompatibleBitmap(pDC,m_nOffBufferWidth,clientRect.Height());
	m_objOffBufferDC.SelectObject(&m_objOffBufferBmp);
	ReleaseDC(pDC);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オフスクリーンバッファ破棄処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::DestroyOffScreenBuf()
{
	//オフスクリーンバッファ破棄
	m_objOffBufferDC.DeleteDC();
	m_objOffBufferBmp.DeleteObject();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オフスクリーンバッファのアップデート処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::UpdateOffBuffer()
{
	//背景色ブラシ作成
	CBrush	objBackBrush;
	objBackBrush.CreateSolidBrush(m_BackColor);
	CBrush	objRulerBackBrush;
	objRulerBackBrush.CreateSolidBrush(m_RulerBackColor);

	//行番号表示フィールドの幅
	if( m_bDispLineNo ) //行番号を表示する
		m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
	else
		m_lLineNoWidth = 0;

	//描画開始行と描画終了行を計算
	ULONG	lineStart, lineStop;	//描画行番号
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	lineStart = (ULONG)((ULONG)GetScrollPosLogY() / (ULONG)(m_lfHeight+m_lLineInterval));
	lineStop  = lineStart + (ULONG)((ULONG)clientRect.Height() / (ULONG)(m_lfHeight+m_lLineInterval)) + 1;

	//フォント設定
	CFont* oldFont = m_objOffBufferDC.SelectObject(&m_objDispFont);

	//１行づつ描画
	ULONG ulCnt = 0;
	if( m_bDispRuler ) //ルーラを表示する
	{
		//背景色で塗りつぶし
		CRect	lineRect(0,(ulCnt*(m_lfHeight+m_lLineInterval)),m_nOffBufferWidth,((ulCnt+1)*(m_lfHeight+m_lLineInterval)));
		lineRect.NormalizeRect();
		m_objOffBufferDC.FillRect(&lineRect,&objRulerBackBrush);
		MyDrawingRuler(&m_objOffBufferDC,0,0,m_nOffBufferWidth,m_RulerColor,m_RulerBackColor);
		ulCnt++;
		lineStop--;
	}
	ULONG ulLineNo;
	for ( ulLineNo=lineStart; ulLineNo<lineStop; ulLineNo++)
	{
		//背景色で塗りつぶし
		CRect	lineRect(0,(ulCnt*(m_lfHeight+m_lLineInterval)),m_nOffBufferWidth,((ulCnt+1)*(m_lfHeight+m_lLineInterval)));
		lineRect.NormalizeRect();
		m_objOffBufferDC.FillRect(&lineRect,&objBackBrush);

		CLine*  pobjLine = pDoc->GetOneLine(ulLineNo);
		if(pobjLine == NULL)
		{
			CRect objRect(  0,
							(ulCnt*(m_lfHeight+m_lLineInterval)),
							clientRect.Width()+m_lfWidth*10,
							((ulCnt+1)*(m_lfHeight+m_lLineInterval))
			);
			m_objOffBufferDC.FillRect(&objRect,&objBackBrush);
		}
		MyTextOut(&m_objOffBufferDC,m_lLineNoWidth,(ulCnt*(m_lfHeight+m_lLineInterval)),pobjLine,m_lfWidth,ulLineNo);

		ulCnt++;
	}
	if( m_bDispUnderLine )//アンダーラインを描画する場合
	{
		MyDrawingUnderLine(&m_objOffBufferDC);
	}

	//フォントをもとに戻す
	m_objOffBufferDC.SelectObject(oldFont);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１行描画処理															|*/
/*|	解説	：																		|*/
/*|	備考	：OnPrepareDCでﾋﾞｭｰﾎﾟｰﾄの原点調整を行っていない場合用					|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyTextOut
	(	CDC*	pDC,			//描画用デバイスコンテキスト
		long	nX,				//テキストの開始点のデバイス x 座標
		long	nY,				//テキストの開始点のデバイス y 座標
		CLine*  pobjLine,		//描画される文字列
		long	lfWidth,		//フォントの幅
		long	lLineNo			//行番号
	)
{
	ULONG	ulCnt=0;
	ULONG	ulOffset=0;
	char	szSpace[] = "            ";
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	BOOL	bMarked = pDoc->GetFlagMark(lLineNo);
	int		nDiffLineKind = pDoc->GetDiffLineKind(lLineNo);
	int		nDirCmpLineKind = pDoc->GetDirCmpLineKind(lLineNo);
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();
	CFont* pOldFont=NULL;

	if(pDC->IsPrinting())
		return;

	//////////////////////////
	//	範囲選択位置調整	//
	//////////////////////////
	long	lSelectStartLine=0;
	long	lSelectEndLine=0;
	long	lSelectStartOffset=0;
	long	lSelectEndOffset=0;
	if( m_lSelectStartLine < m_lSelectEndLine )
	{
		lSelectStartLine   = m_lSelectStartLine;
		lSelectEndLine     = m_lSelectEndLine;
		lSelectStartOffset = m_lSelectStartOffset;
		lSelectEndOffset   = m_lSelectEndOffset;
	}
	else if( m_lSelectStartLine == m_lSelectEndLine )
	{
		lSelectStartLine = m_lSelectStartLine;
		lSelectEndLine   = m_lSelectEndLine;
		if( m_lSelectStartOffset <= m_lSelectEndOffset )
		{
			lSelectStartOffset = m_lSelectStartOffset;
			lSelectEndOffset   = m_lSelectEndOffset;
		}
		else
		{
			lSelectStartOffset = m_lSelectEndOffset;
			lSelectEndOffset   = m_lSelectStartOffset;
		}
	}
	else
	{
		lSelectStartLine   = m_lSelectEndLine;
		lSelectEndLine     = m_lSelectStartLine;
		lSelectStartOffset = m_lSelectEndOffset;
		lSelectEndOffset   = m_lSelectStartOffset;
	}

	//背景色ブラシ作成
	CBrush	objBackBrush;
	objBackBrush.CreateSolidBrush(m_BackColor);
	//反転背景色ブラシ作成
	CBrush	objInvertBackBrush;
	objInvertBackBrush.CreateSolidBrush(m_InvertBackColor);
	//マーク色ブラシ作成
	CBrush	objMarkLineBrush;
	objMarkLineBrush.CreateSolidBrush(m_MarkLineColor);
	//ファイル比較用削除行色ブラシ作成
	CBrush	objDiffDelLineBrush;
	objDiffDelLineBrush.CreateSolidBrush(m_DiffDelLineColor);
	//ファイル比較用追加行色ブラシ作成
	CBrush	objDiffAddLineBrush;
	objDiffAddLineBrush.CreateSolidBrush(m_DiffAddLineColor);
	//ディレクトリ比較用相違ファイル名色ブラシ作成
	CBrush	objDirDiffLineBrush;
	objDirDiffLineBrush.CreateSolidBrush(m_DirDiffLineColor);

	//////////////////////////////////////
	//	水平方向のスクロールを考慮し、	//
	//	描画開始オフセットを計算する	//
	//////////////////////////////////////
	long	lScrollPosLogX = GetScrollPosLogX();
	CPoint	objLimitPos(lScrollPosLogX,0);
	CPoint	objTempPos(0,0);
	//lCnt    <- 水平スクロール位置直前のインデックス
	//lOffset <- 水平スクロール位置直前のオフセット
	GetPosition5(lLineNo,&objLimitPos,&ulCnt,&objTempPos);
	ulOffset = objTempPos.x;
	ULONG ulStartOffset = ulOffset;
	//水平スクロールを考慮した描画開始のデバイスＸ座標
	long lDevX = nX - GetScrollPosLogX();


	int			nOldBkMode = pDC->GetBkMode();
	COLORREF    clrOldText = pDC->GetTextColor();
	COLORREF	clrOldBack = pDC->GetBkColor();
	if( pobjLine != NULL )
	{
		//////////////////////////////////
		//	キーワードカラーリング		//
		//////////////////////////////////
		pobjLine->ResizeColorBuffer(pobjLine->GetLength(),0);
		pobjLine->ResizeUnderLineBuffer(pobjLine->GetLength(),FALSE);
		pDoc->KeyWordColoring(pobjLine);

		//////////////////
		//	文字出力	//
		//////////////////
		for( ; ulCnt<(ULONG)pobjLine->GetLength(); )//文字列長分ループ
		{
			//クライアント領域の幅の分だけにする
			if( (ulOffset-ulStartOffset) > (ULONG)(clientRect.Width()+m_lfWidth*10) )
				break;

			//選択範囲内判定
			BOOL bSelected = IsWithinSelectArea( lSelectStartLine,
												 lSelectStartOffset,
												 lSelectEndLine,
												 lSelectEndOffset,
												 lLineNo,
												 ulCnt);
			if(IsDBCSLeadByteEx(0,pobjLine->GetAt(ulCnt)))//２バイト文字の第一バイト
			{
				if( bSelected ) //選択範囲内
				{
					//反転背景色で塗りつぶし
					CRect	lineRect((lDevX+ulOffset),nY,(lDevX+ulOffset+m_lfWidth*2),(nY+m_lfHeight+m_lLineInterval));
					lineRect.NormalizeRect();
					pDC->FillRect(&lineRect,&objInvertBackBrush);

					//文字色変更
					pDC->SetBkMode( TRANSPARENT );
					pDC->SetBkColor( m_InvertBackColor );
					pDC->SetTextColor( m_InvertFontColor );

					//下線付きチェック
					if(pobjLine->GetUnderLineAt(ulCnt))
					{
						pOldFont = pDC->SelectObject(&m_objDispFontUnderLine);
						//２文字分出力
						pDC->TextOut((lDevX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),2);
						pDC->SelectObject(pOldFont);
					}
					else
					{
						//２文字分出力
						pDC->TextOut((lDevX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),2);
					}

					if( (pobjLine->GetAt(ulCnt)==129)  && //全角スペースを描画する場合
						(pobjLine->GetAt(ulCnt+1)==64) &&
						(m_bDispZenkakuSpace!=FALSE)
					)
					{
						MyDrawingZenkakuSpace(	pDC,
								(lDevX+ulOffset),
								nY,
								m_lfWidth*2,
								m_lfHeight,
								m_InvertFontColor,
								m_InvertBackColor );
					}

					ulCnt+=2;
					ulOffset += (lfWidth * 2);
				}
				else //選択範囲外
				{
					CRect	lineRect((lDevX+ulOffset),nY,(lDevX+ulOffset+m_lfWidth*2),(nY+m_lfHeight+m_lLineInterval));
					lineRect.NormalizeRect();
					if(bMarked)
					{
						//マーク色で塗りつぶし
						pDC->FillRect(&lineRect,&objMarkLineBrush);
					}
					else if(nDiffLineKind==DIFF_LINEKIND_DELETED)
					{
						//削除行色で塗りつぶし
						pDC->FillRect(&lineRect,&objDiffDelLineBrush);
					}
					else if(nDiffLineKind==DIFF_LINEKIND_ADDED)
					{
						//追加行色で塗りつぶし
						pDC->FillRect(&lineRect,&objDiffAddLineBrush);
					}
					else if(nDirCmpLineKind==DIRCMP_LINEKIND_DIFF)
					{
						//相違色で塗りつぶし
						pDC->FillRect(&lineRect,&objDirDiffLineBrush);
					}
					else
					{
						//背景色で塗りつぶし
						pDC->FillRect(&lineRect,&objBackBrush);
					}

					//文字色変更
					pDC->SetBkMode( TRANSPARENT );
					if(bMarked)
						pDC->SetBkColor( m_MarkLineColor );
					else
						pDC->SetBkColor( m_BackColor );
					pDC->SetTextColor( pobjLine->GetColorAt(ulCnt) );

					//下線付きチェック
					if(pobjLine->GetUnderLineAt(ulCnt))
					{
						pOldFont = pDC->SelectObject(&m_objDispFontUnderLine);
						//２文字分出力
						pDC->TextOut((lDevX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),2);
						pDC->SelectObject(pOldFont);
					}
					else
					{
						//２文字分出力
						pDC->TextOut((lDevX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),2);
					}

					if( (pobjLine->GetAt(ulCnt)==129)  && //全角スペースを描画する場合
						(pobjLine->GetAt(ulCnt+1)==64) &&
						(m_bDispZenkakuSpace!=FALSE)
					)
					{
						if(bMarked)
						{
							MyDrawingZenkakuSpace(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth*2,
									m_lfHeight,
									m_ZenkakuSpaceColor,
									m_MarkLineColor );
						}
						else if(nDiffLineKind==DIFF_LINEKIND_DELETED)
						{
							MyDrawingZenkakuSpace(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth*2,
									m_lfHeight,
									m_ZenkakuSpaceColor,
									m_DiffDelLineColor );
						}
						else if(nDiffLineKind==DIFF_LINEKIND_ADDED)
						{
							MyDrawingZenkakuSpace(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth*2,
									m_lfHeight,
									m_ZenkakuSpaceColor,
									m_DiffAddLineColor );
						}
						else if(nDirCmpLineKind==DIRCMP_LINEKIND_DIFF)
						{
							MyDrawingZenkakuSpace(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth*2,
									m_lfHeight,
									m_ZenkakuSpaceColor,
									m_DirDiffLineColor );
						}
						else
						{
							MyDrawingZenkakuSpace(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth*2,
									m_lfHeight,
									m_ZenkakuSpaceColor,
									m_BackColor );
						}
					}

					ulCnt+=2;
					ulOffset += (lfWidth * 2);
				}
			}
			else //１バイト文字
			{
				if(pobjLine->GetAt(ulCnt) == '\t')//タブコード
				{
					if( bSelected ) //選択範囲内
					{
						//タブサイズ分計算
						int nMargin = pDoc->GetTabSize() - ( (ulOffset/lfWidth) % pDoc->GetTabSize() );

						//反転背景色で塗りつぶし
						CRect	lineRect((lDevX+ulOffset),nY,(lDevX+ulOffset+m_lfWidth*nMargin),(nY+m_lfHeight+m_lLineInterval));
						lineRect.NormalizeRect();
						pDC->FillRect(&lineRect,&objInvertBackBrush);

						//文字色変更
						pDC->SetBkMode( TRANSPARENT );
						pDC->SetBkColor( m_InvertBackColor );
						pDC->SetTextColor( m_InvertFontColor );

						//タブサイズ分スペース描画
						pDC->TextOut((lDevX+ulOffset),nY,szSpace,nMargin);

						if( m_bDispTab ) //タブコードを描画する場合
						{
							MyDrawingTab(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth*nMargin,
									m_lfHeight,
									m_InvertFontColor,
									m_InvertBackColor );
						}

						ulOffset += (lfWidth * nMargin);
						ulCnt++;
					}
					else // 範囲選択中でない
					{
						//タブサイズ計算
						int nMargin = pDoc->GetTabSize() - ( (ulOffset/lfWidth) % pDoc->GetTabSize() );

						CRect	lineRect((lDevX+ulOffset),nY,(lDevX+ulOffset+m_lfWidth*nMargin),(nY+m_lfHeight+m_lLineInterval));
						lineRect.NormalizeRect();
						if(bMarked)
						{
							//マーク色で塗りつぶし
							pDC->FillRect(&lineRect,&objMarkLineBrush);
						}
						else if(nDiffLineKind==DIFF_LINEKIND_DELETED)
						{
							//削除行色で塗りつぶし
							pDC->FillRect(&lineRect,&objDiffDelLineBrush);
						}
						else if(nDiffLineKind==DIFF_LINEKIND_ADDED)
						{
							//追加行色で塗りつぶし
							pDC->FillRect(&lineRect,&objDiffAddLineBrush);
						}
						else if(nDirCmpLineKind==DIRCMP_LINEKIND_DIFF)
						{
							//相違色で塗りつぶし
							pDC->FillRect(&lineRect,&objDirDiffLineBrush);
						}
						else
						{
							//背景色で塗りつぶし
							pDC->FillRect(&lineRect,&objBackBrush);
						}

						//文字色変更
						pDC->SetBkMode( TRANSPARENT );
						if(bMarked)
							pDC->SetBkColor( m_MarkLineColor );
						else
							pDC->SetBkColor( m_BackColor );
						pDC->SetTextColor( pobjLine->GetColorAt(ulCnt) );

						//タブサイズ分スペース描画
						pDC->TextOut((lDevX+ulOffset),nY,szSpace,nMargin);

						if( m_bDispTab ) //タブコードを描画する場合
						{
							if(bMarked)
							{
								MyDrawingTab(	pDC,
										(lDevX+ulOffset),
										nY,
										m_lfWidth*nMargin,
										m_lfHeight,
										m_TabColor,
										m_MarkLineColor );
							}
							else if(nDiffLineKind==DIFF_LINEKIND_DELETED)
							{
								MyDrawingTab(	pDC,
										(lDevX+ulOffset),
										nY,
										m_lfWidth*nMargin,
										m_lfHeight,
										m_TabColor,
										m_DiffDelLineColor );
							}
							else if(nDiffLineKind==DIFF_LINEKIND_ADDED)
							{
								MyDrawingTab(	pDC,
										(lDevX+ulOffset),
										nY,
										m_lfWidth*nMargin,
										m_lfHeight,
										m_TabColor,
										m_DiffAddLineColor );
							}
							else if(nDirCmpLineKind==DIRCMP_LINEKIND_DIFF)
							{
								MyDrawingTab(	pDC,
										(lDevX+ulOffset),
										nY,
										m_lfWidth*nMargin,
										m_lfHeight,
										m_TabColor,
										m_DirDiffLineColor );
							}
							else
							{
								MyDrawingTab(	pDC,
										(lDevX+ulOffset),
										nY,
										m_lfWidth*nMargin,
										m_lfHeight,
										m_TabColor,
										m_BackColor );
							}
						}

						ulOffset += (lfWidth * nMargin);
						ulCnt++;
					}
				}
				else //タブ以外
				{
					if( bSelected ) //選択範囲内
					{
						//反転背景色で塗りつぶし
						CRect	lineRect((lDevX+ulOffset),nY,(lDevX+ulOffset+m_lfWidth),(nY+m_lfHeight+m_lLineInterval));
						lineRect.NormalizeRect();
						pDC->FillRect(&lineRect,&objInvertBackBrush);

						//文字色変更
						pDC->SetBkMode( TRANSPARENT );
						pDC->SetBkColor( m_InvertBackColor );
						pDC->SetTextColor( m_InvertFontColor );

						//下線付きチェック
						if(pobjLine->GetUnderLineAt(ulCnt))
						{
							pOldFont = pDC->SelectObject(&m_objDispFontUnderLine);
							//１文字分出力
							pDC->TextOut((lDevX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),1);
							pDC->SelectObject(pOldFont);
						}
						else
						{
							//１文字分出力
							pDC->TextOut((lDevX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),1);
						}
						ulCnt++;
						ulOffset += lfWidth;
					}
					else // 範囲選択中でない
					{
						CRect	lineRect((lDevX+ulOffset),nY,(lDevX+ulOffset+m_lfWidth),(nY+m_lfHeight+m_lLineInterval));
						lineRect.NormalizeRect();
						if(bMarked)
						{
							//マーク色で塗りつぶし
							pDC->FillRect(&lineRect,&objMarkLineBrush);
						}
						else if(nDiffLineKind==DIFF_LINEKIND_DELETED)
						{
							//削除行色で塗りつぶし
							pDC->FillRect(&lineRect,&objDiffDelLineBrush);
						}
						else if(nDiffLineKind==DIFF_LINEKIND_ADDED)
						{
							//追加行色で塗りつぶし
							pDC->FillRect(&lineRect,&objDiffAddLineBrush);
						}
						else if(nDirCmpLineKind==DIRCMP_LINEKIND_DIFF)
						{
							//相違色で塗りつぶし
							pDC->FillRect(&lineRect,&objDirDiffLineBrush);
						}
						else
						{
							//背景色で塗りつぶし
							pDC->FillRect(&lineRect,&objBackBrush);
						}

						//文字色変更
						pDC->SetBkMode( TRANSPARENT );
						if(bMarked)
							pDC->SetBkColor( m_MarkLineColor );
						else
							pDC->SetBkColor( m_BackColor );
						pDC->SetTextColor( pobjLine->GetColorAt(ulCnt) );

						//下線付きチェック
						if(pobjLine->GetUnderLineAt(ulCnt))
						{
							pOldFont = pDC->SelectObject(&m_objDispFontUnderLine);
							//１文字分出力
							pDC->TextOut((lDevX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),1);
							pDC->SelectObject(pOldFont);
						}
						else
						{
							//１文字分出力
							pDC->TextOut((lDevX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),1);
						}
						ulCnt++;
						ulOffset += lfWidth;
					}
				}
			}
		}

		//改行マーク描画
		if( (m_bDispCRLF!=FALSE) && ((lLineNo+1)!=pDoc->GetLineCount()) )
		{
			//選択範囲内判定
			BOOL bSelected = IsWithinSelectArea( lSelectStartLine,
											lSelectStartOffset,
											lSelectEndLine,
											lSelectEndOffset,
											lLineNo,
											ulCnt);
			if( bSelected ) //選択範囲内
			{
				//反転背景色で塗りつぶし
				CRect	lineRect((lDevX+ulOffset),nY,(lDevX+ulOffset+m_lfWidth),(nY+m_lfHeight+m_lLineInterval));
				lineRect.NormalizeRect();
				pDC->FillRect(&lineRect,&objInvertBackBrush);

				MyDrawingCRLF(	pDC,
								(lDevX+ulOffset),
								nY,
								m_lfWidth,
								m_lfHeight,
								OPAQUE,
								m_InvertFontColor,
								m_InvertBackColor );
			}
			else
			{
				CRect	lineRect((lDevX+ulOffset),nY,(lDevX+ulOffset+m_lfWidth),(nY+m_lfHeight+m_lLineInterval));
				lineRect.NormalizeRect();
				if(bMarked)
				{
					//マーク色で塗りつぶし
					pDC->FillRect(&lineRect,&objMarkLineBrush);
				}
				else if(nDiffLineKind==DIFF_LINEKIND_DELETED)
				{
					//削除行色で塗りつぶし
					pDC->FillRect(&lineRect,&objDiffDelLineBrush);
				}
				else if(nDiffLineKind==DIFF_LINEKIND_ADDED)
				{
					//追加行色で塗りつぶし
					pDC->FillRect(&lineRect,&objDiffAddLineBrush);
				}
				else if(nDirCmpLineKind==DIRCMP_LINEKIND_DIFF)
				{
					//追加行色で塗りつぶし
					pDC->FillRect(&lineRect,&objDirDiffLineBrush);
				}
				else
				{
					//背景色で塗りつぶし
					pDC->FillRect(&lineRect,&objBackBrush);
				}

				if(bMarked)
				{
					MyDrawingCRLF(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth,
									m_lfHeight,
									TRANSPARENT,
									m_CRLFColor,
									m_MarkLineColor );
				}
				else if(nDiffLineKind==DIFF_LINEKIND_DELETED)
				{
					MyDrawingCRLF(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth,
									m_lfHeight,
									TRANSPARENT,
									m_CRLFColor,
									m_DiffDelLineColor );
				}
				else if(nDiffLineKind==DIFF_LINEKIND_ADDED)
				{
					MyDrawingCRLF(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth,
									m_lfHeight,
									TRANSPARENT,
									m_CRLFColor,
									m_DiffAddLineColor );
				}
				else if(nDirCmpLineKind==DIRCMP_LINEKIND_DIFF)
				{
					MyDrawingCRLF(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth,
									m_lfHeight,
									TRANSPARENT,
									m_CRLFColor,
									m_DirDiffLineColor );
				}
				else
				{
					MyDrawingCRLF(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth,
									m_lfHeight,
									TRANSPARENT,
									m_CRLFColor,
									m_BackColor );
				}
			}
		}
	}

	if( ((lLineNo+1)==pDoc->GetLineCount()) || //最終行の描画だった
		((pDoc->GetLineCount()==0) && (lLineNo==0))
	)
	{
		//設定読み込み
		BOOL bIsDispEOFMark = m_pApp->GetProfileInt("CEditorView","bIsDispEOFMark",1);
		if( bIsDispEOFMark != 0 )
		{
			//ＥＯＦマークを反転描画
			pDC->SetBkMode( OPAQUE );
			pDC->SetBkColor( m_InvertBackColor );
			pDC->SetTextColor( m_InvertFontColor );
			char szEOF[] = "[ EOF ]";
			pDC->TextOut((lDevX+ulOffset),nY,szEOF,strlen(szEOF));
			ulOffset += (lfWidth * strlen(szEOF));
			ulCnt+=strlen(szEOF);
		}
	}

	//行番号描画
	if( m_bDispLineNo )
	{
		if( ((lLineNo+1)<=pDoc->GetLineCount()) ||
			((pDoc->GetLineCount()==0) && (lLineNo==0))
		)
		{
			//行番号
			char	szLineNo[32];
			sprintf(szLineNo, "%6d  ", (lLineNo+1) );

			//背景色ブラシ作成＆背景塗りつぶし(1999.03.27 Modify for Smooth Horz Scrolling)
			CBrush	objBackBrush;
			objBackBrush.CreateSolidBrush(m_LineNoBackColor);
			CRect	rect( 0/*GetScrollPosLogX()*/,
						  nY,
						  (/*GetScrollPosLogX()+*/strlen(szLineNo)*m_lfWidth),
						  (nY+m_lfHeight+m_lLineInterval));
			rect.NormalizeRect();
			pDC->FillRect(&rect,&objBackBrush);

			//文字色変更
			pDC->SetBkMode( TRANSPARENT );
			pDC->SetBkColor( m_LineNoBackColor );
			pDC->SetTextColor( m_LineNoColor );

			//１文字分出力(1999.03.27 Modify for Smooth Horz Scrolling)
			pDC->TextOut(0,nY,szLineNo,strlen(szLineNo));

			//セパレータ描画(1999.03.27 Modify for Smooth Horz Scrolling)
			long lStartX = 6*m_lfWidth + m_lfWidth/2;
			long lStartY = nY;
			long lEndY   = nY + m_lfHeight + m_lLineInterval;
			CPen	objPen;
			CPen*	pobjOldPen;
			objPen.CreatePen(PS_SOLID,1,m_LineNoColor);
			pobjOldPen = pDC->SelectObject(&objPen);
			pDC->MoveTo(lStartX, lStartY);
			pDC->LineTo(lStartX, lEndY);
			pDC->SelectObject(pobjOldPen);
		}
	}

	//元に戻す
	pDC->SetBkMode( nOldBkMode );
	pDC->SetBkColor( clrOldBack );
	pDC->SetTextColor( clrOldText );
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１行描画（印刷用）処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::MyTextOutForPrint		// 表示をした最終オフセット
	(	CDC*	pDC,					// 描画用デバイスコンテキスト
		long	nX,						// テキストの開始点の論理 x 座標
		long	nY,						// テキストの開始点の論理 y 座標
		CLine*  pobjLine,				// 描画される文字列
		long	lfWidth,				// フォントの幅
		long	lLineNo,				// 行番号
		int		nLimitWidth				// 表示幅
	)
{
	ULONG	ulCnt=0;
	ULONG	ulOffset=0;
	char	szSpace[] = "            ";
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	if(!pDC->IsPrinting())
		return 0;
	if( pobjLine == NULL )
		return 0;

	//文字色設定
	COLORREF    clrOldText = pDC->GetTextColor();
	COLORREF	clrOldBack = pDC->GetBkColor();
	pDC->SetBkColor( RGB(255,255,255) );
	pDC->SetTextColor( RGB(0,0,0) );

	for(ulCnt=0; ulCnt<(ULONG)pobjLine->GetLength(); )//文字列長分ループ
	{
		if(IsDBCSLeadByteEx(0,pobjLine->GetAt(ulCnt)))//２バイト文字の第一バイト
		{
			if((ulOffset+lfWidth*2) < (ULONG)nLimitWidth) //表示幅以内
			{
				//２文字分出力
				pDC->TextOut((nX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),2);
				ulCnt+=2;
				ulOffset += (lfWidth * 2);
			}
			else
			{
				return ulCnt;
			}
		}
		else //１バイト文字
		{
			if(pobjLine->GetAt(ulCnt) == '\t')//タブコード
			{
				int nMargin = pDoc->GetTabSize() - ( (ulOffset/lfWidth) % pDoc->GetTabSize() );
				if((ulOffset+lfWidth*nMargin) < (ULONG)nLimitWidth) //表示幅以内
				{
					//描画開始位置をタブサイズ分進める
					ulOffset += (lfWidth * nMargin);
					ulCnt++;
				}
				else
				{
					return ulCnt;
				}
			}
			else //その他
			{
				if((ulOffset+lfWidth) < (ULONG)nLimitWidth) //表示幅以内
				{
					//１文字分出力
					pDC->TextOut((nX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),1);
					ulCnt++;
					ulOffset += lfWidth;
				}
				else
				{
					return ulCnt;
				}
			}
		}
	}

	//元に戻す
	pDC->SetBkColor( clrOldBack );
	pDC->SetTextColor( clrOldText );

	return ulCnt;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：改行マーク描画処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyDrawingCRLF
	(	CDC*		pDC,				//デバイスコンテキスト
		long		lX,					//描画開始位置（左上のＸ座標）
		long		lY,					//描画開始位置（左上のＹ座標）
		long		lWidth,				//描画領域の幅
		long		lHeight,			//描画領域の高さ
		int			nMode,				//BkMode
		COLORREF	foreColor,			//前面色
		COLORREF	backColor			//背景色
	)
{
	if( nMode == OPAQUE	) //描画前に塗りつぶし必要
	{
		CRect	drawRect(lX,lY,lX+lWidth,lY+lHeight);
		drawRect.NormalizeRect();
		CBrush	objBackBrush;
		objBackBrush.CreateSolidBrush(backColor);
		pDC->FillRect(&drawRect,&objBackBrush);
	}

	//座標値計算
	long lStartX = lX + (lWidth  / 4) * 3;		//矢印スタート位置
	long lStartY = lY + (lHeight / 6) * 1;
	long lEdgeX  = lX + (lWidth  / 4) * 3;		//曲がり角の位置
	long lEdgeY  = lY + (lHeight / 6) * 5;
	long lTopX   = lX + (lWidth  / 4) * 1;		//矢印先端の位置
	long lTopY   = lY + (lHeight / 6) * 5;
	long lRWingX = lX + (lWidth  / 4) * 2;		//矢印の右翼先端の位置
	long lRWingY = lY + (lHeight / 6) * 4;
	long lLWingX = lX + (lWidth  / 4) * 2;		//矢印の左翼先端の位置
	long lLWingY = lY + (lHeight / 6) * 6;

	//ペンの作成をする
	CPen	objPen;
	CPen*	pobjOldPen;
	objPen.CreatePen(PS_SOLID,1,foreColor);
	pobjOldPen = pDC->SelectObject(&objPen);

	//矢印描画
	pDC->MoveTo(lStartX, lStartY);
	pDC->LineTo(lEdgeX,  lEdgeY);				//スタート位置から曲がり角まで
	pDC->LineTo(lTopX,   lTopY);				//曲がり角から矢印の先端まで
	pDC->LineTo(lRWingX, lRWingY);				//矢印の先端から右翼先端まで
	pDC->MoveTo(lTopX,   lTopY);
	pDC->LineTo(lLWingX, lLWingY);				//矢印の先端から左翼先端まで

	//ペンを元に戻す
	pDC->SelectObject(pobjOldPen);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ルーラ描画処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyDrawingRuler
	(	CDC*		pDC,				//デバイスコンテキスト
		long		lX,					//描画開始位置（左上のＸ座標）
		long		lY,					//描画開始位置（左上のＹ座標）
		long		lWidth,				//描画領域の幅
		COLORREF	foreColor,			//前面色
		COLORREF	backColor			//背景色
	)
{
	char	szSpace[] = "            ";
	char	szBuf[4];
	ULONG	ulDigit=0;
	ULONG	ulCnt=0;
	ULONG	ulOffset=0;

	//保存
	int			nOldBkMode = pDC->GetBkMode();
	COLORREF    clrOldText = pDC->GetTextColor();
	COLORREF	clrOldBack = pDC->GetBkColor();

	//文字色変更
	pDC->SetBkMode( OPAQUE );
	pDC->SetBkColor( backColor );
	pDC->SetTextColor( foreColor );

	if( m_bDispLineNo )//行番号を表示する
	{
		ulCnt += strlen(m_szLineNoFormat);
		ulOffset += (m_lfWidth * strlen(m_szLineNoFormat));
	}

	for(;;)
	{
		//ルーラ１０文字分出力
		pDC->TextOut((lX+ulOffset-GetScrollPosLogX()),lY,m_szRuler,strlen(m_szRuler));
		ulCnt+=strlen(m_szRuler);
		ulOffset += (m_lfWidth * strlen(m_szRuler));
		//数字出力
		ulDigit++;
		if(ulDigit>=10)
			ulDigit=0;
		sprintf(szBuf,"%1d",ulDigit);
		pDC->TextOut((lX+ulOffset-GetScrollPosLogX()),lY,szBuf,1);
		ulCnt++;
		ulOffset += m_lfWidth;

		if(ulOffset > (ULONG)lWidth)
			break;
	}

	if( m_bDispLineNo )//行番号を表示する
	{
		pDC->TextOut(0,lY,m_szLineNoFormat,strlen(m_szLineNoFormat));
//		pDC->TextOut(GetScrollPosLogX(),lY,m_szLineNoFormat,strlen(m_szLineNoFormat));
	}

	//アンダーラインを描画する
	CPen	objPen;
	CPen*	pobjOldPen;
	objPen.CreatePen(PS_SOLID,1,foreColor);
	pobjOldPen = pDC->SelectObject(&objPen);
	pDC->MoveTo(lX, lY+m_lfHeight-1);
	pDC->LineTo(lX+lWidth, lY+m_lfHeight-1);
	pDC->SelectObject(pobjOldPen);

	//元に戻す
	pDC->SetBkMode( nOldBkMode );
	pDC->SetBkColor( clrOldBack );
	pDC->SetTextColor( clrOldText );
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：全角スペース描画処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyDrawingZenkakuSpace
	(	CDC*		pDC,				// デバイスコンテキスト
		long		lX,					// 描画開始位置（左上のＸ座標）
		long		lY,					// 描画開始位置（左上のＹ座標）
		long		lWidth,				// 描画領域の幅
		long		lHeight,			// 描画領域の高さ
		COLORREF	foreColor,			// 前面色
		COLORREF	backColor			// 背景色
	)
{
	//座標値計算
	long lEdge1X = lX + (lWidth  / 6) * 1;		//左上
	long lEdge1Y = lY + (lHeight / 6) * 1;
	long lEdge2X = lX + (lWidth  / 6) * 1;		//左下
	long lEdge2Y = lY + (lHeight / 6) * 6;
	long lEdge3X = lX + (lWidth  / 6) * 5;		//右下
	long lEdge3Y = lY + (lHeight / 6) * 6;
	long lEdge4X = lX + (lWidth  / 6) * 5;		//右上
	long lEdge4Y = lY + (lHeight / 6) * 1;

	//ペンの作成をする
	CPen	objPen;
	CPen*	pobjOldPen;
	objPen.CreatePen(PS_SOLID,1,foreColor);
	pobjOldPen = pDC->SelectObject(&objPen);

	//全角スペース描画
	pDC->MoveTo(lEdge1X, lEdge1Y);
	pDC->LineTo(lEdge2X, lEdge2Y);
	pDC->LineTo(lEdge3X, lEdge3Y);
	pDC->LineTo(lEdge4X, lEdge4Y);
	pDC->LineTo(lEdge1X, lEdge1Y);

	//ペンを元に戻す
	pDC->SelectObject(pobjOldPen);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：タブコード描画処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyDrawingTab
	(	CDC*		pDC,				// デバイスコンテキスト
		long		lX,					// 描画開始位置（左上のＸ座標）
		long		lY,					// 描画開始位置（左上のＹ座標）
		long		lWidth,				// 描画領域の幅
		long		lHeight,			// 描画領域の高さ
		COLORREF	foreColor,			// 前面色
		COLORREF	backColor			// 背景色
	)
{
	//座標値計算
	long lStartX, lStartY;
	long lTopX,   lTopY;
	long lRWingX, lRWingY;
	long lLWingX, lLWingY;
	long lUpX,    lUpY;
	long lDownX,  lDownY;
	lStartX = lX + (m_lfWidth / 6) * 1;				//矢印スタート位置
	lStartY = lY + (lHeight   / 4) * 2;
	lTopX   = lX + lWidth - (m_lfWidth / 6) * 1;	//矢印先端の位置
	lTopY   = lY + (lHeight   / 4) * 2;
	lRWingX = lX + lWidth - (m_lfWidth / 6) * 4;
	lRWingY = lY + (lHeight   / 4) * 3;
	lLWingX = lX + lWidth - (m_lfWidth / 6) * 4;
	lLWingY = lY + (lHeight   / 4) * 1;
	lUpX    = lX + (m_lfWidth / 6) * 1;
	lUpY    = lY + (lHeight   / 4) * 1;
	lDownX  = lX + (m_lfWidth / 6) * 1;
	lDownY  = lY + (lHeight   / 4) * 3;

	//ペンの作成をする
	CPen	objPenSolid, objPenDot;
	CPen*	pobjOldPen;
	objPenSolid.CreatePen(PS_SOLID,1,foreColor);
	objPenDot.CreatePen(PS_DOT,1,foreColor);

	//タブ描画
	pobjOldPen = pDC->SelectObject(&objPenDot);
	pDC->MoveTo(lStartX, lStartY);
	pDC->LineTo(lTopX,   lTopY);

	pDC->SelectObject(&objPenSolid);
	pDC->MoveTo(lRWingX, lRWingY);
	pDC->LineTo(lTopX,   lTopY);
	pDC->LineTo(lLWingX, lLWingY);
	pDC->MoveTo(lUpX,    lUpY);
	pDC->LineTo(lDownX,  lDownY);

	//ペンを元に戻す
	pDC->SelectObject(pobjOldPen);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：アンダーライン描画処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyDrawingUnderLine
	(	CDC*		pDC					// デバイスコンテキスト
	)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	long lStartX, lStartY, lEndX, lEndY;
	CPen	objPen;
	CPen*	pobjOldPen=NULL;

	//最大幅計算
	ULONG ulRight = 0;
	if( ((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth) > clientRect.Width() )
		ulRight = (ULONG)((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth);
	else
		ulRight = clientRect.Width();

	if(m_bDispLineNo)
	{
		objPen.CreatePen(PS_SOLID,1,m_UnderLineColor);
		pobjOldPen = pDC->SelectObject(&objPen);
		lStartY = m_caretPos.y + m_lfHeight + 1;//m_lLineInterval;
		lEndY   = lStartY;
		lStartX = 0;
		lEndX   = m_lfWidth*6;
		pDC->MoveTo(lStartX, lStartY);
		pDC->LineTo(lEndX,   lStartY);
		lStartX = m_LineNoPos.x;
		lEndX   = ulRight;
		pDC->MoveTo(lStartX, lStartY);
		pDC->LineTo(lEndX,   lStartY);
		pDC->SelectObject(pobjOldPen);
	}
	else
	{
		objPen.CreatePen(PS_SOLID,1,m_UnderLineColor);
		pobjOldPen = pDC->SelectObject(&objPen);
		lStartX = 0;
		lEndX   = ulRight;
		lStartY = m_caretPos.y + m_lfHeight + 1;//m_lLineInterval;
		lEndY   = lStartY;
		pDC->MoveTo(lStartX, lStartY);
		pDC->LineTo(lEndX,   lStartY);
		pDC->SelectObject(pobjOldPen);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：アンダーライン消去処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyEraseUnderLine
	(	CDC*		pDC					// デバイスコンテキスト
	)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	long lStartX, lStartY, lEndX, lEndY;
	CPen	objPen, objPen2;
	CPen*	pobjOldPen=NULL;

	//最大幅計算
	ULONG ulRight = 0;
	if( ((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth) > clientRect.Width() )
		ulRight = (ULONG)((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth);
	else
		ulRight = clientRect.Width();

	if(m_bDispLineNo)
	{
		objPen.CreatePen(PS_SOLID,1,m_LineNoBackColor);
		pobjOldPen = pDC->SelectObject(&objPen);
		lStartY = m_caretPos.y + m_lfHeight + 1;//m_lLineInterval;
		lEndY   = lStartY;
		lStartX = 0;
		lEndX   = m_lfWidth*6;
		pDC->MoveTo(lStartX, lStartY);
		pDC->LineTo(lEndX,   lStartY);

		objPen2.CreatePen(PS_SOLID,1,m_BackColor);
		pDC->SelectObject(&objPen2);
		lStartX = m_LineNoPos.x;
		lEndX   = ulRight;
		pDC->MoveTo(lStartX, lStartY);
		pDC->LineTo(lEndX,   lStartY);
		pDC->SelectObject(pobjOldPen);
	}
	else
	{
		objPen.CreatePen(PS_SOLID,1,m_BackColor);
		pobjOldPen = pDC->SelectObject(&objPen);
		lStartX = 0;
		lEndX   = ulRight;
		lStartY = m_caretPos.y + m_lfHeight + 1;//m_lLineInterval;
		lEndY   = lStartY;
		pDC->MoveTo(lStartX, lStartY);
		pDC->LineTo(lEndX,   lStartY);
		pDC->SelectObject(pobjOldPen);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：選択範囲内判定処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::IsWithinSelectArea	//ＴＲＵＥ：選択範囲内、ＦＡＬＳＥ：選択範囲外
	(	long	lSelectStartLine,		//選択開始行番号
		long	lSelectStartOffset,		//選択開始列番号
		long	lSelectEndLine,			//選択終了行番号
		long	lSelectEndOffset,		//選択終了列番号
		long	lCurrentLine,			//現在行番号
		long	lCurrentOffset			//現在列番号
	)
{
	if( (lSelectStartLine   == -1) || (lSelectEndLine   == -1) ||		//範囲選択なし
		(lSelectStartOffset == -1) || (lSelectEndOffset == -1)
	)
	{
		return FALSE;
	}
	if( (lCurrentLine < lSelectStartLine) || (lCurrentLine > lSelectEndLine) )	//範囲選択外
		return FALSE;
	if(lSelectStartLine != lSelectEndLine) //複数行選択
	{
		if(lCurrentLine == lSelectStartLine) //描画する１行が選択開始行
		{
			if( lSelectStartOffset <= lCurrentOffset) //選択範囲内
				return TRUE;
			else
				return FALSE;
		}
		else if(lCurrentLine == lSelectEndLine) //描画する１行が選択終了行
		{
			if( lCurrentOffset < lSelectEndOffset ) //選択範囲内
				return TRUE;
			else
				return FALSE;
		}
		else if( (lCurrentLine > lSelectStartLine) && (lCurrentLine < lSelectEndLine) ) //描画する１行全体が選択範囲内
			return TRUE;
	}
	else //１行選択
	{
		if( (lSelectStartOffset <= lCurrentOffset) &&	 //選択範囲内
			(lCurrentOffset     <  lSelectEndOffset )
		)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：選択範囲の再描画処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::RedrawSelectArea
	(	long	lPreMoveCaretLine,		// キャレット移動前の行位置
		long	lPreMoveCaretOffset		// キャレット移動前の列位置
	)
{
	long lRedrawLineCnt=0;

	//キャレット移動後の行番号と列番号取得
	long lPostMoveCaretLine=0;
	long lPostMoveCaretOffset=0;
	if( CaretToLineOffset(&lPostMoveCaretLine,&lPostMoveCaretOffset) == FALSE )
		return;

	//キャレット移動前と後が同じ場合は再描画しない
	if( lPreMoveCaretLine==lPostMoveCaretLine && lPreMoveCaretOffset==lPostMoveCaretOffset )
		return;

	if(m_lSelectEndLine == -1 && m_lSelectEndOffset == -1) //範囲選択初回
	{
		//移動していない場合は再描画しない
		if(m_lSelectStartLine==lPostMoveCaretLine && m_lSelectStartOffset==lPostMoveCaretOffset)
			return;

		//選択終了のセット
		m_lSelectEndLine = lPostMoveCaretLine;
		m_lSelectEndOffset = lPostMoveCaretOffset;

		//キャレットの周囲をふくめて再描画
		if(m_lSelectStartLine == m_lSelectEndLine)
		{
			AdjustSelectArea();
			lRedrawLineCnt = 0;
		}
		else if(m_lSelectStartLine > m_lSelectEndLine) //下から上へ向かう
		{
			AdjustSelectArea();
			lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
		}
		else if(m_lSelectStartLine < m_lSelectEndLine) //上から下へ向かう
		{
			AdjustSelectArea();
			lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
		}
		OnUpdate(NULL,NULL,NULL);
		InvalidateCaretLineEx(lRedrawLineCnt);
	}
	else //範囲選択２回目以降
	{
		//下から上に向かう場合プラス値、上から下へ向かう場合マイナス値
		lRedrawLineCnt = lPreMoveCaretLine - lPostMoveCaretLine;

		//選択範囲再設定
		if(lPreMoveCaretLine==m_lSelectStartLine && lPreMoveCaretOffset==m_lSelectStartOffset)
		{
			m_lSelectStartLine = lPostMoveCaretLine;
			m_lSelectStartOffset = lPostMoveCaretOffset;
		}
		else if(lPreMoveCaretLine==m_lSelectEndLine && lPreMoveCaretOffset==m_lSelectEndOffset)
		{
			m_lSelectEndLine = lPostMoveCaretLine;
			m_lSelectEndOffset = lPostMoveCaretOffset;
		}
		else
		{
			return;
		}

		//選択範囲調整
		AdjustSelectArea();

		//キャレットの周囲をふくめて再描画
		OnUpdate(NULL,NULL,NULL);
		InvalidateCaretLineEx(lRedrawLineCnt);
	}
}

/////////////////////////////////////////////////////////////////////////////
//	CEditorView プロテクト関数（キャレット関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：対応する括弧にキャレットを位置づける処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MySetCaretToMatchBrace
	(	BOOL	bSelect				//範囲選択もするか？ ＦＡＬＳＥ：しない、ＴＲＵＥ：する
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CString objMsg;

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	// キャレット位置の行取得
	CLine*	pobjLine = NULL;
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
		return;

	// キャレット位置の文字取得
	UCHAR unCurrentChar = pobjLine->GetAt(lCurrentOffset);
	if( IsDBCSLeadByteEx(0,unCurrentChar) )
		return;
	if( unCurrentChar==0 )
		return;
	if( unCurrentChar!='(' &&
		unCurrentChar!=')' &&
		unCurrentChar!='{' &&
		unCurrentChar!='}' &&
		unCurrentChar!='[' &&
		unCurrentChar!=']' &&
		unCurrentChar!='<' &&
		unCurrentChar!='>'
	)
	{
		return;
	}

	// 対応する括弧の検索＆キャレット移動
	long lLine=lCurrentLine;
	long lOffset=lCurrentOffset;
	UCHAR unChar;
	if( unCurrentChar=='(' ||  //キャレット位置の文字が括弧開始
		unCurrentChar=='{' ||
		unCurrentChar=='[' ||
		unCurrentChar=='<'
	)
	{
		UCHAR	unParenStart,unParenEnd;
		if( unCurrentChar == '(' )
		{
			unParenStart = '(';
			unParenEnd = ')';
		}
		else if( unCurrentChar == '{' )
		{
			unParenStart = '{';
			unParenEnd = '}';
		}
		else if( unCurrentChar == '[' )
		{
			unParenStart = '[';
			unParenEnd = ']';
		}
		else if( unCurrentChar == '<' )
		{
			unParenStart = '<';
			unParenEnd = '>';
		}

		lOffset++; // 次へ位置づけ
		ULONG nParenCnt=0; // 括弧開始の数初期化
		while(pobjLine != NULL)
		{
			for(;;)
			{
				unChar = pobjLine->GetAt(lOffset);
				if(unChar==0) //行終了
					break;
				if( IsDBCSLeadByteEx(0,unChar) ) //２バイト文字
				{
					lOffset+=2;
					continue;
				}
				else if( unChar==unParenStart ) //１バイト文字、括弧開始
				{
					nParenCnt++; // 括弧開始の数カウントアップ
					lOffset++;
					continue;
				}
				else if( unChar==unParenEnd ) //１バイト文字、括弧終了
				{
					if( nParenCnt > 0 )
					{
						nParenCnt--; // 括弧開始の数カウントダウン
						lOffset++;
						continue;
					}
					else if( nParenCnt == 0 ) //対応する括弧発見
					{
						//その位置へキャレットを移動させる
						Jump2LineOffsetEx(lLine,lOffset);
						if(bSelect)
						{
							m_lSelectStartLine = lCurrentLine;
							m_lSelectStartOffset = lCurrentOffset;
							m_lSelectEndLine = lLine;
							m_lSelectEndOffset = lOffset;
							//再描画
							OnUpdate(NULL,NULL,NULL);
							Invalidate(FALSE);
						}
						return;
					}
				}
				else //括弧以外の１バイト文字
				{
					lOffset++;
					continue;
				}
			}

			//次の行取得
			lOffset=0;
			lLine++;
			pobjLine = pDoc->GetOneLine(lLine);
		}
		objMsg.LoadString(IDS_PAREN_NOTFOUND);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		return;
	}
	else if( unCurrentChar==')' ||  //キャレット位置の文字が括弧終了
			 unCurrentChar=='}' ||
			 unCurrentChar==']' ||
			 unCurrentChar=='>'
	)
	{
		UCHAR	unParenStart,unParenEnd;
		if( unCurrentChar == ')' )
		{
			unParenStart = '(';
			unParenEnd = ')';
		}
		else if( unCurrentChar == '}' )
		{
			unParenStart = '{';
			unParenEnd = '}';
		}
		else if( unCurrentChar == ']' )
		{
			unParenStart = '[';
			unParenEnd = ']';
		}
		else if( unCurrentChar == '>' )
		{
			unParenStart = '<';
			unParenEnd = '>';
		}

		lOffset = GetPreOffset(lLine,lOffset); // 前へ位置づけ
		int nParenCnt=0; // 括弧終了の数初期化
		while(pobjLine != NULL)
		{
			for(;;)
			{
				if( lOffset == -1 ) //行終了
					break;
				unChar = pobjLine->GetAt(lOffset);
				if( IsDBCSLeadByteEx(0,unChar) ) //２バイト文字
				{
					lOffset = GetPreOffset(lLine,lOffset);
					continue;
				}
				else if( unChar==unParenEnd ) //１バイト文字、括弧終了
				{
					nParenCnt++; // 括弧終了の数カウントアップ
					lOffset = GetPreOffset(lLine,lOffset);
					continue;
				}
				else if( unChar==unParenStart ) //１バイト文字、括弧開始
				{
					if( nParenCnt > 0 )
					{
						nParenCnt--; // 括弧終了の数カウントダウン
						lOffset = GetPreOffset(lLine,lOffset);
						continue;
					}
					else if( nParenCnt == 0 ) //対応する括弧発見
					{
						//その位置へキャレットを移動させる
						Jump2LineOffsetEx(lLine,lOffset);
						if(bSelect)
						{
							m_lSelectStartLine = lCurrentLine;
							m_lSelectStartOffset = lCurrentOffset;
							m_lSelectEndLine = lLine;
							m_lSelectEndOffset = lOffset;
							//再描画
							OnUpdate(NULL,NULL,NULL);
							Invalidate(FALSE);
						}
						return;
					}
				}
				else //括弧以外の１バイト文字
				{
					lOffset = GetPreOffset(lLine,lOffset);
					continue;
				}
			}

			//前の行取得
			lLine--;
			if( lLine < 0 )
				break;
			pobjLine = pDoc->GetOneLine(lLine);
//1998.10.29 Modify Start By S.Ekura
//			lOffset=pobjLine->GetLength()-1;
			lOffset=pobjLine->GetLength();		 // 行末へ位置づけ
			lOffset=GetPreOffset(lLine,lOffset); // 前へ位置づけ
//1998.10.29 Modify End
		}
		objMsg.LoadString(IDS_PAREN_NOTFOUND);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		return;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：指定行、列にキャレットを位置づける処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MySetCaretToLineOffset
	(	long	lDestLine,
		long	lDestOffset
	)
{
	if( (m_lfWidth==0) || (m_lfHeight==0) )
		return;

	//ｽｸﾛｰﾙ位置行番号
	long lScrollLine = GetScrollPosLogY() / (m_lfHeight+m_lLineInterval);
	if( lDestLine < lScrollLine )
		return;

	//キャレット位置のＹ座標
	long lY = (lDestLine - lScrollLine) * (m_lfHeight+m_lLineInterval) + m_RulerPos.y;
	CPoint	tempPos(0,lY);

	//キャレット位置のＸ座標（論理座標）
	GetPosition3(lDestLine,lDestOffset,&tempPos);
	m_nLogPreCaretX = tempPos.x; //論理Ｘ座標値を記憶

	//キャレット移動
	tempPos.x += m_LineNoPos.x;
	tempPos.x -= GetScrollPosLogX();//(1999.03.06 Add)
	MySetCaretPos(tempPos); //デバイス座標指定
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット位置設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MySetCaretPos
	(	CPoint&		point			//デバイス位置
	)
{
	long lMaxLine = 0;
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//事前チェック
	if(point.x < 0 || point.y < 0)
		return;

	if(m_bDispUnderLine) //アンダーラインを表示する
	{
		if(m_caretPos.y != point.y) //Ｙ座標が異なる位置へキャレットが移動する
		{
			//前回のキャレット位置のアンダーラインを消去
			CClientDC	dc(this);
			CPoint ptVpOrg(0, 0);
			ptVpOrg.x = -1 * GetScrollPosLogX();
			dc.SetViewportOrg(ptVpOrg);
			MyEraseUnderLine(&dc);
		}
		//キャレット位置設定
		m_caretPos = point;
		if(m_bInsertKey) //挿入モード
		{
			if(m_nCaretType==CARET_IBEAM) //縦棒キャレット
			{
				SetCaretPos(point);
			}
			else if(m_nCaretType==CARET_SQUARE) //箱型キャレット
			{	//キャレットを１行の下半分に位置付ける（表示上のみ）
				CPoint tempPos = point;
				tempPos.y += (m_lfHeight/2);
				SetCaretPos(tempPos);
			}
		}
		else //上書きモード
		{
			SetCaretPos(point);
		}
		//キャレット位置にアンダーラインを描画
		CClientDC	dc(this);
		CPoint ptVpOrg(0, 0);
		ptVpOrg.x = -1 * GetScrollPosLogX();
		dc.SetViewportOrg(ptVpOrg);
		MyDrawingUnderLine(&dc);
	}
	else //アンダーラインを表示しない
	{
		//キャレット位置設定
		m_caretPos = point;
		if(m_bInsertKey) //挿入モード
		{
			if(m_nCaretType==CARET_IBEAM) //縦棒キャレット
			{
				SetCaretPos(point);
			}
			else if(m_nCaretType==CARET_SQUARE) //箱型キャレット
			{	//キャレットを１行の下半分に位置付ける（表示上のみ）
				CPoint tempPos = point;
				tempPos.y += (m_lfHeight/2);
				SetCaretPos(tempPos);
			}
		}
		else //上書きモード
		{
			SetCaretPos(point);
		}
	}

	//////////////////////////////////////////////////////////////////
	//	ステータスバーペインにキャレット位置の行列番号を表示する	//
	//////////////////////////////////////////////////////////////////

	if(pWnd->IsDispIndicatorGyoretu())
	{
		//	現在のキャレット位置の行、列番号取得
		lCurrentLine = 0;
		lCurrentOffset = 0;
		if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
			return;
		lMaxLine = pDoc->GetLineCount();

		CPoint retPoint(0,0);
		CPoint logicalPoint = m_caretPos - m_LineNoPos - m_RulerPos;
		logicalPoint.x += GetScrollPosLogX();
		logicalPoint.y += GetScrollPosLogY();
		GetPosition2(lCurrentLine,&logicalPoint,&retPoint);
		if( m_lfWidth != 0 )
			lCurrentOffset = retPoint.x / m_lfWidth;
		else
			lCurrentOffset = 0;

		//ステータスバーペインにキャレット位置の行列番号を表示する
		pWnd->SetGyoRetu((lCurrentLine+1),(lCurrentOffset+1),lMaxLine);
	}

	//////////////////////////////////////////////////////////////////
	//	ステータスバーペインにキャレット位置のバイト数を表示する	//
	//////////////////////////////////////////////////////////////////

	if(pWnd->IsDispIndicatorByte())
	{
		//	現在のキャレット位置の行、列番号取得
		lCurrentLine = 0;
		lCurrentOffset = 0;
		if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
			return;
		long lSumByte = 0;
		long lCaretByte = 0;
		pDoc->GetBytes(lCurrentLine,&lCaretByte,&lSumByte);
		lCaretByte += lCurrentOffset;
		pWnd->SetCaretPosByte(lCaretByte,lSumByte);
	}

	//////////////////////////////////////////////////////////////
	//	ステータスバーペインにキャレット位置のコードを表示する	//
	//////////////////////////////////////////////////////////////

	if(pWnd->IsDispIndicatorCode())
	{
		//	現在のキャレット位置の行、列番号取得
		lMaxLine = pDoc->GetLineCount();
		lCurrentLine = 0;
		lCurrentOffset = 0;
		if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
			return;
		CLine* pobjLine = pDoc->GetOneLine(lCurrentLine);
		if(pobjLine != NULL)
		{
			if(lCurrentOffset == pobjLine->GetLength())
			{
				if(lCurrentLine+1 == lMaxLine)
					pWnd->SetCaretPosCharCode(CODE_EOF,NULL);
				else
				{
					long lKaigyoKind = pDoc->GetKaigyoKind();
					if(lKaigyoKind == KAIGYO_CRLF)
						pWnd->SetCaretPosCharCode(CODE_CR,CODE_LF);
					else if(lKaigyoKind == KAIGYO_LF)
						pWnd->SetCaretPosCharCode(CODE_LF,NULL);
					else if(lKaigyoKind == KAIGYO_CR)
						pWnd->SetCaretPosCharCode(CODE_CR,NULL);
					else
						pWnd->SetCaretPosCharCode(CODE_CR,CODE_LF);
				}
			}
			else
			{
				UCHAR	uc1stChar = pobjLine->GetAt(lCurrentOffset);
				UCHAR	uc2ndChar = pobjLine->GetAt(lCurrentOffset+1);
				if(IsDBCSLeadByteEx(0,uc1stChar))
					pWnd->SetCaretPosCharCode(uc1stChar,uc2ndChar);
				else
					pWnd->SetCaretPosCharCode(uc1stChar,NULL);
			}
		}
	}

	//////////////////////////////////////
	//	ＩＭＭ変換ウインドウ位置調整	//
	//////////////////////////////////////

	if( m_bIsImmOpen )
		CgAdjustCompositionWindow();

	//ステータスバーペイン設定
	pWnd->SetInsMode(m_bInsertKey);
	pWnd->SetReadOnlyMode(pDoc->IsReadOnly());
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット移動処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MoveCaret(UINT nChar, int nRepCnt)
{
	CPoint	objPoint;
	CPoint	objCaretPos = m_caretPos;

	//キャレット位置を論理座標へ変換する
	CPoint	logicalPoint = objCaretPos;
	logicalPoint -= m_LineNoPos;
	logicalPoint -= m_RulerPos;
	logicalPoint.x += GetScrollPosLogX();
	logicalPoint.y += GetScrollPosLogY();

	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	if( (m_lfWidth==0) || (m_lfHeight==0) )
		return;
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	long lLineLength    = pDoc->GetLineLength(lCurrentLine);
	long lMaxLine       = pDoc->GetLineCount();

	switch(nChar)
	{
	case VK_LEFT:	//左矢印キーに対応するキャレット移動処理
		if(logicalPoint.x > 0) //行の先頭でない
		{
			//２バイト文字やタブコードを考慮し、１文字前のキャレット位置を取得
			CPoint tempPos(0,logicalPoint.y);
			GetPosition1(lCurrentLine,&logicalPoint,&tempPos);
			objCaretPos = tempPos;
			m_nLogPreCaretX = tempPos.x;//論理Ｘ位置を記憶
			//論理座標－＞デバイス座標
			objCaretPos.x -= GetScrollPosLogX();
			objCaretPos.y -= GetScrollPosLogY();
			objCaretPos += m_RulerPos;
			objCaretPos += m_LineNoPos;
			if(objCaretPos.x < m_LineNoPos.x)//表示範囲の左側へ行った場合
			{
				//左方向スクロール
				long lLogX = objCaretPos.x - m_LineNoPos.x - m_RulerPos.x + GetScrollPosLogX();
				MyScrollToPosition(CPoint(lLogX, GetScrollPosLogY()));
				objCaretPos.x = m_LineNoPos.x;
			}
		}
		else //行頭での左矢印キー
		{
			if(lCurrentLine > 0)//まだ上へ行ける
			{
				//MyScrollToPotisionを１回のコールにするための処理
				CPoint	destPos(GetScrollPosLogX(),GetScrollPosLogY());
				//キャレットを文字列の最後尾へ
				CPoint	tempLogPos(0,logicalPoint.y-(m_lfHeight+m_lLineInterval));
				GetPosition(lCurrentLine-1,&tempLogPos);//行末の位置取得
				if(tempLogPos.x > nWidth)//表示範囲の右側へ行った場合
				{
					//右方向スクロール
					long lLogX = tempLogPos.x - nWidth + 1;
					destPos.x = lLogX;
				}
				m_nLogPreCaretX = tempLogPos.x;//論理Ｘ位置を記憶
				//論理座標－＞デバイス座標
				objCaretPos = tempLogPos;
				objCaretPos -= destPos;/*m_ScrollPosLog;*/
				objCaretPos += m_RulerPos;
				objCaretPos += m_LineNoPos;
				if(objCaretPos.y < m_RulerPos.y )//表示範囲の上側に行った場合
				{
					objCaretPos.y = m_RulerPos.y;
					//１行分上方向スクロール
					destPos.y -= (m_lfHeight+m_lLineInterval);
				}
				if( destPos.x != GetScrollPosLogX() ||
					destPos.y != GetScrollPosLogY()
				)
				{
					MyScrollToPosition(destPos);
				}
			}
		}
		break;
	case VK_RIGHT:	//右矢印キーに対応するキャレット移動処理
		if(lCurrentOffset < lLineLength)//文字列長を超えていない
		{
			int nMargin=0;
			if(pDoc->MyIsDBCSLeadByteEx(lCurrentLine,lCurrentOffset))//キャレット位置が２バイト文字
			{
				//キャレットを２つ後ろへ
				logicalPoint.x += (m_lfWidth * 2);
				nMargin = (m_lfWidth * 2);
			}
			else //キャレット位置が１バイト文字
			{
				if(pDoc->IsTabCode(lCurrentLine,lCurrentOffset))//タブコード
				{
					int n = pDoc->GetTabSize() - ( (logicalPoint.x/m_lfWidth) % pDoc->GetTabSize() );
					logicalPoint.x += (m_lfWidth * n);
					nMargin = (m_lfWidth * n);
				}
				else //タブコード以外
				{
					logicalPoint.x += m_lfWidth;
					nMargin = m_lfWidth;
				}
			}
			if( logicalPoint.x > nWidth ) //表示範囲の右側へ行った場合
			{
				//右方向スクロール
				MyScrollToPosition(CPoint(GetScrollPosLogX()+nMargin, GetScrollPosLogY()));
			}
			m_nLogPreCaretX = logicalPoint.x;//論理Ｘ位置を記憶
			//論理座標－＞デバイス座標
			objCaretPos = logicalPoint;
			objCaretPos.x -= GetScrollPosLogX();
			objCaretPos.y -= GetScrollPosLogY();
			objCaretPos += m_RulerPos;
			objCaretPos += m_LineNoPos;
		}
		else //行末での右矢印キー
		{
			if(lCurrentLine+1 < lMaxLine)//まだ下へ行ける
			{
				//MyScrollToPositionを１回のみコールするため
				CPoint	destPos(GetScrollPosLogX(),GetScrollPosLogY());
				m_nLogPreCaretX = 0;//論理Ｘ位置を記憶
				//キャレットを行の先頭へ
				objCaretPos.x = m_LineNoPos.x;
				if(GetScrollPosLogX() > 0)
				{
					//左スクロール
					destPos.x = 0;
				}
				//キャレットを次の行へ
				if((objCaretPos.y+(m_lfHeight+m_lLineInterval)*2) > nHeight)//表示範囲の下側に行った場合
				{
					//１行分下方向スクロール
					destPos.y += (m_lfHeight+m_lLineInterval);
				}
				else
				{
					objCaretPos.y += (m_lfHeight+m_lLineInterval);
				}

				if( destPos.x != GetScrollPosLogX() ||
					destPos.y != GetScrollPosLogY()
				)
				{
					MyScrollToPosition( destPos );
				}
			}
		}
		break;
	case VK_DOWN: //下矢印キーに対応するキャレット移動処理
		if(lCurrentLine+nRepCnt < lMaxLine)//まだ下へ行ける
		{
			//MyScrollToPositionを１回のコールにするための処理
			CPoint destPos(GetScrollPosLogX(),GetScrollPosLogY());
			if((objCaretPos.y+(m_lfHeight+m_lLineInterval)*(nRepCnt+1)) > nHeight)//表示範囲の下側に行った場合
			{
				//１行分下方向スクロール
				destPos.y += (m_lfHeight+m_lLineInterval)*nRepCnt;
			}
			else
			{
				objCaretPos.y += (m_lfHeight+m_lLineInterval)*nRepCnt;
			}

			//２バイト文字やタブコードを考慮し、キャレット位置を取得
			CPoint tempPos(0,0);
			if(m_nCaretMoveType == CARET_MOVE_WORDTYPE)
				logicalPoint.x = m_nLogPreCaretX; //キャレット論理位置を復元
			GetPosition2(lCurrentLine+nRepCnt,&logicalPoint,&tempPos);
			objCaretPos.x = tempPos.x;
			//論理座標－＞デバイス座標
			objCaretPos.x -= GetScrollPosLogX();
			objCaretPos.x += m_RulerPos.x;
			objCaretPos.x += m_LineNoPos.x;
			if(objCaretPos.x < m_LineNoPos.x)//表示範囲の左側へ行った場合
			{
				//左方向スクロール
				long lLogX = objCaretPos.x - m_LineNoPos.x - m_RulerPos.x + GetScrollPosLogX();
				destPos.x = lLogX;
				objCaretPos.x = m_LineNoPos.x;
			}
			if(tempPos.x > (nWidth+GetScrollPosLogX()))//表示範囲の右側へ行った場合
			{
				//右方向スクロール
				long lLogX = tempPos.x - nWidth - GetScrollPosLogX() + m_lfWidth;
				destPos.x += lLogX;
				objCaretPos.x -= lLogX;
			}

			if( destPos.x != GetScrollPosLogX() ||
				destPos.y != GetScrollPosLogY()
			)
			{
				MyScrollToPosition( destPos );
			}
		}
		break;
	case VK_UP: //上矢印キーに対応するキャレット移動処理
		if(lCurrentLine-nRepCnt >= 0)//まだ上へ行ける
		{
			//MyScrollToPosition を１回のみのコールとするための処理
			CPoint	destPos(GetScrollPosLogX(),GetScrollPosLogY());
			objCaretPos.y -= (m_lfHeight+m_lLineInterval)*nRepCnt;
			if(objCaretPos.y < m_RulerPos.y )//表示範囲の上側に行った場合
			{
				objCaretPos.y = m_RulerPos.y;
				//nRepCnt行分上方向スクロール
				destPos.y -= (m_lfHeight+m_lLineInterval)*nRepCnt;
			}

			//２バイト文字やタブコードを考慮し、キャレット位置を取得
			CPoint tempPos(0,0);
			if(m_nCaretMoveType == CARET_MOVE_WORDTYPE)
				logicalPoint.x = m_nLogPreCaretX; //キャレット論理位置を復元
			GetPosition2(lCurrentLine-nRepCnt,&logicalPoint,&tempPos);
			objCaretPos.x = tempPos.x;
			//論理座標－＞デバイス座標
			objCaretPos.x -= GetScrollPosLogX();
			objCaretPos.x += m_RulerPos.x;
			objCaretPos.x += m_LineNoPos.x;
			if(objCaretPos.x < m_LineNoPos.x)//表示範囲の左側へ行った場合
			{
				//左方向スクロール
				long lLogX = objCaretPos.x - m_LineNoPos.x - m_RulerPos.x + GetScrollPosLogX();
				destPos.x = lLogX;
				objCaretPos.x = m_LineNoPos.x;
			}
			if(tempPos.x > (nWidth+GetScrollPosLogX()))//表示範囲の右側へ行った場合
			{
				//右方向スクロール
				long lLogX = tempPos.x - nWidth - GetScrollPosLogX() + m_lfWidth;
				destPos.x += lLogX;
				objCaretPos.x -= lLogX;
			}

			if( destPos.x != GetScrollPosLogX() ||
				destPos.y != GetScrollPosLogY()
			)
			{
				MyScrollToPosition( destPos );
			}
		}
		break;
	case VK_HOME: //HOMEキーに対応するキャレット移動処理
		m_nLogPreCaretX = 0;//論理Ｘ位置を記憶
		//キャレットを行頭へ
		objCaretPos.x = m_LineNoPos.x;
		MyScrollToPosition(CPoint(0,GetScrollPosLogY()));
		break;
	case VK_END: //ENDキーに対応するキャレット移動処理
		//キャレットを文字列の最後尾へ
		{
			CPoint	tempLogPos(0,logicalPoint.y);
			GetPosition(lCurrentLine,&tempLogPos);//行末の位置取得
			if(tempLogPos.x > nWidth)//表示範囲の右側へ行った場合
			{
				//右方向スクロール
				long lLogX = tempLogPos.x - nWidth + 1;
				MyScrollToPosition(CPoint(lLogX, GetScrollPosLogY()));
			}
			m_nLogPreCaretX = tempLogPos.x;//論理Ｘ位置を記憶
			//論理座標－＞デバイス座標
			objCaretPos = tempLogPos;
			objCaretPos.x -= GetScrollPosLogX();
			objCaretPos.y -= GetScrollPosLogY();
			objCaretPos += m_RulerPos;
			objCaretPos += m_LineNoPos;
		}
		break;
	case VK_PRIOR: //PAGE UPキーに対応するキャレット移動処理
		if(lCurrentLine > 0)//まだ上へ行ける
		{
			//１０行分上方向スクロール
			CPoint objScrollPos(GetScrollPosLogX(),GetScrollPosLogY());
			objScrollPos.x = 0;
			objCaretPos.x = m_LineNoPos.x;
			objScrollPos.y -= (m_VPageLog.cy * nRepCnt);
			if(objScrollPos.y < 0)
			{
				objScrollPos.y = 0;
				objCaretPos.y = m_RulerPos.y;		//キャレット位置先頭移動
			}
			MyScrollToPosition(objScrollPos);
		}
		break;
	case VK_NEXT: //PAGE DOWNキーに対応するキャレット移動処理
		CPoint objScrollPos(GetScrollPosLogX(),GetScrollPosLogY());
		objScrollPos.x = 0;
		objCaretPos.x = m_LineNoPos.x;
		if( (objScrollPos.y + nHeight + (m_VPageLog.cy * nRepCnt)) < m_totalLog.cy )
		{
			objScrollPos.y += (m_VPageLog.cy * nRepCnt);
		}
		else
		{
			while( (objScrollPos.y + nHeight + m_VLineLog.cy) < m_totalLog.cy )
				objScrollPos.y += m_VLineLog.cy;

			objCaretPos.y = (lMaxLine-1)*(m_lfHeight+m_lLineInterval) - objScrollPos.y + m_RulerPos.y;
		}
		MyScrollToPosition(objScrollPos);
		break;
	}

	MySetCaretPos(objCaretPos);		//キャレット位置設定（クライアント座標系）
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行末の位置取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetPosition
	(	long	lLine,				//行番号
		CPoint*	pobjPoint			//行末の位置(out, Ｘ座標ゼロからスタートと想定)
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//////////////////////////////////////////////////
	//	２バイト文字やタブコードを考慮して
	//	行末のキャレット位置を決定する

	pobjPoint->x = 0;
	long lLineLength = pDoc->GetLineLength(lLine);
	long lCnt=0;
	while(lCnt < lLineLength)
	{
		if(pDoc->MyIsDBCSLeadByteEx(lLine,lCnt))//２バイト文字の第一バイト
		{
			lCnt+=2;
			pobjPoint->x += (m_lfWidth*2);
		}
		else //１バイト文字
		{
			if(pDoc->IsTabCode(lLine,lCnt)) //タブコード
			{
				int nMargin = pDoc->GetTabSize() - ( (pobjPoint->x/m_lfWidth) % pDoc->GetTabSize() );
				pobjPoint->x += (m_lfWidth * nMargin);
				lCnt++;
			}
			else //タブコード以外
			{
				lCnt++;
				pobjPoint->x += m_lfWidth;
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット未満位置取得処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetPosition1
	(	long	lLine,			//行番号
		CPoint*	pobjLimitPos,	//リミットポジション（Ｘ座標ゼロからスタートと想定）
		CPoint* pobjRetPos		//返却用ポジション（Ｘ座標ゼロからスタートと想定）
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//////////////////////////////////////////////////////////////////////
	//	２バイト文字やタブコードを考慮してキャレット位置を指定する

	long lLineLength = pDoc->GetLineLength(lLine);
	pobjRetPos->x = 0;
	long lCnt=0;
	while(lCnt < lLineLength)
	{
		if(pDoc->MyIsDBCSLeadByteEx(lLine,lCnt))//２バイト文字の第一バイト
		{
			if( (pobjRetPos->x+m_lfWidth*2) < pobjLimitPos->x)//まだ大丈夫
			{
				lCnt+=2;
				pobjRetPos->x += (m_lfWidth*2);
			}
			else
			{
				break;
			}
		}
		else //１バイト文字
		{
			if(pDoc->IsTabCode(lLine,lCnt)) //タブコード
			{
				int nMargin = pDoc->GetTabSize() - ( (pobjRetPos->x/m_lfWidth) % pDoc->GetTabSize() );
				if( (pobjRetPos->x+m_lfWidth*nMargin) < pobjLimitPos->x)//まだ大丈夫
				{
					pobjRetPos->x += ( m_lfWidth * nMargin );
					lCnt++;
				}
				else
				{
					break;
				}
			}
			else //タブコード以外
			{
				if( (pobjRetPos->x+m_lfWidth) < pobjLimitPos->x)//まだ大丈夫
				{
					lCnt++;
					pobjRetPos->x += m_lfWidth;
				}
				else
				{
					break;
				}
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット以下位置取得処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetPosition2
	(	long	lLine,			//行番号
		CPoint*	pobjLimitPos,	//リミットポジション（Ｘ座標ゼロからスタートと想定）
		CPoint* pobjRetPos		//返却用ポジション（Ｘ座標ゼロからスタートと想定）
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//////////////////////////////////////////////////////////////////////
	//	２バイト文字やタブコードを考慮してキャレット位置を指定する

	long lLineLength = pDoc->GetLineLength(lLine);
	pobjRetPos->x = 0;
	long lCnt=0;
	while(lCnt < lLineLength)
	{
		if(pDoc->MyIsDBCSLeadByteEx(lLine,lCnt))//２バイト文字の第一バイト
		{
			if( (pobjRetPos->x+m_lfWidth*2) <= pobjLimitPos->x)//まだ大丈夫
			{
				lCnt+=2;
				pobjRetPos->x += (m_lfWidth*2);
			}
			else
			{
				break;
			}
		}
		else //１バイト文字
		{
			if(pDoc->IsTabCode(lLine,lCnt)) //タブコード
			{
				int nMargin = pDoc->GetTabSize() - ( (pobjRetPos->x/m_lfWidth) % pDoc->GetTabSize() );
				if( (pobjRetPos->x+m_lfWidth*nMargin) <= pobjLimitPos->x)//まだ大丈夫
				{
					pobjRetPos->x += ( m_lfWidth * nMargin );
					lCnt++;
				}
				else
				{
					break;
				}
			}
			else //タブコード以外
			{
				if( (pobjRetPos->x+m_lfWidth) <= pobjLimitPos->x)//まだ大丈夫
				{
					lCnt++;
					pobjRetPos->x += m_lfWidth;
				}
				else
				{
					break;
				}
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：指定オフセット未満位置取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetPosition3
	(	long	lLine,			//行番号
		long	lLimitOffset,	//リミットオフセット
		CPoint* pobjRetPos		//返却用ポジション（Ｘ座標ゼロからスタートと想定）
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//////////////////////////////////////////////////////////////////////
	//	２バイト文字やタブコードを考慮してキャレット位置を指定する

	pobjRetPos->x = 0;
	long lCnt=0;
	while(lCnt < lLimitOffset)
	{
		if(pDoc->MyIsDBCSLeadByteEx(lLine,lCnt))//２バイト文字の第一バイト
		{
			pobjRetPos->x += (m_lfWidth*2);
			lCnt+=2;
		}
		else //１バイト文字
		{
			if(pDoc->IsTabCode(lLine,lCnt)) //タブコード
			{
				int nMargin = pDoc->GetTabSize() - ( (pobjRetPos->x/m_lfWidth) % pDoc->GetTabSize() );
				pobjRetPos->x += ( m_lfWidth * nMargin );
				lCnt++;
			}
			else //タブコード以外
			{
				pobjRetPos->x += m_lfWidth;
				lCnt++;
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷時の行末位置取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetPosition4
	(	long	lLine,				//行番号
		CPoint*	pobjPoint,			//行末の位置(out, Ｘ座標ゼロからスタートと想定)
		long	lPWidth				//印刷フォントの幅
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//////////////////////////////////////////////////
	//	２バイト文字やタブコードを考慮して
	//	行末のキャレット位置を決定する

	pobjPoint->x = 0;
	long lLineLength = pDoc->GetLineLength(lLine);
	long lCnt=0;
	while(lCnt < lLineLength)
	{
		if(pDoc->MyIsDBCSLeadByteEx(lLine,lCnt))//２バイト文字の第一バイト
		{
			lCnt+=2;
			pobjPoint->x += (lPWidth*2);
		}
		else //１バイト文字
		{
			if(pDoc->IsTabCode(lLine,lCnt)) //タブコード
			{
				int nMargin = pDoc->GetTabSize() - ( (pobjPoint->x/lPWidth) % pDoc->GetTabSize() );
				pobjPoint->x += (lPWidth * nMargin);
				lCnt++;
			}
			else //タブコード以外
			{
				lCnt++;
				pobjPoint->x += lPWidth;
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オフセット取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetPosition5
	(	long	lLine,			//行番号
		CPoint*	pobjLimitPos,	//リミットポジション（Ｘ座標ゼロからスタートと想定）
		ULONG*	plOffset,		//オフセット返却用
		CPoint* pobjRetPos		//返却用ポジション（Ｘ座標ゼロからスタートと想定）
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	////////////////////////////////////////////////////////////////////
	//	２バイト文字やタブコードを考慮してキャレット位置を指定する

	long lLineLength = pDoc->GetLineLength(lLine);
	pobjRetPos->x = 0;
	pobjRetPos->y = 0;
	(*plOffset) = 0;
	while((*plOffset) < (ULONG)lLineLength)
	{
		if(pDoc->MyIsDBCSLeadByteEx(lLine,(*plOffset)))//２バイト文字の第一バイト
		{
			if( (pobjRetPos->x+m_lfWidth*2) <= pobjLimitPos->x)//まだ大丈夫
			{
				(*plOffset)+=2;
				pobjRetPos->x += (m_lfWidth*2);
			}
			else
			{
				break;
			}
		}
		else //１バイト文字
		{
			if(pDoc->IsTabCode(lLine,(*plOffset))) //タブコード
			{
				int nMargin = pDoc->GetTabSize() - ( (pobjRetPos->x/m_lfWidth) % pDoc->GetTabSize() );
				if( (pobjRetPos->x+m_lfWidth*nMargin) <= pobjLimitPos->x)//まだ大丈夫
				{
					(*plOffset)++;
					pobjRetPos->x += ( m_lfWidth * nMargin );
				}
				else
				{
					break;
				}
			}
			else //タブコード以外
			{
				if( (pobjRetPos->x+m_lfWidth) <= pobjLimitPos->x)//まだ大丈夫
				{
					(*plOffset)++;
					pobjRetPos->x += m_lfWidth;
				}
				else
				{
					break;
				}
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オフセット取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorView::GetOffset
	(	long	lLine,				//行番号
		CPoint*	pobjLimitPos		//リミットポジション（Ｘ座標ゼロからスタートと想定）
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	////////////////////////////////////////////////////////////////////
	//	２バイト文字やタブコードを考慮してキャレット位置を指定する

	long lLineLength = pDoc->GetLineLength(lLine);
	CPoint	tempPoint(0,0);
	long lCnt=0;
	while(lCnt < lLineLength)
	{
		if(pDoc->MyIsDBCSLeadByteEx(lLine,lCnt))//２バイト文字の第一バイト
		{
			if( (tempPoint.x+m_lfWidth*2) <= pobjLimitPos->x)//まだ大丈夫
			{
				lCnt+=2;
				tempPoint.x += (m_lfWidth*2);
			}
			else
			{
				break;
			}
		}
		else //１バイト文字
		{
			if(pDoc->IsTabCode(lLine,lCnt)) //タブコード
			{
				int nMargin = pDoc->GetTabSize() - ( (tempPoint.x/m_lfWidth) % pDoc->GetTabSize() );
				if( (tempPoint.x+m_lfWidth*nMargin) <= pobjLimitPos->x)//まだ大丈夫
				{
					tempPoint.x += ( m_lfWidth * nMargin );
					lCnt++;
				}
				else
				{
					break;
				}
			}
			else //タブコード以外
			{
				if( (tempPoint.x+m_lfWidth) <= pobjLimitPos->x)//まだ大丈夫
				{
					lCnt++;
					tempPoint.x += m_lfWidth;
				}
				else
				{
					break;
				}
			}
		}
	}

	//オフセット値返却
	return lCnt;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：直前オフセット取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorView::GetPreOffset	//直前オフセット
	(	long	lLine,			//行番号
		long	lLimitOffset	//リミットオフセット
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( lLimitOffset == 0 ) //直前はもうない
		return -1;

	long lCnt=0;
	for(;;)
	{
		if(pDoc->MyIsDBCSLeadByteEx(lLine,lCnt))//２バイト文字の第一バイト
		{
			if( (lCnt+2) < lLimitOffset ) //まだ大丈夫
				lCnt+=2;
			else
				return lCnt;
		}
		else //１バイト文字
		{
			if( (lCnt+1) < lLimitOffset ) //まだ大丈夫
				lCnt++;
			else
				return lCnt;
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット位置の行番号、列番号取得									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::CaretToLineOffset
	(	long*	plLine,			//行番号(out)
		long*	plOffset		//列番号(out)
	)
{
	//キャレット位置を論理座標系へ変換する
	CPoint logicalPoint = m_caretPos;
	logicalPoint -= m_LineNoPos;
	logicalPoint -= m_RulerPos;
	logicalPoint.x += GetScrollPosLogX();
	logicalPoint.y += GetScrollPosLogY();

	//キャレット位置を表示フォントの高さと幅に合わせる
	if( (m_lfWidth==0) || (m_lfHeight==0) )
	{
		(*plLine)   = 0;
		(*plOffset) = 0;
		return FALSE;
	}
	logicalPoint.x -= (logicalPoint.x % m_lfWidth);
	logicalPoint.y -= (logicalPoint.y % (m_lfHeight+m_lLineInterval));

	//キャレットが今何行目、何文字目にあるかを計算
	//（２バイト文字やタブコードを考慮）
	(*plLine)   = logicalPoint.y / (m_lfHeight+m_lLineInterval);
	(*plOffset) = GetOffset( (*plLine) ,&logicalPoint );

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット行　１行再描画処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::InvalidateCaretLine()
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect	clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	//最大幅計算
	ULONG ulRight = 0;
	if( ((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth) > clientRect.Width() )
		ulRight = (ULONG)((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth);
	else
		ulRight = clientRect.Width();

	//再描画
	CRect	lineRect;
	lineRect = CRect(0,m_caretPos.y,ulRight,m_caretPos.y+(m_lfHeight+m_lLineInterval));
	lineRect.NormalizeRect();

	if(m_pobjInvalidRect)
		delete m_pobjInvalidRect;
	m_pobjInvalidRect = new CRect();
	(*m_pobjInvalidRect) = lineRect;
	m_bUseClipRgn = TRUE; //クリップされた領域のみ描画更新する
	InvalidateRect(&lineRect,FALSE);
	//すぐに描画
	UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット行　再描画処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::InvalidateCaretLineEx
	(	long lPreLine		//キャレット行のまわり何行をいっしょに再描画するのか
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect	clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	//最大幅計算
	ULONG ulRight = 0;
	if( ((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth) > clientRect.Width() )
		ulRight = (ULONG)((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth);
	else
		ulRight = clientRect.Width();

	//再描画
	CRect	lineRect;
	int nTop=0;
	int nBottom=0;
	if(lPreLine == 0) //キャレット行のみ再描画
		lineRect = CRect(0,m_caretPos.y,ulRight,m_caretPos.y+(m_lfHeight+m_lLineInterval));
	else if(lPreLine < 0) //キャレットの前の行をいっしょに再描画
	{
		nTop = m_caretPos.y + (m_lfHeight+m_lLineInterval)*lPreLine;
		lineRect = CRect(0,nTop,ulRight,m_caretPos.y+(m_lfHeight+m_lLineInterval));
	}
	else if(lPreLine > 0) //キャレットの後の行をいっしょに再描画
	{
		nBottom = m_caretPos.y + (m_lfHeight+m_lLineInterval)*(1+lPreLine);
		lineRect = CRect(0,m_caretPos.y,ulRight,nBottom);
	}
	lineRect.NormalizeRect();

	if(m_pobjInvalidRect)
		delete m_pobjInvalidRect;
	m_pobjInvalidRect = new CRect();
	(*m_pobjInvalidRect) = lineRect;
	m_bUseClipRgn = TRUE; //クリップされた領域のみ描画更新する
	InvalidateRect(&lineRect,FALSE);
	//すぐに描画
	UpdateWindow();
}

/////////////////////////////////////////////////////////////////////////////
//	CEditorView プロテクト関数（マウス関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：マウス位置の行番号、列番号取得処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LONG CEditorView::MouseToLineOffset	//０：失敗、１：成功、２：行番号エリア内
	(	CPoint&		mousePoint,		// マウス位置(in)
		long*		plLine,			// 行番号(out)
		long*		plOffset,		// 列番号(out)
		CPoint*		pCaretPoint		// キャレット位置(out)
	)
{
	long	lCurrentOffset=0;
	CEditorDoc*	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if((m_bDispRuler!=FALSE) &&	(mousePoint.y<m_RulerPos.y)) //ルーラ領域内
	{
		(*plLine)	= 0;
		(*plOffset)	= 0;
		pCaretPoint->x = m_LineNoPos.x;
		pCaretPoint->y = m_RulerPos.y;
		return 0;
	}
	else if((m_bDispLineNo!=FALSE) && (mousePoint.x<m_LineNoPos.x)) //行番号領域内
	{
		//マウス位置をデバイス座標系から論理座標系へ変換する
		CPoint	logicalPoint = mousePoint;
		logicalPoint -=	m_LineNoPos;
		logicalPoint -=	m_RulerPos;
		logicalPoint.x += GetScrollPosLogX();
		logicalPoint.y += GetScrollPosLogY();

		//マウス位置を表示フォントの高さと幅に合わせる
		if(	(m_lfWidth==0)  || (m_lfHeight==0) )
		{
			(*plLine)	= 0;
			(*plOffset)	= 0;
			pCaretPoint->x = m_LineNoPos.x;
			pCaretPoint->y = m_RulerPos.y;
			return 0;
		}
		logicalPoint.x -= (logicalPoint.x %	m_lfWidth);
		logicalPoint.y -= (logicalPoint.y %	(m_lfHeight+m_lLineInterval));

		//マウス位置の行番号を計算
		long lCurrentLine	= logicalPoint.y / (m_lfHeight+m_lLineInterval);
		long lMaxLine		= pDoc->GetLineCount();
		if(	lCurrentLine < lMaxLine	)
		{
		}
		else
		{
			if(	lMaxLine > 1 )
				lCurrentLine = lMaxLine	- 1;
			else
				lCurrentLine = 0;
		}

		pCaretPoint->x = 0;
		pCaretPoint->y = 0;
		//行番号、列番号返却
		(*plLine) =	lCurrentLine;
		(*plOffset)	= 0;
		return 2;
	}
	else //その他
	{
		//マウス位置をデバイス座標系から論理座標系へ変換する
		CPoint	logicalPoint = mousePoint;
		logicalPoint -=	m_LineNoPos;
		logicalPoint -=	m_RulerPos;
		logicalPoint.x += GetScrollPosLogX();
		logicalPoint.y += GetScrollPosLogY();

		//マウス位置を表示フォントの高さと幅に合わせる
		if(	(m_lfWidth==0)  || (m_lfHeight==0) )
		{
			(*plLine)	= 0;
			(*plOffset)	= 0;
			pCaretPoint->x = m_LineNoPos.x;
			pCaretPoint->y = m_RulerPos.y;
			return 0;
		}
		logicalPoint.x -= (logicalPoint.x %	m_lfWidth);
		logicalPoint.y -= (logicalPoint.y %	(m_lfHeight+m_lLineInterval));

		//マウス位置の行番号を計算
		long lCurrentLine	= logicalPoint.y / (m_lfHeight+m_lLineInterval);
		long lMaxLine		= pDoc->GetLineCount();

		if(	lCurrentLine < lMaxLine	)
		{
			//２バイト文字やタブコードを考慮してキャレット位置を指定する（論理座標）
			CPoint	tempPos(0,logicalPoint.y);
			GetPosition2(lCurrentLine,&logicalPoint,&tempPos);
			(*pCaretPoint) = tempPos;

			//マウス位置の列番号を計算
			lCurrentOffset = GetOffset(lCurrentLine,&tempPos);

			//キャレット位置をデバイス座標へ戻す
			(*pCaretPoint).x -=	GetScrollPosLogX();
			(*pCaretPoint).y -=	GetScrollPosLogY();
			(*pCaretPoint) += m_RulerPos;
			(*pCaretPoint) += m_LineNoPos;
		}
		else
		{
			pCaretPoint->x = m_LineNoPos.x;
			lCurrentOffset = 0;
			if(	lMaxLine > 1 )
			{
				lCurrentLine = lMaxLine	- 1;
				pCaretPoint->y = lCurrentLine *	(m_lfHeight+m_lLineInterval);

				//キャレット位置をデバイス座標へ戻す
				pCaretPoint->y -= GetScrollPosLogY();
				pCaretPoint->y += m_RulerPos.y;
			}
			else
			{
				lCurrentLine = 0;
				pCaretPoint->y = m_RulerPos.y;
			}
		}

		//行番号、列番号返却
		(*plLine) =	lCurrentLine;
		(*plOffset)	= lCurrentOffset;
		return 1;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	CEditorView プロテクト関数（選択範囲関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：選択範囲調整処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::AdjustSelectArea()
{
	long	lSelectStartLine;
	long	lSelectEndLine;
	long	lSelectStartOffset;
	long	lSelectEndOffset;
	if( m_lSelectStartLine < m_lSelectEndLine )
	{
		lSelectStartLine   = m_lSelectStartLine;
		lSelectEndLine     = m_lSelectEndLine;
		lSelectStartOffset = m_lSelectStartOffset;
		lSelectEndOffset   = m_lSelectEndOffset;
	}
	else if( m_lSelectStartLine == m_lSelectEndLine )
	{
		lSelectStartLine = m_lSelectStartLine;
		lSelectEndLine   = m_lSelectEndLine;
		if( m_lSelectStartOffset <= m_lSelectEndOffset )
		{
			lSelectStartOffset = m_lSelectStartOffset;
			lSelectEndOffset   = m_lSelectEndOffset;
		}
		else
		{
			lSelectStartOffset = m_lSelectEndOffset;
			lSelectEndOffset   = m_lSelectStartOffset;
		}
	}
	else
	{
		lSelectStartLine   = m_lSelectEndLine;
		lSelectEndLine     = m_lSelectStartLine;
		lSelectStartOffset = m_lSelectEndOffset;
		lSelectEndOffset   = m_lSelectStartOffset;
	}

	m_lSelectStartLine   = lSelectStartLine;
	m_lSelectStartOffset = lSelectStartOffset;
	m_lSelectEndLine     = lSelectEndLine;
	m_lSelectEndOffset   = lSelectEndOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：CTRL+クリックによるワードセレクト処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::WordSelectByCtrlClick
	(	CPoint&		point
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	long	lCnt=0;
	long	lCurrentLine=0;
	long	lCurrentOffset=0;
	CPoint	prePos(0,0);
	CPoint	nowPos(0,0);
	CPoint	mousePos(0,0);
	CLine*	pobjLine = NULL;

	//マウス位置　－＞　行、列番号変換
	LONG lRet =	MouseToLineOffset( point, &lCurrentLine, &lCurrentOffset, &mousePos	);
	if(lRet	== 0 ||	lRet ==	2)
		return;

	//マウス位置の行取得
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
		return;

	//まず選択を解除する
	ClearSelectArea();
	m_pApp->MessagePump();

	//////////////////////////////
	//		選択範囲設定		//
	//////////////////////////////
	if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //位置が半角
	{
		if( isHanKana(pobjLine->GetAt(lCurrentOffset)) )//半角ｶﾀｶﾅ
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// 選択範囲の後ろを決定
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//選択範囲の前を決定

			//マウス位置を論理座標へ変換する
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//１文字前の位置取得
				if( ((nowPos.x-prePos.x)==m_lfWidth) &&
					isHanKana(pobjLine->GetAt(lCnt-1))
				)
				{
					m_lSelectStartOffset--;
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt--;
				if( nowPos.x == 0 )
					break;
			}
		}
		else if( ( (pobjLine->GetAt(lCurrentOffset)>='A' && pobjLine->GetAt(lCurrentOffset)<='Z') ||
			  	 (pobjLine->GetAt(lCurrentOffset)>='a' && pobjLine->GetAt(lCurrentOffset)<='z') ||
			     (pobjLine->GetAt(lCurrentOffset)>='0' && pobjLine->GetAt(lCurrentOffset)<='9') ||
			     (pobjLine->GetAt(lCurrentOffset)=='_')
			     ) &&
				 !isHanKana(pobjLine->GetAt(lCurrentOffset))
		)
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// 選択範囲の後ろを決定
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) 						||
					( (pobjLine->GetAt(lCnt)<'A' || pobjLine->GetAt(lCnt)>'Z') &&
					  (pobjLine->GetAt(lCnt)<'a' || pobjLine->GetAt(lCnt)>'z') &&
					  (pobjLine->GetAt(lCnt)<'0' || pobjLine->GetAt(lCnt)>'9') &&
					  (pobjLine->GetAt(lCnt)!='_')
					)																||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//選択範囲の前を決定

			//マウス位置を論理座標へ変換する
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//１文字前の位置取得
				if( ( (nowPos.x-prePos.x)==m_lfWidth) &&
					( (pobjLine->GetAt(lCnt-1)>='A' && pobjLine->GetAt(lCnt-1)<='Z') ||
			  		  (pobjLine->GetAt(lCnt-1)>='a' && pobjLine->GetAt(lCnt-1)<='z') ||
			  		  (pobjLine->GetAt(lCnt-1)>='0' && pobjLine->GetAt(lCnt-1)<='9') ||
			  		  (pobjLine->GetAt(lCnt-1)=='_')
					) &&
					!isHanKana(pobjLine->GetAt(lCnt-1))
				)
				{
					m_lSelectStartOffset--;
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt--;
				if( nowPos.x == 0 )
					break;
			}
		}
		else if( !isspace(pobjLine->GetAt(lCurrentOffset)) ) //位置が半角英数字でも半角ｶﾀｶﾅでもｽﾍﾟｰｽでもない
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// 選択範囲の後ろを決定
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) 								||
					( (pobjLine->GetAt(lCnt)>='A' && pobjLine->GetAt(lCnt)<='Z') ||
			  		  (pobjLine->GetAt(lCnt)>='a' && pobjLine->GetAt(lCnt)<='z') ||
			  		  (pobjLine->GetAt(lCnt)>='0' && pobjLine->GetAt(lCnt)<='9') ||
			  		  (pobjLine->GetAt(lCnt)=='_')
					) 																		||
					isHanKana(pobjLine->GetAt(lCnt))										||
					isspace(pobjLine->GetAt(lCnt))
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//選択範囲の前を決定

			//マウス位置を論理座標へ変換する
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//１文字前の位置取得
				if( ((nowPos.x-prePos.x)==m_lfWidth)  									&&
					( (pobjLine->GetAt(lCnt-1)<'A' || pobjLine->GetAt(lCnt-1)>'Z') &&
					  (pobjLine->GetAt(lCnt-1)<'a' || pobjLine->GetAt(lCnt-1)>'z') &&
					  (pobjLine->GetAt(lCnt-1)<'0' || pobjLine->GetAt(lCnt-1)>'9') &&
					  (pobjLine->GetAt(lCnt-1)!='_')
					)																	&&
					!isHanKana(pobjLine->GetAt(lCnt-1))									&&
					!isspace(pobjLine->GetAt(lCnt-1))
				)
				{
					m_lSelectStartOffset--;
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt--;
				if( nowPos.x == 0 )
					break;
			}
		}
	}
	else if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //位置が全角文字
	{
		//文字コード取得
		UINT unChar = ( (pobjLine->GetAt(lCurrentOffset) << 8) | (pobjLine->GetAt(lCurrentOffset+1)) );
		if( _ismbchira( unChar ) ) //位置がﾀﾞﾌﾞﾙﾊﾞｲﾄひらがな
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// 選択範囲の後ろを決定
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!_ismbchira( unCharTmp )
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//選択範囲の前を決定

			//マウス位置を論理座標へ変換する
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//１文字前の位置取得
				if( (nowPos.x-prePos.x)==(m_lfWidth*2) )
				{
					//文字コード取得
					UINT unCharTmp = ( (pobjLine->GetAt(lCnt-2) << 8) | (pobjLine->GetAt(lCnt-1)) );
					if( _ismbchira( unCharTmp ) ) //ひらがな継続
					{
						m_lSelectStartOffset -= 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt -= 2;
				if( nowPos.x == 0 )
					break;
			}
		}
		else if( (_ismbckata(unChar)) ||
				 (unChar==0x815B)			//位置がﾀﾞﾌﾞﾙﾊﾞｲﾄかたかなｏｒ”ー”
		)
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// 選択範囲の後ろを決定
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbckata( unCharTmp ) && (unCharTmp!=0x815B) )
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//選択範囲の前を決定

			//マウス位置を論理座標へ変換する
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//１文字前の位置取得
				if( (nowPos.x-prePos.x)==(m_lfWidth*2) )
				{
					//文字コード取得
					UINT unCharTmp = ( (pobjLine->GetAt(lCnt-2) << 8) | (pobjLine->GetAt(lCnt-1)) );
					if( _ismbckata( unCharTmp ) || (unCharTmp==0x815B) ) //カタカナ継続
					{
						m_lSelectStartOffset -= 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt -= 2;
				if( nowPos.x == 0 )
					break;
			}
		}
		else if( _ismbcl1(unChar) || _ismbcl2(unChar) ) //位置がﾀﾞﾌﾞﾙﾊﾞｲﾄ漢字
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// 選択範囲の後ろを決定
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbcl1( unCharTmp ) && !_ismbcl2( unCharTmp ) )
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//選択範囲の前を決定

			//マウス位置を論理座標へ変換する
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//１文字前の位置取得
				if( (nowPos.x-prePos.x)==(m_lfWidth*2) )
				{
					//文字コード取得
					UINT unCharTmp = ( (pobjLine->GetAt(lCnt-2) << 8) | (pobjLine->GetAt(lCnt-1)) );
					if( _ismbcl1(unCharTmp) || _ismbcl2(unCharTmp) ) //漢字継続
					{
						m_lSelectStartOffset -= 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt -= 2;
				if( nowPos.x == 0 )
					break;
			}
		}
		else //その他の２バイト文字
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// 選択範囲の後ろを決定
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( _ismbchira( unCharTmp ) ||
					  _ismbckata(unCharTmp)   ||
					  (unCharTmp==0x815B)     ||
					  _ismbcl1( unCharTmp )   ||
					  _ismbcl2( unCharTmp )
					)
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//選択範囲の前を決定

			//マウス位置を論理座標へ変換する
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//１文字前の位置取得
				if( (nowPos.x-prePos.x)==(m_lfWidth*2) )
				{
					//文字コード取得
					UINT unCharTmp = ( (pobjLine->GetAt(lCnt-2) << 8) | (pobjLine->GetAt(lCnt-1)) );
					if(	( !_ismbchira( unCharTmp ) &&
					      !_ismbckata(unCharTmp)   &&
					      (unCharTmp!=0x815B)      &&
					      !_ismbcl1( unCharTmp )   &&
					      !_ismbcl2( unCharTmp )
					    )
					)
					{
						m_lSelectStartOffset -= 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt -= 2;
				if( nowPos.x == 0 )
					break;
			}
		}
	}

	//キャレット位置設定
	MySetCaretToLineOffset(m_lSelectEndLine,m_lSelectEndOffset);

	//マウス位置の行を再描画
//	InvalidateCaretLine();
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
//	CEditorView プロテクト関数（ジャンプ関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：指定行、列へのジャンプ処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::Jump2LineOffset
	(	long	lJumpLine,			//ジャンプ先行番号（ゼロオリジン）
		long	lJumpOffset			//ジャンプ先列番号
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	if( (m_lfWidth==0) || (m_lfHeight==0) )
		return;
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//クライアント領域に表示できる行数
	if( m_lfHeight==0 )
		return;
	long lLineCnt = clientRect.Height() / (m_lfHeight+m_lLineInterval);
	//ドキュメントの総行数
	long lMaxLine = pDoc->GetLineCount();

	//指定オフセット以下位置取得（論理座標で戻ってくる）
	CPoint	objTempPos(0,0);
	GetPosition3(lJumpLine,lJumpOffset,&objTempPos);

	if( lLineCnt < lMaxLine )
	{
		if( ((lJumpLine+1) + lLineCnt) <= lMaxLine )
		{
			long lX = (objTempPos.x > nWidth) ? (objTempPos.x-nWidth+1) : 0;
			long lY = lJumpLine * (m_lfHeight+m_lLineInterval);
			MyScrollToPosition(CPoint(lX,lY));				//論理座標で位置を渡している
			CPoint caretPos(objTempPos.x,lY);
			//論理座標－＞デバイス座標
			caretPos.x -= GetScrollPosLogX();
			caretPos.y -= GetScrollPosLogY();
			caretPos += m_RulerPos;
			caretPos += m_LineNoPos;
			MySetCaretPos(caretPos);
		}
		else
		{
			long lX = (objTempPos.x > nWidth) ? (objTempPos.x-nWidth+1) : 0;
			long lY = (lMaxLine - lLineCnt) * (m_lfHeight+m_lLineInterval);
			MyScrollToPosition(CPoint(lX,lY));					//論理座標で位置を渡している
			CPoint caretPos(objTempPos.x,(lJumpLine*(m_lfHeight+m_lLineInterval)));
			//論理座標－＞デバイス座標
			caretPos.x -= GetScrollPosLogX();
			caretPos.y -= GetScrollPosLogY();
			caretPos += m_RulerPos;
			caretPos += m_LineNoPos;
			MySetCaretPos(caretPos);
		}
	}
	else
	{
		long lX = (objTempPos.x > nWidth) ? (objTempPos.x-nWidth+1) : 0;
		MyScrollToPosition(CPoint(lX,0));					//論理座標で位置を渡している
		long lY = lJumpLine * (m_lfHeight+m_lLineInterval);
		CPoint caretPos(objTempPos.x,lY);
		//論理座標－＞デバイス座標
		caretPos.x -= GetScrollPosLogX();
		caretPos.y -= GetScrollPosLogY();
		caretPos += m_RulerPos;
		caretPos += m_LineNoPos;
		MySetCaretPos(caretPos);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：指定行、列へのジャンプ処理											|*/
/*|	解説	：																		|*/
/*|	備考	：ジャンプ先行が表示エリア内にあればスクロールしない					|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::Jump2LineOffsetEx
	(	long	lJumpLine,			//ジャンプ先行番号（ゼロオリジン）
		long	lJumpOffset			//ジャンプ先列番号
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	if( (m_lfWidth==0) || (m_lfHeight==0) )
		return;
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//スクロール位置の行番号取得
	long lScrollLine = GetScrollPosLogY() / (m_lfHeight+m_lLineInterval);

	//クライアント領域に表示できる行数
	long lLineCnt = clientRect.Height() / (m_lfHeight+m_lLineInterval);

	if( lJumpLine >= lScrollLine &&  // ジャンプ先行番号が表示範囲内
		lJumpLine < (lScrollLine+lLineCnt)
	)
	{
		//指定オフセット以下位置取得（論理座標で戻ってくる）
		CPoint	objTempPos(0,0);
		GetPosition3(lJumpLine,lJumpOffset,&objTempPos);

		long lX = (objTempPos.x > nWidth) ? (objTempPos.x-nWidth+1) : 0;
		long lY = GetScrollPosLogY();
		MyScrollToPosition(CPoint(lX,lY));	//論理座標で位置を渡している
		lY += (lJumpLine-lScrollLine)*(m_lfHeight+m_lLineInterval);
		CPoint caretPos(objTempPos.x,lY);
		//論理座標－＞デバイス座標
		caretPos.x -= GetScrollPosLogX();
		caretPos.y -= GetScrollPosLogY();
		caretPos += m_RulerPos;
		caretPos += m_LineNoPos;
		MySetCaretPos(caretPos);
	}
	else // ジャンプ先行番号が表示範囲外
	{
		Jump2LineOffset(lJumpLine,lJumpOffset);
	}
}

/////////////////////////////////////////////////////////////////////////////
//	CEditorView プロテクト関数（領域関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：クライアント領域取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyGetClientRect
	(	CRect*	pobjRect
	)
{
	GetClientRect(pobjRect);

	//行番号を表示している場合
	if( m_bDispLineNo )
	{
		//行番号表示している分だけクライアント領域が小さくなる
		pobjRect->left += m_lLineNoWidth;
	}
	//ルーラを表示している場合
	if( m_bDispRuler )
	{
		//ルーラを表示している分だけクライアント領域が小さくなる
		pobjRect->top += m_RulerPos.y;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	CEditorView プロテクト関数（印刷ヘッダ＆フッタ関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷ヘッダフォーマット解析処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::ParsePrintHeaderFormat
	(	CString&	objStrParseIn,
		CString&	objStrParseResult,
		CPrintInfo*	pInfo
	)
{
	char	szBuf[256];
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//クリア
	objStrParseResult = _T("");

	for(long lCnt=0; lCnt<objStrParseIn.GetLength(); )
	{
		char cByte = objStrParseIn.GetAt(lCnt);

		if(IsDBCSLeadByteEx(0,cByte)) //２バイト文字
		{
			//そのままコピー
			objStrParseResult += cByte;
			lCnt++;
			objStrParseResult += objStrParseIn.GetAt(lCnt);
			lCnt++;
		}
		else //１バイト文字
		{
			if( (cByte=='&') && ((lCnt+1)<objStrParseIn.GetLength()) )
			{
				//次の文字取得
				lCnt++;
				char cNextByte = objStrParseIn.GetAt(lCnt);
				if( cNextByte=='f' ) // &f でフルパス
				{
					objStrParseResult += pDoc->GetTitle();
				}
				else if( cNextByte=='n' ) // &n でファイル名
				{
					pDoc->MyGetFileName(szBuf);
					objStrParseResult += szBuf;
				}
				else if( cNextByte=='d' ) // &d で日付
				{
					CTime objTime = CTime::GetCurrentTime();
					objStrParseResult += objTime.Format("%Y/%m/%d");
				}
				else if( cNextByte=='t' ) // &t で時刻
				{
					CTime objTime = CTime::GetCurrentTime();
					objStrParseResult += objTime.Format("%H:%M:%S");
				}
				else if( cNextByte=='p' ) // &p でページ番号
				{
					sprintf(szBuf,"%d",	pInfo->m_nCurPage);
					objStrParseResult += szBuf;
				}
				else if( cNextByte=='P' ) // &P で総ページ数
				{
					sprintf(szBuf,"%d",	pInfo->GetMaxPage());
					objStrParseResult += szBuf;
				}
				else
				{
					//そのままコピー
					objStrParseResult += cByte;
					objStrParseResult += cNextByte;
				}
				lCnt++;
			}
			else
			{
				//そのままコピー
				objStrParseResult += cByte;
				lCnt++;
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷フッタフォーマット解析処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::ParsePrintFooterFormat
	(	CString&	objStrParseIn,
		CString&	objStrParseResult,
		CPrintInfo* pInfo
	)
{
	ParsePrintHeaderFormat(objStrParseIn,objStrParseResult,pInfo);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷総ページ数カウント処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CEditorView::CountPrintMaxPage
	(	CPrintInfo* 	pInfo,
		long			lLinePerPrintPage,
		long			lPrintCharWidth
	)
{
	long	lCnt=0;
	UINT	unMaxPage=0;
	long	lFoldLineCnt=0;

	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(m_bPrint2Dangumi==FALSE) //１段組み印刷
	{
		unMaxPage = pDoc->GetLineCount() / lLinePerPrintPage;
		if( (pDoc->GetLineCount() % lLinePerPrintPage) != 0 )
			unMaxPage += 1;
		return unMaxPage;
	}
	else //２段組み印刷
	{
		unMaxPage = pDoc->GetLineCount() / (lLinePerPrintPage*2);
		if( (pDoc->GetLineCount() % (lLinePerPrintPage*2)) != 0 )
			unMaxPage += 1;
		return unMaxPage;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル内容１ページ分印刷出力処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyPrintOut1Page
	(	CDC*			pDC,
		CPrintInfo*		pInfo,
		long			lLinePerPrintPage,
		long			lPrintLineHeight,
		long			lPrintCharWidth
	)
{
	int		nRet=0;
	ULONG	lCnt=0;
	ULONG	lLineCnt=0;
	ULONG	lLineStart=0;
	ULONG	lLineStop=0;
	CString	objLineNoStr=_T("");
	CLine*	pobjLine=NULL;
	long	lMargin=0;
	BOOL	bEnd=FALSE;
	int		nDiffLineKind;
	int		nDirCmpLineKind;


	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(m_bPrint2Dangumi==FALSE) //１段組み印刷
	{
		lLineStart = (pInfo->m_nCurPage - 1) * lLinePerPrintPage;
		lLineStop  = lLineStart + lLinePerPrintPage;
		if(lLineStop > (ULONG)pDoc->GetLineCount())
		{
			lLineStop = pDoc->GetLineCount();
		}
		for(lCnt=0,lLineCnt=lLineStart; lLineCnt<lLineStop; lCnt++,lLineCnt++)
		{
			objLineNoStr = _T("");
			if( m_bPrintLineNo ) //行番号印刷する
				objLineNoStr.Format(" %6d | ",lLineCnt+1);
			else
				objLineNoStr = _T("  ");
			//ファイル比較、ディレクトリ比較での相違の目印
			nDiffLineKind = pDoc->GetDiffLineKind(lLineCnt);
			nDirCmpLineKind = pDoc->GetDirCmpLineKind(lLineCnt);
			if(nDiffLineKind == DIFF_LINEKIND_ADDED)
				objLineNoStr += _T("+ ");
			else if(nDiffLineKind == DIFF_LINEKIND_DELETED)
				objLineNoStr += _T("- ");
			else if(nDirCmpLineKind == DIRCMP_LINEKIND_DIFF)
				objLineNoStr += _T("! ");
			else
				objLineNoStr += _T("  ");

			pDC->TextOut( pInfo->m_rectDraw.left,
						  ((lCnt+2)*lPrintLineHeight) + pInfo->m_rectDraw.top,
						  objLineNoStr);

			//１行
			pobjLine = pDoc->GetOneLine(lLineCnt);
			if(pobjLine != NULL) //あった
			{
				lMargin = objLineNoStr.GetLength()*lPrintCharWidth;
				lMargin += pInfo->m_rectDraw.left;
				MyTextOutForPrint(  pDC,
									lMargin,
									((lCnt+2)*lPrintLineHeight) + pInfo->m_rectDraw.top,
									pobjLine,
									lPrintCharWidth,
									lLineCnt,
									pInfo->m_rectDraw.Width());
			}
		}
	}
	else //２段組み印刷
	{
		//////////////////////////////
		//		左の段を印字		//
		//////////////////////////////
		lLineStart = (pInfo->m_nCurPage - 1) * (lLinePerPrintPage*2);
		lLineStop  = lLineStart + lLinePerPrintPage;
		if(lLineStop >= (ULONG)pDoc->GetLineCount())
		{
			lLineStop = pDoc->GetLineCount();
			bEnd=TRUE;//左段印字でおしまい
		}
		for(lCnt=0,lLineCnt=lLineStart; lLineCnt<lLineStop; lCnt++,lLineCnt++)
		{
			objLineNoStr = _T("");
			if( m_bPrintLineNo  ) //行番号印刷する
				objLineNoStr.Format(" %6d | ",lLineCnt+1);
			else
				objLineNoStr = _T("  ");
			//ファイル比較、ディレクトリ比較での相違の目印
			nDiffLineKind = pDoc->GetDiffLineKind(lLineCnt);
			nDirCmpLineKind = pDoc->GetDirCmpLineKind(lLineCnt);
			if(nDiffLineKind == DIFF_LINEKIND_ADDED)
				objLineNoStr += _T("+ ");
			else if(nDiffLineKind == DIFF_LINEKIND_DELETED)
				objLineNoStr += _T("- ");
			else if(nDirCmpLineKind == DIRCMP_LINEKIND_DIFF)
				objLineNoStr += _T("! ");
			else
				objLineNoStr += _T("  ");

			pDC->TextOut( pInfo->m_rectDraw.left,
						  ((lCnt+2)*lPrintLineHeight) + pInfo->m_rectDraw.top,
						  objLineNoStr);

			//１行
			pobjLine = pDoc->GetOneLine(lLineCnt);
			if(pobjLine != NULL) //あった
			{
				lMargin = objLineNoStr.GetLength()*lPrintCharWidth;
				lMargin += pInfo->m_rectDraw.left;
				MyTextOutForPrint(  pDC,
									lMargin,
									((lCnt+2)*lPrintLineHeight) + pInfo->m_rectDraw.top,
									pobjLine,
									lPrintCharWidth,
									lLineCnt,
									pInfo->m_rectDraw.Width()/2 - 2*lPrintCharWidth);
			}
		}
		//////////////////////////////
		//		右の段を印字		//
		//////////////////////////////
		if( bEnd != FALSE )
			return;
		lLineStart = lLineStop;
		lLineStop  = lLineStart + lLinePerPrintPage;
		if(lLineStop >= (ULONG)pDoc->GetLineCount())
		{
			lLineStop = pDoc->GetLineCount();
		}
		for(lCnt=0,lLineCnt=lLineStart; lLineCnt<lLineStop; lCnt++,lLineCnt++)
		{
			objLineNoStr = _T("");
			if( m_bPrintLineNo ) //行番号印刷する
				objLineNoStr.Format(" %6d | ",lLineCnt+1);
			else
				objLineNoStr = _T("  ");

			//ファイル比較、ディレクトリ比較での相違の目印
			nDiffLineKind = pDoc->GetDiffLineKind(lLineCnt);
			nDirCmpLineKind = pDoc->GetDirCmpLineKind(lLineCnt);
			if(nDiffLineKind == DIFF_LINEKIND_ADDED)
				objLineNoStr += _T("+ ");
			else if(nDiffLineKind == DIFF_LINEKIND_DELETED)
				objLineNoStr += _T("- ");
			else if(nDirCmpLineKind == DIRCMP_LINEKIND_DIFF)
				objLineNoStr += _T("! ");
			else
				objLineNoStr += _T("  ");

			pDC->TextOut( pInfo->m_rectDraw.left + pInfo->m_rectDraw.Width()/2,
						  ((lCnt+2)*lPrintLineHeight) + pInfo->m_rectDraw.top,
						  objLineNoStr);

			//１行
			pobjLine = pDoc->GetOneLine(lLineCnt);
			if(pobjLine != NULL) //あった
			{
				lMargin = pInfo->m_rectDraw.Width()/2 + objLineNoStr.GetLength()*lPrintCharWidth;
				lMargin += pInfo->m_rectDraw.left;
				MyTextOutForPrint(  pDC,
									lMargin,
									((lCnt+2)*lPrintLineHeight) + pInfo->m_rectDraw.top,
									pobjLine,
									lPrintCharWidth,
									lLineCnt,
									pInfo->m_rectDraw.Width());
			}
		}
	}
}


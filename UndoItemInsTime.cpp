// UndoItemInsTime.cpp: CUndoItemInsTime クラスのインプリメンテーション
//

#include "stdafx.h"
#include <Afxtempl.h>			//ＭＦＣのＣＭａｐを使用する
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include "Editor_Def.h"
#include "ColorManager.h"
#include "Line.h"
#include "LineManager.h"
#include "UndoManager.h"
#include "EditorDoc.h"
#include "BCMenu.h"
#include "EditorView.h"
#include "UndoItem.h"
#include "UndoItemInsTime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUndoItemInsTime::CUndoItemInsTime()
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = NULL;

	m_lCaretLine = 0;
	m_lCaretOffset = 0;
	m_lSelectStartLine = 0;
	m_lSelectStartOffset = 0;
	m_lSelectEndLine = 0;
	m_lSelectEndOffset = 0;

	m_objTimeStr = _T("");

	m_bOriginalModifiedFlag = 0;
	m_objOriginalDocTitle = _T("");
	m_objOriginalCaretPos.x = 0;
	m_objOriginalCaretPos.y = 0;
	m_objOriginalScrollPosLog.x = 0;
	m_objOriginalScrollPosLog.y = 0;
	m_bOriginalDispRuler = 0;
	m_bOriginalDispLineNo = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUndoItemInsTime::CUndoItemInsTime
	(	CEditorDoc*		pobjEditorDoc
	)
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = pobjEditorDoc;

	m_lCaretLine = 0;
	m_lCaretOffset = 0;
	m_lSelectStartLine = 0;
	m_lSelectStartOffset = 0;
	m_lSelectEndLine = 0;
	m_lSelectEndOffset = 0;

	m_objTimeStr = _T("");

	m_bOriginalModifiedFlag = 0;
	m_objOriginalDocTitle = _T("");
	m_objOriginalCaretPos.x = 0;
	m_objOriginalCaretPos.y = 0;
	m_objOriginalScrollPosLog.x = 0;
	m_objOriginalScrollPosLog.y = 0;
	m_bOriginalDispRuler = 0;
	m_bOriginalDispLineNo = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUndoItemInsTime::~CUndoItemInsTime()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：実行																	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemInsTime::Do()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：実行																	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemInsTime::Do 
	(	long		lCurrentLine,			// キャレット位置行
		long		lCurrentOffset,			// キャレット位置オフセット
		CString&	objTimeStr,				// 時刻文字列
		CPoint&		objCaretPos,			// キャレット位置
		CPoint& 	objScrollPosLog,		// スクロール位置
		BOOL		bDispRuler,				// ルーラ表示フラグ
		BOOL		bDispLineNo				// 行番号表示フラグ
	)
{
	/////////////////////////////
	//	実行前情報保持
	/////////////////////////////
	//パラメータ
	m_lCaretLine              = lCurrentLine;
	m_lCaretOffset            = lCurrentOffset;
	m_objTimeStr              = objTimeStr;
	m_objOriginalCaretPos     = objCaretPos;
	m_objOriginalScrollPosLog = objScrollPosLog;
	m_bOriginalDispRuler      = bDispRuler;
	m_bOriginalDispLineNo     = bDispLineNo;

	//ドキュメント情報
	m_bOriginalModifiedFlag = m_pobjEditorDoc->IsModified();
	m_objOriginalDocTitle   = m_pobjEditorDoc->GetTitle();

	///////////////////////////////
	//	処理実行
	///////////////////////////////

	//時刻文字列挿入
	CLine* pobjLine = m_pobjEditorDoc->GetOneLine(lCurrentLine);
	if( pobjLine != NULL )
	{
		pobjLine->Insert(lCurrentOffset,(const unsigned char*)(LPCTSTR)objTimeStr);
		m_pobjEditorDoc->SetOneLine(lCurrentLine,pobjLine);
	}
	else
	{
		pobjLine = new CLine((const unsigned char*)(LPCTSTR)objTimeStr);
		m_pobjEditorDoc->InsertOneLine(lCurrentLine,pobjLine);
	}

	//時刻文字列挿入処理後の行、列情報保持
	m_lSelectStartLine   = lCurrentLine;
	m_lSelectStartOffset = lCurrentOffset;
	m_lSelectEndLine     = lCurrentLine;
	m_lSelectEndOffset   = lCurrentOffset + objTimeStr.GetLength();

	//////////////
	//	後処理	//
	//////////////

	//ドキュメントダーティ
	m_pobjEditorDoc->SetModifiedFlag(TRUE);

	//タイトル更新
	CString objTitle = m_pobjEditorDoc->GetTitle();
	if( (objTitle.GetLength() < 2) ||
		(objTitle.Right(2) != " *")
	)
	{
		objTitle += " *";
		m_pobjEditorDoc->SetTitle(objTitle);
	}

	POSITION pos = m_pobjEditorDoc->GetFirstViewPosition();
	while(pos != 0)
	{
		CEditorView* pView = (CEditorView*)m_pobjEditorDoc->GetNextView(pos);

		//キャレット位置設定(1999.03.20 Add for StatusBar Pane ID_INDICATOR_BYTE)
//		pView->MySetCaretPos(objCaretPos);
		pView->MySetCaretToLineOffset(m_lSelectEndLine,m_lSelectEndOffset);

		//選択範囲再設定
		pView->m_lSelectStartLine   = m_lSelectStartLine;
		pView->m_lSelectStartOffset = m_lSelectStartOffset;
		pView->m_lSelectEndLine     = m_lSelectEndLine;
		pView->m_lSelectEndOffset   = m_lSelectEndOffset;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Undo 実行																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemInsTime::doUndo()
{
	///////////////////////////////
	//	処理実行
	///////////////////////////////

	CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lCaretLine);
	if( pobjLine != NULL )
	{
		pobjLine->Trim(m_lSelectStartOffset,m_lSelectEndOffset);
		m_pobjEditorDoc->SetOneLine(m_lCaretLine,pobjLine);
	}

	//////////////
	//	後処理	//
	//////////////

	//ドキュメント情報復元
	m_pobjEditorDoc->SetModifiedFlag(m_bOriginalModifiedFlag);
	m_pobjEditorDoc->SetTitle(m_objOriginalDocTitle);

	POSITION pos = m_pobjEditorDoc->GetFirstViewPosition();
	while(pos != 0)
	{
		CEditorView* pView = (CEditorView*)m_pobjEditorDoc->GetNextView(pos);

		//ルーラ、行番号表示の復元
		pView->SetFlagDispRuler(m_bOriginalDispRuler);
		pView->SetFlagDispLineNo(m_bOriginalDispLineNo);

		//スクロール位置、キャレット位置の復元
		pView->MyScrollToPosition(m_objOriginalScrollPosLog);
		pView->MySetCaretPos(m_objOriginalCaretPos);

		//選択範囲解除
		pView->m_lSelectStartLine   = -1;
		pView->m_lSelectStartOffset = -1;
		pView->m_lSelectEndLine     = -1;
		pView->m_lSelectEndOffset   = -1;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Redo 実行																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemInsTime::doRedo()
{
	///////////////////////////////
	//	処理実行
	///////////////////////////////

	//時刻文字列挿入
	char* pPtr = m_objTimeStr.GetBuffer(1024);
	CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lCaretLine);
	if( pobjLine != NULL )
	{
		pobjLine->Insert(m_lCaretOffset,(const unsigned char*)pPtr);
		m_pobjEditorDoc->SetOneLine(m_lCaretLine,pobjLine);
	}
	else
	{
		pobjLine = new CLine((const unsigned char*)pPtr);
		m_pobjEditorDoc->InsertOneLine(m_lCaretLine,pobjLine);
	}
	m_objTimeStr.ReleaseBuffer();

	//////////////
	//	後処理	//
	//////////////

	//ドキュメントダーティ
	m_pobjEditorDoc->SetModifiedFlag(TRUE);

	//タイトル更新
	CString objTitle = m_pobjEditorDoc->GetTitle();
	if( (objTitle.GetLength() < 2) ||
		(objTitle.Right(2) != " *")
	)
	{
		objTitle += " *";
		m_pobjEditorDoc->SetTitle(objTitle);
	}

	POSITION pos = m_pobjEditorDoc->GetFirstViewPosition();
	while(pos != 0)
	{
		CEditorView* pView = (CEditorView*)m_pobjEditorDoc->GetNextView(pos);

		//ルーラ、行番号表示の復元
		pView->SetFlagDispRuler(m_bOriginalDispRuler);
		pView->SetFlagDispLineNo(m_bOriginalDispLineNo);

		//スクロール位置、キャレット位置の復元
		pView->MyScrollToPosition(m_objOriginalScrollPosLog);
//		pView->MySetCaretPos(m_objOriginalCaretPos);
		pView->MySetCaretToLineOffset(m_lSelectEndLine,m_lSelectEndOffset);

		//選択範囲再設定
		pView->m_lSelectStartLine   = m_lSelectStartLine;
		pView->m_lSelectStartOffset = m_lSelectStartOffset;
		pView->m_lSelectEndLine     = m_lSelectEndLine;
		pView->m_lSelectEndOffset   = m_lSelectEndOffset;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Undo 実行可能調査														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoItemInsTime::IsCanUndo()
{
	return m_bIsCanUndo;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Redo 実行可能調査														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoItemInsTime::IsCanRedo()
{
	return m_bIsCanRedo;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Undo 可能設定															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemInsTime::setCanUndo
	(	BOOL	bIsCanUndo
	)
{
	m_bIsCanUndo = bIsCanUndo;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Redo 可能設定															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemInsTime::setCanRedo
	(	BOOL	bIsCanRedo
	)
{
	m_bIsCanRedo = bIsCanRedo;
}


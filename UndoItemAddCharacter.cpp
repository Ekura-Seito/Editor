// UndoItemAddCharacter.cpp: CUndoItemAddCharacter クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

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
#include "UndoItemAddCharacter.h"

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
CUndoItemAddCharacter::CUndoItemAddCharacter()
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = NULL;

	m_lCurrentLine = 0;
	m_lCurrentOffset = 0;
	m_n1stChar = 0;
	m_n2ndChar = 0;
	m_bInsertKey = 0;
	m_nOriginal1stChar = 0;
	m_nOriginal2ndChar = 0;
	m_nOriginal3rdChar = 0;
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
CUndoItemAddCharacter::CUndoItemAddCharacter
	(	CEditorDoc*		pobjEditorDoc
	)
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = pobjEditorDoc;

	m_lCurrentLine = 0;
	m_lCurrentOffset = 0;
	m_n1stChar = 0;
	m_n2ndChar = 0;
	m_bInsertKey = 0;
	m_nOriginal1stChar = 0;
	m_nOriginal2ndChar = 0;
	m_nOriginal3rdChar = 0;
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
CUndoItemAddCharacter::~CUndoItemAddCharacter()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：実行																	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemAddCharacter::Do()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：実行																	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemAddCharacter::Do
	(	long	lCurrentLine,			// 文字登録対象の行番号
		long	lCurrentOffset,			// 文字登録対象の列番号（ﾊﾞｲﾄ単位）
		UINT	n1stChar,				// 登録文字第一ﾊﾞｲﾄ
		UINT	n2ndChar,				// 登録文字第二ﾊﾞｲﾄ
		BOOL	bInsertKey,				// 挿入／上書きﾓｰﾄﾞﾌﾗｸﾞ
		CPoint&	objCaretPos,			// キャレット位置
		CPoint& objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	/////////////////////////////
	//	実行前情報保持
	/////////////////////////////
	//パラメータ
	m_lCurrentLine            = lCurrentLine;
	m_lCurrentOffset          = lCurrentOffset;
	m_n1stChar                = n1stChar;
	m_n2ndChar                = n2ndChar;
	m_bInsertKey              = bInsertKey;
	m_objOriginalCaretPos     = objCaretPos;
	m_objOriginalScrollPosLog = objScrollPosLog;
	m_bOriginalDispRuler      = bDispRuler;
	m_bOriginalDispLineNo     = bDispLineNo;

	//登録位置のオリジナル文字情報
	CLine* pobjLine = m_pobjEditorDoc->GetOneLine(lCurrentLine);
	if(pobjLine != NULL)
	{
		m_nOriginal1stChar = pobjLine->GetAt(lCurrentOffset);
		m_nOriginal2ndChar = pobjLine->GetAt(lCurrentOffset+1);
		m_nOriginal3rdChar = pobjLine->GetAt(lCurrentOffset+2);
	}

	//ドキュメント情報
	m_bOriginalModifiedFlag = m_pobjEditorDoc->IsModified();
	m_objOriginalDocTitle   = m_pobjEditorDoc->GetTitle();

	///////////////////////////////
	//	処理実行
	///////////////////////////////

	//文字登録
	m_pobjEditorDoc->m_objLineManager.AddCharacter(lCurrentLine,lCurrentOffset,n1stChar,n2ndChar,bInsertKey);

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
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Undo 実行																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemAddCharacter::doUndo()
{
	unsigned char	c1stChar = 0;
	unsigned char	c2ndChar = 0;

	if(m_bInsertKey) //挿入操作の取り消し
	{
		//挿入した位置にある文字をDELETEする
		m_pobjEditorDoc->m_objLineManager.ProcessDeleteCode(m_lCurrentLine,m_lCurrentOffset,&c1stChar,&c2ndChar);
	}
	else //上書き操作の取り消し
	{
		//オリジナル文字を上書きする
		if(IsDBCSLeadByteEx(0,m_n1stChar))	//２バイト文字入力操作取り消し
		{
			if( !IsDBCSLeadByteEx(0,m_nOriginal1stChar) &&
				IsDBCSLeadByteEx(0,m_nOriginal2ndChar)
			)
			{
				m_pobjEditorDoc->m_objLineManager.AddCharacter(m_lCurrentLine,m_lCurrentOffset,m_nOriginal1stChar,0,m_bInsertKey);
				m_pobjEditorDoc->m_objLineManager.AddCharacter(m_lCurrentLine,m_lCurrentOffset+1,m_nOriginal2ndChar,m_nOriginal3rdChar,m_bInsertKey);
			}
			else
			{
				m_pobjEditorDoc->m_objLineManager.AddCharacter(m_lCurrentLine,m_lCurrentOffset,m_nOriginal1stChar,m_nOriginal2ndChar,m_bInsertKey);
			}
		}
		else //１バイト文字入力操作取り消し
		{
			if(IsDBCSLeadByteEx(0,m_nOriginal1stChar))
				m_pobjEditorDoc->m_objLineManager.AddCharacter(m_lCurrentLine,m_lCurrentOffset,m_nOriginal1stChar,m_nOriginal2ndChar,m_bInsertKey);
			else
				m_pobjEditorDoc->m_objLineManager.AddCharacter(m_lCurrentLine,m_lCurrentOffset,m_nOriginal1stChar,0,m_bInsertKey);
		}
	}

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

		//キャレット位置＆スクロール位置の復元
		pView->MyScrollToPosition(m_objOriginalScrollPosLog);
		pView->MySetCaretPos(m_objOriginalCaretPos);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Redo 実行																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemAddCharacter::doRedo()
{
	//文字登録
	m_pobjEditorDoc->m_objLineManager.AddCharacter(
		m_lCurrentLine,
		m_lCurrentOffset,
		m_n1stChar,
		m_n2ndChar,
		m_bInsertKey);

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

		//キャレット位置＆スクロール位置更新
		pView->MyScrollToPosition(m_objOriginalScrollPosLog);
		pView->MySetCaretPos(m_objOriginalCaretPos);

		pView->MoveCaret(VK_RIGHT,1);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Undo 実行可能調査														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoItemAddCharacter::IsCanUndo()
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
BOOL CUndoItemAddCharacter::IsCanRedo()
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
void CUndoItemAddCharacter::setCanUndo
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
void CUndoItemAddCharacter::setCanRedo
	(	BOOL	bIsCanRedo
	)
{
	m_bIsCanRedo = bIsCanRedo;
}

// UndoItemDelChar.cpp: CUndoItemDelChar クラスのインプリメンテーション
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
#include "UndoItemDelChar.h"

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
CUndoItemDelChar::CUndoItemDelChar()
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = NULL;

	m_lCurrentLine = 0;
	m_lCurrentOffset = 0;
	m_cOriginal1stChar = 0;
	m_cOriginal2ndChar = 0;
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
CUndoItemDelChar::CUndoItemDelChar
	(	CEditorDoc*		pobjEditorDoc
	)
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = pobjEditorDoc;

	m_lCurrentLine = 0;
	m_lCurrentOffset = 0;
	m_cOriginal1stChar = 0;
	m_cOriginal2ndChar = 0;
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
CUndoItemDelChar::~CUndoItemDelChar()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：実行																	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemDelChar::Do()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：実行																	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemDelChar::Do
	(	long	lCurrentLine,			// 文字登録対象の行番号
		long	lCurrentOffset,			// 文字登録対象の列番号（ﾊﾞｲﾄ単位）
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

	unsigned char c1stChar=0;				//DELETEされた文字コードバッファ
	unsigned char c2ndChar=0;

	//DELETEコード処理
	m_pobjEditorDoc->m_objLineManager.ProcessDeleteCode(lCurrentLine,lCurrentOffset,&c1stChar,&c2ndChar);

	//DELETEされた文字を保持
	m_cOriginal1stChar = c1stChar;
	m_cOriginal2ndChar = c2ndChar;

	if( (m_cOriginal1stChar != 0) ||	//何かしらDELETEされた
		(m_cOriginal2ndChar != 0)
	)
	{
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

			//キャレット位置設定(1999.03.20 Add for StatusBar ID_INDICATOR_BYTE)
			pView->MySetCaretPos(objCaretPos);

			if( (m_cOriginal1stChar == '\r') &&	//行末でのDELETE
				(m_cOriginal2ndChar == '\n')
			)
			{
				//再描画
				pView->OnUpdate(NULL,NULL,NULL);
				pView->Invalidate(FALSE);
			}
			else
			{
				//キャレット行のみ再描画
				pView->OnUpdate(NULL,NULL,NULL);
				pView->InvalidateCaretLine();
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Undo 実行																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemDelChar::doUndo()
{
	if( (m_cOriginal1stChar == 0) &&		//何もDELETEされていなかった
		(m_cOriginal2ndChar == 0)
	)
	{
		// 何もしない
		return;
	}


	if( (m_cOriginal1stChar == '\r') &&	//行末でのDELETEのUNDO
		(m_cOriginal2ndChar == '\n')
	)
	{
		//改行コード登録
		m_pobjEditorDoc->m_objLineManager.ProcessReturnCode(m_lCurrentLine,m_lCurrentOffset);
	}
	else //文字DELETEのUNDO
	{
		//DELETE文字再登録
		m_pobjEditorDoc->m_objLineManager.AddCharacter(
							m_lCurrentLine,
							m_lCurrentOffset,
							m_cOriginal1stChar,
							m_cOriginal2ndChar,
							TRUE);
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
void CUndoItemDelChar::doRedo()
{
	unsigned char c1stChar=0;				//DELETEされた文字コードバッファ
	unsigned char c2ndChar=0;

	//DELETEコード処理
	m_pobjEditorDoc->m_objLineManager.ProcessDeleteCode(m_lCurrentLine,m_lCurrentOffset,&c1stChar,&c2ndChar);

	if( (m_cOriginal1stChar != 0) ||	//何かしらDELETEされた
		(m_cOriginal2ndChar != 0)
	)
	{
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

			//キャレット位置＆スクロール位置の復元
			pView->MyScrollToPosition(m_objOriginalScrollPosLog);
			pView->MySetCaretPos(m_objOriginalCaretPos);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Undo 実行可能調査														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoItemDelChar::IsCanUndo()
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
BOOL CUndoItemDelChar::IsCanRedo()
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
void CUndoItemDelChar::setCanUndo
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
void CUndoItemDelChar::setCanRedo
	(	BOOL	bIsCanRedo
	)
{
	m_bIsCanRedo = bIsCanRedo;
}


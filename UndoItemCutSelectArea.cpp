// UndoItemCutSelectArea.cpp: CUndoItemCutSelectArea クラスのインプリメンテーション
//

#include "stdafx.h"
#include <Afxtempl.h>			//ＭＦＣのＣＭａｐを使用する
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include <afxadv.h>			//for CSharedFile
#include "Editor_Def.h"
#include "ColorManager.h"
#include "Line.h"
#include "LineManager.h"
#include "UndoManager.h"
#include "EditorDoc.h"
#include "BCMenu.h"
#include "EditorView.h"
#include "UndoItem.h"
#include "UndoItemCutSelectArea.h"

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
CUndoItemCutSelectArea::CUndoItemCutSelectArea()
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = NULL;

	m_lSelectStartLine = 0;
	m_lSelectStartOffset = 0;
	m_lSelectEndLine = 0;
	m_lSelectEndOffset = 0;
	m_lSelectLineCnt = 0;
	m_ppobjSelectLine = NULL;
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
CUndoItemCutSelectArea::CUndoItemCutSelectArea
	(	CEditorDoc*		pobjEditorDoc
	)
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = pobjEditorDoc;

	m_lSelectStartLine = 0;
	m_lSelectStartOffset = 0;
	m_lSelectEndLine = 0;
	m_lSelectEndOffset = 0;
	m_lSelectLineCnt = 0;
	m_ppobjSelectLine = NULL;
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
CUndoItemCutSelectArea::~CUndoItemCutSelectArea()
{
	if( m_ppobjSelectLine != NULL )
	{
		for(ULONG lCnt=0; lCnt<(ULONG)m_lSelectLineCnt; lCnt++)
		{
			delete m_ppobjSelectLine[lCnt];
		}
		delete m_ppobjSelectLine;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：実行																	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemCutSelectArea::Do()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：実行																	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemCutSelectArea::Do
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		BOOL	bToClipBoard,			// クリップボードへコピーするか？(TRUE:する、FALSE:しない）
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
	m_lSelectStartLine        = lSelectStartLine;
	m_lSelectStartOffset      = lSelectStartOffset;
	m_lSelectEndLine          = lSelectEndLine;
	m_lSelectEndOffset        = lSelectEndOffset;
	m_bToClipBoard            = bToClipBoard;
	m_objOriginalCaretPos     = objCaretPos;
	m_objOriginalScrollPosLog = objScrollPosLog;
	m_bOriginalDispRuler      = bDispRuler;
	m_bOriginalDispLineNo     = bDispLineNo;

	//ドキュメント情報
	m_bOriginalModifiedFlag = m_pobjEditorDoc->IsModified();
	m_objOriginalDocTitle   = m_pobjEditorDoc->GetTitle();

	//選択範囲のテキスト取得
	m_lSelectLineCnt = lSelectEndLine - lSelectStartLine + 1;
	m_ppobjSelectLine = new CLine*[m_lSelectLineCnt];
	for(ULONG lCnt=0; lCnt<(ULONG)m_lSelectLineCnt; lCnt++)
	{
		CLine* pobjTempLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine+lCnt);
		m_ppobjSelectLine[lCnt] = new CLine();
		if( pobjTempLine != NULL )
			m_ppobjSelectLine[lCnt]->Set(pobjTempLine->String());
	}

	if( bToClipBoard )//クリップボードへのコピーをする場合
	{
		//ＯＬＥデータソース作成
		COleDataSource*	pobjOleDataSource = new COleDataSource();

		//コピーするデータ作成
		CSharedFile objShareFile(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
		if(lSelectStartLine == lSelectEndLine) //１行
		{
			CLine* pobjLine = (CLine*)m_pobjEditorDoc->GetOneLine(lSelectStartLine);
			objShareFile.Write( pobjLine->Data(lSelectStartOffset),
								(lSelectEndOffset-lSelectStartOffset) );
		}
		else //複数行
		{
			for(ULONG lLineNo=lSelectStartLine; lLineNo<=(ULONG)lSelectEndLine; lLineNo++)
			{
				CLine* pobjLine = (CLine*)m_pobjEditorDoc->GetOneLine(lLineNo);
				if(!pobjLine)
					continue;
				if(lLineNo == (ULONG)lSelectStartLine)//選択開始行
				{
					objShareFile.Write(	pobjLine->Data(lSelectStartOffset),
										(pobjLine->GetLength()-lSelectStartOffset) );
				}
				else if(lLineNo == (ULONG)lSelectEndLine)//選択終了行
				{
					objShareFile.Write( "\r\n", 2 );
					objShareFile.Write( pobjLine->String(), lSelectEndOffset );
				}
				else //途中行
				{
					objShareFile.Write( "\r\n", 2 );
					objShareFile.Write( pobjLine->String(), pobjLine->GetLength() );
				}
			}
		}

		//クリップボードへ
		pobjOleDataSource->CacheGlobalData(CF_TEXT, objShareFile.Detach());
		pobjOleDataSource->SetClipboard();
	}

	///////////////////////////////
	//	処理実行
	///////////////////////////////

	if( lSelectStartLine == lSelectEndLine ) //１行選択
	{
		CLine* pobjLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine);
		if( pobjLine != NULL )
		{
			//選択範囲切り取り
			pobjLine->Trim(lSelectStartOffset,lSelectEndOffset);
			m_pobjEditorDoc->SetOneLine(lSelectStartLine,pobjLine);
		}
	}
	else //複数行選択
	{
		CLine*	pobjStartLine = NULL;
		CLine*	pobjEndLine = NULL;
		for(ULONG lCnt=0; lCnt<(ULONG)(lSelectEndLine-lSelectStartLine+1); lCnt++ )
		{
			if( lCnt == 0 ) //選択開始行
			{
				pobjStartLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine);
				//選択開始オフセット以降切り取り
				if( pobjStartLine != NULL )
				{
					pobjStartLine->SetAt(lSelectStartOffset,0);
					m_pobjEditorDoc->SetOneLine(lSelectStartLine,pobjStartLine);
				}
			}
			else if( lCnt < (ULONG)(lSelectEndLine-lSelectStartLine) ) //選択中間行
			{
				m_pobjEditorDoc->DeleteOneLine(lSelectStartLine+1);
			}
			else if( lCnt == (ULONG)(lSelectEndLine-lSelectStartLine) )	//選択終了行
			{
				pobjEndLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine+1);
				//選択終了オフセットよりまえ切り取り
				if( pobjEndLine != NULL )
				{
					pobjEndLine->Trim(0,lSelectEndOffset);
					m_pobjEditorDoc->SetOneLine(lSelectStartLine+1,pobjEndLine);
				}
			}
		}
		//選択開始行と選択終了行を連結
		if( pobjEndLine != NULL )
		{
			pobjStartLine->Catenate(pobjEndLine->String());
			m_pobjEditorDoc->DeleteOneLine(lSelectStartLine+1);
		}
		m_pobjEditorDoc->SetOneLine(lSelectStartLine,pobjStartLine);
	}

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

		//選択範囲解除
		pView->m_lSelectStartLine = -1;
		pView->m_lSelectStartOffset = -1;
		pView->m_lSelectEndLine = -1;
		pView->m_lSelectEndOffset = -1;

		//現在のスクロール位置の行番号と、選択開始行番号を比べる
		long lScrollLine = 0;
		if( pView->m_lfHeight != 0 )
			lScrollLine = objScrollPosLog.y / (pView->m_lfHeight+pView->m_lLineInterval);
		if( lScrollLine >= m_lSelectStartLine )
		{
			//選択開始行、列へ移動
			pView->Jump2LineOffset(m_lSelectStartLine,m_lSelectStartOffset);
		}
		else
		{
			//1999.03.10 Add for test
			pView->MyScrollToPosition(objScrollPosLog);
		}

		//キャレットを選択開始位置へ位置づけ
		pView->MySetCaretToLineOffset(lSelectStartLine,lSelectStartOffset);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Undo 実行																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemCutSelectArea::doUndo()
{
	if( m_lSelectStartLine == m_lSelectEndLine ) //１行選択
	{
		CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
		//もとの行に入れ替える
		if( pobjLine != NULL )
		{
			pobjLine->Set(m_ppobjSelectLine[0]->String());
			m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjLine);
		}
	}
	else //複数行選択
	{
		for(ULONG lCnt=m_lSelectLineCnt-1; lCnt>=0; lCnt-- )
		{
			if( lCnt == 0 ) //選択開始行
			{
				CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
				//もとの行に入れ替える
				if( pobjLine != NULL )
				{
					pobjLine->Set(m_ppobjSelectLine[0]->String());
					m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjLine);
				}
				break;
			}
			else
			{
				//削除していた行を挿入しなおす
				CLine*	pobjLine = new CLine();
				pobjLine->Set(m_ppobjSelectLine[lCnt]->String());
				m_pobjEditorDoc->InsertOneLine(m_lSelectStartLine+1,pobjLine);
			}
		}
	}

	//ドキュメント情報復元
	m_pobjEditorDoc->SetModifiedFlag(m_bOriginalModifiedFlag);
	m_pobjEditorDoc->SetTitle(m_objOriginalDocTitle);


	POSITION pos = m_pobjEditorDoc->GetFirstViewPosition();
	while(pos != 0)
	{
		CEditorView* pView = (CEditorView*)m_pobjEditorDoc->GetNextView(pos);

		//選択範囲復元
		pView->m_lSelectStartLine   = m_lSelectStartLine;
		pView->m_lSelectStartOffset = m_lSelectStartOffset;
		pView->m_lSelectEndLine     = m_lSelectEndLine;
		pView->m_lSelectEndOffset   = m_lSelectEndOffset;

		//ルーラ、行番号表示の復元
		pView->SetFlagDispRuler(m_bOriginalDispRuler);
		pView->SetFlagDispLineNo(m_bOriginalDispLineNo);

		//キャレット位置＆スクロール位置の復元
//1999.03.06 復活
		pView->MyScrollToPosition(m_objOriginalScrollPosLog);
//1998.10.18 Cut By S.Ekura スクロール動作バグのフィックス
//		pView->MyScrollToPosition(m_objOriginalScrollPosLog);
//1998.10.18 Cut End
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
void CUndoItemCutSelectArea::doRedo()
{
	if( m_bToClipBoard )//クリップボードへのコピーをする場合
	{
		//ＯＬＥデータソース作成
		COleDataSource*	pobjOleDataSource = new COleDataSource();

		//コピーするデータ作成
		CSharedFile objShareFile(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
		if(m_lSelectStartLine == m_lSelectEndLine) //１行
		{
			CLine* pobjLine = (CLine*)m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
			objShareFile.Write( pobjLine->Data(m_lSelectStartOffset),
								(m_lSelectEndOffset-m_lSelectStartOffset) );
		}
		else //複数行
		{
			for(ULONG lLineNo=m_lSelectStartLine; lLineNo<=(ULONG)m_lSelectEndLine; lLineNo++)
			{
				CLine* pobjLine = (CLine*)m_pobjEditorDoc->GetOneLine(lLineNo);
				if(!pobjLine)
					continue;
				if(lLineNo == (ULONG)m_lSelectStartLine)//選択開始行
				{
					objShareFile.Write(	pobjLine->Data(m_lSelectStartOffset),
										(pobjLine->GetLength()-m_lSelectStartOffset) );
				}
				else if(lLineNo == (ULONG)m_lSelectEndLine)//選択終了行
				{
					objShareFile.Write( "\r\n", 2 );
					objShareFile.Write( pobjLine->String(), m_lSelectEndOffset );
				}
				else //途中行
				{
					objShareFile.Write( "\r\n", 2 );
					objShareFile.Write( pobjLine->String(), pobjLine->GetLength() );
				}
			}
		}

		//クリップボードへ
		pobjOleDataSource->CacheGlobalData(CF_TEXT, objShareFile.Detach());
		pobjOleDataSource->SetClipboard();
	}


	if( m_lSelectStartLine == m_lSelectEndLine ) //１行選択
	{
		CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
		//選択範囲切り取り
		if( pobjLine != NULL )
		{
			pobjLine->Trim(m_lSelectStartOffset,m_lSelectEndOffset);
			m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjLine);
		}
	}
	else //複数行選択
	{
		CLine*	pobjStartLine = NULL;
		CLine*	pobjEndLine = NULL;
		for(ULONG lCnt=0; lCnt<(ULONG)(m_lSelectEndLine-m_lSelectStartLine+1); lCnt++ )
		{
			if( lCnt == 0 ) //選択開始行
			{
				pobjStartLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
				//選択開始オフセット以降切り取り
				if( pobjStartLine != NULL )
				{
					pobjStartLine->SetAt(m_lSelectStartOffset,0);
					m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjStartLine);
				}
			}
			else if( lCnt < (ULONG)(m_lSelectEndLine-m_lSelectStartLine) ) //選択中間行
			{
				m_pobjEditorDoc->DeleteOneLine(m_lSelectStartLine+1);
			}
			else if( lCnt == (ULONG)(m_lSelectEndLine-m_lSelectStartLine) )	//選択終了行
			{
				pobjEndLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine+1);
				//選択終了オフセットよりまえ切り取り
				if( pobjEndLine != NULL )
				{
					pobjEndLine->Trim(0,m_lSelectEndOffset);
					m_pobjEditorDoc->SetOneLine(m_lSelectStartLine+1,pobjEndLine);
				}
			}
		}
		//選択開始行と選択終了行を連結
		if( pobjEndLine != NULL )
		{
			pobjStartLine->Catenate(pobjEndLine->String());
			m_pobjEditorDoc->DeleteOneLine(m_lSelectStartLine+1);
		}
		m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjStartLine);
	}

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

		//選択範囲解除
		pView->m_lSelectStartLine = -1;
		pView->m_lSelectStartOffset = -1;
		pView->m_lSelectEndLine = -1;
		pView->m_lSelectEndOffset = -1;

		//ルーラ、行番号表示の復元
		pView->SetFlagDispRuler(m_bOriginalDispRuler);
		pView->SetFlagDispLineNo(m_bOriginalDispLineNo);

		//キャレット位置＆スクロール位置の復元
		pView->MyScrollToPosition(m_objOriginalScrollPosLog);

		//スクロール位置の行番号と、選択開始行番号を比べる
		long lScrollLine = 0;
		if( pView->m_lfHeight != 0 )
			lScrollLine = m_objOriginalScrollPosLog.y / (pView->m_lfHeight+pView->m_lLineInterval);
		if( lScrollLine >= m_lSelectStartLine )
		{
			//選択開始行、列へ移動
			pView->Jump2LineOffset(m_lSelectStartLine,m_lSelectStartOffset);
		}
		else
		{
			//1999.03.10 Add for test
			pView->MyScrollToPosition(m_objOriginalScrollPosLog);
		}

		//キャレットを選択開始位置へ位置づけ
		pView->MySetCaretToLineOffset(m_lSelectStartLine,m_lSelectStartOffset);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Undo 実行可能調査														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoItemCutSelectArea::IsCanUndo()
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
BOOL CUndoItemCutSelectArea::IsCanRedo()
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
void CUndoItemCutSelectArea::setCanUndo
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
void CUndoItemCutSelectArea::setCanRedo
	(	BOOL	bIsCanRedo
	)
{
	m_bIsCanRedo = bIsCanRedo;
}


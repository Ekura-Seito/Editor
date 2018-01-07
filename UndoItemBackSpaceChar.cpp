// UndoItemBackSpaceChar.cpp: CUndoItemBackSpaceChar クラスのインプリメンテーション
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
#include "UndoItemBackSpaceChar.h"

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
CUndoItemBackSpaceChar::CUndoItemBackSpaceChar()
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = NULL;

	m_lCurrentLine = 0;
	m_lCurrentOffset = 0;
	m_lAfterLine = 0;
	m_lAfterOffset = 0;
	m_cOriginal1stChar = 0;
	m_cOriginal2ndChar = 0;
	m_bOriginalModifiedFlag = 0;
	m_objOriginalDocTitle = _T("");
	m_objOriginalCaretPos.x = 0;
	m_objOriginalCaretPos.y = 0;
	m_objAfterCaretPos.x = 0;
	m_objAfterCaretPos.y = 0;
	m_objPreLineEndPos.x = 0;
	m_objPreLineEndPos.y = 0;
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
CUndoItemBackSpaceChar::CUndoItemBackSpaceChar
	(	CEditorDoc*		pobjEditorDoc
	)
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = pobjEditorDoc;

	m_lCurrentLine = 0;
	m_lCurrentOffset = 0;
	m_lAfterLine = 0;
	m_lAfterOffset = 0;
	m_cOriginal1stChar = 0;
	m_cOriginal2ndChar = 0;
	m_bOriginalModifiedFlag = 0;
	m_objOriginalDocTitle = _T("");
	m_objOriginalCaretPos.x = 0;
	m_objOriginalCaretPos.y = 0;
	m_objAfterCaretPos.x = 0;
	m_objAfterCaretPos.y = 0;
	m_objPreLineEndPos.x = 0;
	m_objPreLineEndPos.y = 0;
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
CUndoItemBackSpaceChar::~CUndoItemBackSpaceChar()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：実行																	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemBackSpaceChar::Do()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：実行																	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemBackSpaceChar::Do
	(	long	lCurrentLine,			// BACK SPACE 対象の行番号
		long	lCurrentOffset,			// BACK SPACE 対象の列番号（ﾊﾞｲﾄ単位）
		CPoint&	objCaretPos,			// キャレット位置
		CPoint& objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//クライアント領域
	CRect clientRect;
	int	nHeight,nWidth;
	POSITION pos = m_pobjEditorDoc->GetFirstViewPosition();
	while(pos != 0)
	{
		CEditorView* pView = (CEditorView*)m_pobjEditorDoc->GetNextView(pos);

		pView->MyGetClientRect(&clientRect);
		clientRect.NormalizeRect();
		nHeight = clientRect.Height();
		nWidth = clientRect.Width();
		if( (pView->m_lfWidth==0) || (pView->m_lfHeight==0) )
			return;
		nHeight -= ( clientRect.Height() % (pView->m_lfHeight+pView->m_lLineInterval) );
		nWidth -= ( clientRect.Width() % pView->m_lfWidth );
	}

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

	if( (lCurrentLine > 0) && (lCurrentOffset == 0) ) //行の先頭でのBACK SPACE
	{
		//BACK SPACE する前に前行の行末位置を記憶
		POSITION pos = m_pobjEditorDoc->GetFirstViewPosition();
		while(pos != 0)
		{
			CEditorView* pView = (CEditorView*)m_pobjEditorDoc->GetNextView(pos);
			CPoint	objScrollPosLog(pView->GetScrollPosLogX(),pView->GetScrollPosLogY());

			//キャレット位置　デバイス－＞論理座標
			CPoint logicalPoint = pView->m_caretPos;
			logicalPoint -= pView->m_LineNoPos;
			logicalPoint -= pView->m_RulerPos;
			logicalPoint += objScrollPosLog;
			m_objPreLineEndPos.y = logicalPoint.y - (pView->m_lfHeight+pView->m_lLineInterval);
			pView->GetPosition(lCurrentLine-1,&m_objPreLineEndPos);
			//m_objPreLineEndPos は論理座標
		}
	}
	else
	{
		//BACK SPACE する前に前の位置を記憶
		POSITION pos = m_pobjEditorDoc->GetFirstViewPosition();
		while(pos != 0)
		{
			CEditorView* pView = (CEditorView*)m_pobjEditorDoc->GetNextView(pos);
			CPoint	objScrollPosLog(pView->GetScrollPosLogX(),pView->GetScrollPosLogY());

			//キャレット位置　デバイス－＞論理座標
			CPoint logicalPoint = pView->m_caretPos;
			logicalPoint -= pView->m_LineNoPos;
			logicalPoint -= pView->m_RulerPos;
			logicalPoint += objScrollPosLog;
			m_objAfterCaretPos.y = logicalPoint.y;
			pView->GetPosition1(lCurrentLine,&logicalPoint,&m_objAfterCaretPos);
			//m_objAfterCaretPos は論理座標
		}
	}

	///////////////////////////////
	//	処理実行
	///////////////////////////////

	unsigned char c1stChar=0;				//BACK SPACEされた文字コードバッファ
	unsigned char c2ndChar=0;

	//BACK SPACE コード処理
	m_pobjEditorDoc->m_objLineManager.ProcessBackSpaceCode(lCurrentLine,lCurrentOffset,&c1stChar,&c2ndChar);

	//BACK SPACE された文字を保持
	m_cOriginal1stChar = c1stChar;
	m_cOriginal2ndChar = c2ndChar;

	if( (m_cOriginal1stChar != 0) ||	//何かしら BACK SPACE された
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
			//////////////////////////////////////////////
			//	BACK SPACE 後のキャレット位置を求める
			//////////////////////////////////////////////
			CEditorView* pView = (CEditorView*)m_pobjEditorDoc->GetNextView(pos);
			pView->OnUpdate(NULL,NULL,NULL);

			if(c1stChar == '\r' && c2ndChar == '\n') //改行のBACK SPACEだった
			{
				if(lCurrentLine > 0)//まだ上へ行ける
				{
					//MyScrollToPotisionを１回のコールにするための処理
					CPoint	destPos(pView->GetScrollPosLogX(),pView->GetScrollPosLogY());
					//キャレットを文字列の最後尾へ
					CPoint	tempLogPos = m_objPreLineEndPos;
					if(tempLogPos.x > nWidth)//表示範囲の右側へ行った場合
					{
						//右方向スクロール
						long lLogX = tempLogPos.x - nWidth + 1;
						destPos.x = lLogX;
					}
					//論理座標－＞デバイス座標
					tempLogPos -= destPos/*pView->m_ScrollPosLog*/;
					tempLogPos += pView->m_RulerPos;
					tempLogPos += pView->m_LineNoPos;
					if(tempLogPos.y < pView->m_RulerPos.y )//表示範囲の上側に行った場合
					{
						tempLogPos.y = pView->m_RulerPos.y;
						//１行分上方向スクロール
						destPos.y -= (pView->m_lfHeight+pView->m_lLineInterval);
					}
					if( destPos.x != pView->GetScrollPosLogX() ||
						destPos.y != pView->GetScrollPosLogY() )
					{
						pView->MyScrollToPosition(destPos);
					}
					//キャレット移動
					pView->MySetCaretPos(tempLogPos);
					//再描画
					pView->OnUpdate(NULL,NULL,NULL);
					pView->Invalidate(FALSE);
				}
			}
			else //改行ではなかった
			{
				CPoint objCaretPos = m_objAfterCaretPos;
				//論理座標－＞デバイス座標
				objCaretPos.x -= pView->GetScrollPosLogX();
				objCaretPos.y -= pView->GetScrollPosLogY();
				objCaretPos += pView->m_RulerPos;
				objCaretPos += pView->m_LineNoPos;
				if(objCaretPos.x < pView->m_LineNoPos.x)//表示範囲の左側へ行った場合
				{
					//左方向スクロール
					long lLogX = objCaretPos.x - pView->m_LineNoPos.x - pView->m_RulerPos.x + pView->GetScrollPosLogX();
					pView->MyScrollToPosition(CPoint(lLogX, pView->GetScrollPosLogY()));
					objCaretPos.x = pView->m_LineNoPos.x;
					//キャレット移動
					pView->MySetCaretPos(objCaretPos);
					//再描画
					pView->OnUpdate(NULL,NULL,NULL);
					pView->Invalidate(FALSE);
				}
				else
				{
					//キャレット移動
					pView->MySetCaretPos(objCaretPos);
					//キャレット行のみ再描画
					pView->OnUpdate(NULL,NULL,NULL);
					pView->InvalidateCaretLine();
				}
			}

			////////////////////////////////////////////////
			//	BACK SPACE 後の行番号と列番号を求める
			////////////////////////////////////////////////
			long lLine, lOffset;
			if( pView->CaretToLineOffset(&lLine,&lOffset) != FALSE )
			{
				m_lAfterLine = lLine;
				m_lAfterOffset = lOffset;
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
void CUndoItemBackSpaceChar::doUndo()
{
	if( (m_cOriginal1stChar == 0) &&		//何も BACK SPACE されていなかった
		(m_cOriginal2ndChar == 0)
	)
	{
		// 何もしない
		return;
	}


	if( (m_cOriginal1stChar == '\r') &&	//行頭での改行コード BACK SPACE の UNDO
		(m_cOriginal2ndChar == '\n')
	)
	{
		//改行コード登録
		m_pobjEditorDoc->m_objLineManager.ProcessReturnCode(m_lAfterLine,m_lAfterOffset);
	}
	else //文字 BACK SPACE のUNDO
	{
		//BACK SPACE文字再登録
		m_pobjEditorDoc->m_objLineManager.AddCharacter(
							m_lAfterLine,
							m_lAfterOffset,
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
void CUndoItemBackSpaceChar::doRedo()
{
	//クライアント領域
	CRect clientRect;
	int	nHeight,nWidth;
	POSITION pos = m_pobjEditorDoc->GetFirstViewPosition();
	while(pos != 0)
	{
		CEditorView* pView = (CEditorView*)m_pobjEditorDoc->GetNextView(pos);

		pView->MyGetClientRect(&clientRect);
		clientRect.NormalizeRect();
		nHeight = clientRect.Height();
		nWidth = clientRect.Width();
		if( (pView->m_lfWidth==0) || (pView->m_lfHeight==0) )
			return;
		nHeight -= ( clientRect.Height() % (pView->m_lfHeight+pView->m_lLineInterval) );
		nWidth -= ( clientRect.Width() % pView->m_lfWidth );
	}
	
	unsigned char c1stChar=0;				//BACK SPACEされた文字コードバッファ
	unsigned char c2ndChar=0;

	//BACK SPACE コード処理
	m_pobjEditorDoc->m_objLineManager.ProcessBackSpaceCode(m_lCurrentLine,m_lCurrentOffset,&c1stChar,&c2ndChar);

	if( (c1stChar != 0) ||	//何かしら BACK SPACE された
		(c2ndChar != 0)
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
			pView->OnUpdate(NULL,NULL,NULL);

			//ルーラ、行番号表示の復元
			pView->SetFlagDispRuler(m_bOriginalDispRuler);
			pView->SetFlagDispLineNo(m_bOriginalDispLineNo);

			//キャレット位置＆スクロール位置の復元
			pView->MyScrollToPosition(m_objOriginalScrollPosLog);
			pView->MySetCaretPos(m_objOriginalCaretPos);

			if(c1stChar == '\r' && c2ndChar == '\n') //改行のBACK SPACEだった
			{
				if(m_lCurrentLine > 0)//まだ上へ行ける
				{
					//MyScrollToPotisionを１回のコールにするための処理
					CPoint	destPos(pView->GetScrollPosLogX(),pView->GetScrollPosLogY());
					//キャレットを文字列の最後尾へ
					CPoint	tempLogPos = m_objPreLineEndPos;
					if(tempLogPos.x > nWidth)//表示範囲の右側へ行った場合
					{
						//右方向スクロール
						long lLogX = tempLogPos.x - nWidth + 1;
						destPos.x = lLogX;
					}
					//論理座標－＞デバイス座標
					tempLogPos -= destPos/*pView->m_ScrollPosLog*/;
					tempLogPos += pView->m_RulerPos;
					tempLogPos += pView->m_LineNoPos;
					if(tempLogPos.y < pView->m_RulerPos.y )//表示範囲の上側に行った場合
					{
						tempLogPos.y = pView->m_RulerPos.y;
						//１行分上方向スクロール
						destPos.y -= (pView->m_lfHeight+pView->m_lLineInterval);
					}
					if( destPos.x != pView->GetScrollPosLogX() ||
						destPos.y != pView->GetScrollPosLogY() )
					{
						pView->MyScrollToPosition(destPos);
					}
					//キャレット移動
					pView->MySetCaretPos(tempLogPos);
				}
			}
			else //改行ではなかった
			{
				CPoint objCaretPos = m_objAfterCaretPos;
				//論理座標－＞デバイス座標
				objCaretPos.x -= pView->GetScrollPosLogX();
				objCaretPos.y -= pView->GetScrollPosLogY();
				objCaretPos += pView->m_RulerPos;
				objCaretPos += pView->m_LineNoPos;
				if(objCaretPos.x < pView->m_LineNoPos.x)//表示範囲の左側へ行った場合
				{
					//左方向スクロール
					long lLogX = objCaretPos.x - pView->m_LineNoPos.x - pView->m_RulerPos.x + pView->GetScrollPosLogX();
					pView->MyScrollToPosition(CPoint(lLogX, pView->GetScrollPosLogY()));
					objCaretPos.x = pView->m_LineNoPos.x;
				}
				//キャレット移動
				pView->MySetCaretPos(objCaretPos);
			}
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
BOOL CUndoItemBackSpaceChar::IsCanUndo()
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
BOOL CUndoItemBackSpaceChar::IsCanRedo()
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
void CUndoItemBackSpaceChar::setCanUndo
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
void CUndoItemBackSpaceChar::setCanRedo
	(	BOOL	bIsCanRedo
	)
{
	m_bIsCanRedo = bIsCanRedo;
}


// UndoItemSpace2Tab.cpp: CUndoItemSpace2Tab クラスのインプリメンテーション
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
#include "UndoItemSpace2Tab.h"

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
CUndoItemSpace2Tab::CUndoItemSpace2Tab()
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = NULL;

	m_lfWidth = 0;
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
CUndoItemSpace2Tab::CUndoItemSpace2Tab
	(	CEditorDoc*		pobjEditorDoc
	)
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = pobjEditorDoc;

	m_lfWidth = 0;
	m_lSelectStartLine = 0;
	m_lSelectStartOffset = 0;
	m_lSelectEndLine = 0;
	m_lSelectEndOffset = 0;
	m_lSelectLineCnt = 0;
	m_ppobjSelectLine = 0;

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
CUndoItemSpace2Tab::~CUndoItemSpace2Tab()
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
void CUndoItemSpace2Tab::Do()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：実行																	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemSpace2Tab::Do 
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint& objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo,			// 行番号表示フラグ
		long	lfWidth					// フォント幅
	)
{
	long lEndOffsetAfter=0;

	/////////////////////////////
	//	実行前情報保持
	/////////////////////////////
	//パラメータ
	m_lSelectStartLine        = lSelectStartLine;
	m_lSelectStartOffset      = lSelectStartOffset;
	m_lSelectEndLine          = lSelectEndLine;
	m_lSelectEndOffset        = lSelectEndOffset;
	m_objOriginalCaretPos     = objCaretPos;
	m_objOriginalScrollPosLog = objScrollPosLog;
	m_bOriginalDispRuler      = bDispRuler;
	m_bOriginalDispLineNo     = bDispLineNo;
	m_lfWidth				  = lfWidth;

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

	///////////////////////////////
	//	処理実行
	///////////////////////////////

	//選択開始位置までのオフセットを計算
	long nOffset=0;
	CLine* pobjLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine);
	if(pobjLine != NULL)
	{
		for(long nCnt=0; nCnt<lSelectStartOffset; )
		{
			if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//２バイト文字の第一バイト
			{
				nCnt+=2;
				nOffset += (lfWidth * 2);
			}
			else //１バイト文字
			{
				if(pobjLine->GetAt(nCnt) == '\t')//タブコード
				{
					//マージン計算
					int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/lfWidth) % m_pobjEditorDoc->GetTabSize() );
					nOffset += (lfWidth * nMargin);
					nCnt++;
				}
				else //タブ以外
				{
					nCnt++;
					nOffset += lfWidth;
				}
			}
		}
	}


	if( lSelectStartLine == lSelectEndLine ) //１行選択
	{
		CLine* pobjLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine);
		if( pobjLine != NULL )
		{
			lEndOffsetAfter = lSelectEndOffset;
			for(long nCnt=lSelectStartOffset; nCnt<lEndOffsetAfter; )
			{
				if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//２バイト文字の第一バイト
				{
					nCnt+=2;
					nOffset += (lfWidth * 2);
					continue;
				}
				if(pobjLine->GetAt(nCnt) == '\t')//タブコード
				{
					//マージン計算
					int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/lfWidth) % m_pobjEditorDoc->GetTabSize() );
					nOffset += (lfWidth * nMargin);
					nCnt++;
					continue;
				}
				else if(pobjLine->GetAt(nCnt) != ' ')//半角スペース以外
				{
					nCnt++;
					nOffset += lfWidth;
					continue;
				}

				//マージンを計算
				int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/lfWidth) % m_pobjEditorDoc->GetTabSize() );
				//スペース数をカウント
				int nSpaceCnt=0;
				for(int i=0; ; )
				{
					if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+i)))
						break;
					else if(pobjLine->GetAt(nCnt+i) == ' ') //半角スペース継続
					{
						i++;
						nSpaceCnt++;
					}
					else
						break;
				}
				//タブに置き換えられる分だけスペースがあるかどうかチェック
				if(nSpaceCnt < nMargin) //ない！！
				{
					nCnt++;
					nOffset += lfWidth;
					continue;
				}

				//マージンの分だけ半角スペースを削除
				unsigned char c1stChar=0, c2ndChar=0;
				for(int i=0; i<nMargin; i++)
					pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
				//タブを挿入
				pobjLine->InsertChar(nCnt,VK_TAB,0);

				nOffset += (lfWidth * nMargin);
				nCnt++;
				//終了オフセットを更新
				lEndOffsetAfter-=nMargin;
				lEndOffsetAfter++;
			}

			//再設定(1999.03.20 Add for StatusBar Pane ID_INDICATOR_BYTE)
			m_pobjEditorDoc->SetOneLine(lSelectStartLine,pobjLine);
		}
	}
	else //複数行選択
	{
		for(ULONG lLineCnt=0; lLineCnt<(ULONG)(lSelectEndLine-lSelectStartLine+1); lLineCnt++ )//行ループ
		{
			CLine* pobjLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine+lLineCnt);
			if( pobjLine == NULL )
				continue;

			if( lLineCnt == 0 ) //選択開始行
			{
				long lEndOffset = pobjLine->GetLength();
				for(long nCnt=lSelectStartOffset; nCnt<lEndOffset; )
				{
					if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//２バイト文字の第一バイト
					{
						nCnt+=2;
						nOffset += (lfWidth * 2);
						continue;
					}
					if(pobjLine->GetAt(nCnt) == '\t')//タブコード
					{
						//マージン計算
						int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/lfWidth) % m_pobjEditorDoc->GetTabSize() );
						nOffset += (lfWidth * nMargin);
						nCnt++;
						continue;
					}
					else if(pobjLine->GetAt(nCnt) != ' ')//半角スペース以外
					{
						nCnt++;
						nOffset += lfWidth;
						continue;
					}
	
					//マージンを計算
					int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/lfWidth) % m_pobjEditorDoc->GetTabSize() );
					//スペース数をカウント
					int nSpaceCnt=0;
					for(int i=0; ; )
					{
						if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+i)))
							break;
						else if(pobjLine->GetAt(nCnt+i) == ' ') //半角スペース継続
						{
							i++;
							nSpaceCnt++;
						}
						else
							break;
					}
					//タブに置き換えられる分だけスペースがあるかどうかチェック
					if(nSpaceCnt < nMargin) //ない！！
					{
						nCnt++;
						nOffset += lfWidth;
						continue;
					}
	
					//マージンの分だけ半角スペースを削除
					unsigned char c1stChar=0, c2ndChar=0;
					for(int i=0; i<nMargin; i++)
						pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
					//タブを挿入
					pobjLine->InsertChar(nCnt,VK_TAB,0);
	
					nOffset += (lfWidth * nMargin);
					nCnt++;
					//終了オフセットを更新
					lEndOffset-=nMargin;
					lEndOffset++;
				}
			}
			else if( lLineCnt < (ULONG)(lSelectEndLine-lSelectStartLine) ) //選択中間行
			{
				nOffset=0;
				long lEndOffset = pobjLine->GetLength();
				for(long nCnt=0; nCnt<lEndOffset; )
				{
					if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//２バイト文字の第一バイト
					{
						nCnt+=2;
						nOffset += (lfWidth * 2);
						continue;
					}
					if(pobjLine->GetAt(nCnt) == '\t')//タブコード
					{
						//マージン計算
						int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/lfWidth) % m_pobjEditorDoc->GetTabSize() );
						nOffset += (lfWidth * nMargin);
						nCnt++;
						continue;
					}
					else if(pobjLine->GetAt(nCnt) != ' ')//半角スペース以外
					{
						nCnt++;
						nOffset += lfWidth;
						continue;
					}
	
					//マージンを計算
					int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/lfWidth) % m_pobjEditorDoc->GetTabSize() );
					//スペース数をカウント
					int nSpaceCnt=0;
					for(int i=0; ; )
					{
						if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+i)))
							break;
						else if(pobjLine->GetAt(nCnt+i) == ' ') //半角スペース継続
						{
							i++;
							nSpaceCnt++;
						}
						else
							break;
					}
					//タブに置き換えられる分だけスペースがあるかどうかチェック
					if(nSpaceCnt < nMargin) //ない！！
					{
						nCnt++;
						nOffset += lfWidth;
						continue;
					}
	
					//マージンの分だけ半角スペースを削除
					unsigned char c1stChar=0, c2ndChar=0;
					for(int i=0; i<nMargin; i++)
						pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
					//タブを挿入
					pobjLine->InsertChar(nCnt,VK_TAB,0);
	
					nOffset += (lfWidth * nMargin);
					nCnt++;
					//終了オフセットを更新
					lEndOffset-=nMargin;
					lEndOffset++;
				}
			}
			else if( lLineCnt == (ULONG)(lSelectEndLine-lSelectStartLine) )	//選択終了行
			{
				nOffset=0;
				lEndOffsetAfter = lSelectEndOffset;
				for(long nCnt=0; nCnt<lEndOffsetAfter; )
				{
					if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//２バイト文字の第一バイト
					{
						nCnt+=2;
						nOffset += (lfWidth * 2);
						continue;
					}
					if(pobjLine->GetAt(nCnt) == '\t')//タブコード
					{
						//マージン計算
						int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/lfWidth) % m_pobjEditorDoc->GetTabSize() );
						nOffset += (lfWidth * nMargin);
						nCnt++;
						continue;
					}
					else if(pobjLine->GetAt(nCnt) != ' ')//半角スペース以外
					{
						nCnt++;
						nOffset += lfWidth;
						continue;
					}
	
					//マージンを計算
					int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/lfWidth) % m_pobjEditorDoc->GetTabSize() );
					//スペース数をカウント
					int nSpaceCnt=0;
					for(int i=0; ; )
					{
						if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+i)))
							break;
						else if(pobjLine->GetAt(nCnt+i) == ' ') //半角スペース継続
						{
							i++;
							nSpaceCnt++;
						}
						else
							break;
					}
					//タブに置き換えられる分だけスペースがあるかどうかチェック
					if(nSpaceCnt < nMargin) //ない！！
					{
						nCnt++;
						nOffset += lfWidth;
						continue;
					}
	
					//マージンの分だけ半角スペースを削除
					unsigned char c1stChar=0, c2ndChar=0;
					for(int i=0; i<nMargin; i++)
						pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
					//タブを挿入
					pobjLine->InsertChar(nCnt,VK_TAB,0);
	
					nOffset += (lfWidth * nMargin);
					nCnt++;
					//終了オフセットを更新
					lEndOffsetAfter-=nMargin;
					lEndOffsetAfter++;
				}
			}

			//再設定(1999.03.20 Add for StatusBar Pane ID_INDICATOR_BYTE)
			m_pobjEditorDoc->SetOneLine(lSelectStartLine+lLineCnt,pobjLine);
		}
	}

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

		//キャレット位置を設定
		pView->MySetCaretToLineOffset(m_lSelectEndLine,lEndOffsetAfter);

		//範囲選択再設定
		pView->m_lSelectEndOffset = lEndOffsetAfter;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Undo 実行																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemSpace2Tab::doUndo()
{
	///////////////////////////////
	//	処理実行
	///////////////////////////////

	//保持しておいたオリジナル行をもとに戻す

	if( m_lSelectStartLine == m_lSelectEndLine ) //１行選択
	{
		CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
		if( pobjLine != NULL )
		{
			pobjLine->Set(m_ppobjSelectLine[0]->String());
			m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjLine);
		}
	}
	else //複数行選択
	{
		for(ULONG lCnt=0; lCnt<(ULONG)(m_lSelectEndLine-m_lSelectStartLine+1); lCnt++ )
		{
			CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine+lCnt);
			if( pobjLine != NULL )
			{
				pobjLine->Set(m_ppobjSelectLine[lCnt]->String());
				m_pobjEditorDoc->SetOneLine(m_lSelectStartLine+lCnt,pobjLine);
			}
		}
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

		//範囲選択解除
		pView->m_lSelectStartLine   = m_lSelectStartLine;
		pView->m_lSelectStartOffset = m_lSelectStartOffset;
		pView->m_lSelectEndLine     = m_lSelectEndLine;
		pView->m_lSelectEndOffset   = m_lSelectEndOffset;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Redo 実行																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemSpace2Tab::doRedo()
{
	long lEndOffsetAfter=0;

	///////////////////////////////
	//	処理実行
	///////////////////////////////

	//選択開始位置までのオフセットを計算
	long nOffset=0;
	CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
	if(pobjLine != NULL)
	{
		for(long nCnt=0; nCnt<m_lSelectStartOffset; )
		{
			if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//２バイト文字の第一バイト
			{
				nCnt+=2;
				nOffset += (m_lfWidth * 2);
			}
			else //１バイト文字
			{
				if(pobjLine->GetAt(nCnt) == '\t')//タブコード
				{
					//マージン計算
					int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/m_lfWidth) % m_pobjEditorDoc->GetTabSize() );
					nOffset += (m_lfWidth * nMargin);
					nCnt++;
				}
				else //タブ以外
				{
					nCnt++;
					nOffset += m_lfWidth;
				}
			}
		}
	}


	if( m_lSelectStartLine == m_lSelectEndLine ) //１行選択
	{
		CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
		if( pobjLine != NULL )
		{
			lEndOffsetAfter = m_lSelectEndOffset;
			for(long nCnt=m_lSelectStartOffset; nCnt<lEndOffsetAfter; )
			{
				if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//２バイト文字の第一バイト
				{
					nCnt+=2;
					nOffset += (m_lfWidth * 2);
					continue;
				}
				if(pobjLine->GetAt(nCnt) == '\t')//タブコード
				{
					//マージン計算
					int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/m_lfWidth) % m_pobjEditorDoc->GetTabSize() );
					nOffset += (m_lfWidth * nMargin);
					nCnt++;
					continue;
				}
				else if(pobjLine->GetAt(nCnt) != ' ')//半角スペース以外
				{
					nCnt++;
					nOffset += m_lfWidth;
					continue;
				}

				//マージンを計算
				int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/m_lfWidth) % m_pobjEditorDoc->GetTabSize() );
				//スペース数をカウント
				int nSpaceCnt=0;
				for(int i=0; ; )
				{
					if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+i)))
						break;
					else if(pobjLine->GetAt(nCnt+i) == ' ') //半角スペース継続
					{
						i++;
						nSpaceCnt++;
					}
					else
						break;
				}
				//タブに置き換えられる分だけスペースがあるかどうかチェック
				if(nSpaceCnt < nMargin) //ない！！
				{
					nCnt++;
					nOffset += m_lfWidth;
					continue;
				}

				//マージンの分だけ半角スペースを削除
				unsigned char c1stChar=0, c2ndChar=0;
				for(int i=0; i<nMargin; i++)
					pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
				//タブを挿入
				pobjLine->InsertChar(nCnt,VK_TAB,0);

				nOffset += (m_lfWidth * nMargin);
				nCnt++;
				//終了オフセットを更新
				lEndOffsetAfter-=nMargin;
				lEndOffsetAfter++;
			}

			//再設定(1999.03.20 Add for StatusBar Pane ID_INDICATOR_BYTE)
			m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjLine);
		}
	}
	else //複数行選択
	{
		for(ULONG lLineCnt=0; lLineCnt<(ULONG)(m_lSelectEndLine-m_lSelectStartLine+1); lLineCnt++ )//行ループ
		{
			CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine+lLineCnt);
			if( pobjLine == NULL )
				continue;

			if( lLineCnt == 0 ) //選択開始行
			{
				long lEndOffset = pobjLine->GetLength();
				for(long nCnt=m_lSelectStartOffset; nCnt<lEndOffset; )
				{
					if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//２バイト文字の第一バイト
					{
						nCnt+=2;
						nOffset += (m_lfWidth * 2);
						continue;
					}
					if(pobjLine->GetAt(nCnt) == '\t')//タブコード
					{
						//マージン計算
						int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/m_lfWidth) % m_pobjEditorDoc->GetTabSize() );
						nOffset += (m_lfWidth * nMargin);
						nCnt++;
						continue;
					}
					else if(pobjLine->GetAt(nCnt) != ' ')//半角スペース以外
					{
						nCnt++;
						nOffset += m_lfWidth;
						continue;
					}

					//マージンを計算
					int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/m_lfWidth) % m_pobjEditorDoc->GetTabSize() );
					//スペース数をカウント
					int nSpaceCnt=0;
					for(int i=0; ; )
					{
						if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+i)))
							break;
						else if(pobjLine->GetAt(nCnt+i) == ' ') //半角スペース継続
						{
							i++;
							nSpaceCnt++;
						}
						else
							break;
					}
					//タブに置き換えられる分だけスペースがあるかどうかチェック
					if(nSpaceCnt < nMargin) //ない！！
					{
						nCnt++;
						nOffset += m_lfWidth;
						continue;
					}

					//マージンの分だけ半角スペースを削除
					unsigned char c1stChar=0, c2ndChar=0;
					for(int i=0; i<nMargin; i++)
						pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
					//タブを挿入
					pobjLine->InsertChar(nCnt,VK_TAB,0);

					nOffset += (m_lfWidth * nMargin);
					nCnt++;
					//終了オフセットを更新
					lEndOffset-=nMargin;
					lEndOffset++;
				}
			}
			else if( lLineCnt < (ULONG)(m_lSelectEndLine-m_lSelectStartLine) ) //選択中間行
			{
				nOffset=0;
				long lEndOffset = pobjLine->GetLength();
				for(long nCnt=0; nCnt<lEndOffset; )
				{
					if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//２バイト文字の第一バイト
					{
						nCnt+=2;
						nOffset += (m_lfWidth * 2);
						continue;
					}
					if(pobjLine->GetAt(nCnt) == '\t')//タブコード
					{
						//マージン計算
						int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/m_lfWidth) % m_pobjEditorDoc->GetTabSize() );
						nOffset += (m_lfWidth * nMargin);
						nCnt++;
						continue;
					}
					else if(pobjLine->GetAt(nCnt) != ' ')//半角スペース以外
					{
						nCnt++;
						nOffset += m_lfWidth;
						continue;
					}

					//マージンを計算
					int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/m_lfWidth) % m_pobjEditorDoc->GetTabSize() );
					//スペース数をカウント
					int nSpaceCnt=0;
					for(int i=0; ; )
					{
						if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+i)))
							break;
						else if(pobjLine->GetAt(nCnt+i) == ' ') //半角スペース継続
						{
							i++;
							nSpaceCnt++;
						}
						else
							break;
					}
					//タブに置き換えられる分だけスペースがあるかどうかチェック
					if(nSpaceCnt < nMargin) //ない！！
					{
						nCnt++;
						nOffset += m_lfWidth;
						continue;
					}

					//マージンの分だけ半角スペースを削除
					unsigned char c1stChar=0, c2ndChar=0;
					for(int i=0; i<nMargin; i++)
						pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
					//タブを挿入
					pobjLine->InsertChar(nCnt,VK_TAB,0);

					nOffset += (m_lfWidth * nMargin);
					nCnt++;
					//終了オフセットを更新
					lEndOffset-=nMargin;
					lEndOffset++;
				}
			}
			else if( lLineCnt == (ULONG)(m_lSelectEndLine-m_lSelectStartLine) )	//選択終了行
			{
				nOffset=0;
				lEndOffsetAfter = m_lSelectEndOffset;
				for(long nCnt=0; nCnt<lEndOffsetAfter; )
				{
					if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//２バイト文字の第一バイト
					{
						nCnt+=2;
						nOffset += (m_lfWidth * 2);
						continue;
					}
					if(pobjLine->GetAt(nCnt) == '\t')//タブコード
					{
						//マージン計算
						int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/m_lfWidth) % m_pobjEditorDoc->GetTabSize() );
						nOffset += (m_lfWidth * nMargin);
						nCnt++;
						continue;
					}
					else if(pobjLine->GetAt(nCnt) != ' ')//半角スペース以外
					{
						nCnt++;
						nOffset += m_lfWidth;
						continue;
					}

					//マージンを計算
					int nMargin = m_pobjEditorDoc->GetTabSize() - ( (nOffset/m_lfWidth) % m_pobjEditorDoc->GetTabSize() );
					//スペース数をカウント
					int nSpaceCnt=0;
					for(int i=0; ; )
					{
						if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+i)))
							break;
						else if(pobjLine->GetAt(nCnt+i) == ' ') //半角スペース継続
						{
							i++;
							nSpaceCnt++;
						}
						else
							break;
					}
					//タブに置き換えられる分だけスペースがあるかどうかチェック
					if(nSpaceCnt < nMargin) //ない！！
					{
						nCnt++;
						nOffset += m_lfWidth;
						continue;
					}

					//マージンの分だけ半角スペースを削除
					unsigned char c1stChar=0, c2ndChar=0;
					for(int i=0; i<nMargin; i++)
						pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
					//タブを挿入
					pobjLine->InsertChar(nCnt,VK_TAB,0);

					nOffset += (m_lfWidth * nMargin);
					nCnt++;
					//終了オフセットを更新
					lEndOffsetAfter-=nMargin;
					lEndOffsetAfter++;
				}
			}

			//再設定(1999.03.20 Add for StatusBar Pane ID_INDICATOR_BYTE)
			m_pobjEditorDoc->SetOneLine(m_lSelectStartLine+lLineCnt,pobjLine);
		}
	}

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
		//キャレット位置を設定
		pView->MySetCaretToLineOffset(m_lSelectEndLine,lEndOffsetAfter);

		//選択範囲再復元
		pView->m_lSelectStartLine   = m_lSelectStartLine;
		pView->m_lSelectStartOffset = m_lSelectStartOffset;
		pView->m_lSelectEndLine     = m_lSelectEndLine;
		pView->m_lSelectEndOffset   = lEndOffsetAfter;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Undo 実行可能調査														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoItemSpace2Tab::IsCanUndo()
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
BOOL CUndoItemSpace2Tab::IsCanRedo()
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
void CUndoItemSpace2Tab::setCanUndo
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
void CUndoItemSpace2Tab::setCanRedo
	(	BOOL	bIsCanRedo
	)
{
	m_bIsCanRedo = bIsCanRedo;
}


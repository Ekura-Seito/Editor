// UndoItemOleDrop.cpp: CUndoItemOleDrop クラスのインプリメンテーション
//

#include "stdafx.h"
#include <Afxtempl.h>			//ＭＦＣのＣＭａｐを使用する
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include <afxadv.h>				//for CSharedFile
#include "Editor_Def.h"
#include "ColorManager.h"
#include "Editor.h"
#include "Line.h"
#include "LineManager.h"
#include "UndoManager.h"
#include "EditorDoc.h"
#include "BCMenu.h"
#include "EditorView.h"
#include "UndoItem.h"
#include "UndoItemOleDrop.h"

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
CUndoItemOleDrop::CUndoItemOleDrop()
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = NULL;

	m_lStartLine = 0;
	m_lStartOffset = 0;
	m_lEndLine = 0;
	m_lEndOffset = 0;

	m_objDropLineArray.SetSize(0);				//ドロップテキストの配列（初期サイズゼロ）

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
CUndoItemOleDrop::CUndoItemOleDrop
	(	CEditorDoc*		pobjEditorDoc
	)
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = pobjEditorDoc;

	m_lStartLine = 0;
	m_lStartOffset = 0;
	m_lEndLine = 0;
	m_lEndOffset = 0;

	m_objDropLineArray.SetSize(0);				//ドロップテキストの配列（初期サイズゼロ）

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
CUndoItemOleDrop::~CUndoItemOleDrop()
{
	long lCnt;

	//ドロップ行
	for(lCnt=0; lCnt<m_objDropLineArray.GetSize(); lCnt++)
	{
		CLine* pobjLine = (CLine*)m_objDropLineArray.GetAt(lCnt);
		delete pobjLine;
	}
	m_objDropLineArray.RemoveAll();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：実行																	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemOleDrop::Do()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：実行																	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemOleDrop::Do
	(	COleDataObject*	pobjOleDataObject,	// ＯＬＥデータオブジェクト
		long	lCurrentLine,				// キャレット位置の行番号
		long	lCurrentOffset,				// キャレット位置の列番号
		CPoint&	objCaretPos,				// キャレット位置
		CPoint& objScrollPosLog,			// スクロール位置
		BOOL	bDispRuler,					// ルーラ表示フラグ
		BOOL	bDispLineNo					// 行番号表示フラグ
	)
{
	long lCRCount=0;
	long lLFCount=0;

	/////////////////////////////
	//	実行前情報保持
	/////////////////////////////
	//パラメータ
	m_lStartLine              = lCurrentLine;
	m_lStartOffset            = lCurrentOffset;
	m_lEndLine                = lCurrentLine;		//張り付け終了位置行番号に初期値を設定
	m_lEndOffset              = lCurrentOffset;		//張り付け終了位置列番号に初期値を設定
	m_objOriginalCaretPos     = objCaretPos;
	m_objOriginalScrollPosLog = objScrollPosLog;
	m_bOriginalDispRuler      = bDispRuler;
	m_bOriginalDispLineNo     = bDispLineNo;

	//ドキュメント情報
	m_bOriginalModifiedFlag = m_pobjEditorDoc->IsModified();
	m_objOriginalDocTitle   = m_pobjEditorDoc->GetTitle();


	//////////////////////////////////
	//	ドロップデータより読み出し	//
	//////////////////////////////////

	CFile* pobjFile = pobjOleDataObject->GetFileData(CF_TEXT);

	//////////////////////////////////
	//	ドロップ位置への挿入処理	//
	//////////////////////////////////

	//ドロップ位置から行末までの文字列を退避しておく
	CLine	objEnd;	
	CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lStartLine);
	if( (pobjLine != NULL) && (m_lStartOffset < pobjLine->GetLength()) )
	{
		objEnd.Set(pobjLine->Data(m_lStartOffset));
		pobjLine->SetAt(m_lStartOffset,0);
		m_pobjEditorDoc->SetOneLine(m_lStartLine,pobjLine);
	}

	//ドロップデータ読み出し
	CArchive ar( pobjFile, CArchive::load );
	pobjLine = NULL;
	long lCnt;
	int  nNowRet=0;
	int  nBeforeRet=0;
	m_pApp->PrepareMyReadString();
	for(lCnt=0; ;lCnt++)//１行づつ読み込み
	{
		CString objStrBuf = _T("");
		nNowRet = m_pApp->MyReadString(ar,objStrBuf,&lCRCount,&lLFCount);
		if( nNowRet == 0 ) //EOF終了
		{
			if( objStrBuf.IsEmpty() )
			{
				if( nBeforeRet != 1 )
					lCnt--;
				break;
			}
		}

		//MyReadString リターン値保持
		nBeforeRet = nNowRet;

		if( lCnt==0 ) //張り付け、１回目
		{
			//貼り付け行退避
			pobjLine = new CLine((const unsigned char*)(LPCTSTR)objStrBuf);
			m_objDropLineArray.Add((CObject*)pobjLine);

			//貼り付け行登録
			pobjLine = m_pobjEditorDoc->GetOneLine(m_lStartLine+lCnt);
			if( pobjLine != NULL )
			{
				pobjLine->Catenate((const unsigned char*)(LPCTSTR)objStrBuf);
				m_pobjEditorDoc->SetOneLine(m_lStartLine+lCnt,pobjLine);
			}
			else
			{
				pobjLine = new CLine((const unsigned char*)(LPCTSTR)objStrBuf);
				m_pobjEditorDoc->InsertOneLine(m_lStartLine+lCnt,pobjLine);
			}
			//張り付け終了位置列番号更新
			m_lEndOffset = pobjLine->GetLength();
		}
		else //張り付け、２回目以降
		{
			//貼り付け行更新
			pobjLine = (CLine*)m_objDropLineArray.GetAt(m_objDropLineArray.GetSize()-1);
			pobjLine->Set((const unsigned char*)(LPCTSTR)objStrBuf);

			//貼り付け行登録
			pobjLine = m_pobjEditorDoc->GetOneLine(m_lStartLine+lCnt);
			pobjLine->Set((const unsigned char*)(LPCTSTR)objStrBuf);
			m_pobjEditorDoc->SetOneLine(m_lStartLine+lCnt,pobjLine);
			//張り付け終了位置列番号更新
			m_lEndOffset = pobjLine->GetLength();
		}

		if(nNowRet == 1) //改行終了していた場合
		{
			//次の行の分の貼り付け行退避
			pobjLine = new CLine();
			m_objDropLineArray.Add((CObject*)pobjLine);

			//次の行の分を登録
			pobjLine = new CLine();
			m_pobjEditorDoc->InsertOneLine(m_lStartLine+lCnt+1,pobjLine);
			//張り付け終了位置の行番号を更新
			m_lEndLine++;
			//張り付け終了位置列番号更新
			m_lEndOffset = 0;
		}
		else //ＥＯＦ終了していた場合
		{
			break;
		}
	}

	//ドロップ位置から行末までの文字列が退避されていた場合
	if( objEnd.GetLength() > 0 )
	{
		//最後に連結する
		pobjLine = m_pobjEditorDoc->GetOneLine(m_lStartLine+lCnt);
		if( pobjLine != NULL )
			pobjLine->Catenate(objEnd.String());
		else
			pobjLine = new CLine(objEnd.String());
		m_pobjEditorDoc->SetOneLine(m_lStartLine+lCnt,pobjLine);
	}

	//ドロップデータクローズ
	ar.Abort();
	pobjFile->Close();
	delete pobjFile;

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

	//選択範囲再設定（張り付け開始～終了位置）
	POSITION pos = m_pobjEditorDoc->GetFirstViewPosition();
	while(pos != 0)
	{
		CEditorView* pView = (CEditorView*)m_pobjEditorDoc->GetNextView(pos);

		//キャレット位置を設定
		pView->MySetCaretToLineOffset(m_lEndLine,m_lEndOffset);

		//選択範囲再設定
		pView->m_lSelectStartLine   = m_lStartLine;
		pView->m_lSelectStartOffset = m_lStartOffset;
		pView->m_lSelectEndLine     = m_lEndLine;
		pView->m_lSelectEndOffset   = m_lEndOffset;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Undo 実行																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemOleDrop::doUndo()
{
	//////////////////////////////////////////////////
	//	ドロップ開始位置～終了位置の部分を切り取る	//
	//////////////////////////////////////////////////

	if( m_lStartLine == m_lEndLine ) //１行貼り付けのＵｎｄｏ
	{
		CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lStartLine);
		if( pobjLine != NULL )
		{
			//貼り付け範囲切り取り
			pobjLine->Trim(m_lStartOffset,m_lEndOffset);
			m_pobjEditorDoc->SetOneLine(m_lStartLine,pobjLine);
		}
	}
	else //複数行貼り付けのＵｎｄｏ
	{
		CLine*	pobjStartLine = NULL;
		CLine*	pobjEndLine = NULL;
		for(ULONG lCnt=0; lCnt<(ULONG)(m_lEndLine-m_lStartLine+1); lCnt++ )
		{
			if( lCnt == 0 ) //貼り付け開始行
			{
				pobjStartLine = m_pobjEditorDoc->GetOneLine(m_lStartLine);
				//貼り付け開始オフセット以降切り取り
				if( pobjStartLine != NULL )
				{
					pobjStartLine->SetAt(m_lStartOffset,0);
					m_pobjEditorDoc->SetOneLine(m_lStartLine,pobjStartLine);
				}
			}
			else if( lCnt < (ULONG)(m_lEndLine-m_lStartLine) ) //貼り付け中間行
			{
				m_pobjEditorDoc->DeleteOneLine(m_lStartLine+1);
			}
			else if( lCnt == (ULONG)(m_lEndLine-m_lStartLine) )//貼り付け終了行
			{
				pobjEndLine = m_pobjEditorDoc->GetOneLine(m_lStartLine+1);
				//貼り付け終了オフセットよりまえ切り取り
				if( pobjEndLine != NULL )
				{
					pobjEndLine->Trim(0,m_lEndOffset);
					m_pobjEditorDoc->SetOneLine(m_lStartLine+1,pobjEndLine);
				}
			}
		}
		//貼り付け開始行と貼り付け終了行を連結
		if( pobjEndLine != NULL )
		{
			pobjStartLine->Catenate(pobjEndLine->String());
			m_pobjEditorDoc->DeleteOneLine(m_lStartLine+1);
		}
		m_pobjEditorDoc->SetOneLine(m_lStartLine,pobjStartLine);
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
void CUndoItemOleDrop::doRedo()
{
	//////////////////////////////////
	//	ドロップ位置への挿入処理	//
	//////////////////////////////////

	//張り付け開始位置から行末までの文字列を退避しておく
	CLine	objEnd;	
	CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lStartLine);
	if( (pobjLine != NULL) && (m_lStartOffset < pobjLine->GetLength()) )
	{
		objEnd.Set(pobjLine->Data(m_lStartOffset));
		pobjLine->SetAt(m_lStartOffset,0);
		m_pobjEditorDoc->SetOneLine(m_lStartLine,pobjLine);
	}

	//１行づつ貼り付け
	pobjLine = NULL;
	CLine* pobjDropLine = NULL;
	ULONG lCnt=0;
	for(lCnt=0; lCnt<(ULONG)m_objDropLineArray.GetSize(); lCnt++)
	{
		pobjDropLine = (CLine*)m_objDropLineArray.GetAt(lCnt);

		if( lCnt==0 ) //張り付け、１回目
		{
			pobjLine = m_pobjEditorDoc->GetOneLine(m_lStartLine);
			if( pobjLine != NULL )
			{
				pobjLine->Catenate(pobjDropLine->String());
				m_pobjEditorDoc->SetOneLine(m_lStartLine,pobjLine);
			}
			else
			{
				pobjLine = new CLine(pobjDropLine->String());
				m_pobjEditorDoc->InsertOneLine(m_lStartLine,pobjLine);
			}
		}
		else //張り付け、２回目以降
		{
			pobjLine = new CLine(pobjDropLine->String());
			m_pobjEditorDoc->InsertOneLine(m_lStartLine+lCnt,pobjLine);
		}
	}
	lCnt--;

	//張り付け開始位置から行末までの文字列が退避されていた場合
	if( objEnd.GetLength() > 0 )
	{
		//最後に連結する
		pobjLine = m_pobjEditorDoc->GetOneLine(m_lStartLine+lCnt);
		if( pobjLine != NULL )
			pobjLine->Catenate(objEnd.String());
		else
			pobjLine = new CLine(objEnd.String());
		m_pobjEditorDoc->SetOneLine(m_lStartLine+lCnt,pobjLine);
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
		pView->MySetCaretPos(m_objOriginalCaretPos);

		//選択範囲再設定
		pView->m_lSelectStartLine   = m_lStartLine;
		pView->m_lSelectStartOffset = m_lStartOffset;
		pView->m_lSelectEndLine     = m_lEndLine;
		pView->m_lSelectEndOffset   = m_lEndOffset;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Undo 実行可能調査														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoItemOleDrop::IsCanUndo()
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
BOOL CUndoItemOleDrop::IsCanRedo()
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
void CUndoItemOleDrop::setCanUndo
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
void CUndoItemOleDrop::setCanRedo
	(	BOOL	bIsCanRedo
	)
{
	m_bIsCanRedo = bIsCanRedo;
}



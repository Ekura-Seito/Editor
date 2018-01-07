// UndoManager.cpp : CUndoManager クラスの動作の定義を行います。
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "UndoItem.h"
#include "UndoManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CUndoManager クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUndoManager::CUndoManager()
{
	clearAllItem();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUndoManager::~CUndoManager()
{
	clearAllItem();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：初期化																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoManager::Initialize()
{
	clearAllItem();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：CUndoItem リストのクリア												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoManager::clearAllItem()
{
	POSITION pos = m_objUndoItemList.GetHeadPosition();
	POSITION oldPos = pos;
	CUndoItem*	pobjUndoItem = NULL;
	for(;;)
	{
		if( pos == 0 )
			break;
		pobjUndoItem = (CUndoItem*)m_objUndoItemList.GetNext(pos);
		if( pobjUndoItem == NULL )
			break;
		m_objUndoItemList.RemoveAt(oldPos);
		delete pobjUndoItem;
		oldPos = pos;
	}
	m_lCurrentPosition = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：CUndoItem の登録														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoManager::addUndoItem
	(	CUndoItem*	pobjUndoItem
	)
{
	if( m_lCurrentPosition != m_objUndoItemList.GetTailPosition() )
	{
		//CUndoItem 登録時に、現在位置がリストの最後尾でないということは、
		//「元に戻す」や「繰り返し」操作を行っている最中に、
		//新たな取り消し可能操作を行ったと考えられる。
		//この場合、現在位置からリスト最後尾までの操作オブジェクトは無効になる。
		POSITION tailPos;
		for(;;)
		{
			tailPos = m_objUndoItemList.GetTailPosition();
			if( m_lCurrentPosition == tailPos )
				break;

			CUndoItem* pobjUndoItem = (CUndoItem*)m_objUndoItemList.GetAt(tailPos);
			if( pobjUndoItem != NULL )
				delete pobjUndoItem;
			m_objUndoItemList.RemoveAt(tailPos);
		}
	}

	m_objUndoItemList.AddTail(pobjUndoItem);
	m_lCurrentPosition = m_objUndoItemList.GetTailPosition();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Undo 可能調査															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoManager::IsCanUndo()
{
	if(m_lCurrentPosition)
	{
		CUndoItem*	pobjUndoItem = (CUndoItem*)m_objUndoItemList.GetAt(m_lCurrentPosition);
		return( pobjUndoItem->IsCanUndo() );
	}
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Redo 可能調査															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoManager::IsCanRedo()
{
	if( (m_lCurrentPosition==0) && (m_objUndoItemList.GetCount()>0) )
	{
		POSITION pos = m_objUndoItemList.GetHeadPosition();
		CUndoItem* pobjUndoItem=(CUndoItem*)m_objUndoItemList.GetAt(pos);
		return( pobjUndoItem->IsCanRedo() );
	}
	else if( (m_lCurrentPosition!=0) && (m_lCurrentPosition!=m_objUndoItemList.GetTailPosition()) )
	{
		POSITION pos = m_lCurrentPosition;
		m_objUndoItemList.GetNext(pos);
		CUndoItem* pobjUndoItem=(CUndoItem*)m_objUndoItemList.GetAt(pos);
		return( pobjUndoItem->IsCanRedo() );
	}
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Undo 実行																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoManager::doUndo()
{
	if(IsCanUndo()==FALSE)
		return FALSE;
	
	CUndoItem*	pobjUndoItem=(CUndoItem*)m_objUndoItemList.GetAt(m_lCurrentPosition);
	pobjUndoItem->doUndo();
	m_objUndoItemList.GetPrev(m_lCurrentPosition);
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Redo 実行																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoManager::doRedo()
{
	if(IsCanRedo()==FALSE)
		return FALSE;

	if(m_lCurrentPosition==0)
	{
		POSITION pos = m_objUndoItemList.GetHeadPosition();
		CUndoItem* pobjUndoItem=(CUndoItem*)m_objUndoItemList.GetAt(pos);
		pobjUndoItem->doRedo();
		m_lCurrentPosition = pos;
	}
	else
	{
		POSITION pos = m_lCurrentPosition;
		m_objUndoItemList.GetNext(pos);
		CUndoItem* pobjUndoItem=(CUndoItem*)m_objUndoItemList.GetAt(pos);
		pobjUndoItem->doRedo();
		m_lCurrentPosition = pos;
	}
	return TRUE;
}


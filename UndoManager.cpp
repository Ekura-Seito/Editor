// UndoManager.cpp : CUndoManager �N���X�̓���̒�`���s���܂��B
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
// CUndoManager �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUndoManager::CUndoManager()
{
	clearAllItem();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUndoManager::~CUndoManager()
{
	clearAllItem();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F������																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoManager::Initialize()
{
	clearAllItem();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FCUndoItem ���X�g�̃N���A												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
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
/*|	�T�v	�FCUndoItem �̓o�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoManager::addUndoItem
	(	CUndoItem*	pobjUndoItem
	)
{
	if( m_lCurrentPosition != m_objUndoItemList.GetTailPosition() )
	{
		//CUndoItem �o�^���ɁA���݈ʒu�����X�g�̍Ō���łȂ��Ƃ������Ƃ́A
		//�u���ɖ߂��v��u�J��Ԃ��v������s���Ă���Œ��ɁA
		//�V���Ȏ������\������s�����ƍl������B
		//���̏ꍇ�A���݈ʒu���烊�X�g�Ō���܂ł̑���I�u�W�F�N�g�͖����ɂȂ�B
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
/*|	�T�v	�FUndo �\����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
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
/*|	�T�v	�FRedo �\����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
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
/*|	�T�v	�FUndo ���s																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
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
/*|	�T�v	�FRedo ���s																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
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


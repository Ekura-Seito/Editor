// UndoItemInsTime.cpp: CUndoItemInsTime �N���X�̃C���v�������e�[�V����
//

#include "stdafx.h"
#include <Afxtempl.h>			//�l�e�b�̂b�l�������g�p����
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
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
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
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
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
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUndoItemInsTime::~CUndoItemInsTime()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s																	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemInsTime::Do()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s																	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemInsTime::Do 
	(	long		lCurrentLine,			// �L�����b�g�ʒu�s
		long		lCurrentOffset,			// �L�����b�g�ʒu�I�t�Z�b�g
		CString&	objTimeStr,				// ����������
		CPoint&		objCaretPos,			// �L�����b�g�ʒu
		CPoint& 	objScrollPosLog,		// �X�N���[���ʒu
		BOOL		bDispRuler,				// ���[���\���t���O
		BOOL		bDispLineNo				// �s�ԍ��\���t���O
	)
{
	/////////////////////////////
	//	���s�O���ێ�
	/////////////////////////////
	//�p�����[�^
	m_lCaretLine              = lCurrentLine;
	m_lCaretOffset            = lCurrentOffset;
	m_objTimeStr              = objTimeStr;
	m_objOriginalCaretPos     = objCaretPos;
	m_objOriginalScrollPosLog = objScrollPosLog;
	m_bOriginalDispRuler      = bDispRuler;
	m_bOriginalDispLineNo     = bDispLineNo;

	//�h�L�������g���
	m_bOriginalModifiedFlag = m_pobjEditorDoc->IsModified();
	m_objOriginalDocTitle   = m_pobjEditorDoc->GetTitle();

	///////////////////////////////
	//	�������s
	///////////////////////////////

	//����������}��
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

	//����������}��������̍s�A����ێ�
	m_lSelectStartLine   = lCurrentLine;
	m_lSelectStartOffset = lCurrentOffset;
	m_lSelectEndLine     = lCurrentLine;
	m_lSelectEndOffset   = lCurrentOffset + objTimeStr.GetLength();

	//////////////
	//	�㏈��	//
	//////////////

	//�h�L�������g�_�[�e�B
	m_pobjEditorDoc->SetModifiedFlag(TRUE);

	//�^�C�g���X�V
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

		//�L�����b�g�ʒu�ݒ�(1999.03.20 Add for StatusBar Pane ID_INDICATOR_BYTE)
//		pView->MySetCaretPos(objCaretPos);
		pView->MySetCaretToLineOffset(m_lSelectEndLine,m_lSelectEndOffset);

		//�I��͈͍Đݒ�
		pView->m_lSelectStartLine   = m_lSelectStartLine;
		pView->m_lSelectStartOffset = m_lSelectStartOffset;
		pView->m_lSelectEndLine     = m_lSelectEndLine;
		pView->m_lSelectEndOffset   = m_lSelectEndOffset;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FUndo ���s																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemInsTime::doUndo()
{
	///////////////////////////////
	//	�������s
	///////////////////////////////

	CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lCaretLine);
	if( pobjLine != NULL )
	{
		pobjLine->Trim(m_lSelectStartOffset,m_lSelectEndOffset);
		m_pobjEditorDoc->SetOneLine(m_lCaretLine,pobjLine);
	}

	//////////////
	//	�㏈��	//
	//////////////

	//�h�L�������g��񕜌�
	m_pobjEditorDoc->SetModifiedFlag(m_bOriginalModifiedFlag);
	m_pobjEditorDoc->SetTitle(m_objOriginalDocTitle);

	POSITION pos = m_pobjEditorDoc->GetFirstViewPosition();
	while(pos != 0)
	{
		CEditorView* pView = (CEditorView*)m_pobjEditorDoc->GetNextView(pos);

		//���[���A�s�ԍ��\���̕���
		pView->SetFlagDispRuler(m_bOriginalDispRuler);
		pView->SetFlagDispLineNo(m_bOriginalDispLineNo);

		//�X�N���[���ʒu�A�L�����b�g�ʒu�̕���
		pView->MyScrollToPosition(m_objOriginalScrollPosLog);
		pView->MySetCaretPos(m_objOriginalCaretPos);

		//�I��͈͉���
		pView->m_lSelectStartLine   = -1;
		pView->m_lSelectStartOffset = -1;
		pView->m_lSelectEndLine     = -1;
		pView->m_lSelectEndOffset   = -1;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FRedo ���s																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemInsTime::doRedo()
{
	///////////////////////////////
	//	�������s
	///////////////////////////////

	//����������}��
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
	//	�㏈��	//
	//////////////

	//�h�L�������g�_�[�e�B
	m_pobjEditorDoc->SetModifiedFlag(TRUE);

	//�^�C�g���X�V
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

		//���[���A�s�ԍ��\���̕���
		pView->SetFlagDispRuler(m_bOriginalDispRuler);
		pView->SetFlagDispLineNo(m_bOriginalDispLineNo);

		//�X�N���[���ʒu�A�L�����b�g�ʒu�̕���
		pView->MyScrollToPosition(m_objOriginalScrollPosLog);
//		pView->MySetCaretPos(m_objOriginalCaretPos);
		pView->MySetCaretToLineOffset(m_lSelectEndLine,m_lSelectEndOffset);

		//�I��͈͍Đݒ�
		pView->m_lSelectStartLine   = m_lSelectStartLine;
		pView->m_lSelectStartOffset = m_lSelectStartOffset;
		pView->m_lSelectEndLine     = m_lSelectEndLine;
		pView->m_lSelectEndOffset   = m_lSelectEndOffset;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FUndo ���s�\����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoItemInsTime::IsCanUndo()
{
	return m_bIsCanUndo;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FRedo ���s�\����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoItemInsTime::IsCanRedo()
{
	return m_bIsCanRedo;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FUndo �\�ݒ�															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemInsTime::setCanUndo
	(	BOOL	bIsCanUndo
	)
{
	m_bIsCanUndo = bIsCanUndo;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FRedo �\�ݒ�															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemInsTime::setCanRedo
	(	BOOL	bIsCanRedo
	)
{
	m_bIsCanRedo = bIsCanRedo;
}


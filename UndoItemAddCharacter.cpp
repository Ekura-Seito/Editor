// UndoItemAddCharacter.cpp: CUndoItemAddCharacter �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

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
#include "UndoItemAddCharacter.h"

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
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
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
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUndoItemAddCharacter::~CUndoItemAddCharacter()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s																	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemAddCharacter::Do()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s																	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemAddCharacter::Do
	(	long	lCurrentLine,			// �����o�^�Ώۂ̍s�ԍ�
		long	lCurrentOffset,			// �����o�^�Ώۂ̗�ԍ��i�޲ĒP�ʁj
		UINT	n1stChar,				// �o�^��������޲�
		UINT	n2ndChar,				// �o�^��������޲�
		BOOL	bInsertKey,				// �}���^�㏑��Ӱ���׸�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint& objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	/////////////////////////////
	//	���s�O���ێ�
	/////////////////////////////
	//�p�����[�^
	m_lCurrentLine            = lCurrentLine;
	m_lCurrentOffset          = lCurrentOffset;
	m_n1stChar                = n1stChar;
	m_n2ndChar                = n2ndChar;
	m_bInsertKey              = bInsertKey;
	m_objOriginalCaretPos     = objCaretPos;
	m_objOriginalScrollPosLog = objScrollPosLog;
	m_bOriginalDispRuler      = bDispRuler;
	m_bOriginalDispLineNo     = bDispLineNo;

	//�o�^�ʒu�̃I���W�i���������
	CLine* pobjLine = m_pobjEditorDoc->GetOneLine(lCurrentLine);
	if(pobjLine != NULL)
	{
		m_nOriginal1stChar = pobjLine->GetAt(lCurrentOffset);
		m_nOriginal2ndChar = pobjLine->GetAt(lCurrentOffset+1);
		m_nOriginal3rdChar = pobjLine->GetAt(lCurrentOffset+2);
	}

	//�h�L�������g���
	m_bOriginalModifiedFlag = m_pobjEditorDoc->IsModified();
	m_objOriginalDocTitle   = m_pobjEditorDoc->GetTitle();

	///////////////////////////////
	//	�������s
	///////////////////////////////

	//�����o�^
	m_pobjEditorDoc->m_objLineManager.AddCharacter(lCurrentLine,lCurrentOffset,n1stChar,n2ndChar,bInsertKey);

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
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FUndo ���s																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemAddCharacter::doUndo()
{
	unsigned char	c1stChar = 0;
	unsigned char	c2ndChar = 0;

	if(m_bInsertKey) //�}������̎�����
	{
		//�}�������ʒu�ɂ��镶����DELETE����
		m_pobjEditorDoc->m_objLineManager.ProcessDeleteCode(m_lCurrentLine,m_lCurrentOffset,&c1stChar,&c2ndChar);
	}
	else //�㏑������̎�����
	{
		//�I���W�i���������㏑������
		if(IsDBCSLeadByteEx(0,m_n1stChar))	//�Q�o�C�g�������͑��������
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
		else //�P�o�C�g�������͑��������
		{
			if(IsDBCSLeadByteEx(0,m_nOriginal1stChar))
				m_pobjEditorDoc->m_objLineManager.AddCharacter(m_lCurrentLine,m_lCurrentOffset,m_nOriginal1stChar,m_nOriginal2ndChar,m_bInsertKey);
			else
				m_pobjEditorDoc->m_objLineManager.AddCharacter(m_lCurrentLine,m_lCurrentOffset,m_nOriginal1stChar,0,m_bInsertKey);
		}
	}

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

		//�L�����b�g�ʒu���X�N���[���ʒu�̕���
		pView->MyScrollToPosition(m_objOriginalScrollPosLog);
		pView->MySetCaretPos(m_objOriginalCaretPos);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FRedo ���s																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemAddCharacter::doRedo()
{
	//�����o�^
	m_pobjEditorDoc->m_objLineManager.AddCharacter(
		m_lCurrentLine,
		m_lCurrentOffset,
		m_n1stChar,
		m_n2ndChar,
		m_bInsertKey);

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

		//�L�����b�g�ʒu���X�N���[���ʒu�X�V
		pView->MyScrollToPosition(m_objOriginalScrollPosLog);
		pView->MySetCaretPos(m_objOriginalCaretPos);

		pView->MoveCaret(VK_RIGHT,1);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FUndo ���s�\����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoItemAddCharacter::IsCanUndo()
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
BOOL CUndoItemAddCharacter::IsCanRedo()
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
void CUndoItemAddCharacter::setCanUndo
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
void CUndoItemAddCharacter::setCanRedo
	(	BOOL	bIsCanRedo
	)
{
	m_bIsCanRedo = bIsCanRedo;
}

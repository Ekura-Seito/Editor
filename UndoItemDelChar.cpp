// UndoItemDelChar.cpp: CUndoItemDelChar �N���X�̃C���v�������e�[�V����
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
#include "UndoItemDelChar.h"

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
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
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
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUndoItemDelChar::~CUndoItemDelChar()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s																	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemDelChar::Do()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s																	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemDelChar::Do
	(	long	lCurrentLine,			// �����o�^�Ώۂ̍s�ԍ�
		long	lCurrentOffset,			// �����o�^�Ώۂ̗�ԍ��i�޲ĒP�ʁj
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

	unsigned char c1stChar=0;				//DELETE���ꂽ�����R�[�h�o�b�t�@
	unsigned char c2ndChar=0;

	//DELETE�R�[�h����
	m_pobjEditorDoc->m_objLineManager.ProcessDeleteCode(lCurrentLine,lCurrentOffset,&c1stChar,&c2ndChar);

	//DELETE���ꂽ������ێ�
	m_cOriginal1stChar = c1stChar;
	m_cOriginal2ndChar = c2ndChar;

	if( (m_cOriginal1stChar != 0) ||	//��������DELETE���ꂽ
		(m_cOriginal2ndChar != 0)
	)
	{
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

			//�L�����b�g�ʒu�ݒ�(1999.03.20 Add for StatusBar ID_INDICATOR_BYTE)
			pView->MySetCaretPos(objCaretPos);

			if( (m_cOriginal1stChar == '\r') &&	//�s���ł�DELETE
				(m_cOriginal2ndChar == '\n')
			)
			{
				//�ĕ`��
				pView->OnUpdate(NULL,NULL,NULL);
				pView->Invalidate(FALSE);
			}
			else
			{
				//�L�����b�g�s�̂ݍĕ`��
				pView->OnUpdate(NULL,NULL,NULL);
				pView->InvalidateCaretLine();
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FUndo ���s																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemDelChar::doUndo()
{
	if( (m_cOriginal1stChar == 0) &&		//����DELETE����Ă��Ȃ�����
		(m_cOriginal2ndChar == 0)
	)
	{
		// �������Ȃ�
		return;
	}


	if( (m_cOriginal1stChar == '\r') &&	//�s���ł�DELETE��UNDO
		(m_cOriginal2ndChar == '\n')
	)
	{
		//���s�R�[�h�o�^
		m_pobjEditorDoc->m_objLineManager.ProcessReturnCode(m_lCurrentLine,m_lCurrentOffset);
	}
	else //����DELETE��UNDO
	{
		//DELETE�����ēo�^
		m_pobjEditorDoc->m_objLineManager.AddCharacter(
							m_lCurrentLine,
							m_lCurrentOffset,
							m_cOriginal1stChar,
							m_cOriginal2ndChar,
							TRUE);
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
void CUndoItemDelChar::doRedo()
{
	unsigned char c1stChar=0;				//DELETE���ꂽ�����R�[�h�o�b�t�@
	unsigned char c2ndChar=0;

	//DELETE�R�[�h����
	m_pobjEditorDoc->m_objLineManager.ProcessDeleteCode(m_lCurrentLine,m_lCurrentOffset,&c1stChar,&c2ndChar);

	if( (m_cOriginal1stChar != 0) ||	//��������DELETE���ꂽ
		(m_cOriginal2ndChar != 0)
	)
	{
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

			//�L�����b�g�ʒu���X�N���[���ʒu�̕���
			pView->MyScrollToPosition(m_objOriginalScrollPosLog);
			pView->MySetCaretPos(m_objOriginalCaretPos);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FUndo ���s�\����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoItemDelChar::IsCanUndo()
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
BOOL CUndoItemDelChar::IsCanRedo()
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
void CUndoItemDelChar::setCanUndo
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
void CUndoItemDelChar::setCanRedo
	(	BOOL	bIsCanRedo
	)
{
	m_bIsCanRedo = bIsCanRedo;
}


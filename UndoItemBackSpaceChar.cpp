// UndoItemBackSpaceChar.cpp: CUndoItemBackSpaceChar �N���X�̃C���v�������e�[�V����
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
#include "UndoItemBackSpaceChar.h"

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
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
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
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUndoItemBackSpaceChar::~CUndoItemBackSpaceChar()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s																	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemBackSpaceChar::Do()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s																	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemBackSpaceChar::Do
	(	long	lCurrentLine,			// BACK SPACE �Ώۂ̍s�ԍ�
		long	lCurrentOffset,			// BACK SPACE �Ώۂ̗�ԍ��i�޲ĒP�ʁj
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint& objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	//�N���C�A���g�̈�
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

	if( (lCurrentLine > 0) && (lCurrentOffset == 0) ) //�s�̐擪�ł�BACK SPACE
	{
		//BACK SPACE ����O�ɑO�s�̍s���ʒu���L��
		POSITION pos = m_pobjEditorDoc->GetFirstViewPosition();
		while(pos != 0)
		{
			CEditorView* pView = (CEditorView*)m_pobjEditorDoc->GetNextView(pos);
			CPoint	objScrollPosLog(pView->GetScrollPosLogX(),pView->GetScrollPosLogY());

			//�L�����b�g�ʒu�@�f�o�C�X�|���_�����W
			CPoint logicalPoint = pView->m_caretPos;
			logicalPoint -= pView->m_LineNoPos;
			logicalPoint -= pView->m_RulerPos;
			logicalPoint += objScrollPosLog;
			m_objPreLineEndPos.y = logicalPoint.y - (pView->m_lfHeight+pView->m_lLineInterval);
			pView->GetPosition(lCurrentLine-1,&m_objPreLineEndPos);
			//m_objPreLineEndPos �͘_�����W
		}
	}
	else
	{
		//BACK SPACE ����O�ɑO�̈ʒu���L��
		POSITION pos = m_pobjEditorDoc->GetFirstViewPosition();
		while(pos != 0)
		{
			CEditorView* pView = (CEditorView*)m_pobjEditorDoc->GetNextView(pos);
			CPoint	objScrollPosLog(pView->GetScrollPosLogX(),pView->GetScrollPosLogY());

			//�L�����b�g�ʒu�@�f�o�C�X�|���_�����W
			CPoint logicalPoint = pView->m_caretPos;
			logicalPoint -= pView->m_LineNoPos;
			logicalPoint -= pView->m_RulerPos;
			logicalPoint += objScrollPosLog;
			m_objAfterCaretPos.y = logicalPoint.y;
			pView->GetPosition1(lCurrentLine,&logicalPoint,&m_objAfterCaretPos);
			//m_objAfterCaretPos �͘_�����W
		}
	}

	///////////////////////////////
	//	�������s
	///////////////////////////////

	unsigned char c1stChar=0;				//BACK SPACE���ꂽ�����R�[�h�o�b�t�@
	unsigned char c2ndChar=0;

	//BACK SPACE �R�[�h����
	m_pobjEditorDoc->m_objLineManager.ProcessBackSpaceCode(lCurrentLine,lCurrentOffset,&c1stChar,&c2ndChar);

	//BACK SPACE ���ꂽ������ێ�
	m_cOriginal1stChar = c1stChar;
	m_cOriginal2ndChar = c2ndChar;

	if( (m_cOriginal1stChar != 0) ||	//�������� BACK SPACE ���ꂽ
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
			//////////////////////////////////////////////
			//	BACK SPACE ��̃L�����b�g�ʒu�����߂�
			//////////////////////////////////////////////
			CEditorView* pView = (CEditorView*)m_pobjEditorDoc->GetNextView(pos);
			pView->OnUpdate(NULL,NULL,NULL);

			if(c1stChar == '\r' && c2ndChar == '\n') //���s��BACK SPACE������
			{
				if(lCurrentLine > 0)//�܂���֍s����
				{
					//MyScrollToPotision���P��̃R�[���ɂ��邽�߂̏���
					CPoint	destPos(pView->GetScrollPosLogX(),pView->GetScrollPosLogY());
					//�L�����b�g�𕶎���̍Ō����
					CPoint	tempLogPos = m_objPreLineEndPos;
					if(tempLogPos.x > nWidth)//�\���͈͂̉E���֍s�����ꍇ
					{
						//�E�����X�N���[��
						long lLogX = tempLogPos.x - nWidth + 1;
						destPos.x = lLogX;
					}
					//�_�����W�|���f�o�C�X���W
					tempLogPos -= destPos/*pView->m_ScrollPosLog*/;
					tempLogPos += pView->m_RulerPos;
					tempLogPos += pView->m_LineNoPos;
					if(tempLogPos.y < pView->m_RulerPos.y )//�\���͈͂̏㑤�ɍs�����ꍇ
					{
						tempLogPos.y = pView->m_RulerPos.y;
						//�P�s��������X�N���[��
						destPos.y -= (pView->m_lfHeight+pView->m_lLineInterval);
					}
					if( destPos.x != pView->GetScrollPosLogX() ||
						destPos.y != pView->GetScrollPosLogY() )
					{
						pView->MyScrollToPosition(destPos);
					}
					//�L�����b�g�ړ�
					pView->MySetCaretPos(tempLogPos);
					//�ĕ`��
					pView->OnUpdate(NULL,NULL,NULL);
					pView->Invalidate(FALSE);
				}
			}
			else //���s�ł͂Ȃ�����
			{
				CPoint objCaretPos = m_objAfterCaretPos;
				//�_�����W�|���f�o�C�X���W
				objCaretPos.x -= pView->GetScrollPosLogX();
				objCaretPos.y -= pView->GetScrollPosLogY();
				objCaretPos += pView->m_RulerPos;
				objCaretPos += pView->m_LineNoPos;
				if(objCaretPos.x < pView->m_LineNoPos.x)//�\���͈͂̍����֍s�����ꍇ
				{
					//�������X�N���[��
					long lLogX = objCaretPos.x - pView->m_LineNoPos.x - pView->m_RulerPos.x + pView->GetScrollPosLogX();
					pView->MyScrollToPosition(CPoint(lLogX, pView->GetScrollPosLogY()));
					objCaretPos.x = pView->m_LineNoPos.x;
					//�L�����b�g�ړ�
					pView->MySetCaretPos(objCaretPos);
					//�ĕ`��
					pView->OnUpdate(NULL,NULL,NULL);
					pView->Invalidate(FALSE);
				}
				else
				{
					//�L�����b�g�ړ�
					pView->MySetCaretPos(objCaretPos);
					//�L�����b�g�s�̂ݍĕ`��
					pView->OnUpdate(NULL,NULL,NULL);
					pView->InvalidateCaretLine();
				}
			}

			////////////////////////////////////////////////
			//	BACK SPACE ��̍s�ԍ��Ɨ�ԍ������߂�
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
/*|	�T�v	�FUndo ���s																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemBackSpaceChar::doUndo()
{
	if( (m_cOriginal1stChar == 0) &&		//���� BACK SPACE ����Ă��Ȃ�����
		(m_cOriginal2ndChar == 0)
	)
	{
		// �������Ȃ�
		return;
	}


	if( (m_cOriginal1stChar == '\r') &&	//�s���ł̉��s�R�[�h BACK SPACE �� UNDO
		(m_cOriginal2ndChar == '\n')
	)
	{
		//���s�R�[�h�o�^
		m_pobjEditorDoc->m_objLineManager.ProcessReturnCode(m_lAfterLine,m_lAfterOffset);
	}
	else //���� BACK SPACE ��UNDO
	{
		//BACK SPACE�����ēo�^
		m_pobjEditorDoc->m_objLineManager.AddCharacter(
							m_lAfterLine,
							m_lAfterOffset,
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
void CUndoItemBackSpaceChar::doRedo()
{
	//�N���C�A���g�̈�
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
	
	unsigned char c1stChar=0;				//BACK SPACE���ꂽ�����R�[�h�o�b�t�@
	unsigned char c2ndChar=0;

	//BACK SPACE �R�[�h����
	m_pobjEditorDoc->m_objLineManager.ProcessBackSpaceCode(m_lCurrentLine,m_lCurrentOffset,&c1stChar,&c2ndChar);

	if( (c1stChar != 0) ||	//�������� BACK SPACE ���ꂽ
		(c2ndChar != 0)
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
			pView->OnUpdate(NULL,NULL,NULL);

			//���[���A�s�ԍ��\���̕���
			pView->SetFlagDispRuler(m_bOriginalDispRuler);
			pView->SetFlagDispLineNo(m_bOriginalDispLineNo);

			//�L�����b�g�ʒu���X�N���[���ʒu�̕���
			pView->MyScrollToPosition(m_objOriginalScrollPosLog);
			pView->MySetCaretPos(m_objOriginalCaretPos);

			if(c1stChar == '\r' && c2ndChar == '\n') //���s��BACK SPACE������
			{
				if(m_lCurrentLine > 0)//�܂���֍s����
				{
					//MyScrollToPotision���P��̃R�[���ɂ��邽�߂̏���
					CPoint	destPos(pView->GetScrollPosLogX(),pView->GetScrollPosLogY());
					//�L�����b�g�𕶎���̍Ō����
					CPoint	tempLogPos = m_objPreLineEndPos;
					if(tempLogPos.x > nWidth)//�\���͈͂̉E���֍s�����ꍇ
					{
						//�E�����X�N���[��
						long lLogX = tempLogPos.x - nWidth + 1;
						destPos.x = lLogX;
					}
					//�_�����W�|���f�o�C�X���W
					tempLogPos -= destPos/*pView->m_ScrollPosLog*/;
					tempLogPos += pView->m_RulerPos;
					tempLogPos += pView->m_LineNoPos;
					if(tempLogPos.y < pView->m_RulerPos.y )//�\���͈͂̏㑤�ɍs�����ꍇ
					{
						tempLogPos.y = pView->m_RulerPos.y;
						//�P�s��������X�N���[��
						destPos.y -= (pView->m_lfHeight+pView->m_lLineInterval);
					}
					if( destPos.x != pView->GetScrollPosLogX() ||
						destPos.y != pView->GetScrollPosLogY() )
					{
						pView->MyScrollToPosition(destPos);
					}
					//�L�����b�g�ړ�
					pView->MySetCaretPos(tempLogPos);
				}
			}
			else //���s�ł͂Ȃ�����
			{
				CPoint objCaretPos = m_objAfterCaretPos;
				//�_�����W�|���f�o�C�X���W
				objCaretPos.x -= pView->GetScrollPosLogX();
				objCaretPos.y -= pView->GetScrollPosLogY();
				objCaretPos += pView->m_RulerPos;
				objCaretPos += pView->m_LineNoPos;
				if(objCaretPos.x < pView->m_LineNoPos.x)//�\���͈͂̍����֍s�����ꍇ
				{
					//�������X�N���[��
					long lLogX = objCaretPos.x - pView->m_LineNoPos.x - pView->m_RulerPos.x + pView->GetScrollPosLogX();
					pView->MyScrollToPosition(CPoint(lLogX, pView->GetScrollPosLogY()));
					objCaretPos.x = pView->m_LineNoPos.x;
				}
				//�L�����b�g�ړ�
				pView->MySetCaretPos(objCaretPos);
			}
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
BOOL CUndoItemBackSpaceChar::IsCanUndo()
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
BOOL CUndoItemBackSpaceChar::IsCanRedo()
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
void CUndoItemBackSpaceChar::setCanUndo
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
void CUndoItemBackSpaceChar::setCanRedo
	(	BOOL	bIsCanRedo
	)
{
	m_bIsCanRedo = bIsCanRedo;
}


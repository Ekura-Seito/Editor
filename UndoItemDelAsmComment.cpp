// UndoItemDelAsmComment.cpp: CUndoItemDelAsmComment �N���X�̃C���v�������e�[�V����
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
#include "UndoItemDelAsmComment.h"

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
CUndoItemDelAsmComment::CUndoItemDelAsmComment()
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = NULL;

	m_lStartLine = 0;
	m_lStartOffset = 0;
	m_lEndLine = 0;
	m_lEndOffset = 0;

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
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUndoItemDelAsmComment::CUndoItemDelAsmComment
	(	CEditorDoc*		pobjEditorDoc
	)
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = pobjEditorDoc;

	m_lStartLine = 0;
	m_lStartOffset = 0;
	m_lEndLine = 0;
	m_lEndOffset = 0;

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
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUndoItemDelAsmComment::~CUndoItemDelAsmComment()
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
/*|	�T�v	�F���s																	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemDelAsmComment::Do()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s																	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemDelAsmComment::Do 
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint& objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	BOOL	bIsEndTrim=FALSE;
	unsigned char	c1stChar=0;				//DELETE���ꂽ���o�C�g����
	unsigned char	c2ndChar=0;				//DELETE���ꂽ���o�C�g����

	/////////////////////////////
	//	���s�O���ێ�
	/////////////////////////////
	//�p�����[�^
	m_lSelectStartLine        = lSelectStartLine;
	m_lSelectStartOffset      = lSelectStartOffset;
	m_lSelectEndLine          = lSelectEndLine;
	m_lSelectEndOffset        = lSelectEndOffset;
	m_objOriginalCaretPos     = objCaretPos;
	m_objOriginalScrollPosLog = objScrollPosLog;
	m_bOriginalDispRuler      = bDispRuler;
	m_bOriginalDispLineNo     = bDispLineNo;

	//�h�L�������g���
	m_bOriginalModifiedFlag = m_pobjEditorDoc->IsModified();
	m_objOriginalDocTitle   = m_pobjEditorDoc->GetTitle();

	//�I��͈͂̃e�L�X�g�擾
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
	//	�������s
	///////////////////////////////

	if( lSelectStartLine == lSelectEndLine ) //�P�s�I��
	{
		//�����폜
		CLine* pobjLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine);
		if( pobjLine != NULL )
		{
			c1stChar = pobjLine->GetAt(lSelectStartOffset);
			if( !IsDBCSLeadByteEx(0,c1stChar) && c1stChar == CODE_SEMICOLON )
			{
				//�Z�~�R�����̂`�r�l�R�����g�폜
				pobjLine->DeleteChar(lSelectStartOffset,&c1stChar,&c2ndChar);
				m_pobjEditorDoc->SetOneLine(lSelectStartLine,pobjLine);

				bIsEndTrim = TRUE;
			}
		}
	}
	else //�����s�I��
	{
		for(ULONG lCnt=0; lCnt<(ULONG)(lSelectEndLine-lSelectStartLine+1); lCnt++ )
		{
			CLine* pobjLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine+lCnt);
			if( pobjLine != NULL )
			{
				if( lCnt == 0 ) //�I���J�n�s
				{
					c1stChar = pobjLine->GetAt(lSelectStartOffset);
					if( !IsDBCSLeadByteEx(0,c1stChar) && c1stChar == CODE_SEMICOLON )
					{
						pobjLine->DeleteChar(lSelectStartOffset,&c1stChar,&c2ndChar);
					}
				}
				else if( lCnt < (ULONG)(lSelectEndLine-lSelectStartLine) ) //�I�𒆊ԍs
				{
					c1stChar = pobjLine->GetAt(0);
					if( !IsDBCSLeadByteEx(0,c1stChar) && c1stChar == CODE_SEMICOLON )
					{
						pobjLine->DeleteChar(0,&c1stChar,&c2ndChar);
					}
				}
				else if( lCnt == (ULONG)(lSelectEndLine-lSelectStartLine) )	//�I���I���s
				{
					if( lSelectEndOffset != 0 )
					{
						c1stChar = pobjLine->GetAt(0);
						if( !IsDBCSLeadByteEx(0,c1stChar) && c1stChar == CODE_SEMICOLON )
						{
							pobjLine->DeleteChar(0,&c1stChar,&c2ndChar);
							bIsEndTrim = TRUE;
						}
					}
				}

				//�Đݒ�(1999.03.20 Add for StatusBar Pane ID_INDICATOR_BYTE)
				m_pobjEditorDoc->SetOneLine(lSelectStartLine+lCnt,pobjLine);
			}
		}
	}

	//�`�r�l�R�����g������̍s�A����ێ�
	m_lStartLine   = lSelectStartLine;
	m_lStartOffset = lSelectStartOffset;
	m_lEndLine     = lSelectEndLine;
	if( lSelectEndOffset != 0 && bIsEndTrim )
		m_lEndOffset = lSelectEndOffset - 1;
	else
		m_lEndOffset = lSelectEndOffset;

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
//		pView->MySetCaretToLineOffset(m_lEndLine,m_lEndOffset);
		pView->MySetCaretPos(objCaretPos);

		//�I��͈͍Đݒ�
		pView->m_lSelectStartLine   = m_lStartLine;
		pView->m_lSelectStartOffset = m_lStartOffset;
		pView->m_lSelectEndLine     = m_lEndLine;
		pView->m_lSelectEndOffset   = m_lEndOffset;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FUndo ���s																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemDelAsmComment::doUndo()
{
	///////////////////////////////
	//	�������s
	///////////////////////////////

	//�ێ����Ă������I���W�i���s�����Ƃɖ߂�

	if( m_lSelectStartLine == m_lSelectEndLine ) //�P�s�I��
	{
		CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
		if( pobjLine != NULL )
		{
			pobjLine->Set(m_ppobjSelectLine[0]->String());
			m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjLine);
		}
	}
	else //�����s�I��
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

		//�I��͈͍Đݒ�
		pView->m_lSelectStartLine   = m_lSelectStartLine;
		pView->m_lSelectStartOffset = m_lSelectStartOffset;
		pView->m_lSelectEndLine     = m_lSelectEndLine;
		pView->m_lSelectEndOffset   = m_lSelectEndOffset;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FRedo ���s																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemDelAsmComment::doRedo()
{
	unsigned char c1stChar=0;				//DELETE���ꂽ���o�C�g����
	unsigned char c2ndChar=0;				//DELETE���ꂽ���o�C�g����

	///////////////////////////////
	//	�������s
	///////////////////////////////

	if( m_lSelectStartLine == m_lSelectEndLine ) //�P�s�I��
	{
		//�����폜
		CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
		if( pobjLine != NULL )
		{
			c1stChar = pobjLine->GetAt(m_lSelectStartOffset);
			if( !IsDBCSLeadByteEx(0,c1stChar) && c1stChar == CODE_SEMICOLON )
			{
				//�Z�~�R�����̂`�r�l�R�����g�폜
				pobjLine->DeleteChar(m_lSelectStartOffset,&c1stChar,&c2ndChar);
				m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjLine);
			}
		}
	}
	else //�����s�I��
	{
		for(ULONG lCnt=0; lCnt<(ULONG)(m_lSelectEndLine-m_lSelectStartLine+1); lCnt++ )
		{
			CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine+lCnt);
			if( pobjLine != NULL )
			{
				if( lCnt == 0 ) //�I���J�n�s
				{
					c1stChar = pobjLine->GetAt(m_lSelectStartOffset);
					if( !IsDBCSLeadByteEx(0,c1stChar) && c1stChar == CODE_SEMICOLON )
					{
						pobjLine->DeleteChar(m_lSelectStartOffset,&c1stChar,&c2ndChar);
					}
				}
				else if( lCnt < (ULONG)(m_lSelectEndLine-m_lSelectStartLine) ) //�I�𒆊ԍs
				{
					c1stChar = pobjLine->GetAt(0);
					if( !IsDBCSLeadByteEx(0,c1stChar) && c1stChar == CODE_SEMICOLON )
					{
						pobjLine->DeleteChar(0,&c1stChar,&c2ndChar);
					}
				}
				else if( lCnt == (ULONG)(m_lSelectEndLine-m_lSelectStartLine) )	//�I���I���s
				{
					if( m_lSelectEndOffset != 0 )
					{
						c1stChar = pobjLine->GetAt(0);
						if( !IsDBCSLeadByteEx(0,c1stChar) && c1stChar == CODE_SEMICOLON )
						{
							pobjLine->DeleteChar(0,&c1stChar,&c2ndChar);
						}
					}
				}

				//�Đݒ�(1999.03.20 Add for StatusBar Pane ID_INDICATOR_BYTE)
				m_pobjEditorDoc->SetOneLine(m_lSelectStartLine+lCnt,pobjLine);
			}
		}
	}


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
		pView->MySetCaretPos(m_objOriginalCaretPos);
//		pView->MySetCaretToLineOffset(m_lEndLine,m_lEndOffset);

		//�I��͈͍Đݒ�
		pView->m_lSelectStartLine   = m_lStartLine;
		pView->m_lSelectStartOffset = m_lStartOffset;
		pView->m_lSelectEndLine     = m_lEndLine;
		pView->m_lSelectEndOffset   = m_lEndOffset;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FUndo ���s�\����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoItemDelAsmComment::IsCanUndo()
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
BOOL CUndoItemDelAsmComment::IsCanRedo()
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
void CUndoItemDelAsmComment::setCanUndo
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
void CUndoItemDelAsmComment::setCanRedo
	(	BOOL	bIsCanRedo
	)
{
	m_bIsCanRedo = bIsCanRedo;
}

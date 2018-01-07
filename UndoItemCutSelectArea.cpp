// UndoItemCutSelectArea.cpp: CUndoItemCutSelectArea �N���X�̃C���v�������e�[�V����
//

#include "stdafx.h"
#include <Afxtempl.h>			//�l�e�b�̂b�l�������g�p����
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include <afxadv.h>			//for CSharedFile
#include "Editor_Def.h"
#include "ColorManager.h"
#include "Line.h"
#include "LineManager.h"
#include "UndoManager.h"
#include "EditorDoc.h"
#include "BCMenu.h"
#include "EditorView.h"
#include "UndoItem.h"
#include "UndoItemCutSelectArea.h"

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
CUndoItemCutSelectArea::CUndoItemCutSelectArea()
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = NULL;

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
CUndoItemCutSelectArea::CUndoItemCutSelectArea
	(	CEditorDoc*		pobjEditorDoc
	)
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = pobjEditorDoc;

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
CUndoItemCutSelectArea::~CUndoItemCutSelectArea()
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
void CUndoItemCutSelectArea::Do()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s																	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemCutSelectArea::Do
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		BOOL	bToClipBoard,			// �N���b�v�{�[�h�փR�s�[���邩�H(TRUE:����AFALSE:���Ȃ��j
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
	m_lSelectStartLine        = lSelectStartLine;
	m_lSelectStartOffset      = lSelectStartOffset;
	m_lSelectEndLine          = lSelectEndLine;
	m_lSelectEndOffset        = lSelectEndOffset;
	m_bToClipBoard            = bToClipBoard;
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

	if( bToClipBoard )//�N���b�v�{�[�h�ւ̃R�s�[������ꍇ
	{
		//�n�k�d�f�[�^�\�[�X�쐬
		COleDataSource*	pobjOleDataSource = new COleDataSource();

		//�R�s�[����f�[�^�쐬
		CSharedFile objShareFile(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
		if(lSelectStartLine == lSelectEndLine) //�P�s
		{
			CLine* pobjLine = (CLine*)m_pobjEditorDoc->GetOneLine(lSelectStartLine);
			objShareFile.Write( pobjLine->Data(lSelectStartOffset),
								(lSelectEndOffset-lSelectStartOffset) );
		}
		else //�����s
		{
			for(ULONG lLineNo=lSelectStartLine; lLineNo<=(ULONG)lSelectEndLine; lLineNo++)
			{
				CLine* pobjLine = (CLine*)m_pobjEditorDoc->GetOneLine(lLineNo);
				if(!pobjLine)
					continue;
				if(lLineNo == (ULONG)lSelectStartLine)//�I���J�n�s
				{
					objShareFile.Write(	pobjLine->Data(lSelectStartOffset),
										(pobjLine->GetLength()-lSelectStartOffset) );
				}
				else if(lLineNo == (ULONG)lSelectEndLine)//�I���I���s
				{
					objShareFile.Write( "\r\n", 2 );
					objShareFile.Write( pobjLine->String(), lSelectEndOffset );
				}
				else //�r���s
				{
					objShareFile.Write( "\r\n", 2 );
					objShareFile.Write( pobjLine->String(), pobjLine->GetLength() );
				}
			}
		}

		//�N���b�v�{�[�h��
		pobjOleDataSource->CacheGlobalData(CF_TEXT, objShareFile.Detach());
		pobjOleDataSource->SetClipboard();
	}

	///////////////////////////////
	//	�������s
	///////////////////////////////

	if( lSelectStartLine == lSelectEndLine ) //�P�s�I��
	{
		CLine* pobjLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine);
		if( pobjLine != NULL )
		{
			//�I��͈͐؂���
			pobjLine->Trim(lSelectStartOffset,lSelectEndOffset);
			m_pobjEditorDoc->SetOneLine(lSelectStartLine,pobjLine);
		}
	}
	else //�����s�I��
	{
		CLine*	pobjStartLine = NULL;
		CLine*	pobjEndLine = NULL;
		for(ULONG lCnt=0; lCnt<(ULONG)(lSelectEndLine-lSelectStartLine+1); lCnt++ )
		{
			if( lCnt == 0 ) //�I���J�n�s
			{
				pobjStartLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine);
				//�I���J�n�I�t�Z�b�g�ȍ~�؂���
				if( pobjStartLine != NULL )
				{
					pobjStartLine->SetAt(lSelectStartOffset,0);
					m_pobjEditorDoc->SetOneLine(lSelectStartLine,pobjStartLine);
				}
			}
			else if( lCnt < (ULONG)(lSelectEndLine-lSelectStartLine) ) //�I�𒆊ԍs
			{
				m_pobjEditorDoc->DeleteOneLine(lSelectStartLine+1);
			}
			else if( lCnt == (ULONG)(lSelectEndLine-lSelectStartLine) )	//�I���I���s
			{
				pobjEndLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine+1);
				//�I���I���I�t�Z�b�g���܂��؂���
				if( pobjEndLine != NULL )
				{
					pobjEndLine->Trim(0,lSelectEndOffset);
					m_pobjEditorDoc->SetOneLine(lSelectStartLine+1,pobjEndLine);
				}
			}
		}
		//�I���J�n�s�ƑI���I���s��A��
		if( pobjEndLine != NULL )
		{
			pobjStartLine->Catenate(pobjEndLine->String());
			m_pobjEditorDoc->DeleteOneLine(lSelectStartLine+1);
		}
		m_pobjEditorDoc->SetOneLine(lSelectStartLine,pobjStartLine);
	}

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

		//�I��͈͉���
		pView->m_lSelectStartLine = -1;
		pView->m_lSelectStartOffset = -1;
		pView->m_lSelectEndLine = -1;
		pView->m_lSelectEndOffset = -1;

		//���݂̃X�N���[���ʒu�̍s�ԍ��ƁA�I���J�n�s�ԍ����ׂ�
		long lScrollLine = 0;
		if( pView->m_lfHeight != 0 )
			lScrollLine = objScrollPosLog.y / (pView->m_lfHeight+pView->m_lLineInterval);
		if( lScrollLine >= m_lSelectStartLine )
		{
			//�I���J�n�s�A��ֈړ�
			pView->Jump2LineOffset(m_lSelectStartLine,m_lSelectStartOffset);
		}
		else
		{
			//1999.03.10 Add for test
			pView->MyScrollToPosition(objScrollPosLog);
		}

		//�L�����b�g��I���J�n�ʒu�ֈʒu�Â�
		pView->MySetCaretToLineOffset(lSelectStartLine,lSelectStartOffset);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FUndo ���s																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemCutSelectArea::doUndo()
{
	if( m_lSelectStartLine == m_lSelectEndLine ) //�P�s�I��
	{
		CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
		//���Ƃ̍s�ɓ���ւ���
		if( pobjLine != NULL )
		{
			pobjLine->Set(m_ppobjSelectLine[0]->String());
			m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjLine);
		}
	}
	else //�����s�I��
	{
		for(ULONG lCnt=m_lSelectLineCnt-1; lCnt>=0; lCnt-- )
		{
			if( lCnt == 0 ) //�I���J�n�s
			{
				CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
				//���Ƃ̍s�ɓ���ւ���
				if( pobjLine != NULL )
				{
					pobjLine->Set(m_ppobjSelectLine[0]->String());
					m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjLine);
				}
				break;
			}
			else
			{
				//�폜���Ă����s��}�����Ȃ���
				CLine*	pobjLine = new CLine();
				pobjLine->Set(m_ppobjSelectLine[lCnt]->String());
				m_pobjEditorDoc->InsertOneLine(m_lSelectStartLine+1,pobjLine);
			}
		}
	}

	//�h�L�������g��񕜌�
	m_pobjEditorDoc->SetModifiedFlag(m_bOriginalModifiedFlag);
	m_pobjEditorDoc->SetTitle(m_objOriginalDocTitle);


	POSITION pos = m_pobjEditorDoc->GetFirstViewPosition();
	while(pos != 0)
	{
		CEditorView* pView = (CEditorView*)m_pobjEditorDoc->GetNextView(pos);

		//�I��͈͕���
		pView->m_lSelectStartLine   = m_lSelectStartLine;
		pView->m_lSelectStartOffset = m_lSelectStartOffset;
		pView->m_lSelectEndLine     = m_lSelectEndLine;
		pView->m_lSelectEndOffset   = m_lSelectEndOffset;

		//���[���A�s�ԍ��\���̕���
		pView->SetFlagDispRuler(m_bOriginalDispRuler);
		pView->SetFlagDispLineNo(m_bOriginalDispLineNo);

		//�L�����b�g�ʒu���X�N���[���ʒu�̕���
//1999.03.06 ����
		pView->MyScrollToPosition(m_objOriginalScrollPosLog);
//1998.10.18 Cut By S.Ekura �X�N���[������o�O�̃t�B�b�N�X
//		pView->MyScrollToPosition(m_objOriginalScrollPosLog);
//1998.10.18 Cut End
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
void CUndoItemCutSelectArea::doRedo()
{
	if( m_bToClipBoard )//�N���b�v�{�[�h�ւ̃R�s�[������ꍇ
	{
		//�n�k�d�f�[�^�\�[�X�쐬
		COleDataSource*	pobjOleDataSource = new COleDataSource();

		//�R�s�[����f�[�^�쐬
		CSharedFile objShareFile(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
		if(m_lSelectStartLine == m_lSelectEndLine) //�P�s
		{
			CLine* pobjLine = (CLine*)m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
			objShareFile.Write( pobjLine->Data(m_lSelectStartOffset),
								(m_lSelectEndOffset-m_lSelectStartOffset) );
		}
		else //�����s
		{
			for(ULONG lLineNo=m_lSelectStartLine; lLineNo<=(ULONG)m_lSelectEndLine; lLineNo++)
			{
				CLine* pobjLine = (CLine*)m_pobjEditorDoc->GetOneLine(lLineNo);
				if(!pobjLine)
					continue;
				if(lLineNo == (ULONG)m_lSelectStartLine)//�I���J�n�s
				{
					objShareFile.Write(	pobjLine->Data(m_lSelectStartOffset),
										(pobjLine->GetLength()-m_lSelectStartOffset) );
				}
				else if(lLineNo == (ULONG)m_lSelectEndLine)//�I���I���s
				{
					objShareFile.Write( "\r\n", 2 );
					objShareFile.Write( pobjLine->String(), m_lSelectEndOffset );
				}
				else //�r���s
				{
					objShareFile.Write( "\r\n", 2 );
					objShareFile.Write( pobjLine->String(), pobjLine->GetLength() );
				}
			}
		}

		//�N���b�v�{�[�h��
		pobjOleDataSource->CacheGlobalData(CF_TEXT, objShareFile.Detach());
		pobjOleDataSource->SetClipboard();
	}


	if( m_lSelectStartLine == m_lSelectEndLine ) //�P�s�I��
	{
		CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
		//�I��͈͐؂���
		if( pobjLine != NULL )
		{
			pobjLine->Trim(m_lSelectStartOffset,m_lSelectEndOffset);
			m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjLine);
		}
	}
	else //�����s�I��
	{
		CLine*	pobjStartLine = NULL;
		CLine*	pobjEndLine = NULL;
		for(ULONG lCnt=0; lCnt<(ULONG)(m_lSelectEndLine-m_lSelectStartLine+1); lCnt++ )
		{
			if( lCnt == 0 ) //�I���J�n�s
			{
				pobjStartLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
				//�I���J�n�I�t�Z�b�g�ȍ~�؂���
				if( pobjStartLine != NULL )
				{
					pobjStartLine->SetAt(m_lSelectStartOffset,0);
					m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjStartLine);
				}
			}
			else if( lCnt < (ULONG)(m_lSelectEndLine-m_lSelectStartLine) ) //�I�𒆊ԍs
			{
				m_pobjEditorDoc->DeleteOneLine(m_lSelectStartLine+1);
			}
			else if( lCnt == (ULONG)(m_lSelectEndLine-m_lSelectStartLine) )	//�I���I���s
			{
				pobjEndLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine+1);
				//�I���I���I�t�Z�b�g���܂��؂���
				if( pobjEndLine != NULL )
				{
					pobjEndLine->Trim(0,m_lSelectEndOffset);
					m_pobjEditorDoc->SetOneLine(m_lSelectStartLine+1,pobjEndLine);
				}
			}
		}
		//�I���J�n�s�ƑI���I���s��A��
		if( pobjEndLine != NULL )
		{
			pobjStartLine->Catenate(pobjEndLine->String());
			m_pobjEditorDoc->DeleteOneLine(m_lSelectStartLine+1);
		}
		m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjStartLine);
	}

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

		//�I��͈͉���
		pView->m_lSelectStartLine = -1;
		pView->m_lSelectStartOffset = -1;
		pView->m_lSelectEndLine = -1;
		pView->m_lSelectEndOffset = -1;

		//���[���A�s�ԍ��\���̕���
		pView->SetFlagDispRuler(m_bOriginalDispRuler);
		pView->SetFlagDispLineNo(m_bOriginalDispLineNo);

		//�L�����b�g�ʒu���X�N���[���ʒu�̕���
		pView->MyScrollToPosition(m_objOriginalScrollPosLog);

		//�X�N���[���ʒu�̍s�ԍ��ƁA�I���J�n�s�ԍ����ׂ�
		long lScrollLine = 0;
		if( pView->m_lfHeight != 0 )
			lScrollLine = m_objOriginalScrollPosLog.y / (pView->m_lfHeight+pView->m_lLineInterval);
		if( lScrollLine >= m_lSelectStartLine )
		{
			//�I���J�n�s�A��ֈړ�
			pView->Jump2LineOffset(m_lSelectStartLine,m_lSelectStartOffset);
		}
		else
		{
			//1999.03.10 Add for test
			pView->MyScrollToPosition(m_objOriginalScrollPosLog);
		}

		//�L�����b�g��I���J�n�ʒu�ֈʒu�Â�
		pView->MySetCaretToLineOffset(m_lSelectStartLine,m_lSelectStartOffset);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FUndo ���s�\����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoItemCutSelectArea::IsCanUndo()
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
BOOL CUndoItemCutSelectArea::IsCanRedo()
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
void CUndoItemCutSelectArea::setCanUndo
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
void CUndoItemCutSelectArea::setCanRedo
	(	BOOL	bIsCanRedo
	)
{
	m_bIsCanRedo = bIsCanRedo;
}


// UndoItemEditPaste.cpp: CUndoItemEditPaste �N���X�̃C���v�������e�[�V����
//

#include "stdafx.h"
#include <Afxtempl.h>			//�l�e�b�̂b�l�������g�p����
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include <afxadv.h>			//for CSharedFile
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
#include "UndoItemEditPaste.h"

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
CUndoItemEditPaste::CUndoItemEditPaste()
{
	m_pApp = (CEditorApp*)AfxGetApp();
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

	m_objSelectLineArray.SetSize(0);			//�I���e�L�X�g�̔z��i�����T�C�Y�[���j
	m_objPasteLineArray.SetSize(0);				//�\��t���e�L�X�g�̔z��i�����T�C�Y�[���j

	m_bOriginalModifiedFlag = 0;
	m_objOriginalDocTitle = _T("");
	m_objOriginalCaretPos.x = 0;
	m_objOriginalCaretPos.y = 0;
	m_objOriginalScrollPosLog.x = 0;
	m_objOriginalScrollPosLog.y = 0;
	m_bOriginalDispRuler = 0;
	m_bOriginalDispLineNo = 0;
	m_bSetCaretToEndOnPaste = FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUndoItemEditPaste::CUndoItemEditPaste
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

	m_lSelectStartLine = 0;
	m_lSelectStartOffset = 0;
	m_lSelectEndLine = 0;
	m_lSelectEndOffset = 0;

	m_objSelectLineArray.SetSize(0);			//�I���e�L�X�g�̔z��i�����T�C�Y�[���j
	m_objPasteLineArray.SetSize(0);				//�\��t���e�L�X�g�̔z��i�����T�C�Y�[���j

	m_objOriginalDocTitle = _T("");
	m_objOriginalCaretPos.x = 0;
	m_objOriginalCaretPos.y = 0;
	m_objOriginalScrollPosLog.x = 0;
	m_objOriginalScrollPosLog.y = 0;
	m_bOriginalDispRuler = 0;
	m_bOriginalDispLineNo = 0;
	m_bSetCaretToEndOnPaste = FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUndoItemEditPaste::~CUndoItemEditPaste()
{
	long lCnt;

	//�I���s
	for(lCnt=0; lCnt<m_objSelectLineArray.GetSize(); lCnt++)
	{
		CLine* pobjLine = (CLine*)m_objSelectLineArray.GetAt(lCnt);
		delete pobjLine;
	}
	m_objSelectLineArray.RemoveAll();

	//�\��t���s
	for(lCnt=0; lCnt<m_objPasteLineArray.GetSize(); lCnt++)
	{
		CLine* pobjLine = (CLine*)m_objPasteLineArray.GetAt(lCnt);
		delete pobjLine;
	}
	m_objPasteLineArray.RemoveAll();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s																	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemEditPaste::Do()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s																	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemEditPaste::Do
	(	long	lCurrentLine,			// �L�����b�g�ʒu�̍s�ԍ�
		long	lCurrentOffset,			// �L�����b�g�ʒu�̗�ԍ�
		long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint& objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo				// �s�ԍ��\���t���O
	)
{
	long lCRCount=0;
	long lLFCount=0;

	/////////////////////////////
	//	���s�O���ێ�
	/////////////////////////////
	//�p�����[�^
	m_lStartLine              = lCurrentLine;
	m_lStartOffset            = lCurrentOffset;
	m_lEndLine                = lCurrentLine;		//����t���I���ʒu�s�ԍ��ɏ����l��ݒ�
	m_lEndOffset              = lCurrentOffset;		//����t���I���ʒu��ԍ��ɏ����l��ݒ�
	m_lSelectStartLine        = lSelectStartLine;
	m_lSelectStartOffset      = lSelectStartOffset;
	m_lSelectEndLine          = lSelectEndLine;
	m_lSelectEndOffset        = lSelectEndOffset;
	m_objOriginalCaretPos     = objCaretPos;
	m_objOriginalScrollPosLog = objScrollPosLog;
	m_bOriginalDispRuler      = bDispRuler;
	m_bOriginalDispLineNo     = bDispLineNo;
	m_bSetCaretToEndOnPaste   = m_pApp->GetProfileInt("CEditorDoc","bSetCaretToEndOnPaste",1);

	//�h�L�������g���
	m_bOriginalModifiedFlag = m_pobjEditorDoc->IsModified();
	m_objOriginalDocTitle   = m_pobjEditorDoc->GetTitle();


	//////////////////////////////////////////////////////
	//	�͈͑I������Ă���ꍇ�A�܂����̕�����؂���	//
	//////////////////////////////////////////////////////

	if( (lSelectStartLine   != -1) && (lSelectEndLine   != -1) &&
		(lSelectStartOffset != -1) && (lSelectEndOffset != -1)
	)
	{
		if( lSelectStartLine == lSelectEndLine ) //�P�s�I��
		{
			CLine* pobjLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine);
			if( pobjLine != NULL )
			{
				//�I��͈͑ޔ�
				CLine* pobjTempLine = new CLine(pobjLine->Data(lSelectStartOffset));
				pobjTempLine->SetAt(lSelectEndOffset-lSelectStartOffset,0);
				m_objSelectLineArray.Add((CObject*)pobjTempLine);
				//�I��͈͐؂���
				pobjLine->Trim(lSelectStartOffset,lSelectEndOffset);
				m_pobjEditorDoc->SetOneLine(lSelectStartLine,pobjLine);
			}
			else
			{
				//�_�~�[�I���s�ޔ�
				CLine* pobjTempLine = new CLine();
				m_objSelectLineArray.Add((CObject*)pobjTempLine);
			}
		}
		else //�����s�I��
		{
			CLine*	pobjLine = NULL;
			CLine*	pobjStartLine = NULL;
			CLine*	pobjEndLine = NULL;
			for(ULONG lCnt=0; lCnt<(ULONG)(lSelectEndLine-lSelectStartLine+1); lCnt++ )
			{
				if( lCnt == 0 ) //�I���J�n�s
				{
					pobjStartLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine);
					if( pobjStartLine != NULL )
					{
						//�I���J�n�s�ޔ�
						CLine* pobjTempLine = new CLine(pobjStartLine->Data(lSelectStartOffset));
						m_objSelectLineArray.Add((CObject*)pobjTempLine);
						//�I���J�n�I�t�Z�b�g�ȍ~�؂���
						pobjStartLine->SetAt(lSelectStartOffset,0);
						m_pobjEditorDoc->SetOneLine(lSelectStartLine,pobjStartLine);
					}
					else
					{
						//�_�~�[�I���s�ޔ�
						CLine* pobjTempLine = new CLine();
						m_objSelectLineArray.Add((CObject*)pobjTempLine);
					}
				}
				else if( lCnt < (ULONG)(lSelectEndLine-lSelectStartLine) ) //�I�𒆊ԍs
				{
					pobjLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine+1);
					if( pobjLine != NULL )
					{
						//�I�𒆊ԍs�ޔ�
						CLine* pobjTempLine = new CLine(pobjLine->String());
						m_objSelectLineArray.Add((CObject*)pobjTempLine);
						//�I�𒆊ԍs�؂���
						m_pobjEditorDoc->DeleteOneLine(lSelectStartLine+1);
					}
					else
					{
						//�_�~�[�I���s�ޔ�
						CLine* pobjTempLine = new CLine();
						m_objSelectLineArray.Add((CObject*)pobjTempLine);
					}
				}
				else if( lCnt == (ULONG)(lSelectEndLine-lSelectStartLine) )//�I���I���s
				{
					pobjEndLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine+1);
					if( pobjEndLine != NULL )
					{
						//�I���I���s�ޔ�
						CLine* pobjTempLine = new CLine(pobjEndLine->String());
						pobjTempLine->SetAt(lSelectEndOffset,0);
						m_objSelectLineArray.Add((CObject*)pobjTempLine);
						//�I���I���I�t�Z�b�g���܂��؂���
						pobjEndLine->Trim(0,lSelectEndOffset);
						m_pobjEditorDoc->SetOneLine(lSelectStartLine+1,pobjEndLine);
					}
					else
					{
						//�_�~�[�I���s�ޔ�
						CLine* pobjTempLine = new CLine();
						m_objSelectLineArray.Add((CObject*)pobjTempLine);
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
	}

	//////////////////////////////
	//	����t���J�n�ʒu����	//
	//////////////////////////////

	if( (m_lSelectStartLine   != -1) && (m_lSelectEndLine   != -1) &&
		(m_lSelectStartOffset != -1) && (m_lSelectEndOffset != -1)
	)
	{
		if( (m_lStartLine   != m_lSelectStartLine)  ||
			(m_lStartOffset != m_lSelectStartOffset) 
		)
		{
			//����t���J�n�ʒu���A�I���J�n�ʒu�Ɠ����ɂ���
			m_lStartLine   = m_lSelectStartLine;
			m_lStartOffset = m_lSelectStartOffset;
			m_lEndLine     = m_lSelectStartLine;		//����t���I���ʒu�s�ԍ��ɏ����l�ݒ�
			m_lEndOffset   = m_lSelectStartOffset;		//�\��t���I���ʒu��ԍ��ɏ����l�ݒ�
		}
	}

	//////////////////////////////////////////
	//	�N���b�v�{�[�h���f�[�^�ǂݏo��	//
	//////////////////////////////////////////

	//�n�k�d�f�[�^�I�u�W�F�N�g�쐬
	COleDataObject	objOleDataObject;
	//�N���b�v�{�[�h��̃f�[�^�I�u�W�F�N�g�Ɍ��ѕt���܂��B
	objOleDataObject.AttachClipboard();
	CFile* pobjFile = objOleDataObject.GetFileData(CF_TEXT);

	//////////////////////////////////
	//	����t���ʒu�ւ̑}������	//
	//////////////////////////////////

	//����t���J�n�ʒu����s���܂ł̕������ޔ����Ă���
	CLine	objEnd;	
	CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lStartLine);
	if( (pobjLine != NULL) && (m_lStartOffset < pobjLine->GetLength()) )
	{
		objEnd.Set(pobjLine->Data(m_lStartOffset));
		pobjLine->SetAt(m_lStartOffset,0);
		m_pobjEditorDoc->SetOneLine(m_lStartLine,pobjLine);
	}

	//�N���b�v�{�[�h���f�[�^�ǂݏo��
	CArchive ar( pobjFile, CArchive::load );
	pobjLine = NULL;
	ULONG lCnt=0;
	int  nNowRet=0;
	int  nBeforeRet=0;
	m_pApp->PrepareMyReadString();
	for(lCnt=0; ;lCnt++)//�P�s�Âǂݍ���
	{
		CString objStrBuf = _T("");
		nNowRet = m_pApp->MyReadString(ar,objStrBuf,&lCRCount,&lLFCount);
		if( nNowRet == 0 ) //EOF�I��
		{
			if( objStrBuf.IsEmpty() )
			{
				if( nBeforeRet != 1 )
					lCnt--;
				break;
			}
		}

		//MyReadString ���^�[���l�ێ�
		nBeforeRet = nNowRet;

		if( lCnt==0 ) //����t���A�P���
		{
			//�\��t���s�ޔ�
			pobjLine = new CLine((const unsigned char*)(LPCTSTR)objStrBuf);
			m_objPasteLineArray.Add((CObject*)pobjLine);

			//�\��t���s�o�^
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
			//����t���I���ʒu��ԍ��X�V
			m_lEndOffset = pobjLine->GetLength();
		}
		else //����t���A�Q��ڈȍ~
		{
			//�\��t���s�X�V
			pobjLine = (CLine*)m_objPasteLineArray.GetAt(m_objPasteLineArray.GetSize()-1);
			pobjLine->Set((const unsigned char*)(LPCTSTR)objStrBuf);

			//�\��t���s�o�^
			pobjLine = m_pobjEditorDoc->GetOneLine(m_lStartLine+lCnt);
			pobjLine->Set((const unsigned char*)(LPCTSTR)objStrBuf);
			m_pobjEditorDoc->SetOneLine(m_lStartLine+lCnt,pobjLine);
			//����t���I���ʒu��ԍ��X�V
			m_lEndOffset = pobjLine->GetLength();
		}

		if(nNowRet == 1) //���s�I�����Ă����ꍇ
		{
			//���̍s�̕��̓\��t���s�ޔ�
			pobjLine = new CLine();
			m_objPasteLineArray.Add((CObject*)pobjLine);

			//���̍s�̕���o�^
			pobjLine = new CLine();
			m_pobjEditorDoc->InsertOneLine(m_lStartLine+lCnt+1,pobjLine);
			//����t���I���ʒu�̍s�ԍ����X�V
			m_lEndLine++;
			//����t���I���ʒu��ԍ��X�V
			m_lEndOffset = 0;
		}
		else //�d�n�e�I�����Ă����ꍇ
		{
			break;
		}
	}

	//����t���J�n�ʒu����s���܂ł̕����񂪑ޔ�����Ă����ꍇ
	if( objEnd.GetLength() > 0 )
	{
		//�Ō�ɘA������
		pobjLine = m_pobjEditorDoc->GetOneLine(m_lStartLine+lCnt);
		if( pobjLine != NULL )
			pobjLine->Catenate(objEnd.String());
		else
			pobjLine = new CLine(objEnd.String());
		m_pobjEditorDoc->SetOneLine(m_lStartLine+lCnt,pobjLine);
	}

	//�N���b�v�{�[�h�N���[�Y
	ar.Abort();
	pobjFile->Close();
	delete pobjFile;

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

	//�I��͈͍Đݒ�i����t���J�n�`�I���ʒu�j
	POSITION pos = m_pobjEditorDoc->GetFirstViewPosition();
	while(pos != 0)
	{
		CEditorView* pView = (CEditorView*)m_pobjEditorDoc->GetNextView(pos);

		//�L�����b�g�ʒu��ݒ�
		if(m_bSetCaretToEndOnPaste)
			pView->MySetCaretToLineOffset(m_lEndLine,m_lEndOffset);
		else
			pView->MySetCaretToLineOffset(m_lStartLine,m_lStartOffset);

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
void CUndoItemEditPaste::doUndo()
{
	//////////////////////////////////////////////////
	//	�\��t���J�n�ʒu�`�I���ʒu�̕�����؂���	//
	//////////////////////////////////////////////////

	if( m_lStartLine == m_lEndLine ) //�P�s�\��t���̂t������
	{
		CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lStartLine);
		if( pobjLine != NULL )
		{
			//�\��t���͈͐؂���
			pobjLine->Trim(m_lStartOffset,m_lEndOffset);
			m_pobjEditorDoc->SetOneLine(m_lStartLine,pobjLine);
		}
	}
	else //�����s�\��t���̂t������
	{
		CLine*	pobjStartLine = NULL;
		CLine*	pobjEndLine = NULL;
		for(ULONG lCnt=0; lCnt<(ULONG)(m_lEndLine-m_lStartLine+1); lCnt++ )
		{
			if( lCnt == 0 ) //�\��t���J�n�s
			{
				pobjStartLine = m_pobjEditorDoc->GetOneLine(m_lStartLine);
				//�\��t���J�n�I�t�Z�b�g�ȍ~�؂���
				if( pobjStartLine != NULL )
				{
					pobjStartLine->SetAt(m_lStartOffset,0);
					m_pobjEditorDoc->SetOneLine(m_lStartLine,pobjStartLine);
				}
			}
			else if( lCnt < (ULONG)(m_lEndLine-m_lStartLine) ) //�\��t�����ԍs
			{
				m_pobjEditorDoc->DeleteOneLine(m_lStartLine+1);
			}
			else if( lCnt == (ULONG)(m_lEndLine-m_lStartLine) )//�\��t���I���s
			{
				pobjEndLine = m_pobjEditorDoc->GetOneLine(m_lStartLine+1);
				//�\��t���I���I�t�Z�b�g���܂��؂���
				if( pobjEndLine != NULL )
				{
					pobjEndLine->Trim(0,m_lEndOffset);
					m_pobjEditorDoc->SetOneLine(m_lStartLine+1,pobjEndLine);
				}
			}
		}
		//�\��t���J�n�s�Ɠ\��t���I���s��A��
		if( pobjEndLine != NULL )
		{
			pobjStartLine->Catenate(pobjEndLine->String());
			m_pobjEditorDoc->DeleteOneLine(m_lStartLine+1);
		}
		m_pobjEditorDoc->SetOneLine(m_lStartLine,pobjStartLine);
	}

	//////////////////////////////////////////////////////
	//	�͈͑I������Ă����ꍇ�́A���̕��������ɖ߂�	//
	//////////////////////////////////////////////////////

	if( (m_lSelectStartLine   != -1) && (m_lSelectEndLine   != -1) &&
		(m_lSelectStartOffset != -1) && (m_lSelectEndOffset != -1)
	)
	{
		//����t���J�n�ʒu����s���܂ł̕������ޔ����Ă���
		CLine	objEnd;	
		CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
		if( (pobjLine != NULL) && (m_lSelectStartOffset < pobjLine->GetLength()) )
		{
			objEnd.Set(pobjLine->Data(m_lSelectStartOffset));
			pobjLine->SetAt(m_lSelectStartOffset,0);
			m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjLine);
		}

		//�ޔ����Ă������͈͑I�𕔕����P�s�Â��Ƃɖ߂�
		pobjLine = NULL;
		CLine* pobjSelectLine = NULL;
		ULONG lCnt=0;
		for(lCnt=0; lCnt<(ULONG)m_objSelectLineArray.GetSize(); lCnt++)
		{
			pobjSelectLine = (CLine*)m_objSelectLineArray.GetAt(lCnt);
			if( lCnt==0 ) //�P���
			{
				pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
				if( pobjLine != NULL )
					pobjLine->Catenate(pobjSelectLine->String());
				else if( pobjLine == NULL )
					pobjLine = new CLine(pobjSelectLine->String());
				m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjLine);
			}
			else //����t���A�Q��ڈȍ~
			{
				pobjLine = new CLine(pobjSelectLine->String());
				m_pobjEditorDoc->InsertOneLine(m_lSelectStartLine+lCnt,pobjLine);
			}
		}
		lCnt--;

		//����t���J�n�ʒu����s���܂ł̕����񂪑ޔ�����Ă����ꍇ
		if( objEnd.GetLength() > 0 )
		{
			//�Ō�ɘA������
			pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine+lCnt);
			if( pobjLine != NULL )
				pobjLine->Catenate(objEnd.String());
			else
				pobjLine = new CLine(objEnd.String());
			m_pobjEditorDoc->SetOneLine(m_lSelectStartLine+lCnt,pobjLine);
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
void CUndoItemEditPaste::doRedo()
{
	//////////////////////////////////////////////////////
	//	�͈͑I������Ă���ꍇ�A�܂����̕�����؂���	//
	//////////////////////////////////////////////////////

	if( (m_lSelectStartLine   != -1) && (m_lSelectEndLine   != -1) &&
		(m_lSelectStartOffset != -1) && (m_lSelectEndOffset != -1)
	)
	{
		if( m_lSelectStartLine == m_lSelectEndLine ) //�P�s�I��
		{
			CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
			if( pobjLine != NULL )
			{
				//�I��͈͐؂���
				pobjLine->Trim(m_lSelectStartOffset,m_lSelectEndOffset);
				m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjLine);
			}
		}
		else //�����s�I��
		{
			CLine*	pobjLine = NULL;
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
					//�I�𒆊ԍs�؂���
					m_pobjEditorDoc->DeleteOneLine(m_lSelectStartLine+1);
				}
				else if( lCnt == (ULONG)(m_lSelectEndLine-m_lSelectStartLine) )//�I���I���s
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
	}

	//////////////////////////////////
	//	����t���ʒu�ւ̑}������	//
	//////////////////////////////////

	//����t���J�n�ʒu����s���܂ł̕������ޔ����Ă���
	CLine	objEnd;	
	CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lStartLine);
	if( (pobjLine != NULL) && (m_lStartOffset < pobjLine->GetLength()) )
	{
		objEnd.Set(pobjLine->Data(m_lStartOffset));
		pobjLine->SetAt(m_lStartOffset,0);
		m_pobjEditorDoc->SetOneLine(m_lStartLine,pobjLine);
	}

	//�P�s�Â\��t��
	pobjLine = NULL;
	CLine* pobjPasteLine = NULL;
	ULONG lCnt=0;
	for(lCnt=0; lCnt<(ULONG)m_objPasteLineArray.GetSize(); lCnt++)
	{
		pobjPasteLine = (CLine*)m_objPasteLineArray.GetAt(lCnt);

		if( lCnt==0 ) //����t���A�P���
		{
			pobjLine = m_pobjEditorDoc->GetOneLine(m_lStartLine);
			if( pobjLine != NULL )
			{
				pobjLine->Catenate(pobjPasteLine->String());
				m_pobjEditorDoc->SetOneLine(m_lStartLine,pobjLine);
			}
			else
			{
				pobjLine = new CLine(pobjPasteLine->String());
				m_pobjEditorDoc->InsertOneLine(m_lStartLine,pobjLine);
			}
		}
		else //����t���A�Q��ڈȍ~
		{
			pobjLine = new CLine(pobjPasteLine->String());
			m_pobjEditorDoc->InsertOneLine(m_lStartLine+lCnt,pobjLine);
		}
	}
	lCnt--;

	//����t���J�n�ʒu����s���܂ł̕����񂪑ޔ�����Ă����ꍇ
	if( objEnd.GetLength() > 0 )
	{
		//�Ō�ɘA������
		pobjLine = m_pobjEditorDoc->GetOneLine(m_lStartLine+lCnt);
		if( pobjLine != NULL )
			pobjLine->Catenate(objEnd.String());
		else
			pobjLine = new CLine(objEnd.String());
		m_pobjEditorDoc->SetOneLine(m_lStartLine+lCnt,pobjLine);
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
//		pView->MySetCaretPos(m_objOriginalCaretPos);
		//�L�����b�g�ʒu��ݒ�
		if(m_bSetCaretToEndOnPaste)
			pView->MySetCaretToLineOffset(m_lEndLine,m_lEndOffset);
		else
			pView->MySetCaretToLineOffset(m_lStartLine,m_lStartOffset);

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
BOOL CUndoItemEditPaste::IsCanUndo()
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
BOOL CUndoItemEditPaste::IsCanRedo()
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
void CUndoItemEditPaste::setCanUndo
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
void CUndoItemEditPaste::setCanRedo
	(	BOOL	bIsCanRedo
	)
{
	m_bIsCanRedo = bIsCanRedo;
}


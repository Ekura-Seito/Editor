// UndoItemOleDrop.cpp: CUndoItemOleDrop �N���X�̃C���v�������e�[�V����
//

#include "stdafx.h"
#include <Afxtempl.h>			//�l�e�b�̂b�l�������g�p����
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
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
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

	m_objDropLineArray.SetSize(0);				//�h���b�v�e�L�X�g�̔z��i�����T�C�Y�[���j

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

	m_objDropLineArray.SetSize(0);				//�h���b�v�e�L�X�g�̔z��i�����T�C�Y�[���j

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
CUndoItemOleDrop::~CUndoItemOleDrop()
{
	long lCnt;

	//�h���b�v�s
	for(lCnt=0; lCnt<m_objDropLineArray.GetSize(); lCnt++)
	{
		CLine* pobjLine = (CLine*)m_objDropLineArray.GetAt(lCnt);
		delete pobjLine;
	}
	m_objDropLineArray.RemoveAll();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s																	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemOleDrop::Do()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s																	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemOleDrop::Do
	(	COleDataObject*	pobjOleDataObject,	// �n�k�d�f�[�^�I�u�W�F�N�g
		long	lCurrentLine,				// �L�����b�g�ʒu�̍s�ԍ�
		long	lCurrentOffset,				// �L�����b�g�ʒu�̗�ԍ�
		CPoint&	objCaretPos,				// �L�����b�g�ʒu
		CPoint& objScrollPosLog,			// �X�N���[���ʒu
		BOOL	bDispRuler,					// ���[���\���t���O
		BOOL	bDispLineNo					// �s�ԍ��\���t���O
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
	m_objOriginalCaretPos     = objCaretPos;
	m_objOriginalScrollPosLog = objScrollPosLog;
	m_bOriginalDispRuler      = bDispRuler;
	m_bOriginalDispLineNo     = bDispLineNo;

	//�h�L�������g���
	m_bOriginalModifiedFlag = m_pobjEditorDoc->IsModified();
	m_objOriginalDocTitle   = m_pobjEditorDoc->GetTitle();


	//////////////////////////////////
	//	�h���b�v�f�[�^���ǂݏo��	//
	//////////////////////////////////

	CFile* pobjFile = pobjOleDataObject->GetFileData(CF_TEXT);

	//////////////////////////////////
	//	�h���b�v�ʒu�ւ̑}������	//
	//////////////////////////////////

	//�h���b�v�ʒu����s���܂ł̕������ޔ����Ă���
	CLine	objEnd;	
	CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lStartLine);
	if( (pobjLine != NULL) && (m_lStartOffset < pobjLine->GetLength()) )
	{
		objEnd.Set(pobjLine->Data(m_lStartOffset));
		pobjLine->SetAt(m_lStartOffset,0);
		m_pobjEditorDoc->SetOneLine(m_lStartLine,pobjLine);
	}

	//�h���b�v�f�[�^�ǂݏo��
	CArchive ar( pobjFile, CArchive::load );
	pobjLine = NULL;
	long lCnt;
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
			m_objDropLineArray.Add((CObject*)pobjLine);

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
			pobjLine = (CLine*)m_objDropLineArray.GetAt(m_objDropLineArray.GetSize()-1);
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
			m_objDropLineArray.Add((CObject*)pobjLine);

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

	//�h���b�v�ʒu����s���܂ł̕����񂪑ޔ�����Ă����ꍇ
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

	//�h���b�v�f�[�^�N���[�Y
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
		pView->MySetCaretToLineOffset(m_lEndLine,m_lEndOffset);

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
void CUndoItemOleDrop::doUndo()
{
	//////////////////////////////////////////////////
	//	�h���b�v�J�n�ʒu�`�I���ʒu�̕�����؂���	//
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
void CUndoItemOleDrop::doRedo()
{
	//////////////////////////////////
	//	�h���b�v�ʒu�ւ̑}������	//
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
	CLine* pobjDropLine = NULL;
	ULONG lCnt=0;
	for(lCnt=0; lCnt<(ULONG)m_objDropLineArray.GetSize(); lCnt++)
	{
		pobjDropLine = (CLine*)m_objDropLineArray.GetAt(lCnt);

		if( lCnt==0 ) //����t���A�P���
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
		else //����t���A�Q��ڈȍ~
		{
			pobjLine = new CLine(pobjDropLine->String());
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
		pView->MySetCaretPos(m_objOriginalCaretPos);

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
BOOL CUndoItemOleDrop::IsCanUndo()
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
BOOL CUndoItemOleDrop::IsCanRedo()
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
void CUndoItemOleDrop::setCanUndo
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
void CUndoItemOleDrop::setCanRedo
	(	BOOL	bIsCanRedo
	)
{
	m_bIsCanRedo = bIsCanRedo;
}



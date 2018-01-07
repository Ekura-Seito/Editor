// UndoItemKatakanaHan2Zen.cpp: CUndoItemKatakanaHan2Zen �N���X�̃C���v�������e�[�V����
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
#include "CodeConvManager.h"
#include "UndoItem.h"
#include "UndoItemKatakanaHan2Zen.h"

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
CUndoItemKatakanaHan2Zen::CUndoItemKatakanaHan2Zen()
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = NULL;

	m_lfWidth = 0;
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
CUndoItemKatakanaHan2Zen::CUndoItemKatakanaHan2Zen
	(	CEditorDoc*		pobjEditorDoc
	)
{
	m_bIsCanUndo = TRUE;
	m_bIsCanRedo = TRUE;

	m_pobjEditorDoc = pobjEditorDoc;

	m_lfWidth = 0;
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
CUndoItemKatakanaHan2Zen::~CUndoItemKatakanaHan2Zen()
{
	if( m_ppobjSelectLine != NULL )
	{
		for(long lCnt=0; lCnt<m_lSelectLineCnt; lCnt++)
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
void CUndoItemKatakanaHan2Zen::Do()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s																	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemKatakanaHan2Zen::Do 
	(	long	lSelectStartLine,		// �I���J�n�s�ԍ�
		long	lSelectStartOffset,		// �I���J�n��ԍ�
		long	lSelectEndLine,			// �I���I���s�ԍ�
		long	lSelectEndOffset,		// �I���I����ԍ�
		CPoint&	objCaretPos,			// �L�����b�g�ʒu
		CPoint& objScrollPosLog,		// �X�N���[���ʒu
		BOOL	bDispRuler,				// ���[���\���t���O
		BOOL	bDispLineNo,			// �s�ԍ��\���t���O
		long	lfWidth					// �t�H���g��
	)
{
	CCodeConvManager	objCodeConvManager;
	long lEndOffsetAfter=0;


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
	m_lfWidth				  = lfWidth;

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
		CLine* pobjLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine);
		if( pobjLine != NULL )
		{
			lEndOffsetAfter = lSelectEndOffset;
			for(long nCnt=lSelectStartOffset; nCnt<lEndOffsetAfter; )
			{
				if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//�Q�o�C�g�����̑��o�C�g
				{
					nCnt+=2;
					continue;
				}
				else //���p
				{
					if( isHanKana(pobjLine->GetAt(nCnt)) ) //���p�J�^�J�i
					{
						//����
					}
					else
					{
						nCnt++;
						continue;
					}
				}


				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+1)) && // ���� �
					pobjLine->GetAt(nCnt+1) == 0xde
				)
				{
					//���p�J�^�J�i�擾
					UINT nHanKana = pobjLine->GetAt(nCnt);
					//���p�J�^�J�i�i�������j���폜
					unsigned char c1stChar=0, c2ndChar=0;
					for(int i=0; i<2; i++)
						pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
					//�S�p�J�^�J�i�ϊ����}��
					UINT nZenKana = _mbbtombc(nHanKana);
					nZenKana = objCodeConvManager.AddDakuonToKana(nZenKana);
					pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
					nCnt+=2;
				}
				else if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+1)) && // ���� �
						 pobjLine->GetAt(nCnt+1) == 0xdf
				)
				{
					//���p�J�^�J�i�擾
					UINT nHanKana = pobjLine->GetAt(nCnt);
					//���p�J�^�J�i�i�ۂ��j���폜
					unsigned char c1stChar=0, c2ndChar=0;
					for(int i=0; i<2; i++)
						pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
					//�S�p�J�^�J�i�ϊ����}��
					UINT nZenKana = _mbbtombc(nHanKana);
					nZenKana = objCodeConvManager.AddMaruToKana(nZenKana);
					pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
					nCnt+=2;
				}
				else
				{
					//���p�J�^�J�i�擾
					UINT nHanKana = pobjLine->GetAt(nCnt);
					//���p�J�^�J�i���폜
					unsigned char c1stChar=0, c2ndChar=0;
					pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
					//�S�p�J�^�J�i�ϊ����}��
					UINT nZenKana = _mbbtombc(nHanKana);
					pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
					nCnt+=2;
					//�I���I�t�Z�b�g���X�V
					lEndOffsetAfter++;
				}
			}

			//�Đݒ�(1999.03.20 Add for StatusBar Pane ID_INDICATOR_BYTE)
			m_pobjEditorDoc->SetOneLine(lSelectStartLine,pobjLine);
		}
	}
	else //�����s�I��
	{
		for(ULONG lLineCnt=0; lLineCnt<(ULONG)(lSelectEndLine-lSelectStartLine+1); lLineCnt++ )//�s���[�v
		{
			CLine* pobjLine = m_pobjEditorDoc->GetOneLine(lSelectStartLine+lLineCnt);
			if( pobjLine == NULL )
				continue;

			if( lLineCnt == 0 ) //�I���J�n�s
			{
				long lEndOffset = pobjLine->GetLength();
				for(long nCnt=lSelectStartOffset; nCnt<lEndOffset; )
				{
					if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//�Q�o�C�g�����̑��o�C�g
					{
						nCnt+=2;
						continue;
					}
					else //���p
					{
						if( isHanKana(pobjLine->GetAt(nCnt)) ) //���p�J�^�J�i
						{
							//����
						}
						else
						{
							nCnt++;
							continue;
						}
					}


					if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+1)) && // ���� �
						pobjLine->GetAt(nCnt+1) == 0xde
					)
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i�i�������j���폜
						unsigned char c1stChar=0, c2ndChar=0;
						for(int i=0; i<2; i++)
							pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						nZenKana = objCodeConvManager.AddDakuonToKana(nZenKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
					}
					else if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+1)) && // ���� �
							 pobjLine->GetAt(nCnt+1) == 0xdf
					)
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i�i�ۂ��j���폜
						unsigned char c1stChar=0, c2ndChar=0;
						for(int i=0; i<2; i++)
							pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						nZenKana = objCodeConvManager.AddMaruToKana(nZenKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
					}
					else
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i���폜
						unsigned char c1stChar=0, c2ndChar=0;
						pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
						//�I���I�t�Z�b�g���X�V
						lEndOffset++;
					}
				}
			}
			else if( lLineCnt < (ULONG)(lSelectEndLine-lSelectStartLine) ) //�I�𒆊ԍs
			{
				long lEndOffset = pobjLine->GetLength();
				for(long nCnt=0; nCnt<lEndOffset; )
				{
					if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//�Q�o�C�g�����̑��o�C�g
					{
						nCnt+=2;
						continue;
					}
					else //���p
					{
						if( isHanKana(pobjLine->GetAt(nCnt)) ) //���p�J�^�J�i
						{
							//����
						}
						else
						{
							nCnt++;
							continue;
						}
					}


					if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+1)) && // ���� �
						pobjLine->GetAt(nCnt+1) == 0xde
					)
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i�i�������j���폜
						unsigned char c1stChar=0, c2ndChar=0;
						for(int i=0; i<2; i++)
							pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						nZenKana = objCodeConvManager.AddDakuonToKana(nZenKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
					}
					else if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+1)) && // ���� �
							 pobjLine->GetAt(nCnt+1) == 0xdf
					)
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i�i�ۂ��j���폜
						unsigned char c1stChar=0, c2ndChar=0;
						for(int i=0; i<2; i++)
							pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						nZenKana = objCodeConvManager.AddMaruToKana(nZenKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
					}
					else
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i���폜
						unsigned char c1stChar=0, c2ndChar=0;
						pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
						//�I���I�t�Z�b�g���X�V
						lEndOffset++;
					}
				}
			}
			else if( lLineCnt == (ULONG)(lSelectEndLine-lSelectStartLine) )	//�I���I���s
			{
				lEndOffsetAfter = lSelectEndOffset;
				for(long nCnt=0; nCnt<lEndOffsetAfter; )
				{
					if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//�Q�o�C�g�����̑��o�C�g
					{
						nCnt+=2;
						continue;
					}
					else //���p
					{
						if( isHanKana(pobjLine->GetAt(nCnt)) ) //���p�J�^�J�i
						{
							//����
						}
						else
						{
							nCnt++;
							continue;
						}
					}


					if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+1)) && // ���� �
						pobjLine->GetAt(nCnt+1) == 0xde
					)
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i�i�������j���폜
						unsigned char c1stChar=0, c2ndChar=0;
						for(int i=0; i<2; i++)
							pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						nZenKana = objCodeConvManager.AddDakuonToKana(nZenKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
					}
					else if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+1)) && // ���� �
							 pobjLine->GetAt(nCnt+1) == 0xdf
					)
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i�i�ۂ��j���폜
						unsigned char c1stChar=0, c2ndChar=0;
						for(int i=0; i<2; i++)
							pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						nZenKana = objCodeConvManager.AddMaruToKana(nZenKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
					}
					else
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i���폜
						unsigned char c1stChar=0, c2ndChar=0;
						pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
						//�I���I�t�Z�b�g���X�V
						lEndOffsetAfter++;
					}
				}
			}

			//�Đݒ�(1999.03.20 Add for StatusBar Pane ID_INDICATOR_BYTE)
			m_pobjEditorDoc->SetOneLine(lSelectStartLine+lLineCnt,pobjLine);
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

		//�L�����b�g�ʒu��ݒ�
		pView->MySetCaretToLineOffset(m_lSelectEndLine,lEndOffsetAfter);

		//�I��͈͍X�V
		pView->m_lSelectEndOffset = lEndOffsetAfter;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FUndo ���s																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CUndoItemKatakanaHan2Zen::doUndo()
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

		//�͈͑I�𕜌�
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
void CUndoItemKatakanaHan2Zen::doRedo()
{
	CCodeConvManager	objCodeConvManager;
	long lEndOffsetAfter=0;


	///////////////////////////////
	//	�������s
	///////////////////////////////

	if( m_lSelectStartLine == m_lSelectEndLine ) //�P�s�I��
	{
		CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine);
		if( pobjLine != NULL )
		{
			lEndOffsetAfter = m_lSelectEndOffset;
			for(long nCnt=m_lSelectStartOffset; nCnt<lEndOffsetAfter; )
			{
				if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//�Q�o�C�g�����̑��o�C�g
				{
					nCnt+=2;
					continue;
				}
				else //���p
				{
					if( isHanKana(pobjLine->GetAt(nCnt)) ) //���p�J�^�J�i
					{
						//����
					}
					else
					{
						nCnt++;
						continue;
					}
				}


				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+1)) && // ���� �
					pobjLine->GetAt(nCnt+1) == 0xde
				)
				{
					//���p�J�^�J�i�擾
					UINT nHanKana = pobjLine->GetAt(nCnt);
					//���p�J�^�J�i�i�������j���폜
					unsigned char c1stChar=0, c2ndChar=0;
					for(int i=0; i<2; i++)
						pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
					//�S�p�J�^�J�i�ϊ����}��
					UINT nZenKana = _mbbtombc(nHanKana);
					nZenKana = objCodeConvManager.AddDakuonToKana(nZenKana);
					pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
					nCnt+=2;
				}
				else if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+1)) && // ���� �
						 pobjLine->GetAt(nCnt+1) == 0xdf
				)
				{
					//���p�J�^�J�i�擾
					UINT nHanKana = pobjLine->GetAt(nCnt);
					//���p�J�^�J�i�i�ۂ��j���폜
					unsigned char c1stChar=0, c2ndChar=0;
					for(int i=0; i<2; i++)
						pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
					//�S�p�J�^�J�i�ϊ����}��
					UINT nZenKana = _mbbtombc(nHanKana);
					nZenKana = objCodeConvManager.AddMaruToKana(nZenKana);
					pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
					nCnt+=2;
				}
				else
				{
					//���p�J�^�J�i�擾
					UINT nHanKana = pobjLine->GetAt(nCnt);
					//���p�J�^�J�i���폜
					unsigned char c1stChar=0, c2ndChar=0;
					pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
					//�S�p�J�^�J�i�ϊ����}��
					UINT nZenKana = _mbbtombc(nHanKana);
					pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
					nCnt+=2;
					//�I���I�t�Z�b�g���X�V
					lEndOffsetAfter++;
				}
			}

			//�Đݒ�(1999.03.20 Add for StatusBar Pane ID_INDICATOR_BYTE)
			m_pobjEditorDoc->SetOneLine(m_lSelectStartLine,pobjLine);
		}
	}
	else //�����s�I��
	{
		for(ULONG lLineCnt=0; lLineCnt<(ULONG)(m_lSelectEndLine-m_lSelectStartLine+1); lLineCnt++ )//�s���[�v
		{
			CLine* pobjLine = m_pobjEditorDoc->GetOneLine(m_lSelectStartLine+lLineCnt);
			if( pobjLine == NULL )
				continue;

			if( lLineCnt == 0 ) //�I���J�n�s
			{
				long lEndOffset = pobjLine->GetLength();
				for(long nCnt=m_lSelectStartOffset; nCnt<lEndOffset; )
				{
					if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//�Q�o�C�g�����̑��o�C�g
					{
						nCnt+=2;
						continue;
					}
					else //���p
					{
						if( isHanKana(pobjLine->GetAt(nCnt)) ) //���p�J�^�J�i
						{
							//����
						}
						else
						{
							nCnt++;
							continue;
						}
					}


					if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+1)) && // ���� �
						pobjLine->GetAt(nCnt+1) == 0xde
					)
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i�i�������j���폜
						unsigned char c1stChar=0, c2ndChar=0;
						for(int i=0; i<2; i++)
							pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						nZenKana = objCodeConvManager.AddDakuonToKana(nZenKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
					}
					else if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+1)) && // ���� �
							 pobjLine->GetAt(nCnt+1) == 0xdf
					)
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i�i�ۂ��j���폜
						unsigned char c1stChar=0, c2ndChar=0;
						for(int i=0; i<2; i++)
							pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						nZenKana = objCodeConvManager.AddMaruToKana(nZenKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
					}
					else
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i���폜
						unsigned char c1stChar=0, c2ndChar=0;
						pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
						//�I���I�t�Z�b�g���X�V
						lEndOffset++;
					}
				}
			}
			else if( lLineCnt < (ULONG)(m_lSelectEndLine-m_lSelectStartLine) ) //�I�𒆊ԍs
			{
				long lEndOffset = pobjLine->GetLength();
				for(long nCnt=0; nCnt<lEndOffset; )
				{
					if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//�Q�o�C�g�����̑��o�C�g
					{
						nCnt+=2;
						continue;
					}
					else //���p
					{
						if( isHanKana(pobjLine->GetAt(nCnt)) ) //���p�J�^�J�i
						{
							//����
						}
						else
						{
							nCnt++;
							continue;
						}
					}


					if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+1)) && // ���� �
						pobjLine->GetAt(nCnt+1) == 0xde
					)
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i�i�������j���폜
						unsigned char c1stChar=0, c2ndChar=0;
						for(int i=0; i<2; i++)
							pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						nZenKana = objCodeConvManager.AddDakuonToKana(nZenKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
					}
					else if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+1)) && // ���� �
							 pobjLine->GetAt(nCnt+1) == 0xdf
					)
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i�i�ۂ��j���폜
						unsigned char c1stChar=0, c2ndChar=0;
						for(int i=0; i<2; i++)
							pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						nZenKana = objCodeConvManager.AddMaruToKana(nZenKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
					}
					else
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i���폜
						unsigned char c1stChar=0, c2ndChar=0;
						pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
						//�I���I�t�Z�b�g���X�V
						lEndOffset++;
					}
				}
			}
			else if( lLineCnt == (ULONG)(m_lSelectEndLine-m_lSelectStartLine) )	//�I���I���s
			{
				lEndOffsetAfter = m_lSelectEndOffset;
				for(long nCnt=0; nCnt<lEndOffsetAfter; )
				{
					if(IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt)))//�Q�o�C�g�����̑��o�C�g
					{
						nCnt+=2;
						continue;
					}
					else //���p
					{
						if( isHanKana(pobjLine->GetAt(nCnt)) ) //���p�J�^�J�i
						{
							//����
						}
						else
						{
							nCnt++;
							continue;
						}
					}


					if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+1)) && // ���� �
						pobjLine->GetAt(nCnt+1) == 0xde
					)
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i�i�������j���폜
						unsigned char c1stChar=0, c2ndChar=0;
						for(int i=0; i<2; i++)
							pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						nZenKana = objCodeConvManager.AddDakuonToKana(nZenKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
					}
					else if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(nCnt+1)) && // ���� �
							 pobjLine->GetAt(nCnt+1) == 0xdf
					)
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i�i�ۂ��j���폜
						unsigned char c1stChar=0, c2ndChar=0;
						for(int i=0; i<2; i++)
							pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						nZenKana = objCodeConvManager.AddMaruToKana(nZenKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
					}
					else
					{
						//���p�J�^�J�i�擾
						UINT nHanKana = pobjLine->GetAt(nCnt);
						//���p�J�^�J�i���폜
						unsigned char c1stChar=0, c2ndChar=0;
						pobjLine->DeleteChar(nCnt,&c1stChar,&c2ndChar);
						//�S�p�J�^�J�i�ϊ����}��
						UINT nZenKana = _mbbtombc(nHanKana);
						pobjLine->InsertChar(nCnt,(((nZenKana>>8)&0x00ff)),(nZenKana&0x00ff));
						nCnt+=2;
						//�I���I�t�Z�b�g���X�V
						lEndOffsetAfter++;
					}
				}
			}

			//�Đݒ�(1999.03.20 Add for StatusBar Pane ID_INDICATOR_BYTE)
			m_pobjEditorDoc->SetOneLine(m_lSelectStartLine+lLineCnt,pobjLine);
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
//		pView->MySetCaretToLineOffset(m_lSelectStartLine,m_lSelectStartOffset);
//		pView->MySetCaretPos(m_objOriginalCaretPos);
		//�L�����b�g�ʒu��ݒ�
		pView->MySetCaretToLineOffset(m_lSelectEndLine,lEndOffsetAfter);

		//�I��͈͍Đݒ�
		pView->m_lSelectStartLine   = m_lSelectStartLine;
		pView->m_lSelectStartOffset = m_lSelectStartOffset;
		pView->m_lSelectEndLine     = m_lSelectEndLine;
		pView->m_lSelectEndOffset   = lEndOffsetAfter;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FUndo ���s�\����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CUndoItemKatakanaHan2Zen::IsCanUndo()
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
BOOL CUndoItemKatakanaHan2Zen::IsCanRedo()
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
void CUndoItemKatakanaHan2Zen::setCanUndo
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
void CUndoItemKatakanaHan2Zen::setCanRedo
	(	BOOL	bIsCanRedo
	)
{
	m_bIsCanRedo = bIsCanRedo;
}


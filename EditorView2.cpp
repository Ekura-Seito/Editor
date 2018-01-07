// EditorView2.cpp : CEditorView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include <Afxtempl.h>			//�l�e�b�̂b�l�������g�p����
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include <afxadv.h>				//for CSharedFile
#include <mbstring.h>			//for OnLButtonDblClk
#include <io.h>					//for _access
#include "Editor_Def.h"
#include "StatusBarDisp.h"		//�X�e�[�^�X�o�[�\�����\����
#include "ColorManager.h"
#include "CodeConvManager.h"
#include "BitmapClient.h"
#include "Operation.h"
#include "OperationManager.h"
#include "MainFrm.h"
#include "Editor.h"
#include "Line.h"
#include "LineManager.h"
#include "UndoItem.h"
#include "UndoManager.h"
#include "EditorDoc.h"
#include "EditorView.h"
#include "LineJumpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorView �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�͈͑I�𒆃`�F�b�N����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::IsSelected()
{
	if( (m_lSelectStartLine   != -1) && (m_lSelectEndLine   != -1) &&
		(m_lSelectStartOffset != -1) && (m_lSelectEndOffset != -1)
	)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I���s���擾����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorView::GetSelectLineCount()
{
	if(!IsSelected())
		return 0;
	if(m_lSelectEndLine > m_lSelectStartLine)
		return (m_lSelectEndLine - m_lSelectStartLine + 1);
	else if(m_lSelectEndLine < m_lSelectStartLine)
		return (m_lSelectStartLine - m_lSelectEndLine + 1);
	else
		return 1;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I��͈͉�������														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::ClearSelectArea()
{
	//�͈͑I�𒆂ł���Ή�������
	if( (m_lSelectStartLine != -1) || (m_lSelectStartOffset != -1) ||
		(m_lSelectEndLine   != -1) || (m_lSelectEndOffset   != -1)
	)
	{
		//�I��͈͉���
		m_lSelectStartLine   = -1;
		m_lSelectStartOffset = -1;
		m_lSelectEndLine     = -1;
		m_lSelectEndOffset   = -1;

		//�ĕ`��
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);
	}
	m_bIsShiftKeyDown=FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�������s����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::DoFind					//�s�q�t�d�F�����I���A�e�`�k�r�d�F�������I��
	(	CString&	objFindStr,				//�����Ώە�����
		BOOL		bIsMatchCase,			//�啶���A����������ʂ��邩�H�i�s�q�t�d�F����A�e�`�k�r�d�F���Ȃ��j
		BOOL		bIsMatchWholeWord,		//�P��S�̂���v������́H�i�s�q�t�d�F�S�́A�e�`�k�r�d�F�ꕔ�j
		BOOL		bIsSearchDown,			//���������ւ̌����H�i�s�q�t�d�F���������A�e�`�k�r�d�F���������j
		BOOL		bIsUseRegularExpression	//���K�\�����g�����H
	)
{
	long lMatchLength = 0;
	long lMatchStartOffset = -1;

	//////////////////////////////////////////////
	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾	//
	//////////////////////////////////////////////

	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	long lMaxLine = pDoc->GetLineCount();
	long lStartLine = 0;
	long lStartOffset = 0;
	if( CaretToLineOffset(&lStartLine,&lStartOffset) == FALSE )
		return FALSE;

	//�͈͑I�𒆂ł���Ή�������
	ClearSelectArea();

	//�����Ώە����񂪑S�ĉp�������琬�邩�`�F�b�N
	BOOL bAllAlphaNum = TRUE;
	for(long lCnt=0; lCnt<objFindStr.GetLength(); lCnt++)
	{
		if( IsDBCSLeadByteEx(0,objFindStr.GetAt(lCnt)) )
		{
			bAllAlphaNum = FALSE;
			break;
		}
		else if( (!isAlnum(objFindStr.GetAt(lCnt))) &&
				 (objFindStr.GetAt(lCnt) != '_')
		)
		{
			bAllAlphaNum = FALSE;
			break;
		}
	}

	//////////////////////
	//	�������[�v		//
	//////////////////////
	long lCurrentLine   = lStartLine;
	long lCurrentOffset = lStartOffset;
	for(;;)
	{
		//�P�s�擾
		CLine* pobjLine = pDoc->GetOneLine( lCurrentLine );
		if( lCurrentLine != lStartLine )
		{
			if( bIsSearchDown ) //������������
				lCurrentOffset = 0;
			else //������������
				lCurrentOffset = pobjLine->GetLength();
		}
		lMatchStartOffset = -1;
		lMatchLength = 0;
		if( pobjLine != NULL )
		{
			//����
			if( bIsSearchDown ) //������������
			{
				lMatchStartOffset = pobjLine->DoFind( lCurrentOffset,		//�����J�n�I�t�Z�b�g
													  pobjLine->GetLength(),//�����I���I�t�Z�b�g
													  objFindStr,			//�����Ώە�����
													  bAllAlphaNum,			//�����Ώە����񂪂��ׂĉp�����ł��邩�H
													  bIsMatchCase,			//�啶���A����������ʂ��邩�H
													  bIsMatchWholeWord,	//�P��S�̂���v������́H
													  bIsUseRegularExpression,
													  &lMatchLength );
			}
			else //������������
			{
				lMatchStartOffset = pobjLine->DoReverseFind( 0,				//�����J�n�I�t�Z�b�g
													  lCurrentOffset,		//�����I���I�t�Z�b�g
													  objFindStr,			//�����Ώە�����
													  bAllAlphaNum,			//�����Ώە����񂪂��ׂĉp�����ł��邩�H
													  bIsMatchCase,			//�啶���A����������ʂ��邩�H
													  bIsMatchWholeWord,	//�P��S�̂���v������́H
													  bIsUseRegularExpression,
													  &lMatchLength );
			}
		}
		if( lMatchStartOffset != -1 ) //��������
		{
			//�I��͈͐ݒ�
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lMatchStartOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lMatchStartOffset + lMatchLength;

			//�����ʒu�փW�����v
			if( bIsSearchDown ) //������������
			{
				Jump2LineOffsetEx(m_lSelectEndLine,m_lSelectEndOffset);
			}
			else //������������
			{
				Jump2LineOffsetEx(m_lSelectStartLine,m_lSelectStartOffset);
			}
			//�ĕ`��
			OnUpdate(NULL,NULL,NULL);
			Invalidate(FALSE);

			return TRUE;
		}
		else //������Ȃ�����
		{
			//�����ʒu�̍X�V
			if( bIsSearchDown ) //������������
			{
				if( (lCurrentLine+1) >= lMaxLine ) //�ŏI�s���B
					break;
				else
					lCurrentLine++;
			}
			else if( bIsSearchDown == FALSE ) //������������
			{
				if( lCurrentLine == 0 ) //�擪�s���B
					break;
				else
					lCurrentLine--;
			}
		}
	}

	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�C���N�������^���������s����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::DoIncrimentalSearch		//�s�q�t�d�F�����I���A�e�`�k�r�d�F�������I��
	(	long		lStartLine,				// �����J�n�s
		long		lStartOffset,			// �����J�n�I�t�Z�b�g
		CString&	objFindStr,				// ����������
		BOOL		bIsSearchDown			// ���������ւ̌����H�i�s�q�t�d�F���������A�e�`�k�r�d�F���������j
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	long lMaxLine = pDoc->GetLineCount();
	BOOL		bIsMatchCase=TRUE;			// �啶���A����������ʂ��邩�H�i�s�q�t�d�F����A�e�`�k�r�d�F���Ȃ��j
	BOOL		bIsMatchWholeWord=FALSE;	// �P��S�̂���v������́H�i�s�q�t�d�F�S�́A�e�`�k�r�d�F�ꕔ�j
	long	lMatchLength=0;
	long	lMatchStartOffset=-1;


	//�͈͑I�𒆂ł���Ή�������
	ClearSelectArea();

	//�����Ώە����񂪑S�ĉp�������琬�邩�`�F�b�N
	BOOL bAllAlphaNum = TRUE;
	for(long lCnt=0; lCnt<objFindStr.GetLength(); lCnt++)
	{
		if( IsDBCSLeadByteEx(0,objFindStr.GetAt(lCnt)) )
		{
			bAllAlphaNum = FALSE;
			break;
		}
		else if( (!isAlnum(objFindStr.GetAt(lCnt))) &&
				 (objFindStr.GetAt(lCnt) != '_')
		)
		{
			bAllAlphaNum = FALSE;
			break;
		}
	}

	//////////////////////
	//	�������[�v		//
	//////////////////////
	long lCurrentLine   = lStartLine;
	long lCurrentOffset = lStartOffset;
	for(;;)
	{
		//�P�s�擾
		CLine* pobjLine = pDoc->GetOneLine( lCurrentLine );
		if( lCurrentLine != lStartLine )
		{
			if( bIsSearchDown ) //������������
				lCurrentOffset = 0;
			else //������������
				lCurrentOffset = pobjLine->GetLength();
		}
		lMatchStartOffset = -1;
		lMatchLength = 0;
		if( pobjLine != NULL )
		{
			//����
			if( bIsSearchDown ) //������������
			{
				lMatchStartOffset = pobjLine->DoFind( lCurrentOffset,		//�����J�n�I�t�Z�b�g
													  pobjLine->GetLength(),//�����I���I�t�Z�b�g
													  objFindStr,			//�����Ώە�����
													  bAllAlphaNum,			//�����Ώە����񂪂��ׂĉp�����ł��邩�H
													  bIsMatchCase,			//�啶���A����������ʂ��邩�H
													  bIsMatchWholeWord,	//�P��S�̂���v������́H
													  FALSE,
													  &lMatchLength);
			}
			else //������������
			{
				lMatchStartOffset = pobjLine->DoReverseFind( 0,				//�����J�n�I�t�Z�b�g
													  lCurrentOffset,		//�����I���I�t�Z�b�g
													  objFindStr,			//�����Ώە�����
													  bAllAlphaNum,			//�����Ώە����񂪂��ׂĉp�����ł��邩�H
													  bIsMatchCase,			//�啶���A����������ʂ��邩�H
													  bIsMatchWholeWord,	//�P��S�̂���v������́H
													  FALSE,
													  &lMatchLength);
			}
		}
		if( lMatchStartOffset != -1 ) //��������
		{
			//�I��͈͐ݒ�
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lMatchStartOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lMatchStartOffset + objFindStr.GetLength();

			//�����ʒu�փW�����v
			if( bIsSearchDown ) //������������
			{
				Jump2LineOffsetEx(m_lSelectEndLine,m_lSelectEndOffset);
			}
			else //������������
			{
				Jump2LineOffsetEx(m_lSelectStartLine,m_lSelectStartOffset);
			}
			//�ĕ`��
			OnUpdate(NULL,NULL,NULL);
			Invalidate(FALSE);

			return TRUE;
		}
		else //������Ȃ�����
		{
			//�����ʒu�̍X�V
			if( bIsSearchDown ) //������������
			{
				if( (lCurrentLine+1) >= lMaxLine ) //�ŏI�s���B
					break;
				else
					lCurrentLine++;
			}
			else if( bIsSearchDown == FALSE ) //������������
			{
				if( lCurrentLine == 0 ) //�擪�s���B
					break;
				else
					lCurrentLine--;
			}
		}
	}

	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�����s����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::DoReplace
	(	CString&	objReplaceStr			//�u���Ώە�����
	)
{
	if( !IsSelected() )
		return;

	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//�͈͑I���ʒu����
	AdjustSelectArea();

	//�u������
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessReplaceSelectWord(
						  objReplaceStr,		//�u���㕶����
						  lCurrentLine,			//�L�����b�g�ʒu�̍s
						  lCurrentOffset,		//�L�����b�g�ʒu�̃I�t�Z�b�g
						  m_lSelectStartLine,	//�I���J�n�s
						  m_lSelectStartOffset,	//�I���J�n�I�t�Z�b�g
						  m_lSelectEndLine,		//�I���I���s
						  m_lSelectEndOffset,	//�I���I���I�t�Z�b�g
						  m_caretPos,			//�L�����b�g�ʒu
						  objScrollPosLog,		//�X�N���[���ʒu
						  m_bDispRuler,
						  m_bDispLineNo );

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�\���t�H���g�̂k�n�f�e�n�m�s�\���̎擾����							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetLogfontOfDisplay
	(	LOGFONT*	pstLogFont
	)
{
	if( m_objDispFont.GetLogFont(pstLogFont) != 0 )
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�󎚃t�H���g�̂k�n�f�e�n�m�s�\���̎擾����							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetLogfontOfPrint
	(	LOGFONT*	pstLogFont
	)
{
	//������
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�\���t�H���g�̃|�C���g�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorView::GetDispFontPoint()
{
	return m_lDispFontPoint;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�\���t�H���g�̃|�C���g�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetDispFontPoint(long lDispFontPoint)
{
	if(m_lDispFontPoint == lDispFontPoint)
		return;

	//�V�����|�C���g��ێ�
	m_lDispFontPoint = lDispFontPoint;

	//�\���t�H���g�쐬���Ȃ���
	m_objDispFont.DeleteObject();
	m_objDispFontUnderLine.DeleteObject();

	CClientDC	dc(this);
	CPoint ptVpOrg(0, 0);
	ptVpOrg.x = -1 * GetScrollPosLogX();
	dc.SetViewportOrg(ptVpOrg);

	//�\���t�H���g�쐬
	m_objDispFont.CreatePointFont(m_lDispFontPoint,m_objDispFontName,&dc);
	//�����t���\���t�H���g�쐬
	LOGFONT logfont;
	memset(&logfont,0,sizeof(LOGFONT));
	m_objDispFont.GetLogFont(&logfont);
	logfont.lfUnderline = TRUE;
	m_objDispFontUnderLine.CreateFontIndirect(&logfont);

	CFont* oldFont = dc.SelectObject(&m_objDispFont);
	TEXTMETRIC	tm;
	dc.GetTextMetrics(&tm);
	m_lfHeight = tm.tmHeight + tm.tmExternalLeading;
	m_lfWidth  = tm.tmAveCharWidth + tm.tmOverhang;
	dc.SelectObject(oldFont);

	//��x�L�����b�g����(1999.03.16 Add)
	if(m_bDispUnderLine)
		MyEraseUnderLine(&dc);
	HideCaret();

	//�s�ԍ��\���֘A������
	if( m_bDispLineNo )
	{
		m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		m_LineNoPos = CPoint(m_lLineNoWidth,0);
	}
	else
	{
		m_lLineNoWidth = 0;
		m_LineNoPos = CPoint(0,0);
	}

	//���[���\���֘A������
	if( m_bDispRuler )
		m_RulerPos = CPoint(0,(m_lfHeight+m_lLineInterval));
	else
		m_RulerPos = CPoint(0,0);

	//�L�����b�g�ʒu������
	if(m_bInsertKey)
		CreateSolidCaret(m_lfWidth/4, m_lfHeight); //�}���L�����b�g�쐬
	else
		CreateSolidCaret(m_lfWidth, m_lfHeight); //�㏑���L�����b�g�쐬
	ShowCaret();					//�L�����b�g�\��
	MySetCaretPos(CPoint(m_LineNoPos.x,m_RulerPos.y));		//�L�����b�g�ʒu�ݒ�

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�\���t�H���g���擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CString CEditorView::GetDispFontName()
{
	return m_objDispFontName;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�\���t�H���g���ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetDispFontName(CString& objDispFontName)
{
	if(!m_objDispFontName.CompareNoCase(objDispFontName))
		return;

	//�V�����t�H���g���ێ�
	m_objDispFontName = objDispFontName;

	//�\���t�H���g�쐬���Ȃ���
	m_objDispFont.DeleteObject();
	m_objDispFontUnderLine.DeleteObject();

	CClientDC	dc(this);
	CPoint ptVpOrg(0, 0);
	ptVpOrg.x = -1 * GetScrollPosLogX();
	dc.SetViewportOrg(ptVpOrg);

	//�\���t�H���g�쐬
	m_objDispFont.CreatePointFont(m_lDispFontPoint,m_objDispFontName,&dc);
	//�����t���\���t�H���g�쐬
	LOGFONT logfont;
	memset(&logfont,0,sizeof(LOGFONT));
	m_objDispFont.GetLogFont(&logfont);
	logfont.lfUnderline = TRUE;
	m_objDispFontUnderLine.CreateFontIndirect(&logfont);
	
	CFont* oldFont = dc.SelectObject(&m_objDispFont);
	TEXTMETRIC	tm;
	dc.GetTextMetrics(&tm);
	m_lfHeight = tm.tmHeight + tm.tmExternalLeading;
	m_lfWidth  = tm.tmAveCharWidth + tm.tmOverhang;
	dc.SelectObject(oldFont);

	//��x�L�����b�g����(1999.03.16 Add)
	if(m_bDispUnderLine)
		MyEraseUnderLine(&dc);
	HideCaret();

	//�s�ԍ��\���֘A������
	if( m_bDispLineNo )
	{
		m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		m_LineNoPos = CPoint(m_lLineNoWidth,0);
	}
	else
	{
		m_lLineNoWidth = 0;
		m_LineNoPos = CPoint(0,0);
	}

	//���[���\���֘A������
	if( m_bDispRuler )
		m_RulerPos = CPoint(0,(m_lfHeight+m_lLineInterval));
	else
		m_RulerPos = CPoint(0,0);

	//�L�����b�g�ʒu������
	if(m_bInsertKey)
		CreateSolidCaret(m_lfWidth/4, m_lfHeight); //�}���L�����b�g�쐬
	else
		CreateSolidCaret(m_lfWidth, m_lfHeight); //�㏑���L�����b�g�쐬
	ShowCaret();					//�L�����b�g�\��
	MySetCaretPos(CPoint(m_LineNoPos.x,m_RulerPos.y));		//�L�����b�g�ʒu�ݒ�

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�󎚃t�H���g�̃|�C���g�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorView::GetPrintFontPoint()
{
	return m_lPrintFontPoint;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�󎚃t�H���g�̃|�C���g�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetPrintFontPoint(long lPrintFontPoint)
{
	//�V�����|�C���g��ێ�
	m_lPrintFontPoint = lPrintFontPoint;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�󎚃t�H���g���擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CString CEditorView::GetPrintFontName()
{
	return m_objPrintFontName;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�󎚃t�H���g���ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetPrintFontName(CString& objPrintFontName)
{
	//�V�����t�H���g���ێ�
	m_objPrintFontName = objPrintFontName;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ʏ�̕����F�擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetFontColor()
{
	return m_FontColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ʏ�̕����F�ݒ菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFontColor(COLORREF color)
{
	m_FontColor = color;

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ʏ�̔w�i�F�擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetBackColor()
{
	return m_BackColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ʏ�̔w�i�F�ݒ菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetBackColor(COLORREF color)
{
	m_BackColor = color;

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���]���̕����F�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetInvertFontColor()
{
	return m_InvertFontColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���]���̕����F�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetInvertFontColor(COLORREF color)
{
	m_InvertFontColor = color;

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���]���̔w�i�F�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetInvertBackColor()
{
	return m_InvertBackColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���]���̔w�i�F�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetInvertBackColor(COLORREF color)
{
	m_InvertBackColor = color;

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s�}�[�N�\���t���O�擾����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetFlagDispCRLF()
{
	return m_bDispCRLF;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s�}�[�N�\���t���O�ݒ菈��											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagDispCRLF
	(	BOOL	bDispCRLF
	)
{
	m_bDispCRLF = bDispCRLF;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s�}�[�N�F�擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetCRLFColor()
{
	return m_CRLFColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s�}�[�N�F�ݒ菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetCRLFColor
	(	COLORREF	color
	)
{
	m_CRLFColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s�ԍ��\���t���O�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetFlagDispLineNo()
{
	return m_bDispLineNo;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s�ԍ��\���t���O�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagDispLineNo
	(	BOOL	bDispLineNo
	)
{
	if( m_bDispLineNo == bDispLineNo ) //�t���O�ɕω��Ȃ�
		return;

	m_bDispLineNo = bDispLineNo;
	if( m_bDispLineNo )
	{
		m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		m_LineNoPos = CPoint(m_lLineNoWidth,0);
		CPoint objPoint = m_caretPos + m_LineNoPos;
		MySetCaretPos(objPoint);
	}
	else
	{
		m_lLineNoWidth = 0;
		if(m_caretPos.x >= m_LineNoPos.x)
		{
			CPoint objPoint = m_caretPos - m_LineNoPos;
			m_LineNoPos = CPoint(0,0);
			MySetCaretPos(objPoint);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s�ԍ��F�擾����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetLineNoColor()
{
	return m_LineNoColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s�ԍ��F�ݒ菈��														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetLineNoColor
	(	COLORREF	color
	)
{
	m_LineNoColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s�ԍ��̔w�i�̐F�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetLineNoBackColor()
{
	return m_LineNoBackColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s�ԍ��̔w�i�̐F�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetLineNoBackColor
	(	COLORREF	color
	)
{
	m_LineNoBackColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[���\���t���O�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetFlagDispRuler()
{
	return m_bDispRuler;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[���\���t���O�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagDispRuler
	(	BOOL	bDispRuler
	)
{
	if( m_bDispRuler == bDispRuler ) //�t���O�ɕω��Ȃ�
		return;

	m_bDispRuler = bDispRuler;
	if( m_bDispRuler )
	{
		m_RulerPos = CPoint(0,(m_lfHeight+m_lLineInterval));
		CPoint objPoint = m_caretPos + m_RulerPos;
		MySetCaretPos(objPoint);
	}
	else
	{
		CPoint objPoint = m_caretPos - m_RulerPos;
		m_RulerPos = CPoint(0,0);
		MySetCaretPos(objPoint);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[���F�擾����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetRulerColor()
{
	return m_RulerColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[���F�ݒ菈��														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetRulerColor
	(	COLORREF	color
	)
{
	m_RulerColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[���̔w�i�̐F�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetRulerBackColor()
{
	return m_RulerBackColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[���̔w�i�̐F�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetRulerBackColor
	(	COLORREF	color
	)
{
	m_RulerBackColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�S�p�X�y�[�X�\���t���O�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetFlagDispZenkakuSpace()
{
	return m_bDispZenkakuSpace;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�S�p�X�y�[�X�\���t���O�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagDispZenkakuSpace
	(	BOOL	bDispZenkakuSpace
	)
{
	m_bDispZenkakuSpace = bDispZenkakuSpace;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�S�p�X�y�[�X�F�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetZenkakuSpaceColor()
{
	return m_ZenkakuSpaceColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�S�p�X�y�[�X�F�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetZenkakuSpaceColor
	(	COLORREF	color
	)
{
	m_ZenkakuSpaceColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�^�u�R�[�h�\���t���O�擾����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetFlagDispTab()
{
	return m_bDispTab;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�^�u�R�[�h�\���t���O�ݒ菈��											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagDispTab
	(	BOOL	bDispTab
	)
{
	m_bDispTab = bDispTab;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�^�u�R�[�h�F�擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetTabColor()
{
	return m_TabColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�^�u�R�[�h�F�ݒ菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetTabColor
	(	COLORREF	color
	)
{
	m_TabColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s�ԍ�����t���O�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetFlagPrintLineNo()
{
	return m_bPrintLineNo;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s�ԍ�����t���O�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagPrintLineNo
	(	BOOL	bPrintLineNo
	)
{
	m_bPrintLineNo = bPrintLineNo;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�Q�i�g�݈���t���O�擾����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetFlagPrint2Dangumi()
{
	return m_bPrint2Dangumi;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�Q�i�g�݈���t���O�ݒ菈��											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagPrint2Dangumi
	(	BOOL	bPrint2Dangumi
	)
{
	m_bPrint2Dangumi = bPrint2Dangumi;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����w�b�_���C�A�E�g�ʒu�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::GetPrintHeaderLayoutPos()
{
	return m_nHeaderLayout;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����w�b�_���C�A�E�g�ʒu�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetPrintHeaderLayoutPos
	(	int	nHeaderLayout
	)
{
	if( nHeaderLayout>=0 && nHeaderLayout<=2 )
	{
		m_nHeaderLayout = nHeaderLayout;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����t�b�^���C�A�E�g�ʒu�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::GetPrintFooterLayoutPos()
{
	return m_nFooterLayout;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����t�b�^���C�A�E�g�ʒu�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetPrintFooterLayoutPos
	(	int	nFooterLayout
	)
{
	if( nFooterLayout>=0 && nFooterLayout<=2 )
	{
		m_nFooterLayout = nFooterLayout;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����w�b�_�t�H�[�}�b�g�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetStrHeaderFormat
	(	CString&	objStr
	)
{
	objStr = m_objStrHeaderFormat;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����w�b�_�t�H�[�}�b�g�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetStrHeaderFormat
	(	CString&	objStr
	)
{
	m_objStrHeaderFormat = objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����t�b�^�t�H�[�}�b�g�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetStrFooterFormat
	(	CString&	objStr
	)
{
	objStr = m_objStrFooterFormat;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����t�b�^�t�H�[�}�b�g�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetStrFooterFormat
	(	CString&	objStr
	)
{
	m_objStrFooterFormat = objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s�Ԏ擾����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorView::GetLineInterval()
{
	return m_lLineInterval;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s�Ԑݒ菈��															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetLineInterval
	(	long	lLineInterval
	)
{
	if(m_lLineInterval == lLineInterval)
		return;
	if(lLineInterval<0 || lLineInterval>2)
		return;

	//�s�Ԑݒ�
	m_lLineInterval = lLineInterval;

	MyScrollToPosition(CPoint(0,0));
	MySetCaretPos(CPoint(m_LineNoPos.x,m_RulerPos.y));

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A���_�[���C���\���t���O�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::GetFlagDispUnderLine()
{
	return m_bDispUnderLine;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A���_�[���C���\���t���O�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagDispUnderLine
	(	BOOL	bDispUnderLine
	)
{
	m_bDispUnderLine = bDispUnderLine;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A���_�[���C���F�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetUnderLineColor()
{
	return m_UnderLineColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A���_�[���C���F�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetUnderLineColor
	(	COLORREF	color
	)
{
	m_UnderLineColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�`��擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::GetCaretType()
{
	return m_nCaretType;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�`��ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetCaretType
	(	int		nCaretType
	)
{
	if(m_nCaretType == nCaretType) //���łɓ���
		return;
	if( (nCaretType!=CARET_IBEAM) && (nCaretType!=CARET_SQUARE) )
		return;

	//�L�����b�g�`��ێ�
	m_nCaretType = nCaretType;

	if(!m_bInsertKey) //�}�����[�h�ł͔��f�����Ȃ�
		return;

	if(m_nCaretType==CARET_IBEAM) //�c�^�L�����b�g
	{
		HideCaret();								//�L�����b�g��\��
		CreateSolidCaret(m_lfWidth/4, m_lfHeight); 	//�c�_�L�����b�g�쐬
		ShowCaret();								//�L�����b�g�\��
		MySetCaretPos(m_caretPos);					//�L�����b�g�ʒu�ݒ�
	}
	else if(m_nCaretType==CARET_SQUARE) //���^�L�����b�g
	{
		HideCaret();								//�L�����b�g��\��
		CreateSolidCaret(m_lfWidth, m_lfHeight/2); 	//���^�L�����b�g�쐬
		ShowCaret();								//�L�����b�g�\��
		MySetCaretPos(m_caretPos);					//�L�����b�g�ʒu�ݒ�
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�_�ŊԊu�ێ�����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::HoldCaretBlinkTime
	(	UINT	unCaretBlinkTime
	)
{
	m_unCaretBlinkTime = unCaretBlinkTime;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�̐U�镑���擾����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::GetCaretBehave()
{
	return m_nCaretBehave;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�̐U�镑���ݒ菈��											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetCaretBehave
	(	int		nCaretBehave
	)
{
	if((nCaretBehave!=CARET_BEHAVE_MOVE)&&(nCaretBehave!=CARET_BEHAVE_STAY))
		return;

	m_nCaretBehave = nCaretBehave;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ړ��^�C�v�擾����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::GetCaretMoveType()
{
	return m_nCaretMoveType;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ړ��^�C�v�ݒ菈��											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetCaretMoveType
	(	int		nCaretMoveType
	)
{
	if(nCaretMoveType != CARET_MOVE_WORDTYPE && nCaretMoveType != CARET_MOVE_NOTEPADTYPE)
		return;

	m_nCaretMoveType = nCaretMoveType;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�d�n�e�}�[�N�\���t���O�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetFlagDispEOFMark
	(	BOOL	bIsDispEOFMark
	)
{
	BOOL bOrg = m_pApp->GetProfileInt("CEditorView","bIsDispEOFMark",1);
	m_pApp->WriteProfileInt("CEditorView","bIsDispEOFMark",bIsDispEOFMark);

	if(bOrg != bIsDispEOFMark)
	{
		//�ĕ`��
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I��͈̓e�L�X�g�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLine* CEditorView::MyGetSelectedText
	(	long*	plSelectLineCnt			// �I��͈͍s���ԋp�p�o�b�t�@�|�C���^
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(!IsSelected())
	{
		(*plSelectLineCnt) = 0;
		return NULL;
	}
	else
	{
		if(m_lSelectStartLine == m_lSelectEndLine)
			(*plSelectLineCnt) = 1;
		else if(m_lSelectStartLine < m_lSelectEndLine)
			(*plSelectLineCnt) = m_lSelectEndLine - m_lSelectStartLine + 1;
		else if(m_lSelectStartLine > m_lSelectEndLine)
			(*plSelectLineCnt) = m_lSelectStartLine - m_lSelectEndLine + 1;

		return pDoc->MyGetSelectedText( m_lSelectStartLine,
										m_lSelectStartOffset,
										m_lSelectEndLine,
										m_lSelectEndOffset );
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_���X�N���[���ʒu�w�擾����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::GetScrollPosLogX()
{
	SCROLLINFO	hinfo;
	memset(&hinfo,0,sizeof(SCROLLINFO));
	CScrollBar* pobjHScrollBar = GetScrollBarCtrl(SB_HORZ);
	pobjHScrollBar->GetScrollInfo(&hinfo,SIF_ALL);
	if( (hinfo.nPos%m_lfWidth) != 0 )
	{
		AdjustHScrollPos(&hinfo);
		pobjHScrollBar->SetScrollInfo(&hinfo,TRUE);
	}
	return hinfo.nPos;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_���X�N���[���ʒu�x�擾����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::GetScrollPosLogY()
{
	SCROLLINFO	vinfo;
	memset(&vinfo,0,sizeof(SCROLLINFO));
	CScrollBar* pobjVScrollBar = GetScrollBarCtrl(SB_VERT);
	pobjVScrollBar->GetScrollInfo(&vinfo,SIF_ALL);
	if( (vinfo.nPos%(m_lfHeight+m_lLineInterval)) != 0 )
	{
		AdjustVScrollPos(&vinfo);
		pobjVScrollBar->SetScrollInfo(&vinfo,TRUE);
	}
	return vinfo.nPos;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����X�N���[���ʒu��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::AdjustHScrollPos
	(	SCROLLINFO*	pInfo
	)
{
	if( (pInfo->nPos%m_lfWidth) == 0)
		return;

	int nAmari = pInfo->nPos%m_lfWidth;
	int nSyo   = pInfo->nPos/m_lfWidth;
	if(nAmari >= m_lfWidth/2)
		pInfo->nPos = (nSyo+1)*m_lfWidth;
	else
		pInfo->nPos = nSyo * m_lfWidth;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����X�N���[���ʒu��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::AdjustVScrollPos
	(	SCROLLINFO*	pInfo
	)
{
	if( (pInfo->nPos%(m_lfHeight+m_lLineInterval)) == 0)
		return;

	int nAmari = pInfo->nPos%(m_lfHeight+m_lLineInterval);
	int nSyo   = pInfo->nPos/(m_lfHeight+m_lLineInterval);
	if(nAmari >= ((m_lfHeight+m_lLineInterval)/2))
		pInfo->nPos = (nSyo+1)*(m_lfHeight+m_lLineInterval);
	else
		pInfo->nPos = nSyo * (m_lfHeight+m_lLineInterval);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�[�N�s�F�擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetMarkLineColor()
{
	return m_MarkLineColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�[�N�s�F�ݒ菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetMarkLineColor
	(	COLORREF	MarkLineColor
	)
{
	m_MarkLineColor = MarkLineColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r�p�A�폜�s�F�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetDiffDelLineColor()
{
	return m_DiffDelLineColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r�p�A�폜�s�F�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetDiffDelLineColor
	(	COLORREF	color
	)
{
	m_DiffDelLineColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r�p�A�ǉ��s�F�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetDiffAddLineColor()
{
	return m_DiffAddLineColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r�p�A�ǉ��s�F�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetDiffAddLineColor
	(	COLORREF	color
	)
{
	m_DiffAddLineColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r�p�A����t�@�C�����F�擾����							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorView::GetDirDiffLineColor()
{
	return m_DirDiffLineColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r�p�A����t�@�C�����F�ݒ菈��							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::SetDirDiffLineColor
	(	COLORREF	color
	)
{
	m_DirDiffLineColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�C���N�������^���T�[�`�J�n�s�A�I�t�Z�b�g�N���A����					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::ClearIncrimentalSearchLineOffset()
{
	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) != FALSE )
	{
		m_lIncrimentalSearchStartLine = lCurrentLine;
		m_lIncrimentalSearchStartOffset = lCurrentOffset;
	}
	else
	{
		m_lIncrimentalSearchStartLine = 0;
		m_lIncrimentalSearchStartOffset = 0;
	}
	m_objIncrimentalSearchLine.Set((const unsigned char*)"");
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�C���N�������^���T�[�`������擾����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetIncrimentalSearchString
	(	CString&	objStr
	)
{
	objStr = m_objIncrimentalSearchLine.String();
}

/////////////////////////////////////////////////////////////////////////////
//	CEditorView �v���e�N�g�֐��i�X�N���[���֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�N���[���T�C�Y�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MySetScrollSizes
	(	int		nMapMode,		// �}�b�s���O���[�h
		SIZE	sizeTotal,		// �g�[�^���T�C�Y
		const SIZE&	sizeVPage,	// �����X�N���[���o�[�V���t�g�N���b�N�ł̃X�N���[���T�C�Y
		const SIZE& sizeVLine,	// �����X�N���[���o�[���N���b�N�ł̃X�N���[���T�C�Y
		const SIZE& sizeHPage,	// �����X�N���[���o�[�V���t�g�N���b�N�ł̃X�N���[���T�C�Y
		const SIZE& sizeHLine	// �����X�N���[���o�[���N���b�N�ł̃X�N���[���T�C�Y
	)
{
	BOOL bRet;

	// �����o�ϐ��֕ێ�
	m_nMapMode = nMapMode;				// �}�b�s���O���[�h
	m_totalLog = sizeTotal;				// �_���P�ʂł̃g�[�^���T�C�Y
	m_VPageLog = sizeVPage;				// �����X�N���[���o�[�̘_���P�ʂł̂P�y�[�W������X�N���[���T�C�Y
	m_VLineLog = sizeVLine;				// �����X�N���[���o�[�̘_���P�ʂł̂P�s������X�N���[���T�C�Y
	m_HPageLog = sizeHPage;				// �����X�N���[���o�[�̘_���P�ʂł̂P�y�[�W������X�N���[���T�C�Y
	m_HLineLog = sizeHLine;				// �����X�N���[���o�[�̘_���P�ʂł̂P�s������X�N���[���T�C�Y

	{
		CWindowDC dc(NULL);
		dc.SetMapMode(m_nMapMode);			// �}�b�s���O���[�h�ݒ�
	}

	// �����X�N���[���o�[�X�V
	SCROLLINFO		vinfo;
	memset(&vinfo,0,sizeof(SCROLLINFO));
	CScrollBar*	pobjVScrollBar = GetScrollBarCtrl(SB_VERT);
	pobjVScrollBar->GetScrollInfo(&vinfo,SIF_ALL);
	vinfo.fMask = SIF_ALL;
	vinfo.nMin  = 0;
	vinfo.nMax  = m_totalLog.cy;
	vinfo.nPage = m_VPageLog.cy;
	if( (vinfo.nPos%(m_lfHeight+m_lLineInterval)) != 0 )
		AdjustVScrollPos(&vinfo);
	bRet = pobjVScrollBar->SetScrollInfo(&vinfo,TRUE);

	// ������۰��ް�X�V
	SCROLLINFO		hinfo;
	memset(&hinfo,0,sizeof(SCROLLINFO));
	CScrollBar*	pobjHScrollBar = GetScrollBarCtrl(SB_HORZ);
	pobjHScrollBar->GetScrollInfo(&hinfo,SIF_ALL);
	hinfo.fMask = SIF_ALL;
	hinfo.nMin  = 0;
	hinfo.nMax  = m_totalLog.cx;
	hinfo.nPage = m_HPageLog.cx;
	if( (hinfo.nPos%m_lfWidth) != 0 )
		AdjustHScrollPos(&hinfo);
	bRet = pobjHScrollBar->SetScrollInfo(&hinfo,TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�N���[���ʒu�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyScrollToPosition
	(	CPoint&	point			//�_�����W�l
	)
{
	if(m_bDispUnderLine) //�A���_�[���C����`�悷��ꍇ
	{
		//�X�N���[������O�ɃA���_�[���C�����������Ă���
		CClientDC dc(this);
		CPoint ptVpOrg(0, 0);
		ptVpOrg.x = -1 * GetScrollPosLogX();
		dc.SetViewportOrg(ptVpOrg);
		MyEraseUnderLine(&dc);
	}

	/////////////////////////////////
	// ������۰��ް�X�V
	/////////////////////////////////
	SCROLLINFO		vinfo;
	memset(&vinfo,0,sizeof(SCROLLINFO));
	CScrollBar*	pobjVScrollBar = GetScrollBarCtrl(SB_VERT);
	pobjVScrollBar->GetScrollInfo(&vinfo,SIF_ALL);
	int yOrig = vinfo.nPos;	//�O��ʒu
	vinfo.nPos = point.y;	//����ʒu
	if(vinfo.nPos<0)
		vinfo.nPos = 0;

	//��۰��ް���݈ʒu�X�V
	if( (vinfo.nPos%(m_lfHeight+m_lLineInterval)) != 0 )
		AdjustVScrollPos(&vinfo);
	pobjVScrollBar->SetScrollInfo(&vinfo,TRUE);

	//////////////////////////////////////
	// ������۰��ް�X�V
	//////////////////////////////////////
	SCROLLINFO		hinfo;
	memset(&hinfo,0,sizeof(SCROLLINFO));
	CScrollBar*	pobjHScrollBar = GetScrollBarCtrl(SB_HORZ);
	pobjHScrollBar->GetScrollInfo(&hinfo,SIF_ALL);
	int xOrig = hinfo.nPos;	//�O��ʒu
	hinfo.nPos = point.x;	//����ʒu
	if(hinfo.nPos<0)
		hinfo.nPos=0;

	//��۰��ް���݈ʒu�X�V
	if( (hinfo.nPos%m_lfWidth) != 0 )
		AdjustHScrollPos(&hinfo);
	pobjHScrollBar->SetScrollInfo(&hinfo,TRUE);

	if( (xOrig!=hinfo.nPos) || (yOrig!=vinfo.nPos) )
	{
#if 1
		if(yOrig != vinfo.nPos)
		{
			int nAmount = -(vinfo.nPos - yOrig);
			MyScrollWindow(2,nAmount);
		}
		if(xOrig != hinfo.nPos)
		{
			int nAmount = -(hinfo.nPos - xOrig);
			MyScrollWindow(1,nAmount);
		}
#else
		//�ĕ`��
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);
#endif
	}

	//��گĈʒu
	MySetCaretPos(m_caretPos);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����޳��۰ُ���														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyScrollWindow
	(	BOOL	bHorzVert,			//��۰ٕ���( 1 : ���������A2 : ���������j
		int		nAmount				//��۰ٗ�
	)
{
	CRect	clientRect, rect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();
	rect = clientRect;

	if( bHorzVert == 1 )// ������۰�
	{
#if 1
		InvalidateRect(&clientRect,FALSE);
		UpdateWindow();
		return;
#endif

		if(nAmount > 0)//����
		{
			clientRect.right = nAmount;
			if( m_bDispLineNo )
			{
				clientRect.left -= m_lLineNoWidth;
				clientRect.right += (m_lLineNoWidth + m_lfWidth*4);
			}
		}
		else //�E��
		{
			clientRect.left = clientRect.right - (clientRect.right % m_lfWidth);
			clientRect.left += nAmount;
		}
		ScrollWindow( nAmount, 0 );
		if(m_pobjInvalidRect)
			delete m_pobjInvalidRect;
		m_pobjInvalidRect = new CRect();
		(*m_pobjInvalidRect) = clientRect;
		m_bUseClipRgn = TRUE; //�N���b�v���ꂽ�̈�̂ݕ`��X�V����
		InvalidateRect(&clientRect,FALSE);
//		if( m_bDispLineNo )
//		{
//			CRect linenoRect(0,0,m_lLineNoWidth,clientRect.bottom);
//			m_bUseClipRgn = TRUE; //�N���b�v���ꂽ�̈�̂ݕ`��X�V����
//			InvalidateRect(&linenoRect,FALSE);
//		}
		//�����ɕ`��
		UpdateWindow();
	}
	else // ������۰�
	{
		if(nAmount > 0)//���
		{
			clientRect.bottom = nAmount;
			if( m_bDispRuler )
			{
				clientRect.top += (m_lfHeight+m_lLineInterval);
				clientRect.bottom += (m_lfHeight+m_lLineInterval);
				//�X�N���[�����Ƀ��[�������������Ȃ��悤�A�X�N���[���̈悩�烋�[���̈������
				rect.top += (m_lfHeight+m_lLineInterval);
			}
		}
		else //����
		{
			clientRect.top = clientRect.bottom - (clientRect.bottom % (m_lfHeight+m_lLineInterval));
			clientRect.top += nAmount;
			if( m_bDispRuler )
			{
				//�X�N���[�����Ƀ��[����������Ȃ��悤�A�X�N���[���̈悩�烋�[���̈������
				int nScrollLineCount = ((-1)*nAmount)/(m_lfHeight+m_lLineInterval);
				rect.top += (m_lfHeight+m_lLineInterval)*(1+nScrollLineCount);
			}
		}
		ScrollWindow( 0, nAmount, rect );
		if(m_pobjInvalidRect)
			delete m_pobjInvalidRect;
		m_pobjInvalidRect = new CRect();
		(*m_pobjInvalidRect) = clientRect;
		m_bUseClipRgn = TRUE; //�N���b�v���ꂽ�̈�̂ݕ`��X�V����
		InvalidateRect(&clientRect,FALSE);
		//�����ɕ`��
		UpdateWindow();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�[�g�X�N���[������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::AutoScroll()
{
	MSG		msg;
	CEditorDoc*	pDoc = (CEditorDoc*)GetDocument();
	CPoint	tempPos(0,0);
	CRect	clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	memset(&msg,0,sizeof(MSG));
	msg.message	= 0;
	for(;;)
	{
		//���b�Z�[�W�|���v
		if(::PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		if(msg.message == WM_LBUTTONUP)
		{
			break;
		}

		//�}�E�X�ʒu�擾
		POINT	stCursorPos;
		::GetCursorPos(&stCursorPos);
		ScreenToClient(&stCursorPos);

		//�I�[�g�X�N���[��
		if(stCursorPos.y < 0) //��
		{
			OnVScroll(SB_LINEUP,NULL,GetScrollBarCtrl(SB_VERT));
			continue;
		}
		else if(stCursorPos.y >	clientRect.Height()) //��
		{
			OnVScroll(SB_LINEDOWN,NULL,GetScrollBarCtrl(SB_VERT));
			continue;
		}
		else if(stCursorPos.x <	0) //��
		{
			OnHScroll(SB_LINEUP,NULL,GetScrollBarCtrl(SB_HORZ));
			continue;
		}
		else if(stCursorPos.x >	clientRect.Width())//�E
		{
			OnHScroll(SB_LINEDOWN,NULL,GetScrollBarCtrl(SB_HORZ));
			continue;
		}

//		if(stCursorPos.x < m_LineNoPos.x)
//			continue;
		if(stCursorPos.y < m_RulerPos.y)
			continue;

		//�}�E�X�ʒu�@�|���@�s�A��ԍ��ϊ�
		long lLineBuf=0;
		long lOffsetBuf=0;
		tempPos.x =	0;
		tempPos.y =	0;
		CPoint objCursorPos(stCursorPos.x,stCursorPos.y);
		long lRet =	MouseToLineOffset( objCursorPos, &lLineBuf,	&lOffsetBuf, &tempPos );
		if(lRet	== 0)
		{
			continue;
		}
		else if(lRet ==	1)
		{
			//�L�����b�g�ʒu�����݂̃}�E�X�ʒu�ɐݒ�
			MySetCaretPos(tempPos);
		}
		else if(lRet ==	2)
		{
			//�L�����b�g���}�E�X�ʒu�s�̍Ō�Ɉʒu�Â�
			CLine* pobjLine	= pDoc->GetOneLine(lLineBuf);
			if(pobjLine	!= NULL)
				lOffsetBuf = pobjLine->GetLength();
			MySetCaretToLineOffset(lLineBuf,lOffsetBuf);
		}

		// �I��͈͂̍ĕ`�揈��
		RedrawSelectArea(m_lPreMoveCaretLine,m_lPreMoveCaretOffset);

		//�Đݒ�
		m_lPreMoveCaretLine	= lLineBuf;
		m_lPreMoveCaretOffset =	lOffsetBuf;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	CEditorView �v���e�N�g�֐��i�`��֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�t�X�N���[���o�b�t�@�쐬����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::CreateOffScreenBuf()
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	//�ő啝�v�Z
	m_nOffBufferWidth = 0;
	if( ((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth) > clientRect.Width() )
		m_nOffBufferWidth = (ULONG)((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth);
	else
		m_nOffBufferWidth = clientRect.Width();

	//�I�t�X�N���[���o�b�t�@�쐬
	CDC* pDC = GetDC();
	m_objOffBufferDC.CreateCompatibleDC(pDC);
	m_objOffBufferBmp.CreateCompatibleBitmap(pDC,m_nOffBufferWidth,clientRect.Height());
	m_objOffBufferDC.SelectObject(&m_objOffBufferBmp);
	ReleaseDC(pDC);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�t�X�N���[���o�b�t�@�j������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::DestroyOffScreenBuf()
{
	//�I�t�X�N���[���o�b�t�@�j��
	m_objOffBufferDC.DeleteDC();
	m_objOffBufferBmp.DeleteObject();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�t�X�N���[���o�b�t�@�̃A�b�v�f�[�g����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::UpdateOffBuffer()
{
	//�w�i�F�u���V�쐬
	CBrush	objBackBrush;
	objBackBrush.CreateSolidBrush(m_BackColor);
	CBrush	objRulerBackBrush;
	objRulerBackBrush.CreateSolidBrush(m_RulerBackColor);

	//�s�ԍ��\���t�B�[���h�̕�
	if( m_bDispLineNo ) //�s�ԍ���\������
		m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
	else
		m_lLineNoWidth = 0;

	//�`��J�n�s�ƕ`��I���s���v�Z
	ULONG	lineStart, lineStop;	//�`��s�ԍ�
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	lineStart = (ULONG)((ULONG)GetScrollPosLogY() / (ULONG)(m_lfHeight+m_lLineInterval));
	lineStop  = lineStart + (ULONG)((ULONG)clientRect.Height() / (ULONG)(m_lfHeight+m_lLineInterval)) + 1;

	//�t�H���g�ݒ�
	CFont* oldFont = m_objOffBufferDC.SelectObject(&m_objDispFont);

	//�P�s�Â`��
	ULONG ulCnt = 0;
	if( m_bDispRuler ) //���[����\������
	{
		//�w�i�F�œh��Ԃ�
		CRect	lineRect(0,(ulCnt*(m_lfHeight+m_lLineInterval)),m_nOffBufferWidth,((ulCnt+1)*(m_lfHeight+m_lLineInterval)));
		lineRect.NormalizeRect();
		m_objOffBufferDC.FillRect(&lineRect,&objRulerBackBrush);
		MyDrawingRuler(&m_objOffBufferDC,0,0,m_nOffBufferWidth,m_RulerColor,m_RulerBackColor);
		ulCnt++;
		lineStop--;
	}
	ULONG ulLineNo;
	for ( ulLineNo=lineStart; ulLineNo<lineStop; ulLineNo++)
	{
		//�w�i�F�œh��Ԃ�
		CRect	lineRect(0,(ulCnt*(m_lfHeight+m_lLineInterval)),m_nOffBufferWidth,((ulCnt+1)*(m_lfHeight+m_lLineInterval)));
		lineRect.NormalizeRect();
		m_objOffBufferDC.FillRect(&lineRect,&objBackBrush);

		CLine*  pobjLine = pDoc->GetOneLine(ulLineNo);
		if(pobjLine == NULL)
		{
			CRect objRect(  0,
							(ulCnt*(m_lfHeight+m_lLineInterval)),
							clientRect.Width()+m_lfWidth*10,
							((ulCnt+1)*(m_lfHeight+m_lLineInterval))
			);
			m_objOffBufferDC.FillRect(&objRect,&objBackBrush);
		}
		MyTextOut(&m_objOffBufferDC,m_lLineNoWidth,(ulCnt*(m_lfHeight+m_lLineInterval)),pobjLine,m_lfWidth,ulLineNo);

		ulCnt++;
	}
	if( m_bDispUnderLine )//�A���_�[���C����`�悷��ꍇ
	{
		MyDrawingUnderLine(&m_objOffBufferDC);
	}

	//�t�H���g�����Ƃɖ߂�
	m_objOffBufferDC.SelectObject(oldFont);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�s�`�揈��															|*/
/*|	���	�F																		|*/
/*|	���l	�FOnPrepareDC���ޭ��߰Ă̌��_�������s���Ă��Ȃ��ꍇ�p					|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyTextOut
	(	CDC*	pDC,			//�`��p�f�o�C�X�R���e�L�X�g
		long	nX,				//�e�L�X�g�̊J�n�_�̃f�o�C�X x ���W
		long	nY,				//�e�L�X�g�̊J�n�_�̃f�o�C�X y ���W
		CLine*  pobjLine,		//�`�悳��镶����
		long	lfWidth,		//�t�H���g�̕�
		long	lLineNo			//�s�ԍ�
	)
{
	ULONG	ulCnt=0;
	ULONG	ulOffset=0;
	char	szSpace[] = "            ";
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	BOOL	bMarked = pDoc->GetFlagMark(lLineNo);
	int		nDiffLineKind = pDoc->GetDiffLineKind(lLineNo);
	int		nDirCmpLineKind = pDoc->GetDirCmpLineKind(lLineNo);
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();
	CFont* pOldFont=NULL;

	if(pDC->IsPrinting())
		return;

	//////////////////////////
	//	�͈͑I���ʒu����	//
	//////////////////////////
	long	lSelectStartLine=0;
	long	lSelectEndLine=0;
	long	lSelectStartOffset=0;
	long	lSelectEndOffset=0;
	if( m_lSelectStartLine < m_lSelectEndLine )
	{
		lSelectStartLine   = m_lSelectStartLine;
		lSelectEndLine     = m_lSelectEndLine;
		lSelectStartOffset = m_lSelectStartOffset;
		lSelectEndOffset   = m_lSelectEndOffset;
	}
	else if( m_lSelectStartLine == m_lSelectEndLine )
	{
		lSelectStartLine = m_lSelectStartLine;
		lSelectEndLine   = m_lSelectEndLine;
		if( m_lSelectStartOffset <= m_lSelectEndOffset )
		{
			lSelectStartOffset = m_lSelectStartOffset;
			lSelectEndOffset   = m_lSelectEndOffset;
		}
		else
		{
			lSelectStartOffset = m_lSelectEndOffset;
			lSelectEndOffset   = m_lSelectStartOffset;
		}
	}
	else
	{
		lSelectStartLine   = m_lSelectEndLine;
		lSelectEndLine     = m_lSelectStartLine;
		lSelectStartOffset = m_lSelectEndOffset;
		lSelectEndOffset   = m_lSelectStartOffset;
	}

	//�w�i�F�u���V�쐬
	CBrush	objBackBrush;
	objBackBrush.CreateSolidBrush(m_BackColor);
	//���]�w�i�F�u���V�쐬
	CBrush	objInvertBackBrush;
	objInvertBackBrush.CreateSolidBrush(m_InvertBackColor);
	//�}�[�N�F�u���V�쐬
	CBrush	objMarkLineBrush;
	objMarkLineBrush.CreateSolidBrush(m_MarkLineColor);
	//�t�@�C����r�p�폜�s�F�u���V�쐬
	CBrush	objDiffDelLineBrush;
	objDiffDelLineBrush.CreateSolidBrush(m_DiffDelLineColor);
	//�t�@�C����r�p�ǉ��s�F�u���V�쐬
	CBrush	objDiffAddLineBrush;
	objDiffAddLineBrush.CreateSolidBrush(m_DiffAddLineColor);
	//�f�B���N�g����r�p����t�@�C�����F�u���V�쐬
	CBrush	objDirDiffLineBrush;
	objDirDiffLineBrush.CreateSolidBrush(m_DirDiffLineColor);

	//////////////////////////////////////
	//	���������̃X�N���[�����l�����A	//
	//	�`��J�n�I�t�Z�b�g���v�Z����	//
	//////////////////////////////////////
	long	lScrollPosLogX = GetScrollPosLogX();
	CPoint	objLimitPos(lScrollPosLogX,0);
	CPoint	objTempPos(0,0);
	//lCnt    <- �����X�N���[���ʒu���O�̃C���f�b�N�X
	//lOffset <- �����X�N���[���ʒu���O�̃I�t�Z�b�g
	GetPosition5(lLineNo,&objLimitPos,&ulCnt,&objTempPos);
	ulOffset = objTempPos.x;
	ULONG ulStartOffset = ulOffset;
	//�����X�N���[�����l�������`��J�n�̃f�o�C�X�w���W
	long lDevX = nX - GetScrollPosLogX();


	int			nOldBkMode = pDC->GetBkMode();
	COLORREF    clrOldText = pDC->GetTextColor();
	COLORREF	clrOldBack = pDC->GetBkColor();
	if( pobjLine != NULL )
	{
		//////////////////////////////////
		//	�L�[���[�h�J���[�����O		//
		//////////////////////////////////
		pobjLine->ResizeColorBuffer(pobjLine->GetLength(),0);
		pobjLine->ResizeUnderLineBuffer(pobjLine->GetLength(),FALSE);
		pDoc->KeyWordColoring(pobjLine);

		//////////////////
		//	�����o��	//
		//////////////////
		for( ; ulCnt<(ULONG)pobjLine->GetLength(); )//�����񒷕����[�v
		{
			//�N���C�A���g�̈�̕��̕������ɂ���
			if( (ulOffset-ulStartOffset) > (ULONG)(clientRect.Width()+m_lfWidth*10) )
				break;

			//�I��͈͓�����
			BOOL bSelected = IsWithinSelectArea( lSelectStartLine,
												 lSelectStartOffset,
												 lSelectEndLine,
												 lSelectEndOffset,
												 lLineNo,
												 ulCnt);
			if(IsDBCSLeadByteEx(0,pobjLine->GetAt(ulCnt)))//�Q�o�C�g�����̑��o�C�g
			{
				if( bSelected ) //�I��͈͓�
				{
					//���]�w�i�F�œh��Ԃ�
					CRect	lineRect((lDevX+ulOffset),nY,(lDevX+ulOffset+m_lfWidth*2),(nY+m_lfHeight+m_lLineInterval));
					lineRect.NormalizeRect();
					pDC->FillRect(&lineRect,&objInvertBackBrush);

					//�����F�ύX
					pDC->SetBkMode( TRANSPARENT );
					pDC->SetBkColor( m_InvertBackColor );
					pDC->SetTextColor( m_InvertFontColor );

					//�����t���`�F�b�N
					if(pobjLine->GetUnderLineAt(ulCnt))
					{
						pOldFont = pDC->SelectObject(&m_objDispFontUnderLine);
						//�Q�������o��
						pDC->TextOut((lDevX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),2);
						pDC->SelectObject(pOldFont);
					}
					else
					{
						//�Q�������o��
						pDC->TextOut((lDevX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),2);
					}

					if( (pobjLine->GetAt(ulCnt)==129)  && //�S�p�X�y�[�X��`�悷��ꍇ
						(pobjLine->GetAt(ulCnt+1)==64) &&
						(m_bDispZenkakuSpace!=FALSE)
					)
					{
						MyDrawingZenkakuSpace(	pDC,
								(lDevX+ulOffset),
								nY,
								m_lfWidth*2,
								m_lfHeight,
								m_InvertFontColor,
								m_InvertBackColor );
					}

					ulCnt+=2;
					ulOffset += (lfWidth * 2);
				}
				else //�I��͈͊O
				{
					CRect	lineRect((lDevX+ulOffset),nY,(lDevX+ulOffset+m_lfWidth*2),(nY+m_lfHeight+m_lLineInterval));
					lineRect.NormalizeRect();
					if(bMarked)
					{
						//�}�[�N�F�œh��Ԃ�
						pDC->FillRect(&lineRect,&objMarkLineBrush);
					}
					else if(nDiffLineKind==DIFF_LINEKIND_DELETED)
					{
						//�폜�s�F�œh��Ԃ�
						pDC->FillRect(&lineRect,&objDiffDelLineBrush);
					}
					else if(nDiffLineKind==DIFF_LINEKIND_ADDED)
					{
						//�ǉ��s�F�œh��Ԃ�
						pDC->FillRect(&lineRect,&objDiffAddLineBrush);
					}
					else if(nDirCmpLineKind==DIRCMP_LINEKIND_DIFF)
					{
						//����F�œh��Ԃ�
						pDC->FillRect(&lineRect,&objDirDiffLineBrush);
					}
					else
					{
						//�w�i�F�œh��Ԃ�
						pDC->FillRect(&lineRect,&objBackBrush);
					}

					//�����F�ύX
					pDC->SetBkMode( TRANSPARENT );
					if(bMarked)
						pDC->SetBkColor( m_MarkLineColor );
					else
						pDC->SetBkColor( m_BackColor );
					pDC->SetTextColor( pobjLine->GetColorAt(ulCnt) );

					//�����t���`�F�b�N
					if(pobjLine->GetUnderLineAt(ulCnt))
					{
						pOldFont = pDC->SelectObject(&m_objDispFontUnderLine);
						//�Q�������o��
						pDC->TextOut((lDevX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),2);
						pDC->SelectObject(pOldFont);
					}
					else
					{
						//�Q�������o��
						pDC->TextOut((lDevX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),2);
					}

					if( (pobjLine->GetAt(ulCnt)==129)  && //�S�p�X�y�[�X��`�悷��ꍇ
						(pobjLine->GetAt(ulCnt+1)==64) &&
						(m_bDispZenkakuSpace!=FALSE)
					)
					{
						if(bMarked)
						{
							MyDrawingZenkakuSpace(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth*2,
									m_lfHeight,
									m_ZenkakuSpaceColor,
									m_MarkLineColor );
						}
						else if(nDiffLineKind==DIFF_LINEKIND_DELETED)
						{
							MyDrawingZenkakuSpace(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth*2,
									m_lfHeight,
									m_ZenkakuSpaceColor,
									m_DiffDelLineColor );
						}
						else if(nDiffLineKind==DIFF_LINEKIND_ADDED)
						{
							MyDrawingZenkakuSpace(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth*2,
									m_lfHeight,
									m_ZenkakuSpaceColor,
									m_DiffAddLineColor );
						}
						else if(nDirCmpLineKind==DIRCMP_LINEKIND_DIFF)
						{
							MyDrawingZenkakuSpace(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth*2,
									m_lfHeight,
									m_ZenkakuSpaceColor,
									m_DirDiffLineColor );
						}
						else
						{
							MyDrawingZenkakuSpace(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth*2,
									m_lfHeight,
									m_ZenkakuSpaceColor,
									m_BackColor );
						}
					}

					ulCnt+=2;
					ulOffset += (lfWidth * 2);
				}
			}
			else //�P�o�C�g����
			{
				if(pobjLine->GetAt(ulCnt) == '\t')//�^�u�R�[�h
				{
					if( bSelected ) //�I��͈͓�
					{
						//�^�u�T�C�Y���v�Z
						int nMargin = pDoc->GetTabSize() - ( (ulOffset/lfWidth) % pDoc->GetTabSize() );

						//���]�w�i�F�œh��Ԃ�
						CRect	lineRect((lDevX+ulOffset),nY,(lDevX+ulOffset+m_lfWidth*nMargin),(nY+m_lfHeight+m_lLineInterval));
						lineRect.NormalizeRect();
						pDC->FillRect(&lineRect,&objInvertBackBrush);

						//�����F�ύX
						pDC->SetBkMode( TRANSPARENT );
						pDC->SetBkColor( m_InvertBackColor );
						pDC->SetTextColor( m_InvertFontColor );

						//�^�u�T�C�Y���X�y�[�X�`��
						pDC->TextOut((lDevX+ulOffset),nY,szSpace,nMargin);

						if( m_bDispTab ) //�^�u�R�[�h��`�悷��ꍇ
						{
							MyDrawingTab(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth*nMargin,
									m_lfHeight,
									m_InvertFontColor,
									m_InvertBackColor );
						}

						ulOffset += (lfWidth * nMargin);
						ulCnt++;
					}
					else // �͈͑I�𒆂łȂ�
					{
						//�^�u�T�C�Y�v�Z
						int nMargin = pDoc->GetTabSize() - ( (ulOffset/lfWidth) % pDoc->GetTabSize() );

						CRect	lineRect((lDevX+ulOffset),nY,(lDevX+ulOffset+m_lfWidth*nMargin),(nY+m_lfHeight+m_lLineInterval));
						lineRect.NormalizeRect();
						if(bMarked)
						{
							//�}�[�N�F�œh��Ԃ�
							pDC->FillRect(&lineRect,&objMarkLineBrush);
						}
						else if(nDiffLineKind==DIFF_LINEKIND_DELETED)
						{
							//�폜�s�F�œh��Ԃ�
							pDC->FillRect(&lineRect,&objDiffDelLineBrush);
						}
						else if(nDiffLineKind==DIFF_LINEKIND_ADDED)
						{
							//�ǉ��s�F�œh��Ԃ�
							pDC->FillRect(&lineRect,&objDiffAddLineBrush);
						}
						else if(nDirCmpLineKind==DIRCMP_LINEKIND_DIFF)
						{
							//����F�œh��Ԃ�
							pDC->FillRect(&lineRect,&objDirDiffLineBrush);
						}
						else
						{
							//�w�i�F�œh��Ԃ�
							pDC->FillRect(&lineRect,&objBackBrush);
						}

						//�����F�ύX
						pDC->SetBkMode( TRANSPARENT );
						if(bMarked)
							pDC->SetBkColor( m_MarkLineColor );
						else
							pDC->SetBkColor( m_BackColor );
						pDC->SetTextColor( pobjLine->GetColorAt(ulCnt) );

						//�^�u�T�C�Y���X�y�[�X�`��
						pDC->TextOut((lDevX+ulOffset),nY,szSpace,nMargin);

						if( m_bDispTab ) //�^�u�R�[�h��`�悷��ꍇ
						{
							if(bMarked)
							{
								MyDrawingTab(	pDC,
										(lDevX+ulOffset),
										nY,
										m_lfWidth*nMargin,
										m_lfHeight,
										m_TabColor,
										m_MarkLineColor );
							}
							else if(nDiffLineKind==DIFF_LINEKIND_DELETED)
							{
								MyDrawingTab(	pDC,
										(lDevX+ulOffset),
										nY,
										m_lfWidth*nMargin,
										m_lfHeight,
										m_TabColor,
										m_DiffDelLineColor );
							}
							else if(nDiffLineKind==DIFF_LINEKIND_ADDED)
							{
								MyDrawingTab(	pDC,
										(lDevX+ulOffset),
										nY,
										m_lfWidth*nMargin,
										m_lfHeight,
										m_TabColor,
										m_DiffAddLineColor );
							}
							else if(nDirCmpLineKind==DIRCMP_LINEKIND_DIFF)
							{
								MyDrawingTab(	pDC,
										(lDevX+ulOffset),
										nY,
										m_lfWidth*nMargin,
										m_lfHeight,
										m_TabColor,
										m_DirDiffLineColor );
							}
							else
							{
								MyDrawingTab(	pDC,
										(lDevX+ulOffset),
										nY,
										m_lfWidth*nMargin,
										m_lfHeight,
										m_TabColor,
										m_BackColor );
							}
						}

						ulOffset += (lfWidth * nMargin);
						ulCnt++;
					}
				}
				else //�^�u�ȊO
				{
					if( bSelected ) //�I��͈͓�
					{
						//���]�w�i�F�œh��Ԃ�
						CRect	lineRect((lDevX+ulOffset),nY,(lDevX+ulOffset+m_lfWidth),(nY+m_lfHeight+m_lLineInterval));
						lineRect.NormalizeRect();
						pDC->FillRect(&lineRect,&objInvertBackBrush);

						//�����F�ύX
						pDC->SetBkMode( TRANSPARENT );
						pDC->SetBkColor( m_InvertBackColor );
						pDC->SetTextColor( m_InvertFontColor );

						//�����t���`�F�b�N
						if(pobjLine->GetUnderLineAt(ulCnt))
						{
							pOldFont = pDC->SelectObject(&m_objDispFontUnderLine);
							//�P�������o��
							pDC->TextOut((lDevX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),1);
							pDC->SelectObject(pOldFont);
						}
						else
						{
							//�P�������o��
							pDC->TextOut((lDevX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),1);
						}
						ulCnt++;
						ulOffset += lfWidth;
					}
					else // �͈͑I�𒆂łȂ�
					{
						CRect	lineRect((lDevX+ulOffset),nY,(lDevX+ulOffset+m_lfWidth),(nY+m_lfHeight+m_lLineInterval));
						lineRect.NormalizeRect();
						if(bMarked)
						{
							//�}�[�N�F�œh��Ԃ�
							pDC->FillRect(&lineRect,&objMarkLineBrush);
						}
						else if(nDiffLineKind==DIFF_LINEKIND_DELETED)
						{
							//�폜�s�F�œh��Ԃ�
							pDC->FillRect(&lineRect,&objDiffDelLineBrush);
						}
						else if(nDiffLineKind==DIFF_LINEKIND_ADDED)
						{
							//�ǉ��s�F�œh��Ԃ�
							pDC->FillRect(&lineRect,&objDiffAddLineBrush);
						}
						else if(nDirCmpLineKind==DIRCMP_LINEKIND_DIFF)
						{
							//����F�œh��Ԃ�
							pDC->FillRect(&lineRect,&objDirDiffLineBrush);
						}
						else
						{
							//�w�i�F�œh��Ԃ�
							pDC->FillRect(&lineRect,&objBackBrush);
						}

						//�����F�ύX
						pDC->SetBkMode( TRANSPARENT );
						if(bMarked)
							pDC->SetBkColor( m_MarkLineColor );
						else
							pDC->SetBkColor( m_BackColor );
						pDC->SetTextColor( pobjLine->GetColorAt(ulCnt) );

						//�����t���`�F�b�N
						if(pobjLine->GetUnderLineAt(ulCnt))
						{
							pOldFont = pDC->SelectObject(&m_objDispFontUnderLine);
							//�P�������o��
							pDC->TextOut((lDevX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),1);
							pDC->SelectObject(pOldFont);
						}
						else
						{
							//�P�������o��
							pDC->TextOut((lDevX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),1);
						}
						ulCnt++;
						ulOffset += lfWidth;
					}
				}
			}
		}

		//���s�}�[�N�`��
		if( (m_bDispCRLF!=FALSE) && ((lLineNo+1)!=pDoc->GetLineCount()) )
		{
			//�I��͈͓�����
			BOOL bSelected = IsWithinSelectArea( lSelectStartLine,
											lSelectStartOffset,
											lSelectEndLine,
											lSelectEndOffset,
											lLineNo,
											ulCnt);
			if( bSelected ) //�I��͈͓�
			{
				//���]�w�i�F�œh��Ԃ�
				CRect	lineRect((lDevX+ulOffset),nY,(lDevX+ulOffset+m_lfWidth),(nY+m_lfHeight+m_lLineInterval));
				lineRect.NormalizeRect();
				pDC->FillRect(&lineRect,&objInvertBackBrush);

				MyDrawingCRLF(	pDC,
								(lDevX+ulOffset),
								nY,
								m_lfWidth,
								m_lfHeight,
								OPAQUE,
								m_InvertFontColor,
								m_InvertBackColor );
			}
			else
			{
				CRect	lineRect((lDevX+ulOffset),nY,(lDevX+ulOffset+m_lfWidth),(nY+m_lfHeight+m_lLineInterval));
				lineRect.NormalizeRect();
				if(bMarked)
				{
					//�}�[�N�F�œh��Ԃ�
					pDC->FillRect(&lineRect,&objMarkLineBrush);
				}
				else if(nDiffLineKind==DIFF_LINEKIND_DELETED)
				{
					//�폜�s�F�œh��Ԃ�
					pDC->FillRect(&lineRect,&objDiffDelLineBrush);
				}
				else if(nDiffLineKind==DIFF_LINEKIND_ADDED)
				{
					//�ǉ��s�F�œh��Ԃ�
					pDC->FillRect(&lineRect,&objDiffAddLineBrush);
				}
				else if(nDirCmpLineKind==DIRCMP_LINEKIND_DIFF)
				{
					//�ǉ��s�F�œh��Ԃ�
					pDC->FillRect(&lineRect,&objDirDiffLineBrush);
				}
				else
				{
					//�w�i�F�œh��Ԃ�
					pDC->FillRect(&lineRect,&objBackBrush);
				}

				if(bMarked)
				{
					MyDrawingCRLF(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth,
									m_lfHeight,
									TRANSPARENT,
									m_CRLFColor,
									m_MarkLineColor );
				}
				else if(nDiffLineKind==DIFF_LINEKIND_DELETED)
				{
					MyDrawingCRLF(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth,
									m_lfHeight,
									TRANSPARENT,
									m_CRLFColor,
									m_DiffDelLineColor );
				}
				else if(nDiffLineKind==DIFF_LINEKIND_ADDED)
				{
					MyDrawingCRLF(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth,
									m_lfHeight,
									TRANSPARENT,
									m_CRLFColor,
									m_DiffAddLineColor );
				}
				else if(nDirCmpLineKind==DIRCMP_LINEKIND_DIFF)
				{
					MyDrawingCRLF(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth,
									m_lfHeight,
									TRANSPARENT,
									m_CRLFColor,
									m_DirDiffLineColor );
				}
				else
				{
					MyDrawingCRLF(	pDC,
									(lDevX+ulOffset),
									nY,
									m_lfWidth,
									m_lfHeight,
									TRANSPARENT,
									m_CRLFColor,
									m_BackColor );
				}
			}
		}
	}

	if( ((lLineNo+1)==pDoc->GetLineCount()) || //�ŏI�s�̕`�悾����
		((pDoc->GetLineCount()==0) && (lLineNo==0))
	)
	{
		//�ݒ�ǂݍ���
		BOOL bIsDispEOFMark = m_pApp->GetProfileInt("CEditorView","bIsDispEOFMark",1);
		if( bIsDispEOFMark != 0 )
		{
			//�d�n�e�}�[�N�𔽓]�`��
			pDC->SetBkMode( OPAQUE );
			pDC->SetBkColor( m_InvertBackColor );
			pDC->SetTextColor( m_InvertFontColor );
			char szEOF[] = "[ EOF ]";
			pDC->TextOut((lDevX+ulOffset),nY,szEOF,strlen(szEOF));
			ulOffset += (lfWidth * strlen(szEOF));
			ulCnt+=strlen(szEOF);
		}
	}

	//�s�ԍ��`��
	if( m_bDispLineNo )
	{
		if( ((lLineNo+1)<=pDoc->GetLineCount()) ||
			((pDoc->GetLineCount()==0) && (lLineNo==0))
		)
		{
			//�s�ԍ�
			char	szLineNo[32];
			sprintf(szLineNo, "%6d  ", (lLineNo+1) );

			//�w�i�F�u���V�쐬���w�i�h��Ԃ�(1999.03.27 Modify for Smooth Horz Scrolling)
			CBrush	objBackBrush;
			objBackBrush.CreateSolidBrush(m_LineNoBackColor);
			CRect	rect( 0/*GetScrollPosLogX()*/,
						  nY,
						  (/*GetScrollPosLogX()+*/strlen(szLineNo)*m_lfWidth),
						  (nY+m_lfHeight+m_lLineInterval));
			rect.NormalizeRect();
			pDC->FillRect(&rect,&objBackBrush);

			//�����F�ύX
			pDC->SetBkMode( TRANSPARENT );
			pDC->SetBkColor( m_LineNoBackColor );
			pDC->SetTextColor( m_LineNoColor );

			//�P�������o��(1999.03.27 Modify for Smooth Horz Scrolling)
			pDC->TextOut(0,nY,szLineNo,strlen(szLineNo));

			//�Z�p���[�^�`��(1999.03.27 Modify for Smooth Horz Scrolling)
			long lStartX = 6*m_lfWidth + m_lfWidth/2;
			long lStartY = nY;
			long lEndY   = nY + m_lfHeight + m_lLineInterval;
			CPen	objPen;
			CPen*	pobjOldPen;
			objPen.CreatePen(PS_SOLID,1,m_LineNoColor);
			pobjOldPen = pDC->SelectObject(&objPen);
			pDC->MoveTo(lStartX, lStartY);
			pDC->LineTo(lStartX, lEndY);
			pDC->SelectObject(pobjOldPen);
		}
	}

	//���ɖ߂�
	pDC->SetBkMode( nOldBkMode );
	pDC->SetBkColor( clrOldBack );
	pDC->SetTextColor( clrOldText );
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�s�`��i����p�j����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::MyTextOutForPrint		// �\���������ŏI�I�t�Z�b�g
	(	CDC*	pDC,					// �`��p�f�o�C�X�R���e�L�X�g
		long	nX,						// �e�L�X�g�̊J�n�_�̘_�� x ���W
		long	nY,						// �e�L�X�g�̊J�n�_�̘_�� y ���W
		CLine*  pobjLine,				// �`�悳��镶����
		long	lfWidth,				// �t�H���g�̕�
		long	lLineNo,				// �s�ԍ�
		int		nLimitWidth				// �\����
	)
{
	ULONG	ulCnt=0;
	ULONG	ulOffset=0;
	char	szSpace[] = "            ";
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	if(!pDC->IsPrinting())
		return 0;
	if( pobjLine == NULL )
		return 0;

	//�����F�ݒ�
	COLORREF    clrOldText = pDC->GetTextColor();
	COLORREF	clrOldBack = pDC->GetBkColor();
	pDC->SetBkColor( RGB(255,255,255) );
	pDC->SetTextColor( RGB(0,0,0) );

	for(ulCnt=0; ulCnt<(ULONG)pobjLine->GetLength(); )//�����񒷕����[�v
	{
		if(IsDBCSLeadByteEx(0,pobjLine->GetAt(ulCnt)))//�Q�o�C�g�����̑��o�C�g
		{
			if((ulOffset+lfWidth*2) < (ULONG)nLimitWidth) //�\�����ȓ�
			{
				//�Q�������o��
				pDC->TextOut((nX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),2);
				ulCnt+=2;
				ulOffset += (lfWidth * 2);
			}
			else
			{
				return ulCnt;
			}
		}
		else //�P�o�C�g����
		{
			if(pobjLine->GetAt(ulCnt) == '\t')//�^�u�R�[�h
			{
				int nMargin = pDoc->GetTabSize() - ( (ulOffset/lfWidth) % pDoc->GetTabSize() );
				if((ulOffset+lfWidth*nMargin) < (ULONG)nLimitWidth) //�\�����ȓ�
				{
					//�`��J�n�ʒu���^�u�T�C�Y���i�߂�
					ulOffset += (lfWidth * nMargin);
					ulCnt++;
				}
				else
				{
					return ulCnt;
				}
			}
			else //���̑�
			{
				if((ulOffset+lfWidth) < (ULONG)nLimitWidth) //�\�����ȓ�
				{
					//�P�������o��
					pDC->TextOut((nX+ulOffset),nY,(const char*)pobjLine->Data(ulCnt),1);
					ulCnt++;
					ulOffset += lfWidth;
				}
				else
				{
					return ulCnt;
				}
			}
		}
	}

	//���ɖ߂�
	pDC->SetBkColor( clrOldBack );
	pDC->SetTextColor( clrOldText );

	return ulCnt;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s�}�[�N�`�揈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyDrawingCRLF
	(	CDC*		pDC,				//�f�o�C�X�R���e�L�X�g
		long		lX,					//�`��J�n�ʒu�i����̂w���W�j
		long		lY,					//�`��J�n�ʒu�i����̂x���W�j
		long		lWidth,				//�`��̈�̕�
		long		lHeight,			//�`��̈�̍���
		int			nMode,				//BkMode
		COLORREF	foreColor,			//�O�ʐF
		COLORREF	backColor			//�w�i�F
	)
{
	if( nMode == OPAQUE	) //�`��O�ɓh��Ԃ��K�v
	{
		CRect	drawRect(lX,lY,lX+lWidth,lY+lHeight);
		drawRect.NormalizeRect();
		CBrush	objBackBrush;
		objBackBrush.CreateSolidBrush(backColor);
		pDC->FillRect(&drawRect,&objBackBrush);
	}

	//���W�l�v�Z
	long lStartX = lX + (lWidth  / 4) * 3;		//���X�^�[�g�ʒu
	long lStartY = lY + (lHeight / 6) * 1;
	long lEdgeX  = lX + (lWidth  / 4) * 3;		//�Ȃ���p�̈ʒu
	long lEdgeY  = lY + (lHeight / 6) * 5;
	long lTopX   = lX + (lWidth  / 4) * 1;		//����[�̈ʒu
	long lTopY   = lY + (lHeight / 6) * 5;
	long lRWingX = lX + (lWidth  / 4) * 2;		//���̉E����[�̈ʒu
	long lRWingY = lY + (lHeight / 6) * 4;
	long lLWingX = lX + (lWidth  / 4) * 2;		//���̍�����[�̈ʒu
	long lLWingY = lY + (lHeight / 6) * 6;

	//�y���̍쐬������
	CPen	objPen;
	CPen*	pobjOldPen;
	objPen.CreatePen(PS_SOLID,1,foreColor);
	pobjOldPen = pDC->SelectObject(&objPen);

	//���`��
	pDC->MoveTo(lStartX, lStartY);
	pDC->LineTo(lEdgeX,  lEdgeY);				//�X�^�[�g�ʒu����Ȃ���p�܂�
	pDC->LineTo(lTopX,   lTopY);				//�Ȃ���p������̐�[�܂�
	pDC->LineTo(lRWingX, lRWingY);				//���̐�[����E����[�܂�
	pDC->MoveTo(lTopX,   lTopY);
	pDC->LineTo(lLWingX, lLWingY);				//���̐�[���獶����[�܂�

	//�y�������ɖ߂�
	pDC->SelectObject(pobjOldPen);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[���`�揈��														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyDrawingRuler
	(	CDC*		pDC,				//�f�o�C�X�R���e�L�X�g
		long		lX,					//�`��J�n�ʒu�i����̂w���W�j
		long		lY,					//�`��J�n�ʒu�i����̂x���W�j
		long		lWidth,				//�`��̈�̕�
		COLORREF	foreColor,			//�O�ʐF
		COLORREF	backColor			//�w�i�F
	)
{
	char	szSpace[] = "            ";
	char	szBuf[4];
	ULONG	ulDigit=0;
	ULONG	ulCnt=0;
	ULONG	ulOffset=0;

	//�ۑ�
	int			nOldBkMode = pDC->GetBkMode();
	COLORREF    clrOldText = pDC->GetTextColor();
	COLORREF	clrOldBack = pDC->GetBkColor();

	//�����F�ύX
	pDC->SetBkMode( OPAQUE );
	pDC->SetBkColor( backColor );
	pDC->SetTextColor( foreColor );

	if( m_bDispLineNo )//�s�ԍ���\������
	{
		ulCnt += strlen(m_szLineNoFormat);
		ulOffset += (m_lfWidth * strlen(m_szLineNoFormat));
	}

	for(;;)
	{
		//���[���P�O�������o��
		pDC->TextOut((lX+ulOffset-GetScrollPosLogX()),lY,m_szRuler,strlen(m_szRuler));
		ulCnt+=strlen(m_szRuler);
		ulOffset += (m_lfWidth * strlen(m_szRuler));
		//�����o��
		ulDigit++;
		if(ulDigit>=10)
			ulDigit=0;
		sprintf(szBuf,"%1d",ulDigit);
		pDC->TextOut((lX+ulOffset-GetScrollPosLogX()),lY,szBuf,1);
		ulCnt++;
		ulOffset += m_lfWidth;

		if(ulOffset > (ULONG)lWidth)
			break;
	}

	if( m_bDispLineNo )//�s�ԍ���\������
	{
		pDC->TextOut(0,lY,m_szLineNoFormat,strlen(m_szLineNoFormat));
//		pDC->TextOut(GetScrollPosLogX(),lY,m_szLineNoFormat,strlen(m_szLineNoFormat));
	}

	//�A���_�[���C����`�悷��
	CPen	objPen;
	CPen*	pobjOldPen;
	objPen.CreatePen(PS_SOLID,1,foreColor);
	pobjOldPen = pDC->SelectObject(&objPen);
	pDC->MoveTo(lX, lY+m_lfHeight-1);
	pDC->LineTo(lX+lWidth, lY+m_lfHeight-1);
	pDC->SelectObject(pobjOldPen);

	//���ɖ߂�
	pDC->SetBkMode( nOldBkMode );
	pDC->SetBkColor( clrOldBack );
	pDC->SetTextColor( clrOldText );
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�S�p�X�y�[�X�`�揈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyDrawingZenkakuSpace
	(	CDC*		pDC,				// �f�o�C�X�R���e�L�X�g
		long		lX,					// �`��J�n�ʒu�i����̂w���W�j
		long		lY,					// �`��J�n�ʒu�i����̂x���W�j
		long		lWidth,				// �`��̈�̕�
		long		lHeight,			// �`��̈�̍���
		COLORREF	foreColor,			// �O�ʐF
		COLORREF	backColor			// �w�i�F
	)
{
	//���W�l�v�Z
	long lEdge1X = lX + (lWidth  / 6) * 1;		//����
	long lEdge1Y = lY + (lHeight / 6) * 1;
	long lEdge2X = lX + (lWidth  / 6) * 1;		//����
	long lEdge2Y = lY + (lHeight / 6) * 6;
	long lEdge3X = lX + (lWidth  / 6) * 5;		//�E��
	long lEdge3Y = lY + (lHeight / 6) * 6;
	long lEdge4X = lX + (lWidth  / 6) * 5;		//�E��
	long lEdge4Y = lY + (lHeight / 6) * 1;

	//�y���̍쐬������
	CPen	objPen;
	CPen*	pobjOldPen;
	objPen.CreatePen(PS_SOLID,1,foreColor);
	pobjOldPen = pDC->SelectObject(&objPen);

	//�S�p�X�y�[�X�`��
	pDC->MoveTo(lEdge1X, lEdge1Y);
	pDC->LineTo(lEdge2X, lEdge2Y);
	pDC->LineTo(lEdge3X, lEdge3Y);
	pDC->LineTo(lEdge4X, lEdge4Y);
	pDC->LineTo(lEdge1X, lEdge1Y);

	//�y�������ɖ߂�
	pDC->SelectObject(pobjOldPen);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�^�u�R�[�h�`�揈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyDrawingTab
	(	CDC*		pDC,				// �f�o�C�X�R���e�L�X�g
		long		lX,					// �`��J�n�ʒu�i����̂w���W�j
		long		lY,					// �`��J�n�ʒu�i����̂x���W�j
		long		lWidth,				// �`��̈�̕�
		long		lHeight,			// �`��̈�̍���
		COLORREF	foreColor,			// �O�ʐF
		COLORREF	backColor			// �w�i�F
	)
{
	//���W�l�v�Z
	long lStartX, lStartY;
	long lTopX,   lTopY;
	long lRWingX, lRWingY;
	long lLWingX, lLWingY;
	long lUpX,    lUpY;
	long lDownX,  lDownY;
	lStartX = lX + (m_lfWidth / 6) * 1;				//���X�^�[�g�ʒu
	lStartY = lY + (lHeight   / 4) * 2;
	lTopX   = lX + lWidth - (m_lfWidth / 6) * 1;	//����[�̈ʒu
	lTopY   = lY + (lHeight   / 4) * 2;
	lRWingX = lX + lWidth - (m_lfWidth / 6) * 4;
	lRWingY = lY + (lHeight   / 4) * 3;
	lLWingX = lX + lWidth - (m_lfWidth / 6) * 4;
	lLWingY = lY + (lHeight   / 4) * 1;
	lUpX    = lX + (m_lfWidth / 6) * 1;
	lUpY    = lY + (lHeight   / 4) * 1;
	lDownX  = lX + (m_lfWidth / 6) * 1;
	lDownY  = lY + (lHeight   / 4) * 3;

	//�y���̍쐬������
	CPen	objPenSolid, objPenDot;
	CPen*	pobjOldPen;
	objPenSolid.CreatePen(PS_SOLID,1,foreColor);
	objPenDot.CreatePen(PS_DOT,1,foreColor);

	//�^�u�`��
	pobjOldPen = pDC->SelectObject(&objPenDot);
	pDC->MoveTo(lStartX, lStartY);
	pDC->LineTo(lTopX,   lTopY);

	pDC->SelectObject(&objPenSolid);
	pDC->MoveTo(lRWingX, lRWingY);
	pDC->LineTo(lTopX,   lTopY);
	pDC->LineTo(lLWingX, lLWingY);
	pDC->MoveTo(lUpX,    lUpY);
	pDC->LineTo(lDownX,  lDownY);

	//�y�������ɖ߂�
	pDC->SelectObject(pobjOldPen);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A���_�[���C���`�揈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyDrawingUnderLine
	(	CDC*		pDC					// �f�o�C�X�R���e�L�X�g
	)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	long lStartX, lStartY, lEndX, lEndY;
	CPen	objPen;
	CPen*	pobjOldPen=NULL;

	//�ő啝�v�Z
	ULONG ulRight = 0;
	if( ((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth) > clientRect.Width() )
		ulRight = (ULONG)((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth);
	else
		ulRight = clientRect.Width();

	if(m_bDispLineNo)
	{
		objPen.CreatePen(PS_SOLID,1,m_UnderLineColor);
		pobjOldPen = pDC->SelectObject(&objPen);
		lStartY = m_caretPos.y + m_lfHeight + 1;//m_lLineInterval;
		lEndY   = lStartY;
		lStartX = 0;
		lEndX   = m_lfWidth*6;
		pDC->MoveTo(lStartX, lStartY);
		pDC->LineTo(lEndX,   lStartY);
		lStartX = m_LineNoPos.x;
		lEndX   = ulRight;
		pDC->MoveTo(lStartX, lStartY);
		pDC->LineTo(lEndX,   lStartY);
		pDC->SelectObject(pobjOldPen);
	}
	else
	{
		objPen.CreatePen(PS_SOLID,1,m_UnderLineColor);
		pobjOldPen = pDC->SelectObject(&objPen);
		lStartX = 0;
		lEndX   = ulRight;
		lStartY = m_caretPos.y + m_lfHeight + 1;//m_lLineInterval;
		lEndY   = lStartY;
		pDC->MoveTo(lStartX, lStartY);
		pDC->LineTo(lEndX,   lStartY);
		pDC->SelectObject(pobjOldPen);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A���_�[���C����������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyEraseUnderLine
	(	CDC*		pDC					// �f�o�C�X�R���e�L�X�g
	)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	long lStartX, lStartY, lEndX, lEndY;
	CPen	objPen, objPen2;
	CPen*	pobjOldPen=NULL;

	//�ő啝�v�Z
	ULONG ulRight = 0;
	if( ((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth) > clientRect.Width() )
		ulRight = (ULONG)((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth);
	else
		ulRight = clientRect.Width();

	if(m_bDispLineNo)
	{
		objPen.CreatePen(PS_SOLID,1,m_LineNoBackColor);
		pobjOldPen = pDC->SelectObject(&objPen);
		lStartY = m_caretPos.y + m_lfHeight + 1;//m_lLineInterval;
		lEndY   = lStartY;
		lStartX = 0;
		lEndX   = m_lfWidth*6;
		pDC->MoveTo(lStartX, lStartY);
		pDC->LineTo(lEndX,   lStartY);

		objPen2.CreatePen(PS_SOLID,1,m_BackColor);
		pDC->SelectObject(&objPen2);
		lStartX = m_LineNoPos.x;
		lEndX   = ulRight;
		pDC->MoveTo(lStartX, lStartY);
		pDC->LineTo(lEndX,   lStartY);
		pDC->SelectObject(pobjOldPen);
	}
	else
	{
		objPen.CreatePen(PS_SOLID,1,m_BackColor);
		pobjOldPen = pDC->SelectObject(&objPen);
		lStartX = 0;
		lEndX   = ulRight;
		lStartY = m_caretPos.y + m_lfHeight + 1;//m_lLineInterval;
		lEndY   = lStartY;
		pDC->MoveTo(lStartX, lStartY);
		pDC->LineTo(lEndX,   lStartY);
		pDC->SelectObject(pobjOldPen);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I��͈͓����菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::IsWithinSelectArea	//�s�q�t�d�F�I��͈͓��A�e�`�k�r�d�F�I��͈͊O
	(	long	lSelectStartLine,		//�I���J�n�s�ԍ�
		long	lSelectStartOffset,		//�I���J�n��ԍ�
		long	lSelectEndLine,			//�I���I���s�ԍ�
		long	lSelectEndOffset,		//�I���I����ԍ�
		long	lCurrentLine,			//���ݍs�ԍ�
		long	lCurrentOffset			//���ݗ�ԍ�
	)
{
	if( (lSelectStartLine   == -1) || (lSelectEndLine   == -1) ||		//�͈͑I���Ȃ�
		(lSelectStartOffset == -1) || (lSelectEndOffset == -1)
	)
	{
		return FALSE;
	}
	if( (lCurrentLine < lSelectStartLine) || (lCurrentLine > lSelectEndLine) )	//�͈͑I���O
		return FALSE;
	if(lSelectStartLine != lSelectEndLine) //�����s�I��
	{
		if(lCurrentLine == lSelectStartLine) //�`�悷��P�s���I���J�n�s
		{
			if( lSelectStartOffset <= lCurrentOffset) //�I��͈͓�
				return TRUE;
			else
				return FALSE;
		}
		else if(lCurrentLine == lSelectEndLine) //�`�悷��P�s���I���I���s
		{
			if( lCurrentOffset < lSelectEndOffset ) //�I��͈͓�
				return TRUE;
			else
				return FALSE;
		}
		else if( (lCurrentLine > lSelectStartLine) && (lCurrentLine < lSelectEndLine) ) //�`�悷��P�s�S�̂��I��͈͓�
			return TRUE;
	}
	else //�P�s�I��
	{
		if( (lSelectStartOffset <= lCurrentOffset) &&	 //�I��͈͓�
			(lCurrentOffset     <  lSelectEndOffset )
		)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I��͈͂̍ĕ`�揈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::RedrawSelectArea
	(	long	lPreMoveCaretLine,		// �L�����b�g�ړ��O�̍s�ʒu
		long	lPreMoveCaretOffset		// �L�����b�g�ړ��O�̗�ʒu
	)
{
	long lRedrawLineCnt=0;

	//�L�����b�g�ړ���̍s�ԍ��Ɨ�ԍ��擾
	long lPostMoveCaretLine=0;
	long lPostMoveCaretOffset=0;
	if( CaretToLineOffset(&lPostMoveCaretLine,&lPostMoveCaretOffset) == FALSE )
		return;

	//�L�����b�g�ړ��O�ƌオ�����ꍇ�͍ĕ`�悵�Ȃ�
	if( lPreMoveCaretLine==lPostMoveCaretLine && lPreMoveCaretOffset==lPostMoveCaretOffset )
		return;

	if(m_lSelectEndLine == -1 && m_lSelectEndOffset == -1) //�͈͑I������
	{
		//�ړ����Ă��Ȃ��ꍇ�͍ĕ`�悵�Ȃ�
		if(m_lSelectStartLine==lPostMoveCaretLine && m_lSelectStartOffset==lPostMoveCaretOffset)
			return;

		//�I���I���̃Z�b�g
		m_lSelectEndLine = lPostMoveCaretLine;
		m_lSelectEndOffset = lPostMoveCaretOffset;

		//�L�����b�g�̎��͂��ӂ��߂čĕ`��
		if(m_lSelectStartLine == m_lSelectEndLine)
		{
			AdjustSelectArea();
			lRedrawLineCnt = 0;
		}
		else if(m_lSelectStartLine > m_lSelectEndLine) //�������֌�����
		{
			AdjustSelectArea();
			lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
		}
		else if(m_lSelectStartLine < m_lSelectEndLine) //�ォ�牺�֌�����
		{
			AdjustSelectArea();
			lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
		}
		OnUpdate(NULL,NULL,NULL);
		InvalidateCaretLineEx(lRedrawLineCnt);
	}
	else //�͈͑I���Q��ڈȍ~
	{
		//�������Ɍ������ꍇ�v���X�l�A�ォ�牺�֌������ꍇ�}�C�i�X�l
		lRedrawLineCnt = lPreMoveCaretLine - lPostMoveCaretLine;

		//�I��͈͍Đݒ�
		if(lPreMoveCaretLine==m_lSelectStartLine && lPreMoveCaretOffset==m_lSelectStartOffset)
		{
			m_lSelectStartLine = lPostMoveCaretLine;
			m_lSelectStartOffset = lPostMoveCaretOffset;
		}
		else if(lPreMoveCaretLine==m_lSelectEndLine && lPreMoveCaretOffset==m_lSelectEndOffset)
		{
			m_lSelectEndLine = lPostMoveCaretLine;
			m_lSelectEndOffset = lPostMoveCaretOffset;
		}
		else
		{
			return;
		}

		//�I��͈͒���
		AdjustSelectArea();

		//�L�����b�g�̎��͂��ӂ��߂čĕ`��
		OnUpdate(NULL,NULL,NULL);
		InvalidateCaretLineEx(lRedrawLineCnt);
	}
}

/////////////////////////////////////////////////////////////////////////////
//	CEditorView �v���e�N�g�֐��i�L�����b�g�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�Ή����銇�ʂɃL�����b�g���ʒu�Â��鏈��								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MySetCaretToMatchBrace
	(	BOOL	bSelect				//�͈͑I�������邩�H �e�`�k�r�d�F���Ȃ��A�s�q�t�d�F����
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CString objMsg;

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	// �L�����b�g�ʒu�̍s�擾
	CLine*	pobjLine = NULL;
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
		return;

	// �L�����b�g�ʒu�̕����擾
	UCHAR unCurrentChar = pobjLine->GetAt(lCurrentOffset);
	if( IsDBCSLeadByteEx(0,unCurrentChar) )
		return;
	if( unCurrentChar==0 )
		return;
	if( unCurrentChar!='(' &&
		unCurrentChar!=')' &&
		unCurrentChar!='{' &&
		unCurrentChar!='}' &&
		unCurrentChar!='[' &&
		unCurrentChar!=']' &&
		unCurrentChar!='<' &&
		unCurrentChar!='>'
	)
	{
		return;
	}

	// �Ή����銇�ʂ̌������L�����b�g�ړ�
	long lLine=lCurrentLine;
	long lOffset=lCurrentOffset;
	UCHAR unChar;
	if( unCurrentChar=='(' ||  //�L�����b�g�ʒu�̕��������ʊJ�n
		unCurrentChar=='{' ||
		unCurrentChar=='[' ||
		unCurrentChar=='<'
	)
	{
		UCHAR	unParenStart,unParenEnd;
		if( unCurrentChar == '(' )
		{
			unParenStart = '(';
			unParenEnd = ')';
		}
		else if( unCurrentChar == '{' )
		{
			unParenStart = '{';
			unParenEnd = '}';
		}
		else if( unCurrentChar == '[' )
		{
			unParenStart = '[';
			unParenEnd = ']';
		}
		else if( unCurrentChar == '<' )
		{
			unParenStart = '<';
			unParenEnd = '>';
		}

		lOffset++; // ���ֈʒu�Â�
		ULONG nParenCnt=0; // ���ʊJ�n�̐�������
		while(pobjLine != NULL)
		{
			for(;;)
			{
				unChar = pobjLine->GetAt(lOffset);
				if(unChar==0) //�s�I��
					break;
				if( IsDBCSLeadByteEx(0,unChar) ) //�Q�o�C�g����
				{
					lOffset+=2;
					continue;
				}
				else if( unChar==unParenStart ) //�P�o�C�g�����A���ʊJ�n
				{
					nParenCnt++; // ���ʊJ�n�̐��J�E���g�A�b�v
					lOffset++;
					continue;
				}
				else if( unChar==unParenEnd ) //�P�o�C�g�����A���ʏI��
				{
					if( nParenCnt > 0 )
					{
						nParenCnt--; // ���ʊJ�n�̐��J�E���g�_�E��
						lOffset++;
						continue;
					}
					else if( nParenCnt == 0 ) //�Ή����銇�ʔ���
					{
						//���̈ʒu�փL�����b�g���ړ�������
						Jump2LineOffsetEx(lLine,lOffset);
						if(bSelect)
						{
							m_lSelectStartLine = lCurrentLine;
							m_lSelectStartOffset = lCurrentOffset;
							m_lSelectEndLine = lLine;
							m_lSelectEndOffset = lOffset;
							//�ĕ`��
							OnUpdate(NULL,NULL,NULL);
							Invalidate(FALSE);
						}
						return;
					}
				}
				else //���ʈȊO�̂P�o�C�g����
				{
					lOffset++;
					continue;
				}
			}

			//���̍s�擾
			lOffset=0;
			lLine++;
			pobjLine = pDoc->GetOneLine(lLine);
		}
		objMsg.LoadString(IDS_PAREN_NOTFOUND);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		return;
	}
	else if( unCurrentChar==')' ||  //�L�����b�g�ʒu�̕��������ʏI��
			 unCurrentChar=='}' ||
			 unCurrentChar==']' ||
			 unCurrentChar=='>'
	)
	{
		UCHAR	unParenStart,unParenEnd;
		if( unCurrentChar == ')' )
		{
			unParenStart = '(';
			unParenEnd = ')';
		}
		else if( unCurrentChar == '}' )
		{
			unParenStart = '{';
			unParenEnd = '}';
		}
		else if( unCurrentChar == ']' )
		{
			unParenStart = '[';
			unParenEnd = ']';
		}
		else if( unCurrentChar == '>' )
		{
			unParenStart = '<';
			unParenEnd = '>';
		}

		lOffset = GetPreOffset(lLine,lOffset); // �O�ֈʒu�Â�
		int nParenCnt=0; // ���ʏI���̐�������
		while(pobjLine != NULL)
		{
			for(;;)
			{
				if( lOffset == -1 ) //�s�I��
					break;
				unChar = pobjLine->GetAt(lOffset);
				if( IsDBCSLeadByteEx(0,unChar) ) //�Q�o�C�g����
				{
					lOffset = GetPreOffset(lLine,lOffset);
					continue;
				}
				else if( unChar==unParenEnd ) //�P�o�C�g�����A���ʏI��
				{
					nParenCnt++; // ���ʏI���̐��J�E���g�A�b�v
					lOffset = GetPreOffset(lLine,lOffset);
					continue;
				}
				else if( unChar==unParenStart ) //�P�o�C�g�����A���ʊJ�n
				{
					if( nParenCnt > 0 )
					{
						nParenCnt--; // ���ʏI���̐��J�E���g�_�E��
						lOffset = GetPreOffset(lLine,lOffset);
						continue;
					}
					else if( nParenCnt == 0 ) //�Ή����銇�ʔ���
					{
						//���̈ʒu�փL�����b�g���ړ�������
						Jump2LineOffsetEx(lLine,lOffset);
						if(bSelect)
						{
							m_lSelectStartLine = lCurrentLine;
							m_lSelectStartOffset = lCurrentOffset;
							m_lSelectEndLine = lLine;
							m_lSelectEndOffset = lOffset;
							//�ĕ`��
							OnUpdate(NULL,NULL,NULL);
							Invalidate(FALSE);
						}
						return;
					}
				}
				else //���ʈȊO�̂P�o�C�g����
				{
					lOffset = GetPreOffset(lLine,lOffset);
					continue;
				}
			}

			//�O�̍s�擾
			lLine--;
			if( lLine < 0 )
				break;
			pobjLine = pDoc->GetOneLine(lLine);
//1998.10.29 Modify Start By S.Ekura
//			lOffset=pobjLine->GetLength()-1;
			lOffset=pobjLine->GetLength();		 // �s���ֈʒu�Â�
			lOffset=GetPreOffset(lLine,lOffset); // �O�ֈʒu�Â�
//1998.10.29 Modify End
		}
		objMsg.LoadString(IDS_PAREN_NOTFOUND);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		return;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w��s�A��ɃL�����b�g���ʒu�Â��鏈��								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MySetCaretToLineOffset
	(	long	lDestLine,
		long	lDestOffset
	)
{
	if( (m_lfWidth==0) || (m_lfHeight==0) )
		return;

	//��۰وʒu�s�ԍ�
	long lScrollLine = GetScrollPosLogY() / (m_lfHeight+m_lLineInterval);
	if( lDestLine < lScrollLine )
		return;

	//�L�����b�g�ʒu�̂x���W
	long lY = (lDestLine - lScrollLine) * (m_lfHeight+m_lLineInterval) + m_RulerPos.y;
	CPoint	tempPos(0,lY);

	//�L�����b�g�ʒu�̂w���W�i�_�����W�j
	GetPosition3(lDestLine,lDestOffset,&tempPos);
	m_nLogPreCaretX = tempPos.x; //�_���w���W�l���L��

	//�L�����b�g�ړ�
	tempPos.x += m_LineNoPos.x;
	tempPos.x -= GetScrollPosLogX();//(1999.03.06 Add)
	MySetCaretPos(tempPos); //�f�o�C�X���W�w��
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ʒu�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MySetCaretPos
	(	CPoint&		point			//�f�o�C�X�ʒu
	)
{
	long lMaxLine = 0;
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//���O�`�F�b�N
	if(point.x < 0 || point.y < 0)
		return;

	if(m_bDispUnderLine) //�A���_�[���C����\������
	{
		if(m_caretPos.y != point.y) //�x���W���قȂ�ʒu�փL�����b�g���ړ�����
		{
			//�O��̃L�����b�g�ʒu�̃A���_�[���C��������
			CClientDC	dc(this);
			CPoint ptVpOrg(0, 0);
			ptVpOrg.x = -1 * GetScrollPosLogX();
			dc.SetViewportOrg(ptVpOrg);
			MyEraseUnderLine(&dc);
		}
		//�L�����b�g�ʒu�ݒ�
		m_caretPos = point;
		if(m_bInsertKey) //�}�����[�h
		{
			if(m_nCaretType==CARET_IBEAM) //�c�_�L�����b�g
			{
				SetCaretPos(point);
			}
			else if(m_nCaretType==CARET_SQUARE) //���^�L�����b�g
			{	//�L�����b�g���P�s�̉������Ɉʒu�t����i�\����̂݁j
				CPoint tempPos = point;
				tempPos.y += (m_lfHeight/2);
				SetCaretPos(tempPos);
			}
		}
		else //�㏑�����[�h
		{
			SetCaretPos(point);
		}
		//�L�����b�g�ʒu�ɃA���_�[���C����`��
		CClientDC	dc(this);
		CPoint ptVpOrg(0, 0);
		ptVpOrg.x = -1 * GetScrollPosLogX();
		dc.SetViewportOrg(ptVpOrg);
		MyDrawingUnderLine(&dc);
	}
	else //�A���_�[���C����\�����Ȃ�
	{
		//�L�����b�g�ʒu�ݒ�
		m_caretPos = point;
		if(m_bInsertKey) //�}�����[�h
		{
			if(m_nCaretType==CARET_IBEAM) //�c�_�L�����b�g
			{
				SetCaretPos(point);
			}
			else if(m_nCaretType==CARET_SQUARE) //���^�L�����b�g
			{	//�L�����b�g���P�s�̉������Ɉʒu�t����i�\����̂݁j
				CPoint tempPos = point;
				tempPos.y += (m_lfHeight/2);
				SetCaretPos(tempPos);
			}
		}
		else //�㏑�����[�h
		{
			SetCaretPos(point);
		}
	}

	//////////////////////////////////////////////////////////////////
	//	�X�e�[�^�X�o�[�y�C���ɃL�����b�g�ʒu�̍s��ԍ���\������	//
	//////////////////////////////////////////////////////////////////

	if(pWnd->IsDispIndicatorGyoretu())
	{
		//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
		lCurrentLine = 0;
		lCurrentOffset = 0;
		if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
			return;
		lMaxLine = pDoc->GetLineCount();

		CPoint retPoint(0,0);
		CPoint logicalPoint = m_caretPos - m_LineNoPos - m_RulerPos;
		logicalPoint.x += GetScrollPosLogX();
		logicalPoint.y += GetScrollPosLogY();
		GetPosition2(lCurrentLine,&logicalPoint,&retPoint);
		if( m_lfWidth != 0 )
			lCurrentOffset = retPoint.x / m_lfWidth;
		else
			lCurrentOffset = 0;

		//�X�e�[�^�X�o�[�y�C���ɃL�����b�g�ʒu�̍s��ԍ���\������
		pWnd->SetGyoRetu((lCurrentLine+1),(lCurrentOffset+1),lMaxLine);
	}

	//////////////////////////////////////////////////////////////////
	//	�X�e�[�^�X�o�[�y�C���ɃL�����b�g�ʒu�̃o�C�g����\������	//
	//////////////////////////////////////////////////////////////////

	if(pWnd->IsDispIndicatorByte())
	{
		//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
		lCurrentLine = 0;
		lCurrentOffset = 0;
		if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
			return;
		long lSumByte = 0;
		long lCaretByte = 0;
		pDoc->GetBytes(lCurrentLine,&lCaretByte,&lSumByte);
		lCaretByte += lCurrentOffset;
		pWnd->SetCaretPosByte(lCaretByte,lSumByte);
	}

	//////////////////////////////////////////////////////////////
	//	�X�e�[�^�X�o�[�y�C���ɃL�����b�g�ʒu�̃R�[�h��\������	//
	//////////////////////////////////////////////////////////////

	if(pWnd->IsDispIndicatorCode())
	{
		//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
		lMaxLine = pDoc->GetLineCount();
		lCurrentLine = 0;
		lCurrentOffset = 0;
		if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
			return;
		CLine* pobjLine = pDoc->GetOneLine(lCurrentLine);
		if(pobjLine != NULL)
		{
			if(lCurrentOffset == pobjLine->GetLength())
			{
				if(lCurrentLine+1 == lMaxLine)
					pWnd->SetCaretPosCharCode(CODE_EOF,NULL);
				else
				{
					long lKaigyoKind = pDoc->GetKaigyoKind();
					if(lKaigyoKind == KAIGYO_CRLF)
						pWnd->SetCaretPosCharCode(CODE_CR,CODE_LF);
					else if(lKaigyoKind == KAIGYO_LF)
						pWnd->SetCaretPosCharCode(CODE_LF,NULL);
					else if(lKaigyoKind == KAIGYO_CR)
						pWnd->SetCaretPosCharCode(CODE_CR,NULL);
					else
						pWnd->SetCaretPosCharCode(CODE_CR,CODE_LF);
				}
			}
			else
			{
				UCHAR	uc1stChar = pobjLine->GetAt(lCurrentOffset);
				UCHAR	uc2ndChar = pobjLine->GetAt(lCurrentOffset+1);
				if(IsDBCSLeadByteEx(0,uc1stChar))
					pWnd->SetCaretPosCharCode(uc1stChar,uc2ndChar);
				else
					pWnd->SetCaretPosCharCode(uc1stChar,NULL);
			}
		}
	}

	//////////////////////////////////////
	//	�h�l�l�ϊ��E�C���h�E�ʒu����	//
	//////////////////////////////////////

	if( m_bIsImmOpen )
		CgAdjustCompositionWindow();

	//�X�e�[�^�X�o�[�y�C���ݒ�
	pWnd->SetInsMode(m_bInsertKey);
	pWnd->SetReadOnlyMode(pDoc->IsReadOnly());
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ړ�����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MoveCaret(UINT nChar, int nRepCnt)
{
	CPoint	objPoint;
	CPoint	objCaretPos = m_caretPos;

	//�L�����b�g�ʒu��_�����W�֕ϊ�����
	CPoint	logicalPoint = objCaretPos;
	logicalPoint -= m_LineNoPos;
	logicalPoint -= m_RulerPos;
	logicalPoint.x += GetScrollPosLogX();
	logicalPoint.y += GetScrollPosLogY();

	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	if( (m_lfWidth==0) || (m_lfHeight==0) )
		return;
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	long lLineLength    = pDoc->GetLineLength(lCurrentLine);
	long lMaxLine       = pDoc->GetLineCount();

	switch(nChar)
	{
	case VK_LEFT:	//�����L�[�ɑΉ�����L�����b�g�ړ�����
		if(logicalPoint.x > 0) //�s�̐擪�łȂ�
		{
			//�Q�o�C�g������^�u�R�[�h���l�����A�P�����O�̃L�����b�g�ʒu���擾
			CPoint tempPos(0,logicalPoint.y);
			GetPosition1(lCurrentLine,&logicalPoint,&tempPos);
			objCaretPos = tempPos;
			m_nLogPreCaretX = tempPos.x;//�_���w�ʒu���L��
			//�_�����W�|���f�o�C�X���W
			objCaretPos.x -= GetScrollPosLogX();
			objCaretPos.y -= GetScrollPosLogY();
			objCaretPos += m_RulerPos;
			objCaretPos += m_LineNoPos;
			if(objCaretPos.x < m_LineNoPos.x)//�\���͈͂̍����֍s�����ꍇ
			{
				//�������X�N���[��
				long lLogX = objCaretPos.x - m_LineNoPos.x - m_RulerPos.x + GetScrollPosLogX();
				MyScrollToPosition(CPoint(lLogX, GetScrollPosLogY()));
				objCaretPos.x = m_LineNoPos.x;
			}
		}
		else //�s���ł̍����L�[
		{
			if(lCurrentLine > 0)//�܂���֍s����
			{
				//MyScrollToPotision���P��̃R�[���ɂ��邽�߂̏���
				CPoint	destPos(GetScrollPosLogX(),GetScrollPosLogY());
				//�L�����b�g�𕶎���̍Ō����
				CPoint	tempLogPos(0,logicalPoint.y-(m_lfHeight+m_lLineInterval));
				GetPosition(lCurrentLine-1,&tempLogPos);//�s���̈ʒu�擾
				if(tempLogPos.x > nWidth)//�\���͈͂̉E���֍s�����ꍇ
				{
					//�E�����X�N���[��
					long lLogX = tempLogPos.x - nWidth + 1;
					destPos.x = lLogX;
				}
				m_nLogPreCaretX = tempLogPos.x;//�_���w�ʒu���L��
				//�_�����W�|���f�o�C�X���W
				objCaretPos = tempLogPos;
				objCaretPos -= destPos;/*m_ScrollPosLog;*/
				objCaretPos += m_RulerPos;
				objCaretPos += m_LineNoPos;
				if(objCaretPos.y < m_RulerPos.y )//�\���͈͂̏㑤�ɍs�����ꍇ
				{
					objCaretPos.y = m_RulerPos.y;
					//�P�s��������X�N���[��
					destPos.y -= (m_lfHeight+m_lLineInterval);
				}
				if( destPos.x != GetScrollPosLogX() ||
					destPos.y != GetScrollPosLogY()
				)
				{
					MyScrollToPosition(destPos);
				}
			}
		}
		break;
	case VK_RIGHT:	//�E���L�[�ɑΉ�����L�����b�g�ړ�����
		if(lCurrentOffset < lLineLength)//�����񒷂𒴂��Ă��Ȃ�
		{
			int nMargin=0;
			if(pDoc->MyIsDBCSLeadByteEx(lCurrentLine,lCurrentOffset))//�L�����b�g�ʒu���Q�o�C�g����
			{
				//�L�����b�g���Q����
				logicalPoint.x += (m_lfWidth * 2);
				nMargin = (m_lfWidth * 2);
			}
			else //�L�����b�g�ʒu���P�o�C�g����
			{
				if(pDoc->IsTabCode(lCurrentLine,lCurrentOffset))//�^�u�R�[�h
				{
					int n = pDoc->GetTabSize() - ( (logicalPoint.x/m_lfWidth) % pDoc->GetTabSize() );
					logicalPoint.x += (m_lfWidth * n);
					nMargin = (m_lfWidth * n);
				}
				else //�^�u�R�[�h�ȊO
				{
					logicalPoint.x += m_lfWidth;
					nMargin = m_lfWidth;
				}
			}
			if( logicalPoint.x > nWidth ) //�\���͈͂̉E���֍s�����ꍇ
			{
				//�E�����X�N���[��
				MyScrollToPosition(CPoint(GetScrollPosLogX()+nMargin, GetScrollPosLogY()));
			}
			m_nLogPreCaretX = logicalPoint.x;//�_���w�ʒu���L��
			//�_�����W�|���f�o�C�X���W
			objCaretPos = logicalPoint;
			objCaretPos.x -= GetScrollPosLogX();
			objCaretPos.y -= GetScrollPosLogY();
			objCaretPos += m_RulerPos;
			objCaretPos += m_LineNoPos;
		}
		else //�s���ł̉E���L�[
		{
			if(lCurrentLine+1 < lMaxLine)//�܂����֍s����
			{
				//MyScrollToPosition���P��̂݃R�[�����邽��
				CPoint	destPos(GetScrollPosLogX(),GetScrollPosLogY());
				m_nLogPreCaretX = 0;//�_���w�ʒu���L��
				//�L�����b�g���s�̐擪��
				objCaretPos.x = m_LineNoPos.x;
				if(GetScrollPosLogX() > 0)
				{
					//���X�N���[��
					destPos.x = 0;
				}
				//�L�����b�g�����̍s��
				if((objCaretPos.y+(m_lfHeight+m_lLineInterval)*2) > nHeight)//�\���͈͂̉����ɍs�����ꍇ
				{
					//�P�s���������X�N���[��
					destPos.y += (m_lfHeight+m_lLineInterval);
				}
				else
				{
					objCaretPos.y += (m_lfHeight+m_lLineInterval);
				}

				if( destPos.x != GetScrollPosLogX() ||
					destPos.y != GetScrollPosLogY()
				)
				{
					MyScrollToPosition( destPos );
				}
			}
		}
		break;
	case VK_DOWN: //�����L�[�ɑΉ�����L�����b�g�ړ�����
		if(lCurrentLine+nRepCnt < lMaxLine)//�܂����֍s����
		{
			//MyScrollToPosition���P��̃R�[���ɂ��邽�߂̏���
			CPoint destPos(GetScrollPosLogX(),GetScrollPosLogY());
			if((objCaretPos.y+(m_lfHeight+m_lLineInterval)*(nRepCnt+1)) > nHeight)//�\���͈͂̉����ɍs�����ꍇ
			{
				//�P�s���������X�N���[��
				destPos.y += (m_lfHeight+m_lLineInterval)*nRepCnt;
			}
			else
			{
				objCaretPos.y += (m_lfHeight+m_lLineInterval)*nRepCnt;
			}

			//�Q�o�C�g������^�u�R�[�h���l�����A�L�����b�g�ʒu���擾
			CPoint tempPos(0,0);
			if(m_nCaretMoveType == CARET_MOVE_WORDTYPE)
				logicalPoint.x = m_nLogPreCaretX; //�L�����b�g�_���ʒu�𕜌�
			GetPosition2(lCurrentLine+nRepCnt,&logicalPoint,&tempPos);
			objCaretPos.x = tempPos.x;
			//�_�����W�|���f�o�C�X���W
			objCaretPos.x -= GetScrollPosLogX();
			objCaretPos.x += m_RulerPos.x;
			objCaretPos.x += m_LineNoPos.x;
			if(objCaretPos.x < m_LineNoPos.x)//�\���͈͂̍����֍s�����ꍇ
			{
				//�������X�N���[��
				long lLogX = objCaretPos.x - m_LineNoPos.x - m_RulerPos.x + GetScrollPosLogX();
				destPos.x = lLogX;
				objCaretPos.x = m_LineNoPos.x;
			}
			if(tempPos.x > (nWidth+GetScrollPosLogX()))//�\���͈͂̉E���֍s�����ꍇ
			{
				//�E�����X�N���[��
				long lLogX = tempPos.x - nWidth - GetScrollPosLogX() + m_lfWidth;
				destPos.x += lLogX;
				objCaretPos.x -= lLogX;
			}

			if( destPos.x != GetScrollPosLogX() ||
				destPos.y != GetScrollPosLogY()
			)
			{
				MyScrollToPosition( destPos );
			}
		}
		break;
	case VK_UP: //����L�[�ɑΉ�����L�����b�g�ړ�����
		if(lCurrentLine-nRepCnt >= 0)//�܂���֍s����
		{
			//MyScrollToPosition ���P��݂̂̃R�[���Ƃ��邽�߂̏���
			CPoint	destPos(GetScrollPosLogX(),GetScrollPosLogY());
			objCaretPos.y -= (m_lfHeight+m_lLineInterval)*nRepCnt;
			if(objCaretPos.y < m_RulerPos.y )//�\���͈͂̏㑤�ɍs�����ꍇ
			{
				objCaretPos.y = m_RulerPos.y;
				//nRepCnt�s��������X�N���[��
				destPos.y -= (m_lfHeight+m_lLineInterval)*nRepCnt;
			}

			//�Q�o�C�g������^�u�R�[�h���l�����A�L�����b�g�ʒu���擾
			CPoint tempPos(0,0);
			if(m_nCaretMoveType == CARET_MOVE_WORDTYPE)
				logicalPoint.x = m_nLogPreCaretX; //�L�����b�g�_���ʒu�𕜌�
			GetPosition2(lCurrentLine-nRepCnt,&logicalPoint,&tempPos);
			objCaretPos.x = tempPos.x;
			//�_�����W�|���f�o�C�X���W
			objCaretPos.x -= GetScrollPosLogX();
			objCaretPos.x += m_RulerPos.x;
			objCaretPos.x += m_LineNoPos.x;
			if(objCaretPos.x < m_LineNoPos.x)//�\���͈͂̍����֍s�����ꍇ
			{
				//�������X�N���[��
				long lLogX = objCaretPos.x - m_LineNoPos.x - m_RulerPos.x + GetScrollPosLogX();
				destPos.x = lLogX;
				objCaretPos.x = m_LineNoPos.x;
			}
			if(tempPos.x > (nWidth+GetScrollPosLogX()))//�\���͈͂̉E���֍s�����ꍇ
			{
				//�E�����X�N���[��
				long lLogX = tempPos.x - nWidth - GetScrollPosLogX() + m_lfWidth;
				destPos.x += lLogX;
				objCaretPos.x -= lLogX;
			}

			if( destPos.x != GetScrollPosLogX() ||
				destPos.y != GetScrollPosLogY()
			)
			{
				MyScrollToPosition( destPos );
			}
		}
		break;
	case VK_HOME: //HOME�L�[�ɑΉ�����L�����b�g�ړ�����
		m_nLogPreCaretX = 0;//�_���w�ʒu���L��
		//�L�����b�g���s����
		objCaretPos.x = m_LineNoPos.x;
		MyScrollToPosition(CPoint(0,GetScrollPosLogY()));
		break;
	case VK_END: //END�L�[�ɑΉ�����L�����b�g�ړ�����
		//�L�����b�g�𕶎���̍Ō����
		{
			CPoint	tempLogPos(0,logicalPoint.y);
			GetPosition(lCurrentLine,&tempLogPos);//�s���̈ʒu�擾
			if(tempLogPos.x > nWidth)//�\���͈͂̉E���֍s�����ꍇ
			{
				//�E�����X�N���[��
				long lLogX = tempLogPos.x - nWidth + 1;
				MyScrollToPosition(CPoint(lLogX, GetScrollPosLogY()));
			}
			m_nLogPreCaretX = tempLogPos.x;//�_���w�ʒu���L��
			//�_�����W�|���f�o�C�X���W
			objCaretPos = tempLogPos;
			objCaretPos.x -= GetScrollPosLogX();
			objCaretPos.y -= GetScrollPosLogY();
			objCaretPos += m_RulerPos;
			objCaretPos += m_LineNoPos;
		}
		break;
	case VK_PRIOR: //PAGE UP�L�[�ɑΉ�����L�����b�g�ړ�����
		if(lCurrentLine > 0)//�܂���֍s����
		{
			//�P�O�s��������X�N���[��
			CPoint objScrollPos(GetScrollPosLogX(),GetScrollPosLogY());
			objScrollPos.x = 0;
			objCaretPos.x = m_LineNoPos.x;
			objScrollPos.y -= (m_VPageLog.cy * nRepCnt);
			if(objScrollPos.y < 0)
			{
				objScrollPos.y = 0;
				objCaretPos.y = m_RulerPos.y;		//�L�����b�g�ʒu�擪�ړ�
			}
			MyScrollToPosition(objScrollPos);
		}
		break;
	case VK_NEXT: //PAGE DOWN�L�[�ɑΉ�����L�����b�g�ړ�����
		CPoint objScrollPos(GetScrollPosLogX(),GetScrollPosLogY());
		objScrollPos.x = 0;
		objCaretPos.x = m_LineNoPos.x;
		if( (objScrollPos.y + nHeight + (m_VPageLog.cy * nRepCnt)) < m_totalLog.cy )
		{
			objScrollPos.y += (m_VPageLog.cy * nRepCnt);
		}
		else
		{
			while( (objScrollPos.y + nHeight + m_VLineLog.cy) < m_totalLog.cy )
				objScrollPos.y += m_VLineLog.cy;

			objCaretPos.y = (lMaxLine-1)*(m_lfHeight+m_lLineInterval) - objScrollPos.y + m_RulerPos.y;
		}
		MyScrollToPosition(objScrollPos);
		break;
	}

	MySetCaretPos(objCaretPos);		//�L�����b�g�ʒu�ݒ�i�N���C�A���g���W�n�j
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s���̈ʒu�擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetPosition
	(	long	lLine,				//�s�ԍ�
		CPoint*	pobjPoint			//�s���̈ʒu(out, �w���W�[������X�^�[�g�Ƒz��)
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//////////////////////////////////////////////////
	//	�Q�o�C�g������^�u�R�[�h���l������
	//	�s���̃L�����b�g�ʒu�����肷��

	pobjPoint->x = 0;
	long lLineLength = pDoc->GetLineLength(lLine);
	long lCnt=0;
	while(lCnt < lLineLength)
	{
		if(pDoc->MyIsDBCSLeadByteEx(lLine,lCnt))//�Q�o�C�g�����̑��o�C�g
		{
			lCnt+=2;
			pobjPoint->x += (m_lfWidth*2);
		}
		else //�P�o�C�g����
		{
			if(pDoc->IsTabCode(lLine,lCnt)) //�^�u�R�[�h
			{
				int nMargin = pDoc->GetTabSize() - ( (pobjPoint->x/m_lfWidth) % pDoc->GetTabSize() );
				pobjPoint->x += (m_lfWidth * nMargin);
				lCnt++;
			}
			else //�^�u�R�[�h�ȊO
			{
				lCnt++;
				pobjPoint->x += m_lfWidth;
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�����ʒu�擾����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetPosition1
	(	long	lLine,			//�s�ԍ�
		CPoint*	pobjLimitPos,	//���~�b�g�|�W�V�����i�w���W�[������X�^�[�g�Ƒz��j
		CPoint* pobjRetPos		//�ԋp�p�|�W�V�����i�w���W�[������X�^�[�g�Ƒz��j
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//////////////////////////////////////////////////////////////////////
	//	�Q�o�C�g������^�u�R�[�h���l�����ăL�����b�g�ʒu���w�肷��

	long lLineLength = pDoc->GetLineLength(lLine);
	pobjRetPos->x = 0;
	long lCnt=0;
	while(lCnt < lLineLength)
	{
		if(pDoc->MyIsDBCSLeadByteEx(lLine,lCnt))//�Q�o�C�g�����̑��o�C�g
		{
			if( (pobjRetPos->x+m_lfWidth*2) < pobjLimitPos->x)//�܂����v
			{
				lCnt+=2;
				pobjRetPos->x += (m_lfWidth*2);
			}
			else
			{
				break;
			}
		}
		else //�P�o�C�g����
		{
			if(pDoc->IsTabCode(lLine,lCnt)) //�^�u�R�[�h
			{
				int nMargin = pDoc->GetTabSize() - ( (pobjRetPos->x/m_lfWidth) % pDoc->GetTabSize() );
				if( (pobjRetPos->x+m_lfWidth*nMargin) < pobjLimitPos->x)//�܂����v
				{
					pobjRetPos->x += ( m_lfWidth * nMargin );
					lCnt++;
				}
				else
				{
					break;
				}
			}
			else //�^�u�R�[�h�ȊO
			{
				if( (pobjRetPos->x+m_lfWidth) < pobjLimitPos->x)//�܂����v
				{
					lCnt++;
					pobjRetPos->x += m_lfWidth;
				}
				else
				{
					break;
				}
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ȉ��ʒu�擾����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetPosition2
	(	long	lLine,			//�s�ԍ�
		CPoint*	pobjLimitPos,	//���~�b�g�|�W�V�����i�w���W�[������X�^�[�g�Ƒz��j
		CPoint* pobjRetPos		//�ԋp�p�|�W�V�����i�w���W�[������X�^�[�g�Ƒz��j
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//////////////////////////////////////////////////////////////////////
	//	�Q�o�C�g������^�u�R�[�h���l�����ăL�����b�g�ʒu���w�肷��

	long lLineLength = pDoc->GetLineLength(lLine);
	pobjRetPos->x = 0;
	long lCnt=0;
	while(lCnt < lLineLength)
	{
		if(pDoc->MyIsDBCSLeadByteEx(lLine,lCnt))//�Q�o�C�g�����̑��o�C�g
		{
			if( (pobjRetPos->x+m_lfWidth*2) <= pobjLimitPos->x)//�܂����v
			{
				lCnt+=2;
				pobjRetPos->x += (m_lfWidth*2);
			}
			else
			{
				break;
			}
		}
		else //�P�o�C�g����
		{
			if(pDoc->IsTabCode(lLine,lCnt)) //�^�u�R�[�h
			{
				int nMargin = pDoc->GetTabSize() - ( (pobjRetPos->x/m_lfWidth) % pDoc->GetTabSize() );
				if( (pobjRetPos->x+m_lfWidth*nMargin) <= pobjLimitPos->x)//�܂����v
				{
					pobjRetPos->x += ( m_lfWidth * nMargin );
					lCnt++;
				}
				else
				{
					break;
				}
			}
			else //�^�u�R�[�h�ȊO
			{
				if( (pobjRetPos->x+m_lfWidth) <= pobjLimitPos->x)//�܂����v
				{
					lCnt++;
					pobjRetPos->x += m_lfWidth;
				}
				else
				{
					break;
				}
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w��I�t�Z�b�g�����ʒu�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetPosition3
	(	long	lLine,			//�s�ԍ�
		long	lLimitOffset,	//���~�b�g�I�t�Z�b�g
		CPoint* pobjRetPos		//�ԋp�p�|�W�V�����i�w���W�[������X�^�[�g�Ƒz��j
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//////////////////////////////////////////////////////////////////////
	//	�Q�o�C�g������^�u�R�[�h���l�����ăL�����b�g�ʒu���w�肷��

	pobjRetPos->x = 0;
	long lCnt=0;
	while(lCnt < lLimitOffset)
	{
		if(pDoc->MyIsDBCSLeadByteEx(lLine,lCnt))//�Q�o�C�g�����̑��o�C�g
		{
			pobjRetPos->x += (m_lfWidth*2);
			lCnt+=2;
		}
		else //�P�o�C�g����
		{
			if(pDoc->IsTabCode(lLine,lCnt)) //�^�u�R�[�h
			{
				int nMargin = pDoc->GetTabSize() - ( (pobjRetPos->x/m_lfWidth) % pDoc->GetTabSize() );
				pobjRetPos->x += ( m_lfWidth * nMargin );
				lCnt++;
			}
			else //�^�u�R�[�h�ȊO
			{
				pobjRetPos->x += m_lfWidth;
				lCnt++;
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F������̍s���ʒu�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetPosition4
	(	long	lLine,				//�s�ԍ�
		CPoint*	pobjPoint,			//�s���̈ʒu(out, �w���W�[������X�^�[�g�Ƒz��)
		long	lPWidth				//����t�H���g�̕�
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//////////////////////////////////////////////////
	//	�Q�o�C�g������^�u�R�[�h���l������
	//	�s���̃L�����b�g�ʒu�����肷��

	pobjPoint->x = 0;
	long lLineLength = pDoc->GetLineLength(lLine);
	long lCnt=0;
	while(lCnt < lLineLength)
	{
		if(pDoc->MyIsDBCSLeadByteEx(lLine,lCnt))//�Q�o�C�g�����̑��o�C�g
		{
			lCnt+=2;
			pobjPoint->x += (lPWidth*2);
		}
		else //�P�o�C�g����
		{
			if(pDoc->IsTabCode(lLine,lCnt)) //�^�u�R�[�h
			{
				int nMargin = pDoc->GetTabSize() - ( (pobjPoint->x/lPWidth) % pDoc->GetTabSize() );
				pobjPoint->x += (lPWidth * nMargin);
				lCnt++;
			}
			else //�^�u�R�[�h�ȊO
			{
				lCnt++;
				pobjPoint->x += lPWidth;
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�t�Z�b�g�擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::GetPosition5
	(	long	lLine,			//�s�ԍ�
		CPoint*	pobjLimitPos,	//���~�b�g�|�W�V�����i�w���W�[������X�^�[�g�Ƒz��j
		ULONG*	plOffset,		//�I�t�Z�b�g�ԋp�p
		CPoint* pobjRetPos		//�ԋp�p�|�W�V�����i�w���W�[������X�^�[�g�Ƒz��j
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	////////////////////////////////////////////////////////////////////
	//	�Q�o�C�g������^�u�R�[�h���l�����ăL�����b�g�ʒu���w�肷��

	long lLineLength = pDoc->GetLineLength(lLine);
	pobjRetPos->x = 0;
	pobjRetPos->y = 0;
	(*plOffset) = 0;
	while((*plOffset) < (ULONG)lLineLength)
	{
		if(pDoc->MyIsDBCSLeadByteEx(lLine,(*plOffset)))//�Q�o�C�g�����̑��o�C�g
		{
			if( (pobjRetPos->x+m_lfWidth*2) <= pobjLimitPos->x)//�܂����v
			{
				(*plOffset)+=2;
				pobjRetPos->x += (m_lfWidth*2);
			}
			else
			{
				break;
			}
		}
		else //�P�o�C�g����
		{
			if(pDoc->IsTabCode(lLine,(*plOffset))) //�^�u�R�[�h
			{
				int nMargin = pDoc->GetTabSize() - ( (pobjRetPos->x/m_lfWidth) % pDoc->GetTabSize() );
				if( (pobjRetPos->x+m_lfWidth*nMargin) <= pobjLimitPos->x)//�܂����v
				{
					(*plOffset)++;
					pobjRetPos->x += ( m_lfWidth * nMargin );
				}
				else
				{
					break;
				}
			}
			else //�^�u�R�[�h�ȊO
			{
				if( (pobjRetPos->x+m_lfWidth) <= pobjLimitPos->x)//�܂����v
				{
					(*plOffset)++;
					pobjRetPos->x += m_lfWidth;
				}
				else
				{
					break;
				}
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�t�Z�b�g�擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorView::GetOffset
	(	long	lLine,				//�s�ԍ�
		CPoint*	pobjLimitPos		//���~�b�g�|�W�V�����i�w���W�[������X�^�[�g�Ƒz��j
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	////////////////////////////////////////////////////////////////////
	//	�Q�o�C�g������^�u�R�[�h���l�����ăL�����b�g�ʒu���w�肷��

	long lLineLength = pDoc->GetLineLength(lLine);
	CPoint	tempPoint(0,0);
	long lCnt=0;
	while(lCnt < lLineLength)
	{
		if(pDoc->MyIsDBCSLeadByteEx(lLine,lCnt))//�Q�o�C�g�����̑��o�C�g
		{
			if( (tempPoint.x+m_lfWidth*2) <= pobjLimitPos->x)//�܂����v
			{
				lCnt+=2;
				tempPoint.x += (m_lfWidth*2);
			}
			else
			{
				break;
			}
		}
		else //�P�o�C�g����
		{
			if(pDoc->IsTabCode(lLine,lCnt)) //�^�u�R�[�h
			{
				int nMargin = pDoc->GetTabSize() - ( (tempPoint.x/m_lfWidth) % pDoc->GetTabSize() );
				if( (tempPoint.x+m_lfWidth*nMargin) <= pobjLimitPos->x)//�܂����v
				{
					tempPoint.x += ( m_lfWidth * nMargin );
					lCnt++;
				}
				else
				{
					break;
				}
			}
			else //�^�u�R�[�h�ȊO
			{
				if( (tempPoint.x+m_lfWidth) <= pobjLimitPos->x)//�܂����v
				{
					lCnt++;
					tempPoint.x += m_lfWidth;
				}
				else
				{
					break;
				}
			}
		}
	}

	//�I�t�Z�b�g�l�ԋp
	return lCnt;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���O�I�t�Z�b�g�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorView::GetPreOffset	//���O�I�t�Z�b�g
	(	long	lLine,			//�s�ԍ�
		long	lLimitOffset	//���~�b�g�I�t�Z�b�g
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( lLimitOffset == 0 ) //���O�͂����Ȃ�
		return -1;

	long lCnt=0;
	for(;;)
	{
		if(pDoc->MyIsDBCSLeadByteEx(lLine,lCnt))//�Q�o�C�g�����̑��o�C�g
		{
			if( (lCnt+2) < lLimitOffset ) //�܂����v
				lCnt+=2;
			else
				return lCnt;
		}
		else //�P�o�C�g����
		{
			if( (lCnt+1) < lLimitOffset ) //�܂����v
				lCnt++;
			else
				return lCnt;
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ʒu�̍s�ԍ��A��ԍ��擾									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::CaretToLineOffset
	(	long*	plLine,			//�s�ԍ�(out)
		long*	plOffset		//��ԍ�(out)
	)
{
	//�L�����b�g�ʒu��_�����W�n�֕ϊ�����
	CPoint logicalPoint = m_caretPos;
	logicalPoint -= m_LineNoPos;
	logicalPoint -= m_RulerPos;
	logicalPoint.x += GetScrollPosLogX();
	logicalPoint.y += GetScrollPosLogY();

	//�L�����b�g�ʒu��\���t�H���g�̍����ƕ��ɍ��킹��
	if( (m_lfWidth==0) || (m_lfHeight==0) )
	{
		(*plLine)   = 0;
		(*plOffset) = 0;
		return FALSE;
	}
	logicalPoint.x -= (logicalPoint.x % m_lfWidth);
	logicalPoint.y -= (logicalPoint.y % (m_lfHeight+m_lLineInterval));

	//�L�����b�g�������s�ځA�������ڂɂ��邩���v�Z
	//�i�Q�o�C�g������^�u�R�[�h���l���j
	(*plLine)   = logicalPoint.y / (m_lfHeight+m_lLineInterval);
	(*plOffset) = GetOffset( (*plLine) ,&logicalPoint );

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�s�@�P�s�ĕ`�揈��											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::InvalidateCaretLine()
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect	clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	//�ő啝�v�Z
	ULONG ulRight = 0;
	if( ((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth) > clientRect.Width() )
		ulRight = (ULONG)((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth);
	else
		ulRight = clientRect.Width();

	//�ĕ`��
	CRect	lineRect;
	lineRect = CRect(0,m_caretPos.y,ulRight,m_caretPos.y+(m_lfHeight+m_lLineInterval));
	lineRect.NormalizeRect();

	if(m_pobjInvalidRect)
		delete m_pobjInvalidRect;
	m_pobjInvalidRect = new CRect();
	(*m_pobjInvalidRect) = lineRect;
	m_bUseClipRgn = TRUE; //�N���b�v���ꂽ�̈�̂ݕ`��X�V����
	InvalidateRect(&lineRect,FALSE);
	//�����ɕ`��
	UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�s�@�ĕ`�揈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::InvalidateCaretLineEx
	(	long lPreLine		//�L�����b�g�s�̂܂�艽�s����������ɍĕ`�悷��̂�
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect	clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	//�ő啝�v�Z
	ULONG ulRight = 0;
	if( ((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth) > clientRect.Width() )
		ulRight = (ULONG)((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth);
	else
		ulRight = clientRect.Width();

	//�ĕ`��
	CRect	lineRect;
	int nTop=0;
	int nBottom=0;
	if(lPreLine == 0) //�L�����b�g�s�̂ݍĕ`��
		lineRect = CRect(0,m_caretPos.y,ulRight,m_caretPos.y+(m_lfHeight+m_lLineInterval));
	else if(lPreLine < 0) //�L�����b�g�̑O�̍s����������ɍĕ`��
	{
		nTop = m_caretPos.y + (m_lfHeight+m_lLineInterval)*lPreLine;
		lineRect = CRect(0,nTop,ulRight,m_caretPos.y+(m_lfHeight+m_lLineInterval));
	}
	else if(lPreLine > 0) //�L�����b�g�̌�̍s����������ɍĕ`��
	{
		nBottom = m_caretPos.y + (m_lfHeight+m_lLineInterval)*(1+lPreLine);
		lineRect = CRect(0,m_caretPos.y,ulRight,nBottom);
	}
	lineRect.NormalizeRect();

	if(m_pobjInvalidRect)
		delete m_pobjInvalidRect;
	m_pobjInvalidRect = new CRect();
	(*m_pobjInvalidRect) = lineRect;
	m_bUseClipRgn = TRUE; //�N���b�v���ꂽ�̈�̂ݕ`��X�V����
	InvalidateRect(&lineRect,FALSE);
	//�����ɕ`��
	UpdateWindow();
}

/////////////////////////////////////////////////////////////////////////////
//	CEditorView �v���e�N�g�֐��i�}�E�X�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�E�X�ʒu�̍s�ԍ��A��ԍ��擾����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LONG CEditorView::MouseToLineOffset	//�O�F���s�A�P�F�����A�Q�F�s�ԍ��G���A��
	(	CPoint&		mousePoint,		// �}�E�X�ʒu(in)
		long*		plLine,			// �s�ԍ�(out)
		long*		plOffset,		// ��ԍ�(out)
		CPoint*		pCaretPoint		// �L�����b�g�ʒu(out)
	)
{
	long	lCurrentOffset=0;
	CEditorDoc*	pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if((m_bDispRuler!=FALSE) &&	(mousePoint.y<m_RulerPos.y)) //���[���̈��
	{
		(*plLine)	= 0;
		(*plOffset)	= 0;
		pCaretPoint->x = m_LineNoPos.x;
		pCaretPoint->y = m_RulerPos.y;
		return 0;
	}
	else if((m_bDispLineNo!=FALSE) && (mousePoint.x<m_LineNoPos.x)) //�s�ԍ��̈��
	{
		//�}�E�X�ʒu���f�o�C�X���W�n����_�����W�n�֕ϊ�����
		CPoint	logicalPoint = mousePoint;
		logicalPoint -=	m_LineNoPos;
		logicalPoint -=	m_RulerPos;
		logicalPoint.x += GetScrollPosLogX();
		logicalPoint.y += GetScrollPosLogY();

		//�}�E�X�ʒu��\���t�H���g�̍����ƕ��ɍ��킹��
		if(	(m_lfWidth==0)  || (m_lfHeight==0) )
		{
			(*plLine)	= 0;
			(*plOffset)	= 0;
			pCaretPoint->x = m_LineNoPos.x;
			pCaretPoint->y = m_RulerPos.y;
			return 0;
		}
		logicalPoint.x -= (logicalPoint.x %	m_lfWidth);
		logicalPoint.y -= (logicalPoint.y %	(m_lfHeight+m_lLineInterval));

		//�}�E�X�ʒu�̍s�ԍ����v�Z
		long lCurrentLine	= logicalPoint.y / (m_lfHeight+m_lLineInterval);
		long lMaxLine		= pDoc->GetLineCount();
		if(	lCurrentLine < lMaxLine	)
		{
		}
		else
		{
			if(	lMaxLine > 1 )
				lCurrentLine = lMaxLine	- 1;
			else
				lCurrentLine = 0;
		}

		pCaretPoint->x = 0;
		pCaretPoint->y = 0;
		//�s�ԍ��A��ԍ��ԋp
		(*plLine) =	lCurrentLine;
		(*plOffset)	= 0;
		return 2;
	}
	else //���̑�
	{
		//�}�E�X�ʒu���f�o�C�X���W�n����_�����W�n�֕ϊ�����
		CPoint	logicalPoint = mousePoint;
		logicalPoint -=	m_LineNoPos;
		logicalPoint -=	m_RulerPos;
		logicalPoint.x += GetScrollPosLogX();
		logicalPoint.y += GetScrollPosLogY();

		//�}�E�X�ʒu��\���t�H���g�̍����ƕ��ɍ��킹��
		if(	(m_lfWidth==0)  || (m_lfHeight==0) )
		{
			(*plLine)	= 0;
			(*plOffset)	= 0;
			pCaretPoint->x = m_LineNoPos.x;
			pCaretPoint->y = m_RulerPos.y;
			return 0;
		}
		logicalPoint.x -= (logicalPoint.x %	m_lfWidth);
		logicalPoint.y -= (logicalPoint.y %	(m_lfHeight+m_lLineInterval));

		//�}�E�X�ʒu�̍s�ԍ����v�Z
		long lCurrentLine	= logicalPoint.y / (m_lfHeight+m_lLineInterval);
		long lMaxLine		= pDoc->GetLineCount();

		if(	lCurrentLine < lMaxLine	)
		{
			//�Q�o�C�g������^�u�R�[�h���l�����ăL�����b�g�ʒu���w�肷��i�_�����W�j
			CPoint	tempPos(0,logicalPoint.y);
			GetPosition2(lCurrentLine,&logicalPoint,&tempPos);
			(*pCaretPoint) = tempPos;

			//�}�E�X�ʒu�̗�ԍ����v�Z
			lCurrentOffset = GetOffset(lCurrentLine,&tempPos);

			//�L�����b�g�ʒu���f�o�C�X���W�֖߂�
			(*pCaretPoint).x -=	GetScrollPosLogX();
			(*pCaretPoint).y -=	GetScrollPosLogY();
			(*pCaretPoint) += m_RulerPos;
			(*pCaretPoint) += m_LineNoPos;
		}
		else
		{
			pCaretPoint->x = m_LineNoPos.x;
			lCurrentOffset = 0;
			if(	lMaxLine > 1 )
			{
				lCurrentLine = lMaxLine	- 1;
				pCaretPoint->y = lCurrentLine *	(m_lfHeight+m_lLineInterval);

				//�L�����b�g�ʒu���f�o�C�X���W�֖߂�
				pCaretPoint->y -= GetScrollPosLogY();
				pCaretPoint->y += m_RulerPos.y;
			}
			else
			{
				lCurrentLine = 0;
				pCaretPoint->y = m_RulerPos.y;
			}
		}

		//�s�ԍ��A��ԍ��ԋp
		(*plLine) =	lCurrentLine;
		(*plOffset)	= lCurrentOffset;
		return 1;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	CEditorView �v���e�N�g�֐��i�I��͈͊֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I��͈͒�������														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::AdjustSelectArea()
{
	long	lSelectStartLine;
	long	lSelectEndLine;
	long	lSelectStartOffset;
	long	lSelectEndOffset;
	if( m_lSelectStartLine < m_lSelectEndLine )
	{
		lSelectStartLine   = m_lSelectStartLine;
		lSelectEndLine     = m_lSelectEndLine;
		lSelectStartOffset = m_lSelectStartOffset;
		lSelectEndOffset   = m_lSelectEndOffset;
	}
	else if( m_lSelectStartLine == m_lSelectEndLine )
	{
		lSelectStartLine = m_lSelectStartLine;
		lSelectEndLine   = m_lSelectEndLine;
		if( m_lSelectStartOffset <= m_lSelectEndOffset )
		{
			lSelectStartOffset = m_lSelectStartOffset;
			lSelectEndOffset   = m_lSelectEndOffset;
		}
		else
		{
			lSelectStartOffset = m_lSelectEndOffset;
			lSelectEndOffset   = m_lSelectStartOffset;
		}
	}
	else
	{
		lSelectStartLine   = m_lSelectEndLine;
		lSelectEndLine     = m_lSelectStartLine;
		lSelectStartOffset = m_lSelectEndOffset;
		lSelectEndOffset   = m_lSelectStartOffset;
	}

	m_lSelectStartLine   = lSelectStartLine;
	m_lSelectStartOffset = lSelectStartOffset;
	m_lSelectEndLine     = lSelectEndLine;
	m_lSelectEndOffset   = lSelectEndOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FCTRL+�N���b�N�ɂ�郏�[�h�Z���N�g����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::WordSelectByCtrlClick
	(	CPoint&		point
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	long	lCnt=0;
	long	lCurrentLine=0;
	long	lCurrentOffset=0;
	CPoint	prePos(0,0);
	CPoint	nowPos(0,0);
	CPoint	mousePos(0,0);
	CLine*	pobjLine = NULL;

	//�}�E�X�ʒu�@�|���@�s�A��ԍ��ϊ�
	LONG lRet =	MouseToLineOffset( point, &lCurrentLine, &lCurrentOffset, &mousePos	);
	if(lRet	== 0 ||	lRet ==	2)
		return;

	//�}�E�X�ʒu�̍s�擾
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
		return;

	//�܂��I������������
	ClearSelectArea();
	m_pApp->MessagePump();

	//////////////////////////////
	//		�I��͈͐ݒ�		//
	//////////////////////////////
	if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //�ʒu�����p
	{
		if( isHanKana(pobjLine->GetAt(lCurrentOffset)) )//���p����
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// �I��͈͂̌�������
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//�I��͈͂̑O������

			//�}�E�X�ʒu��_�����W�֕ϊ�����
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//�P�����O�̈ʒu�擾
				if( ((nowPos.x-prePos.x)==m_lfWidth) &&
					isHanKana(pobjLine->GetAt(lCnt-1))
				)
				{
					m_lSelectStartOffset--;
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt--;
				if( nowPos.x == 0 )
					break;
			}
		}
		else if( ( (pobjLine->GetAt(lCurrentOffset)>='A' && pobjLine->GetAt(lCurrentOffset)<='Z') ||
			  	 (pobjLine->GetAt(lCurrentOffset)>='a' && pobjLine->GetAt(lCurrentOffset)<='z') ||
			     (pobjLine->GetAt(lCurrentOffset)>='0' && pobjLine->GetAt(lCurrentOffset)<='9') ||
			     (pobjLine->GetAt(lCurrentOffset)=='_')
			     ) &&
				 !isHanKana(pobjLine->GetAt(lCurrentOffset))
		)
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// �I��͈͂̌�������
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) 						||
					( (pobjLine->GetAt(lCnt)<'A' || pobjLine->GetAt(lCnt)>'Z') &&
					  (pobjLine->GetAt(lCnt)<'a' || pobjLine->GetAt(lCnt)>'z') &&
					  (pobjLine->GetAt(lCnt)<'0' || pobjLine->GetAt(lCnt)>'9') &&
					  (pobjLine->GetAt(lCnt)!='_')
					)																||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//�I��͈͂̑O������

			//�}�E�X�ʒu��_�����W�֕ϊ�����
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//�P�����O�̈ʒu�擾
				if( ( (nowPos.x-prePos.x)==m_lfWidth) &&
					( (pobjLine->GetAt(lCnt-1)>='A' && pobjLine->GetAt(lCnt-1)<='Z') ||
			  		  (pobjLine->GetAt(lCnt-1)>='a' && pobjLine->GetAt(lCnt-1)<='z') ||
			  		  (pobjLine->GetAt(lCnt-1)>='0' && pobjLine->GetAt(lCnt-1)<='9') ||
			  		  (pobjLine->GetAt(lCnt-1)=='_')
					) &&
					!isHanKana(pobjLine->GetAt(lCnt-1))
				)
				{
					m_lSelectStartOffset--;
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt--;
				if( nowPos.x == 0 )
					break;
			}
		}
		else if( !isspace(pobjLine->GetAt(lCurrentOffset)) ) //�ʒu�����p�p�����ł����p���łł��߰��ł��Ȃ�
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// �I��͈͂̌�������
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) 								||
					( (pobjLine->GetAt(lCnt)>='A' && pobjLine->GetAt(lCnt)<='Z') ||
			  		  (pobjLine->GetAt(lCnt)>='a' && pobjLine->GetAt(lCnt)<='z') ||
			  		  (pobjLine->GetAt(lCnt)>='0' && pobjLine->GetAt(lCnt)<='9') ||
			  		  (pobjLine->GetAt(lCnt)=='_')
					) 																		||
					isHanKana(pobjLine->GetAt(lCnt))										||
					isspace(pobjLine->GetAt(lCnt))
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//�I��͈͂̑O������

			//�}�E�X�ʒu��_�����W�֕ϊ�����
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//�P�����O�̈ʒu�擾
				if( ((nowPos.x-prePos.x)==m_lfWidth)  									&&
					( (pobjLine->GetAt(lCnt-1)<'A' || pobjLine->GetAt(lCnt-1)>'Z') &&
					  (pobjLine->GetAt(lCnt-1)<'a' || pobjLine->GetAt(lCnt-1)>'z') &&
					  (pobjLine->GetAt(lCnt-1)<'0' || pobjLine->GetAt(lCnt-1)>'9') &&
					  (pobjLine->GetAt(lCnt-1)!='_')
					)																	&&
					!isHanKana(pobjLine->GetAt(lCnt-1))									&&
					!isspace(pobjLine->GetAt(lCnt-1))
				)
				{
					m_lSelectStartOffset--;
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt--;
				if( nowPos.x == 0 )
					break;
			}
		}
	}
	else if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //�ʒu���S�p����
	{
		//�����R�[�h�擾
		UINT unChar = ( (pobjLine->GetAt(lCurrentOffset) << 8) | (pobjLine->GetAt(lCurrentOffset+1)) );
		if( _ismbchira( unChar ) ) //�ʒu��������޲ĂЂ炪��
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// �I��͈͂̌�������
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!_ismbchira( unCharTmp )
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//�I��͈͂̑O������

			//�}�E�X�ʒu��_�����W�֕ϊ�����
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//�P�����O�̈ʒu�擾
				if( (nowPos.x-prePos.x)==(m_lfWidth*2) )
				{
					//�����R�[�h�擾
					UINT unCharTmp = ( (pobjLine->GetAt(lCnt-2) << 8) | (pobjLine->GetAt(lCnt-1)) );
					if( _ismbchira( unCharTmp ) ) //�Ђ炪�Ȍp��
					{
						m_lSelectStartOffset -= 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt -= 2;
				if( nowPos.x == 0 )
					break;
			}
		}
		else if( (_ismbckata(unChar)) ||
				 (unChar==0x815B)			//�ʒu��������޲Ă������Ȃ����h�[�h
		)
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// �I��͈͂̌�������
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbckata( unCharTmp ) && (unCharTmp!=0x815B) )
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//�I��͈͂̑O������

			//�}�E�X�ʒu��_�����W�֕ϊ�����
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//�P�����O�̈ʒu�擾
				if( (nowPos.x-prePos.x)==(m_lfWidth*2) )
				{
					//�����R�[�h�擾
					UINT unCharTmp = ( (pobjLine->GetAt(lCnt-2) << 8) | (pobjLine->GetAt(lCnt-1)) );
					if( _ismbckata( unCharTmp ) || (unCharTmp==0x815B) ) //�J�^�J�i�p��
					{
						m_lSelectStartOffset -= 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt -= 2;
				if( nowPos.x == 0 )
					break;
			}
		}
		else if( _ismbcl1(unChar) || _ismbcl2(unChar) ) //�ʒu��������޲Ċ���
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// �I��͈͂̌�������
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbcl1( unCharTmp ) && !_ismbcl2( unCharTmp ) )
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//�I��͈͂̑O������

			//�}�E�X�ʒu��_�����W�֕ϊ�����
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//�P�����O�̈ʒu�擾
				if( (nowPos.x-prePos.x)==(m_lfWidth*2) )
				{
					//�����R�[�h�擾
					UINT unCharTmp = ( (pobjLine->GetAt(lCnt-2) << 8) | (pobjLine->GetAt(lCnt-1)) );
					if( _ismbcl1(unCharTmp) || _ismbcl2(unCharTmp) ) //�����p��
					{
						m_lSelectStartOffset -= 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt -= 2;
				if( nowPos.x == 0 )
					break;
			}
		}
		else //���̑��̂Q�o�C�g����
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// �I��͈͂̌�������
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( _ismbchira( unCharTmp ) ||
					  _ismbckata(unCharTmp)   ||
					  (unCharTmp==0x815B)     ||
					  _ismbcl1( unCharTmp )   ||
					  _ismbcl2( unCharTmp )
					)
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//�I��͈͂̑O������

			//�}�E�X�ʒu��_�����W�֕ϊ�����
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//�P�����O�̈ʒu�擾
				if( (nowPos.x-prePos.x)==(m_lfWidth*2) )
				{
					//�����R�[�h�擾
					UINT unCharTmp = ( (pobjLine->GetAt(lCnt-2) << 8) | (pobjLine->GetAt(lCnt-1)) );
					if(	( !_ismbchira( unCharTmp ) &&
					      !_ismbckata(unCharTmp)   &&
					      (unCharTmp!=0x815B)      &&
					      !_ismbcl1( unCharTmp )   &&
					      !_ismbcl2( unCharTmp )
					    )
					)
					{
						m_lSelectStartOffset -= 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt -= 2;
				if( nowPos.x == 0 )
					break;
			}
		}
	}

	//�L�����b�g�ʒu�ݒ�
	MySetCaretToLineOffset(m_lSelectEndLine,m_lSelectEndOffset);

	//�}�E�X�ʒu�̍s���ĕ`��
//	InvalidateCaretLine();
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
//	CEditorView �v���e�N�g�֐��i�W�����v�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w��s�A��ւ̃W�����v����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::Jump2LineOffset
	(	long	lJumpLine,			//�W�����v��s�ԍ��i�[���I���W���j
		long	lJumpOffset			//�W�����v���ԍ�
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	if( (m_lfWidth==0) || (m_lfHeight==0) )
		return;
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//�N���C�A���g�̈�ɕ\���ł���s��
	if( m_lfHeight==0 )
		return;
	long lLineCnt = clientRect.Height() / (m_lfHeight+m_lLineInterval);
	//�h�L�������g�̑��s��
	long lMaxLine = pDoc->GetLineCount();

	//�w��I�t�Z�b�g�ȉ��ʒu�擾�i�_�����W�Ŗ߂��Ă���j
	CPoint	objTempPos(0,0);
	GetPosition3(lJumpLine,lJumpOffset,&objTempPos);

	if( lLineCnt < lMaxLine )
	{
		if( ((lJumpLine+1) + lLineCnt) <= lMaxLine )
		{
			long lX = (objTempPos.x > nWidth) ? (objTempPos.x-nWidth+1) : 0;
			long lY = lJumpLine * (m_lfHeight+m_lLineInterval);
			MyScrollToPosition(CPoint(lX,lY));				//�_�����W�ňʒu��n���Ă���
			CPoint caretPos(objTempPos.x,lY);
			//�_�����W�|���f�o�C�X���W
			caretPos.x -= GetScrollPosLogX();
			caretPos.y -= GetScrollPosLogY();
			caretPos += m_RulerPos;
			caretPos += m_LineNoPos;
			MySetCaretPos(caretPos);
		}
		else
		{
			long lX = (objTempPos.x > nWidth) ? (objTempPos.x-nWidth+1) : 0;
			long lY = (lMaxLine - lLineCnt) * (m_lfHeight+m_lLineInterval);
			MyScrollToPosition(CPoint(lX,lY));					//�_�����W�ňʒu��n���Ă���
			CPoint caretPos(objTempPos.x,(lJumpLine*(m_lfHeight+m_lLineInterval)));
			//�_�����W�|���f�o�C�X���W
			caretPos.x -= GetScrollPosLogX();
			caretPos.y -= GetScrollPosLogY();
			caretPos += m_RulerPos;
			caretPos += m_LineNoPos;
			MySetCaretPos(caretPos);
		}
	}
	else
	{
		long lX = (objTempPos.x > nWidth) ? (objTempPos.x-nWidth+1) : 0;
		MyScrollToPosition(CPoint(lX,0));					//�_�����W�ňʒu��n���Ă���
		long lY = lJumpLine * (m_lfHeight+m_lLineInterval);
		CPoint caretPos(objTempPos.x,lY);
		//�_�����W�|���f�o�C�X���W
		caretPos.x -= GetScrollPosLogX();
		caretPos.y -= GetScrollPosLogY();
		caretPos += m_RulerPos;
		caretPos += m_LineNoPos;
		MySetCaretPos(caretPos);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w��s�A��ւ̃W�����v����											|*/
/*|	���	�F																		|*/
/*|	���l	�F�W�����v��s���\���G���A���ɂ���΃X�N���[�����Ȃ�					|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::Jump2LineOffsetEx
	(	long	lJumpLine,			//�W�����v��s�ԍ��i�[���I���W���j
		long	lJumpOffset			//�W�����v���ԍ�
	)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	if( (m_lfWidth==0) || (m_lfHeight==0) )
		return;
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//�X�N���[���ʒu�̍s�ԍ��擾
	long lScrollLine = GetScrollPosLogY() / (m_lfHeight+m_lLineInterval);

	//�N���C�A���g�̈�ɕ\���ł���s��
	long lLineCnt = clientRect.Height() / (m_lfHeight+m_lLineInterval);

	if( lJumpLine >= lScrollLine &&  // �W�����v��s�ԍ����\���͈͓�
		lJumpLine < (lScrollLine+lLineCnt)
	)
	{
		//�w��I�t�Z�b�g�ȉ��ʒu�擾�i�_�����W�Ŗ߂��Ă���j
		CPoint	objTempPos(0,0);
		GetPosition3(lJumpLine,lJumpOffset,&objTempPos);

		long lX = (objTempPos.x > nWidth) ? (objTempPos.x-nWidth+1) : 0;
		long lY = GetScrollPosLogY();
		MyScrollToPosition(CPoint(lX,lY));	//�_�����W�ňʒu��n���Ă���
		lY += (lJumpLine-lScrollLine)*(m_lfHeight+m_lLineInterval);
		CPoint caretPos(objTempPos.x,lY);
		//�_�����W�|���f�o�C�X���W
		caretPos.x -= GetScrollPosLogX();
		caretPos.y -= GetScrollPosLogY();
		caretPos += m_RulerPos;
		caretPos += m_LineNoPos;
		MySetCaretPos(caretPos);
	}
	else // �W�����v��s�ԍ����\���͈͊O
	{
		Jump2LineOffset(lJumpLine,lJumpOffset);
	}
}

/////////////////////////////////////////////////////////////////////////////
//	CEditorView �v���e�N�g�֐��i�̈�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�N���C�A���g�̈�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyGetClientRect
	(	CRect*	pobjRect
	)
{
	GetClientRect(pobjRect);

	//�s�ԍ���\�����Ă���ꍇ
	if( m_bDispLineNo )
	{
		//�s�ԍ��\�����Ă��镪�����N���C�A���g�̈悪�������Ȃ�
		pobjRect->left += m_lLineNoWidth;
	}
	//���[����\�����Ă���ꍇ
	if( m_bDispRuler )
	{
		//���[����\�����Ă��镪�����N���C�A���g�̈悪�������Ȃ�
		pobjRect->top += m_RulerPos.y;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	CEditorView �v���e�N�g�֐��i����w�b�_���t�b�^�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����w�b�_�t�H�[�}�b�g��͏���										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::ParsePrintHeaderFormat
	(	CString&	objStrParseIn,
		CString&	objStrParseResult,
		CPrintInfo*	pInfo
	)
{
	char	szBuf[256];
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�N���A
	objStrParseResult = _T("");

	for(long lCnt=0; lCnt<objStrParseIn.GetLength(); )
	{
		char cByte = objStrParseIn.GetAt(lCnt);

		if(IsDBCSLeadByteEx(0,cByte)) //�Q�o�C�g����
		{
			//���̂܂܃R�s�[
			objStrParseResult += cByte;
			lCnt++;
			objStrParseResult += objStrParseIn.GetAt(lCnt);
			lCnt++;
		}
		else //�P�o�C�g����
		{
			if( (cByte=='&') && ((lCnt+1)<objStrParseIn.GetLength()) )
			{
				//���̕����擾
				lCnt++;
				char cNextByte = objStrParseIn.GetAt(lCnt);
				if( cNextByte=='f' ) // &f �Ńt���p�X
				{
					objStrParseResult += pDoc->GetTitle();
				}
				else if( cNextByte=='n' ) // &n �Ńt�@�C����
				{
					pDoc->MyGetFileName(szBuf);
					objStrParseResult += szBuf;
				}
				else if( cNextByte=='d' ) // &d �œ��t
				{
					CTime objTime = CTime::GetCurrentTime();
					objStrParseResult += objTime.Format("%Y/%m/%d");
				}
				else if( cNextByte=='t' ) // &t �Ŏ���
				{
					CTime objTime = CTime::GetCurrentTime();
					objStrParseResult += objTime.Format("%H:%M:%S");
				}
				else if( cNextByte=='p' ) // &p �Ńy�[�W�ԍ�
				{
					sprintf(szBuf,"%d",	pInfo->m_nCurPage);
					objStrParseResult += szBuf;
				}
				else if( cNextByte=='P' ) // &P �ő��y�[�W��
				{
					sprintf(szBuf,"%d",	pInfo->GetMaxPage());
					objStrParseResult += szBuf;
				}
				else
				{
					//���̂܂܃R�s�[
					objStrParseResult += cByte;
					objStrParseResult += cNextByte;
				}
				lCnt++;
			}
			else
			{
				//���̂܂܃R�s�[
				objStrParseResult += cByte;
				lCnt++;
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����t�b�^�t�H�[�}�b�g��͏���										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::ParsePrintFooterFormat
	(	CString&	objStrParseIn,
		CString&	objStrParseResult,
		CPrintInfo* pInfo
	)
{
	ParsePrintHeaderFormat(objStrParseIn,objStrParseResult,pInfo);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F������y�[�W���J�E���g����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CEditorView::CountPrintMaxPage
	(	CPrintInfo* 	pInfo,
		long			lLinePerPrintPage,
		long			lPrintCharWidth
	)
{
	long	lCnt=0;
	UINT	unMaxPage=0;
	long	lFoldLineCnt=0;

	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(m_bPrint2Dangumi==FALSE) //�P�i�g�݈��
	{
		unMaxPage = pDoc->GetLineCount() / lLinePerPrintPage;
		if( (pDoc->GetLineCount() % lLinePerPrintPage) != 0 )
			unMaxPage += 1;
		return unMaxPage;
	}
	else //�Q�i�g�݈��
	{
		unMaxPage = pDoc->GetLineCount() / (lLinePerPrintPage*2);
		if( (pDoc->GetLineCount() % (lLinePerPrintPage*2)) != 0 )
			unMaxPage += 1;
		return unMaxPage;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C�����e�P�y�[�W������o�͏���									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::MyPrintOut1Page
	(	CDC*			pDC,
		CPrintInfo*		pInfo,
		long			lLinePerPrintPage,
		long			lPrintLineHeight,
		long			lPrintCharWidth
	)
{
	int		nRet=0;
	ULONG	lCnt=0;
	ULONG	lLineCnt=0;
	ULONG	lLineStart=0;
	ULONG	lLineStop=0;
	CString	objLineNoStr=_T("");
	CLine*	pobjLine=NULL;
	long	lMargin=0;
	BOOL	bEnd=FALSE;
	int		nDiffLineKind;
	int		nDirCmpLineKind;


	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(m_bPrint2Dangumi==FALSE) //�P�i�g�݈��
	{
		lLineStart = (pInfo->m_nCurPage - 1) * lLinePerPrintPage;
		lLineStop  = lLineStart + lLinePerPrintPage;
		if(lLineStop > (ULONG)pDoc->GetLineCount())
		{
			lLineStop = pDoc->GetLineCount();
		}
		for(lCnt=0,lLineCnt=lLineStart; lLineCnt<lLineStop; lCnt++,lLineCnt++)
		{
			objLineNoStr = _T("");
			if( m_bPrintLineNo ) //�s�ԍ��������
				objLineNoStr.Format(" %6d | ",lLineCnt+1);
			else
				objLineNoStr = _T("  ");
			//�t�@�C����r�A�f�B���N�g����r�ł̑���̖ڈ�
			nDiffLineKind = pDoc->GetDiffLineKind(lLineCnt);
			nDirCmpLineKind = pDoc->GetDirCmpLineKind(lLineCnt);
			if(nDiffLineKind == DIFF_LINEKIND_ADDED)
				objLineNoStr += _T("+ ");
			else if(nDiffLineKind == DIFF_LINEKIND_DELETED)
				objLineNoStr += _T("- ");
			else if(nDirCmpLineKind == DIRCMP_LINEKIND_DIFF)
				objLineNoStr += _T("! ");
			else
				objLineNoStr += _T("  ");

			pDC->TextOut( pInfo->m_rectDraw.left,
						  ((lCnt+2)*lPrintLineHeight) + pInfo->m_rectDraw.top,
						  objLineNoStr);

			//�P�s
			pobjLine = pDoc->GetOneLine(lLineCnt);
			if(pobjLine != NULL) //������
			{
				lMargin = objLineNoStr.GetLength()*lPrintCharWidth;
				lMargin += pInfo->m_rectDraw.left;
				MyTextOutForPrint(  pDC,
									lMargin,
									((lCnt+2)*lPrintLineHeight) + pInfo->m_rectDraw.top,
									pobjLine,
									lPrintCharWidth,
									lLineCnt,
									pInfo->m_rectDraw.Width());
			}
		}
	}
	else //�Q�i�g�݈��
	{
		//////////////////////////////
		//		���̒i����		//
		//////////////////////////////
		lLineStart = (pInfo->m_nCurPage - 1) * (lLinePerPrintPage*2);
		lLineStop  = lLineStart + lLinePerPrintPage;
		if(lLineStop >= (ULONG)pDoc->GetLineCount())
		{
			lLineStop = pDoc->GetLineCount();
			bEnd=TRUE;//���i�󎚂ł����܂�
		}
		for(lCnt=0,lLineCnt=lLineStart; lLineCnt<lLineStop; lCnt++,lLineCnt++)
		{
			objLineNoStr = _T("");
			if( m_bPrintLineNo  ) //�s�ԍ��������
				objLineNoStr.Format(" %6d | ",lLineCnt+1);
			else
				objLineNoStr = _T("  ");
			//�t�@�C����r�A�f�B���N�g����r�ł̑���̖ڈ�
			nDiffLineKind = pDoc->GetDiffLineKind(lLineCnt);
			nDirCmpLineKind = pDoc->GetDirCmpLineKind(lLineCnt);
			if(nDiffLineKind == DIFF_LINEKIND_ADDED)
				objLineNoStr += _T("+ ");
			else if(nDiffLineKind == DIFF_LINEKIND_DELETED)
				objLineNoStr += _T("- ");
			else if(nDirCmpLineKind == DIRCMP_LINEKIND_DIFF)
				objLineNoStr += _T("! ");
			else
				objLineNoStr += _T("  ");

			pDC->TextOut( pInfo->m_rectDraw.left,
						  ((lCnt+2)*lPrintLineHeight) + pInfo->m_rectDraw.top,
						  objLineNoStr);

			//�P�s
			pobjLine = pDoc->GetOneLine(lLineCnt);
			if(pobjLine != NULL) //������
			{
				lMargin = objLineNoStr.GetLength()*lPrintCharWidth;
				lMargin += pInfo->m_rectDraw.left;
				MyTextOutForPrint(  pDC,
									lMargin,
									((lCnt+2)*lPrintLineHeight) + pInfo->m_rectDraw.top,
									pobjLine,
									lPrintCharWidth,
									lLineCnt,
									pInfo->m_rectDraw.Width()/2 - 2*lPrintCharWidth);
			}
		}
		//////////////////////////////
		//		�E�̒i����		//
		//////////////////////////////
		if( bEnd != FALSE )
			return;
		lLineStart = lLineStop;
		lLineStop  = lLineStart + lLinePerPrintPage;
		if(lLineStop >= (ULONG)pDoc->GetLineCount())
		{
			lLineStop = pDoc->GetLineCount();
		}
		for(lCnt=0,lLineCnt=lLineStart; lLineCnt<lLineStop; lCnt++,lLineCnt++)
		{
			objLineNoStr = _T("");
			if( m_bPrintLineNo ) //�s�ԍ��������
				objLineNoStr.Format(" %6d | ",lLineCnt+1);
			else
				objLineNoStr = _T("  ");

			//�t�@�C����r�A�f�B���N�g����r�ł̑���̖ڈ�
			nDiffLineKind = pDoc->GetDiffLineKind(lLineCnt);
			nDirCmpLineKind = pDoc->GetDirCmpLineKind(lLineCnt);
			if(nDiffLineKind == DIFF_LINEKIND_ADDED)
				objLineNoStr += _T("+ ");
			else if(nDiffLineKind == DIFF_LINEKIND_DELETED)
				objLineNoStr += _T("- ");
			else if(nDirCmpLineKind == DIRCMP_LINEKIND_DIFF)
				objLineNoStr += _T("! ");
			else
				objLineNoStr += _T("  ");

			pDC->TextOut( pInfo->m_rectDraw.left + pInfo->m_rectDraw.Width()/2,
						  ((lCnt+2)*lPrintLineHeight) + pInfo->m_rectDraw.top,
						  objLineNoStr);

			//�P�s
			pobjLine = pDoc->GetOneLine(lLineCnt);
			if(pobjLine != NULL) //������
			{
				lMargin = pInfo->m_rectDraw.Width()/2 + objLineNoStr.GetLength()*lPrintCharWidth;
				lMargin += pInfo->m_rectDraw.left;
				MyTextOutForPrint(  pDC,
									lMargin,
									((lCnt+2)*lPrintLineHeight) + pInfo->m_rectDraw.top,
									pobjLine,
									lPrintCharWidth,
									lLineCnt,
									pInfo->m_rectDraw.Width());
			}
		}
	}
}


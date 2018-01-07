// LineManager.cpp: CLineManager �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxtempl.h>			//for CTypedPtrList
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "StatusBarDisp.h"		//�X�e�[�^�X�o�[�\�����\����
#include "Editor.h"					//�A�v���P�[�V�����N���X
#include "BitmapClient.h"
#include "Operation.h"
#include "OperationManager.h"
#include "MainFrm.h"
#include "Line.h"					//�P�s�N���X
#include "CodeConvManager.h"
#include "LineManager.h"

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
CLineManager::CLineManager()
{
	m_pApp = (CEditorApp*)AfxGetApp();

	m_lMarkLineCount = 0;					// �}�[�N�s��
	m_lCRCount = 0;
	m_lLFCount = 0;
	m_lKaigyoKind = KAIGYO_CRLF;
	m_lTabSize = m_pApp->GetProfileInt("CLineManager","TabSize",4);//�^�u�T�C�Y
	m_lMaxLineLength = 0;					//�P�s�̒����̍ő�l
	m_objStringArray.SetSize(0);				//�����T�C�Y�[��
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.07.07 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLineManager::~CLineManager()
{
	//�ݒ�����W�X�g���֕ۑ�
	m_pApp->WriteProfileInt("CLineManager","TabSize",m_lTabSize);

	Clear();
}

/////////////////////////////////////////////////////////////////////////////
// CLineManager �N���X�̃p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F������																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::Initialize()
{
	//�s�N���A
	Clear();

	m_lMarkLineCount = 0;					// �}�[�N�s��
	m_lTabSize = m_pApp->GetProfileInt("CLineManager","TabSize",4);//�^�u�T�C�Y
	m_lMaxLineLength = 0;					//�P�s�̒����̍ő�l
	m_objStringArray.SetSize(0);				//�����T�C�Y�[��
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�V���A���C�Y															|*/
/*|	���	�F																		|*/
/*|	���l	�FCArchive exception: endOfFile.�̗�O���f�o�b�O�E�C���h�E�ɕ\�������	|*/
/*|	����	�F1998.07.07 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::Serialize(CArchive& ar)
{
	long lCRCount=0;
	long lLFCount=0;
	long	lCnt=0;
	long	lMaxOffset=0;

	if(ar.IsStoring()) //�ۑ�
	{
		for(lCnt=0; lCnt<m_objStringArray.GetSize(); lCnt++)
		{
			CLine* pobjLine = (CLine*)m_objStringArray.GetAt(lCnt);
			ar.WriteString((const char*)pobjLine->String());

			if(lCnt < m_objStringArray.GetSize() - 1)
			{
				if(m_lKaigyoKind == KAIGYO_CRLF)
					ar.WriteString("\r\n");
				else if(m_lKaigyoKind == KAIGYO_LF)
					ar.WriteString("\n");
				else if(m_lKaigyoKind == KAIGYO_CR)
					ar.WriteString("\r");
				else
					ar.WriteString("\r\n");
			}
		}
	}
	else //�ǂݏo��
	{
		//������
		Clear();

		int  nNowRet=0;
		int  nBeforeRet=0;
		CLine* pobjLine=NULL;
		m_pApp->PrepareMyReadString();
		for(lCnt=0; ;lCnt++)//�P�s�Âǂݍ���
		{
			lCRCount=0;
			lLFCount=0;
			CString objStr = _T("");
			nNowRet = m_pApp->MyReadString(ar,objStr,&lCRCount,&lLFCount);
			if( nNowRet == 0 ) //EOF�I��
			{
				if( objStr.IsEmpty() ) //�f�[�^�Ȃ�
					break;
			}

			//MyReadString ���^�[���l�ێ�
			nBeforeRet = nNowRet;

			if(m_lCRCount==0 && m_lLFCount==0) //CR, LF �̐��ۑ�
			{
				m_lCRCount = lCRCount;
				m_lLFCount = lLFCount;

				if(lCRCount==1 && lLFCount==1)
					m_lKaigyoKind=KAIGYO_CRLF;
				else if(lCRCount==0 && lLFCount==1)
					m_lKaigyoKind=KAIGYO_LF;
				else if(lCRCount==1 && lLFCount==0)
					m_lKaigyoKind=KAIGYO_CR;
				else
					m_lKaigyoKind=KAIGYO_CRLF;
			}

			if( lCnt==0 ) //�P�s�ڂ̓ǂݍ���
			{
				pobjLine = new CLine((const unsigned char*)(LPCTSTR)objStr);
				m_objStringArray.Add((CObject*)pobjLine);
			}
			else //�Q�s�ڈȍ~
			{
				//�ŏI�s�ɃZ�b�g
				pobjLine = (CLine*)m_objStringArray.GetAt(m_objStringArray.GetSize()-1);
				pobjLine->Set((const unsigned char*)(LPCTSTR)objStr);
			}

			//�P�s�̍ő�I�t�Z�b�g�v�Z
			long lTabCnt = pobjLine->GetTabCount();
			lMaxOffset = objStr.GetLength() - lTabCnt + (lTabCnt*GetTabSize());
			if(m_lMaxLineLength < lMaxOffset)
				m_lMaxLineLength = lMaxOffset;

			if(nNowRet == 1) //���s�I�����Ă����ꍇ
			{
				//���̍s�̕��o�^
				pobjLine = new CLine();
				m_objStringArray.Add((CObject*)pobjLine);
			}
			else //�d�n�e�I�����Ă����ꍇ
			{
				break;
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�V���A���C�Y�i�o�C�i�����[�h�j										|*/
/*|	���	�F																		|*/
/*|	���l	�FCArchive exception: endOfFile.�̗�O���f�o�b�O�E�C���h�E�ɕ\�������	|*/
/*|	����	�F                   													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::SerializeBinary(CArchive& ar)
{
	long	lCnt=0;
	long	lSumCnt=0;
	UINT	unReaded=0;
	long	lOffset=0;
	BYTE	byBuf[32];
	CLine*  pobjLine=NULL;
	char	szAsciiDataBuf[24];
	char	szAsciiData[128];
	strcpy(szAsciiData,ASCII_DATA);

	if(ar.IsStoring()) //�ۑ��͖��T�|�[�g
		return;

	//������
	Clear();

	//�����݋֎~Ӱ�޽ð���߲ݐݒ�
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	pWnd->SetReadOnlyMode(TRUE);

	//�Q�[�W��������
	CLine objBinaryLine;
	objBinaryLine.Set((const unsigned char*)"");
	AppendBinaryLine(objBinaryLine);
	objBinaryLine.Set((const unsigned char*)"| Offset | 0001 0203 0405 0607 0809 0A0B 0C0D 0E0F |      ASCII DATA");
	AppendBinaryLine(objBinaryLine);

	objBinaryLine.Set((const unsigned char*)"");
	memset(szAsciiDataBuf,0,sizeof(szAsciiDataBuf));
	lSumCnt=0;
	for(;;)
	{
		TRY
		{
			//�P�U�o�C�g���ǂݍ���
			memset(byBuf,0,sizeof(byBuf));
			unReaded = ar.Read(byBuf,16);
			if(unReaded <= 0)
				break;
		}
		CATCH_ALL(e)
		{
			break;
		}
		END_CATCH_ALL

		for(lOffset=0; lOffset<(signed)unReaded; lOffset++, lSumCnt++)
		{
			if(!(lSumCnt%256))
			{
				if(objBinaryLine.GetLength()!=0)
				{
					//�����ް�������
					objBinaryLine.Catenate((const unsigned char*)"     ");
					objBinaryLine.Catenate((const unsigned char*)szAsciiDataBuf);
					memset(szAsciiDataBuf,0,sizeof(szAsciiDataBuf));
					//���܂܂ł̃o�C�i���\�����ʂ�o�^
					AppendBinaryLine(objBinaryLine);
				}
				//�Z�p���[�^�o�^
				CLine objTempLine((const unsigned char*)"|--------|-----------------------------------------|");
				AppendBinaryLine(objTempLine);
			}
			if((lSumCnt!=0)&&(!(lSumCnt%2)))
			{
				objBinaryLine.Catenate((const unsigned char*)" ");
			}
			if((!(lSumCnt%16)))
			{
				if( (lSumCnt%256)!=0 )
				{
					if(objBinaryLine.GetLength()!=0)
					{
						//�����ް�������
						objBinaryLine.Catenate((const unsigned char*)"    ");
						objBinaryLine.Catenate((const unsigned char*)szAsciiDataBuf);
						memset(szAsciiDataBuf,0,sizeof(szAsciiDataBuf));
						//���܂܂ł̃o�C�i���\�����ʂ�o�^
						AppendBinaryLine(objBinaryLine);
					}
				}
				//�N���A
				objBinaryLine.Set((const unsigned char*)"");
				char szBuf[64];
				sprintf(szBuf,"| %06X | ",lSumCnt);
				objBinaryLine.Set((const unsigned char*)szBuf);
			}
			char szBuf[64];
			sprintf(szBuf,"%02X",byBuf[lOffset]);
			objBinaryLine.Catenate((const unsigned char*)szBuf);

			//////////////////////////
			//	�`�r�b�h�h�ł̕\��	//
			//////////////////////////
			if(byBuf[lOffset] <= 0x7f)
				szAsciiDataBuf[(lSumCnt%16)] = szAsciiData[byBuf[lOffset]];
			else
				szAsciiDataBuf[(lSumCnt%16)] = '.';
		}
	}
	if(objBinaryLine.GetLength()!=0)
	{
		//�����ް�������
		objBinaryLine.PadSpace(55);
		objBinaryLine.Catenate((const unsigned char*)szAsciiDataBuf);
		memset(szAsciiDataBuf,0,sizeof(szAsciiDataBuf));
		//���܂܂ł̃o�C�i���\�����ʂ�o�^
		AppendBinaryLine(objBinaryLine);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�o�C�g���擾����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::GetBytes
	(	long	lLimitLine,				//�w��s
		long*	plLimitBytes,			//�w��s�܂ł̃o�C�g���ԋp�p�o�b�t�@
		long*	plSumBytes				//���o�C�g���ԋp�p�o�b�t�@
	)
{
	long	lCnt=0;
	CLine*	pobjLine=NULL;

	//������
	(*plLimitBytes) = 0;
	(*plSumBytes) = 0;

	for(lCnt=0; lCnt<GetLineCount(); lCnt++)
	{
		pobjLine = GetOneLine(lCnt);
		if(pobjLine==NULL)
			continue;

		//���o�C�g��
		(*plSumBytes) += pobjLine->GetLength();
		if(lCnt < GetLineCount()-1)
		{
			if(m_lKaigyoKind == KAIGYO_CRLF)
				(*plSumBytes) += 2;
			else if(m_lKaigyoKind == KAIGYO_LF)
				(*plSumBytes) += 1;
			else if(m_lKaigyoKind == KAIGYO_CR)
				(*plSumBytes) += 1;
			else
				(*plSumBytes) += 2;
		}

		//�w��s�܂ł̃o�C�g��
		if(lCnt < lLimitLine)
			(*plLimitBytes) += pobjLine->GetLength();
		if(lCnt < lLimitLine)
		{
			if(m_lKaigyoKind == KAIGYO_CRLF)
				(*plLimitBytes) += 2;
			else if(m_lKaigyoKind == KAIGYO_LF)
				(*plLimitBytes) += 1;
			else if(m_lKaigyoKind == KAIGYO_CR)
				(*plLimitBytes) += 1;
			else
				(*plLimitBytes) += 2;
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s���擾����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLineManager::GetLineCount()
{
	return m_objStringArray.GetSize();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�s�擾����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLine* CLineManager::GetOneLine
	(	long		lLineNo		//�s�ԍ�
	)
{
	if(!(lLineNo < m_objStringArray.GetSize()))	//�Ȃ�
	{
		return NULL;
	}
	else	//����
	{
		CLine* pobjLine = (CLine*)m_objStringArray.GetAt(lLineNo);
		//���݂̕����񒷕ێ�(1999.03.21 Add for StatusBar Pane ID_INDICATOR_BYTE)
		pobjLine->SaveLength();
		return pobjLine;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�s�ݒ菈��															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::SetOneLine
	(	long		lLineNo,		//�s�ԍ�
		CLine*		pobjLine		//�s
	)
{
	if( pobjLine==NULL)
		return;

	//�ݒ�s�̍s���̃I�t�Z�b�g�v�Z
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());

	//�P�s�̒����̍ő�l���X�V
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;

	//�P�s�ݒ�
	m_objStringArray.SetAt(lLineNo,(CObject*)pobjLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w��s�폜����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::DeleteOneLine
	(	long	lLineNo			//�폜�Ώۍs
	)
{
	CLine* pobjLine = NULL;
	pobjLine = (CLine*)m_objStringArray.GetAt(lLineNo);
	if(pobjLine != NULL)
	{
		delete pobjLine;
		m_objStringArray.RemoveAt(lLineNo);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s�}������															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::InsertOneLine
	(	long	lLineNo,
		CLine*	pobjLine
	)
{
	if( pobjLine==NULL)
		return;

	//�}���s�̍s���̃I�t�Z�b�g�v�Z
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());

	//�P�s�̒����̍ő�l���X�V
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;

	//�P�s�}��
	m_objStringArray.InsertAt(lLineNo,(CObject*)pobjLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�s�̒����̍ő�l���擾����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLineManager::GetMaxLineLength()
{
	return m_lMaxLineLength;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w��s�̕����񒷎擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLineManager::GetLineLength(long lLineNo)
{
	if(lLineNo >= m_objStringArray.GetSize())
		return 0;

	CLine* pobjLine = (CLine*)m_objStringArray.GetAt(lLineNo);
	return pobjLine->GetLength();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����o�^����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AddCharacter
	(	long	lLine,					//�����o�^�Ώۂ̍s�ԍ��i��۵ؼ�݁j
		long	lOffset,				//�����o�^�Ώۂ̗�ԍ��i�޲ĒP�ʁA��۵ؼ�݁j
		UINT	n1stChar,				//�o�^��������޲�
		UINT	n2ndChar,				//�o�^��������޲�
		BOOL	bInsertKey				//�}���^�㏑��Ӱ���׸�(TRUE:�}���AFALSE:�㏑��)
	)
{
	CLine*	pobjLine = NULL;

	//////////////////////////////
	//	�o�^�Ώۍs�擾
	//////////////////////////////
	if(lLine >= GetLineCount())//�w�肳�ꂽ�s���ް����܂��Ȃ�
		pobjLine = new CLine();
	else //�w�肳�ꂽ�s���ް������ɗL��
		pobjLine = (CLine*)m_objStringArray.GetAt(lLine);

	if(lOffset > pobjLine->GetLength()) //�����o�^�ꏊ�������Ō�������
		bInsertKey = TRUE;//�}���ɂ���

	//////////////////////////////////////
	//	�����o�^
	//////////////////////////////////////
	if(bInsertKey)//�}��Ӱ��
		pobjLine->InsertChar(lOffset,n1stChar,n2ndChar);
	else //�㏑��Ӱ��
		pobjLine->OverWriteChar(lOffset,n1stChar,n2ndChar);

	//////////////////////////////
	//	�s���ēo�^
	//////////////////////////////
	if(lLine >= GetLineCount())//�w�肳�ꂽ�s���ް����܂��Ȃ�
		m_objStringArray.Add((CObject*)pobjLine);
	else //�w�肳�ꂽ�s���ް������ɗL��
		m_objStringArray.SetAt(lLine,(CObject*)pobjLine);

	//////////////////////////////////////////
	//	�P�s�̒����̍ő�l���X�V
	//////////////////////////////////////////

	//�s���̃I�t�Z�b�g�v�Z
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());

	//�P�s�̒����̍ő�l���X�V
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s�R�[�h����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::ProcessReturnCode
	(	long	lLine,				//�����o�^�Ώۂ̍s�ԍ�
		long	lOffset				//�����o�^�Ώۂ̗�ԍ��i�޲ĒP�ʁj
	)
{
	CLine*	pobjLine = NULL;

	if(lLine >= GetLineCount())//�w�肳�ꂽ�s���ް����܂��Ȃ�
	{
		pobjLine = new CLine();
		m_objStringArray.Add((CObject*)pobjLine);
		pobjLine = new CLine();
		m_objStringArray.Add((CObject*)pobjLine);
	}
	else //�w�肳�ꂽ�s���ް�������
	{
		pobjLine = (CLine*)m_objStringArray.GetAt(lLine);
		if(lOffset >= pobjLine->GetLength())//�s���ł̉��s
		{
			pobjLine = new CLine();
			//���̍s�ɑ}��
			m_objStringArray.InsertAt(lLine+1,(CObject*)pobjLine);
		}
		else //�s�̓r���ł̉��s
		{
			//�L�����b�g�ʒu������ێ�
			CLine* pobjNextLine = new CLine();
			pobjNextLine->Set(pobjLine->Data(lOffset));
			//�L�����b�g�ʒu�łP�s��؂�
			pobjLine->SetAt(lOffset,0);
			//���s�ɂ���Đ؂�o���ꂽ�s�����̍s�Ƃ���
			m_objStringArray.InsertAt(lLine+1,(CObject*)pobjNextLine);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FDELETE�R�[�h����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::ProcessDeleteCode
	(	long	lLine,					//�����o�^�Ώۂ̍s�ԍ�
		long	lOffset,				//�����o�^�Ώۂ̗�ԍ��i�޲ĒP�ʁj
		unsigned char*	pc1stChar,		//DELETE���ꂽ���o�C�g����(OUT)
		unsigned char*	pc2ndChar		//DELETE���ꂽ���o�C�g����(OUT)
	)
{
	CLine*	pobjLine = NULL;

	if(lLine >= GetLineCount())//�w�肳�ꂽ�s���ް����܂��Ȃ�
	{
		//���ɉ������Ȃ�
		(*pc1stChar) = 0;
		(*pc2ndChar) = 0;
		return;
	}

	//�w�肳�ꂽ�s���ް�������
	pobjLine = (CLine*)m_objStringArray.GetAt(lLine);
	if(lOffset >= pobjLine->GetLength())//�s���ł�DELETE
	{
		if((lLine+1) >= GetLineCount())//���̍s�Ȃ�
		{
			//���ɉ������Ȃ�
			(*pc1stChar) = 0;
			(*pc2ndChar) = 0;
			return;
		}
		else //���̍s����
		{
			CLine* pobjNextLine = (CLine*)m_objStringArray.GetAt(lLine+1);
			//���ݍs�Ɏ��s��A��
			pobjLine->Catenate(pobjNextLine->String());
			//���s���폜
			m_objStringArray.RemoveAt(lLine+1);
			delete pobjNextLine;
			//���s�R�[�h��DELETE�ɑ�������
			(*pc1stChar) = '\r';
			(*pc2ndChar) = '\n';

			//�s���̃I�t�Z�b�g�v�Z
			long lTabCnt = pobjLine->GetTabCount();
			long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
			//�P�s�̒����̍ő�l���X�V
			if(m_lMaxLineLength < lMaxOffset)
				m_lMaxLineLength = lMaxOffset;

			return;
		}
	}
	else //�s�̓r���ł�DELETE
	{
		//�����폜
		pobjLine->DeleteChar(lOffset,pc1stChar,pc2ndChar);
		return;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FBACK SPACE�R�[�h����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::ProcessBackSpaceCode
	(	long	lLine,				//�����o�^�Ώۂ̍s�ԍ�
		long	lOffset,			//�����o�^�Ώۂ̗�ԍ��i�޲ĒP�ʁj
		unsigned char*	pc1stChar,	//BACK SPACE���ꂽ���o�C�g����(OUT)
		unsigned char*	pc2ndChar	//BACK SPACE���ꂽ���o�C�g����(OUT)
	)
{
	CLine*	pobjLine = NULL;

	if(lLine >= GetLineCount())//�w�肳�ꂽ�s���ް����܂��Ȃ�
	{
		//���ɉ������Ȃ�
		(*pc1stChar) = 0;
		(*pc2ndChar) = 0;
		return;
	}

	//�w�肳�ꂽ�s���ް�������
	CLine* pobjPrevLine = NULL;
	pobjLine = (CLine*)m_objStringArray.GetAt(lLine);
	if(lOffset == 0)//�s���ł�BACK SPACE
	{
		if(lLine == 0)//�O�̍s�Ȃ�
		{
			//���ɉ������Ȃ�
			(*pc1stChar) = 0;
			(*pc2ndChar) = 0;
			return;
		}
		else //�O�̍s����
		{
			pobjPrevLine = (CLine*)m_objStringArray.GetAt(lLine-1);
			//�O�̍s�Ɍ��ݍs��A��
			pobjPrevLine->Catenate(pobjLine->String());
			//���ݍs���폜
			m_objStringArray.RemoveAt(lLine);
			delete pobjLine;
			//���s�R�[�h��BACK SPACE�ɑ�������
			(*pc1stChar) = '\r';
			(*pc2ndChar) = '\n';

			//�s���̃I�t�Z�b�g�v�Z
			long lTabCnt = pobjPrevLine->GetTabCount();
			long lMaxOffset = pobjPrevLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
			//�P�s�̒����̍ő�l���X�V
			if(m_lMaxLineLength < lMaxOffset)
				m_lMaxLineLength = lMaxOffset;

			return;
		}
	}
	else //�s�̓r���ł�BACK SPACE
	{
		//�����폜
		pobjLine->BackSpaceChar(lOffset,pc1stChar,pc2ndChar);
		return;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�^�u�T�C�Y�擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLineManager::GetTabSize()
{
	return m_lTabSize;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�^�u�T�C�Y�ݒ菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::SetTabSize(long lTabSize)
{
	m_lTabSize = lTabSize;

	//�ݒ�����W�X�g���֕ۑ�
	m_pApp->WriteProfileInt("CLineManager","TabSize",m_lTabSize);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I��͈̓e�L�X�g�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLine* CLineManager::MyGetSelectedText
	(	long	lSelectStartLine,			//�I���J�n�s�ԍ�
		long	lSelectStartOffset,			//�I���J�n��ԍ�
		long	lSelectEndLine,				//�I���I���s�ԍ�
		long	lSelectEndOffset			//�I���I����ԍ�
	)
{
	CLine* pobjLine = new CLine();

	if( lSelectStartLine == lSelectEndLine ) //�I���s���P�s�̂�
	{
		//�I��Ώۍs�擾
		CLine* pobjTempLine = (CLine*)m_objStringArray.GetAt(lSelectStartLine);
		ASSERT(pobjTempLine != NULL);
		pobjLine->Set(pobjTempLine->Data(lSelectStartOffset));
		pobjLine->SetAt((lSelectEndOffset-lSelectStartOffset),0);
		return pobjLine;
	}
	else //�I���s�������s�ɂ킽��
	{
		for(long lLineNo=lSelectStartLine; lLineNo<=lSelectEndLine; lLineNo++)
		{
			//�I��Ώۍs�擾
			CLine* pobjTempLine = (CLine*)GetOneLine(lLineNo);
			if(!pobjTempLine)
				continue;

			if(lLineNo == lSelectStartLine)//�I���J�n�s
			{
				pobjLine->Set(pobjTempLine->Data(lSelectStartOffset));
			}
			else if(lLineNo == lSelectEndLine)//�I���I���s
			{
				pobjLine->Catenate((const unsigned char*)"\r\n");
				CLine* pobjTemp2Line = new CLine();
				pobjTemp2Line->Set(pobjTempLine->String());
				pobjTemp2Line->SetAt(lSelectEndOffset,0);
				pobjLine->Catenate(pobjTemp2Line->String());
				delete pobjTemp2Line;
			}
			else //�r���s
			{
				pobjLine->Catenate((const unsigned char*)"\r\n");
				pobjLine->Catenate(pobjTempLine->String());
			}
		}

		return pobjLine;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�q�d�o���ʂP�s�ǉ�����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendGrepLine
	(	CLine&	objGrepLine				// �f�q�d�o���ʂP�s
	)
{
	CLine* pobjLine = new CLine(objGrepLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//�P�s�̍ő�I�t�Z�b�g�v�Z
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�g�s�s�o�_�E�����[�h���ʂP�s�ǉ�										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendHttpLine
	(	CLine&	objHttpLine				// ���ʂP�s
	)
{
	CLine* pobjLine = new CLine(objHttpLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//�P�s�̍ő�I�t�Z�b�g�v�Z
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����R�[�h�ϊ����ʂP�s�ǉ�����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendConvLine
	(	CLine&	objConvLine				// �����R�[�h�ϊ����ʂP�s
	)
{
	CLine* pobjLine = new CLine(objConvLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//�P�s�̍ő�I�t�Z�b�g�v�Z
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_���v���ʂP�s�ǉ�����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendDumpLine
	(	CLine&	objDumpLine				// �_���v���ʂP�s
	)
{
	CLine* pobjLine = new CLine(objDumpLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//�P�s�̍ő�I�t�Z�b�g�v�Z
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[�h�J�E���g���ʂP�s�ǉ�����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendWCountLine
	(	CLine&	objWCountLine			// ���[�h�J�E���g���ʂP�s
	)
{
	CLine* pobjLine = new CLine(objWCountLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//�P�s�̍ő�I�t�Z�b�g�v�Z
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���������ʂP�s�ǉ�����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendFSearchLine
	(	CLine&	objFSearchLine			// �t�@�C���������ʂP�s
	)
{
	CLine* pobjLine = new CLine(objFSearchLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//�P�s�̍ő�I�t�Z�b�g�v�Z
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�o�C�i���\���P�s�ǉ�����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendBinaryLine
	(	CLine&	objBinaryLine
	)
{
	CLine* pobjLine = new CLine(objBinaryLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//�P�s�̍ő�I�t�Z�b�g�v�Z
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���Í������ʂP�s�ǉ�����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendEncriptLine
	(	CLine&	objEncriptLine
	)
{
	CLine* pobjLine = new CLine(objEncriptLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//�P�s�̍ő�I�t�Z�b�g�v�Z
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C�����������ʂP�s�ǉ�����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendDecriptLine
	(	CLine&	objDecriptLine
	)
{
	CLine* pobjLine = new CLine(objDecriptLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//�P�s�̍ő�I�t�Z�b�g�v�Z
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�G���R�[�h���ʂP�s�ǉ�����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendEncodeLine
	(	CLine&	objEncodeLine
	)
{
	CLine* pobjLine = new CLine(objEncodeLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//�P�s�̍ő�I�t�Z�b�g�v�Z
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r���ʂP�s�ǉ�����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendCompareLine
	(	CLine*	pobjCompareLine
	)
{
	CLine* pobjLine = new CLine(pobjCompareLine->String());
	//�t�@�C����r�p�A�s��ʂ������p��
	pobjLine->SetDiffLineKind(pobjCompareLine->GetDiffLineKind());
	m_objStringArray.Add((CObject*)pobjLine);

	//�P�s�̍ő�I�t�Z�b�g�v�Z
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r���ʂP�s�}������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::InsertCompareLine
	(	long	lLine,
		CLine*	pobjCompareLine
	)
{
	CLine* pobjLine = new CLine(pobjCompareLine->String());
	//�t�@�C����r�p�A�s��ʂ������p��
	pobjLine->SetDiffLineKind(pobjCompareLine->GetDiffLineKind());
	m_objStringArray.InsertAt(lLine,(CObject*)pobjLine);

	//�P�s�̍ő�I�t�Z�b�g�v�Z
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r�p�s��ʎ擾												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CLineManager::GetDiffLineKind
	(	long	lLine
	)
{
	if(lLine >= m_objStringArray.GetSize())
		return DIFF_LINEKIND_COMMON;

	CLine* pobjLine = NULL;
	pobjLine = (CLine*)m_objStringArray.GetAt(lLine);
	if(pobjLine != NULL)
		return pobjLine->GetDiffLineKind();
	else
		return DIFF_LINEKIND_COMMON;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r���ʂP�s�}������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::InsertDirCmpLine
	(	long	lLine,
		CLine*	pobjDirCmpLine
	)
{
	CLine* pobjLine = new CLine(pobjDirCmpLine->String());
	//�f�B���N�g����r�p�A�s��ʂ������p��
	pobjLine->SetDirCmpLineKind(pobjDirCmpLine->GetDirCmpLineKind());
	//�f�B���N�g����r�p�A�t�@�C�����������p��
	CString objStr=_T("");
	pobjDirCmpLine->GetDirCmpFileName(objStr);
	pobjLine->SetDirCmpFileName(objStr);
	//�o�^
	m_objStringArray.InsertAt(lLine,(CObject*)pobjLine);

	//�P�s�̍ő�I�t�Z�b�g�v�Z
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r�p�����ʎ擾										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CLineManager::GetDirCmpLineKind
	(	long	lLine
	)
{
	if(lLine >= m_objStringArray.GetSize())
		return DIRCMP_LINEKIND_NORMAL;

	CLine* pobjLine = NULL;
	pobjLine = (CLine*)m_objStringArray.GetAt(lLine);
	if(pobjLine != NULL)
		return pobjLine->GetDirCmpLineKind();
	else
		return DIRCMP_LINEKIND_NORMAL;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�b�q�A�k�e�̐��擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::GetCRLFCount
	(	long*	plCRCount,		//CR�̐�(out)
		long*	plLFCount		//LF�̐�(out)
	)
{
	(*plCRCount) = m_lCRCount;
	(*plLFCount) = m_lLFCount;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s�R�[�h�敪�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLineManager::GetKaigyoKind()
{
	return m_lKaigyoKind;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s�R�[�h�敪�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::SetKaigyoKind
	(	long	lKaigyoKind
	)
{
	if( m_lKaigyoKind == lKaigyoKind )
		return;

	if( lKaigyoKind==KAIGYO_CRLF || lKaigyoKind==KAIGYO_LF || lKaigyoKind==KAIGYO_CR)
		m_lKaigyoKind = lKaigyoKind;

	if( m_lKaigyoKind==KAIGYO_CRLF )
	{
		m_lCRCount = 1;
		m_lLFCount = 1;
	}
	else if( m_lKaigyoKind==KAIGYO_LF )
	{
		m_lCRCount = 0;
		m_lLFCount = 1;
	}
	else if( m_lKaigyoKind==KAIGYO_CR )
	{
		m_lCRCount = 1;
		m_lLFCount = 0;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�[�N�t���O�ݒ菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::SetFlagMark
	(	long	lLine,
		BOOL	bMark
	)
{
	if(lLine >= m_objStringArray.GetSize())
		return;

	CLine* pobjLine = NULL;
	pobjLine = (CLine*)m_objStringArray.GetAt(lLine);
	if(pobjLine != NULL)
	{
		if(bMark)
			m_lMarkLineCount++;
		else
			m_lMarkLineCount--;

		pobjLine->SetFlagMark(bMark);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�[�N�t���O�擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CLineManager::GetFlagMark
	(	long	lLine
	)
{
	if(lLine >= m_objStringArray.GetSize())
		return FALSE;

	CLine* pobjLine = NULL;
	pobjLine = (CLine*)m_objStringArray.GetAt(lLine);
	if(pobjLine != NULL)
		return pobjLine->GetFlagMark();
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�[�N�s���擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLineManager::GetMarkLineCount()
{
	return m_lMarkLineCount;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w��s�̍s���󔒍폜����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::DelEndSpaces
	(	long	lLine					// �s�ԍ�
	)
{
	if(lLine < 0)
		return;
	if(lLine >= m_objStringArray.GetSize())
		return;

	CLine* pobjLine = NULL;
	pobjLine = (CLine*)m_objStringArray.GetAt(lLine);
	if(pobjLine != NULL)
	{
		pobjLine->DelEndSpaces();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ۑ����p�X�y�[�X�|���^�u�ϊ�����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::ConvSpace2TabOnSave
	(	long	lfWidth,		//�t�H���g����
		long	lTabSize		//�^�u�T�C�Y
	)
{
	long lOffset=0;
	long lCnt=0;
	long lLine=0;
	CLine* pobjLine=NULL;
	long lEndOffsetAfter=0;

	for(lLine=0; lLine<GetLineCount(); lLine++)
	{
		lOffset=0;
		lCnt=0;

		pobjLine = NULL;
		pobjLine = GetOneLine(lLine);
		if( pobjLine == NULL )
			continue;

		lEndOffsetAfter = pobjLine->GetLength();
		for(lCnt=0; lCnt<lEndOffsetAfter; )
		{
			if(IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)))//�Q�o�C�g�����̑��o�C�g
			{
				lCnt+=2;
				lOffset += (lfWidth * 2);
				continue;
			}
			if(pobjLine->GetAt(lCnt) == '\t')//�^�u�R�[�h
			{
				//�}�[�W���v�Z
				int nMargin = lTabSize - ( (lOffset/lfWidth) % lTabSize );
				lOffset += (lfWidth * nMargin);
				lCnt++;
				continue;
			}
			else if(pobjLine->GetAt(lCnt) != ' ')//���p�X�y�[�X�ȊO
			{
				lCnt++;
				lOffset += lfWidth;
				continue;
			}

			//�}�[�W�����v�Z
			int nMargin = lTabSize - ( (lOffset/lfWidth) % lTabSize );
			//�X�y�[�X�����J�E���g
			int nSpaceCnt=0;
			for(int i=0; ; )
			{
				if(IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt+i)))
					break;
				else if(pobjLine->GetAt(lCnt+i) == ' ') //���p�X�y�[�X�p��
				{
					i++;
					nSpaceCnt++;
				}
				else
					break;
			}
			//�^�u�ɒu���������镪�����X�y�[�X�����邩�ǂ����`�F�b�N
			if(nSpaceCnt < nMargin) //�Ȃ��I�I
			{
				lCnt++;
				lOffset += lfWidth;
				continue;
			}

			//�}�[�W���̕��������p�X�y�[�X���폜
			unsigned char c1stChar=0, c2ndChar=0;
			for(int i=0; i<nMargin; i++)
				pobjLine->DeleteChar(lCnt,&c1stChar,&c2ndChar);
			//�^�u��}��
			pobjLine->InsertChar(lCnt,VK_TAB,0);

			lOffset += (lfWidth * nMargin);
			lCnt++;
			//�I���I�t�Z�b�g���X�V
			lEndOffsetAfter-=nMargin;
			lEndOffsetAfter++;
		}

		//�Đݒ�(1999.03.20 Add for StatusBar Pane ID_INDICATOR_BYTE)
		SetOneLine(lLine,pobjLine);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CLineManager �N���X�̃v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�h�L�������g�̍s�P�ʂ̃��X�g��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.07.07 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::Clear()
{
	m_lMarkLineCount = 0;					// �}�[�N�s��
	m_lMaxLineLength = 0;					// �P�s�̒����̍ő�l

	for(int nCnt=0; nCnt<m_objStringArray.GetSize(); nCnt++)
	{
		CLine* pobjLine = (CLine*)m_objStringArray.GetAt(nCnt);
		delete pobjLine;
	}
	m_objStringArray.RemoveAll();
}


// FileCompare.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include <map>					// for map
using namespace std;
#include "Editor.h"
#include "Editor_Def.h"
#include "Line.h"
#include "LineManager.h"
#include "UndoItem.h"
#include "UndoManager.h"
#include "ColorManager.h"
#include "EditorDoc.h"
#include "StaticFilespec.h"
#include "FileCompareStatusDlg.h"
#include "FileCompare.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileCompare �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFileCompare::CFileCompare()
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_pobjFileCompareStatusDlg = NULL;
	m_pobjCompareDoc = NULL;
	m_objStringArray1.SetSize(0);
	m_objStringArray2.SetSize(0);
	m_lFileLineCount1 = 0;
	m_lFileLineCount2 = 0;
	m_lDelLineCount = 0;
	m_lAddLineCount = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFileCompare::~CFileCompare()
{
	Clear();
}

/////////////////////////////////////////////////////////////////////////////
// CFileCompare �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r���s														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileCompare::DoCompare			//�s�q�t�d�F����I���A�e�`�k�r�d�F�ُ�I��
	(	CFileCompareStatusDlg* pobjFileCompareStatusDlg,
		CEditorDoc* pobjCompareDoc,		// �h�L�������g�N���X
		CArchive&	ar1,				// ��r���t�@�C���A�[�J�C�u
		CArchive&	ar2					// ��r���t�@�C���A�[�J�C�u
	)
{
	BOOL	bRet=FALSE;
	LONG	lCnt=0;
	CLine*	pobjLine=NULL;
	CString objMsg;

	//�p�����[�^�ێ�
	m_pobjFileCompareStatusDlg = pobjFileCompareStatusDlg;
	m_pobjCompareDoc = pobjCompareDoc;

	//�t�@�C���ǂݍ���
	objMsg.LoadString(IDS_FILECOMPARE_READ1FILE);
	m_pobjFileCompareStatusDlg->SetStatus(objMsg);
	if( ReadFile(ar1,&m_objStringArray1) == FALSE )
		return FALSE;
	objMsg.LoadString(IDS_FILECOMPARE_READ2FILE);
	m_pobjFileCompareStatusDlg->SetStatus(objMsg);
	if( ReadFile(ar2,&m_objStringArray2) == FALSE )
		return FALSE;
	m_lFileLineCount1 = m_objStringArray1.GetSize();
	m_lFileLineCount2 = m_objStringArray2.GetSize();
	if(m_lFileLineCount1 == 0)
	{
		objMsg.LoadString(IDS_1STZERO_FILECMP);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	if(m_lFileLineCount2 == 0)
	{
		objMsg.LoadString(IDS_2NDZERO_FILECMP);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//�Œ����ʕ����i�k�b�r : Longest Common Subsequence�j�T��
	objMsg.LoadString(IDS_FILECOMPARE_COMPFILE);
	m_pobjFileCompareStatusDlg->SetStatus(objMsg);
	if( SearchLCS() == FALSE )
		return FALSE;

	//���ʏo��
	objMsg.LoadString(IDS_FILECOMPARE_OUTRESULT);
	m_pobjFileCompareStatusDlg->SetStatus(objMsg);
	if( OutputResult() == FALSE )
		return FALSE;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ǉ��s���A�폜�s���擾												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileCompare::GetDelAddLineCount
	(	long*	plDelLineCount,
		long*	plAddLineCount
	)
{
	(*plDelLineCount) = m_lDelLineCount;
	(*plAddLineCount) = m_lAddLineCount;
}

/////////////////////////////////////////////////////////////////////////////
// CFileCompare �N���X�̃v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���ǂݍ���														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileCompare::ReadFile			//�s�q�t�d�F����I���A�e�`�k�r�d�F�ُ�A�L�����Z���I��
	(	CArchive&	ar,
		CObArray*	pobjObArray
	)
{
	char* 		pPtr=NULL;
	long 		lCRCount=0;
	long 		lLFCount=0;
	long 		lCnt=0;
	int  		nNowRet=0;
	int  		nBeforeRet=0;
	CLine* 		pobjLine=NULL;
	CString 	objStr = _T("");

	m_pApp->PrepareMyReadString();
	for(lCnt=0; ;lCnt++)//�P�s�Âǂݍ���
	{
		m_pApp->MessagePump();
		if(m_pobjFileCompareStatusDlg->IsCanceled())
			return FALSE;

		objStr = _T("");
		nNowRet = m_pApp->MyReadString(ar,objStr,&lCRCount,&lLFCount);
		if( nNowRet == 0 ) //EOF�I��
		{
			if( objStr.IsEmpty() ) //�f�[�^�Ȃ�
				break;
		}

		//MyReadString ���^�[���l�ێ�
		nBeforeRet = nNowRet;

		if( lCnt==0 ) //�P�s�ڂ̓ǂݍ���
		{
			pobjLine = new CLine((const unsigned char*)(LPCTSTR)objStr);
			pobjObArray->Add((CObject*)pobjLine);
		}
		else //�Q�s�ڈȍ~
		{
			//�ŏI�s�ɃZ�b�g
			pobjLine = (CLine*)pobjObArray->GetAt(pobjObArray->GetSize()-1);
			pobjLine->Set((const unsigned char*)(LPCTSTR)objStr);
		}

		if(nNowRet == 1) //���s�I�����Ă����ꍇ
		{
			//���̍s�̕��o�^
			pobjLine = new CLine();
			pobjObArray->Add((CObject*)pobjLine);
		}
		else //�d�n�e�I�����Ă����ꍇ
		{
			break;
		}
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�Œ����ʕ����i�k�b�r : Longest Common Subsequence�j�T��				|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileCompare::SearchLCS()		//�s�q�t�d�F����I���A�e�`�k�r�d�F�ُ�A�L�����Z���I��
{
	LONG	lPreX=0;
	LONG	lPreY=0;
	LONG	lNowX=0;
	LONG	lNowY=0;
	LONG	lPreNode=0;
	LONG	lNowNode=0;
	LONG	lPreK=0;
	LONG	lX=0;
	LONG	lY=0;
	LONG	lD=0;
	LONG	lK=0;				// �Ίp�����ʔԍ��iy-x : -M�`N�j
	const LONG lOffset=m_lFileLineCount1;
	CLine*	pobjLine1=NULL;
	CLine*	pobjLine2=NULL;
	CLine	objLine1;
	CLine	objLine2;
	LONG	lFarY_KPLUS1=0;
	LONG	lFarY_KMINUS1=0;
	BOOL	bIsAssumeTabEqual2Space = m_pApp->GetProfileInt("CFileCompare","bIsAssumeTabEqual2Space",FALSE);
	BOOL	bIsAssumeAlphabetBigAndSmall = m_pApp->GetProfileInt("CFileCompare","bIsAssumeAlphabetBigAndSmall",FALSE);
	BOOL	bIsAssumeZenEqual2Han = m_pApp->GetProfileInt("CFileCompare","bIsAssumeZenEqual2Han",FALSE);

	for(lD=0; lD<=m_lFileLineCount1+m_lFileLineCount2; lD++) //  �e�c�ɂ���
	{
		m_pApp->MessagePump();
		if(m_pobjFileCompareStatusDlg->IsCanceled())
			return FALSE;

		for(lK=-lD; lK<=lD; lK+=2) 
		{
			m_pApp->MessagePump();
			if(m_pobjFileCompareStatusDlg->IsCanceled())
				return FALSE;

			//lK-1 or lK+1�̍ŉ��_�̂��A�����W
			lFarY_KPLUS1 = GetValueFarY(lK+1+lOffset);
			lFarY_KMINUS1 = GetValueFarY(lK-1+lOffset);
			if( lK==-lD || lK!=lD && lFarY_KMINUS1 < lFarY_KPLUS1 )
			{
				lPreK = lK+1;
				lPreY = lFarY_KPLUS1;
				lPreX = lPreY - lPreK;
				lY = lFarY_KPLUS1;
			}
			else
			{
				lPreK = lK-1;
				lPreY = lFarY_KMINUS1;
				lPreX = lPreY - lPreK;
				lY = lFarY_KMINUS1 + 1;
			}
			//k �̂��A�����W���o��
			lX = lY - lK;
			lNowX = lX;
			lNowY = lY;

			//�m�[�h�ԍ��v�Z
			lPreNode = MakeNodeNo(lPreX,lPreY);
			lNowNode = MakeNodeNo(lNowX,lNowY);
			//���O�m�[�h�ԍ��ێ�
			if(lPreNode != -1 && lNowNode != -1)
				SetValuePreIndex(lNowNode,lPreNode);

			//�Ίp���������āA�X�ɐ�ɐi�߂�ꍇ�A�l���X�V
			for(;;)
			{
				m_pApp->MessagePump();
				if(m_pobjFileCompareStatusDlg->IsCanceled())
					return FALSE;

				if(lX >= m_lFileLineCount1 || lY >= m_lFileLineCount2 )
					break;
				pobjLine1 = (CLine*)m_objStringArray1.GetAt(lX);
				pobjLine2 = (CLine*)m_objStringArray2.GetAt(lY);
				objLine1.Set(pobjLine1->String());
				objLine2.Set(pobjLine2->String());
				if(bIsAssumeZenEqual2Han) //��r�I�v�V�����F�S�p�Ɣ��p����ʂ��Ȃ�
				{
					objLine1.AlnumZen2Han();
					objLine1.KatakanaZen2Han();
					objLine1.SpaceZen2Han();
					objLine2.AlnumZen2Han();
					objLine2.KatakanaZen2Han();
					objLine2.SpaceZen2Han();
				}
				if(bIsAssumeTabEqual2Space) //��r�I�v�V�����F�󔒕�������ʂ��Ȃ�
				{
					objLine1.Spaces2One();
					objLine2.Spaces2One();
				}
				if(bIsAssumeAlphabetBigAndSmall) //��r�I�v�V�����F�p���̑啶������������ʂ��Ȃ�
				{
					objLine1.Al2Upper();
					objLine2.Al2Upper();
				}
				if( objLine1.Compare(0,objLine2.String(),objLine1.GetLength()) != 0 )
					break;

				lPreNode = MakeNodeNo(lX,lY);
				lX += 1;
				lY += 1;
				lNowNode = MakeNodeNo(lX,lY);
				//���O�m�[�h�ԍ��ێ�
				if(lPreNode != -1 && lNowNode != -1)
					SetValuePreIndex(lNowNode,lPreNode);
			}

			//�Ίp�����j�ɑΉ�����ŉ��_�̂��x��ێ�
			SetValueFarY(lK+lOffset,lY);
#ifdef _DEBUG
//			char	szBuf[128];
//			sprintf(szBuf,"nPreK=%2d (%2d,%2d) ---> k=%2d : (%2d,%2d) ---> (%2d,%2d)",
//					lPreK,lPreX,lPreY,lK,lNowX,lNowY,lX,lY);
//			pApp->ExecuteLogging(szBuf);
#endif
			if( lX>=m_lFileLineCount1 && lY>=m_lFileLineCount2 )
			{
				return TRUE;                         //  found !!!
			}
		}
	}

	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���ʏo��																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileCompare::OutputResult()		//�s�q�t�d�F����I���A�e�`�k�r�d�F�ُ�A�L�����Z���I��
{
	LONG	lPreNode=0;
	LONG	lNodeNo=0;
	LONG	lDiff=0;
	LONG	lEndNode = MakeNodeNo(m_lFileLineCount1,m_lFileLineCount2);
	LONG	lPreX=0;
	LONG	lPreY=0;
	CLine*	pobjLine1=NULL;
	CLine*	pobjLine2=NULL;

	for(lNodeNo=lEndNode; ; )
	{
		m_pApp->MessagePump();
		if(m_pobjFileCompareStatusDlg->IsCanceled())
			return FALSE;

		lPreNode = GetValuePreIndex(lNodeNo);
		if(lPreNode == 0)
			break;

		lDiff = lNodeNo - lPreNode;
		lPreX=lPreY=0;
		MakeXY(lPreNode,&lPreX,&lPreY);

		if(lDiff == 1 && lPreY >= m_lFileLineCount2)
			break;
		else if(lDiff == (m_lFileLineCount2 + 1) && lPreX >= m_lFileLineCount1)
			break;

		if(lDiff == 1)//�����E�����ֈړ�����i���t�@�C���̕�������o�́j
		{
			//�ǉ����ꂽ�s
			pobjLine2 = (CLine*)m_objStringArray2.GetAt(lPreY);
			pobjLine2->SetDiffLineKind(DIFF_LINEKIND_ADDED);
			m_pobjCompareDoc->InsertCompareLine(0,pobjLine2);
			m_lAddLineCount++;
		}
		else if(lDiff == (m_lFileLineCount2 + 1)) //�����������ֈړ�����i���t�@�C���̕�������o�́j
		{
			//�폜���ꂽ�s
			pobjLine1 = (CLine*)m_objStringArray1.GetAt(lPreX);
			pobjLine1->SetDiffLineKind(DIFF_LINEKIND_DELETED);
			m_pobjCompareDoc->InsertCompareLine(0,pobjLine1);
			m_lDelLineCount++;
		}
		else //�΂߉E�������ֈړ�����i���ʁj
		{
			//���ʍs�ł���
			pobjLine1 = (CLine*)m_objStringArray1.GetAt(lPreX);
			pobjLine1->SetDiffLineKind(DIFF_LINEKIND_COMMON);
			m_pobjCompareDoc->InsertCompareLine(0,pobjLine1);
		}

		lNodeNo = lPreNode;
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�m�[�h�ԍ��擾														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LONG CFileCompare::MakeNodeNo
	(	LONG	x, 
		LONG	y
	)
{
	if(x < 0 || y < 0)
		return -1;
	if(x > m_lFileLineCount1)
		return -1;
	if(y > m_lFileLineCount2)
		return -1;

	return 	x*(m_lFileLineCount2+1) + (y+1);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w�C�x�ԍ��擾														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileCompare::MakeXY
	(	LONG	lNode,
		LONG*	plX,
		LONG*	plY
	)
{
	LONG lAmari = lNode % (m_lFileLineCount2+1);
	if(lAmari != 0)
		(*plX) = lNode / (m_lFileLineCount2+1);
	else
		(*plX) = lNode / (m_lFileLineCount2+1) -1;

	(*plY) = lNode - (*plX)*(m_lFileLineCount2+1) - 1;
	if( (*plY) < 0 )
		(*plY) = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�O�m�[�h�ݒ�															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileCompare::SetValuePreIndex
	(	LONG	x,
		LONG	y
	)
{
	m_objMapPreIndex[x] = y;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�O�m�[�h�擾															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LONG CFileCompare::GetValuePreIndex
	(	LONG	x
	)
{
	map<long,long>::iterator it;

	it = m_objMapPreIndex.find(x);
	if( it != m_objMapPreIndex.end() ) //�����I�I
		return (*it).second;
	else
		return 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�e�Ίp���̍ŉ��_�̂����W�l�ݒ�										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileCompare::SetValueFarY
	(	LONG	x,
		LONG	y
	)
{
	m_objMapFarY[x] = y;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�e�Ίp���̍ŉ��_�̂����W�l�擾										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LONG CFileCompare::GetValueFarY
	(	LONG	x
	)
{
	map<long,long>::iterator it;

	it = m_objMapFarY.find(x);
	if( it != m_objMapFarY.end() ) //�����I�I
		return (*it).second;
	else
		return 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�N���A																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileCompare::Clear()
{
	int 		nCnt=0;
	CLine*		pobjLine=NULL;

	//���t�@�C���̍s�f�[�^�폜
	for(nCnt=0; nCnt<m_objStringArray1.GetSize(); nCnt++)
	{
		pobjLine = (CLine*)m_objStringArray1.GetAt(nCnt);
		if(pobjLine != NULL)
			delete pobjLine;
	}
	m_objStringArray1.RemoveAll();

	//���t�@�C���̍s�f�[�^�폜
	for(nCnt=0; nCnt<m_objStringArray2.GetSize(); nCnt++)
	{
		pobjLine = (CLine*)m_objStringArray2.GetAt(nCnt);
		if(pobjLine != NULL)
			delete pobjLine;
	}
	m_objStringArray2.RemoveAll();

	//�}�b�v�f�[�^�N���A
	m_objMapPreIndex.clear();
	m_objMapFarY.clear();
}


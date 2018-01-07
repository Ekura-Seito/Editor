// DirCompare.cpp : �C���v�������e�[�V���� �t�@�C��
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
#include "DirCompareStatusDlg.h"
#include "DirCompare.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirCompare �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDirCompare::CDirCompare()
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_pobjDirCompareStatusDlg = NULL;
	m_pobjCompareDoc = NULL;
	m_objStringArray1.SetSize(0);
	m_objStringArray2.SetSize(0);
	m_lFileCount1 = 0;
	m_lFileCount2 = 0;
	m_objDir1 = _T("");
	m_objDir2 = _T("");
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDirCompare::~CDirCompare()
{
	Clear();
}

/////////////////////////////////////////////////////////////////////////////
// CDirCompare �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r���s													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDirCompare::DoCompare			//�s�q�t�d�F����I���A�e�`�k�r�d�F�ُ�I��
	(	CDirCompareStatusDlg* pobjDirCompareStatusDlg,
		CEditorDoc* pobjCompareDoc,		// �h�L�������g�N���X
		CString&	objDir1,			// ��r���f�B���N�g����
		CString&	objDir2				// ��r���f�B���N�g����
	)
{
	BOOL	bRet=FALSE;
	LONG	lCnt=0;
	CLine*	pobjLine=NULL;
	CString objMsg;

	//�p�����[�^�ێ�
	m_pobjDirCompareStatusDlg = pobjDirCompareStatusDlg;
	m_pobjCompareDoc = pobjCompareDoc;
	m_objDir1 = objDir1;
	m_objDir2 = objDir2;

	//�f�B���N�g�����t�@�C���ꗗ�쐬
	objMsg.LoadString(IDS_DIRCOMPARE_MAKE1DIR);
	m_pobjDirCompareStatusDlg->SetStatus(objMsg);
	if( MakeFileList(objDir1,&m_objStringArray1) == FALSE )
		return FALSE;
	objMsg.LoadString(IDS_DIRCOMPARE_MAKE2DIR);
	m_pobjDirCompareStatusDlg->SetStatus(objMsg);
	if( MakeFileList(objDir2,&m_objStringArray2) == FALSE )
		return FALSE;
	m_lFileCount1 = m_objStringArray1.GetSize();
	m_lFileCount2 = m_objStringArray2.GetSize();
	if(m_lFileCount1 == 0)
	{
		objMsg.LoadString(IDS_1STZERO_DIRCMP);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	if(m_lFileCount2 == 0)
	{
		objMsg.LoadString(IDS_2NDZERO_DIRCMP);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//�Œ����ʕ����i�k�b�r : Longest Common Subsequence�j�T��
	objMsg.LoadString(IDS_DIRCOMPARE_NOWCOMPARE);
	m_pobjDirCompareStatusDlg->SetStatus(objMsg);
	if( SearchLCS() == FALSE )
		return FALSE;

	//���ʏo��
	objMsg.LoadString(IDS_DIRCOMPARE_OUTRESULT);
	m_pobjDirCompareStatusDlg->SetStatus(objMsg);
	if( OutputResult() == FALSE )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFileCompare �N���X�̃v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g�����t�@�C�����X�g�쐬										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDirCompare::MakeFileList			//�s�q�t�d�F����I���A�e�`�k�r�d�F�ُ�A�L�����Z���I��
	(	CString&	objDir,				//�f�B���N�g����
		CObArray*	pobjObArray
	)
{
	BOOL		bRet=FALSE;
	CString		objFilter=_T("");
	CString		objFileName=_T("");
	CFileFind	objFileFind;

	//�t�@�C�������L�[�쐬
	if(m_pApp->IsDriveRoot(objDir))
		objFilter.Format("%s*",objDir);
	else
		objFilter.Format("%s\\*",objDir);

	//�t�@�C������
	bRet = objFileFind.FindFile(objFilter);
	if(bRet != FALSE)
	{
		while(objFileFind.FindNextFile()!=FALSE)
		{
			m_pApp->MessagePump();
			if(m_pobjDirCompareStatusDlg->IsCanceled())
			{
				objFileFind.Close();
				return FALSE;
			}

			if( objFileFind.IsDots() != FALSE ) // "." or ".."
				continue;
			if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
				continue;

			//�t�@�C�����擾
			objFileName = objFileFind.GetFileName();
			//�A���t�@�x�b�g���Ƀ��X�g�֒ǉ�
			AddFileBySort(objFileName,pobjObArray);
		}
		//�Ō�̂P��
		if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
		{
			//�t�@�C�����擾
			objFileName = objFileFind.GetFileName();
			//�A���t�@�x�b�g���Ƀ��X�g�֒ǉ�
			AddFileBySort(objFileName,pobjObArray);
		}
	}
	objFileFind.Close();

	if(bRet)
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A���t�@�x�b�g���Ƀ��X�g�֒ǉ�										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompare::AddFileBySort
	(	CString&	objFileName,
		CObArray*	pobjArray
	)
{
	int nIndex=0;
	int nSize=pobjArray->GetSize();
	CString* pobjStrTmp=NULL;
	CString* pobjStrBuf = new CString();
	(*pobjStrBuf) = objFileName;

	for(nIndex=0; nIndex<nSize; nIndex++)
	{
		pobjStrTmp = (CString*)pobjArray->GetAt(nIndex);
		if(pobjStrTmp->CompareNoCase((LPCTSTR)objFileName) <= 0)
			continue;
		else
			break;
	}

	if(nIndex == nSize)
		pobjArray->Add((CObject*)pobjStrBuf); //�Ō�ɒǉ�
	else
		pobjArray->InsertAt(nIndex,(CObject*)pobjStrBuf);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�Œ����ʕ����i�k�b�r : Longest Common Subsequence�j�T��				|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDirCompare::SearchLCS()		//�s�q�t�d�F����I���A�e�`�k�r�d�F�ُ�A�L�����Z���I��
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
	const LONG lOffset=m_lFileCount1;
	CString* pobjStr1=NULL;
	CString* pobjStr2=NULL;
	LONG	lFarY_KPLUS1=0;
	LONG	lFarY_KMINUS1=0;

	for(lD=0; lD<=m_lFileCount1+m_lFileCount2; lD++) //  �e�c�ɂ���
	{
		m_pApp->MessagePump();
		if(m_pobjDirCompareStatusDlg->IsCanceled())
			return FALSE;

		for(lK=-lD; lK<=lD; lK+=2) 
		{
			m_pApp->MessagePump();
			if(m_pobjDirCompareStatusDlg->IsCanceled())
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
				if(m_pobjDirCompareStatusDlg->IsCanceled())
					return FALSE;

				if(lX >= m_lFileCount1 || lY >= m_lFileCount2 )
					break;
				pobjStr1 = (CString*)m_objStringArray1.GetAt(lX);
				pobjStr2 = (CString*)m_objStringArray2.GetAt(lY);
				if( pobjStr1->CompareNoCase((LPCTSTR)(*pobjStr2)) != 0 )
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
			if( lX>=m_lFileCount1 && lY>=m_lFileCount2 )
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
BOOL CDirCompare::OutputResult()		//�s�q�t�d�F����I���A�e�`�k�r�d�F�ُ�A�L�����Z���I��
{
	LONG	lPreNode=0;
	LONG	lNodeNo=0;
	LONG	lDiff=0;
	LONG	lEndNode = MakeNodeNo(m_lFileCount1,m_lFileCount2);
	LONG	lPreX=0;
	LONG	lPreY=0;
	CString* pobjStr1=NULL;
	CString* pobjStr2=NULL;
	CLine	objLine;
	char	szBuf[256];
	CString objMsg;

	for(lNodeNo=lEndNode; ; )
	{
		m_pApp->MessagePump();
		if(m_pobjDirCompareStatusDlg->IsCanceled())
			return FALSE;

		lPreNode = GetValuePreIndex(lNodeNo);
		if(lPreNode == 0)
			break;

		lDiff = lNodeNo - lPreNode;
		lPreX=lPreY=0;
		MakeXY(lPreNode,&lPreX,&lPreY);

		if(lDiff == 1 && lPreY >= m_lFileCount2)
			break;
		else if(lDiff == (m_lFileCount2 + 1) && lPreX >= m_lFileCount1)
			break;

		if(lDiff == 1)//�����E�����ֈړ�����i���f�B���N�g���̃t�@�C�������o�́j
		{
			//�ǉ����ꂽ�t�@�C��
			pobjStr2 = (CString*)m_objStringArray2.GetAt(lPreY);

			objMsg.LoadString(IDS_DIRCOMPARE_RESULTMSG1);
			sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)(*pobjStr2), (LPCTSTR)m_objDir2);
			objLine.Set((const unsigned char*)szBuf);
			objLine.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
			m_pobjCompareDoc->InsertDirCmpLine(0,&objLine);
			m_pobjDirCompareStatusDlg->IncCompareFileCount();
			m_pobjDirCompareStatusDlg->IncAddFileCount();
		}
		else if(lDiff == (m_lFileCount2 + 1)) //�����������ֈړ�����i���f�B���N�g���̃t�@�C�������o�́j
		{
			//�폜���ꂽ�t�@�C��
			pobjStr1 = (CString*)m_objStringArray1.GetAt(lPreX);

			objMsg.LoadString(IDS_DIRCOMPARE_RESULTMSG1);
			sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)(*pobjStr1), (LPCTSTR)m_objDir1);
			objLine.Set((const unsigned char*)szBuf);
			objLine.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
			m_pobjCompareDoc->InsertDirCmpLine(0,&objLine);
			m_pobjDirCompareStatusDlg->IncCompareFileCount();
			m_pobjDirCompareStatusDlg->IncDelFileCount();
		}
		else //�΂߉E�������ֈړ�����i���ʁj
		{
			//���ʃt�@�C����
			pobjStr1 = (CString*)m_objStringArray1.GetAt(lPreX);

			//����`�F�b�N
			int nDate=0;
			int nRet = CheckIsSameFile(pobjStr1,&nDate);
			if(nRet == 1)
			{
				objMsg.LoadString(IDS_DIRCOMPARE_RESULTMSG4);
				sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)(*pobjStr1));
				objLine.Set((const unsigned char*)szBuf);
				objLine.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
			}
			else if(nRet == 0)
			{
				if(nDate == 1)
				{
					objMsg.LoadString(IDS_DIRCOMPARE_RESULTMSG2);
					sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)(*pobjStr1), (LPCTSTR)m_objDir1);
				}
				else if(nDate == 2)
				{
					objMsg.LoadString(IDS_DIRCOMPARE_RESULTMSG2);
					sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)(*pobjStr1), (LPCTSTR)m_objDir2);
				}
				else if(nDate == 3)
				{
					objMsg.LoadString(IDS_DIRCOMPARE_RESULTMSG3);
					sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)(*pobjStr1));
				}
				objLine.Set((const unsigned char*)szBuf);
				objLine.SetDirCmpFileName((*pobjStr1));
				objLine.SetDirCmpLineKind(DIRCMP_LINEKIND_DIFF);
				m_pobjDirCompareStatusDlg->IncDiffFileCount();
			}
			else if(nRet == -1)
			{
				objMsg.LoadString(IDS_FILEACCESS_ERROR);
				sprintf(szBuf,"  %-30s : %s", (LPCTSTR)(*pobjStr1), (LPCTSTR)objMsg);
				objLine.Set((const unsigned char*)szBuf);
				objLine.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
			}
			m_pobjCompareDoc->InsertDirCmpLine(0,&objLine);
			m_pobjDirCompareStatusDlg->IncCompareFileCount();
		}

		lNodeNo = lPreNode;
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C������`�F�b�N													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CDirCompare::CheckIsSameFile			//�P�F����A�O�F���Ⴀ��A�|�P�F�ُ�I��
	(	CString*	pobjFileNameStr,
		int*		pnDate					//�P�F��ꂪ�V�����A�Q�F��񂪐V�����A�R�F����
	)
{
	CString		objFilePath1=_T("");
	CString		objFilePath2=_T("");
	CFile		objFile1, objFile2;
	CFileStatus objFileStatus1, objFileStatus2;
	char		szBufForAr1[512];
	char		szBufForAr2[512];
	char ch1=0;
	char ch2=0;

	//�t�@�C���p�X�ҏW
	if(m_pApp->IsDriveRoot(m_objDir1))
		objFilePath1.Format("%s%s",m_objDir1,(*pobjFileNameStr));
	else
		objFilePath1.Format("%s\\%s",m_objDir1,(*pobjFileNameStr));
	if(m_pApp->IsDriveRoot(m_objDir2))
		objFilePath2.Format("%s%s",m_objDir2,(*pobjFileNameStr));
	else
		objFilePath2.Format("%s\\%s",m_objDir2,(*pobjFileNameStr));

	CFile::GetStatus((LPCTSTR)objFilePath1,objFileStatus1);
	CFile::GetStatus((LPCTSTR)objFilePath2,objFileStatus2);
	//�t�@�C�����t�`�F�b�N
	if(objFileStatus1.m_mtime > objFileStatus2.m_mtime)
		(*pnDate) = 1;
	else if(objFileStatus1.m_mtime < objFileStatus2.m_mtime)
		(*pnDate) = 2;
	else if(objFileStatus1.m_mtime == objFileStatus2.m_mtime)
		(*pnDate) = 3;
	//�t�@�C���T�C�Y�`�F�b�N
	if(objFileStatus1.m_size != objFileStatus2.m_size)
		return 0;

	//�t�@�C���I�[�v��
	memset(szBufForAr1,0,sizeof(szBufForAr1));
	memset(szBufForAr2,0,sizeof(szBufForAr2));
	if( !objFile1.Open( objFilePath1, CFile::modeRead|CFile::typeBinary, NULL ) )
		return -1;
	CArchive ar1( &objFile1, CArchive::load, sizeof(szBufForAr1), szBufForAr1 );
	if( !objFile2.Open( objFilePath2, CFile::modeRead|CFile::typeBinary, NULL ) )
	{
		ar1.Close();
		objFile1.Close();
		return -1;
	}
	CArchive ar2( &objFile2, CArchive::load, sizeof(szBufForAr2), szBufForAr2 );

	//�P�o�C�g���`�F�b�N
	TRY
	{
		for(;;)
		{
			ar1 >> (BYTE&)ch1;
			ar2 >> (BYTE&)ch2;
			if(ch1 != ch2)
			{
				//���Ⴀ��
				ar1.Close();
				objFile1.Close();
				ar2.Close();
				objFile2.Close();
				return 0;
			}
		}
	}
	CATCH_ALL(e)
	{
		//����
		ar1.Close();
		objFile1.Close();
		ar2.Close();
		objFile2.Close();
		return 1;
	}
	END_CATCH_ALL

	ar1.Close();
	objFile1.Close();
	ar2.Close();
	objFile2.Close();
	return 1;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�m�[�h�ԍ��擾														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LONG CDirCompare::MakeNodeNo
	(	LONG	x, 
		LONG	y
	)
{
	if(x < 0 || y < 0)
		return -1;
	if(x > m_lFileCount1)
		return -1;
	if(y > m_lFileCount2)
		return -1;

	return 	x*(m_lFileCount2+1) + (y+1);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w�C�x�ԍ��擾														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompare::MakeXY
	(	LONG	lNode,
		LONG*	plX,
		LONG*	plY
	)
{
	LONG lAmari = lNode % (m_lFileCount2+1);
	if(lAmari != 0)
		(*plX) = lNode / (m_lFileCount2+1);
	else
		(*plX) = lNode / (m_lFileCount2+1) -1;

	(*plY) = lNode - (*plX)*(m_lFileCount2+1) - 1;
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
void CDirCompare::SetValuePreIndex
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
LONG CDirCompare::GetValuePreIndex
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
void CDirCompare::SetValueFarY
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
LONG CDirCompare::GetValueFarY
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
void CDirCompare::Clear()
{
	int 		nCnt=0;
	CString*	pobjStr=NULL;

	//���f�B���N�g���̃t�@�C�����f�[�^�폜
	for(nCnt=0; nCnt<m_objStringArray1.GetSize(); nCnt++)
	{
		pobjStr = (CString*)m_objStringArray1.GetAt(nCnt);
		if(pobjStr != NULL)
			delete pobjStr;
	}
	m_objStringArray1.RemoveAll();

	//���f�B���N�g���̃t�@�C�����f�[�^�폜
	for(nCnt=0; nCnt<m_objStringArray2.GetSize(); nCnt++)
	{
		pobjStr = (CString*)m_objStringArray2.GetAt(nCnt);
		if(pobjStr != NULL)
			delete pobjStr;
	}
	m_objStringArray2.RemoveAll();

	//�}�b�v�f�[�^�N���A
	m_objMapPreIndex.clear();
	m_objMapFarY.clear();
}


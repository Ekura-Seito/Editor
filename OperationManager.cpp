// OperationManager.cpp : COperationManager �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include <afxtempl.h>			//for CTypedPtrList
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "Editor.h"
#include "Operation.h"
#include "OperationManager.h"
#include "StaticFilespec.h"
#include "ListCtrlEx.h"
#include "AutoResizer.h"
#include "KeyOpeDispDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// COperationManager �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COperationManager::COperationManager()
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_objOperationFileName = _T("");
	m_objOperationName = _T("");
	m_lCurrentPosition = 0;
	clearAllItem();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COperationManager::~COperationManager()
{
	clearAllItem();
}

/////////////////////////////////////////////////////////////////////////////
// COperationManager �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F������																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::Initialize()
{
	m_objOperationFileName = _T("");
	m_objOperationName = _T("");
	m_lCurrentPosition = 0;
	clearAllItem();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�y���[�V�����ꗗ�\��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::DispOperationLists()
{
	//�I�y���[�V�����ꗗ�_�C�A���O�\��
	CKeyOpeDispDlg	objKeyOpeDispDlg;
	objKeyOpeDispDlg.Initialize(this);
	objKeyOpeDispDlg.DoModal();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�y���[�V�����t�@�C�����ݒ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::SetOperationFileName
	(	CString&	objFName
	)
{
	m_objOperationFileName = objFName;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�y���[�V�����t�@�C�����擾											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::GetOperationFileName
	(	CString&	objFName
	)
{
	objFName = m_objOperationFileName;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�y���[�V�������ݒ�													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::SetOperationName
	(	CString&	objName
	)
{
	m_objOperationName = objName;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�y���[�V�������擾													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::GetOperationName
	(	CString&	objName
	)
{
	objName = m_objOperationName;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�y���[�V�������擾													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long COperationManager::GetOperationCount()
{
	return m_objOperationList.GetCount();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FCOperation �̓o�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::addOperation
	(	COperation*	pobjOperation
	)
{
	m_objOperationList.AddTail(pobjOperation);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�y���[�V�����A�����s												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::doOperation
	(	CEditorView*	pobjEditorView,		//�I�y���[�V�������s�Ώۂ̂u������
		CEditorDoc*		pobjEditorDoc		//�I�y���[�V�������s�Ώۂ̂c����
	)
{
	POSITION pos = m_objOperationList.GetHeadPosition();
	COperation*	pobjOperation = NULL;
	for(;;)
	{
		m_pApp->MessagePump();

		if( pos == 0 )
			break;
		pobjOperation = (COperation*)m_objOperationList.GetNext(pos);
		if( pobjOperation == NULL )
			break;
		//�I�y���[�V�������s
		pobjOperation->Do(pobjEditorView,pobjEditorDoc);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�y���[�V�����̃X�e�b�v���s											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL COperationManager::stepDoOperation
	(	CEditorView*	pobjEditorView,		//�I�y���[�V�������s�Ώۂ̂u������
		CEditorDoc*		pobjEditorDoc,		//�I�y���[�V�������s�Ώۂ̂c����
		BOOL			bIsFirst			//����t���O
	)
											//�֐����^�[��
											// TRUE  : �Â�����
											// FALSE : �I��
{
	COperation*	pobjOperation = NULL;

	if(bIsFirst) //�X�e�b�v���s�̊J�n
	{
		//�擪�I�y���[�V�����擾
		m_lCurrentPosition = m_objOperationList.GetHeadPosition();
	}

	if(m_lCurrentPosition != 0)
	{
		pobjOperation = (COperation*)m_objOperationList.GetNext(m_lCurrentPosition);
		//�I�y���[�V�������s
		if( pobjOperation != NULL )
			pobjOperation->Do(pobjEditorView,pobjEditorDoc);
	}

	if(m_lCurrentPosition != 0)
		return TRUE; 	//�Â�����
	else
		return FALSE;	//�I��
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�y���[�V�����̃X�e�b�v���s�I��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::endStepDoOperation()
{
	m_lCurrentPosition = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�y���[�V�����t�@�C���`�F�b�N										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL COperationManager::checkIsOperationFile
	(	CArchive&	ar
	)
												//�֐����^�[��
												//  TRUE : �I�y���[�V�����t�@�C���ł���
												// FALSE : �I�y���[�V�����t�@�C���łȂ�
{
	int  nRet=0;
	long lCRCount=0;
	long lLFCount=0;

	//�}�W�b�N���[�h�`�F�b�N
	CString objStrBuf = _T("");
	m_pApp->PrepareMyReadString();
	nRet = m_pApp->MyReadString(ar,objStrBuf,&lCRCount,&lLFCount);
	if( nRet == 0 ) //EOF�I��
	{
		if( objStrBuf.IsEmpty() ) //�f�[�^�Ȃ�
			return FALSE;
	}
	if( objStrBuf.Compare(KEYOPE_MAGICWORD) != 0 )//�}�W�b�N���[�h�s��v
	{
		return FALSE;
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�V���A���C�Y															|*/
/*|	���	�F																		|*/
/*|	���l	�F�{�֐���ǂݍ��݂ŌĂԏꍇ�A���炩����checkIsOperationFile�֐���		|*/
/*|			  �}�W�b�N���[�h�`�F�b�N�����܂��Ă������ƁB							|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL COperationManager::Serialize		//TRUE : ����I���AFALSE : �ُ�I��
	(	CArchive&	ar
	)
{
	int  nRet=0;
	long lCRCount=0;
	long lLFCount=0;
	char*	pPtr=NULL;
	COperation*	pobjOperation = NULL;
	BOOL bRet=FALSE;

	if(ar.IsStoring()) //�ۑ�
	{
		//�}�W�b�N���[�h������
		ar.WriteString(KEYOPE_MAGICWORD);
		ar.WriteString("\r\n");

		//�I�y���[�V������������
		ar.WriteString(KEYOPE_OPENAME_HEADER);
		ar.WriteString((LPCTSTR)m_objOperationName);
		ar.WriteString("\r\n");

		m_objOperationList.Serialize(ar);
		return TRUE;
	}
	else //�r�݂���
	{
		//������
		Initialize();

		//����Serialize�֐����R�[������O��
		//�}�W�b�N���[�h�`�F�b�N�ς݂ł���B

		//�I�y���[�V�������擾
		CString objStrBuf = _T("");
		m_pApp->PrepareMyReadString();
		nRet = m_pApp->MyReadString(ar,objStrBuf,&lCRCount,&lLFCount);
		if( nRet == 0 ) //EOF�I��
		{
			if( objStrBuf.IsEmpty() ) //�f�[�^�Ȃ�
				return FALSE;
		}
		if( objStrBuf.Left(strlen(KEYOPE_OPENAME_HEADER)) != KEYOPE_OPENAME_HEADER )//�I�y���[�V�������w�b�_�ł͂Ȃ�
		{
			return FALSE;
		}
		else
		{
			//�I�y���[�V�������擾���ݒ�
			CString objOpeName=_T("");
			if( objStrBuf.GetLength() != (signed)strlen(KEYOPE_OPENAME_HEADER) )
			{
				pPtr = objStrBuf.GetBuffer(objStrBuf.GetLength());
				objOpeName = (char*)(pPtr + strlen(KEYOPE_OPENAME_HEADER));
				objStrBuf.ReleaseBuffer();
			}
			SetOperationName(objOpeName);
		}

		m_objOperationList.Serialize(ar);
		return TRUE;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�y���[�V�������������J�n											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::SearchStartOperation()
{
	m_lCurrentPosition = m_objOperationList.GetHeadPosition();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�y���[�V������������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COperation* COperationManager::SearchGetOperation()
{
	if(m_lCurrentPosition == 0)
		return NULL;
	else
		return (COperation*)m_objOperationList.GetNext(m_lCurrentPosition);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�y���[�V�������������I��											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::SearchEndOperation()
{
	m_lCurrentPosition = 0;
}

/////////////////////////////////////////////////////////////////////////////
// COperationManager �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FCOperation ���X�g�̃N���A												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::clearAllItem()
{
	POSITION pos = m_objOperationList.GetHeadPosition();
	POSITION oldPos = pos;
	COperation*	pobjOperation = NULL;
	for(;;)
	{
		if( pos == 0 )
			break;
		pobjOperation = (COperation*)m_objOperationList.GetNext(pos);
		if( pobjOperation == NULL )
			break;
		m_objOperationList.RemoveAt(oldPos);
		delete pobjOperation;
		oldPos = pos;
	}
	m_lCurrentPosition = 0;
}

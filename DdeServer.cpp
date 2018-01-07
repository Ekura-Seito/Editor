// DdeServer.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <afxtempl.h>					//for CTypedPtrList
#include <ddeml.h>						// for DDE
#include <io.h>							// for _access
#include "Editor.h"
#include "Editor_Def.h"
#include "StatusBarDisp.h"				//�X�e�[�^�X�o�[�\�����\����
#include "ColorManager.h"
#include "CodeConvManager.h"
#include "BitmapClient.h"
#include "Operation.h"
#include "OperationManager.h"
#include "MainFrm.h"
#include "DdeServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// �ÓI�� m_pDdeServer �I�u�W�F�N�g���`����
CDdeServer* CDdeServer::m_pDdeServer = NULL;

/////////////////////////////////////////////////////////////////////////////
// CDdeServer �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDdeServer::CDdeServer()
{
	UINT	unRet=0;
	HDDEDATA hData=0;

	//�����o�ϐ��N���A
	m_pApp = (CEditorApp*)AfxGetApp();
	m_pMainWnd = NULL;
	m_idInst = 0;
	m_ServiceName = _T("");
	m_TopicName = _T("");

	m_pDdeServer = this;

	//�c�c�d������
	unRet = ::DdeInitialize( &m_idInst,
					 DdeCallback,
					 APPCLASS_STANDARD|CBF_FAIL_ADVISES|CBF_FAIL_EXECUTES|CBF_FAIL_POKES|CBF_SKIP_REGISTRATIONS|CBF_SKIP_UNREGISTRATIONS,
					 0L);
#ifdef _DEBUG
//	if(unRet != DMLERR_NO_ERROR)
//		AfxMessageBox("DdeInitialize Error",(MB_OK|MB_ICONSTOP),0);
//	else
//		AfxMessageBox("DdeInitialize OK",(MB_OK|MB_ICONINFORMATION),0);
#endif

	//�c�c�d�T�[�o�[�Ƃ��ēo�^
	m_ServiceName = DDE_SERVICENAME;
	HSZ hszService = ::DdeCreateStringHandle( m_idInst,
											  m_ServiceName,
											  0 );
	hData = ::DdeNameService( m_idInst,
					  hszService,
					  NULL,
					  DNS_REGISTER );
	::DdeFreeStringHandle( m_idInst, hszService );
#ifdef _DEBUG
//	if(hData == 0)
//		AfxMessageBox("�c�c�d�T�[�o�[�Ƃ��ēo�^ Error",(MB_OK|MB_ICONSTOP),0);
//	else
//		AfxMessageBox("�c�c�d�T�[�o�[�Ƃ��ēo�^ OK",(MB_OK|MB_ICONINFORMATION),0);
#endif
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDdeServer::~CDdeServer()
{
	//�c�c�d�I��
	::DdeUninitialize(m_idInst);
}

/////////////////////////////////////////////////////////////////////////////
// CDdeServer �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�|�C���^�ݒ�													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDdeServer::SetMainWnd
	(	CWnd*	pWnd
	)
{
	m_pMainWnd = pWnd;
}

/////////////////////////////////////////////////////////////////////////////
// CDdeServer �X�^�e�B�b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�c�c�d�R�[���o�b�N													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
HDDEDATA CALLBACK EXPORT CDdeServer::DdeCallback
	(	UINT		iType,
		UINT		uFmt,
		HCONV		hCconv,
		HSZ			hsz1,
		HSZ			hsz2,
		HDDEDATA	hData,
		DWORD		dwData1,
		DWORD		dwData2
	)
{
	char	szBuffer[512];
	char	szBuffer2[512];
//	char	szMsg[256];

	memset(szBuffer,0,sizeof(szBuffer));
	memset(szBuffer2,0,sizeof(szBuffer2));

	switch(iType)
	{
	case XTYP_CONNECT:			//�u�ڑ��v
		//�T�[�r�X���擾
		::DdeQueryString( m_pDdeServer->m_idInst,
						  hsz2,
						  szBuffer,
						  sizeof(szBuffer),
						  CP_WINANSI );
		//�g�s�b�N���擾
		::DdeQueryString( m_pDdeServer->m_idInst,
						  hsz1,
						  szBuffer2,
						  sizeof(szBuffer2),
						  CP_WINANSI );
#ifdef _DEBUG
//		sprintf(szMsg,"XTYP_CONNECT!! SERVICE=%s,TOPIC=%s",szBuffer,szBuffer2);
//		AfxMessageBox(szMsg,(MB_OK|MB_ICONINFORMATION),0);
#endif
		if( strcmp(szBuffer,DDE_SERVICENAME) != 0 )
			return (HDDEDATA)FALSE;
		if( strcmp(szBuffer2,DDE_TOPICNAME) != 0 )
			return (HDDEDATA)FALSE;
		else
			return (HDDEDATA)TRUE;
		break;

	case XTYP_REQUEST:			//�u�v���v
#ifdef _DEBUG
//		sprintf(szMsg,"XTYP_REQUEST!!");
//		AfxMessageBox(szMsg,(MB_OK|MB_ICONINFORMATION),0);
#endif
		//�g�s�b�N���擾
		::DdeQueryString( m_pDdeServer->m_idInst,
						  hsz1,
						  szBuffer,
						  sizeof(szBuffer),
						  CP_WINANSI );
		if( strcmp(szBuffer,DDE_TOPICNAME) == 0 )
		{
			//���ږ��擾
			::DdeQueryString( m_pDdeServer->m_idInst,
							  hsz2,
							  szBuffer2,
							  sizeof(szBuffer2),
							  CP_WINANSI );
			//���݃`�F�b�N
			if( _access(szBuffer2,0) == 0 )
			{
				//�w��t�@�C�����I�[�v��
				CString objStr = szBuffer2;
				m_pDdeServer->m_pApp->OpenFile(objStr);
				if(m_pDdeServer->m_pMainWnd != NULL)
				{
					if(m_pDdeServer->m_pMainWnd->IsIconic())
						m_pDdeServer->m_pMainWnd->ShowWindow(SW_RESTORE);
//					m_pDdeServer->m_pMainWnd->SetForegroundWindow();
					m_pDdeServer->m_pApp->MySetForegroundWindow(m_pDdeServer->m_pMainWnd->m_hWnd);
				}
				//����I��
				return ::DdeCreateDataHandle( m_pDdeServer->m_idInst,
											  (LPBYTE)"OK",
											  strlen("OK")+1,
											  0,
											  hsz2,
											  CF_TEXT,
											  0 );
			}
		}
		//�ُ�I��
		return ::DdeCreateDataHandle( m_pDdeServer->m_idInst,
									  (LPBYTE)"NG",
									  strlen("NG")+1,
									  0,
									  hsz2,
									  CF_TEXT,
									  0 );
		break;

	case XTYP_DISCONNECT:			//�u�ؒf�v
#ifdef _DEBUG
//		sprintf(szMsg,"XTYP_DISCONNECT!!");
//		AfxMessageBox(szMsg,(MB_OK|MB_ICONINFORMATION),0);
#endif
		break;
	}
	return NULL;
}


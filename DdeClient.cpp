// DdeClient.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <ddeml.h>					// for DDE
#pragma warning(disable:4786)
#include "DdeClient.h"

#define DDE_SERVICENAME					"Xerxes_DDEServer"
#define DDE_TOPICNAME					"CONTEXT_FILEOPEN"
#define DDE_TIMEOUT						10000

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDdeClient �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDdeClient::CDdeClient()
{
	UINT	unRet=0;

	//�����o�ϐ��N���A
	m_idInst = NULL;
	m_ServiceName = _T("");
	m_TopicName = _T("");
	m_hConv = NULL;

	//�c�c�d������
	unRet = ::DdeInitialize( &m_idInst,
					 DdeCallback,
					 APPCLASS_STANDARD|APPCMD_CLIENTONLY,
					 0L);
#ifdef _DEBUG
//	if(unRet != DMLERR_NO_ERROR)
//		AfxMessageBox("DdeInitialize Error",(MB_OK|MB_ICONSTOP),0);
//	else
//		AfxMessageBox("DdeInitialize OK",(MB_OK|MB_ICONINFORMATION),0);
#endif
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDdeClient::~CDdeClient()
{
	if(m_hConv)
		DisConnect();

	//�c�c�d�I��
	::DdeUninitialize(m_idInst);
}

/////////////////////////////////////////////////////////////////////////////
// CDdeClient �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�c�c�d�T�[�o�[�ւ̐ڑ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDdeClient::Connect()
{
	//�T�[�r�X�l�[��
	m_ServiceName = DDE_SERVICENAME;
	HSZ hszService = ::DdeCreateStringHandle( m_idInst,
											  m_ServiceName,
											  CP_WINANSI );
	//�g�s�b�N�l�[��
	m_TopicName = DDE_TOPICNAME;
	HSZ hszTopic = ::DdeCreateStringHandle( m_idInst,
										    m_TopicName,
											CP_WINANSI );
	//�T�[�o�[�֐ڑ�
	m_hConv = ::DdeConnect( m_idInst,
							hszService,
							hszTopic,
							NULL );
	//���
	::DdeFreeStringHandle( m_idInst, hszService );
	::DdeFreeStringHandle( m_idInst, hszTopic );

	if(m_hConv == 0)
	{
#ifdef _DEBUG
//		AfxMessageBox("DdeConnect ERROR",(MB_OK|MB_ICONSTOP),0);
#endif
		return FALSE;
	}
	else
	{
#ifdef _DEBUG
//		AfxMessageBox("DdeConnect OK",(MB_OK|MB_ICONSTOP),0);
#endif
		return TRUE;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�c�c�d�T�[�o�[����̐ؒf												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDdeClient::DisConnect()
{
	BOOL bRet=FALSE;

	if(m_hConv)
		bRet = ::DdeDisconnect(m_hConv);
	m_hConv = NULL;
#ifdef _DEBUG
//	if(bRet == FALSE)
//		AfxMessageBox("DdeDisConnect ERROR",(MB_OK|MB_ICONSTOP),0);
//	else
//		AfxMessageBox("DdeDisConnect OK",(MB_OK|MB_ICONINFORMATION),0);
#endif
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�c�c�d�T�[�o�[�ւ̃��N�G�X�g											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDdeClient::Request
	(	const char*	pszPath
	)
{
	//�A�C�e���l�[��
	CString objItemName = pszPath;
	HSZ hszItem = ::DdeCreateStringHandle( m_idInst,
										   objItemName,
										   CP_WINANSI );
	HDDEDATA hData = ::DdeClientTransaction( NULL,
											 0L,
											 m_hConv,
											 hszItem,
											 CF_TEXT,
											 XTYP_REQUEST,
											 DDE_TIMEOUT,
											 NULL );
	::DdeFreeStringHandle( m_idInst, hszItem );
	if(hData)
	{
#if 0
//		DWORD dwLen=0;
//		LPBYTE lpbyPtr=NULL;
//		char	szBuf[128];
//		lpbyPtr = ::DdeAccessData(hData,&dwLen);
//		if(lpbyPtr != NULL)
//		{
//			sprintf(szBuf,"return=%s",lpbyPtr);
//#ifdef _DEBUG
//			AfxMessageBox(szBuf,(MB_OK|MB_ICONINFORMATION),0);
//#endif
//		}
//		::DdeUnaccessData(hData);
#endif
		::DdeFreeDataHandle(hData); 
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDdeClient �X�^�e�B�b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�c�c�d�R�[���o�b�N													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
HDDEDATA CALLBACK EXPORT CDdeClient::DdeCallback
	(	UINT		iType,
		UINT		uFmt,
		HCONV		hCconv,
		HSZ			hsz1,			//�g�s�b�N
		HSZ			hsz2,			//����
		HDDEDATA	hData,
		DWORD		dwData1,
		DWORD		dwData2
	)
{
	return (HDDEDATA)NULL;
}


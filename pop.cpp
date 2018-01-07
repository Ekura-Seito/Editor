// Pop.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <io.h>					//for _access
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "Editor.h"
#include "pop.h"
#include "CodeConvManager.h"
#include "Base64.h"
#include "QuotedPrintable.h"
#include "Uudecoder.h"
#include "mymd5.h"				//for APOP

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//�����`�F�b�N���
#define CONNECTION_CHECK 	0
#define USER_CHECK  		1
#define PASSWORD_CHECK 		2
#define APOP_CHECK			3
#define QUIT_CHECK  		4
#define DELETE_CHECK 		5
#define RSET_CHECK  		6
#define STAT_CHECK  		7
#define NOOP_CHECK  		8
#define LIST_CHECK  		9
#define RETR_CHECK  		10
#define TOP_CHECK			11

/////////////////////////////////////////////////////////////////////////////
// CPop �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CPop::CPop()
{
	CString objMsg;

	m_pApp = (CEditorApp*)AfxGetApp();
	m_lSockID = -1;
	m_bIsConnected = FALSE;
	memset(m_szDestIPAddress,0,sizeof(m_szDestIPAddress));
	m_wDestPort = 110;		//POP3 Port
	m_nMailCount = 0;
	m_nByteSum = 0;
	memset(m_szRecvBuf,0,sizeof(m_szRecvBuf));
	m_nMailIndex=0;
	m_nMailSize=0;
	memset(m_szTimeStamp,0,sizeof(m_szTimeStamp));

	m_nRecvRetryCount = 20;
	m_nRecvTimeout = 1;

	//�\�P�b�g�c�k�k�̖����I�����N
	if(m_pApp->m_hSockDllInstance == NULL)
	{
		CWaitCursor	objWaitCursor;
		m_pApp->m_hSockDllInstance = ::LoadLibrary("sock_dll.dll");
		if(m_pApp->m_hSockDllInstance == NULL)
		{
			objMsg.LoadString(IDS_POP_DLLNOTFOUND);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		}
	}

	//�\�P�b�g�c�k�k�֐��A�h���X�擾
	if(m_pApp->m_hSockDllInstance)
	{
		m_pApp->m_lpfnSckFunc1  = (LPFNSCKFUNC1)::GetProcAddress(m_pApp->m_hSockDllInstance,"GetLocalHostInfo");
		m_pApp->m_lpfnSckFunc2  = (LPFNSCKFUNC2)::GetProcAddress(m_pApp->m_hSockDllInstance,"SockServerOpen");
		m_pApp->m_lpfnSckFunc3  = (LPFNSCKFUNC3)::GetProcAddress(m_pApp->m_hSockDllInstance,"SockServerSend");
		m_pApp->m_lpfnSckFunc4  = (LPFNSCKFUNC4)::GetProcAddress(m_pApp->m_hSockDllInstance,"SockServerRecv");
		m_pApp->m_lpfnSckFunc5  = (LPFNSCKFUNC5)::GetProcAddress(m_pApp->m_hSockDllInstance,"SockServerClose");
		m_pApp->m_lpfnSckFunc6  = (LPFNSCKFUNC6)::GetProcAddress(m_pApp->m_hSockDllInstance,"SockClientOpen");
		m_pApp->m_lpfnSckFunc7  = (LPFNSCKFUNC7)::GetProcAddress(m_pApp->m_hSockDllInstance,"SockClientSend");
		m_pApp->m_lpfnSckFunc8  = (LPFNSCKFUNC8)::GetProcAddress(m_pApp->m_hSockDllInstance,"SockClientRecv");
		m_pApp->m_lpfnSckFunc9  = (LPFNSCKFUNC9)::GetProcAddress(m_pApp->m_hSockDllInstance,"SockClientClose");
		m_pApp->m_lpfnSckFunc10 = (LPFNSCKFUNC10)::GetProcAddress(m_pApp->m_hSockDllInstance,"SockClientRecv2");

		if( m_pApp->m_lpfnSckFunc1 == NULL || m_pApp->m_lpfnSckFunc2 == NULL ||
			m_pApp->m_lpfnSckFunc3 == NULL || m_pApp->m_lpfnSckFunc4 == NULL ||
			m_pApp->m_lpfnSckFunc5 == NULL || m_pApp->m_lpfnSckFunc6 == NULL ||
			m_pApp->m_lpfnSckFunc7 == NULL || m_pApp->m_lpfnSckFunc8 == NULL ||
			m_pApp->m_lpfnSckFunc9 == NULL || m_pApp->m_lpfnSckFunc10 == NULL
		)
		{
			objMsg.LoadString(IDS_POP_DLLNOTFOUND);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CPop::~CPop()
{
	Disconnect();
}

/////////////////////////////////////////////////////////////////////////////
// CPop �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F��M�p�����[�^�ݒ�													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CPop::SetConfig
	(	int	nRecvRetryCount,
		int	nRecvTimeout
	)
{
	m_nRecvRetryCount = nRecvRetryCount;
	m_nRecvTimeout = nRecvTimeout;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�o�n�o�T�[�o�[�ւ̐ڑ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CPop::Connect
	(	CString&	objDestIPAddress,		//�T�[�o�[�h�o�A�h���X
		CString& 	objUser,				//���[�U��
		CString&	objPassword,			//�p�X���[�h
		int			nAuthorizationType		//���[�U�F�ؕ����i�O�FUSER,PASS   �P�FAPOP�j
	)
{
	BOOL	bRet=FALSE;
	LONG	lRet=0;
	DWORD	dwRead=0,dwWrite=0;
	char	szBuf[1024],szMsg[1024];
	char	szDigestBase[512],szMD5[64];
	CString objMsg;

	//�\�P�b�g�c�k�k�`�F�b�N
	if(!DllCheck())
		return FALSE;

	//�^�C���X�^���v�ێ��̈揉����
	memset(m_szTimeStamp,0,sizeof(m_szTimeStamp));

	//�p�����[�^�ێ�
	strcpy(m_szDestIPAddress,(LPCTSTR)objDestIPAddress);

	//�o�n�o�T�[�o�\�P�b�g�ڑ�(SockClientOpen)�A��M�X���b�h���N�����Ȃ�
	m_lSockID = m_pApp->m_lpfnSckFunc6(m_szDestIPAddress,m_wDestPort,FALSE);
	if(m_lSockID < 0)
	{
		objMsg.LoadString(IDS_POP_CONNECTERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	ClearRecvBuf();
	bRet = CheckServerResponse(CONNECTION_CHECK,szBuf);
	if(bRet == FALSE)
	{
		objMsg.LoadString(IDS_POP_CONNECTERR);
		sprintf(szMsg,"%s\r\n%s",(LPCTSTR)objMsg,szBuf);
		m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	if(nAuthorizationType == POP3_AUTHENTICATE_STANDARD) //USER, PASS�ɂ��F��
	{
		//USER�R�}���h���M(SockClientSend)
		sprintf(szBuf,"USER %s\r\n", (LPCTSTR)objUser);
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			objMsg.LoadString(IDS_POP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
		ClearRecvBuf();
		bRet = CheckServerResponse(USER_CHECK,szBuf);
		if(bRet == FALSE)
		{
			objMsg.LoadString(IDS_POP_CONNECTERR);
			sprintf(szMsg,"%s\r\n%s",(LPCTSTR)objMsg,szBuf);
			m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//PASS�R�}���h���M(SockClientSend)
		sprintf(szBuf,"PASS %s\r\n", (LPCTSTR)objPassword);
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			objMsg.LoadString(IDS_POP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
		ClearRecvBuf();
		bRet = CheckServerResponse(PASSWORD_CHECK,szBuf);
		if(bRet == FALSE)
		{
			objMsg.LoadString(IDS_POP_CONNECTERR);
			sprintf(szMsg,"%s\r\n%s",(LPCTSTR)objMsg,szBuf);
			m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
	}
	else if(nAuthorizationType == POP3_AUTHENTICATE_APOP) //APOP�ɂ��F��
	{
		//APOP�R�}���h�p�����[�^��Digest�p������쐬�i��F<1896.697170952@dbc.mtview.ca.us>tanstaaf�j
		memset(szDigestBase,0,sizeof(szDigestBase));
		sprintf(szDigestBase,"%s%s",m_szTimeStamp,(LPCTSTR)objPassword);

		//�l�c�T�擾�i��Fc4c9334bac560ecc979e58001b3e22fb�j
		CMd5	objMd5;
		memset(szMD5,0,sizeof(szMD5));
		objMd5.MD5String(szDigestBase,szMD5);

		//APOP�R�}���h���M(SockClientSend)
		sprintf(szBuf,"APOP %s %s\r\n", (LPCTSTR)objUser, szMD5);
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			objMsg.LoadString(IDS_POP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
		ClearRecvBuf();
		bRet = CheckServerResponse(APOP_CHECK,szBuf);
		if(bRet == FALSE)
		{
			objMsg.LoadString(IDS_APOP_CONNECTERR);
			sprintf(szMsg,"%s\r\n%s",(LPCTSTR)objMsg,szBuf);
			m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
	}
	else
	{
		objMsg.LoadString(IDS_POP_CONNECTERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	m_bIsConnected = TRUE;
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�o�n�o�T�[�o�[����̐ؒf												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CPop::Disconnect()
{
	long	lRet=0;
	char	szBuf[1024];
	DWORD	dwWrite=0;

	//�\�P�b�g�c�k�k�`�F�b�N
	if(!DllCheck())
		return;

	if(m_bIsConnected)
	{
		//QUIT�R�}���h���M(SockClientSend)
		strcpy(szBuf,"QUIT\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		//�����m�F
		ClearRecvBuf();
		CheckServerResponse(QUIT_CHECK,szBuf);
	}
	m_bIsConnected = FALSE;

	if(m_lSockID != -1)
	{
		//�\�P�b�g�N���[�Y(SockClientClose)
		m_pApp->m_lpfnSckFunc9(m_lSockID);
	}
	m_lSockID = -1;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[�����擾															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CPop::GetMailCount()
{
	BOOL	bRet=FALSE;
	long	lRet=0;
	char	szBuf[1024],szMsg[1024];
	DWORD	dwWrite=0;
	CString objMsg;

	//�\�P�b�g�c�k�k�`�F�b�N
	if(!DllCheck())
		return -1;
	//���O�`�F�b�N
	if(m_bIsConnected == FALSE)
		return -1;

	//STAT�R�}���h���M(SockClientSend)
	sprintf(szBuf,"STAT\r\n");
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		objMsg.LoadString(IDS_POP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return -1;
	}
	ClearRecvBuf();
	bRet = CheckServerResponse(STAT_CHECK,szBuf);
	if(bRet == FALSE)
	{
		objMsg.LoadString(IDS_POP_SENDERR);
		sprintf(szMsg,"%s\r\n%s",(LPCTSTR)objMsg,szBuf);
		m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);
		return -1;
	}

	return m_nMailCount;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P���[���w�b�_���擾												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CPop::GetMailInfo
	(	int		nMailIndex,				//���[���ԍ��i�P�`�j
		char*	szSubject,				//�����ԋp�p�o�b�t�@
		char*	szFrom,					//���o�l�ԋp�p�o�b�t�@
		char*	szDate,					//���t�ԋp�p�o�b�t�@
		BOOL*	pbIsMultiPart			//�Y�t�t�@�C�����肩�H�ԋp�p�o�b�t�@
	)
{
	BOOL	bRet=FALSE;
	long	lRet=0;
	DWORD	dwWrite=0;
	char	szBuf[1024],szContentType[1024];
	CString objBuf, objMsg;

	//�\�P�b�g�c�k�k�`�F�b�N
	if(!DllCheck())
		return FALSE;

	//���O�`�F�b�N
	if(m_bIsConnected == FALSE)
		return FALSE;
	if(nMailIndex<1 || nMailIndex>m_nMailCount)
		return FALSE;

	//TOP�R�}���h���M(SockClientSend)
	sprintf(szBuf,"TOP %d 0\r\n", nMailIndex);
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		objMsg.LoadString(IDS_POP_TOPERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	ClearRecvBuf();
	bRet = CheckServerResponse(TOP_CHECK,szBuf);
	if(bRet == FALSE)
	{
//DELE�R�}���h�ō폜���TOP�R�}���h�𔭍s�����ꍇ�A
//DELE�������[���ɂ��ẮA-ERR���A���Ă���\��������̂�
//�{���b�Z�[�W�{�b�N�X�͕\�����Ȃ�
//		objMsg.LoadString(IDS_POP_TOPERR);
//		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//�w�b�_�[���P�s���A�u\r\n�v�܂Ŏ�M����
	memset(szBuf,0,sizeof(szBuf));
	for(;;)
	{
		if(!strlen(szBuf))
		{
			memset(szBuf,0,sizeof(szBuf));
			lRet = RecvOneLine(szBuf);
			if(lRet < 0)
				return FALSE;
			if(szBuf[0]=='.' && szBuf[1]!='.')
				return TRUE;
			if(!strlen(szBuf))
				return TRUE;
		}

		if(!strnicmp(szBuf,"Subject:",strlen("Subject:")))
		{
			strcpy(szSubject,&szBuf[strlen("Subject:")]);
			MailHeaderDecode(szSubject);
			for(;;) //�����s�ɕ������ꍇ�����邽�߁A���̍s���`�F�b�N
			{
				memset(szBuf,0,sizeof(szBuf));
				lRet = RecvOneLine(szBuf);
				if(lRet < 0)
					return FALSE;
				if(szBuf[0]=='.' && szBuf[1]!='.')
					return TRUE;
				if(!strlen(szBuf))
					return TRUE;
				if(strchr(szBuf,':') != NULL) //���̃w�b�_�o��
					break;
				else
				{
					MailHeaderDecode(szBuf);
					objBuf = szBuf;
					objBuf.TrimLeft();
					strcat(szSubject,(LPCTSTR)objBuf);
				}
			}
		}
		else if(!strnicmp(szBuf,"From:",strlen("From:")))
		{
			strcpy(szFrom,&szBuf[strlen("From:")]);
			MailHeaderDecode(szFrom);
			for(;;) //�����s�ɕ������ꍇ�����邽�߁A���̍s���`�F�b�N
			{
				memset(szBuf,0,sizeof(szBuf));
				lRet = RecvOneLine(szBuf);
				if(lRet < 0)
					return FALSE;
				if(szBuf[0]=='.' && szBuf[1]!='.')
					return TRUE;
				if(!strlen(szBuf))
					return TRUE;
				if(strchr(szBuf,':') != NULL) //���̃w�b�_�o��
					break;
				else
				{
					MailHeaderDecode(szBuf);
					strcat(szFrom,szBuf);
				}
			}
		}
		else if(!strnicmp(szBuf,"Date:",strlen("Date:")))
		{
			strcpy(szDate,&szBuf[strlen("Date:")]);
			for(;;) //�����s�ɕ������ꍇ�����邽�߁A���̍s���`�F�b�N
			{
				memset(szBuf,0,sizeof(szBuf));
				lRet = RecvOneLine(szBuf);
				if(lRet < 0)
					return FALSE;
				if(szBuf[0]=='.' && szBuf[1]!='.')
					return TRUE;
				if(!strlen(szBuf))
					return TRUE;
				if(strchr(szBuf,':') != NULL) //���̃w�b�_�o��
					break;
				else
				{
					MailHeaderDecode(szBuf);
					strcat(szDate,szBuf);
				}
			}
		}
		else if(!strnicmp(szBuf,"Content-Type:",strlen("Content-Type:")))
		{
			strcpy(szContentType,szBuf);
			MailHeaderDecode(szContentType);
			for(;;) //�����s�ɕ������ꍇ�����邽�߁A���̍s���`�F�b�N
			{
				memset(szBuf,0,sizeof(szBuf));
				lRet = RecvOneLine(szBuf);
				if(lRet < 0)
					return FALSE;
				if(szBuf[0]=='.' && szBuf[1]!='.')
					return TRUE;
				if(!strlen(szBuf))
					return TRUE;
				if(strchr(szBuf,':') != NULL) //���̃w�b�_�o��
					break;
				else
				{
					MailHeaderDecode(szBuf);
					strcat(szContentType,szBuf);
				}

				//Content-Type�w�b�_���
				objBuf = szContentType;
				objBuf.MakeLower();
				if( objBuf.Find("multipart/mixed") != -1 )
					(*pbIsMultiPart) = TRUE;
				else
					(*pbIsMultiPart) = FALSE;
			}

			//Content-Type�w�b�_���
			objBuf = szContentType;
			objBuf.MakeLower();
			if( objBuf.Find("multipart/mixed") != -1 )
				(*pbIsMultiPart) = TRUE;
			else
				(*pbIsMultiPart) = FALSE;
		}
		else
		{
			memset(szBuf,0,sizeof(szBuf));
		}
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P���[���T�C�Y���擾												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CPop::GetMailSize
	(	int		nMailIndex,				//���[���ԍ��i�P�`�j
		int*	pnSize
	)
{
	char	*p=NULL;
	BOOL	bRet=FALSE;
	long	lRet=0;
	DWORD	dwWrite=0;
	char	szBuf[1024];
	CString objBuf, objMsg;

	(*pnSize) = 0;

	//�\�P�b�g�c�k�k�`�F�b�N
	if(!DllCheck())
		return FALSE;

	//���O�`�F�b�N
	if(m_bIsConnected == FALSE)
		return FALSE;
	if(nMailIndex<1 || nMailIndex>m_nMailCount)
		return FALSE;

	//LIST�R�}���h���M(SockClientSend)
	sprintf(szBuf,"LIST %d\r\n", nMailIndex);
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		objMsg.LoadString(IDS_POP_LISTERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	ClearRecvBuf();
	bRet = CheckServerResponse(LIST_CHECK,szBuf);
	if(bRet == FALSE)
	{
//DELE�R�}���h�ō폜���LIST�R�}���h�𔭍s�����ꍇ�A
//DELE�������[���ɂ��ẮA-ERR���A���Ă���\��������̂�
//�{���b�Z�[�W�{�b�N�X�͕\�����Ȃ�
//		objMsg.LoadString(IDS_POP_LISTERR);
//		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	if(m_nMailIndex == nMailIndex)
	{
		(*pnSize) = m_nMailSize;
		return TRUE;
	}
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P���[����M															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CPop::RecvOneMail
	(	int		nMailIndex,				//��M���[���ԍ��i�P�`�j
		char*	szMailSaveFilePath,		//��M���[�����e�ۑ���t�@�C���p�X
		char*	szAttachPath			//�Y�t�t�@�C���ۑ���p�X
	)
{
	BYTE	byBuf=0;
	int		nPos=0,nCnt=0,nEncodeType=0;
	BOOL	bRet=FALSE;
	long	lRet=0;
	DWORD	dwWrite=0;
	char	szBuf[1024],szMsg[1024],szTemp[1024];
	CString objBuf, objMsg;
	CString objContentType,objContentTransferEncoding,objContentDisposition;
	CString objAttachFileName;
	FILE*	fp=NULL;
	BOOL	bIsMultiPartMail=FALSE;			// �}���`�p�[�g���[�����H
	char	szBoundary[1024];				// �Y�t�t�@�C������̏ꍇ�̋��E

	//�\�P�b�g�c�k�k�`�F�b�N
	if(!DllCheck())
		return FALSE;

	//���O�`�F�b�N
	if(m_bIsConnected == FALSE)
		return FALSE;
	if(nMailIndex<1 || nMailIndex>m_nMailCount)
		return FALSE;

	//��M���[�����e�ۑ���t�@�C���I�[�v��
	fp=fopen(szMailSaveFilePath,"w");
	if(fp==NULL)
		return FALSE;

	//RETR�R�}���h���M(SockClientSend)
	sprintf(szBuf,"RETR %d\r\n", nMailIndex);
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		objMsg.LoadString(IDS_POP_RETRERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		fclose(fp);
		return FALSE;
	}
	ClearRecvBuf();
	bRet = CheckServerResponse(RETR_CHECK,szBuf);
	if(bRet == FALSE)
	{
		objMsg.LoadString(IDS_POP_RETRERR);
		sprintf(szMsg,"%s\r\n%s",(LPCTSTR)objMsg,szBuf);
		m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);
		fclose(fp);
		return FALSE;
	}

	////////////////////////////////
	//���[���w�b�_��M�u\r\n�v�܂�//
	////////////////////////////////
	memset(szBuf,0,sizeof(szBuf));
	for(;;)
	{
		if(!strlen(szBuf))
		{
			memset(szBuf,0,sizeof(szBuf));
			lRet = RecvOneLine(szBuf);
			if(lRet < 0)
			{
				fclose(fp);
				return FALSE;
			}
			if(!strlen(szBuf))
			{
				fprintf(fp,"%s\n",szBuf);
				break;
			}
		}

		if(!strnicmp(szBuf,"Content-Type:",strlen("Content-Type:")))
		{
			MailHeaderDecode(szBuf);
			fprintf(fp,"%s\n",szBuf);
			objContentType = szBuf;
			for(;;) //�����s�ɕ������ꍇ�����邽�߁A���̍s���`�F�b�N
			{
				memset(szBuf,0,sizeof(szBuf));
				lRet = RecvOneLine(szBuf);
				if(lRet < 0)
				{
					fclose(fp);
					return FALSE;
				}
				if(!strlen(szBuf))
				{
					fprintf(fp,"%s\n",szBuf);
					break;
				}
				if(strchr(szBuf,':') != NULL) //���̃w�b�_�o��
					break;
				else
				{
					MailHeaderDecode(szBuf);
					objBuf = szBuf;
					objBuf.TrimLeft();
					objContentType += objBuf;
					fprintf(fp,"%s\n",szBuf);
				}
			}
		}
		else
		{
			MailHeaderDecode(szBuf);
			fprintf(fp,"%s\n",szBuf);
			memset(szBuf,0,sizeof(szBuf));
		}
	}

	//Content-Type�w�b�_���
	objBuf = objContentType;
	objBuf.MakeLower();
	if( objBuf.Find("multipart/mixed") != -1 &&
		(nPos = objBuf.Find("boundary=")) != -1
	)
	{
		bIsMultiPartMail = TRUE;
		memset(szBoundary,0,sizeof(szBoundary));
		//�o�E���_���擾
		nPos += strlen("boundary=");
		strcpy(szBoundary,"--");
		for(nCnt=strlen(szBoundary); nPos<objContentType.GetLength(); nPos++)
		{
			byBuf = objContentType.GetAt(nPos);
			if(byBuf=='"')
				continue;
			if(isspace(byBuf))
				break;
			szBoundary[nCnt] = byBuf;
			nCnt++;
		}
		strcat(szBoundary,"--");
		//�Y�t�t�@�C���ۑ���f�B���N�g���쐬
		if( _access( szAttachPath, 0 ) == -1 )
			::CreateDirectory(szAttachPath,NULL);
	}
	else
	{
		bIsMultiPartMail = FALSE;
		memset(szBoundary,0,sizeof(szBoundary));
	}

	//////////////////////////////////////////////
	//�}���`�p�[�g�ł΂Ȃ��ꍇ�̃��[���{�f�B��M//
	//////////////////////////////////////////////
	if(!bIsMultiPartMail) //�}���`�p�[�g�ł͂Ȃ��ꍇ
	{
		for(;;)
		{
			memset(szBuf,0,sizeof(szBuf));
			lRet = RecvOneLine(szBuf);
			if(lRet < 0)
			{
				fclose(fp);
				return FALSE;
			}
			if(szBuf[0]=='.' && szBuf[1]!='.')//���[���I���̂��邵
			{
				fprintf(fp,"%s\n",szBuf);
				break;
			}
			if(szBuf[0]=='.' && szBuf[1]=='.')//'.'���Q�񑱂��Ă���
			{
				//'.'���P�ɂ���
				memset(szTemp,0,sizeof(szTemp));
				strcpy(szTemp,&szBuf[1]);
				strcpy(szBuf,szTemp);
			}

			MailBodyDecode(szBuf);
			fprintf(fp,"%s\n",szBuf);
		}
		fclose(fp);
		return TRUE;
	}

	////////////////////////////////////////
	//�}���`�p�[�g�̏ꍇ�̃��[���{�f�B��M//
	////////////////////////////////////////
	nEncodeType = -1;
	objAttachFileName.Empty();
	memset(szBuf,0,sizeof(szBuf));
	for(;;)
	{
		//�P�s��M
		if(!strlen(szBuf))
		{
			memset(szBuf,0,sizeof(szBuf));
			lRet = RecvOneLine(szBuf);
			if(lRet < 0)
			{
				fclose(fp);
				return FALSE;
			}
			if(szBuf[0]=='.' && szBuf[1]!='.')//���[���I���̂��邵
			{
				fprintf(fp,"%s\n",szBuf);
				break;
			}
			if(szBuf[0]=='.' && szBuf[1]=='.')//'.'���Q�񑱂��Ă���
			{
				//'.'���P�ɂ���
				memset(szTemp,0,sizeof(szTemp));
				strcpy(szTemp,&szBuf[1]);
				strcpy(szBuf,szTemp);
			}
		}

		if(!strnicmp(szBuf,szBoundary,strlen(szBoundary)-2)) //�o�E���_������
		{
			fprintf(fp,"%s\n",szBuf);
			if(!strnicmp(szBuf,szBoundary,strlen(szBoundary))) //�S�p�[�g�I��
				break;

			//�T�u�w�b�_�[���
			objContentType.Empty();
			objContentTransferEncoding.Empty();
			objContentDisposition.Empty();
			lRet = AnalyzeSubHeader(fp,objContentType,objContentTransferEncoding,objContentDisposition);
			if(lRet <= 0)
			{
				fclose(fp);
				return FALSE;
			}

			//�G���R�[�h�^�C�v����
			objBuf = objContentTransferEncoding;
			objBuf.MakeLower();
			nEncodeType = -1;
			if( objBuf.Find("base64") != -1 )
				nEncodeType = SMTP_ENCODE_BASE64;
			else if( objBuf.Find("quoted-printable") != -1 )
				nEncodeType = SMTP_ENCODE_QUOTED;
			else if( objBuf.Find("x-uuencode") != -1 )
				nEncodeType = SMTP_ENCODE_UUENCODE;

			//�Y�t�t�@�C�����擾
			objBuf = objContentDisposition;
			objBuf.MakeLower();
			objAttachFileName.Empty();
			if( (nPos = objBuf.Find("filename=")) != -1 )
			{
				char* pPtr = objContentDisposition.GetBuffer(1024);
				if(pPtr!=NULL)
				{
					for(nPos+=strlen("filename="); nPos<objContentDisposition.GetLength(); )
					{
						if( IsDBCSLeadByteEx( *(pPtr+nPos), 0 ) )
						{
							objAttachFileName += *(pPtr+nPos);
							nPos++;
							objAttachFileName += *(pPtr+nPos);
							nPos++;
						}
						else
						{
							if( *(pPtr+nPos) == '"' )
								nPos++;
							else if( *(pPtr+nPos) == ' ' || *(pPtr+nPos) == '\t' )
								break;
							else
							{
								objAttachFileName += *(pPtr+nPos);
								nPos++;
							}
						}
					}
					objContentDisposition.ReleaseBuffer();
				}
			}
			if( nEncodeType != -1 && !objAttachFileName.IsEmpty() )
			{
				//�f�R�[�h���ʃt�@�C�����v���C�x�[�g�w�b�_�Ƃ��ďo�́i�T�u�w�b�_�I���t���j
				fprintf(fp,"X-Xerxes-Attachment-File: \"%s\\%s\"\n\n",szAttachPath,(LPCTSTR)objAttachFileName);
				//�Y�t�t�@�C���f�R�[�h
				if(nEncodeType == SMTP_ENCODE_BASE64 || nEncodeType == SMTP_ENCODE_UUENCODE)
					lRet = DecodeAttachFile(szAttachPath,objAttachFileName,nEncodeType);
				else if(nEncodeType == SMTP_ENCODE_QUOTED)
					lRet = DecodeQuotedAttachFile(szAttachPath,objAttachFileName);
				memset(szBuf,0,sizeof(szBuf));
			}
			else
			{
				//�T�u�w�b�_�[�I���s�o��
				fprintf(fp,"\n");
				memset(szBuf,0,sizeof(szBuf));
			}
		}
		else
		{
			//���̂܂܏o��
			MailBodyDecode(szBuf);
			fprintf(fp,"%s\n",szBuf);
			memset(szBuf,0,sizeof(szBuf));
		}
	}

	fclose(fp);
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P���[���폜															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CPop::DelOneMail
	(	int		nMailIndex				//��M���[���ԍ��i�P�`�j
	)
{
	BOOL	bRet=FALSE;
	long	lRet=0;
	char	szBuf[1024],szMsg[1024];
	DWORD	dwWrite=0;
	CString objMsg;

	//�\�P�b�g�c�k�k�`�F�b�N
	if(!DllCheck())
		return FALSE;

	//���O�`�F�b�N
	if(m_bIsConnected == FALSE)
		return FALSE;
	if(nMailIndex<1 || nMailIndex>m_nMailCount)
		return FALSE;

	//DELE�R�}���h���M(SockClientSend)
	sprintf(szBuf,"DELE %d\r\n", nMailIndex);
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		objMsg.LoadString(IDS_POP_DELERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	ClearRecvBuf();
	bRet = CheckServerResponse(DELETE_CHECK,szBuf);
	if(bRet == FALSE)
	{
		objMsg.LoadString(IDS_POP_DELERR);
		sprintf(szMsg,"%s\r\n%s", (LPCTSTR)objMsg,szBuf);
		m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPop �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�\�P�b�g�c�k�k�`�F�b�N												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CPop::DllCheck()
{
	if(m_pApp->m_hSockDllInstance == NULL)
		return FALSE;

	if( m_pApp->m_lpfnSckFunc1 == NULL || m_pApp->m_lpfnSckFunc2 == NULL ||
		m_pApp->m_lpfnSckFunc3 == NULL || m_pApp->m_lpfnSckFunc4 == NULL ||
		m_pApp->m_lpfnSckFunc5 == NULL || m_pApp->m_lpfnSckFunc6 == NULL ||
		m_pApp->m_lpfnSckFunc7 == NULL || m_pApp->m_lpfnSckFunc8 == NULL ||
		m_pApp->m_lpfnSckFunc9 == NULL || m_pApp->m_lpfnSckFunc10 == NULL
	)
	{
		return FALSE;
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�o�n�o�T�[�o�[���X�|���X�`�F�b�N										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CPop::CheckServerResponse
	(	int		nCheckType,
		char*	szBuf
	)
{
	long	lRet=0;
	int		nCnt=0;
	char	*p=NULL;
	BOOL	bRet=FALSE,bEmailNumber=FALSE;
	CString objMsg;

	if(nCheckType == CONNECTION_CHECK)//�T�[�o�ڑ��`�F�b�N
	{
		for(;;)
		{
			memset(szBuf,0,sizeof(szBuf));
			lRet = RecvOneLine(szBuf);
			if(lRet < 0)
			{
				if(lRet == -2)
					objMsg.LoadString(IDS_POP_TIMEOUT);
				else if(lRet == -3)
					objMsg.LoadString(IDS_POP_DISCONNECTED);

				strcpy(szBuf,(LPCTSTR)objMsg);
				return FALSE;
			}
			if(!strnicmp(szBuf,"+OK",3))
			{
				//banner greeting����<>�ł�����ꂽ�^�C���X�^���v�𒊏o����
				p = strchr(szBuf,'<');
				if(p != NULL)
				{
					for(nCnt=0; *p!=NULL; p++)
					{
						m_szTimeStamp[nCnt] = *p;
						nCnt++;
						if(*p == '>')
							break;
					}
				}
				return TRUE;
			}
			else if(!strnicmp(szBuf,"-ERR",4))
				return FALSE;
		}
	}
	else if(nCheckType == USER_CHECK		||	//USER�R�}���h�����`�F�b�N
			nCheckType == QUIT_CHECK		||	//QUIT�R�}���h�����`�F�b�N
			nCheckType == DELETE_CHECK		||	//DELE�R�}���h�����`�F�b�N
			nCheckType == RSET_CHECK		||	//RSET�R�}���h�����`�F�b�N
			nCheckType == NOOP_CHECK			//NOOP�R�}���h�����`�F�b�N
	)
	{
		for(;;)
		{
			memset(szBuf,0,sizeof(szBuf));
			lRet = RecvOneLine(szBuf);
			if(lRet < 0)
			{
				if(lRet == -2)
					objMsg.LoadString(IDS_POP_TIMEOUT);
				else if(lRet == -3)
					objMsg.LoadString(IDS_POP_DISCONNECTED);

				strcpy(szBuf,(LPCTSTR)objMsg);
				return FALSE;
			}
			if(!strnicmp(szBuf,"+OK",3))
				return TRUE;
			else if(!strnicmp(szBuf,"-ERR",4))
				return FALSE;
		}
	}
	else if(nCheckType == STAT_CHECK) //STAT�R�}���h�����`�F�b�N
	{
		for(;;)
		{
			memset(szBuf,0,sizeof(szBuf));
			lRet = RecvOneLine(szBuf);
			if(lRet < 0)
			{
				if(lRet == -2)
					objMsg.LoadString(IDS_POP_TIMEOUT);
				else if(lRet == -3)
					objMsg.LoadString(IDS_POP_DISCONNECTED);

				strcpy(szBuf,(LPCTSTR)objMsg);
				return FALSE;
			}
			if(!strnicmp(szBuf,"+OK",3))
			{
				for(bEmailNumber=TRUE,p=&szBuf[strlen("+OK")]; *p!=NULL; p++)
				{
					if(*p=='\t' || *p==' ')
					{
						if(bEmailNumber)//���[�������擾
						{
							m_nMailCount = atoi(p); 
							bEmailNumber = FALSE; 
						}
						else //���o�C�g���擾
						{
							m_nByteSum = atoi(p); 
							break;
						}
					}
				}
				return TRUE;
			}
			else if(!strnicmp(szBuf,"-ERR",4))
			{
				m_nMailCount=0;
				m_nByteSum=0;
				return FALSE;
			}
		}
	}
	else if(nCheckType == LIST_CHECK) //LIST�R�}���h�����`�F�b�N
	{
		for(;;)
		{
			memset(szBuf,0,sizeof(szBuf));
			lRet = RecvOneLine(szBuf);
			if(lRet < 0)
			{
				if(lRet == -2)
					objMsg.LoadString(IDS_POP_TIMEOUT);
				else if(lRet == -3)
					objMsg.LoadString(IDS_POP_DISCONNECTED);

				strcpy(szBuf,(LPCTSTR)objMsg);
				return FALSE;
			}
			if(!strnicmp(szBuf,"+OK",3))
			{
				for(bEmailNumber=TRUE,p=&szBuf[strlen("+OK")]; *p!=NULL; p++)
				{
					if(*p=='\t' || *p==' ')
					{
						if(bEmailNumber)//���[���ԍ��擾
						{
							m_nMailIndex = atoi(p); 
							bEmailNumber = FALSE; 
						}
						else //���[���T�C�Y�擾
						{
							m_nMailSize = atoi(p); 
							break;
						}
					}
				}
				return TRUE;
			}
			else if(!strnicmp(szBuf,"-ERR",4))
			{
				m_nMailIndex=0;
				m_nMailSize=0;
				return FALSE;
			}
		}
	}
	else if(nCheckType == TOP_CHECK)
	{
		//+OK .....���b�Z�[�W�̎��ɁA�u�w�b�_�[\r\n�v���u.\r\n�v�܂ő���
		for(;;)
		{
			memset(szBuf,0,sizeof(szBuf));
			lRet = RecvOneLine(szBuf);
			if(lRet < 0)
			{
				if(lRet == -2)
					objMsg.LoadString(IDS_POP_TIMEOUT);
				else if(lRet == -3)
					objMsg.LoadString(IDS_POP_DISCONNECTED);

				strcpy(szBuf,(LPCTSTR)objMsg);
				return FALSE;
			}
			if(!strnicmp(szBuf,"+OK",3))
				return TRUE;
			else if(!strnicmp(szBuf,"-ERR",4))
				return FALSE;
		}
	}
	else if(nCheckType == RETR_CHECK)
	{
		//+OK .....���b�Z�[�W�̎��ɁA�u�w�b�_�[�{�{�f�B�v���u.\r\n�v�܂ő���
		for(;;)
		{
			memset(szBuf,0,sizeof(szBuf));
			lRet = RecvOneLine(szBuf);
			if(lRet < 0)
			{
				if(lRet == -2)
					objMsg.LoadString(IDS_POP_TIMEOUT);
				else if(lRet == -3)
					objMsg.LoadString(IDS_POP_DISCONNECTED);

				strcpy(szBuf,(LPCTSTR)objMsg);
				return FALSE;
			}
			if(!strnicmp(szBuf,"+OK",3))
				return TRUE;
			else if(!strnicmp(szBuf,"-ERR",4))
				return FALSE;
		}
	}
	else if( nCheckType == PASSWORD_CHECK || //PASS�R�}���h�����`�F�b�N
			 nCheckType == APOP_CHECK
	)
	{
		for(;;)
		{
			memset(szBuf,0,sizeof(szBuf));
			lRet = RecvOneLine(szBuf);
			if(lRet < 0)
			{
				if(lRet == -2)
					objMsg.LoadString(IDS_POP_TIMEOUT);
				else if(lRet == -3)
					objMsg.LoadString(IDS_POP_DISCONNECTED);

				strcpy(szBuf,(LPCTSTR)objMsg);
				return FALSE;
			}
			if(!strnicmp(szBuf,"+OK",3))
				return TRUE;
			else if(!strnicmp(szBuf,"-ERR",4))
				return FALSE;
		}
	}
	else
	{
		for(;;)
		{
			memset(szBuf,0,sizeof(szBuf));
			lRet = RecvOneLine(szBuf);
			if(lRet < 0)
			{
				if(lRet == -2)
					objMsg.LoadString(IDS_POP_TIMEOUT);
				else if(lRet == -3)
					objMsg.LoadString(IDS_POP_DISCONNECTED);

				strcpy(szBuf,(LPCTSTR)objMsg);
				return FALSE;
			}
			if(!strnicmp(szBuf,"+OK",3))
				return TRUE;
			else if(!strnicmp(szBuf,"-ERR",4))
				return FALSE;
		}
	}

	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�s��M�o�b�t�@�N���A												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CPop::ClearRecvBuf()
{
	m_dwRecvByte = 0;
	memset(m_szRecvBuf,0,sizeof(m_szRecvBuf));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�s��M																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LONG CPop::RecvOneLine				//  1 : ����I��
									// -2 : ��M�ς݃f�[�^����
									// -3 : �ڑ����؂ꂽ
	(	char*	szBuf
	)
{
	char	*pr=NULL,*pn=NULL;		//m_szRecvBuf����'\r','\n'�̏o���ʒu
	int		nPosR=-1,nPosN=-1;		//m_szRecvBuf����'\r','\n'�̏o���ʒu�I�t�Z�b�g
	int		nLen=0;
	int		nRetLen=0;				//szBuf�֕ԋp�����o�C�g��
	LONG	lRet=0;

	for(;;)
	{
		//'\r', '\n'����
		nPosR = nPosN = -1;
		pr = (char*)memchr(m_szRecvBuf,'\r',m_dwRecvByte);
		pn = (char*)memchr(m_szRecvBuf,'\n',m_dwRecvByte);
		if(pr != NULL)
			nPosR = pr - m_szRecvBuf;
		if(pn != NULL)
			nPosN = pn - m_szRecvBuf; 

		if(pr != NULL && (pn==NULL||nPosR<nPosN) ) 
		{
			//\r�̑O�܂ł�ԋp
			nLen = pr - m_szRecvBuf;
			memcpy(&szBuf[nRetLen],m_szRecvBuf,nLen);
			nRetLen += nLen;
			if(m_szRecvBuf[nLen+1] == '\n')
			{
				//"\n"��ǂݎ̂Ă��đO�ɋl�߂�
				memmove(&m_szRecvBuf[0],&m_szRecvBuf[nLen+2],m_dwRecvByte-nLen-2);
				m_dwRecvByte -= (nLen+2);
			}
			else
			{
				//�O�ɋl�߂�
				memmove(&m_szRecvBuf[0],&m_szRecvBuf[nLen+1],m_dwRecvByte-nLen-1);
				m_dwRecvByte -= (nLen+1);
			}
			return TRUE;
		}
		else if(pn != NULL && (pr==NULL||nPosN<nPosR))
		{
			//\n�̑O�܂ł�ԋp
			nLen = pn - m_szRecvBuf;
			memcpy(&szBuf[nRetLen],m_szRecvBuf,nLen);
			nRetLen += nLen;
			//�O�ɋl�߂�
			memmove(&m_szRecvBuf[0],&m_szRecvBuf[nLen+1],m_dwRecvByte-nLen-1);
			m_dwRecvByte -= (nLen+1);
			return TRUE;
		}
		else
		{
			if(m_dwRecvByte != 0)
			{
				memcpy(&szBuf[nRetLen],m_szRecvBuf,m_dwRecvByte);
				nRetLen += m_dwRecvByte;
				m_dwRecvByte = 0;
			}

			//�\�P�b�g����ǂ�
			lRet = RecvToLocal();
			if(lRet < 0)
				return lRet;
		}
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�Fm_szRecvBuf�ֈꊇ�ǂݍ���												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LONG CPop::RecvToLocal()				//  0 : ����I��
										// -2 : ���g���C�A�E�g
										// -3 : �ڑ����؂ꂽ
{
	LONG	lRet=0;
	int		nRetryCnt=0;

	for(;;)
	{
		//�\�P�b�g����ǂ�
		m_dwRecvByte = 0;
		memset(m_szRecvBuf,0,sizeof(m_szRecvBuf));
		lRet = m_pApp->m_lpfnSckFunc10( m_lSockID,	//�\�P�b�g�h�c
								(LPVOID)m_szRecvBuf,//�o�b�t�@
								2048-4,				//��M�o�C�g���w��
								m_nRecvTimeout,		//�^�C���A�E�g�i���j
								&m_dwRecvByte);		//��M�ł����o�C�g��
		if(lRet == -2) //��M�ς݃f�[�^�Ȃ�
		{
			nRetryCnt++;
			if(nRetryCnt > m_nRecvRetryCount)
				return lRet;
			else
				continue;
		}
		else
			return lRet;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[���w�b�_�f�R�[�h													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CPop::MailHeaderDecode
	(	char*	szIn			//���[���w�b�_�P�s
	)
{
	LONG	lLen=0;
	char	*pTop=NULL,*pEnd=NULL;
	char	szBuf[1024],szEncoded[1024],szDecoded[1024];
	int		nFrom=0,nTo=0,nEncodeType=0,nLen=0;
	CString objBuf;
	CBase64				objBase64;
	CQuotedPrintable	objQuoted;
	CCodeConvManager	objCodeConvManager;

	//���[���w�b�_�̂l�h�l�d�f�R�[�h(Base64,Quoted-Printable)
	memset(szBuf,0,sizeof(szBuf));
	for(nFrom=0,nTo=0; nFrom<(int)strlen(szIn); )
	{
		if(strnicmp(&szIn[nFrom],"=?",2)) //���[���w�b�_�G���R�[�h�̐擪�łȂ�
		{
			//�f�R�[�h��p�o�b�t�@�ւ��̂܂܃R�s�[
			szBuf[nTo] = szIn[nFrom];
			nTo++;
			nFrom++;
			continue;
		}
		if(strnicmp(&szIn[nFrom],"=?ISO-2022-JP",strlen("=?ISO-2022-JP"))) //ISO-2022-JP�łȂ�
		{
			//"=?"���f�R�[�h��p�o�b�t�@�ւ��̂܂܃R�s�[
			szBuf[nTo] = szIn[nFrom];
			nTo++;	nFrom++;
			szBuf[nTo] = szIn[nFrom];
			nTo++;	nFrom++;
			continue;
		}

		//�G���R�[�h�^�C�v����
		if(!strnicmp(&szIn[nFrom],"=?ISO-2022-JP?B?",strlen("=?ISO-2022-JP?B?")))
			nEncodeType = SMTP_ENCODE_BASE64;
		else if(!strnicmp(&szIn[nFrom],"=?ISO-2022-JP?Q?",strlen("=?ISO-2022-JP?Q?")))
			nEncodeType = SMTP_ENCODE_QUOTED;
		else
		{
			//"=?ISO-2022-JP"���f�R�[�h��p�o�b�t�@�ւ��̂܂܃R�s�[
			strncpy(&szBuf[nTo],&szIn[nFrom],strlen("=?ISO-2022-JP"));
			nTo += strlen("=?ISO-2022-JP");
			nFrom += strlen("=?ISO-2022-JP");
			continue;
		}

		//�G���R�[�h�I����������
		pEnd = strstr(&szIn[nFrom],"?=");
		if(pEnd==NULL)
		{
			//"=?ISO-2022-JP"�ȍ~�S�Ă��f�R�[�h��p�o�b�t�@�ւ��̂܂܃R�s�[
			strcat(&szBuf[nTo],&szIn[nFrom]);
			break;
		}

		//�G���R�[�h�������o��
		memset(szEncoded,0,sizeof(szEncoded));
		if(nEncodeType == SMTP_ENCODE_BASE64)
		{
			nLen = pEnd - &szIn[nFrom+strlen("=?ISO-2022-JP?B?")];
			strncpy(szEncoded,&szIn[nFrom+strlen("=?ISO-2022-JP?B?")],nLen);
		}
		else if(nEncodeType == SMTP_ENCODE_QUOTED)
		{
			nLen = pEnd - &szIn[nFrom+strlen("=?ISO-2022-JP?Q?")];
			strncpy(szEncoded,&szIn[nFrom+strlen("=?ISO-2022-JP?Q?")],nLen);
		}
		objBuf = szEncoded;

		//�f�R�[�h
		memset(szDecoded,0,sizeof(szDecoded));
		if(nEncodeType == SMTP_ENCODE_BASE64)
			objBase64.StringDecode(&objBuf,(LPBYTE)szDecoded,&lLen);
		else if(nEncodeType == SMTP_ENCODE_QUOTED)
			objQuoted.StringDecode(&objBuf,(LPBYTE)szDecoded,&lLen);

		//�f�R�[�h���ʊi�[
		strcpy(&szBuf[nTo],szDecoded);
		nTo += strlen(szDecoded);
		nFrom = pEnd - szIn + 2;
	}

	//RFC1468(ISO-2022-JP)�f�R�[�h
	objBuf = szBuf;
	objCodeConvManager.StringJisToSjis(&objBuf);

	//���ʕԋp
	strcpy(szIn,(LPCTSTR)objBuf);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[���{�̃f�R�[�h													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CPop::MailBodyDecode
	(	char*	szIn			//���[���{�f�B�P�s
	)
{
	CString objBuf;
	CCodeConvManager	objCodeConvManager;

	//RFC1468(ISO-2022-JP)�f�R�[�h
	objBuf = szIn;
	objCodeConvManager.StringJisToSjis(&objBuf);

	//���ʕԋp
	strcpy(szIn,(LPCTSTR)objBuf);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�T�u�w�b�_�[���														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CPop::AnalyzeSubHeader							// -2,-3 : RecvOneLine�ُ�I��
													// 1     : �T�u�w�b�_�[�I���F��
	(	FILE*		fp,
		CString&	objContentType,
		CString&	objContentTransferEncoding,
		CString&	objContentDisposition
	)
{
	long	lRet=0;
	char	szBuf[1024];
	CString objBuf;

	memset(szBuf,0,sizeof(szBuf));
	for(;;)
	{
		if(!strlen(szBuf))
		{
			memset(szBuf,0,sizeof(szBuf));
			lRet = RecvOneLine(szBuf);
			if(lRet < 0)
			{
				fclose(fp);
				return lRet;
			}
			if(!strlen(szBuf)) //�T�u�w�b�_�[�I��
			{
//�T�u�w�b�_�I���s�́A��ʊ֐��ɂďo�͂��邽�߂����ł̓t�@�C���o�͂��Ȃ�
//				fprintf(fp,"%s\n",szBuf);
				return 1;
			}
		}

		if(!strnicmp(szBuf,"Content-Type:",strlen("Content-Type:")))
		{
			MailHeaderDecode(szBuf);
			fprintf(fp,"%s\n",szBuf);
			objContentType = szBuf;
			for(;;) //�����s�ɕ������ꍇ�����邽�߁A���̍s���`�F�b�N
			{
				memset(szBuf,0,sizeof(szBuf));
				lRet = RecvOneLine(szBuf);
				if(lRet < 0)
				{
					fclose(fp);
					return lRet;
				}
				if(!strlen(szBuf))
				{
//�T�u�w�b�_�I���s�́A��ʊ֐��ɂďo�͂��邽�߂����ł̓t�@�C���o�͂��Ȃ�
//					fprintf(fp,"%s\n",szBuf);
					return 1;
				}
				if(strchr(szBuf,':') != NULL) //���̃w�b�_�o��
					break;
				else
				{
					MailHeaderDecode(szBuf);
					objBuf = szBuf;
					objBuf.TrimLeft();
					objContentType += objBuf;
					fprintf(fp,"%s\n",szBuf);
				}
			}
		}
		else if(!strnicmp(szBuf,"Content-Transfer-Encoding:",strlen("Content-Transfer-Encoding:")))
		{
			MailHeaderDecode(szBuf);
			fprintf(fp,"%s\n",szBuf);
			objContentTransferEncoding = szBuf;
			for(;;) //�����s�ɕ������ꍇ�����邽�߁A���̍s���`�F�b�N
			{
				memset(szBuf,0,sizeof(szBuf));
				lRet = RecvOneLine(szBuf);
				if(lRet < 0)
				{
					fclose(fp);
					return lRet;
				}
				if(!strlen(szBuf))
				{
//�T�u�w�b�_�I���s�́A��ʊ֐��ɂďo�͂��邽�߂����ł̓t�@�C���o�͂��Ȃ�
//					fprintf(fp,"%s\n",szBuf);
					return 1;
				}
				if(strchr(szBuf,':') != NULL) //���̃w�b�_�o��
					break;
				else
				{
					MailHeaderDecode(szBuf);
					objBuf = szBuf;
					objBuf.TrimLeft();
					objContentTransferEncoding += objBuf;
					fprintf(fp,"%s\n",szBuf);
				}
			}
		}
		else if(!strnicmp(szBuf,"Content-Disposition:",strlen("Content-Disposition:")))
		{
			MailHeaderDecode(szBuf);
			fprintf(fp,"%s\n",szBuf);
			objContentDisposition = szBuf;
			for(;;) //�����s�ɕ������ꍇ�����邽�߁A���̍s���`�F�b�N
			{
				memset(szBuf,0,sizeof(szBuf));
				lRet = RecvOneLine(szBuf);
				if(lRet < 0)
				{
					fclose(fp);
					return lRet;
				}
				if(!strlen(szBuf))
				{
//�T�u�w�b�_�I���s�́A��ʊ֐��ɂďo�͂��邽�߂����ł̓t�@�C���o�͂��Ȃ�
//					fprintf(fp,"%s\n",szBuf);
					return 1;
				}
				if(strchr(szBuf,':') != NULL) //���̃w�b�_�o��
					break;
				else
				{
					MailHeaderDecode(szBuf);
					objBuf = szBuf;
					objBuf.TrimLeft();
					objContentDisposition += objBuf;
					fprintf(fp,"%s\n",szBuf);
				}
			}
		}
		else
		{
			MailHeaderDecode(szBuf);
			fprintf(fp,"%s\n",szBuf);
			memset(szBuf,0,sizeof(szBuf));
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�Y�t�t�@�C���f�R�[�h(Base64,Uuencode�p)								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CPop::DecodeAttachFile
	(	char*		szAttachPath,
		CString&	objAttachFileName,
		int			nEncodeType
	)
{
	long	lRet=0,lLen=0;
	char	szBuf[1024],szTemp[1024];
	char	szPath[_MAX_PATH];
	FILE*	fp=NULL;
	char	szDecoded[1024];
	CString objBuf;
	CBase64		objBase64;
	CUudecoder	objUudecode;
	BOOL	bRet=FALSE;

	//�Y�t�t�@�C���p�X�ҏW
	sprintf(szPath,"%s\\%s", szAttachPath,(LPCTSTR)objAttachFileName);
	fp = fopen(szPath,"wb");
	if(fp==NULL)
		return FALSE;

	//�f�R�[�h
	objBuf.Empty();
	for(;;)
	{
		memset(szBuf,0,sizeof(szBuf));
		lRet = RecvOneLine(szBuf);
		if(lRet < 0)
		{
			fclose(fp);
			return lRet;
		}
		if(!strlen(szBuf)) //�T�u�p�[�g�I��
		{
			fclose(fp);
			return 1;
		}
		if(szBuf[0]=='.' && szBuf[1]=='.')//'.'���Q�񑱂��Ă���
		{
			//'.'���P�ɂ���
			memset(szTemp,0,sizeof(szTemp));
			strcpy(szTemp,&szBuf[1]);
			strcpy(szBuf,szTemp);
		}

		objBuf = szBuf;
		if( nEncodeType == SMTP_ENCODE_UUENCODE)
		{
			if( objBuf.Find("begin") != -1 )
				continue;
			if( objBuf.Find("end") != -1 )
				continue;
		}

		if(!objBuf.IsEmpty())
		{
			lLen=0;
			memset(szDecoded,0,sizeof(szDecoded));
			if(nEncodeType == SMTP_ENCODE_BASE64)
				bRet = objBase64.StringDecode(&objBuf,(LPBYTE)szDecoded,&lLen);
			else if(nEncodeType == SMTP_ENCODE_UUENCODE)
				bRet = objUudecode.StringDecode(&objBuf,(LPBYTE)szDecoded,&lLen);

			//�Y�t�t�@�C���o��
			if(bRet)
				fwrite(szDecoded,sizeof(BYTE),lLen,fp);
		}
	}

	fclose(fp);
	return 1;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�Y�t�t�@�C���f�R�[�h(Quoted-Printable�p)								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CPop::DecodeQuotedAttachFile
	(	char*		szAttachPath,
		CString&	objAttachFileName
	)
{
	long	lRet=0,lLen=0;
	char	szBuf[1024],szTemp[1024];
	char	szPath[_MAX_PATH];
	FILE*	fp=NULL;
	char	szDecoded[1024];
	CString objBuf;
	CQuotedPrintable	objQuoted;
	BOOL	bRet=FALSE;

	//�Y�t�t�@�C���p�X�ҏW
	sprintf(szPath,"%s\\%s", szAttachPath,(LPCTSTR)objAttachFileName);
	fp = fopen(szPath,"wb");
	if(fp==NULL)
		return FALSE;

	//�f�R�[�h
	objBuf.Empty();
	for(;;)
	{
		memset(szBuf,0,sizeof(szBuf));
		lRet = RecvOneLine(szBuf);
		if(lRet < 0)
		{
			fclose(fp);
			return lRet;
		}
		if(!strlen(szBuf)) //�T�u�p�[�g�I��
		{
			fclose(fp);
			return 1;
		}
		if(szBuf[0]=='.' && szBuf[1]=='.')//'.'���Q�񑱂��Ă���
		{
			//'.'���P�ɂ���
			memset(szTemp,0,sizeof(szTemp));
			strcpy(szTemp,&szBuf[1]);
			strcpy(szBuf,szTemp);
		}

		if(!objBuf.IsEmpty()) //�O��ǂݍ��񂾃f�[�^����
		{
			//�s����"="�łȂ��ꍇ�ɂ́A���������f�[�^�̉��s�Ƃ��ĉ��߂���
			if(objBuf.GetAt(objBuf.GetLength()-1)!='=')
				fwrite("\r\n",sizeof(BYTE),2,fp);
		}

		objBuf.Empty();
		objBuf = szBuf;
		if(!objBuf.IsEmpty())
		{
			lLen=0;
			memset(szDecoded,0,sizeof(szDecoded));
			bRet = objQuoted.StringDecode(&objBuf,(LPBYTE)szDecoded,&lLen);
			//�Y�t�t�@�C���o��
			if(bRet)
				fwrite(szDecoded,sizeof(BYTE),lLen,fp);
		}
	}

	fclose(fp);
	return 1;
}


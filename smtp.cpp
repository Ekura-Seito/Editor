// Smtp.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <afxtempl.h>
#include "Editor_Def.h"
#include "Editor.h"
#include "Line.h"
#include "LineManager.h"
#include "ColorManager.h"
#include "UndoItem.h"
#include "UndoManager.h"
#include "EditorDoc.h"
#include "smtp.h"
#include "CodeConvManager.h"
#include "Base64.h"
#include "QuotedPrintable.h"
#include "Uuencoder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmtp �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CSmtp::CSmtp()
{
	CString objMsg;

	m_pApp = (CEditorApp*)AfxGetApp();
	m_lSockID = -1;
	m_bIsConnected = FALSE;
	memset(m_szDestIPAddress,0,sizeof(m_szDestIPAddress));
	m_wDestPort = 25;

	m_bIsSendDate = FALSE;
	m_bIsSendMessageID = FALSE;
	m_bIsReqMsgDispsition = FALSE;

	m_objSubject    = _T("");
	m_objFrom       = _T("");
	m_objMessageID  = _T("");
	m_objReplyTo    = _T("");
	m_objReference  = _T("");
	m_objBoundary   = _T("--=_Xerxes_Beta_Version_Boundary_051108532000_=--");
	m_objMIMEText   = _T("This is a multi-part message in MIME format.");
	m_objMailerName = _T("Xerxes Beta Version");
	m_nEncodeType   = SMTP_ENCODE_BASE64;
	m_objSignature  = _T("");

	m_lpfnDllFunc1 = NULL;
	m_lpfnDllFunc2 = NULL;
	m_lpfnDllFunc3 = NULL;
	m_lpfnDllFunc4 = NULL;
	m_lpfnDllFunc5 = NULL;
	m_lpfnDllFunc6 = NULL;
	m_lpfnDllFunc7 = NULL;
	m_lpfnDllFunc8 = NULL;
	m_lpfnDllFunc9 = NULL;
	m_lpfnDllFunc10 = NULL;

	//�\�P�b�g�c�k�k�̖����I�����N
	if(m_pApp->m_hSockDllInstance == NULL)
	{
		CWaitCursor	objWaitCursor;
		m_pApp->m_hSockDllInstance = ::LoadLibrary("sock_dll.dll");
		if(m_pApp->m_hSockDllInstance == NULL)
		{
			objMsg.LoadString(IDS_SMTP_DLLNOTFOUND);
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
			objMsg.LoadString(IDS_SMTP_DLLNOTFOUND);
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
CSmtp::~CSmtp()
{
	Disconnect();

	m_objTo.RemoveAll();
	m_objCc.RemoveAll();
	m_objBcc.RemoveAll();
	m_objAttachFiles.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// CSmtp �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����ݒ�																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSmtp::SetSubject
	(	CString	objStr
	)
{
	if(objStr.IsEmpty())
		return;

	if(CheckExistNonAscii(objStr)) //��ASCII�R�[�h���݃`�F�b�N
	{
		//�w�b�_�G���R�[�h
		MessageHeaderEncode(&objStr);
	}
	m_objSubject = objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���o�l�ݒ�															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSmtp::SetFrom
	(	CString	objStr
	)
{
	if(objStr.IsEmpty())
		return;

	m_objFrom = objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���l�ݒ�															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSmtp::SetTo
	(	CString	objStr
	)
{
	if(objStr.IsEmpty())
		return;

	m_objTo.Add(objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�J�[�{���R�s�[���l�ݒ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSmtp::SetCc
	(	CString	objStr
	)
{
	if(objStr.IsEmpty())
		return;

	m_objCc.Add(objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u���C���h�J�[�{���R�s�[���l�ݒ�									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSmtp::SetBcc
	(	CString	objStr
	)
{
	if(objStr.IsEmpty())
		return;

	m_objBcc.Add(objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ԐM��ݒ�															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSmtp::SetReplyTo
	(	CString	objStr
	)
{
	if(objStr.IsEmpty())
		return;

	m_objReplyTo = objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���t�@�����X�ݒ�														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSmtp::SetReference
	(	CString	objStr
	)
{
	if(objStr.IsEmpty())
		return;

	m_objReference = objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�Y�t�t�@�C���ݒ�														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSmtp::SetAttachFile
	(	CString	objStr
	)
{
	if(objStr.IsEmpty())
		return;

	m_objAttachFiles.Add(objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�G���R�[�h�^�C�v�ݒ�													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSmtp::SetEncodeType
	(	int	nEncodeType
	)
{
	if( nEncodeType != SMTP_ENCODE_BASE64 && 
		nEncodeType != SMTP_ENCODE_QUOTED &&
		nEncodeType != SMTP_ENCODE_UUENCODE
	)
	{
		return;
	}
	m_nEncodeType = nEncodeType;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����ݒ�																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSmtp::SetSignature
	(	CString objStr
	)
{
	if(objStr.IsEmpty())
		return;

	m_objSignature = objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�r�l�s�o�T�[�o�[�ւ̐ڑ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CSmtp::Connect
	(	CString	objDestIPAddress //�T�[�o�[�h�o�A�h���X
	)
{
	BOOL	bRet=FALSE;
	int		nSmtpResponseCode=0;
	LONG	lRet=0;
	DWORD	dwRead=0,dwWrite=0;
	char	szBuf[1024];
	char	szHostName[64];		// �z�X�g���ԋp�p�o�b�t�@
	char	szIPAddress[64];	// �h�o�A�h���X�ԋp�p�o�b�t�@
	CString objMsg;

	//�\�P�b�g�c�k�k�`�F�b�N
	if(!DllCheck())
		return FALSE;

	//�p�����[�^�ێ�
	strcpy(m_szDestIPAddress,(LPCTSTR)objDestIPAddress);

	//�N���C�A���g�\�P�b�g�I�[�v��(SockClientOpen)
	m_lSockID = m_pApp->m_lpfnSckFunc6(m_szDestIPAddress,m_wDestPort,TRUE);
	if(m_lSockID < 0)
	{
		objMsg.LoadString(IDS_SMTP_CONNECTERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//Greeting Message�̎�M�m�F
	bRet = CheckServerResponse(&nSmtpResponseCode);
	if(bRet == FALSE || nSmtpResponseCode != 220)
	{
		CancelDisconnect();
		DispSmtpMessage(nSmtpResponseCode,_T(""));
		objMsg.LoadString(IDS_SMTP_CONNECTERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//�z�X�g���擾(GetLocalHostInfo)
	memset(szHostName,0,sizeof(szHostName));
	memset(szIPAddress,0,sizeof(szIPAddress));
	lRet = m_pApp->m_lpfnSckFunc1(szHostName,szIPAddress);
	if(lRet == -1 || lRet == -2)
	{
		CancelDisconnect();
		objMsg.LoadString(IDS_SMTP_LOCALHOSTNAMEERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	else if(lRet == -3)
	{
		CancelDisconnect();
		objMsg.LoadString(IDS_SMTP_LOCALIPERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//HELLO�R�}���h���M(SockClientSend)
	sprintf(szBuf,"HELO %s\r\n", szHostName);
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		CancelDisconnect();
		objMsg.LoadString(IDS_SMTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//HELLO�R�}���h�̎�M�m�F
	bRet = CheckServerResponse(&nSmtpResponseCode);
	if(bRet == FALSE || nSmtpResponseCode != 250)
	{
		CancelDisconnect();
		DispSmtpMessage(nSmtpResponseCode,_T(""));
		objMsg.LoadString(IDS_SMTP_CONNECTERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	m_bIsConnected = TRUE;
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�r�l�s�o�T�[�o�[����̐ؒf											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSmtp::Disconnect()
{
	long	lRet=0;
	char	szBuf[1024];
	DWORD	dwWrite=0;
	int		nSmtpResponseCode=0;

	//�\�P�b�g�c�k�k�`�F�b�N
	if(!DllCheck())
		return;

	if(m_bIsConnected)
	{
		//QUIT�R�}���h���M(SockClientSend)
		strcpy(szBuf,"QUIT\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		//�����m�F
		CheckServerResponse(&nSmtpResponseCode);
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
/*|	�T�v	�F���[�����M															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CSmtp::SendMail
	(	char*	pszMailFile		//���[�����e�t�@�C��
	)
{
	BOOL	bRet=FALSE;
	long	lRet=0, lCnt=0;
	char	szBuf[1024];
	DWORD	dwWrite=0;
	int		nSmtpResponseCode=0;
	CString objMsg;

	//�\�P�b�g�c�k�k�`�F�b�N
	if(!DllCheck())
		return FALSE;

	//���O�`�F�b�N
	if(m_bIsConnected == FALSE)
		return FALSE;
	if(m_objFrom.IsEmpty() || m_objTo.GetSize() == 0)
		return FALSE;

	//MAIL�R�}���h���M(SockClientSend)
	sprintf(szBuf,"MAIL From: <%s>\r\n", (LPCTSTR)m_objFrom);
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		CancelDisconnect();
		objMsg.LoadString(IDS_SMTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	bRet = CheckServerResponse(&nSmtpResponseCode);
	if(bRet == FALSE || nSmtpResponseCode != 250)
	{
		CancelDisconnect();
		DispSmtpMessage(nSmtpResponseCode,_T(""));
		objMsg.LoadString(IDS_SMTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//RCPT�R�}���h���M(SockClientSend)
	for(lCnt=0; lCnt<m_objTo.GetSize(); lCnt++)
	{
		CString objBuf = m_objTo.GetAt(lCnt);
		sprintf(szBuf,"RCPT TO: <%s>\r\n", (LPCTSTR)objBuf);
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		bRet = CheckServerResponse(&nSmtpResponseCode);
		if( bRet == FALSE || 
			(nSmtpResponseCode != 250 && nSmtpResponseCode != 251)
		)
		{
			CancelDisconnect();
			DispSmtpMessage(nSmtpResponseCode,objBuf);
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
	}

	//RCPT�R�}���h���M(SockClientSend)
	for(lCnt=0; lCnt<m_objCc.GetSize(); lCnt++)
	{
		CString objBuf = m_objCc.GetAt(lCnt);
		sprintf(szBuf,"RCPT TO: <%s>\r\n", (LPCTSTR)objBuf);
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		bRet = CheckServerResponse(&nSmtpResponseCode);
		if( bRet == FALSE || 
			(nSmtpResponseCode != 250 && nSmtpResponseCode != 251)
		)
		{
			CancelDisconnect();
			DispSmtpMessage(nSmtpResponseCode,objBuf);
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
	}

	//RCPT�R�}���h���M(SockClientSend)
	for(lCnt=0; lCnt<m_objBcc.GetSize(); lCnt++)
	{
		CString objBuf = m_objBcc.GetAt(lCnt);
		sprintf(szBuf,"RCPT TO: <%s>\r\n", (LPCTSTR)objBuf);
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		bRet = CheckServerResponse(&nSmtpResponseCode);
		if( bRet == FALSE || 
			(nSmtpResponseCode != 250 && nSmtpResponseCode != 251)
		)
		{
			CancelDisconnect();
			DispSmtpMessage(nSmtpResponseCode,objBuf);
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
	}

	//DATA�R�}���h���M(SockClientSend)
	strcpy(szBuf,"DATA\r\n");
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		CancelDisconnect();
		objMsg.LoadString(IDS_SMTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	bRet = CheckServerResponse(&nSmtpResponseCode);
	if(	bRet == FALSE || 
		(nSmtpResponseCode != 250 && nSmtpResponseCode != 354)
	)
	{
		CancelDisconnect();
		DispSmtpMessage(nSmtpResponseCode,_T(""));
		objMsg.LoadString(IDS_SMTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//���[���w�b�_���M
	if(SendMailHeader() == FALSE)
		return FALSE;

	//���[���{�����M
	if(SendMailBody(pszMailFile) == FALSE)
		return FALSE;

	//�Y�t�t�@�C�����M
	if(m_objAttachFiles.GetSize() > 0)
	{
		if(SendAttachedFiles() == FALSE)
			return FALSE;
	}

	//���[���I���ʒm
	if(SendMailEnd() == FALSE)
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSmtp �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[���w�b�_���M														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CSmtp::SendMailHeader()
{
	BOOL	bRet=FALSE;
	long	lRet=0, lCnt=0;
	char	szBuf[1024];
	DWORD	dwWrite=0;
	int		nSmtpResponseCode=0;
	time_t	t_now;
	struct  tm* ptm=NULL;
	CString objMsg;

	//�\�P�b�g�c�k�k�`�F�b�N
	if(!DllCheck())
		return FALSE;

	//���O�`�F�b�N
	if(m_bIsConnected == FALSE)
		return FALSE;
	if(m_objFrom.IsEmpty() || m_objTo.GetSize() == 0)
		return FALSE;

	//From �w�b�_�[(SockClientSend)
	sprintf(szBuf,"From: <%s>\r\n", (LPCTSTR)m_objFrom);
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		CancelDisconnect();
		objMsg.LoadString(IDS_SMTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//To �w�b�_�[(SockClientSend)
	strcpy(szBuf,"To: ");
	for(lCnt=0; lCnt<m_objTo.GetSize(); lCnt++)
	{
		if(lCnt>0)
			strcat(szBuf,",");
		CString objBuf = m_objTo.GetAt(lCnt);
		sprintf(&szBuf[strlen(szBuf)], "<%s>", (LPCTSTR)objBuf);
	}
	strcat(szBuf,"\r\n");
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		CancelDisconnect();
		objMsg.LoadString(IDS_SMTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//Cc �w�b�_�[(SockClientSend)
	if(m_objCc.GetSize() > 0)
	{
		strcpy(szBuf,"Cc: ");
		for(lCnt=0; lCnt<m_objCc.GetSize(); lCnt++)
		{
			if(lCnt>0)
				strcat(szBuf,",");
			CString objBuf = m_objCc.GetAt(lCnt);
			sprintf(&szBuf[strlen(szBuf)], "<%s>", (LPCTSTR)objBuf);
		}
		strcat(szBuf,"\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
	}

	//Bcc �w�b�_�[(SockClientSend)
	if(m_objBcc.GetSize() > 0)
	{
		strcpy(szBuf,"Bcc: ");
		for(lCnt=0; lCnt<m_objBcc.GetSize(); lCnt++)
		{
			if(lCnt>0)
				strcat(szBuf,",");
			CString objBuf = m_objBcc.GetAt(lCnt);
			sprintf(&szBuf[strlen(szBuf)], "<%s>", (LPCTSTR)objBuf);
		}
		strcat(szBuf,"\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
	}

	//Date �w�b�_�[(SockClientSend)
	t_now = time(NULL);
	ptm = localtime(&t_now);
	if(m_bIsSendDate)
	{
		strftime(szBuf,sizeof(szBuf),"Date: %a, %d %b %Y %H:%M:%S", ptm );
		sprintf(&szBuf[strlen(szBuf)], " %s\r\n", (LPCTSTR)GetTimeZone() );
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
	}

	//Subject �w�b�_�[(SockClientSend)
	sprintf(szBuf,"Subject: %s\r\n", (LPCTSTR)m_objSubject);
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		CancelDisconnect();
		objMsg.LoadString(IDS_SMTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//Message-ID �w�b�_�[(SockClientSend)
	if(m_bIsSendMessageID)
	{
		strftime(szBuf,sizeof(szBuf),"Message-Id: <%Y%m%d%H%M%S.", ptm );
		strcat(szBuf,(LPCTSTR)m_objFrom);
		strcat(szBuf,">\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
	}

	//Disposition-Notification-To�w�b�_�[(SockClientSend)
	if(m_bIsReqMsgDispsition)
	{
		sprintf(szBuf,"Disposition-Notification-To: <%s>\r\n", (LPCTSTR)m_objFrom);
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
	}

	//Reply-To �w�b�_�[(SockClientSend)
	if(!m_objReplyTo.IsEmpty())
	{
		sprintf(szBuf,"Reply-To: <%s>\r\n", (LPCTSTR)m_objReplyTo );
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
	}

	//Reference �w�b�_�[(SockClientSend)
	if(!m_objReference.IsEmpty())
	{
		sprintf(szBuf,"References: <%s>\r\n", (LPCTSTR)m_objReference );
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
	}

	//X-Mailer �w�b�_�[(SockClientSend)
	sprintf(szBuf,"X-Mailer: %s\r\n", (LPCTSTR)m_objMailerName);
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		CancelDisconnect();
		objMsg.LoadString(IDS_SMTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	if(m_objAttachFiles.GetSize() > 0) //�Y�t�t�@�C������
	{
		//MIME-Version �w�b�_�[(SockClientSend)
		strcpy(szBuf,"MIME-Version: 1.0\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//Content-type �w�b�_�[(SockClientSend)
		sprintf(szBuf,"Content-Type: multipart/mixed; boundary=\"%s\"\r\n", (LPCTSTR)m_objBoundary );
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
	}
	else //�Y�t�t�@�C���Ȃ�
	{
		//Content-type �w�b�_�[(SockClientSend)
		strcpy(szBuf,"Content-Type: text/plain; charset=\"iso-2022-jp\"\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//Content-Transfer-Encoding �w�b�_�[(SockClientSend)
		strcpy(szBuf,"Content-Transfer-Encoding: 7bit\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
	}

	//�w�b�_�[�I��(SockClientSend)
	strcpy(szBuf,"\r\n");
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		CancelDisconnect();
		objMsg.LoadString(IDS_SMTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[���{�����M														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CSmtp::SendMailBody
	(	char*		pszMailFile			//���[�����e�t�@�C��
	)
{
	BOOL	bRet=FALSE;
	char	szBuf[1024];
	long	lRet=0, lCnt=0;
	DWORD	dwWrite=0;
	CLine*	pobjLine=NULL;
	CString objBuf=_T("");
	CCodeConvManager objCodeConvManager;
	FILE*	fp=NULL;
	CString objMsg;

	//�\�P�b�g�c�k�k�`�F�b�N
	if(!DllCheck())
		return FALSE;

	//���O�`�F�b�N
	if(m_bIsConnected == FALSE)
		return FALSE;
	if(pszMailFile == NULL)
		return FALSE;

	if(m_objAttachFiles.GetSize() > 0) //�Y�t�t�@�C������
	{
		//MIME Text(SockClientSend)
		sprintf(szBuf,"%s\r\n\r\n", (LPCTSTR)m_objMIMEText);
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//�u���b�N���E(SockClientSend)
		sprintf(szBuf,"--%s\r\n", (LPCTSTR)m_objBoundary);
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//Content-type �w�b�_�[(SockClientSend)
		strcpy(szBuf,"Content-Type: text/plain; charset=\"iso-2022-jp\"\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//Content-Transfer-Encoding �w�b�_�[(SockClientSend)
		strcpy(szBuf,"Content-Transfer-Encoding: 7bit\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//�T�u�w�b�_�[�̏I��(SockClientSend)
		strcpy(szBuf,"\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
	}

	//���e���M
	fp = fopen(pszMailFile,"r");
	if(fp!=NULL)
	{
		for(;;)
		{
			memset(szBuf,0,sizeof(szBuf));
			if(fgets(szBuf,sizeof(szBuf),fp) == NULL)
			{
				if(feof(fp))
					break;
			}

			if(szBuf[0] == '.') //�s���� '.'���A'..'�ɂ���
			{
				objBuf = ".";
				objBuf += szBuf;
			}
			else
				objBuf = szBuf;

			//���p�J�^�J�i��S�p�֕ϊ�
			objCodeConvManager.StringKatakanaHan2Zen(&objBuf);

			//RFC1468(ISO-2022-JP)�G���R�[�h
			objCodeConvManager.StringSjisToJis(&objBuf);

			//�P�s���M(SockClientSend)
			lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,(char*)(LPCTSTR)objBuf,objBuf.GetLength(),&dwWrite);
			if(lRet == -2 || lRet == -3 || dwWrite != (DWORD)objBuf.GetLength())
			{
				CancelDisconnect();
				objMsg.LoadString(IDS_SMTP_SENDERR);
				m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
				return FALSE;
			}
		}
		fclose(fp);
	}

	if(!m_objSignature.IsEmpty()) //��������
	{
		//�{���Ə����̊Ԃ̂P�s(SockClientSend)
		strcpy(szBuf,"\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		objBuf = m_objSignature;

		//���p�J�^�J�i��S�p�֕ϊ�
		objCodeConvManager.StringKatakanaHan2Zen(&objBuf);

		//RFC1468(ISO-2022-JP)�G���R�[�h
		objCodeConvManager.StringSjisToJis(&objBuf);

		//�P�s���M(SockClientSend)
		objBuf += "\r\n";
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,(char*)(LPCTSTR)objBuf,objBuf.GetLength(),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != (DWORD)objBuf.GetLength())
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�Y�t�t�@�C�����M														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CSmtp::SendAttachedFiles()
{
	long	lCnt=0;
	long	lRet=0;
	char	szBuf[1024], szMsg[1024];
	char	szFName[_MAX_FNAME], szExt[_MAX_EXT];
	DWORD	dwWrite=0;
	BYTE	byBuf[1024];
	UINT	nBytesRead=0;
	CFile	objFile;
	CString objBuf;
	CString objAttachFileName;
	CBase64	objBase64;
	CQuotedPrintable objQuoted;
	CUuencoder objUuencode;
	CCodeConvManager	objCodeConvManager;
	int		nReadLength=0;
	CString objMsg;

	//�\�P�b�g�c�k�k�`�F�b�N
	if(!DllCheck())
		return FALSE;

	if(m_nEncodeType == SMTP_ENCODE_BASE64)
		nReadLength = 54;
	else if(m_nEncodeType == SMTP_ENCODE_QUOTED)
		nReadLength = 22;
	else if(m_nEncodeType == SMTP_ENCODE_UUENCODE)
		nReadLength = 45;

	//���O�`�F�b�N
	if(m_bIsConnected == FALSE)
		return FALSE;
	if(m_objAttachFiles.GetSize() == 0)
		return TRUE;

	for(lCnt=0; lCnt<m_objAttachFiles.GetSize(); lCnt++)
	{
		//�Y�t�t�@�C�����擾
		strcpy(szBuf,(LPCTSTR)m_objAttachFiles.GetAt(lCnt));
		_tsplitpath( szBuf, NULL, NULL, szFName, szExt );

		//�Y�t�t�@�C�����G���R�[�h
		objAttachFileName.Format("%s%s", szFName, szExt );
		if(CheckExistNonAscii(objAttachFileName))
			MessageHeaderEncode(&objAttachFileName);

		//�Y�t�t�@�C���I�[�v��
		if( !objFile.Open( szBuf, CFile::modeRead|CFile::typeBinary ) )
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_ATTACHFILEOPENERR);
			sprintf(szMsg,(LPCTSTR)objMsg,szBuf);
			m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//�u���b�N���E(SockClientSend)
		sprintf(szBuf,"\r\n--%s\r\n", (LPCTSTR)m_objBoundary);
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//Content-Type �w�b�_�[(SockClientSend)
		sprintf(szBuf,"Content-Type: application/octet-stream; file=%s\r\n", (LPCTSTR)objAttachFileName);
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//Content-Transfer-Encoding �w�b�_�[(SockClientSend)
		if(m_nEncodeType == SMTP_ENCODE_BASE64)
			strcpy(szBuf,"Content-Transfer-Encoding: base64\r\n");
		else if(m_nEncodeType == SMTP_ENCODE_QUOTED)
			strcpy(szBuf,"Content-Transfer-Encoding: quoted-printable\r\n");
		else if(m_nEncodeType == SMTP_ENCODE_UUENCODE)
			strcpy(szBuf,"Content-Transfer-Encoding: x-uuencode\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//Content-Disposition �w�b�_�[(SockClientSend)
		sprintf(szBuf,"Content-Disposition: attachment; filename=%s\r\n", (LPCTSTR)objAttachFileName);
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//�T�u�w�b�_�[�̏I��(SockClientSend)
		strcpy(szBuf,"\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//uuencode�̏ꍇ�̂�begin�s
		if(m_nEncodeType == SMTP_ENCODE_UUENCODE)
		{
			objBuf.Format("%s%s", szFName, szExt );
			objCodeConvManager.StringSjisToJis(&objBuf);
			sprintf(szBuf, "begin 666 %s\r\n", (LPCTSTR)objBuf);
			lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
			if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
			{
				CancelDisconnect();
				objMsg.LoadString(IDS_SMTP_SENDERR);
				m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
				return FALSE;
			}
		}

		//�Y�t�t�@�C�����e���M
		if( m_nEncodeType == SMTP_ENCODE_BASE64 || m_nEncodeType == SMTP_ENCODE_UUENCODE )
		{
			objBuf.Empty();
			for(;;)
			{
				try
				{
					memset(byBuf,0,sizeof(byBuf));
					nBytesRead = objFile.Read( byBuf, nReadLength );
				}
				catch( CFileException* e )
				{
					delete e;
					break;
				}

				if(nBytesRead == 0)
					break;

				//�G���R�[�h
				if(m_nEncodeType == SMTP_ENCODE_BASE64)
					objBuf = objBase64.BytesEncode(byBuf,nBytesRead);
				else if(m_nEncodeType == SMTP_ENCODE_UUENCODE)
					objBuf = objUuencode.BytesEncode(byBuf,nBytesRead);

				//���M(SockClientSend)
				sprintf(szBuf,"%s\r\n",(LPCTSTR)objBuf);
				lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
				if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
				{
					CancelDisconnect();
					objMsg.LoadString(IDS_SMTP_SENDERR);
					m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
					return FALSE;
				}
			}
		}
		else if( m_nEncodeType == SMTP_ENCODE_QUOTED )
		{
			objBuf.Empty();
			for(;;)
			{
				try
				{
					memset(byBuf,0,sizeof(byBuf));
					nBytesRead = objFile.Read( byBuf, nReadLength );
				}
				catch( CFileException* e )
				{
					delete e;
					break;
				}

				if(!objBuf.IsEmpty())
				{
					if(nBytesRead != 0)
						objBuf += "=";
					//�O��ǂ񂾕��𑗐M(SockClientSend)
					sprintf(szBuf,"%s\r\n",(LPCTSTR)objBuf);
					lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
					if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
					{
						CancelDisconnect();
						objMsg.LoadString(IDS_SMTP_SENDERR);
						m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
						return FALSE;
					}
				}

				if(nBytesRead == 0)
					break;

				//�G���R�[�h
				objBuf.Empty();
				objBuf = objQuoted.BytesEncode(byBuf,nBytesRead);
			}
		}

		//�Y�t�t�@�C���N���[�Y
		objFile.Close();

		//uuencode�̏ꍇ�̂�end�s
		if(m_nEncodeType == SMTP_ENCODE_UUENCODE)
		{
			strcpy(szBuf, "`\r\n");
			lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
			if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
			{
				CancelDisconnect();
				objMsg.LoadString(IDS_SMTP_SENDERR);
				m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
				return FALSE;
			}

			strcpy(szBuf, "end\r\n");
			lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
			if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
			{
				CancelDisconnect();
				objMsg.LoadString(IDS_SMTP_SENDERR);
				m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
				return FALSE;
			}
		}
	}

	//�u���b�N���E�I��(SockClientSend)
	sprintf(szBuf,"\r\n--%s--\r\n", (LPCTSTR)m_objBoundary);
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		CancelDisconnect();
		objMsg.LoadString(IDS_SMTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[���I���ʒm														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CSmtp::SendMailEnd()
{
	long	lRet=0;
	char	szBuf[1024];
	DWORD	dwWrite=0;
	int		nSmtpResponseCode=0;
	CString objMsg;

	//�\�P�b�g�c�k�k�`�F�b�N
	if(!DllCheck())
		return FALSE;

	//���O�`�F�b�N
	if(m_bIsConnected == FALSE)
		return FALSE;

	//�{���I��(SockClientSend)
	strcpy(szBuf,"\r\n.\r\n");
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		CancelDisconnect();
		objMsg.LoadString(IDS_SMTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	//�����m�F
	CheckServerResponse(&nSmtpResponseCode);

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�r�l�s�o�����R�[�h���b�Z�[�W�\��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSmtp::DispSmtpMessage
	(	int nSmtpResponseCode,
		CString objHint
	)
{
	char	szBuf[1024];
	CString objMsg;

	switch(nSmtpResponseCode)
	{
	case 211:
		objMsg.LoadString(IDS_SMTP_RESCODE_211);
		break;
	case 214:
		objMsg.LoadString(IDS_SMTP_RESCODE_214);
		break;
	case 220:
		objMsg.LoadString(IDS_SMTP_RESCODE_220);
		break;
	case 221:
		objMsg.LoadString(IDS_SMTP_RESCODE_221);
		break;
	case 250:
		objMsg.LoadString(IDS_SMTP_RESCODE_250);
		break;
	case 251:
		objMsg.LoadString(IDS_SMTP_RESCODE_251);
		sprintf(szBuf,(LPCTSTR)objMsg,(LPCTSTR)objHint);
		objMsg = szBuf;
		break;
	case 252:
		objMsg.LoadString(IDS_SMTP_RESCODE_252);
		break;
	case 354:
		objMsg.LoadString(IDS_SMTP_RESCODE_354);
		break;
	case 421:
		objMsg.LoadString(IDS_SMTP_RESCODE_421);
		break;
	case 450:
		objMsg.LoadString(IDS_SMTP_RESCODE_450);
		break;
	case 451:
		objMsg.LoadString(IDS_SMTP_RESCODE_451);
		break;
	case 500:
		objMsg.LoadString(IDS_SMTP_RESCODE_500);
		break;
	case 501:
		objMsg.LoadString(IDS_SMTP_RESCODE_501);
		break;
	case 502:
		objMsg.LoadString(IDS_SMTP_RESCODE_502);
		break;
	case 503:
		objMsg.LoadString(IDS_SMTP_RESCODE_503);
		break;
	case 504:
		objMsg.LoadString(IDS_SMTP_RESCODE_504);
		break;
	case 550:
		objMsg.LoadString(IDS_SMTP_RESCODE_550);
		sprintf(szBuf,(LPCTSTR)objMsg,(LPCTSTR)objHint);
		objMsg = szBuf;
		break;
	case 551:
		objMsg.LoadString(IDS_SMTP_RESCODE_551);
		sprintf(szBuf,(LPCTSTR)objMsg,(LPCTSTR)objHint);
		objMsg = szBuf;
		break;
	case 552:
		objMsg.LoadString(IDS_SMTP_RESCODE_552);
		break;
	case 553:
		objMsg.LoadString(IDS_SMTP_RESCODE_553);
		break;
	case 554:
		objMsg.LoadString(IDS_SMTP_RESCODE_554);
		break;
	default:
		return;
	}

	m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�^�C���]�[��������擾												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CString	CSmtp::GetTimeZone()
{
	SYSTEMTIME locsystime;
	FILETIME locfiletime;
	FILETIME sysfiletime;
	DWORDLONG ldw1;
	DWORDLONG ldw2;
	LONG lBias;
	TCHAR chSign;

	GetLocalTime(&locsystime);
	SystemTimeToFileTime(&locsystime, &locfiletime);
	LocalFileTimeToFileTime(&locfiletime, &sysfiletime);

	ldw1 = static_cast<DWORDLONG>(locfiletime.dwHighDateTime) << 32;
	ldw1 +=	locfiletime.dwLowDateTime;
	ldw2 = static_cast<DWORDLONG>(sysfiletime.dwHighDateTime) << 32;
	ldw2 +=	sysfiletime.dwLowDateTime;
	ldw1 -=	ldw2;
	lBias =	static_cast<LONG>(ldw1 / 600000000);

	chSign = _T('+');
	if (lBias <	0)
	{
		chSign = _T('-');
		lBias =	-lBias;
	}

	CString strTZ;
	strTZ.Format(_T("%c%02ld%02ld"), chSign, lBias/60, lBias%60);
	return strTZ;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�r�l�s�o�T�[�o�[����̃L�����Z���ؒf									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSmtp::CancelDisconnect()
{
	long	lRet=0;
	char	szBuf[1024];
	DWORD	dwWrite=0;
	int		nSmtpResponseCode=0;

	//�\�P�b�g�c�k�k�`�F�b�N
	if(!DllCheck())
		return;

	if(m_bIsConnected)
	{
		//RSET�R�}���h���M(SockClientSend)
		strcpy(szBuf,"RSET\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		//�����m�F
		CheckServerResponse(&nSmtpResponseCode);

		//QUIT�R�}���h���M(SockClientSend)
		strcpy(szBuf,"QUIT\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		//�����m�F
		CheckServerResponse(&nSmtpResponseCode);
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
/*|	�T�v	�F�r�l�s�o�T�[�o�[���X�|���X�`�F�b�N									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CSmtp::CheckServerResponse
	(	int*	pnSmtpResponseCode
	)
{
	long	lRet=0;
	char	szBuf[1024];
	DWORD	dwRead=0;
	CTime	objStartTime = CTime::GetCurrentTime();
	CTimeSpan objDiff, objBase(0,0,0,10);

	//�\�P�b�g�c�k�k�`�F�b�N
	if(!DllCheck())
		return FALSE;

	(*pnSmtpResponseCode) = 0;
	for(;;)
	{
		m_pApp->MessagePump();
		Sleep(10);
		objDiff = CTime::GetCurrentTime() - objStartTime;
		if(objDiff > objBase)//timeout
			return FALSE;

		//��M(SockClientRecv)
		memset(szBuf,0,sizeof(szBuf));
		lRet = m_pApp->m_lpfnSckFunc8(m_lSockID,(LPVOID)szBuf,&dwRead);
		if(lRet == -2) //��M�ς݃f�[�^�Ȃ�
			continue;
		else if(lRet == -3)//�ڑ����؂ꂽ
			return FALSE;
		else
			break;
	}

	//SMTP�����R�[�h�擾
	szBuf[3] = NULL;
	(*pnSmtpResponseCode) = atoi(szBuf);

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F��`�r�b�h�h�R�[�h���݃`�F�b�N										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CSmtp::CheckExistNonAscii
	(	CString&	objStr
	)
{
	int	lPos=0;
	for(lPos=0; lPos<objStr.GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,objStr.GetAt(lPos)))//�Q�o�C�g�����̑��o�C�g
			return TRUE;
		else
		{
			if( isHanKana(objStr.GetAt(lPos)) ) //���p�J�^�J�i
				return TRUE;
			else
				lPos++;
		}
	}
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���b�Z�[�W�w�b�_�G���R�[�h											|*/
/*|	���	�F																		|*/
/*|	���l	�FRFC1522�ɏ]��															|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSmtp::MessageHeaderEncode
	(	CString*	pobjStr
	)
{
	CString				objBuf=_T("");
	CBase64				objBase64;
	CQuotedPrintable	objQuoted;
	CCodeConvManager	objCodeConvManager;

	if(pobjStr == NULL)
		return;
	if(pobjStr->GetLength() == 0)
		return;
	objBuf = (*pobjStr);
	pobjStr->Empty();

	//���p�J�^�J�i��S�p�֕ϊ�
	objCodeConvManager.StringKatakanaHan2Zen(&objBuf);

	//RFC1468(ISO-2022-JP)�G���R�[�h
	objCodeConvManager.StringSjisToJis(&objBuf);

	//MIME�G���R�[�h
	if(m_nEncodeType == SMTP_ENCODE_BASE64)
		objBase64.StringEncode(&objBuf);
	else if(m_nEncodeType == SMTP_ENCODE_QUOTED)
		objQuoted.StringEncode(&objBuf);
	else if(m_nEncodeType == SMTP_ENCODE_UUENCODE)
		objBase64.StringEncode(&objBuf);

	//�w�b�_�t��
	if(m_nEncodeType == SMTP_ENCODE_BASE64)
		pobjStr->Format("=?ISO-2022-JP?B?%s?=", objBuf);
	else if(m_nEncodeType == SMTP_ENCODE_QUOTED)
		pobjStr->Format("=?ISO-2022-JP?Q?%s?=", objBuf);
	else if(m_nEncodeType == SMTP_ENCODE_UUENCODE)
		pobjStr->Format("=?ISO-2022-JP?B?%s?=", objBuf);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�\�P�b�g�c�k�k�`�F�b�N												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CSmtp::DllCheck()
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


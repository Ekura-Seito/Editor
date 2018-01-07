// http.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "Editor.h"
#include "Line.h"
#include "HtmlOpenStatusDlg.h"
#include "http.h"
#include "Base64.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHttp �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CHttp::CHttp()
{
	CString objMsg;

	m_pApp = (CEditorApp*)AfxGetApp();
	m_lSockID = -1;
	memset(m_szRecvBuf,0,sizeof(m_szRecvBuf));
	m_dwRecvByte = 0;
	m_nRecvRetryCount = 20;
	m_nRecvTimeout = 1;
	m_nContentLength = 0;
	m_nRecvSum = 0;

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
			objMsg.LoadString(IDS_HTTP_DLLNOTFOUND);
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
CHttp::~CHttp()
{
	Disconnect();
}

/////////////////////////////////////////////////////////////////////////////
// CHttp �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�g�s�s�o�T�[�o�ڑ�													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CHttp::Connect
	(	CString&	objHost,				//�ڑ���z�X�g
		BOOL		bIsUseProxy				//�v���L�V�T�[�o�o�R���H
	)
{
	WORD	wPort=0;
	CString objMsg;

	//�p�����[�^�`�F�b�N
	if(!DllCheck())
	{
		objMsg.LoadString(IDS_HTTP_DLLNOTFOUND);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//�ڑ���|�[�g
	if(bIsUseProxy)
		wPort = 8080;
	else
		wPort = 80;

	//�\�P�b�g�ڑ�(SockClientOpen)�A��M�X���b�h���N�����Ȃ�
	m_lSockID = m_pApp->m_lpfnSckFunc6((char*)(LPCTSTR)objHost,wPort,FALSE);
	if(m_lSockID < 0)
	{
		objMsg.LoadString(IDS_HTTP_CONNECTERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	else
		return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�g�s�l�k�t�@�C���擾													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CHttp::Get
	(	CString&	objPath,					//�擾��p�X
		CArchive&	ar,							//HTML�t�@�C���ۑ���
		CHtmlOpenStatusDlg* pStatusDlg,			//�󋵕\���p�_�C�A���O
		BOOL&		bIsUseBasicAuth,			//Basic�F�؂��g�p���邩�H
		CString		objUserId,					//Basic�F�ؗp���[�U�h�c
		CString		objPasswd,					//Basic�F�ؗp�p�X���[�h
		CString&	objRealm					//Basic�F�ؗ̈�
	)
{
	long lRet=0,lRetFirst=0;
	DWORD	dwWrite=0;
	CString objBuf, objMsg, objAuth;
	char szBuf[2048];
	char *p=NULL;
	CLine 	objLine;
	CBase64	objBase64;
	BOOL bIsAuthorizationRequired=FALSE;

	if(pStatusDlg == NULL)
		return FALSE;

	//�擾�R�}���h�ҏW�����M
	if(bIsUseBasicAuth && !objUserId.IsEmpty() && !objPasswd.IsEmpty())
	{
		objAuth.Format("%s:%s",objUserId,objPasswd);
		objBase64.StringEncode(&objAuth);
	 	objBuf.Format("GET %s HTTP/1.0\r\nAuthorization: Basic %s\r\n\r\n", objPath, objAuth );
	}
	else
	 	objBuf.Format("GET %s HTTP/1.0\r\n\r\n", objPath);
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,(char*)(LPCTSTR)objBuf,objBuf.GetLength(),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != (DWORD)objBuf.GetLength())
	{
		objMsg.LoadString(IDS_HTTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	ClearRecvBuf();

	//�w�b�_���
	m_nContentLength = 0;
	m_objResponseHeader.Empty();
	for(;;)
	{
		memset(szBuf,0,sizeof(szBuf));
		lRet = RecvOneLine(szBuf);
		if(lRet < 0)
		{
			if(lRet == -2)
				objMsg.LoadString(IDS_HTTP_TIMEOUT);
			else if(lRet == -3)
				objMsg.LoadString(IDS_HTTP_DISCONNECTED);

			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//HTTP���X�|���X
		if(!strnicmp(szBuf,"HTTP/1.",strlen("HTTP/1.")))
		{
			p = strstr(szBuf,"401 ");
			if(p != NULL)
				bIsAuthorizationRequired = TRUE;
		}
		//Content-Length�w�b�_
		if(!strnicmp(szBuf,"Content-Length:",strlen("Content-Length:")))
			m_nContentLength = atoi( &szBuf[strlen("Content-Length:")] );
		//WWW-Authenticate�w�b�_
		if(!strnicmp(szBuf,"WWW-Authenticate:",strlen("WWW-Authenticate:")))
		{
			strupr(szBuf);
			p = strstr(szBuf,"BASIC REALM=");
			if(p != NULL)
				objRealm = p+strlen("BASIC REALM=");
		}

		//���X�|���X�w�b�_�ێ�
		m_objResponseHeader += szBuf;
		m_objResponseHeader += "\r\n";

		if(!strlen(szBuf)) //��s�I
			break;
	}

	//Basic�F�؂��K�v
	if(bIsAuthorizationRequired)
	{
		bIsUseBasicAuth = TRUE;
		//�{�f�B����ǂݎ̂Ă�
		for(;;)
		{
			lRet = RecvToLocal();
			if(lRet < 0)
				break;
			m_pApp->MessagePump();
		}
		//�ؒf
		Disconnect();
		return FALSE;
	}

	//�_�E�����[�h����o�C�g������\��
	m_nRecvSum = 0;
	if(m_nContentLength != 0 && pStatusDlg != NULL)
	{
		pStatusDlg->SetRange(0,m_nContentLength);
		pStatusDlg->SetPos(0);
	}

	//�{�f�B����ۑ�
	ar.Write(m_szRecvBuf,m_dwRecvByte);
	m_nRecvSum += m_dwRecvByte;
	if(m_nContentLength != 0 && pStatusDlg != NULL)
		pStatusDlg->SetPos(m_nRecvSum);
	m_pApp->MessagePump();

	for(;;)
	{
		lRet = RecvToLocal();
		if(lRet < 0)
			break;
		ar.Write(m_szRecvBuf,m_dwRecvByte);

		m_nRecvSum += m_dwRecvByte;
		if(m_nContentLength != 0 && pStatusDlg != NULL)
			pStatusDlg->SetPos(m_nRecvSum);
		m_pApp->MessagePump();
	}

	//�ؒf
	Disconnect();
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�T�[�o�[����̐ؒf													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CHttp::Disconnect()
{
	//�\�P�b�g�c�k�k�`�F�b�N
	if(!DllCheck())
		return;

	if(m_lSockID != -1)
	{
		//�\�P�b�g�N���[�Y(SockClientClose)
		m_pApp->m_lpfnSckFunc9(m_lSockID);
	}
	m_lSockID = -1;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���X�|���X�w�b�_�S�̎擾												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CHttp::GetResponseHeader(CString& objBuf)
{
	objBuf = m_objResponseHeader;
}

/////////////////////////////////////////////////////////////////////////////
// CHttp �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�\�P�b�g�c�k�k�`�F�b�N												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CHttp::DllCheck()
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
/*|	�T�v	�F�P�s��M�o�b�t�@�N���A												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CHttp::ClearRecvBuf()
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
LONG CHttp::RecvOneLine				//  1 : ����I��
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
LONG CHttp::RecvToLocal()				//  0 : ����I��
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
		lRet = m_pApp->m_lpfnSckFunc10( m_lSockID,			//�\�P�b�g�h�c
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


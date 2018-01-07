// Smtp.cpp : インプリメンテーション ファイル
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
// CSmtp クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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

	//ソケットＤＬＬの明示的リンク
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

	//ソケットＤＬＬ関数アドレス取得
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
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
// CSmtp パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：件名設定																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSmtp::SetSubject
	(	CString	objStr
	)
{
	if(objStr.IsEmpty())
		return;

	if(CheckExistNonAscii(objStr)) //非ASCIIコード存在チェック
	{
		//ヘッダエンコード
		MessageHeaderEncode(&objStr);
	}
	m_objSubject = objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：差出人設定															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：受取人設定															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：カーボンコピー受取人設定												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：ブラインドカーボンコピー受取人設定									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：返信先設定															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：レファレンス設定														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：添付ファイル設定														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：エンコードタイプ設定													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：署名設定																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：ＳＭＴＰサーバーへの接続												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CSmtp::Connect
	(	CString	objDestIPAddress //サーバーＩＰアドレス
	)
{
	BOOL	bRet=FALSE;
	int		nSmtpResponseCode=0;
	LONG	lRet=0;
	DWORD	dwRead=0,dwWrite=0;
	char	szBuf[1024];
	char	szHostName[64];		// ホスト名返却用バッファ
	char	szIPAddress[64];	// ＩＰアドレス返却用バッファ
	CString objMsg;

	//ソケットＤＬＬチェック
	if(!DllCheck())
		return FALSE;

	//パラメータ保持
	strcpy(m_szDestIPAddress,(LPCTSTR)objDestIPAddress);

	//クライアントソケットオープン(SockClientOpen)
	m_lSockID = m_pApp->m_lpfnSckFunc6(m_szDestIPAddress,m_wDestPort,TRUE);
	if(m_lSockID < 0)
	{
		objMsg.LoadString(IDS_SMTP_CONNECTERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//Greeting Messageの受信確認
	bRet = CheckServerResponse(&nSmtpResponseCode);
	if(bRet == FALSE || nSmtpResponseCode != 220)
	{
		CancelDisconnect();
		DispSmtpMessage(nSmtpResponseCode,_T(""));
		objMsg.LoadString(IDS_SMTP_CONNECTERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//ホスト名取得(GetLocalHostInfo)
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

	//HELLOコマンド送信(SockClientSend)
	sprintf(szBuf,"HELO %s\r\n", szHostName);
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		CancelDisconnect();
		objMsg.LoadString(IDS_SMTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//HELLOコマンドの受信確認
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
/*|	概要	：ＳＭＴＰサーバーからの切断											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSmtp::Disconnect()
{
	long	lRet=0;
	char	szBuf[1024];
	DWORD	dwWrite=0;
	int		nSmtpResponseCode=0;

	//ソケットＤＬＬチェック
	if(!DllCheck())
		return;

	if(m_bIsConnected)
	{
		//QUITコマンド送信(SockClientSend)
		strcpy(szBuf,"QUIT\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		//応答確認
		CheckServerResponse(&nSmtpResponseCode);
	}
	m_bIsConnected = FALSE;

	if(m_lSockID != -1)
	{
		//ソケットクローズ(SockClientClose)
		m_pApp->m_lpfnSckFunc9(m_lSockID);
	}
	m_lSockID = -1;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：メール送信															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CSmtp::SendMail
	(	char*	pszMailFile		//メール内容ファイル
	)
{
	BOOL	bRet=FALSE;
	long	lRet=0, lCnt=0;
	char	szBuf[1024];
	DWORD	dwWrite=0;
	int		nSmtpResponseCode=0;
	CString objMsg;

	//ソケットＤＬＬチェック
	if(!DllCheck())
		return FALSE;

	//事前チェック
	if(m_bIsConnected == FALSE)
		return FALSE;
	if(m_objFrom.IsEmpty() || m_objTo.GetSize() == 0)
		return FALSE;

	//MAILコマンド送信(SockClientSend)
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

	//RCPTコマンド送信(SockClientSend)
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

	//RCPTコマンド送信(SockClientSend)
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

	//RCPTコマンド送信(SockClientSend)
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

	//DATAコマンド送信(SockClientSend)
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

	//メールヘッダ送信
	if(SendMailHeader() == FALSE)
		return FALSE;

	//メール本文送信
	if(SendMailBody(pszMailFile) == FALSE)
		return FALSE;

	//添付ファイル送信
	if(m_objAttachFiles.GetSize() > 0)
	{
		if(SendAttachedFiles() == FALSE)
			return FALSE;
	}

	//メール終了通知
	if(SendMailEnd() == FALSE)
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSmtp プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：メールヘッダ送信														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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

	//ソケットＤＬＬチェック
	if(!DllCheck())
		return FALSE;

	//事前チェック
	if(m_bIsConnected == FALSE)
		return FALSE;
	if(m_objFrom.IsEmpty() || m_objTo.GetSize() == 0)
		return FALSE;

	//From ヘッダー(SockClientSend)
	sprintf(szBuf,"From: <%s>\r\n", (LPCTSTR)m_objFrom);
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		CancelDisconnect();
		objMsg.LoadString(IDS_SMTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//To ヘッダー(SockClientSend)
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

	//Cc ヘッダー(SockClientSend)
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

	//Bcc ヘッダー(SockClientSend)
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

	//Date ヘッダー(SockClientSend)
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

	//Subject ヘッダー(SockClientSend)
	sprintf(szBuf,"Subject: %s\r\n", (LPCTSTR)m_objSubject);
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		CancelDisconnect();
		objMsg.LoadString(IDS_SMTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//Message-ID ヘッダー(SockClientSend)
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

	//Disposition-Notification-Toヘッダー(SockClientSend)
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

	//Reply-To ヘッダー(SockClientSend)
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

	//Reference ヘッダー(SockClientSend)
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

	//X-Mailer ヘッダー(SockClientSend)
	sprintf(szBuf,"X-Mailer: %s\r\n", (LPCTSTR)m_objMailerName);
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		CancelDisconnect();
		objMsg.LoadString(IDS_SMTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	if(m_objAttachFiles.GetSize() > 0) //添付ファイルあり
	{
		//MIME-Version ヘッダー(SockClientSend)
		strcpy(szBuf,"MIME-Version: 1.0\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//Content-type ヘッダー(SockClientSend)
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
	else //添付ファイルなし
	{
		//Content-type ヘッダー(SockClientSend)
		strcpy(szBuf,"Content-Type: text/plain; charset=\"iso-2022-jp\"\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//Content-Transfer-Encoding ヘッダー(SockClientSend)
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

	//ヘッダー終了(SockClientSend)
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
/*|	概要	：メール本文送信														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CSmtp::SendMailBody
	(	char*		pszMailFile			//メール内容ファイル
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

	//ソケットＤＬＬチェック
	if(!DllCheck())
		return FALSE;

	//事前チェック
	if(m_bIsConnected == FALSE)
		return FALSE;
	if(pszMailFile == NULL)
		return FALSE;

	if(m_objAttachFiles.GetSize() > 0) //添付ファイルあり
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

		//ブロック境界(SockClientSend)
		sprintf(szBuf,"--%s\r\n", (LPCTSTR)m_objBoundary);
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//Content-type ヘッダー(SockClientSend)
		strcpy(szBuf,"Content-Type: text/plain; charset=\"iso-2022-jp\"\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//Content-Transfer-Encoding ヘッダー(SockClientSend)
		strcpy(szBuf,"Content-Transfer-Encoding: 7bit\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//サブヘッダーの終了(SockClientSend)
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

	//内容送信
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

			if(szBuf[0] == '.') //行頭の '.'を、'..'にする
			{
				objBuf = ".";
				objBuf += szBuf;
			}
			else
				objBuf = szBuf;

			//半角カタカナを全角へ変換
			objCodeConvManager.StringKatakanaHan2Zen(&objBuf);

			//RFC1468(ISO-2022-JP)エンコード
			objCodeConvManager.StringSjisToJis(&objBuf);

			//１行送信(SockClientSend)
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

	if(!m_objSignature.IsEmpty()) //署名あり
	{
		//本文と署名の間の１行(SockClientSend)
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

		//半角カタカナを全角へ変換
		objCodeConvManager.StringKatakanaHan2Zen(&objBuf);

		//RFC1468(ISO-2022-JP)エンコード
		objCodeConvManager.StringSjisToJis(&objBuf);

		//１行送信(SockClientSend)
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
/*|	概要	：添付ファイル送信														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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

	//ソケットＤＬＬチェック
	if(!DllCheck())
		return FALSE;

	if(m_nEncodeType == SMTP_ENCODE_BASE64)
		nReadLength = 54;
	else if(m_nEncodeType == SMTP_ENCODE_QUOTED)
		nReadLength = 22;
	else if(m_nEncodeType == SMTP_ENCODE_UUENCODE)
		nReadLength = 45;

	//事前チェック
	if(m_bIsConnected == FALSE)
		return FALSE;
	if(m_objAttachFiles.GetSize() == 0)
		return TRUE;

	for(lCnt=0; lCnt<m_objAttachFiles.GetSize(); lCnt++)
	{
		//添付ファイル名取得
		strcpy(szBuf,(LPCTSTR)m_objAttachFiles.GetAt(lCnt));
		_tsplitpath( szBuf, NULL, NULL, szFName, szExt );

		//添付ファイル名エンコード
		objAttachFileName.Format("%s%s", szFName, szExt );
		if(CheckExistNonAscii(objAttachFileName))
			MessageHeaderEncode(&objAttachFileName);

		//添付ファイルオープン
		if( !objFile.Open( szBuf, CFile::modeRead|CFile::typeBinary ) )
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_ATTACHFILEOPENERR);
			sprintf(szMsg,(LPCTSTR)objMsg,szBuf);
			m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//ブロック境界(SockClientSend)
		sprintf(szBuf,"\r\n--%s\r\n", (LPCTSTR)m_objBoundary);
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//Content-Type ヘッダー(SockClientSend)
		sprintf(szBuf,"Content-Type: application/octet-stream; file=%s\r\n", (LPCTSTR)objAttachFileName);
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//Content-Transfer-Encoding ヘッダー(SockClientSend)
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

		//Content-Disposition ヘッダー(SockClientSend)
		sprintf(szBuf,"Content-Disposition: attachment; filename=%s\r\n", (LPCTSTR)objAttachFileName);
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//サブヘッダーの終了(SockClientSend)
		strcpy(szBuf,"\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
		{
			CancelDisconnect();
			objMsg.LoadString(IDS_SMTP_SENDERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return FALSE;
		}

		//uuencodeの場合のみbegin行
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

		//添付ファイル内容送信
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

				//エンコード
				if(m_nEncodeType == SMTP_ENCODE_BASE64)
					objBuf = objBase64.BytesEncode(byBuf,nBytesRead);
				else if(m_nEncodeType == SMTP_ENCODE_UUENCODE)
					objBuf = objUuencode.BytesEncode(byBuf,nBytesRead);

				//送信(SockClientSend)
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
					//前回読んだ分を送信(SockClientSend)
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

				//エンコード
				objBuf.Empty();
				objBuf = objQuoted.BytesEncode(byBuf,nBytesRead);
			}
		}

		//添付ファイルクローズ
		objFile.Close();

		//uuencodeの場合のみend行
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

	//ブロック境界終了(SockClientSend)
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
/*|	概要	：メール終了通知														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CSmtp::SendMailEnd()
{
	long	lRet=0;
	char	szBuf[1024];
	DWORD	dwWrite=0;
	int		nSmtpResponseCode=0;
	CString objMsg;

	//ソケットＤＬＬチェック
	if(!DllCheck())
		return FALSE;

	//事前チェック
	if(m_bIsConnected == FALSE)
		return FALSE;

	//本文終了(SockClientSend)
	strcpy(szBuf,"\r\n.\r\n");
	lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
	if(lRet == -2 || lRet == -3 || dwWrite != strlen(szBuf))
	{
		CancelDisconnect();
		objMsg.LoadString(IDS_SMTP_SENDERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	//応答確認
	CheckServerResponse(&nSmtpResponseCode);

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＳＭＴＰ応答コードメッセージ表示										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：タイムゾーン文字列取得												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：ＳＭＴＰサーバーからのキャンセル切断									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSmtp::CancelDisconnect()
{
	long	lRet=0;
	char	szBuf[1024];
	DWORD	dwWrite=0;
	int		nSmtpResponseCode=0;

	//ソケットＤＬＬチェック
	if(!DllCheck())
		return;

	if(m_bIsConnected)
	{
		//RSETコマンド送信(SockClientSend)
		strcpy(szBuf,"RSET\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		//応答確認
		CheckServerResponse(&nSmtpResponseCode);

		//QUITコマンド送信(SockClientSend)
		strcpy(szBuf,"QUIT\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		//応答確認
		CheckServerResponse(&nSmtpResponseCode);
	}
	m_bIsConnected = FALSE;

	if(m_lSockID != -1)
	{
		//ソケットクローズ(SockClientClose)
		m_pApp->m_lpfnSckFunc9(m_lSockID);
	}
	m_lSockID = -1;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＳＭＴＰサーバーレスポンスチェック									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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

	//ソケットＤＬＬチェック
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

		//受信(SockClientRecv)
		memset(szBuf,0,sizeof(szBuf));
		lRet = m_pApp->m_lpfnSckFunc8(m_lSockID,(LPVOID)szBuf,&dwRead);
		if(lRet == -2) //受信済みデータなし
			continue;
		else if(lRet == -3)//接続が切れた
			return FALSE;
		else
			break;
	}

	//SMTP応答コード取得
	szBuf[3] = NULL;
	(*pnSmtpResponseCode) = atoi(szBuf);

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：非ＡＳＣＩＩコード存在チェック										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CSmtp::CheckExistNonAscii
	(	CString&	objStr
	)
{
	int	lPos=0;
	for(lPos=0; lPos<objStr.GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,objStr.GetAt(lPos)))//２バイト文字の第一バイト
			return TRUE;
		else
		{
			if( isHanKana(objStr.GetAt(lPos)) ) //半角カタカナ
				return TRUE;
			else
				lPos++;
		}
	}
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：メッセージヘッダエンコード											|*/
/*|	解説	：																		|*/
/*|	備考	：RFC1522に従う															|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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

	//半角カタカナを全角へ変換
	objCodeConvManager.StringKatakanaHan2Zen(&objBuf);

	//RFC1468(ISO-2022-JP)エンコード
	objCodeConvManager.StringSjisToJis(&objBuf);

	//MIMEエンコード
	if(m_nEncodeType == SMTP_ENCODE_BASE64)
		objBase64.StringEncode(&objBuf);
	else if(m_nEncodeType == SMTP_ENCODE_QUOTED)
		objQuoted.StringEncode(&objBuf);
	else if(m_nEncodeType == SMTP_ENCODE_UUENCODE)
		objBase64.StringEncode(&objBuf);

	//ヘッダ付加
	if(m_nEncodeType == SMTP_ENCODE_BASE64)
		pobjStr->Format("=?ISO-2022-JP?B?%s?=", objBuf);
	else if(m_nEncodeType == SMTP_ENCODE_QUOTED)
		pobjStr->Format("=?ISO-2022-JP?Q?%s?=", objBuf);
	else if(m_nEncodeType == SMTP_ENCODE_UUENCODE)
		pobjStr->Format("=?ISO-2022-JP?B?%s?=", objBuf);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ソケットＤＬＬチェック												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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


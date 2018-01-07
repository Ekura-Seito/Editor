// Pop.cpp : インプリメンテーション ファイル
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

//応答チェック種別
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
// CPop クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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

	//ソケットＤＬＬの明示的リンク
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
			objMsg.LoadString(IDS_POP_DLLNOTFOUND);
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
CPop::~CPop()
{
	Disconnect();
}

/////////////////////////////////////////////////////////////////////////////
// CPop パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：受信パラメータ設定													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：ＰＯＰサーバーへの接続												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CPop::Connect
	(	CString&	objDestIPAddress,		//サーバーＩＰアドレス
		CString& 	objUser,				//ユーザ名
		CString&	objPassword,			//パスワード
		int			nAuthorizationType		//ユーザ認証方式（０：USER,PASS   １：APOP）
	)
{
	BOOL	bRet=FALSE;
	LONG	lRet=0;
	DWORD	dwRead=0,dwWrite=0;
	char	szBuf[1024],szMsg[1024];
	char	szDigestBase[512],szMD5[64];
	CString objMsg;

	//ソケットＤＬＬチェック
	if(!DllCheck())
		return FALSE;

	//タイムスタンプ保持領域初期化
	memset(m_szTimeStamp,0,sizeof(m_szTimeStamp));

	//パラメータ保持
	strcpy(m_szDestIPAddress,(LPCTSTR)objDestIPAddress);

	//ＰＯＰサーバソケット接続(SockClientOpen)、受信スレッドを起動しない
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

	if(nAuthorizationType == POP3_AUTHENTICATE_STANDARD) //USER, PASSによる認証
	{
		//USERコマンド送信(SockClientSend)
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

		//PASSコマンド送信(SockClientSend)
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
	else if(nAuthorizationType == POP3_AUTHENTICATE_APOP) //APOPによる認証
	{
		//APOPコマンドパラメータのDigest用文字列作成（例：<1896.697170952@dbc.mtview.ca.us>tanstaaf）
		memset(szDigestBase,0,sizeof(szDigestBase));
		sprintf(szDigestBase,"%s%s",m_szTimeStamp,(LPCTSTR)objPassword);

		//ＭＤ５取得（例：c4c9334bac560ecc979e58001b3e22fb）
		CMd5	objMd5;
		memset(szMD5,0,sizeof(szMD5));
		objMd5.MD5String(szDigestBase,szMD5);

		//APOPコマンド送信(SockClientSend)
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
/*|	概要	：ＰＯＰサーバーからの切断												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CPop::Disconnect()
{
	long	lRet=0;
	char	szBuf[1024];
	DWORD	dwWrite=0;

	//ソケットＤＬＬチェック
	if(!DllCheck())
		return;

	if(m_bIsConnected)
	{
		//QUITコマンド送信(SockClientSend)
		strcpy(szBuf,"QUIT\r\n");
		lRet = m_pApp->m_lpfnSckFunc7(m_lSockID,szBuf,strlen(szBuf),&dwWrite);
		//応答確認
		ClearRecvBuf();
		CheckServerResponse(QUIT_CHECK,szBuf);
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
/*|	概要	：メール数取得															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CPop::GetMailCount()
{
	BOOL	bRet=FALSE;
	long	lRet=0;
	char	szBuf[1024],szMsg[1024];
	DWORD	dwWrite=0;
	CString objMsg;

	//ソケットＤＬＬチェック
	if(!DllCheck())
		return -1;
	//事前チェック
	if(m_bIsConnected == FALSE)
		return -1;

	//STATコマンド送信(SockClientSend)
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
/*|	概要	：１メールヘッダ情報取得												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CPop::GetMailInfo
	(	int		nMailIndex,				//メール番号（１～）
		char*	szSubject,				//件名返却用バッファ
		char*	szFrom,					//差出人返却用バッファ
		char*	szDate,					//日付返却用バッファ
		BOOL*	pbIsMultiPart			//添付ファイルありか？返却用バッファ
	)
{
	BOOL	bRet=FALSE;
	long	lRet=0;
	DWORD	dwWrite=0;
	char	szBuf[1024],szContentType[1024];
	CString objBuf, objMsg;

	//ソケットＤＬＬチェック
	if(!DllCheck())
		return FALSE;

	//事前チェック
	if(m_bIsConnected == FALSE)
		return FALSE;
	if(nMailIndex<1 || nMailIndex>m_nMailCount)
		return FALSE;

	//TOPコマンド送信(SockClientSend)
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
//DELEコマンドで削除後にTOPコマンドを発行した場合、
//DELEしたメールについては、-ERRが帰ってくる可能性があるので
//本メッセージボックスは表示しない
//		objMsg.LoadString(IDS_POP_TOPERR);
//		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//ヘッダーを１行ずつ、「\r\n」まで受信する
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
			for(;;) //複数行に分かれる場合があるため、次の行もチェック
			{
				memset(szBuf,0,sizeof(szBuf));
				lRet = RecvOneLine(szBuf);
				if(lRet < 0)
					return FALSE;
				if(szBuf[0]=='.' && szBuf[1]!='.')
					return TRUE;
				if(!strlen(szBuf))
					return TRUE;
				if(strchr(szBuf,':') != NULL) //次のヘッダ出現
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
			for(;;) //複数行に分かれる場合があるため、次の行もチェック
			{
				memset(szBuf,0,sizeof(szBuf));
				lRet = RecvOneLine(szBuf);
				if(lRet < 0)
					return FALSE;
				if(szBuf[0]=='.' && szBuf[1]!='.')
					return TRUE;
				if(!strlen(szBuf))
					return TRUE;
				if(strchr(szBuf,':') != NULL) //次のヘッダ出現
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
			for(;;) //複数行に分かれる場合があるため、次の行もチェック
			{
				memset(szBuf,0,sizeof(szBuf));
				lRet = RecvOneLine(szBuf);
				if(lRet < 0)
					return FALSE;
				if(szBuf[0]=='.' && szBuf[1]!='.')
					return TRUE;
				if(!strlen(szBuf))
					return TRUE;
				if(strchr(szBuf,':') != NULL) //次のヘッダ出現
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
			for(;;) //複数行に分かれる場合があるため、次の行もチェック
			{
				memset(szBuf,0,sizeof(szBuf));
				lRet = RecvOneLine(szBuf);
				if(lRet < 0)
					return FALSE;
				if(szBuf[0]=='.' && szBuf[1]!='.')
					return TRUE;
				if(!strlen(szBuf))
					return TRUE;
				if(strchr(szBuf,':') != NULL) //次のヘッダ出現
					break;
				else
				{
					MailHeaderDecode(szBuf);
					strcat(szContentType,szBuf);
				}

				//Content-Typeヘッダ解析
				objBuf = szContentType;
				objBuf.MakeLower();
				if( objBuf.Find("multipart/mixed") != -1 )
					(*pbIsMultiPart) = TRUE;
				else
					(*pbIsMultiPart) = FALSE;
			}

			//Content-Typeヘッダ解析
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
/*|	概要	：１メールサイズ情報取得												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CPop::GetMailSize
	(	int		nMailIndex,				//メール番号（１～）
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

	//ソケットＤＬＬチェック
	if(!DllCheck())
		return FALSE;

	//事前チェック
	if(m_bIsConnected == FALSE)
		return FALSE;
	if(nMailIndex<1 || nMailIndex>m_nMailCount)
		return FALSE;

	//LISTコマンド送信(SockClientSend)
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
//DELEコマンドで削除後にLISTコマンドを発行した場合、
//DELEしたメールについては、-ERRが帰ってくる可能性があるので
//本メッセージボックスは表示しない
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
/*|	概要	：１メール受信															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CPop::RecvOneMail
	(	int		nMailIndex,				//受信メール番号（１～）
		char*	szMailSaveFilePath,		//受信メール内容保存先ファイルパス
		char*	szAttachPath			//添付ファイル保存先パス
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
	BOOL	bIsMultiPartMail=FALSE;			// マルチパートメールか？
	char	szBoundary[1024];				// 添付ファイルありの場合の境界

	//ソケットＤＬＬチェック
	if(!DllCheck())
		return FALSE;

	//事前チェック
	if(m_bIsConnected == FALSE)
		return FALSE;
	if(nMailIndex<1 || nMailIndex>m_nMailCount)
		return FALSE;

	//受信メール内容保存先ファイルオープン
	fp=fopen(szMailSaveFilePath,"w");
	if(fp==NULL)
		return FALSE;

	//RETRコマンド送信(SockClientSend)
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
	//メールヘッダ受信「\r\n」まで//
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
			for(;;) //複数行に分かれる場合があるため、次の行もチェック
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
				if(strchr(szBuf,':') != NULL) //次のヘッダ出現
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

	//Content-Typeヘッダ解析
	objBuf = objContentType;
	objBuf.MakeLower();
	if( objBuf.Find("multipart/mixed") != -1 &&
		(nPos = objBuf.Find("boundary=")) != -1
	)
	{
		bIsMultiPartMail = TRUE;
		memset(szBoundary,0,sizeof(szBoundary));
		//バウンダリ取得
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
		//添付ファイル保存先ディレクトリ作成
		if( _access( szAttachPath, 0 ) == -1 )
			::CreateDirectory(szAttachPath,NULL);
	}
	else
	{
		bIsMultiPartMail = FALSE;
		memset(szBoundary,0,sizeof(szBoundary));
	}

	//////////////////////////////////////////////
	//マルチパートでばない場合のメールボディ受信//
	//////////////////////////////////////////////
	if(!bIsMultiPartMail) //マルチパートではない場合
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
			if(szBuf[0]=='.' && szBuf[1]!='.')//メール終了のしるし
			{
				fprintf(fp,"%s\n",szBuf);
				break;
			}
			if(szBuf[0]=='.' && szBuf[1]=='.')//'.'が２回続いている
			{
				//'.'を１つにする
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
	//マルチパートの場合のメールボディ受信//
	////////////////////////////////////////
	nEncodeType = -1;
	objAttachFileName.Empty();
	memset(szBuf,0,sizeof(szBuf));
	for(;;)
	{
		//１行受信
		if(!strlen(szBuf))
		{
			memset(szBuf,0,sizeof(szBuf));
			lRet = RecvOneLine(szBuf);
			if(lRet < 0)
			{
				fclose(fp);
				return FALSE;
			}
			if(szBuf[0]=='.' && szBuf[1]!='.')//メール終了のしるし
			{
				fprintf(fp,"%s\n",szBuf);
				break;
			}
			if(szBuf[0]=='.' && szBuf[1]=='.')//'.'が２回続いている
			{
				//'.'を１つにする
				memset(szTemp,0,sizeof(szTemp));
				strcpy(szTemp,&szBuf[1]);
				strcpy(szBuf,szTemp);
			}
		}

		if(!strnicmp(szBuf,szBoundary,strlen(szBoundary)-2)) //バウンダリ発見
		{
			fprintf(fp,"%s\n",szBuf);
			if(!strnicmp(szBuf,szBoundary,strlen(szBoundary))) //全パート終了
				break;

			//サブヘッダー解析
			objContentType.Empty();
			objContentTransferEncoding.Empty();
			objContentDisposition.Empty();
			lRet = AnalyzeSubHeader(fp,objContentType,objContentTransferEncoding,objContentDisposition);
			if(lRet <= 0)
			{
				fclose(fp);
				return FALSE;
			}

			//エンコードタイプ判定
			objBuf = objContentTransferEncoding;
			objBuf.MakeLower();
			nEncodeType = -1;
			if( objBuf.Find("base64") != -1 )
				nEncodeType = SMTP_ENCODE_BASE64;
			else if( objBuf.Find("quoted-printable") != -1 )
				nEncodeType = SMTP_ENCODE_QUOTED;
			else if( objBuf.Find("x-uuencode") != -1 )
				nEncodeType = SMTP_ENCODE_UUENCODE;

			//添付ファイル名取得
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
				//デコード結果ファイルをプライベートヘッダとして出力（サブヘッダ終了付き）
				fprintf(fp,"X-Xerxes-Attachment-File: \"%s\\%s\"\n\n",szAttachPath,(LPCTSTR)objAttachFileName);
				//添付ファイルデコード
				if(nEncodeType == SMTP_ENCODE_BASE64 || nEncodeType == SMTP_ENCODE_UUENCODE)
					lRet = DecodeAttachFile(szAttachPath,objAttachFileName,nEncodeType);
				else if(nEncodeType == SMTP_ENCODE_QUOTED)
					lRet = DecodeQuotedAttachFile(szAttachPath,objAttachFileName);
				memset(szBuf,0,sizeof(szBuf));
			}
			else
			{
				//サブヘッダー終了行出力
				fprintf(fp,"\n");
				memset(szBuf,0,sizeof(szBuf));
			}
		}
		else
		{
			//そのまま出力
			MailBodyDecode(szBuf);
			fprintf(fp,"%s\n",szBuf);
			memset(szBuf,0,sizeof(szBuf));
		}
	}

	fclose(fp);
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１メール削除															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CPop::DelOneMail
	(	int		nMailIndex				//受信メール番号（１～）
	)
{
	BOOL	bRet=FALSE;
	long	lRet=0;
	char	szBuf[1024],szMsg[1024];
	DWORD	dwWrite=0;
	CString objMsg;

	//ソケットＤＬＬチェック
	if(!DllCheck())
		return FALSE;

	//事前チェック
	if(m_bIsConnected == FALSE)
		return FALSE;
	if(nMailIndex<1 || nMailIndex>m_nMailCount)
		return FALSE;

	//DELEコマンド送信(SockClientSend)
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
// CPop プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ソケットＤＬＬチェック												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：ＰＯＰサーバーレスポンスチェック										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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

	if(nCheckType == CONNECTION_CHECK)//サーバ接続チェック
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
				//banner greetingから<>でくくられたタイムスタンプを抽出する
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
	else if(nCheckType == USER_CHECK		||	//USERコマンド応答チェック
			nCheckType == QUIT_CHECK		||	//QUITコマンド応答チェック
			nCheckType == DELETE_CHECK		||	//DELEコマンド応答チェック
			nCheckType == RSET_CHECK		||	//RSETコマンド応答チェック
			nCheckType == NOOP_CHECK			//NOOPコマンド応答チェック
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
	else if(nCheckType == STAT_CHECK) //STATコマンド応答チェック
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
						if(bEmailNumber)//メール総数取得
						{
							m_nMailCount = atoi(p); 
							bEmailNumber = FALSE; 
						}
						else //総バイト数取得
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
	else if(nCheckType == LIST_CHECK) //LISTコマンド応答チェック
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
						if(bEmailNumber)//メール番号取得
						{
							m_nMailIndex = atoi(p); 
							bEmailNumber = FALSE; 
						}
						else //メールサイズ取得
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
		//+OK .....メッセージの次に、「ヘッダー\r\n」が「.\r\n」まで続く
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
		//+OK .....メッセージの次に、「ヘッダー＋ボディ」が「.\r\n」まで続く
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
	else if( nCheckType == PASSWORD_CHECK || //PASSコマンド応答チェック
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
/*|	概要	：１行受信バッファクリア												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CPop::ClearRecvBuf()
{
	m_dwRecvByte = 0;
	memset(m_szRecvBuf,0,sizeof(m_szRecvBuf));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１行受信																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LONG CPop::RecvOneLine				//  1 : 正常終了
									// -2 : 受信済みデータ無し
									// -3 : 接続が切れた
	(	char*	szBuf
	)
{
	char	*pr=NULL,*pn=NULL;		//m_szRecvBuf中の'\r','\n'の出現位置
	int		nPosR=-1,nPosN=-1;		//m_szRecvBuf中の'\r','\n'の出現位置オフセット
	int		nLen=0;
	int		nRetLen=0;				//szBufへ返却したバイト数
	LONG	lRet=0;

	for(;;)
	{
		//'\r', '\n'検索
		nPosR = nPosN = -1;
		pr = (char*)memchr(m_szRecvBuf,'\r',m_dwRecvByte);
		pn = (char*)memchr(m_szRecvBuf,'\n',m_dwRecvByte);
		if(pr != NULL)
			nPosR = pr - m_szRecvBuf;
		if(pn != NULL)
			nPosN = pn - m_szRecvBuf; 

		if(pr != NULL && (pn==NULL||nPosR<nPosN) ) 
		{
			//\rの前までを返却
			nLen = pr - m_szRecvBuf;
			memcpy(&szBuf[nRetLen],m_szRecvBuf,nLen);
			nRetLen += nLen;
			if(m_szRecvBuf[nLen+1] == '\n')
			{
				//"\n"を読み捨てして前に詰める
				memmove(&m_szRecvBuf[0],&m_szRecvBuf[nLen+2],m_dwRecvByte-nLen-2);
				m_dwRecvByte -= (nLen+2);
			}
			else
			{
				//前に詰める
				memmove(&m_szRecvBuf[0],&m_szRecvBuf[nLen+1],m_dwRecvByte-nLen-1);
				m_dwRecvByte -= (nLen+1);
			}
			return TRUE;
		}
		else if(pn != NULL && (pr==NULL||nPosN<nPosR))
		{
			//\nの前までを返却
			nLen = pn - m_szRecvBuf;
			memcpy(&szBuf[nRetLen],m_szRecvBuf,nLen);
			nRetLen += nLen;
			//前に詰める
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

			//ソケットから読む
			lRet = RecvToLocal();
			if(lRet < 0)
				return lRet;
		}
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：m_szRecvBufへ一括読み込み												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LONG CPop::RecvToLocal()				//  0 : 正常終了
										// -2 : リトライアウト
										// -3 : 接続が切れた
{
	LONG	lRet=0;
	int		nRetryCnt=0;

	for(;;)
	{
		//ソケットから読む
		m_dwRecvByte = 0;
		memset(m_szRecvBuf,0,sizeof(m_szRecvBuf));
		lRet = m_pApp->m_lpfnSckFunc10( m_lSockID,	//ソケットＩＤ
								(LPVOID)m_szRecvBuf,//バッファ
								2048-4,				//受信バイト数指定
								m_nRecvTimeout,		//タイムアウト（ｓ）
								&m_dwRecvByte);		//受信できたバイト数
		if(lRet == -2) //受信済みデータなし
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
/*|	概要	：メールヘッダデコード													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CPop::MailHeaderDecode
	(	char*	szIn			//メールヘッダ１行
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

	//メールヘッダのＭＩＭＥデコード(Base64,Quoted-Printable)
	memset(szBuf,0,sizeof(szBuf));
	for(nFrom=0,nTo=0; nFrom<(int)strlen(szIn); )
	{
		if(strnicmp(&szIn[nFrom],"=?",2)) //メールヘッダエンコードの先頭でない
		{
			//デコード後用バッファへそのままコピー
			szBuf[nTo] = szIn[nFrom];
			nTo++;
			nFrom++;
			continue;
		}
		if(strnicmp(&szIn[nFrom],"=?ISO-2022-JP",strlen("=?ISO-2022-JP"))) //ISO-2022-JPでない
		{
			//"=?"をデコード後用バッファへそのままコピー
			szBuf[nTo] = szIn[nFrom];
			nTo++;	nFrom++;
			szBuf[nTo] = szIn[nFrom];
			nTo++;	nFrom++;
			continue;
		}

		//エンコードタイプ判定
		if(!strnicmp(&szIn[nFrom],"=?ISO-2022-JP?B?",strlen("=?ISO-2022-JP?B?")))
			nEncodeType = SMTP_ENCODE_BASE64;
		else if(!strnicmp(&szIn[nFrom],"=?ISO-2022-JP?Q?",strlen("=?ISO-2022-JP?Q?")))
			nEncodeType = SMTP_ENCODE_QUOTED;
		else
		{
			//"=?ISO-2022-JP"をデコード後用バッファへそのままコピー
			strncpy(&szBuf[nTo],&szIn[nFrom],strlen("=?ISO-2022-JP"));
			nTo += strlen("=?ISO-2022-JP");
			nFrom += strlen("=?ISO-2022-JP");
			continue;
		}

		//エンコード終了部分検索
		pEnd = strstr(&szIn[nFrom],"?=");
		if(pEnd==NULL)
		{
			//"=?ISO-2022-JP"以降全てをデコード後用バッファへそのままコピー
			strcat(&szBuf[nTo],&szIn[nFrom]);
			break;
		}

		//エンコード部分取り出し
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

		//デコード
		memset(szDecoded,0,sizeof(szDecoded));
		if(nEncodeType == SMTP_ENCODE_BASE64)
			objBase64.StringDecode(&objBuf,(LPBYTE)szDecoded,&lLen);
		else if(nEncodeType == SMTP_ENCODE_QUOTED)
			objQuoted.StringDecode(&objBuf,(LPBYTE)szDecoded,&lLen);

		//デコード結果格納
		strcpy(&szBuf[nTo],szDecoded);
		nTo += strlen(szDecoded);
		nFrom = pEnd - szIn + 2;
	}

	//RFC1468(ISO-2022-JP)デコード
	objBuf = szBuf;
	objCodeConvManager.StringJisToSjis(&objBuf);

	//結果返却
	strcpy(szIn,(LPCTSTR)objBuf);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：メール本体デコード													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CPop::MailBodyDecode
	(	char*	szIn			//メールボディ１行
	)
{
	CString objBuf;
	CCodeConvManager	objCodeConvManager;

	//RFC1468(ISO-2022-JP)デコード
	objBuf = szIn;
	objCodeConvManager.StringJisToSjis(&objBuf);

	//結果返却
	strcpy(szIn,(LPCTSTR)objBuf);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：サブヘッダー解析														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CPop::AnalyzeSubHeader							// -2,-3 : RecvOneLine異常終了
													// 1     : サブヘッダー終了認識
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
			if(!strlen(szBuf)) //サブヘッダー終了
			{
//サブヘッダ終了行は、上位関数にて出力するためここではファイル出力しない
//				fprintf(fp,"%s\n",szBuf);
				return 1;
			}
		}

		if(!strnicmp(szBuf,"Content-Type:",strlen("Content-Type:")))
		{
			MailHeaderDecode(szBuf);
			fprintf(fp,"%s\n",szBuf);
			objContentType = szBuf;
			for(;;) //複数行に分かれる場合があるため、次の行もチェック
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
//サブヘッダ終了行は、上位関数にて出力するためここではファイル出力しない
//					fprintf(fp,"%s\n",szBuf);
					return 1;
				}
				if(strchr(szBuf,':') != NULL) //次のヘッダ出現
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
			for(;;) //複数行に分かれる場合があるため、次の行もチェック
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
//サブヘッダ終了行は、上位関数にて出力するためここではファイル出力しない
//					fprintf(fp,"%s\n",szBuf);
					return 1;
				}
				if(strchr(szBuf,':') != NULL) //次のヘッダ出現
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
			for(;;) //複数行に分かれる場合があるため、次の行もチェック
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
//サブヘッダ終了行は、上位関数にて出力するためここではファイル出力しない
//					fprintf(fp,"%s\n",szBuf);
					return 1;
				}
				if(strchr(szBuf,':') != NULL) //次のヘッダ出現
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
/*|	概要	：添付ファイルデコード(Base64,Uuencode用)								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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

	//添付ファイルパス編集
	sprintf(szPath,"%s\\%s", szAttachPath,(LPCTSTR)objAttachFileName);
	fp = fopen(szPath,"wb");
	if(fp==NULL)
		return FALSE;

	//デコード
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
		if(!strlen(szBuf)) //サブパート終了
		{
			fclose(fp);
			return 1;
		}
		if(szBuf[0]=='.' && szBuf[1]=='.')//'.'が２回続いている
		{
			//'.'を１つにする
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

			//添付ファイル出力
			if(bRet)
				fwrite(szDecoded,sizeof(BYTE),lLen,fp);
		}
	}

	fclose(fp);
	return 1;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：添付ファイルデコード(Quoted-Printable用)								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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

	//添付ファイルパス編集
	sprintf(szPath,"%s\\%s", szAttachPath,(LPCTSTR)objAttachFileName);
	fp = fopen(szPath,"wb");
	if(fp==NULL)
		return FALSE;

	//デコード
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
		if(!strlen(szBuf)) //サブパート終了
		{
			fclose(fp);
			return 1;
		}
		if(szBuf[0]=='.' && szBuf[1]=='.')//'.'が２回続いている
		{
			//'.'を１つにする
			memset(szTemp,0,sizeof(szTemp));
			strcpy(szTemp,&szBuf[1]);
			strcpy(szBuf,szTemp);
		}

		if(!objBuf.IsEmpty()) //前回読み込んだデータあり
		{
			//行末が"="でない場合には、そこを元データの改行として解釈する
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
			//添付ファイル出力
			if(bRet)
				fwrite(szDecoded,sizeof(BYTE),lLen,fp);
		}
	}

	fclose(fp);
	return 1;
}


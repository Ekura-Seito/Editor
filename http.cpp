// http.cpp : インプリメンテーション ファイル
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
// CHttp クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
			objMsg.LoadString(IDS_HTTP_DLLNOTFOUND);
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
CHttp::~CHttp()
{
	Disconnect();
}

/////////////////////////////////////////////////////////////////////////////
// CHttp パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＨＴＴＰサーバ接続													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CHttp::Connect
	(	CString&	objHost,				//接続先ホスト
		BOOL		bIsUseProxy				//プロキシサーバ経由か？
	)
{
	WORD	wPort=0;
	CString objMsg;

	//パラメータチェック
	if(!DllCheck())
	{
		objMsg.LoadString(IDS_HTTP_DLLNOTFOUND);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//接続先ポート
	if(bIsUseProxy)
		wPort = 8080;
	else
		wPort = 80;

	//ソケット接続(SockClientOpen)、受信スレッドを起動しない
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
/*|	概要	：ＨＴＭＬファイル取得													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CHttp::Get
	(	CString&	objPath,					//取得先パス
		CArchive&	ar,							//HTMLファイル保存先
		CHtmlOpenStatusDlg* pStatusDlg,			//状況表示用ダイアログ
		BOOL&		bIsUseBasicAuth,			//Basic認証を使用するか？
		CString		objUserId,					//Basic認証用ユーザＩＤ
		CString		objPasswd,					//Basic認証用パスワード
		CString&	objRealm					//Basic認証領域
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

	//取得コマンド編集＆送信
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

	//ヘッダ解析
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

		//HTTPレスポンス
		if(!strnicmp(szBuf,"HTTP/1.",strlen("HTTP/1.")))
		{
			p = strstr(szBuf,"401 ");
			if(p != NULL)
				bIsAuthorizationRequired = TRUE;
		}
		//Content-Lengthヘッダ
		if(!strnicmp(szBuf,"Content-Length:",strlen("Content-Length:")))
			m_nContentLength = atoi( &szBuf[strlen("Content-Length:")] );
		//WWW-Authenticateヘッダ
		if(!strnicmp(szBuf,"WWW-Authenticate:",strlen("WWW-Authenticate:")))
		{
			strupr(szBuf);
			p = strstr(szBuf,"BASIC REALM=");
			if(p != NULL)
				objRealm = p+strlen("BASIC REALM=");
		}

		//レスポンスヘッダ保持
		m_objResponseHeader += szBuf;
		m_objResponseHeader += "\r\n";

		if(!strlen(szBuf)) //空行！
			break;
	}

	//Basic認証が必要
	if(bIsAuthorizationRequired)
	{
		bIsUseBasicAuth = TRUE;
		//ボディ部を読み捨てる
		for(;;)
		{
			lRet = RecvToLocal();
			if(lRet < 0)
				break;
			m_pApp->MessagePump();
		}
		//切断
		Disconnect();
		return FALSE;
	}

	//ダウンロードするバイト数情報を表示
	m_nRecvSum = 0;
	if(m_nContentLength != 0 && pStatusDlg != NULL)
	{
		pStatusDlg->SetRange(0,m_nContentLength);
		pStatusDlg->SetPos(0);
	}

	//ボディ部を保存
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

	//切断
	Disconnect();
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：サーバーからの切断													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CHttp::Disconnect()
{
	//ソケットＤＬＬチェック
	if(!DllCheck())
		return;

	if(m_lSockID != -1)
	{
		//ソケットクローズ(SockClientClose)
		m_pApp->m_lpfnSckFunc9(m_lSockID);
	}
	m_lSockID = -1;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：レスポンスヘッダ全体取得												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CHttp::GetResponseHeader(CString& objBuf)
{
	objBuf = m_objResponseHeader;
}

/////////////////////////////////////////////////////////////////////////////
// CHttp プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ソケットＤＬＬチェック												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：１行受信バッファクリア												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CHttp::ClearRecvBuf()
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
LONG CHttp::RecvOneLine				//  1 : 正常終了
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
LONG CHttp::RecvToLocal()				//  0 : 正常終了
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
		lRet = m_pApp->m_lpfnSckFunc10( m_lSockID,			//ソケットＩＤ
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


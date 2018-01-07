// DdeClient.cpp : インプリメンテーション ファイル
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
// CDdeClient クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDdeClient::CDdeClient()
{
	UINT	unRet=0;

	//メンバ変数クリア
	m_idInst = NULL;
	m_ServiceName = _T("");
	m_TopicName = _T("");
	m_hConv = NULL;

	//ＤＤＥ初期化
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
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDdeClient::~CDdeClient()
{
	if(m_hConv)
		DisConnect();

	//ＤＤＥ終了
	::DdeUninitialize(m_idInst);
}

/////////////////////////////////////////////////////////////////////////////
// CDdeClient パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＤＤＥサーバーへの接続												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDdeClient::Connect()
{
	//サービスネーム
	m_ServiceName = DDE_SERVICENAME;
	HSZ hszService = ::DdeCreateStringHandle( m_idInst,
											  m_ServiceName,
											  CP_WINANSI );
	//トピックネーム
	m_TopicName = DDE_TOPICNAME;
	HSZ hszTopic = ::DdeCreateStringHandle( m_idInst,
										    m_TopicName,
											CP_WINANSI );
	//サーバーへ接続
	m_hConv = ::DdeConnect( m_idInst,
							hszService,
							hszTopic,
							NULL );
	//解放
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
/*|	概要	：ＤＤＥサーバーからの切断												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：ＤＤＥサーバーへのリクエスト											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDdeClient::Request
	(	const char*	pszPath
	)
{
	//アイテムネーム
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
// CDdeClient スタティック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＤＤＥコールバック													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
HDDEDATA CALLBACK EXPORT CDdeClient::DdeCallback
	(	UINT		iType,
		UINT		uFmt,
		HCONV		hCconv,
		HSZ			hsz1,			//トピック
		HSZ			hsz2,			//項目
		HDDEDATA	hData,
		DWORD		dwData1,
		DWORD		dwData2
	)
{
	return (HDDEDATA)NULL;
}


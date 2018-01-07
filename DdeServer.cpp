// DdeServer.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <afxtempl.h>					//for CTypedPtrList
#include <ddeml.h>						// for DDE
#include <io.h>							// for _access
#include "Editor.h"
#include "Editor_Def.h"
#include "StatusBarDisp.h"				//ステータスバー表示情報構造体
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

// 静的な m_pDdeServer オブジェクトを定義する
CDdeServer* CDdeServer::m_pDdeServer = NULL;

/////////////////////////////////////////////////////////////////////////////
// CDdeServer クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDdeServer::CDdeServer()
{
	UINT	unRet=0;
	HDDEDATA hData=0;

	//メンバ変数クリア
	m_pApp = (CEditorApp*)AfxGetApp();
	m_pMainWnd = NULL;
	m_idInst = 0;
	m_ServiceName = _T("");
	m_TopicName = _T("");

	m_pDdeServer = this;

	//ＤＤＥ初期化
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

	//ＤＤＥサーバーとして登録
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
//		AfxMessageBox("ＤＤＥサーバーとして登録 Error",(MB_OK|MB_ICONSTOP),0);
//	else
//		AfxMessageBox("ＤＤＥサーバーとして登録 OK",(MB_OK|MB_ICONINFORMATION),0);
#endif
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDdeServer::~CDdeServer()
{
	//ＤＤＥ終了
	::DdeUninitialize(m_idInst);
}

/////////////////////////////////////////////////////////////////////////////
// CDdeServer パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドポインタ設定													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDdeServer::SetMainWnd
	(	CWnd*	pWnd
	)
{
	m_pMainWnd = pWnd;
}

/////////////////////////////////////////////////////////////////////////////
// CDdeServer スタティック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＤＤＥコールバック													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
	case XTYP_CONNECT:			//「接続」
		//サービス名取得
		::DdeQueryString( m_pDdeServer->m_idInst,
						  hsz2,
						  szBuffer,
						  sizeof(szBuffer),
						  CP_WINANSI );
		//トピック名取得
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

	case XTYP_REQUEST:			//「要求」
#ifdef _DEBUG
//		sprintf(szMsg,"XTYP_REQUEST!!");
//		AfxMessageBox(szMsg,(MB_OK|MB_ICONINFORMATION),0);
#endif
		//トピック名取得
		::DdeQueryString( m_pDdeServer->m_idInst,
						  hsz1,
						  szBuffer,
						  sizeof(szBuffer),
						  CP_WINANSI );
		if( strcmp(szBuffer,DDE_TOPICNAME) == 0 )
		{
			//項目名取得
			::DdeQueryString( m_pDdeServer->m_idInst,
							  hsz2,
							  szBuffer2,
							  sizeof(szBuffer2),
							  CP_WINANSI );
			//存在チェック
			if( _access(szBuffer2,0) == 0 )
			{
				//指定ファイルをオープン
				CString objStr = szBuffer2;
				m_pDdeServer->m_pApp->OpenFile(objStr);
				if(m_pDdeServer->m_pMainWnd != NULL)
				{
					if(m_pDdeServer->m_pMainWnd->IsIconic())
						m_pDdeServer->m_pMainWnd->ShowWindow(SW_RESTORE);
//					m_pDdeServer->m_pMainWnd->SetForegroundWindow();
					m_pDdeServer->m_pApp->MySetForegroundWindow(m_pDdeServer->m_pMainWnd->m_hWnd);
				}
				//正常終了
				return ::DdeCreateDataHandle( m_pDdeServer->m_idInst,
											  (LPBYTE)"OK",
											  strlen("OK")+1,
											  0,
											  hsz2,
											  CF_TEXT,
											  0 );
			}
		}
		//異常終了
		return ::DdeCreateDataHandle( m_pDdeServer->m_idInst,
									  (LPBYTE)"NG",
									  strlen("NG")+1,
									  0,
									  hsz2,
									  CF_TEXT,
									  0 );
		break;

	case XTYP_DISCONNECT:			//「切断」
#ifdef _DEBUG
//		sprintf(szMsg,"XTYP_DISCONNECT!!");
//		AfxMessageBox(szMsg,(MB_OK|MB_ICONINFORMATION),0);
#endif
		break;
	}
	return NULL;
}


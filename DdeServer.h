// DdeServer.h : CDdeServer クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(DDESERVER_H)		//２重インクルード防止
#define DDESERVER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CMainFrame;

class CDdeServer
{
public:
	CDdeServer();
	~CDdeServer();
	static HDDEDATA CALLBACK EXPORT DdeCallback(UINT,UINT,HCONV,HSZ,HSZ,HDDEDATA,DWORD,DWORD);

	static CDdeServer* m_pDdeServer;
	void SetMainWnd(CWnd*);

private:
	CEditorApp* m_pApp;
	CWnd* m_pMainWnd;
	DWORD	m_idInst;				// ＤＤＥインスタンスＩＤ
	CString m_ServiceName;			// サービスネーム
	CString m_TopicName;
};

#endif //!defined(DDESERVER_H)


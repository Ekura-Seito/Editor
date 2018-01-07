// DdeClient.h : CDdeClient クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(DDECLIENT_H)		//２重インクルード防止
#define DDECLIENT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDdeClient
{
public:
	CDdeClient();
	~CDdeClient();
	static HDDEDATA CALLBACK EXPORT DdeCallback(UINT,UINT,HCONV,HSZ,HSZ,HDDEDATA,DWORD,DWORD);

	BOOL Connect();
	void DisConnect();
	BOOL Request(const char*);

private:
	DWORD	m_idInst;				// ＤＤＥインスタンスＩＤ
	CString m_ServiceName;			// サービスネーム
	CString m_TopicName;			// トピックネーム
	HCONV	m_hConv;
};

#endif //!defined(DDECLIENT_H)


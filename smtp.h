/////////////////////////////////////////////////////////////////////////////
// CSmtp

#if !defined(SMTP_H)		//２重インクルード防止
#define SMTP_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

//ソケットＤＬＬ関数アドレス
typedef LONG  (CALLBACK* LPFNDLLFUNC1)(char*,char*);						//GetLocalHostInfo
typedef LONG  (CALLBACK* LPFNDLLFUNC2)(WORD);								//SockServerOpen
typedef LONG  (CALLBACK* LPFNDLLFUNC3)(LONG,LONG,LPVOID,DWORD,LPDWORD);		//SockServerSend
typedef LONG  (CALLBACK* LPFNDLLFUNC4)(LONG,LONG,LPVOID,LPDWORD);			//SockServerRecv
typedef LONG  (CALLBACK* LPFNDLLFUNC5)(LONG);								//SockServerClose
typedef LONG  (CALLBACK* LPFNDLLFUNC6)(char*,WORD,BOOL);					//SockClientOpen
typedef LONG  (CALLBACK* LPFNDLLFUNC7)(LONG,LPVOID,DWORD,LPDWORD);			//SockClientSend
typedef LONG  (CALLBACK* LPFNDLLFUNC8)(LONG,LPVOID,LPDWORD);				//SockClientRecv
typedef LONG  (CALLBACK* LPFNDLLFUNC9)(LONG);								//SockClientClose
typedef LONG  (CALLBACK* LPFNDLLFUNC10)(LONG,LPVOID,DWORD,DWORD,LPDWORD);	//SockClientRecv2

class CSmtp
{
public:
	CSmtp();
	~CSmtp();

	BOOL	Connect(CString);
	void	Disconnect();
	BOOL	SendMail(char*);

	void	SetSubject(CString);
	void	SetFrom(CString);
	void	SetTo(CString);
	void	SetCc(CString);
	void	SetBcc(CString);
	void	SetReplyTo(CString);
	void	SetReference(CString);
	void	SetAttachFile(CString);
	void	SetEncodeType(int);
	void	SetSignature(CString);

	BOOL	m_bIsSendDate;
	BOOL	m_bIsSendMessageID;
	BOOL	m_bIsReqMsgDispsition;

protected:
	BOOL	DllCheck();
	void	DispSmtpMessage(int,CString);
	CString	GetTimeZone();
	void	CancelDisconnect();
	BOOL	CheckServerResponse(int*);
	BOOL	CheckExistNonAscii(CString&);
	void	MessageHeaderEncode(CString*);
	BOOL	SendMailHeader();
	BOOL	SendMailBody(char*);
	BOOL	SendAttachedFiles();
	BOOL	SendMailEnd();

private:
	CEditorApp* m_pApp;
	BOOL	m_bIsConnected;
	LONG	m_lSockID;
	char	m_szDestIPAddress[256];				// 接続先のＩＰアドレス
	WORD	m_wDestPort;						// 接続先のポート番号

	CString m_objSubject;						// メールの件名
	CString	m_objFrom;							// 差し出し人
	CString m_objMessageID;						// メッセージＩＤ
	CString m_objReplyTo;						// 返信先
	CString m_objReference;						// レファレンス
	CString m_objBoundary;						// ブロック境界
	CString m_objMIMEText;
	CString m_objMailerName;
	CString m_objSignature;						// 署名
	int		m_nEncodeType;						// エンコード方式(Base64, Quoted-Printable)
	CArray<CString,CString&> m_objTo;			// 受け取り人
	CArray<CString,CString&> m_objCc;			// カーボンコピー受取人
	CArray<CString,CString&> m_objBcc;  		// ブラインドカーボンコピー受取人
	CArray<CString,CString&> m_objAttachFiles;	// 添付ファイル名

	LPFNDLLFUNC1	m_lpfnDllFunc1;
	LPFNDLLFUNC2	m_lpfnDllFunc2;
	LPFNDLLFUNC3	m_lpfnDllFunc3;
	LPFNDLLFUNC4	m_lpfnDllFunc4;
	LPFNDLLFUNC5	m_lpfnDllFunc5;
	LPFNDLLFUNC6	m_lpfnDllFunc6;
	LPFNDLLFUNC7	m_lpfnDllFunc7;
	LPFNDLLFUNC8	m_lpfnDllFunc8;
	LPFNDLLFUNC9	m_lpfnDllFunc9;
	LPFNDLLFUNC10	m_lpfnDllFunc10;
};

#endif //!defined(SMTP_H)


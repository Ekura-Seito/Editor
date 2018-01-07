/////////////////////////////////////////////////////////////////////////////
// CPop

#if !defined(POP_H)		//２重インクルード防止
#define POP_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CPop
{
public:
	CPop();
	~CPop();

	void	SetConfig(int,int);
	BOOL	Connect(CString&,CString&,CString&,int);
	void	Disconnect();
	int		GetMailCount();
	BOOL	GetMailInfo(int,char*,char*,char*,BOOL*);
	BOOL	GetMailSize(int,int*);
	BOOL	RecvOneMail(int,char*,char*);
	BOOL	DelOneMail(int);

protected:
	BOOL	DllCheck();
	BOOL	CheckServerResponse(int,char*);
	void	ClearRecvBuf();
	LONG	RecvOneLine(char*);
	LONG	RecvToLocal();
	void	MailHeaderDecode(char*);
	void	MailBodyDecode(char*);
	LONG	AnalyzeSubHeader(FILE*,CString&,CString&,CString&);
	LONG	DecodeAttachFile(char*,CString&,int);
	LONG	DecodeQuotedAttachFile(char*,CString&);

private:
	CEditorApp* m_pApp;
	BOOL	m_bIsConnected;
	LONG	m_lSockID;
	char	m_szDestIPAddress[256];				// 接続先のＩＰアドレス
	WORD	m_wDestPort;						// 接続先のポート番号
	int		m_nMailCount;						// ＰＯＰサーバ内メール総数
	int		m_nByteSum;							// ＰＯＰサーバ内メールの総バイト数
	char	m_szRecvBuf[2048];
	DWORD	m_dwRecvByte;
	int		m_nMailIndex;
	int		m_nMailSize;
	char	m_szTimeStamp[256];					// ＡＰＯＰコマンド用タイムスタンプ保持用

	int		m_nRecvRetryCount;
	int		m_nRecvTimeout;
};

#endif //!defined(POP_H)


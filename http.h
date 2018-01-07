/////////////////////////////////////////////////////////////////////////////
// CHttp

#if !defined(HTTP_H)		//‚QdƒCƒ“ƒNƒ‹[ƒh–hŽ~
#define HTTP_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CHtmlOpenStatusDlg;

class CHttp
{
public:
	CHttp();
	~CHttp();

	BOOL	Connect(CString&,BOOL);
	BOOL	Get(CString&,CArchive&,CHtmlOpenStatusDlg*,BOOL&,CString,CString,CString&);
	void	Disconnect();
	void	GetResponseHeader(CString&);

protected:
	BOOL	DllCheck();
	void	ClearRecvBuf();
	LONG	RecvOneLine(char*);
	LONG	RecvToLocal();

private:
	CEditorApp* m_pApp;
	LONG	m_lSockID;
	char	m_szRecvBuf[2048];
	DWORD	m_dwRecvByte;
	int		m_nRecvRetryCount;
	int		m_nRecvTimeout;
	CString m_objResponseHeader;
	int		m_nContentLength;
	int		m_nRecvSum;
};

#endif //!define(HTTP_H)


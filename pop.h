/////////////////////////////////////////////////////////////////////////////
// CPop

#if !defined(POP_H)		//�Q�d�C���N���[�h�h�~
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
	char	m_szDestIPAddress[256];				// �ڑ���̂h�o�A�h���X
	WORD	m_wDestPort;						// �ڑ���̃|�[�g�ԍ�
	int		m_nMailCount;						// �o�n�o�T�[�o�����[������
	int		m_nByteSum;							// �o�n�o�T�[�o�����[���̑��o�C�g��
	char	m_szRecvBuf[2048];
	DWORD	m_dwRecvByte;
	int		m_nMailIndex;
	int		m_nMailSize;
	char	m_szTimeStamp[256];					// �`�o�n�o�R�}���h�p�^�C���X�^���v�ێ��p

	int		m_nRecvRetryCount;
	int		m_nRecvTimeout;
};

#endif //!defined(POP_H)


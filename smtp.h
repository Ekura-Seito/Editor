/////////////////////////////////////////////////////////////////////////////
// CSmtp

#if !defined(SMTP_H)		//�Q�d�C���N���[�h�h�~
#define SMTP_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

//�\�P�b�g�c�k�k�֐��A�h���X
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
	char	m_szDestIPAddress[256];				// �ڑ���̂h�o�A�h���X
	WORD	m_wDestPort;						// �ڑ���̃|�[�g�ԍ�

	CString m_objSubject;						// ���[���̌���
	CString	m_objFrom;							// �����o���l
	CString m_objMessageID;						// ���b�Z�[�W�h�c
	CString m_objReplyTo;						// �ԐM��
	CString m_objReference;						// ���t�@�����X
	CString m_objBoundary;						// �u���b�N���E
	CString m_objMIMEText;
	CString m_objMailerName;
	CString m_objSignature;						// ����
	int		m_nEncodeType;						// �G���R�[�h����(Base64, Quoted-Printable)
	CArray<CString,CString&> m_objTo;			// �󂯎��l
	CArray<CString,CString&> m_objCc;			// �J�[�{���R�s�[���l
	CArray<CString,CString&> m_objBcc;  		// �u���C���h�J�[�{���R�s�[���l
	CArray<CString,CString&> m_objAttachFiles;	// �Y�t�t�@�C����

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


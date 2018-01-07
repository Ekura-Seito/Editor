// DdeServer.h : CDdeServer �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(DDESERVER_H)		//�Q�d�C���N���[�h�h�~
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
	DWORD	m_idInst;				// �c�c�d�C���X�^���X�h�c
	CString m_ServiceName;			// �T�[�r�X�l�[��
	CString m_TopicName;
};

#endif //!defined(DDESERVER_H)


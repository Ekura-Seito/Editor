// DdeClient.h : CDdeClient �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(DDECLIENT_H)		//�Q�d�C���N���[�h�h�~
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
	DWORD	m_idInst;				// �c�c�d�C���X�^���X�h�c
	CString m_ServiceName;			// �T�[�r�X�l�[��
	CString m_TopicName;			// �g�s�b�N�l�[��
	HCONV	m_hConv;
};

#endif //!defined(DDECLIENT_H)


// MailSendStatusDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(MAILSENDSTATUSDLG_H)
#define MAILSENDSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CMailSendStatusDlg : public CDialog
{
public:
	CMailSendStatusDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	BOOL IsCanceled();						// �L�����Z���`�F�b�N
	void SetStatus(char*);					// �X�e�[�^�X���b�Z�[�W�ݒ�
	void SetStatus(CString&);				// �X�e�[�^�X���b�Z�[�W�ݒ�

	//{{AFX_DATA(CMailSendStatusDlg)
	enum { IDD = IDD_MAILSENDSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMailSendStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMailSendStatusDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL			m_bIsCanceled;
	CEditorApp*		m_pApp;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(MAILSENDSTATUSDLG_H)


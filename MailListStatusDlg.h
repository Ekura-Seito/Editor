// MailListStatusDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(MAILLISTSTATUSDLG_H)
#define MAILLISTSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CMailListStatusDlg : public CDialog
{
public:
	CMailListStatusDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	BOOL IsCanceled();						// �L�����Z���`�F�b�N
	void SetStatus(char*);					// �X�e�[�^�X���b�Z�[�W�ݒ�
	void SetStatus(CString&);				// �X�e�[�^�X���b�Z�[�W�ݒ�

	//{{AFX_DATA(CMailListStatusDlg)
	enum { IDD = IDD_MAILLISTSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMailListStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMailListStatusDlg)
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

#endif // !defined(MAILLISTSTATUSDLG_H)


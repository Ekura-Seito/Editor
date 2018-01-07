// HtmlOpenStatusDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(HTMLOPENSTATUSDLG_H)
#define HTMLOPENSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CHtmlOpenStatusDlg : public CDialog
{
public:
	CHtmlOpenStatusDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	BOOL IsCanceled();						// �L�����Z���`�F�b�N
	void SetStatus(char*);					// �X�e�[�^�X���b�Z�[�W�ݒ�
	void SetStatus(CString&);				// �X�e�[�^�X���b�Z�[�W�ݒ�
	void SetRange(int,int);					//�v���O���X�o�[�͈͐ݒ�
	void SetPos(int);						//�v���O���X�o�[�ʒu�ݒ�


	//{{AFX_DATA(CHtmlOpenStatusDlg)
	enum { IDD = IDD_HTMLOPENSTATUS };
	CProgressCtrl	m_objProgressBar;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CHtmlOpenStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CHtmlOpenStatusDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL			m_bIsCanceled;
	CEditorApp*		m_pApp;
	HICON			m_hIcon;
	HICON			m_hIconS;
	int				m_nProgressLower;
	int				m_nProgressUpper;
};

#endif // !defined(HTMLOPENSTATUSDLG_H)


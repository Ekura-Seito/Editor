// ExecPasswdDlg.h : CExecPasswdDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(EXECPASSWDDLG_H)		//�Q�d�C���N���[�h�h�~
#define EXECPASSWDDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CExecPasswdDlg : public CDialog
{
public:
	CExecPasswdDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	CString	m_objExecPasswdStr;				// �N���p�X���[�h

	//{{AFX_DATA(CExecPasswdDlg)
	enum { IDD = IDD_EXECPASSWD };
	CEdit	m_objExecPasswdEdit;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CExecPasswdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExecPasswdDlg)
	afx_msg void OnChangeEditExecpasswd();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*		m_pApp;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(EXECPASSWDDLG_H)


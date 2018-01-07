// CheckExecPasswdDlg.h : CCheckExecPasswdDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(CHECKEXECPASSWDDLG_H)		//�Q�d�C���N���[�h�h�~
#define CHECKEXECPASSWDDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CCheckExecPasswdDlg : public CDialog
{
public:
	CCheckExecPasswdDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	BOOL	IsPasswdOK();						//�p�X���[�h�n�j�`�F�b�N

	//{{AFX_DATA(CCheckExecPasswdDlg)
	enum { IDD = IDD_CHECKEXECPASSWD };
	CEdit	m_objExecPasswdEdit;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCheckExecPasswdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCheckExecPasswdDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL		m_bIsPasswdOK;
	CEditorApp*	m_pApp;						//�A�v���P�[�V�����N���X�ւ̃|�C���^
	HICON		m_hIcon;
	HICON		m_hIconS;
};

#endif // !defined(CHECKEXECPASSWDDLG_H)


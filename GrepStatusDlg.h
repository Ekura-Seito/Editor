// GrepStatusDlg.h : CGrepStatusDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(GREPSTATUSDLG_H)		//�Q�d�C���N���[�h�h�~
#define GREPSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CGrepStatusDlg : public CDialog
{
public:
	CGrepStatusDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	BOOL IsCanceled();						// �L�����Z���`�F�b�N
	void SetGrepStr(CString&);				// ���������񃁃b�Z�[�W�ݒ�
	void SetFileKind(CString&);				// �t�@�C����ʃ��b�Z�[�W�ݒ�
	void SetFolder(CString&);				// �����t�H���_���b�Z�[�W�ݒ�
	void SetFile(CString&);					// �����t�@�C�����b�Z�[�W�ݒ�
	void IncMatchCount();					// �Y�������J�E���g�A�b�v����
	long GetMatchCount();					// �Y�������擾

	//{{AFX_DATA(CGrepStatusDlg)
	enum { IDD = IDD_GREPSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CGrepStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CGrepStatusDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;
	BOOL	m_bIsCanceled;
	long	m_lMatchCount;
	CStaticFilespec   m_wndStaticFolder;
	HICON	m_hIcon;
	HICON	m_hIconS;
};

#endif // !defined(GREPSTATUSDLG_H)


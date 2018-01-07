// EncriptStatusDlg.h : CEncriptStatusDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ENCRIPTSTATUSDLG_H)		//�Q�d�C���N���[�h�h�~
#define ENCRIPTSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CEncriptStatusDlg : public CDialog
{
public:
	CEncriptStatusDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	BOOL IsCanceled();						// �L�����Z���`�F�b�N
	void SetFileKind(CString&);				// �t�@�C����ʃ��b�Z�[�W�ݒ�
	void SetFolder(CString&);				// �t�H���_���b�Z�[�W�ݒ�
	void SetFile(CString&);					// �t�@�C�����b�Z�[�W�ݒ�
	void IncEncriptFileCount();				// �Í����t�@�C�������J�E���g�A�b�v����
	long GetEncriptFileCount();				// �Í����t�@�C�������擾

	//{{AFX_DATA(CEncriptStatusDlg)
	enum { IDD = IDD_ENCRIPTSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CEncriptStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CEncriptStatusDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;
	BOOL	m_bIsCanceled;
	long	m_lEncriptFileCount;
	CStaticFilespec   m_wndStaticFolder;
	HICON	m_hIcon;
	HICON	m_hIconS;
};

#endif // !defined(ENCRIPTSTATUSDLG_H)


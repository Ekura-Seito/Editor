// DecriptStatusDlg.h : CDecriptStatusDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(DECRIPTSTATUSDLG_H)		//�Q�d�C���N���[�h�h�~
#define DECRIPTSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CDecriptStatusDlg : public CDialog
{
public:
	CDecriptStatusDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	//{{AFX_DATA(CDecriptStatusDlg)
	enum { IDD = IDD_DECRIPTSTATUS };
	//}}AFX_DATA

	BOOL IsCanceled();						// �L�����Z���`�F�b�N
	void SetFileKind(CString&);				// �t�@�C����ʃ��b�Z�[�W�ݒ�
	void SetFolder(CString&);				// �t�H���_���b�Z�[�W�ݒ�
	void SetFile(CString&);					// �t�@�C�����b�Z�[�W�ݒ�
	void IncDecriptFileCount();				// �������t�@�C�������J�E���g�A�b�v����
	long GetDecriptFileCount();				// �������t�@�C�������擾

	//{{AFX_VIRTUAL(CDecriptStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDecriptStatusDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp* m_pApp;
	BOOL	m_bIsCanceled;
	long	m_lDecriptFileCount;
	CStaticFilespec   m_wndStaticFolder;
	HICON	m_hIcon;
	HICON	m_hIconS;
};

#endif // !defined(DECRIPTSTATUSDLG_H)


// WCntStatusDlg.h : CWCntStatusDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(WCNTSTATUSDLG_H)		//�Q�d�C���N���[�h�h�~
#define WCNTSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CWCntStatusDlg : public CDialog
{
public:
	CWCntStatusDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	BOOL IsCanceled();						// �L�����Z���`�F�b�N
	void SetFileKind(CString&);				// �t�@�C����ʃ��b�Z�[�W�ݒ�
	void SetFolder(CString&);				// �t�H���_���b�Z�[�W�ݒ�
	void SetFile(CString&);					// �J�E���g���t�@�C�����b�Z�[�W�ݒ�

	//{{AFX_DATA(CWCntStatusDlg)
	enum { IDD = IDD_WCNTSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CWCntStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWCntStatusDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;
	BOOL	m_bIsCanceled;
	CStaticFilespec   m_wndStaticFolder;
	HICON	m_hIcon;
	HICON	m_hIconS;
};

#endif //!defined(WCNTSTATUSDLG_H)


// FileSearchStatusDlg.h : CFileSearchStatusDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(FILESEARCHSTATUSDLG_H)		//�Q�d�C���N���[�h�h�~
#define FILESEARCHSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CFileSearchStatusDlg : public CDialog
{
public:
	CFileSearchStatusDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	BOOL IsCanceled();						// �L�����Z���`�F�b�N
	void SetFileKind(CString&);				// �t�@�C����ʃ��b�Z�[�W�ݒ�
	void SetFolder(CString&);				// �����t�H���_���b�Z�[�W�ݒ�
	void SetFile(CString&);					// �����t�@�C�����b�Z�[�W�ݒ�
	void IncMatchCount();					// �Y�������J�E���g�A�b�v����
	long GetMatchCount();					// �Y�������擾

	//{{AFX_DATA(CFileSearchStatusDlg)
	enum { IDD = IDD_FILESEARCHSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CFileSearchStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CFileSearchStatusDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;
	CStaticFilespec m_wndStaticFolder;
	BOOL	m_bIsCanceled;
	long	m_lMatchCount;
	HICON	m_hIcon;
	HICON	m_hIconS;
};

#endif // !defined(FILESEARCHSTATUSDLG_H)


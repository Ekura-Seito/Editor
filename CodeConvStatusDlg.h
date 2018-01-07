// CodeConvStatusDlg.h : CCodeConvStatusDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(CODECONVSTATUSDLG_H)		//�Q�d�C���N���[�h�h�~
#define CODECONVSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CCodeConvStatusDlg : public CDialog
{
public:
	CCodeConvStatusDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	BOOL IsCanceled();						// �L�����Z���`�F�b�N
	void SetConvKindStr(int);				// �R�[�h�ϊ���ʃ��b�Z�[�W�ݒ�
	void SetFileKind(CString&);				// �t�@�C����ʃ��b�Z�[�W�ݒ�
	void SetFolder(CString&);				// �t�H���_���b�Z�[�W�ݒ�
	void SetFile(CString&);					// �t�@�C�����b�Z�[�W�ݒ�
	void IncConvFileCount();				// �R�[�h�ϊ��t�@�C�������J�E���g�A�b�v����
	long GetConvFileCount();				// �R�[�h�ϊ��t�@�C�������擾

	//{{AFX_DATA(CCodeConvStatusDlg)
	enum { IDD = IDD_CONVSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCodeConvStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCodeConvStatusDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*		m_pApp;
	BOOL	m_bIsCanceled;
	long	m_lConvFileCount;
	CStaticFilespec   m_wndStaticFolder;
	HICON	m_hIcon;
	HICON	m_hIconS;
};

#endif // !defined(CODECONVSTATUSDLG_H)


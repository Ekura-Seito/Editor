// KaigyoConvStatusDlg.h : CKaigyoConvStatusDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(KAIGYOCONVSTATUSDLG_H)
#define KAIGYOCONVSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CKaigyoConvStatusDlg : public CDialog
{
public:
	CKaigyoConvStatusDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	BOOL IsCanceled();						// �L�����Z���`�F�b�N
	void SetConvKindStr(CString&);			// �R�[�h�ϊ���ʃ��b�Z�[�W�ݒ�
	void SetFileKind(CString&);				// �t�@�C����ʃ��b�Z�[�W�ݒ�
	void SetFolder(CString&);				// �t�H���_���b�Z�[�W�ݒ�
	void SetFile(CString&);					// �t�@�C�����b�Z�[�W�ݒ�
	void IncConvFileCount();				// �R�[�h�ϊ��t�@�C�������J�E���g�A�b�v����
	long GetConvFileCount();				// �R�[�h�ϊ��t�@�C�������擾

	//{{AFX_DATA(CKaigyoConvStatusDlg)
	enum { IDD = IDD_KAIGYOCONVSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CKaigyoConvStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CKaigyoConvStatusDlg)
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

#endif // !defined(KAIGYOCONVSTATUSDLG_H)


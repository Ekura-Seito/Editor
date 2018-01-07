// DirCompareStatusDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(DirCompareStatusDLG_H)		//�Q�d�C���N���[�h�h�~
#define DirCompareStatusDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CDirCompareStatusDlg : public CDialog
{
public:
	CDirCompareStatusDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	BOOL IsCanceled();						// �L�����Z���`�F�b�N
	void SetDir1(CString&);					// ��r���f�B���N�g���ݒ�
	void SetDir2(CString&);					// ��r���f�B���N�g���ݒ�
	void SetStatus(char*);					// ��r�󋵐ݒ�
	void SetStatus(CString&);				// ��r�󋵐ݒ�
	void IncCompareFileCount();				// ��r�t�@�C�����J�E���g�A�b�v
	long GetCompareFileCount();				// ��r�t�@�C�����擾
	void IncAddFileCount();					// �ǉ��t�@�C�����J�E���g�A�b�v
	long GetAddFileCount();					// �ǉ��t�@�C�����擾
	void IncDelFileCount();					// �폜�t�@�C�����J�E���g�A�b�v
	long GetDelFileCount();					// �폜�t�@�C�����擾
	void IncDiffFileCount();				// ����t�@�C�����J�E���g�A�b�v
	long GetDiffFileCount();				// ����t�@�C�����擾

	//{{AFX_DATA(CDirCompareStatusDlg)
	enum { IDD = IDD_DIRCOMPARESTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDirCompareStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDirCompareStatusDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*		m_pApp;
	BOOL			m_bIsCanceled;
	CStaticFilespec m_wndStaticDirName1;
	CStaticFilespec m_wndStaticDirName2;
	long			m_lCompareFileCount;
	long			m_lAddFileCount;
	long			m_lDelFileCount;
	long			m_lDiffFileCount;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif //!defined(DirCompareStatusDLG_H)


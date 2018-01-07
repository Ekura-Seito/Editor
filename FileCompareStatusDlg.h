// FileCompareStatusDlg.h : CFileCompareStatusDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(FileCompareStatusDLG_H)		//�Q�d�C���N���[�h�h�~
#define FileCompareStatusDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CFileCompareStatusDlg : public CDialog
{
public:
	CFileCompareStatusDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	BOOL IsCanceled();						// �L�����Z���`�F�b�N
	void SetStatus(char*);					// �X�e�[�^�X���b�Z�[�W�ݒ�
	void SetStatus(CString&);				// �X�e�[�^�X���b�Z�[�W�ݒ�

	CString m_objFile1Name;					// ���t�@�C����
	CString m_objFile2Name;					// ���t�@�C����

	//{{AFX_DATA(CFileCompareStatusDlg)
	enum { IDD = IDD_FILECOMPARE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CFileCompareStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CFileCompareStatusDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*		m_pApp;
	BOOL			m_bIsCanceled;
	CStaticFilespec m_wndStaticFileName1;
	CStaticFilespec m_wndStaticFileName2;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(FileCompareStatusDLG_H)


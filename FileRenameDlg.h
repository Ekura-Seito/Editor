// FileRenameDlg.h : CFileRenameDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(FILERENAMEDLG_H)		//�Q�d�C���N���[�h�h�~
#define FILERENAMEDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CFileRenameDlg : public CDialog
{
public:
	CFileRenameDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	//{{AFX_DATA(CFileRenameDlg)
	enum { IDD = IDD_FILERENAME };
	CEdit	m_objEditToFile;
	//}}AFX_DATA

	char		m_szFromPath[_MAX_PATH];
	CString		m_objToFile;
	char		m_szToPath[_MAX_PATH];

	//{{AFX_VIRTUAL(CFileRenameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CFileRenameDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;
	char		m_szFromDir[_MAX_PATH];
	char		m_szFromName[_MAX_FNAME];
	HICON		m_hIcon;
	HICON		m_hIconS;
};

#endif //!defined(FILERENAMEDLG_H)


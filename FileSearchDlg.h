// FileSearchDlg.h : CFileSearchDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(FILESEARCHDLG_H)		//�Q�d�C���N���[�h�h�~
#define FILESEARCHDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CAutoResizer;

class CFileSearchDlg : public CDialog
{
public:
	CFileSearchDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_DATA(CFileSearchDlg)
	enum { IDD = IDD_FILESEARCH };
	CEdit	m_objEditFolder;
	CEdit	m_objEditFileKind;
	BOOL	m_bIsSearchSubFolder;
	//}}AFX_DATA

	CString	m_objFolder;					// �t�@�C���������s�t�H���_
	CString	m_objFileKind;					// �����Ώۂ̃t�@�C�����

	//{{AFX_VIRTUAL(CFileSearchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CFileSearchDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	virtual void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CToolTipCtrl 	m_tooltip;
	CEditorApp*		m_pApp;
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(FILESEARCHDLG_H)


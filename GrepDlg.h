// GrepDlg.h : CGrepDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(GREPDLG_H)		//�Q�d�C���N���[�h�h�~
#define GREPDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CMyComboBoxEx;
class CAutoResizer;

class CGrepDlg : public CDialog
{
public:
	CGrepDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_DATA(CGrepDlg)
	enum { IDD = IDD_GREP };
	CMyComboBoxEx	m_objComboGrepString;
	CString	m_objGrepString;
	CEdit	m_objEditFolder;
	CEdit	m_objEditFileKind;
	BOOL	m_bIsMatchCaseGrep;
	BOOL	m_bIsMatchWholeWordGrep;
	BOOL	m_bIsSearchSubFolderGrep;
	BOOL	m_bIsUseRegularExpression;
	//}}AFX_DATA

	CString m_objPastGrepString[20];		// �ߋ��ɂf�q�d�o����������
	CString	m_objFolder;					// �f�q�d�o���s�t�H���_
	CString	m_objFileKind;					// �����Ώۂ̃t�@�C�����

	//{{AFX_VIRTUAL(CGrepDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CGrepDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	virtual void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCheckRegex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CToolTipCtrl 	m_tooltip;
	CEditorApp*		m_pApp;						// �A�v���P�[�V�����N���X�ւ̃|�C���^
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(GREPDLG_H)


// WordCountDlg.h : CWordCountDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(WORDCOUNTDLG_H)		//�Q�d�C���N���[�h�h�~
#define WORDCOUNTDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CAutoResizer;

class CWordCountDlg : public CDialog
{
public:
	CWordCountDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_DATA(CWordCountDlg)
	enum { IDD = IDD_WORDCOUNT };
	CEdit	m_objEditFolder;
	CEdit	m_objEditFileKind;
	BOOL	m_bIsSearchSubFolder;
	//}}AFX_DATA

	CString	m_objFolder;					// ���[�h�J�E���g���s�t�H���_
	CString	m_objFileKind;					// �J�E���g�Ώۂ̃t�@�C�����

	//{{AFX_VIRTUAL(CWordCountDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWordCountDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	virtual void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CToolTipCtrl m_tooltip;
	CEditorApp*		m_pApp;
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(WORDCOUNTDLG_H)


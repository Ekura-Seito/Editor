// EncriptDlg.h : CEncriptDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(ENCRIPTDLG_H)		//�Q�d�C���N���[�h�h�~
#define ENCRIPTDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CAutoResizer;

class CEncriptDlg : public CDialog
{
public:
	CEncriptDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_DATA(CEncriptDlg)
	enum { IDD = IDD_ENCRIPT };
	CEdit	m_objEditKey;
	CEdit	m_objEditFileKind;
	CEdit	m_objEditFolder;
	BOOL	m_bIsOutOtherFile;
	BOOL	m_bIsSearchSubFolder;
	//}}AFX_DATA

	CString m_objKey;						// �Í����L�[
	CString	m_objFolder;					// �Í������s�t�H���_
	CString	m_objFileKind;					// �Í����Ώۂ̃t�@�C�����

	//{{AFX_VIRTUAL(CEncriptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CEncriptDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBrowse();
	afx_msg void OnMyHelp();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CToolTipCtrl m_tooltip;
	CEditorApp*	m_pApp;						// �A�v���P�[�V�����N���X�ւ̃|�C���^
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(ENCRIPTDLG_H)


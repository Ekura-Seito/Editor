// DecriptDlg.h : CDecriptDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(DECRIPTDLG_H)		//�Q�d�C���N���[�h�h�~
#define DECRIPTDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CAutoResizer;

class CDecriptDlg : public CDialog
{
public:
	CDecriptDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_DATA(CDecriptDlg)
	enum { IDD = IDD_DECRIPT };
	CEdit	m_objEditKey;
	CEdit	m_objEditFolder;
	CEdit	m_objEditFileKind;
	BOOL	m_bIsOutOtherFile;
	BOOL	m_bIsSearchSubFolder;
	//}}AFX_DATA

	CString m_objKey;						// �������L�[
	CString	m_objFolder;					// ���������s�t�H���_
	CString	m_objFileKind;					// �������Ώۂ̃t�@�C�����

	//{{AFX_VIRTUAL(CDecriptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDecriptDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	virtual void OnOK();
	afx_msg void OnMyHelp();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CToolTipCtrl	m_tooltip;
	CEditorApp*		m_pApp;						// �A�v���P�[�V�����N���X�ւ̃|�C���^
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(DECRIPTDLG_H)


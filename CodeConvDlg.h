// CodeConvDlg.h : CCodeConvDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(CODECONVDLG_H)		//�Q�d�C���N���[�h�h�~
#define CODECONVDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CAutoResizer;

class CCodeConvDlg : public CDialog
{
public:
	CCodeConvDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_DATA(CCodeConvDlg)
	enum { IDD = IDD_CODECONV };
	CComboBox		m_objComboConvKind;
	CEdit	m_objEditFolder;
	CEdit	m_objEditFileKind;
	BOOL	m_bIsOutOtherFile;
	BOOL	m_bIsSearchSubFolder;
	CComboBox		m_objComboInCode;
	//}}AFX_DATA

	CString	m_objFolder;					// �����R�[�h�ϊ����s�t�H���_
	CString	m_objFileKind;					// �ϊ��Ώۂ̃t�@�C�����
	int			m_nConvKind;
	int         m_nInCode;

	//{{AFX_VIRTUAL(CCodeConvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCodeConvDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	virtual void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CToolTipCtrl	m_tooltip;
	CEditorApp*		m_pApp;
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(CODECONVDLG_H)


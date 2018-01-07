// KaigyoConvDlg.h : CKaigyoConvDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(KAIGYOCONVDLG_H)
#define KAIGYOCONVDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CAutoResizer;

class CKaigyoConvDlg : public CDialog
{
public:
	CKaigyoConvDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_DATA(CKaigyoConvDlg)
	enum { IDD = IDD_KAIGYOCONV };
	CComboBox		m_objComboConvKind;
	CString			m_objConvKind;
	CEdit	m_objEditFolder;
	CEdit	m_objEditFileKind;
	BOOL	m_bIsOutOtherFile;
	BOOL	m_bIsSearchSubFolder;
	//}}AFX_DATA

	CString	m_objFolder;					// ���s�R�[�h�ϊ����s�t�H���_
	CString	m_objFileKind;					// �ϊ��Ώۂ̃t�@�C�����

	//{{AFX_VIRTUAL(CKaigyoConvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CKaigyoConvDlg)
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

#endif // !defined(KAIGYOCONVDLG_H)


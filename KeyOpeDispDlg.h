// KeyOpeDispDlg.h: CKeyOpeDispDlg �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(KeyOpeDispDlg_H)		//�Q�d�C���N���[�h�h�~
#define KeyOpeDispDlg_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class COperationManager;
class CStaticFilespec;
class CListCtrlEx;
class CEditorApp;
class CAutoResizer;

class CKeyOpeDispDlg : public CDialog
{
public:
	CKeyOpeDispDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	void Initialize(COperationManager*);	// ������

	//{{AFX_DATA(CKeyOpeDispDlg)
	enum { IDD = IDD_KEYOPE_DISP };
	CEdit	m_objOperatinNameEdit;
	CListCtrlEx	m_objKeyOpeList;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CKeyOpeDispDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CKeyOpeDispDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	COperationManager*	m_pobjOperationManager;
	CStaticFilespec		m_wndStaticFileTitle;
	CEditorApp*			m_pApp;
	CAutoResizer		m_objAutoResizer;
	HICON				m_hIcon;
	HICON				m_hIconS;
};

#endif // !defined(KeyOpeDispDlg_H)


/////////////////////////////////////////////////////////////////////////////
// CMarkLineListDlg �_�C�A���O

#if !defined(MARKLINELISTDLG_H)		//�Q�d�C���N���[�h�h�~
#define MARKLINELISTDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CEditorView;
class CEditorDoc;
class CAutoResizer;

class CMarkLineListDlg : public CDialog
{
public:
	CMarkLineListDlg(CWnd* pParent = NULL);		// �W���̃R���X�g���N�^
	void Init(CEditorView*,CEditorDoc*);		// ������

	long	m_lSelectedLineNo;

	//{{AFX_DATA(CMarkLineListDlg)
	enum { IDD = IDD_MARKLINELIST };
	CListBox	m_objMarkLineList;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMarkLineListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMarkLineListDlg)
	afx_msg void OnDblclkListMarkline();
	afx_msg void OnJumpbtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CEditorApp*		m_pApp;
	CEditorView*	m_pobjEditorView;
	CEditorDoc*		m_pobjEditorDoc;
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(MARKLINELISTDLG_H)


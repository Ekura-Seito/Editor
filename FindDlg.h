// FindDlg.h : ヘッダー ファイル
//

#if !defined(FINDDLG_H)
#define FINDDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMyComboBoxEx;
class CEditorApp;
class CEditorView;
class CMainFrame;
class CAutoResizer;

class CFindDlg : public CDialog
{
public:
	CFindDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	//{{AFX_DATA(CFindDlg)
	enum { IDD = IDD_FIND };
	CMyComboBoxEx	m_objComboFindString;
	CString		m_objFindString;
	BOOL		m_bIsMatchWholeWordFind;
	BOOL		m_bIsMatchCaseFind;
	int			m_bIsSearchDownFind;
	BOOL		m_bIsUseRegularExpression;
	//}}AFX_DATA

	CMainFrame* 	m_pobjMainFrame;
	CEditorView*	m_pobjEditorView;
	CString			m_objPastFindString[20];

	//{{AFX_VIRTUAL(CFindDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CFindDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnFind();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCheckRegex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CEditorApp*		m_pApp;
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(FINDDLG_H)


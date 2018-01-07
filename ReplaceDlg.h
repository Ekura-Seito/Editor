// ReplaceDlg.h : ヘッダー ファイル
//

#if !defined(REPLACEDLG_H)
#define REPLACEDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMyComboBoxEx;
class CEditorApp;
class CEditorView;
class CMainFrame;
class CAutoResizer;

class CReplaceDlg : public CDialog
{
public:
	CReplaceDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	//{{AFX_DATA(CReplaceDlg)
	enum { IDD = IDD_REPLACE };
	CMyComboBoxEx	m_objComboFindString;
	CString		m_objFindString;
	CMyComboBoxEx	m_objComboReplaceString;
	CString		m_objReplaceString;
	BOOL		m_bIsMatchCaseReplace;
	BOOL		m_bIsMatchWholeWordReplace;
	BOOL		m_bIsUseRegularExpression;
	//}}AFX_DATA

	CMainFrame* 	m_pobjMainFrame;
	CEditorView*	m_pobjEditorView;
	CString			m_objPastFindString[20];
	CString			m_objPastReplaceString[20];

	//{{AFX_VIRTUAL(CReplaceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CReplaceDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnFind();
	afx_msg void OnReplaceall();
	afx_msg void OnReplacenext();
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

#endif // !defined(REPLACEDLG_H)


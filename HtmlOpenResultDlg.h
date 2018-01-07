// HtmlOpenResultDlg.h : ヘッダー ファイル
//

#if !defined(HTMLOPENRESULTDLG_H)
#define HTMLOPENRESULTDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CAutoResizer;

class CHtmlOpenResultDlg : public CDialog
{
public:
	CHtmlOpenResultDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	//{{AFX_DATA(CHtmlOpenResultDlg)
	enum { IDD = IDD_HTMLOPENRESULT };
	CString	m_objStatus;
	CString	m_objHeader;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CHtmlOpenResultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CHtmlOpenResultDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CEditorApp*		m_pApp;
	HICON			m_hIcon;
	HICON			m_hIconS;
	CAutoResizer	m_objAutoResizer;
};

#endif // !defined(HTMLOPENRESULTDLG_H)


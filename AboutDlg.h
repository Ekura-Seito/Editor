/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ

#if !defined(ABOUTDLG_H)		//２重インクルード防止
#define ABOUTDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CHyperLink;
class CLabel;

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStatic	m_objAppIcon;
	CHyperLink	m_hyperlink;
	CLabel	m_objSoftName;
	CLabel	m_objCredit;
	CLabel	m_Kaisya;
	CLabel	m_User;
	//}}AFX_DATA

	CString m_objKaisya;
	CString m_objUser;

	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // // DDX/DDV のサポート
	//}}AFX_VIRTUAL

protected:
	virtual BOOL OnInitDialog();
	//{{AFX_MSG(CAboutDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*		m_pApp;
	CBrush			m_brush;
	HICON			m_hIcon;
	HICON			m_hIconLaugh;
};

#endif // !defined(ABOUTDLG_H)


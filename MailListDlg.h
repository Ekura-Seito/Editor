// MailListDlg.h : ヘッダー ファイル
//

#if !defined(MAILLISTDLG_H)
#define MAILLISTDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CPop;
class CListCtrlEx;
class CAutoResizer;

class CMailListDlg : public CDialog
{
public:
	CMailListDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	CPop*	m_pPop;
	int		m_nMailCount;						// ＰＯＰサーバ内メール総数
	CString m_objPopPasswdStr;

	//{{AFX_DATA(CMailListDlg)
	enum { IDD = IDD_MAILLIST };
	CListCtrlEx	m_objListMail;
	CString	m_objPopServerStr;
	CString	m_objPopUserStr;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMailListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CMailListDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRecvbtn();
	afx_msg void OnDelbtn();
	afx_msg void OnRefreshbtn();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

protected:
	void	DispMailList();

private:
	CEditorApp*	m_pApp;
	CImageList	m_AttachImageList;
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(MAILLISTDLG_H)


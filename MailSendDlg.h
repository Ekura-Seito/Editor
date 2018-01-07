// MailSendDlg.h : ヘッダー ファイル
//

#if !defined(MAILSENDDLG_H)
#define MAILSENDDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CMyComboBoxEx;
class CAutoResizer;

class CMailSendDlg : public CDialog
{
public:
	CMailSendDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_DATA(CMailSendDlg)
	enum { IDD = IDD_MAILSEND };
	CMyComboBoxEx	m_objComboTo;
	CString		m_objTo;
	CString		m_objSubject;
	CString		m_objCc;
	CString		m_objBcc;
	CString		m_objReplyTo;
	CString		m_objReference;
	CListCtrl	m_objListAttachFiles;
	BOOL		m_bIsReqMsgDispsition;
	//}}AFX_DATA

	CString		m_objPastTo[20];
	CArray<CString,CString&>	m_objAttachFiles;
	CArray<CString,CString&>	m_objToArray;
	CArray<CString,CString&>	m_objCcArray;
	CArray<CString,CString&>	m_objBccArray;

	//{{AFX_VIRTUAL(CMailSendDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMailSendDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnFileattach();
	virtual void OnOK();
	afx_msg void OnFiledetach();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CToolTipCtrl m_tooltip;
	CEditorApp*		m_pApp;
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(MAILSENDDLG_H)


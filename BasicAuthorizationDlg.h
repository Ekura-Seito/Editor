// BasicAuthorizationDlg.h : ヘッダー ファイル
//

#if !defined(BASICAUTHORIZATIONDLG_H)
#define BASICAUTHORIZATIONDLG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEditorApp;
class CAutoResizer;

class CBasicAuthorizationDlg : public CDialog
{
public:
	CBasicAuthorizationDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	CString		m_objAuthSite;
	CString		m_objAuthRealm;
	CString		m_objAuthUser;
	CString		m_objAuthPasswd;

	//{{AFX_DATA(CBasicAuthorizationDlg)
	enum { IDD = IDD_BASICAUTH };
	CEdit		m_objAuthUserEdit;
	CEdit		m_objAuthPasswdEdit;
	BOOL		m_bIsSaveThisPassword;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CBasicAuthorizationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CBasicAuthorizationDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
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

#endif // !defined(BASICAUTHORIZATIONDLG_H)


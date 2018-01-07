// CheckExecPasswdDlg.h : CCheckExecPasswdDlg クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(CHECKEXECPASSWDDLG_H)		//２重インクルード防止
#define CHECKEXECPASSWDDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CCheckExecPasswdDlg : public CDialog
{
public:
	CCheckExecPasswdDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	BOOL	IsPasswdOK();						//パスワードＯＫチェック

	//{{AFX_DATA(CCheckExecPasswdDlg)
	enum { IDD = IDD_CHECKEXECPASSWD };
	CEdit	m_objExecPasswdEdit;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCheckExecPasswdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCheckExecPasswdDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL		m_bIsPasswdOK;
	CEditorApp*	m_pApp;						//アプリケーションクラスへのポインタ
	HICON		m_hIcon;
	HICON		m_hIconS;
};

#endif // !defined(CHECKEXECPASSWDDLG_H)


// SetLicenseDlg.h : CSetLicenseDlg クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(SETLICENSEDLG_H)		//２重インクルード防止
#define SETLICENSEDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CSetLicenseDlg : public CDialog
{
public:
	CSetLicenseDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	//{{AFX_DATA(CSetLicenseDlg)
	enum { IDD = IDD_LICENSE };
	CEdit	m_objKaisyaEdit;
	CEdit	m_objUserEdit;
	CEdit	m_objLicenseKeyEdit;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSetLicenseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSetLicenseDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;							// アプリケーションクラスへのポインタ
	HICON		m_hIcon;
	HICON		m_hIconS;
};

#endif // !defined(SETLICENSEDLG_H)


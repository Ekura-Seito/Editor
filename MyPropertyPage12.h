// MyPropertyPage12.h : ヘッダー ファイル
//

#if !defined(MYPROPERTYPAGE12_H)
#define MYPROPERTYPAGE12_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CMyPropertyPage12 : public CPropertyPage
{
	DECLARE_DYNCREATE(CMyPropertyPage12)

public:
	CMyPropertyPage12();
	~CMyPropertyPage12();

	BOOL IsChanged();									// 変更発生調査
	void ClearChangeFlag();								// 

	CString		m_objProxyIPAddress;
	CString		m_objDialupEntry;
	CString		m_objRasUserName;
	CString		m_objRasPasswd;

	//{{AFX_DATA(CMyPropertyPage12)
	enum { IDD = IDD_PROPPAGE12 };
	BOOL		m_bIsUseProxy;
	CEdit		m_objEditProxyIPAddress;
	BOOL		m_bIsDialup;
	CComboBox	m_objComboDialupEntry;
	CEdit		m_objEditRasUserName;
	CEdit		m_objEditRasPasswd;
	BOOL		m_bIsAutoDisconnect;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMyPropertyPage12)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMyPropertyPage12)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckUseproxy();
	afx_msg void OnChangeEditProxyipaddress();
	afx_msg void OnCheckDialup();
	afx_msg void OnSelchangeComboDialupentry();
	afx_msg void OnChangeEditRasusername();
	afx_msg void OnChangeEditRaspasswd();
	afx_msg void OnCheckAutoDisconnect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void SetDialupEntryCombo();

private:
	CEditorApp*		m_pApp;
	BOOL			m_bIsChanged;								// 変更発生フラグ
};

#endif // !defined(MYPROPERTYPAGE12_H)


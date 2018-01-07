// MyPropertyPage9.h : ヘッダー ファイル
//

#if !defined(MYPROPERTYPAGE9_H)
#define MYPROPERTYPAGE9_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CMyPropertyPage9 : public CPropertyPage
{
	DECLARE_DYNCREATE(CMyPropertyPage9)

public:
	CMyPropertyPage9();
	~CMyPropertyPage9();

	BOOL IsChanged();									// 変更発生調査
	void ClearChangeFlag();								// 
	void ChangesToRegistry();

	//{{AFX_DATA(CMyPropertyPage9)
	enum { IDD = IDD_PROPPAGE9 };
	CComboBox	m_objComboAccount;
	CEdit	m_objEditMailAddress;
	CEdit	m_objEditSmtpServer;
	CEdit	m_objEditPopServer;
	CEdit	m_objEditPopUserName;
	CEdit	m_objEditPopPasswd;
	BOOL	m_bIsDialup;
	CComboBox	m_objComboDialupEntry;
	CEdit	m_objEditRasUserName;
	CEdit	m_objEditRasPasswd;
	BOOL	m_bAutoDisconnect;
	int		m_nEncodeType;
	CEdit	m_objEditSignature;
	BOOL	m_bIsSendDate;
	BOOL	m_bIsSendMessageID;
	BOOL	m_bIsAutoSetSubject;
	int		m_nAuthorizationType;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMyPropertyPage9)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMyPropertyPage9)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditMailaddress();
	afx_msg void OnChangeEditSmtpserver();
	afx_msg void OnChangeEditPopserver();
	afx_msg void OnChangeEditPopusername();
	afx_msg void OnChangeEditPoppasswd();
	afx_msg void OnCheckDialup();
	afx_msg void OnSelchangeComboDialupentry();
	afx_msg void OnChangeEditRasusername();
	afx_msg void OnChangeEditRaspasswd();
	afx_msg void OnCheckAutoDisconnect();
	afx_msg void OnRadioEncodeBase64();
	afx_msg void OnRadioEncodeQuoted();
	afx_msg void OnChangeEditSignature();
	afx_msg void OnRadioEncodeUuencode();
	afx_msg void OnCheckSenddate();
	afx_msg void OnCheckSendmsgid();
	afx_msg void OnCheckSubjectautoset();
	afx_msg void OnRadioAuthStandard();
	afx_msg void OnRadioAuthApop();
	afx_msg void OnSelchangeComboAccount();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void SetDialupEntryCombo();

private:
	CEditorApp*		m_pApp;
	BOOL			m_bIsChanged;								// 変更発生フラグ
	int				m_nCurrentAccountNo;						// 現在のアカウント番号
	MAIL_SETTINGS	m_stMailSettings[MAIL_ACCOUNT_MAX];			// アカウント別設定情報保持用構造体
};

#endif // !defined(MYPROPERTYPAGE9_H)


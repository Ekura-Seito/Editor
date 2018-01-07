// MyPropertyPage5.h : CMyPropertyPage5 クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MYPROPERTYPAGE5_H)		//２重インクルード防止
#define MYPROPERTYPAGE5_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CMyPropertyPage5 : public CPropertyPage
{
	DECLARE_DYNCREATE(CMyPropertyPage5)

public:
	CMyPropertyPage5();
	~CMyPropertyPage5();

	BOOL IsChanged();									// 変更発生調査
	void ClearChangeFlag();								// 

	//{{AFX_DATA(CMyPropertyPage5)
	enum { IDD = IDD_PROPPAGE5 };
	BOOL	m_bPrintLineNo;
	BOOL	m_bPrint2Dangumi;
	int		m_nPrintOrientation;
	CString	m_objStrHeaderFormat;
	int		m_nHeaderLayout;
	CString	m_objStrFooterFormat;
	int		m_nFooterLayout;
	int		m_nYohakuHidari;
	int		m_nYohakuMigi;
	int		m_nYohakuSita;
	int		m_nYohakuUe;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMyPropertyPage5)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMyPropertyPage5)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckLineno();
	afx_msg void OnButtonLayouthelp();
	afx_msg void OnRadioFooterLeft();
	afx_msg void OnRadioFooterCenter();
	afx_msg void OnRadioFooterRight();
	afx_msg void OnRadioHeaderLeft();
	afx_msg void OnRadioHeaderCenter();
	afx_msg void OnRadioHeaderRight();
	afx_msg void OnChangeEditFooterformat();
	afx_msg void OnChangeEditHeaderformat();
	afx_msg void OnCheck2dangumi();
	afx_msg void OnRadioPrintorientationTate();
	afx_msg void OnRadioPrintorientationYoko();
	afx_msg void OnChangeEditYohakuHidari();
	afx_msg void OnChangeEditYohakuMigi();
	afx_msg void OnChangeEditYohakuSita();
	afx_msg void OnChangeEditYohakuUe();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;
	BOOL	m_bIsChanged;								// 変更発生フラグ
};

#endif // !defined(MYPROPERTYPAGE5_H)



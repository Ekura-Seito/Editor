// MyPropertyPage4.h : CMyPropertyPage4 クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MYPROPERTYPAGE4_H)		//２重インクルード防止
#define MYPROPERTYPAGE4_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CMyPropertyPage4 : public CPropertyPage
{
	DECLARE_DYNCREATE(CMyPropertyPage4)

public:
	CMyPropertyPage4();
	~CMyPropertyPage4();

	BOOL IsChanged();									// 変更発生調査
	void ClearChangeFlag();								// 

	long	m_lSaveInterval;

	//{{AFX_DATA(CMyPropertyPage4)
	enum { IDD = IDD_PROPPAGE4 };
	CEdit	m_objSaveIntervalEdit;
	int		m_nKaigyoKind;
	BOOL	m_bIsAutoSave;
	BOOL	m_bDelEndSpacesOnSave;
	BOOL	m_bSpace2TabOnSave;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMyPropertyPage4)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMyPropertyPage4)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioCrlf();
	afx_msg void OnRadioLf();
	afx_msg void OnRadioCr();
	afx_msg void OnCheckAutosave();
	afx_msg void OnChangeEditSaveinterval();
	afx_msg void OnCheckDelendspacesonsave();
	afx_msg void OnCheckSpace2tab();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*		m_pApp;
	BOOL	m_bIsChanged;								// 変更発生フラグ
};

#endif // !defined(MYPROPERTYPAGE4_H)



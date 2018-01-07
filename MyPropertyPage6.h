// MyPropertyPage6.h : CMyPropertyPage6 クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MYPROPERTYPAGE6_H)		//２重インクルード防止
#define MYPROPERTYPAGE6_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMyPropertyPage6 : public CPropertyPage
{
	DECLARE_DYNCREATE(CMyPropertyPage6)

public:
	CMyPropertyPage6();
	~CMyPropertyPage6();

	BOOL IsChanged();									// 変更発生調査
	void ClearChangeFlag();								// 

	//{{AFX_DATA(CMyPropertyPage6)
	enum { IDD = IDD_PROPPAGE6 };
	int		m_nCaretType;
	UINT	m_unCaretBlinkTime;
	int		m_nCaretBehave;
	int		m_nCaretMoveType;
	int		m_nMultValueOfCaretAccelarete;
	int		m_nSetCaretToEndOnPaste;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMyPropertyPage6)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMyPropertyPage6)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioCaretIbeam();
	afx_msg void OnRadioCaretSquare();
	afx_msg void OnChangeEditBlinktime();
	afx_msg void OnRadioCaretmove();
	afx_msg void OnRadioCaretstay();
	afx_msg void OnRadioWordtype();
	afx_msg void OnRadioNotepadtype();
	afx_msg void OnRadioAccel1();
	afx_msg void OnRadioAccel2();
	afx_msg void OnRadioAccel3();
	afx_msg void OnRadioAccel4();
	afx_msg void OnRadioAccel5();
	afx_msg void OnRadioPastetop();
	afx_msg void OnRadioPasteend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL	m_bIsChanged;								//変更発生フラグ
};

#endif // !defined(MYPROPERTYPAGE6_H)


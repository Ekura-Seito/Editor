#if !defined(AFX_COMBOBOXEX2_H__115F422E_5CD5_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_COMBOBOXEX2_H__115F422E_5CD5_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// MyComboBoxEx2.h : header file
//
// Copyright (c) Chris Maunder 1997.
// Please feel free to use and distribute.


/////////////////////////////////////////////////////////////////////////////
// CMyComboBoxEx2 window

class CMyComboBoxEx2 : public CComboBox
{
// Construction
public:
	CMyComboBoxEx2();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyComboBoxEx2)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyComboBoxEx2();

	BOOL m_bAutoComplete;

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyComboBoxEx2)
	afx_msg void OnEditUpdate();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOBOXEX2_H__115F422E_5CD5_11D1_ABBA_00A0243D1382__INCLUDED_)



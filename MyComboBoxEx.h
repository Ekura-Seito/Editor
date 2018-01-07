// MyComboBoxEx.h : header file
//
#if !defined(AFX_COMBOBOXEX_H__115F422E_5CD5_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_COMBOBOXEX_H__115F422E_5CD5_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMyComboBoxEx : public CComboBox
{
public:
	CMyComboBoxEx();
	virtual ~CMyComboBoxEx();

	void SetFlagCompareNoCase(BOOL bFlg=FALSE);

public:
	//{{AFX_VIRTUAL(CMyComboBoxEx)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMyComboBoxEx)
	afx_msg void OnEditUpdate();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bAutoComplete;
	BOOL m_bIsCompareNoCase;
};

#endif // !defined(AFX_COMBOBOXEX_H__115F422E_5CD5_11D1_ABBA_00A0243D1382__INCLUDED_)


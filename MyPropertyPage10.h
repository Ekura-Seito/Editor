// MyPropertyPage10.h : ヘッダー ファイル
//

#if !defined(MYPROPERTYPAGE10_H)
#define MYPROPERTYPAGE10_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CMyPropertyPage10 : public CPropertyPage
{
	DECLARE_DYNCREATE(CMyPropertyPage10)

public:
	CMyPropertyPage10();
	~CMyPropertyPage10();

	BOOL IsChanged();									// 変更発生調査
	void ClearChangeFlag();								// 


	//{{AFX_DATA(CMyPropertyPage10)
	enum { IDD = IDD_PROPPAGE10 };
	CButton	m_objAssignBtn;
	CButton	m_objKaijyoBtn;
	CListBox	m_objCmdList;
	CComboBox	m_objFuncCombo;
	CListBox	m_objKeyList;
	BOOL	m_bIsCtrl;
	BOOL	m_bIsShift;
	CString	m_objMenuExplain;
	BOOL	m_bIsAlt;
	CString	m_objCmdID;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMyPropertyPage10)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMyPropertyPage10)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckCtrl();
	afx_msg void OnCheckShift();
	afx_msg void OnSelchangeComboFunc();
	afx_msg void OnSelchangeListCmd();
	afx_msg void OnSelchangeListKey();
	afx_msg void OnButtonAssign();
	afx_msg void OnButtonKaijyo();
	afx_msg void OnButtonList();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonWrite();
	afx_msg void OnButtonRead();
	afx_msg void OnCheckAlt();
	afx_msg void OnButtonHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;									//アプリケーションクラスへのポインタ
	BOOL		m_bIsChanged;							//変更発生フラグ
};

#endif // !defined(MYPROPERTYPAGE10_H)


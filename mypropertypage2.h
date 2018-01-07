// MyPropertyPage2.h : CMyPropertyPage2 クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MYPROPERTYPAGE2_H)		//２重インクルード防止
#define MYPROPERTYPAGE2_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CMyPropertyPage2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CMyPropertyPage2)

public:
	CMyPropertyPage2();
	~CMyPropertyPage2();

	BOOL IsChanged();						// 変更発生調査
	void ClearChangeFlag();					// 

	CString		m_objFileExt;				// ファイル拡張子
	LOGFONT		m_stStatusBarLogFont;		// ｽﾃｰﾀｽﾊﾞｰのLOGFONT構造体
	long		m_lDispFontPoint;			// 表示フォントのポイント
	CString		m_objDispFontName;			// 表示フォント名
	long		m_lPrintFontPoint;			// 印字フォントのポイント
	CString		m_objPrintFontName;			// 印字フォント名

	//{{AFX_DATA(CMyPropertyPage2)
	enum { IDD = IDD_PROPPAGE2 };
	CComboBox	m_objDispFontNameCombo;
	CComboBox	m_objDispFontSizeCombo;
	CComboBox	m_objPrintFontNameCombo;
	CComboBox	m_objPrintFontSizeCombo;
	CComboBox	m_objFileExtCombo;
	int		m_nTabSize;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMyPropertyPage2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	friend int CALLBACK EnumFontFamiliesCallBack(ENUMLOGFONT*,NEWTEXTMETRIC*,int,LPARAM);

	//{{AFX_MSG(CMyPropertyPage2)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonStsbarfont();
	afx_msg void OnRadio4tab();
	afx_msg void OnRadio8tab();
	afx_msg void OnRadio2tab();
	afx_msg void OnSelchangeComboDispfontname();
	afx_msg void OnSelchangeComboDispfontsize();
	afx_msg void OnSelchangeComboFileext();
	afx_msg void OnSelchangeComboPrintfontname();
	afx_msg void OnSelchangeComboPrintfontsize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp* m_pApp;
	BOOL	m_bIsChanged;								//変更発生フラグ
};

#endif // !defined(MYPROPERTYPAGE2_H)


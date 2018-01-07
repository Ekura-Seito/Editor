// GrepDlg.h : CGrepDlg クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(GREPDLG_H)		//２重インクルード防止
#define GREPDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CMyComboBoxEx;
class CAutoResizer;

class CGrepDlg : public CDialog
{
public:
	CGrepDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_DATA(CGrepDlg)
	enum { IDD = IDD_GREP };
	CMyComboBoxEx	m_objComboGrepString;
	CString	m_objGrepString;
	CEdit	m_objEditFolder;
	CEdit	m_objEditFileKind;
	BOOL	m_bIsMatchCaseGrep;
	BOOL	m_bIsMatchWholeWordGrep;
	BOOL	m_bIsSearchSubFolderGrep;
	BOOL	m_bIsUseRegularExpression;
	//}}AFX_DATA

	CString m_objPastGrepString[20];		// 過去にＧＲＥＰした文字列
	CString	m_objFolder;					// ＧＲＥＰ実行フォルダ
	CString	m_objFileKind;					// 検索対象のファイル種別

	//{{AFX_VIRTUAL(CGrepDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CGrepDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	virtual void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCheckRegex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CToolTipCtrl 	m_tooltip;
	CEditorApp*		m_pApp;						// アプリケーションクラスへのポインタ
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(GREPDLG_H)


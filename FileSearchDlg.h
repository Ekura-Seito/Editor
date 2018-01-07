// FileSearchDlg.h : CFileSearchDlg クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(FILESEARCHDLG_H)		//２重インクルード防止
#define FILESEARCHDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CAutoResizer;

class CFileSearchDlg : public CDialog
{
public:
	CFileSearchDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_DATA(CFileSearchDlg)
	enum { IDD = IDD_FILESEARCH };
	CEdit	m_objEditFolder;
	CEdit	m_objEditFileKind;
	BOOL	m_bIsSearchSubFolder;
	//}}AFX_DATA

	CString	m_objFolder;					// ファイル検索実行フォルダ
	CString	m_objFileKind;					// 検索対象のファイル種別

	//{{AFX_VIRTUAL(CFileSearchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CFileSearchDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	virtual void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CToolTipCtrl 	m_tooltip;
	CEditorApp*		m_pApp;
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(FILESEARCHDLG_H)


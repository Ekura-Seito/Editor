// DirCompareDlg.h : ヘッダー ファイル
//

#if !defined(DIRCOMPAREDLG_H)		//２重インクルード防止
#define DIRCOMPAREDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CAutoResizer;
class CMyComboBoxEx;

class CDirCompareDlg : public CDialog
{
public:
	CDirCompareDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_DATA(CDirCompareDlg)
	enum { IDD = IDD_DIRCOMPARE };
	CString	m_objFolder1;					// ディレクトリ比較実行フォルダその１
	CString	m_objFolder2;					// ディレクトリ比較実行フォルダその２
	CMyComboBoxEx	m_objComboFolder1;
	CMyComboBoxEx	m_objComboFolder2;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDirCompareDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDirCompareDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBrowse1();
	afx_msg void OnBrowse2();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CToolTipCtrl m_tooltip;
	CEditorApp*		m_pApp;
	CAutoResizer	m_objAutoResizer;
	CString			m_objPastFolder1[20];
	CString			m_objPastFolder2[20];
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif //!defined(DIRCOMPAREDLG_H)


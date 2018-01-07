// KeyBindListDlg.h : ヘッダー ファイル
//

#if !defined(KEYBINDLISTDLG_H)
#define KEYBINDLISTDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CListCtrlEx;
class CAutoResizer;

class CKeyBindListDlg : public CDialog
{
public:
	CKeyBindListDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	//{{AFX_DATA(CKeyBindListDlg)
	enum { IDD = IDD_KEYBINDLIST };
	CListCtrlEx	m_objKeyBindList;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CKeyBindListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CKeyBindListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CEditorApp*		m_pApp;
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(KEYBINDLISTDLG_H)


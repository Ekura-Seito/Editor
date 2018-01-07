// KeyOpeDispDlg.h: CKeyOpeDispDlg クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(KeyOpeDispDlg_H)		//２重インクルード防止
#define KeyOpeDispDlg_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class COperationManager;
class CStaticFilespec;
class CListCtrlEx;
class CEditorApp;
class CAutoResizer;

class CKeyOpeDispDlg : public CDialog
{
public:
	CKeyOpeDispDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	void Initialize(COperationManager*);	// 初期化

	//{{AFX_DATA(CKeyOpeDispDlg)
	enum { IDD = IDD_KEYOPE_DISP };
	CEdit	m_objOperatinNameEdit;
	CListCtrlEx	m_objKeyOpeList;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CKeyOpeDispDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CKeyOpeDispDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	COperationManager*	m_pobjOperationManager;
	CStaticFilespec		m_wndStaticFileTitle;
	CEditorApp*			m_pApp;
	CAutoResizer		m_objAutoResizer;
	HICON				m_hIcon;
	HICON				m_hIconS;
};

#endif // !defined(KeyOpeDispDlg_H)


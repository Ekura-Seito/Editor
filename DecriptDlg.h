// DecriptDlg.h : CDecriptDlg クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(DECRIPTDLG_H)		//２重インクルード防止
#define DECRIPTDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CAutoResizer;

class CDecriptDlg : public CDialog
{
public:
	CDecriptDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_DATA(CDecriptDlg)
	enum { IDD = IDD_DECRIPT };
	CEdit	m_objEditKey;
	CEdit	m_objEditFolder;
	CEdit	m_objEditFileKind;
	BOOL	m_bIsOutOtherFile;
	BOOL	m_bIsSearchSubFolder;
	//}}AFX_DATA

	CString m_objKey;						// 復号化キー
	CString	m_objFolder;					// 復号化実行フォルダ
	CString	m_objFileKind;					// 復号化対象のファイル種別

	//{{AFX_VIRTUAL(CDecriptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDecriptDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	virtual void OnOK();
	afx_msg void OnMyHelp();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CToolTipCtrl	m_tooltip;
	CEditorApp*		m_pApp;						// アプリケーションクラスへのポインタ
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(DECRIPTDLG_H)


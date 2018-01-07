// EncriptDlg.h : CEncriptDlg クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(ENCRIPTDLG_H)		//２重インクルード防止
#define ENCRIPTDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CAutoResizer;

class CEncriptDlg : public CDialog
{
public:
	CEncriptDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_DATA(CEncriptDlg)
	enum { IDD = IDD_ENCRIPT };
	CEdit	m_objEditKey;
	CEdit	m_objEditFileKind;
	CEdit	m_objEditFolder;
	BOOL	m_bIsOutOtherFile;
	BOOL	m_bIsSearchSubFolder;
	//}}AFX_DATA

	CString m_objKey;						// 暗号化キー
	CString	m_objFolder;					// 暗号化実行フォルダ
	CString	m_objFileKind;					// 暗号化対象のファイル種別

	//{{AFX_VIRTUAL(CEncriptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CEncriptDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBrowse();
	afx_msg void OnMyHelp();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CToolTipCtrl m_tooltip;
	CEditorApp*	m_pApp;						// アプリケーションクラスへのポインタ
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(ENCRIPTDLG_H)


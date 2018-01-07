// FileCopyDlg.h : CFileCopyDlg クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(FILECOPYDLG_H)		//２重インクルード防止
#define FILECOPYDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;
class CAutoResizer;

class CFileCopyDlg : public CDialog
{
public:
	CFileCopyDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_DATA(CFileCopyDlg)
	enum { IDD = IDD_FILECOPY };
	CEdit	m_objEditFolder;
	BOOL	m_bRenameOnCollision;
	//}}AFX_DATA

	CString m_objFile;						// コピー元ファイル名
	CString	m_objFolder;					// コピー先フォルダ

	//{{AFX_VIRTUAL(CFileCopyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CFileCopyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	virtual void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CToolTipCtrl m_tooltip;
	CEditorApp*		m_pApp;
	CStaticFilespec m_wndStaticFileName;
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif //!defined(FILECOPYDLG_H)


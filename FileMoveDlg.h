// FileMoveDlg.h : CFileMoveDlg クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(FILEMOVEDLG_H)		//２重インクルード防止
#define FILEMOVEDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;
class CAutoResizer;

class CFileMoveDlg : public CDialog
{
public:
	CFileMoveDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_DATA(CFileMoveDlg)
	enum { IDD = IDD_FILEMOVE };
	CEdit	m_objEditFolder;
	//}}AFX_DATA

	char		m_szFromPath[_MAX_PATH];				//移動元パス
	CString		m_objFolder;							//移動先ディレクトリのデフォルト
	char		m_szToPath[_MAX_PATH];					//確定した移動先ファイルパス

	//{{AFX_VIRTUAL(CFileMoveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CFileMoveDlg)
	afx_msg void OnBrowse();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CToolTipCtrl m_tooltip;
	CEditorApp*	m_pApp;
	char		m_szFromDir[_MAX_PATH];
	char		m_szFromName[_MAX_FNAME];
	CStaticFilespec   m_wndStaticFileName;
	CAutoResizer	m_objAutoResizer;
	HICON		m_hIcon;
	HICON		m_hIconS;
};

#endif //!defined(FILEMOVEDLG_H)


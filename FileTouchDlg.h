// FileTouchDlg.h : CFileTouchDlg クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(FILETOUCHDLG_H)		//２重インクルード防止
#define FILETOUCHDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDateMaskEdit;
class CTimeMaskEdit;
class CEditorApp;
class CAutoResizer;

class CFileTouchDlg : public CDialog
{
public:
	CFileTouchDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_DATA(CFileTouchDlg)
	enum { IDD = IDD_FILETOUCH };
	CTimeMaskEdit	m_objEditTime;
	CDateMaskEdit	m_objEditDate;
	CEdit	m_objEditFileKind;
	CEdit	m_objEditFolder;
	BOOL	m_bIsSearchSubFolder;
	//}}AFX_DATA

	CString	m_objFolder;					// 実行フォルダ
	CString	m_objFileKind;					// 対象のファイル種別
	CTime m_mtime;							// ファイルの最終更新日時

	//{{AFX_VIRTUAL(CFileTouchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CFileTouchDlg)
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

#endif //!defined(FILETOUCHDLG_H)


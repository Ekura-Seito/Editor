// FilePropertyDlg.h : CFilePropertyDlg クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(FILEPROPERTYDLG_H)		//２重インクルード防止
#define FILEPROPERTYDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;
class CAutoResizer;

class CFilePropertyDlg : public CDialog
{
public:
	CFilePropertyDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	CString m_objTitle;

	//{{AFX_DATA(CFilePropertyDlg)
	enum { IDD = IDD_FILEPROPERTY };
	CString	m_objFileName;
	CString	m_objCTime;
	CString	m_objMTime;
	CString	m_objATime;
	CString	m_objSize;
	CString	m_objAttribute;
	CString	m_objKaigyo;
	CString	m_objCode;
	CString	m_objLineCnt;
	CString	m_objTabSize;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CFilePropertyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CFilePropertyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CEditorApp*		m_pApp;
	CStaticFilespec m_wndStaticFileTitle;
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(FILEPROPERTYDLG_H)


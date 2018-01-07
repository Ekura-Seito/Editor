// FileCompareResultDlg.h : CFileCompareResultDlg クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(FileCompareResultDLG_H)		//２重インクルード防止
#define FileCompareResultDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CFileCompareResultDlg : public CDialog
{
public:
	CFileCompareResultDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	CString m_objFile1Name;					// 第一ファイル名
	CString m_objFile2Name;					// 第二ファイル名

	//{{AFX_DATA(CFileCompareResultDlg)
	enum { IDD = IDD_FILECOMPARERESULT };
	CString	m_objAddLineCountStr;
	CString	m_objDelLineCountStr;
	CString	m_objStatusStr;
	CString	m_objTimeStr;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CFileCompareResultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CFileCompareResultDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*		m_pApp;
	CStaticFilespec   m_wndStaticFileName1;
	CStaticFilespec   m_wndStaticFileName2;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(FileCompareResultDLG_H)


// DirCompareResultDlg.h : CDirCompareResultDlg クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(DirCompareResultDLG_H)		//２重インクルード防止
#define DirCompareResultDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CDirCompareResultDlg : public CDialog
{
public:
	CDirCompareResultDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	CString m_objDir1Name;					// 第一ディレクトリ名
	CString m_objDir2Name;					// 第二ディレクトリ名
	long	m_lCompareFileCount;
	long	m_lAddFileCount;
	long	m_lDelFileCount;
	long	m_lDiffFileCount;
	long	m_lTime;

	//{{AFX_DATA(CDirCompareResultDlg)
	enum { IDD = IDD_DIRCOMPARERESULT };
	CString	m_objStatusStr;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDirCompareResultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDirCompareResultDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*		m_pApp;
	CStaticFilespec   m_wndStaticDirName1;
	CStaticFilespec   m_wndStaticDirName2;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(DirCompareResultDLG_H)


// FileCompareStatusDlg.h : CFileCompareStatusDlg クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(FileCompareStatusDLG_H)		//２重インクルード防止
#define FileCompareStatusDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CFileCompareStatusDlg : public CDialog
{
public:
	CFileCompareStatusDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	BOOL IsCanceled();						// キャンセルチェック
	void SetStatus(char*);					// ステータスメッセージ設定
	void SetStatus(CString&);				// ステータスメッセージ設定

	CString m_objFile1Name;					// 第一ファイル名
	CString m_objFile2Name;					// 第二ファイル名

	//{{AFX_DATA(CFileCompareStatusDlg)
	enum { IDD = IDD_FILECOMPARE };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CFileCompareStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CFileCompareStatusDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*		m_pApp;
	BOOL			m_bIsCanceled;
	CStaticFilespec m_wndStaticFileName1;
	CStaticFilespec m_wndStaticFileName2;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(FileCompareStatusDLG_H)


// GrepStatusDlg.h : CGrepStatusDlg クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(GREPSTATUSDLG_H)		//２重インクルード防止
#define GREPSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CGrepStatusDlg : public CDialog
{
public:
	CGrepStatusDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	BOOL IsCanceled();						// キャンセルチェック
	void SetGrepStr(CString&);				// 検索文字列メッセージ設定
	void SetFileKind(CString&);				// ファイル種別メッセージ設定
	void SetFolder(CString&);				// 検索フォルダメッセージ設定
	void SetFile(CString&);					// 検索ファイルメッセージ設定
	void IncMatchCount();					// 該当件数カウントアップ処理
	long GetMatchCount();					// 該当件数取得

	//{{AFX_DATA(CGrepStatusDlg)
	enum { IDD = IDD_GREPSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CGrepStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CGrepStatusDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;
	BOOL	m_bIsCanceled;
	long	m_lMatchCount;
	CStaticFilespec   m_wndStaticFolder;
	HICON	m_hIcon;
	HICON	m_hIconS;
};

#endif // !defined(GREPSTATUSDLG_H)


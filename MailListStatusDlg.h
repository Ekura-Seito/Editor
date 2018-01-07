// MailListStatusDlg.h : ヘッダー ファイル
//

#if !defined(MAILLISTSTATUSDLG_H)
#define MAILLISTSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CMailListStatusDlg : public CDialog
{
public:
	CMailListStatusDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	BOOL IsCanceled();						// キャンセルチェック
	void SetStatus(char*);					// ステータスメッセージ設定
	void SetStatus(CString&);				// ステータスメッセージ設定

	//{{AFX_DATA(CMailListStatusDlg)
	enum { IDD = IDD_MAILLISTSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMailListStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMailListStatusDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL			m_bIsCanceled;
	CEditorApp*		m_pApp;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(MAILLISTSTATUSDLG_H)


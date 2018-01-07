// MailSendStatusDlg.h : ヘッダー ファイル
//

#if !defined(MAILSENDSTATUSDLG_H)
#define MAILSENDSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CMailSendStatusDlg : public CDialog
{
public:
	CMailSendStatusDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	BOOL IsCanceled();						// キャンセルチェック
	void SetStatus(char*);					// ステータスメッセージ設定
	void SetStatus(CString&);				// ステータスメッセージ設定

	//{{AFX_DATA(CMailSendStatusDlg)
	enum { IDD = IDD_MAILSENDSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMailSendStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMailSendStatusDlg)
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

#endif // !defined(MAILSENDSTATUSDLG_H)


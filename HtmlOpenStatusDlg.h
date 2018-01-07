// HtmlOpenStatusDlg.h : ヘッダー ファイル
//

#if !defined(HTMLOPENSTATUSDLG_H)
#define HTMLOPENSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CHtmlOpenStatusDlg : public CDialog
{
public:
	CHtmlOpenStatusDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	BOOL IsCanceled();						// キャンセルチェック
	void SetStatus(char*);					// ステータスメッセージ設定
	void SetStatus(CString&);				// ステータスメッセージ設定
	void SetRange(int,int);					//プログレスバー範囲設定
	void SetPos(int);						//プログレスバー位置設定


	//{{AFX_DATA(CHtmlOpenStatusDlg)
	enum { IDD = IDD_HTMLOPENSTATUS };
	CProgressCtrl	m_objProgressBar;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CHtmlOpenStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CHtmlOpenStatusDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL			m_bIsCanceled;
	CEditorApp*		m_pApp;
	HICON			m_hIcon;
	HICON			m_hIconS;
	int				m_nProgressLower;
	int				m_nProgressUpper;
};

#endif // !defined(HTMLOPENSTATUSDLG_H)


// WCntStatusDlg.h : CWCntStatusDlg クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(WCNTSTATUSDLG_H)		//２重インクルード防止
#define WCNTSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CWCntStatusDlg : public CDialog
{
public:
	CWCntStatusDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	BOOL IsCanceled();						// キャンセルチェック
	void SetFileKind(CString&);				// ファイル種別メッセージ設定
	void SetFolder(CString&);				// フォルダメッセージ設定
	void SetFile(CString&);					// カウント中ファイルメッセージ設定

	//{{AFX_DATA(CWCntStatusDlg)
	enum { IDD = IDD_WCNTSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CWCntStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWCntStatusDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;
	BOOL	m_bIsCanceled;
	CStaticFilespec   m_wndStaticFolder;
	HICON	m_hIcon;
	HICON	m_hIconS;
};

#endif //!defined(WCNTSTATUSDLG_H)


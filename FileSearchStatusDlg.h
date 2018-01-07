// FileSearchStatusDlg.h : CFileSearchStatusDlg クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(FILESEARCHSTATUSDLG_H)		//２重インクルード防止
#define FILESEARCHSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CFileSearchStatusDlg : public CDialog
{
public:
	CFileSearchStatusDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	BOOL IsCanceled();						// キャンセルチェック
	void SetFileKind(CString&);				// ファイル種別メッセージ設定
	void SetFolder(CString&);				// 検索フォルダメッセージ設定
	void SetFile(CString&);					// 検索ファイルメッセージ設定
	void IncMatchCount();					// 該当件数カウントアップ処理
	long GetMatchCount();					// 該当件数取得

	//{{AFX_DATA(CFileSearchStatusDlg)
	enum { IDD = IDD_FILESEARCHSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CFileSearchStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CFileSearchStatusDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;
	CStaticFilespec m_wndStaticFolder;
	BOOL	m_bIsCanceled;
	long	m_lMatchCount;
	HICON	m_hIcon;
	HICON	m_hIconS;
};

#endif // !defined(FILESEARCHSTATUSDLG_H)


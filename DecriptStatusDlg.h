// DecriptStatusDlg.h : CDecriptStatusDlg クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(DECRIPTSTATUSDLG_H)		//２重インクルード防止
#define DECRIPTSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CDecriptStatusDlg : public CDialog
{
public:
	CDecriptStatusDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	//{{AFX_DATA(CDecriptStatusDlg)
	enum { IDD = IDD_DECRIPTSTATUS };
	//}}AFX_DATA

	BOOL IsCanceled();						// キャンセルチェック
	void SetFileKind(CString&);				// ファイル種別メッセージ設定
	void SetFolder(CString&);				// フォルダメッセージ設定
	void SetFile(CString&);					// ファイルメッセージ設定
	void IncDecriptFileCount();				// 復号化ファイル件数カウントアップ処理
	long GetDecriptFileCount();				// 復号化ファイル件数取得

	//{{AFX_VIRTUAL(CDecriptStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDecriptStatusDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp* m_pApp;
	BOOL	m_bIsCanceled;
	long	m_lDecriptFileCount;
	CStaticFilespec   m_wndStaticFolder;
	HICON	m_hIcon;
	HICON	m_hIconS;
};

#endif // !defined(DECRIPTSTATUSDLG_H)


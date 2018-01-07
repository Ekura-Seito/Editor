// EncriptStatusDlg.h : CEncriptStatusDlg クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(ENCRIPTSTATUSDLG_H)		//２重インクルード防止
#define ENCRIPTSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CEncriptStatusDlg : public CDialog
{
public:
	CEncriptStatusDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	BOOL IsCanceled();						// キャンセルチェック
	void SetFileKind(CString&);				// ファイル種別メッセージ設定
	void SetFolder(CString&);				// フォルダメッセージ設定
	void SetFile(CString&);					// ファイルメッセージ設定
	void IncEncriptFileCount();				// 暗号化ファイル件数カウントアップ処理
	long GetEncriptFileCount();				// 暗号化ファイル件数取得

	//{{AFX_DATA(CEncriptStatusDlg)
	enum { IDD = IDD_ENCRIPTSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CEncriptStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CEncriptStatusDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;
	BOOL	m_bIsCanceled;
	long	m_lEncriptFileCount;
	CStaticFilespec   m_wndStaticFolder;
	HICON	m_hIcon;
	HICON	m_hIconS;
};

#endif // !defined(ENCRIPTSTATUSDLG_H)


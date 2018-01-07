// KaigyoConvStatusDlg.h : CKaigyoConvStatusDlg クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(KAIGYOCONVSTATUSDLG_H)
#define KAIGYOCONVSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CKaigyoConvStatusDlg : public CDialog
{
public:
	CKaigyoConvStatusDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	BOOL IsCanceled();						// キャンセルチェック
	void SetConvKindStr(CString&);			// コード変換種別メッセージ設定
	void SetFileKind(CString&);				// ファイル種別メッセージ設定
	void SetFolder(CString&);				// フォルダメッセージ設定
	void SetFile(CString&);					// ファイルメッセージ設定
	void IncConvFileCount();				// コード変換ファイル件数カウントアップ処理
	long GetConvFileCount();				// コード変換ファイル件数取得

	//{{AFX_DATA(CKaigyoConvStatusDlg)
	enum { IDD = IDD_KAIGYOCONVSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CKaigyoConvStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CKaigyoConvStatusDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*		m_pApp;
	BOOL	m_bIsCanceled;
	long	m_lConvFileCount;
	CStaticFilespec   m_wndStaticFolder;
	HICON	m_hIcon;
	HICON	m_hIconS;
};

#endif // !defined(KAIGYOCONVSTATUSDLG_H)


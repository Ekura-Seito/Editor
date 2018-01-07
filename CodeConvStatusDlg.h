// CodeConvStatusDlg.h : CCodeConvStatusDlg クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(CODECONVSTATUSDLG_H)		//２重インクルード防止
#define CODECONVSTATUSDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CCodeConvStatusDlg : public CDialog
{
public:
	CCodeConvStatusDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	BOOL IsCanceled();						// キャンセルチェック
	void SetConvKindStr(int);				// コード変換種別メッセージ設定
	void SetFileKind(CString&);				// ファイル種別メッセージ設定
	void SetFolder(CString&);				// フォルダメッセージ設定
	void SetFile(CString&);					// ファイルメッセージ設定
	void IncConvFileCount();				// コード変換ファイル件数カウントアップ処理
	long GetConvFileCount();				// コード変換ファイル件数取得

	//{{AFX_DATA(CCodeConvStatusDlg)
	enum { IDD = IDD_CONVSTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCodeConvStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCodeConvStatusDlg)
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

#endif // !defined(CODECONVSTATUSDLG_H)


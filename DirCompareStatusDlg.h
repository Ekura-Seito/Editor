// DirCompareStatusDlg.h : ヘッダー ファイル
//

#if !defined(DirCompareStatusDLG_H)		//２重インクルード防止
#define DirCompareStatusDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CDirCompareStatusDlg : public CDialog
{
public:
	CDirCompareStatusDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	BOOL IsCanceled();						// キャンセルチェック
	void SetDir1(CString&);					// 比較第一ディレクトリ設定
	void SetDir2(CString&);					// 比較第二ディレクトリ設定
	void SetStatus(char*);					// 比較状況設定
	void SetStatus(CString&);				// 比較状況設定
	void IncCompareFileCount();				// 比較ファイル数カウントアップ
	long GetCompareFileCount();				// 比較ファイル数取得
	void IncAddFileCount();					// 追加ファイル数カウントアップ
	long GetAddFileCount();					// 追加ファイル数取得
	void IncDelFileCount();					// 削除ファイル数カウントアップ
	long GetDelFileCount();					// 削除ファイル数取得
	void IncDiffFileCount();				// 相違ファイル数カウントアップ
	long GetDiffFileCount();				// 相違ファイル数取得

	//{{AFX_DATA(CDirCompareStatusDlg)
	enum { IDD = IDD_DIRCOMPARESTATUS };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDirCompareStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDirCompareStatusDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*		m_pApp;
	BOOL			m_bIsCanceled;
	CStaticFilespec m_wndStaticDirName1;
	CStaticFilespec m_wndStaticDirName2;
	long			m_lCompareFileCount;
	long			m_lAddFileCount;
	long			m_lDelFileCount;
	long			m_lDiffFileCount;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif //!defined(DirCompareStatusDLG_H)


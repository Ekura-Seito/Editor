// ClipBoardViewDlg.h : CClipBoardViewDlg クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(CLIPBOARDVIEWDLG_H)		//２重インクルード防止
#define CLIPBOARDVIEWDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CAutoResizer;

class CClipBoardViewDlg : public CDialog
{
public:
	CClipBoardViewDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	//{{AFX_DATA(CClipBoardViewDlg)
	enum { IDD = IDD_CLIPBOARDVIEW };
	CEdit	m_objClipContentsEdit;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CClipBoardViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	void ReadFromClipBoard(CString&);			// クリップボードから読む

	//{{AFX_MSG(CClipBoardViewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CEditorApp*		m_pApp;						//アプリケーションクラスへのポインタ
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(CLIPBOARDVIEWDLG_H)


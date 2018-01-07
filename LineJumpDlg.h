// LineJumpDlg.h : CLineJumpDlg クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(LINEJUMPDLG_H)		//２重インクルード防止
#define LINEJUMPDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CLineJumpDlg : public CDialog
{
public:
	CLineJumpDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

	long		m_lMaxNo;							//番号最大値

	//{{AFX_DATA(CLineJumpDlg)
	enum { IDD = IDD_DIALOG_JUMP };
	UINT	m_lLineNo;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CLineJumpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CLineJumpDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;
	HICON		m_hIcon;
	HICON		m_hIconS;
};

#endif // !defined(LINEJUMPDLG_H)


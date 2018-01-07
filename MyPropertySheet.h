// MyPropertySheet.h : ヘッダー ファイル
//
// このクラスはカスタム モーダル プロパティ シートを定義します 
// CMyPropertySheet.
 
#ifndef __MYPROPERTYSHEET_H__
#define __MYPROPERTYSHEET_H__

class CEditorApp;
class CMainFrame;
class CEditorView;
class CEditorDoc;
class CMyPropertyPage1;
class CMyPropertyPage2;
class CMyPropertyPage3;
class CMyPropertyPage4;
class CMyPropertyPage5;
class CMyPropertyPage6;
class CMyPropertyPage7;
class CMyPropertyPage8;
class CMyPropertyPage9;
class CMyPropertyPage10;
class CMyPropertyPage11;
class CMyPropertyPage12;

class CMyPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMyPropertySheet)

// 構築
public:
	CMyPropertySheet(CWnd* pParentWnd = NULL);

// アトリビュート
public:
	CMainFrame*			m_pobjMainFrame;
	CEditorDoc*			m_pobjEditorActiveDoc;
	CEditorView*		m_pobjEditorActiveView;
	CMyPropertyPage1	m_Page1;
	CMyPropertyPage2	m_Page2;
	CMyPropertyPage3	m_Page3;
	CMyPropertyPage4	m_Page4;
	CMyPropertyPage5	m_Page5;
	CMyPropertyPage6	m_Page6;
	CMyPropertyPage7	m_Page7;
	CMyPropertyPage8	m_Page8;
	CMyPropertyPage9	m_Page9;
	CMyPropertyPage10	m_Page10;
	CMyPropertyPage11	m_Page11;
	CMyPropertyPage12	m_Page12;

	//{{AFX_VIRTUAL(CMyPropertySheet)
	public:
	virtual int DoModal();
	//}}AFX_VIRTUAL

public:
	virtual ~CMyPropertySheet();

protected:
	//{{AFX_MSG(CMyPropertySheet)
	afx_msg void OnApplyNow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;						//アプリケーションクラスへのポインタ
};

#endif	//!defined(__MYPROPERTYSHEET_H__)


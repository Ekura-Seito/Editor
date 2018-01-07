// ListCtrlEx.h : ヘッダー ファイル
//

#if !defined(LISTCTRLEX_H)
#define LISTCTRLEX_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CListCtrlEx : public CListCtrl
{
public:
	CListCtrlEx();

	//{{AFX_VIRTUAL(CListCtrlEx)
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

public:
	virtual ~CListCtrlEx();

protected:
	//{{AFX_MSG(CListCtrlEx)
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
    enum EHighlight {HIGHLIGHT_NORMAL, HIGHLIGHT_ALLCOLUMNS, HIGHLIGHT_ROW};

protected:
	int  SetHighlightType(EHighlight);
	void RepaintSelectedItems();

private:
    int  m_nHighlight;              // Indicate type of selection highlighting
};

#endif // !defined(LISTCTRLEX_H)


#if !defined(TOOLBAREX_H)
#define TOOLBAREX_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CToolBarInfo
{
public:
	TBBUTTON		 m_tbButton;	// ボタン情報
	LPCTSTR			 m_btnText;		// ボタンテキスト
};

class CToolBarEx : public CToolBar
{
	DECLARE_DYNAMIC(CToolBarEx)

public:
	CToolBarEx();
	virtual ~CToolBarEx();

	void	SetFlatLook( BOOL bFlat = TRUE );
	BOOL	IsFlatLook() const;
	void	GetSizes( CSize & szBtn, CSize & szImg ) const;
//	CWnd*	GetParentFrame() const;
	BOOL	HasButtonText() const;

	//{{AFX_VIRTUAL(CToolBarEx)
	//}}AFX_VIRTUAL

protected:
	virtual void OnUpdateCmdUI( CFrameWnd* pTarget, BOOL bDisableIfNoHndler );
	//{{AFX_MSG(CToolBarEx)
	afx_msg void OnPaint();
	afx_msg void OnSysColorChange();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT		OnSetButtonSize(WPARAM, LPARAM);
	afx_msg LRESULT		OnSetBitmapSize(WPARAM, LPARAM);
	afx_msg void OnToolBarQueryDelete(NMHDR*, LRESULT*);
	afx_msg void OnToolBarQueryInsert(NMHDR*, LRESULT*);
	afx_msg void OnToolBarChange(NMHDR*, LRESULT*);
	afx_msg void OnToolBarBeginDrag(NMHDR*, LRESULT*);
	afx_msg void OnToolBarEndDrag(NMHDR*, LRESULT*);
	afx_msg void OnToolBarBeginAdjust(NMHDR*, LRESULT*);
	afx_msg void OnToolBarCustomHelp(NMHDR*, LRESULT*);
	afx_msg void OnToolBarEndAdjust(NMHDR*, LRESULT*);
	afx_msg void OnToolBarGetButtonInfo(NMHDR*, LRESULT*);
	afx_msg void OnToolBarReset(NMHDR*, LRESULT*);
	DECLARE_MESSAGE_MAP()

private:
	void	CalculateOffset();
	void	DrawDisabledButton( CDC &, const CRect & ) const;
	void	DrawSeparator( CDC &, CRect & ) const;
	void	DrawGripper( CDC & ) const;
	HIMAGELIST	GetImageList();

private:
	CEditorApp* m_pApp;
	BOOL		m_bFlatLook;
	CSize		m_sizeOffset;		// real starting point of the image
	COLORREF	m_clrBtnFace;
	COLORREF	m_clrBtnHilight;
	COLORREF	m_clrBtnShadow;
	COLORREF	m_clrBtnLight;
	int			m_nLastBtn;			// index of last formed button
	UINT		m_uTimerEvent;
	CUIntArray	m_Styles;
	HWND		m_hwndParent;		// "real" parent (even in floating mode)
	BOOL		m_bDeleteImgList;	// remember the way we've built the image list
	CFont		m_GuiFont;


////////////////////////////////////////
// for Customize Toolbar
public:
	void Customize();
	BOOL Create(CWnd*,
			DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP,
			UINT nID = AFX_IDW_TOOLBAR,
			CToolBarInfo *tbInfo = NULL,
			CString regSubKey = "",
			CString regValue = "",
			HKEY regKey = HKEY_CURRENT_USER);
	BOOL LoadToolBar(UINT, BOOL restore = TRUE);
	void SaveState();
	void RestoreState();
	int	 NButtons();			// number of buttons on toolbar

protected:
	CToolBarInfo	*m_pToolBarInfo;		// table of buttons and text
	int				 m_nButtons;			// number of buttons on toolbar
	HKEY			 registryKey;		// key where toolbar information is kept
	CString			 registrySubKey;	// key where toolbar information is kept
	CString			 registryValue;		// value where toolbar information is kept
};

// There is a bug in the patch of VC++ 4.2b: The commctrl.h, which already
// contains NM_CUSTOMDRAW is replaced with one version that doesn't.
// Thanks to Todd C. Wilson for the code below.
#if _MFC_VER >= 0x0420  &&  ! defined(NM_CUSTOMDRAW)

#define NM_CUSTOMDRAW           (NM_FIRST-12)
#define NM_HOVER                (NM_FIRST-13)

#define TB_SETIMAGELIST         (WM_USER + 48)
#define TB_GETIMAGELIST         (WM_USER + 49)
#define TB_LOADIMAGES           (WM_USER + 50)
#define TB_GETRECT              (WM_USER + 51) // wParam is the Cmd instead of index
#define TB_SETHOTIMAGELIST      (WM_USER + 52)
#define TB_GETHOTIMAGELIST      (WM_USER + 53)
#define TB_SETDISABLEDIMAGELIST (WM_USER + 54)
#define TB_GETDISABLEDIMAGELIST (WM_USER + 55)
#define TB_SETSTYLE             (WM_USER + 56)
#define TB_GETSTYLE             (WM_USER + 57)
#define TB_GETBUTTONSIZE        (WM_USER + 58)

//==================== CUSTOM DRAW ==========================================

// custom draw return flags
// values under 0x00010000 are reserved for global custom draw values.
// above that are for specific controls
#define CDRF_DODEFAULT          0x00000000
#define CDRF_NEWFONT            0x00000002
#define CDRF_SKIPDEFAULT        0x00000004

#define CDRF_NOTIFYPOSTPAINT    0x00000010
#define CDRF_NOTIFYITEMDRAW     0x00000020

// drawstage flags
// values under 0x00010000 are reserved for global custom draw values.
// above that are for specific controls
#define CDDS_PREPAINT           0x000000001
#define CDDS_POSTPAINT          0x000000002
// the 0x000010000 bit means it's individual item specific
#define CDDS_ITEM               0x000010000
#define CDDS_ITEMPREPAINT       (CDDS_ITEM | CDDS_PREPAINT)
#define CDDS_ITEMPOSTPAINT      (CDDS_ITEM | CDDS_POSTPAINT)

// itemState flags
#define CDIS_SELECTED    0x0001
#define CDIS_GRAYED      0x0002
#define CDIS_DISABLED    0x0004
#define CDIS_CHECKED     0x0008
#define CDIS_FOCUS       0x0010
#define CDIS_DEFAULT     0x0020

typedef struct tagNMCUSTOMDRAWINFO
{
    NMHDR hdr;
    DWORD dwDrawStage;
    HDC hdc;
    RECT rc;
    DWORD dwItemSpec;  // this is control specific, but it's how to specify an item.  valid only with CDDS_ITEM bit set
    UINT  uItemState;
    LPARAM lItemlParam;
} NMCUSTOMDRAW, FAR * LPNMCUSTOMDRAW;

// for tooltips

typedef struct tagNMTTCUSTOMDRAW
{
    NMCUSTOMDRAW nmcd;
    UINT uDrawFlags;
} NMTTCUSTOMDRAW, FAR * LPNMTTCUSTOMDRAW;

#endif  // _MFC_VER >= 4.2 && ! defined(NM_CUSTOMDRAW)

#endif // !defined(AFX_TOOLBAREX_H)



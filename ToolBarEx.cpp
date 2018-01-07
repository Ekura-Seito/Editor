#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "editor_def.h"
#include "ToolBarEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PADWIDTH(x)	(((x)*8+31)&~31)/8

/////////////////////////////////////////////////////////////////////////////
// local helper class CCustomDrawInfo
//
// The helper class CCustomDrawInfo handles the messaging to the docking
// frame of the toolbar in flat mode only. If flat-mode is disabled, then
// MFC's own messanger will be used.
//
// A few words about custom draw on toolbars:
// o custom draw is possible for MFC >= 4.2 only (older versions don't know
//   anything about certain structures ...)
// o MFC does not set the "rc" member of NMCUSTOMDRAW to the rectangle of the
//	 button that will be drawn. However, we do, so watch out, wether the
//	 toolbar is flat or not (or ignore the "rc" member in both cases).
//	 If the current mode is not "flat", then MFC's art of message arrives ...
// o MFC does not send a message for separators, so we too don't do it.
// o It seems that MFC toolbars never send *ERASE notifications; instead they
//   send TBN_QUERYDELETE for instance.
// o The CDRF_NEWFONT notification result is ignored (in flat mode. Never
//   tried with original MFC, because it is broken on toolbars).
/////////////////////////////////////////////////////////////////////////////



class CCustomDrawInfo 
{
#if _MFC_VER >= 0x0420
	NMCUSTOMDRAW	m_CDRW;				// custom draw information holder
	LRESULT			m_PrePaint;			// result from prepaint notification
	LRESULT			m_ItemPrePaint;		// dito for specific item
	CToolBarEx *	m_pToolBar;			// the real sender of the notification
	CWnd *			m_pReceiver;		// the receiver of the notification

	LRESULT			NotifyParent();
#endif // _MFC_VER

	public:		// construction
		CCustomDrawInfo( CDC & dc, CToolBarEx * pToolBar );

	public:
		// NotifyItemPrePaint() returns TRUE,
		// if the user wants to do the default
		// (CDRF_DODEFAULT) or FALSE, if the
		// user wants to skip (CDRF_SKIPDEFAULT)
		// Note that CDRF_SKIPDEFAULT is not
		// allowed for CDDS_PREPAINT, CDDS_POSTPAINT !
		// and CDDS_ITEMPOSTPAINT
		void	NotifyPrePaint();
		BOOL	NotifyItemPrePaint(int item);
		void	NotifyItemPostPaint(int item);
		void	NotifyPostPaint();
};


#if _MFC_VER >= 0x420

LRESULT CCustomDrawInfo :: NotifyParent() 
{
	LRESULT lRes = CDRF_DODEFAULT;

	if( m_pReceiver )
		lRes = m_pReceiver->SendMessage(WM_NOTIFY,WPARAM(m_CDRW.hdr.idFrom),LPARAM(&m_CDRW));
	return lRes;
}

CCustomDrawInfo :: CCustomDrawInfo( CDC & dc, CToolBarEx * pBar )
		: m_PrePaint(0)	, m_ItemPrePaint(0)
{
	VERIFY((m_pToolBar = pBar) != 0);
	VERIFY((m_CDRW.hdc = dc.GetSafeHdc()) != 0);

	HWND hwnd = pBar->GetSafeHwnd();
	VERIFY(::IsWindow(hwnd));

	// initialise the NMHDR member of the customdraw structure
	m_CDRW.hdr.hwndFrom = hwnd;
	m_CDRW.hdr.idFrom = UINT(::GetWindowLong(hwnd, GWL_ID));
	m_CDRW.hdr.code = NM_CUSTOMDRAW;

	// Do not use CControlBar::GetDockingFrame() to receive
	// the parent. CWnd::GetParent() is inacceptable too.
	// Both these functions don't work, if the toolbar is
	// floating in the air!
	m_pReceiver = pBar->GetParentFrame();
	if( m_pReceiver )
		VERIFY(::IsWindow(m_pReceiver->GetSafeHwnd()));
}

void CCustomDrawInfo :: NotifyPrePaint() 
{
	// fill the customdraw structure with values for CDDS_PREPAINT
	m_CDRW.dwDrawStage = CDDS_PREPAINT;
	// the rest of the structure stays undefined in this stage
	// of drawing.
	m_PrePaint = NotifyParent();
}

BOOL CCustomDrawInfo :: NotifyItemPrePaint( int nItem ) 
{
	BOOL bRet = TRUE;	// we assume to do the default
	if( m_PrePaint & CDRF_NOTIFYITEMDRAW ) 
	{
		m_CDRW.dwDrawStage = CDDS_ITEMPREPAINT;
		m_pToolBar->GetItemRect(nItem, &m_CDRW.rc);
		m_CDRW.dwItemSpec = DWORD(m_pToolBar->GetItemID(nItem));
		UINT uStyle = m_pToolBar->GetButtonStyle(nItem);
		BOOL bEnable = m_pToolBar->GetToolBarCtrl().IsButtonEnabled(m_CDRW.dwItemSpec);
		m_CDRW.uItemState = (bEnable ? 0 : CDIS_DISABLED) |(((uStyle & TBBS_PRESSED) || (uStyle & TBBS_CHECKED)) ? CDIS_CHECKED : 0);
		m_CDRW.lItemlParam = 0;
		m_ItemPrePaint = NotifyParent();
		if( m_ItemPrePaint & CDRF_SKIPDEFAULT )
			bRet = FALSE;
	}
	return bRet;
}

void CCustomDrawInfo :: NotifyItemPostPaint( int nItem ) 
{
	if( m_ItemPrePaint & CDRF_NOTIFYPOSTPAINT ) 
	{
		m_CDRW.dwDrawStage = CDDS_ITEMPOSTPAINT;
		// the rest of the data has not been changed since ITEMPREPAINT
		// make sure it is so:
		ASSERT(m_pToolBar->GetItemID(nItem) == m_CDRW.dwItemSpec);
		NotifyParent();
	}
}

void CCustomDrawInfo :: NotifyPostPaint() 
{
	if( m_PrePaint & CDRF_NOTIFYPOSTPAINT ) 
	{
		m_CDRW.dwDrawStage = CDDS_POSTPAINT;
		NotifyParent();
	}
}
			
#else	// _MFC_VER < 4.2

CCustomDrawInfo :: CCustomDrawInfo( CDC & dc, CToolBarEx * pParent ) 
{
}

void CCustomDrawInfo :: NotifyPrePaint() 
{
}

void CCustomDrawInfo :: NotifyPostPaint() 
{
}

BOOL CCustomDrawInfo :: NotifyItemPrePaint( int ) 
{
	return TRUE;	// we always make the drawing by ourself
}

void CCustomDrawInfo :: NotifyItemPostPaint( int ) 
{
}

#endif	// _MFC_VER



/////////////////////////////////////////////////////////////////////////////
// CToolBarEx

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CToolBarEx::CToolBarEx()
	: m_bFlatLook(TRUE)
	, m_clrBtnFace(::GetSysColor(COLOR_BTNFACE))
	, m_clrBtnHilight(::GetSysColor(COLOR_BTNHILIGHT))
	, m_clrBtnShadow(::GetSysColor(COLOR_BTNSHADOW))
	, m_clrBtnLight(::GetSysColor(COLOR_3DLIGHT))
	, m_nLastBtn(-1)
	, m_uTimerEvent(0)
{
	m_pApp = (CEditorApp*)AfxGetApp();

	// the systems I've tested, made no difference between
	// COLOR_BTNFACE and COLOR_3DLIGHT ...
	if( m_clrBtnFace == m_clrBtnLight )
		m_clrBtnLight = m_clrBtnHilight;

	CalculateOffset();

	// create the default font, used for buttons with text
	CFont Font;
	BOOL bOldSys = FALSE;
	if( ! Font.CreateStockObject( DEFAULT_GUI_FONT ) ) {
		// older versions of Windows* (NT 3.51 for instance)
		// fail with DEFAULT_GUI_FONT
		VERIFY( Font.CreateStockObject( SYSTEM_FONT ) );
		bOldSys = TRUE;
	}
	LOGFONT logfont ;
	Font.GetLogFont( &logfont ) ;
	if( bOldSys ) {
		logfont.lfWeight = 400;
		strcpy(logfont.lfFaceName,"MS Sans Serif");
	}
	logfont.lfHeight = 6 ;
	logfont.lfWidth = 0 ;	// let windows compute this.
	VERIFY( m_GuiFont.CreateFontIndirect( &logfont ) ) ;

//for Customize
	m_pToolBarInfo = NULL;
	m_nButtons = 0;
	registryKey=NULL;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CToolBarEx::~CToolBarEx()
{
}

IMPLEMENT_DYNAMIC(CToolBarEx, CToolBar)

BEGIN_MESSAGE_MAP(CToolBarEx, CToolBar)
	//{{AFX_MSG_MAP(CToolBarEx)
	ON_WM_PAINT()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_NCCALCSIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_NCPAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(TB_SETBUTTONSIZE, OnSetButtonSize)
	ON_MESSAGE(TB_SETBITMAPSIZE, OnSetBitmapSize)
	ON_NOTIFY_REFLECT(TBN_BEGINADJUST, OnToolBarBeginAdjust)
	ON_NOTIFY_REFLECT(TBN_BEGINDRAG, OnToolBarBeginDrag)
	ON_NOTIFY_REFLECT(TBN_CUSTHELP, OnToolBarCustomHelp)
	ON_NOTIFY_REFLECT(TBN_ENDADJUST, OnToolBarEndAdjust)
	ON_NOTIFY_REFLECT(TBN_ENDDRAG, OnToolBarEndDrag)
	ON_NOTIFY_REFLECT(TBN_GETBUTTONINFO, OnToolBarGetButtonInfo)
	ON_NOTIFY_REFLECT(TBN_QUERYDELETE, OnToolBarQueryDelete)
	ON_NOTIFY_REFLECT(TBN_QUERYINSERT, OnToolBarQueryInsert)
	ON_NOTIFY_REFLECT(TBN_RESET, OnToolBarReset)
	ON_NOTIFY_REFLECT(TBN_TOOLBARCHANGE, OnToolBarChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarEx カスタマイズ用関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：カスタマイズ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CToolBarEx::Customize() 
{
	//カスタマイズダイアログ表示
	GetToolBarCtrl().Customize();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ツールバー作成														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
// This function creates the toolbar and associates it with its parent.  Also,
// the registry key to be used for saving and restoring the toolbar's state is
// stored for later use.
BOOL CToolBarEx::Create
	(	CWnd*	pParentWnd,
		DWORD	dwStyle,
		UINT	nID,
		CToolBarInfo*	tbInfo,
		CString	regSubKey,
		CString regValue,
		HKEY regKey
	)
{
	BOOL success;		// indicates if the toolbar was created

	// if default processing is ok
	if (CToolBar::Create(pParentWnd, dwStyle, nID))
	{
		// indicate success
		success = TRUE;

		// modify the style to include adjustable
		ModifyStyle(0, CCS_ADJUSTABLE);

		// keep the pointer to the toolbar information
		m_pToolBarInfo = tbInfo;

		// if there aren't any buttons to customize
		if (!tbInfo)
		{
			m_nButtons = 0;
		}
	}

	// else default processing failed
	else
	{
		TRACE0("Failed to create toolbar\n");
		success = FALSE;
	}

	// keep record of where our registry entry lives
	registryKey = regKey;
	registrySubKey = regSubKey;
	registryValue = regValue;

	// indicate success
	return success;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ツールバーロード														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
// overridden LoadToolBar allows automatic restoration
// of toolbar information and calculation of total
// button count
BOOL CToolBarEx::LoadToolBar(UINT idResource, BOOL restore)
{
	BOOL success;		// indicates success;

	// load the toolbar bitmap
	success = CToolBar::LoadToolBar(idResource);

	// count the buttons
	m_nButtons = GetToolBarCtrl().GetButtonCount();

	// if we're supposed to restore last setting
	if (restore)
	{
		RestoreState();
	}
	return success;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ボタン数取得															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CToolBarEx::NButtons()
{
	return m_nButtons;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：状態保存																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
// This function saves the state (visible buttons, toolbar position, etc.)
// of the toolbar, using the registry key provided to the Create(...) function.
void CToolBarEx::SaveState()
{
	// if there is an associated registry subkey
	if (registrySubKey.GetLength())
	{
		// save the toolbar state to the registry
		GetToolBarCtrl().SaveState(registryKey, registrySubKey, registryValue);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：状態復元																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
// This function restores the state (visible buttons, toolbar position, etc.)
// of the toolbar, using the registry key provided to the Create(...) function.
void CToolBarEx::RestoreState()
{
	// if there is an associated registry subkey
	if (registrySubKey.GetLength())
	{
		// restore the toolbar state from the registry
		GetToolBarCtrl().RestoreState(registryKey, registrySubKey, registryValue);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ユーザーがカスタマイズ中に、ツールバー コントロールからボタンを削除で	|*/
/*|			  きるかどうかを調べるために送られます									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CToolBarEx::OnToolBarQueryDelete(NMHDR *notify, LRESULT *result)
{
	// if we're not floating - user can delete anything
	*result = !IsFloating();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ユーザーがカスタマイズ中に、指定されたボタンの左にボタンを挿入できるか|*/
/*|			  どうかを調べるために送られます										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CToolBarEx::OnToolBarQueryInsert(NMHDR *notify, LRESULT *result)
{
	// if we're not floating - user can insert anywhere
	*result = !IsFloating();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ユーザーがツールバー コントロールのカスタマイズを完了したときに送られ	|*/
/*|			  ます																	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CToolBarEx::OnToolBarChange(NMHDR *notify, LRESULT *result)
{
	// force the frame window to recalculate the size
	GetParentFrame()->RecalcLayout();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ユーザーが、ツールバー コントロールのボタンのドラッグを開始したときに	|*/
/*|			  送られます															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CToolBarEx::OnToolBarBeginDrag(NMHDR *notify, LRESULT *result)
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ユーザーが、ツールバー コントロールのボタンのドラッグを終了したときに	|*/
/*|			  送られます															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CToolBarEx::OnToolBarEndDrag(NMHDR *notify, LRESULT *result)
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ユーザーが、[ツールバーのカスタマイズ] ダイアログ ボックスの [ヘルプ] |*/
/*|			  をクリックすると送られます											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CToolBarEx::OnToolBarCustomHelp(NMHDR *notify, LRESULT *result)
{
	m_pApp->OpenHelpTopix(HELPTOPIC_TOOLBAR);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ユーザーが、ツールバー コントロールのカスタマイズを開始したときに		|*/
/*|			  送られます															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CToolBarEx::OnToolBarBeginAdjust(NMHDR *notify, LRESULT *result)
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ユーザーがツールバー コントロールのカスタマイズを完了したときに送られ	|*/
/*|			  ます																	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CToolBarEx::OnToolBarEndAdjust(NMHDR *notify, LRESULT *result)
{
	// save the state of the toolbar for reinitialization
	SaveState();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ユーザーが、ツールバー コントロールをカスタマイズしてるときに送られ	|*/
/*|			  ます																	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CToolBarEx::OnToolBarGetButtonInfo(NMHDR *notify, LRESULT *result)
{
	TBNOTIFY		*tbStruct;		// data needed by customize dialog box

	// init the pointer
	tbStruct = (TBNOTIFY *)notify;

	// if the index is valid
	if (0 <= tbStruct->iItem && tbStruct->iItem < NButtons())
	{
		// copy the stored button structure
		tbStruct->tbButton = m_pToolBarInfo[tbStruct->iItem].m_tbButton;

		// copy the text for the button label in the dialog
		strcpy(tbStruct->pszText, m_pToolBarInfo[tbStruct->iItem].m_btnText);

		// indicate valid data was sent
		*result = TRUE;
	}

	// else there is no button for this index
	else
	{
		*result = FALSE;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	： [ツールバーのカスタマイズ] ダイアログ ボックスの内容をユーザーが		|*/
/*|			  リセットするときに送られます											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CToolBarEx::OnToolBarReset(NMHDR *notify, LRESULT *result)
{
	// restore the toolbar to the way it was before entering customization
	RestoreState();
	GetParentFrame()->RecalcLayout();
}

/////////////////////////////////////////////////////////////////////////////
// CToolBarEx パブリック関数

void CToolBarEx::SetFlatLook( BOOL bFlat )
{
	if( bFlat != m_bFlatLook ) {
		m_bFlatLook = bFlat;
		if( ::IsWindow(GetSafeHwnd()) ) {
			// force a repaint of all buttons
			Invalidate();
			// erase/draw the gripper
			OnNcPaint();
		}
	}
}

BOOL CToolBarEx::IsFlatLook() const
{
	return m_bFlatLook;
}

// This function I've missed in CToolBar for more than one time ...
void CToolBarEx::GetSizes( CSize & szBtn, CSize & szImg ) const 
{
	szBtn = m_sizeButton;
	szImg = m_sizeImage;
}

// Get the window to which the toolbar initially was docked. This
// is not necessarily the window returned by CWnd::GetParent() or
// CControlBar::GetDockingFrame(). Both these functions don't
// return the expected window, if the toolbar is floating ...
//CWnd*	CToolBarEx::GetParentFrame() const 
//{
//	return ::IsWindow(m_hwndParent) ? CWnd::FromHandle(m_hwndParent) : 0;
//}

BOOL CToolBarEx::HasButtonText() const 
{
	return m_pStringMap != 0 && !m_pStringMap->IsEmpty();
}

/////////////////////////////////////////////////////////////////////////////
// CToolBarEx message handlers

LRESULT CToolBarEx :: OnSetButtonSize(WPARAM wParam, LPARAM lParam) {
	LRESULT lResult = CToolBar::OnSetButtonSize(wParam, lParam);
	if( lResult )
		CalculateOffset();
	return lResult;
}

LRESULT CToolBarEx :: OnSetBitmapSize(WPARAM wParam, LPARAM lParam) {
	LRESULT lResult = CToolBar::OnSetBitmapSize(wParam, lParam);
	if( lResult )
		CalculateOffset();
	return lResult;
}


void CToolBarEx::OnPaint() 
{
	HIMAGELIST hImg = GetImageList();

#ifdef _DEBUG
	if( hImg == 0 ) {
		TRACE0("CToolBarEx::OnPaint(): could not get image list\n");
	}
#endif

	if( m_bFlatLook && hImg ) {
		CRect rcUpdate;
		if( ! GetUpdateRect(rcUpdate) )
			return;

		if( HasButtonText() )
			CalculateOffset();		// strings may have been added

		// attach image-list for even more MFC feeling :)
		CImageList imglist;
		imglist.Attach(hImg);

		POINT cursor;
		::GetCursorPos(&cursor);
		ScreenToClient(&cursor);

		CPaintDC dc(this); // device context for painting
		CFont * pOldFont = dc.SelectObject(&m_GuiFont);

		// Now it's time for the first custom-draw-notification...
		CCustomDrawInfo cdrw(dc, this);
		cdrw.NotifyPrePaint();

		register const int nBtn = GetToolBarCtrl().GetButtonCount();

		for( register int i = 0; i < nBtn; ++i ) {
			CRect rc;
			GetItemRect(i, rc);

			int nBitmap; UINT uID, uStyleState;
			GetButtonInfo(i, uID, uStyleState, nBitmap);
			WORD wStyle = LOWORD(uStyleState);
			WORD wState = HIWORD(uStyleState);

			if( wState & TBSTATE_HIDDEN )
				continue;

			if( wStyle == TBSTYLE_SEP ) {
				if( !(wState & TBSTATE_WRAP) || ! IsFloating() )
					DrawSeparator(dc, rc);
			} else {
				if( ! CRect().IntersectRect(rcUpdate, rc) )
					continue;	// this button needs no repaint

				BOOL bBtnDown = (wState & TBSTATE_CHECKED) || (wState & TBSTATE_PRESSED);
				BOOL bBtnEnabled = GetToolBarCtrl().IsButtonEnabled(int(uID));
				BOOL bHasCursor = rc.PtInRect(cursor);
				COLORREF clrRect = (bBtnDown && !bHasCursor) ? m_clrBtnLight : m_clrBtnFace;

				// maybe the button has text
				dc.SetTextColor(RGB(0,0,0));
				dc.SetBkColor(clrRect);

				// There is a bug in CToolBar: If there are texts assigned
				// to buttons, then the button-widths may change transparently
				// (without notified by CToolBar), so we recalculate the
				// horizontal offset here:
				m_sizeOffset.cx = (rc.Width() - m_sizeImage.cx) / 2;

				if( ! cdrw.NotifyItemPrePaint(i) )
					continue;	// parent has already drawn the button

				dc.FillSolidRect(rc, clrRect);
				//CBrush brush(clrRect);
				//dc.FillRect(rc, &brush);

				// it seems, that CDC::Draw3dRect() changes the background color
				COLORREF clrBk = dc.GetBkColor();

				if( bBtnDown ) {
					// draw a pressed button
					dc.Draw3dRect(rc, m_clrBtnShadow, m_clrBtnHilight);
					if( ! bHasCursor ) {
						// draw an invisible frame around the hilighted area
						CRect rcCheck = rc;
						rcCheck.DeflateRect(1,1);
						dc.Draw3dRect(rcCheck, m_clrBtnFace, m_clrBtnFace);
					}
				} else if( bHasCursor && ! bBtnDown && bBtnEnabled )
					// draw a normal button
					dc.Draw3dRect(rc, m_clrBtnHilight, m_clrBtnShadow);
				else if( ! bBtnDown && bBtnEnabled )
					// Draw an invisible rect around the button.
					// This prevents us from erasing the background
					// if the button was formed before
					// (that would cause the button to flicker ...)
					dc.Draw3dRect(rc, m_clrBtnFace, m_clrBtnFace);
				
				dc.SetBkColor(clrBk);

				// the point where to start with the image
				CPoint pt(rc.left + m_sizeOffset.cx + bBtnDown,
						  rc.top + m_sizeOffset.cy + bBtnDown);
				
				imglist.Draw(&dc, nBitmap, pt, ILD_TRANSPARENT);

				CString strText = GetButtonText(i);
				if( strText.GetLength() ) {
					CRect rectText(
						rc.left+3+bBtnDown,
						rc.top+m_sizeOffset.cy+m_sizeImage.cy+1+bBtnDown,
						rc.right-3+bBtnDown,
						rc.bottom-3+bBtnDown
					);
					dc.DrawText(strText, rectText, DT_CENTER|DT_VCENTER|DT_NOCLIP);
				}
				
				if( ! bBtnEnabled )
					// gray out that button
					DrawDisabledButton(dc, rc);

				cdrw.NotifyItemPostPaint(i);
			}
		}

		dc.SelectObject(pOldFont);

		if( ! m_bDeleteImgList )
			imglist.Detach();

		// last but not least: inform the parent for end of painting
		cdrw.NotifyPostPaint();
	} else
		// classic mode (or couldn't receive imagelist)
		CToolBar::OnPaint();
}

/////////////////////////////////////////////////////////////////////////////
// CToolBarEx プライベート関数

// Recalculate the starting point of the button's image.
void CToolBarEx::CalculateOffset() 
{
	if( HasButtonText() ) {
		// there are strings assigned to at least one button
		// center the image horizontal
		m_sizeOffset.cx = (m_sizeButton.cx-m_sizeImage.cx)/2;
		m_sizeOffset.cy = m_cyTopBorder;
	} else {
		// no button has text assigned: center the image on the button
		m_sizeOffset.cx = (m_sizeButton.cx-m_sizeImage.cx)/2;
		m_sizeOffset.cy = (m_sizeButton.cy-m_sizeImage.cy)/2;
	}
}

void CToolBarEx :: DrawDisabledButton( CDC & dc, const CRect & rc ) const {
	// create a monochrome memory DC
	CDC ddc;
	ddc.CreateCompatibleDC(0);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&ddc, rc.Width(), rc.Height());
	CBitmap * pOldBmp = ddc.SelectObject(&bmp);
	
	// build a mask
	ddc.PatBlt(0, 0, rc.Width(), rc.Height(), WHITENESS);
	dc.SetBkColor(m_clrBtnFace);
	ddc.BitBlt(0, 0, rc.Width(), rc.Height(), &dc, rc.left, rc.top, SRCCOPY);
	dc.SetBkColor(m_clrBtnHilight);
	ddc.BitBlt(0, 0, rc.Width(), rc.Height(), &dc, rc.left, rc.top, SRCPAINT);

	// Copy the image from the toolbar into the memory DC
	// and draw it (grayed) back into the toolbar.
	dc.FillSolidRect(rc.left, rc.top, rc.Width(), rc.Height(), m_clrBtnFace);
	dc.SetBkColor(RGB(0, 0, 0));
	dc.SetTextColor(RGB(255, 255, 255));
	CBrush brShadow, brHilight;
	brHilight.CreateSolidBrush(m_clrBtnHilight);
	brShadow.CreateSolidBrush(m_clrBtnShadow);
	CBrush * pOldBrush = dc.SelectObject(&brHilight);
	dc.BitBlt(rc.left+1, rc.top+1, rc.Width(), rc.Height(), &ddc, 0, 0, 0x00E20746L);
	dc.SelectObject(&brShadow);
	dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &ddc, 0, 0, 0x00E20746L);
	
	// reset DCs
	dc.SelectObject(pOldBrush);
	ddc.SelectObject(pOldBmp);
	ddc.DeleteDC();

	bmp.DeleteObject();
}


void CToolBarEx :: DrawSeparator( CDC & dc, CRect & rc ) const {
    BOOL bHorz = ((m_dwStyle & CBRS_ORIENT_HORZ) != 0) ? TRUE : FALSE;
	// make sure, this separator is not a placeholder for
	// another control.
	if( rc.Width() <= 8 ) {
		if( bHorz ) {
			// draw the separator bar in the middle
			int x = (rc.left + rc.right) / 2;
			rc.left = x-1; rc.right = x+1;
			dc.Draw3dRect(
				rc,
				m_clrBtnShadow,
				m_clrBtnHilight
			);
		} else {
			// draw the separator bar in the middle
			rc.left = rc.left - m_sizeButton.cx;
			rc.right = rc.left + m_sizeButton.cx;
			rc.top = rc.bottom+1;
			rc.bottom = rc.top+3;
			int y = (rc.top+rc.bottom)/2;
			rc.top = y-1; rc.bottom = y+1;
			dc.Draw3dRect(
				rc,
				m_clrBtnShadow,
				m_clrBtnHilight
			);
		}
	}
}


void CToolBarEx :: DrawGripper( CDC & dc ) const { 
    if( m_dwStyle & CBRS_FLOATING )
		return;		// no gripper if floating

	CRect gripper;
	GetWindowRect(gripper);
	ScreenToClient(gripper);
	gripper.OffsetRect(-gripper.left, -gripper.top);

	if( m_dwStyle & CBRS_ORIENT_HORZ ) {
		// gripper at left
		gripper.DeflateRect(4, 4);
		gripper.right = gripper.left+3;
        dc.Draw3dRect(
			gripper,
			m_clrBtnHilight,
            m_clrBtnShadow
		);
		gripper.OffsetRect(3, 0);
        dc.Draw3dRect(
			gripper,
			m_clrBtnHilight,
            m_clrBtnShadow
		);
	} else {
		// gripper at top
		gripper.DeflateRect(4, 4);
		gripper.bottom = gripper.top+3;
		dc.Draw3dRect(
			gripper,
			m_clrBtnHilight,
            m_clrBtnShadow
		);
		gripper.OffsetRect(0, 3);
        dc.Draw3dRect(
			gripper,
			m_clrBtnHilight,
            m_clrBtnShadow
		);
	}
}

HIMAGELIST CToolBarEx :: GetImageList() {
	m_bDeleteImgList = FALSE;

	HIMAGELIST hImg = 0;

#ifdef TB_GETIMAGELIST
	// Some older versions of VC++ do not know of
	// the TB_GETIMAGELIST macro (defined in commctrl.h).

	hImg = HIMAGELIST(SendMessage(TB_GETIMAGELIST));

#ifdef _DEBUG
	if( hImg == 0 ) {
		TRACE0("CToolBarEx::OnPaint(): could not get image list\n");
	}
#endif
#endif // TB_GETIMAGELIST

	if( ! hImg ) {
		// comctl32.dll version prior to 4.70 doesn't know
		// anything of the TB_GETIMAGELIST message
		if( m_hbmImageWell != 0 ) {
			// Yup - we have a valid image.
			// But beware: Do not use this bitmap directly.
			// We make the copy by ourself. CopyImage() (for
			// instace) produces inacceptable copies under
			// some circumstances ...
			CImageList imglist;
			CBitmap bmp;
			
			// retrieve the size of the bitmap
			BITMAP bmHdr;
			::GetObject(m_hbmImageWell, sizeof(BITMAP), &bmHdr);
			
			DWORD dwWidth, dwHeight = bmHdr.bmHeight;
			
			if (bmHdr.bmBitsPixel > 8)
				dwWidth = PADWIDTH(bmHdr.bmWidth * 3);
			else
				dwWidth = PADWIDTH(bmHdr.bmWidth);
			
			// copy the bitmap
			CClientDC cdc(this);
			CDC dc1, dc2;
			dc1.CreateCompatibleDC(&cdc);
			dc2.CreateCompatibleDC(&cdc);
			bmp.CreateCompatibleBitmap(&cdc, dwWidth, dwHeight);
			CBitmap * pOBmp = dc1.SelectObject(&bmp);
			HGDIOBJ hOObj = ::SelectObject(dc2.GetSafeHdc(), m_hbmImageWell);
			dc1.BitBlt(0,0,dwWidth,dwHeight,&dc2,0,0,SRCCOPY);
			::SelectObject(dc2.GetSafeHdc(), hOObj);
			dc1.SelectObject(pOBmp);
			dc1.DeleteDC();
			dc2.DeleteDC();

			imglist.Create(m_sizeImage.cx, m_sizeImage.cy,TRUE,dwWidth/m_sizeImage.cx,1);
			imglist.SetBkColor(m_clrBtnFace);
			imglist.Add(&bmp,m_clrBtnFace);
			hImg = imglist.Detach();
			bmp.DeleteObject();
			m_bDeleteImgList = TRUE;
		}
	}

	return hImg;
}


/////////////////////////////////////////////////////////////////////////////
// CToolBarEx プロテクト関数

void CToolBarEx :: OnUpdateCmdUI( CFrameWnd* pTarget, BOOL bDisableIfNoHndler ) {
	if( m_bFlatLook ) {
		// save current styles
		register const int nBtn = GetToolBarCtrl().GetButtonCount();
		register int nIdx;
		for( nIdx = 0; nIdx < nBtn; ++nIdx )
			m_Styles.SetAtGrow(nIdx, GetButtonStyle(nIdx));

		// do base class processing
		CToolBar::OnUpdateCmdUI(pTarget,bDisableIfNoHndler);

		//check wether styles have been changed
		for( nIdx = 0; nIdx < nBtn; ++nIdx ) {
			if( m_Styles[nIdx] != GetButtonStyle(nIdx) ) {
				// invalidate that button
				CRect rc;
				GetItemRect(nIdx, rc);
				InvalidateRect(rc);
			}
		}
	} else
		// simply delegate
		CToolBar::OnUpdateCmdUI(pTarget,bDisableIfNoHndler);
}

void CToolBarEx::OnSysColorChange() 
{
	CToolBar::OnSysColorChange();

	m_clrBtnFace    = ::GetSysColor(COLOR_BTNFACE);
	m_clrBtnHilight = ::GetSysColor(COLOR_BTNHILIGHT);
	m_clrBtnShadow  = ::GetSysColor(COLOR_BTNSHADOW);
	m_clrBtnLight   = ::GetSysColor(COLOR_3DLIGHT);

	if( m_clrBtnFace == m_clrBtnLight )
		m_clrBtnLight = m_clrBtnHilight;
}


void CToolBarEx::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	CToolBar::OnNcCalcSize(bCalcValidRects, lpncsp);
	// adjust non-client area for gripper at left or top
	if( m_dwStyle & CBRS_ORIENT_HORZ ) {
		lpncsp->rgrc[0].left += 4;
		lpncsp->rgrc[0].right += 4;
	} else {
		lpncsp->rgrc[0].top += 6;
		lpncsp->rgrc[0].bottom += 6;
	}
}


void CToolBarEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( m_bFlatLook ) {
		register const int nBtn = GetToolBarCtrl().GetButtonCount();
		const int nLastBtn = m_nLastBtn;
		m_nLastBtn = -1;

		for( register int i = 0 ; i < nBtn ; ++i ) {
			CRect rc;
			GetItemRect(i, rc);

			const BOOL bBtnEnabled = GetToolBarCtrl().IsButtonEnabled(int(GetItemID(i)));
			const BOOL bSep = GetButtonStyle(i) & TBBS_SEPARATOR;

			if( bSep || ! bBtnEnabled )
				continue;

			const BOOL bHasCursor = rc.PtInRect(point);

			if( bHasCursor && bBtnEnabled ) {
				if( nLastBtn != i ) {
					// force a repaint of the button with the cursor on it
					InvalidateRect(rc, FALSE);
				}
				m_nLastBtn = i;
			} else if( !bHasCursor && i == nLastBtn ) {
				// force a repaint of the last formed button
				InvalidateRect(rc, FALSE);
			}
		}
		// One problem occures with WM_MOUSEMOVE: we cannot detect
		// that the mouse leaves the window. If the mouse moves quick
		// enough, then the last formed button stays visible. To
		// resolve this problem, we set a timer and check, wether
		// the mouse is outside the window ...
		KillTimer(m_uTimerEvent);
		m_uTimerEvent = SetTimer(1, 250, 0);
	}
	CToolBar::OnMouseMove(nFlags, point);
}


void CToolBarEx::OnNcPaint() 
{
	if( m_bFlatLook ) {
		CToolBar::EraseNonClient();
		CWindowDC dc(this);
		DrawGripper(dc);
	} else
		CToolBar::OnNcPaint();
}

void CToolBarEx::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == m_uTimerEvent && m_nLastBtn >= 0 ) {
		POINT pt;
		::GetCursorPos(&pt);
		CRect rc;
		GetWindowRect(rc);
		if( ! rc.PtInRect(pt) ) {
			GetItemRect(m_nLastBtn, rc);
			InvalidateRect(rc, FALSE);
			m_nLastBtn = -1;
			KillTimer(nIDEvent);
		}
	} else
		CToolBar::OnTimer(nIDEvent);
}

int CToolBarEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Save the parent at creation time. It may change, if
	// the toolbar is floating; but we want to know of the
	// "real" parent (for notification messages)!
	m_hwndParent = lpCreateStruct->hwndParent;
	
	return 0;
}

void CToolBarEx::OnDestroy() 
{
	// save the current state of the toolbar
	SaveState();

	// default processing
	CToolBar::OnDestroy();
}


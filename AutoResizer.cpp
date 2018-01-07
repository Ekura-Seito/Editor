// AutoResizer.cpp : CAutoResizer クラスの動作の定義を行います。
//

#include "StdAfx.h"
#pragma warning(disable:4786)
#include "AutoResizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoResizer クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CAutoResizer::CAutoResizer(CWnd* pParent /* = NULL */)
{
	m_pParent = NULL;
	m_nOldParentWidth = 0;
	m_nOldParentHeight = 0;
	m_nOriginalParentWidth = 0;
	m_nOriginalParentHeight = 0;
	m_fNegativeMoves = FALSE;

	ResetControls();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CAutoResizer::~CAutoResizer()
{
	ResetControls();
}

/////////////////////////////////////////////////////////////////////////////
// CAutoResizer パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：親ウインドウ設定														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CAutoResizer::SetParent(CWnd* pParent)
{
	CRect rcParentOriginalSize;

	m_pParent = pParent;

	m_pParent->GetClientRect(rcParentOriginalSize);
	m_nOriginalParentWidth = rcParentOriginalSize.Width();
	m_nOriginalParentHeight = rcParentOriginalSize.Height();

	UpdateParentSize();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：親ウインドウ設定														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CAutoResizer::SetParent(CWnd* pParent, int nWidth, int nHeight)
{
	m_pParent = pParent;

	m_nOriginalParentWidth = nWidth;
	m_nOriginalParentHeight = nHeight;
	m_nOldParentWidth = nWidth;
	m_nOldParentHeight = nHeight;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コントロール追加														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CAutoResizer::AddControl(CWnd* pControl, const DWORD& dwStyle /* = CP_MOVE_HORIZONTAL */)
{
	BOOL fReturnValue = TRUE;

	if (pControl && m_pParent)
	{
		LPCONTROLDATA pstControl = new CONTROLDATA;
		pstControl->hControl = pControl->GetSafeHwnd();
		pstControl->dwStyle = dwStyle;
		m_awndControls.AddTail((CObject*)pstControl);
	}
	else
	{
		fReturnValue = FALSE;
	}

	return (fReturnValue);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コントロール追加														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CAutoResizer::AddControl(const UINT& unId, const DWORD& dwStyle /* = CP_MOVE_HORIZONTAL */)
{
	CWnd* pControl;

	if (m_pParent)
	{
		pControl = m_pParent->GetDlgItem(unId);
		return (AddControl(pControl, dwStyle));
	}
	else
	{
		return (FALSE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コントロール削除														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CAutoResizer::RemoveControl(CWnd* pControl)
{
	BOOL fReturnValue = FALSE;
	POSITION pos;

	for(pos=m_awndControls.GetHeadPosition(); pos!=NULL; )
	{
		LPCONTROLDATA pstControl = (LPCONTROLDATA)m_awndControls.GetNext(pos);
		if(pstControl->hControl == pControl->GetSafeHwnd())
		{
			m_awndControls.RemoveAt(pos);
			delete pstControl;
			fReturnValue = TRUE;
			break;
		}
	}

	return (fReturnValue);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コントロール削除														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CAutoResizer::RemoveControl(const UINT& unId)
{
	CWnd* pControl;

	if (m_pParent)
	{
		pControl = m_pParent->GetDlgItem(unId);
		return (RemoveControl(pControl));
	}
	else
	{
		return (FALSE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：全コントロール削除													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CAutoResizer::ResetControls(void)
{
	POSITION pos;

	for(pos=m_awndControls.GetHeadPosition(); pos!=NULL; )
	{
		LPCONTROLDATA pstControl = (LPCONTROLDATA)m_awndControls.GetNext(pos);
		if(pstControl)
			delete pstControl;
	}
	m_awndControls.RemoveAll();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コントロール移動														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CAutoResizer::MoveControls(void)
{
	POSITION pos;

	if (m_pParent)
	{
		//--------------------------------------------------------------------
		// for each control that has been added to our object, we want to
		// check its style and move it based off of the parent control's
		// movements.
		// the thing to keep in mind is that when you resize a window, you
		// can resize by more than one pixel at a time. this is important
		// when, for example, you start with a width smaller than the
		// original width and you finish with a width larger than the
		// original width. you know that you want to move the control, but
		// by how much? that is why so many if's and calculations are made
		//
		for(pos=m_awndControls.GetHeadPosition(); pos!=NULL; )
		{
			LPCONTROLDATA pstControl = (LPCONTROLDATA)m_awndControls.GetNext(pos);
			CRect rcParentBounds;
			CRect rcBounds;
			CWnd* pControl = m_pParent->FromHandle(pstControl->hControl);

			pControl->GetWindowRect(rcBounds);
			m_pParent->GetClientRect(rcParentBounds);

			if ((pstControl->dwStyle & (CP_RESIZE_VERTICAL)) == (CP_RESIZE_VERTICAL))
			{
				if (!m_fNegativeMoves)
				{
					if (rcParentBounds.bottom > m_nOriginalParentHeight)
					{
						if (m_nOriginalParentHeight <= m_nOldParentHeight)
						{
							rcBounds.bottom += rcParentBounds.bottom - m_nOldParentHeight;
						}
						else
						{
							rcBounds.bottom += rcParentBounds.bottom - m_nOriginalParentHeight;
						}
					}
					else
					{
						if (m_nOldParentHeight > m_nOriginalParentHeight)
						{
							rcBounds.bottom += m_nOriginalParentHeight - m_nOldParentHeight;
						}
					}
				}
				else
				{
					rcBounds.bottom += rcParentBounds.bottom - m_nOldParentHeight;
				}
			}

			if ((pstControl->dwStyle & (CP_RESIZE_HORIZONTAL)) == (CP_RESIZE_HORIZONTAL))
			{
				if (!m_fNegativeMoves)
				{
					if (rcParentBounds.right > m_nOriginalParentWidth)
					{
						if (m_nOriginalParentWidth <= m_nOldParentWidth)
						{
							rcBounds.right += rcParentBounds.right - m_nOldParentWidth;
						}
						else
						{
							rcBounds.right += rcParentBounds.right - m_nOriginalParentWidth;
						}
					}
					else
					{
						if (m_nOldParentWidth > m_nOriginalParentWidth)
						{
							rcBounds.right += m_nOriginalParentWidth - m_nOldParentWidth;
						}
					}
				}
				else
				{
					rcBounds.right += rcParentBounds.right - m_nOldParentWidth;
				}
			}

			if ((pstControl->dwStyle & (CP_MOVE_VERTICAL)) == (CP_MOVE_VERTICAL))
			{
				if (!m_fNegativeMoves)
				{
					if (rcParentBounds.bottom > m_nOriginalParentHeight)
					{
						if (m_nOriginalParentHeight <= m_nOldParentHeight)
						{
							rcBounds.bottom += rcParentBounds.bottom - m_nOldParentHeight;
							rcBounds.top += rcParentBounds.bottom - m_nOldParentHeight;
						}
						else
						{
							rcBounds.bottom += rcParentBounds.bottom - m_nOriginalParentHeight;
							rcBounds.top += rcParentBounds.bottom - m_nOriginalParentHeight;
						}
					}
					else
					{
						if (m_nOldParentHeight > m_nOriginalParentHeight)
						{
							rcBounds.bottom += m_nOriginalParentHeight - m_nOldParentHeight;
							rcBounds.top += m_nOriginalParentHeight - m_nOldParentHeight;
						}
					}
				}
				else
				{
					rcBounds.bottom += rcParentBounds.bottom - m_nOldParentHeight;
					rcBounds.top += rcParentBounds.bottom - m_nOldParentHeight;
				}
			}

			if ((pstControl->dwStyle & (CP_MOVE_HORIZONTAL)) == (CP_MOVE_HORIZONTAL))
			{
				if (!m_fNegativeMoves)
				{
					if (rcParentBounds.right > m_nOriginalParentWidth)
					{
						if (m_nOriginalParentWidth <= m_nOldParentWidth)
						{
							rcBounds.right += rcParentBounds.right - m_nOldParentWidth;
							rcBounds.left += rcParentBounds.right - m_nOldParentWidth;
						}
						else
						{
							rcBounds.right += rcParentBounds.right - m_nOriginalParentWidth;
							rcBounds.left += rcParentBounds.right - m_nOriginalParentWidth;
						}
					}
					else
					{
						if (m_nOldParentWidth > m_nOriginalParentWidth)
						{
							rcBounds.right += m_nOriginalParentWidth - m_nOldParentWidth;
							rcBounds.left += m_nOriginalParentWidth - m_nOldParentWidth;
						}
					}
				}
				else
				{
					rcBounds.right += rcParentBounds.right - m_nOldParentWidth;
					rcBounds.left += rcParentBounds.right - m_nOldParentWidth;
				}
			}

			m_pParent->ScreenToClient(rcBounds);
			pControl->MoveWindow(rcBounds);
			pControl->Invalidate();
		}

		UpdateParentSize();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：fNegativeMovesフラグ設定												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
//	This sets the NegativeMoves boolean parameter of the object. When the 
//    parent window becomes smaller than it started, setting this to FALSE 
//    will not allow controls to be moved; the parent size may change, but 
//    it'll just force the controls to go off of the 
//    This parameter defaults to FALSE on object creation.
void CAutoResizer::SetNegativeMoves(const BOOL& fNegativeMoves /* = TRUE */)
{
	m_fNegativeMoves = fNegativeMoves;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：fNegativeMovesフラグ取得												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CAutoResizer::GetNegativeMoves(void) const
{
	return (m_fNegativeMoves);
}

/////////////////////////////////////////////////////////////////////////////
// CAutoResizer プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：親ウインドウサイズ更新												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CAutoResizer::UpdateParentSize(void)
{
	if (m_pParent)
	{
		CRect rcBounds;
		m_pParent->GetClientRect(rcBounds);

		m_nOldParentWidth = rcBounds.Width();
		m_nOldParentHeight = rcBounds.Height();
	}
}



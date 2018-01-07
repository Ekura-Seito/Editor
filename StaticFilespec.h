/*
 *  StaticFilespec.h
 *
 *  CStaticFilespec interface
 *    A simple class for displaying long filespecs in static text controls
 *
 *  Usage:          See http://www.codeguru.com/controls/cstatic_filespec.shtml
 *  Author:         Ravi Bhavnani
 *  Edit history:   24-Nov-1997   Ravi B    Original creation
 */

#ifndef _StaticFilespec_h
#define _StaticFilespec_h

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CStaticFilespec : public CWnd
{
public:
	CStaticFilespec
	(DWORD	dwFormat = DT_LEFT | DT_NOPREFIX | DT_VCENTER,
	 BOOL	bPathEllipsis =	FALSE);

public:
	void	SetText(CString strText){ m_strText = strText; Invalidate(); }
	BOOL	IsPath() { return m_bPathEllipsis; }
	void	SetPath	(BOOL bPathEllipsis)  {	m_bPathEllipsis	= bPathEllipsis; } 
	DWORD	GetFormat()	{ return m_dwFormat; } 
	void	SetFormat (DWORD dwFormat) { m_dwFormat	= dwFormat;	} 

	//{{AFX_VIRTUAL(CStaticFilespec)
	//}}AFX_VIRTUAL

public:
	virtual ~CStaticFilespec();

protected:
	//{{AFX_MSG(CStaticFilespec)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL	m_bPathEllipsis;    // flag: draw text as path
	DWORD	m_dwFormat;         // text format
	CString m_strText;
};

#endif // End StaticFilespec.h

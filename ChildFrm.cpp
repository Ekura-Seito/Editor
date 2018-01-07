// ChildFrm.cpp : CChildFrame クラスの動作の定義を行います。
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include <io.h>				// for _access
#include "Editor.h"
#include "Editor_Def.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CChildFrame::CChildFrame()
{
	m_bMaximizeWindow = 0;
	m_pApp = (CEditorApp*)AfxGetApp();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CChildFrame::~CChildFrame()
{
//	//設定保存
//	m_pApp->WriteProfileInt("CChildFrame","bMaximizeWindow",m_bMaximizeWindow);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：OnCreate()実行中にフレームワークから呼び出される処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CChildFrame::OnCreateClient( LPCREATESTRUCT /*lpcs*/,	CCreateContext* pContext)
{
	//スプリッタウインドウ作成
	return m_wndSplitter.Create( this,
		2, 2,                 // TODO: 行と列の数を調整してください。
		CSize( 10, 10 ),      // TODO: 最小の区画サイズを調整してください。
		pContext );
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウ作成前に呼び出される処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs の設定を行って、Window クラスまたは
	//       スタイルを変更してください。

	//設定取得
	m_bMaximizeWindow = m_pApp->GetProfileInt("CChildFrame","bMaximizeWindow",0);
	if( m_bMaximizeWindow != 0 )
	{
		//ＭＤＩの子ウィンドウをはじめから最大化して表示
		cs.style |= (WS_MAXIMIZE | WS_VISIBLE);
	}
	else
	{
		cs.style |= WS_VISIBLE;
		cs.style &= ~(WS_MAXIMIZE);
	}
	return CMDIChildWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame クラスの診断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame クラスのメッセージハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：メッセージフィルタ処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CChildFrame::PreTranslateMessage(MSG* pMsg) 
{
	return CMDIChildWnd::PreTranslateMessage(pMsg);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：システムコマンド選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CChildFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	UINT nItemID = (nID & 0xFFF0);
	CString strPath;

	switch (nItemID)
	{
	case SC_MAXIMIZE:
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowMaximize",0) != 0)
		{
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowMaximize",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
		break;
	case SC_MINIMIZE:
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowMinimize",0) != 0)
		{
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowMinimize",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
		break;
	case SC_RESTORE:
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowRestore",0) != 0)
		{
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowRestore",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
		break;
	case SC_NEXTWINDOW:
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowNext",0) != 0)
		{
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowNext",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
		break;
	case SC_PREVWINDOW:
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowPrev",0) != 0)
		{
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowPrev",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
		break;
	case SC_CLOSE:
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowClose",0) != 0)
		{
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowClose",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
		break;
	default:
		break;
	}

	CMDIChildWnd::OnSysCommand(nID, lParam);
}


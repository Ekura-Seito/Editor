// MyPropertySheet.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <Afxtempl.h>			//ＭＦＣのＣＭａｐを使用する
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include <io.h>					//for _access
#include "Editor_Def.h"
#include "StatusBarDisp.h"				//ステータスバー表示情報構造体
#include "Editor.h"
#include "BitmapClient.h"
#include "Operation.h"
#include "OperationManager.h"
#include "MainFrm.h"
#include "ColorButton.h"
#include "ColorManager.h"
#include "Line.h"
#include "LineManager.h"
#include "UndoItem.h"
#include "UndoManager.h"
#include "EditorDoc.h"
#include "EditorView.h"
#include "MyPropertyPage1.h"
#include "MyPropertyPage2.h"
#include "MyPropertyPage3.h"
#include "MyPropertyPage4.h"
#include "MyPropertyPage5.h"
#include "MyPropertyPage6.h"
#include "MyPropertyPage7.h"
#include "MyPropertyPage8.h"
#include "MyPropertyPage9.h"
#include "MyPropertyPage10.h"
#include "MyPropertyPage11.h"
#include "MyPropertyPage12.h"
#include "MyPropertySheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet

IMPLEMENT_DYNAMIC(CMyPropertySheet, CPropertySheet)

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertySheet::CMyPropertySheet(CWnd* pWndParent)
	 : CPropertySheet(IDS_PROPSHT_CAPTION1, pWndParent)
{
	// この位置にすべてのプロパティ ページを追加してください。
	// ここに表示されている順に画面に表示されます。
	// デフォルトでは、最初のページがアクティブなページです。
	// 他のプロパティ ページをアクティブにさせるには、
	// SetActivePage() を呼び出すのもひとつの方法です。

	m_pApp = (CEditorApp*)AfxGetApp();
	m_pobjMainFrame = NULL;
	m_pobjEditorActiveDoc = NULL;
	m_pobjEditorActiveView = NULL;

	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
	AddPage(&m_Page4);
	AddPage(&m_Page5);
	AddPage(&m_Page6);
	AddPage(&m_Page7);
	AddPage(&m_Page8);
	AddPage(&m_Page9);
	AddPage(&m_Page10);
	AddPage(&m_Page11);
	AddPage(&m_Page12);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertySheet::~CMyPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CMyPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CMyPropertySheet)
	ON_COMMAND(ID_APPLY_NOW, OnApplyNow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CMyPropertySheet::DoModal() 
{
	return CPropertySheet::DoModal();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「適用」ボタン押下時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertySheet::OnApplyNow()
{
	Default();
	if( m_Page1.IsChanged() ) //ページ１に変更あり
	{
		m_pApp->SetShowSplash(m_Page1.m_bShowSplash);
		if(m_Page1.m_lSplashTime>=1 && m_Page1.m_lSplashTime<=10)
			m_pApp->WriteProfileInt("CEditorApp","SplashTime",m_Page1.m_lSplashTime);
		m_pApp->WriteProfileInt("CEditorApp","nMaxMRU",m_Page1.m_nMaxMRU);
		m_pApp->SetMaxMRU(m_Page1.m_nMaxMRU);
		m_pApp->WriteProfileInt("CEditorApp","bIsOpenNewFile",m_Page1.m_bIsOpenNewFile);
		m_pApp->SetOpenNewFile(m_Page1.m_bIsOpenNewFile);
		m_pApp->WriteProfileInt("CEditorApp","IsChdirToOpenFile",m_Page1.m_bIsChdirToOpenFile);
		m_pApp->SetChdirToOpenFile(m_Page1.m_bIsChdirToOpenFile);
		m_pApp->WriteProfileInt("CChildFrame","bMaximizeWindow",m_Page1.m_bMaximizeWindow);
		m_pobjMainFrame->SetSaveWindowPlace( m_Page1.m_bIsSaveWindowPlace );
		m_pApp->WriteProfileInt("CEditorDoc","bIsFullTitle",m_Page1.m_bIsFullTitle);
		m_pobjEditorActiveView->SetFlagDispEOFMark(m_Page1.m_bIsDispEOFMark);
		m_pApp->WriteProfileInt("CEditorApp","bIsNoDoubleExecute",m_Page1.m_bIsNoDoubleExecute);
		if(m_Page1.m_bIsExecPasswd!=FALSE && !m_Page1.m_objExecPasswdStr.IsEmpty())
		{
			m_pApp->WriteProfileInt("CEditorApp","bIsExecPasswd",TRUE);
			m_pApp->WriteProfileString("CEditorApp","ExecPasswd",m_Page1.m_objExecPasswdStr);
		}
		else
		{
			m_pApp->WriteProfileInt("CEditorApp","bIsExecPasswd",FALSE);
			m_pApp->WriteProfileString("CEditorApp","ExecPasswd","");
		}
		m_pApp->WriteProfileInt("CEditorApp","bIsEnableShellOpen",m_Page1.m_bIsEnableShellOpen);
		if(m_Page1.m_bIsEnableShellOpen)
			m_pApp->RegisterShellExtDll();
		else
			m_pApp->UnRegisterShellExtDll();
		m_pApp->WriteProfileInt("CEditorApp","bIsMakeShortcutDsktop",m_Page1.m_bIsMakeShotcutDsktop);
		m_pApp->MakeXerxesShortcut(m_Page1.m_bIsMakeShotcutDsktop);
		m_pApp->WriteProfileInt("CEditorView","IsImmAutoOpen",m_Page1.m_bIsImmAutoOpen);

		m_Page1.ClearChangeFlag();
	}
	if( m_Page2.IsChanged() ) //ページ２に変更あり
	{
		//フォント設定
		m_pApp->SetFontSettings2AllWindow(m_pobjEditorActiveDoc);

		//ステータスバーフォント設定
		m_pobjMainFrame->SetStatusBarFont(m_Page2.m_stStatusBarLogFont);
		//タブサイズ設定
		if(m_Page2.m_nTabSize == 0)//２タブ
			m_pobjEditorActiveDoc->SetTabSize(2);
		else if(m_Page2.m_nTabSize == 1)//４タブ
			m_pobjEditorActiveDoc->SetTabSize(4);
		else					 //８タブ
			m_pobjEditorActiveDoc->SetTabSize(8);

		m_Page2.ClearChangeFlag();
	}
	if( m_Page3.IsChanged() ) //ページ３に変更あり
	{
		//カラー設定
		m_pApp->SetColorSettings2AllWindow();

		m_pobjMainFrame->SetMDIBkgndColor( m_Page3.m_MDIBkgndColor );
		m_pobjEditorActiveView->SetDiffDelLineColor(m_Page3.m_DiffDelLineColor);
		m_pobjEditorActiveView->SetDiffAddLineColor(m_Page3.m_DiffAddLineColor);
		m_pobjEditorActiveView->SetDirDiffLineColor(m_Page3.m_DirDiffLineColor);

		m_Page3.ClearChangeFlag();
	}
	if( m_Page4.IsChanged() ) //ページ４に変更あり
	{
		//改行コード区分設定
		m_pobjEditorActiveDoc->SetKaigyoKind(m_Page4.m_nKaigyoKind);
		//オートセーブ設定
		m_pobjMainFrame->SetFlagAutoSave(m_Page4.m_bIsAutoSave);
		//オートセーブ間隔
		if(m_Page4.m_lSaveInterval>=1 && m_Page4.m_lSaveInterval<=60)
			m_pobjMainFrame->SetSaveInterval(m_Page4.m_lSaveInterval);
		//行末空白削除フラグ
		m_pobjEditorActiveDoc->SetFlagDelEndSpacesOnSave(m_Page4.m_bDelEndSpacesOnSave);
		//空白－＞タブ変換フラグ
		m_pobjEditorActiveDoc->SetFlagSpace2TabOnSave(m_Page4.m_bSpace2TabOnSave);

		m_Page4.ClearChangeFlag();
	}
	if( m_Page5.IsChanged() ) //ページ５に変更あり
	{
		//行番号印刷フラグ
		m_pobjEditorActiveView->SetFlagPrintLineNo(m_Page5.m_bPrintLineNo);
		//２段組み印刷フラグ
		m_pobjEditorActiveView->SetFlagPrint2Dangumi(m_Page5.m_bPrint2Dangumi);
		//印刷用紙方向
		m_pApp->SetPrintOrientation(m_Page5.m_nPrintOrientation);
		//印刷余白
		CRect objMarginRect;
		objMarginRect.left   = m_Page5.m_nYohakuHidari;
		objMarginRect.top    = m_Page5.m_nYohakuUe;
		objMarginRect.right  = m_Page5.m_nYohakuMigi;
		objMarginRect.bottom = m_Page5.m_nYohakuSita;
		objMarginRect.NormalizeRect();
		m_pApp->SetPrintMargin(objMarginRect);
		//ヘッダ＆フッタ位置
		m_pobjEditorActiveView->SetPrintHeaderLayoutPos(m_Page5.m_nHeaderLayout);
		m_pobjEditorActiveView->SetPrintFooterLayoutPos(m_Page5.m_nFooterLayout);
		//ヘッダ＆フッタのフォーマット
		m_pobjEditorActiveView->SetStrHeaderFormat(m_Page5.m_objStrHeaderFormat);
		m_pobjEditorActiveView->SetStrFooterFormat(m_Page5.m_objStrFooterFormat);

		m_Page5.ClearChangeFlag();
	}
	if( m_Page6.IsChanged() ) //ページ６に変更あり
	{
		//キャレット形状設定
		if(m_Page6.m_nCaretType == 0)//縦型キャレット
			m_pobjEditorActiveView->SetCaretType(CARET_IBEAM);
		else if(m_Page6.m_nCaretType == 1)//箱型キャレット
			m_pobjEditorActiveView->SetCaretType(CARET_SQUARE);
		//キャレット点滅間隔設定
		::SetCaretBlinkTime(m_Page6.m_unCaretBlinkTime);
		m_pobjEditorActiveView->HoldCaretBlinkTime(m_Page6.m_unCaretBlinkTime);
		//キャレットの振る舞い設定
		if(m_Page6.m_nCaretBehave == 0)//画面スクロール時に行位置変更
			m_pobjEditorActiveView->SetCaretBehave(CARET_BEHAVE_MOVE);
		else if(m_Page6.m_nCaretBehave == 1)//画面スクロール時に行位置保持
			m_pobjEditorActiveView->SetCaretBehave(CARET_BEHAVE_STAY);
		//キャレット移動タイプ設定
		m_pobjEditorActiveView->SetCaretMoveType(m_Page6.m_nCaretMoveType);
		//キャレット加速スピード設定
		m_pApp->WriteProfileInt("CEditorView","nMultValueOfCaretAccelarete",m_Page6.m_nMultValueOfCaretAccelarete+1);
		//貼り付け後のキャレット位置
		m_pApp->WriteProfileInt("CEditorDoc","bSetCaretToEndOnPaste",m_Page6.m_nSetCaretToEndOnPaste);

		m_Page6.ClearChangeFlag();
	}
	if( m_Page7.IsChanged() ) //ページ７に変更あり
	{
		m_pobjMainFrame->SetFlagDispLogoBmp( m_Page7.m_bDispLogoBmp );
		m_pobjMainFrame->SetLogoBmpKind(m_Page7.m_nLogoBmpKind);
		m_pobjMainFrame->SetLogoBmpFilePath(m_Page7.m_objBmpFilePath);
		m_pobjMainFrame->SetLogoPosKind(m_Page7.m_nLogoPosKind);
		if(m_Page7.m_unLogoPosX>=0 && m_Page7.m_unLogoPosX<=100)
			m_pobjMainFrame->SetLogoPosX( m_Page7.m_unLogoPosX );
		if(m_Page7.m_unLogoPosY>=0 && m_Page7.m_unLogoPosY<=100)
			m_pobjMainFrame->SetLogoPosY( m_Page7.m_unLogoPosY );

		m_Page7.ClearChangeFlag();
	}
	if( m_Page8.IsChanged() ) //ページ８に変更あり
	{
		m_pobjMainFrame->SetStatusBarDispInfo(&m_Page8.m_stStatusBarDisp);
		m_pobjMainFrame->SetStatusDayStyle(m_Page8.m_lStatusDayStyle);
		m_pobjMainFrame->SetStatusHourStyle(m_Page8.m_lStatusHourStyle);
		m_pobjMainFrame->SetStatusLineStyle(m_Page8.m_lStatusLineStyle);

		m_Page8.ClearChangeFlag();
	}
	if( m_Page9.IsChanged() ) //ページ９に変更あり
	{
		m_Page9.ChangesToRegistry();
		m_Page9.ClearChangeFlag();
	}
	if( m_Page11.IsChanged() ) //ページ１１に変更あり
	{
		m_Page11.ChangesToRegistry();
		m_Page11.ClearChangeFlag();
	}
	if( m_Page12.IsChanged() ) //ページ１２に変更あり
	{
		m_pobjMainFrame->SetHtmlOpenParameter( m_Page12.m_bIsUseProxy,
											   m_Page12.m_objProxyIPAddress,
											   m_Page12.m_bIsDialup,
											   m_Page12.m_objDialupEntry,
											   m_Page12.m_objRasUserName,
											   m_Page12.m_objRasPasswd,
											   m_Page12.m_bIsAutoDisconnect );
	}
}


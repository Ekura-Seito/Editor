// MainFrm.cpp : CMainFrame クラスの動作の定義を行います。
//

#include "stdafx.h"
#include <Afxtempl.h>			//ＭＦＣのＣＭａｐを使用する
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include <io.h>					//for _access()
#pragma warning(disable:4786)
#include <map>					// for map
using namespace std;
//#include "zmouse.h"				//for インテリマウス
#include "Editor_Def.h"			//共通ＤＥＦＩＮＥ
#include "StatusBarDisp.h"		//ステータスバー表示情報構造体
#include "ColorManager.h"
#include "Editor.h"
#include "ChildFrm.h"
#include "BitmapClient.h"
#include "Operation.h"
#include "OperationManager.h"
#include "MainFrm.h"
#include "ColorButton.h"
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
#include "Line.h"
#include "LineManager.h"
#include "UndoItem.h"
#include "UndoManager.h"
#include "EditorDoc.h"
#include "EditorView.h"
#include "MyComboBoxEx.h"
#include "StaticFilespec.h"
#include "AutoResizer.h"
#include "GrepDlg.h"
#include "GrepStatusDlg.h"
#include "MyComboBoxEx2.h"
#include "CodeConvDlg.h"
#include "CodeConvStatusDlg.h"
#include "KaigyoConvDlg.h"
#include "KaigyoConvStatusDlg.h"
#include "CodeConvManager.h"
#include "StaticFilespec.h"
#include "FilePropertyDlg.h"
#include "WordCountDlg.h"
#include "WCntStatusDlg.h"
#include "FileSearchDlg.h"
#include "FileSearchStatusDlg.h"
#include "Splash.h"
#include "ClipBoardViewDlg.h"
#include "EncriptDlg.h"
#include "EncriptStatusDlg.h"
#include "DecriptDlg.h"
#include "DecriptStatusDlg.h"
//#include "MyFeal.h"
#include "MyDes.h"
#include "Uuencoder.h"
#include "Base64.h"
#include "QuotedPrintable.h"
#include "FileCompareStatusDlg.h"
#include "FileCompareResultDlg.h"
#include "FileCompare.h"
#include "DirCompareDlg.h"
#include "DirCompareStatusDlg.h"
#include "DirCompareResultDlg.h"
#include "DirCompare.h"
#include "DateMaskEdit.h"
#include "TimeMaskEdit.h"
#include "FileTouchDlg.h"
#include "FindDlg.h"
#include "ReplaceDlg.h"
#include "MailListStatusDlg.h"
#include "ListCtrlEx.h"
#include "MailListDlg.h"
#include "Pop.h"
#include "KeyBind.h"
#include "BasicAuthorizationDlg.h"
#include "HtmlOpenDlg.h"
#include "HtmlOpenStatusDlg.h"
#include "HtmlOpenResultDlg.h"
#include "Http.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////検索、置換用登録済みメッセージ取得
//static UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);
//インテリマウス対応
static UINT nMyMouseWheel  = ::RegisterWindowMessage(MSH_MOUSEWHEEL);


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_SETTINGS, OnSettings)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS, OnUpdateSettings)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateEditReplace)
	ON_COMMAND(ID_EDIT_FINDCONTINUE, OnEditFindcontinue)
	ON_COMMAND(ID_EDIT_FINDREVCONTINUE, OnEditFindrevcontinue)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FINDCONTINUE, OnUpdateEditFindcontinue)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FINDREVCONTINUE, OnUpdateEditFindrevcontinue)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_WINDOW_ALLCLOSE, OnUpdateWindowAllclose)
	ON_COMMAND(ID_WINDOW_ALLCLOSE, OnWindowAllclose)
	ON_WM_MEASUREITEM()
	ON_WM_MENUCHAR()
	ON_WM_INITMENUPOPUP()
	ON_UPDATE_COMMAND_UI(ID_EDIT_GREP, OnUpdateEditGrep)
	ON_COMMAND(ID_EDIT_GREP, OnEditGrep)
	ON_WM_ACTIVATE()
	ON_UPDATE_COMMAND_UI(ID_EDIT_CODECONV, OnUpdateEditCodeconv)
	ON_COMMAND(ID_EDIT_CODECONV, OnEditCodeconv)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ASCIIDUMP, OnUpdateEditAsciidump)
	ON_COMMAND(ID_EDIT_ASCIIDUMP, OnEditAsciidump)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SJISDUMP, OnUpdateEditSjisdump)
	ON_COMMAND(ID_EDIT_SJISDUMP, OnEditSjisdump)
	ON_UPDATE_COMMAND_UI(ID_FILE_REREAD, OnUpdateFileReread)
	ON_COMMAND(ID_FILE_REREAD, OnFileReread)
	ON_UPDATE_COMMAND_UI(ID_FILE_PROPERTY, OnUpdateFileProperty)
	ON_COMMAND(ID_FILE_PROPERTY, OnFileProperty)
	ON_COMMAND(ID_EDIT_SETREADONLY, OnEditSetreadonly)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SETREADONLY, OnUpdateEditSetreadonly)
	ON_COMMAND(ID_EDIT_UNSETREADONLY, OnEditUnsetreadonly)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNSETREADONLY, OnUpdateEditUnsetreadonly)
	ON_COMMAND(ID_EDIT_WORDCOUNT, OnEditWordcount)
	ON_UPDATE_COMMAND_UI(ID_EDIT_WORDCOUNT, OnUpdateEditWordcount)
	ON_COMMAND(ID_EDIT_FILESEARCH, OnEditFilesearch)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FILESEARCH, OnUpdateEditFilesearch)
	ON_COMMAND(ID_FILE_SAVECLOSE, OnFileSaveclose)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVECLOSE, OnUpdateFileSaveclose)
	ON_COMMAND(ID_TOOL_OPE_STARTENDRECORD, OnToolOpeStartendrecord)
	ON_UPDATE_COMMAND_UI(ID_TOOL_OPE_STARTENDRECORD, OnUpdateToolOpeStartendrecord)
	ON_COMMAND(ID_TOOL_OPE_DO, OnToolOpeDo)
	ON_UPDATE_COMMAND_UI(ID_TOOL_OPE_DO, OnUpdateToolOpeDo)
	ON_COMMAND(ID_TOOL_OPE_SAVE, OnToolOpeSave)
	ON_UPDATE_COMMAND_UI(ID_TOOL_OPE_SAVE, OnUpdateToolOpeSave)
	ON_COMMAND(ID_TOOL_OPE_OPEN, OnToolOpeOpen)
	ON_UPDATE_COMMAND_UI(ID_TOOL_OPE_OPEN, OnUpdateToolOpeOpen)
	ON_COMMAND(ID_TOOL_OPE_DISP, OnToolOpeDisp)
	ON_UPDATE_COMMAND_UI(ID_TOOL_OPE_DISP, OnUpdateToolOpeDisp)
	ON_COMMAND(ID_TOOL_OPE_STEPDO_NEXT, OnToolOpeStepdoNext)
	ON_UPDATE_COMMAND_UI(ID_TOOL_OPE_STEPDO_NEXT, OnUpdateToolOpeStepdoNext)
	ON_COMMAND(ID_TOOL_OPE_STEPDO_END, OnToolOpeStepdoEnd)
	ON_UPDATE_COMMAND_UI(ID_TOOL_OPE_STEPDO_END, OnUpdateToolOpeStepdoEnd)
	ON_COMMAND(ID_WND_NEXT, OnWndNext)
	ON_UPDATE_COMMAND_UI(ID_WND_NEXT, OnUpdateWndNext)
	ON_COMMAND(ID_WND_PREV, OnWndPrev)
	ON_UPDATE_COMMAND_UI(ID_WND_PREV, OnUpdateWndPrev)
	ON_COMMAND(ID_EDIT_CLIPVIEW, OnEditClipview)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLIPVIEW, OnUpdateEditClipview)
	ON_COMMAND(ID_EDIT_INCRIMENTALSEARCH, OnEditIncrimentalsearch)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INCRIMENTALSEARCH, OnUpdateEditIncrimentalsearch)
	ON_COMMAND(ID_SECURITY_ENCRIPT, OnSecurityEncript)
	ON_UPDATE_COMMAND_UI(ID_SECURITY_ENCRIPT, OnUpdateSecurityEncript)
	ON_COMMAND(ID_SECURITY_DECRIPT, OnSecurityDecript)
	ON_UPDATE_COMMAND_UI(ID_SECURITY_DECRIPT, OnUpdateSecurityDecript)
	ON_COMMAND(ID_EDIT_UUENCODE, OnEditUuencode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UUENCODE, OnUpdateEditUuencode)
	ON_COMMAND(ID_EDIT_BASE64ENCODE, OnEditBase64encode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BASE64ENCODE, OnUpdateEditBase64encode)
	ON_COMMAND(ID_EDIT_QUOTEDENCODE, OnEditQuotedencode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_QUOTEDENCODE, OnUpdateEditQuotedencode)
	ON_COMMAND(ID_EDIT_FILECOMPARE, OnEditFilecompare)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FILECOMPARE, OnUpdateEditFilecompare)
	ON_COMMAND(ID_EDIT_DIRCOMPARE, OnEditDircompare)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DIRCOMPARE, OnUpdateEditDircompare)
	ON_COMMAND(ID_FILE_DELETE, OnFileDelete)
	ON_UPDATE_COMMAND_UI(ID_FILE_DELETE, OnUpdateFileDelete)
	ON_COMMAND(ID_EDIT_FILETOUCH, OnEditFiletouch)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FILETOUCH, OnUpdateEditFiletouch)
	ON_COMMAND(ID_WINDOW_RESTORE, OnWindowRestore)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_RESTORE, OnUpdateWindowRestore)
	ON_COMMAND(ID_WINDOW_MINIMIZE, OnWindowMinimize)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_MINIMIZE, OnUpdateWindowMinimize)
	ON_COMMAND(ID_WINDOW_MAXIMIZE, OnWindowMaximize)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_MAXIMIZE, OnUpdateWindowMaximize)
	ON_COMMAND(ID_VIEW_TOOLBAR_WND, OnViewToolbarWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_WND, OnUpdateViewToolbarWnd)
	ON_COMMAND(ID_VIEW_TOOLBAR_TOOLS, OnViewToolbarTools)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_TOOLS, OnUpdateViewToolbarTools)
	ON_COMMAND(ID_VIEW_TOOLBAR_FIND, OnViewToolbarFind)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_FIND, OnUpdateViewToolbarFind)
	ON_COMMAND(ID_VIEW_TOOLBAR_EDIT, OnViewToolbarEdit)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_EDIT, OnUpdateViewToolbarEdit)
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateViewToolbar)
	ON_COMMAND(ID_VIEW_TOOLBAR_JMP, OnViewToolbarJmp)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_JMP, OnUpdateViewToolbarJmp)
	ON_COMMAND(ID_FILE_MAILLIST, OnFileMaillist)
	ON_UPDATE_COMMAND_UI(ID_FILE_MAILLIST, OnUpdateFileMaillist)
	ON_COMMAND(ID_EDIT_KAIGYOCONV, OnEditKaigyoconv)
	ON_UPDATE_COMMAND_UI(ID_EDIT_KAIGYOCONV, OnUpdateEditKaigyoconv)
	ON_COMMAND(ID_CUSTOMIZE_TOOLBAR, OnCustomizeToolbar)
	ON_UPDATE_COMMAND_UI(ID_CUSTOMIZE_TOOLBAR, OnUpdateCustomizeToolbar)
	ON_COMMAND(ID_CUSTOMIZE_TOOLBAREDIT, OnCustomizeToolbaredit)
	ON_UPDATE_COMMAND_UI(ID_CUSTOMIZE_TOOLBAREDIT, OnUpdateCustomizeToolbaredit)
	ON_COMMAND(ID_CUSTOMIZE_TOOLBARFIND, OnCustomizeToolbarfind)
	ON_UPDATE_COMMAND_UI(ID_CUSTOMIZE_TOOLBARFIND, OnUpdateCustomizeToolbarfind)
	ON_COMMAND(ID_CUSTOMIZE_TOOLBARJMP, OnCustomizeToolbarjmp)
	ON_UPDATE_COMMAND_UI(ID_CUSTOMIZE_TOOLBARJMP, OnUpdateCustomizeToolbarjmp)
	ON_COMMAND(ID_CUSTOMIZE_TOOLBARTOOLS, OnCustomizeToolbartools)
	ON_UPDATE_COMMAND_UI(ID_CUSTOMIZE_TOOLBARTOOLS, OnUpdateCustomizeToolbartools)
	ON_COMMAND(ID_CUSTOMIZE_TOOLBARWND, OnCustomizeToolbarwnd)
	ON_UPDATE_COMMAND_UI(ID_CUSTOMIZE_TOOLBARWND, OnUpdateCustomizeToolbarwnd)
	ON_WM_SIZE()
	ON_COMMAND(ID_WINDOW_CLOSE, OnWindowClose)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_CLOSE, OnUpdateWindowClose)
	ON_COMMAND(ID_HTTP_FILE_OPEN, OnHttpFileOpen)
	ON_COMMAND(ID_EDIT_FCOMP_ALPHABETBIGANDSMALL, OnEditFcompAlphabetbigandsmall)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FCOMP_ALPHABETBIGANDSMALL, OnUpdateEditFcompAlphabetbigandsmall)
	ON_COMMAND(ID_EDIT_FCOMP_TABEQUAL2SPACE, OnEditFcompTabequal2space)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FCOMP_TABEQUAL2SPACE, OnUpdateEditFcompTabequal2space)
	ON_COMMAND(ID_EDIT_FCOMP_ZENEQUAL2HAN, OnEditFcompZenequal2han)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FCOMP_ZENEQUAL2HAN, OnUpdateEditFcompZenequal2han)
	ON_COMMAND(ID_EDIT_FILERECOMPARE, OnEditFilerecompare)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FILERECOMPARE, OnUpdateEditFilerecompare)
	//}}AFX_MSG_MAP
//	// 検索、置換用
//	ON_REGISTERED_MESSAGE(WM_FINDREPLACE,OnFindReplace)
	// インテリマウス対応
	ON_REGISTERED_MESSAGE(nMyMouseWheel,OnMSHMouseWheel)
END_MESSAGE_MAP()


//static UINT indicators[] =
//{
//	ID_SEPARATOR,           // ステータス ライン インジケータ
//	ID_INDICATOR_GYORETU,
//	ID_INDICATOR_BYTE,
//	ID_INDICATOR_INS,
//	ID_INDICATOR_READONLY,
//	ID_INDICATOR_TIME,
//	ID_INDICATOR_CHARCODE,
//	ID_INDICATOR_KANA,
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
//};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMainFrame::CMainFrame()
{
	int	nCnt=0;
	int nBuf=0;
	char	szKey[64];

	m_pApp = (CEditorApp*)AfxGetApp();

	//////////////////////////
	//	メンバ変数初期化	//
	//////////////////////////

	// WEB上のHTMLファイルを開く関連
	m_bIsUseProxyHtml = FALSE;			// プロキシサーバ経由か？
	m_objRequestPathHtml = _T("");		// 要求パス
	m_objProxyIPAddressHtml = _T("");	// プロキシサーバのＩＰアドレス
	m_bIsDialupHtml = FALSE;			// ダイアルアップ接続するか？
	m_objDialupEntryHtml = _T("");		// 使用するダイアルアップエントリ
	m_objRasUserNameHtml = _T("");		// ユーザ名
	m_objRasPasswdHtml = _T("");		// パスワード
	m_bIsAutoDisconnectHtml = FALSE;	// 自動切断するか？
	m_pobjHtmlOpenStatusDlg = NULL;		// 状況ダイアログ

	//検索関連
	m_objFindString = _T("");			// 前回検索時の検索文字列
	m_bIsMatchCaseFind = 0;				// 大文字、小文字区別の有無
	m_bIsMatchWholeWordFind = 0;		// 単語全体が一致する事が条件か？
	m_bIsSearchDownFind = 0;			// 検索方向
	m_bIsUseRegularExpressionFind = 0;	// 正規表現を使用するか？

	//置換関連
	m_objReplaceString = _T("");		// 置換文字列
	m_bIsMatchCaseReplace = 0;			// 大文字、小文字区別の有無
	m_bIsMatchWholeWordReplace = 0;		// 単語全体が一致する事が条件か？
	m_bIsNowIncrimentalSearching = 0;	// インクリメンタルサーチ実行中フラグ
	m_bIsUseRegularExpressionReplace = 0;// 正規表現を使用するか？

	// ＧＲＥＰ関連
	m_bIsMatchCaseGrep = 0;				// 大文字、小文字区別の有無
	m_bIsMatchWholeWordGrep = 0;		// 単語全体が一致する事が条件か？
	m_bIsSearchSubFolderGrep = 0;		// サブフォルダも検索するか？
	m_bIsUseRegularExpressionGrep = 0;	// 正規表現を使用するか？
	m_objFolderGrep = _T("");			// ＧＲＥＰ実行フォルダ
	m_objGrepStr = _T("");				// 検索文字列
	m_objFileKindGrep = _T("");			// 検索対象のファイル種別
	m_pobjGrepDoc = NULL;				// ＧＲＥＰ用ドキュメントクラス
	m_pobjGrepFrameWnd = NULL;			// ＧＲＥＰ用子ウインドウクラス
	m_pobjGrepStatusDlg = NULL;			// ＧＲＥＰ状況ダイアログ

	// 文字コード変換関連
	m_bIsSearchSubFolderConv = 0;		// サブフォルダも変換するか？
	m_bIsOutOtherFileConv = 0;			// 変換結果を別ファイルへ出力するか？
	m_nConvKind = 0;					// 文字コード変換種別
	m_objFolderConv = _T("");			// 文字コード変換実行フォルダ
	m_objFileKindConv = _T("");			// 変換対象のファイル種別
	m_nInCodeConv = 0;					// 入力ファイルの文字コード判定種別
	m_pobjConvDoc = NULL;				// 文字コード変換用ドキュメントクラス
	m_pobjConvFrameWnd = NULL;			// 文字コード変換用ウインドウクラス
	m_pobjConvStatusDlg = NULL;			// 文字コード変換状況ダイアログ

	// 改行コード変換関連
	m_bIsSearchSubFolderKaigyoConv = 0;	// サブフォルダも変換するか？
	m_bIsOutOtherFileKaigyoConv = 0;	// 変換結果を別ファイルへ出力するか？
	m_objKaigyoConvKind = _T("");		// 改行コード変換種別
	m_objFolderKaigyoConv = _T("");		// 改行コード変換実行フォルダ
	m_objFileKindKaigyoConv = _T("");	// 変換対象のファイル種別
	m_pobjKaigyoConvDoc = NULL;			// 改行コード変換用ドキュメントクラス
	m_pobjKaigyoConvFrameWnd = NULL;	// 改行コード変換用ウインドウクラス
	m_pobjKaigyoConvStatusDlg = NULL;	// 改行コード変換状況ダイアログ

	// ファイル暗号化関連
	m_bIsSearchSubFolderEncript = 0;	// サブフォルダも変換するか？
	m_bIsOutOtherFileEncript = 0;		// 暗号結果を別ファイルへ出力するか？
	m_objKeyEncript = _T("");			// ファイル暗号化キー
	m_objFolderEncript = _T("");		// ファイル暗号化実行フォルダ
	m_objFileKindEncript = _T("");		// ファイル暗号化対象のファイル種別
	m_pobjEncriptDoc = NULL;			// ファイル暗号化用ドキュメントクラス
	m_pobjEncriptFrameWnd = NULL;		// ファイル暗号化用ウインドウクラス
	m_pobjEncriptStatusDlg = NULL;		// ファイル暗号化状況ダイアログ

	// ファイル復号化関連
	m_bIsSearchSubFolderDecript = 0;	// サブフォルダも変換するか？
	m_bIsOutOtherFileDecript = 0;		// 復号結果を別ファイルへ出力するか？
	m_objKeyDecript = _T("");			// ファイル復号化キー
	m_objFolderDecript = _T("");		// ファイル復号化実行フォルダ
	m_objFileKindDecript = _T("");		// ファイル復号化対象のファイル種別
	m_pobjDecriptDoc = NULL;			// ファイル復号化用ドキュメントクラス
	m_pobjDecriptFrameWnd = NULL;		// ファイル復号化用ウインドウクラス
	m_pobjDecriptStatusDlg = NULL;		// ファイル復号化状況ダイアログ

	// ダンプ関連
	m_pobjDumpDoc = NULL;				// ダンプ用ドキュメントクラス
	m_pobjDumpFrameWnd = NULL;			// ダンプ用ウインドウクラス

	// ワードカウント関連
	m_lByteCountSum = 0;				// バイト数合計
	m_lWordCountSum = 0;				// ワード数合計
	m_lLineCountSum = 0;				// 行数合計
	m_lEmptyLineCountSum = 0;			// 空行数合計
	m_lCPPCommentLineCountSum = 0;		// Ｃ・Ｃ＋＋コメント行数合計
	m_lWCountFileCnt = 0;				// ワードカウント対象ファイル数
	m_bIsSearchSubFolderWCount = 0;		// サブフォルダも検索するか？
	m_objFolderWCount = _T("");			// ワードカウント実行フォルダ
	m_objFileKindWCount = _T("");		// ワードカウント対象のファイル種別
	m_pobjWCountDoc = NULL;				// ワードカウント用ドキュメントクラス
	m_pobjWCountFrameWnd = NULL;		// ワードカウント用子ウインドウクラス

	// ファイル検索関連
	m_bIsSearchSubFolderFSearch = 0;	// サブフォルダも変換するか？
	m_objFolderFSearch = _T("");		// ファイル検索変換実行フォルダ
	m_objFileKindFSearch = _T("");		// 検索対象のファイル種別
	m_pobjFSearchDoc = NULL;			// ファイル検索用ドキュメントクラス
	m_pobjFSearchFrameWnd = NULL;		// ファイル検索用ウインドウクラス
	m_pobjFileSearchStatusDlg = NULL;	// ファイル検索状況ダイアログ

	// ファイルタッチ関連
	m_bIsSearchSubFolderTouch = 0;		// サブフォルダもタッチするか？
	m_objFolderTouch = _T("");			// タッチ実行フォルダ
	m_objFileKindTouch = _T("");		// タッチ対象のファイル種別

	// オートセーブ関連
	m_bIsAutoSave = 0;					// オートセーブをするか？
	m_lSaveInterval = 0;				// オートセーブ間隔（分単位）
	m_lTimeUpCount = 0;					// タイムアップカウンタ

	// キー操作の記録／再生関連
	m_bIsNowOperationRecording = 0;		// キー操作記録中フラグ
	m_bIsNowStepDoOperation = 0;		// キー操作のステップ実行中フラグ
	m_objOperationManager.Initialize();	// キー操作マネージャ

	// ステータスバー関連
	memset(&m_stStatusBarLogFont,0,sizeof(LOGFONT));		// ステータスバーのLOGFONT構造体
	memset(&m_stStatusBarDisp,0,sizeof(STATUSBARDISP));	// ステータスバー表示情報構造体
	m_lStatusDayStyle = 0;				// ステータスバーへ表示する日付表示スタイル
	m_lStatusHourStyle = 0;				// ステータスバーへ表示する時刻表示スタイル
	m_lStatusLineStyle = 0;				// ステータスバーへ表示する行、列表示スタイル
	m_lSavedGyo = 0;					// 行番号
	m_lSavedRetu = 0;					// 列番号
	m_lSavedMaxGyo = 0;					// 全行数

	// ファイル比較間連
	m_pobjFileCompareStatusDlg = NULL;	// ファイル比較状況ダイアログ

	// ディレクトリ比較関連
	m_pobjDirCompareStatusDlg = NULL;	// ディレクトリ比較状況ダイアログ
	m_objFolder1DirCmp = _T("");		// 第一比較フォルダ
	m_objFolder2DirCmp = _T("");		// 第二比較フォルダ
	m_pobjDirCmpDoc = NULL;				// ディレクトリ比較用ドキュメントクラス
	m_pobjDirCmpFrameWnd = NULL;		// ディレクトリ比較用ウインドウクラス

	//////////////////////////////
	//	レジストリから設定取得	//
	//////////////////////////////
	m_bIsSaveWindowPlace        = m_pApp->GetProfileInt("CMainFrame","bIsSaveWindowPlace",0);
	m_bIsUseProxyHtml           = m_pApp->GetProfileInt("CMainFrame","bIsUseProxyHtml",0);
	m_objRequestPathHtml        = m_pApp->GetProfileString("CMainFrame","RequestPathHtml","");
	m_objProxyIPAddressHtml     = m_pApp->GetProfileString("CMainFrame","ProxyIPAddress","");
	m_bIsDialupHtml             = m_pApp->GetProfileInt("CMainFrame","bIsDialupHtml",0);
	m_objDialupEntryHtml        = m_pApp->GetProfileString("CMainFrame","DialupEntryHtml","");
	m_objRasUserNameHtml        = m_pApp->GetProfileString("CMainFrame","RasUserNameHtml","");
	m_objRasPasswdHtml          = m_pApp->GetProfileString("CMainFrame","RasPasswdHtml","");
	m_bIsAutoDisconnectHtml     = m_pApp->GetProfileInt("CMainFrame","bIsAutoDisconnectHtml",0);
	m_objFindString             = m_pApp->GetProfileString("CMainFrame","FindString","");
	m_bIsMatchCaseFind          = m_pApp->GetProfileInt("CMainFrame","bIsMatchCaseFind",0);
	m_bIsMatchWholeWordFind     = m_pApp->GetProfileInt("CMainFrame","bIsMatchWholeWordFind",0);
	m_bIsSearchDownFind         = m_pApp->GetProfileInt("CMainFrame","bIsSearchDownFind",TRUE);
	m_bIsUseRegularExpressionFind = m_pApp->GetProfileInt("CMainFrame","bIsUseRegularExpressionFind",0);
	m_objReplaceString          = m_pApp->GetProfileString("CMainFrame","ReplaceString","");
	m_bIsMatchCaseReplace       = m_pApp->GetProfileInt("CMainFrame","bIsMatchCaseReplace",0);
	m_bIsMatchWholeWordReplace  = m_pApp->GetProfileInt("CMainFrame","bIsMatchWholeWordReplace",0);
	m_bIsUseRegularExpressionReplace= m_pApp->GetProfileInt("CMainFrame","bIsUseRegularExpressionReplace",0);
	m_bIsMatchCaseGrep		    = m_pApp->GetProfileInt("CMainFrame","bIsMatchCaseGrep",0);
	m_bIsMatchWholeWordGrep     = m_pApp->GetProfileInt("CMainFrame","bIsMatchWholeWordGrep",0);
	m_bIsSearchSubFolderGrep    = m_pApp->GetProfileInt("CMainFrame","bIsSearchSubFolderGrep",0);
	m_bIsUseRegularExpressionGrep= m_pApp->GetProfileInt("CMainFrame","bIsUseRegularExpressionGrep",0);
	m_objFolderGrep			    = m_pApp->GetProfileString("CMainFrame","FolderGrep","");
	m_objGrepStr			    = m_pApp->GetProfileString("CMainFrame","GrepStr","");
	m_objFileKindGrep		    = m_pApp->GetProfileString("CMainFrame","FileKindGrep","");
	m_bIsSearchSubFolderConv    = m_pApp->GetProfileInt("CMainFrame","bIsSearchSubFolderConv",0);
	m_bIsOutOtherFileConv	    = m_pApp->GetProfileInt("CMainFrame","bIsOutOtherFileConv",1);
	m_nConvKind	  		        = m_pApp->GetProfileInt("CMainFrame","nConvKind",0);
	m_objFolderConv			    = m_pApp->GetProfileString("CMainFrame","FolderConv","");
	m_objFileKindConv		    = m_pApp->GetProfileString("CMainFrame","FileKindConv","");
	m_nInCodeConv  		        = m_pApp->GetProfileInt("CMainFrame","InCodeConv",0);
	m_bIsSearchSubFolderKaigyoConv = m_pApp->GetProfileInt("CMainFrame","bIsSearchSubFolderKaigyoConv",0);
	m_bIsOutOtherFileKaigyoConv	= m_pApp->GetProfileInt("CMainFrame","bIsOutOtherFileKaigyoConv",1);
	m_objKaigyoConvKind		    = m_pApp->GetProfileString("CMainFrame","KaigyoConvKind","");
	m_objFolderKaigyoConv		= m_pApp->GetProfileString("CMainFrame","FolderKaigyoConv","");
	m_objFileKindKaigyoConv		= m_pApp->GetProfileString("CMainFrame","FileKindKaigyoConv","");
	m_bIsSearchSubFolderWCount  = m_pApp->GetProfileInt("CMainFrame","bIsSearchSubFolderWCount",0);
	m_objFolderWCount		    = m_pApp->GetProfileString("CMainFrame","FolderWCount","");
	m_objFileKindWCount		    = m_pApp->GetProfileString("CMainFrame","FileKindWCount","");
	m_bIsAutoSave 			    = m_pApp->GetProfileInt("CMainFrame","bIsAutoSave",0);
	m_lSaveInterval 		    = m_pApp->GetProfileInt("CMainFrame","lSaveInterval",0);
	m_bIsSearchSubFolderFSearch = m_pApp->GetProfileInt("CMainFrame","bIsSearchSubFolderFSearch",0);
	m_objFolderFSearch          = m_pApp->GetProfileString("CMainFrame","FolderFSearch","");
	m_objFileKindFSearch        = m_pApp->GetProfileString("CMainFrame","FileKindFSearch","");
	m_lStatusDayStyle           = m_pApp->GetProfileInt("CMainFrame","StatusDayStyle",STS_DAYSTYLE_YYYYMMDD);
	m_lStatusHourStyle          = m_pApp->GetProfileInt("CMainFrame","StatusHourStyle",STS_HOURSTYLE_HHMM);
	m_lStatusLineStyle          = m_pApp->GetProfileInt("CMainFrame","StatusLineStyle",STS_LINESTYLE_LINE_OFFS_ALL);
	m_bIsSearchSubFolderEncript = m_pApp->GetProfileInt("CMainFrame","bIsSearchSubFolderEncript",0);
	m_bIsOutOtherFileEncript    = m_pApp->GetProfileInt("CMainFrame","bIsOutOtherFileEncript",1);
	m_objFolderEncript          = m_pApp->GetProfileString("CMainFrame","FolderEncript","");
	m_objFileKindEncript        = m_pApp->GetProfileString("CMainFrame","FileKindEncript","");
	m_bIsSearchSubFolderDecript = m_pApp->GetProfileInt("CMainFrame","bIsSearchSubFolderDecript",0);
	m_bIsOutOtherFileDecript    = m_pApp->GetProfileInt("CMainFrame","bIsOutOtherFileDecript",1);
	m_objFolderDecript          = m_pApp->GetProfileString("CMainFrame","FolderDecript","");
	m_objFileKindDecript        = m_pApp->GetProfileString("CMainFrame","FileKindDecript","");
	m_objFolder1DirCmp          = m_pApp->GetProfileString("CMainFrame","Folder1DirCmp","");
	m_objFolder2DirCmp          = m_pApp->GetProfileString("CMainFrame","Folder2DirCmp","");
	m_bIsSearchSubFolderTouch   = m_pApp->GetProfileInt("CMainFrame","bIsSearchSubFolderTouch",0);
	m_objFolderTouch            = m_pApp->GetProfileString("CMainFrame","FolderTouch","");
	m_objFileKindTouch          = m_pApp->GetProfileString("CMainFrame","FileKindTouch","");

	//現在のステータスバー表示情報取得
	m_stStatusBarDisp.m_nCurrentItemCount=0;
	for(nCnt=0; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		sprintf(szKey,"CurrentItem%d",nCnt);
		nBuf = m_pApp->GetProfileInt("CMainFrame",szKey,0);
		if(nBuf != 0)
		{
			m_stStatusBarDisp.m_nCurrentItems[m_stStatusBarDisp.m_nCurrentItemCount] = nBuf;
			m_stStatusBarDisp.m_nCurrentItemCount++;
		}
	}
	//追加のステータスバー表示情報取得
	m_stStatusBarDisp.m_nAddItemCount=0;
	for(nCnt=0; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		sprintf(szKey,"AddItem%d",nCnt);
		nBuf = m_pApp->GetProfileInt("CMainFrame",szKey,0);
		if(nBuf != 0)
		{
			m_stStatusBarDisp.m_nAddItems[m_stStatusBarDisp.m_nAddItemCount] = nBuf;
			m_stStatusBarDisp.m_nAddItemCount++;
		}
	}
	//デフォルト設定
	if(m_stStatusBarDisp.m_nCurrentItemCount==0 && m_stStatusBarDisp.m_nAddItemCount==0)
	{
		m_stStatusBarDisp.m_nCurrentItems[0] = ID_INDICATOR_GYORETU;
		m_stStatusBarDisp.m_nCurrentItems[1] = ID_INDICATOR_BYTE;
		m_stStatusBarDisp.m_nCurrentItems[2] = ID_INDICATOR_CHARCODE;
		m_stStatusBarDisp.m_nCurrentItems[3] = ID_INDICATOR_INS;
		m_stStatusBarDisp.m_nCurrentItems[4] = ID_INDICATOR_READONLY;
		m_stStatusBarDisp.m_nCurrentItems[5] = ID_INDICATOR_TIME;
		m_stStatusBarDisp.m_nCurrentItemCount = 6;
		m_stStatusBarDisp.m_nAddItems[0] = ID_INDICATOR_KANA;
		m_stStatusBarDisp.m_nAddItems[1] = ID_INDICATOR_CAPS;
		m_stStatusBarDisp.m_nAddItems[2] = ID_INDICATOR_NUM;
		m_stStatusBarDisp.m_nAddItems[3] = ID_INDICATOR_SCRL;
		m_stStatusBarDisp.m_nAddItemCount = 4;
	}

	//検索・置換ダイアログ表示中
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMainFrame::~CMainFrame()
{
	int	nCnt=0;
	int nBuf=0;
	char	szKey[64];

	//	レジストリへ設定保存
	m_pApp->WriteProfileInt("CMainFrame","bIsSaveWindowPlace",m_bIsSaveWindowPlace);
	m_pApp->WriteProfileInt("CMainFrame","bIsUseProxyHtml",m_bIsUseProxyHtml);
	m_pApp->WriteProfileString("CMainFrame","RequestPathHtml",m_objRequestPathHtml);
	m_pApp->WriteProfileString("CMainFrame","ProxyIPAddress",m_objProxyIPAddressHtml);
	m_pApp->WriteProfileInt("CMainFrame","bIsDialupHtml",m_bIsDialupHtml);
	m_pApp->WriteProfileString("CMainFrame","DialupEntryHtml",m_objDialupEntryHtml);
	m_pApp->WriteProfileString("CMainFrame","RasUserNameHtml",m_objRasUserNameHtml);
	m_pApp->WriteProfileString("CMainFrame","RasPasswdHtml",m_objRasPasswdHtml);
	m_pApp->WriteProfileInt("CMainFrame","bIsAutoDisconnectHtml",m_bIsAutoDisconnectHtml);
	m_pApp->WriteProfileString("CMainFrame","FindString",m_objFindString);
	m_pApp->WriteProfileInt("CMainFrame","bIsMatchCaseFind",m_bIsMatchCaseFind);
	m_pApp->WriteProfileInt("CMainFrame","bIsMatchWholeWordFind",m_bIsMatchWholeWordFind);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchDownFind",m_bIsSearchDownFind);
	m_pApp->WriteProfileInt("CMainFrame","bIsUseRegularExpressionFind",m_bIsUseRegularExpressionFind);
	m_pApp->WriteProfileString("CMainFrame","ReplaceString",m_objReplaceString);
	m_pApp->WriteProfileInt("CMainFrame","bIsMatchCaseReplace",m_bIsMatchCaseReplace);
	m_pApp->WriteProfileInt("CMainFrame","bIsMatchWholeWordReplace",m_bIsMatchWholeWordReplace);
	m_pApp->WriteProfileInt("CMainFrame","bIsUseRegularExpressionReplace",m_bIsUseRegularExpressionReplace);
	m_pApp->WriteProfileInt("CMainFrame","bIsMatchCaseGrep",m_bIsMatchCaseGrep);
	m_pApp->WriteProfileInt("CMainFrame","bIsMatchWholeWordGrep",m_bIsMatchWholeWordGrep);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchSubFolderGrep",m_bIsSearchSubFolderGrep);
	m_pApp->WriteProfileInt("CMainFrame","bIsUseRegularExpressionGrep",m_bIsUseRegularExpressionGrep);
	m_pApp->WriteProfileString("CMainFrame","FolderGrep",m_objFolderGrep);
	m_pApp->WriteProfileString("CMainFrame","FileKindGrep",m_objFileKindGrep);
	m_pApp->WriteProfileString("CMainFrame","GrepStr",m_objGrepStr);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchSubFolderConv",m_bIsSearchSubFolderConv);
	m_pApp->WriteProfileInt("CMainFrame","bIsOutOtherFileConv",m_bIsOutOtherFileConv);
	m_pApp->WriteProfileInt("CMainFrame","nConvKind",m_nConvKind);
	m_pApp->WriteProfileString("CMainFrame","FolderConv",m_objFolderConv);
	m_pApp->WriteProfileString("CMainFrame","FileKindConv",m_objFileKindConv);
	m_pApp->WriteProfileInt("CMainFrame","InCodeConv",m_nInCodeConv);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchSubFolderKaigyoConv",m_bIsSearchSubFolderKaigyoConv);
	m_pApp->WriteProfileInt("CMainFrame","bIsOutOtherFileKaigyoConv",m_bIsOutOtherFileKaigyoConv);
	m_pApp->WriteProfileString("CMainFrame","KaigyoConvKind",m_objKaigyoConvKind);
	m_pApp->WriteProfileString("CMainFrame","FolderKaigyoConv",m_objFolderKaigyoConv);
	m_pApp->WriteProfileString("CMainFrame","FileKindKaigyoConv",m_objFileKindKaigyoConv);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchSubFolderWCount",m_bIsSearchSubFolderWCount);
	m_pApp->WriteProfileString("CMainFrame","FolderWCount",m_objFolderWCount);
	m_pApp->WriteProfileString("CMainFrame","FileKindWCount",m_objFileKindWCount);
	m_pApp->WriteProfileInt("CMainFrame","bIsAutoSave",m_bIsAutoSave);
	m_pApp->WriteProfileInt("CMainFrame","lSaveInterval",m_lSaveInterval);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchSubFolderFSearch",m_bIsSearchSubFolderFSearch);
	m_pApp->WriteProfileString("CMainFrame","FolderFSearch",m_objFolderFSearch);
	m_pApp->WriteProfileString("CMainFrame","FileKindFSearch",m_objFileKindFSearch);
	m_pApp->WriteProfileInt("CMainFrame","StatusDayStyle",m_lStatusDayStyle);
	m_pApp->WriteProfileInt("CMainFrame","StatusHourStyle",m_lStatusHourStyle);
	m_pApp->WriteProfileInt("CMainFrame","StatusLineStyle",m_lStatusLineStyle);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchSubFolderEncript",m_bIsSearchSubFolderEncript);
	m_pApp->WriteProfileInt("CMainFrame","bIsOutOtherFileEncript",m_bIsOutOtherFileEncript);
	m_pApp->WriteProfileString("CMainFrame","FolderEncript",m_objFolderEncript);
	m_pApp->WriteProfileString("CMainFrame","FileKindEncript",m_objFileKindEncript);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchSubFolderDecript",m_bIsSearchSubFolderDecript);
	m_pApp->WriteProfileInt("CMainFrame","bIsOutOtherFileDecript",m_bIsOutOtherFileDecript);
	m_pApp->WriteProfileString("CMainFrame","FolderDecript",m_objFolderDecript);
	m_pApp->WriteProfileString("CMainFrame","FileKindDecript",m_objFileKindDecript);
	m_pApp->WriteProfileString("CMainFrame","Folder1DirCmp",m_objFolder1DirCmp);
	m_pApp->WriteProfileString("CMainFrame","Folder2DirCmp",m_objFolder2DirCmp);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchSubFolderTouch",m_bIsSearchSubFolderTouch);
	m_pApp->WriteProfileString("CMainFrame","FolderTouch",m_objFolderTouch);
	m_pApp->WriteProfileString("CMainFrame","FileKindTouch",m_objFileKindTouch);

	//現在のステータスバー表示情報
	for(nCnt=0; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		sprintf(szKey,"CurrentItem%d",nCnt);
		if(m_stStatusBarDisp.m_nCurrentItems[nCnt] != 0)
			m_pApp->WriteProfileInt("CMainFrame",szKey,m_stStatusBarDisp.m_nCurrentItems[nCnt]);
		else
			m_pApp->WriteProfileInt("CMainFrame",szKey,0);
	}
	//追加のステータスバー表示情報取得
	for(nCnt=0; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		sprintf(szKey,"AddItem%d",nCnt);
		if(m_stStatusBarDisp.m_nAddItems[nCnt] != 0)
			m_pApp->WriteProfileInt("CMainFrame",szKey,m_stStatusBarDisp.m_nAddItems[nCnt]);
		else
			m_pApp->WriteProfileInt("CMainFrame",szKey,0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メッセージハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ｳｲﾝﾄﾞｳが作成される前に呼ばれる処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	LPCSTR   lpzsNewName = "XerxesWindowClass";
	WNDCLASS wndcls;

	BOOL bRes = CMDIFrameWnd::PreCreateWindow( cs );   // MDI では CMDIFrameWnd
	HINSTANCE hInst = AfxGetInstanceHandle();

	// see if the class already exists
	if ( !::GetClassInfo( hInst, lpzsNewName, &wndcls ) )
	{
		// get default stuff
		::GetClassInfo( hInst, cs.lpszClass, &wndcls );
		// register a new class
		wndcls.lpszClassName = lpzsNewName;
		wndcls.hIcon = ::LoadIcon(hInst,MAKEINTRESOURCE(IDR_MAINFRAME));
		::RegisterClass( &wndcls );
	}
	cs.lpszClass = lpzsNewName;
	return bRes;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＳＩＺＥ処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	//最小化時－＞ツールバー状態保存
	if(nType == SIZE_MINIMIZED)
		SaveBarState(_T("ToolBar_State2"));

	CMDIFrameWnd::OnSize(nType, cx, cy);

	//最小化時からの復帰時－＞ツールバー状態復元
	if(nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
		LoadBarState(_T("ToolBar_State2"));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウの作成を要求するときに呼ばれる処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//ＭＤＩ背景
	if(!m_wndClient.SubclassWindow(m_hWndMDIClient))
	{
		TRACE0("Failed to subclass MDI client window.\n");
		return -1;
	}
	//ツールバー
	if(CreateToolBar() == FALSE)
	{
		TRACE0("Failed to Create ToolBar.\n");
		return -1;
	}
	//ステータスバー
	if(!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // 作成に失敗
	}
	if(!SetStatusBarDispInfo(&m_stStatusBarDisp))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // 作成に失敗
	}

	if( m_bIsSaveWindowPlace != 0 ) //ウインドウ位置保持
	{
		//アプリケーションの状態と位置を思い出す
		WINDOWPLACEMENT		wp;
		if(LoadWindowPlacement(&wp))
		{
			SetWindowPlacement(&wp);
		}
	}

	//ステータスバーのフォント作成＆設定
	memset(&m_stStatusBarLogFont,0,sizeof(LOGFONT));
	m_stStatusBarLogFont.lfHeight  = m_pApp->GetProfileInt("CMainFrame","STSFONT_lfHeight",12);
	m_stStatusBarLogFont.lfCharSet = m_pApp->GetProfileInt("CMainFrame","STSFONT_lfCharSet",DEFAULT_CHARSET);
	m_stStatusBarLogFont.lfItalic  = m_pApp->GetProfileInt("CMainFrame","STSFONT_lfItalic",FALSE);
	strcpy(m_stStatusBarLogFont.lfFaceName,m_pApp->GetProfileString("CMainFrame","STSFONT_lfFaceName","ＭＳ 明朝"));
	m_objStatusBarFont.CreateFontIndirect(&m_stStatusBarLogFont);
	m_wndStatusBar.SetFont(&m_objStatusBarFont);

	//日付、時刻用初期タイマースタート
	SetTimer(1,55,NULL);

	// CG: 次の行は「スプラッシュ スクリーン」コンポーネントにより追加されています。
	CSplashWnd::ShowSplashScreen(this);

	//ツールバー状態復元
	LoadBarState(_T("ToolBar_State2"));

	return 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：タイムアップ時処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == 1 ) //最初のタイマー
	{
		//日付、時刻用初期タイマー終了
		KillTimer(1);
		//日付、時刻用タイマースタート（１０秒間隔）
		SetTimer(2,(10*1000),NULL);
	}

	//日付、時刻設定
	SetDateTime();

	if(m_bIsAutoSave) //オートセーブを行う
	{
		//タイムアップカウンタ更新
		m_lTimeUpCount++;
		if(m_lTimeUpCount >= (m_lSaveInterval*6) ) //オートセーブ間隔になった
		{
			m_lTimeUpCount=0;
			m_pApp->DoFileAllSave();
		}
	}

	CMDIFrameWnd::OnTimer(nIDEvent);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウが破棄されるときに呼ばれる処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnDestroy() 
{
	::RemoveProp(GetSafeHwnd(),m_pApp->m_pszExeName);

	CMDIFrameWnd::OnDestroy();

	//ステータスバーフォントの設定保存
	m_pApp->WriteProfileInt("CMainFrame","STSFONT_lfHeight",m_stStatusBarLogFont.lfHeight);
	m_pApp->WriteProfileInt("CMainFrame","STSFONT_lfCharSet",m_stStatusBarLogFont.lfCharSet);
	m_pApp->WriteProfileInt("CMainFrame","STSFONT_lfItalic",m_stStatusBarLogFont.lfItalic);
	m_pApp->WriteProfileString("CMainFrame","STSFONT_lfFaceName",m_stStatusBarLogFont.lfFaceName);

	//日付、時刻用１分タイマー終了
	KillTimer(2);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ｳｲﾝﾄﾞｳまたはｱﾌﾟﾘｹｰｼｮﾝが終了しなければならないときに呼ばれる処理		|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnClose() 
{
	//////////////////////////////////////////
	//	終了時に開いていたファイル名保存	//
	//////////////////////////////////////////
	int nFileCnt=0;
	POSITION pos1 = m_pApp->GetFirstDocTemplatePosition();
	while(pos1 != 0)
	{
		CDocTemplate* pTempl = m_pApp->GetNextDocTemplate(pos1);
		POSITION pos2 = pTempl->GetFirstDocPosition();
		while(pos2 != 0)
		{
			CEditorDoc* pDoc = (CEditorDoc*)pTempl->GetNextDoc(pos2);
			if(pDoc != 0)
			{
				char	szEntry[32];
				sprintf(szEntry,"FnameOnExit%d",nFileCnt);
				m_pApp->WriteProfileString("CEditorApp",szEntry,pDoc->GetPathName());
				nFileCnt++;
			}
		}
	}
	m_pApp->WriteProfileInt("CEditorApp","FCountOnExit",nFileCnt);

	//////////////////////////
	//	ウインドウ位置保持	//
	//////////////////////////
	if( m_bIsSaveWindowPlace != 0 ) 
	{
		//アプリケーションの状態と位置を記憶させる
		WINDOWPLACEMENT		wp;
		if(GetWindowPlacement(&wp))
		{
			if(IsZoomed())
				wp.flags |= WPF_RESTORETOMAXIMIZED;
			SaveWindowPlacement(&wp);
		}
	}

	//ツールバー状態保持
	SaveBarState(_T("ToolBar_State2"));

	CMDIFrameWnd::OnClose();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ｳｲﾝﾄﾞｳがｱｸﾃｨﾌﾞ/非ｱｸﾃｨﾌﾞになる時に呼ばれる処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if( nState==WA_ACTIVE || nState==WA_CLICKACTIVE )
	{
		//ファイル変更監視
		m_pApp->CheckFileChange();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（ツールバーのカスタマイズ関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：標準ツールバーのカスタマイズ処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnCustomizeToolbar() 
{
	m_wndToolBar.Customize();
}
void CMainFrame::OnUpdateCustomizeToolbar(CCmdUI* pCmdUI) 
{
	BOOL bVisible = m_wndToolBar.IsWindowVisible();
	BOOL bIsFloating = m_wndToolBar.IsFloating();
	if(bVisible && !bIsFloating)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：編集関連ツールバーのカスタマイズ処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnCustomizeToolbaredit() 
{
	m_wndToolBarEdit.Customize();
}
void CMainFrame::OnUpdateCustomizeToolbaredit(CCmdUI* pCmdUI) 
{
	BOOL bVisible = m_wndToolBarEdit.IsWindowVisible();
	BOOL bIsFloating = m_wndToolBarEdit.IsFloating();
	if(bVisible && !bIsFloating)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：検索関連ツールバーのカスタマイズ処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnCustomizeToolbarfind() 
{
	m_wndToolBarFind.Customize();
}
void CMainFrame::OnUpdateCustomizeToolbarfind(CCmdUI* pCmdUI) 
{
	BOOL bVisible = m_wndToolBarFind.IsWindowVisible();
	BOOL bIsFloating = m_wndToolBarFind.IsFloating();
	if(bVisible && !bIsFloating)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ジャンプ関連ツールバーのカスタマイズ処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnCustomizeToolbarjmp() 
{
	m_wndToolBarJmp.Customize();
}
void CMainFrame::OnUpdateCustomizeToolbarjmp(CCmdUI* pCmdUI) 
{
	BOOL bVisible = m_wndToolBarJmp.IsWindowVisible();
	BOOL bIsFloating = m_wndToolBarJmp.IsFloating();
	if(bVisible && !bIsFloating)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ツール関連ツールバーのカスタマイズ処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnCustomizeToolbartools() 
{
	m_wndToolBarTools.Customize();
}
void CMainFrame::OnUpdateCustomizeToolbartools(CCmdUI* pCmdUI) 
{
	BOOL bVisible = m_wndToolBarTools.IsWindowVisible();
	BOOL bIsFloating = m_wndToolBarTools.IsFloating();
	if(bVisible && !bIsFloating)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウ関連ツールバーのカスタマイズ処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnCustomizeToolbarwnd() 
{
	m_wndToolBarWnd.Customize();
}
void CMainFrame::OnUpdateCustomizeToolbarwnd(CCmdUI* pCmdUI) 
{
	BOOL bVisible = m_wndToolBarWnd.IsWindowVisible();
	BOOL bIsFloating = m_wndToolBarWnd.IsFloating();
	if(bVisible && !bIsFloating)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（インテリマウス関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ホイールスクロール時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LRESULT CMainFrame::OnMSHMouseWheel(WPARAM wParam, LPARAM lParam)
{
	//現在のアクティブビューとアクティブドキュメントを取得
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd==NULL)
		return 0;
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc==NULL)
		return 0;
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView==NULL)
		return 0;

	int nDelta = (int)wParam;
	if(nDelta < 0) //ユーザ方向へマウスホイールをローテート
	{
		pobjEditorView->OnVScroll(SB_PAGEDOWN,NULL,pobjEditorView->GetScrollBarCtrl(SB_VERT));
	}
	else //モニター方向へマウスホイールをローテート
	{
		pobjEditorView->OnVScroll(SB_PAGEUP,NULL,pobjEditorView->GetScrollBarCtrl(SB_VERT));
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（ファイル関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル－プロパティ」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnFileProperty() 
{
	UINT				unCodeKind;
	CFileStatus			objFileStatus;
	CFilePropertyDlg	objFilePropertyDlg;
	CCodeConvManager	objCodeConvManager;
	char	szBuf[256], szFileName[_MAX_PATH], szFilePath[_MAX_PATH];
	CString				objMsg;

	//アクティブドキュメント取得
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd==NULL)
		return;
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc==NULL)
		return;

	//ドキュメント情報取得
	objFilePropertyDlg.m_objTitle = pobjEditorDoc->GetTitle();
	memset(szFileName,0,sizeof(szFileName));
	memset(szFilePath,0,sizeof(szFilePath));
	pobjEditorDoc->MyGetFileName(szFileName);
	objFilePropertyDlg.m_objFileName = szFileName;
	pobjEditorDoc->MyGetFilePath(szFilePath);

	if( _access(szFilePath, 0) != 0 )
	{
		objMsg.LoadString(IDS_CANTACCESS_FILE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	if(strlen(szFilePath))
	{
		CFile::GetStatus(szFilePath,objFileStatus);
		objFilePropertyDlg.m_objCTime = objFileStatus.m_ctime.Format("%Y/%m/%d %H:%M:%S");
		objFilePropertyDlg.m_objMTime = objFileStatus.m_mtime.Format("%Y/%m/%d %H:%M:%S");
		objFilePropertyDlg.m_objATime = objFileStatus.m_atime.Format("%Y/%m/%d");

		long lSumByte = 0;
		long lCaretByte = 0;
		pobjEditorDoc->GetBytes(0,&lCaretByte,&lSumByte);
		long lKB = (long) ((objFileStatus.m_size / 1024) + ((objFileStatus.m_size % 1024) ? 1 : 0));
		sprintf(szBuf,"%ld KB (%ld Byte)",lKB, lSumByte);

		objFilePropertyDlg.m_objSize  = szBuf;
		objFilePropertyDlg.m_objAttribute = _T("");
		if(objFileStatus.m_attribute == 0x00)
			objFilePropertyDlg.m_objAttribute += "normal, ";
		if( (objFileStatus.m_attribute & 0x01) == 0x01 )
			objFilePropertyDlg.m_objAttribute += "ReadOnly, ";
		else
			objFilePropertyDlg.m_objAttribute += "WriteOK, ";
		if( (objFileStatus.m_attribute & 0x02) == 0x02 )
			objFilePropertyDlg.m_objAttribute += "Hidden, ";
		if( (objFileStatus.m_attribute & 0x04) == 0x04 )
			objFilePropertyDlg.m_objAttribute += "System, ";
		if( (objFileStatus.m_attribute & 0x08) == 0x08 )
			objFilePropertyDlg.m_objAttribute += "Volume, ";
		if( (objFileStatus.m_attribute & 0x10) == 0x10 )
			objFilePropertyDlg.m_objAttribute += "Directory, ";
		if( (objFileStatus.m_attribute & 0x20) == 0x20 )
			objFilePropertyDlg.m_objAttribute += "Archive";
		unCodeKind = objCodeConvManager.DetectCode(szFilePath);	
		if(unCodeKind == INPUT_SJIS)
			objFilePropertyDlg.m_objCode = "SJIS";
		else if(unCodeKind == INPUT_EUC)
			objFilePropertyDlg.m_objCode = "EUC";
		else if(unCodeKind == INPUT_JIS)
			objFilePropertyDlg.m_objCode = "JIS";
		else if(unCodeKind == INPUT_ASCII)
			objFilePropertyDlg.m_objCode = "ASCII";
		else if(unCodeKind == INPUT_UNKNOWN)
			objFilePropertyDlg.m_objCode = "UNKNOWN";
		else if(unCodeKind == INPUT_BINARY)
			objFilePropertyDlg.m_objCode = "BINARY";
	}
	if(pobjEditorDoc->GetKaigyoKind() == KAIGYO_CRLF)
		objFilePropertyDlg.m_objKaigyo = "CR+LF";
	else if(pobjEditorDoc->GetKaigyoKind() == KAIGYO_LF)
		objFilePropertyDlg.m_objKaigyo = "LF";
	else if(pobjEditorDoc->GetKaigyoKind() == KAIGYO_CR)
		objFilePropertyDlg.m_objKaigyo = "CR";
	sprintf(szBuf,"%ld",pobjEditorDoc->GetLineCount());
	objFilePropertyDlg.m_objLineCnt  = szBuf;
	sprintf(szBuf,"%ld",pobjEditorDoc->GetTabSize());
	objFilePropertyDlg.m_objTabSize  = szBuf;

	//ファイルプロパティダイアログ表示
	objFilePropertyDlg.DoModal();
}
void CMainFrame::OnUpdateFileProperty(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ファイル名チェック
	char	szBuf[_MAX_PATH];
	pobjEditorDoc->MyGetFilePath(szBuf);
	if(!strlen(szBuf))
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pobjEditorDoc->MyGetFileName(szBuf);
	if(!strlen(szBuf))
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索、
	//暗号化、復号化チェック
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル－上書き保存して閉じる」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnFileSaveclose() 
{
	//アクティブドキュメント取得
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();

	//上書き保存
	if( !pobjEditorDoc->IsReadOnly() )
		pobjEditorDoc->OnFileSave();

	//閉じる
	pobjFrameWnd->SendMessage(WM_CLOSE);
}
void CMainFrame::OnUpdateFileSaveclose(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//ドキュメントエンプティチェック
	if( pobjEditorDoc->IsEmpty() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//書込み禁止チェック
	if( pobjEditorDoc->IsReadOnly() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索
	//暗号化、復号化チェック
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル－WEB上のHTMLファイルを開く」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnHttpFileOpen() 
{
	long	lRet=0;
	CString objMsg, objTitle, objFilePath;
	CBasicAuthorizationDlg objBasicAuthorizationDlg;
	CHtmlOpenDlg		objHtmlOpenDlg;
	CHtmlOpenResultDlg	objHtmlOpenResultDlg;
	BOOL bRet=FALSE;
	BOOL bIsUseBasicAuth=FALSE;
	CString objRealm = _T("");
	CHttp	objHttp;
	char	szBuf[256],szMsg[512],szRasMsg[512];
	DWORD	dwRasState=0;
	CFile	objFile;


	//開く対象のＨＴＭＬファイルを指定してもらう
	objHtmlOpenDlg.m_bIsUseProxy = m_bIsUseProxyHtml;
	objHtmlOpenDlg.m_objRequestPath = m_objRequestPathHtml;
	objHtmlOpenDlg.m_objProxyIPAddress = m_objProxyIPAddressHtml;
	if(objHtmlOpenDlg.DoModal() != IDOK)
		return;
	m_bIsUseProxyHtml = objHtmlOpenDlg.m_bIsUseProxy;
	m_objRequestPathHtml = objHtmlOpenDlg.m_objRequestPath;
	m_objProxyIPAddressHtml = objHtmlOpenDlg.m_objProxyIPAddress;

//	//test
//	objBasicAuthorizationDlg.m_objAuthSite  = objHtmlOpenDlg.m_objSite;
//	objBasicAuthorizationDlg.m_objAuthRealm = objRealm;
//	objBasicAuthorizationDlg.DoModal();
//	return;

	//状況モードレスダイアログ表示
	m_pobjHtmlOpenStatusDlg = new CHtmlOpenStatusDlg();
	m_pobjHtmlOpenStatusDlg->Create(IDD_HTMLOPENSTATUS);
	WaitForHtmlOpenDlgDisp();

	if(m_bIsDialupHtml) //ダイアルアップ接続
	{
		if(m_pApp->m_hRasDllInstance != NULL)
		{
			//関数アドレス取得
			m_pApp->m_lpfnRasFunc3 = (LPFNRASFUNC3)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasConnect");
			m_pApp->m_lpfnRasFunc4 = (LPFNRASFUNC4)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasIsConnected");
			m_pApp->m_lpfnRasFunc5 = (LPFNRASFUNC5)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasDisConnect");
			if( m_pApp->m_lpfnRasFunc3 != NULL && 
				m_pApp->m_lpfnRasFunc4 != NULL && 
				m_pApp->m_lpfnRasFunc5 != NULL)
			{
				//ＲＡＳ接続(rasConnect)
				objMsg.LoadString(IDS_RASCONNECT);
				m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
				lRet = m_pApp->m_lpfnRasFunc3((char*)(LPCTSTR)m_objDialupEntryHtml,
									(char*)(LPCTSTR)m_objRasUserNameHtml,
									(char*)(LPCTSTR)m_objRasPasswdHtml,
									"",
									szBuf );
				if(lRet == -6)
				{
					//ＲＡＳ切断(rasDisconnect)
					objMsg.LoadString(IDS_RASDISCONNECT);
					m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
					m_pApp->m_lpfnRasFunc5();
					objMsg.LoadString(IDS_RASCONNECTERR);
					sprintf(szMsg,(LPCTSTR)objMsg,szBuf);
					m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);

					//状況モードレスダイアログ消去
					m_pobjHtmlOpenStatusDlg->DestroyWindow();
					delete m_pobjHtmlOpenStatusDlg;
					m_pobjHtmlOpenStatusDlg = NULL;
					return;
				}
				else if(lRet == 1) //すでにオープン済み
				{
					objMsg.LoadString(IDS_RASCONNECTEND);
					m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
					//そのままＨＴＭＬファイルオープンへ
				}
				else if(lRet == 0) //ＲＡＳ接続ＯＫ
				{
					//接続完了待ち
					for(;;)
					{
						m_pApp->MessagePump();
						Sleep(10);
						if(m_pobjHtmlOpenStatusDlg->IsCanceled())
						{
							//ＲＡＳ切断(rasDisconnect)
							objMsg.LoadString(IDS_RASDISCONNECT);
							m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
							m_pApp->m_lpfnRasFunc5();

							//状況モードレスダイアログ消去
							m_pobjHtmlOpenStatusDlg->DestroyWindow();
							delete m_pobjHtmlOpenStatusDlg;
							m_pobjHtmlOpenStatusDlg = NULL;
							return;
						}

						//状態チェック(rasIsConnected)
						memset(szRasMsg,0,sizeof(szRasMsg));
						lRet = m_pApp->m_lpfnRasFunc4( &dwRasState, szRasMsg );
						objMsg.LoadString(IDS_RASISCONNECT);
						sprintf(szMsg,(LPCTSTR)objMsg,dwRasState,szRasMsg);
						m_pobjHtmlOpenStatusDlg->SetStatus(szMsg);
						if(lRet) //接続された
						{
							objMsg.LoadString(IDS_RASCONNECTEND);
							m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
							break;
						}
						else
						{
							if(dwRasState == 100 || dwRasState == 8193)
							{
								//ＲＡＳ切断(rasDisconnect)
								m_pApp->m_lpfnRasFunc5();
								objMsg.LoadString(IDS_RAS_CONNECTERR);
								m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);

								//状況モードレスダイアログ消去
								m_pobjHtmlOpenStatusDlg->DestroyWindow();
								delete m_pobjHtmlOpenStatusDlg;
								m_pobjHtmlOpenStatusDlg = NULL;
								return;
							}
						}
					}
				}
			}
		}
	}

	//状況表示
	if(m_pobjHtmlOpenStatusDlg->IsCanceled())
	{
		if( m_bIsDialupHtml && m_bIsAutoDisconnectHtml && m_pApp->m_lpfnRasFunc5) //ダイアルアップ接続
		{
			//ＲＡＳ切断(rasDisconnect)
			objMsg.LoadString(IDS_RASDISCONNECT);
			m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
			m_pApp->m_lpfnRasFunc5();
		}
		//状況モードレスダイアログ消去
		m_pobjHtmlOpenStatusDlg->DestroyWindow();
		delete m_pobjHtmlOpenStatusDlg;
		m_pobjHtmlOpenStatusDlg = NULL;
		return;
	}

	//キャッシュ用ファイルパス名作成
	if(m_pApp->IsDriveRoot(m_pApp->m_strCashePath))
		objFilePath.Format("%s%s",m_pApp->m_strCashePath,objHtmlOpenDlg.m_objFileName);
	else
		objFilePath.Format("%s\\%s",m_pApp->m_strCashePath,objHtmlOpenDlg.m_objFileName);
	if(!objFile.Open(objFilePath,(CFile::modeCreate|CFile::modeWrite),NULL))
	{
		objMsg.LoadString(IDS_HTTP_FILEOPENERR);
		m_pApp->MyMessageBox((LPCTSTR)objMsg,(MB_OK|MB_ICONSTOP),0);

		//状況モードレスダイアログ消去
		m_pobjHtmlOpenStatusDlg->DestroyWindow();
		delete m_pobjHtmlOpenStatusDlg;
		m_pobjHtmlOpenStatusDlg = NULL;

		return;
	}
	CArchive ar( &objFile, CArchive::store );

	//ＨＴＭＬサーバ接続
	objMsg.LoadString(IDS_HTTP_CONNECT);
	m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
	bRet = objHttp.Connect(objHtmlOpenDlg.m_objHost,objHtmlOpenDlg.m_bIsUseProxy);
	if(bRet) //OK
	{
		//状況表示
		objMsg.LoadString(IDS_HTTP_GET);
		m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
		if(m_pobjHtmlOpenStatusDlg->IsCanceled())
		{
			if( m_bIsDialupHtml && m_bIsAutoDisconnectHtml && m_pApp->m_lpfnRasFunc5) //ダイアルアップ接続
			{
				//ＲＡＳ切断(rasDisconnect)
				objMsg.LoadString(IDS_RASDISCONNECT);
				m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
				m_pApp->m_lpfnRasFunc5();
			}

			//切断
			objHttp.Disconnect();

			//状況モードレスダイアログ消去
			m_pobjHtmlOpenStatusDlg->DestroyWindow();
			delete m_pobjHtmlOpenStatusDlg;
			m_pobjHtmlOpenStatusDlg = NULL;

			//キャッシュファイルクローズ
			ar.Close();
			objFile.Close();
			return;
		}

		//HTMLファイル取得
		bIsUseBasicAuth = FALSE;
		objRealm = _T("");
		bRet = objHttp.Get(objHtmlOpenDlg.m_objPath,ar,m_pobjHtmlOpenStatusDlg,bIsUseBasicAuth,"","",objRealm);
		if(!bRet && bIsUseBasicAuth) //Basic認証が必要
		{
			objBasicAuthorizationDlg.m_objAuthSite  = objHtmlOpenDlg.m_objSite;
			objBasicAuthorizationDlg.m_objAuthRealm = objRealm;
			if(objBasicAuthorizationDlg.DoModal() == IDOK)
			{
				//Basic認証を使用して再接続＆再取得
				bRet = objHttp.Connect(objHtmlOpenDlg.m_objHost,objHtmlOpenDlg.m_bIsUseProxy);
				if(bRet) //OK
				{
					bRet = objHttp.Get( objHtmlOpenDlg.m_objPath,
										ar,
										m_pobjHtmlOpenStatusDlg,
										bIsUseBasicAuth,
										objBasicAuthorizationDlg.m_objAuthUser,
										objBasicAuthorizationDlg.m_objAuthPasswd,
										objRealm);
				}
			}
		}
	}

	if( m_bIsDialupHtml && m_bIsAutoDisconnectHtml && m_pApp->m_lpfnRasFunc5) //ダイアルアップ接続
	{
		//ＲＡＳ切断(rasDisconnect)
		objMsg.LoadString(IDS_RASDISCONNECT);
		m_pobjMailListStatusDlg->SetStatus(objMsg);
		m_pApp->m_lpfnRasFunc5();
	}

	//キャッシュファイルクローズ
	ar.Close();
	objFile.Close();
	if(bRet != FALSE)//オープン
		m_pApp->OpenFile(objFilePath);

	//状況モードレスダイアログ消去
	m_pobjHtmlOpenStatusDlg->DestroyWindow();
	delete m_pobjHtmlOpenStatusDlg;
	m_pobjHtmlOpenStatusDlg = NULL;

	//終了通知
	if( bRet != FALSE )
		objMsg.LoadString(IDS_HTTPOPEN_END);
	else
		objMsg.LoadString(IDS_HTTPOPEN_CANCEL);
	objHtmlOpenResultDlg.m_objStatus = objMsg;
	objHttp.GetResponseHeader(objHtmlOpenResultDlg.m_objHeader);
	objHtmlOpenResultDlg.DoModal();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル－再読み込み」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnFileReread() 
{
	//アクティブドキュメント取得
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();

	//マウスカーソル砂時計
	CWaitCursor	objWaitCursor;

	//再読み込み（ダイアログ確認なし）
	pobjEditorDoc->ReloadDocument(FALSE);
}
void CMainFrame::OnUpdateFileReread(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//ドキュメントエンプティチェック
	if( pobjEditorDoc->IsEmpty() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() != FALSE )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索
	//暗号化、復号化チェック
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル－削除」選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnFileDelete() 
{
	//現在のアクティブビューとアクティブドキュメントを取得
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd==NULL)
		return;
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc==NULL)
		return;

	SHFILEOPSTRUCT	stFileOp;
	memset(&stFileOp,0,sizeof(SHFILEOPSTRUCT));
	char	szFrom[_MAX_PATH];
	memset(szFrom,0,sizeof(szFrom));

	//途中経過を表示するウインドウの親のウインドウ
	CWnd* pWnd = (CWnd*)AfxGetMainWnd();
	stFileOp.hwnd = pWnd->m_hWnd;
	//オペレーション
	stFileOp.wFunc = FO_DELETE;
	//元
	pobjEditorDoc->MyGetFilePath(szFrom);
	stFileOp.pFrom = szFrom;
	//動作に関するフラグ
	stFileOp.fFlags = FOF_ALLOWUNDO;

	//ファイル削除処理実行
	if( ::SHFileOperation(&stFileOp) == 0 ) //OK
	{
		if( _access(szFrom,0) == -1 ) //本当に削除された
		{
			//ウインドウクローズ
			pobjFrameWnd->SendMessage(WM_CLOSE);
			//ＭＲＵリストから削除
			m_pApp->RemoveRecentFileList(szFrom);
		}
	}
}
void CMainFrame::OnUpdateFileDelete(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd == NULL) || (pobjFrameWnd == this) )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc==NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ファイル名チェック
	char	szBuf[_MAX_PATH];
	pobjEditorDoc->MyGetFilePath(szBuf);
	if(!strlen(szBuf))
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pobjEditorDoc->MyGetFileName(szBuf);
	if(!strlen(szBuf))
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索、
	//暗号化、復号化チェック
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（編集関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－クリップボードの中を見る」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditClipview() 
{
	//クリップボードの中身を表示
	CClipBoardViewDlg	objClipBoardViewDlg;
	objClipBoardViewDlg.DoModal();
}
void CMainFrame::OnUpdateEditClipview(CCmdUI* pCmdUI) 
{
	//ＯＬＥデータオブジェクト作成
	COleDataObject	objOleDataObject;

	//クリップボード上のデータオブジェクトに結び付けます。
	BOOL bRet = objOleDataObject.AttachClipboard();
	if( bRet != 0 )
	{
		//データがCF_TEXT形式で利用できるかを調査
		bRet = objOleDataObject.IsDataAvailable(CF_TEXT);
		if( bRet != 0 )
			pCmdUI->Enable(TRUE);
		else
			pCmdUI->Enable(FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－書込み禁止設定」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditSetreadonly() 
{
	//アクティブドキュメント取得
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd==NULL)
		return;
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc==NULL)
		return;

	//書込み禁止設定
	if(pobjEditorDoc->SetReadOnly())
	{
		//書込み禁止ﾓｰﾄﾞｽﾃｰﾀｽﾍﾟｲﾝ設定
		SetReadOnlyMode(TRUE);
	}
}
void CMainFrame::OnUpdateEditSetreadonly(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索
	//暗号化、復号化チェック
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//書込み禁止チェック
	if( pobjEditorDoc->IsReadOnly() ) //すでに書込み禁止になっている
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－書込み禁止解除」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditUnsetreadonly() 
{
	//アクティブドキュメント取得
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd==NULL)
		return;
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc==NULL)
		return;

	//書込み禁止解除
	if(pobjEditorDoc->UnsetReadOnly())
	{
		//書込み禁止ﾓｰﾄﾞｽﾃｰﾀｽﾍﾟｲﾝ設定
		SetReadOnlyMode(FALSE);
	}
}
void CMainFrame::OnUpdateEditUnsetreadonly(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() == TRUE )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索
	//暗号化、復号化チェック
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//書込み禁止チェック
	if( !pobjEditorDoc->IsReadOnly() ) //すでに書込み禁止が解除されている
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（表示関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－標準ツールバー」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnViewToolbar() 
{
	BOOL bVisible = m_wndToolBar.IsWindowVisible();

	ShowControlBar(&m_wndToolBar, bVisible ? FALSE : TRUE, FALSE );
}
void CMainFrame::OnUpdateViewToolbar(CCmdUI* pCmdUI) 
{
	if(m_wndToolBar.IsWindowVisible())
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－編集関連ツールバー」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnViewToolbarEdit() 
{
	BOOL bVisible = m_wndToolBarEdit.IsWindowVisible();

	ShowControlBar(&m_wndToolBarEdit, bVisible ? FALSE : TRUE, FALSE );
}
void CMainFrame::OnUpdateViewToolbarEdit(CCmdUI* pCmdUI) 
{
	if(m_wndToolBarEdit.IsWindowVisible())
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－検索関連ツールバー」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnViewToolbarFind() 
{
	BOOL bVisible = m_wndToolBarFind.IsWindowVisible();

	ShowControlBar(&m_wndToolBarFind, bVisible ? FALSE : TRUE, FALSE );
}
void CMainFrame::OnUpdateViewToolbarFind(CCmdUI* pCmdUI) 
{
	if(m_wndToolBarFind.IsWindowVisible())
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－ジャンプ関連ツールバー」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnViewToolbarJmp() 
{
	BOOL bVisible = m_wndToolBarJmp.IsWindowVisible();

	ShowControlBar(&m_wndToolBarJmp, bVisible ? FALSE : TRUE, FALSE );
}
void CMainFrame::OnUpdateViewToolbarJmp(CCmdUI* pCmdUI) 
{
	if(m_wndToolBarJmp.IsWindowVisible())
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－ツール関連ツールバー」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnViewToolbarTools() 
{
	BOOL bVisible = m_wndToolBarTools.IsWindowVisible();

	ShowControlBar(&m_wndToolBarTools, bVisible ? FALSE : TRUE, FALSE );
}
void CMainFrame::OnUpdateViewToolbarTools(CCmdUI* pCmdUI) 
{
	if(m_wndToolBarTools.IsWindowVisible())
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－ウインドウ関連ツールバー」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnViewToolbarWnd() 
{
	BOOL bVisible = m_wndToolBarWnd.IsWindowVisible();

	ShowControlBar(&m_wndToolBarWnd, bVisible ? FALSE : TRUE, FALSE );
}
void CMainFrame::OnUpdateViewToolbarWnd(CCmdUI* pCmdUI) 
{
	if(m_wndToolBarWnd.IsWindowVisible())
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（ツール関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ツール－メール一覧」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnFileMaillist() 
{
	int				nMailCount=0;
	long			lCnt=0, lRet=0;
	char			szBuf[256],szMsg[512],szRasMsg[512];
	CPop			objPop;
	CMailListDlg	objMailListDlg;
	CString			objStr;

	//現在のアカウントに対応する、メール関連設定を取得
	char			szSection[64];
	int				nAccountNo = m_pApp->GetProfileInt("Mail","CurrentAccountNo",0);
	if(nAccountNo < 0 || nAccountNo > 8)
		nAccountNo = 0;
	sprintf(szSection,"Mail_Account%d",nAccountNo);

	CString objPopServer   = m_pApp->GetProfileString(szSection,"PopServer","");
	CString objPopUser     = m_pApp->GetProfileString(szSection,"PopUser","");
	CString objPopPasswd   = m_pApp->GetProfileString(szSection,"PopPasswd","");
	BOOL bIsDialup         = m_pApp->GetProfileInt(szSection,"bIsDialup",0);
	CString objDialupEntry = m_pApp->GetProfileString(szSection,"DialupEntry","");
	CString objRasUserName = m_pApp->GetProfileString(szSection,"RasUserName","");
	CString objRasPasswd   = m_pApp->GetProfileString(szSection,"RasPasswd","");
	BOOL bAutoDisconnect   = m_pApp->GetProfileInt(szSection,"AutoDisconnect",0);
	int nAuthorizationType = m_pApp->GetProfileInt(szSection,"nAuthorizationType",POP3_AUTHENTICATE_STANDARD);
	DWORD			dwRasState=0;
	CString			objMsg;

	//事前チェック
	if(objPopServer.IsEmpty())
	{
		objMsg.LoadString(IDS_POP_NOSERVER);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if(objPopUser.IsEmpty())
	{
		objMsg.LoadString(IDS_POP_NOUSER);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if(objPopPasswd.IsEmpty())
	{
		objMsg.LoadString(IDS_POP_NOPASSWD);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//メールリスト状況モードレスダイアログ表示
	m_pobjMailListStatusDlg = new CMailListStatusDlg();
	m_pobjMailListStatusDlg->Create(IDD_MAILLISTSTATUS);
	WaitForMailListDlgDisp();


	if(bIsDialup) //ダイアルアップ接続
	{
		if(m_pApp->m_hRasDllInstance != NULL)
		{
			//関数アドレス取得
			m_pApp->m_lpfnRasFunc3 = (LPFNRASFUNC3)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasConnect");
			m_pApp->m_lpfnRasFunc4 = (LPFNRASFUNC4)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasIsConnected");
			m_pApp->m_lpfnRasFunc5 = (LPFNRASFUNC5)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasDisConnect");
			if( m_pApp->m_lpfnRasFunc3 != NULL && 
				m_pApp->m_lpfnRasFunc4 != NULL && 
				m_pApp->m_lpfnRasFunc5 != NULL)
			{
				//ＲＡＳ接続(rasConnect)
				objMsg.LoadString(IDS_RASCONNECT);
				m_pobjMailListStatusDlg->SetStatus(objMsg);
				lRet = m_pApp->m_lpfnRasFunc3((char*)(LPCTSTR)objDialupEntry,
									(char*)(LPCTSTR)objRasUserName,
									(char*)(LPCTSTR)objRasPasswd,
									"",
									szBuf );
				if(lRet == -6)
				{
					//ＲＡＳ切断(rasDisconnect)
					objMsg.LoadString(IDS_RASDISCONNECT);
					m_pobjMailListStatusDlg->SetStatus(objMsg);
					m_pApp->m_lpfnRasFunc5();
					objMsg.LoadString(IDS_RASCONNECTERR);
					sprintf(szMsg,(LPCTSTR)objMsg,szBuf);
					m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);

					//メールリスト状況モードレスダイアログ消去
					m_pobjMailListStatusDlg->DestroyWindow();
					delete m_pobjMailListStatusDlg;
					m_pobjMailListStatusDlg = NULL;
					return;
				}
				else if(lRet == 1) //すでにオープン済み
				{
					objMsg.LoadString(IDS_RASCONNECTEND);
					m_pobjMailListStatusDlg->SetStatus(objMsg);
					//そのままメールリスト表示へ
				}
				else if(lRet == 0) //ＲＡＳ接続ＯＫ
				{
					//接続完了待ち
					for(;;)
					{
						m_pApp->MessagePump();
						Sleep(10);
						if(m_pobjMailListStatusDlg->IsCanceled())
						{
							//ＲＡＳ切断(rasDisconnect)
							objMsg.LoadString(IDS_RASDISCONNECT);
							m_pobjMailListStatusDlg->SetStatus(objMsg);
							m_pApp->m_lpfnRasFunc5();

							//メールリスト状況モードレスダイアログ消去
							m_pobjMailListStatusDlg->DestroyWindow();
							delete m_pobjMailListStatusDlg;
							m_pobjMailListStatusDlg = NULL;
							return;
						}

						//状態チェック(rasIsConnected)
						memset(szRasMsg,0,sizeof(szRasMsg));
						lRet = m_pApp->m_lpfnRasFunc4( &dwRasState, szRasMsg );
						objMsg.LoadString(IDS_RASISCONNECT);
						sprintf(szMsg,(LPCTSTR)objMsg,dwRasState,szRasMsg);
						m_pobjMailListStatusDlg->SetStatus(szMsg);
						if(lRet) //接続された
						{
							objMsg.LoadString(IDS_RASCONNECTEND);
							m_pobjMailListStatusDlg->SetStatus(objMsg);
							break;
						}
						else
						{
							if(dwRasState == 100 || dwRasState == 8193)
							{
								//ＲＡＳ切断(rasDisconnect)
								m_pApp->m_lpfnRasFunc5();
								objMsg.LoadString(IDS_RAS_CONNECTERR);
								m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);

								//メールリスト状況モードレスダイアログ消去
								m_pobjMailListStatusDlg->DestroyWindow();
								delete m_pobjMailListStatusDlg;
								m_pobjMailListStatusDlg = NULL;
								return;
							}
						}
					}
				}
			}
		}
	}

	//状況表示
	if(m_pobjMailListStatusDlg->IsCanceled())
	{
		if( bIsDialup && bAutoDisconnect && m_pApp->m_lpfnRasFunc5) //ダイアルアップ接続
		{
			//ＲＡＳ切断(rasDisconnect)
			objMsg.LoadString(IDS_RASDISCONNECT);
			m_pobjMailListStatusDlg->SetStatus(objMsg);
			m_pApp->m_lpfnRasFunc5();
		}
		//メールリスト状況モードレスダイアログ消去
		m_pobjMailListStatusDlg->DestroyWindow();
		delete m_pobjMailListStatusDlg;
		m_pobjMailListStatusDlg = NULL;
		return;
	}

	//メールリスト
	objMsg.LoadString(IDS_MAILSERVER_CONNECT);
	m_pobjMailListStatusDlg->SetStatus(objMsg);
	if(objPop.Connect(objPopServer,objPopUser,objPopPasswd,nAuthorizationType))
	{
		//状況表示
		objMsg.LoadString(IDS_MAILLIST_GET);
		m_pobjMailListStatusDlg->SetStatus(objMsg);
		if(m_pobjMailListStatusDlg->IsCanceled())
		{
			if( bIsDialup && bAutoDisconnect && m_pApp->m_lpfnRasFunc5) //ダイアルアップ接続
			{
				//ＲＡＳ切断(rasDisconnect)
				objMsg.LoadString(IDS_RASDISCONNECT);
				m_pobjMailListStatusDlg->SetStatus(objMsg);
				m_pApp->m_lpfnRasFunc5();
			}

			//切断
			objPop.Disconnect();

			//メールリスト状況モードレスダイアログ消去
			m_pobjMailListStatusDlg->DestroyWindow();
			delete m_pobjMailListStatusDlg;
			m_pobjMailListStatusDlg = NULL;
			return;
		}

		nMailCount = objPop.GetMailCount();
		if(nMailCount==0) //ＰＯＰサーバメール数チェック
		{
			//メール数ゼロ通知
			objMsg.LoadString(IDS_POP_LISTZERO);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
			//切断
			objPop.Disconnect();
		}
		else if(nMailCount > 0)
		{
			//メールリストダイアログ表示
			objMailListDlg.m_objPopServerStr = objPopServer;
			objMailListDlg.m_objPopUserStr = objPopUser;
			objMailListDlg.m_objPopPasswdStr = objPopPasswd;
			objMailListDlg.m_nMailCount = nMailCount;
			objMailListDlg.m_pPop = &objPop;
			objMailListDlg.DoModal();
		}
	}

	if( bIsDialup && bAutoDisconnect && m_pApp->m_lpfnRasFunc5) //ダイアルアップ接続
	{
		//ＲＡＳ切断(rasDisconnect)
		objMsg.LoadString(IDS_RASDISCONNECT);
		m_pobjMailListStatusDlg->SetStatus(objMsg);
		m_pApp->m_lpfnRasFunc5();
	}

	//メールリスト状況モードレスダイアログ消去
	m_pobjMailListStatusDlg->DestroyWindow();
	delete m_pobjMailListStatusDlg;
	m_pobjMailListStatusDlg = NULL;
}
void CMainFrame::OnUpdateFileMaillist(CCmdUI* pCmdUI) 
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「キー操作の記録開始/終了」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnToolOpeStartendrecord() 
{
	int		nRet=0;
	char	szMsg[256];
	CString	objStr = _T("");
	CString objMsg;

	if(m_bIsNowOperationRecording == FALSE) //記録開始の場合
	{
		//キー操作未保存のチェック
		if(m_objOperationManager.GetOperationCount() > 0 )
		{
			m_objOperationManager.GetOperationFileName(objStr);
			if(objStr.IsEmpty())
			{
				objMsg.LoadString(IDS_KEYOPE_UNSAVED);
				nRet = m_pApp->MyMessageBox(objMsg,(MB_OKCANCEL|MB_ICONEXCLAMATION),0);
				if(nRet == IDCANCEL)
					return;
			}
		}

		m_objOperationManager.Initialize();
		objMsg.LoadString(IDS_KEYOPE_STARTRECORD);
		if(m_pApp->MyMessageBox(objMsg,(MB_OKCANCEL|MB_ICONINFORMATION),0)==IDCANCEL)
			return;
	}
	else //記録終了の場合
	{
		objMsg.LoadString(IDS_KEYOPE_RECORDEND);
		sprintf(szMsg,(LPCTSTR)objMsg,m_objOperationManager.GetOperationCount() );
		m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
	//キー操作記録中フラグ切替え
	m_bIsNowOperationRecording = (m_bIsNowOperationRecording==TRUE) ? FALSE : TRUE;
}
void CMainFrame::OnUpdateToolOpeStartendrecord(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索
	//暗号化、復号化チェック
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//書込み禁止チェック
	if( pobjEditorDoc->IsReadOnly() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//キー操作のステップ実行中チェック
	if(	IsNowStepDoOperation() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//インクリメンタルサーチ実行中チェック
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「キー操作の実行」選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnToolOpeDo() 
{
	//現在のアクティブビューとアクティブドキュメントを取得
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();

	//キー操作の実行
	m_objOperationManager.doOperation(pobjEditorView,pobjEditorDoc);
}
void CMainFrame::OnUpdateToolOpeDo(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索
	//暗号化、復号化チェック
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//書込み禁止チェック
	if( pobjEditorDoc->IsReadOnly() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//キー操作記録中チェック
	if( IsNowOperationRecording() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//キー操作記録数チェック
	if( m_objOperationManager.GetOperationCount() == 0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//キー操作のステップ実行中チェック
	if(	IsNowStepDoOperation() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//インクリメンタルサーチ実行中チェック
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「キー操作のステップ実行　－　次のキー操作」選択時処理				|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnToolOpeStepdoNext() 
{
	BOOL bRet = 0;
	CString objMsg;

	//現在のアクティブビューとアクティブドキュメントを取得
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();

	//キー操作のステップ実行
	if(m_bIsNowStepDoOperation == FALSE)
		bRet = m_objOperationManager.stepDoOperation(pobjEditorView,pobjEditorDoc,TRUE);//初回
	else
		bRet = m_objOperationManager.stepDoOperation(pobjEditorView,pobjEditorDoc,FALSE);//２回目以降

	//キー操作のステップ実行中フラグオン
	m_bIsNowStepDoOperation = TRUE;

	if(bRet == FALSE) //これで終わり
	{
		objMsg.LoadString(IDS_KEYOPE_STEPDOEND);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		//キー操作のステップ実行中フラグオフ
		m_bIsNowStepDoOperation = FALSE;
	}
}
void CMainFrame::OnUpdateToolOpeStepdoNext(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索
	//暗号化、復号化チェック
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//書込み禁止チェック
	if( pobjEditorDoc->IsReadOnly() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//キー操作記録中チェック
	if( IsNowOperationRecording() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//キー操作記録数チェック
	if( m_objOperationManager.GetOperationCount() == 0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//インクリメンタルサーチ実行中チェック
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「キー操作のステップ実行　－　終了」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnToolOpeStepdoEnd() 
{
	//キー操作のステップ実行中フラグオフ
	m_bIsNowStepDoOperation = FALSE;

	//キー操作ステップ実行終了
	m_objOperationManager.endStepDoOperation();

	CString objMsg;
	objMsg.LoadString(IDS_KEYOPE_STEPDOEND2);
	m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
}
void CMainFrame::OnUpdateToolOpeStepdoEnd(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索
	//暗号化、復号化チェック
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//書込み禁止チェック
	if( pobjEditorDoc->IsReadOnly() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//キー操作記録中チェック
	if( IsNowOperationRecording() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//キー操作記録数チェック
	if( m_objOperationManager.GetOperationCount() == 0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//キー操作ステップ実行中チェック
	if( IsNowStepDoOperation() == FALSE)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//インクリメンタルサーチ実行中チェック
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「キー操作をファイルへ保存」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnToolOpeSave() 
{
	char*	pPtr=NULL;
	char	szFileName[_MAX_FNAME];
	CString objStrFName=_T("");
	CString objMsg;

	//ファイル保存コモンダイアログ表示
	CFileDialog	objFileDialog(FALSE);
	if( objFileDialog.DoModal() == IDOK )
	{
		//ファイル名取得
		objStrFName = objFileDialog.GetPathName();
		strcpy(szFileName,(LPCTSTR)objStrFName);

		//ファイルオープン
		CFile	objFile;
		if( !objFile.Open( szFileName, CFile::modeWrite | CFile::modeCreate, NULL ) )
		{
			objMsg.LoadString(IDS_FOPEN_ERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
		CArchive	ar(&objFile,CArchive::store);

		//キー操作をファイル保存
		m_objOperationManager.Serialize(ar);

		//キー操作ファイル名設定
		m_objOperationManager.SetOperationFileName(objStrFName);

		//ファイルクローズ
		ar.Close();
		objFile.Close();
	}
}
void CMainFrame::OnUpdateToolOpeSave(CCmdUI* pCmdUI) 
{
	//キー操作記録数チェック
	if( m_objOperationManager.GetOperationCount() == 0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//キー操作記録中チェック
	if( IsNowOperationRecording() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//キー操作ステップ実行中チェック
	if( IsNowStepDoOperation() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//インクリメンタルサーチ実行中チェック
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「キー操作をファイルから読み込む」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnToolOpeOpen() 
{
	char*	pPtr=NULL;
	char	szFileName[_MAX_FNAME];
	CString objStrFName=_T("");
	CString objMsg;

	//ファイル読み込みコモンダイアログ表示
	CFileDialog	objFileDialog(TRUE);
	if( objFileDialog.DoModal() == IDOK )
	{
		//ファイル名取得
		objStrFName = objFileDialog.GetPathName();
		strcpy(szFileName,(LPCTSTR)objStrFName);

		//ファイルオープン
		CFile	objFile;
		if( !objFile.Open( szFileName, CFile::modeRead, NULL ) )
		{
			objMsg.LoadString(IDS_FOPEN_ERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
		CArchive	ar(&objFile,CArchive::load);

		//キー操作ファイルチェック
		if(m_objOperationManager.checkIsOperationFile(ar) != FALSE)
		{
			//オペレーション初期化
			m_objOperationManager.Initialize();

			//キー操作をファイルから読み込み
			BOOL bRet = m_objOperationManager.Serialize(ar);
			if(bRet == FALSE)
			{
				objMsg.LoadString(IDS_KEYOPE_FOPENERR);
				m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
				m_objOperationManager.Initialize();
			}
			else
			{
				//オペレーションファイル名設定
				m_objOperationManager.SetOperationFileName(objStrFName);

				objMsg.LoadString(IDS_KEYOPE_OPENEND);
				m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
			}
		}
		else //指定されたファイルはキー操作ファイルではない
		{
			objMsg.LoadString(IDS_NOT_KEYOPEFILE);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		}

		//ファイルクローズ
		ar.Close();
		objFile.Close();
	}
}
void CMainFrame::OnUpdateToolOpeOpen(CCmdUI* pCmdUI) 
{
	//キー操作記録中チェック
	if( IsNowOperationRecording() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//キー操作ステップ実行中チェック
	if( IsNowStepDoOperation() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//インクリメンタルサーチ実行中チェック
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「キー操作の内容表示」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnToolOpeDisp() 
{
	//オペレーション一覧表示
	m_objOperationManager.DispOperationLists();
}
void CMainFrame::OnUpdateToolOpeDisp(CCmdUI* pCmdUI) 
{
	//キー操作記録数チェック
	if( m_objOperationManager.GetOperationCount() == 0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//キー操作記録中チェック
	if( IsNowOperationRecording() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//キー操作ステップ実行中チェック
	if( IsNowStepDoOperation() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//インクリメンタルサーチ実行中チェック
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（ウインドウ関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「最小化」選択時処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnWindowMinimize() 
{
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd != NULL) && (pobjFrameWnd != this) )
	{
		pobjFrameWnd->ShowWindow(SW_SHOWMINIMIZED);

		//サウンド
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowMinimize",0) != 0)
		{
			CString strPath;
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowMinimize",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
	}
}
void CMainFrame::OnUpdateWindowMinimize(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd == NULL) || (pobjFrameWnd == this) )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「最大化」選択時処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnWindowMaximize() 
{
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd != NULL) && (pobjFrameWnd != this) )
	{
		pobjFrameWnd->ShowWindow(SW_SHOWMAXIMIZED);

		//サウンド
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowMaximize",0) != 0)
		{
			CString strPath;
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowMaximize",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
	}
}
void CMainFrame::OnUpdateWindowMaximize(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd == NULL) || (pobjFrameWnd == this) )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「もとのサイズに戻す」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnWindowRestore() 
{
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd != NULL) && (pobjFrameWnd != this) )
	{
		pobjFrameWnd->ShowWindow(SW_RESTORE);

		//サウンド
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowRestore",0) != 0)
		{
			CString strPath;
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowRestore",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
	}
}
void CMainFrame::OnUpdateWindowRestore(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd == NULL) || (pobjFrameWnd == this) )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「次のウインドウ」選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnWndNext() 
{
	MDINext();

	//サウンド
	if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowNext",0) != 0)
	{
		CString strPath;
		strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowNext",""));
		if(_access((LPCTSTR)strPath,0) != -1)
			m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
	}
}
void CMainFrame::OnUpdateWndNext(CCmdUI* pCmdUI) 
{
	int	nCount=0;
	CWnd* pWnd = MDIGetActive();
	while(pWnd != NULL)
	{
		if(pWnd != FromHandle(m_hWnd))
			nCount++;
		pWnd = pWnd->GetNextWindow();
	}

	if(nCount <= 1)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「前のウインドウ」選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnWndPrev() 
{
	CWnd* pWnd = MDIGetActive()->GetNextWindow(GW_HWNDLAST);
	if(pWnd != NULL)
	{
		pWnd->BringWindowToTop();

		//サウンド
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowPrev",0) != 0)
		{
			CString strPath;
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowPrev",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
	}
}
void CMainFrame::OnUpdateWndPrev(CCmdUI* pCmdUI) 
{
	int	nCount=0;
	CWnd* pWnd = MDIGetActive();
	while(pWnd != NULL)
	{
		if(pWnd != FromHandle(m_hWnd))
			nCount++;
		pWnd = pWnd->GetNextWindow();
	}

	if(nCount <= 1)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ウインドウを閉じる」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnWindowClose() 
{
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd != NULL) && (pobjFrameWnd != this) )
	{
		pobjFrameWnd->SendMessage(WM_CLOSE);

		//サウンド
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowClose",0) != 0)
		{
			CString strPath;
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowClose",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
	}
}
void CMainFrame::OnUpdateWindowClose(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd == NULL) || (pobjFrameWnd == this) )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「すべてのウインドウを閉じる」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnWindowAllclose() 
{
	CFrameWnd* pobjFrameWnd = NULL;

	//サウンド
	if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowAllClose",0) != 0)
	{
		//Waveファイルパス取得
		CString strPath;
		strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowAllClose",""));
		if(_access((LPCTSTR)strPath,0) != -1)
			m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
	}

	for(;;)
	{
		pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
		if( (pobjFrameWnd == NULL) || (pobjFrameWnd == this) )
			break;

		pobjFrameWnd->SendMessage(WM_CLOSE);
	}
}
void CMainFrame::OnUpdateWindowAllclose(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd == NULL) || (pobjFrameWnd == this) )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（検索関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「検索－文字列の検索」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFind() 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
		return;
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
		return;
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
		return;
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
		return;

	//もし範囲選択されていた場合には、それを検索文字列とする
	if(pobjEditorView->IsSelected() && pobjEditorView->GetSelectLineCount()==1)
	{
		long lSelectLineCnt=0;
		CLine* pobjLine=pobjEditorView->MyGetSelectedText(&lSelectLineCnt);
		m_objFindString = pobjLine->String();
		delete pobjLine;
	}

	//検索ダイアログ表示
	CFindDlg	objFindDlg;
	objFindDlg.m_pobjMainFrame = this;
	objFindDlg.m_pobjEditorView = pobjEditorView;
	objFindDlg.m_objFindString = m_objFindString;
	objFindDlg.m_bIsMatchWholeWordFind = m_bIsMatchWholeWordFind;
	objFindDlg.m_bIsMatchCaseFind = m_bIsMatchCaseFind;
	objFindDlg.m_bIsSearchDownFind = m_bIsSearchDownFind;
	objFindDlg.m_bIsUseRegularExpression = m_bIsUseRegularExpressionFind;
	objFindDlg.DoModal();
}
void CMainFrame::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//ドキュメントエンプティチェック
	if( pobjEditorDoc->IsEmpty() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//インクリメンタルサーチ実行中チェック
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「検索－文字列の置換」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditReplace() 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
		return;
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
		return;
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
		return;
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
		return;

	//もし範囲選択されていた場合には、それを検索文字列とする
	if(pobjEditorView->IsSelected() && pobjEditorView->GetSelectLineCount()==1)
	{
		long lSelectLineCnt=0;
		CLine* pobjLine=pobjEditorView->MyGetSelectedText(&lSelectLineCnt);
		m_objFindString = pobjLine->String();
		delete pobjLine;
	}

	//置換ダイアログ表示
	CReplaceDlg		objReplaceDlg;
	objReplaceDlg.m_pobjMainFrame = this;
	objReplaceDlg.m_pobjEditorView = pobjEditorView;
	objReplaceDlg.m_objFindString = m_objFindString;
	objReplaceDlg.m_objReplaceString = m_objReplaceString;
	objReplaceDlg.m_bIsMatchWholeWordReplace = m_bIsMatchWholeWordReplace;
	objReplaceDlg.m_bIsMatchCaseReplace = m_bIsMatchCaseReplace;
	objReplaceDlg.m_bIsUseRegularExpression = m_bIsUseRegularExpressionReplace;
	objReplaceDlg.DoModal();
}
void CMainFrame::OnUpdateEditReplace(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//エンプティ＆書込み禁止チェック
	if( pobjEditorDoc->IsEmpty() || pobjEditorDoc->IsReadOnly() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//インクリメンタルサーチ実行中チェック
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「文字列の検索下方向続行（Ｆ３）」押下時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFindcontinue() 
{
	CString objMsg;

	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
		return;
	//アクティブビュー存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
		return;
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
		return;

	if( m_objFindString.IsEmpty() )
	{
		MessageBeep((UINT)-1);
		return;
	}

	//マウスカーソル砂時計
	CWaitCursor	objWaitCursor;

	//検索実行
	BOOL bRet = pobjEditorView->DoFind( m_objFindString,
										m_bIsMatchCaseFind,
										m_bIsMatchWholeWordFind,
										TRUE,
										m_bIsUseRegularExpressionFind);
	if( bRet == FALSE ) // 未発見終了
	{
		objMsg.LoadString(IDS_FINDENDMSG);
		CString objMsgStr;
		objMsgStr.Format(objMsg,m_objFindString);
		m_pApp->MyMessageBox((LPCTSTR)objMsgStr,(MB_OK|MB_ICONINFORMATION),0);
		//選択範囲解除
		pobjEditorView->ClearSelectArea();
	}
}
void CMainFrame::OnUpdateEditFindcontinue(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//ドキュメントエンプティチェック
	if( pobjEditorDoc->IsEmpty() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//前回検索文字列存在チェック
	if( m_objFindString.IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//インクリメンタルサーチ実行中チェック
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「文字列の検索下方向続行（Ｓｈｉｆｔ＋Ｆ３）」押下時処理				|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFindrevcontinue() 
{
	CString objMsg;

	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
		return;
	//アクティブビュー存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
		return;
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
		return;

	if( m_objFindString.IsEmpty() )
	{
		MessageBeep((UINT)-1);
		return;
	}

	//マウスカーソル砂時計
	CWaitCursor	objWaitCursor;

	//検索実行
	BOOL bRet = pobjEditorView->DoFind( m_objFindString,
										m_bIsMatchCaseFind,
										m_bIsMatchWholeWordFind,
										FALSE,
										m_bIsUseRegularExpressionFind);
	if( bRet == FALSE ) // 未発見終了
	{
		objMsg.LoadString(IDS_FINDENDMSG);
		CString objMsgStr;
		objMsgStr.Format(objMsg,m_objFindString);
		m_pApp->MyMessageBox((LPCTSTR)objMsgStr,(MB_OK|MB_ICONINFORMATION),0);
		//選択範囲解除
		pobjEditorView->ClearSelectArea();
	}
}
void CMainFrame::OnUpdateEditFindrevcontinue(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//ドキュメントエンプティチェック
	if( pobjEditorDoc->IsEmpty() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//前回検索文字列存在チェック
	if( m_objFindString.IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//インクリメンタルサーチ実行中チェック
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「検索－インクリメンタルサーチ」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditIncrimentalsearch() 
{
	CString objMsg;
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();

	//インクリメンタルサーチ実行中フラグ切り替え
	m_bIsNowIncrimentalSearching = (m_bIsNowIncrimentalSearching!=FALSE) ? FALSE : TRUE;

	if(m_bIsNowIncrimentalSearching)//開始
	{
		objMsg.LoadString(IDS_START_INCRIMENTALSEARCH);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		//インクリメンタルサーチ開始行、オフセットクリア
		pobjEditorView->ClearIncrimentalSearchLineOffset();
		//ステータスペインにメッセージ設定
		objMsg.LoadString(IDS_INCREMENTALSEARCH_NOW);
		SetStatusMessage(objMsg);
	}
	else //終了
	{
		objMsg.LoadString(IDS_END_INCRIMENTALSEARCH);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		//インクリメンタルサーチ文字列を引き継ぐ
		pobjEditorView->GetIncrimentalSearchString(m_objFindString);
		//インクリメンタルサーチ開始行、オフセットクリア
		pobjEditorView->ClearIncrimentalSearchLineOffset();
		//ステータスペインにメッセージ設定
		objMsg.LoadString(AFX_IDS_IDLEMESSAGE);
		SetStatusMessage(objMsg);
	}
}
void CMainFrame::OnUpdateEditIncrimentalsearch(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//エンプティ＆書込み禁止チェック
	if( pobjEditorDoc->IsEmpty() || pobjEditorDoc->IsReadOnly() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（ＧＲＥＰ関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「検索－ＧＲＥＰ」選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditGrep() 
{
	CGrepDlg		objGrepDlg;

	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd != NULL)
	{
		//アクティブビュー存在チェック
		CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
		CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
		if(pobjEditorView != NULL && pobjEditorDoc != NULL)
		{
			//印刷・印刷プレビュー中チェック
			if( pobjEditorDoc->GetFlagPrintPreview() == FALSE )
			{
				//もし範囲選択されていた場合には、それを検索文字列とする
				if(pobjEditorView->IsSelected() && pobjEditorView->GetSelectLineCount()==1)
				{
					long lSelectLineCnt=0;
					CLine* pobjLine=pobjEditorView->MyGetSelectedText(&lSelectLineCnt);
					m_objGrepStr = pobjLine->String();
					delete pobjLine;
				}
			}
		}
	}

	//ＧＲＥＰ条件デフォルト設定
	objGrepDlg.m_bIsMatchCaseGrep = m_bIsMatchCaseGrep;				// 大文字、小文字区別の有無
	objGrepDlg.m_bIsMatchWholeWordGrep = m_bIsMatchWholeWordGrep;	// 単語全体が一致する事が条件か？
	objGrepDlg.m_bIsSearchSubFolderGrep = m_bIsSearchSubFolderGrep;	// サブフォルダも検索するか？
	objGrepDlg.m_bIsUseRegularExpression = m_bIsUseRegularExpressionGrep; // 正規表現を使用するか？
	objGrepDlg.m_objFolder = m_objFolderGrep;						// ＧＲＥＰ実行フォルダ
	objGrepDlg.m_objGrepString = m_objGrepStr;						// 検索文字列
	objGrepDlg.m_objFileKind = m_objFileKindGrep;					// 検索対象のファイル種別

	if( objGrepDlg.DoModal() != IDOK )
		return;

	//ＧＲＥＰ条件取得＆保持
	m_bIsMatchCaseGrep = objGrepDlg.m_bIsMatchCaseGrep;				// 大文字、小文字区別の有無
	m_bIsMatchWholeWordGrep = objGrepDlg.m_bIsMatchWholeWordGrep;	// 単語全体が一致する事が条件か？
	m_bIsSearchSubFolderGrep = objGrepDlg.m_bIsSearchSubFolderGrep;	// サブフォルダも検索するか？
	m_bIsUseRegularExpressionGrep = objGrepDlg.m_bIsUseRegularExpression;	// 正規表現を使用するか？
	m_objFolderGrep = objGrepDlg.m_objFolder;						// ＧＲＥＰ実行フォルダ
	m_objGrepStr = objGrepDlg.m_objGrepString;						// 検索文字列
	m_objFileKindGrep = objGrepDlg.m_objFileKind;					// 検索対象のファイル種別

	//ＧＲＥＰ実行
	DoGrep();

	//正規表現を使った場合、regfreeを呼び出して後始末する
	if(m_bIsUseRegularExpressionGrep && m_pApp->m_lpfnRegFunc4!=NULL)
		m_pApp->m_lpfnRegFunc4();
}
void CMainFrame::OnUpdateEditGrep(CCmdUI* pCmdUI) 
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（文字コード変換関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ツール－文字コード変換」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditCodeconv()
{
	CString			objMsg;
	CCodeConvDlg	objCodeConvDlg;

	// 文字コード変換条件デフォルト設定
	objCodeConvDlg.m_bIsOutOtherFile = m_bIsOutOtherFileConv;		// 変換結果を別ファイルへ出力するか？
	objCodeConvDlg.m_bIsSearchSubFolder = m_bIsSearchSubFolderConv;	// サブフォルダも変換するか？
	objCodeConvDlg.m_nConvKind = m_nConvKind;						// 文字コード変換種別
	objCodeConvDlg.m_objFolder = m_objFolderConv;					// 文字コード変換実行フォルダ
	objCodeConvDlg.m_objFileKind = m_objFileKindConv;				// 変換対象のファイル種別
	objCodeConvDlg.m_nInCode = m_nInCodeConv;						// 入力ファイルの文字コード判定種別

	if( objCodeConvDlg.DoModal() != IDOK )
		return;

	// 文字コード変換条件取得＆保持
	m_bIsOutOtherFileConv = objCodeConvDlg.m_bIsOutOtherFile;		// 変換結果を別ファイルへ出力するか？
	m_bIsSearchSubFolderConv = objCodeConvDlg.m_bIsSearchSubFolder;	// サブフォルダも変換するか？
	m_nConvKind = objCodeConvDlg.m_nConvKind;						// 文字コード変換種別
	m_objFolderConv = objCodeConvDlg.m_objFolder;					// 文字コード変換実行フォルダ
	m_objFileKindConv = objCodeConvDlg.m_objFileKind;				// 変換対象のファイル種別
	m_nInCodeConv = objCodeConvDlg.m_nInCode;						// 入力ファイルの文字コード判定種別

	// 文字コード変換実行
	DoCodeConv();
}
void CMainFrame::OnUpdateEditCodeconv(CCmdUI* pCmdUI)
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（改行コード変換関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ツール－改行コード変換」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditKaigyoconv() 
{
	CKaigyoConvDlg	objKaigyoConvDlg;

	// 改行コード変換条件デフォルト設定
	objKaigyoConvDlg.m_bIsOutOtherFile = m_bIsOutOtherFileKaigyoConv;		// 変換結果を別ファイルへ出力するか？
	objKaigyoConvDlg.m_bIsSearchSubFolder = m_bIsSearchSubFolderKaigyoConv;	// サブフォルダも変換するか？
	objKaigyoConvDlg.m_objConvKind = m_objKaigyoConvKind;					// 改行コード変換種別
	objKaigyoConvDlg.m_objFolder = m_objFolderKaigyoConv;					// 改行コード変換実行フォルダ
	objKaigyoConvDlg.m_objFileKind = m_objFileKindKaigyoConv;				// 変換対象のファイル種別

	if( objKaigyoConvDlg.DoModal() != IDOK )
		return;

	// 改行コード変換条件取得＆保持
	m_bIsOutOtherFileKaigyoConv = objKaigyoConvDlg.m_bIsOutOtherFile;		// 変換結果を別ファイルへ出力するか？
	m_bIsSearchSubFolderKaigyoConv = objKaigyoConvDlg.m_bIsSearchSubFolder;	// サブフォルダも変換するか？
	m_objKaigyoConvKind = objKaigyoConvDlg.m_objConvKind;					// 文字コード変換種別
	m_objFolderKaigyoConv = objKaigyoConvDlg.m_objFolder;					// 文字コード変換実行フォルダ
	m_objFileKindKaigyoConv = objKaigyoConvDlg.m_objFileKind;				// 変換対象のファイル種別

	// 改行コード変換実行
	DoKaigyoCodeConv();
}
void CMainFrame::OnUpdateEditKaigyoconv(CCmdUI* pCmdUI) 
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（ワードカウント関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ツール－ワードカウント」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditWordcount() 
{
	CWordCountDlg	objWordCountDlg;

	// ワードカウント条件デフォルト設定
	objWordCountDlg.m_bIsSearchSubFolder = m_bIsSearchSubFolderWCount;	// サブフォルダも変換するか？
	objWordCountDlg.m_objFolder = m_objFolderWCount;					// ワードカウント実行フォルダ
	objWordCountDlg.m_objFileKind = m_objFileKindWCount;				// ワードカウント対象のファイル種別

	if( objWordCountDlg.DoModal() != IDOK )
		return;

	// ワードカウント条件取得＆保持
	m_bIsSearchSubFolderWCount = objWordCountDlg.m_bIsSearchSubFolder;	// サブフォルダも変換するか？
	m_objFolderWCount = objWordCountDlg.m_objFolder;					// ワードカウント実行フォルダ
	m_objFileKindWCount = objWordCountDlg.m_objFileKind;				// ワードカウント対象のファイル種別

	// ワードカウント実行
	DoWordCount();
}
void CMainFrame::OnUpdateEditWordcount(CCmdUI* pCmdUI) 
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（ファイル検索関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ツール－ファイル検索」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFilesearch() 
{
	CFileSearchDlg	objFileSearchDlg;

	// ファイル検索条件デフォルト設定
	objFileSearchDlg.m_bIsSearchSubFolder = m_bIsSearchSubFolderFSearch;// サブフォルダも変換するか？
	objFileSearchDlg.m_objFolder = m_objFolderFSearch;					// ワードカウント実行フォルダ
	objFileSearchDlg.m_objFileKind = m_objFileKindFSearch;				// ワードカウント対象のファイル種別

	if( objFileSearchDlg.DoModal() != IDOK )
		return;

	// ファイル検索条件取得＆保持
	m_bIsSearchSubFolderFSearch = objFileSearchDlg.m_bIsSearchSubFolder;// サブフォルダも変換するか？
	m_objFolderFSearch = objFileSearchDlg.m_objFolder;					// ワードカウント実行フォルダ
	m_objFileKindFSearch = objFileSearchDlg.m_objFileKind;				// ワードカウント対象のファイル種別

	// ファイル検索実行
	DoFileSearch();
}
void CMainFrame::OnUpdateEditFilesearch(CCmdUI* pCmdUI) 
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（ファイルタッチ関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ツール－ファイルタッチ」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFiletouch() 
{
	CFileTouchDlg	objFileTouchDlg;

	// ファイルタッチ条件デフォルト設定
	objFileTouchDlg.m_bIsSearchSubFolder = m_bIsSearchSubFolderTouch;	// サブフォルダもタッチするか？
	objFileTouchDlg.m_objFolder = m_objFolderTouch;						// タッチ実行フォルダ
	objFileTouchDlg.m_objFileKind = m_objFileKindTouch;					// タッチ対象のファイル種別

	if( objFileTouchDlg.DoModal() != IDOK )
		return;

	// ファイルタッチ条件取得＆保持
	m_bIsSearchSubFolderTouch = objFileTouchDlg.m_bIsSearchSubFolder;	// サブフォルダもタッチするか？
	m_objFolderTouch = objFileTouchDlg.m_objFolder;						// タッチ実行フォルダ
	m_objFileKindTouch = objFileTouchDlg.m_objFileKind;					// タッチ対象のファイル種別
	m_objFileTouchTime = objFileTouchDlg.m_mtime;

	// ファイルタッチ実行
	DoFileTouch();
}
void CMainFrame::OnUpdateEditFiletouch(CCmdUI* pCmdUI) 
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（ダンプ関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ツール－ＡＳＣＩＩコード表示ダンプ」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditAsciidump() 
{
	long	lCRCount=0;
	long	lLFCount=0;
	char	szAsciiDataBuf[24];
	char	szAsciiData[128];
	strcpy(szAsciiData,ASCII_DATA);
	CLine	objHeaderLine;
	CString objMsg;

	//アクティブフレーム取得
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
		return;
	//アクティブドキュメント取得
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
		return;
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
		return;
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
		return;

	//ダンプ結果出力用ＭＤＩ子ウインドウ作成
	m_pobjDumpDoc = NULL;
	m_pobjDumpFrameWnd = NULL;
	BOOL bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjDumpDoc,&m_pobjDumpFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjDumpDoc->SetFontOfCurrentFileExt("");
	m_pobjDumpDoc->SetColorOfCurrentFileExt("");

	//ダンプモード設定
	SetReadOnlyMode(TRUE);//書込み禁止ﾓｰﾄﾞｽﾃｰﾀｽﾍﾟｲﾝ設定
	m_pobjDumpDoc->SetDumpMode(TRUE);

	//タイトル設定
	CString	objTitle;
	objMsg.LoadString(IDS_ASCIIDUMP_TITLE);
	objTitle.Format(objMsg,pobjEditorDoc->GetTitle());
	m_pobjDumpDoc->SetTitle(objTitle);

	//空白書込み
	objHeaderLine.Set((const unsigned char*)"");
	m_pobjDumpDoc->AppendDumpLine(objHeaderLine);
	//タイトル書込み
	objHeaderLine.Set((const unsigned char*)(LPCTSTR)objTitle);
	m_pobjDumpDoc->AppendDumpLine(objHeaderLine);
	//空白書込み
	objHeaderLine.Set((const unsigned char*)"");
	m_pobjDumpDoc->AppendDumpLine(objHeaderLine);
	//ゲージ書き込み
	objHeaderLine.Set((const unsigned char*)"| Offset | 0001 0203 0405 0607 0809 0A0B 0C0D 0E0F |      ASCII DATA");
	m_pobjDumpDoc->AppendDumpLine(objHeaderLine);


	long lLineCnt=0;		//行数カウンタ
	long lSumCnt=0;			//累積バイト数カウンタ
	long lOffset=0;			//１行内文字カウンタ
	CLine* pobjLine=NULL;	//１行バッファ
	CLine objDumpLine;		//ダンプ結果１行
	memset(szAsciiDataBuf,0,sizeof(szAsciiDataBuf));
	for( ; ; )
	{
		pobjLine = pobjEditorDoc->GetOneLine(lLineCnt);
		if(pobjLine==NULL)
			break;

		//一時的に改行コード追加
		CLine	objTempLine;
		if(lLineCnt>0)
		{
			//CR,LFの数取得
			long lCnt;
			pobjEditorDoc->GetCRLFCount(&lCRCount,&lLFCount);
			if( lCRCount!=0 || lLFCount!=0 )
			{
				for(lCnt=0; lCnt<lCRCount; lCnt++)
					objTempLine.Catenate((const unsigned char*)"\r");
				for(lCnt=0; lCnt<lLFCount; lCnt++)
					objTempLine.Catenate((const unsigned char*)"\n");
			}
			else
			{
				long lKaigyoKind = pobjEditorDoc->GetKaigyoKind();
				if(lKaigyoKind==KAIGYO_CRLF)
					objTempLine.Catenate((const unsigned char*)"\r\n");
				else if(lKaigyoKind==KAIGYO_LF)
					objTempLine.Catenate((const unsigned char*)"\n");
				else if(lKaigyoKind==KAIGYO_CR)
					objTempLine.Catenate((const unsigned char*)"\r");
				else 
					objTempLine.Catenate((const unsigned char*)"\r\n");
			}
			objTempLine.Catenate(pobjLine->String());
		}
		else
		{
			objTempLine.Set(pobjLine->String());
		}

		for(lOffset=0; lOffset<objTempLine.GetLength(); lOffset++, lSumCnt++) //１行内文字ループ
		{
			if(!(lSumCnt%256))
			{
				if(objDumpLine.GetLength()!=0)
				{
					//ｱｽｷｰﾃﾞｰﾀ書込み
					objDumpLine.Catenate((const unsigned char*)"     ");
					objDumpLine.Catenate((const unsigned char*)szAsciiDataBuf);
					memset(szAsciiDataBuf,0,sizeof(szAsciiDataBuf));
					//いままでのダンプ結果を登録
					m_pobjDumpDoc->AppendDumpLine(objDumpLine);
				}
				//セパレータ登録
				CLine objTempLine((const unsigned char*)"|--------|-----------------------------------------|");
				m_pobjDumpDoc->AppendDumpLine(objTempLine);
			}
			if((lSumCnt!=0)&&(!(lSumCnt%2)))
			{
				objDumpLine.Catenate((const unsigned char*)" ");
			}
			if((!(lSumCnt%16)))
			{
				if( (lSumCnt%256)!=0 )
				{
					if(objDumpLine.GetLength()!=0)
					{
						//ｱｽｷｰﾃﾞｰﾀ書込み
						objDumpLine.Catenate((const unsigned char*)"    ");
						objDumpLine.Catenate((const unsigned char*)szAsciiDataBuf);
						memset(szAsciiDataBuf,0,sizeof(szAsciiDataBuf));
						//いままでのダンプ結果を登録
						m_pobjDumpDoc->AppendDumpLine(objDumpLine);
					}
				}
				//クリア
				objDumpLine.Set((const unsigned char*)"");
				char szBuf[64];
				sprintf(szBuf,"| %06X | ",lSumCnt);
				objDumpLine.Set((const unsigned char*)szBuf);
			}
			char szBuf[64];
			sprintf(szBuf,"%02X",(unsigned int)objTempLine.GetAt(lOffset));
			objDumpLine.Catenate((const unsigned char*)szBuf);

			//////////////////////////
			//	ＡＳＣＩＩでの表示	//
			//////////////////////////
			if(objTempLine.GetAt(lOffset) <= 0x7f)
				szAsciiDataBuf[(lSumCnt%16)] = szAsciiData[objTempLine.GetAt(lOffset)];
			else
				szAsciiDataBuf[(lSumCnt%16)] = '.';
		}

		//次の行へ
		lLineCnt++;
	}

	if(objDumpLine.GetLength()!=0)
	{
		//ｱｽｷｰﾃﾞｰﾀ書込み
		objDumpLine.PadSpace(55);
		objDumpLine.Catenate((const unsigned char*)szAsciiDataBuf);
		memset(szAsciiDataBuf,0,sizeof(szAsciiDataBuf));
		//いままでのダンプ結果を登録
		m_pobjDumpDoc->AppendDumpLine(objDumpLine);
	}

	m_pobjDumpDoc->UpdateAllViews(NULL,NULL,NULL);
}
void CMainFrame::OnUpdateEditAsciidump(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//ドキュメントエンプティチェック
	if( pobjEditorDoc->IsEmpty() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ツール－ＳＪＩＳコード表示ダンプ」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditSjisdump()
{
	long	lCRCount=0;
	long	lLFCount=0;
	char	szSjisDataBuf[24];
	char	szAsciiData[128];
	strcpy(szAsciiData,ASCII_DATA);
	CLine	objHeaderLine;
	CString objMsg;

	//アクティブフレーム取得
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
		return;
	//アクティブドキュメント取得
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
		return;
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
		return;
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
		return;

	//ダンプ結果出力用ＭＤＩ子ウインドウ作成
	m_pobjDumpDoc = NULL;
	m_pobjDumpFrameWnd = NULL;
	BOOL bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjDumpDoc,&m_pobjDumpFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjDumpDoc->SetFontOfCurrentFileExt("");
	m_pobjDumpDoc->SetColorOfCurrentFileExt("");

	//ダンプモード設定
	SetReadOnlyMode(TRUE);//書込み禁止ﾓｰﾄﾞｽﾃｰﾀｽﾍﾟｲﾝ設定
	m_pobjDumpDoc->SetDumpMode(TRUE);

	//タイトル設定
	CString	objTitle;
	objMsg.LoadString(IDS_SJISDUMP_TITLE);
	objTitle.Format(objMsg,pobjEditorDoc->GetTitle());
	m_pobjDumpDoc->SetTitle(objTitle);

	//空白書込み
	objHeaderLine.Set((const unsigned char*)"");
	m_pobjDumpDoc->AppendDumpLine(objHeaderLine);
	//タイトル書込み
	objHeaderLine.Set((const unsigned char*)(LPCTSTR)objTitle);
	m_pobjDumpDoc->AppendDumpLine(objHeaderLine);
	//空白書込み
	objHeaderLine.Set((const unsigned char*)"");
	m_pobjDumpDoc->AppendDumpLine(objHeaderLine);
	//ゲージ書き込み
	objHeaderLine.Set((const unsigned char*)"| Offset | 0001 0203 0405 0607 0809 0A0B 0C0D 0E0F |      SJIS DATA");
	m_pobjDumpDoc->AppendDumpLine(objHeaderLine);


	BOOL bPreIsDBCSLeadByte=FALSE;
	long lLineCnt=0;		//行数カウンタ
	long lSumCnt=0;			//累積バイト数カウンタ
	long lOffset=0;			//１行内文字カウンタ
	CLine* pobjLine=NULL;	//１行バッファ
	CLine objDumpLine;		//ダンプ結果１行
	memset(szSjisDataBuf,0,sizeof(szSjisDataBuf));
	for( ; ; )
	{
		pobjLine = pobjEditorDoc->GetOneLine(lLineCnt);
		if(pobjLine==NULL)
			break;

		//一時的に改行コード追加
		CLine	objTempLine;
		if(lLineCnt>0)
		{
			//CR,LFの数取得
			long lCnt;
			pobjEditorDoc->GetCRLFCount(&lCRCount,&lLFCount);
			if( lCRCount!=0 || lLFCount!=0 )
			{
				for(lCnt=0; lCnt<lCRCount; lCnt++)
					objTempLine.Catenate((const unsigned char*)"\r");
				for(lCnt=0; lCnt<lLFCount; lCnt++)
					objTempLine.Catenate((const unsigned char*)"\n");
			}
			else
			{
				long lKaigyoKind = pobjEditorDoc->GetKaigyoKind();
				if(lKaigyoKind==KAIGYO_CRLF)
					objTempLine.Catenate((const unsigned char*)"\r\n");
				else if(lKaigyoKind==KAIGYO_LF)
					objTempLine.Catenate((const unsigned char*)"\n");
				else if(lKaigyoKind==KAIGYO_CR)
					objTempLine.Catenate((const unsigned char*)"\r");
				else
					objTempLine.Catenate((const unsigned char*)"\r\n");
			}
			objTempLine.Catenate(pobjLine->String());
		}
		else
		{
			objTempLine.Set(pobjLine->String());
		}

		for(lOffset=0; lOffset<objTempLine.GetLength(); lOffset++, lSumCnt++) //１行内文字ループ
		{
			if(!(lSumCnt%256))
			{
				if(objDumpLine.GetLength()!=0)
				{
					//SJISﾃﾞｰﾀ書込み
					objDumpLine.Catenate((const unsigned char*)"     ");
					objDumpLine.Catenate((const unsigned char*)szSjisDataBuf);
					memset(szSjisDataBuf,0,sizeof(szSjisDataBuf));
					//いままでのダンプ結果を登録
					m_pobjDumpDoc->AppendDumpLine(objDumpLine);
				}
				//セパレータ登録
				CLine objTempLine((const unsigned char*)"|--------|-----------------------------------------|");
				m_pobjDumpDoc->AppendDumpLine(objTempLine);
			}
			if((lSumCnt!=0)&&(!(lSumCnt%2)))
			{
				objDumpLine.Catenate((const unsigned char*)" ");
			}
			if((!(lSumCnt%16)))
			{
				if( (lSumCnt%256)!=0 )
				{
					if(objDumpLine.GetLength()!=0)
					{
						//SJISﾃﾞｰﾀ書込み
						objDumpLine.Catenate((const unsigned char*)"    ");
						objDumpLine.Catenate((const unsigned char*)szSjisDataBuf);
						memset(szSjisDataBuf,0,sizeof(szSjisDataBuf));
						//いままでのダンプ結果を登録
						m_pobjDumpDoc->AppendDumpLine(objDumpLine);
					}
				}
				//クリア
				objDumpLine.Set((const unsigned char*)"");
				char szBuf[64];
				sprintf(szBuf,"| %06X | ",lSumCnt);
				objDumpLine.Set((const unsigned char*)szBuf);
			}
			char szBuf[64];
			sprintf(szBuf,"%02X",(unsigned int)objTempLine.GetAt(lOffset));
			objDumpLine.Catenate((const unsigned char*)szBuf);

			//////////////////////////////////
			//	Ｓｈｉｆｔ－ＪＩＳでの表示	//
			//////////////////////////////////

			if(bPreIsDBCSLeadByte==FALSE)
			{
				if(!IsDBCSLeadByteEx(0,objTempLine.GetAt(lOffset))) //１バイト文字
				{
					if(objTempLine.GetAt(lOffset) <= 0x7f)
						szSjisDataBuf[(lSumCnt%16)] = szAsciiData[objTempLine.GetAt(lOffset)];
					else if(isHanKana(objTempLine.GetAt(lOffset)))
						szSjisDataBuf[(lSumCnt%16)] = objTempLine.GetAt(lOffset);
					else
						szSjisDataBuf[(lSumCnt%16)] = '.';
					bPreIsDBCSLeadByte=FALSE;
				}
				else if(IsDBCSLeadByteEx(0,objTempLine.GetAt(lOffset)))//２バイト文字の第一バイト
				{
					if((lSumCnt%16)==15) //１６番目
						szSjisDataBuf[(lSumCnt%16)] = '.';
					else
						szSjisDataBuf[(lSumCnt%16)] = objTempLine.GetAt(lOffset);
					bPreIsDBCSLeadByte=TRUE;
				}
			}
			else //２バイト文字の第二バイト
			{
				if((lSumCnt%16)==0) //１番目
					szSjisDataBuf[(lSumCnt%16)] = '.';
				else
					szSjisDataBuf[(lSumCnt%16)] = objTempLine.GetAt(lOffset);
				bPreIsDBCSLeadByte=FALSE;
			}
		}

		//次の行へ
		lLineCnt++;
	}

	if(objDumpLine.GetLength()!=0)
	{
		//SJISﾃﾞｰﾀ書込み
		objDumpLine.PadSpace(55);
		objDumpLine.Catenate((const unsigned char*)szSjisDataBuf);
		memset(szSjisDataBuf,0,sizeof(szSjisDataBuf));
		//いままでのダンプ結果を登録
		m_pobjDumpDoc->AppendDumpLine(objDumpLine);
	}

	m_pobjDumpDoc->UpdateAllViews(NULL,NULL,NULL);
}
void CMainFrame::OnUpdateEditSjisdump(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//ドキュメントエンプティチェック
	if( pobjEditorDoc->IsEmpty() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（暗号化／復号化関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ツール－ファイル暗号化」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnSecurityEncript() 
{
	CEncriptDlg	objEncriptDlg;

	// ファイル暗号化条件デフォルト設定
	objEncriptDlg.m_bIsOutOtherFile = m_bIsOutOtherFileEncript;			// 変換結果を別ファイルへ出力するか？
	objEncriptDlg.m_bIsSearchSubFolder = m_bIsSearchSubFolderEncript;	// サブフォルダも変換するか？
	objEncriptDlg.m_objKey = "";										// ファイル暗号化キー
	objEncriptDlg.m_objFolder = m_objFolderEncript;						// ファイル暗号化実行フォルダ
	objEncriptDlg.m_objFileKind = m_objFileKindEncript;					// 暗号化対象のファイル種別

	if( objEncriptDlg.DoModal() != IDOK )
		return;

	// ファイル暗号化条件取得＆保持
	m_bIsOutOtherFileEncript = objEncriptDlg.m_bIsOutOtherFile;			// 変換結果を別ファイルへ出力するか？
	m_bIsSearchSubFolderEncript = objEncriptDlg.m_bIsSearchSubFolder;	// サブフォルダも変換するか？
	m_objKeyEncript = objEncriptDlg.m_objKey;							// ファイル暗号化キー
	m_objFolderEncript = objEncriptDlg.m_objFolder;						// ファイル暗号化実行フォルダ
	m_objFileKindEncript = objEncriptDlg.m_objFileKind;					// 暗号化対象のファイル種別

	// ファイル暗号化実行
	DoFileEncript();
}
void CMainFrame::OnUpdateSecurityEncript(CCmdUI* pCmdUI) 
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ツール－ファイル復号化」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnSecurityDecript() 
{
	CDecriptDlg	objDecriptDlg;

	// ファイル復号化条件デフォルト設定
	objDecriptDlg.m_bIsOutOtherFile = m_bIsOutOtherFileDecript;			// 変換結果を別ファイルへ出力するか？
	objDecriptDlg.m_bIsSearchSubFolder = m_bIsSearchSubFolderDecript;	// サブフォルダも変換するか？
	objDecriptDlg.m_objKey = "";										// ファイル復号化キー
	objDecriptDlg.m_objFolder = m_objFolderDecript;						// ファイル復号化実行フォルダ
	objDecriptDlg.m_objFileKind = m_objFileKindDecript;					// 復号化対象のファイル種別

	if( objDecriptDlg.DoModal() != IDOK )
		return;

	// ファイル復号化条件取得＆保持
	m_bIsOutOtherFileDecript = objDecriptDlg.m_bIsOutOtherFile;			// 変換結果を別ファイルへ出力するか？
	m_bIsSearchSubFolderDecript = objDecriptDlg.m_bIsSearchSubFolder;	// サブフォルダも変換するか？
	m_objKeyDecript = objDecriptDlg.m_objKey;							// ファイル復号化キー
	m_objFolderDecript = objDecriptDlg.m_objFolder;						// ファイル復号化実行フォルダ
	m_objFileKindDecript = objDecriptDlg.m_objFileKind;					// 復号化対象のファイル種別

	// ファイル復号化実行
	DoFileDecript();
}
void CMainFrame::OnUpdateSecurityDecript(CCmdUI* pCmdUI) 
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（エンコード関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ツール－ＵＵＥＮＣＯＤＥ」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditUuencode() 
{
	CString objMsg;

	//入力ファイル名取得
	char	szBuf[1024];
	char	szFileName[_MAX_FNAME];
	char	szFilePath[_MAX_PATH];
	CString objStrFName, objStrFPath;
	CFileDialog	objFileDialog(TRUE);
	objMsg.LoadString(IDS_UUENCODE_INFILE_TITLE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if( objFileDialog.DoModal() != IDOK )
		return;

	//ファイル名取得
	objStrFPath = objFileDialog.GetPathName();
	strcpy(szFilePath,(LPCTSTR)objStrFPath);
	objStrFName = objFileDialog.GetFileName();
	strcpy(szFileName,(LPCTSTR)objStrFName);

	//入力ファイルオープン
	CFile	objFile;
	if( objFile.Open( szFilePath, CFile::modeRead|CFile::typeBinary ) == NULL )
	{
		/* ファイルオープンエラー */
		objMsg.LoadString(IDS_FOPEN_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	CArchive	ar(&objFile,CArchive::load);

	//エンコード結果出力用ＭＤＩ子ウインドウ作成
	CEditorDoc*	pobjEncodeDoc = NULL;
	CFrameWnd*	pobjEncodeFrameWnd = NULL;
	BOOL bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&pobjEncodeDoc,&pobjEncodeFrameWnd);
	if( bRet == FALSE )
		return;
	pobjEncodeDoc->SetFontOfCurrentFileExt("");
	pobjEncodeDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//書込み禁止ﾓｰﾄﾞｽﾃｰﾀｽﾍﾟｲﾝ設定
	CString	objTitle;
	objTitle = szFilePath;
	objMsg.LoadString(IDS_UUENCODE_RESULT_TITLE);
	objTitle += objMsg;
	pobjEncodeDoc->SetTitle(objTitle);
	pobjEncodeDoc->SetEncodeMode(TRUE);

	//タイトル追加
	CLine	objEncodeResult;
	objEncodeResult.Set((const unsigned char*)szFilePath);
	objMsg.LoadString(IDS_UUENCODE_RESULT_TITLE);
	objEncodeResult.Catenate((const unsigned char*)(LPCTSTR)objMsg);
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
	objEncodeResult.Set((const unsigned char*)"");
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);

	//begin 行書込み
	sprintf(szBuf, "begin 666 %s", szFileName);
	objEncodeResult.Set((const unsigned char*)szBuf);
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);

	//ＵＵＥＮＣＯＤＥ
	CUuencoder	objUuencoder;
	CString objBuf;
	BYTE	byBuf[1024];
	int		nReadLength = 45, nBytesRead=0;
	for(;;)
	{
		try
		{
			memset(byBuf,0,sizeof(byBuf));
			nBytesRead = ar.Read( byBuf, nReadLength );
		}
		catch( CFileException* e )
		{
			delete e;
			break;
		}
		if(nBytesRead == 0)
			break;

		//エンコード
		objBuf.Empty();
		objBuf = objUuencoder.BytesEncode(byBuf,nBytesRead);
		if(!objBuf.IsEmpty())
		{
			objEncodeResult.Set((const unsigned char*)(LPCTSTR)objBuf);
			pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
		}
	}

	//end 行書込み
	objEncodeResult.Set((const unsigned char*)"`");
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
	objEncodeResult.Set((const unsigned char*)"end");
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
	objEncodeResult.Set((const unsigned char*)"");
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);

	pobjEncodeDoc->UpdateAllViews(NULL,NULL,NULL);

	//入力ファイルクローズ
	ar.Close();
	objFile.Close();
}
void CMainFrame::OnUpdateEditUuencode(CCmdUI* pCmdUI) 
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ツール－Ｂａｓｅ６４エンコード」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditBase64encode() 
{
	CString objMsg;

	//入力ファイル名取得
	char	szFileName[_MAX_FNAME];
	char	szFilePath[_MAX_PATH];
	CString objStrFName, objStrFPath;
	CFileDialog	objFileDialog(TRUE);
	objMsg.LoadString(IDS_BASE64_INFILE_TITLE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if( objFileDialog.DoModal() != IDOK )
		return;

	//ファイル名取得
	objStrFPath = objFileDialog.GetPathName();
	strcpy(szFilePath,(LPCTSTR)objStrFPath);
	objStrFName = objFileDialog.GetFileName();
	strcpy(szFileName,(LPCTSTR)objStrFName);

	//入力ファイルオープン
	CFile	objFile;
	if( objFile.Open( szFilePath, CFile::modeRead|CFile::typeBinary ) == NULL )
	{
		/* ファイルオープンエラー */
		objMsg.LoadString(IDS_FOPEN_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	CArchive	ar(&objFile,CArchive::load);

	//エンコード結果出力用ＭＤＩ子ウインドウ作成
	CEditorDoc*	pobjEncodeDoc = NULL;
	CFrameWnd*	pobjEncodeFrameWnd = NULL;
	BOOL bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&pobjEncodeDoc,&pobjEncodeFrameWnd);
	if( bRet == FALSE )
		return;
	pobjEncodeDoc->SetFontOfCurrentFileExt("");
	pobjEncodeDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//書込み禁止ﾓｰﾄﾞｽﾃｰﾀｽﾍﾟｲﾝ設定
	CString	objTitle;
	objTitle = szFilePath;
	objMsg.LoadString(IDS_BASE64_RESULT_TITLE);
	objTitle += objMsg;
	pobjEncodeDoc->SetTitle(objTitle);
	pobjEncodeDoc->SetEncodeMode(TRUE);

	//タイトル追加
	CLine	objEncodeResult;
	objEncodeResult.Set((const unsigned char*)szFilePath);
	objMsg.LoadString(IDS_BASE64_RESULT_TITLE);
	objEncodeResult.Catenate((const unsigned char*)(LPCTSTR)objMsg);
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
	objEncodeResult.Set((const unsigned char*)"");
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);

	//Ｂａｓｅ６４エンコード
	CBase64	objBase64;
	CString objBuf;
	BYTE	byBuf[1024];
	int		nReadLength = 54, nBytesRead=0;
	for(;;)
	{
		try
		{
			memset(byBuf,0,sizeof(byBuf));
			nBytesRead = ar.Read( byBuf, nReadLength );
		}
		catch( CFileException* e )
		{
			delete e;
			break;
		}
		if(nBytesRead == 0)
			break;

		//エンコード
		objBuf.Empty();
		objBuf = objBase64.BytesEncode(byBuf,nBytesRead);
		if(!objBuf.IsEmpty())
		{
			objEncodeResult.Set((const unsigned char*)(LPCTSTR)objBuf);
			pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
		}
	}

	objEncodeResult.Set((const unsigned char*)"");
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
	pobjEncodeDoc->UpdateAllViews(NULL,NULL,NULL);

	//入力ファイルクローズ
	ar.Close();
	objFile.Close();
}
void CMainFrame::OnUpdateEditBase64encode(CCmdUI* pCmdUI) 
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ツール－Quoted-Printableエンコード」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditQuotedencode() 
{
	CString objMsg;

	//入力ファイル名取得
	char*	pPtr=NULL;
	char	szFileName[_MAX_FNAME];
	char	szFilePath[_MAX_PATH];
	CString objStrFName, objStrFPath;
	CFileDialog	objFileDialog(TRUE);
	objMsg.LoadString(IDS_QUOTED_INFILE_TITLE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if( objFileDialog.DoModal() != IDOK )
		return;

	//ファイル名取得
	objStrFPath = objFileDialog.GetPathName();
	strcpy(szFilePath,(LPCTSTR)objStrFPath);
	objStrFName = objFileDialog.GetFileName();
	strcpy(szFileName,(LPCTSTR)objStrFName);

	//入力ファイルオープン
	CFile	objFile;
	if( objFile.Open( szFilePath, CFile::modeRead|CFile::typeBinary ) == NULL )
	{
		/* ファイルオープンエラー */
		objMsg.LoadString(IDS_FOPEN_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	CArchive	ar(&objFile,CArchive::load);

	//エンコード結果出力用ＭＤＩ子ウインドウ作成
	CEditorDoc*	pobjEncodeDoc = NULL;
	CFrameWnd*	pobjEncodeFrameWnd = NULL;
	BOOL bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&pobjEncodeDoc,&pobjEncodeFrameWnd);
	if( bRet == FALSE )
		return;
	pobjEncodeDoc->SetFontOfCurrentFileExt("");
	pobjEncodeDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//書込み禁止ﾓｰﾄﾞｽﾃｰﾀｽﾍﾟｲﾝ設定
	CString	objTitle;
	objTitle = szFilePath;
	objMsg.LoadString(IDS_QUOTED_RESULT_TITLE);
	objTitle += objMsg;
	pobjEncodeDoc->SetTitle(objTitle);
	pobjEncodeDoc->SetEncodeMode(TRUE);

	//タイトル追加
	CLine	objEncodeResult;
	objEncodeResult.Set((const unsigned char*)szFilePath);
	objMsg.LoadString(IDS_QUOTED_RESULT_TITLE);
	objEncodeResult.Catenate((const unsigned char*)(LPCTSTR)objMsg);
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
	objEncodeResult.Set((const unsigned char*)"");
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);

	//Ｑｕｏｔｅｄ－Ｐｒｉｎｔａｂｌｅエンコード
	CQuotedPrintable	objQuotedPrintable;
	CString objBuf=_T("");
	BYTE	byBuf[1024];
	int		nReadLength = 22, nBytesRead=0;
	for(;;)
	{
		try
		{
			memset(byBuf,0,sizeof(byBuf));
			nBytesRead = ar.Read( byBuf, nReadLength );
		}
		catch( CFileException* e )
		{
			delete e;
			break;
		}

		if(!objBuf.IsEmpty())
		{
			if(nBytesRead != 0)
				objBuf += '=';//ソフト改行
			objEncodeResult.Set((const unsigned char*)(LPCTSTR)objBuf);
			pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
		}

		if(nBytesRead == 0)
			break;

		//エンコード
		objBuf.Empty();
		objBuf = objQuotedPrintable.BytesEncode(byBuf,nBytesRead);
	}

	objEncodeResult.Set((const unsigned char*)"");
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
	pobjEncodeDoc->UpdateAllViews(NULL,NULL,NULL);

	//入力ファイルクローズ
	ar.Close();
	objFile.Close();
}
void CMainFrame::OnUpdateEditQuotedencode(CCmdUI* pCmdUI) 
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（ファイル比較関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「検索－ファイル比較」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFilecompare() 
{
	CString			objMsg;
	CFileDialog		objFileDialog(TRUE);
	CString 		objFilePath1, objFilePath2;

	//第一ファイルパス取得
	objMsg.LoadString(IDS_FILECOMPARE_SEL1FILE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if( objFileDialog.DoModal() != IDOK )
		return;
	objFilePath1 = objFileDialog.GetPathName();

	//第二ファイルパス取得
	objMsg.LoadString(IDS_FILECOMPARE_SEL2FILE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if( objFileDialog.DoModal() != IDOK )
		return;
	objFilePath2 = objFileDialog.GetPathName();

	//ファイル比較実行
	DoFileCompare(objFilePath1,objFilePath2,NULL);
}
void CMainFrame::OnUpdateEditFilecompare(CCmdUI* pCmdUI) 
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル比較詳細オプション－タブとスペースを区別しない」選択時処理	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFcompTabequal2space() 
{
	BOOL bIsAssumeTabEqual2Space = m_pApp->GetProfileInt("CFileCompare","bIsAssumeTabEqual2Space",FALSE);
	if(bIsAssumeTabEqual2Space)
		m_pApp->WriteProfileInt("CFileCompare","bIsAssumeTabEqual2Space",FALSE);
	else
		m_pApp->WriteProfileInt("CFileCompare","bIsAssumeTabEqual2Space",TRUE);
}
void CMainFrame::OnUpdateEditFcompTabequal2space(CCmdUI* pCmdUI) 
{
	BOOL bIsAssumeTabEqual2Space = m_pApp->GetProfileInt("CFileCompare","bIsAssumeTabEqual2Space",FALSE);
	if(bIsAssumeTabEqual2Space)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル比較詳細オプション－全角と半角を区別しない」選択時処理		|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFcompZenequal2han() 
{
	BOOL bIsAssumeZenEqual2Han = m_pApp->GetProfileInt("CFileCompare","bIsAssumeZenEqual2Han",FALSE);
	if(bIsAssumeZenEqual2Han)
		m_pApp->WriteProfileInt("CFileCompare","bIsAssumeZenEqual2Han",FALSE);
	else
		m_pApp->WriteProfileInt("CFileCompare","bIsAssumeZenEqual2Han",TRUE);
}
void CMainFrame::OnUpdateEditFcompZenequal2han(CCmdUI* pCmdUI) 
{
	BOOL bIsAssumeZenEqual2Han = m_pApp->GetProfileInt("CFileCompare","bIsAssumeZenEqual2Han",FALSE);
	if(bIsAssumeZenEqual2Han)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル比較詳細オプション－英字の大文字小文字を区別しない」		|*/
/*|           選択時処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFcompAlphabetbigandsmall() 
{
	BOOL bIsAssumeAlphabetBigAndSmall = m_pApp->GetProfileInt("CFileCompare","bIsAssumeAlphabetBigAndSmall",FALSE);
	if(bIsAssumeAlphabetBigAndSmall)
		m_pApp->WriteProfileInt("CFileCompare","bIsAssumeAlphabetBigAndSmall",FALSE);
	else
		m_pApp->WriteProfileInt("CFileCompare","bIsAssumeAlphabetBigAndSmall",TRUE);
}
void CMainFrame::OnUpdateEditFcompAlphabetbigandsmall(CCmdUI* pCmdUI) 
{
	BOOL bIsAssumeAlphabetBigAndSmall = m_pApp->GetProfileInt("CFileCompare","bIsAssumeAlphabetBigAndSmall",FALSE);
	if(bIsAssumeAlphabetBigAndSmall)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「検索－ファイル再比較」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFilerecompare() 
{
	//アクティブドキュメント取得
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd==NULL)
		return;
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc==NULL)
		return;
	ASSERT_VALID(pobjEditorDoc);

	//ファイル比較結果表示か？
	if(pobjEditorDoc->IsCompare() == FALSE)
		return;

	//ファイル比較第一ファイル、第二ファイル取得
	CString objFilePath1,objFilePath2;
	pobjEditorDoc->GetCompareFiles(objFilePath1,objFilePath2);

	//表示中の結果データをいったんクリア
	for(;;)
	{
		if(pobjEditorDoc->GetLineCount() <= 0)
			break;
		pobjEditorDoc->DeleteOneLine(0);
	}

	//ファイル比較実行
	DoFileCompare(objFilePath1,objFilePath2,pobjEditorDoc);
}
void CMainFrame::OnUpdateEditFilerecompare(CCmdUI* pCmdUI) 
{
	//アクティブドキュメント取得
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd==NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc==NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	ASSERT_VALID(pobjEditorDoc);

	//印刷中チェック
	if(pobjEditorDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ドキュメントに１行もデータがない場合は選択不可にする
	if( pobjEditorDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ファイル比較結果かどうか
	if(pobjEditorDoc->IsCompare() == FALSE)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ファイル比較第一ファイル、第二ファイル取得 ＆存在チェック
	CString objFilePath1,objFilePath2;
	pobjEditorDoc->GetCompareFiles(objFilePath1,objFilePath2);
	if( _access( (LPCTSTR)objFilePath1, 0 ) == -1 || 
		_access( (LPCTSTR)objFilePath2, 0 ) == -1
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（ディレクトリ比較関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「検索－ディレクトリ比較」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditDircompare() 
{
	CDirCompareDlg	objDirCompareDlg;

	// ディレクトリ比較条件デフォルト設定
	objDirCompareDlg.m_objFolder1 = m_objFolder1DirCmp;					// 比較第一フォルダ
	objDirCompareDlg.m_objFolder2 = m_objFolder2DirCmp;					// 比較第二フォルダ

	if( objDirCompareDlg.DoModal() != IDOK )
		return;

	// ディレクトリ比較条件取得＆保持
	m_objFolder1DirCmp = objDirCompareDlg.m_objFolder1;					// 比較第一フォルダ
	m_objFolder2DirCmp = objDirCompareDlg.m_objFolder2;					// 比較第二フォルダ

	// ディレクトリ比較実行
	DoDirCompare();
}
void CMainFrame::OnUpdateEditDircompare(CCmdUI* pCmdUI) 
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（設定関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「設定－設定」選択時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：「例外処理 (初回) は Editor.exe (COMCTL32.DLL) にあります: 0xC0000005:|*/
/*|			   ｱｸｾｽ違反。」のメッセージがデバッグウインドウに表示される。しかし、	|*/
/*|			   CPropertySheet::DoModal()のヘルプによると、このアクセス違反は無視	|*/
/*|			   してよいものであるそうだ。											|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnSettings() 
{
	CFrameWnd*   pobjFrameWnd   = (CFrameWnd*)GetActiveFrame();
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();

	//印刷中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
		return;

	//設定用プロパティシート
	CMyPropertySheet propSheet;
	propSheet.m_pobjMainFrame = this;
	propSheet.m_pobjEditorActiveDoc = pobjEditorDoc;
	propSheet.m_pobjEditorActiveView = pobjEditorView;

	//現状の設定を反映
	propSheet.m_Page1.m_bShowSplash         = m_pApp->IsShowSplash();
	propSheet.m_Page1.m_lSplashTime         = m_pApp->GetProfileInt("CEditorApp","SplashTime",2);
	propSheet.m_Page1.m_nMaxMRU             = m_pApp->GetMaxMRU();
	propSheet.m_Page1.m_bIsNoDoubleExecute  = m_pApp->GetProfileInt("CEditorApp","bIsNoDoubleExecute",0);
	propSheet.m_Page1.m_bIsOpenNewFile      = m_pApp->IsOpenNewFile();
	propSheet.m_Page1.m_bIsChdirToOpenFile  = m_pApp->IsChdirToOpenFile();
	propSheet.m_Page1.m_bMaximizeWindow     = m_pApp->GetProfileInt("CChildFrame","bMaximizeWindow",0);
	propSheet.m_Page1.m_bIsSaveWindowPlace  = m_bIsSaveWindowPlace;
	propSheet.m_Page1.m_bIsFullTitle        = m_pApp->GetProfileInt("CEditorDoc","bIsFullTitle",0);
	propSheet.m_Page1.m_bIsDispEOFMark      = m_pApp->GetProfileInt("CEditorView","bIsDispEOFMark",1);
	propSheet.m_Page1.m_bIsExecPasswd       = m_pApp->GetProfileInt("CEditorApp","bIsExecPasswd",0);
	propSheet.m_Page1.m_objExecPasswdStr    = m_pApp->GetProfileString("CEditorApp","ExecPasswd","");
	propSheet.m_Page1.m_bIsEnableShellOpen  = m_pApp->GetProfileInt("CEditorApp","bIsEnableShellOpen",0);
	propSheet.m_Page1.m_bIsMakeShotcutDsktop= m_pApp->GetProfileInt("CEditorApp","bIsMakeShortcutDsktop",0);
	propSheet.m_Page1.m_bIsImmAutoOpen      = m_pApp->GetProfileInt("CEditorView","IsImmAutoOpen",FALSE);
	propSheet.m_Page2.m_objFileExt          = pobjEditorDoc->MyGetFileExt();
	propSheet.m_Page2.m_lDispFontPoint      = pobjEditorView->GetDispFontPoint();
	propSheet.m_Page2.m_objDispFontName     = pobjEditorView->GetDispFontName();
	propSheet.m_Page2.m_lPrintFontPoint     = pobjEditorView->GetPrintFontPoint();
	propSheet.m_Page2.m_objPrintFontName    = pobjEditorView->GetPrintFontName();
	memcpy(&propSheet.m_Page2.m_stStatusBarLogFont,&m_stStatusBarLogFont,sizeof(LOGFONT));
	if( pobjEditorDoc->GetTabSize() == 2 )
		propSheet.m_Page2.m_nTabSize        = 0;
	else if( pobjEditorDoc->GetTabSize() == 4 )
		propSheet.m_Page2.m_nTabSize        = 1;
	else if( pobjEditorDoc->GetTabSize() == 8 )
		propSheet.m_Page2.m_nTabSize        = 2;
	propSheet.m_Page3.m_objFileExt          = pobjEditorDoc->MyGetFileExt();
	propSheet.m_Page3.m_FontColor           = pobjEditorView->GetFontColor();
	propSheet.m_Page3.m_BackColor           = pobjEditorView->GetBackColor();
	propSheet.m_Page3.m_InvertFontColor     = pobjEditorView->GetInvertFontColor();
	propSheet.m_Page3.m_InvertBackColor     = pobjEditorView->GetInvertBackColor();
	propSheet.m_Page3.m_bDispLineNo         = pobjEditorView->GetFlagDispLineNo();
	propSheet.m_Page3.m_LineNoColor         = pobjEditorView->GetLineNoColor();
	propSheet.m_Page3.m_LineNoBackColor     = pobjEditorView->GetLineNoBackColor();
	propSheet.m_Page3.m_KeyNo1Color         = pobjEditorDoc->GetKeyNo1Color();
	propSheet.m_Page3.m_KeyNo2Color         = pobjEditorDoc->GetKeyNo2Color();
	propSheet.m_Page3.m_KeyNo3Color         = pobjEditorDoc->GetKeyNo3Color();
	propSheet.m_Page3.m_KeyNo4Color         = pobjEditorDoc->GetKeyNo4Color();
	propSheet.m_Page3.m_MDIBkgndColor       = m_wndClient.GetMDIBkgndColor();
	propSheet.m_Page3.m_MarkLineColor       = pobjEditorView->GetMarkLineColor();
	propSheet.m_Page3.m_bComment1Clr        = pobjEditorDoc->GetFlagComment1Clr();
	propSheet.m_Page3.m_CommentColor        = pobjEditorDoc->GetCommentColor();
	propSheet.m_Page3.m_bComment2Clr        = pobjEditorDoc->GetFlagComment2Clr();
	propSheet.m_Page3.m_CommentColor2       = pobjEditorDoc->GetCommentColor2();
	propSheet.m_Page3.m_bComment3Clr        = pobjEditorDoc->GetFlagComment3Clr();
	propSheet.m_Page3.m_CommentColor3       = pobjEditorDoc->GetCommentColor3();
	propSheet.m_Page3.m_bComment4Clr        = pobjEditorDoc->GetFlagComment4Clr();
	propSheet.m_Page3.m_CommentColor4       = pobjEditorDoc->GetCommentColor4();
	propSheet.m_Page3.m_bDispRuler          = pobjEditorView->GetFlagDispRuler();
	propSheet.m_Page3.m_RulerColor          = pobjEditorView->GetRulerColor();
	propSheet.m_Page3.m_RulerBackColor      = pobjEditorView->GetRulerBackColor();
	propSheet.m_Page3.m_bDispCRLF           = pobjEditorView->GetFlagDispCRLF();
	propSheet.m_Page3.m_CRLFColor           = pobjEditorView->GetCRLFColor();
	propSheet.m_Page3.m_bDispTab		    = pobjEditorView->GetFlagDispTab();
	propSheet.m_Page3.m_TabColor		    = pobjEditorView->GetTabColor();
	propSheet.m_Page3.m_bDispZenkakuSpace   = pobjEditorView->GetFlagDispZenkakuSpace();
	propSheet.m_Page3.m_ZenkakuSpaceColor   = pobjEditorView->GetZenkakuSpaceColor();
	propSheet.m_Page3.m_bDispUnderLine	    = pobjEditorView->GetFlagDispUnderLine();
	propSheet.m_Page3.m_UnderLineColor	    = pobjEditorView->GetUnderLineColor();
	propSheet.m_Page3.m_bDispUrl            = pobjEditorDoc->GetFlagUrlColor();
	propSheet.m_Page3.m_UrlColor			= pobjEditorDoc->GetUrlColor();
	propSheet.m_Page3.m_bDispEMail			= pobjEditorDoc->GetFlagEMailColor();
	propSheet.m_Page3.m_EMailColor          = pobjEditorDoc->GetEMailColor();
	propSheet.m_Page3.m_DiffDelLineColor    = pobjEditorView->GetDiffDelLineColor();
	propSheet.m_Page3.m_DiffAddLineColor    = pobjEditorView->GetDiffAddLineColor();
	propSheet.m_Page3.m_DirDiffLineColor    = pobjEditorView->GetDirDiffLineColor();
	propSheet.m_Page4.m_nKaigyoKind		    = (int)pobjEditorDoc->GetKaigyoKind();
	propSheet.m_Page4.m_bIsAutoSave		    = m_bIsAutoSave;
	propSheet.m_Page4.m_lSaveInterval	    = m_lSaveInterval;
	propSheet.m_Page4.m_bDelEndSpacesOnSave = pobjEditorDoc->GetFlagDelEndSpacesOnSave();
	propSheet.m_Page4.m_bSpace2TabOnSave    = pobjEditorDoc->GetFlagSpace2TabOnSave();
	propSheet.m_Page5.m_bPrintLineNo        = pobjEditorView->GetFlagPrintLineNo();
	propSheet.m_Page5.m_bPrint2Dangumi      = pobjEditorView->GetFlagPrint2Dangumi();
	propSheet.m_Page5.m_nPrintOrientation   = m_pApp->GetPrintOrientation();
	CRect objMarginRect;
	m_pApp->GetPrintMargin(objMarginRect);
	objMarginRect.NormalizeRect();
	propSheet.m_Page5.m_nYohakuHidari       = objMarginRect.left;
	propSheet.m_Page5.m_nYohakuUe           = objMarginRect.top;
	propSheet.m_Page5.m_nYohakuMigi         = objMarginRect.right;
	propSheet.m_Page5.m_nYohakuSita         = objMarginRect.bottom;
	propSheet.m_Page5.m_nHeaderLayout       = pobjEditorView->GetPrintHeaderLayoutPos();
	propSheet.m_Page5.m_nFooterLayout       = pobjEditorView->GetPrintFooterLayoutPos();
	pobjEditorView->GetStrHeaderFormat(propSheet.m_Page5.m_objStrHeaderFormat);
	pobjEditorView->GetStrFooterFormat(propSheet.m_Page5.m_objStrFooterFormat);
	if( pobjEditorView->GetCaretType() == CARET_IBEAM )
		propSheet.m_Page6.m_nCaretType      = 0;
	else if( pobjEditorView->GetCaretType() == CARET_SQUARE )
		propSheet.m_Page6.m_nCaretType      = 1;
	propSheet.m_Page6.m_unCaretBlinkTime    = ::GetCaretBlinkTime();
	if( pobjEditorView->GetCaretBehave() == CARET_BEHAVE_MOVE )
		propSheet.m_Page6.m_nCaretBehave    = 0;
	else if( pobjEditorView->GetCaretBehave() == CARET_BEHAVE_STAY )
		propSheet.m_Page6.m_nCaretBehave    = 1;
	propSheet.m_Page6.m_nCaretMoveType      = pobjEditorView->GetCaretMoveType();
	int nMultValueOfCaretAccelarete = m_pApp->GetProfileInt("CEditorView","nMultValueOfCaretAccelarete",3);
	if( nMultValueOfCaretAccelarete!=1 && nMultValueOfCaretAccelarete!=2 &&
		nMultValueOfCaretAccelarete!=3 && nMultValueOfCaretAccelarete!=4 &&
		nMultValueOfCaretAccelarete!=5 )
	{
		nMultValueOfCaretAccelarete = 3;
	}
	propSheet.m_Page6.m_nMultValueOfCaretAccelarete = nMultValueOfCaretAccelarete-1;
	int nSetCaretToEndOnPaste = m_pApp->GetProfileInt("CEditorDoc","bSetCaretToEndOnPaste",1);
	if(nSetCaretToEndOnPaste!=0 && nSetCaretToEndOnPaste!=1)
		nSetCaretToEndOnPaste = 1;
	propSheet.m_Page6.m_nSetCaretToEndOnPaste = nSetCaretToEndOnPaste;
	propSheet.m_Page7.m_bDispLogoBmp        = m_wndClient.GetFlagDispLogoBmp();
	propSheet.m_Page7.m_nLogoBmpKind        = m_wndClient.GetLogoBmpKind();
	propSheet.m_Page7.m_objBmpFilePath      = m_wndClient.GetLogoBmpFilePath();
	propSheet.m_Page7.m_nLogoPosKind        = m_wndClient.GetLogoPosKind();
	propSheet.m_Page7.m_unLogoPosX          = m_wndClient.GetLogoPosX();
	propSheet.m_Page7.m_unLogoPosY          = m_wndClient.GetLogoPosY();
	memcpy(&propSheet.m_Page8.m_stStatusBarDisp,&m_stStatusBarDisp,sizeof(STATUSBARDISP));
	propSheet.m_Page8.m_lStatusDayStyle     = m_lStatusDayStyle;
	propSheet.m_Page8.m_lStatusHourStyle    = m_lStatusHourStyle;
	propSheet.m_Page8.m_lStatusLineStyle    = m_lStatusLineStyle;
	propSheet.m_Page12.m_bIsUseProxy        = m_bIsUseProxyHtml;
	propSheet.m_Page12.m_objProxyIPAddress  = m_objProxyIPAddressHtml;
	propSheet.m_Page12.m_bIsDialup          = m_bIsDialupHtml;
	propSheet.m_Page12.m_objDialupEntry     = m_objDialupEntryHtml;
	propSheet.m_Page12.m_objRasUserName     = m_objRasUserNameHtml;
	propSheet.m_Page12.m_objRasPasswd       = m_objRasPasswdHtml;
	propSheet.m_Page12.m_bIsAutoDisconnect  = m_bIsAutoDisconnectHtml;

	//プロパティシート表示
	int nRet = propSheet.DoModal();
	if(nRet == IDOK)
	{
		if( propSheet.m_Page1.IsChanged() ) //ページ１に変更あり
		{
			m_pApp->SetShowSplash(propSheet.m_Page1.m_bShowSplash);
			if(propSheet.m_Page1.m_lSplashTime>=1 && propSheet.m_Page1.m_lSplashTime<=10)
				m_pApp->WriteProfileInt("CEditorApp","SplashTime",propSheet.m_Page1.m_lSplashTime);
			m_pApp->WriteProfileInt("CEditorApp","nMaxMRU",propSheet.m_Page1.m_nMaxMRU);
			m_pApp->SetMaxMRU(propSheet.m_Page1.m_nMaxMRU);
			m_pApp->WriteProfileInt("CEditorApp","bIsOpenNewFile",propSheet.m_Page1.m_bIsOpenNewFile);
			m_pApp->SetOpenNewFile(propSheet.m_Page1.m_bIsOpenNewFile);
			m_pApp->WriteProfileInt("CEditorApp","IsChdirToOpenFile",propSheet.m_Page1.m_bIsChdirToOpenFile);
			m_pApp->SetChdirToOpenFile(propSheet.m_Page1.m_bIsChdirToOpenFile);
			m_pApp->WriteProfileInt("CChildFrame","bMaximizeWindow",propSheet.m_Page1.m_bMaximizeWindow);
			m_bIsSaveWindowPlace = propSheet.m_Page1.m_bIsSaveWindowPlace;
			m_pApp->WriteProfileInt("CEditorDoc","bIsFullTitle",propSheet.m_Page1.m_bIsFullTitle);
			pobjEditorView->SetFlagDispEOFMark(propSheet.m_Page1.m_bIsDispEOFMark);
			m_pApp->WriteProfileInt("CEditorApp","bIsNoDoubleExecute",propSheet.m_Page1.m_bIsNoDoubleExecute);
			if(propSheet.m_Page1.m_bIsExecPasswd!=FALSE && !propSheet.m_Page1.m_objExecPasswdStr.IsEmpty())
			{
				m_pApp->WriteProfileInt("CEditorApp","bIsExecPasswd",TRUE);
				m_pApp->WriteProfileString("CEditorApp","ExecPasswd",propSheet.m_Page1.m_objExecPasswdStr);
			}
			else
			{
				m_pApp->WriteProfileInt("CEditorApp","bIsExecPasswd",FALSE);
				m_pApp->WriteProfileString("CEditorApp","ExecPasswd","");
			}
	  		m_pApp->WriteProfileInt("CEditorApp","bIsEnableShellOpen",propSheet.m_Page1.m_bIsEnableShellOpen);
			if(propSheet.m_Page1.m_bIsEnableShellOpen)
				m_pApp->RegisterShellExtDll();
			else
				m_pApp->UnRegisterShellExtDll();
			m_pApp->WriteProfileInt("CEditorApp","bIsMakeShortcutDsktop",propSheet.m_Page1.m_bIsMakeShotcutDsktop);
			m_pApp->MakeXerxesShortcut(propSheet.m_Page1.m_bIsMakeShotcutDsktop);
			m_pApp->WriteProfileInt("CEditorView","IsImmAutoOpen",propSheet.m_Page1.m_bIsImmAutoOpen);
		}
		if( propSheet.m_Page2.IsChanged() ) //ページ２に変更あり
		{
			//フォント設定
			m_pApp->SetFontSettings2AllWindow(pobjEditorDoc);

			//ステータスバーフォント設定
			SetStatusBarFont(propSheet.m_Page2.m_stStatusBarLogFont);
			//タブサイズ設定
			if(propSheet.m_Page2.m_nTabSize == 0)//２タブ
				pobjEditorDoc->SetTabSize(2);
			else if(propSheet.m_Page2.m_nTabSize == 1)//４タブ
				pobjEditorDoc->SetTabSize(4);
			else								 //８タブ
				pobjEditorDoc->SetTabSize(8);
		}
		if( propSheet.m_Page3.IsChanged() ) //ページ３に変更あり
		{
			//カラー設定
			m_pApp->SetColorSettings2AllWindow();

			m_wndClient.SetMDIBkgndColor(propSheet.m_Page3.m_MDIBkgndColor);
			pobjEditorView->SetDiffDelLineColor(propSheet.m_Page3.m_DiffDelLineColor);
			pobjEditorView->SetDiffAddLineColor(propSheet.m_Page3.m_DiffAddLineColor);
			pobjEditorView->SetDirDiffLineColor(propSheet.m_Page3.m_DirDiffLineColor);
		}
		if( propSheet.m_Page4.IsChanged() ) //ページ４に変更あり
		{
			//改行コード
			pobjEditorDoc->SetKaigyoKind(propSheet.m_Page4.m_nKaigyoKind);
			//オートセーブ
			m_bIsAutoSave = propSheet.m_Page4.m_bIsAutoSave;
			//セーブ間隔
			if(propSheet.m_Page4.m_lSaveInterval>=1 && propSheet.m_Page4.m_lSaveInterval<=60)
			{
				m_lSaveInterval = propSheet.m_Page4.m_lSaveInterval;
				m_lTimeUpCount = 0;
			}
			//行末空白削除フラグ
			pobjEditorDoc->SetFlagDelEndSpacesOnSave(propSheet.m_Page4.m_bDelEndSpacesOnSave);
			//空白－＞タブ変換フラグ
			pobjEditorDoc->SetFlagSpace2TabOnSave(propSheet.m_Page4.m_bSpace2TabOnSave);
		}
		if( propSheet.m_Page5.IsChanged() ) //ページ５に変更あり
		{
			//行番号印刷フラグ
			pobjEditorView->SetFlagPrintLineNo(propSheet.m_Page5.m_bPrintLineNo);
			//２段組み印刷フラグ
			pobjEditorView->SetFlagPrint2Dangumi(propSheet.m_Page5.m_bPrint2Dangumi);
			//印刷用紙方向
			m_pApp->SetPrintOrientation(propSheet.m_Page5.m_nPrintOrientation);
			//印刷余白
			CRect objMarginRect;
			objMarginRect.left   = propSheet.m_Page5.m_nYohakuHidari;
			objMarginRect.top    = propSheet.m_Page5.m_nYohakuUe;
			objMarginRect.right  = propSheet.m_Page5.m_nYohakuMigi;
			objMarginRect.bottom = propSheet.m_Page5.m_nYohakuSita;
			objMarginRect.NormalizeRect();
			m_pApp->SetPrintMargin(objMarginRect);
			//ヘッダ＆フッタ位置
			pobjEditorView->SetPrintHeaderLayoutPos(propSheet.m_Page5.m_nHeaderLayout);
			pobjEditorView->SetPrintFooterLayoutPos(propSheet.m_Page5.m_nFooterLayout);
			//ヘッダ＆フッタのフォーマット
			pobjEditorView->SetStrHeaderFormat(propSheet.m_Page5.m_objStrHeaderFormat);
			pobjEditorView->SetStrFooterFormat(propSheet.m_Page5.m_objStrFooterFormat);
		}
		if( propSheet.m_Page6.IsChanged() ) //ページ６に変更あり
		{
			//キャレット形状設定
			if(propSheet.m_Page6.m_nCaretType == 0)//縦型キャレット
				pobjEditorView->SetCaretType(CARET_IBEAM);
			else if(propSheet.m_Page6.m_nCaretType == 1)//箱型キャレット
				pobjEditorView->SetCaretType(CARET_SQUARE);
			//キャレット点滅間隔設定
			::SetCaretBlinkTime(propSheet.m_Page6.m_unCaretBlinkTime);
			pobjEditorView->HoldCaretBlinkTime(propSheet.m_Page6.m_unCaretBlinkTime);
			//キャレットの振る舞い設定
			if(propSheet.m_Page6.m_nCaretBehave == 0)//画面スクロール時に行位置変更
				pobjEditorView->SetCaretBehave(CARET_BEHAVE_MOVE);
			else if(propSheet.m_Page6.m_nCaretBehave == 1)//画面スクロール時に行位置保持
				pobjEditorView->SetCaretBehave(CARET_BEHAVE_STAY);
			//キャレット移動タイプ設定
			pobjEditorView->SetCaretMoveType(propSheet.m_Page6.m_nCaretMoveType);
			//キャレット加速スピード設定
			m_pApp->WriteProfileInt("CEditorView","nMultValueOfCaretAccelarete",propSheet.m_Page6.m_nMultValueOfCaretAccelarete+1);
			//貼り付け後のキャレット位置
			m_pApp->WriteProfileInt("CEditorDoc","bSetCaretToEndOnPaste",propSheet.m_Page6.m_nSetCaretToEndOnPaste);
		}
		if( propSheet.m_Page7.IsChanged() ) //ページ７に変更あり
		{
			m_wndClient.SetFlagDispLogoBmp(propSheet.m_Page7.m_bDispLogoBmp);
			m_wndClient.SetLogoBmpKind(propSheet.m_Page7.m_nLogoBmpKind);
	 		m_wndClient.SetLogoBmpFilePath(propSheet.m_Page7.m_objBmpFilePath);
			m_wndClient.SetLogoPosKind(propSheet.m_Page7.m_nLogoPosKind);
			if(propSheet.m_Page7.m_unLogoPosX>=0 && propSheet.m_Page7.m_unLogoPosX<=100)
				m_wndClient.SetLogoPosX(propSheet.m_Page7.m_unLogoPosX);
			if(propSheet.m_Page7.m_unLogoPosY>=0 && propSheet.m_Page7.m_unLogoPosY<=100)
				m_wndClient.SetLogoPosY(propSheet.m_Page7.m_unLogoPosY);
		}
		if( propSheet.m_Page8.IsChanged() ) //ページ８に変更あり
		{
			SetStatusBarDispInfo(&propSheet.m_Page8.m_stStatusBarDisp);
			SetStatusDayStyle(propSheet.m_Page8.m_lStatusDayStyle);
			SetStatusHourStyle(propSheet.m_Page8.m_lStatusHourStyle);
			SetStatusLineStyle(propSheet.m_Page8.m_lStatusLineStyle);
		}
		if( propSheet.m_Page9.IsChanged() ) //ページ９に変更あり
		{
			propSheet.m_Page9.ChangesToRegistry();
		}
		if( propSheet.m_Page11.IsChanged() ) //ページ１１に変更あり
		{
			propSheet.m_Page11.ChangesToRegistry();
		}
		if( propSheet.m_Page12.IsChanged() ) //ページ１２に変更あり
		{
			m_bIsUseProxyHtml       = propSheet.m_Page12.m_bIsUseProxy;
			m_objProxyIPAddressHtml = propSheet.m_Page12.m_objProxyIPAddress;
			m_bIsDialupHtml         = propSheet.m_Page12.m_bIsDialup;
			m_objDialupEntryHtml    = propSheet.m_Page12.m_objDialupEntry;
			m_objRasUserNameHtml    = propSheet.m_Page12.m_objRasUserName;
			m_objRasPasswdHtml      = propSheet.m_Page12.m_objRasPasswd;
			m_bIsAutoDisconnectHtml   = propSheet.m_Page12.m_bIsAutoDisconnect;
		}
	}
}
void CMainFrame::OnUpdateSettings(CCmdUI* pCmdUI) 
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブドキュメント存在チェック
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//印刷・印刷プレビュー中チェック
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メニューハンドラ関数（アイコン付きメニュー関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＭＥＡＳＵＲＥＩＴＥＭ処理										|*/
/*|	解説	：																		|*/
/*|	備考	：This handler ensure that the popup menu items are drawn correctly		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	BOOL setflag=FALSE;

	if(lpMeasureItemStruct->CtlType==ODT_MENU)
	{
		if(IsMenu((HMENU)lpMeasureItemStruct->itemID))
		{
			CMenu* cmenu=CMenu::FromHandle((HMENU)lpMeasureItemStruct->itemID);
			if(m_menu.IsMenu(cmenu)||m_default.IsMenu(cmenu))
			{
				m_menu.MeasureItem(lpMeasureItemStruct);
				setflag=TRUE;
			}
		}
	}
	if(!setflag)
		CMDIFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＭＥＮＵＣＨＡＲ処理											|*/
/*|	解説	：																		|*/
/*|	備考	：This handler ensures that keyboard shortcuts work						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LRESULT CMainFrame::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
{
	LRESULT lresult;
	if(m_menu.IsMenu(pMenu)||m_default.IsMenu(pMenu))
		lresult=BCMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
	else
		lresult=CMDIFrameWnd::OnMenuChar(nChar, nFlags, pMenu);
	return(lresult);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＭＥＮＵＣＨＡＲ処理											|*/
/*|	解説	：																		|*/
/*|	備考	：This handler updates the menus from time to time						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CMDIFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	if(!bSysMenu)
	{
		if(m_menu.IsMenu(pPopupMenu)||m_default.IsMenu(pPopupMenu))
			BCMenu::UpdateMenu(pPopupMenu);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：子ウインドウがあるか？												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsHaveChildWnd()
{
	//アクティブフレーム存在チェック
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
		return FALSE;
	//アクティブビュー存在チェック
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
		return FALSE;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：検索パラメータ設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetFindParameter
	(	CString&	objFindString,
		BOOL		bIsMatchCaseFind,
		BOOL		bIsMatchWholeWordFind,
		BOOL		bIsSearchDownFind,
		BOOL		bIsUseRegularExpression
	)
{
	m_objFindString         = objFindString;
	m_bIsMatchCaseFind      = bIsMatchCaseFind;
	m_bIsMatchWholeWordFind = bIsMatchWholeWordFind;
	m_bIsSearchDownFind     = bIsSearchDownFind;
	m_bIsUseRegularExpressionFind = bIsUseRegularExpression;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：置換パラメータ設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetReplaceParameter
	(	CString&	objFindString,
		CString&	objReplaceString,
		BOOL		bIsMatchCaseReplace,
		BOOL		bIsMatchWholeWordReplace,
		BOOL		bIsUseRegularExpression
	)
{
	m_objFindString					 = objFindString;
	m_objReplaceString				 = objReplaceString;
	m_bIsMatchCaseReplace			 = bIsMatchCaseReplace;
	m_bIsMatchWholeWordReplace		 = bIsMatchWholeWordReplace;
	m_bIsUseRegularExpressionReplace = bIsUseRegularExpression;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＨＴＭＬファイルオープンパラメータ設定処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetHtmlOpenParameter
	(	BOOL		bIsUseProxy,
		CString&	objProxyIPAddress,
		BOOL		bIsDialup,
		CString&	objDialupEntry,
		CString&	objRasUserName,
		CString&	objRasPasswd,
		BOOL		bIsAutoDisconnect
	)
{
	m_bIsUseProxyHtml       = bIsUseProxy;
	m_objProxyIPAddressHtml = objProxyIPAddress;
	m_bIsDialupHtml         = bIsDialup;
	m_objDialupEntryHtml    = objDialupEntry;
	m_objRasUserNameHtml    = objRasUserName;
	m_objRasPasswdHtml      = objRasPasswd;
	m_bIsAutoDisconnectHtml = bIsAutoDisconnect;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスバー表示項目情報設定処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::SetStatusBarDispInfo
	(	LPSTATUSBARDISP	lpStatusBarDisp
	)
{
	BOOL bRet = FALSE;
	int nCnt = 0;
	UINT* punIndicators = (UINT*)malloc(sizeof(UINT)*(1+lpStatusBarDisp->m_nCurrentItemCount));
	int nIndicatorsCount=0;

	punIndicators[0] = ID_SEPARATOR;
	nIndicatorsCount++;
	for(nCnt=0; nCnt<lpStatusBarDisp->m_nCurrentItemCount; nCnt++)
	{
		punIndicators[nIndicatorsCount] = lpStatusBarDisp->m_nCurrentItems[nCnt];
		nIndicatorsCount++;
	}

	bRet = m_wndStatusBar.SetIndicators(punIndicators,nIndicatorsCount);
	m_wndStatusBar.UpdateWindow();
	free(punIndicators);

	if( lpStatusBarDisp != &m_stStatusBarDisp )
		memcpy(&m_stStatusBarDisp,lpStatusBarDisp,sizeof(STATUSBARDISP));

	return bRet;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット位置バイト数を表示するかのチェック処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsDispIndicatorByte()
{
	long lCnt=0;
	for(lCnt=0; lCnt<m_stStatusBarDisp.m_nCurrentItemCount; lCnt++)
	{
		if(m_stStatusBarDisp.m_nCurrentItems[lCnt] == ID_INDICATOR_BYTE)
			return TRUE;
	}
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット位置行、列を表示するかのチェック処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsDispIndicatorGyoretu()
{
	long lCnt=0;
	for(lCnt=0; lCnt<m_stStatusBarDisp.m_nCurrentItemCount; lCnt++)
	{
		if(m_stStatusBarDisp.m_nCurrentItems[lCnt] == ID_INDICATOR_GYORETU)
			return TRUE;
	}
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット位置コードを表示するかのチェック処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsDispIndicatorCode()
{
	long lCnt=0;
	for(lCnt=0; lCnt<m_stStatusBarDisp.m_nCurrentItemCount; lCnt++)
	{
		if(m_stStatusBarDisp.m_nCurrentItems[lCnt] == ID_INDICATOR_CHARCODE)
			return TRUE;
	}
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウ位置保存フラグ設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetSaveWindowPlace
	(	BOOL	bIsSaveWindowPlace
	)
{
	m_bIsSaveWindowPlace = bIsSaveWindowPlace;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスペインメッセージ設定処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetStatusMessage
	(	char*	pszMsg
	)
{
	//ペイン幅を更新してテキストにあわせる
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_SEPARATOR);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(pszMsg,strlen(pszMsg));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//ペインにメッセージ設定
	m_wndStatusBar.SetPaneText(nIndex,pszMsg);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスペインメッセージ設定処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetStatusMessage
	(	CString& objMsg
	)
{
	//ペイン幅を更新してテキストにあわせる
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_SEPARATOR);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent((LPCTSTR)objMsg,objMsg.GetLength());
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//ペインにメッセージ設定
	m_wndStatusBar.SetPaneText(nIndex,(LPCTSTR)objMsg);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスペインへの行、列番号設定処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetGyoRetu
	(	long	lGyo,		//行番号
		long	lRetu,		//列番号
		long	lMaxGyo		//全行数
	)
{
	char	buf[256];
	CString objMsg;

	if( (lGyo<0) || (lRetu<0) || (lMaxGyo<0) )
		return;

	//行、列数保存
	m_lSavedGyo = lGyo;
	m_lSavedRetu = lRetu;
	m_lSavedMaxGyo = lMaxGyo;

	//行、列番号編集
	if(m_lStatusLineStyle==STS_LINESTYLE_LINE_OFFS_ALL)
	{
		objMsg.LoadString(IDS_LINESTYLE_LINE_OFFS_ALL);
		sprintf( buf, (LPCTSTR)objMsg, lGyo, lRetu, lMaxGyo );
	}
	else if(m_lStatusLineStyle==STS_LINESTYLE_LINE_OFFS)
	{
		objMsg.LoadString(IDS_LINESTYLE_LINE_OFFS);
		sprintf( buf, (LPCTSTR)objMsg, lGyo, lRetu );
	}
	else if(m_lStatusLineStyle==STS_LINESTYLE_LINE_ALL)
	{
		objMsg.LoadString(IDS_LINESTYLE_LINE_ALL);
		sprintf( buf, (LPCTSTR)objMsg, lGyo, lMaxGyo );
	}
	else if(m_lStatusLineStyle==STS_LINESTYLE_ALL)
	{
		objMsg.LoadString(IDS_LINESTYLE_ALL);
		sprintf( buf, (LPCTSTR)objMsg, lMaxGyo );
	}
	else if(m_lStatusLineStyle==STS_LINESTYLE_LINE)
	{
		objMsg.LoadString(IDS_LINESTYLE_LINE);
		sprintf( buf, (LPCTSTR)objMsg, lGyo );
	}
	else if(m_lStatusLineStyle==STS_LINESTYLE_OFFS)
	{
		objMsg.LoadString(IDS_LINESTYLE_OFFS);
		sprintf( buf, (LPCTSTR)objMsg, lRetu );
	}

	//ペイン幅を更新してテキストにあわせる
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_GYORETU);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//ペインに行、列番号表示
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスペインの行、列番号クリア処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::ClearGyoRetu()
{
	char	buf[256];

	//ダミー
	strcpy( buf, "    " );

	//ペイン幅を更新してテキストにあわせる
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_GYORETU);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//ペインの行、列番号クリア
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスペインへの挿入、上書きﾓｰﾄﾞ設定処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetInsMode
	(	BOOL	bInsertKey			//Insertキー押下フラグ(TRUE:挿入、FALSE:上書き）
	)
{
	char	buf[256];
	CString objMsg;

	//編集
	if(bInsertKey)
		objMsg.LoadString(IDS_INSMODE_INSERT);
	else
		objMsg.LoadString(IDS_INSMODE_OVERWRITE);
	strcpy(buf,(LPCTSTR)objMsg);

	//ペイン幅を更新してテキストにあわせる
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_INS);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//ペインに挿入、上書きﾓｰﾄﾞ設定
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスペインの挿入、上書きﾓｰﾄﾞｸﾘｱ処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::ClearInsMode()
{
	char	buf[256];

	//ダミー
	strcpy( buf, "    " );

	//ペイン幅を更新してテキストにあわせる
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_INS);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//ペインの挿入、上書きﾓｰﾄﾞｸﾘｱ
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスペインの書込み禁止モード設定処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetReadOnlyMode
	(	BOOL	bIsReadOnly
	)
{
	char	buf[256];
	CString objMsg;

	//ｽﾃｰﾀｽﾊﾞｰﾒｯｾｰｼﾞ
	if( bIsReadOnly )
		objMsg.LoadString(IDS_READONLYMODE_READONLY);
	else
		objMsg.LoadString(IDS_READONLYMODE_WRITEOK);
	strcpy(buf,(LPCTSTR)objMsg);

	//ペイン幅を更新してテキストにあわせる
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_READONLY);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//書込み禁止モード設定
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスペインの書込み禁止ﾓｰﾄﾞｸﾘｱ処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::ClearReadOnlyMode()
{
	char	buf[256];

	//ダミー
	strcpy( buf, "    " );

	//ペイン幅を更新してテキストにあわせる
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_READONLY);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//ペインの挿入、上書きﾓｰﾄﾞｸﾘｱ
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスペインへの日付、時刻設定処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetDateTime()
{
	char	buf[64];
	char*	pPtr=NULL;
	CString	objDayStr=_T("");
	CString objTimeStr=_T("");
	CString objMsg;

	//現在時刻取得
	CTime	objTimeNow = CTime::GetCurrentTime();

	//日付表示フォーマット
	if(m_lStatusDayStyle==STS_DAYSTYLE_NONE)
		objDayStr = "";
	else if(m_lStatusDayStyle==STS_DAYSTYLE_YYYYMMDD)
		objDayStr = objTimeNow.Format( "%Y/%m/%d (%a)" );
	else if(m_lStatusDayStyle==STS_DAYSTYLE_YYMMDD)
		objDayStr = objTimeNow.Format( "'%y/%m/%d (%a)" );
	else if(m_lStatusDayStyle==STS_DAYSTYLE_MMDDYYYY)
		objDayStr = objTimeNow.Format( "%m/%d/%Y (%a)" );
	else if(m_lStatusDayStyle==STS_DAYSTYLE_MMDDYY)
		objDayStr = objTimeNow.Format( "%m/%d/'%y (%a)" );
	else if(m_lStatusDayStyle==STS_DAYSTYLE_MMDD)
		objDayStr = objTimeNow.Format( "%m/%d (%a)" );
	else if(m_lStatusDayStyle==STS_DAYSTYLE_DDMMYYYY)
		objDayStr = objTimeNow.Format( "%d/%m/%Y (%a)" );
	else if(m_lStatusDayStyle==STS_DAYSTYLE_DDMMYY)
		objDayStr = objTimeNow.Format( "%d/%m/'%y (%a)" );
	else if(m_lStatusDayStyle==STS_DAYSTYLE_DDMM)
		objDayStr = objTimeNow.Format( "%d/%m (%a)" );
	else if(m_lStatusDayStyle==STS_DAYSTYLE_YYYY_MM_DD)
	{
		objMsg.LoadString(IDS_DAYSTYLE_YYYY_MM_DD);
		objDayStr = objTimeNow.Format( objMsg );
	}
	else if(m_lStatusDayStyle==STS_DAYSTYLE_YY_MM_DD)
	{
		objMsg.LoadString(IDS_DAYSTYLE_YY_MM_DD);
		objDayStr = objTimeNow.Format( objMsg );
	}
	else if(m_lStatusDayStyle==STS_DAYSTYLE_MM_DD)
	{
		objMsg.LoadString(IDS_DAYSTYLE_MM_DD);
		objDayStr = objTimeNow.Format( objMsg );
	}

	//時間表示フォーマット
	if(m_lStatusHourStyle==STS_HOURSTYLE_NONE)
		objTimeStr = "";
	else if(m_lStatusHourStyle==STS_HOURSTYLE_HHMM)
		objTimeStr = objTimeNow.Format( "%H:%M" );
	else if(m_lStatusHourStyle==STS_HOURSTYLE_AMPM_HHMM)
		objTimeStr = objTimeNow.Format( "%p %I:%M" );
	else if(m_lStatusHourStyle==STS_HOURSTYLE_HH_MM)
	{
		objMsg.LoadString(IDS_HOURSTYLE_HH_MM);
		objTimeStr = objTimeNow.Format( objMsg );
	}
	else if(m_lStatusHourStyle==STS_HOURSTYLE_AMPM_HH_MM)
	{
		objMsg.LoadString(IDS_HOURSTYLE_AMPM_HH_MM);
		objTimeStr = objTimeNow.Format( objMsg );
	}

	if(objDayStr.IsEmpty())
	{
		if(objTimeStr.IsEmpty())
			objDayStr = "    ";
		else
			objDayStr = objTimeStr;
	}
	else
	{
		if(objTimeStr.IsEmpty())
			;
		else 
		{
			objDayStr += " ";
			objDayStr += objTimeStr;
		}
	}

	strcpy(buf,(LPCTSTR)objDayStr);

	//ペイン幅を更新してテキストにあわせる
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_TIME);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//ペインに現在日付時刻設定
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスペインの日付、時刻クリア処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::ClearDateTime()
{
	char	buf[256];

	//ダミー
	strcpy( buf, "    " );

	//ペイン幅を更新してテキストにあわせる
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_TIME);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//ペインの挿入、上書きﾓｰﾄﾞｸﾘｱ
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスペインへのキャレット位置コード設定処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetCaretPosCharCode
	(	UCHAR	uc1stChar,				// キャレット位置コード第一バイト
		UCHAR	uc2ndChar				// キャレット位置コード第二バイト
	)
{
	char	buf[256];

	//キャレット位置コード設定
	if(uc2ndChar == 0)
		sprintf( buf, "CODE = %02XH    ", uc1stChar );
	else 
		sprintf( buf, "CODE = %02XH,%02XH", uc1stChar, uc2ndChar );

	//ペイン幅を更新してテキストにあわせる
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_CHARCODE);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//ペインのキャレット位置コード設定
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスペインへのキャレット位置コードクリア処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::ClearCaretPosCharCode()
{
	char	buf[256];

	//ダミー
	strcpy( buf, "    " );

	//ペイン幅を更新してテキストにあわせる
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_CHARCODE);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//ペインのキャレット位置コードクリア
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスペインへのキャレット位置バイト数設定処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetCaretPosByte
	(	long	lCaretByte,				// キャレット位置バイト数
		long	lSumByte				// 合計バイト数
	)
{
	char	buf[256];

	//バイト数設定
	sprintf( buf, "BYTE=%6d / SUM=%6d", lCaretByte, lSumByte );

	//ペイン幅を更新してテキストにあわせる
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_BYTE);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//ペインのキャレット位置バイト数設定
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスペインへのキャレット位置バイト数クリア処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::ClearCaretPosByte()
{
	char	buf[256];

	//ダミー
	strcpy( buf, "    " );

	//ペイン幅を更新してテキストにあわせる
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_BYTE);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//ペインのキャレット位置バイト数クリア
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスバーフォント設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetStatusBarFont
	(	LOGFONT		stLogFont
	)
{
	m_objStatusBarFont.DeleteObject();
	m_objStatusBarFont.CreateFontIndirect(&stLogFont);
	m_wndStatusBar.SetFont(&m_objStatusBarFont);
	RecalcLayout();

	memcpy(&m_stStatusBarLogFont,&stLogFont,sizeof(LOGFONT));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オートセーブフラグ設定処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetFlagAutoSave
	(	BOOL	bIsAutoSave
	)
{
	m_bIsAutoSave = bIsAutoSave;

	//タイムアップカウンタクリア
	m_lTimeUpCount = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オートセーブ間隔設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetSaveInterval
	(	long	lSaveInterval
	)
{
	if(lSaveInterval>=1 && lSaveInterval<=60)
		m_lSaveInterval = lSaveInterval;

	//タイムアップカウンタクリア
	m_lTimeUpCount = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＭＤＩ背景色設定処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetMDIBkgndColor
	(	COLORREF	color
	)
{
	m_wndClient.SetMDIBkgndColor(color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＭＤＩ　ロゴ表示フラグ設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetFlagDispLogoBmp
	(	BOOL	bDispLogoBmp
	)
{
	m_wndClient.SetFlagDispLogoBmp(bDispLogoBmp);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＭＤＩ　ロゴビットマップ種別設定処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetLogoBmpKind
	(	int		nLogoBmpKind
	)
{
	m_wndClient.SetLogoBmpKind(nLogoBmpKind);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＭＤＩ　ロゴ表示位置種別設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetLogoPosKind
	(	int		nLogoPosKind
	)
{
	m_wndClient.SetLogoPosKind(nLogoPosKind);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＭＤＩ　ロゴビットマップファイルパス設定処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetLogoBmpFilePath
	(	CString	objLogoBmpFilePath
	)
{
	m_wndClient.SetLogoBmpFilePath(objLogoBmpFilePath);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＭＤＩ　ロゴ表示位置Ｘ設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetLogoPosX
	(	UINT	unLogoPosX
	)
{
	m_wndClient.SetLogoPosX(unLogoPosX);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＭＤＩ　ロゴ表示位置Ｙ設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetLogoPosY
	(	UINT	unLogoPosY
	)
{
	m_wndClient.SetLogoPosY(unLogoPosY);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キー操作記録中調査処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsNowOperationRecording()
{
	return m_bIsNowOperationRecording;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キー操作ステップ実行中調査処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsNowStepDoOperation()
{
	return m_bIsNowStepDoOperation;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キー操作記録処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::addOperation
	(	COperation*		pobjOperation
	)
{
	m_objOperationManager.addOperation(pobjOperation);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスバーへ表示する日付表示スタイル取得処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CMainFrame::GetStatusDayStyle()
{
	return m_lStatusDayStyle;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスバーへ表示する日付表示スタイル設定処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetStatusDayStyle
	(	long	lStatusDayStyle
	)
{
	if(m_lStatusDayStyle == lStatusDayStyle)
		return;
	if(lStatusDayStyle < STS_DAYSTYLE_NONE || lStatusDayStyle > STS_DAYSTYLE_MM_DD)
		return;

	m_lStatusDayStyle = lStatusDayStyle;
	SetDateTime();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスバーへ表示する時刻表示スタイル取得処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CMainFrame::GetStatusHourStyle()
{
	return m_lStatusHourStyle;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスバーへ表示する時刻表示スタイル設定処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetStatusHourStyle
	(	long	lStatusHourStyle
	)
{
	if(m_lStatusHourStyle == lStatusHourStyle)
		return;
	if(lStatusHourStyle < STS_HOURSTYLE_NONE || lStatusHourStyle > STS_HOURSTYLE_AMPM_HH_MM)
		return;

	m_lStatusHourStyle = lStatusHourStyle;
	SetDateTime();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ステータスバーへ表示する行、列表示スタイル設定処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetStatusLineStyle
	(	long	lStatusLineStyle
	)
{
	if(m_lStatusLineStyle == lStatusLineStyle)
		return;
	if(lStatusLineStyle < STS_LINESTYLE_LINE_OFFS_ALL || lStatusLineStyle > STS_LINESTYLE_OFFS)
		return;

	m_lStatusLineStyle = lStatusLineStyle;
	SetGyoRetu(m_lSavedGyo,m_lSavedRetu,m_lSavedMaxGyo);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：インクリメンタルサーチ実行中調査処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsNowIncrimentalSearching()
{
	return m_bIsNowIncrimentalSearching;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：インクリメンタルサーチ実行中フラグ設定処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetFlagIncrimentalSearching
	(	BOOL	bIsNowIncrimentalSearching
	)
{
	CString objMsg;

	m_bIsNowIncrimentalSearching = bIsNowIncrimentalSearching;

	if(bIsNowIncrimentalSearching==FALSE)
	{
		//ステータスペインにメッセージ設定
		objMsg.LoadString(AFX_IDS_IDLEMESSAGE);
		SetStatusMessage(objMsg);
		//インクリメンタルサーチ文字列を引き継ぐ
		CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
		if(pobjFrameWnd!=NULL)
		{
			CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
			if(pobjEditorView!=NULL)
				pobjEditorView->GetIncrimentalSearchString(m_objFindString);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame パブリック関数（アイコン付きメニュー関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：メニュー処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
HMENU CMainFrame::NewMenu()
{
	static UINT toolbars[]={
		IDR_MAINFRAME,
		IDR_TOOLBAR_EDIT,
		IDR_TOOLBAR_FIND,
		IDR_TOOLBAR_JMP,
		IDR_TOOLBAR_TOOLS,
		IDR_TOOLBAR_WND
	};

	// Load the menu from the resources
	m_menu.LoadMenu(IDR_EDITORTYPE);  // ****replace IDR_MENUTYPE with your menu ID****

	// Use ModifyODMenu to add a bitmap to a menu options.The first parameter
	// is the menu option text string.If it's NULL, keep the current text
	// string.The second option is the ID of the menu option, or the menu
	// option text (can use this for adding bitmaps to popup options) to change.
	// The third option is the resource ID of the bitmap.This can also be a
	// toolbar ID in which case the class searches the toolbar for the
	// appropriate bitmap.Only Bitmap and Toolbar resources are supported.
//	m_menu.ModifyODMenu(NULL,ID_EDIT_INSDAY,IDB_EDIT_INSDAY);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_INSTIME,IDB_EDIT_INSTIME);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_INSASMCOMMENT,IDB_EDIT_INSASMCOMMENT);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_INSVBCOMMENT,IDB_EDIT_INSVBCOMMENT);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_INSCPPCOMMENT,IDB_EDIT_INSCPPCOMMENT);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_INSTAB,IDB_EDIT_INSTAB);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_DELTAB,IDB_EDIT_DELTAB);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_SETREADONLY,IDB_SETREADONLY);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_UNSETREADONLY,IDB_UNSETREADONLY);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_CLIPVIEW,IDB_EDIT_CLIPVIEW);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_FILECOMPARE,IDB_EDIT_FILECOMPARE);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_DIRCOMPARE,IDB_EDIT_DIRCOMPARE);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_FILETOUCH,IDB_EDIT_FILETOUCH);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_BROWSEURL,IDB_EDIT_BROWSEURL);
//	m_menu.ModifyODMenu(NULL,ID_JUMP_SETMARK,IDB_JUMP_SETMARK);
//	m_menu.ModifyODMenu(NULL,ID_JUMP_DELMARK,IDB_JUMP_DELMARK);
//	m_menu.ModifyODMenu(NULL,ID_JUMP_NEXTMARK,IDB_JUMP_NEXTMARK);
//	m_menu.ModifyODMenu(NULL,ID_JUMP_PREVMARK,IDB_JUMP_PREVMARK);
//	m_menu.ModifyODMenu(NULL,ID_WINDOW_NEW,IDB_WINDOW_NEW);
//	m_menu.ModifyODMenu(NULL,ID_WINDOW_TILE_HORZ,IDB_WINDOW_TILE_HORZ);
//	m_menu.ModifyODMenu(NULL,ID_WINDOW_TILE_VERT,IDB_WINDOW_TILE_VERT);
//	m_menu.ModifyODMenu(NULL,ID_WINDOW_CASCADE,IDB_WINDOW_CASCADE);
//	m_menu.ModifyODMenu(NULL,ID_WINDOW_CLOSE,IDB_WINDOW_CLOSE);
//	m_menu.ModifyODMenu(NULL,ID_WINDOW_ALLCLOSE,IDB_WINDOW_ALLCLOSE);
//	m_menu.ModifyODMenu(NULL,ID_WINDOW_SPLIT,IDB_WINDOW_SPLIT);
//	m_menu.ModifyODMenu(NULL,ID_WINDOW_ARRANGE,IDB_WINDOW_ARRANGE);
//	m_menu.ModifyODMenu(NULL,ID_WND_NEXT,IDB_WND_NEXT);
//	m_menu.ModifyODMenu(NULL,ID_WND_PREV,IDB_WND_PREV);
	m_menu.ModifyODMenu(NULL,ID_VIEW_REDRAW,IDB_VIEW_REDRAW);
//	m_menu.ModifyODMenu(NULL,ID_SECURITY_ENCRIPT,IDB_SECURITY_ENCRIPT);
//	m_menu.ModifyODMenu(NULL,ID_SECURITY_DECRIPT,IDB_SECURITY_DECRIPT);

	// Another method for adding bitmaps to menu options is through the
	// LoadToolbars member function.This method allows you to add all
	// the bitmaps in a toolbar object to menu options (if they exist).
	// The first function parameter is an array of toolbar id's.
	// The second is the number of toolbar id's. There is also a
	// function called LoadToolbar that just takes an id.
	m_menu.LoadToolbars(toolbars,6);

	return(m_menu.Detach());
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：アイコン付きメニュー処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
HMENU CMainFrame::NewDefaultMenu()
{
	m_default.LoadMenu(IDR_MAINFRAME);
	m_default.LoadToolbar(IDR_MAINFRAME);
	return(m_default.Detach());
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame プロテクト関数（ツールバー関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ツールバー作成処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::CreateToolBar()
{
	DWORD	dwStyle = WS_CHILD | WS_VISIBLE | CBRS_SIZE_DYNAMIC | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY;

	//標準のツールバー作成
	if( !m_wndToolBar.Create(this,dwStyle,ID_VIEW_TOOLBAR,m_wndToolBar_Info,
							 "Software\\S.Ekura\\Xerxes\\CustomizingToolbar","CustomizeInfo") || 
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
		return FALSE;
	//編集関連ツールバー作成
	if( !m_wndToolBarEdit.Create(this,dwStyle,IDW_TOOLBAR_EDIT,m_wndToolBarEdit_Info,
							 "Software\\S.Ekura\\Xerxes\\CustomizingToolbarEdit","CustomizeInfo") || 
		!m_wndToolBarEdit.LoadToolBar(IDR_TOOLBAR_EDIT))
		return FALSE;
	//検索関連ツールバー作成
	if( !m_wndToolBarFind.Create(this,dwStyle,IDW_TOOLBAR_FIND,m_wndToolBarFind_Info,
							 "Software\\S.Ekura\\Xerxes\\CustomizingToolbarFind","CustomizeInfo") || 
		!m_wndToolBarFind.LoadToolBar(IDR_TOOLBAR_FIND))
		return FALSE;
	//ジャンプ関連ツールバー作成
	if( !m_wndToolBarJmp.Create(this,dwStyle,IDW_TOOLBAR_JMP,m_wndToolBarJmp_Info,
							 "Software\\S.Ekura\\Xerxes\\CustomizingToolbarJmp","CustomizeInfo") || 
		!m_wndToolBarJmp.LoadToolBar(IDR_TOOLBAR_JMP))
		return FALSE;
	//ツール関連ツールバー作成
	if( !m_wndToolBarTools.Create(this,dwStyle,IDW_TOOLBAR_TOOLS,m_wndToolBarTools_Info,
							 "Software\\S.Ekura\\Xerxes\\CustomizingToolbarTools","CustomizeInfo") || 
		!m_wndToolBarTools.LoadToolBar(IDR_TOOLBAR_TOOLS))
		return FALSE;
	//ウインドウ関連ツールバー作成
	if( !m_wndToolBarWnd.Create(this,dwStyle,IDW_TOOLBAR_WND,m_wndToolBarWnd_Info,
							 "Software\\S.Ekura\\Xerxes\\CustomizingToolbarWnd","CustomizeInfo") || 
		!m_wndToolBarWnd.LoadToolBar(IDR_TOOLBAR_WND))
		return FALSE;


	//ツールバードッキング可能化
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarEdit.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarFind.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarJmp.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarTools.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarWnd.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);

	//ツールバー位置決め
	DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_TOP);
	DockControlBarLeftOf(&m_wndToolBarEdit,&m_wndToolBar);
	DockControlBar(&m_wndToolBarFind);
	DockControlBarLeftOf(&m_wndToolBarJmp,&m_wndToolBarFind);
	DockControlBarLeftOf(&m_wndToolBarTools,&m_wndToolBarJmp);
	DockControlBarLeftOf(&m_wndToolBarWnd,&m_wndToolBarTools);

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ツールバー配置処理													|*/
/*|	解説	：																		|*/
/*|	備考	：LeftOfのツールバーの右隣にBarを配置する								|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf)
{
	CRect RC;

	RecalcLayout();
	LeftOf->GetWindowRect(&RC);
	//右へ１オフセット分移動
	RC.OffsetRect(1,0);
	DWORD  dw=LeftOf->GetBarStyle();
	UINT n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;
	DockControlBar(Bar,n,&RC);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame プロテクト関数（ウインドウ位置関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウ位置ロード処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CMainFrame","WindowPos");
	if(strBuffer.IsEmpty())
		return FALSE;

	int cRead = _stscanf(strBuffer,"%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
					&pwp->flags,
					&pwp->showCmd,
					&pwp->ptMinPosition.x, &pwp->ptMinPosition.y,
					&pwp->ptMaxPosition.x, &pwp->ptMaxPosition.y,
					&pwp->rcNormalPosition.left,  &pwp->rcNormalPosition.top,
					&pwp->rcNormalPosition.right, &pwp->rcNormalPosition.bottom);
	if(cRead != 10)
		return FALSE;
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウ位置セーブ処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	//ウインドウの位置を保存する
	CString	strBuffer;
	strBuffer.Format("%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
					pwp->flags,
					pwp->showCmd,
					pwp->ptMinPosition.x, pwp->ptMinPosition.y,
					pwp->ptMaxPosition.x, pwp->ptMaxPosition.y,
					pwp->rcNormalPosition.left,  pwp->rcNormalPosition.top,
					pwp->rcNormalPosition.right, pwp->rcNormalPosition.bottom);
	m_pApp->WriteProfileString("CMainFrame","WindowPos",strBuffer);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame プロテクト関数（ＨＴＭＬオープン関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＨＴＭＬオープン状況ダイアログ表示待ち処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForHtmlOpenDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjHtmlOpenStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjHtmlOpenStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjHtmlOpenStatusDlg->m_hWnd);
			if(bRet!=0)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame プロテクト関数（ＧＲＥＰ関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＧＲＥＰ実行処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoGrep()
{
	CString objMsg;

	//ＧＲＥＰ実行ディレクトリ存在チェック
	CFileFind		objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolderGrep);
	if(!bRet)//ない！！
	{
		if(m_pApp->IsDriveRoot(m_objFolderGrep) == FALSE) //ルートディレクトリでもない
		{
			objMsg.LoadString(IDS_NOGREPFOLDER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//ＧＲＥＰ結果出力用ＭＤＩ子ウインドウ作成
	m_pobjGrepDoc = NULL;
	m_pobjGrepFrameWnd = NULL;
	bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjGrepDoc,&m_pobjGrepFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjGrepDoc->SetFontOfCurrentFileExt("");
	m_pobjGrepDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);
	CString	objTitle;
	objMsg.LoadString(IDS_GERP_TITLE);
	objTitle.Format(objMsg,m_objGrepStr);
	m_pobjGrepDoc->SetTitle(objTitle);
	m_pobjGrepDoc->SetGrepMode(TRUE);

	//ＧＲＥＰ状況モードレスダイアログ表示
	m_pobjGrepStatusDlg = new CGrepStatusDlg();
	m_pobjGrepStatusDlg->Create(IDD_GREPSTATUS);
	m_pobjGrepStatusDlg->SetGrepStr(m_objGrepStr);
	m_pobjGrepStatusDlg->SetFileKind(m_objFileKindGrep);
	m_pobjGrepStatusDlg->SetFolder(m_objFolderGrep);
	WaitForGrepDlgDisp();

	//１ディレクトリ内ＧＲＥＰ実行
	bRet = DoGrepInOneDirectory(m_objFolderGrep,m_objFileKindGrep,m_objGrepStr);

	//該当件数タイトル追加表示
	long lMatchCount = m_pobjGrepStatusDlg->GetMatchCount();
	char	szBuf[32];
	objMsg.LoadString(IDS_GREP_MATCHCOUNT);
	sprintf(szBuf, (LPCTSTR)objMsg, lMatchCount );
	CString objStr = szBuf;
	objTitle = m_pobjGrepDoc->GetTitle();
	objTitle += objStr;
	m_pobjGrepDoc->SetTitle(objTitle);

	//ＧＲＥＰ状況モードレスダイアログ消去
	m_pobjGrepStatusDlg->DestroyWindow();
	delete m_pobjGrepStatusDlg;
	m_pobjGrepStatusDlg = NULL;

	m_pobjGrepDoc->UpdateAllViews(NULL,NULL,NULL);

	//ＧＲＥＰ終了通知
	if( bRet != FALSE )
	{
		objMsg.LoadString(IDS_GREPEND);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
	else
	{
		objMsg.LoadString(IDS_GREPCANCEL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１ディレクトリ内ＧＲＥＰ実行処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoGrepInOneDirectory		// TRUE : 正常終了、FALSE : キャンセル終了
	(	CString		objFolder,				// ＧＲＥＰを行うフォルダ
		CString		objFileKinds,			// ＧＲＥＰ対象ファイル種別
		CString		objGrepStr				// ＧＲＥＰ文字列
	)
{
	BOOL		bRetStatus = TRUE;
	BOOL		bRet, bRetTmp;
	CString		objFilter;
	CString		objOneKind, objRight, objBuf;
	CFileFind	objFileFind;

	//ＧＲＥＰ状況ダイアログへフォルダ通知
	if( !IsGrepCanceled() )
		m_pobjGrepStatusDlg->SetFolder(objFolder);

	objRight = objFileKinds;
	for(;;)
	{
		//ウインドウズメッセージ処理
		m_pApp->MessagePump();

		//キャンセルチェック
		if( IsGrepCanceled() )
			return FALSE;

		//ＧＲＥＰ対象ファイル種別切り取り
		int nIndex = objRight.Find(';');
		if(nIndex != -1)
		{
			objOneKind = objRight.Left(nIndex);
			objBuf = objRight.Right( objRight.GetLength() - nIndex - 1 );
			objRight = objBuf;
		}
		else
		{
			objOneKind = objRight;
		}

		//ファイル検索キー作成
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s%s",objFolder,objOneKind);
		else
			objFilter.Format("%s\\%s",objFolder,objOneKind);

		//ＧＲＥＰ対象ファイル検索
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//ウインドウズメッセージ処理
				m_pApp->MessagePump();

				//キャンセルチェック
				if( IsGrepCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
					continue;

				//１ファイル内ＧＲＥＰ実行
				bRetTmp = DoGrepInOneFile(objFileFind.GetFileName(),objFileFind.GetFilePath(),objGrepStr);
				if( bRetTmp == FALSE ) //キャンセル終了してきた
				{
					objFileFind.Close();
					return FALSE;
				}
			}
			//最後の１個
			if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
			{
				bRetTmp = DoGrepInOneFile(objFileFind.GetFileName(),objFileFind.GetFilePath(),objGrepStr);
				if( bRetTmp == FALSE ) //キャンセル終了してきた
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();

		if( nIndex == -1 ) // ファイル種別残りなし
		{
			break;
		}
	}

	//ウインドウズメッセージ処理
	m_pApp->MessagePump();
	//キャンセルチェック
	if( IsGrepCanceled() )
		return FALSE;

	if( m_bIsSearchSubFolderGrep ) //サブフォルダも検索する場合
	{
		//ファイル検索キー作成（ワイルドカード検索）
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s*.*",objFolder);
		else
			objFilter.Format("%s\\*.*",objFolder);

		//サブディレクトリ検索
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//ウインドウズメッセージ処理
				m_pApp->MessagePump();

				//キャンセルチェック
				if( IsGrepCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}
				
				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
				{
					//１ディレクトリ内ＧＲＥＰ実行（自分を再帰呼び出し）
					bRetTmp = DoGrepInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objGrepStr);
					if(bRetTmp == FALSE) //キャンセル終了してきた
					{
						objFileFind.Close();
						return FALSE;
					}
				}
			}
			//最後の１個
			if( (objFileFind.IsDots()==FALSE) && (objFileFind.IsDirectory()!=FALSE) )
			{
				//１ディレクトリ内ＧＲＥＰ実行（自分を再帰呼び出し）
				bRetTmp = DoGrepInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objGrepStr);
				if(bRetTmp == FALSE) //キャンセル終了してきた
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１ファイル内ＧＲＥＰ実行処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoGrepInOneFile			// TRUE : 正常終了、FALSE : キャンセル終了
	(	CString		objFileName,			// ＧＲＥＰを行うファイル
		CString		objFilePath,			// ＧＲＥＰを行うファイルパス
		CString		objGrepStr				// ＧＲＥＰ文字列
	)
{
	char*   pPtr=NULL;
	int		eflags=0, start=0, end=0;
	long	lMatchLength=0;
	long	lCRCount=0;
	long	lLFCount=0;
	long	lOffset=0;
	BOOL	bRetStatus = TRUE;
	CFile	objFile;
	UINT	unCodeKind=0;
	CCodeConvManager	objCodeConvManager;

	//ファイルの文字コードをチェック
	unCodeKind = objCodeConvManager.DetectCode((LPCTSTR)objFilePath);
	if(unCodeKind == INPUT_UNKNOWN || unCodeKind == INPUT_BINARY)
		return TRUE;

	//ＧＲＥＰ状況ダイアログへフォルダ通知
	if( !IsGrepCanceled() )
		m_pobjGrepStatusDlg->SetFile(objFileName);

	//ファイルオープン
	if( !objFile.Open( objFilePath, CFile::modeRead, NULL ) )
		return bRetStatus;
	CArchive ar( &objFile, CArchive::load );

	//検索対象文字列が全て英数字から成るかチェック
	BOOL bAllAlphaNum = TRUE;
	for(long lCnt=0; lCnt<objGrepStr.GetLength(); lCnt++)
	{
		if( IsDBCSLeadByteEx(0,objGrepStr.GetAt(lCnt)) )
		{
			bAllAlphaNum = FALSE;
			break;
		}
		else if( (!isAlnum(objGrepStr.GetAt(lCnt))) &&
				 (objGrepStr.GetAt(lCnt) != '_')
		)
		{
			bAllAlphaNum = FALSE;
			break;
		}
	}

	m_pApp->PrepareMyReadString();
	for(long lLineCnt=0; ; lLineCnt++)
	{
		//ウインドウズメッセージ処理
		m_pApp->MessagePump();
		//キャンセルチェック
		if( IsGrepCanceled() )
		{
			bRetStatus = FALSE;
			break;
		}

		//１行づつ読み込み
		CString objStr=_T("");
		if( m_pApp->MyReadString(ar,objStr,&lCRCount,&lLFCount) == 0 )
		{
			if( objStr.IsEmpty() )
				break;
		}

		//文字列検索
		CLine	objLine((const unsigned char*)(LPCTSTR)objStr);
		if(m_bIsUseRegularExpressionGrep) //正規表現を使う場合
		{
			start = end = 0;
			if(m_pApp->m_lpfnRegFunc2!=NULL)
			{
				//myRegexec
				if(m_pApp->m_lpfnRegFunc2((const char*)objLine.String(),eflags,&start,&end)==0)//match!
					lOffset = start;
				else
					lOffset = -1;
			}
			else
				lOffset=-1;
		}
		else
		{
			lOffset = objLine.DoFind( 0,
									  objLine.GetLength(),
									  objGrepStr,
									  bAllAlphaNum,
									  m_bIsMatchCaseGrep,
									  m_bIsMatchWholeWordGrep,
									  FALSE,
									  &lMatchLength);
		}

		if(lOffset != -1) //発見！！
		{
			CLine	objGrepResult;

			//発見ファイルパス
			objGrepResult.Set((const unsigned char*)(LPCTSTR)objFilePath);

			//発見行番号
			char	szLine[32];
			sprintf(szLine,"(%d) : ",lLineCnt+1);
			objGrepResult.Catenate((const unsigned char*)szLine);

			//発見行の内容
			objGrepResult.Catenate(objLine.String());

			//ＧＲＥＰ結果をドキュメントへ登録
			if( m_pobjGrepDoc != NULL )
				m_pobjGrepDoc->AppendGrepLine(objGrepResult);

			//該当件数カウントアップ
			if( !IsGrepCanceled() )
				m_pobjGrepStatusDlg->IncMatchCount();
		}
	}

	//ファイルクローズ
	ar.Close();
	objFile.Close();

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＧＲＥＰ状況ダイアログ表示待ち処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForGrepDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjGrepStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjGrepStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjGrepStatusDlg->m_hWnd);
			if(bRet!=0)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＧＲＥＰキャンセルチェック処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsGrepCanceled()	//TRUE : キャンセル、FALSE : 未キャンセル
{
	if( m_pobjGrepStatusDlg->IsCanceled() )
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame プロテクト関数（文字コード変換関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字コード変換実行処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoCodeConv()
{
	CString objMsg;
	CLine	objConvResult;

	//文字コード変換実行ディレクトリ存在チェック
	CFileFind		objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolderConv);
	if(!bRet)//ない！！
	{
		if(m_pApp->IsDriveRoot(m_objFolderConv) == FALSE) //ルートディレクトリでもない
		{
			objMsg.LoadString(IDS_NOCONVFOLDER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//文字コード変換結果出力用ＭＤＩ子ウインドウ作成
	m_pobjConvDoc = NULL;
	m_pobjConvFrameWnd = NULL;
	bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjConvDoc,&m_pobjConvFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjConvDoc->SetFontOfCurrentFileExt("");
	m_pobjConvDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//書込み禁止ﾓｰﾄﾞｽﾃｰﾀｽﾍﾟｲﾝ設定
	CString	objTitle;
	objMsg.LoadString(IDS_CONV_TITLE2);
	m_pobjConvDoc->SetTitle(objTitle);
	m_pobjConvDoc->SetConvMode(TRUE);

	//文字コード変換状況モードレスダイアログ表示
	m_pobjConvStatusDlg = new CCodeConvStatusDlg();
	m_pobjConvStatusDlg->Create(IDD_CONVSTATUS);
	m_pobjConvStatusDlg->SetConvKindStr(m_nConvKind);
	m_pobjConvStatusDlg->SetFileKind(m_objFileKindConv);
	m_pobjConvStatusDlg->SetFolder(m_objFolderConv);
	WaitForConvDlgDisp();

	//タイトル追加
	objMsg.LoadString(IDS_CONV_TITLE2);
	objConvResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjConvDoc->AppendConvLine(objConvResult);

	//１ディレクトリ内文字コード変換実行
	bRet = DoConvInOneDirectory(m_objFolderConv,m_objFileKindConv,m_nConvKind,m_nInCodeConv);

	//コード変換ファイル数タイトル追加表示
	long lConvFileCount = m_pobjConvStatusDlg->GetConvFileCount();
	char	szBuf[64];
	objMsg.LoadString(IDS_CONV_FILECOUNT);
	sprintf(szBuf, (LPCTSTR)objMsg, lConvFileCount );
	CString objStr = szBuf;
	objTitle = m_pobjConvDoc->GetTitle();
	objTitle += objStr;
	m_pobjConvDoc->SetTitle(objTitle);

	//注記出力
	objConvResult.Set((const unsigned char*)"");
	m_pobjConvDoc->AppendConvLine(objConvResult);
	objConvResult.Set((const unsigned char*)"");
	m_pobjConvDoc->AppendConvLine(objConvResult);
	objMsg.LoadString(IDS_CONV_MEMO);
	objConvResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjConvDoc->AppendConvLine(objConvResult);
	objConvResult.Set((const unsigned char*)"");
	m_pobjConvDoc->AppendConvLine(objConvResult);

	//文字コード変換状況モードレスダイアログ消去
	m_pobjConvStatusDlg->DestroyWindow();
	delete m_pobjConvStatusDlg;
	m_pobjConvStatusDlg = NULL;

	m_pobjConvDoc->UpdateAllViews(NULL,NULL,NULL);

	//文字コード変換終了通知
	if( bRet != FALSE )
	{
		if(lConvFileCount>0)
		{
			objMsg.LoadString(IDS_CONVEND);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
		else
		{
			objMsg.LoadString(IDS_CONVZERO);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
	}
	else
	{
		objMsg.LoadString(IDS_CONVCANCEL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１ディレクトリ内文字コード変換実行処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoConvInOneDirectory		// TRUE : 正常終了、FALSE : キャンセル終了
	(	CString		objFolder,				// 文字コード変換を行うフォルダ
		CString		objFileKinds,			// 文字コード変換対象ファイル種別
		int			nConvKind,				// 文字コード変換種別
		int			nInCode					// 入力ファイルの文字コード判定種別
	)
{
	char		szBuf[512];
	BOOL		bCanceled=FALSE;
	BOOL		bRetStatus = TRUE;
	BOOL		bRet, bRetTmp;
	CString		objFilter;
	CString		objOneKind, objRight, objBuf;
	CFileFind	objFileFind;
	CString		objMsg;

	//ディレクトリ名を追加
	CLine	objConvResult;
	m_pobjConvDoc->AppendConvLine(objConvResult);
	objConvResult.Set((const unsigned char*)"");
	m_pobjConvDoc->AppendConvLine(objConvResult);
	objMsg.LoadString(IDS_CONV_FOLDER_TITLE);
	sprintf(szBuf, (LPCTSTR)objMsg, (LPCTSTR)objFolder);
	objConvResult.Set((const unsigned char*)szBuf);
	m_pobjConvDoc->AppendConvLine(objConvResult);
	objMsg.LoadString(IDS_CONV_LIST_TITLE);
	objConvResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjConvDoc->AppendConvLine(objConvResult);
	objConvResult.Set((const unsigned char*)"-------------------------------------------------------------------------------------------------");
	m_pobjConvDoc->AppendConvLine(objConvResult);


	//文字コード変換状況ダイアログへフォルダ通知
	if( !IsConvCanceled() )
		m_pobjConvStatusDlg->SetFolder(objFolder);

	//////////////////////////////////////////////////
	//	まず、該当するファイルの一覧を作成する		//
	//////////////////////////////////////////////////

	//ファイル名リスト
	CObList		objFileList;

	objRight = objFileKinds;
	for(;;)
	{
		//文字コード変換対象ファイル種別切り取り
		int nIndex = objRight.Find(';');
		if(nIndex != -1)
		{
			objOneKind = objRight.Left(nIndex);
			objBuf = objRight.Right( objRight.GetLength() - nIndex - 1 );
			objRight = objBuf;
		}
		else
		{
			objOneKind = objRight;
		}

		//ファイル検索キー作成
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s%s",objFolder,objOneKind);
		else
			objFilter.Format("%s\\%s",objFolder,objOneKind);

		//文字コード変換対象ファイル検索
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
					continue;

				//発見したファイルパス取得
				CString*	pobjFileStr = new CString;
				(*pobjFileStr) = objFileFind.GetFileName();
				//リストへ登録
				objFileList.AddTail((CObject*)pobjFileStr);
			}
			if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
			{
				//最後の１個
				CString*	pobjFileStr = new CString;
				(*pobjFileStr) = objFileFind.GetFileName();
				objFileList.AddTail((CObject*)pobjFileStr);
			}
		}
		objFileFind.Close();

		if( nIndex == -1 ) // ファイル種別残りなし
		{
			break;
		}
	}

	//////////////////////////////////////////////////////
	//		ファイルリストをたどりながらコード変換		//
	//////////////////////////////////////////////////////

	bCanceled=FALSE;
	if(!objFileList.IsEmpty()) //発見ファイルあり
	{
		POSITION	pos = objFileList.GetHeadPosition();
		for( ; pos != 0; ) //リスト順次アクセス
		{
			//ウインドウズメッセージ処理
			m_pApp->MessagePump();
			//キャンセルチェック
			if( IsConvCanceled() )
				break;

			//ファイル名取得
			CString* pobjFileName = (CString*)objFileList.GetNext(pos);
			if(pobjFileName!=NULL)
			{
				//ファイルパス作成
				CString objFilePath = objFolder + "\\" + (*pobjFileName);
				//１ファイル文字コード変換実行
				bRetTmp = DoConvInOneFile(objFolder,(*pobjFileName),objFilePath,nConvKind,nInCode);
				if( bRetTmp == FALSE ) //キャンセル終了してきた
				{
					bCanceled=TRUE;
					break;
				}
				else //文字コード変換ファイル数カウントアップ
				{
					m_pobjConvStatusDlg->IncConvFileCount();
				}
			}
		}
	}

	//ファイルリスト全削除
	if(!objFileList.IsEmpty())
	{
		POSITION pos;
		for(pos=objFileList.GetHeadPosition(); pos!=0; )
		{
			CString* pobjStr = (CString*)objFileList.GetNext(pos);
			if(pobjStr!=NULL)
				delete pobjStr;
		}
		objFileList.RemoveAll();
	}

	//ウインドウズメッセージ処理
	m_pApp->MessagePump();
	//キャンセルチェック
	if(bCanceled)
		return FALSE;
	if( IsConvCanceled() )
		return FALSE;

	if( m_bIsSearchSubFolderConv ) //サブフォルダも検索する場合
	{
		//ファイル検索キー作成（ワイルドカード検索）
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s*.*",objFolder);
		else
			objFilter.Format("%s\\*.*",objFolder);

		//サブディレクトリ検索
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//ウインドウズメッセージ処理
				m_pApp->MessagePump();
				//キャンセルチェック
				if( IsConvCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
				{
					//１ディレクトリ内文字コード変換実行（自分を再帰呼び出し）
					bRetTmp = DoConvInOneDirectory(objFileFind.GetFilePath(),objFileKinds,nConvKind,nInCode);
					if(bRetTmp == FALSE) //キャンセル終了してきた
					{
						objFileFind.Close();
						return FALSE;
					}
				}
			}
			//最後の１個
			if( (objFileFind.IsDots()==FALSE) && (objFileFind.IsDirectory()!=FALSE) )
			{
				//１ディレクトリ内文字コード変換実行（自分を再帰呼び出し）
				bRetTmp = DoConvInOneDirectory(objFileFind.GetFilePath(),objFileKinds,nConvKind,nInCode);
				if(bRetTmp == FALSE) //キャンセル終了してきた
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１ファイル文字コード変換実行処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoConvInOneFile			// TRUE : 正常終了、FALSE : キャンセル終了
	(	CString		objFolder,				// 文字コード変換を行うフォルダ
		CString		objInFileName,			// 文字コード変換を行うファイル
		CString		objInFilePath,			// 文字コード変換を行うファイルパス
		int			nConvKind,				// 文字コード変換種別
		int			nInCode					// 入力ファイルの文字コード判定種別
	)
{
	CLine	objConvResult;
	char	szResultMsg[256];
	char	szBuf[512];
	char*	pPtr=NULL;
	BOOL	bRet=TRUE;
	BOOL	bRetStatus=TRUE;
	CFile	objInFile, objOutFile;
	DWORD	dwRet=0;
	INT		nRet=0;
	UINT	unRet=0;
	char	szTempFileName[_MAX_FNAME],szTempPathName[_MAX_PATH+16];
	char	szInFilePath[_MAX_PATH+16],szInFileName[_MAX_PATH],szOutFileName[_MAX_PATH];
	CCodeConvManager	objCodeConvManager;
	CString objMsg;
	UINT nDetectCode=0;

	//////////////////////////////////
	//		入出力ファイル準備		//
	//////////////////////////////////

	//文字コード変換状況ダイアログへファイル通知
	if( !IsConvCanceled() )
		m_pobjConvStatusDlg->SetFile(objInFileName);

	//入力ファイル名取得
	strcpy(szInFilePath,(LPCTSTR)objInFilePath);
	strcpy(szInFileName,(LPCTSTR)objInFileName);

	//出力用一時ファイル名取得
	strcpy(szTempPathName,(LPCTSTR)objFolder);
	unRet = ::GetTempFileName( szTempPathName,"Cnv",0,szTempFileName);
	if(unRet==0) //Error
	{
		objMsg.LoadString(IDS_CANTGET_CONVTMPFNAME);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	else
	{
		long lPos=0;
		long lCnt=0;
		for(lCnt=0; lCnt<(signed)strlen(szTempFileName); )
		{
			if(IsDBCSLeadByteEx(0,szTempFileName[lCnt]))
			{
				lCnt+=2;
				continue;
			}
			else
			{
				if(szTempFileName[lCnt]=='\\')
				{
					//'\'位置保持
					lPos = lCnt;
				}
				lCnt++;
			}
		}
		strcpy(szOutFileName,&szTempFileName[lPos+1]);
	}


	//入力ファイルオープン
	if( !objInFile.Open( objInFilePath, CFile::modeRead, NULL ) )
	{
		//出力用ファイル削除
		if( (_access( szTempFileName, 0 )) != -1 )
		{
			::DeleteFile(szTempFileName);
		}
		return FALSE;
	}
	else
	{
		objInFile.Close(); //すぐクローズ
	}

	//出力ファイルオープン
	if( !objOutFile.Open( szTempFileName, (CFile::modeCreate|CFile::modeWrite), NULL ) )
	{
		objMsg.LoadString(IDS_CANTMAKE_CONVTMPFILE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		objInFile.Close();
		return FALSE;
	}
	else
	{
		objOutFile.Close(); //すぐクローズ
	}

	//////////////////////////////////////
	//		入力ファイルチェック		//
	//////////////////////////////////////

	//入力ファイル文字コードの判別
	if(nInCode == INCODE_AUTO)
		nDetectCode = objCodeConvManager.DetectCode(szInFilePath);
	else if(nInCode == INCODE_EUC)
		nDetectCode = INPUT_EUC;
	else if(nInCode == INCODE_SJIS)
		nDetectCode = INPUT_SJIS;
	else if(nInCode == INCODE_JIS)
		nDetectCode = INPUT_JIS;
	else
		nDetectCode = objCodeConvManager.DetectCode(szInFilePath);
	
	
	if( nDetectCode == INPUT_BINARY ||		//入力ファイルがバイナリファイル
		nDetectCode == INPUT_ASCII  ||		//入力ファイルに漢字コードなし
		nDetectCode == INPUT_UNKNOWN		//文字コード不明
	)
	{
		//変換結果
		if( nDetectCode==INPUT_BINARY )
			objMsg.LoadString(IDS_CONV_MSG_BINARY);
		else if( nDetectCode==INPUT_ASCII )
			objMsg.LoadString(IDS_CONV_MSG_ASCII);
		else if( nDetectCode==INPUT_UNKNOWN )
			objMsg.LoadString(IDS_CONV_MSG_UNKNOWN);
		strcpy(szResultMsg, (LPCTSTR)objMsg);

		if(strlen(szInFileName)>30)
			szInFileName[30]=0;
		sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
		objConvResult.Set((const unsigned char*)szBuf);

		//結果をドキュメントへ登録
		m_pobjConvDoc->AppendConvLine(objConvResult);

		//出力用ファイル削除
		if( (_access( szTempFileName, 0 )) != -1 )
		{
			::DeleteFile(szTempFileName);
		}

		//コード変換終了
		return TRUE;
	}
	else
	{
		if( (nDetectCode==INPUT_SJIS && nConvKind==CODECONV_TO_SJIS) ||	//すでに変換済み
			(nDetectCode==INPUT_JIS  && nConvKind==CODECONV_TO_JIS)  ||
			(nDetectCode==INPUT_EUC  && nConvKind==CODECONV_TO_EUC)
		)
		{
			//変換結果
			if( nDetectCode==INPUT_SJIS )
				objMsg.LoadString(IDS_CONV_MSG_SJISEXIST);
			else if( nDetectCode==INPUT_JIS )
				objMsg.LoadString(IDS_CONV_MSG_JISEXIST);
			else if( nDetectCode==INPUT_EUC )
				objMsg.LoadString(IDS_CONV_MSG_EUCEXIST);
			strcpy(szResultMsg, (LPCTSTR)objMsg );

			if(strlen(szInFileName)>30)
				szInFileName[30]=0;
			sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
			objConvResult.Set((const unsigned char*)szBuf);

			//結果をドキュメントへ登録
			m_pobjConvDoc->AppendConvLine(objConvResult);

			//出力用ファイル削除
			if( (_access( szTempFileName, 0 )) != -1 )
			{
				::DeleteFile(szTempFileName);
			}

			//コード変換終了
			return TRUE;
		}
	}


	//////////////////////////////////
	//		文字コード変換実行		//
	//////////////////////////////////
	if( nDetectCode==INPUT_SJIS ) //入力ファイルＳＪＩＳ
	{
		if( nConvKind==CODECONV_TO_EUC ) //SJIS -> EUC
		{
			nRet = objCodeConvManager.ConvertSjisToEuc(szInFilePath,szTempFileName);
			if(nRet==1)
				objMsg.LoadString(IDS_CONV_SJIS2EUC);
			else if(nRet==-1)
				objMsg.LoadString(IDS_INFOPEN_ERR);
			else if(nRet==-2)
				objMsg.LoadString(IDS_OUTFOPEN_ERR);
			strcpy(szResultMsg,(LPCTSTR)objMsg);
		}
		else if( nConvKind==CODECONV_TO_JIS ) //SJIS -> JIS
		{
			nRet = objCodeConvManager.ConvertSjisToJis(szInFilePath,szTempFileName);
			if(nRet==1)
				objMsg.LoadString(IDS_CONV_SJIS2JIS);
			else if(nRet==-1)
				objMsg.LoadString(IDS_INFOPEN_ERR);
			else if(nRet==-2)
				objMsg.LoadString(IDS_OUTFOPEN_ERR);
			strcpy(szResultMsg,(LPCTSTR)objMsg);
		}
	}
	else if( nDetectCode==INPUT_EUC ) //入力ファイルＥＵＣ
	{
		if( nConvKind==CODECONV_TO_SJIS ) //EUC -> SJIS
		{
			nRet = objCodeConvManager.ConvertEucToSjis(szInFilePath,szTempFileName);
			if(nRet==1)
				objMsg.LoadString(IDS_CONV_EUC2SJIS);
			else if(nRet==-1)
				objMsg.LoadString(IDS_INFOPEN_ERR);
			else if(nRet==-2)
				objMsg.LoadString(IDS_OUTFOPEN_ERR);
			strcpy(szResultMsg,(LPCTSTR)objMsg);
		}
		else if( nConvKind==CODECONV_TO_JIS )	//EUC -> JIS
		{
			nRet = objCodeConvManager.ConvertEucToJis(szInFilePath,szTempFileName);
			if(nRet==1)
				objMsg.LoadString(IDS_CONV_EUC2JIS);
			else if(nRet==-1)
				objMsg.LoadString(IDS_INFOPEN_ERR);
			else if(nRet==-2)
				objMsg.LoadString(IDS_OUTFOPEN_ERR);
			strcpy(szResultMsg,(LPCTSTR)objMsg);
		}
	}
	else if( nDetectCode==INPUT_JIS ) //入力ファイルＪＩＳ
	{
		if( nConvKind==CODECONV_TO_SJIS )		//JIS -> SJIS
		{
			nRet = objCodeConvManager.ConvertJisToSjis(szInFilePath,szTempFileName);
			if(nRet==1)
				objMsg.LoadString(IDS_CONV_JIS2SJIS);
			else if(nRet==-1)
				objMsg.LoadString(IDS_INFOPEN_ERR);
			else if(nRet==-2)
				objMsg.LoadString(IDS_OUTFOPEN_ERR);
			strcpy(szResultMsg,(LPCTSTR)objMsg);
		}
		else if( nConvKind==CODECONV_TO_EUC ) //JIS -> EUC
		{
			nRet = objCodeConvManager.ConvertJisToEuc(szInFilePath,szTempFileName);
			if(nRet==1)
				objMsg.LoadString(IDS_CONV_JIS2EUC);
			else if(nRet==-1)
				objMsg.LoadString(IDS_INFOPEN_ERR);
			else if(nRet==-2)
				objMsg.LoadString(IDS_OUTFOPEN_ERR);
			strcpy(szResultMsg,(LPCTSTR)objMsg);
		}
	}

	//////////////////////////////////////////
	//		コード変換結果を画面出力		//
	//////////////////////////////////////////

	if(m_bIsOutOtherFileConv) //コード変換結果を別ファイル出力する
	{
		//変換結果
		if(strlen(szInFileName)>30)
			szInFileName[30]=0;
		sprintf(szBuf,"  %-30s %-15s %s", szInFileName,szOutFileName,szResultMsg);
		objConvResult.Set((const unsigned char*)szBuf);
		//結果をドキュメントへ登録
		m_pobjConvDoc->AppendConvLine(objConvResult);
	}
	else //コード変換結果を同一ファイルへ出力する
	{
		//入力ファイル削除
		bRet = ::DeleteFile(szInFilePath);
		if(bRet!=FALSE) //入力ファイル削除成功
		{
			//出力ファイル名を入力ファイル名へ変更
			nRet = rename(szTempFileName,szInFilePath);
			if(nRet==0) //OK
			{
				//変換結果
				if(strlen(szInFileName)>30)
					szInFileName[30]=0;
				sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"same to left",szResultMsg);
				objConvResult.Set((const unsigned char*)szBuf);
				//結果をドキュメントへ登録
				m_pobjConvDoc->AppendConvLine(objConvResult);
				//コード変換終了
				return TRUE;
			}
			else //NG
			{
				//変換結果
				objMsg.LoadString(IDS_CONV_OUT_RENAMEERR);
				strcpy(szResultMsg,(LPCTSTR)objMsg);
				if(strlen(szInFileName)>30)
					szInFileName[30]=0;
				sprintf(szBuf,"  %-30s %-15s %s", szInFileName,szTempFileName,szResultMsg);
				objConvResult.Set((const unsigned char*)szBuf);
				//結果をドキュメントへ登録
				m_pobjConvDoc->AppendConvLine(objConvResult);
				//コード変換終了
				return TRUE;
			}
		}
		else //入力ファイル削除失敗
		{
			//変換結果
			objMsg.LoadString(IDS_CONV_IN_RENAMEERR);
			strcpy(szResultMsg, (LPCTSTR)objMsg);
			if(strlen(szInFileName)>30)
				szInFileName[30]=0;
			sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
			objConvResult.Set((const unsigned char*)szBuf);
			//結果をドキュメントへ登録
			m_pobjConvDoc->AppendConvLine(objConvResult);
			//出力用ファイル削除
			if( (_access( szTempFileName, 0 )) != -1 )
			{
				::DeleteFile(szTempFileName);
			}
			//コード変換終了
			return TRUE;
		}
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字コード変換状況ダイアログ表示待ち処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForConvDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjConvStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjConvStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjConvStatusDlg->m_hWnd);
			if(bRet!=NULL)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字コード変換キャンセルチェック処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsConvCanceled()	//TRUE : キャンセル、FALSE : 未キャンセル
{
	if( m_pobjConvStatusDlg->IsCanceled() )
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame プロテクト関数（改行コード変換関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：改行コード変換実行処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoKaigyoCodeConv()
{
	CLine	objConvResult;
	CString objMsg;

	//改行コード変換実行ディレクトリ存在チェック
	CFileFind		objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolderKaigyoConv);
	if(!bRet)//ない！！
	{
		if(m_pApp->IsDriveRoot(m_objFolderKaigyoConv) == FALSE) //ルートディレクトリでもない
		{
			objMsg.LoadString(IDS_NOKAIGYOCONVFOLDER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//改行コード変換結果出力用ＭＤＩ子ウインドウ作成
	m_pobjKaigyoConvDoc = NULL;
	m_pobjKaigyoConvFrameWnd = NULL;
	bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjKaigyoConvDoc,&m_pobjKaigyoConvFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjKaigyoConvDoc->SetFontOfCurrentFileExt("");
	m_pobjKaigyoConvDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//書込み禁止ﾓｰﾄﾞｽﾃｰﾀｽﾍﾟｲﾝ設定
	CString	objTitle;
	objMsg.LoadString(IDS_KAIGYOCONV_TITLE);
	objTitle.Format(objMsg,m_objKaigyoConvKind);
	m_pobjKaigyoConvDoc->SetTitle(objTitle);
	m_pobjKaigyoConvDoc->SetConvMode(TRUE);

	//改行コード変換状況モードレスダイアログ表示
	m_pobjKaigyoConvStatusDlg = new CKaigyoConvStatusDlg();
	m_pobjKaigyoConvStatusDlg->Create(IDD_KAIGYOCONVSTATUS);
	m_pobjKaigyoConvStatusDlg->SetConvKindStr(m_objKaigyoConvKind);
	m_pobjKaigyoConvStatusDlg->SetFileKind(m_objFileKindKaigyoConv);
	m_pobjKaigyoConvStatusDlg->SetFolder(m_objFolderKaigyoConv);
	WaitForKaigyoConvDlgDisp();

	//タイトル追加
	objMsg.LoadString(IDS_KAIGYOCONV_TITLE2);
	objConvResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);

	//１ディレクトリ内改行コード変換実行
	bRet = DoKaigyoConvInOneDirectory(m_objFolderKaigyoConv,m_objFileKindKaigyoConv,m_objKaigyoConvKind);

	//コード変換ファイル数タイトル追加表示
	long lConvFileCount = m_pobjKaigyoConvStatusDlg->GetConvFileCount();
	char	szBuf[64];
	objMsg.LoadString(IDS_KAIGYOCONV_FILECOUNT);
	sprintf(szBuf, (LPCTSTR)objMsg, lConvFileCount );
	CString objStr = szBuf;
	objTitle = m_pobjKaigyoConvDoc->GetTitle();
	objTitle += objStr;
	m_pobjKaigyoConvDoc->SetTitle(objTitle);

	//注記出力
	objConvResult.Set((const unsigned char*)"");
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
	objConvResult.Set((const unsigned char*)"");
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
	objMsg.LoadString(IDS_KAIGYOCONV_MEMO);
	objConvResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
	objConvResult.Set((const unsigned char*)"");
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);

	//改行コード変換状況モードレスダイアログ消去
	m_pobjKaigyoConvStatusDlg->DestroyWindow();
	delete m_pobjKaigyoConvStatusDlg;
	m_pobjKaigyoConvStatusDlg = NULL;

	m_pobjKaigyoConvDoc->UpdateAllViews(NULL,NULL,NULL);

	//改行コード変換終了通知
	if( bRet != FALSE )
	{
		if(lConvFileCount>0)
		{
			objMsg.LoadString(IDS_KAIGYOCONVEND);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
		else
		{
			objMsg.LoadString(IDS_KAIGYOCONVZERO);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
	}
	else
	{
		objMsg.LoadString(IDS_KAIGYOCONVCANCEL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１ディレクトリ内改行コード変換実行処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoKaigyoConvInOneDirectory	// TRUE : 正常終了、FALSE : キャンセル終了
	(	CString		objFolder,				// 改行コード変換を行うフォルダ
		CString		objFileKinds,			// 改行コード変換対象ファイル種別
		CString		objConvKind				// 改行コード変換種別
	)
{
	char		szBuf[512];
	BOOL		bCanceled=FALSE;
	BOOL		bRetStatus = TRUE;
	BOOL		bRet, bRetTmp;
	CString		objFilter;
	CString		objOneKind, objRight, objBuf;
	CFileFind	objFileFind;
	CString		objMsg;

	//ディレクトリ名を追加
	CLine	objConvResult;
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
	objConvResult.Set((const unsigned char*)"");
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
	objMsg.LoadString(IDS_KAIGYOCONV_FOLDER_TITLE);
	sprintf(szBuf, (LPCTSTR)objMsg, (LPCTSTR)objFolder);
	objConvResult.Set((const unsigned char*)szBuf);
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
	objMsg.LoadString(IDS_KAIGYOCONV_LIST_TITLE);
	objConvResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
	objConvResult.Set((const unsigned char*)"-------------------------------------------------------------------------------------------------");
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);


	//改行コード変換状況ダイアログへフォルダ通知
	if( !IsKaigyoConvCanceled() )
		m_pobjKaigyoConvStatusDlg->SetFolder(objFolder);

	//////////////////////////////////////////////////
	//	まず、該当するファイルの一覧を作成する		//
	//////////////////////////////////////////////////

	//ファイル名リスト
	CObList		objFileList;

	objRight = objFileKinds;
	for(;;)
	{
		//改行コード変換対象ファイル種別切り取り
		int nIndex = objRight.Find(';');
		if(nIndex != -1)
		{
			objOneKind = objRight.Left(nIndex);
			objBuf = objRight.Right( objRight.GetLength() - nIndex - 1 );
			objRight = objBuf;
		}
		else
		{
			objOneKind = objRight;
		}

		//ファイル検索キー作成
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s%s",objFolder,objOneKind);
		else
			objFilter.Format("%s\\%s",objFolder,objOneKind);

		//改行コード変換対象ファイル検索
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
					continue;

				//発見したファイルパス取得
				CString*	pobjFileStr = new CString;
				(*pobjFileStr) = objFileFind.GetFileName();
				//リストへ登録
				objFileList.AddTail((CObject*)pobjFileStr);
			}
			if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
			{
				//最後の１個
				CString*	pobjFileStr = new CString;
				(*pobjFileStr) = objFileFind.GetFileName();
				objFileList.AddTail((CObject*)pobjFileStr);
			}
		}
		objFileFind.Close();

		if( nIndex == -1 ) // ファイル種別残りなし
		{
			break;
		}
	}

	//////////////////////////////////////////////////////
	//		ファイルリストをたどりながらコード変換		//
	//////////////////////////////////////////////////////

	bCanceled=FALSE;
	if(!objFileList.IsEmpty()) //発見ファイルあり
	{
		POSITION	pos = objFileList.GetHeadPosition();
		for( ; pos != 0; ) //リスト順次アクセス
		{
			//ウインドウズメッセージ処理
			m_pApp->MessagePump();
			//キャンセルチェック
			if( IsKaigyoConvCanceled() )
				break;

			//ファイル名取得
			CString* pobjFileName = (CString*)objFileList.GetNext(pos);
			if(pobjFileName!=NULL)
			{
				//ファイルパス作成
				CString objFilePath = objFolder + "\\" + (*pobjFileName);
				//１ファイル改行コード変換実行
				bRetTmp = DoKaigyoConvInOneFile(objFolder,(*pobjFileName),objFilePath,objConvKind);
				if( bRetTmp == FALSE ) //キャンセル終了してきた
				{
					bCanceled=TRUE;
					break;
				}
				else //改行コード変換ファイル数カウントアップ
				{
					m_pobjKaigyoConvStatusDlg->IncConvFileCount();
				}
			}
		}
	}

	//ファイルリスト全削除
	if(!objFileList.IsEmpty())
	{
		POSITION pos;
		for(pos=objFileList.GetHeadPosition(); pos!=0; )
		{
			CString* pobjStr = (CString*)objFileList.GetNext(pos);
			if(pobjStr!=NULL)
				delete pobjStr;
		}
		objFileList.RemoveAll();
	}

	//ウインドウズメッセージ処理
	m_pApp->MessagePump();
	//キャンセルチェック
	if(bCanceled)
		return FALSE;
	if( IsKaigyoConvCanceled() )
		return FALSE;

	if( m_bIsSearchSubFolderKaigyoConv ) //サブフォルダも検索する場合
	{
		//ファイル検索キー作成（ワイルドカード検索）
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s*.*",objFolder);
		else
			objFilter.Format("%s\\*.*",objFolder);

		//サブディレクトリ検索
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//ウインドウズメッセージ処理
				m_pApp->MessagePump();
				//キャンセルチェック
				if( IsKaigyoConvCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
				{
					//１ディレクトリ内改行コード変換実行（自分を再帰呼び出し）
					bRetTmp = DoKaigyoConvInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objConvKind);
					if(bRetTmp == FALSE) //キャンセル終了してきた
					{
						objFileFind.Close();
						return FALSE;
					}
				}
			}
			//最後の１個
			if( (objFileFind.IsDots()==FALSE) && (objFileFind.IsDirectory()!=FALSE) )
			{
				//１ディレクトリ内改行コード変換実行（自分を再帰呼び出し）
				bRetTmp = DoKaigyoConvInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objConvKind);
				if(bRetTmp == FALSE) //キャンセル終了してきた
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１ファイル文字コード変換実行処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoKaigyoConvInOneFile		// TRUE : 正常終了、FALSE : キャンセル終了
	(	CString		objFolder,				// 改行コード変換を行うフォルダ
		CString		objInFileName,			// 改行コード変換を行うファイル
		CString		objInFilePath,			// 改行コード変換を行うファイルパス
		CString		objConvKind				// 改行コード変換種別
	)
{
	CLine	objConvResult;
	char	szResultMsg[256];
	char	szBuf[512];
	char	buf[16], buf2[16];
	char*	pPtr=NULL;
	BOOL	bRet=TRUE;
	CFile	objInFile, objOutFile;
	DWORD	dwRet=0;
	INT		nRet=0;
	UINT	unRet=0;
	char	szTempFileName[_MAX_FNAME],szTempPathName[_MAX_PATH+16];
	char	szInFilePath[_MAX_PATH+16],szInFileName[_MAX_PATH],szOutFileName[_MAX_PATH];
	CCodeConvManager	objCodeConvManager;
	BYTE	byBuf=0;
	long	lCRCount=0,lLFCount=0;
	CString objMsg;

	//////////////////////////////////
	//		入出力ファイル準備		//
	//////////////////////////////////

	//改行コード変換状況ダイアログへファイル通知
	if( !IsKaigyoConvCanceled() )
		m_pobjKaigyoConvStatusDlg->SetFile(objInFileName);

	//入力ファイル名取得
	strcpy(szInFilePath,(LPCTSTR)objInFilePath);
	strcpy(szInFileName,(LPCTSTR)objInFileName);

	//出力用一時ファイル名取得
	strcpy(szTempPathName,(LPCTSTR)objFolder);
	unRet = ::GetTempFileName( szTempPathName,"Cnv",0,szTempFileName);
	if(unRet==0) //Error
	{
		objMsg.LoadString(IDS_CANTGET_KAIGYOCONVTMPFNAME);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	else
	{
		long lPos=0;
		long lCnt=0;
		for(lCnt=0; lCnt<(signed)strlen(szTempFileName); )
		{
			if(IsDBCSLeadByteEx(0,szTempFileName[lCnt]))
			{
				lCnt+=2;
				continue;
			}
			else
			{
				if(szTempFileName[lCnt]=='\\')
				{
					//'\'位置保持
					lPos = lCnt;
				}
				lCnt++;
			}
		}
		strcpy(szOutFileName,&szTempFileName[lPos+1]);
	}


	//入力ファイルオープン
	if( !objInFile.Open( objInFilePath, CFile::modeRead, NULL ) )
	{
		//出力用ファイル削除
		if( (_access( szTempFileName, 0 )) != -1 )
		{
			::DeleteFile(szTempFileName);
		}
		return FALSE;
	}
	else
	{
		objInFile.Close();
	}

	//出力ファイルオープン
	if( !objOutFile.Open( szTempFileName, (CFile::modeCreate|CFile::modeWrite), NULL ) )
	{
		objMsg.LoadString(IDS_CANTMAKE_KAIGYOCONVTMPFILE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		objInFile.Close();
		return FALSE;
	}
	else
	{
		objOutFile.Close();
	}

	//////////////////////////////////////
	//		入力ファイルチェック		//
	//////////////////////////////////////
	//入力ファイル文字コードの判別
	UINT nDetectCode = objCodeConvManager.DetectCode(szInFilePath);
	if( nDetectCode == INPUT_BINARY ||		//入力ファイルがバイナリファイル
		nDetectCode == INPUT_UNKNOWN		//文字コード不明
	)
	{
		//変換結果
		if( nDetectCode==INPUT_BINARY )
			objMsg.LoadString(IDS_KAIGYOCONV_MSG_BINARY);
		else if( nDetectCode==INPUT_UNKNOWN )
			objMsg.LoadString(IDS_KAIGYOCONV_MSG_UNKNOWN);
		strcpy(szResultMsg,(LPCTSTR)objMsg);

		if(strlen(szInFileName)>30)
			szInFileName[30]=0;
		sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
		objConvResult.Set((const unsigned char*)szBuf);

		//結果をドキュメントへ登録
		m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);

		//出力用ファイル削除
		if( (_access( szTempFileName, 0 )) != -1 )
		{
			::DeleteFile(szTempFileName);
		}

		//コード変換終了
		return TRUE;
	}

	//入力ファイル改行コードの判別
	nDetectCode = objCodeConvManager.DetectKaigyoCode(szInFilePath);
	if( nDetectCode == KAIGYO_UNKNOWN )	//改行コード不明
	{
		//変換結果
		objMsg.LoadString(IDS_KAIGYOCONV_MSG_UNKNOWN);
		strcpy(szResultMsg,(LPCTSTR)objMsg);

		if(strlen(szInFileName)>30)
			szInFileName[30]=0;
		sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
		objConvResult.Set((const unsigned char*)szBuf);

		//結果をドキュメントへ登録
		m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);

		//出力用ファイル削除
		if( (_access( szTempFileName, 0 )) != -1 )
		{
			::DeleteFile(szTempFileName);
		}

		//コード変換終了
		return TRUE;
	}
	else
	{
		if( (nDetectCode==KAIGYO_CRLF && objConvKind==KAIGYOCONV_TO_CRLF) ||	//すでに変換済み
			(nDetectCode==KAIGYO_LF   && objConvKind==KAIGYOCONV_TO_LF)   ||
			(nDetectCode==KAIGYO_CR   && objConvKind==KAIGYOCONV_TO_CR)
		)
		{
			//変換結果
			if( nDetectCode==KAIGYO_CRLF )
				objMsg.LoadString(IDS_KAIGYOCONV_MSG_CRLFEXIST);
			else if( nDetectCode==KAIGYO_LF )
				objMsg.LoadString(IDS_KAIGYOCONV_MSG_LFEXIST);
			else if( nDetectCode==KAIGYO_CR )
				objMsg.LoadString(IDS_KAIGYOCONV_MSG_CREXIST);
			strcpy(szResultMsg,(LPCTSTR)objMsg);

			if(strlen(szInFileName)>30)
				szInFileName[30]=0;
			sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
			objConvResult.Set((const unsigned char*)szBuf);

			//結果をドキュメントへ登録
			m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);

			//出力用ファイル削除
			if( (_access( szTempFileName, 0 )) != -1 )
			{
				::DeleteFile(szTempFileName);
			}

			//コード変換終了
			return TRUE;
		}
	}


	//////////////////////////////////
	//		改行コード変換実行		//
	//////////////////////////////////

	//入力ファイルオープン
	objInFile.Open( objInFilePath, CFile::modeRead, NULL );
	CArchive	arIn( &objInFile, CArchive::load, 16, buf );
	//出力ファイルオープン
	objOutFile.Open( szTempFileName, (CFile::modeCreate|CFile::modeWrite), NULL );
	CArchive	arOut( &objOutFile, CArchive::store, 16, buf2 );

	byBuf=0;
	lCRCount=lLFCount=0;
	for(;;)
	{
		TRY
		{
			arIn >> (BYTE&)byBuf;
		}
		CATCH_ALL(e) //EOF
		{
			break;
		}
		END_CATCH_ALL

		if(IsDBCSLeadByteEx(0,byBuf)) //２バイト文字
		{
			if(lCRCount!=0 || lLFCount!=0)
			{
				if(objConvKind==KAIGYOCONV_TO_CRLF)
				{
					arOut << (BYTE)'\r';
					arOut << (BYTE)'\n';
				}
				else if(objConvKind==KAIGYOCONV_TO_LF)
					arOut << (BYTE)'\n';
				else if(objConvKind==KAIGYOCONV_TO_CR)
					arOut << (BYTE)'\r';

				lCRCount=lLFCount=0;
			}

			arOut << (BYTE)byBuf;
			arIn  >> (BYTE&)byBuf;
			arOut << (BYTE)byBuf;
		}
		else
		{
			if(byBuf=='\r')
			{
				if( (nDetectCode==KAIGYO_CRLF && lCRCount==1 && lLFCount==1) ||
					(nDetectCode==KAIGYO_CR   && lCRCount==1)
				)
				{
					if(objConvKind==KAIGYOCONV_TO_CRLF)
					{
						arOut << (BYTE)'\r';
						arOut << (BYTE)'\n';
					}
					else if(objConvKind==KAIGYOCONV_TO_LF)
						arOut << (BYTE)'\n';
					else if(objConvKind==KAIGYOCONV_TO_CR)
						arOut << (BYTE)'\r';

					lCRCount=lLFCount=0;
				}
				lCRCount++;
			}
			else if(byBuf=='\n')
			{
				if(nDetectCode==KAIGYO_LF && lLFCount==1)
				{
					if(objConvKind==KAIGYOCONV_TO_CRLF)
					{
						arOut << (BYTE)'\r';
						arOut << (BYTE)'\n';
					}
					else if(objConvKind==KAIGYOCONV_TO_LF)
						arOut << (BYTE)'\n';
					else if(objConvKind==KAIGYOCONV_TO_CR)
						arOut << (BYTE)'\r';

					lCRCount=lLFCount=0;
				}
				lLFCount++;
			}
			else
			{
				if(lCRCount!=0 || lLFCount!=0)
				{
					if(objConvKind==KAIGYOCONV_TO_CRLF)
					{
						arOut << (BYTE)'\r';
						arOut << (BYTE)'\n';
					}
					else if(objConvKind==KAIGYOCONV_TO_LF)
						arOut << (BYTE)'\n';
					else if(objConvKind==KAIGYOCONV_TO_CR)
						arOut << (BYTE)'\r';

					lCRCount=lLFCount=0;
				}

				arOut << (BYTE)byBuf;
			}
		}
	}

	if(lCRCount!=0 || lLFCount!=0)
	{
		if(objConvKind==KAIGYOCONV_TO_CRLF)
		{
			arOut << (BYTE)'\r';
			arOut << (BYTE)'\n';
		}
		else if(objConvKind==KAIGYOCONV_TO_LF)
			arOut << (BYTE)'\n';
		else if(objConvKind==KAIGYOCONV_TO_CR)
			arOut << (BYTE)'\r';

		lCRCount=lLFCount=0;
	}

	//クローズ
	arIn.Close();
	objInFile.Close();
	arOut.Close();
	objOutFile.Close();

	//変換結果
	memset(szResultMsg,0,sizeof(szResultMsg));
	if(nDetectCode==KAIGYO_CRLF)
	{
		if(objConvKind==KAIGYOCONV_TO_LF)
			objMsg.LoadString(IDS_KAIGYOCONV_CRLF2LF);
		else if(objConvKind==KAIGYOCONV_TO_CR)
			objMsg.LoadString(IDS_KAIGYOCONV_CRLF2CR);
		strcpy(szResultMsg,(LPCTSTR)objMsg);
	}
	else if(nDetectCode==KAIGYO_LF)
	{
		if(objConvKind==KAIGYOCONV_TO_CRLF)
			objMsg.LoadString(IDS_KAIGYOCONV_LF2CRLF);
		else if(objConvKind==KAIGYOCONV_TO_CR)
			objMsg.LoadString(IDS_KAIGYOCONV_LF2CR);
		strcpy(szResultMsg,(LPCTSTR)objMsg);
	}
	else if(nDetectCode==KAIGYO_CR)
	{
		if(objConvKind==KAIGYOCONV_TO_CRLF)
			objMsg.LoadString(IDS_KAIGYOCONV_CR2CRLF);
		else if(objConvKind==KAIGYOCONV_TO_LF)
			objMsg.LoadString(IDS_KAIGYOCONV_CR2LF);
		strcpy(szResultMsg,(LPCTSTR)objMsg);
	}

	//////////////////////////////////////////
	//		コード変換結果を画面出力		//
	//////////////////////////////////////////

	if(m_bIsOutOtherFileKaigyoConv) //コード変換結果を別ファイル出力する
	{
		//変換結果
		if(strlen(szInFileName)>30)
			szInFileName[30]=0;
		sprintf(szBuf,"  %-30s %-15s %s", szInFileName,szOutFileName,szResultMsg);
		objConvResult.Set((const unsigned char*)szBuf);
		//結果をドキュメントへ登録
		m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
	}
	else //コード変換結果を同一ファイルへ出力する
	{
		//入力ファイル削除
		bRet = ::DeleteFile(szInFilePath);
		if(bRet!=FALSE) //入力ファイル削除成功
		{
			//出力ファイル名を入力ファイル名へ変更
			nRet = rename(szTempFileName,szInFilePath);
			if(nRet==0) //OK
			{
				//変換結果
				if(strlen(szInFileName)>30)
					szInFileName[30]=0;
				sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"same to left",szResultMsg);
				objConvResult.Set((const unsigned char*)szBuf);
				//結果をドキュメントへ登録
				m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
				//コード変換終了
				return TRUE;
			}
			else //NG
			{
				//変換結果
				objMsg.LoadString(IDS_KAIGYOCONV_OUT_RENAMEERR);
				strcpy(szResultMsg, (LPCTSTR)objMsg);
				if(strlen(szInFileName)>30)
					szInFileName[30]=0;
				sprintf(szBuf,"  %-30s %-15s %s", szInFileName,szTempFileName,szResultMsg);
				objConvResult.Set((const unsigned char*)szBuf);
				//結果をドキュメントへ登録
				m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
				//コード変換終了
				return TRUE;
			}
		}
		else //入力ファイル削除失敗
		{
			//変換結果
			objMsg.LoadString(IDS_KAIGYOCONV_IN_RENAMEERR);
			strcpy(szResultMsg, (LPCTSTR)objMsg );
			if(strlen(szInFileName)>30)
				szInFileName[30]=0;
			sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
			objConvResult.Set((const unsigned char*)szBuf);
			//結果をドキュメントへ登録
			m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
			//出力用ファイル削除
			if( (_access( szTempFileName, 0 )) != -1 )
			{
				::DeleteFile(szTempFileName);
			}
			//コード変換終了
			return TRUE;
		}
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：改行コード変換状況ダイアログ表示待ち処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForKaigyoConvDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjKaigyoConvStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjKaigyoConvStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjKaigyoConvStatusDlg->m_hWnd);
			if(bRet!=NULL)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：改行コード変換キャンセルチェック処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsKaigyoConvCanceled()	//TRUE : キャンセル、FALSE : 未キャンセル
{
	if( m_pobjKaigyoConvStatusDlg->IsCanceled() )
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame プロテクト関数（ワードカウント関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ワードカウント実行処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoWordCount()
{
	char	szBuf[256];
	CLine	objWCountResult;
	CString objMsg;

	//ワードカウント実行ディレクトリ存在チェック
	CFileFind		objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolderWCount);
	if(!bRet)//ない！！
	{
		if(m_pApp->IsDriveRoot(m_objFolderWCount) == FALSE)//ルートディレクトリでもない
		{
			objMsg.LoadString(IDS_NOWCOUNTFOLDER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//ワードカウント結果出力用ＭＤＩ子ウインドウ作成
	m_pobjWCountDoc = NULL;
	m_pobjWCountFrameWnd = NULL;
	bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjWCountDoc,&m_pobjWCountFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjWCountDoc->SetFontOfCurrentFileExt("");
	m_pobjWCountDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//書込み禁止モードステータスペイン設定
	CString	objTitle;
	objTitle.LoadString(IDS_WCOUNT_TITLE);
	m_pobjWCountDoc->SetTitle(objTitle);
	m_pobjWCountDoc->SetWCountMode(TRUE);

	//ワードカウント状況モードレスダイアログ表示
	m_pobjWCntStatusDlg = new CWCntStatusDlg();
	m_pobjWCntStatusDlg->Create(IDD_WCNTSTATUS);
	m_pobjWCntStatusDlg->SetFileKind(m_objFileKindWCount);
	m_pobjWCntStatusDlg->SetFolder(m_objFolderWCount);
	WaitForWCntDlgDisp();

	//カウンタクリア
	m_lByteCountSum = 0;				// バイト数合計
	m_lWordCountSum = 0;				// ワード数合計
	m_lLineCountSum = 0;				// 行数合計
	m_lEmptyLineCountSum = 0;		// 空行数合計
	m_lCPPCommentLineCountSum = 0;	// Ｃ・Ｃ＋＋コメント行数合計
	m_lWCountFileCnt = 0;			// ワードカウント対象ファイル数

	//タイトル追加
	objMsg.LoadString(IDS_WCOUNT_TITLE);
	objWCountResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);

	//１ディレクトリ内ワードカウント実行
	bRet = DoWCountInOneDirectory(m_objFolderWCount,m_objFileKindWCount);

	//ワードカウント対象ファイル数タイトル追加表示
	objMsg.LoadString(IDS_WCOUNT_FILECOUNT);
	sprintf(szBuf, (LPCTSTR)objMsg, m_lWCountFileCnt );
	CString objStr = szBuf;
	objTitle = m_pobjWCountDoc->GetTitle();
	objTitle += objStr;
	m_pobjWCountDoc->SetTitle(objTitle);

	//バイト数、ワード数、行数、空行数、Ｃ・Ｃ＋＋コメント行累計を追加
	objWCountResult.Set((const unsigned char*)"");
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objWCountResult.Set((const unsigned char*)"");
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objWCountResult.Set((const unsigned char*)"-----------------------------------------------------------------------------------------------");
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objMsg.LoadString(IDS_WCOUNT_LIST_SUM);
	sprintf(szBuf,(LPCTSTR)objMsg,m_lByteCountSum,m_lWordCountSum,m_lLineCountSum,m_lEmptyLineCountSum,m_lCPPCommentLineCountSum);
	objWCountResult.Set((const unsigned char*)szBuf);
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objWCountResult.Set((const unsigned char*)"");
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objMsg.LoadString(IDS_WCOUNT_MEMO);
	objWCountResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objMsg.LoadString(IDS_WCOUNT_MEMO2);
	objWCountResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objMsg.LoadString(IDS_WCOUNT_MEMO3);
	objWCountResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objMsg.LoadString(IDS_WCOUNT_MEMO4);
	objWCountResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objWCountResult.Set((const unsigned char*)"");
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objWCountResult.Set((const unsigned char*)"");
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);

	//ワードカウント状況モードレスダイアログ消去
	m_pobjWCntStatusDlg->DestroyWindow();
	delete m_pobjWCntStatusDlg;
	m_pobjWCntStatusDlg = NULL;

	//結果表示
	m_pobjWCountDoc->UpdateAllViews(NULL,NULL,NULL);

	//ワードカウント終了通知
	if( bRet != FALSE )
	{
		if(m_lWCountFileCnt==0)
		{
			objMsg.LoadString(IDS_WCOUNTZERO);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
		else
		{
			objMsg.LoadString(IDS_WCOUNTEND);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
	}
	else
	{
		objMsg.LoadString(IDS_WCOUNTCANCEL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１ディレクトリ内ワードカウント実行処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoWCountInOneDirectory		// TRUE : 正常終了、FALSE : キャンセル終了
	(	CString		objFolder,				// ワードカウントを行うフォルダ
		CString		objFileKinds			// ワードカウント対象ファイル種別
	)
{
	char		szBuf[512];
	BOOL		bRetStatus = TRUE;
	BOOL		bRet, bRetTmp;
	CString		objFilter, objMsg;
	CString		objOneKind, objRight, objBuf;
	CFileFind	objFileFind;

	//ワードカウント状況ダイアログへフォルダ通知
	if( !IsWCntCanceled() )
		m_pobjWCntStatusDlg->SetFolder(objFolder);

	//ディレクトリ名を追加
	CLine	objWCountResult;
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objWCountResult.Set((const unsigned char*)"");
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objMsg.LoadString(IDS_WCOUNT_FOLDER_TITLE);
	sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)objFolder);
	objWCountResult.Set((const unsigned char*)szBuf);
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objMsg.LoadString(IDS_WCOUNT_LIST_TITLE);
	objWCountResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objWCountResult.Set((const unsigned char*)"-----------------------------------------------------------------------------------------------");
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);


	objRight = objFileKinds;
	for(;;)
	{
		//ウインドウズメッセージ処理
		m_pApp->MessagePump();
		//キャンセルチェック
		if( IsWCntCanceled() )
			return FALSE;

		//ワードカウント対象ファイル種別切り取り
		int nIndex = objRight.Find(';');
		if(nIndex != -1)
		{
			objOneKind = objRight.Left(nIndex);
			objBuf = objRight.Right( objRight.GetLength() - nIndex - 1 );
			objRight = objBuf;
		}
		else
		{
			objOneKind = objRight;
		}

		//ファイル検索キー作成
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s%s",objFolder,objOneKind);
		else
			objFilter.Format("%s\\%s",objFolder,objOneKind);

		//ワードカウント対象ファイル検索
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//ウインドウズメッセージ処理
				m_pApp->MessagePump();
				//キャンセルチェック
				if( IsWCntCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
					continue;

				//ファイル名表示
				CString objStr1 = objFileFind.GetFileName();
				m_pobjWCntStatusDlg->SetFile(objStr1);

				//１ファイル内ワードカウント実行
				bRetTmp = DoWCountInOneFile(objFileFind.GetFileName(),objFileFind.GetFilePath());
				if( bRetTmp == FALSE ) //キャンセル終了してきた
				{
					objFileFind.Close();
					return FALSE;
				}
				else
				{
					//ワードカウント対象ファイル数カウントアップ
					m_lWCountFileCnt++;
				}
			}
			if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
			{
				//ファイル名表示
				CString objStr1 = objFileFind.GetFileName();
				m_pobjWCntStatusDlg->SetFile(objStr1);

				//最後の１個
				bRetTmp = DoWCountInOneFile(objFileFind.GetFileName(),objFileFind.GetFilePath());
				if( bRetTmp == FALSE ) //キャンセル終了してきた
				{
					objFileFind.Close();
					return FALSE;
				}
				else
				{
					//ワードカウント対象ファイル数カウントアップ
					m_lWCountFileCnt++;
				}
			}
		}
		objFileFind.Close();

		if( nIndex == -1 ) // ファイル種別残りなし
		{
			break;
		}
	}

	//ウインドウズメッセージ処理
	m_pApp->MessagePump();
	//キャンセルチェック
	if( IsWCntCanceled() )
		return FALSE;

	if( m_bIsSearchSubFolderWCount ) //サブフォルダも検索する場合
	{
		//ファイル検索キー作成（ワイルドカード検索）
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s*.*",objFolder);
		else
			objFilter.Format("%s\\*.*",objFolder);

		//サブディレクトリ検索
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//ウインドウズメッセージ処理
				m_pApp->MessagePump();
				//キャンセルチェック
				if( IsWCntCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
				{
					//１ディレクトリ内ワードカウント実行（自分を再帰呼び出し）
					bRetTmp = DoWCountInOneDirectory(objFileFind.GetFilePath(),objFileKinds);
					if(bRetTmp == FALSE) //キャンセル終了してきた
					{
						objFileFind.Close();
						return FALSE;
					}
				}
			}
			//最後の１個
			if( (objFileFind.IsDots()==FALSE) && (objFileFind.IsDirectory()!=FALSE) )
			{
				//１ディレクトリ内ワードカウント実行（自分を再帰呼び出し）
				bRetTmp = DoWCountInOneDirectory(objFileFind.GetFilePath(),objFileKinds);
				if(bRetTmp == FALSE) //キャンセル終了してきた
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１ファイル内ワードカウント実行処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoWCountInOneFile			// TRUE : 正常終了、FALSE : キャンセル終了
	(	CString		objFileName,			// ワードカウントを行うファイル
		CString		objFilePath				// ワードカウントを行うファイルパス
	)
{
	long	lCnt=0;
	long	lCRCount=0;
	long	lLFCount=0;
	long	lByteCount=0;
	long	lWordCount=0;
	long	lLineCount=0;
	long	lEmptyLineCountSum=0;
	long	lCPPCommentLineCountSum=0;
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	BOOL	bRetStatus = TRUE;
	BOOL	bSpaceFlg=FALSE;
	CFile	objFile;
	CCodeConvManager	objCodeConvManager;

	char	szBuf[MAX_BUF];
	char	szFilePath[_MAX_PATH];
	char	szFileName[_MAX_PATH];
	strcpy(szFilePath,(LPCTSTR)objFilePath);
	strcpy(szFileName,(LPCTSTR)objFileName);

	//バイナリファイルチェック
	UINT nDetectCode = objCodeConvManager.DetectCode(szFilePath);
	if( nDetectCode==INPUT_BINARY )
	{
		CLine	objWCountResult;
		if(strlen(szFileName)>35)
			szFileName[35]=0;
		sprintf(szBuf,"  %-35s        ---        ---        ---        ---        ---  (Binary File)",szFileName);
		objWCountResult.Set((const unsigned char*)szBuf);
		m_pobjWCountDoc->AppendWCountLine(objWCountResult);
		return bRetStatus;
	}

	//ファイルオープン
	if( !objFile.Open( objFilePath, CFile::modeRead, NULL ) )
		return bRetStatus;
	CArchive ar( &objFile, CArchive::load );

	//////////////////////////
	//	ワードカウント開始	//
	//////////////////////////

	int  nNowRet=0;
	int  nBeforeRet=0;
	m_pApp->PrepareMyReadString();
	for(lLineCount=0; ;lLineCount++)//１行づつ読み込み
	{
		lCRCount=0;
		lLFCount=0;
		CString objStrBuf=_T("");
		nNowRet = m_pApp->MyReadString(ar,objStrBuf,&lCRCount,&lLFCount);
		if( nNowRet == 0 ) //EOF終了
		{
			if( objStrBuf.IsEmpty() ) //データなし
			{
				if( nNowRet != nBeforeRet )
					lLineCount++;
				break;
			}
		}

		//MyReadString リターン値保持
		nBeforeRet = nNowRet;

		//バイト数カウント
		lByteCount += objStrBuf.GetLength();
		lByteCount += lCRCount;
		lByteCount += lLFCount;

		//ワード数カウント
		bSpaceFlg=FALSE;
		for(lCnt=0; lCnt<objStrBuf.GetLength(); )
		{
			if( IsDBCSLeadByteEx(0,objStrBuf.GetAt(lCnt)) )//２バイト文字の第一バイト
			{
				bSpaceFlg=FALSE;
				lCnt+=2;
			}
			else //１バイト文字
			{
				if( objStrBuf.GetAt(lCnt)==' ' || objStrBuf.GetAt(lCnt)=='\t' )
				{
					if(bSpaceFlg==FALSE) //空白文字新規出現
					{
						bSpaceFlg=TRUE;
						lWordCount++;
					}
				}
				else if(lCnt < objStrBuf.GetLength() - 1 && objStrBuf.GetAt(lCnt+1)==0)
				{
					if(bSpaceFlg==FALSE) //空白文字新規出現
					{
						bSpaceFlg=TRUE;
						lWordCount++;
					}
				}
				else //空白文字以外
				{
					bSpaceFlg=FALSE;
				}
				lCnt++;
			}
		}

		//空白行数カウント
		if( objStrBuf.IsEmpty() )
			lEmptyLineCountSum++;
		else
		{
			BOOL bFindFlg=FALSE;
			for(lCnt=0; lCnt<objStrBuf.GetLength(); lCnt++)
			{
				if( IsDBCSLeadByteEx(0,objStrBuf.GetAt(lCnt)) )
				{
					bFindFlg=TRUE;
					break;
				}
				else
				{
					if(objStrBuf.GetAt(lCnt)==' ' || objStrBuf.GetAt(lCnt)=='\t')
						continue;
					else
					{
						bFindFlg=TRUE;
						break;
					}
				}
			}
			if(bFindFlg==FALSE) //空白文字のみだった
				lEmptyLineCountSum++;
		}

		//Ｃ・Ｃ＋＋コメント行数カウント
		for(lCnt=0; lCnt<objStrBuf.GetLength(); lCnt++)
		{
			if( IsDBCSLeadByteEx(0,objStrBuf.GetAt(lCnt)) )//２バイト文字の第一バイト
			{
				break;
			}
			else
			{
				if(objStrBuf.GetAt(lCnt)==' ' || objStrBuf.GetAt(lCnt)=='\t') //空白文字の場合読み飛ばし
					continue;
				if( objStrBuf.GetAt(lCnt)=='/' )
				{
					if(lCnt < objStrBuf.GetLength()-1 )
					{
						if( objStrBuf.GetAt(lCnt+1)=='/' && 
							!IsDBCSLeadByteEx(0,objStrBuf.GetAt(lCnt+1)) )
						{
							lCPPCommentLineCountSum++;
							break;
						}
						else if( objStrBuf.GetAt(lCnt+1)=='*' && 
								 !IsDBCSLeadByteEx(0,objStrBuf.GetAt(lCnt+1)) )
						{
							lCPPCommentLineCountSum++;
							break;
						}
						else
							break;
					}
					else
						break;
				}
				else
					break;
			}
		}
	}

	//ファイルクローズ
	ar.Close();
	objFile.Close();

	//バイト数、ワード数、行数、空白行数、Ｃ・Ｃ＋＋コメント行数カウント結果を１行表示
	CLine	objWCountResult;
	if(strlen(szFileName)>35)
		szFileName[35]=0;
	sprintf(szBuf,"  %-35s  %9ld  %9ld  %9ld  %9ld  %9ld",
		szFileName,lByteCount,lWordCount,lLineCount,lEmptyLineCountSum,lCPPCommentLineCountSum);
	objWCountResult.Set((const unsigned char*)szBuf);
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);

	//バイト数、ワード数、行数、空白行数、Ｃ・Ｃ＋＋コメント行数を累計
	m_lByteCountSum += lByteCount;
	m_lWordCountSum += lWordCount;
	m_lLineCountSum += lLineCount;
	m_lEmptyLineCountSum += lEmptyLineCountSum;
	m_lCPPCommentLineCountSum += lCPPCommentLineCountSum;

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ワードカウント状況ダイアログ表示待ち処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForWCntDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjWCntStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjWCntStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjWCntStatusDlg->m_hWnd);
			if(bRet!=0)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ワードカウントキャンセルチェック処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsWCntCanceled()	//TRUE : キャンセル、FALSE : 未キャンセル
{
	if( m_pobjWCntStatusDlg->IsCanceled() )
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame プロテクト関数（ファイル検索関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル検索実行処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoFileSearch()
{
	CString objMsg;

	//ファイル検索実行ディレクトリ存在チェック
	CFileFind		objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolderFSearch);
	if(!bRet)//ない！！
	{
		if(m_pApp->IsDriveRoot(m_objFolderFSearch) == FALSE) //ルートディレクトリでもない
		{
			objMsg.LoadString(IDS_NOFSEARCHFOLDER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//ファイル検索結果出力用ＭＤＩ子ウインドウ作成
	m_pobjFSearchDoc = NULL;
	m_pobjFSearchFrameWnd = NULL;
	bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjFSearchDoc,&m_pobjFSearchFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjFSearchDoc->SetFontOfCurrentFileExt("");
	m_pobjFSearchDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);
	CString	objTitle;
	objMsg.LoadString(IDS_FSEARCH_TITLE);
	objTitle.Format( objMsg, m_objFileKindFSearch );
	m_pobjFSearchDoc->SetTitle(objTitle);
	m_pobjFSearchDoc->SetFSearchMode(TRUE);

	//ファイル検索状況モードレスダイアログ表示
	m_pobjFileSearchStatusDlg = new CFileSearchStatusDlg();
	m_pobjFileSearchStatusDlg->Create(IDD_FILESEARCHSTATUS);
	m_pobjFileSearchStatusDlg->SetFileKind(m_objFileKindFSearch);
	m_pobjFileSearchStatusDlg->SetFolder(m_objFolderFSearch);
	WaitForFSearchDlgDisp();

	//１ディレクトリ内ファイル検索実行
	bRet = DoFSearchInOneDirectory(m_objFolderFSearch,m_objFileKindFSearch);

	//該当件数タイトル追加表示
	long lMatchCount = m_pobjFileSearchStatusDlg->GetMatchCount();
	char	szBuf[32];
	objMsg.LoadString(IDS_FSEARCH_FILECOUNT);
	sprintf(szBuf, (LPCTSTR)objMsg,lMatchCount );
	CString objStr = szBuf;
	objTitle = m_pobjFSearchDoc->GetTitle();
	objTitle += objStr;
	m_pobjFSearchDoc->SetTitle(objTitle);

	//ファイル検索状況モードレスダイアログ消去
	m_pobjFileSearchStatusDlg->DestroyWindow();
	delete m_pobjFileSearchStatusDlg;
	m_pobjFileSearchStatusDlg = NULL;

	m_pobjFSearchDoc->UpdateAllViews(NULL,NULL,NULL);

	//ファイル検索終了通知
	if( bRet != FALSE )
	{
		objMsg.LoadString(IDS_FSEARCHEND);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
	else
	{
		objMsg.LoadString(IDS_FSEARCHCANCEL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１ディレクトリ内ファイル検索実行処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoFSearchInOneDirectory	// TRUE : 正常終了、FALSE : キャンセル終了
	(	CString		objFolder,				// 検索を行うフォルダ
		CString		objFileKinds			// 検索対象ファイル種別
	)
{
	BOOL		bRetStatus = TRUE;
	BOOL		bRet, bRetTmp;
	CString		objFilter;
	CString		objOneKind, objRight, objBuf;
	CFileFind	objFileFind;

	//ファイル検索状況ダイアログへフォルダ通知
	if( !IsFSearchCanceled() )
		m_pobjFileSearchStatusDlg->SetFolder(objFolder);

	objRight = objFileKinds;
	for(;;)
	{
		//ウインドウズメッセージ処理
		m_pApp->MessagePump();
		//キャンセルチェック
		if( IsFSearchCanceled() )
			return FALSE;

		//検索対象ファイル種別切り取り
		int nIndex = objRight.Find(';');
		if(nIndex != -1)
		{
			objOneKind = objRight.Left(nIndex);
			objBuf = objRight.Right( objRight.GetLength() - nIndex - 1 );
			objRight = objBuf;
		}
		else
		{
			objOneKind = objRight;
		}

		//ファイル検索キー作成
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s%s",objFolder,objOneKind);
		else
			objFilter.Format("%s\\%s",objFolder,objOneKind);

		//対象ファイル検索
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//ウインドウズメッセージ処理
				m_pApp->MessagePump();
				//キャンセルチェック
				if( IsFSearchCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
					continue;

				//ファイル発見
				m_pobjFileSearchStatusDlg->IncMatchCount();
				CString objStr1 = objFileFind.GetFileName();
				m_pobjFileSearchStatusDlg->SetFile(objStr1);
				//表示
				CLine	objFSearchResult;
				CString objStr2 = objFileFind.GetFilePath();
				objFSearchResult.Set((const unsigned char*)(LPCTSTR)objStr2);
				m_pobjFSearchDoc->AppendFSearchLine(objFSearchResult);
			}
			//最後の１個
			if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
			{
				//ファイル発見
				m_pobjFileSearchStatusDlg->IncMatchCount();
				CString objStr1 = objFileFind.GetFileName();
				m_pobjFileSearchStatusDlg->SetFile(objStr1);
				//表示
				CLine	objFSearchResult;
				CString objStr2 = objFileFind.GetFilePath();
				objFSearchResult.Set((const unsigned char*)(LPCTSTR)objStr2);
				m_pobjFSearchDoc->AppendFSearchLine(objFSearchResult);
			}
		}
		objFileFind.Close();

		if( nIndex == -1 ) // ファイル種別残りなし
		{
			break;
		}
	}

	//ウインドウズメッセージ処理
	m_pApp->MessagePump();
	//キャンセルチェック
	if( IsFSearchCanceled() )
		return FALSE;

	if( m_bIsSearchSubFolderFSearch ) //サブフォルダも検索する場合
	{
		//ファイル検索キー作成（ワイルドカード検索）
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s*.*",objFolder);
		else
			objFilter.Format("%s\\*.*",objFolder);

		//サブディレクトリ検索
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//ウインドウズメッセージ処理
				m_pApp->MessagePump();
				//キャンセルチェック
				if( IsFSearchCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}
				
				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
				{
					//１ディレクトリ内ファイル検索実行（自分を再帰呼び出し）
					bRetTmp = DoFSearchInOneDirectory(objFileFind.GetFilePath(),objFileKinds);
					if(bRetTmp == FALSE) //キャンセル終了してきた
					{
						objFileFind.Close();
						return FALSE;
					}
				}
			}
			//最後の１個
			if( (objFileFind.IsDots()==FALSE) && (objFileFind.IsDirectory()!=FALSE) )
			{
				//１ディレクトリ内ファイル検索実行（自分を再帰呼び出し）
				bRetTmp = DoFSearchInOneDirectory(objFileFind.GetFilePath(),objFileKinds);
				if(bRetTmp == FALSE) //キャンセル終了してきた
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル検索状況ダイアログ表示待ち処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForFSearchDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjFileSearchStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjFileSearchStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjFileSearchStatusDlg->m_hWnd);
			if(bRet!=NULL)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル検索キャンセルチェック処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsFSearchCanceled()	//TRUE : キャンセル、FALSE : 未キャンセル
{
	if( m_pobjFileSearchStatusDlg->IsCanceled() )
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame プロテクト関数（ファイル暗号化関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル暗号化実行処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoFileEncript()
{
	CLine	objEncriptResult;
	CString objMsg;

	//ファイル暗号化実行ディレクトリ存在チェック
	CFileFind		objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolderEncript);
	if(!bRet)//ない！！
	{
		if(m_pApp->IsDriveRoot(m_objFolderEncript) == FALSE) //ルートディレクトリでもない
		{
			objMsg.LoadString(IDS_NOENCRIPTFOLDER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//ファイル暗号化結果出力用ＭＤＩ子ウインドウ作成
	m_pobjEncriptDoc = NULL;
	m_pobjEncriptFrameWnd = NULL;
	bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjEncriptDoc,&m_pobjEncriptFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjEncriptDoc->SetFontOfCurrentFileExt("");
	m_pobjEncriptDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//書込み禁止ﾓｰﾄﾞｽﾃｰﾀｽﾍﾟｲﾝ設定
	CString	objTitle;
	objTitle.LoadString(IDS_ENCRYPT_TITLE);
	m_pobjEncriptDoc->SetTitle(objTitle);
	m_pobjEncriptDoc->SetEncriptMode(TRUE);

	//ファイル暗号化状況モードレスダイアログ表示
	m_pobjEncriptStatusDlg = new CEncriptStatusDlg();
	m_pobjEncriptStatusDlg->Create(IDD_ENCRIPTSTATUS);
	m_pobjEncriptStatusDlg->SetFileKind(m_objFileKindEncript);
	m_pobjEncriptStatusDlg->SetFolder(m_objFolderEncript);
	WaitForEncriptDlgDisp();

	//タイトル追加
	objMsg.LoadString(IDS_ENCRYPT_TITLE);
	objEncriptResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);

	//１ディレクトリ内ファイル暗号化実行
	bRet = DoEncriptInOneDirectory(m_objFolderEncript,m_objFileKindEncript,m_objKeyEncript);

	//暗号化対象ファイル数タイトル追加表示
	long lEncriptFileCount = m_pobjEncriptStatusDlg->GetEncriptFileCount();
	char	szBuf[64];
	objMsg.LoadString(IDS_ENCRYPT_FILECOUNT);
	sprintf(szBuf,(LPCTSTR)objMsg,lEncriptFileCount);
	CString objStr = szBuf;
	objTitle = m_pobjEncriptDoc->GetTitle();
	objTitle += objStr;
	m_pobjEncriptDoc->SetTitle(objTitle);

	//注記出力
	objEncriptResult.Set((const unsigned char*)"");
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
	objEncriptResult.Set((const unsigned char*)"");
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
	objMsg.LoadString(IDS_ENCRYPT_MEMO);
	objEncriptResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
	objEncriptResult.Set((const unsigned char*)"");
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);

	//ファイル暗号化状況モードレスダイアログ消去
	m_pobjEncriptStatusDlg->DestroyWindow();
	delete m_pobjEncriptStatusDlg;
	m_pobjEncriptStatusDlg = NULL;

	m_pobjEncriptDoc->UpdateAllViews(NULL,NULL,NULL);

	//ファイル暗号化終了通知
	if( bRet != FALSE )
	{
		if(lEncriptFileCount>0)
		{
			objMsg.LoadString(IDS_ENCRIPTEND);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
		else
		{
			objMsg.LoadString(IDS_ENCRIPTZERO);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
	}
	else
	{
		objMsg.LoadString(IDS_ENCRIPTCANCEL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１ディレクトリ内ファイル暗号化実行処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoEncriptInOneDirectory	// TRUE : 正常終了、FALSE : キャンセル終了
	(	CString		objFolder,				// ファイル暗号化を行うフォルダ
		CString		objFileKinds,			// ファイル暗号化対象ファイル種別
		CString		objKey					// ファイル暗号化キー
	)
{
	char		szBuf[512];
	BOOL		bCanceled=FALSE;
	BOOL		bRetStatus = TRUE;
	BOOL		bRet, bRetTmp;
	CString		objFilter, objMsg;
	CString		objOneKind, objRight, objBuf;
	CFileFind	objFileFind;

	//ディレクトリ名を追加
	CLine	objEncriptResult;
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
	objEncriptResult.Set((const unsigned char*)"");
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
	objMsg.LoadString(IDS_ENCRYPT_FOLDER_TITLE);
	sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)objFolder);
	objEncriptResult.Set((const unsigned char*)szBuf);
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
	objMsg.LoadString(IDS_ENCRYPT_LIST_TITLE);
	objEncriptResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
	objEncriptResult.Set((const unsigned char*)"-------------------------------------------------------------------------------------------------");
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);


	//ファイル暗号化状況ダイアログへフォルダ通知
	if( !IsEncriptCanceled() )
		m_pobjEncriptStatusDlg->SetFolder(objFolder);

	//////////////////////////////////////////////////
	//	まず、該当するファイルの一覧を作成する		//
	//////////////////////////////////////////////////

	//ファイル名リスト
	CObList		objFileList;

	objRight = objFileKinds;
	for(;;)
	{
		//暗号可対象ファイル種別切り取り
		int nIndex = objRight.Find(';');
		if(nIndex != -1)
		{
			objOneKind = objRight.Left(nIndex);
			objBuf = objRight.Right( objRight.GetLength() - nIndex - 1 );
			objRight = objBuf;
		}
		else
		{
			objOneKind = objRight;
		}

		//ファイル検索キー作成
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s%s",objFolder,objOneKind);
		else
			objFilter.Format("%s\\%s",objFolder,objOneKind);

		//対象ファイル検索
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
					continue;

				//発見したファイルパス取得
				CString*	pobjFileStr = new CString;
				(*pobjFileStr) = objFileFind.GetFileName();
				//リストへ登録
				objFileList.AddTail((CObject*)pobjFileStr);
			}
			if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
			{
				//最後の１個
				CString*	pobjFileStr = new CString;
				(*pobjFileStr) = objFileFind.GetFileName();
				objFileList.AddTail((CObject*)pobjFileStr);
			}
		}
		objFileFind.Close();

		if( nIndex == -1 ) // ファイル種別残りなし
		{
			break;
		}
	}

	//////////////////////////////////////////////////
	//		ファイルリストをたどりながら暗号化		//
	//////////////////////////////////////////////////

	bCanceled=FALSE;
	if(!objFileList.IsEmpty()) //発見ファイルあり
	{
		POSITION	pos = objFileList.GetHeadPosition();
		for( ; pos != 0; ) //リスト順次アクセス
		{
			//ウインドウズメッセージ処理
			m_pApp->MessagePump();
			//キャンセルチェック
			if( IsEncriptCanceled() )
				break;

			//ファイル名取得
			CString* pobjFileName = (CString*)objFileList.GetNext(pos);
			if(pobjFileName!=NULL)
			{
				//ファイルパス作成
				CString objFilePath = objFolder + "\\" + (*pobjFileName);
				//１ファイル 暗号化実行
				bRetTmp = DoEncriptInOneFile(objFolder,(*pobjFileName),objFilePath,objKey);
				if( bRetTmp == FALSE ) //キャンセル終了してきた
				{
					bCanceled=TRUE;
					break;
				}
				else //暗号化ファイル数カウントアップ
				{
					m_pobjEncriptStatusDlg->IncEncriptFileCount();
				}
			}
		}
	}

	//ファイルリスト全削除
	if(!objFileList.IsEmpty())
	{
		POSITION pos;
		for(pos=objFileList.GetHeadPosition(); pos!=0; )
		{
			CString* pobjStr = (CString*)objFileList.GetNext(pos);
			if(pobjStr!=NULL)
				delete pobjStr;
		}
		objFileList.RemoveAll();
	}

	//ウインドウズメッセージ処理
	m_pApp->MessagePump();
	//キャンセルチェック
	if(bCanceled)
		return FALSE;
	if( IsEncriptCanceled() )
		return FALSE;

	if( m_bIsSearchSubFolderEncript ) //サブフォルダも検索する場合
	{
		//ファイル検索キー作成（ワイルドカード検索）
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s*.*",objFolder);
		else
			objFilter.Format("%s\\*.*",objFolder);

		//サブディレクトリ検索
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//ウインドウズメッセージ処理
				m_pApp->MessagePump();
				//キャンセルチェック
				if( IsEncriptCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
				{
					//１ディレクトリ内ファイル暗号化実行（自分を再帰呼び出し）
					bRetTmp = DoEncriptInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objKey);
					if(bRetTmp == FALSE) //キャンセル終了してきた
					{
						objFileFind.Close();
						return FALSE;
					}
				}
			}
			//最後の１個
			if( (objFileFind.IsDots()==FALSE) && (objFileFind.IsDirectory()!=FALSE) )
			{
				//１ディレクトリ内ファイル暗号化実行（自分を再帰呼び出し）
				bRetTmp = DoEncriptInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objKey);
				if(bRetTmp == FALSE) //キャンセル終了してきた
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１ファイル暗号化実行処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoEncriptInOneFile			// TRUE : 正常終了、FALSE : キャンセル終了
	(	CString		objFolder,				// 暗号化を行うフォルダ
		CString		objInFileName,			// 暗号化を行うファイル
		CString		objInFilePath,			// 暗号化を行うファイルパス
		CString		objKey					// 暗号化キー
	)
{
	int		nCnt=0;
	CLine	objEncriptResult;
	char	szResultMsg[256];
	char	szBuf[512];
	char*	pPtr=NULL;
	BOOL	bRet=TRUE;
	BOOL	bRetStatus=TRUE;
	CFile	objInFile, objOutFile;
	DWORD	dwRet=0;
	INT		nRet=0;
	UINT	unRet=0;
	char	szTempFileName[_MAX_FNAME],szTempPathName[_MAX_PATH+16];
	char	szInFilePath[_MAX_PATH+16],szInFileName[_MAX_PATH],szOutFileName[_MAX_PATH];
	char	szKeyString[64];
	CDes	objDes;
	CString objMsg;

	//////////////////////////////
	//		暗号化キー準備		//
	//////////////////////////////

	//暗号化キーを文字－＞バイナリへ変換
	memset(szKeyString,0,sizeof(szKeyString));
	for(nCnt=0; nCnt<objKey.GetLength()/2; nCnt++)
	{
		int nHigh=0,nLow=0;
		BYTE byBuf = objKey.GetAt(2*nCnt);
		if( byBuf >= '0' && byBuf <= '9')
			nHigh = byBuf - '0';
		else if( byBuf >= 'a' && byBuf <= 'f' )
			nHigh = byBuf - 'a' + 10;
		else if( byBuf >= 'A' && byBuf <= 'F' )
			nHigh = byBuf - 'A' + 10;

		byBuf = objKey.GetAt(2*nCnt+1);
		if( byBuf >= '0' && byBuf <= '9')
			nLow = byBuf - '0';
		else if( byBuf >= 'a' && byBuf <= 'f' )
			nLow = byBuf - 'a' + 10;
		else if( byBuf >= 'A' && byBuf <= 'F' )
			nLow = byBuf - 'A' + 10;

		szKeyString[nCnt] = nHigh*16 + nLow;
	}

	//////////////////////////////////
	//		入出力ファイル準備		//
	//////////////////////////////////

	//ファイル暗号化状況ダイアログへファイル通知
	if( !IsEncriptCanceled() )
		m_pobjEncriptStatusDlg->SetFile(objInFileName);

	//入力ファイル名取得
	strcpy(szInFilePath,(LPCTSTR)objInFilePath);
	strcpy(szInFileName,(LPCTSTR)objInFileName);

	//出力用一時ファイル名取得
	strcpy(szTempPathName,(LPCTSTR)objFolder);
	unRet = ::GetTempFileName( szTempPathName,"Enc",0,szTempFileName);
	if(unRet==0) //Error
	{
		objMsg.LoadString(IDS_CANTGET_ENCRIPTTMPFNAME);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	else
	{
		long lCnt=0;
		long lPos=0;
		for(lCnt=0; lCnt<(signed)strlen(szTempFileName); )
		{
			if(IsDBCSLeadByteEx(0,szTempFileName[lCnt]))
			{
				lCnt+=2;
				continue;
			}
			else
			{
				if(szTempFileName[lCnt]=='\\')
				{
					//'\'位置保持
					lPos = lCnt;
				}
				lCnt++;
			}
		}
		strcpy(szOutFileName,&szTempFileName[lPos+1]);
	}


	//入力ファイルオープン
	if( !objInFile.Open( objInFilePath, CFile::modeRead, NULL ) ) //Error
	{
		//出力用ファイル削除
		if( (_access( szTempFileName, 0 )) != -1 )
		{
			::DeleteFile(szTempFileName);
		}
		return FALSE;
	}
	else
	{
		objInFile.Close(); //すぐクローズ
	}

	//出力ファイルオープン
	if( !objOutFile.Open( szTempFileName, (CFile::modeCreate|CFile::modeWrite), NULL ) ) //Error
	{
		objMsg.LoadString(IDS_CANTMAKE_ENCRIPTTMPFILE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		objInFile.Close();
		return FALSE;
	}
	else
	{
		objOutFile.Close(); //すぐクローズ
	}

	//////////////////////////////////
	//		ファイル暗号化実行		//
	//////////////////////////////////
	int	argc = 0;
	char**	argv=NULL;

	//メモリ領域確保
	argc = 6;
	argv = (char**)malloc(sizeof(char*)*6);
	for(nCnt=0; nCnt<6; nCnt++)
		argv[nCnt] = (char*)malloc(sizeof(char)*1024);

	//DES 暗号化パラメータ設定
	strcpy(argv[0],"des");
	strcpy(argv[1],"-e");
	strcpy(argv[2],"-k");
	memcpy(argv[3],szKeyString,sizeof(szKeyString));
	strcpy(argv[4],szInFilePath);
	strcpy(argv[5],szTempFileName);

	//DES 暗号化
	nRet = objDes.main(argc,argv);
	if(nRet==0)
		objMsg.LoadString(IDS_ENCRYPT_RESULTMSG);
	else if(nRet==-1)
		objMsg.LoadString(IDS_ENCRYPT_PARAMERR);
	else if(nRet==-2)
		objMsg.LoadString(IDS_ENCRYPT_FACCESSERR);
	strcpy(szResultMsg,(LPCTSTR)objMsg);

	//メモリ領域開放
	for(nCnt=0; nCnt<6; nCnt++)
		free(argv[nCnt]);
	free(argv);

	//////////////////////////////////////////////
	//		ファイル暗号化結果を画面出力		//
	//////////////////////////////////////////////

	if(m_bIsOutOtherFileEncript) //暗号化結果を別ファイル出力する
	{
		//変換結果
		if(strlen(szInFileName)>30)
			szInFileName[30]=0;
		sprintf(szBuf,"  %-30s %-15s %s", szInFileName,szOutFileName,szResultMsg);
		objEncriptResult.Set((const unsigned char*)szBuf);
		//結果をドキュメントへ登録
		m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
	}
	else //暗号化結果を同一ファイルへ出力する
	{
		//入力ファイル削除
		bRet = ::DeleteFile(szInFilePath);
		if(bRet != FALSE) //入力ファイル削除成功
		{
			//出力ファイル名を入力ファイル名へ変更
			nRet = ::rename(szTempFileName,szInFilePath);
			if(nRet==0) //OK
			{
				//変換結果
				if(strlen(szInFileName)>30)
					szInFileName[30]=0;
				sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"same to left",szResultMsg);
				objEncriptResult.Set((const unsigned char*)szBuf);
				//結果をドキュメントへ登録
				m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
				//ファイル暗号化終了
				return TRUE;
			}
			else //NG
			{
				//変換結果
				objMsg.LoadString(IDS_ENCRYPT_OUT_RENAMEERR);
				strcpy(szResultMsg,(LPCTSTR)objMsg);
				if(strlen(szInFileName)>30)
					szInFileName[30]=0;
				sprintf(szBuf,"  %-30s %-15s %s", szInFileName,szTempFileName,szResultMsg);
				objEncriptResult.Set((const unsigned char*)szBuf);
				//結果をドキュメントへ登録
				m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
				//ファイル暗号化終了
				return TRUE;
			}
		}
		else //入力ファイル削除失敗
		{
			//変換結果
			objMsg.LoadString(IDS_ENCRYPT_IN_RENAMEERR);
			strcpy(szResultMsg,(LPCTSTR)objMsg);
			if(strlen(szInFileName)>30)
				szInFileName[30]=0;
			sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
			objEncriptResult.Set((const unsigned char*)szBuf);
			//結果をドキュメントへ登録
			m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
			//出力用ファイル削除
			if( (_access( szTempFileName, 0 )) != -1 )
			{
				::DeleteFile(szTempFileName);
			}
			//ファイル暗号化終了
			return TRUE;
		}
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル暗号化状況ダイアログ表示待ち処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForEncriptDlgDisp()
{
	BOOL	bRet=FALSE;

	for(;;)
	{
		if( m_pobjEncriptStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjEncriptStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjEncriptStatusDlg->m_hWnd);
			if(bRet!=NULL)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル暗号化キャンセルチェック処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsEncriptCanceled()	//TRUE : キャンセル、FALSE : 未キャンセル
{
	if( m_pobjEncriptStatusDlg->IsCanceled() )
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame プロテクト関数（ファイル復号化関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル復号化実行処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoFileDecript()
{
	CLine	objDecriptResult;
	CString objMsg;

	//ファイル復号化実行ディレクトリ存在チェック
	CFileFind		objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolderDecript);
	if(!bRet)//ない！！
	{
		if(m_pApp->IsDriveRoot(m_objFolderDecript) == FALSE) //ルートディレクトリでもない
		{
			objMsg.LoadString(IDS_NODECRIPTFOLDER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//ファイル復号化結果出力用ＭＤＩ子ウインドウ作成
	m_pobjDecriptDoc = NULL;
	m_pobjDecriptFrameWnd = NULL;
	bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjDecriptDoc,&m_pobjDecriptFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjDecriptDoc->SetFontOfCurrentFileExt("");
	m_pobjDecriptDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//書込み禁止ﾓｰﾄﾞｽﾃｰﾀｽﾍﾟｲﾝ設定
	CString	objTitle;
	objTitle.LoadString(IDS_DECRYPT_TITLE);
	m_pobjDecriptDoc->SetTitle(objTitle);
	m_pobjDecriptDoc->SetDecriptMode(TRUE);

	//ファイル復号化状況モードレスダイアログ表示
	m_pobjDecriptStatusDlg = new CDecriptStatusDlg();
	m_pobjDecriptStatusDlg->Create(IDD_DECRIPTSTATUS);
	m_pobjDecriptStatusDlg->SetFileKind(m_objFileKindDecript);
	m_pobjDecriptStatusDlg->SetFolder(m_objFolderDecript);
	WaitForDecriptDlgDisp();

	//タイトル追加
	objMsg.LoadString(IDS_DECRYPT_TITLE);
	objDecriptResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);

	//１ディレクトリ内ファイル復号化実行
	bRet = DoDecriptInOneDirectory(m_objFolderDecript,m_objFileKindDecript,m_objKeyDecript);

	//復号化対象ファイル数タイトル追加表示
	long lDecriptFileCount = m_pobjDecriptStatusDlg->GetDecriptFileCount();
	char	szBuf[64];
	objMsg.LoadString(IDS_DECRYPT_FILECOUNT);
	sprintf(szBuf,(LPCTSTR)objMsg,lDecriptFileCount);
	CString objStr = szBuf;
	objTitle = m_pobjDecriptDoc->GetTitle();
	objTitle += objStr;
	m_pobjDecriptDoc->SetTitle(objTitle);

	//注記出力
	objDecriptResult.Set((const unsigned char*)"");
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
	objDecriptResult.Set((const unsigned char*)"");
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
	objMsg.LoadString(IDS_DECRYPT_MEMO);
	objDecriptResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
	objDecriptResult.Set((const unsigned char*)"");
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);

	//ファイル復号化状況モードレスダイアログ消去
	m_pobjDecriptStatusDlg->DestroyWindow();
	delete m_pobjDecriptStatusDlg;
	m_pobjDecriptStatusDlg = NULL;

	m_pobjDecriptDoc->UpdateAllViews(NULL,NULL,NULL);

	//ファイル復号化終了通知
	if( bRet != FALSE )
	{
		if(lDecriptFileCount>0)
		{
			objMsg.LoadString(IDS_DECRIPTEND);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
		else
		{
			objMsg.LoadString(IDS_DECRIPTZERO);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
	}
	else
	{
		objMsg.LoadString(IDS_DECRIPTCANCEL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１ディレクトリ内ファイル復号化実行処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoDecriptInOneDirectory	// TRUE : 正常終了、FALSE : キャンセル終了
	(	CString		objFolder,				// ファイル復号化を行うフォルダ
		CString		objFileKinds,			// ファイル復号化対象ファイル種別
		CString		objKey					// ファイル復号化キー
	)
{
	char		szBuf[512];
	BOOL		bCanceled=FALSE;
	BOOL		bRetStatus = TRUE;
	BOOL		bRet, bRetTmp;
	CString		objFilter, objMsg;
	CString		objOneKind, objRight, objBuf;
	CFileFind	objFileFind;

	//ディレクトリ名を追加
	CLine	objDecriptResult;
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
	objDecriptResult.Set((const unsigned char*)"");
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
	objMsg.LoadString(IDS_DECRYPT_FOLDER_TITLE);
	sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)objFolder);
	objDecriptResult.Set((const unsigned char*)szBuf);
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
	objMsg.LoadString(IDS_DECRYPT_LIST_TITLE);
	objDecriptResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
	objDecriptResult.Set((const unsigned char*)"-------------------------------------------------------------------------------------------------");
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);


	//ファイル復号化状況ダイアログへフォルダ通知
	if( !IsDecriptCanceled() )
		m_pobjDecriptStatusDlg->SetFolder(objFolder);

	//////////////////////////////////////////////////
	//	まず、該当するファイルの一覧を作成する		//
	//////////////////////////////////////////////////

	//ファイル名リスト
	CObList		objFileList;

	objRight = objFileKinds;
	for(;;)
	{
		//暗号可対象ファイル種別切り取り
		int nIndex = objRight.Find(';');
		if(nIndex != -1)
		{
			objOneKind = objRight.Left(nIndex);
			objBuf = objRight.Right( objRight.GetLength() - nIndex - 1 );
			objRight = objBuf;
		}
		else
		{
			objOneKind = objRight;
		}

		//ファイル検索キー作成
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s%s",objFolder,objOneKind);
		else
			objFilter.Format("%s\\%s",objFolder,objOneKind);

		//対象ファイル検索
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
					continue;

				//発見したファイルパス取得
				CString*	pobjFileStr = new CString;
				(*pobjFileStr) = objFileFind.GetFileName();
				//リストへ登録
				objFileList.AddTail((CObject*)pobjFileStr);
			}
			if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
			{
				//最後の１個
				CString*	pobjFileStr = new CString;
				(*pobjFileStr) = objFileFind.GetFileName();
				objFileList.AddTail((CObject*)pobjFileStr);
			}
		}
		objFileFind.Close();

		if( nIndex == -1 ) // ファイル種別残りなし
		{
			break;
		}
	}

	//////////////////////////////////////////////////
	//		ファイルリストをたどりながら復号化		//
	//////////////////////////////////////////////////

	bCanceled=FALSE;
	if(!objFileList.IsEmpty()) //発見ファイルあり
	{
		POSITION	pos = objFileList.GetHeadPosition();
		for( ; pos != 0; ) //リスト順次アクセス
		{
			//ウインドウズメッセージ処理
			m_pApp->MessagePump();
			//キャンセルチェック
			if( IsDecriptCanceled() )
				break;

			//ファイル名取得
			CString* pobjFileName = (CString*)objFileList.GetNext(pos);
			if(pobjFileName!=NULL)
			{
				//ファイルパス作成
				CString objFilePath = objFolder + "\\" + (*pobjFileName);
				//１ファイル 復号化実行
				bRetTmp = DoDecriptInOneFile(objFolder,(*pobjFileName),objFilePath,objKey);
				if( bRetTmp == FALSE ) //キャンセル終了してきた
				{
					bCanceled=TRUE;
					break;
				}
				else //復号化ファイル数カウントアップ
				{
					m_pobjDecriptStatusDlg->IncDecriptFileCount();
				}
			}
		}
	}

	//ファイルリスト全削除
	if(!objFileList.IsEmpty())
	{
		POSITION pos;
		for(pos=objFileList.GetHeadPosition(); pos!=0; )
		{
			CString* pobjStr = (CString*)objFileList.GetNext(pos);
			if(pobjStr!=NULL)
				delete pobjStr;
		}
		objFileList.RemoveAll();
	}

	//ウインドウズメッセージ処理
	m_pApp->MessagePump();
	//キャンセルチェック
	if(bCanceled)
		return FALSE;
	if( IsDecriptCanceled() )
		return FALSE;

	if( m_bIsSearchSubFolderDecript ) //サブフォルダも検索する場合
	{
		//ファイル検索キー作成（ワイルドカード検索）
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s*.*",objFolder);
		else
			objFilter.Format("%s\\*.*",objFolder);

		//サブディレクトリ検索
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//ウインドウズメッセージ処理
				m_pApp->MessagePump();
				//キャンセルチェック
				if( IsDecriptCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}
				
				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
				{
					//１ディレクトリ内ファイル復号化実行（自分を再帰呼び出し）
					bRetTmp = DoDecriptInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objKey);
					if(bRetTmp == FALSE) //キャンセル終了してきた
					{
						objFileFind.Close();
						return FALSE;
					}
				}
			}
			//最後の１個
			if( (objFileFind.IsDots()==FALSE) && (objFileFind.IsDirectory()!=FALSE) )
			{
				//１ディレクトリ内ファイル復号化実行（自分を再帰呼び出し）
				bRetTmp = DoDecriptInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objKey);
				if(bRetTmp == FALSE) //キャンセル終了してきた
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１ファイル復号化実行処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoDecriptInOneFile			// TRUE : 正常終了、FALSE : キャンセル終了
	(	CString		objFolder,				// 復号化を行うフォルダ
		CString		objInFileName,			// 復号化を行うファイル
		CString		objInFilePath,			// 復号化を行うファイルパス
		CString		objKey					// 復号化キー
	)
{
	int		nCnt=0;
	CLine	objDecriptResult;
	char	szResultMsg[256];
	char	szBuf[512];
	char*	pPtr=NULL;
	BOOL	bRet=TRUE;
	BOOL	bRetStatus=TRUE;
	CFile	objInFile, objOutFile;
	DWORD	dwRet=0;
	INT		nRet=0;
	UINT	unRet=0;
	char	szTempFileName[_MAX_FNAME],szTempPathName[_MAX_PATH+16];
	char	szInFilePath[_MAX_PATH+16],szInFileName[_MAX_PATH],szOutFileName[_MAX_PATH];
	char	szKeyString[64];
	CDes	objDes;
	CString objMsg;

	//////////////////////////////
	//		復号化キー準備		//
	//////////////////////////////

	//復号化キーを文字－＞バイナリへ変換
	memset(szKeyString,0,sizeof(szKeyString));
	for(nCnt=0; nCnt<objKey.GetLength()/2; nCnt++)
	{
		int nHigh=0,nLow=0;
		BYTE byBuf = objKey.GetAt(2*nCnt);
		if( byBuf >= '0' && byBuf <= '9')
			nHigh = byBuf - '0';
		else if( byBuf >= 'a' && byBuf <= 'f' )
			nHigh = byBuf - 'a' + 10;
		else if( byBuf >= 'A' && byBuf <= 'F' )
			nHigh = byBuf - 'A' + 10;

		byBuf = objKey.GetAt(2*nCnt+1);
		if( byBuf >= '0' && byBuf <= '9')
			nLow = byBuf - '0';
		else if( byBuf >= 'a' && byBuf <= 'f' )
			nLow = byBuf - 'a' + 10;
		else if( byBuf >= 'A' && byBuf <= 'F' )
			nLow = byBuf - 'A' + 10;

		szKeyString[nCnt] = nHigh*16 + nLow;
	}

	//////////////////////////////////
	//		入出力ファイル準備		//
	//////////////////////////////////

	//ファイル暗号化状況ダイアログへファイル通知
	if( !IsDecriptCanceled() )
		m_pobjDecriptStatusDlg->SetFile(objInFileName);

	//入力ファイル名取得
	strcpy(szInFilePath,(LPCTSTR)objInFilePath);
	strcpy(szInFileName,(LPCTSTR)objInFileName);

	//出力用一時ファイル名取得
	strcpy(szTempPathName,(LPCTSTR)objFolder);
	unRet = ::GetTempFileName( szTempPathName,"Dec",0,szTempFileName);
	if(unRet==0) //Error
	{
		objMsg.LoadString(IDS_CANTGET_DECRIPTTMPFNAME);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	else
	{
		long lCnt=0;
		long lPos=0;
		for(lCnt=0; lCnt<(signed)strlen(szTempFileName); )
		{
			if(IsDBCSLeadByteEx(0,szTempFileName[lCnt]))
			{
				lCnt+=2;
				continue;
			}
			else
			{
				if(szTempFileName[lCnt]=='\\')
				{
					//'\'位置保持
					lPos = lCnt;
				}
				lCnt++;
			}
		}
		strcpy(szOutFileName,&szTempFileName[lPos+1]);
	}


	//入力ファイルオープン
	if( !objInFile.Open( objInFilePath, CFile::modeRead, NULL ) ) //Error
	{
		//出力用ファイル削除
		if( (_access( szTempFileName, 0 )) != -1 )
		{
			::DeleteFile(szTempFileName);
		}
		return FALSE;
	}
	else
	{
		objInFile.Close(); //すぐクローズ
	}

	//出力ファイルオープン
	if( !objOutFile.Open( szTempFileName, (CFile::modeCreate|CFile::modeWrite), NULL ) ) //Error
	{
		objMsg.LoadString(IDS_CANTMAKE_DECRIPTTMPFILE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		objInFile.Close();
		return FALSE;
	}
	else
	{
		objOutFile.Close(); //すぐクローズ
	}

	//////////////////////////////////
	//		ファイル復号化実行		//
	//////////////////////////////////
	int	argc = 0;
	char**	argv=NULL;

	//メモリ領域確保
	argc = 6;
	argv = (char**)malloc(sizeof(char*)*6);
	for(nCnt=0; nCnt<6; nCnt++)
		argv[nCnt] = (char*)malloc(sizeof(char)*1024);

	//DES 復号化パラメータ設定
	strcpy(argv[0],"des");
	strcpy(argv[1],"-d");
	strcpy(argv[2],"-k");
	memcpy(argv[3],szKeyString,sizeof(szKeyString));
	strcpy(argv[4],szInFilePath);
	strcpy(argv[5],szTempFileName);

	//DES 復号化
	nRet = objDes.main(argc,argv);
	if(nRet==0)
		objMsg.LoadString(IDS_DECRYPT_RESULTMSG);
	else if(nRet==-1)
		objMsg.LoadString(IDS_DECRYPT_PARAMERR);
	else if(nRet==-2)
		objMsg.LoadString(IDS_DECRYPT_FACCESSERR);
	else if(nRet==-3)
		objMsg.LoadString(IDS_DECRYPT_FCOLLUPT);
	strcpy(szResultMsg,(LPCTSTR)objMsg);

	//メモリ領域開放
	for(nCnt=0; nCnt<6; nCnt++)
		free(argv[nCnt]);
	free(argv);

	//////////////////////////////////////////////
	//		ファイル復号化結果を画面出力		//
	//////////////////////////////////////////////

	if(m_bIsOutOtherFileDecript) //復号化結果を別ファイル出力する
	{
		//変換結果
		if(strlen(szInFileName)>30)
			szInFileName[30]=0;
		sprintf(szBuf,"  %-30s %-15s %s", szInFileName,szOutFileName,szResultMsg);
		objDecriptResult.Set((const unsigned char*)szBuf);
		//結果をドキュメントへ登録
		m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
	}
	else //復号化結果を同一ファイルへ出力する
	{
		//入力ファイル削除
		bRet = ::DeleteFile(szInFilePath);
		if(bRet != FALSE) //入力ファイル削除成功
		{
			//出力ファイル名を入力ファイル名へ変更
			nRet = ::rename(szTempFileName,szInFilePath);
			if(nRet==0) //OK
			{
				//変換結果
				if(strlen(szInFileName)>30)
					szInFileName[30]=0;
				sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"same to left",szResultMsg);
				objDecriptResult.Set((const unsigned char*)szBuf);
				//結果をドキュメントへ登録
				m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
				//ファイル復号化終了
				return TRUE;
			}
			else //NG
			{
				//変換結果
				objMsg.LoadString(IDS_DECRYPT_OUT_RENAMEERR);
				strcpy(szResultMsg,(LPCTSTR)objMsg);
				if(strlen(szInFileName)>30)
					szInFileName[30]=0;
				sprintf(szBuf,"  %-30s %-15s %s", szInFileName,szTempFileName,szResultMsg);
				objDecriptResult.Set((const unsigned char*)szBuf);
				//結果をドキュメントへ登録
				m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
				//ファイル復号化終了
				return TRUE;
			}
		}
		else //入力ファイル削除失敗
		{
			//変換結果
			objMsg.LoadString(IDS_DECRYPT_IN_RENAMEERR);
			strcpy(szResultMsg,(LPCTSTR)objMsg);
			if(strlen(szInFileName)>30)
				szInFileName[30]=0;
			sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
			objDecriptResult.Set((const unsigned char*)szBuf);
			//結果をドキュメントへ登録
			m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
			//出力用ファイル削除
			if( (_access( szTempFileName, 0 )) != -1 )
			{
				::DeleteFile(szTempFileName);
			}
			//ファイル復号化終了
			return TRUE;
		}
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル復号化状況ダイアログ表示待ち処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForDecriptDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjDecriptStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjDecriptStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjDecriptStatusDlg->m_hWnd);
			if(bRet!=NULL)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル復号化キャンセルチェック処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsDecriptCanceled()	//TRUE : キャンセル、FALSE : 未キャンセル
{
	if( m_pobjDecriptStatusDlg->IsCanceled() )
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame プロテクト関数（ファイル比較関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル比較実行処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoFileCompare
	(	CString&	objFilePath1,		//第一ファイルパス
		CString&	objFilePath2,		//第二ファイルパス
		CEditorDoc* pDoc				//ドキュメントクラスへのポインタ
	)
{
	clock_t			startClock, endClock;
	ULONG			ulDuration=0;
	char			szBuf[512];
	char			szBufForAr1[512], szBufForAr2[512];
	UINT			unCodeKind=0;
	char*			pPtr=NULL;
	char			szFilePath1[_MAX_PATH],  szFilePath2[_MAX_PATH];
	CCodeConvManager	objCodeConvManager;
	CFile			objFile1, objFile2;
	BOOL 			bRet=FALSE;
	CString			objTitle=_T("");
	long			lDelLineCount=0;
	long			lAddLineCount=0;
	BOOL			bIsCanceled=FALSE;
	CString			objMsg;
	CEditorDoc*	pobjCompareDoc=NULL;
	CFrameWnd*	pobjCompareFrameWnd=NULL;

	//ファイルオープン
	strcpy(szFilePath1,(LPCTSTR)objFilePath1);
	if(objCodeConvManager.DetectCode(szFilePath1) == INPUT_BINARY) //バイナリファイルチェック
	{
		objMsg.LoadString(IDS_FILECOMPARE_BINARY);
		sprintf(szBuf,(LPCTSTR)objMsg,szFilePath1);
		m_pApp->MyMessageBox(szBuf,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if( objFile1.Open( szFilePath1, CFile::modeRead ) == NULL ) //第一ファイルオープン
	{
		objMsg.LoadString(IDS_FOPEN_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	CArchive	ar1(&objFile1,CArchive::load,512,szBufForAr1);

	strcpy(szFilePath2,(LPCTSTR)objFilePath2);
	if(objCodeConvManager.DetectCode(szFilePath2) == INPUT_BINARY) //バイナリファイルチェック
	{
		objMsg.LoadString(IDS_FILECOMPARE_BINARY);
		sprintf(szBuf,(LPCTSTR)objMsg,szFilePath2);
		m_pApp->MyMessageBox(szBuf,(MB_OK|MB_ICONSTOP),0);
		ar1.Close();
		objFile1.Close();
		return;
	}
	if( objFile2.Open( szFilePath2, CFile::modeRead ) == NULL ) //第二ファイルオープン
	{
		objMsg.LoadString(IDS_FOPEN_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		ar1.Close();
		objFile1.Close();
		return;
	}
	CArchive	ar2(&objFile2,CArchive::load,512,szBufForAr2);

	if(pDoc == NULL)
	{
		//ファイル比較結果出力用ＭＤＩ子ウインドウ作成
		bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&pobjCompareDoc,&pobjCompareFrameWnd);
		if( bRet == FALSE )
		{
			ar1.Close();
			ar2.Close();
			objFile1.Close();
			objFile2.Close();
			return;
		}
	}
	else
	{
		pobjCompareDoc = pDoc;
	}
	pobjCompareDoc->SetFontOfCurrentFileExt("");
	pobjCompareDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//書込み禁止ﾓｰﾄﾞｽﾃｰﾀｽﾍﾟｲﾝ設定
	pobjCompareDoc->SetCompareMode(TRUE,szFilePath1,szFilePath2);
	//タイトル表示
	objMsg.LoadString(IDS_FILECOMPARE_TITLE);
	objTitle.Format(objMsg,szFilePath1,objFilePath2);
	pobjCompareDoc->SetTitle(objTitle);

	//ファイル比較状況モードレスダイアログ表示
	m_pobjFileCompareStatusDlg = new CFileCompareStatusDlg();
	m_pobjFileCompareStatusDlg->m_objFile1Name = szFilePath1;
	m_pobjFileCompareStatusDlg->m_objFile2Name = szFilePath2;
	m_pobjFileCompareStatusDlg->Create(IDD_FILECOMPARE);
	WaitForFileCompareStatusDlgDisp();

	//タスクスタート時間計測
	startClock = clock();

	//ファイル比較
	CFileCompare	objFileCompare;
	bRet = objFileCompare.DoCompare( m_pobjFileCompareStatusDlg,	// ファイル比較状況ダイアログ
									 pobjCompareDoc,				// ドキュメントクラス
									 ar1,							// 第一ファイルアーカイブ
									 ar2);							// 第二ファイルアーカイブ

	//ファイル比較状況モードレスダイアログ消去
	bIsCanceled = m_pobjFileCompareStatusDlg->IsCanceled();
	m_pobjFileCompareStatusDlg->DestroyWindow();
	delete m_pobjFileCompareStatusDlg;
	m_pobjFileCompareStatusDlg = NULL;

	pobjCompareDoc->UpdateAllViews(NULL,NULL,NULL);

	//タスクエンド時間計測
	endClock = clock();
	ulDuration = (ULONG)(endClock - startClock);
	ulDuration /= (ULONG)CLOCKS_PER_SEC;

	//結果表示
	CFileCompareResultDlg	objFileCompareResultDlg;
	objFileCompareResultDlg.m_objFile1Name = szFilePath1;
	objFileCompareResultDlg.m_objFile2Name = szFilePath2;
	objFileCompare.GetDelAddLineCount(&lDelLineCount,&lAddLineCount);
	sprintf(szBuf,"%6d",lDelLineCount);
	objFileCompareResultDlg.m_objDelLineCountStr = szBuf;
	sprintf(szBuf,"%6d",lAddLineCount);
	objFileCompareResultDlg.m_objAddLineCountStr = szBuf;
	if(bRet==FALSE)
	{
		if(bIsCanceled)
			objMsg.LoadString(IDS_FILECOMPARECANCEL);
		else
			objMsg.LoadString(IDS_FILECOMPARENG);
		objFileCompareResultDlg.m_objStatusStr = objMsg;
	}
	else 
	{
		if(lDelLineCount==0 && lAddLineCount==0)
			objMsg.LoadString(IDS_FILECOMPARE_END);
		else
			objMsg.LoadString(IDS_FILECOMPARE_END2);
		objFileCompareResultDlg.m_objStatusStr = objMsg;
	}
	objMsg.LoadString(IDS_TIME);
	sprintf(szBuf,(LPCTSTR)objMsg,ulDuration);
	objFileCompareResultDlg.m_objTimeStr = szBuf;
	objFileCompareResultDlg.DoModal();

	//ファイルクローズ
	ar1.Close();
	ar2.Close();
	objFile1.Close();
	objFile2.Close();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル比較状況ダイアログ表示待ち									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForFileCompareStatusDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjFileCompareStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjFileCompareStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjFileCompareStatusDlg->m_hWnd);
			if(bRet!=NULL)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame プロテクト関数（ディレクトリ比較関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ比較実行処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoDirCompare()
{
	clock_t	startClock, endClock;
	ULONG	ulDuration=0;
	char	szBuf[512];
	CLine	objCmpResult;
	CString objMsg;

	//実行ディレクトリ存在チェック
	CFileFind objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolder1DirCmp);
	if(!bRet)//ない！！
	{
		if(m_pApp->IsDriveRoot(m_objFolder1DirCmp) == FALSE) //ルートディレクトリでもない
		{
			objMsg.LoadString(IDS_NO1STDIR_CMP);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}
	bRet = objFileFind.FindFile(m_objFolder2DirCmp);
	if(!bRet)//ない！！
	{
		if(m_pApp->IsDriveRoot(m_objFolder2DirCmp) == FALSE) //ルートディレクトリでもない
		{
			objMsg.LoadString(IDS_NO2NDDIR_CMP);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//ディレクトリ比較結果出力用ＭＤＩ子ウインドウ作成
	m_pobjDirCmpDoc = NULL;
	m_pobjDirCmpFrameWnd = NULL;
	bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjDirCmpDoc,&m_pobjDirCmpFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjDirCmpDoc->SetFontOfCurrentFileExt("");
	m_pobjDirCmpDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//書込み禁止ﾓｰﾄﾞｽﾃｰﾀｽﾍﾟｲﾝ設定
	CString	objTitle;
	objMsg.LoadString(IDS_DIRCOMPARE_TITLE);
	objTitle.Format(objMsg, m_objFolder1DirCmp, m_objFolder2DirCmp);
	m_pobjDirCmpDoc->SetTitle(objTitle);
	m_pobjDirCmpDoc->SetDirCmpMode(TRUE);

	//ディレクトリ名登録
	m_pobjDirCmpDoc->SetDirCmpFolder(m_objFolder1DirCmp,m_objFolder2DirCmp);

	//ディレクトリ比較状況モードレスダイアログ表示
	m_pobjDirCompareStatusDlg = new CDirCompareStatusDlg();
	m_pobjDirCompareStatusDlg->Create(IDD_DIRCOMPARESTATUS);
	WaitForDirCompareStatusDlgDisp();
	m_pobjDirCompareStatusDlg->SetDir1(m_objFolder1DirCmp);
	m_pobjDirCompareStatusDlg->SetDir2(m_objFolder2DirCmp);

	//空行
	objCmpResult.Set((const unsigned char*)"");
	objCmpResult.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
	m_pobjDirCmpDoc->InsertDirCmpLine(0,&objCmpResult);

	//タスクスタート時間計測
	startClock = clock();

	//１ディレクトリ比較実行
	bRet = DoDirCmpInOneDirectory();

	//タイトル追加
	objCmpResult.Set((const unsigned char*)"-------------------------------------------------------------------------------------------------");
	objCmpResult.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
	m_pobjDirCmpDoc->InsertDirCmpLine(0,&objCmpResult);
	objMsg.LoadString(IDS_DIRCOMPARE_LIST_TITLE);
	objCmpResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	objCmpResult.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
	m_pobjDirCmpDoc->InsertDirCmpLine(0,&objCmpResult);
	objMsg.LoadString(IDS_DIRCOMPARE_2NDDIR);
	sprintf(szBuf,(LPCTSTR)objMsg,(LPCTSTR)m_objFolder2DirCmp);
	objCmpResult.Set((const unsigned char*)szBuf);
	objCmpResult.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
	m_pobjDirCmpDoc->InsertDirCmpLine(0,&objCmpResult);
	objMsg.LoadString(IDS_DIRCOMPARE_1STDIR);
	sprintf(szBuf,(LPCTSTR)objMsg,(LPCTSTR)m_objFolder1DirCmp);
	objCmpResult.Set((const unsigned char*)szBuf);
	objCmpResult.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
	m_pobjDirCmpDoc->InsertDirCmpLine(0,&objCmpResult);
	objCmpResult.Set((const unsigned char*)"");
	objCmpResult.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
	m_pobjDirCmpDoc->InsertDirCmpLine(0,&objCmpResult);
	objMsg.LoadString(IDS_DIRCOMPARE_TITLE2);
	objCmpResult.Set((const unsigned char*)(LPCTSTR)objMsg);
	objCmpResult.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
	m_pobjDirCmpDoc->InsertDirCmpLine(0,&objCmpResult);

	//ディレクトリ比較状況モードレスダイアログ消去
	BOOL bIsCanceled       = m_pobjDirCompareStatusDlg->IsCanceled();
	long lCompareFileCount = m_pobjDirCompareStatusDlg->GetCompareFileCount();
	long lAddFileCount     = m_pobjDirCompareStatusDlg->GetAddFileCount();
	long lDelFileCount     = m_pobjDirCompareStatusDlg->GetDelFileCount();
	long lDiffFileCount    = m_pobjDirCompareStatusDlg->GetDiffFileCount();
	m_pobjDirCompareStatusDlg->DestroyWindow();
	delete m_pobjDirCompareStatusDlg;
	m_pobjDirCompareStatusDlg = NULL;

	m_pobjDirCmpDoc->UpdateAllViews(NULL,NULL,NULL);

	//タスクエンド時間計測
	endClock = clock();
	ulDuration = (ULONG)(endClock - startClock);
	ulDuration /= (ULONG)CLOCKS_PER_SEC;

	//ディレクトリ比較終了通知
	CDirCompareResultDlg	objDirCompareResultDlg;
	objDirCompareResultDlg.m_objDir1Name = m_objFolder1DirCmp;
	objDirCompareResultDlg.m_objDir2Name = m_objFolder2DirCmp;
	objDirCompareResultDlg.m_lCompareFileCount = lCompareFileCount;
	objDirCompareResultDlg.m_lAddFileCount = lAddFileCount;
	objDirCompareResultDlg.m_lDelFileCount = lDelFileCount;
	objDirCompareResultDlg.m_lDiffFileCount = lDiffFileCount;
	objDirCompareResultDlg.m_lTime = (long)ulDuration;
	if(bRet==FALSE)
	{
		if(bIsCanceled)
			objMsg.LoadString(IDS_DIRCOMPARECANCEL);
		else
			objMsg.LoadString(IDS_DIRCOMPARENG);
		objDirCompareResultDlg.m_objStatusStr = objMsg;
	}
	else 
	{
		objMsg.LoadString(IDS_DIRCOMAPRE_END);
		objDirCompareResultDlg.m_objStatusStr = objMsg;
	}
	objDirCompareResultDlg.DoModal();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１ディレクトリ比較実行												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoDirCmpInOneDirectory()
{
	//ディレクトリ比較実行
	CDirCompare	objDirCompare;
	return objDirCompare.DoCompare( m_pobjDirCompareStatusDlg,
									m_pobjDirCmpDoc,
									m_objFolder1DirCmp,
							 		m_objFolder2DirCmp );
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ比較状況ダイアログ表示待ち								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForDirCompareStatusDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjDirCompareStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjDirCompareStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjDirCompareStatusDlg->m_hWnd);
			if(bRet!=NULL)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame プロテクト関数（ファイルタッチ関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイルタッチ実行処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoFileTouch()
{
	CString objMsg;

	//ファイルタッチ実行ディレクトリ存在チェック
	CFileFind		objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolderTouch);
	if(!bRet)//ない！！
	{
		if(m_pApp->IsDriveRoot(m_objFolderTouch) == FALSE) //ルートディレクトリでもない
		{
			objMsg.LoadString(IDS_NOFSEARCHFOLDER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//１ディレクトリ内ファイルタッチ実行
	bRet = DoFileTouchInOneDirectory(m_objFolderTouch,m_objFileKindTouch,m_objFileTouchTime);

	//ファイルタッチ終了通知
	if( bRet != FALSE )
	{
		objMsg.LoadString(IDS_FTOUCHEND);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
	else
	{
		objMsg.LoadString(IDS_FTOUCHCANCEL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１ディレクトリ内ファイルタッチ実行処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoFileTouchInOneDirectory	// TRUE : 正常終了、FALSE : キャンセル終了
	(	CString		objFolder,				// タッチを行うフォルダ
		CString		objFileKinds,			// タッチ対象ファイル種別
		CTime		objTouchTime			// タッチ日時
	)
{
	BOOL		bRetStatus = TRUE;
	BOOL		bRet, bRetTmp;
	CString		objFilter;
	CString		objOneKind, objRight, objBuf;
	CString		objFilePath;
	CFileFind	objFileFind;
	CFileStatus objFileStatus;

	objRight = objFileKinds;
	for(;;)
	{
		//ウインドウズメッセージ処理
		m_pApp->MessagePump();

		//検索対象ファイル種別切り取り
		int nIndex = objRight.Find(';');
		if(nIndex != -1)
		{
			objOneKind = objRight.Left(nIndex);
			objBuf = objRight.Right( objRight.GetLength() - nIndex - 1 );
			objRight = objBuf;
		}
		else
		{
			objOneKind = objRight;
		}

		//ファイル検索キー作成
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s%s",objFolder,objOneKind);
		else
			objFilter.Format("%s\\%s",objFolder,objOneKind);

		//対象ファイル検索
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//ウインドウズメッセージ処理
				m_pApp->MessagePump();

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
					continue;

				//ファイルタッチ
				objFilePath = objFileFind.GetFilePath();
				CFile::GetStatus((LPCTSTR)objFilePath,objFileStatus);
				objFileStatus.m_mtime = objTouchTime;
				CFile::SetStatus((LPCTSTR)objFilePath,objFileStatus);
			}
			//最後の１個
			if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
			{
				//ファイルタッチ
				objFilePath = objFileFind.GetFilePath();
				CFile::GetStatus((LPCTSTR)objFilePath,objFileStatus);
				objFileStatus.m_mtime = objTouchTime;
				CFile::SetStatus((LPCTSTR)objFilePath,objFileStatus);
			}
		}
		objFileFind.Close();

		if( nIndex == -1 ) // ファイル種別残りなし
		{
			break;
		}
	}

	//ウインドウズメッセージ処理
	m_pApp->MessagePump();

	if( m_bIsSearchSubFolderTouch ) //サブフォルダも検索する場合
	{
		//ファイル検索キー作成（ワイルドカード検索）
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s*.*",objFolder);
		else
			objFilter.Format("%s\\*.*",objFolder);

		//サブディレクトリ検索
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//ウインドウズメッセージ処理
				m_pApp->MessagePump();

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
				{
					//１ディレクトリ内ファイル検索実行（自分を再帰呼び出し）
					bRetTmp = DoFileTouchInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objTouchTime);
					if(bRetTmp == FALSE) //キャンセル終了してきた
					{
						objFileFind.Close();
						return FALSE;
					}
				}
			}
			//最後の１個
			if( (objFileFind.IsDots()==FALSE) && (objFileFind.IsDirectory()!=FALSE) )
			{
				//１ディレクトリ内ファイル検索実行（自分を再帰呼び出し）
				bRetTmp = DoFileTouchInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objTouchTime);
				if(bRetTmp == FALSE) //キャンセル終了してきた
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();
	}

	return bRetStatus;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame プロテクト関数（メールリスト関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：メールリスト状況ダイアログ表示待ち									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForMailListDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjMailListStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjMailListStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjMailListStatusDlg->m_hWnd);
			if(bRet!=0)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}


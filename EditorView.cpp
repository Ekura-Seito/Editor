// EditorView.cpp : CEditorView クラスの動作の定義を行います。
//

#include "stdafx.h"
#include <Afxtempl.h>			//ＭＦＣのＣＭａｐを使用する
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include <afxadv.h>				//for CSharedFile
#include <mbstring.h>			//for OnLButtonDblClk
#include <io.h>					//for _access
#include "Editor_Def.h"
#include "StatusBarDisp.h"		//ステータスバー表示情報構造体
#include "ColorManager.h"
#include "CodeConvManager.h"
#include "BitmapClient.h"
#include "Operation.h"
#include "OperationManager.h"
#include "MainFrm.h"
#include "Editor.h"
#include "Line.h"
#include "LineManager.h"
#include "UndoItem.h"
#include "UndoManager.h"
#include "EditorDoc.h"
#include "EditorView.h"
#include "LineJumpDlg.h"
#include "AutoResizer.h"
#include "MarkLineListDlg.h"
#include "KeyBind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorView

IMPLEMENT_DYNCREATE(CEditorView, CView)

BEGIN_MESSAGE_MAP(CEditorView, CView)
	ON_WM_CONTEXTMENU()
	ON_MESSAGE(WM_IME_NOTIFY, OnMyImeNotify)
	//{{AFX_MSG_MAP(CEditorView)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_SYSKEYDOWN()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectall)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECTALL, OnUpdateEditSelectall)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(ID_EDIT_DEL, OnUpdateEditDel)
	ON_COMMAND(ID_EDIT_DEL, OnEditDel)
	ON_COMMAND(ID_EDIT_JUMP2TOP, OnEditJump2top)
	ON_UPDATE_COMMAND_UI(ID_EDIT_JUMP2TOP, OnUpdateEditJump2top)
	ON_COMMAND(ID_EDIT_JUMP2BOTTOM, OnEditJump2bottom)
	ON_UPDATE_COMMAND_UI(ID_EDIT_JUMP2BOTTOM, OnUpdateEditJump2bottom)
	ON_UPDATE_COMMAND_UI(ID_EDIT_JUMP2LINE, OnUpdateEditJump2line)
	ON_COMMAND(ID_EDIT_JUMP2LINE, OnEditJump2line)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_VIEW_LINENO, OnUpdateViewLineno)
	ON_COMMAND(ID_VIEW_LINENO, OnViewLineno)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CRLF, OnUpdateViewCrlf)
	ON_COMMAND(ID_VIEW_CRLF, OnViewCrlf)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RULER, OnUpdateViewRuler)
	ON_COMMAND(ID_VIEW_RULER, OnViewRuler)
	ON_COMMAND(ID_PARENTHESIS, OnParenthesis)
	ON_COMMAND(ID_VIEW_TAB, OnViewTab)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TAB, OnUpdateViewTab)
	ON_COMMAND(ID_VIEW_ZENKAKUSPACE, OnViewZenkakuspace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZENKAKUSPACE, OnUpdateViewZenkakuspace)
	ON_COMMAND(ID_EDIT_TAB2SPACE, OnEditTab2space)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TAB2SPACE, OnUpdateEditTab2space)
	ON_COMMAND(ID_EDIT_SPACE2TAB, OnEditSpace2tab)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SPACE2TAB, OnUpdateEditSpace2tab)
	ON_COMMAND(ID_EDIT_SPACE_HAN2ZEN, OnEditSpaceHan2zen)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SPACE_HAN2ZEN, OnUpdateEditSpaceHan2zen)
	ON_COMMAND(ID_EDIT_SPACE_ZEN2HAN, OnEditSpaceZen2han)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SPACE_ZEN2HAN, OnUpdateEditSpaceZen2han)
	ON_COMMAND(ID_EDIT_KATAKANA_HAN2ZEN, OnEditKatakanaHan2zen)
	ON_UPDATE_COMMAND_UI(ID_EDIT_KATAKANA_HAN2ZEN, OnUpdateEditKatakanaHan2zen)
	ON_COMMAND(ID_EDIT_KATAKANA_ZEN2HAN, OnEditKatakanaZen2han)
	ON_UPDATE_COMMAND_UI(ID_EDIT_KATAKANA_ZEN2HAN, OnUpdateEditKatakanaZen2han)
	ON_COMMAND(ID_VIEW_UNDERLINE, OnViewUnderline)
	ON_UPDATE_COMMAND_UI(ID_VIEW_UNDERLINE, OnUpdateViewUnderline)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CARET_IBEAM, OnUpdateViewCaretIbeam)
	ON_COMMAND(ID_VIEW_CARET_IBEAM, OnViewCaretIbeam)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CARET_SQUARE, OnUpdateViewCaretSquare)
	ON_COMMAND(ID_VIEW_CARET_SQUARE, OnViewCaretSquare)
	ON_COMMAND(ID_EDIT_ALNUM_HAN2ZEN, OnEditAlnumHan2zen)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ALNUM_HAN2ZEN, OnUpdateEditAlnumHan2zen)
	ON_COMMAND(ID_EDIT_ALNUM_ZEN2HAN, OnEditAlnumZen2han)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ALNUM_ZEN2HAN, OnUpdateEditAlnumZen2han)
	ON_COMMAND(ID_EDIT_DELTOPSPACES, OnEditDeltopspaces)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELTOPSPACES, OnUpdateEditDeltopspaces)
	ON_COMMAND(ID_EDIT_DELTAB, OnEditDeltab)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELTAB, OnUpdateEditDeltab)
	ON_COMMAND(ID_EDIT_INSTAB, OnEditInstab)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSTAB, OnUpdateEditInstab)
	ON_UPDATE_COMMAND_UI(ID_PARENTHESIS, OnUpdateParenthesis)
	ON_COMMAND(ID_EDIT_TAGJUMP, OnEditTagjump)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TAGJUMP, OnUpdateEditTagjump)
	ON_COMMAND(ID_EDIT_INSCPPCOMMENT, OnEditInscppcomment)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSCPPCOMMENT, OnUpdateEditInscppcomment)
	ON_COMMAND(ID_EDIT_INSVBCOMMENT, OnEditInsvbcomment)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSVBCOMMENT, OnUpdateEditInsvbcomment)
	ON_COMMAND(ID_EDIT_INSASMCOMMENT, OnEditInsasmcomment)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSASMCOMMENT, OnUpdateEditInsasmcomment)
	ON_COMMAND(ID_VIEW_REDRAW, OnViewRedraw)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REDRAW, OnUpdateViewRedraw)
	ON_COMMAND(ID_VIEW_EOF, OnViewEof)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EOF, OnUpdateViewEof)
	ON_COMMAND(ID_JUMP_SETMARK, OnJumpSetmark)
	ON_UPDATE_COMMAND_UI(ID_JUMP_SETMARK, OnUpdateJumpSetmark)
	ON_COMMAND(ID_JUMP_DELMARK, OnJumpDelmark)
	ON_UPDATE_COMMAND_UI(ID_JUMP_DELMARK, OnUpdateJumpDelmark)
	ON_COMMAND(ID_JUMP_NEXTMARK, OnJumpNextmark)
	ON_UPDATE_COMMAND_UI(ID_JUMP_NEXTMARK, OnUpdateJumpNextmark)
	ON_COMMAND(ID_JUMP_PREVMARK, OnJumpPrevmark)
	ON_UPDATE_COMMAND_UI(ID_JUMP_PREVMARK, OnUpdateJumpPrevmark)
	ON_COMMAND(ID_JUMP_MARKLIST, OnJumpMarklist)
	ON_UPDATE_COMMAND_UI(ID_JUMP_MARKLIST, OnUpdateJumpMarklist)
	ON_COMMAND(ID_JUMP_ALLDELMARK, OnJumpAlldelmark)
	ON_UPDATE_COMMAND_UI(ID_JUMP_ALLDELMARK, OnUpdateJumpAlldelmark)
	ON_COMMAND(ID_EDIT_INSDAY, OnEditInsday)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSDAY, OnUpdateEditInsday)
	ON_COMMAND(ID_EDIT_INSTIME, OnEditInstime)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSTIME, OnUpdateEditInstime)
	ON_COMMAND(ID_EDIT_AL2UPPER, OnEditAl2upper)
	ON_UPDATE_COMMAND_UI(ID_EDIT_AL2UPPER, OnUpdateEditAl2upper)
	ON_COMMAND(ID_EDIT_AL2LOWER, OnEditAl2lower)
	ON_UPDATE_COMMAND_UI(ID_EDIT_AL2LOWER, OnUpdateEditAl2lower)
	ON_COMMAND(ID_WORDSKIP2RIGHT, OnWordskip2right)
	ON_COMMAND(ID_WORDSKIP2LEFT, OnWordskip2left)
	ON_COMMAND(ID_WORDSELECT2RIGHT, OnWordselect2right)
	ON_COMMAND(ID_WORDSELECT2LEFT, OnWordselect2left)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_EDIT_UUDECODE, OnEditUudecode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UUDECODE, OnUpdateEditUudecode)
	ON_COMMAND(ID_EDIT_BASE64DECODE, OnEditBase64decode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BASE64DECODE, OnUpdateEditBase64decode)
	ON_COMMAND(ID_EDIT_QUOTEDDECODE, OnEditQuoteddecode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_QUOTEDDECODE, OnUpdateEditQuoteddecode)
	ON_COMMAND(ID_EDIT_NEXTDIFF, OnEditNextdiff)
	ON_UPDATE_COMMAND_UI(ID_EDIT_NEXTDIFF, OnUpdateEditNextdiff)
	ON_COMMAND(ID_EDIT_PREVDIFF, OnEditPrevdiff)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PREVDIFF, OnUpdateEditPrevdiff)
	ON_COMMAND(ID_EDIT_JUMP2TOPSELECT, OnEditJump2topselect)
	ON_COMMAND(ID_EDIT_JUMP2BOTTOMSELECT, OnEditJump2bottomselect)
	ON_COMMAND(ID_EDIT_DIRCMP_SYOSAI, OnEditDircmpSyosai)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DIRCMP_SYOSAI, OnUpdateEditDircmpSyosai)
	ON_COMMAND(ID_PARENTHESIS_SELECT, OnParenthesisSelect)
	ON_COMMAND(ID_EDIT_BROWSEURL, OnEditBrowseurl)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BROWSEURL, OnUpdateEditBrowseurl)
	ON_COMMAND(ID_EDIT_EXECMAILER, OnEditExecmailer)
	ON_UPDATE_COMMAND_UI(ID_EDIT_EXECMAILER, OnUpdateEditExecmailer)
	ON_COMMAND(ID_EDIT_INSQUOTE, OnEditInsquote)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSQUOTE, OnUpdateEditInsquote)
	ON_COMMAND(ID_EDIT_DELCPPCOMMENT, OnEditDelcppcomment)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELCPPCOMMENT, OnUpdateEditDelcppcomment)
	ON_COMMAND(ID_EDIT_DELVBCOMMENT, OnEditDelvbcomment)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELVBCOMMENT, OnUpdateEditDelvbcomment)
	ON_COMMAND(ID_EDIT_DELASMCOMMENT, OnEditDelasmcomment)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELASMCOMMENT, OnUpdateEditDelasmcomment)
	ON_COMMAND(ID_EDIT_DELQUOTE, OnEditDelquote)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELQUOTE, OnUpdateEditDelquote)
	ON_COMMAND(ID_CHANGE_INSMODE, OnChangeInsmode)
	//}}AFX_MSG_MAP
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorView クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CEditorView::CEditorView()
{
	CString objMsg;
	m_pApp = (CEditorApp*)AfxGetApp();

	////////////////////////////////////////
	//	メンバ変数初期化
	////////////////////////////////////////

	// 描画関連
	m_pobjInvalidRegion = NULL;			// 無効なﾘｰｼﾞｮﾝ
	m_pobjInvalidRect = NULL;			// 無効な矩形
	m_bUseClipRgn = 0;				// クリップされたリージョンのみ描画
	m_bNoUseOffBuffer = 0;			// ｵﾌｽｸﾘｰﾝﾊﾞｯﾌｧ不使用ﾌﾗｸﾞ

	// 全角スペース関連
	m_bDispZenkakuSpace = 0;			// 全角スペース描画フラグ
	m_ZenkakuSpaceColor = 0;			// 全角スペースの色

	// タブ関連
	m_bDispTab = 0;					// タブコード描画フラグ
	m_TabColor = 0;					// タブコードの色

	// ルーラ関連
	m_bDispRuler = 0;				// ルーラ表示フラグ
	m_RulerColor = 0;				// ルーラの色
	m_RulerBackColor = 0;			// ルーラの背景の色
	memset(m_szRuler,0,sizeof(m_szRuler));
	m_RulerPos = CPoint(0,0);			// ルーラ表示時のルーラ直下位置

	// 行番号関連
	m_bDispLineNo = 0;				// 行番号表示フラグ
	m_LineNoColor = 0;				// 行番号の色
	m_LineNoBackColor = 0;			// 行番号の背景の色
	memset(m_szLineNoFormat,0,sizeof(m_szLineNoFormat));
	m_lLineNoWidth = 0;				// 行番号表示幅
	m_LineNoPos = CPoint(0,0);			// 行番号表示時の行頭位置

	// 改行マーク関連
	m_bDispCRLF = 0;					// 改行マーク表示フラグ
	m_CRLFColor = 0;					// 改行マークの色

	// オフスクリーンバッファ関連
	m_nOffBufferWidth = 0;			// ｵﾌｽｸﾘｰﾝﾊﾞｯﾌｧの幅

	// キャレット関連
	m_nCaretType = 0;				// キャレット形状
	m_nCaretBehave = 0;				// 画面スクロール時のキャレットの振る舞い
	m_caretPos = CPoint(0, 0);			// キャレット表示位置
	m_unCaretBlinkTime = 0;			// キャレット点滅間隔
	m_nCaretMoveType = 0;				// キャレット移動タイプ
	m_nLogPreCaretX = 0;				// 直前のキャレット論理Ｘ位置

	// IME関連
	m_hImc = NULL;
	m_bIsImmOpen = 0;				// ＩＭＭ変換中フラグ

	// ｽｸﾛｰﾙ関連
	m_nMapMode = 0;					// ﾏｯﾋﾟﾝｸﾞﾓｰﾄﾞ
	m_totalLog.cx = 0;				// 論理単位でのﾄｰﾀﾙｻｲｽﾞ
	m_totalLog.cy = 0;
	m_VPageLog.cx = 0;				// 垂直ｽｸﾛｰﾙﾊﾞｰの論理単位での１ﾍﾟｰｼﾞあたりｽｸﾛｰﾙｻｲｽﾞ
	m_VPageLog.cy = 0;
	m_VLineLog.cx = 0;				// 垂直ｽｸﾛｰﾙﾊﾞｰの論理単位での１行あたりｽｸﾛｰﾙｻｲｽﾞ
	m_VLineLog.cy = 0;
	m_HPageLog.cx = 0;				// 水平ｽｸﾛｰﾙﾊﾞｰの論理単位での１ﾍﾟｰｼﾞあたりｽｸﾛｰﾙｻｲｽﾞ
	m_HPageLog.cy = 0;
	m_HLineLog.cx = 0;				// 水平ｽｸﾛｰﾙﾊﾞｰの論理単位での１行あたりｽｸﾛｰﾙｻｲｽﾞ
	m_HLineLog.cy = 0;

	// ﾃｷｽﾄ関連
	m_bInsertKey = TRUE;				// Insertキー押下フラグ（ﾃﾞﾌｫﾙﾄで挿入ﾓｰﾄﾞ）
	m_n1stChar = 0;					// 押下ｷｰ　ﾏﾙﾁﾊﾞｲﾄ文字ﾊﾞｯﾌｧ
	m_n2ndChar = 0;					// 押下ｷｰ　ﾏﾙﾁﾊﾞｲﾄ文字ﾊﾞｯﾌｧ

	// 表示フォント関連
	m_lfHeight = 0;					// フォントの文字の高さ
	m_lfWidth = 0;					// フォントの文字の幅
	m_lDispFontPoint = 0;			// 表示フォントのポイント
	m_objDispFontName = _T("");			// 表示フォント名
	m_FontColor = 0;					// 文字色

	// カラー関連
	m_FontColor = 0;					// 通常の文字色
	m_BackColor = 0;					// 通常の背景色
	m_InvertFontColor = 0;			// 反転時の文字色
	m_InvertBackColor = 0;			// 反転時の背景色

	// 印刷関連
	m_bPrintLineNo = 0;				// 行番号印刷フラグ
	m_bPrint2Dangumi = 0;			// ２段組み印刷フラグ
	//m_objPrintFoldLine;				// 折り返し印刷行
	m_lPrintFoldLineNo = 0;			// 折り返し印刷行番号
	m_nHeaderLayout = 0;				// ヘッダレイアウト位置
	m_nFooterLayout = 0;				// フッタレイアウト位置
	m_objStrHeaderFormat = _T("");		// ヘッダフォーマット
	m_objStrFooterFormat = _T("");		// フッタフォーマット
	m_lPrintLineHeight = 0;			// １行あたりのデバイス単位の高さ
	m_lPrintCharWidth = 0;			// １文字あたりのデバイス単位の幅
	m_lPrintFontPoint = 0;			// 印字フォントのポイント
	m_objPrintFontName = _T("");		// 印字フォント名
	m_lLinePerPrintPage = 0;			// １ページあたりの行数

	// 範囲選択関連
	m_bIsShiftKeyDown = FALSE;			// シフトキー押下中フラグ
	m_bLButtonDown = 0;				// マウス左ボタン押下中フラグ
	m_lSelectStartLine = -1;			// 選択開始行番号
	m_lSelectStartOffset = -1;			// 選択開始列番号
	m_lSelectEndLine = -1;				// 選択終了行番号
	m_lSelectEndOffset = -1;			// 選択終了列番号
	m_lPreMoveCaretLine = 0;			// 移動前の行
	m_lPreMoveCaretOffset = 0;		// 移動前の列
	m_lLButtonDownLine = 0;			// マウス左ボタン押下した行
	m_lLButtonDownOffset = 0;		// マウス左ボタン押下した列

	// 行間関連
	m_lLineInterval = 0;				// 行間（ピクセル単位）

	// アンダーライン関連
	m_bDispUnderLine = 0;			// アンダーライン表示フラグ
	m_UnderLineColor = 0;			// アンダーラインの色

	// マーク関連
	m_MarkLineColor = 0;			// マーク行の色

	// インクリメンタルサーチ関連
	m_lIncrimentalSearchStartLine=0;	// インクリメンタルサーチ開始行
	m_lIncrimentalSearchStartOffset=0;// インクリメンタルサーチ開始オフセット
	m_objIncrimentalSearchLine.Set((const unsigned char*)"");	// インクリメンタルサーチ文字列

	// ファイル比較間連
	m_DiffDelLineColor = 0;			// 削除行の色
	m_DiffAddLineColor = 0;			// 追加行の色

	// ディレクトリ比較関連
	m_DirDiffLineColor = 0;			// 相違ファイル名の色

	//	レジストリから設定取得
	m_FontColor			 = m_pApp->GetProfileInt("CEditorView","FontColor",RGB(255,255,255));
	m_BackColor			 = m_pApp->GetProfileInt("CEditorView","BackColor",RGB(0,0,0));
	m_InvertFontColor	 = m_pApp->GetProfileInt("CEditorView","InvertFontColor",RGB(0,0,0));
	m_InvertBackColor	 = m_pApp->GetProfileInt("CEditorView","InvertBackColor",RGB(255,255,255));
	m_bDispRuler		 = m_pApp->GetProfileInt("CEditorView","bDispRuler",TRUE);
	m_RulerColor		 = m_pApp->GetProfileInt("CEditorView","RulerColor",RGB(0,0,0));
	m_RulerBackColor	 = m_pApp->GetProfileInt("CEditorView","RulerBackColor",RGB(192,192,192));
	strcpy(m_szRuler,m_pApp->GetProfileString("CEditorView","RulerFormat","----+----"));
	m_bDispCRLF			 = m_pApp->GetProfileInt("CEditorView","bDispCRLF",TRUE);
	m_CRLFColor			 = m_pApp->GetProfileInt("CEditorView","CRLFColor",RGB(255,255,0));
	m_bDispLineNo		 = m_pApp->GetProfileInt("CEditorView","bDispLineNo",TRUE);
	m_LineNoColor		 = m_pApp->GetProfileInt("CEditorView","LineNoColor",RGB(0,255,255));
	m_LineNoBackColor	 = m_pApp->GetProfileInt("CEditorView","LineNoBackColor",RGB(0,0,0));
	objMsg.LoadString(IDS_LINENO_FORMAT);
	strcpy(m_szLineNoFormat,(LPCTSTR)objMsg);
	m_bDispZenkakuSpace  = m_pApp->GetProfileInt("CEditorView","bDispZenkakuSpace",FALSE);
	m_ZenkakuSpaceColor  = m_pApp->GetProfileInt("CEditorView","ZenkakuSpaceColor",RGB(255,255,255));
	m_bDispTab           = m_pApp->GetProfileInt("CEditorView","bDispTab",FALSE);
	m_TabColor           = m_pApp->GetProfileInt("CEditorView","TabColor",RGB(255,255,255));
	m_lDispFontPoint	 = m_pApp->GetProfileInt("CEditorView","DispFontPoint",120);
	objMsg.LoadString(IDS_FONTNAME_DISP);
	m_objDispFontName	 = m_pApp->GetProfileString("CEditorView","DispFontName",objMsg);
	m_lPrintFontPoint	 = m_pApp->GetProfileInt("CEditorView","PrintFontPoint",80);
	objMsg.LoadString(IDS_FONTNAME_PRINT);
	m_objPrintFontName	 = m_pApp->GetProfileString("CEditorView","PrintFontName",objMsg);
	m_bPrintLineNo       = m_pApp->GetProfileInt("CEditorView","bPrintLineNo",1);
	m_bPrint2Dangumi     = m_pApp->GetProfileInt("CEditorView","bPrint2Dangumi",0);
	m_nHeaderLayout      = m_pApp->GetProfileInt("CEditorView","nHeaderLayout",0);
	m_nFooterLayout      = m_pApp->GetProfileInt("CEditorView","nFooterLayout",1);
	m_objStrHeaderFormat = m_pApp->GetProfileString("CEditorView","StrHeaderFormat",
								"FILE : &f  DATE : &d &t  PAGE : &p / &P");
	m_objStrFooterFormat = m_pApp->GetProfileString("CEditorView","StrFooterFormat","-- &p --");
//	m_lLineInterval		 = m_pApp->GetProfileInt("CEditorView","lLineInterval",2);
	m_lLineInterval = 2;
	m_bDispUnderLine	 = m_pApp->GetProfileInt("CEditorView","bDispUnderLine",TRUE);
	m_UnderLineColor	 = m_pApp->GetProfileInt("CEditorView","UnderLineColor",RGB(255,255,0));
	m_nCaretType 		 = m_pApp->GetProfileInt("CEditorView","CaretType",CARET_IBEAM);
	if((m_nCaretType != CARET_IBEAM) && (m_nCaretType != CARET_SQUARE))
		m_nCaretType = CARET_IBEAM;
	m_nCaretBehave       = m_pApp->GetProfileInt("CEditorView","CaretBehave",CARET_BEHAVE_MOVE);
	if((m_nCaretBehave != CARET_BEHAVE_MOVE) && (m_nCaretBehave != CARET_BEHAVE_STAY))
		m_nCaretBehave = CARET_BEHAVE_MOVE;
	m_unCaretBlinkTime   = m_pApp->GetProfileInt("CEditorView","CaretBlinkTime",0);
	if(m_unCaretBlinkTime==0)
		m_unCaretBlinkTime = ::GetCaretBlinkTime();
	m_nCaretMoveType     = m_pApp->GetProfileInt("CEditorView","CaretMoveType",CARET_MOVE_WORDTYPE);
	if(m_nCaretMoveType != CARET_MOVE_WORDTYPE && m_nCaretMoveType != CARET_MOVE_NOTEPADTYPE)
		m_nCaretMoveType = CARET_MOVE_WORDTYPE;
	m_MarkLineColor      = m_pApp->GetProfileInt("CEditorView","MarkLineColor",RGB(128,0,128));
	m_DiffDelLineColor   = (COLORREF)m_pApp->GetProfileInt("CEditorView","DiffDelLineColor",RGB(255,0,0));
	m_DiffAddLineColor   = (COLORREF)m_pApp->GetProfileInt("CEditorView","DiffAddLineColor",RGB(0,0,255));
	m_DirDiffLineColor   = (COLORREF)m_pApp->GetProfileInt("CEditorView","DirDiffLineColor",RGB(0,128,0));

	// CG: 次の行は 「IME サポート」 コンポーネントにより追加されています。
	m_hImc = ::ImmCreateContext();
	CgAdjustCompositionWindow();

	//ビットマップ付きポップアップメニュー
	static UINT toolbars[]={
		IDR_MAINFRAME,
		IDR_TOOLBAR_EDIT,
		IDR_TOOLBAR_FIND,
		IDR_TOOLBAR_JMP,
		IDR_TOOLBAR_TOOLS,
		IDR_TOOLBAR_WND
	};
	VERIFY(m_popupmenu.LoadMenu(CG_IDR_POPUP_EDITOR_VIEW));
	// Another method for adding bitmaps to menu options is through the
	// LoadToolbars member function.This method allows you to add all
	// the bitmaps in a toolbar object to menu options (if they exist).
	// The first function parameter is an array of toolbar id's.
	// The second is the number of toolbar id's. There is also a
	// function called LoadToolbar that just takes an id.
	m_popupmenu.LoadToolbars(toolbars,6);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CEditorView::~CEditorView()
{
	//設定をレジストリへ保存
	m_pApp->WriteProfileString("CEditorView","RulerFormat",m_szRuler);
	m_pApp->WriteProfileString("CEditorView","LineNoFormat",m_szLineNoFormat);
	m_pApp->WriteProfileInt("CEditorView","DispFontPoint",m_lDispFontPoint);
	m_pApp->WriteProfileString("CEditorView","DispFontName",m_objDispFontName);
	m_pApp->WriteProfileInt("CEditorView","PrintFontPoint",m_lPrintFontPoint);
	m_pApp->WriteProfileString("CEditorView","PrintFontName",m_objPrintFontName);
	m_pApp->WriteProfileInt("CEditorView","bPrintLineNo",m_bPrintLineNo);
	m_pApp->WriteProfileInt("CEditorView","bPrint2Dangumi",m_bPrint2Dangumi);
	m_pApp->WriteProfileInt("CEditorView","nHeaderLayout",m_nHeaderLayout);
	m_pApp->WriteProfileInt("CEditorView","nFooterLayout",m_nFooterLayout);
	m_pApp->WriteProfileString("CEditorView","StrHeaderFormat",m_objStrHeaderFormat);
	m_pApp->WriteProfileString("CEditorView","StrFooterFormat",m_objStrFooterFormat);
	m_pApp->WriteProfileInt("CEditorView","lLineInterval",m_lLineInterval);
	m_pApp->WriteProfileInt("CEditorView","CaretType",m_nCaretType);
	m_pApp->WriteProfileInt("CEditorView","CaretBehave",m_nCaretBehave);
	m_pApp->WriteProfileInt("CEditorView","CaretBlinkTime",m_unCaretBlinkTime);
	m_pApp->WriteProfileInt("CEditorView","CaretMoveType",m_nCaretMoveType);
	m_pApp->WriteProfileInt("CEditorView","DiffDelLineColor",m_DiffDelLineColor);
	m_pApp->WriteProfileInt("CEditorView","DiffAddLineColor",m_DiffAddLineColor);
	m_pApp->WriteProfileInt("CEditorView","DirDiffLineColor",m_DirDiffLineColor);

	if(m_pobjInvalidRegion)
	{
		delete m_pobjInvalidRegion;
		m_pobjInvalidRegion = NULL;
	}
	if(m_pobjInvalidRect)
	{
		delete m_pobjInvalidRect;
		m_pobjInvalidRect = NULL;
	}
	m_objDispFont.DeleteObject();
	m_objDispFontUnderLine.DeleteObject();

	// CG: 次の行は 「IME サポート」コンポーネントにより追加されています。
	::ImmAssociateContext(m_hWnd, NULL);
	::ImmDestroyContext(m_hImc);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView クラスの描画

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＥＲＡＳＥＢＫＧＮＤ処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;	//ちらつき防止
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デバイスコンテキストの属性調整処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(pDC);

	//ﾏｯﾋﾟﾝｸﾞﾓｰﾄﾞ設定
	pDC->SetMapMode(m_nMapMode);

//	//ﾋﾞｭｰﾎﾟｰﾄの原点設定
//	//これにより、OnDraw内で論理座標(0,0)に描画すると、
//	//デバイス座標(0-GetScrollPosLogX(),0)に描画される
//	CPoint ptVpOrg(0, 0);
//	if(!pDC->IsPrinting())//印刷以外
//	{
//		//Ｘ方向のスクロール分マイナス方向へシフト
//		ptVpOrg.x = -1 * GetScrollPosLogX();
//	}
//	pDC->SetViewportOrg(ptVpOrg);

	//ﾋﾞｭｰﾎﾟｰﾄの原点設定
	CPoint ptVpOrg(0, 0);
	pDC->SetViewportOrg(ptVpOrg);

	//派生元ハンドラをコール
	CView::OnPrepareDC(pDC, pInfo);

	if(pDC->IsPrinting())//印刷中
	{
		//印刷ループ終了タイミング管理
		pInfo->m_bContinuePrinting = (pInfo->m_nCurPage <= pInfo->GetMaxPage());
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：描画処理																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnDraw(CDC* pDC)
{
	if(pDC->IsPrinting())	//印刷用のコードは、OnPrint() に記述
		return;

	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	// OnDraw() 関数を呼ぶ前に現在のスクロール位置に
	// 影響を与えるビューポートの原点を調節しています。

	if( m_bUseClipRgn )//無効領域のみ描画する場合
	{
		//現在の無効領域取得
		CRgn	rgnClipBox;
		rgnClipBox.CreateRectRgnIndirect((*m_pobjInvalidRect));

		//現在のクリップ領域取得
		CRect	rcClip;
		pDC->GetClipBox(rcClip);
		rcClip.NormalizeRect();
		if( rcClip.IsRectEmpty()==FALSE )
		{
			if( rcClip.EqualRect(&clientRect)==FALSE )
			{
				//領域追加
				CRgn	rgn;
				rgn.CreateRectRgnIndirect(rcClip);
				rgnClipBox.CombineRgn(&rgnClipBox,&rgn,RGN_OR);
			}
		}

		//背景色ブラシ作成
		CBrush	objBackBrush;
		objBackBrush.CreateSolidBrush(m_BackColor);
		CBrush	objRulerBackBrush;
		objRulerBackBrush.CreateSolidBrush(m_RulerBackColor);

		//最大幅計算
		ULONG ulRight = 0;
		if( ((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth) > clientRect.Width() )
			ulRight = (ULONG)((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth);
		else
			ulRight = clientRect.Width();

		//行番号表示フィールドの幅
		if( m_bDispLineNo ) //行番号を表示する
			m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		else
			m_lLineNoWidth = 0;

		//描画開始行と描画終了行を計算
		ULONG	lineStart, lineStop;	//描画行番号
		lineStart = (ULONG)((ULONG)GetScrollPosLogY() / (ULONG)(m_lfHeight + m_lLineInterval) );
		lineStop  = lineStart + (ULONG)((ULONG)clientRect.Height() / (ULONG)(m_lfHeight + m_lLineInterval) ) + 1;

		//フォント設定
		CFont* oldFont = pDC->SelectObject(&m_objDispFont);

		//１行づつ描画
		ULONG ulCnt = 0;
		if( m_bDispRuler ) //ルーラを表示する
		{
			CRect	lineRect(0,0,ulRight,(m_lfHeight+m_lLineInterval));
			lineRect.NormalizeRect();
			if( rgnClipBox.RectInRegion(lineRect) ) //無効領域に入っている
			{
				pDC->SelectClipRgn(&rgnClipBox);
				pDC->FillRect(&lineRect,&objRulerBackBrush);
				//ルーラ描画
				MyDrawingRuler(pDC,0,0,ulRight,m_RulerColor,m_RulerBackColor);
			}
			ulCnt++;
			lineStop--;
		}
		ULONG ulLineNo=0;
		for ( ulLineNo=lineStart; ulLineNo<lineStop; ulLineNo++)
		{
			CRect	lineRect(0,(ulCnt*(m_lfHeight+m_lLineInterval)),ulRight,((ulCnt+1)*(m_lfHeight+m_lLineInterval)));
			lineRect.NormalizeRect();
			if( rgnClipBox.RectInRegion(lineRect) ) //無効領域に入っている
			{
				pDC->SelectClipRgn(&rgnClipBox);

				//１行描画
				pDC->FillRect(&lineRect,&objBackBrush);
				CLine*  pobjLine = pDoc->GetOneLine(ulLineNo);
				MyTextOut(pDC,m_lLineNoWidth,(ulCnt*(m_lfHeight+m_lLineInterval)),pobjLine,m_lfWidth,ulLineNo);
			}
			ulCnt++;
		}
		if(m_bDispUnderLine) //キャレット行アンダーライン描画
		{
			MyDrawingUnderLine(pDC);
		}

		//フォントをもとに戻す
		pDC->SelectObject(oldFont);

		//無効領域描画フラグを落としておく
		m_bUseClipRgn = FALSE;
	}
	else if( m_bNoUseOffBuffer ) //オフスクリーンバッファを使用しないで描画する場合
	{
		//背景色ブラシ作成
		CBrush	objBackBrush;
		objBackBrush.CreateSolidBrush(m_BackColor);
		CBrush	objRulerBackBrush;
		objRulerBackBrush.CreateSolidBrush(m_RulerBackColor);

		//最大幅計算
		ULONG ulRight = 0;
		if( ((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth) > clientRect.Width() )
			ulRight = (ULONG)((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth);
		else
			ulRight = clientRect.Width();

		//行番号表示フィールドの幅
		if( m_bDispLineNo ) //行番号を表示する
			m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		else
			m_lLineNoWidth = 0;

		//描画開始行と描画終了行を計算
		ULONG	lineStart, lineStop;	//描画行番号
		lineStart = (ULONG)((ULONG)GetScrollPosLogY() / (ULONG)(m_lfHeight+m_lLineInterval));
		lineStop  = lineStart + (ULONG)((ULONG)clientRect.Height() / (ULONG)(m_lfHeight+m_lLineInterval)) + 1;

		//フォント設定
		CFont* oldFont = pDC->SelectObject(&m_objDispFont);

		//１行づつ描画
		ULONG ulCnt = 0;
		if( m_bDispRuler ) //ルーラを表示する
		{
			CRect	lineRect(0,0,ulRight,(m_lfHeight+m_lLineInterval));
			lineRect.NormalizeRect();
			pDC->FillRect(&lineRect,&objRulerBackBrush);
			MyDrawingRuler(pDC,0,0,ulRight,m_RulerColor,m_RulerBackColor);
			ulCnt++;
			lineStop--;
		}
		ULONG ulLineNo;
		for ( ulLineNo=lineStart; ulLineNo<lineStop; ulLineNo++)
		{
			//背景色で塗りつぶし
			CRect	lineRect(0,(ulCnt*(m_lfHeight+m_lLineInterval)),ulRight,((ulCnt+1)*(m_lfHeight+m_lLineInterval)));
			lineRect.NormalizeRect();

			pDC->FillRect(&lineRect,&objBackBrush);
			CLine*  pobjLine = pDoc->GetOneLine(ulLineNo);
			MyTextOut(pDC,m_lLineNoWidth,(ulCnt*(m_lfHeight+m_lLineInterval)),pobjLine,m_lfWidth,ulLineNo);

			ulCnt++;
		}
		if( m_bDispUnderLine )//アンダーラインを描画する場合
		{
			MyDrawingUnderLine(pDC);
		}

		//フォントをもとに戻す
		pDC->SelectObject(oldFont);

		//オフスクリーンバッファ不使用フラグを落としておく
		m_bNoUseOffBuffer = FALSE;
	}
	else
	{
		//オフスクリーンバッファ更新
		UpdateOffBuffer();

		//オフスクリーンバッファを画面へ転送
		CRect clientRect;
		GetClientRect(&clientRect);
		clientRect.NormalizeRect();
		pDC->BitBlt( 0,						//転送先四角形の左上隅の論理 x 座標
					 0,						//転送先四角形の左上隅の論理 y 座標
					 m_nOffBufferWidth,		//転送先四角形と転送元ビットマップの幅 (論理単位)
					 clientRect.Height(),	//転送先四角形と転送元ビットマップの高さ (論理単位)
					 &m_objOffBufferDC,		//ｵﾌｽｸﾘｰﾝﾊﾞｯﾌｧのDC
					 0,						//転送元ビットマップの左上隅の論理 x 座標
					 0,						//転送元ビットマップの左上隅の論理 y 座標
					 SRCCOPY);				//実行ﾗｽﾀｵﾍﾟﾚｰｼｮﾝ（転送元ビットマップを転送先ビットマップにコピー）
	}

	m_bUseClipRgn = 0;				// クリップされたリージョンのみ描画
	m_bNoUseOffBuffer = 0;			// オフスクリーンバッファ不使用フラグ
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView クラスの印刷

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷準備処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中フラグオン
	if(pInfo->m_bPreview)
	{
		pDoc->SetFlagPrintPreview(TRUE);
	}

	//デフォルトの印刷準備（印刷ダイアログオープン）
	return DoPreparePrinting(pInfo);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷サイクル初期化処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	//プリンタＤＣ取得
	CDC* pobjPrinterDC = CDC::FromHandle(pDC->m_hAttribDC);

	//印刷用フォント作成
	m_objPrintFont.CreatePointFont(m_lPrintFontPoint,m_objPrintFontName,pobjPrinterDC);

	//１ページあたりの行数はまだ不明
	m_lLinePerPrintPage = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷サイクル終了処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	//印刷用フォント破棄
	m_objPrintFont.DeleteObject();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷プレビュー終了処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中フラグオフ
	pDoc->SetFlagPrintPreview(FALSE);

	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ページ単位の印刷処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	long  lHeadLen=0;
	long  lMargin=0;
	long  lFootLen=0;
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//////////////////////////////////////
	//	印刷余白マージン調節
	//////////////////////////////////////
	CRect objMarginRect;
	m_pApp->GetPrintMargin(objMarginRect);
	objMarginRect.NormalizeRect();
	int nHorzRes = pDC->GetDeviceCaps(LOGPIXELSX);
	int nVertRes = pDC->GetDeviceCaps(LOGPIXELSY);
	pInfo->m_rectDraw.left   += ::MulDiv(nHorzRes,objMarginRect.left,  1000);
	pInfo->m_rectDraw.top    += ::MulDiv(nVertRes,objMarginRect.top,   1000);
	pInfo->m_rectDraw.right  -= ::MulDiv(nHorzRes,objMarginRect.right, 1000);
	pInfo->m_rectDraw.bottom -= ::MulDiv(nVertRes,objMarginRect.bottom,1000);

	//////////////////////////////////////
	//	印字フォント選択
	//////////////////////////////////////
	CFont* pOldFont = (CFont*)pDC->SelectObject(&m_objPrintFont);

	//////////////////////////////////////
	//	各行の高さを計算
	//////////////////////////////////////
	TEXTMETRIC	tm;
	pDC->GetTextMetrics(&tm);
	m_lPrintLineHeight = tm.tmHeight + tm.tmExternalLeading;
	m_lPrintCharWidth  = tm.tmAveCharWidth;

	//１行に印字できる文字数
	long lCharCnt = pInfo->m_rectDraw.Width() / m_lPrintCharWidth;

	if(m_lLinePerPrintPage == 0)
	{
		//１ページあたりの行数計算
		m_lLinePerPrintPage = pInfo->m_rectDraw.Height() / m_lPrintLineHeight;
		m_lLinePerPrintPage -= 2; //ヘッダ２行分をひく
		m_lLinePerPrintPage -= 2; //フッタ２行分をひく

		//総ページ数計算
		UINT unMaxPage = CountPrintMaxPage(pInfo,m_lLinePerPrintPage,m_lPrintCharWidth);
		pInfo->SetMaxPage( unMaxPage );
	}

	//////////////////////////////////////
	//	ページヘッダ印字
	//////////////////////////////////////
	CString	objHeader = _T("");
	ParsePrintHeaderFormat(m_objStrHeaderFormat,objHeader,pInfo);
	if(!objHeader.IsEmpty())
	{
		if(m_nHeaderLayout==PRINT_HEADER_LEFT) //ヘッダ左よせ
		{
			//ヘッダ左よせ印字
			pDC->TextOut(pInfo->m_rectDraw.left,pInfo->m_rectDraw.top,objHeader);
		}
		else if(m_nHeaderLayout==PRINT_HEADER_CENTER) //ヘッダ中央
		{
			//ヘッダの文字数
			lHeadLen = objHeader.GetLength();
			//マージン
			lMargin = (lCharCnt - lHeadLen) / 2;
			if(lMargin<0)
				lMargin = 0;
			//ヘッダ中央印字
			lMargin *= m_lPrintCharWidth;
			lMargin += pInfo->m_rectDraw.left;
			pDC->TextOut(lMargin,pInfo->m_rectDraw.top,objHeader);
		}
		else if(m_nHeaderLayout==PRINT_HEADER_RIGHT) //ヘッダ右よせ
		{
			//ヘッダの文字数
			lHeadLen = objHeader.GetLength();
			//マージン
			lMargin = lCharCnt - lHeadLen;
			if(lMargin<0)
				lMargin = 0;
			//ヘッダ右よせ印字
			lMargin *= m_lPrintCharWidth;
			lMargin += pInfo->m_rectDraw.left;
			pDC->TextOut(lMargin,pInfo->m_rectDraw.top,objHeader);
		}
	}

	/////////////////////////////////////////
	//	ファイルの内容１ページ分出力
	/////////////////////////////////////////
	MyPrintOut1Page(pDC,pInfo,m_lLinePerPrintPage,m_lPrintLineHeight,m_lPrintCharWidth);

	//////////////////////////////////////
	//	ページフッタ印字
	//////////////////////////////////////
	CString	objFooter = _T("");
	ParsePrintFooterFormat(m_objStrFooterFormat,objFooter,pInfo);
	if(!objFooter.IsEmpty())
	{
		if(m_nFooterLayout==PRINT_FOOTER_LEFT) //フッタ左よせ
		{
			//フッタ左よせ印字
			pDC->TextOut( pInfo->m_rectDraw.left,
						  ((m_lLinePerPrintPage+4)*m_lPrintLineHeight) + pInfo->m_rectDraw.top,
						  objFooter);
		}
		else if(m_nFooterLayout==PRINT_FOOTER_CENTER) //フッタ中央
		{
			//フッタの文字数
			lFootLen = objFooter.GetLength();
			//マージン
			lMargin = (lCharCnt - lFootLen) / 2;
			if(lMargin<0)
				lMargin = 0;
			//フッタ中央印字
			lMargin *= m_lPrintCharWidth;
			lMargin += pInfo->m_rectDraw.left;
			pDC->TextOut( lMargin,
						  ((m_lLinePerPrintPage+4)*m_lPrintLineHeight) + pInfo->m_rectDraw.top,
						  objFooter);
		}
		else if(m_nFooterLayout==PRINT_FOOTER_RIGHT) //フッタ右よせ
		{
			//フッタの文字数
			lFootLen = objFooter.GetLength();
			//マージン
			lMargin = lCharCnt - lFootLen;
			if(lMargin<0)
				lMargin = 0;
			//フッタ右よせ印字
			lMargin *= m_lPrintCharWidth;
			lMargin += pInfo->m_rectDraw.left;
			pDC->TextOut( lMargin,
						  ((m_lLinePerPrintPage+4)*m_lPrintLineHeight) + pInfo->m_rectDraw.top,
						  objFooter);
		}
	}

	///////////////////////////////////
	//	フォントを戻す
	pDC->SelectObject(pOldFont);

//	CView::OnPrint(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView クラスの診断

#ifdef _DEBUG
void CEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEditorDoc* CEditorView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEditorDoc)));
	return (CEditorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（ドラッグ＆ドロップ関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ドラッグ＆ドロップ中、ウインドウに最初に移動した時の処理				|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
DROPEFFECT CEditorView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	LONG	lCurrentLine=0;
	LONG	lCurrentOffset=0;
	CPoint	tempPos(0,0);
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() ||
		pWnd->IsNowStepDoOperation() 
	)
	{
		//マウス操作を無視
		return CView::OnDragEnter(pDataObject, dwKeyState, point);
	}

	if( pDataObject->IsDataAvailable(CF_TEXT) ) //テキスト形式
	{
		//ドロップ先のビューをアクティブにする
		CSplitterWnd *spltWnd = (CSplitterWnd *)GetParent();
		spltWnd->SetActivePane(0, 0, this);
		((CMDIChildWnd *)spltWnd->GetParent())->MDIActivate();

		//マウス位置　－＞　行、列番号変換
		lCurrentLine=0;
		lCurrentOffset=0;
		LONG lRet = MouseToLineOffset( point, &lCurrentLine, &lCurrentOffset, &tempPos );
		if(	lRet ==	0 || lRet == 2 )
		{
			//基底クラスのハンドラコール
			return CView::OnDragEnter(pDataObject, dwKeyState, point);
		}

		//キャレット位置を現在のマウス位置に設定
		MySetCaretPos(tempPos);

		//コントロールの状態により、コピーがどうかを通知
		return (dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
	}

	return CView::OnDragEnter(pDataObject, dwKeyState, point);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ドラッグ＆ドロップ中、ウインドウ外に移動した時の処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnDragLeave()
{
	//CF_TEXT では特にすることは無い
	CView::OnDragLeave();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ドラッグ＆ドロップ中、ウインドウの外にドラッグされた時の処理			|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
DROPEFFECT CEditorView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	LONG	lCurrentLine=0;
	LONG	lCurrentOffset=0;
	CPoint	tempPos(0,0);
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() ||
		pWnd->IsNowStepDoOperation() 
	)
	{
		//マウス操作を無視
		return CView::OnDragOver(pDataObject, dwKeyState, point);
	}

	if( pDataObject->IsDataAvailable(CF_TEXT) ) //テキスト形式
	{
		//ドロップ先のビューをアクティブにする
		CSplitterWnd *spltWnd = (CSplitterWnd *)GetParent();
		spltWnd->SetActivePane(0, 0, this);
		((CMDIChildWnd *)spltWnd->GetParent())->MDIActivate();

		//マウス位置　－＞　行、列番号変換
		lCurrentLine=0;
		lCurrentOffset=0;
		LONG lRet = MouseToLineOffset( point, &lCurrentLine, &lCurrentOffset, &tempPos );
		if(lRet	== 0 ||	lRet ==	2)
		{
			//基底クラスのハンドラコール
			return CView::OnDragOver(pDataObject, dwKeyState, point);
		}

		//キャレット位置を現在のマウス位置に設定
		MySetCaretPos(tempPos);

		//コントロールの状態により、コピーがどうかを通知
		return (dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
	}
	return CView::OnDragOver(pDataObject, dwKeyState, point);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ドロップ操作が行われる時の処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return CView::OnDrop(pDataObject, dropEffect, point);
	}

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() ||
		pWnd->IsNowStepDoOperation() 
	)
	{
		//マウス操作を無視
		MessageBeep((UINT)-1);
		return CView::OnDrop(pDataObject, dropEffect, point);
	}

	if( pDataObject->IsDataAvailable(CF_TEXT) ) //テキスト形式
	{
		//ドロップ先のビューをアクティブにする
		CSplitterWnd *spltWnd = (CSplitterWnd *)GetParent();
		spltWnd->SetActivePane(0, 0, this);
		((CMDIChildWnd *)spltWnd->GetParent())->MDIActivate();

		//	現在のキャレット位置の行、列番号取得
		if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
			return CView::OnDrop(pDataObject, dropEffect, point);

		//範囲選択位置調整
		AdjustSelectArea();

		//ドロップ処理
		CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
		pDoc->ProcessOleDrop( pDataObject,				// ＯＬＥデータ
							  lCurrentLine, 			// キャレット位置の行
							  lCurrentOffset,			// キャレット位置のオフセット
							  m_caretPos,				// キャレット位置
							  objScrollPosLog,			// スクロール位置
							  m_bDispRuler,				// ルーラ表示フラグ
							  m_bDispLineNo );			// 行番号表示フラグ

		//再描画
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);

		return dropEffect;
	}

	return CView::OnDrop(pDataObject, dropEffect, point);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（表示関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－リフレッシュ」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewRedraw()
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	CRect	clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	//再描画
	OnUpdate(NULL,NULL,NULL);
	InvalidateRect(clientRect,TRUE);
}
void CEditorView::OnUpdateViewRedraw(CCmdUI* pCmdUI)
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－行番号を表示」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewLineno()
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//フラグ反転
	m_bDispLineNo = (m_bDispLineNo!=FALSE) ? FALSE : TRUE;

	//拡張子別設定保存
	CString objKey;
	CString objFileExt = pDoc->MyGetFileExt();
	if(objFileExt.IsEmpty())
		objKey = "FileExt_none";
	if(!objFileExt.CompareNoCase("txt"))
		objKey = "FileExt_txt";
	else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
			!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
		objKey = "FileExt_htm html shtml shtm";
	else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
		objKey = "FileExt_c cpp";
	else if(!objFileExt.CompareNoCase("h"))
		objKey = "FileExt_h";
	else if(!objFileExt.CompareNoCase("asm"))
		objKey = "FileExt_asm";
	else if(!objFileExt.CompareNoCase("asp"))
		objKey = "FileExt_asp";
	else if(!objFileExt.CompareNoCase("frm"))
		objKey = "FileExt_frm";
	else if(!objFileExt.CompareNoCase("bas"))
		objKey = "FileExt_bas";
	else if(!objFileExt.CompareNoCase("pc"))
		objKey = "FileExt_pc";
	else if(!objFileExt.CompareNoCase("log"))
		objKey = "FileExt_log";
	else if(!objFileExt.CompareNoCase("dat"))
		objKey = "FileExt_dat";
	else if(!objFileExt.CompareNoCase("bat"))
		objKey = "FileExt_bat";
	else if(!objFileExt.CompareNoCase("ini"))
		objKey = "FileExt_ini";
	else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
		objKey = "FileExt_mak mk";
	else if(!objFileExt.CompareNoCase("java"))
		objKey = "FileExt_java";
	else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
		objKey = "FileExt_y l";
	else if(!objFileExt.CompareNoCase("wrl"))
		objKey = "FileExt_wrl";
	else
		objKey = "FileExt_other";
	m_pApp->WriteProfileInt(objKey,"bDispLineNo",m_bDispLineNo);
	m_pApp->SetFlag2AllWindow(objFileExt,
							  m_bDispLineNo,
							  m_bDispRuler,
							  m_bDispCRLF,
							  m_bDispTab,
							  m_bDispZenkakuSpace,
							  m_bDispUnderLine);

	if( m_bDispLineNo )
	{
		//キャレット位置
		m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		m_LineNoPos = CPoint(m_lLineNoWidth,0);
		CPoint objPoint = m_caretPos + m_LineNoPos;
		MySetCaretPos(objPoint);
	}
	else
	{
		//キャレット位置
		CPoint objPoint = m_caretPos - m_LineNoPos;
		m_lLineNoWidth = 0;
		m_LineNoPos = CPoint(0,0);
		MySetCaretPos(objPoint);
	}

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateViewLineno(CCmdUI* pCmdUI)
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_bDispLineNo)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－ルーラを表示」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewRuler()
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//フラグ反転
	m_bDispRuler = (m_bDispRuler!=FALSE) ? FALSE : TRUE;

	//拡張子別設定保存
	CString objKey;
	CString objFileExt = pDoc->MyGetFileExt();
	if(objFileExt.IsEmpty())
		objKey = "FileExt_none";
	if(!objFileExt.CompareNoCase("txt"))
		objKey = "FileExt_txt";
	else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
			!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
		objKey = "FileExt_htm html shtml shtm";
	else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
		objKey = "FileExt_c cpp";
	else if(!objFileExt.CompareNoCase("h"))
		objKey = "FileExt_h";
	else if(!objFileExt.CompareNoCase("asm"))
		objKey = "FileExt_asm";
	else if(!objFileExt.CompareNoCase("asp"))
		objKey = "FileExt_asp";
	else if(!objFileExt.CompareNoCase("frm"))
		objKey = "FileExt_frm";
	else if(!objFileExt.CompareNoCase("bas"))
		objKey = "FileExt_bas";
	else if(!objFileExt.CompareNoCase("pc"))
		objKey = "FileExt_pc";
	else if(!objFileExt.CompareNoCase("log"))
		objKey = "FileExt_log";
	else if(!objFileExt.CompareNoCase("dat"))
		objKey = "FileExt_dat";
	else if(!objFileExt.CompareNoCase("bat"))
		objKey = "FileExt_bat";
	else if(!objFileExt.CompareNoCase("ini"))
		objKey = "FileExt_ini";
	else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
		objKey = "FileExt_mak mk";
	else if(!objFileExt.CompareNoCase("java"))
		objKey = "FileExt_java";
	else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
		objKey = "FileExt_y l";
	else if(!objFileExt.CompareNoCase("wrl"))
		objKey = "FileExt_wrl";
	else
		objKey = "FileExt_other";
	m_pApp->WriteProfileInt(objKey,"bDispRuler",m_bDispRuler);
	m_pApp->SetFlag2AllWindow(objFileExt,
							  m_bDispLineNo,
							  m_bDispRuler,
							  m_bDispCRLF,
							  m_bDispTab,
							  m_bDispZenkakuSpace,
							  m_bDispUnderLine);

	if( m_bDispRuler )
	{
		//キャレット位置
		m_RulerPos = CPoint(0,(m_lfHeight+m_lLineInterval));
		CPoint objPoint = m_caretPos + m_RulerPos;
		MySetCaretPos(objPoint);
	}
	else
	{
		//キャレット位置
		CPoint objPoint = m_caretPos - m_RulerPos;
		m_RulerPos = CPoint(0,0);
		MySetCaretPos(objPoint);
	}

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateViewRuler(CCmdUI* pCmdUI)
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_bDispRuler)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－改行マークを表示」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewCrlf()
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//フラグ反転
	m_bDispCRLF = (m_bDispCRLF!=FALSE) ? FALSE : TRUE;

	//拡張子別設定保存
	CString objKey;
	CString objFileExt = pDoc->MyGetFileExt();
	if(objFileExt.IsEmpty())
		objKey = "FileExt_none";
	if(!objFileExt.CompareNoCase("txt"))
		objKey = "FileExt_txt";
	else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
			!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
		objKey = "FileExt_htm html shtml shtm";
	else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
		objKey = "FileExt_c cpp";
	else if(!objFileExt.CompareNoCase("h"))
		objKey = "FileExt_h";
	else if(!objFileExt.CompareNoCase("asm"))
		objKey = "FileExt_asm";
	else if(!objFileExt.CompareNoCase("asp"))
		objKey = "FileExt_asp";
	else if(!objFileExt.CompareNoCase("frm"))
		objKey = "FileExt_frm";
	else if(!objFileExt.CompareNoCase("bas"))
		objKey = "FileExt_bas";
	else if(!objFileExt.CompareNoCase("pc"))
		objKey = "FileExt_pc";
	else if(!objFileExt.CompareNoCase("log"))
		objKey = "FileExt_log";
	else if(!objFileExt.CompareNoCase("dat"))
		objKey = "FileExt_dat";
	else if(!objFileExt.CompareNoCase("bat"))
		objKey = "FileExt_bat";
	else if(!objFileExt.CompareNoCase("ini"))
		objKey = "FileExt_ini";
	else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
		objKey = "FileExt_mak mk";
	else if(!objFileExt.CompareNoCase("java"))
		objKey = "FileExt_java";
	else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
		objKey = "FileExt_y l";
	else if(!objFileExt.CompareNoCase("wrl"))
		objKey = "FileExt_wrl";
	else
		objKey = "FileExt_other";
	m_pApp->WriteProfileInt(objKey,"bDispCRLF",m_bDispCRLF);
	m_pApp->SetFlag2AllWindow(objFileExt,
							  m_bDispLineNo,
							  m_bDispRuler,
							  m_bDispCRLF,
							  m_bDispTab,
							  m_bDispZenkakuSpace,
							  m_bDispUnderLine);

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateViewCrlf(CCmdUI* pCmdUI)
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_bDispCRLF)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－タブ記号を表示」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewTab()
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//フラグ反転
	m_bDispTab = (m_bDispTab!=FALSE) ? FALSE : TRUE;

		//拡張子別設定保存
	CString objKey;
	CString objFileExt = pDoc->MyGetFileExt();
	if(objFileExt.IsEmpty())
		objKey = "FileExt_none";
	if(!objFileExt.CompareNoCase("txt"))
		objKey = "FileExt_txt";
	else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
			!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
		objKey = "FileExt_htm html shtml shtm";
	else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
		objKey = "FileExt_c cpp";
	else if(!objFileExt.CompareNoCase("h"))
		objKey = "FileExt_h";
	else if(!objFileExt.CompareNoCase("asm"))
		objKey = "FileExt_asm";
	else if(!objFileExt.CompareNoCase("asp"))
		objKey = "FileExt_asp";
	else if(!objFileExt.CompareNoCase("frm"))
		objKey = "FileExt_frm";
	else if(!objFileExt.CompareNoCase("bas"))
		objKey = "FileExt_bas";
	else if(!objFileExt.CompareNoCase("pc"))
		objKey = "FileExt_pc";
	else if(!objFileExt.CompareNoCase("log"))
		objKey = "FileExt_log";
	else if(!objFileExt.CompareNoCase("dat"))
		objKey = "FileExt_dat";
	else if(!objFileExt.CompareNoCase("bat"))
		objKey = "FileExt_bat";
	else if(!objFileExt.CompareNoCase("ini"))
		objKey = "FileExt_ini";
	else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
		objKey = "FileExt_mak mk";
	else if(!objFileExt.CompareNoCase("java"))
		objKey = "FileExt_java";
	else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
		objKey = "FileExt_y l";
	else if(!objFileExt.CompareNoCase("wrl"))
		objKey = "FileExt_wrl";
	else
		objKey = "FileExt_other";
	m_pApp->WriteProfileInt(objKey,"bDispTab",m_bDispTab);
	m_pApp->SetFlag2AllWindow(objFileExt,
							  m_bDispLineNo,
							  m_bDispRuler,
							  m_bDispCRLF,
							  m_bDispTab,
							  m_bDispZenkakuSpace,
							  m_bDispUnderLine);

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateViewTab(CCmdUI* pCmdUI)
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_bDispTab)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－全角スペースを表示」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewZenkakuspace()
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//フラグ反転
	m_bDispZenkakuSpace = (m_bDispZenkakuSpace!=FALSE) ? FALSE : TRUE;

		//拡張子別設定保存
	CString objKey;
	CString objFileExt = pDoc->MyGetFileExt();
	if(objFileExt.IsEmpty())
		objKey = "FileExt_none";
	if(!objFileExt.CompareNoCase("txt"))
		objKey = "FileExt_txt";
	else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
			!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
		objKey = "FileExt_htm html shtml shtm";
	else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
		objKey = "FileExt_c cpp";
	else if(!objFileExt.CompareNoCase("h"))
		objKey = "FileExt_h";
	else if(!objFileExt.CompareNoCase("asm"))
		objKey = "FileExt_asm";
	else if(!objFileExt.CompareNoCase("asp"))
		objKey = "FileExt_asp";
	else if(!objFileExt.CompareNoCase("frm"))
		objKey = "FileExt_frm";
	else if(!objFileExt.CompareNoCase("bas"))
		objKey = "FileExt_bas";
	else if(!objFileExt.CompareNoCase("pc"))
		objKey = "FileExt_pc";
	else if(!objFileExt.CompareNoCase("log"))
		objKey = "FileExt_log";
	else if(!objFileExt.CompareNoCase("dat"))
		objKey = "FileExt_dat";
	else if(!objFileExt.CompareNoCase("bat"))
		objKey = "FileExt_bat";
	else if(!objFileExt.CompareNoCase("ini"))
		objKey = "FileExt_ini";
	else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
		objKey = "FileExt_mak mk";
	else if(!objFileExt.CompareNoCase("java"))
		objKey = "FileExt_java";
	else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
		objKey = "FileExt_y l";
	else if(!objFileExt.CompareNoCase("wrl"))
		objKey = "FileExt_wrl";
	else
		objKey = "FileExt_other";
	m_pApp->WriteProfileInt(objKey,"bDispZenkakuSpace",m_bDispZenkakuSpace);
	m_pApp->SetFlag2AllWindow(objFileExt,
							  m_bDispLineNo,
							  m_bDispRuler,
							  m_bDispCRLF,
							  m_bDispTab,
							  m_bDispZenkakuSpace,
							  m_bDispUnderLine);

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateViewZenkakuspace(CCmdUI* pCmdUI)
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_bDispZenkakuSpace)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－アンダーラインを表示」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewUnderline()
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//フラグ反転
	m_bDispUnderLine = (m_bDispUnderLine!=FALSE) ? FALSE : TRUE;

	//拡張子別設定保存
	CString objKey;
	CString objFileExt = pDoc->MyGetFileExt();
	if(objFileExt.IsEmpty())
		objKey = "FileExt_none";
	if(!objFileExt.CompareNoCase("txt"))
		objKey = "FileExt_txt";
	else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
			!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
		objKey = "FileExt_htm html shtml shtm";
	else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
		objKey = "FileExt_c cpp";
	else if(!objFileExt.CompareNoCase("h"))
		objKey = "FileExt_h";
	else if(!objFileExt.CompareNoCase("asm"))
		objKey = "FileExt_asm";
	else if(!objFileExt.CompareNoCase("asp"))
		objKey = "FileExt_asp";
	else if(!objFileExt.CompareNoCase("frm"))
		objKey = "FileExt_frm";
	else if(!objFileExt.CompareNoCase("bas"))
		objKey = "FileExt_bas";
	else if(!objFileExt.CompareNoCase("pc"))
		objKey = "FileExt_pc";
	else if(!objFileExt.CompareNoCase("log"))
		objKey = "FileExt_log";
	else if(!objFileExt.CompareNoCase("dat"))
		objKey = "FileExt_dat";
	else if(!objFileExt.CompareNoCase("bat"))
		objKey = "FileExt_bat";
	else if(!objFileExt.CompareNoCase("ini"))
		objKey = "FileExt_ini";
	else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
		objKey = "FileExt_mak mk";
	else if(!objFileExt.CompareNoCase("java"))
		objKey = "FileExt_java";
	else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
		objKey = "FileExt_y l";
	else if(!objFileExt.CompareNoCase("wrl"))
		objKey = "FileExt_wrl";
	else
		objKey = "FileExt_other";
	m_pApp->WriteProfileInt(objKey,"bDispUnderLine",m_bDispUnderLine);
	m_pApp->SetFlag2AllWindow(objFileExt,
							  m_bDispLineNo,
							  m_bDispRuler,
							  m_bDispCRLF,
							  m_bDispTab,
							  m_bDispZenkakuSpace,
							  m_bDispUnderLine);

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateViewUnderline(CCmdUI* pCmdUI)
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_bDispUnderLine)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－ＥＯＦマークを表示」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewEof()
{
	BOOL bFlg = m_pApp->GetProfileInt("CEditorView","bIsDispEOFMark",1);
	bFlg = (bFlg==1) ? 0 : 1;
	m_pApp->WriteProfileInt("CEditorView","bIsDispEOFMark",bFlg);

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateViewEof(CCmdUI* pCmdUI)
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	BOOL bFlg = m_pApp->GetProfileInt("CEditorView","bIsDispEOFMark",1);
	if(bFlg == 1)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－キャレット形状：縦棒」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewCaretIbeam()
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(m_nCaretType==CARET_IBEAM) //すでに縦棒になっている
		return;
	if(!m_bInsertKey)
		return;

	m_nCaretType = CARET_IBEAM;

	HideCaret();								//キャレット非表示
	CreateSolidCaret(m_lfWidth/4, m_lfHeight); 	//縦棒キャレット作成
	ShowCaret();								//キャレット表示
	MySetCaretPos(m_caretPos);					//キャレット位置設定
}
void CEditorView::OnUpdateViewCaretIbeam(CCmdUI* pCmdUI)
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_nCaretType==CARET_IBEAM)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－キャレット形状：箱型」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewCaretSquare()
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(m_nCaretType==CARET_SQUARE) //すでに箱型になっている
		return;
	if(!m_bInsertKey)
		return;

	m_nCaretType = CARET_SQUARE;

	HideCaret();								//キャレット非表示
	CreateSolidCaret(m_lfWidth, m_lfHeight/2); 	//箱型キャレット作成
	ShowCaret();								//キャレット表示
	MySetCaretPos(m_caretPos);					//キャレット位置設定
}
void CEditorView::OnUpdateViewCaretSquare(CCmdUI* pCmdUI)
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_nCaretType==CARET_SQUARE)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（コピー、切り取り、貼り付け関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－コピー」選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditCopy()
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITCOPY);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//ＯＬＥデータソース作成
	COleDataSource*	pobjOleDataSource = new COleDataSource();

	//コピーするデータ作成
	AdjustSelectArea();
	CSharedFile objShareFile(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
	if(m_lSelectStartLine == m_lSelectEndLine) //１行
	{
		CLine* pobjLine = (CLine*)pDoc->GetOneLine(m_lSelectStartLine);
		objShareFile.Write( pobjLine->Data(m_lSelectStartOffset),
							(m_lSelectEndOffset-m_lSelectStartOffset) );
	}
	else //複数行
	{
		for(ULONG lLineNo=m_lSelectStartLine; lLineNo<=(ULONG)m_lSelectEndLine; lLineNo++)
		{
			CLine* pobjLine = (CLine*)pDoc->GetOneLine(lLineNo);
			if(!pobjLine)
				continue;
			if(lLineNo == (ULONG)m_lSelectStartLine)//選択開始行
			{
				objShareFile.Write(	pobjLine->Data(m_lSelectStartOffset),
									(pobjLine->GetLength()-m_lSelectStartOffset) );
			}
			else if(lLineNo == (ULONG)m_lSelectEndLine)//選択終了行
			{
				objShareFile.Write( "\r\n", 2 );
				objShareFile.Write( pobjLine->String(), m_lSelectEndOffset );
			}
			else //途中行
			{
				objShareFile.Write( "\r\n", 2 );
				objShareFile.Write( pobjLine->String(), pobjLine->GetLength() );
			}
		}
	}

	//クリップボードへ
	pobjOleDataSource->CacheGlobalData(CF_TEXT, objShareFile.Detach());
	pobjOleDataSource->SetClipboard();
}
void CEditorView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－切り取り」選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditCut()
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITCUT);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//範囲選択位置調整
	AdjustSelectArea();
	//選択範囲テキスト削除
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->DeleteSelectedArea(m_lSelectStartLine,		//選択開始行
							 m_lSelectStartOffset,		//選択開始オフセット
							 m_lSelectEndLine,			//選択終了行
							 m_lSelectEndOffset,		//選択終了オフセット
							 TRUE,						//クリップボードへコピーする
							 m_caretPos,				//キャレット位置
							 objScrollPosLog,			//スクロール位置
							 m_bDispRuler,
							 m_bDispLineNo );

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－貼り付け」選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditPaste()
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITPASTE);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//範囲選択位置調整
	AdjustSelectArea();

	//張り付け処理
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessPaste(	lCurrentLine,			//キャレット位置の行
						lCurrentOffset,			//キャレット位置のオフセット
						m_lSelectStartLine,		//選択開始行
						m_lSelectStartOffset,	//選択開始オフセット
						m_lSelectEndLine,		//選択終了行
						m_lSelectEndOffset,		//選択終了オフセット
						m_caretPos,				//キャレット位置
						objScrollPosLog,		//スクロール位置
						m_bDispRuler,
						m_bDispLineNo );

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

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
/*|	概要	：「編集－削除」選択時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditDel()
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITDEL);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//範囲選択位置調整
	//選択範囲テキスト削除
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->DeleteSelectedArea(m_lSelectStartLine,
							 m_lSelectStartOffset,
							 m_lSelectEndLine,
							 m_lSelectEndOffset,
							 FALSE,
							 m_caretPos,
							 objScrollPosLog,
							 m_bDispRuler,
							 m_bDispLineNo );

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateEditDel(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－すべて選択」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditSelectall()
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITSELECTALL);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//最終行へジャンプ
	OnEditJump2bottom();

	//選択開始位置と選択終了位置を設定する
	m_lSelectStartLine = 0;
	m_lSelectStartOffset = 0;
	m_lSelectEndLine = pDoc->GetLineCount() - 1;
	CLine* pobjTempLine = pDoc->GetOneLine( m_lSelectEndLine );
	if( pobjTempLine != NULL )
		m_lSelectEndOffset = pobjTempLine->GetLength();
	else
		m_lSelectEndOffset = 0;

	//キャレット位置設定
	MySetCaretToLineOffset(m_lSelectEndLine,m_lSelectEndOffset);

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateEditSelectall(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ドキュメントに１行もデータがない場合は選択不可にする
	if( pDoc->IsEmpty() )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－ＵＲＬをブラウズ」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditBrowseurl() 
{
	CString objMsg;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//１行取得
	CLine* pobjLine = (CLine*)pDoc->GetOneLine(lCurrentLine);
	if(pobjLine == NULL)
		return;

	//キャレット位置のＵＲＬ取得
	CString objUrl=_T("");
	if(pobjLine->GetUrl(lCurrentOffset,objUrl) == FALSE)
	{
		objMsg.LoadString(IDS_NOTABOVEURL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//ＵＲＬをＷＥＢブラウザで表示
	m_pApp->BrowseUrl(objUrl);
}
void CEditorView::OnUpdateEditBrowseurl(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ドキュメントに１行もデータがない場合は選択不可にする
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ＵＲＬ上チェック
	if(pDoc->IsAboveURL(lCurrentLine,lCurrentOffset))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－メーラーを起動」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditExecmailer() 
{
	CString objMsg;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//１行取得
	CLine* pobjLine = (CLine*)pDoc->GetOneLine(lCurrentLine);
	if(pobjLine == NULL)
		return;

	//キャレット位置のｅ－ｍａｉｌ取得
	CString objEMail=_T("");
	if(pobjLine->GetEMail(lCurrentOffset,objEMail) == FALSE)
	{
		objMsg.LoadString(IDS_NOTABOVEEMAIL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//メーラーを起動
	m_pApp->ExecMailer(objEMail);
}
void CEditorView::OnUpdateEditExecmailer(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ドキュメントに１行もデータがない場合は選択不可にする
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ｅ－ｍａｉｌ上チェック
	if(pDoc->IsAboveEMail(lCurrentLine,lCurrentOffset))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（日付時刻挿入関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　日付挿入」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditInsday()
{
	CString objMsg;

	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITINSDAY);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//日付表示スタイル取得
	long lStatusDayStyle = pWnd->GetStatusDayStyle();
	CTime	objTimeNow = CTime::GetCurrentTime();
	CString	objDayStr=_T("");
	if(lStatusDayStyle==STS_DAYSTYLE_NONE)
	{
		objMsg.LoadString(IDS_DAYSTYLE_NONE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	else if(lStatusDayStyle==STS_DAYSTYLE_YYYYMMDD)
		objDayStr = objTimeNow.Format( "%Y/%m/%d (%a)" );
	else if(lStatusDayStyle==STS_DAYSTYLE_YYMMDD)
		objDayStr = objTimeNow.Format( "'%y/%m/%d (%a)" );
	else if(lStatusDayStyle==STS_DAYSTYLE_MMDDYYYY)
		objDayStr = objTimeNow.Format( "%m/%d/%Y (%a)" );
	else if(lStatusDayStyle==STS_DAYSTYLE_MMDDYY)
		objDayStr = objTimeNow.Format( "%m/%d/'%y (%a)" );
	else if(lStatusDayStyle==STS_DAYSTYLE_MMDD)
		objDayStr = objTimeNow.Format( "%m/%d (%a)" );
	else if(lStatusDayStyle==STS_DAYSTYLE_DDMMYYYY)
		objDayStr = objTimeNow.Format( "%d/%m/%Y (%a)" );
	else if(lStatusDayStyle==STS_DAYSTYLE_DDMMYY)
		objDayStr = objTimeNow.Format( "%d/%m/'%y (%a)" );
	else if(lStatusDayStyle==STS_DAYSTYLE_DDMM)
		objDayStr = objTimeNow.Format( "%d/%m (%a)" );
	else if(lStatusDayStyle==STS_DAYSTYLE_YYYY_MM_DD)
	{
		objMsg.LoadString(IDS_DAYSTYLE_YYYY_MM_DD);
		objDayStr = objTimeNow.Format( (LPCTSTR)objMsg );
	}
	else if(lStatusDayStyle==STS_DAYSTYLE_YY_MM_DD)
	{
		objMsg.LoadString(IDS_DAYSTYLE_YY_MM_DD);
		objDayStr = objTimeNow.Format( (LPCTSTR)objMsg );
	}
	else if(lStatusDayStyle==STS_DAYSTYLE_MM_DD)
	{
		objMsg.LoadString(IDS_DAYSTYLE_MM_DD);
		objDayStr = objTimeNow.Format( (LPCTSTR)objMsg );
	}
	//日付をキャレット位置へ挿入
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessInsDay( lCurrentLine,			//キャレット位置行
						 lCurrentOffset,		//キャレット位置オフセット
						 objDayStr,				//日付文字列
						 m_caretPos,			//キャレット位置
						 objScrollPosLog,		//スクロール位置
						 m_bDispRuler,			//ルーラ表示フラグ
						 m_bDispLineNo );		//行番号表示フラグ

	//キャレット行のみ再描画
	OnUpdate(NULL,NULL,NULL);
	InvalidateCaretLine();
}
void CEditorView::OnUpdateEditInsday(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　時刻挿入」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditInstime()
{
	CString objMsg;

	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITINSTIME);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//時刻表示スタイル取得
	long lStatusHourStyle = pWnd->GetStatusHourStyle();
	CTime	objTimeNow = CTime::GetCurrentTime();
	CString objTimeStr=_T("");
	if(lStatusHourStyle==STS_HOURSTYLE_NONE)
	{
		objMsg.LoadString(IDS_HOURSTYLE_NONE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	else if(lStatusHourStyle==STS_HOURSTYLE_HHMM)
		objTimeStr = objTimeNow.Format( "%H:%M" );
	else if(lStatusHourStyle==STS_HOURSTYLE_AMPM_HHMM)
		objTimeStr = objTimeNow.Format( "%p %I:%M" );
	else if(lStatusHourStyle==STS_HOURSTYLE_HH_MM)
		objTimeStr = objTimeNow.Format( "%H時%M分" );
	else if(lStatusHourStyle==STS_HOURSTYLE_AMPM_HH_MM)
		objTimeStr = objTimeNow.Format( "%p %I時%M分" );

	//時刻をキャレット位置へ挿入
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessInsTime(lCurrentLine,			//キャレット位置行
						 lCurrentOffset,		//キャレット位置オフセット
						 objTimeStr,			//時刻文字列
						 m_caretPos,			//キャレット位置
						 objScrollPosLog,		//スクロール位置
						 m_bDispRuler,			//ルーラ表示フラグ
						 m_bDispLineNo );		//行番号表示フラグ

	//キャレット行のみ再描画
	OnUpdate(NULL,NULL,NULL);
	InvalidateCaretLine();
}
void CEditorView::OnUpdateEditInstime(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　挿入／上書きモード切替え」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnChangeInsmode() 
{
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();

	m_bInsertKey = !m_bInsertKey;	//挿入／上書きモード切換え
	if(m_bInsertKey)
	{
		if(m_nCaretType==CARET_IBEAM)
			CreateSolidCaret(m_lfWidth/4, m_lfHeight); //挿入キャレット作成
		else if(m_nCaretType==CARET_SQUARE)
			CreateSolidCaret(m_lfWidth, m_lfHeight/2); //挿入キャレット作成
	}
	else
	{
		CreateSolidCaret(m_lfWidth, m_lfHeight); //上書きキャレット作成
	}
	ShowCaret();					//キャレット表示
	MySetCaretPos(m_caretPos);		//キャレット位置設定
	//ステータスバーペインに挿入、上書きﾓｰﾄﾞ設定
	pWnd->SetInsMode(m_bInsertKey);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（文字変換関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　タブ－＞半角スペース変換」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditTab2space()
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITTAB2SPACE);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//範囲選択位置調整
	AdjustSelectArea();
	//タブ－＞半角スペース変換
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessTab2Space( m_lSelectStartLine,		//選択開始行
							m_lSelectStartOffset,	//選択開始オフセット
							m_lSelectEndLine,		//選択終了行
							m_lSelectEndOffset,		//選択終了オフセット
							m_caretPos,				//キャレット位置
							objScrollPosLog,		//スクロール位置
							m_bDispRuler,			//ルーラ表示フラグ
							m_bDispLineNo,			//行番号表示フラグ
							m_lfWidth );			//フォント幅

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditTab2space(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　半角スペース－＞タブ変換」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditSpace2tab()
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITSPACE2TAB);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//範囲選択位置調整
	AdjustSelectArea();
	//半角スペース－＞タブ変換
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessSpace2Tab( m_lSelectStartLine,		//選択開始行
							m_lSelectStartOffset,	//選択開始オフセット
							m_lSelectEndLine,		//選択終了行
							m_lSelectEndOffset,		//選択終了オフセット
							m_caretPos,				//キャレット位置
							objScrollPosLog,		//スクロール位置
							m_bDispRuler,			//ルーラ表示フラグ
							m_bDispLineNo,			//行番号表示フラグ
							m_lfWidth );			//フォント幅

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditSpace2tab(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　全角スペース－＞半角スペース変換」選択時処理				|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditSpaceZen2han()
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITSPACEZEN2HAN);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//範囲選択位置調整
	AdjustSelectArea();
	//全角スペース－＞半角スペース変換
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessSpaceZen2Han( m_lSelectStartLine,	//選択開始行
							m_lSelectStartOffset,	//選択開始オフセット
							m_lSelectEndLine,		//選択終了行
							m_lSelectEndOffset,		//選択終了オフセット
							m_caretPos,				//キャレット位置
							objScrollPosLog,		//スクロール位置
							m_bDispRuler,			//ルーラ表示フラグ
							m_bDispLineNo,			//行番号表示フラグ
							m_lfWidth );			//フォント幅

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditSpaceZen2han(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　半角スペース－＞全角スペース変換」選択時処理				|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditSpaceHan2zen()
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITSPACEHAN2ZEN);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//範囲選択位置調整
	AdjustSelectArea();
	//半角スペース－＞全角スペース変換
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessSpaceHan2Zen( m_lSelectStartLine,	//選択開始行
							m_lSelectStartOffset,	//選択開始オフセット
							m_lSelectEndLine,		//選択終了行
							m_lSelectEndOffset,		//選択終了オフセット
							m_caretPos,				//キャレット位置
							objScrollPosLog,		//スクロール位置
							m_bDispRuler,			//ルーラ表示フラグ
							m_bDispLineNo,			//行番号表示フラグ
							m_lfWidth );			//フォント幅

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditSpaceHan2zen(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　半角ｶﾀｶﾅ－＞全角カタカナ変換」選択時処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditKatakanaHan2zen()
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITKATAKANAHAN2ZEN);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//範囲選択位置調整
	AdjustSelectArea();
	//半角ｶﾀｶﾅ－＞全角カタカナ変換
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessKatakanaHan2Zen( m_lSelectStartLine,	//選択開始行
							m_lSelectStartOffset,	//選択開始オフセット
							m_lSelectEndLine,		//選択終了行
							m_lSelectEndOffset,		//選択終了オフセット
							m_caretPos,				//キャレット位置
							objScrollPosLog,		//スクロール位置
							m_bDispRuler,			//ルーラ表示フラグ
							m_bDispLineNo,			//行番号表示フラグ
							m_lfWidth );			//フォント幅

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditKatakanaHan2zen(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　全角カタカナ－＞半角ｶﾀｶﾅ変換」選択時処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditKatakanaZen2han()
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITKATAKANAZEN2HAN);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//範囲選択位置調整
	AdjustSelectArea();
	//全角カタカナ－＞半角ｶﾀｶﾅ変換
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessKatakanaZen2Han( m_lSelectStartLine,	//選択開始行
							m_lSelectStartOffset,	//選択開始オフセット
							m_lSelectEndLine,		//選択終了行
							m_lSelectEndOffset,		//選択終了オフセット
							m_caretPos,				//キャレット位置
							objScrollPosLog,		//スクロール位置
							m_bDispRuler,			//ルーラ表示フラグ
							m_bDispLineNo,			//行番号表示フラグ
							m_lfWidth );			//フォント幅

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditKatakanaZen2han(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　半角英数字－＞全角英数字変換」選択時処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditAlnumHan2zen()
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITALNUMHAN2ZEN);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//範囲選択位置調整
	AdjustSelectArea();
	//半角英数字－＞全角英数字変換
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessAlnumHan2Zen( m_lSelectStartLine,	//選択開始行
							m_lSelectStartOffset,	//選択開始オフセット
							m_lSelectEndLine,		//選択終了行
							m_lSelectEndOffset,		//選択終了オフセット
							m_caretPos,				//キャレット位置
							objScrollPosLog,		//スクロール位置
							m_bDispRuler,			//ルーラ表示フラグ
							m_bDispLineNo,			//行番号表示フラグ
							m_lfWidth );			//フォント幅

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditAlnumHan2zen(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　全角英数字－＞半角英数字変換」選択時処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditAlnumZen2han()
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITALNUMZEN2HAN);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//範囲選択位置調整
	AdjustSelectArea();
	//全角英数字－＞半角英数字変換
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessAlnumZen2Han( m_lSelectStartLine,	//選択開始行
							m_lSelectStartOffset,	//選択開始オフセット
							m_lSelectEndLine,		//選択終了行
							m_lSelectEndOffset,		//選択終了オフセット
							m_caretPos,				//キャレット位置
							objScrollPosLog,		//スクロール位置
							m_bDispRuler,			//ルーラ表示フラグ
							m_bDispLineNo,			//行番号表示フラグ
							m_lfWidth );			//フォント幅

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditAlnumZen2han(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　英字－＞大文字」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditAl2upper()
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITAL2UPPER);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//範囲選択位置調整
	AdjustSelectArea();
	//英字－＞大文字変換
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessAl2Upper(  m_lSelectStartLine,	    //選択開始行
							m_lSelectStartOffset,	//選択開始オフセット
							m_lSelectEndLine,		//選択終了行
							m_lSelectEndOffset,		//選択終了オフセット
							m_caretPos,				//キャレット位置
							objScrollPosLog,		//スクロール位置
							m_bDispRuler,			//ルーラ表示フラグ
							m_bDispLineNo );		//行番号表示フラグ

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditAl2upper(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　英字－＞小文字」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditAl2lower()
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITAL2LOWER);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//範囲選択位置調整
	AdjustSelectArea();
	//英字－＞小文字変換
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessAl2Lower(  m_lSelectStartLine,	    //選択開始行
							m_lSelectStartOffset,	//選択開始オフセット
							m_lSelectEndLine,		//選択終了行
							m_lSelectEndOffset,		//選択終了オフセット
							m_caretPos,				//キャレット位置
							objScrollPosLog,		//スクロール位置
							m_bDispRuler,			//ルーラ表示フラグ
							m_bDispLineNo );		//行番号表示フラグ

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditAl2lower(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　先頭のスペース、タブ削除」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditDeltopspaces()
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITDELTOPSPACES);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//範囲選択位置調整
	AdjustSelectArea();
	//先頭のスペース、タブ削除
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessDelTopSpaces( m_lSelectStartLine,
							  m_lSelectStartOffset,
							  m_lSelectEndLine,
							  m_lSelectEndOffset,
							  m_caretPos,
							  objScrollPosLog,
							  m_bDispRuler,
							  m_bDispLineNo );

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditDeltopspaces(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　タブ挿入」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditInstab()
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITINSTAB);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//範囲選択位置調整
	//範囲選択している状態でのＴＡＢキー操作
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessTabWhenSelected( m_lSelectStartLine,
								  m_lSelectStartOffset,
								  m_lSelectEndLine,
								  m_lSelectEndOffset,
								  m_caretPos,
								  objScrollPosLog,
								  m_bDispRuler,
								  m_bDispLineNo );

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditInstab(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　タブ削除」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditDeltab()
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITDELTAB);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//範囲選択位置調整
	//範囲選択している状態でのＳｈｉｆｔ＋ＴＡＢキー操作
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessShiftTabWhenSelected( m_lSelectStartLine,
									  m_lSelectStartOffset,
									  m_lSelectEndLine,
									  m_lSelectEndOffset,
									  m_caretPos,
									  objScrollPosLog,
									  m_bDispRuler,
									  m_bDispLineNo );

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditDeltab(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　Ｃ／Ｃ＋＋行コメント挿入」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditInscppcomment()
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITINSCPPCOMMENT);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//範囲選択位置調整
	//範囲選択している状態でのＣ／Ｃ＋＋コメント挿入
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessInsCppComment( m_lSelectStartLine,
								m_lSelectStartOffset,
								m_lSelectEndLine,
								m_lSelectEndOffset,
								m_caretPos,
								objScrollPosLog,
								m_bDispRuler,
								m_bDispLineNo );

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditInscppcomment(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　Ｃ／Ｃ＋＋行コメント削除」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditDelcppcomment() 
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITDELCPPCOMMENT);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//範囲選択位置調整
	//範囲選択している状態でのＣ／Ｃ＋＋コメント削除
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessDelCppComment( m_lSelectStartLine,
								m_lSelectStartOffset,
								m_lSelectEndLine,
								m_lSelectEndOffset,
								m_caretPos,
								objScrollPosLog,
								m_bDispRuler,
								m_bDispLineNo );

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditDelcppcomment(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　ＶＢ行コメント挿入」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditInsvbcomment()
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITINSVBCOMMENT);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//範囲選択位置調整
	//範囲選択している状態でのＶＢコメント挿入
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessInsVbComment(  m_lSelectStartLine,
								m_lSelectStartOffset,
								m_lSelectEndLine,
								m_lSelectEndOffset,
								m_caretPos,
								objScrollPosLog,
								m_bDispRuler,
								m_bDispLineNo );

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditInsvbcomment(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　ＶＢ行コメント削除」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditDelvbcomment() 
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITDELVBCOMMENT);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//範囲選択位置調整
	//範囲選択している状態でのＶＢコメント削除
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessDelVbComment(  m_lSelectStartLine,
								m_lSelectStartOffset,
								m_lSelectEndLine,
								m_lSelectEndOffset,
								m_caretPos,
								objScrollPosLog,
								m_bDispRuler,
								m_bDispLineNo );

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditDelvbcomment(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　アセンブラ行コメント挿入」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditInsasmcomment()
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITINSASMCOMMENT);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//範囲選択位置調整
	//範囲選択している状態でのアセンブラコメント挿入
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessInsAsmComment( m_lSelectStartLine,
								m_lSelectStartOffset,
								m_lSelectEndLine,
								m_lSelectEndOffset,
								m_caretPos,
								objScrollPosLog,
								m_bDispRuler,
								m_bDispLineNo );

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditInsasmcomment(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　アセンブラ行コメント削除」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditDelasmcomment() 
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITDELASMCOMMENT);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//範囲選択位置調整
	//範囲選択している状態でのアセンブラコメント削除
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessDelAsmComment( m_lSelectStartLine,
								m_lSelectStartOffset,
								m_lSelectEndLine,
								m_lSelectEndOffset,
								m_caretPos,
								objScrollPosLog,
								m_bDispRuler,
								m_bDispLineNo );

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}

void CEditorView::OnUpdateEditDelasmcomment(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　引用符号挿入」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditInsquote() 
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITINSQUOTE);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//範囲選択位置調整
	//範囲選択している状態での引用符号挿入
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessInsQuote( m_lSelectStartLine,
						   m_lSelectStartOffset,
						   m_lSelectEndLine,
						   m_lSelectEndOffset,
						   m_caretPos,
						   objScrollPosLog,
						   m_bDispRuler,
						   m_bDispLineNo );

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditInsquote(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集　－　引用符号削除」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditDelquote() 
{
	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//メッセージポンプ
	m_pApp->MessagePump();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITDELQUOTE);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//範囲選択位置調整
	//範囲選択している状態での引用符号削除
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessDelQuote( m_lSelectStartLine,
						   m_lSelectStartOffset,
						   m_lSelectEndLine,
						   m_lSelectEndOffset,
						   m_caretPos,
						   objScrollPosLog,
						   m_bDispRuler,
						   m_bDispLineNo );

	//選択領域のみ再描画
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditDelquote(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//書込み禁止の場合は無条件にだめ
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//範囲選択されている場合にだけメニュー選択できるようにする
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（ジャンプ関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ジャンプ－先頭行へジャンプ」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditJump2top()
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITJUMP2TOP);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//選択開始位置と選択終了位置を設定する
	m_lSelectStartLine = 0;
	m_lSelectStartOffset = 0;
	m_lSelectEndLine = 0;
	CLine* pobjLine = pDoc->GetOneLine( m_lSelectEndLine );
	if( pobjLine != NULL )
		m_lSelectEndOffset = pobjLine->GetLength();
	else
		m_lSelectEndOffset = 0;

	m_bUseClipRgn = 0;
	m_bNoUseOffBuffer = 0;
	MyScrollToPosition(CPoint(0,0));
	MySetCaretPos(CPoint(m_LineNoPos.x,m_RulerPos.y));

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateEditJump2top(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ドキュメントに１行もデータがない場合は選択不可にする
	if( pDoc->IsEmpty() )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Ctrl+Shift+Homeキー（先頭行へ選択ジャンプ）押下時処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditJump2topselect() 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	long lCurrentLine = 0;
	long lCurrentOffset = 0;

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
		return;
	if(pDoc->IsEmpty())
		return;

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITJUMP2TOPSELECT);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//ジャンプ前キャレット位置を選択開始位置にする
	lCurrentLine=0;
	lCurrentOffset=0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	m_lSelectStartLine = lCurrentLine;
	m_lSelectStartOffset = lCurrentOffset;
	m_lSelectEndLine = 0;
	m_lSelectEndOffset = 0;

	//ジャンプ
	m_bUseClipRgn = 0;
	m_bNoUseOffBuffer = 0;
	MyScrollToPosition(CPoint(0,0));
	MySetCaretPos(CPoint(m_LineNoPos.x,m_RulerPos.y));

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ジャンプ－最終行へジャンプ」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditJump2bottom()
{
	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITJUMP2BOTTOM);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	CRect	clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();

	long lLineCnt = clientRect.Height() / (m_lfHeight+m_lLineInterval);	//クライアント領域に表示できる行数
	long lMaxLine = pDoc->GetLineCount();								//ドキュメントの総行数

	//選択開始位置と選択終了位置を設定する
	m_lSelectStartLine = lMaxLine-1;
	m_lSelectStartOffset = 0;
	m_lSelectEndLine = lMaxLine-1;
	CLine* pobjLine = pDoc->GetOneLine( m_lSelectEndLine );
	if( pobjLine != NULL )
		m_lSelectEndOffset = pobjLine->GetLength();
	else
		m_lSelectEndOffset = 0;

	m_bUseClipRgn = 0;
	m_bNoUseOffBuffer = 0;
	if( lLineCnt < lMaxLine )
	{
		long lY = (lMaxLine - lLineCnt) * (m_lfHeight+m_lLineInterval);
		MyScrollToPosition(CPoint(0,lY));//論理座標で指定
		long lcaretY = (lLineCnt-1)*(m_lfHeight+m_lLineInterval) + m_RulerPos.y;
		MySetCaretPos(CPoint(m_LineNoPos.x,lcaretY)); //デバイス座標で指定
	}
	else
	{
		MyScrollToPosition(CPoint(0,0));
		long lcaretY = (lMaxLine-1)*(m_lfHeight+m_lLineInterval) + m_RulerPos.y;
		MySetCaretPos(CPoint(m_LineNoPos.x,lcaretY)); //デバイス座標で指定
	}

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateEditJump2bottom(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ドキュメントに１行もデータがない場合は選択不可にする
	if( pDoc->IsEmpty() )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Ctrl+Shift+Endキー（最終行へ選択ジャンプ）押下時処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditJump2bottomselect() 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	long lCurrentLine = 0;
	long lCurrentOffset = 0;

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
		return;
	if(pDoc->IsEmpty())
		return;

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITJUMP2BOTTOMSELECT);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	CRect	clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();

	long lLineCnt = clientRect.Height() / (m_lfHeight+m_lLineInterval);	//クライアント領域に表示できる行数
	long lMaxLine = pDoc->GetLineCount();								//ドキュメントの総行数

	//ジャンプ前キャレット位置を選択開始位置にする
	lCurrentLine=0;
	lCurrentOffset=0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	m_lSelectStartLine = lCurrentLine;
	m_lSelectStartOffset = lCurrentOffset;

	//選択終了位置を設定する
	m_lSelectEndLine = lMaxLine-1;
	CLine* pobjLine = pDoc->GetOneLine( m_lSelectEndLine );
	if( pobjLine != NULL )
		m_lSelectEndOffset = pobjLine->GetLength();
	else
		m_lSelectEndOffset = 0;

	m_bUseClipRgn = 0;
	m_bNoUseOffBuffer = 0;
	if( lLineCnt < lMaxLine )
	{
		long lY = (lMaxLine - lLineCnt) * (m_lfHeight+m_lLineInterval);
		MyScrollToPosition(CPoint(0,lY));//論理座標で指定
		long lcaretY = (lLineCnt-1)*(m_lfHeight+m_lLineInterval) + m_RulerPos.y;
		MySetCaretPos(CPoint(m_LineNoPos.x,lcaretY)); //デバイス座標で指定
	}
	else
	{
		MyScrollToPosition(CPoint(0,0));
		long lcaretY = (lMaxLine-1)*(m_lfHeight+m_lLineInterval) + m_RulerPos.y;
		MySetCaretPos(CPoint(m_LineNoPos.x,lcaretY)); //デバイス座標で指定
	}

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ジャンプ－指定行へジャンプ」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditJump2line()
{
	CString objMsg;

	//印刷中チェック
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	CRect	clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();

	//クライアント領域に表示できる行数
	long lLineCnt = clientRect.Height() / (m_lfHeight+m_lLineInterval);
	//ドキュメントの総行数
	long lMaxLine = pDoc->GetLineCount();
	//キャレット位置行番号
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//ジャンプ先行番号入力ダイアログ表示
	CLineJumpDlg	objLineJumpDlg;
	objLineJumpDlg.m_lLineNo = lCurrentLine+1;
	objLineJumpDlg.m_lMaxNo = lMaxLine;
	if( objLineJumpDlg.DoModal() != IDOK )
		return;
	if( (objLineJumpDlg.m_lLineNo<=0) ||
		(objLineJumpDlg.m_lLineNo>(UINT)objLineJumpDlg.m_lMaxNo)
	)
	{
		MessageBeep((UINT)-1);
		objMsg.LoadString(IDS_OUTOFJUMPRANGE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//選択開始位置と選択終了位置を設定する
	m_lSelectStartLine = objLineJumpDlg.m_lLineNo-1;
	m_lSelectStartOffset = 0;
	m_lSelectEndLine = objLineJumpDlg.m_lLineNo-1;
	CLine* pobjLine = pDoc->GetOneLine( m_lSelectEndLine );
	if( pobjLine != NULL )
		m_lSelectEndOffset = pobjLine->GetLength();
	else
		m_lSelectEndOffset = 0;

	m_bUseClipRgn = 0;
	m_bNoUseOffBuffer = 0;

	long lDlgNo = objLineJumpDlg.m_lLineNo;
	if( lLineCnt < lMaxLine )
	{
		if( (lDlgNo + lLineCnt) <= lMaxLine )
		{
			long lY = (lDlgNo - 1) * (m_lfHeight+m_lLineInterval);
			MyScrollToPosition(CPoint(0,lY));	//論理座標で指定
			MySetCaretPos(CPoint(m_LineNoPos.x,m_RulerPos.y));	//デバイス座標で指定
		}
		else
		{
			long lY = (lMaxLine - lLineCnt) * (m_lfHeight+m_lLineInterval);
			MyScrollToPosition(CPoint(0,lY));	//論理座標で指定
			long lTopLine = lMaxLine - lLineCnt;
			long lcaretY = (lDlgNo - 1 - lTopLine) * (m_lfHeight+m_lLineInterval) + m_RulerPos.y;
			MySetCaretPos(CPoint(m_LineNoPos.x,lcaretY)); //デバイス座標で指定
		}
	}
	else
	{
		MyScrollToPosition(CPoint(0,0)); //論理座標で指定
		long lcaretY = (lDlgNo - 1) * (m_lfHeight+m_lLineInterval) + m_RulerPos.y;
		MySetCaretPos(CPoint(m_LineNoPos.x,lcaretY)); //デバイス座標で指定
	}

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateEditJump2line(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ドキュメントに１行もデータがない場合は選択不可にする
	if( pDoc->IsEmpty() )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ジャンプ－タグジャンプ」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditTagjump()
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CString objMsg;

	if( !pDoc->IsGrep() )
		return;

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	// キャレット位置の行取得
	CLine*	pobjLine = NULL;
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
		return;

	//ＧＲＥＰ結果解釈
	CString		objGrepFileName;
	long		lLineNo = 0;
	pobjLine->ParseGrepResult(objGrepFileName,&lLineNo);

	//ＧＲＥＰファイルオープン
	if( (_access( (LPCTSTR)objGrepFileName, 0 )) != -1 )
	{
		m_pApp->OpenDocumentFile((LPCTSTR)objGrepFileName);
		//指定行番号へのジャンプ指示
		m_pApp->SendJumpMsg((LPCTSTR)objGrepFileName,lLineNo);
	}
	else
	{
		char	szMsg[512];
		objMsg.LoadString(IDS_GREPFILE_NOTFOUND);
		sprintf(szMsg,(LPCTSTR)objMsg,(LPCTSTR)objGrepFileName);
		m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);
		objGrepFileName.ReleaseBuffer();
	}
}
void CEditorView::OnUpdateEditTagjump(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ＧＲＥＰ結果表示中でない
	if( !pDoc->IsGrep() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ドキュメントに１行もデータがない場合は選択不可にする
	if( pDoc->IsEmpty() )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ジャンプ－現在行をマーク」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnJumpSetmark()
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//現在行をマーク
	pDoc->SetFlagMark(lCurrentLine,TRUE);

	//再描画
	OnUpdate(NULL,NULL,NULL);
	InvalidateCaretLine();//キャレットのある１行を再描画
}
void CEditorView::OnUpdateJumpSetmark(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ドキュメントに１行もデータがない場合は選択不可にする
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	BOOL bMarked = pDoc->GetFlagMark(lCurrentLine);
	if(bMarked) //マーク済み
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ジャンプ－現在行のマーク解除」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnJumpDelmark()
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//現在行のマーク解除
	pDoc->SetFlagMark(lCurrentLine,FALSE);

	//再描画
	OnUpdate(NULL,NULL,NULL);
	InvalidateCaretLine();//キャレットのある１行を再描画
}
void CEditorView::OnUpdateJumpDelmark(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ドキュメントに１行もデータがない場合は選択不可にする
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	BOOL bMarked = pDoc->GetFlagMark(lCurrentLine);
	if(bMarked==FALSE) //マークしていない
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ジャンプ－すべてのマーク解除」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnJumpAlldelmark()
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	ULONG lLineCnt=0;

	for(lLineCnt=0; lLineCnt<(ULONG)pDoc->GetLineCount(); lLineCnt++)
	{
		if(pDoc->GetFlagMark(lLineCnt))
		{
			pDoc->SetFlagMark(lLineCnt,FALSE);
		}
	}

	//再描画
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateJumpAlldelmark(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ドキュメントに１行もデータがない場合は選択不可にする
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//マーク済み行数があるか
	if(pDoc->GetMarkLineCount() < 1)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ジャンプ－次のマークへジャンプ」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnJumpNextmark()
{
	CString objMsg;

	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	LONG	lLineCnt=0;
	long 	lMaxLine = pDoc->GetLineCount();

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONJUMPNEXTMARK);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	BOOL bFind=FALSE;
	for(lLineCnt=lCurrentLine+1; lLineCnt<pDoc->GetLineCount(); lLineCnt++)
	{
		if(pDoc->GetFlagMark(lLineCnt))
		{
			bFind=TRUE;
			break;
		}
	}
	if(bFind)
	{
		//次のマーク行へジャンプ
		Jump2LineOffset(lLineCnt,0);
		//再描画
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);
	}
	else
	{
		//マーク済みの行がもうない
		objMsg.LoadString(IDS_NOMARKLINE_NEXT);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
	}
}
void CEditorView::OnUpdateJumpNextmark(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ドキュメントに１行もデータがない場合は選択不可にする
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//マーク済み行数があるか
	if(pDoc->GetMarkLineCount() < 1)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ジャンプ－前のマークへジャンプ」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnJumpPrevmark()
{
	CString objMsg;
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONJUMPPREVMARK);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	LONG	lLineCnt=0;
	long 	lMaxLine = pDoc->GetLineCount();

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	BOOL bFind=FALSE;
	for(lLineCnt=lCurrentLine-1; lLineCnt>=0; lLineCnt--)
	{
		if(pDoc->GetFlagMark(lLineCnt))
		{
			bFind=TRUE;
			break;
		}
	}
	if(bFind)
	{
		//前のマーク行へジャンプ
		Jump2LineOffset(lLineCnt,0);
		//再描画
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);
	}
	else
	{
		//マーク済みの行がもうない
		objMsg.LoadString(IDS_NOMARKLINE_PREV);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
	}
}
void CEditorView::OnUpdateJumpPrevmark(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ドキュメントに１行もデータがない場合は選択不可にする
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//マーク済み行数があるか
	if(pDoc->GetMarkLineCount() < 1)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：マーク位置一覧表示処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnJumpMarklist()
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONJUMPMARKLIST);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//マーク位置一覧表示
	CMarkLineListDlg	objMarkLineListDlg;
	objMarkLineListDlg.Init(this,pDoc);
	int nRet = objMarkLineListDlg.DoModal();
	if(nRet == IDCANCEL)
		return;

	if(objMarkLineListDlg.m_lSelectedLineNo != 0)
	{
		//マーク行へジャンプ
		Jump2LineOffset(objMarkLineListDlg.m_lSelectedLineNo-1,0);
		//再描画
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);
	}
}
void CEditorView::OnUpdateJumpMarklist(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ドキュメントに１行もデータがない場合は選択不可にする
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//マーク済み行数があるか
	if(pDoc->GetMarkLineCount() < 1)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（ポップアップメニュー関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ボップアップメニュー表示ハンドラ										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnContextMenu(CWnd*, CPoint point)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (point.x == -1 && point.y == -1)
	{
		//キーストロークの発動
		CRect rect;
		GetClientRect(rect);
		rect.NormalizeRect();
		ClientToScreen(rect);
		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CMenu* pPopup = m_popupmenu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	//////////////////////////////////////////////
	//		メニュー項目選択可能／不可設定		//
	//////////////////////////////////////////////

	//「元に戻す」メニューの設定
	if( pDoc->IsCanUndo() )
		pPopup->EnableMenuItem( ID_EDIT_UNDO, (MF_BYCOMMAND|MF_ENABLED) );
	else
		pPopup->EnableMenuItem( ID_EDIT_UNDO, (MF_BYCOMMAND|MF_GRAYED) );

	//「繰り返す」メニューの設定
	if( pDoc->IsCanRedo() )
		pPopup->EnableMenuItem( ID_EDIT_REDO, (MF_BYCOMMAND|MF_ENABLED) );
	else
		pPopup->EnableMenuItem( ID_EDIT_REDO, (MF_BYCOMMAND|MF_GRAYED) );

	//「Undo/Redoバッファクリア」メニューの設定
	if( pDoc->IsCanUndo() || pDoc->IsCanRedo() )
		pPopup->EnableMenuItem( ID_EDIT_CLEARUNDOBUF, (MF_BYCOMMAND|MF_ENABLED) );
	else
		pPopup->EnableMenuItem( ID_EDIT_CLEARUNDOBUF, (MF_BYCOMMAND|MF_GRAYED) );

	//「切り取り」メニューの設定
	if( IsSelected() && !pDoc->IsReadOnly() )
		pPopup->EnableMenuItem( ID_EDIT_CUT, (MF_BYCOMMAND|MF_ENABLED) );
	else
		pPopup->EnableMenuItem( ID_EDIT_CUT, (MF_BYCOMMAND|MF_GRAYED) );

	//「コピー」メニューの設定
	if( IsSelected() )
		pPopup->EnableMenuItem( ID_EDIT_COPY, (MF_BYCOMMAND|MF_ENABLED) );
	else
		pPopup->EnableMenuItem( ID_EDIT_COPY, (MF_BYCOMMAND|MF_GRAYED) );

	//「貼り付け」メニューの設定
	if( pDoc->IsReadOnly() )
	{
		pPopup->EnableMenuItem( ID_EDIT_PASTE, (MF_BYCOMMAND|MF_GRAYED) );
	}
	else
	{
		COleDataObject	objOleDataObject;
		BOOL bRet = objOleDataObject.AttachClipboard();
		if( bRet != 0 )
		{
			bRet = objOleDataObject.IsDataAvailable(CF_TEXT);
			if( bRet != 0 )
				pPopup->EnableMenuItem( ID_EDIT_PASTE, (MF_BYCOMMAND|MF_ENABLED) );
			else
				pPopup->EnableMenuItem( ID_EDIT_PASTE, (MF_BYCOMMAND|MF_GRAYED) );
		}
		else
			pPopup->EnableMenuItem( ID_EDIT_PASTE, (MF_BYCOMMAND|MF_GRAYED) );
	}

	//「削除」メニューの設定
	if( IsSelected() && !pDoc->IsReadOnly() )
		pPopup->EnableMenuItem( ID_EDIT_DEL, (MF_BYCOMMAND|MF_ENABLED) );
	else
		pPopup->EnableMenuItem( ID_EDIT_DEL, (MF_BYCOMMAND|MF_GRAYED) );

	//「すべて選択」メニューの設定
	if( pDoc->IsEmpty() )
		pPopup->EnableMenuItem( ID_EDIT_SELECTALL, (MF_BYCOMMAND|MF_ENABLED) );
	else
		pPopup->EnableMenuItem( ID_EDIT_SELECTALL, (MF_BYCOMMAND|MF_GRAYED) );

	//「クリップボードの中身を見る」メニューの設定
	COleDataObject	objOleDataObject;
	BOOL bRet = objOleDataObject.AttachClipboard();
	if( bRet != 0 )
	{
		bRet = objOleDataObject.IsDataAvailable(CF_TEXT);
		if( bRet != 0 )
			pPopup->EnableMenuItem( ID_EDIT_CLIPVIEW, (MF_BYCOMMAND|MF_ENABLED) );
		else
			pPopup->EnableMenuItem( ID_EDIT_CLIPVIEW, (MF_BYCOMMAND|MF_GRAYED) );
	}
	else
		pPopup->EnableMenuItem( ID_EDIT_CLIPVIEW, (MF_BYCOMMAND|MF_GRAYED) );

	//「ＵＲＬをブラウズ」メニューの設定
	pPopup->EnableMenuItem( ID_EDIT_BROWSEURL, (MF_BYCOMMAND|MF_ENABLED) );

	if( !pDoc->IsGrep()   && !pDoc->IsDump()    && !pDoc->IsConv()     &&
		!pDoc->IsWCount() && !pDoc->IsFSearch() && !pDoc->IsReadOnly() && 
		!pDoc->IsBinary() && !pDoc->IsEncript() && !pDoc->IsDecript()  &&
		!pDoc->IsEncode() && !pDoc->IsCompare() && !pDoc->IsDirCmp()
	)
	{
		//「書込み禁止にする」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_SETREADONLY, (MF_BYCOMMAND|MF_ENABLED) );
		//「書込み禁止を解除する」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_UNSETREADONLY, (MF_BYCOMMAND|MF_ENABLED) );
	}
	else
	{
		//「書込み禁止にする」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_SETREADONLY, (MF_BYCOMMAND|MF_GRAYED) );
		//「書込み禁止を解除する」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_UNSETREADONLY, (MF_BYCOMMAND|MF_GRAYED) );
	}


	if( !pDoc->IsReadOnly() )
	{
		//「日付挿入」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_INSDAY, (MF_BYCOMMAND|MF_ENABLED) );
		//「時刻挿入」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_INSTIME, (MF_BYCOMMAND|MF_ENABLED) );
	}
	else
	{
		//「日付挿入」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_INSDAY, (MF_BYCOMMAND|MF_GRAYED) );
		//「時刻挿入」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_INSTIME, (MF_BYCOMMAND|MF_GRAYED) );
	}

	if( IsSelected() && !pDoc->IsReadOnly() )
	{
		//「タブ－＞半角スペース」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_TAB2SPACE, (MF_BYCOMMAND|MF_ENABLED) );
		//「半角スペース－＞タブ」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_SPACE2TAB, (MF_BYCOMMAND|MF_ENABLED) );
		//「全角スペース－＞半角スペース」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_SPACE_ZEN2HAN, (MF_BYCOMMAND|MF_ENABLED) );
		//「半角スペース－＞全角スペース」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_SPACE_HAN2ZEN, (MF_BYCOMMAND|MF_ENABLED) );
		//「半角ｶﾀｶﾅ－＞全角カタカナ」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_KATAKANA_HAN2ZEN, (MF_BYCOMMAND|MF_ENABLED) );
		//「全角カタカナ－＞半角ｶﾀｶﾅ」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_KATAKANA_ZEN2HAN, (MF_BYCOMMAND|MF_ENABLED) );
		//「半角英数字－＞全角英数字」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_ALNUM_HAN2ZEN, (MF_BYCOMMAND|MF_ENABLED) );
		//「全角英数字－＞半角英数字」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_ALNUM_ZEN2HAN, (MF_BYCOMMAND|MF_ENABLED) );
		//「英字－＞大文字」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_AL2UPPER, (MF_BYCOMMAND|MF_ENABLED) );
		//「英字－＞小文字」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_AL2LOWER, (MF_BYCOMMAND|MF_ENABLED) );
		//「先頭のスペース、タブ削除」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_DELTOPSPACES, (MF_BYCOMMAND|MF_ENABLED) );
		//「タブ挿入」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_INSTAB, (MF_BYCOMMAND|MF_ENABLED) );
		//「タブ削除」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_DELTAB, (MF_BYCOMMAND|MF_ENABLED) );
		//「Ｃ／Ｃ＋＋コメント挿入」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_INSCPPCOMMENT, (MF_BYCOMMAND|MF_ENABLED) );
		//「Ｃ／Ｃ＋＋コメント削除」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_DELCPPCOMMENT, (MF_BYCOMMAND|MF_ENABLED) );
		//「ＶＢコメント挿入」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_INSVBCOMMENT, (MF_BYCOMMAND|MF_ENABLED) );
		//「ＶＢコメント削除」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_DELVBCOMMENT, (MF_BYCOMMAND|MF_ENABLED) );
		//「アセンブラコメント挿入」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_INSASMCOMMENT, (MF_BYCOMMAND|MF_ENABLED) );
		//「アセンブラコメント削除」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_DELASMCOMMENT, (MF_BYCOMMAND|MF_ENABLED) );
		//「引用符号挿入」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_INSQUOTE, (MF_BYCOMMAND|MF_ENABLED) );
		//「引用符号削除」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_DELQUOTE, (MF_BYCOMMAND|MF_ENABLED) );
	}
	else
	{
		//「タブ－＞半角スペース」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_TAB2SPACE, (MF_BYCOMMAND|MF_GRAYED) );
		//「半角スペース－＞タブ」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_SPACE2TAB, (MF_BYCOMMAND|MF_GRAYED) );
		//「全角スペース－＞半角スペース」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_SPACE_ZEN2HAN, (MF_BYCOMMAND|MF_GRAYED) );
		//「半角スペース－＞全角スペース」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_SPACE_HAN2ZEN, (MF_BYCOMMAND|MF_GRAYED) );
		//「半角ｶﾀｶﾅ－＞全角カタカナ」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_KATAKANA_HAN2ZEN, (MF_BYCOMMAND|MF_GRAYED) );
		//「全角カタカナ－＞半角ｶﾀｶﾅ」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_KATAKANA_ZEN2HAN, (MF_BYCOMMAND|MF_GRAYED) );
		//「半角英数字－＞全角英数字」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_ALNUM_HAN2ZEN, (MF_BYCOMMAND|MF_GRAYED) );
		//「全角英数字－＞半角英数字」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_ALNUM_ZEN2HAN, (MF_BYCOMMAND|MF_GRAYED) );
		//「英字－＞大文字」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_AL2UPPER, (MF_BYCOMMAND|MF_GRAYED) );
		//「英字－＞小文字」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_AL2LOWER, (MF_BYCOMMAND|MF_GRAYED) );
		//「先頭のスペース、タブ削除」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_DELTOPSPACES, (MF_BYCOMMAND|MF_GRAYED) );
		//「タブ挿入」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_INSTAB, (MF_BYCOMMAND|MF_GRAYED) );
		//「タブ削除」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_DELTAB, (MF_BYCOMMAND|MF_GRAYED) );
		//「Ｃ／Ｃ＋＋コメント挿入」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_INSCPPCOMMENT, (MF_BYCOMMAND|MF_GRAYED) );
		//「Ｃ／Ｃ＋＋コメント削除」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_DELCPPCOMMENT, (MF_BYCOMMAND|MF_GRAYED) );
		//「ＶＢコメント挿入」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_INSVBCOMMENT, (MF_BYCOMMAND|MF_GRAYED) );
		//「ＶＢコメント削除」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_DELVBCOMMENT, (MF_BYCOMMAND|MF_GRAYED) );
		//「アセンブラコメント挿入」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_INSASMCOMMENT, (MF_BYCOMMAND|MF_GRAYED) );
		//「アセンブラコメント削除」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_DELASMCOMMENT, (MF_BYCOMMAND|MF_GRAYED) );
		//「引用符号挿入」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_INSQUOTE, (MF_BYCOMMAND|MF_GRAYED) );
		//「引用符号削除」メニューの設定
		pPopup->EnableMenuItem( ID_EDIT_DELQUOTE, (MF_BYCOMMAND|MF_GRAYED) );
	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（ウインドウ関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ﾋﾞｭｰがｱｸﾃｨﾌﾞ／非ｱｸﾃｨﾌﾞになるときによびだされる処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();

	if (bActivate) //非アクティブー＞アクティブ
	{
		if(m_bInsertKey)
		{
			if(m_nCaretType==CARET_IBEAM)
				CreateSolidCaret(m_lfWidth/4, m_lfHeight); //挿入キャレット作成
			else if(m_nCaretType==CARET_SQUARE)
				CreateSolidCaret(m_lfWidth, m_lfHeight/2); //挿入キャレット作成
		}
		else
		{
			CreateSolidCaret(m_lfWidth, m_lfHeight); //上書きキャレット作成
		}
		//キャレット表示
		ShowCaret();
		//キャレット点滅間隔を設定
		::SetCaretBlinkTime(m_unCaretBlinkTime);
		//キャレット位置設定
		MySetCaretPos(m_caretPos);
		//ステータスバーペイン設定
		pWnd->SetInsMode(m_bInsertKey);
		pWnd->SetReadOnlyMode(pDoc->IsReadOnly());
	}
	else //アクティブー＞非アクティブ
	{
		//キャレット点滅間隔をオリジナル値に戻す
		::SetCaretBlinkTime(m_pApp->m_unOrgCaretBlinkTime);
		//キャレット非表示
		HideCaret();
		//ステータスバーペインクリア
		pWnd->ClearGyoRetu();
		pWnd->ClearCaretPosByte();
		pWnd->ClearInsMode();
		pWnd->ClearReadOnlyMode();
		pWnd->ClearCaretPosCharCode();
	}

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウを作成する前に呼び出される処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。
	return CView::PreCreateWindow(cs);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウの作成を要求する時に呼び出される処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//ｵﾌｽｸﾘｰﾝﾊﾞｯﾌｧ作成
	CreateOffScreenBuf();

	//ビューウインドウをドロップターゲットとして登録
	if( m_objOleDropTarget.Register(this) )
		return 0;
	else
		return -1;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ウインドウが破棄される時に呼び出される処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnDestroy()
{
	CView::OnDestroy();

	//ｵﾌｽｸﾘｰﾝﾊﾞｯﾌｧ破棄
	DestroyOffScreenBuf();
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（更新関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：最初のﾋﾞｭｰがﾄﾞｷｭﾒﾝﾄにｱﾀｯﾁされた後に呼び出される処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnInitialUpdate()
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//表示フォント作成
	CClientDC	dc(this);
	m_objDispFont.DeleteObject();
	m_objDispFont.CreatePointFont(m_lDispFontPoint,m_objDispFontName,&dc);
	//下線付き表示フォント作成
	LOGFONT logfont;
	memset(&logfont,0,sizeof(LOGFONT));
	m_objDispFont.GetLogFont(&logfont);
	logfont.lfUnderline = TRUE;
	m_objDispFontUnderLine.DeleteObject();
	m_objDispFontUnderLine.CreateFontIndirect(&logfont);

	//文字の高さと幅計算
	CFont* oldFont = dc.SelectObject(&m_objDispFont);
	TEXTMETRIC	tm;
	dc.GetTextMetrics(&tm);
	m_lfHeight = tm.tmHeight + tm.tmExternalLeading;
	m_lfWidth  = tm.tmAveCharWidth + tm.tmOverhang;
	dc.SelectObject(oldFont);

	//ステータスバーペインに挿入、上書きﾓｰﾄﾞ設定
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	pWnd->SetInsMode(m_bInsertKey);

	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	// CG: 次の行は 「IME サポート」 コンポーネントにより追加されています。
	::ImmAssociateContext(m_hWnd, m_hImc);
	if(m_pApp->GetProfileInt("CEditorView","IsImmAutoOpen",FALSE))
	{
		::ImmSetOpenStatus(m_hImc,TRUE);
		this->PostMessage(WM_IME_NOTIFY,IMN_SETOPENSTATUS,0);
	}

	//範囲選択解除
	m_lSelectStartLine   = -1;
	m_lSelectStartOffset = -1;
	m_lSelectEndLine     = -1;
	m_lSelectEndOffset   = -1;

	//行番号表示関連初期化
	if( m_bDispLineNo )
	{
		m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		m_LineNoPos = CPoint(m_lLineNoWidth,0);
	}
	else
	{
		m_lLineNoWidth = 0;
		m_LineNoPos = CPoint(0,0);
	}

	//ルーラ表示関連初期化
	if( m_bDispRuler )
		m_RulerPos = CPoint(0,(m_lfHeight+m_lLineInterval));
	else
		m_RulerPos = CPoint(0,0);

	//キャレット位置初期設定
	m_caretPos = CPoint(m_LineNoPos.x,m_RulerPos.y);

	//基底クラスのハンドラコール
	CView::OnInitialUpdate();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ﾄﾞｷｭﾒﾝﾄがｱｯﾌﾟﾃﾞｰﾄされたときに呼び出される処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//////////////////////////////////////
	//		ｵﾌｽｸﾘｰﾝﾊﾞｯﾌｧ作成しなおし	//
	//////////////////////////////////////

	DestroyOffScreenBuf();
	CreateOffScreenBuf();

	//////////////////////////////////
	//		文字の高さと幅計算		//
	//////////////////////////////////

	CClientDC	dc(this);
	CFont* oldFont = dc.SelectObject(&m_objDispFont);
	TEXTMETRIC	tm;
	dc.GetTextMetrics(&tm);
	m_lfHeight = tm.tmHeight + tm.tmExternalLeading;
	m_lfWidth  = tm.tmAveCharWidth + tm.tmOverhang;
	dc.SelectObject(oldFont);
	if( (m_lfWidth==0) || (m_lfHeight==0) )
	{
		//ゼロスクロールサイズ設定
		MySetScrollSizes(MM_TEXT,		// ﾏｯﾋﾟﾝｸﾞﾓｰﾄﾞ
						CSize(0,0),		// ﾄｰﾀﾙｻｲｽﾞ
						CSize(0,0),		// 垂直ｽｸﾛｰﾙﾊﾞｰｼｬﾌﾄｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
						CSize(0,0),		// 垂直ｽｸﾛｰﾙﾊﾞｰ矢印ｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
						CSize(0,0),		// 水平ｽｸﾛｰﾙﾊﾞｰｼｬﾌﾄｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
						CSize(0,0)		// 水平ｽｸﾛｰﾙﾊﾞｰ矢印ｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
		);
		//基底クラスのハンドラコール
		CView::OnUpdate(pSender,lHint,pHint);
		return;
	}

	//行番号表示関連初期化
	if( m_bDispLineNo )
	{
		m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		m_LineNoPos = CPoint(m_lLineNoWidth,0);
	}
	else
	{
		m_lLineNoWidth = 0;
		m_LineNoPos = CPoint(0,0);
	}

	//ルーラ表示関連初期化
	if( m_bDispRuler )
		m_RulerPos = CPoint(0,(m_lfHeight+m_lLineInterval));
	else
		m_RulerPos = CPoint(0,0);

	//////////////////////////////////////
	//	クライアント領域の高さと幅		//
	//////////////////////////////////////

	CRect clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//////////////////////////////////////
	//	クライアント領域に表示できる	//
	//	行数と文字数を求める			//
	//////////////////////////////////////

	long lCanDispLineCnt = nHeight / (m_lfHeight+m_lLineInterval);	// クライアント領域に表示できる行数
	long lCanDispCharCnt = nWidth  / m_lfWidth;						// クライアント領域に表示できる文字数

	//////////////////////////////////////
	//		スクロールサイズ計算		//
	//////////////////////////////////////

	//トータルサイズ計算
	CSize sizeTotal(0,0);
	if( pDoc->GetMaxLineLength() > 0 )
	{
		sizeTotal.cx = (pDoc->GetMaxLineLength() + 1) * m_lfWidth;
		sizeTotal.cx += clientRect.Width()  % m_lfWidth;//あまりを加算
	}
	else
	{
		sizeTotal.cx = 0;
	}
	if( pDoc->GetLineCount() > 0 )
	{
		sizeTotal.cy = (pDoc->GetLineCount() + 1) * (m_lfHeight+m_lLineInterval);
		sizeTotal.cy += clientRect.Height() % (m_lfHeight+m_lLineInterval);	//あまりを加算
	}
	else
	{
		sizeTotal.cy = 0;
	}

	//ページサイズ計算
	CSize sizeVPage;						// 垂直ｽｸﾛｰﾙﾊﾞｰｼｬﾌﾄｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
	CSize sizeHPage;						// 水平ｽｸﾛｰﾙﾊﾞｰｼｬﾌﾄｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
	if( pDoc->GetLineCount() > 10 )
		sizeVPage = CSize(0,((m_lfHeight+m_lLineInterval)*10));
	else
		sizeVPage = CSize(0,((m_lfHeight+m_lLineInterval)*pDoc->GetLineCount()));
	if( pDoc->GetMaxLineLength() > 5 )
		sizeHPage = CSize((m_lfWidth*5),0);
	else
		sizeHPage = CSize((m_lfWidth*pDoc->GetMaxLineLength()),0);

	//行サイズ計算
	CSize sizeVLine;						// 垂直ｽｸﾛｰﾙﾊﾞｰ矢印ｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
	CSize sizeHLine;						// 水平ｽｸﾛｰﾙﾊﾞｰ矢印ｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
	if( pDoc->GetLineCount() > 0 )
		sizeVLine = CSize(0,(m_lfHeight+m_lLineInterval));
	else
		sizeVLine = CSize(0,0);
	if( pDoc->GetMaxLineLength() > 0 )
		sizeHLine = CSize(m_lfWidth,0);
	else
		sizeHLine = CSize(0,0);

	//////////////////////////////////////
	//		スクロールサイズ設定		//
	//////////////////////////////////////

	MySetScrollSizes(MM_TEXT,		// ﾏｯﾋﾟﾝｸﾞﾓｰﾄﾞ
					sizeTotal,		// ﾄｰﾀﾙｻｲｽﾞ
					sizeVPage,		// 垂直ｽｸﾛｰﾙﾊﾞｰｼｬﾌﾄｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
					sizeVLine,		// 垂直ｽｸﾛｰﾙﾊﾞｰ矢印ｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
					sizeHPage,		// 水平ｽｸﾛｰﾙﾊﾞｰｼｬﾌﾄｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
					sizeHLine		// 水平ｽｸﾛｰﾙﾊﾞｰ矢印ｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
	);

	//基底クラスのハンドラコール
	CView::OnUpdate(pSender,lHint,pHint);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＳＩＺＥ処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnSize(UINT nType, int cx, int cy)
{
	//基底クラスのハンドラコール
	CView::OnSize(nType, cx, cy);

	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//////////////////////////////////////
	//		ｵﾌｽｸﾘｰﾝﾊﾞｯﾌｧ作成しなおし	//
	//////////////////////////////////////

	DestroyOffScreenBuf();
	CreateOffScreenBuf();

	//////////////////////////////////
	//		文字の高さと幅計算		//
	//////////////////////////////////

	CClientDC	dc(this);
	CFont* oldFont = dc.SelectObject(&m_objDispFont);
	TEXTMETRIC	tm;
	dc.GetTextMetrics(&tm);
	m_lfHeight = tm.tmHeight + tm.tmExternalLeading;
	m_lfWidth  = tm.tmAveCharWidth + tm.tmOverhang;
	dc.SelectObject(oldFont);
	if( (m_lfWidth==0) || (m_lfHeight==0) )
	{
		//ゼロスクロールサイズ設定
		MySetScrollSizes(MM_TEXT,		// ﾏｯﾋﾟﾝｸﾞﾓｰﾄﾞ
						CSize(0,0),		// ﾄｰﾀﾙｻｲｽﾞ
						CSize(0,0),		// 垂直ｽｸﾛｰﾙﾊﾞｰｼｬﾌﾄｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
						CSize(0,0),		// 垂直ｽｸﾛｰﾙﾊﾞｰ矢印ｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
						CSize(0,0),		// 水平ｽｸﾛｰﾙﾊﾞｰｼｬﾌﾄｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
						CSize(0,0)		// 水平ｽｸﾛｰﾙﾊﾞｰ矢印ｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
		);
		return;
	}

	//行番号表示関連初期化
	if( m_bDispLineNo )
	{
		m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		m_LineNoPos = CPoint(m_lLineNoWidth,0);
	}
	else
	{
		m_lLineNoWidth = 0;
		m_LineNoPos = CPoint(0,0);
	}

	//ルーラ表示関連初期化
	if( m_bDispRuler )
		m_RulerPos = CPoint(0,(m_lfHeight+m_lLineInterval));
	else
		m_RulerPos = CPoint(0,0);

	//////////////////////////////////////
	//	クライアント領域の高さと幅		//
	//////////////////////////////////////

	CRect	clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//////////////////////////////////////
	//	クライアント領域に表示できる	//
	//	行数と文字数を求める			//
	//////////////////////////////////////

	long lCanDispLineCnt = nHeight / (m_lfHeight+m_lLineInterval);	//クライアント領域に表示できる行数
	long lCanDispCharCnt = nWidth  / m_lfWidth;						//クライアント領域に表示できる文字数

	//////////////////////////////////////
	//		スクロールサイズ計算		//
	//////////////////////////////////////

	//トータルサイズ計算
	CSize sizeTotal(0,0);
	if( pDoc->GetMaxLineLength() > 0 )
	{
		sizeTotal.cx = (pDoc->GetMaxLineLength() + 1) * m_lfWidth;
		sizeTotal.cx += clientRect.Width()  % m_lfWidth;//あまりを加算
	}
	else
	{
		sizeTotal.cx = 0;
	}
	if( pDoc->GetLineCount() > 0 )
	{
		sizeTotal.cy = (pDoc->GetLineCount() + 1) * (m_lfHeight+m_lLineInterval);
		sizeTotal.cy += clientRect.Height() % (m_lfHeight+m_lLineInterval);	//あまりを加算
	}
	else
	{
		sizeTotal.cy = 0;
	}

	//ページサイズ計算
	CSize sizeVPage;						// 垂直ｽｸﾛｰﾙﾊﾞｰｼｬﾌﾄｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
	CSize sizeHPage;						// 水平ｽｸﾛｰﾙﾊﾞｰｼｬﾌﾄｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
	if( pDoc->GetLineCount() > 10 )
		sizeVPage = CSize(0,((m_lfHeight+m_lLineInterval)*10));
	else
		sizeVPage = CSize(0,((m_lfHeight+m_lLineInterval)*pDoc->GetLineCount()));
	if( pDoc->GetMaxLineLength() > 5 )
		sizeHPage = CSize((m_lfWidth*5),0);
	else
		sizeHPage = CSize((m_lfWidth*pDoc->GetMaxLineLength()),0);

	//行サイズ計算
	CSize sizeVLine;						// 垂直ｽｸﾛｰﾙﾊﾞｰ矢印ｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
	CSize sizeHLine;						// 水平ｽｸﾛｰﾙﾊﾞｰ矢印ｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
	if( pDoc->GetLineCount() > 0 )
		sizeVLine = CSize(0,(m_lfHeight+m_lLineInterval));
	else
		sizeVLine = CSize(0,0);
	if( pDoc->GetMaxLineLength() > 0 )
		sizeHLine = CSize(m_lfWidth,0);
	else
		sizeHLine = CSize(0,0);

	//////////////////////////////////////
	//		スクロールサイズ設定		//
	//////////////////////////////////////

	MySetScrollSizes(MM_TEXT,		// ﾏｯﾋﾟﾝｸﾞﾓｰﾄﾞ
					sizeTotal,		// ﾄｰﾀﾙｻｲｽﾞ
					sizeVPage,		// 垂直ｽｸﾛｰﾙﾊﾞｰｼｬﾌﾄｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
					sizeVLine,		// 垂直ｽｸﾛｰﾙﾊﾞｰ矢印ｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
					sizeHPage,		// 水平ｽｸﾛｰﾙﾊﾞｰｼｬﾌﾄｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
					sizeHLine		// 水平ｽｸﾛｰﾙﾊﾞｰ矢印ｸﾘｯｸでのｽｸﾛｰﾙｻｲｽﾞ
	);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（スクロール関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：垂直スクロールバー　スクロール処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int	nAmount=0;	//スクロール量

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() ||
		pWnd->IsNowStepDoOperation() 
	)
	{
		//マウス操作を無視
		CView::OnVScroll(nSBCode, nPos, pScrollBar);
		return;
	}

	if(m_bDispUnderLine) //アンダーラインを描画する場合
	{
		//スクロールする前にアンダーラインを消去しておく
		CClientDC dc(this);
		CPoint ptVpOrg(0, 0);
		ptVpOrg.x = -1 * GetScrollPosLogX();
		dc.SetViewportOrg(ptVpOrg);
		MyEraseUnderLine(&dc);
	}

	//ｸﾗｲｱﾝﾄ領域
	CRect	clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	if(m_lfHeight==0)
		return;
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );

	// ｽｸﾛｰﾙﾊﾞｰがﾒｯｾｰｼﾞを処理できた場合
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;

	// 垂直ｽｸﾛｰﾙﾊﾞｰ以外
	if (pScrollBar != GetScrollBarCtrl(SB_VERT))
		return;

	//ｽｸﾛｰﾙﾊﾞｰ取得
	CScrollBar* pobjVScrollBar = GetScrollBarCtrl(SB_VERT);

	//ｽｸﾛｰﾙﾎﾟｼﾞｼｮﾝ取得
	SCROLLINFO	vinfo;
	pobjVScrollBar->GetScrollInfo(&vinfo,SIF_ALL);
	int yOrig = vinfo.nPos; //現在位置取得
	switch(nSBCode)
	{
	case SB_TOP:			// 一番上に来た
		vinfo.nPos = 0;
		break;
	case SB_BOTTOM:			// 一番下に来た
		vinfo.nPos = m_totalLog.cy;
		break;
	case SB_LINEUP:			// 上矢印ｸﾘｯｸ
		vinfo.nPos -= m_VLineLog.cy;
		if(vinfo.nPos < 0)
			vinfo.nPos = 0;
		break;
	case SB_LINEDOWN:		// 下矢印ｸﾘｯｸ
		if( (vinfo.nPos + nHeight + m_VLineLog.cy) < m_totalLog.cy )
			vinfo.nPos += m_VLineLog.cy;
		break;
	case SB_PAGEUP:			// 上ｽｸﾛｰﾙﾊﾞｰｼｬﾌﾄｸﾘｯｸ
		vinfo.nPos -= m_VPageLog.cy;
		if(vinfo.nPos < 0)
			vinfo.nPos = 0;
		break;
	case SB_PAGEDOWN:		// 下ｽｸﾛｰﾙﾊﾞｰｼｬﾌﾄｸﾘｯｸ
		if( (vinfo.nPos + nHeight + m_VPageLog.cy) < m_totalLog.cy )
		{
			vinfo.nPos += m_VPageLog.cy;
		}
		else
		{
			while( (vinfo.nPos + nHeight + m_VLineLog.cy) < m_totalLog.cy )
				vinfo.nPos += m_VLineLog.cy;
		}
		break;
	case SB_THUMBTRACK:		// ｽｸﾛｰﾙﾊﾞｰのﾄﾞﾗｯｸﾞ
		if(m_VLineLog.cy == 0)
			return;
		vinfo.nTrackPos -= (vinfo.nTrackPos % m_VLineLog.cy);
		if( (vinfo.nTrackPos + nHeight) < m_totalLog.cy )
		{
			vinfo.nPos = vinfo.nTrackPos;
		}
		else
		{
			while( (vinfo.nPos + nHeight + m_VLineLog.cy) < m_totalLog.cy )
				vinfo.nPos += m_VLineLog.cy;
		}
		break;
	}

	//ｽｸﾛｰﾙﾊﾞｰ現在位置更新
	AdjustVScrollPos(&vinfo);
	pobjVScrollBar->SetScrollInfo(&vinfo,TRUE);

	//再描画
	if( yOrig != vinfo.nPos )
	{
		//ｳｲﾝﾄﾞｳをｽｸﾛｰﾙ
		nAmount = -(vinfo.nPos - yOrig);
		MyScrollWindow(2,nAmount);
	}

	//ｷｬﾚｯﾄ位置を再設定
	if(m_nCaretBehave == CARET_BEHAVE_MOVE) //画面スクロール時に行列位置を変更する場合
	{
		CPoint objPos = m_caretPos;
		objPos.x = m_LineNoPos.x;
		MySetCaretPos(objPos);
	}
	else if(m_nCaretBehave == CARET_BEHAVE_STAY) //画面スクロール時に行列位置を保持する場合
	{
		CPoint objPos = m_caretPos;
		objPos.y += nAmount; //下へ行くときｙは減少。上へ行くときｙは増加
		if(objPos.y < m_RulerPos.y) //上へ行き過ぎた
		{
			objPos.x = m_LineNoPos.x;
			objPos.y = m_RulerPos.y;
		}
		if(objPos.y > (nHeight - (m_lfHeight+m_lLineInterval)) ) //下へ行き過ぎた
		{
			objPos.x = m_LineNoPos.x;
			objPos.y = (nHeight - (m_lfHeight+m_lLineInterval));
		}
		MySetCaretPos(objPos);
	}

	//派生元ﾊﾝﾄﾞﾗｺｰﾙ
	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：水平スクロールバー　スクロール処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nAmount=0;		//スクロール量

	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording()  ||
		pWnd->IsNowStepDoOperation() 
	)
	{
		//マウス操作を無視
		CView::OnHScroll(nSBCode, nPos, pScrollBar);
		return;
	}

	if(m_bDispUnderLine) //アンダーラインを描画する場合
	{
		//スクロールする前にアンダーラインを消去しておく
		CClientDC dc(this);
		CPoint ptVpOrg(0, 0);
		ptVpOrg.x = -1 * GetScrollPosLogX();
		dc.SetViewportOrg(ptVpOrg);
		MyEraseUnderLine(&dc);
	}

	//ｸﾗｲｱﾝﾄ領域
	CRect	clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nWidth = clientRect.Width();
	if(m_lfWidth==0)
		return;
	nWidth -= ( clientRect.Width() % m_lfWidth );

	// ｽｸﾛｰﾙﾊﾞｰがﾒｯｾｰｼﾞを処理できた場合
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;

	// 水平ｽｸﾛｰﾙﾊﾞｰ以外
	if (pScrollBar != GetScrollBarCtrl(SB_HORZ))
		return;

	//ｽｸﾛｰﾙﾊﾞｰ取得
	CScrollBar* pobjHScrollBar = GetScrollBarCtrl(SB_HORZ);
	SCROLLINFO	hinfo;
	pobjHScrollBar->GetScrollInfo(&hinfo,SIF_ALL);
	int xOrig = hinfo.nPos; //現在位置取得
	switch(nSBCode)
	{
	case SB_TOP:			// 一番左に来た
		hinfo.nPos = 0;
		break;
	case SB_BOTTOM:			// 一番右に来た
		hinfo.nPos = m_totalLog.cx;
		break;
	case SB_LINEUP:			// 左矢印ｸﾘｯｸ
		hinfo.nPos -= m_HLineLog.cx;
		if(hinfo.nPos < 0)
			hinfo.nPos = 0;
		break;
	case SB_LINEDOWN:		// 右矢印ｸﾘｯｸ
		if( (hinfo.nPos + nWidth + m_HLineLog.cx) < m_totalLog.cx )
			hinfo.nPos += m_HLineLog.cx;
		break;
	case SB_PAGEUP:			// 左ｽｸﾛｰﾙﾊﾞｰｼｬﾌﾄｸﾘｯｸ
		hinfo.nPos -= m_HPageLog.cx;
		if(hinfo.nPos < 0)
			hinfo.nPos = 0;
		break;
	case SB_PAGEDOWN:		// 右ｽｸﾛｰﾙﾊﾞｰｼｬﾌﾄｸﾘｯｸ
		if( (hinfo.nPos + nWidth + m_HPageLog.cx) < m_totalLog.cx )
		{
			hinfo.nPos += m_HPageLog.cx;
		}
		else
		{
			while( (hinfo.nPos + nWidth + m_HLineLog.cx) < m_totalLog.cx )
				hinfo.nPos += m_HLineLog.cx;
		}
		break;
	case SB_THUMBTRACK:		// ｽｸﾛｰﾙﾊﾞｰのﾄﾞﾗｯｸﾞ
		if(m_HLineLog.cx==0)
			return;
		hinfo.nTrackPos -= (hinfo.nTrackPos % m_HLineLog.cx);
		if( (hinfo.nTrackPos + nWidth) < m_totalLog.cx )
		{
			hinfo.nPos = hinfo.nTrackPos;
		}
		else
		{
			while( (hinfo.nPos + nWidth + m_HLineLog.cx) < m_totalLog.cx )
				hinfo.nPos += m_HLineLog.cx;
		}
		break;
	}

	//ｽｸﾛｰﾙﾊﾞｰ現在位置更新
	AdjustHScrollPos(&hinfo);
	pobjHScrollBar->SetScrollInfo(&hinfo,TRUE);

	//再描画
	if( xOrig != hinfo.nPos )
	{
		//ｳｲﾝﾄﾞｳをｽｸﾛｰﾙ
		nAmount = -(hinfo.nPos - xOrig);
		MyScrollWindow(1,nAmount);
	}

	//ｷｬﾚｯﾄ位置
	if(m_nCaretBehave == CARET_BEHAVE_MOVE) //画面スクロール時に行列位置を変更する場合
	{
		CPoint objPos = m_caretPos;
		objPos.x = m_LineNoPos.x;
		MySetCaretPos(objPos);
	}
	else if(m_nCaretBehave == CARET_BEHAVE_STAY) //画面スクロール時に行列位置を保持する場合
	{
		CPoint objPos = m_caretPos;
		objPos.x += nAmount; //右へ行くときｘは減少。左へ行くときｘは増加
		if(objPos.x < m_LineNoPos.x) //右へ行き過ぎた
		{
			objPos.x = m_LineNoPos.x;
		}
		if(objPos.x > (nWidth-m_lfWidth) ) //左へ行き過ぎた
		{
			objPos.x = (nWidth-m_lfWidth);
		}
		MySetCaretPos(objPos);
	}

	//派生元ﾊﾝﾄﾞﾗｺｰﾙ
	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（ＩＭＥ関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：IME メッセージ WM_IME_NOTIFYのメッセージハンドラ処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LRESULT CEditorView::OnMyImeNotify(WPARAM dwCommand, LPARAM dwData)
{
	//dwCommand には、以下の種類がある。
	//		IMN_CHANGECANDIDATE,		IMN_CLOSECANDIDATE
	//		IMN_CLOSESTATUSWINDOW,		IMN_GUIDELINE
	//		IMN_OPENCANDIDATE,			IMN_PRIVATE
	//		IMN_SETCANDIDATEPOS,		IMN_SETCOMPOSITIONFONT
	//		IMN_SETCOMPOSITIONWINDOW,	IMN_SETCONVERSIONMODE
	//		IMN_SETOPENSTATUS,			IMN_SETSENTENCEMODE
	//		IMN_SETSTATUSWINDOWPOS
	//調べてみると以下のメッセージが飛んできた
	//		アプリケーション起動時、終了時
	//			IMN_SETOPENSTATUS
	//		Alt + 半角キー押下時
	//			IMN_SETOPENSTATUS と IMN_SETSTATUSWINDOWPOS
	//よって以下のようにした
#if 0
	WPARAM wPram1  = IMN_CHANGECANDIDATE;		//8
	WPARAM wPram2  = IMN_CLOSECANDIDATE;		//4
	WPARAM wPram3  = IMN_CLOSESTATUSWINDOW;		//1
	WPARAM wPram4  = IMN_GUIDELINE;				//13
	WPARAM wPram5  = IMN_OPENCANDIDATE;			//5
	WPARAM wPram6  = IMN_PRIVATE;				//14
	WPARAM wPram7  = IMN_SETCANDIDATEPOS;		//9
	WPARAM wPram8  = IMN_SETCOMPOSITIONFONT;	//10
	WPARAM wPram9  = IMN_SETCOMPOSITIONWINDOW;	//11
	WPARAM wPram10 = IMN_SETCONVERSIONMODE;		//6
	WPARAM wPram11 = IMN_SETOPENSTATUS;			//8
	WPARAM wPram12 = IMN_SETSENTENCEMODE;		//7
	WPARAM wPram13 = IMN_SETSTATUSWINDOWPOS;	//12
#endif

	if(dwCommand==IMN_SETOPENSTATUS) //ＡＬＴ＋半角キー押下
	{
		LRESULT ret = DefWindowProc(WM_IME_NOTIFY, dwCommand, dwData);
		if( m_bIsImmOpen == FALSE ) // IME  CLOSE -> OPEN
		{
			//キャレット位置に変換ウインドウを表示
			CgAdjustCompositionWindow();

			//変換ウインドウ内フォント設定
			LOGFONT	logfont;
			ZeroMemory((PVOID)&logfont,sizeof(LOGFONT));
			m_objDispFont.GetLogFont(&logfont);
			::ImmSetCompositionFont(m_hImc, &logfont);
		}
		//ＩＭＭオープンフラグセット
		m_bIsImmOpen = !m_bIsImmOpen;
		return ret;
	}
	else if( (dwCommand==IMN_OPENCANDIDATE)  || //スペースキー押下により、変換対象漢字一覧のウインドウ(CandidateWindow)
			 (dwCommand==IMN_CLOSECANDIDATE) ||	//が表示されないようにする。
			 (dwCommand==3)
	)
	{
		return 0;
	}
	else
	{
		// CG: この関数は 「IME サポート」 コンポーネントにより追加されています。
  		LRESULT ret = DefWindowProc(WM_IME_NOTIFY, dwCommand, dwData);
		return ret;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：現在のキャレットの位置に変換ウィンドウを移動する処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::CgAdjustCompositionWindow()
{
	// CG: この関数は 「IME サポート」 コンポーネントにより追加されています。
	// 現在のキャレット位置に変換ウィンドウを移動する必要がある場合に、この関数を呼び出してください。
	COMPOSITIONFORM cf;
	if (m_hImc)
	{
		cf.dwStyle = CFS_POINT;
		cf.ptCurrentPos = m_caretPos;
		::ImmSetCompositionWindow(m_hImc, &cf);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（マウス操作関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：マウス左ボタンダウンイベント処理										|*/
/*|	解説	：																		|*/
/*|	備考	：自分のViewｳｲﾝﾄﾞｳ内でﾏｳｽｸﾘｯｸされた時に来る								|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnLButtonDown
	(	UINT nFlags,
		CPoint point		//クライアント座標系におけるマウスクリック位置
	)
{
	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() ||
		pWnd->IsNowStepDoOperation() 
	)
	{
		//マウス操作を無視
		CView::OnLButtonDown(nFlags, point);
		return;
	}

	long	lCurrentLine=0;
	long	lCurrentOffset=0;
	CPoint	tempPos(0,0);
	CEditorDoc* pDoc = (CEditorDoc*)GetDocument();
	CRect	clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	//マウス位置　－＞　行、列番号変換
	LONG lRet =	MouseToLineOffset( point, &lCurrentLine, &lCurrentOffset, &tempPos );
	if(lRet	== 0) //Error
	{
		//基底クラスのハンドラコール
		CView::OnLButtonDown(nFlags, point);
		return;
	}

	if(lRet	== 2) //マウス位置が行番号領域内
	{
		ClearSelectArea();

		//キャレット位置を現在のマウス位置行の先頭に位置づけ
		MySetCaretToLineOffset(lCurrentLine,0);

		// 選択開始位置を記憶しておく（選択開始オフセットは行頭）
		m_lSelectStartLine = lCurrentLine;
		m_lSelectStartOffset = 0;
		m_lSelectEndLine = -1;
		m_lSelectEndOffset = -1;
		m_lPreMoveCaretLine	= -1;
		m_lPreMoveCaretOffset =	-1;
		m_lLButtonDownLine = lCurrentLine;
		m_lLButtonDownOffset = 0;

		//マウス左ボタン押下中を記憶
		m_bLButtonDown = TRUE;

		//マウスキャプチャ開始
		//以降、ﾏｳｽが本ｳｲﾝﾄﾞｳの領域外に出てもｲﾍﾞﾝﾄが本ｳｲﾝﾄﾞｳに届けられる
		SetCapture();

		//基底クラスのハンドラコール
		CView::OnLButtonDown(nFlags, point);

		//オートスクロール
		AutoScroll();
		return;
	}
	else //マウス位置が行番号領域やルーラ領域以外
	{
		//キャレット論理Ｘ位置を記憶
		m_nLogPreCaretX = tempPos.x - m_LineNoPos.x + GetScrollPosLogX();

		//キャレット位置を現在のマウス位置に設定
		MySetCaretPos(tempPos);

		BOOL bWithin = IsWithinSelectArea( m_lSelectStartLine,
										   m_lSelectStartOffset,
										   m_lSelectEndLine,
										   m_lSelectEndOffset,
										   lCurrentLine,
										   lCurrentOffset );
		if(IsSelected()	&& bWithin) //選択領域内をクリックした場合
		{
			//////////////////////////////////////
			//	ＯＬＥドラッグ＆ドロップ対応	//
			//////////////////////////////////////

			//ドロップソースの選択行、列を記憶しておく
			long	lSelectStartLine   = m_lSelectStartLine;
			long	lSelectStartOffset = m_lSelectStartOffset;
			long	lSelectEndLine	   = m_lSelectEndLine;
			long	lSelectEndOffset   = m_lSelectEndOffset;

			//ドラッグデータ準備
			AdjustSelectArea();
			CSharedFile	objShareFile(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
			if(m_lSelectStartLine == m_lSelectEndLine) //１行
			{
				CLine* pobjLine	= (CLine*)pDoc->GetOneLine(m_lSelectStartLine);
				objShareFile.Write(	pobjLine->Data(m_lSelectStartOffset),
									(m_lSelectEndOffset-m_lSelectStartOffset) );
			}
			else //複数行
			{
				for(ULONG lLineNo=m_lSelectStartLine; lLineNo<=(ULONG)m_lSelectEndLine; lLineNo++)
				{
					CLine* pobjLine	= (CLine*)pDoc->GetOneLine(lLineNo);
					if(!pobjLine)
						continue;
					if(lLineNo == (ULONG)m_lSelectStartLine)//選択開始行
					{
						objShareFile.Write(	pobjLine->Data(m_lSelectStartOffset),
											(pobjLine->GetLength()-m_lSelectStartOffset) );
					}
					else if(lLineNo	== (ULONG)m_lSelectEndLine)//選択終了行
					{
						objShareFile.Write(	"\r\n",	2 );
						objShareFile.Write(	pobjLine->String(), m_lSelectEndOffset );
					}
					else //途中行
					{
						objShareFile.Write(	"\r\n",	2 );
						objShareFile.Write(	pobjLine->String(), pobjLine->GetLength() );
					}
				}
			}

			//ドラッグ開始
			COleDataSource	objOleDataSource;
			objOleDataSource.CacheGlobalData(CF_TEXT, objShareFile.Detach());
			DROPEFFECT result =	objOleDataSource.DoDragDrop();
			if(result==DROPEFFECT_MOVE)	//MOVE
			{
				//ドロップソースの選択範囲を削除
				CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
				pDoc->DeleteSelectedArea(
							lSelectStartLine,			// 選択開始行番号
							lSelectStartOffset,			// 選択開始列番号
							lSelectEndLine,				// 選択終了行番号
							lSelectEndOffset,			// 選択終了列番号
							FALSE,						// クリップボードへコピーするか？(TRUE:する、FALSE:しない）
							m_caretPos,					// キャレット位置
							objScrollPosLog,			// スクロール位置
							m_bDispRuler,				// ルーラ表示フラグ
							m_bDispLineNo );			// 行番号表示フラグ
			}

			//選択範囲解除
			ClearSelectArea();

			//基底クラスのハンドラコール
			CView::OnLButtonDown(nFlags, point);
			return;
		}
		else //選択領域外をクリックした場合
		{
			// 選択開始位置を記憶しておく
			m_lSelectStartLine = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine = -1;
			m_lSelectEndOffset = -1;
			m_lPreMoveCaretLine	= -1;
			m_lPreMoveCaretOffset =	-1;
			m_lLButtonDownLine = lCurrentLine;
			m_lLButtonDownOffset = lCurrentOffset;

			//マウス左ボタン押下中を記憶
			m_bLButtonDown = TRUE;

			//マウスキャプチャ開始
			//以降、ﾏｳｽが本ｳｲﾝﾄﾞｳの領域外に出てもｲﾍﾞﾝﾄが本ｳｲﾝﾄﾞｳに届けられる
			SetCapture();

			//基底クラスのハンドラコール
			CView::OnLButtonDown(nFlags, point);

			//オートスクロール
			AutoScroll();
			return;
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：マウスカーソル移動イベント処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	//基底クラスのハンドラコール
	CView::OnMouseMove(nFlags, point);
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：マウス左ボタンアップイベント処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() ||
		pWnd->IsNowStepDoOperation() 
	)
	{
		//マウス操作を無視
		CView::OnLButtonUp(nFlags, point);
		return;
	}

	long lCurrentLine=0;
	long lCurrentOffset=0;
	CEditorDoc* pDoc = (CEditorDoc*)GetDocument();
	CPoint	tempPos(0,0);
	CRect	clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	m_bLButtonDown = FALSE;	//マウス左ボタンアップ

	if( GetCapture() == this )
	{
		ReleaseCapture();		//マウスキャプチャ終了

		if( (clientRect.PtInRect(point) == FALSE) || //マウスがクライアント領域外に出た
			(point.x < 0)						  ||
			(point.y < 0)
		)
		{
			//基底クラスのハンドラコール
			CView::OnLButtonUp(nFlags, point);
			return;
		}
		else
		{
			//マウス位置　－＞　行、列番号変換
			LONG lRet =	MouseToLineOffset( point, &lCurrentLine, &lCurrentOffset, &tempPos );
			if(lRet	== 0)//失敗
			{
				//基底クラスのハンドラコール
				CView::OnLButtonDown(nFlags, point);
				return;
			}
			else
			{
				if(lRet	== 1)//成功
				{
					//キャレット位置設定
					MySetCaretPos(tempPos);
				}
				else if(lRet ==	2)//行番号エリア内
				{
					//キャレットをマウス位置行の最後に位置づけ
					CLine* pobjLine	= pDoc->GetOneLine(lCurrentLine);
					if(pobjLine	!= NULL)
						lCurrentOffset = pobjLine->GetLength();
					MySetCaretToLineOffset(lCurrentLine,lCurrentOffset);
				}

				if(	(m_lLButtonDownLine	!= lCurrentLine) ||	(m_lLButtonDownOffset != lCurrentOffset) )
				{
					//選択範囲再描画
					RedrawSelectArea(m_lPreMoveCaretLine,m_lPreMoveCaretOffset);
				}
				else //マウスアップ位置が選択開始位置と同じ
				{
					ClearSelectArea();
					m_pApp->MessagePump();
					if(::GetAsyncKeyState(VK_CONTROL)<0 && lRet==1)//CTRL押下中かつマージンエリアでない
					{
						//CTRL+クリックによるワードセレクト
						WordSelectByCtrlClick(point);
					}
				}
			}
		}
	}

	//基底クラスのハンドラコール
	CView::OnLButtonUp(nFlags, point);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：マウス左ボタンダブルクリックイベント処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() ||
		pWnd->IsNowStepDoOperation() 
	)
	{
		//マウス操作を無視
		CView::OnLButtonDblClk(nFlags, point);
		return;
	}

	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	long	lCnt=0;
	long	lCurrentLine=0;
	long	lCurrentOffset=0;
	CPoint	prePos(0,0);
	CPoint	nowPos(0,0);
	CPoint	mousePos(0,0);
	CLine*	pobjLine = NULL;
	CString objMsg;

	//マウス位置　－＞　行、列番号変換
	LONG lRet =	MouseToLineOffset( point, &lCurrentLine, &lCurrentOffset, &mousePos	);
	if(lRet	== 0 ||	lRet ==	2)
	{
		//基底クラスのハンドラコール
		CView::OnLButtonDblClk(nFlags, point);
		return;
	}

	//マウス位置の行取得
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
	{
		//基底クラスのハンドラコール
		CView::OnLButtonDblClk(nFlags, point);
		return;
	}

	if( pDoc->IsGrep() ) //ＧＲＥＰ結果表示中である
	{
		CString		objGrepFileName;
		long		lLineNo = 0;

		//ＧＲＥＰ結果解釈
		pobjLine->ParseGrepResult(objGrepFileName,&lLineNo);

		//ＧＲＥＰファイルオープン
		if( (_access( (LPCTSTR)objGrepFileName, 0 )) != -1 )
		{
			m_pApp->OpenDocumentFile((LPCTSTR)objGrepFileName);

			//指定行番号へのジャンプ指示
			m_pApp->SendJumpMsg((LPCTSTR)objGrepFileName,lLineNo);

			//基底クラスのハンドラコール
			CView::OnLButtonDblClk(nFlags, point);
			return;
		}
		else
		{
			char	szMsg[512];
			objMsg.LoadString(IDS_GREPFILE_NOTFOUND);
			sprintf(szMsg,(LPCTSTR)objMsg,(LPCTSTR)objGrepFileName);
			m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);
			objGrepFileName.ReleaseBuffer();

			//基底クラスのハンドラコール
			CView::OnLButtonDblClk(nFlags, point);
			return;
		}
	}
	else if( pDoc->IsFSearch() ) //ファイル検索結果表示中である
	{
		char		szFileName[_MAX_PATH];
		CCodeConvManager	objCodeConvManager;

		//検索ファイルオープン
		strcpy(szFileName,(const char*)pobjLine->String());
		if( (_access( szFileName, 0 )) == -1 )
		{
			char	szMsg[512];
			objMsg.LoadString(IDS_FINDFILE_NOTFOUND);
			sprintf(szMsg,(LPCTSTR)objMsg,szFileName);
			m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);
			CView::OnLButtonDblClk(nFlags, point);
			return;
		}
		else
		{
			//検索結果ファイルオープン
			m_pApp->OpenDocumentFile(szFileName);
			CView::OnLButtonDblClk(nFlags, point);
			return;
		}
	}
	else if( pDoc->IsDirCmp() && 	//ディレクトリ比較表示中　＆＆　相違ファイルである
			 pDoc->GetDirCmpLineKind(lCurrentLine)==DIRCMP_LINEKIND_DIFF 
	)
	{
		//比較ディレクトリ名取得
		CString objDir1="",objDir2="",objFileName="";
		pDoc->GetDirCmpFolder(objDir1,objDir2);
		//相違ファイル名取得
		CLine* pobjLine = pDoc->GetOneLine(lCurrentLine);
		if(pobjLine == NULL)
			return;
		pobjLine->GetDirCmpFileName(objFileName);
		//パス作成
		if(m_pApp->IsDriveRoot(objDir1))
			objDir1 += objFileName;
		else
			objDir1 += "\\" + objFileName;
		if(m_pApp->IsDriveRoot(objDir2))
			objDir2 += objFileName;
		else
			objDir2 += "\\" + objFileName;

		pWnd->DoFileCompare(objDir1,objDir2,NULL);
		return;
	}
	else if(pobjLine->IsAboveURL(lCurrentOffset))//ＵＲＬ上である
	{
		if(pDoc->GetFlagUrlColor()) 
		{
			//キャレット位置のＵＲＬ取得
			CString objUrl=_T("");
			if(pobjLine->GetUrl(lCurrentOffset,objUrl))
			{
				//ＵＲＬをＷＥＢブラウザで表示
				m_pApp->BrowseUrl(objUrl);
				return;
			}
		}
	}
	else if(pobjLine->IsAboveEMail(lCurrentOffset))//ｅ－ｍａｉｌ上である
	{
		if(pDoc->GetFlagEMailColor()) 
		{
			//キャレット位置のｅ－ｍａｉｌ取得
			CString objEMail=_T("");
			if(pobjLine->GetEMail(lCurrentOffset,objEMail))
			{
				//メーラーを起動
				m_pApp->ExecMailer(objEMail);
				return;
			}
		}
	}

	//まず選択を解除する
	if(IsSelected())
		ClearSelectArea();

	//////////////////////////////
	//		選択範囲設定		//
	//////////////////////////////
	if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //ﾀﾞﾌﾞﾙｸﾘｯｸ位置が半角
	{
		if( isHanKana(pobjLine->GetAt(lCurrentOffset)) )//半角ｶﾀｶﾅ
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// 選択範囲の後ろを決定
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//選択範囲の前を決定

			//マウス位置を論理座標へ変換する
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//１文字前の位置取得
				if( ((nowPos.x-prePos.x)==m_lfWidth) &&
					isHanKana(pobjLine->GetAt(lCnt-1))
				)
				{
					m_lSelectStartOffset--;
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt--;
				if( nowPos.x == 0 )
					break;
			}
		}
		else if( ( (pobjLine->GetAt(lCurrentOffset)>='A' && pobjLine->GetAt(lCurrentOffset)<='Z') ||
			  	 (pobjLine->GetAt(lCurrentOffset)>='a' && pobjLine->GetAt(lCurrentOffset)<='z') ||
			     (pobjLine->GetAt(lCurrentOffset)>='0' && pobjLine->GetAt(lCurrentOffset)<='9') ||
			     (pobjLine->GetAt(lCurrentOffset)=='_')
			     ) &&
				 !isHanKana(pobjLine->GetAt(lCurrentOffset))
		)
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// 選択範囲の後ろを決定
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) 						||
					( (pobjLine->GetAt(lCnt)<'A' || pobjLine->GetAt(lCnt)>'Z') &&
					  (pobjLine->GetAt(lCnt)<'a' || pobjLine->GetAt(lCnt)>'z') &&
					  (pobjLine->GetAt(lCnt)<'0' || pobjLine->GetAt(lCnt)>'9') &&
					  (pobjLine->GetAt(lCnt)!='_')
					)																||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//選択範囲の前を決定

			//マウス位置を論理座標へ変換する
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//１文字前の位置取得
				if( ( (nowPos.x-prePos.x)==m_lfWidth) &&
					( (pobjLine->GetAt(lCnt-1)>='A' && pobjLine->GetAt(lCnt-1)<='Z') ||
			  		  (pobjLine->GetAt(lCnt-1)>='a' && pobjLine->GetAt(lCnt-1)<='z') ||
			  		  (pobjLine->GetAt(lCnt-1)>='0' && pobjLine->GetAt(lCnt-1)<='9') ||
			  		  (pobjLine->GetAt(lCnt-1)=='_')
					) &&
					!isHanKana(pobjLine->GetAt(lCnt-1))
				)
				{
					m_lSelectStartOffset--;
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt--;
				if( nowPos.x == 0 )
					break;
			}
		}
		else if( !isspace(pobjLine->GetAt(lCurrentOffset)) ) //ﾀﾞﾌﾞﾙｸﾘｯｸ位置が半角英数字でも半角ｶﾀｶﾅでもｽﾍﾟｰｽでもない
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// 選択範囲の後ろを決定
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) 								||
					( (pobjLine->GetAt(lCnt)>='A' && pobjLine->GetAt(lCnt)<='Z') ||
			  		  (pobjLine->GetAt(lCnt)>='a' && pobjLine->GetAt(lCnt)<='z') ||
			  		  (pobjLine->GetAt(lCnt)>='0' && pobjLine->GetAt(lCnt)<='9') ||
			  		  (pobjLine->GetAt(lCnt)=='_')
					) 																		||
					isHanKana(pobjLine->GetAt(lCnt))										||
					isspace(pobjLine->GetAt(lCnt))
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//選択範囲の前を決定

			//マウス位置を論理座標へ変換する
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//１文字前の位置取得
				if( ((nowPos.x-prePos.x)==m_lfWidth)  									&&
					( (pobjLine->GetAt(lCnt-1)<'A' || pobjLine->GetAt(lCnt-1)>'Z') &&
					  (pobjLine->GetAt(lCnt-1)<'a' || pobjLine->GetAt(lCnt-1)>'z') &&
					  (pobjLine->GetAt(lCnt-1)<'0' || pobjLine->GetAt(lCnt-1)>'9') &&
					  (pobjLine->GetAt(lCnt-1)!='_')
					)																	&&
					!isHanKana(pobjLine->GetAt(lCnt-1))									&&
					!isspace(pobjLine->GetAt(lCnt-1))
				)
				{
					m_lSelectStartOffset--;
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt--;
				if( nowPos.x == 0 )
					break;
			}
		}
	}
	else if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //ﾀﾞﾌﾞﾙｸﾘｯｸ位置が全角文字
	{
		//文字コード取得
		UINT unChar = ( (pobjLine->GetAt(lCurrentOffset) << 8) | (pobjLine->GetAt(lCurrentOffset+1)) );
		if( _ismbchira( unChar ) ) //ﾀﾞﾌﾞﾙｸﾘｯｸ位置がﾀﾞﾌﾞﾙﾊﾞｲﾄひらがな
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// 選択範囲の後ろを決定
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!_ismbchira( unCharTmp )
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//選択範囲の前を決定

			//マウス位置を論理座標へ変換する
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//１文字前の位置取得
				if( (nowPos.x-prePos.x)==(m_lfWidth*2) )
				{
					//文字コード取得
					UINT unCharTmp = ( (pobjLine->GetAt(lCnt-2) << 8) | (pobjLine->GetAt(lCnt-1)) );
					if( _ismbchira( unCharTmp ) ) //ひらがな継続
					{
						m_lSelectStartOffset -= 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt -= 2;
				if( nowPos.x == 0 )
					break;
			}
		}
		else if( (_ismbckata(unChar)) ||
				 (unChar==0x815B)			//ﾀﾞﾌﾞﾙｸﾘｯｸ位置がﾀﾞﾌﾞﾙﾊﾞｲﾄかたかなｏｒ”ー”
		)
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// 選択範囲の後ろを決定
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbckata( unCharTmp ) && (unCharTmp!=0x815B) )
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//選択範囲の前を決定

			//マウス位置を論理座標へ変換する
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//１文字前の位置取得
				if( (nowPos.x-prePos.x)==(m_lfWidth*2) )
				{
					//文字コード取得
					UINT unCharTmp = ( (pobjLine->GetAt(lCnt-2) << 8) | (pobjLine->GetAt(lCnt-1)) );
					if( _ismbckata( unCharTmp ) || (unCharTmp==0x815B) ) //カタカナ継続
					{
						m_lSelectStartOffset -= 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt -= 2;
				if( nowPos.x == 0 )
					break;
			}
		}
		else if( _ismbcl1(unChar) || _ismbcl2(unChar) ) //ﾀﾞﾌﾞﾙｸﾘｯｸ位置がﾀﾞﾌﾞﾙﾊﾞｲﾄ漢字
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// 選択範囲の後ろを決定
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbcl1( unCharTmp ) && !_ismbcl2( unCharTmp ) )
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//選択範囲の前を決定

			//マウス位置を論理座標へ変換する
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//１文字前の位置取得
				if( (nowPos.x-prePos.x)==(m_lfWidth*2) )
				{
					//文字コード取得
					UINT unCharTmp = ( (pobjLine->GetAt(lCnt-2) << 8) | (pobjLine->GetAt(lCnt-1)) );
					if( _ismbcl1(unCharTmp) || _ismbcl2(unCharTmp) ) //漢字継続
					{
						m_lSelectStartOffset -= 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt -= 2;
				if( nowPos.x == 0 )
					break;
			}
		}
		else //その他の２バイト文字
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// 選択範囲の後ろを決定
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( _ismbchira( unCharTmp ) ||
					  _ismbckata(unCharTmp)   ||
					  (unCharTmp==0x815B)     ||
					  _ismbcl1( unCharTmp )   ||
					  _ismbcl2( unCharTmp )
					)
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//選択範囲の前を決定

			//マウス位置を論理座標へ変換する
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//１文字前の位置取得
				if( (nowPos.x-prePos.x)==(m_lfWidth*2) )
				{
					//文字コード取得
					UINT unCharTmp = ( (pobjLine->GetAt(lCnt-2) << 8) | (pobjLine->GetAt(lCnt-1)) );
					if(	( !_ismbchira( unCharTmp ) &&
					      !_ismbckata(unCharTmp)   &&
					      (unCharTmp!=0x815B)      &&
					      !_ismbcl1( unCharTmp )   &&
					      !_ismbcl2( unCharTmp )
					    )
					)
					{
						m_lSelectStartOffset -= 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt -= 2;
				if( nowPos.x == 0 )
					break;
			}
		}
	}

	//キャレット位置設定
	MySetCaretToLineOffset(m_lSelectEndLine,m_lSelectEndOffset);

	//マウス位置の行を再描画
	InvalidateCaretLine();

	//基底クラスのハンドラコール
	CView::OnLButtonDblClk(nFlags, point);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＳＥＴＣＵＲＳＯＲ時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect	clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	//マウス位置取得
	POINT	stCursorPos;
	::GetCursorPos(&stCursorPos);
	ScreenToClient(&stCursorPos);

	if( !(stCursorPos.x >= m_LineNoPos.x) || !(stCursorPos.x < clientRect.Width()) || //文字入力エリア外
		!(stCursorPos.y >= m_RulerPos.y)  || !(stCursorPos.y < clientRect.Height())
	)
	{
		return CView::OnSetCursor(pWnd, nHitTest, message);
	}

	//文字入力エリア内　＆　選択中
	if(IsSelected())
		return CView::OnSetCursor(pWnd, nHitTest, message);

	//マウス位置　－＞　行、列番号変換
	long lLine=0;
	long lOffset=0;
	CPoint tempPos;
	CPoint objCursorPos(stCursorPos.x,stCursorPos.y);
	long lRet =	MouseToLineOffset( objCursorPos, &lLine, &lOffset, &tempPos );//1:OK
	if(lRet != 1)
	{
		//標準Ｉビームカーソル
		::SetCursor(::LoadCursor(NULL, IDC_IBEAM));
		return TRUE;
	}

	//ＵＲＬ上チェック
	if(pDoc->GetFlagUrlColor())
	{
		if(pDoc->IsAboveURL(lLine,lOffset))
		{
			//ハンドカーソル
			::SetCursor(m_pApp->LoadCursor(IDC_HAND));
			return TRUE;
		}
	}

	//ｅ－ｍａｉｌ上チェック
	if(pDoc->GetFlagEMailColor())
	{
		if(pDoc->IsAboveEMail(lLine,lOffset))
		{
			//ハンドカーソル
			::SetCursor(m_pApp->LoadCursor(IDC_HAND));
			return TRUE;
		}
	}

	//標準Ｉビームカーソル
	::SetCursor(::LoadCursor(NULL, IDC_IBEAM));
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（フィルタ関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：メッセージフィルタ処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::PreTranslateMessage(MSG* pMsg) 
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	UINT nChar = LOWORD(pMsg->wParam);
	CKeyBind* pobjKeyBind=NULL;
	BOOL bIsCtrl  = (::GetAsyncKeyState(VK_CONTROL) < 0 ? TRUE : FALSE);
	BOOL bIsShift = (::GetAsyncKeyState(VK_SHIFT)   < 0 ? TRUE : FALSE);
	BOOL bIsAlt   = (::GetAsyncKeyState(VK_MENU)    < 0 ? TRUE : FALSE);
	if(pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_KEYDOWN)
	{
		if(bIsAlt && nChar==VK_F6)
			int a=0;
		if(nChar >= VK_F1 && nChar <= VK_F12) //ファンクションキー
		{
			//キーバインド検索
			if((pobjKeyBind = m_pApp->FindKeyBind(nChar,bIsCtrl,bIsShift,bIsAlt)) != NULL)
			{
				pWnd->PostMessage(WM_COMMAND,pobjKeyBind->m_dwMenuID,0);
				return TRUE;
			}
			else
				return CView::PreTranslateMessage(pMsg);
		}
		if(bIsCtrl || bIsAlt)
		{
			//キーバインド検索
			if((pobjKeyBind = m_pApp->FindKeyBind(nChar,bIsCtrl,bIsShift,bIsAlt)) != NULL)
			{
				pWnd->PostMessage(WM_COMMAND,pobjKeyBind->m_dwMenuID,0);
				return TRUE;
			}
			else
				return CView::PreTranslateMessage(pMsg);
		}
		else if(bIsShift)
		{
			if(nChar==VK_INSERT || nChar==VK_DELETE || nChar==VK_BACK || nChar==VK_RETURN)
			{
				//キーバインド検索
				if((pobjKeyBind = m_pApp->FindKeyBind(nChar,bIsCtrl,bIsShift,bIsAlt)) != NULL)
				{
					pWnd->PostMessage(WM_COMMAND,pobjKeyBind->m_dwMenuID,0);
					return TRUE;
				}
				else
					return CView::PreTranslateMessage(pMsg);
			}
			else
				return CView::PreTranslateMessage(pMsg);
		}
		else
			return CView::PreTranslateMessage(pMsg);
	}
	else
		return CView::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（文字入力関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＳＹＳＫＥＹＤＯＷＮ処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CKeyBind* pobjKeyBind=NULL;
	BOOL bIsCtrl  = (::GetAsyncKeyState(VK_CONTROL) < 0 ? TRUE : FALSE);
	BOOL bIsShift = (::GetAsyncKeyState(VK_SHIFT)   < 0 ? TRUE : FALSE);
	BOOL bIsAlt   = (::GetAsyncKeyState(VK_MENU)    < 0 ? TRUE : FALSE);
	//キーバインド検索
	if((pobjKeyBind = m_pApp->FindKeyBind(nChar,bIsCtrl,bIsShift,bIsAlt)) != NULL)
		pWnd->PostMessage(WM_COMMAND,pobjKeyBind->m_dwMenuID,0);
	CView::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＫＥＹＤＯＷＮ処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nCnt=0;
	int nMultValueOfCaretAccelarete = m_pApp->GetProfileInt("CEditorView","nMultValueOfCaretAccelarete",3);
	if( nMultValueOfCaretAccelarete!=1 && nMultValueOfCaretAccelarete!=2 &&
		nMultValueOfCaretAccelarete!=3 && nMultValueOfCaretAccelarete!=4 &&
		nMultValueOfCaretAccelarete!=5 )
	{
		nMultValueOfCaretAccelarete = 3;
	}
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(nChar == VK_SHIFT)
		m_bIsShiftKeyDown = TRUE;

	TRACE("nChar=%x,nRepCnt=%d,nFlags=%x\r\n",nChar,nRepCnt,nFlags);

	// インクリメンタルサーチ実行中チェック
	if(pWnd->IsNowIncrimentalSearching())
	{
		//派生元ハンドラコール
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	// キー操作の記録中チェック
	if(pWnd->IsNowOperationRecording())
	{
		if( (nChar==VK_SHIFT) || (nChar==VK_INSERT) || (nChar==VK_LEFT)   || (nChar==VK_RIGHT)  ||
			(nChar==VK_DOWN)  || (nChar==VK_UP)     || (nChar==VK_HOME)   || (nChar==VK_END)    ||
			(nChar==VK_PRIOR) || (nChar==VK_NEXT)   || (nChar==VK_RETURN) || (nChar==VK_DELETE) ||
			(nChar==VK_BACK)
		)
		{
			//キーオペレーションオブジェクト作成
			COperation* pobjOperation = new COperation();
			pobjOperation->SetOperationKind(KEYOPE_ONKEYDOWN);
			pobjOperation->SetOnKeyDownParameter(nChar,nRepCnt,nFlags,nMultValueOfCaretAccelarete);
			//記録
			pWnd->addOperation(pobjOperation);
		}
	}

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	long lLineLength    = pDoc->GetLineLength(lCurrentLine);
	long lMaxLine       = pDoc->GetLineCount();

	if( (nChar == VK_LEFT) || (nChar == VK_RIGHT) || (nChar == VK_DOWN) || //キャレット移動キー
		(nChar == VK_UP)   || (nChar == VK_HOME)  || (nChar == VK_END)  ||
		(nChar == VK_PRIOR)|| (nChar == VK_NEXT)
	)
	{
		//キャレット移動するまえの位置を記憶
		m_lPreMoveCaretLine   = lCurrentLine;
		m_lPreMoveCaretOffset = lCurrentOffset;

		if(m_bIsShiftKeyDown)//SHIFTキー押下中
		{
			if( (m_lSelectStartLine == -1) || (m_lSelectStartOffset == -1) )
			{
				//選択開始位置を記憶
				m_lSelectStartLine   = lCurrentLine;
				m_lSelectStartOffset = lCurrentOffset;
				m_lSelectEndLine     = -1;
				m_lSelectEndOffset   = -1;
			}
		}
	}

	//////////////////////////
	//押下キーによる処理分岐//
	//////////////////////////
	if(nChar == VK_INSERT) //INSERTキー
	{
		//挿入・上書きモード切替え
		OnChangeInsmode();
	}
	else if(nChar == VK_LEFT) //左矢印キー
	{
		if((nFlags&BIT_14)!=0)//キー連続押下
		{
			for(nCnt=0; nCnt<nMultValueOfCaretAccelarete; nCnt++)
				MoveCaret(VK_LEFT,1);
		}
		else
			MoveCaret(VK_LEFT,1);
	}
	else if(nChar == VK_RIGHT) //右矢印キー
	{
		if((nFlags&BIT_14)!=0)//キー連続押下
		{
			for(nCnt=0; nCnt<nMultValueOfCaretAccelarete; nCnt++)
				MoveCaret(VK_RIGHT,1);
		}
		else
			MoveCaret(VK_RIGHT,1);
	}
	else if(nChar == VK_DOWN) //下矢印キー
	{
		if((nFlags&BIT_14)!=0)//キー連続押下
		{
			for(nCnt=0; nCnt<nMultValueOfCaretAccelarete; nCnt++)
				MoveCaret(VK_DOWN,(int)nRepCnt);
		}
		else
			MoveCaret(VK_DOWN,(int)nRepCnt);
	}
	else if(nChar == VK_UP)	//上矢印キー
	{
		if((nFlags&BIT_14)!=0)//キー連続押下
		{
			for(nCnt=0; nCnt<nMultValueOfCaretAccelarete; nCnt++)
				MoveCaret(VK_UP,(int)nRepCnt);
		}
		else
			MoveCaret(VK_UP,(int)nRepCnt);
	}
	else if(nChar == VK_HOME) //HOMEキー
		MoveCaret(VK_HOME,0);
	else if(nChar == VK_END) //ENDキー
		MoveCaret(VK_END,0);
	else if(nChar == VK_PRIOR) //PAGE UPキー
	{
		if((nFlags&BIT_14)!=0)//キー連続押下
		{
			for(nCnt=0; nCnt<nMultValueOfCaretAccelarete; nCnt++)
				MoveCaret(VK_PRIOR,(int)nRepCnt);
		}
		else
			MoveCaret(VK_PRIOR,(int)nRepCnt);
	}
	else if(nChar == VK_NEXT) //PAGE DOWNキー
	{
		if((nFlags&BIT_14)!=0)//キー連続押下
		{
			for(nCnt=0; nCnt<nMultValueOfCaretAccelarete; nCnt++)
				MoveCaret(VK_NEXT,(int)nRepCnt);
		}
		else
			MoveCaret(VK_NEXT,(int)nRepCnt);
	}
	else if(nChar == VK_RETURN) //リターンキー
	{
		if( pDoc->IsReadOnly() )
		{
			MessageBeep((UINT)-1);
		}
		else
		{
			//ﾘﾀｰﾝｺｰﾄﾞ処理
			CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
			pDoc->ProcessReturnCode( lCurrentLine,
									 lCurrentOffset,
									 m_caretPos,
									 objScrollPosLog,
									 m_bDispRuler,
									 m_bDispLineNo );
			OnUpdate(NULL,NULL,NULL);
			MoveCaret(VK_DOWN,1);
			MoveCaret(VK_HOME,0);
			Invalidate(FALSE);
		}
	}
	else if(nChar == VK_DELETE) //DELETEキー
	{
		if( pDoc->IsReadOnly() )
		{
			MessageBeep((UINT)-1);
		}
		else
		{
			if( IsSelected() )
			{
				AdjustSelectArea();//範囲選択位置調整
				//選択範囲テキスト削除
				CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
				pDoc->DeleteSelectedArea(m_lSelectStartLine,
										 m_lSelectStartOffset,
										 m_lSelectEndLine,
										 m_lSelectEndOffset,
										 FALSE,
										 m_caretPos,
										 objScrollPosLog,
										 m_bDispRuler,
										 m_bDispLineNo );
				OnUpdate(NULL,NULL,NULL);
				Invalidate(FALSE);
			}
			else //範囲選択していない
			{
				//DELETEコード処理
				CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
				pDoc->ProcessDeleteCode( lCurrentLine,
										 lCurrentOffset,
										 m_caretPos,
										 objScrollPosLog,
										 m_bDispRuler,
										 m_bDispLineNo );
			}
		}
	}
	else if(nChar == VK_BACK) //BACK SPACEキー
	{
		if( pDoc->IsReadOnly() )
		{
			MessageBeep((UINT)-1);
		}
		else
		{
			if( IsSelected() )
			{
				AdjustSelectArea();//範囲選択位置調整
				//選択範囲テキスト削除
				CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
				pDoc->DeleteSelectedArea(m_lSelectStartLine,
										 m_lSelectStartOffset,
										 m_lSelectEndLine,
										 m_lSelectEndOffset,
										 FALSE,
										 m_caretPos,
										 objScrollPosLog,
										 m_bDispRuler,
										 m_bDispLineNo );
				OnUpdate(NULL,NULL,NULL);
				Invalidate(FALSE);
			}
			else //範囲選択していない
			{
				//BACK SPACEコード処理
				CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
				pDoc->ProcessBackSpaceCode( lCurrentLine,
											lCurrentOffset,
											m_caretPos,
											objScrollPosLog,
											m_bDispRuler,
											m_bDispLineNo );
			}
		}
	}

	//派生元ハンドラコール
	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	if( (nChar == VK_LEFT) || (nChar == VK_RIGHT) || (nChar == VK_DOWN) || //キャレット移動キー
		(nChar == VK_UP)   || (nChar == VK_HOME)  || (nChar == VK_END)  ||
		(nChar == VK_PRIOR)|| (nChar == VK_NEXT)
	)
	{
		if(m_bIsShiftKeyDown)//SHIFTキー押下中
		{
			// 選択範囲の再描画処理
			RedrawSelectArea(m_lPreMoveCaretLine,m_lPreMoveCaretOffset);
		}
		else //SHIFTキー押下していない
		{
			//範囲選択解除
			ClearSelectArea();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＫＥＹＵＰ処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();

	// キー操作の記録中チェック
	if(pWnd->IsNowOperationRecording())
	{
		if(nChar==VK_SHIFT)
		{
			//キーオペレーションオブジェクト作成
			COperation* pobjOperation = new COperation();
			pobjOperation->SetOperationKind(KEYOPE_ONKEYUP);
			pobjOperation->SetOnKeyUpParameter(nChar,nRepCnt,nFlags);
			//記録
			pWnd->addOperation(pobjOperation);
		}
	}

	if(nChar == VK_SHIFT)
		m_bIsShiftKeyDown = FALSE;

	// 基底クラスのハンドラコール
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＣＨＡＲ処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect	clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();
	CString objMsg;

	//書込み禁止チェック
	if( pDoc->IsReadOnly() )
	{
		if(!pDoc->IsDirCmp())
			MessageBeep((UINT)-1);
		CView::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	// インクリメンタルサーチ実行中チェック
	if(pWnd->IsNowIncrimentalSearching())
	{
		if(m_n1stChar == 0) //何もまだ保持していない
		{
			if(IsDBCSLeadByteEx(0,nChar))//ﾏﾙﾁﾊﾞｲﾄ文字の１文字目
			{
				//ﾊﾞｯﾌｧへ保持
				m_n1stChar = nChar;
				CView::OnChar(nChar, nRepCnt, nFlags);
				return;
			}
			else //ｼﾝｸﾞﾙﾊﾞｲﾄ文字
			{
				if( (nChar==VK_RETURN) || (nChar == VK_TAB) )
				{
					//無視
					CView::OnChar(nChar, nRepCnt, nFlags);
					return;
				}
				else if(nChar==VK_BACK)
				{
					//後ろの１文字削除
					m_objIncrimentalSearchLine.DelEndChar();
					if(m_objIncrimentalSearchLine.GetLength()==0)
					{
						//文頭へ戻る
						m_lIncrimentalSearchStartLine   = 0;
						m_lIncrimentalSearchStartOffset = 0;
						ClearSelectArea();
						Jump2LineOffsetEx(0,0);
						CView::OnChar(nChar, nRepCnt, nFlags);
						return;
					}
					else
					{
						//リバースインクリメンタルサーチ実行
						CString objStr = m_objIncrimentalSearchLine.String();
						BOOL bRet = DoIncrimentalSearch( m_lIncrimentalSearchStartLine,
														 m_lIncrimentalSearchStartOffset,
														 objStr,
														 FALSE );
						if(bRet != FALSE)//発見終了
						{
							//次回検索開始行、オフセット保持
							m_lIncrimentalSearchStartLine   = m_lSelectStartLine;
							m_lIncrimentalSearchStartOffset = m_lSelectStartOffset;
						}
						//終了
						CView::OnChar(nChar,nRepCnt,nFlags);
						return;
					}
				}
				else if(nChar==VK_ESCAPE)
				{
					//インクリメンタルサーチを終了する
					objMsg.LoadString(IDS_END_INCRIMENTALSEARCH);
					m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
					m_lIncrimentalSearchStartLine = 0;
					m_lIncrimentalSearchStartOffset = 0;
					pWnd->SetFlagIncrimentalSearching(FALSE);
					m_n1stChar = 0;
					m_n2ndChar = 0;
					CView::OnChar(nChar, nRepCnt, nFlags);
					return;
				}
				else
				{
					m_objIncrimentalSearchLine.Catenate((UCHAR)nChar);
					m_n1stChar = 0;
					m_n2ndChar = 0;
				}
			}
		}
		else //第一ﾊﾞｲﾄを保持済み
		{
			char szBuf[4];
			szBuf[0] = m_n1stChar;
			szBuf[1] = nChar;
			szBuf[2] = 0;
			m_objIncrimentalSearchLine.Catenate((const unsigned char*)szBuf);
			m_n1stChar = 0;
			m_n2ndChar = 0;
		}

		//インクリメンタルサーチ実行
		CString objStr = m_objIncrimentalSearchLine.String();
		BOOL bRet = DoIncrimentalSearch( m_lIncrimentalSearchStartLine,
										 m_lIncrimentalSearchStartOffset,
										 objStr,
										 TRUE );
		if(bRet != FALSE)//発見終了
		{
			//次回検索開始行、オフセット保持
			m_lIncrimentalSearchStartLine   = m_lSelectStartLine;
			m_lIncrimentalSearchStartOffset = m_lSelectStartOffset;
		}
		else //未発見終了
		{
			char szBuf[512];
			objMsg.LoadString(IDS_INCREMENTALSEARCH_END);
			sprintf(szBuf,(LPCTSTR)objMsg,(LPCTSTR)objStr);
			m_pApp->MyMessageBox((LPCTSTR)objMsg,(MB_OK|MB_ICONINFORMATION),0);
			m_lIncrimentalSearchStartLine = 0;
			m_lIncrimentalSearchStartOffset = 0;
			pWnd->SetFlagIncrimentalSearching(FALSE);
		}

		//終了
		CView::OnChar(nChar,nRepCnt,nFlags);
		return;
	}

	// キー操作の記録中チェック
	if(pWnd->IsNowOperationRecording())
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONCHAR);
		pobjOperation->SetOnCharParameter(nChar,nRepCnt,nFlags);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	// 範囲選択済みチェック
	if(IsSelected())
	{
		if(!IsDBCSLeadByteEx(0,nChar) && (nChar==VK_TAB)) //半角タブ
		{
			//何もしない
		}
		else if(!IsDBCSLeadByteEx(0,nChar) && (nChar==VK_ESCAPE)) //ESCAPE
		{
			//選択範囲解除
			ClearSelectArea();
		}
		else if(!IsDBCSLeadByteEx(0,nChar) && !isprint(nChar) ) //
		{
			//何もしない
		}
		else
		{
			AdjustSelectArea();//範囲選択位置調整
			//選択範囲テキスト削除
			CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
			pDoc->DeleteSelectedArea(m_lSelectStartLine,
									 m_lSelectStartOffset,
									 m_lSelectEndLine,
									 m_lSelectEndOffset,
									 FALSE,
									 m_caretPos,
									 objScrollPosLog,
									 m_bDispRuler,
									 m_bDispLineNo );
		}
	}

	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	long lLineLength    = pDoc->GetLineLength(lCurrentLine);
	long lMaxLine       = pDoc->GetLineCount();


	if(m_n1stChar == 0) //何もまだ保持していない
	{
		if(IsDBCSLeadByteEx(0,nChar))//ﾏﾙﾁﾊﾞｲﾄ文字の１文字目
		{
			//ﾊﾞｯﾌｧへ保持
			m_n1stChar = nChar;
			CView::OnChar(nChar, nRepCnt, nFlags);
			return;
		}
		else //ｼﾝｸﾞﾙﾊﾞｲﾄ文字
		{
			if( (nChar==VK_BACK) ||	(nChar==VK_RETURN) || (nChar == VK_ESCAPE) )
			{
				//無視
				CView::OnChar(nChar, nRepCnt, nFlags);
				return;
			}
			else
			{
				if( (nChar==VK_TAB) && IsSelected() )
				{
					if(::GetAsyncKeyState(VK_SHIFT)<0) //Shift Key 押下中
					{
						AdjustSelectArea();//範囲選択位置調整
						//範囲選択している状態でのＳｈｉｆｔ＋ＴＡＢキー操作
						CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
						pDoc->ProcessShiftTabWhenSelected( m_lSelectStartLine,
														  m_lSelectStartOffset,
														  m_lSelectEndLine,
														  m_lSelectEndOffset,
														  m_caretPos,
														  objScrollPosLog,
														  m_bDispRuler,
														  m_bDispLineNo );
					}
					else
					{
						AdjustSelectArea();//範囲選択位置調整
						//範囲選択している状態でのＴＡＢキー操作
						CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
						pDoc->ProcessTabWhenSelected( m_lSelectStartLine,
													  m_lSelectStartOffset,
													  m_lSelectEndLine,
													  m_lSelectEndOffset,
													  m_caretPos,
													  objScrollPosLog,
													  m_bDispRuler,
													  m_bDispLineNo );
					}
				}
				else
				{
					//文字登録
					CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
					pDoc->AddCharacter(	lCurrentLine,
										lCurrentOffset,
										nChar,
										NULL,
										m_bInsertKey,
										m_caretPos,
										objScrollPosLog,
										m_bDispRuler,
										m_bDispLineNo );
				}
				m_n1stChar = 0;
				m_n2ndChar = 0;
			}
		}
	}
	else //第一ﾊﾞｲﾄを保持済み
	{
		//文字登録
		CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
		pDoc->AddCharacter( lCurrentLine,
							lCurrentOffset,
							m_n1stChar,
							nChar,
							m_bInsertKey,
							m_caretPos,
							objScrollPosLog,
							m_bDispRuler,
							m_bDispLineNo );
		m_n1stChar = 0;
		m_n2ndChar = 0;
	}


	//再描画
	OnUpdate(NULL,NULL,NULL);
	if( (nChar==VK_TAB) && IsSelected() ) //範囲選択された領域のタブ
	{
		//選択領域のみ再描画
		AdjustSelectArea();
		long lRedrawLineCnt=0;
		if(m_lSelectStartLine == lCurrentLine)
			lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //プラス値
		else if(m_lSelectEndLine == lCurrentLine)
			lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //マイナス値
		InvalidateCaretLineEx(lRedrawLineCnt);
	}
	else
	{
		InvalidateCaretLine();//キャレットのある１行を再描画
		//キャレット移動
		MoveCaret(VK_RIGHT,1);
	}

	//派生元ハンドラコール
	CView::OnChar(nChar, nRepCnt, nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Ctrl + ] 押下時処理													|*/
/*|	解説	：対応する括弧へキャレット移動											|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnParenthesis()
{
	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if(pWnd->IsNowOperationRecording())
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONPARENTHESIS);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//対応する括弧へのキャレット位置づけ
	MySetCaretToMatchBrace(FALSE);
}
void CEditorView::OnUpdateParenthesis(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	// キャレット位置の行取得
	CLine*	pobjLine = NULL;
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	// キャレット位置の文字取得
	UCHAR unCurrentChar = pobjLine->GetAt(lCurrentOffset);
	if( IsDBCSLeadByteEx(0,unCurrentChar) )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if( unCurrentChar==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if( unCurrentChar!='(' &&
		unCurrentChar!=')' &&
		unCurrentChar!='{' &&
		unCurrentChar!='}' &&
		unCurrentChar!='[' &&
		unCurrentChar!=']' &&
		unCurrentChar!='<' &&
		unCurrentChar!='>'
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Ctrl + Shift + E 押下時処理											|*/
/*|	解説	：対応する括弧へキャレット移動し選択									|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnParenthesisSelect()
{
	// キー操作の記録中チェック
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if(pWnd->IsNowOperationRecording())
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONPARENTHESISSELECT);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	//対応する括弧へのキャレット位置づけ
	MySetCaretToMatchBrace(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（キャレット関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Ｃｔｒｌ＋右矢印キー押下時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnWordskip2right() 
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// キー操作の記録中チェック
	if(pWnd->IsNowOperationRecording())
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONWORDSKIP2RIGHT);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	CPoint	objCaretPos = m_caretPos;
	//キャレット位置を論理座標へ変換する
	CPoint	logicalPoint = objCaretPos;
	logicalPoint -= m_LineNoPos;
	logicalPoint -= m_RulerPos;
	logicalPoint.x += GetScrollPosLogX();
	logicalPoint.y += GetScrollPosLogY();

	CRect clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	if( (m_lfWidth==0) || (m_lfHeight==0) )
		return;
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	long	lCnt=0;
	long	lEndOffset = lCurrentOffset;
	CLine*	pobjLine = NULL;
	long lMaxLine       = pDoc->GetLineCount();

	//キャレット位置の行取得
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
		return;

	//まず選択を解除する
	if(IsSelected())
		ClearSelectArea();

	//////////////////////////////////////
	//		キャレット移動先設定		//
	//////////////////////////////////////
	if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) && //キャレット位置が半角
		pobjLine->GetAt(lCurrentOffset) != 0
	) 
	{
		int nMargin=0;
		if( isHanKana(pobjLine->GetAt(lCurrentOffset)) )//半角ｶﾀｶﾅ
		{
			// キャレット右方向移動先を検索
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
				}
			}
		}
		else if( ( isAlnum(pobjLine->GetAt(lCurrentOffset)) || (pobjLine->GetAt(lCurrentOffset)=='_')
				 ) &&
				 !isHanKana(pobjLine->GetAt(lCurrentOffset))
		)
		{
			// キャレット右方向移動先を検索
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt))	||
					( !isAlnum(pobjLine->GetAt(lCnt)) &&
					  (pobjLine->GetAt(lCnt)!='_')
					)											||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
				}
			}
		}
		else
		{
			// キャレット右方向移動先を検索
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) 		||
					isAlnum(pobjLine->GetAt(lCnt))					||
					(pobjLine->GetAt(lCnt)=='_')					||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
				}
			}
		}

		if( logicalPoint.x + nMargin > nWidth ) //表示範囲の右側へ行った場合
		{
			//右方向スクロール
			MyScrollToPosition(CPoint(GetScrollPosLogX()+nMargin, GetScrollPosLogY()));
		}
		//キャレット位置設定
		MySetCaretToLineOffset(lCurrentLine,lEndOffset);
	}
	else if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //キャレット位置が全角文字
	{
		int nMargin=0;

		//文字コード取得
		UINT unChar = ( (pobjLine->GetAt(lCurrentOffset) << 8) | (pobjLine->GetAt(lCurrentOffset+1)) );
		if( _ismbchira( unChar ) ) //キャレット位置がﾀﾞﾌﾞﾙﾊﾞｲﾄひらがな
		{
			// キャレット右方向移動先を検索
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!_ismbchira( unCharTmp )
				)
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
				}
			}
		}
		else if( (_ismbckata(unChar)) ||
				 (unChar==0x815B)			//キャレット位置がﾀﾞﾌﾞﾙﾊﾞｲﾄかたかなｏｒ”ー”
		)
		{
			// キャレット右方向移動先を検索
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbckata( unCharTmp ) && (unCharTmp!=0x815B) )
				)
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
				}
			}
		}
		else if( _ismbcl1(unChar) || _ismbcl2(unChar) ) //キャレット位置がﾀﾞﾌﾞﾙﾊﾞｲﾄ漢字
		{
			// キャレット右方向移動先を検索
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbcl1( unCharTmp ) && !_ismbcl2( unCharTmp ) )
				)
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
				}
			}
		}
		else //その他の２バイト文字
		{
			// キャレット右方向移動先を検索
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( _ismbchira( unCharTmp ) ||
					  _ismbckata(unCharTmp)   ||
					  (unCharTmp==0x815B)     ||
					  _ismbcl1( unCharTmp )   ||
					  _ismbcl2( unCharTmp )
					)
				)
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
				}
			}
		}

		if( logicalPoint.x + nMargin > nWidth ) //表示範囲の右側へ行った場合
		{
			//右方向スクロール
			MyScrollToPosition(CPoint(GetScrollPosLogX()+nMargin, GetScrollPosLogY()));
		}
		//キャレット位置設定
		MySetCaretToLineOffset(lCurrentLine,lEndOffset);
	}
	else //キャレット位置が行末
	{
		if(lCurrentLine+1 < lMaxLine) //まだ下へ行ける
		{
			//MyScrollToPositionを１回のみコールするため
			CPoint	objCaretPos = m_caretPos;
			CPoint	destPos(GetScrollPosLogX(),GetScrollPosLogY());
			if(GetScrollPosLogX() > 0)
			{
				//左スクロール
				destPos.x = 0;
			}
			//キャレットを次の行へ
			if((objCaretPos.y+(m_lfHeight+m_lLineInterval)*2) > nHeight)//表示範囲の下側に行った場合
			{
				//１行分下方向スクロール
				destPos.y += (m_lfHeight+m_lLineInterval);
			}

			if( destPos.x != GetScrollPosLogX() ||
				destPos.y != GetScrollPosLogY()
			)
			{
				MyScrollToPosition( destPos );
			}

			lCurrentLine++;
			lEndOffset=0;
			//キャレット位置設定
			MySetCaretToLineOffset(lCurrentLine,lEndOffset);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Ｃｔｒｌ＋左矢印キー押下時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnWordskip2left() 
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// キー操作の記録中チェック
	if(pWnd->IsNowOperationRecording())
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONWORDSKIP2LEFT);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	CPoint	objCaretPos = m_caretPos;
	//キャレット位置を論理座標へ変換する
	CPoint	logicalPoint = objCaretPos;
	logicalPoint -= m_LineNoPos;
	logicalPoint -= m_RulerPos;
	logicalPoint.x += GetScrollPosLogX();
	logicalPoint.y += GetScrollPosLogY();

	CRect clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	if( (m_lfWidth==0) || (m_lfHeight==0) )
		return;
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	long	lCnt=0;

	//ひとつ前へ
	lCurrentOffset = GetPreOffset(lCurrentLine,lCurrentOffset);
	CLine*	pobjLine = NULL;

	//キャレット位置の行取得
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
		return;

	//まず選択を解除する
	if(IsSelected())
		ClearSelectArea();

	//////////////////////////////////////
	//		キャレット移動先設定		//
	//////////////////////////////////////
	if(lCurrentOffset == -1)
	{
		if(lCurrentLine > 0)//まだ上へ行ける
		{
			//MyScrollToPotisionを１回のコールにするための処理
			CPoint	destPos(GetScrollPosLogX(),GetScrollPosLogY());
			//キャレットを文字列の最後尾へ
			CPoint	tempLogPos(0,logicalPoint.y-(m_lfHeight+m_lLineInterval));
			GetPosition(lCurrentLine-1,&tempLogPos);//行末の位置取得
			if(tempLogPos.x > nWidth)//表示範囲の右側へ行った場合
			{
				//右方向スクロール
				long lLogX = tempLogPos.x - nWidth + 1;
				destPos.x = lLogX;
			}
			//論理座標－＞デバイス座標
			objCaretPos = tempLogPos;
			objCaretPos -= destPos;/*m_ScrollPosLog;*/
			objCaretPos += m_RulerPos;
			objCaretPos += m_LineNoPos;
			if(objCaretPos.y < m_RulerPos.y )//表示範囲の上側に行った場合
			{
				objCaretPos.y = m_RulerPos.y;
				//１行分上方向スクロール
				destPos.y -= (m_lfHeight+m_lLineInterval);
			}
			if( destPos.x != GetScrollPosLogX() ||
				destPos.y != GetScrollPosLogY()
			)
			{
				MyScrollToPosition(destPos);
			}
		}

		//キャレット位置設定
		MySetCaretPos(objCaretPos);		//キャレット位置設定（クライアント座標系）
	}
	else if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //キャレット位置が半角
	{
		CPoint tempPos(0,logicalPoint.y), tempPos2=logicalPoint;
		if( isHanKana(pobjLine->GetAt(lCurrentOffset)) )//半角ｶﾀｶﾅ
		{
			// キャレット左方向移動先を検索
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // 前へ位置づけ
				if(lCnt == -1)
					break;

				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					break;
				}
			}
		}
		else if( ( isAlnum(pobjLine->GetAt(lCurrentOffset)) || (pobjLine->GetAt(lCurrentOffset)=='_')
				 ) &&
				 !isHanKana(pobjLine->GetAt(lCurrentOffset))
		)
		{
			// キャレット左方向移動先を検索
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // 前へ位置づけ
				if(lCnt == -1)
					break;

				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt))	||
					( !isAlnum(pobjLine->GetAt(lCnt)) &&
					  (pobjLine->GetAt(lCnt)!='_')
					)											||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					break;
				}
			}
		}
		else
		{
			// キャレット左方向移動先を検索
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // 前へ位置づけ
				if(lCnt == -1)
					break;

				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) 		||
					isAlnum(pobjLine->GetAt(lCnt))					||
					(pobjLine->GetAt(lCnt)=='_')					||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					break;
				}
			}
		}

		objCaretPos = tempPos;
		//論理座標－＞デバイス座標
		objCaretPos.x -= GetScrollPosLogX();
		objCaretPos.y -= GetScrollPosLogY();
		objCaretPos += m_RulerPos;
		objCaretPos += m_LineNoPos;
		if(objCaretPos.x < m_LineNoPos.x)//表示範囲の左側へ行った場合
		{
			//左方向スクロール
			long lLogX = objCaretPos.x - m_LineNoPos.x - m_RulerPos.x + GetScrollPosLogX();
			MyScrollToPosition(CPoint(lLogX, GetScrollPosLogY()));
			objCaretPos.x = m_LineNoPos.x;
		}

		MySetCaretPos(objCaretPos);		//キャレット位置設定（クライアント座標系）
	}
	else if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //キャレット位置が全角文字
	{
		CPoint tempPos(0,logicalPoint.y), tempPos2=logicalPoint;

		//文字コード取得
		UINT unChar = ( (pobjLine->GetAt(lCurrentOffset) << 8) | (pobjLine->GetAt(lCurrentOffset+1)) );
		if( _ismbchira( unChar ) ) //キャレット位置がﾀﾞﾌﾞﾙﾊﾞｲﾄひらがな
		{
			// キャレット左方向移動先を検索
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // 前へ位置づけ
				if(lCnt == -1)
					break;

				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!_ismbchira( unCharTmp )
				)
				{
					break;
				}
			}
		}
		else if( (_ismbckata(unChar)) ||
				 (unChar==0x815B)			//キャレット位置がﾀﾞﾌﾞﾙﾊﾞｲﾄかたかなｏｒ”ー”
		)
		{
			// キャレット左方向移動先を検索
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // 前へ位置づけ
				if(lCnt == -1)
					break;

				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbckata( unCharTmp ) && (unCharTmp!=0x815B) )
				)
				{
					break;
				}
			}
		}
		else if( _ismbcl1(unChar) || _ismbcl2(unChar) ) //キャレット位置がﾀﾞﾌﾞﾙﾊﾞｲﾄ漢字
		{
			//キャレット左方向移動先を検索
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // 前へ位置づけ
				if(lCnt == -1)
					break;

				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbcl1( unCharTmp ) && !_ismbcl2( unCharTmp ) )
				)
				{
					break;
				}
			}
		}
		else //その他の２バイト文字
		{
			//キャレット左方向移動先を検索
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // 前へ位置づけ
				if(lCnt == -1)
					break;

				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( _ismbchira( unCharTmp ) ||
					  _ismbckata(unCharTmp)   ||
					  (unCharTmp==0x815B)     ||
					  _ismbcl1( unCharTmp )   ||
					  _ismbcl2( unCharTmp )
					)
				)
				{
					break;
				}
			}
		}

		objCaretPos = tempPos;
		//論理座標－＞デバイス座標
		objCaretPos.x -= GetScrollPosLogX();
		objCaretPos.y -= GetScrollPosLogY();
		objCaretPos += m_RulerPos;
		objCaretPos += m_LineNoPos;
		if(objCaretPos.x < m_LineNoPos.x)//表示範囲の左側へ行った場合
		{
			//左方向スクロール
			long lLogX = objCaretPos.x - m_LineNoPos.x - m_RulerPos.x + GetScrollPosLogX();
			MyScrollToPosition(CPoint(lLogX, GetScrollPosLogY()));
			objCaretPos.x = m_LineNoPos.x;
		}

		MySetCaretPos(objCaretPos);		//キャレット位置設定（クライアント座標系）
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Ｃｔｒｌ＋Ｓｈｉｆｔ＋右矢印キー押下時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnWordselect2right() 
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// キー操作の記録中チェック
	if(pWnd->IsNowOperationRecording())
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONWORDSELECT2RIGHT);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	CPoint	objCaretPos = m_caretPos;
	//キャレット位置を論理座標へ変換する
	CPoint	logicalPoint = objCaretPos;
	logicalPoint -= m_LineNoPos;
	logicalPoint -= m_RulerPos;
	logicalPoint.x += GetScrollPosLogX();
	logicalPoint.y += GetScrollPosLogY();

	CRect clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	if( (m_lfWidth==0) || (m_lfHeight==0) )
		return;
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	long	lCnt=0;
	long	lEndOffset = lCurrentOffset;
	CLine*	pobjLine = NULL;
	long lMaxLine       = pDoc->GetLineCount();

	//キャレット移動するまえの位置を記憶
	m_lPreMoveCaretLine   = lCurrentLine;
	m_lPreMoveCaretOffset = lCurrentOffset;
	if( (m_lSelectStartLine == -1) || (m_lSelectStartOffset == -1) )
	{
		//選択開始位置を記憶
		m_lSelectStartLine   = lCurrentLine;
		m_lSelectStartOffset = lCurrentOffset;
		m_lSelectEndLine     = -1;
		m_lSelectEndOffset   = -1;
	}

	//キャレット位置の行取得
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
		return;

	//////////////////////////////////////
	//		キャレット移動先設定		//
	//////////////////////////////////////
	if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) && //キャレット位置が半角
		pobjLine->GetAt(lCurrentOffset) != 0
	)
	{
		int nMargin=0;
		if( isHanKana(pobjLine->GetAt(lCurrentOffset)) )//半角ｶﾀｶﾅ
		{
			// キャレット右方向移動先を検索
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
				}
			}
		}
		else if( ( isAlnum(pobjLine->GetAt(lCurrentOffset)) || (pobjLine->GetAt(lCurrentOffset)=='_')
				 ) &&
				 !isHanKana(pobjLine->GetAt(lCurrentOffset))
		)
		{
			// キャレット右方向移動先を検索
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt))	||
					( !isAlnum(pobjLine->GetAt(lCnt)) &&
					  (pobjLine->GetAt(lCnt)!='_')
					)											||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
				}
			}
		}
		else
		{
			// キャレット右方向移動先を検索
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) 		||
					isAlnum(pobjLine->GetAt(lCnt))					||
					(pobjLine->GetAt(lCnt)=='_')					||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
				}
			}
		}

		if( logicalPoint.x + nMargin > nWidth ) //表示範囲の右側へ行った場合
		{
			//右方向スクロール
			MyScrollToPosition(CPoint(GetScrollPosLogX()+nMargin, GetScrollPosLogY()));
		}
		//キャレット位置設定
		MySetCaretToLineOffset(lCurrentLine,lEndOffset);
	}
	else if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //キャレット位置が全角文字
	{
		int nMargin=0;

		//文字コード取得
		UINT unChar = ( (pobjLine->GetAt(lCurrentOffset) << 8) | (pobjLine->GetAt(lCurrentOffset+1)) );
		if( _ismbchira( unChar ) ) //キャレット位置がﾀﾞﾌﾞﾙﾊﾞｲﾄひらがな
		{
			// キャレット右方向移動先を検索
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!_ismbchira( unCharTmp )
				)
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
				}
			}
		}
		else if( (_ismbckata(unChar)) ||
				 (unChar==0x815B)			//キャレット位置がﾀﾞﾌﾞﾙﾊﾞｲﾄかたかなｏｒ”ー”
		)
		{
			// キャレット右方向移動先を検索
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbckata( unCharTmp ) && (unCharTmp!=0x815B) )
				)
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
				}
			}
		}
		else if( _ismbcl1(unChar) || _ismbcl2(unChar) ) //キャレット位置がﾀﾞﾌﾞﾙﾊﾞｲﾄ漢字
		{
			// キャレット右方向移動先を検索
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbcl1( unCharTmp ) && !_ismbcl2( unCharTmp ) )
				)
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
				}
			}
		}
		else //その他の２バイト文字
		{
			// キャレット右方向移動先を検索
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( _ismbchira( unCharTmp ) ||
					  _ismbckata(unCharTmp)   ||
					  (unCharTmp==0x815B)     ||
					  _ismbcl1( unCharTmp )   ||
					  _ismbcl2( unCharTmp )
					)
				)
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
				}
			}
		}

		if( logicalPoint.x + nMargin > nWidth ) //表示範囲の右側へ行った場合
		{
			//右方向スクロール
			MyScrollToPosition(CPoint(GetScrollPosLogX()+nMargin, GetScrollPosLogY()));
		}
		//キャレット位置設定
		MySetCaretToLineOffset(lCurrentLine,lEndOffset);
	}
	else //キャレット位置が行末
	{
		if(lCurrentLine+1 < lMaxLine) //まだ下へ行ける
		{
			//MyScrollToPositionを１回のみコールするため
			CPoint	objCaretPos = m_caretPos;
			CPoint	destPos(GetScrollPosLogX(),GetScrollPosLogY());
			if(GetScrollPosLogX() > 0)
			{
				//左スクロール
				destPos.x = 0;
			}
			//キャレットを次の行へ
			if((objCaretPos.y+(m_lfHeight+m_lLineInterval)*2) > nHeight)//表示範囲の下側に行った場合
			{
				//１行分下方向スクロール
				destPos.y += (m_lfHeight+m_lLineInterval);
			}

			if( destPos.x != GetScrollPosLogX() ||
				destPos.y != GetScrollPosLogY()
			)
			{
				MyScrollToPosition( destPos );
			}

			lCurrentLine++;
			lEndOffset=0;
			//キャレット位置設定
			MySetCaretToLineOffset(lCurrentLine,lEndOffset);
		}
	}

	// 選択範囲の再描画処理
	RedrawSelectArea(m_lPreMoveCaretLine,m_lPreMoveCaretOffset);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Ｃｔｒｌ＋Ｓｈｉｆｔ＋左矢印キー押下時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnWordselect2left() 
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// キー操作の記録中チェック
	if(pWnd->IsNowOperationRecording())
	{
		//キーオペレーションオブジェクト作成
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONWORDSELECT2LEFT);
		//記録
		pWnd->addOperation(pobjOperation);
	}

	CPoint	objCaretPos = m_caretPos;
	//キャレット位置を論理座標へ変換する
	CPoint	logicalPoint = objCaretPos;
	logicalPoint -= m_LineNoPos;
	logicalPoint -= m_RulerPos;
	logicalPoint.x += GetScrollPosLogX();
	logicalPoint.y += GetScrollPosLogY();

	CRect clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	if( (m_lfWidth==0) || (m_lfHeight==0) )
		return;
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	long	lCnt=0;
	CLine*	pobjLine = NULL;

	//キャレット移動するまえの位置を記憶
	m_lPreMoveCaretLine   = lCurrentLine;
	m_lPreMoveCaretOffset = lCurrentOffset;
	if( (m_lSelectStartLine == -1) || (m_lSelectStartOffset == -1) )
	{
		//選択開始位置を記憶
		m_lSelectStartLine   = lCurrentLine;
		m_lSelectStartOffset = lCurrentOffset;
		m_lSelectEndLine     = -1;
		m_lSelectEndOffset   = -1;
	}

	//ひとつ前へ
	lCurrentOffset = GetPreOffset(lCurrentLine,lCurrentOffset);

	//キャレット位置の行取得
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
		return;

	//////////////////////////////////////
	//		キャレット移動先設定		//
	//////////////////////////////////////
	if(lCurrentOffset == -1)
	{
		if(lCurrentLine > 0)//まだ上へ行ける
		{
			//MyScrollToPotisionを１回のコールにするための処理
			CPoint	destPos(GetScrollPosLogX(),GetScrollPosLogY());
			//キャレットを文字列の最後尾へ
			CPoint	tempLogPos(0,logicalPoint.y-(m_lfHeight+m_lLineInterval));
			GetPosition(lCurrentLine-1,&tempLogPos);//行末の位置取得
			if(tempLogPos.x > nWidth)//表示範囲の右側へ行った場合
			{
				//右方向スクロール
				long lLogX = tempLogPos.x - nWidth + 1;
				destPos.x = lLogX;
			}
			//論理座標－＞デバイス座標
			objCaretPos = tempLogPos;
			objCaretPos -= destPos;/*m_ScrollPosLog;*/
			objCaretPos += m_RulerPos;
			objCaretPos += m_LineNoPos;
			if(objCaretPos.y < m_RulerPos.y )//表示範囲の上側に行った場合
			{
				objCaretPos.y = m_RulerPos.y;
				//１行分上方向スクロール
				destPos.y -= (m_lfHeight+m_lLineInterval);
			}
			if( destPos.x != GetScrollPosLogX() ||
				destPos.y != GetScrollPosLogY()
			)
			{
				MyScrollToPosition(destPos);
			}
		}

		//キャレット位置設定
		MySetCaretPos(objCaretPos);		//キャレット位置設定（クライアント座標系）
	}
	else if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //キャレット位置が半角
	{
		CPoint tempPos(0,logicalPoint.y), tempPos2=logicalPoint;
		if( isHanKana(pobjLine->GetAt(lCurrentOffset)) )//半角ｶﾀｶﾅ
		{
			// キャレット左方向移動先を検索
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // 前へ位置づけ
				if(lCnt == -1)
					break;

				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					break;
				}
			}
		}
		else if( ( isAlnum(pobjLine->GetAt(lCurrentOffset)) || (pobjLine->GetAt(lCurrentOffset)=='_')
				 ) &&
				 !isHanKana(pobjLine->GetAt(lCurrentOffset))
		)
		{
			// キャレット左方向移動先を検索
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // 前へ位置づけ
				if(lCnt == -1)
					break;

				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt))	||
					( !isAlnum(pobjLine->GetAt(lCnt)) &&
					  (pobjLine->GetAt(lCnt)!='_')
					)											||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					break;
				}
			}
		}
		else
		{
			// キャレット左方向移動先を検索
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // 前へ位置づけ
				if(lCnt == -1)
					break;

				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) 		||
					isAlnum(pobjLine->GetAt(lCnt))					||
					(pobjLine->GetAt(lCnt)=='_')					||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					break;
				}
			}
		}

		objCaretPos = tempPos;
		//論理座標－＞デバイス座標
		objCaretPos.x -= GetScrollPosLogX();
		objCaretPos.y -= GetScrollPosLogY();
		objCaretPos += m_RulerPos;
		objCaretPos += m_LineNoPos;
		if(objCaretPos.x < m_LineNoPos.x)//表示範囲の左側へ行った場合
		{
			//左方向スクロール
			long lLogX = objCaretPos.x - m_LineNoPos.x - m_RulerPos.x + GetScrollPosLogX();
			MyScrollToPosition(CPoint(lLogX, GetScrollPosLogY()));
			objCaretPos.x = m_LineNoPos.x;
		}

		MySetCaretPos(objCaretPos);		//キャレット位置設定（クライアント座標系）
	}
	else if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //キャレット位置が全角文字
	{
		CPoint tempPos(0,logicalPoint.y), tempPos2=logicalPoint;

		//文字コード取得
		UINT unChar = ( (pobjLine->GetAt(lCurrentOffset) << 8) | (pobjLine->GetAt(lCurrentOffset+1)) );
		if( _ismbchira( unChar ) ) //キャレット位置がﾀﾞﾌﾞﾙﾊﾞｲﾄひらがな
		{
			// キャレット左方向移動先を検索
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // 前へ位置づけ
				if(lCnt == -1)
					break;

				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!_ismbchira( unCharTmp )
				)
				{
					break;
				}
			}
		}
		else if( (_ismbckata(unChar)) ||
				 (unChar==0x815B)			//キャレット位置がﾀﾞﾌﾞﾙﾊﾞｲﾄかたかなｏｒ”ー”
		)
		{
			// キャレット左方向移動先を検索
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // 前へ位置づけ
				if(lCnt == -1)
					break;

				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbckata( unCharTmp ) && (unCharTmp!=0x815B) )
				)
				{
					break;
				}
			}
		}
		else if( _ismbcl1(unChar) || _ismbcl2(unChar) ) //キャレット位置がﾀﾞﾌﾞﾙﾊﾞｲﾄ漢字
		{
			// キャレット左方向移動先を検索
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // 前へ位置づけ
				if(lCnt == -1)
					break;

				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbcl1( unCharTmp ) && !_ismbcl2( unCharTmp ) )
				)
				{
					break;
				}
			}
		}
		else //その他の２バイト文字
		{
			// キャレット左方向移動先を検索
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // 前へ位置づけ
				if(lCnt == -1)
					break;

				//文字コード取得
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( _ismbchira( unCharTmp ) ||
					  _ismbckata(unCharTmp)   ||
					  (unCharTmp==0x815B)     ||
					  _ismbcl1( unCharTmp )   ||
					  _ismbcl2( unCharTmp )
					)
				)
				{
					break;
				}
			}
		}

		objCaretPos = tempPos;
		//論理座標－＞デバイス座標
		objCaretPos.x -= GetScrollPosLogX();
		objCaretPos.y -= GetScrollPosLogY();
		objCaretPos += m_RulerPos;
		objCaretPos += m_LineNoPos;
		if(objCaretPos.x < m_LineNoPos.x)//表示範囲の左側へ行った場合
		{
			//左方向スクロール
			long lLogX = objCaretPos.x - m_LineNoPos.x - m_RulerPos.x + GetScrollPosLogX();
			MyScrollToPosition(CPoint(lLogX, GetScrollPosLogY()));
			objCaretPos.x = m_LineNoPos.x;
		}

		MySetCaretPos(objCaretPos);		//キャレット位置設定（クライアント座標系）
	}

	// 選択範囲の再描画処理
	RedrawSelectArea(m_lPreMoveCaretLine,m_lPreMoveCaretOffset);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（ツール関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＵＵＤｅｃｏｄｅ選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditUudecode() 
{
	CEditorDoc* pDoc = (CEditorDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	/* 選択領域をＵＵＤｅｃｏｄｅ */
	pDoc->UuDecode( m_lSelectStartLine,
					m_lSelectStartOffset,
					m_lSelectEndLine,
					m_lSelectEndOffset );
}
void CEditorView::OnUpdateEditUudecode(CCmdUI* pCmdUI) 
{
	if(IsSelected())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Ｂａｓｅ６４デコード選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditBase64decode() 
{
	CEditorDoc* pDoc = (CEditorDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	/* 選択領域をＢａｓｅ６４デコード */
	pDoc->Base64Decode( m_lSelectStartLine,
						m_lSelectStartOffset,
						m_lSelectEndLine,
						m_lSelectEndOffset );
}
void CEditorView::OnUpdateEditBase64decode(CCmdUI* pCmdUI) 
{
	if(IsSelected())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Ｑｕｏｔｅｄ－Ｐｒｉｎｔａｂｌｅデコード選択時処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditQuoteddecode() 
{
	CEditorDoc* pDoc = (CEditorDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	//選択領域をＱｕｏｔｅｄ－Ｐｒｉｎｔａｂｌｅデコード
	pDoc->QuotedPrintableDecode( m_lSelectStartLine,
								 m_lSelectStartOffset,
								 m_lSelectEndLine,
								 m_lSelectEndOffset );
}
void CEditorView::OnUpdateEditQuoteddecode(CCmdUI* pCmdUI) 
{
	if(IsSelected())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（ファイル比較関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「次の相違個所へジャンプ」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditNextdiff() 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	LONG	lLineCnt=0;
	int		nDiffLineKindNow = DIFF_LINEKIND_COMMON;
	BOOL 	bFind=FALSE;
	CString objMsg;

	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	// 現在行のファイル比較種別取得
	nDiffLineKindNow = pDoc->GetDiffLineKind(lCurrentLine);
	if(nDiffLineKindNow == DIFF_LINEKIND_COMMON)
	{
		//次の相違個所の行番号を検索
		bFind = FALSE;
		for(lLineCnt=lCurrentLine+1; lLineCnt<pDoc->GetLineCount(); lLineCnt++)
		{
			if( pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_DELETED ||
				pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_ADDED
			)
			{
				bFind=TRUE;
				break;
			}
		}
	}
	else
	{
		//次の共通個所の行番号を検索
		bFind = FALSE;
		for(lLineCnt=lCurrentLine+1; lLineCnt<pDoc->GetLineCount(); lLineCnt++)
		{
			if( pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_COMMON )
			{
				bFind=TRUE;
				break;
			}
		}
		if(bFind != FALSE)
		{
			bFind = FALSE;
			lLineCnt++;
			//次の相違個所の行番号を検索
			for( ; lLineCnt<pDoc->GetLineCount(); lLineCnt++)
			{
				if( pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_DELETED ||
					pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_ADDED
				)
				{
					bFind=TRUE;
					break;
				}
			}
		}
	}

	if(bFind != FALSE)
	{
		//次の相違個所へジャンプ
		Jump2LineOffset(lLineCnt,0);
		//再描画
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);
	}
	else
	{
		//もうない
		objMsg.LoadString(IDS_NOMORE_NEXTDIFF);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
	}
}
void CEditorView::OnUpdateEditNextdiff(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ドキュメントに１行もデータがない場合は選択不可にする
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ファイル比較結果かどうか
	if(pDoc->IsCompare() == FALSE)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「前の相違個所へジャンプ」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditPrevdiff() 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	LONG	lLineCnt=0;
	int		nDiffLineKindNow = DIFF_LINEKIND_COMMON;
	BOOL 	bFind=FALSE;
	CString objMsg;

	// 現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	// 現在行のファイル比較種別取得
	nDiffLineKindNow = pDoc->GetDiffLineKind(lCurrentLine);
	if( nDiffLineKindNow == DIFF_LINEKIND_DELETED || nDiffLineKindNow == DIFF_LINEKIND_ADDED )
	{
		//前の共通個所の行まで移動
		for(lLineCnt=lCurrentLine-1; lLineCnt>=0; lLineCnt--)
		{
			if( pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_COMMON )
			{
				lCurrentLine = lLineCnt;
				nDiffLineKindNow = DIFF_LINEKIND_COMMON;
				break;
			}
		}
	}
	if(nDiffLineKindNow == DIFF_LINEKIND_COMMON)
	{
		//前の相違個所の行番号を検索
		bFind = FALSE;
		for(lLineCnt=lCurrentLine-1; lLineCnt>=0; lLineCnt--)
		{
			if( pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_DELETED ||
				pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_ADDED
			)
			{
				bFind=TRUE;
				break;
			}
		}
		if(bFind != FALSE)
		{
			bFind = FALSE;
			lLineCnt--;
			for( ; lLineCnt>=-1; lLineCnt--)
			{
				if( lLineCnt <= -1 )
				{
					lLineCnt = 0;
					bFind = TRUE;
					break;
				}
				if( pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_COMMON )
				{
					lLineCnt++;
					bFind = TRUE;
					break;
				}
			}
		}
	}

	if(bFind != FALSE)
	{
		//前の相違個所へジャンプ
		Jump2LineOffset(lLineCnt,0);
		//再描画
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);
	}
	else
	{
		//もうない
		objMsg.LoadString(IDS_NOMORE_PREVDIFF);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
	}
}
void CEditorView::OnUpdateEditPrevdiff(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ドキュメントに１行もデータがない場合は選択不可にする
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ファイル比較結果かどうか
	if(pDoc->IsCompare() == FALSE)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView メッセージハンドラ（ディレクトリ比較関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「相違ファイルをさらに詳細に比較」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditDircmpSyosai() 
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	// キャレット位置の行取得
	CLine*	pobjLine = NULL;
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
		return;

	//比較ディレクトリ名取得
	CString objDir1,objDir2,objFileName;
	pDoc->GetDirCmpFolder(objDir1,objDir2);
	//相違ファイル名取得
	pobjLine->GetDirCmpFileName(objFileName);
	//パス作成
	if(m_pApp->IsDriveRoot(objDir1))
		objDir1 += objFileName;
	else
		objDir1 += "\\" + objFileName;
	if(m_pApp->IsDriveRoot(objDir2))
		objDir2 += objFileName;
	else
		objDir2 += "\\" + objFileName;

	pWnd->DoFileCompare(objDir1,objDir2,NULL);
}
void CEditorView::OnUpdateEditDircmpSyosai(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//印刷中チェック
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ディレクトリ比較結果チェック
	if(pDoc->IsDirCmp() == FALSE)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//	現在のキャレット位置の行、列番号取得
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	// キャレット位置の文字取得
	if(pDoc->GetDirCmpLineKind(lCurrentLine)==DIRCMP_LINEKIND_DIFF) //ディレクトリ比較結果相違ファイル
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}


// MainFrm.h : CMainFrame クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MAINFRM_H)		//２重インクルード防止
#define MAINFRM_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ToolBarEx.h"
#include "BCMenu.h"

class CEditorApp;
class CEditorDoc;
class CGrepStatusDlg;
class CCodeConvStatusDlg;
class CKaigyoConvStatusDlg;
class CFileSearchStatusDlg;
class CWCntStatusDlg;
class CBitmapClient;
class COperation;
class COperationManager;
class CEncriptStatusDlg;
class CDecriptStatusDlg;
class CFileCompareStatusDlg;
class CDirCompareStatusDlg;
class CFileTouchDlg;
class CMailListStatusDlg;
class CHtmlOpenStatusDlg;

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();										// コンストラクタ
	BOOL SetStatusBarDispInfo(LPSTATUSBARDISP);			// ステータスバー表示項目情報設定
	BOOL IsDispIndicatorByte();							// キャレット位置バイトを表示するかのチェック処理
	BOOL IsDispIndicatorGyoretu();						// キャレット位置行、列を表示するかのチェック処理
	BOOL IsDispIndicatorCode();							// キャレット位置コードを表示するかのチェック処理
	void SetSaveWindowPlace(BOOL);						// ウインドウ位置保存フラグ設定
	void SetStatusMessage(char*);						// ステータスペインメッセージ設定
	void SetStatusMessage(CString&);					// ステータスペインメッセージ設定
	void SetGyoRetu(long,long,long);					// 行、列番号のステータスペイン設定
	void ClearGyoRetu();								// 行、列番号のステータスペインクリア
	void SetInsMode(BOOL);								// 挿入、上書きモードステータスペイン設定
	void ClearInsMode();								// 挿入、上書きモードステータスペインクリア
	void SetReadOnlyMode(BOOL);							// 書込み禁止モードステータスペイン設定
	void ClearReadOnlyMode();							// 書込み禁止モードステータスペインクリア
	void SetDateTime();									// 日付、時刻ステータスペイン設定
	void ClearDateTime();								// 日付、時刻ステータスペインクリア
	void SetCaretPosCharCode(UCHAR,UCHAR);				// キャレット位置コードステータスペイン設定
	void ClearCaretPosCharCode();						// キャレット位置コードステータスペインクリア
	void SetCaretPosByte(long,long);					// キャレット位置バイトステータスペイン設定
	void ClearCaretPosByte();							// キャレット位置バイトステータスペインクリア
	void SetStatusBarFont(LOGFONT);						// ステータスバーフォント設定
	void SetFlagAutoSave(BOOL);							// オートセーブフラグ設定
	void SetSaveInterval(long);							// オートセーブ間隔設定
	void SetMDIBkgndColor(COLORREF);					// ＭＤＩ背景色設定
	void SetFlagDispLogoBmp(BOOL);						// ロゴビットマップ表示フラグ設定
	void SetLogoBmpKind(int);							// ロゴビットマップ種別設定
	void SetLogoPosKind(int);							// ロゴ表示位置種別設定
	void SetLogoBmpFilePath(CString);					// ロゴビットマップファイルパス設定処理
	void SetLogoPosX(UINT);								// ロゴ表示位置Ｘ設定
	void SetLogoPosY(UINT);								// ロゴ表示位置Ｙ設定
	BOOL IsNowOperationRecording();						// キー操作記録中調査
	BOOL IsNowStepDoOperation();						// キー操作ステップ実行中調査
	void addOperation(COperation*);						// キー操作記録
	long GetStatusDayStyle();							// ステータスバーへ表示する日付表示スタイル取得
	void SetStatusDayStyle(long);						// ステータスバーへ表示する日付表示スタイル設定
	long GetStatusHourStyle();							// ステータスバーへ表示する時刻表示スタイル取得
	void SetStatusHourStyle(long);						// ステータスバーへ表示する時刻表示スタイル設定
	void SetStatusLineStyle(long);						// ステータスバーへ表示する行、列表示スタイル設定
	BOOL IsNowIncrimentalSearching();					// インクリメンタルサーチ実行中調査
	void SetFlagIncrimentalSearching(BOOL);				// インクリメンタルサーチ実行中フラグ設定
	void DoFileCompare(CString&,CString&,CEditorDoc*);	// ファイル比較実行処理
	void SetFindParameter(CString&,BOOL,BOOL,BOOL,BOOL);// 検索パラメータ設定
	void SetReplaceParameter(CString&,CString&,BOOL,BOOL,BOOL);// 置換パラメータ設定
	BOOL IsHaveChildWnd();
	void SetHtmlOpenParameter(BOOL,CString&,BOOL,CString&,CString&,CString&,BOOL);

	//ビットマップ付きメニュー用
	HMENU NewMenu();
	HMENU NewDefaultMenu();
	BCMenu m_menu,m_default;

	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	CEditorApp*				m_pApp;							// アプリケーションクラスへのポインタ

	// ウインドウ位置関係
	BOOL					m_bIsSaveWindowPlace;			// ウインドウ位置を保存する

	// WEB上のHTMLファイルを開く関連
	BOOL					m_bIsUseProxyHtml;				// プロキシサーバ経由か？
	CString					m_objRequestPathHtml;			// 要求パス
	CString					m_objProxyIPAddressHtml;		// プロキシサーバのＩＰアドレス
	BOOL					m_bIsDialupHtml;				// ダイアルアップ接続するか？
	CString					m_objDialupEntryHtml;			// 使用するダイアルアップエントリ
	CString					m_objRasUserNameHtml;			// ユーザ名
	CString					m_objRasPasswdHtml;				// パスワード
	BOOL					m_bIsAutoDisconnectHtml;		// 自動切断するか？
	CHtmlOpenStatusDlg*		m_pobjHtmlOpenStatusDlg;		// 状況ダイアログ

	// 検索関連
	CString					m_objFindString;				// 検索文字列
	BOOL					m_bIsMatchCaseFind;				// 大文字、小文字区別の有無
	BOOL					m_bIsMatchWholeWordFind;		// 単語全体が一致する事が条件か？
	BOOL					m_bIsSearchDownFind;			// 検索方向
	BOOL					m_bIsNowIncrimentalSearching;	// インクリメンタルサーチ実行中フラグ
	BOOL					m_bIsUseRegularExpressionFind;	// 正規表現を使用するか？

	// 置換関連
	CString					m_objReplaceString;				// 置換文字列
	BOOL					m_bIsMatchCaseReplace;			// 大文字、小文字区別の有無
	BOOL					m_bIsMatchWholeWordReplace;		// 単語全体が一致する事が条件か？
	BOOL					m_bIsUseRegularExpressionReplace;// 正規表現を使用するか？

	// ＧＲＥＰ関連
	BOOL					m_bIsMatchCaseGrep;				// 大文字、小文字区別の有無
	BOOL					m_bIsMatchWholeWordGrep;		// 単語全体が一致する事が条件か？
	BOOL					m_bIsSearchSubFolderGrep;		// サブフォルダも検索するか？
	BOOL					m_bIsUseRegularExpressionGrep;	// 正規表現を使用するか？
	CString					m_objFolderGrep;				// ＧＲＥＰ実行フォルダ
	CString					m_objGrepStr;					// 検索文字列
	CString					m_objFileKindGrep;				// 検索対象のファイル種別
	CEditorDoc*				m_pobjGrepDoc;					// ＧＲＥＰ用ドキュメントクラス
	CFrameWnd*				m_pobjGrepFrameWnd;				// ＧＲＥＰ用子ウインドウクラス
	CGrepStatusDlg*			m_pobjGrepStatusDlg;			// ＧＲＥＰ状況ダイアログ

	// 文字コード変換関連
	BOOL					m_bIsSearchSubFolderConv;		// サブフォルダも変換するか？
	BOOL					m_bIsOutOtherFileConv;			// 変換結果を別ファイルへ出力するか？
	int						m_nConvKind;					// 文字コード変換種別
	CString					m_objFolderConv;				// 文字コード変換実行フォルダ
	CString					m_objFileKindConv;				// 変換対象のファイル種別
	int						m_nInCodeConv;					// 入力ファイルの文字コード判定種別
	CEditorDoc*				m_pobjConvDoc;					// 文字コード変換用ドキュメントクラス
	CFrameWnd*				m_pobjConvFrameWnd;				// 文字コード変換用ウインドウクラス
	CCodeConvStatusDlg*		m_pobjConvStatusDlg;			// 文字コード変換状況ダイアログ

	// 改行コード変換関連
	BOOL					m_bIsSearchSubFolderKaigyoConv;	// サブフォルダも変換するか？
	BOOL					m_bIsOutOtherFileKaigyoConv;	// 変換結果を別ファイルへ出力するか？
	CString					m_objKaigyoConvKind;			// 改行コード変換種別
	CString					m_objFolderKaigyoConv;			// 改行コード変換実行フォルダ
	CString					m_objFileKindKaigyoConv;		// 変換対象のファイル種別
	CEditorDoc*				m_pobjKaigyoConvDoc;			// 改行コード変換用ドキュメントクラス
	CFrameWnd*				m_pobjKaigyoConvFrameWnd;		// 改行コード変換用ウインドウクラス
	CKaigyoConvStatusDlg*	m_pobjKaigyoConvStatusDlg;		// 改行コード変換状況ダイアログ

	// ファイル暗号化関連
	BOOL					m_bIsSearchSubFolderEncript;	// サブフォルダも変換するか？
	BOOL					m_bIsOutOtherFileEncript;		// 暗号結果を別ファイルへ出力するか？
	CString					m_objKeyEncript;				// ファイル暗号化キー
	CString					m_objFolderEncript;				// ファイル暗号化実行フォルダ
	CString					m_objFileKindEncript;			// ファイル暗号化対象のファイル種別
	CEditorDoc*				m_pobjEncriptDoc;				// ファイル暗号化用ドキュメントクラス
	CFrameWnd*				m_pobjEncriptFrameWnd;			// ファイル暗号化用ウインドウクラス
	CEncriptStatusDlg*		m_pobjEncriptStatusDlg;			// ファイル暗号化状況ダイアログ

	// ファイル復号化関連
	BOOL					m_bIsSearchSubFolderDecript;	// サブフォルダも変換するか？
	BOOL					m_bIsOutOtherFileDecript;		// 復号結果を別ファイルへ出力するか？
	CString					m_objKeyDecript;				// ファイル復号化キー
	CString					m_objFolderDecript;				// ファイル復号化実行フォルダ
	CString					m_objFileKindDecript;			// ファイル復号化対象のファイル種別
	CEditorDoc*				m_pobjDecriptDoc;				// ファイル復号化用ドキュメントクラス
	CFrameWnd*				m_pobjDecriptFrameWnd;			// ファイル復号化用ウインドウクラス
	CDecriptStatusDlg*		m_pobjDecriptStatusDlg;			// ファイル復号化状況ダイアログ

	// ダンプ関連
	CEditorDoc*				m_pobjDumpDoc;					// ダンプ用ドキュメントクラス
	CFrameWnd*				m_pobjDumpFrameWnd;				// ダンプ用ウインドウクラス

	// ワードカウント関連
	long					m_lByteCountSum;				// バイト数合計
	long					m_lWordCountSum;				// ワード数合計
	long					m_lLineCountSum;				// 行数合計
	long					m_lEmptyLineCountSum;			// 空行数合計
	long					m_lCPPCommentLineCountSum;		// Ｃ・Ｃ＋＋コメント行数合計
	long					m_lWCountFileCnt;				// ワードカウント対象ファイル数
	BOOL					m_bIsSearchSubFolderWCount;		// サブフォルダも検索するか？
	CString					m_objFolderWCount;				// ワードカウント実行フォルダ
	CString					m_objFileKindWCount;			// ワードカウント対象のファイル種別
	CEditorDoc*				m_pobjWCountDoc;				// ワードカウント用ドキュメントクラス
	CFrameWnd*				m_pobjWCountFrameWnd;			// ワードカウント用子ウインドウクラス
	CWCntStatusDlg*			m_pobjWCntStatusDlg;			// ワードカウント状況ダイアログ

	// ファイル検索関連
	BOOL					m_bIsSearchSubFolderFSearch;	// サブフォルダも変換するか？
	CString					m_objFolderFSearch;				// ファイル検索変換実行フォルダ
	CString					m_objFileKindFSearch;			// 検索対象のファイル種別
	CEditorDoc*				m_pobjFSearchDoc;				// ファイル検索用ドキュメントクラス
	CFrameWnd*				m_pobjFSearchFrameWnd;			// ファイル検索用ウインドウクラス
	CFileSearchStatusDlg*	m_pobjFileSearchStatusDlg;		// ファイル検索状況ダイアログ

	// ファイルタッチ関連
	BOOL					m_bIsSearchSubFolderTouch;		// サブフォルダもタッチするか？
	CString					m_objFolderTouch;				// タッチ実行フォルダ
	CString					m_objFileKindTouch;				// タッチ対象のファイル種別
	CTime					m_objFileTouchTime;				// タッチ日時

	// オートセーブ関連
	BOOL					m_bIsAutoSave;					// オートセーブをするか？
	long					m_lSaveInterval;				// オートセーブ間隔（分単位）
	long					m_lTimeUpCount;					// タイムアップカウンタ

	// キー操作の記録／再生関連
	BOOL					m_bIsNowOperationRecording;		// キー操作記録中フラグ
	BOOL					m_bIsNowStepDoOperation;		// キー操作のステップ実行中フラグ
	COperationManager		m_objOperationManager;			// キー操作マネージャ

	// ステータスバー関連
	LOGFONT					m_stStatusBarLogFont;			// ステータスバーのLOGFONT構造体
	CFont					m_objStatusBarFont;				// ステータスバーのフォント
	STATUSBARDISP			m_stStatusBarDisp;				// ステータスバー表示情報構造体
	long					m_lStatusDayStyle;				// ステータスバーへ表示する日付表示スタイル
	long					m_lStatusHourStyle;				// ステータスバーへ表示する時刻表示スタイル
	long					m_lStatusLineStyle;				// ステータスバーへ表示する行、列表示スタイル
	long					m_lSavedGyo;					// 行番号
	long					m_lSavedRetu;					// 列番号
	long					m_lSavedMaxGyo;					// 全行数

	// ファイル比較間連
	CFileCompareStatusDlg*	m_pobjFileCompareStatusDlg;		// ファイル比較状況ダイアログ

	// ディレクトリ比較関連
	CDirCompareStatusDlg*	m_pobjDirCompareStatusDlg;		// ディレクトリ比較状況ダイアログ
	CString					m_objFolder1DirCmp;				// 第一比較フォルダ
	CString					m_objFolder2DirCmp;				// 第二比較フォルダ
	CEditorDoc*				m_pobjDirCmpDoc;				// ディレクトリ比較用ドキュメントクラス
	CFrameWnd*				m_pobjDirCmpFrameWnd;			// ディレクトリ比較用ウインドウクラス

	// メールリスト関連
	CMailListStatusDlg*		m_pobjMailListStatusDlg;		// メールリスト状況ダイアログ

protected:
	CStatusBar				m_wndStatusBar;
	CToolBarEx				m_wndToolBar;					// 標準ツールバー
	CToolBarEx				m_wndToolBarEdit;				// 編集関連ツールバー
	CToolBarEx				m_wndToolBarFind;				// 検索関連ツールバー
	CToolBarEx				m_wndToolBarJmp;				// ジャンプ関連ツールバー
	CToolBarEx				m_wndToolBarTools;				// ツール関連ツールバー
	CToolBarEx				m_wndToolBarWnd;				// ウィンドウ関連ツールバー
	CBitmapClient			m_wndClient;					// ロゴ付きのＭＤＩクライアント

	//ツールバーカスタマイズ用
	static CToolBarInfo		m_wndToolBar_Info[];			// 標準ツールバー情報
	static CToolBarInfo		m_wndToolBarEdit_Info[];		// 編集関連ツールバー情報
	static CToolBarInfo		m_wndToolBarFind_Info[];		// 検索関連ツールバー情報
	static CToolBarInfo		m_wndToolBarJmp_Info[];			// ジャンプ関連ツールバー情報
	static CToolBarInfo		m_wndToolBarTools_Info[];		// ツール関連ツールバー情報
	static CToolBarInfo		m_wndToolBarWnd_Info[];			// ウインドウ関連ツールバー情報

protected:
	void	WaitForHtmlOpenDlgDisp();						// ＨＴＭＬオープン状況ダイアログ表示待ち

// 	afx_msg LONG OnFindReplace(WPARAM,LPARAM);				// 検索、置換ハンドラ
	afx_msg LRESULT OnMSHMouseWheel(WPARAM,LPARAM);			// インテリマウスのホイールスクロール
	BOOL	LoadWindowPlacement(LPWINDOWPLACEMENT);			// ウインドウ位置ロード処理
	void	SaveWindowPlacement(LPWINDOWPLACEMENT);			// ウインドウ位置セーブ処理

	BOOL	CreateToolBar();
	void	DockControlBarLeftOf(CToolBar*,CToolBar*);

	void	DoGrep();										// ＧＲＥＰ実行処理
	BOOL	DoGrepInOneDirectory(CString,CString,CString);	// １ディレクトリ内ＧＲＥＰ実行処理
	BOOL	DoGrepInOneFile(CString,CString,CString);		// １ファイル内ＧＲＥＰ実行処理
	void	WaitForGrepDlgDisp();							// ＧＲＥＰ状況ダイアログ表示待ち
	BOOL	IsGrepCanceled();								// ＧＲＥＰキャンセルチェック

	void	DoCodeConv();									// 文字コード変換実行処理
	BOOL	DoConvInOneDirectory(CString,CString,int,int);	// １ディレクトリ内文字コード変換実行処理
	BOOL	DoConvInOneFile(CString,CString,CString,int,int);// １ファイル文字コード変換実行処理
	void	WaitForConvDlgDisp();							// 文字コード変換状況ダイアログ表示待ち
	BOOL	IsConvCanceled();								// 文字コード変換キャンセルチェック

	void	DoKaigyoCodeConv();								// 改行コード変換実行処理
	BOOL	DoKaigyoConvInOneDirectory(CString,CString,CString);// １ディレクトリ内改行コード変換実行処理
	BOOL	DoKaigyoConvInOneFile(CString,CString,CString,CString);// １ファイル改行コード変換実行処理
	void	WaitForKaigyoConvDlgDisp();						// 改行コード変換状況ダイアログ表示待ち
	BOOL	IsKaigyoConvCanceled();							// 改行コード変換キャンセルチェック

	void	DoWordCount();									// ワードカウント実行処理
	BOOL	DoWCountInOneDirectory(CString,CString);		// １ディレクトリ内ワードカウント実行処理
	BOOL	DoWCountInOneFile(CString,CString);				// １ファイル内ワードカウント実行処理
	void	WaitForWCntDlgDisp();							// ワードカウント状況ダイアログ表示待ち
	BOOL	IsWCntCanceled();								// ワードカウントキャンセルチェック

	void	DoFileSearch();									// ファイル検索実行処理
	BOOL	DoFSearchInOneDirectory(CString,CString);		// １ディレクトリ内ファイル検索実行処理
	void	WaitForFSearchDlgDisp();						// ファイル検索状況ダイアログ表示待ち
	BOOL	IsFSearchCanceled();							// ファイル検索キャンセルチェック

	void	DoFileEncript();								// ファイル暗号化実行処理
	BOOL	DoEncriptInOneDirectory(CString,CString,CString);// １ディレクトリ内ファイル暗号化実行処理
	BOOL	DoEncriptInOneFile(CString,CString,CString,CString);// １ファイル暗号化実行処理
	void	WaitForEncriptDlgDisp();						// ファイル暗号化状況ダイアログ表示待ち
	BOOL	IsEncriptCanceled();							// ファイル暗号化キャンセルチェック

	void	DoFileDecript();								// ファイル復号化実行処理
	BOOL	DoDecriptInOneDirectory(CString,CString,CString);// １ディレクトリ内ファイル復号化実行処理
	BOOL	DoDecriptInOneFile(CString,CString,CString,CString);// １ファイル復号化実行処理
	void	WaitForDecriptDlgDisp();						// ファイル復号化状況ダイアログ表示待ち
	BOOL	IsDecriptCanceled();							// ファイル復号化キャンセルチェック

	void	WaitForFileCompareStatusDlgDisp();				// ファイル比較状況ダイアログ表示待ち

	void	DoDirCompare();									// ディレクトリ比較実行処理
	BOOL	DoDirCmpInOneDirectory();						// １ディレクトリ比較実行
	void	WaitForDirCompareStatusDlgDisp();				// ディレクトリ比較状況ダイアログ表示待ち

	void	DoFileTouch();									// ファイルタッチ実行処理
	BOOL	DoFileTouchInOneDirectory(CString,CString,CTime);// １ディレクトリ内ファイルタッチ実行

	void	WaitForMailListDlgDisp();						// メールリスト状況ダイアログ表示待ち

// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnSettings();
	afx_msg void OnUpdateSettings(CCmdUI* pCmdUI);
	afx_msg void OnEditFind();
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg void OnEditReplace();
	afx_msg void OnUpdateEditReplace(CCmdUI* pCmdUI);
	afx_msg void OnEditFindcontinue();
	afx_msg void OnEditFindrevcontinue();
	afx_msg void OnUpdateEditFindcontinue(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditFindrevcontinue(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateWindowAllclose(CCmdUI* pCmdUI);
	afx_msg void OnWindowAllclose();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnUpdateEditGrep(CCmdUI* pCmdUI);
	afx_msg void OnEditGrep();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnUpdateEditCodeconv(CCmdUI* pCmdUI);
	afx_msg void OnEditCodeconv();
	afx_msg void OnUpdateEditAsciidump(CCmdUI* pCmdUI);
	afx_msg void OnEditAsciidump();
	afx_msg void OnUpdateEditSjisdump(CCmdUI* pCmdUI);
	afx_msg void OnEditSjisdump();
	afx_msg void OnUpdateFileReread(CCmdUI* pCmdUI);
	afx_msg void OnFileReread();
	afx_msg void OnUpdateFileProperty(CCmdUI* pCmdUI);
	afx_msg void OnFileProperty();
	afx_msg void OnEditSetreadonly();
	afx_msg void OnUpdateEditSetreadonly(CCmdUI* pCmdUI);
	afx_msg void OnEditUnsetreadonly();
	afx_msg void OnUpdateEditUnsetreadonly(CCmdUI* pCmdUI);
	afx_msg void OnEditWordcount();
	afx_msg void OnUpdateEditWordcount(CCmdUI* pCmdUI);
	afx_msg void OnEditFilesearch();
	afx_msg void OnUpdateEditFilesearch(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveclose();
	afx_msg void OnUpdateFileSaveclose(CCmdUI* pCmdUI);
	afx_msg void OnToolOpeStartendrecord();
	afx_msg void OnUpdateToolOpeStartendrecord(CCmdUI* pCmdUI);
	afx_msg void OnToolOpeDo();
	afx_msg void OnUpdateToolOpeDo(CCmdUI* pCmdUI);
	afx_msg void OnToolOpeSave();
	afx_msg void OnUpdateToolOpeSave(CCmdUI* pCmdUI);
	afx_msg void OnToolOpeOpen();
	afx_msg void OnUpdateToolOpeOpen(CCmdUI* pCmdUI);
	afx_msg void OnToolOpeDisp();
	afx_msg void OnUpdateToolOpeDisp(CCmdUI* pCmdUI);
	afx_msg void OnToolOpeStepdoNext();
	afx_msg void OnUpdateToolOpeStepdoNext(CCmdUI* pCmdUI);
	afx_msg void OnToolOpeStepdoEnd();
	afx_msg void OnUpdateToolOpeStepdoEnd(CCmdUI* pCmdUI);
	afx_msg void OnWndNext();
	afx_msg void OnUpdateWndNext(CCmdUI* pCmdUI);
	afx_msg void OnWndPrev();
	afx_msg void OnUpdateWndPrev(CCmdUI* pCmdUI);
	afx_msg void OnEditClipview();
	afx_msg void OnUpdateEditClipview(CCmdUI* pCmdUI);
	afx_msg void OnEditIncrimentalsearch();
	afx_msg void OnUpdateEditIncrimentalsearch(CCmdUI* pCmdUI);
	afx_msg void OnSecurityEncript();
	afx_msg void OnUpdateSecurityEncript(CCmdUI* pCmdUI);
	afx_msg void OnSecurityDecript();
	afx_msg void OnUpdateSecurityDecript(CCmdUI* pCmdUI);
	afx_msg void OnEditUuencode();
	afx_msg void OnUpdateEditUuencode(CCmdUI* pCmdUI);
	afx_msg void OnEditBase64encode();
	afx_msg void OnUpdateEditBase64encode(CCmdUI* pCmdUI);
	afx_msg void OnEditQuotedencode();
	afx_msg void OnUpdateEditQuotedencode(CCmdUI* pCmdUI);
	afx_msg void OnEditFilecompare();
	afx_msg void OnUpdateEditFilecompare(CCmdUI* pCmdUI);
	afx_msg void OnEditDircompare();
	afx_msg void OnUpdateEditDircompare(CCmdUI* pCmdUI);
	afx_msg void OnFileDelete();
	afx_msg void OnUpdateFileDelete(CCmdUI* pCmdUI);
	afx_msg void OnEditFiletouch();
	afx_msg void OnUpdateEditFiletouch(CCmdUI* pCmdUI);
	afx_msg void OnWindowRestore();
	afx_msg void OnUpdateWindowRestore(CCmdUI* pCmdUI);
	afx_msg void OnWindowMinimize();
	afx_msg void OnUpdateWindowMinimize(CCmdUI* pCmdUI);
	afx_msg void OnWindowMaximize();
	afx_msg void OnUpdateWindowMaximize(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbarWnd();
	afx_msg void OnUpdateViewToolbarWnd(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbarTools();
	afx_msg void OnUpdateViewToolbarTools(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbarFind();
	afx_msg void OnUpdateViewToolbarFind(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbarEdit();
	afx_msg void OnUpdateViewToolbarEdit(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbar();
	afx_msg void OnUpdateViewToolbar(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbarJmp();
	afx_msg void OnUpdateViewToolbarJmp(CCmdUI* pCmdUI);
	afx_msg void OnFileMaillist();
	afx_msg void OnUpdateFileMaillist(CCmdUI* pCmdUI);
	afx_msg void OnEditKaigyoconv();
	afx_msg void OnUpdateEditKaigyoconv(CCmdUI* pCmdUI);
	afx_msg void OnCustomizeToolbar();
	afx_msg void OnUpdateCustomizeToolbar(CCmdUI* pCmdUI);
	afx_msg void OnCustomizeToolbaredit();
	afx_msg void OnUpdateCustomizeToolbaredit(CCmdUI* pCmdUI);
	afx_msg void OnCustomizeToolbarfind();
	afx_msg void OnUpdateCustomizeToolbarfind(CCmdUI* pCmdUI);
	afx_msg void OnCustomizeToolbarjmp();
	afx_msg void OnUpdateCustomizeToolbarjmp(CCmdUI* pCmdUI);
	afx_msg void OnCustomizeToolbartools();
	afx_msg void OnUpdateCustomizeToolbartools(CCmdUI* pCmdUI);
	afx_msg void OnCustomizeToolbarwnd();
	afx_msg void OnUpdateCustomizeToolbarwnd(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowClose();
	afx_msg void OnUpdateWindowClose(CCmdUI* pCmdUI);
	afx_msg void OnHttpFileOpen();
	afx_msg void OnEditFcompAlphabetbigandsmall();
	afx_msg void OnUpdateEditFcompAlphabetbigandsmall(CCmdUI* pCmdUI);
	afx_msg void OnEditFcompTabequal2space();
	afx_msg void OnUpdateEditFcompTabequal2space(CCmdUI* pCmdUI);
	afx_msg void OnEditFcompZenequal2han();
	afx_msg void OnUpdateEditFcompZenequal2han(CCmdUI* pCmdUI);
	afx_msg void OnEditFilerecompare();
	afx_msg void OnUpdateEditFilerecompare(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(MAINFRM_H)



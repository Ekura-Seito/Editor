// EditorView.h : CEditorView クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(EDITORVIEW_H)		//２重インクルード防止
#define EDITORVIEW_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//class CEditorApp:
class CEditorDoc;
class CUndoItemAddCharacter;
class CUndoItemDelChar;
class CUndoItemReturn;
class CUndoItemBackSpaceChar;
class CUndoItemCutSelectArea;
class CUndoItemEditPaste;
class CUndoItemReplaceSelectWord;
class CUndoItemTabWhenSelected;
class CUndoItemShiftTabWhenSelected;
class CUndoItemDelTopSpaces;
class CUndoItemTab2Space;
class CUndoItemSpace2Tab;
class CUndoItemSpaceZen2Han;
class CUndoItemSpaceHan2Zen;
class CUndoItemKatakanaHan2Zen;
class CUndoItemKatakanaZen2Han;
class CUndoItemAlnumHan2Zen;
class CUndoItemAlnumZen2Han;
class CUndoItemOleDrop;
class CUndoItemInsCppComment;
class CUndoItemDelCppComment;
class CUndoItemInsVbComment;
class CUndoItemDelVbComment;
class CUndoItemInsAsmComment;
class CUndoItemDelAsmComment;
class CUndoItemInsQuote;
class CUndoItemDelQuote;
class CUndoItemInsDay;
class CUndoItemInsTime;
class CUndoItemAl2Upper;
class CUndoItemAl2Lower;
class COperation;
class CMainFrame;
class BCMenu;

class CEditorView : public CView
{
	friend class CEditorDoc;
	friend class CUndoItemAddCharacter;					// フレンドクラス宣言
	friend class CUndoItemDelChar;
	friend class CUndoItemReturn;
	friend class CUndoItemBackSpaceChar;
	friend class CUndoItemCutSelectArea;
	friend class CUndoItemEditPaste;
	friend class CUndoItemReplaceSelectWord;
	friend class CUndoItemTabWhenSelected;
	friend class CUndoItemShiftTabWhenSelected;
	friend class CUndoItemDelTopSpaces;
	friend class CUndoItemTab2Space;
	friend class CUndoItemSpace2Tab;
	friend class CUndoItemSpaceZen2Han;
	friend class CUndoItemSpaceHan2Zen;
	friend class CUndoItemKatakanaHan2Zen;
	friend class CUndoItemKatakanaZen2Han;
	friend class CUndoItemAlnumHan2Zen;
	friend class CUndoItemAlnumZen2Han;
	friend class CUndoItemOleDrop;
	friend class CUndoItemInsCppComment;
	friend class CUndoItemDelCppComment;
	friend class CUndoItemInsVbComment;
	friend class CUndoItemDelVbComment;
	friend class CUndoItemInsAsmComment;
	friend class CUndoItemDelAsmComment;
	friend class CUndoItemInsQuote;
	friend class CUndoItemDelQuote;
	friend class CUndoItemInsDay;
	friend class CUndoItemInsTime;
	friend class CUndoItemAl2Upper;
	friend class CUndoItemAl2Lower;
	friend class COperation;
	friend class CMainFrame;

protected: // シリアライズ機能のみから作成します。
	CEditorView();										// コンストラクタ
	DECLARE_DYNCREATE(CEditorView)

// メンバ関数
public:
	CEditorDoc* GetDocument();
	BOOL		IsSelected();							// 範囲選択中チェック
	long		GetSelectLineCount();					// 選択行数取得
	void		ClearSelectArea();						// 選択範囲解除
	BOOL		DoFind(CString&,BOOL,BOOL,BOOL,BOOL);	// 検索実行
	BOOL		DoIncrimentalSearch(long,long,CString&,BOOL);// インクリメンタル検索実行
	void		DoReplace(CString&);					// 置換実行
	BOOL		GetLogfontOfDisplay(LOGFONT*);			// 表示フォントのＬＯＧＦＯＮＴ構造体取得
	BOOL		GetLogfontOfPrint(LOGFONT*);			// 印字フォントのＬＯＧＦＯＮＴ構造体取得
	long		GetDispFontPoint();						// 表示フォントのポイント取得
	CString		GetDispFontName();						// 表示フォント名取得
	void		SetDispFontPoint(long);					// 表示フォントのポイント設定
	void 		SetDispFontName(CString&);				// 表示フォント名設定
	long		GetPrintFontPoint();					// 印字フォントのポイント取得
	CString		GetPrintFontName();						// 印字フォント名取得
	void		SetPrintFontPoint(long);				// 印字フォントのポイント設定
	void 		SetPrintFontName(CString&);				// 印字フォント名設定
	COLORREF	GetFontColor();							// 通常の文字色取得
	void		SetFontColor(COLORREF);					// 通常の文字色設定
	COLORREF	GetBackColor();							// 通常の背景色取得
	void		SetBackColor(COLORREF);					// 通常の背景色設定
	COLORREF	GetInvertFontColor();					// 反転時の文字色取得
	void		SetInvertFontColor(COLORREF);			// 反転時の文字色設定
	COLORREF	GetInvertBackColor();					// 反転時の背景色取得
	void		SetInvertBackColor(COLORREF);			// 反転時の背景色設定
	BOOL		GetFlagDispLineNo();					// 行番号表示フラグ取得
	void		SetFlagDispLineNo(BOOL);				// 行番号表示フラグ設定
	COLORREF	GetLineNoColor();						// 行番号色取得
	void		SetLineNoColor(COLORREF);				// 行番号色設定
	COLORREF	GetLineNoBackColor();					// 行番号の背景の色取得
	void		SetLineNoBackColor(COLORREF);			// 行番号の背景の色設定
	BOOL		GetFlagDispRuler();						// ルーラ表示フラグ取得
	void		SetFlagDispRuler(BOOL);					// ルーラ表示フラグ設定
	COLORREF	GetRulerColor();						// ルーラ色取得
	void		SetRulerColor(COLORREF);				// ルーラ色設定
	COLORREF	GetRulerBackColor();					// ルーラの背景の色取得
	void		SetRulerBackColor(COLORREF);			// ルーラの背景の色設定
	BOOL		GetFlagDispCRLF();						// 改行マーク表示フラグ取得
	void		SetFlagDispCRLF(BOOL);					// 改行マーク表示フラグ設定
	COLORREF	GetCRLFColor();							// 改行マーク色取得
	void		SetCRLFColor(COLORREF);					// 改行マーク色設定
	BOOL		GetFlagDispTab();						// タブコード表示フラグ取得
	void		SetFlagDispTab(BOOL);					// タブコード表示フラグ設定
	COLORREF	GetTabColor();							// タブコード色取得
	void		SetTabColor(COLORREF);					// タブコード色設定
	BOOL		GetFlagDispZenkakuSpace();				// 全角スペース表示フラグ取得
	void		SetFlagDispZenkakuSpace(BOOL);			// 全角スペース表示フラグ設定
	COLORREF	GetZenkakuSpaceColor();					// 全角スペース色取得
	void		SetZenkakuSpaceColor(COLORREF);			// 全角スペース色設定
	BOOL		GetFlagDispUnderLine();					// アンダーライン表示フラグ取得
	void		SetFlagDispUnderLine(BOOL);				// アンダーライン表示フラグ設定
	COLORREF	GetUnderLineColor();					// アンダーライン色取得
	void		SetUnderLineColor(COLORREF);			// アンダーライン色設定
	BOOL		GetFlagPrintLineNo();					// 行番号印刷フラグ取得
	void		SetFlagPrintLineNo(BOOL);				// 行番号印刷フラグ設定
	BOOL		GetFlagPrint2Dangumi();					// ２段組み印刷フラグ取得
	void		SetFlagPrint2Dangumi(BOOL);				// ２段組み印刷フラグ設定
	int			GetPrintHeaderLayoutPos();				// 印刷ヘッダレイアウト位置取得
	void		SetPrintHeaderLayoutPos(int);			// 印刷ヘッダレイアウト位置設定
	int			GetPrintFooterLayoutPos();				// 印刷フッタレイアウト位置取得
	void		SetPrintFooterLayoutPos(int);			// 印刷フッタレイアウト位置設定
	void		GetStrHeaderFormat(CString&);			// 印刷ヘッダフォーマット取得
	void		SetStrHeaderFormat(CString&);			// 印刷ヘッダフォーマット設定
	void		GetStrFooterFormat(CString&);			// 印刷フッタフォーマット取得
	void		SetStrFooterFormat(CString&);			// 印刷フッタフォーマット設定
	long		GetLineInterval();						// 行間取得
	void		SetLineInterval(long);					// 行間設定
	int			GetCaretType();							// キャレット形状取得
	void		SetCaretType(int);						// キャレット形状設定
	void		HoldCaretBlinkTime(UINT);				// キャレット点滅間隔保持
	int			GetCaretBehave();						// キャレットの振る舞い取得
	void		SetCaretBehave(int);					// キャレットの振る舞い設定
	int			GetCaretMoveType();						// キャレット移動タイプ取得
	void		SetCaretMoveType(int);					// キャレット移動タイプ設定
	void		SetFlagDispEOFMark(BOOL);				// ＥＯＦマーク表示フラグ設定
	CLine* 		MyGetSelectedText(LONG*);				// 選択範囲テキスト取得
	int			GetScrollPosLogX();						// 論理スクロール位置Ｘ取得
	int			GetScrollPosLogY();						// 論理スクロール位置Ｙ取得
	void		AdjustVScrollPos(SCROLLINFO*);			// 垂直スクロール位置調整
	void		AdjustHScrollPos(SCROLLINFO*);			// 水平スクロール位置調整
	COLORREF	GetMarkLineColor();						// マーク行色取得
	void		SetMarkLineColor(COLORREF);				// マーク行色設定
	void		ClearIncrimentalSearchLineOffset();		// インクリメンタルサーチ開始行、オフセットクリア
	void		GetIncrimentalSearchString(CString&);	// インクリメンタルサーチ文字列取得
	COLORREF	GetDiffDelLineColor();					// ファイル比較用、削除行色取得
	void		SetDiffDelLineColor(COLORREF);			// ファイル比較用、削除行色設定
	COLORREF	GetDiffAddLineColor();					// ファイル比較用、追加行色取得
	void		SetDiffAddLineColor(COLORREF);			// ファイル比較用、追加行色設定
	COLORREF	GetDirDiffLineColor();					// ディレクトリ比較用、相違ファイル名色取得
	void		SetDirDiffLineColor(COLORREF);			// ディレクトリ比較用、相違ファイル名色設定

// メンバ変数
private:
	CEditorApp*	m_pApp;									// アプリケーションクラスへのポインタ

	// ドラッグ＆ドロップ関連
	COleDropTarget	m_objOleDropTarget;					// ＯＬＥドロップターゲット

	// 描画関連
	CRgn*		m_pobjInvalidRegion;					// 無効なﾘｰｼﾞｮﾝ
	CRect*		m_pobjInvalidRect;						// 無効な矩形
	BOOL		m_bUseClipRgn;							// ｸﾘｯﾌﾟﾘｰｼﾞｮﾝ描画ﾌﾗｸﾞ
	BOOL		m_bNoUseOffBuffer;						// ｵﾌｽｸﾘｰﾝﾊﾞｯﾌｧ不使用ﾌﾗｸﾞ

	// 全角スペース関連
	BOOL		m_bDispZenkakuSpace;					// 全角スペース描画フラグ
	COLORREF	m_ZenkakuSpaceColor;					// 全角スペースの色

	// タブ関連
	BOOL		m_bDispTab;								// タブコード描画フラグ
	COLORREF	m_TabColor;								// タブコードの色

	// ルーラ関連
	BOOL		m_bDispRuler;							// ルーラ表示フラグ
	COLORREF	m_RulerColor;							// ルーラの色
	COLORREF	m_RulerBackColor;						// ルーラの背景の色
	char		m_szRuler[32];							// ルーラ
	CPoint		m_RulerPos;								// ルーラ表示時のルーラ直下位置

	// 行番号関連
	BOOL		m_bDispLineNo;							// 行番号表示フラグ
	COLORREF	m_LineNoColor;							// 行番号の色
	COLORREF	m_LineNoBackColor;						// 行番号の背景の色
	char		m_szLineNoFormat[32];					// 行番号表示フォーマット
	long		m_lLineNoWidth;							// 行番号表示幅
	CPoint		m_LineNoPos;							// 行番号表示時の行頭位置

	// 改行マーク関連
	BOOL		m_bDispCRLF;							// 改行マーク表示フラグ
	COLORREF	m_CRLFColor;							// 改行マークの色

	// オフスクリーンバッファ関連
	int			m_nOffBufferWidth;						// ｵﾌｽｸﾘｰﾝﾊﾞｯﾌｧの幅
    CDC			m_objOffBufferDC;						// ｵﾌｽｸﾘｰﾝﾊﾞｯﾌｧのDC
    CBitmap		m_objOffBufferBmp;						// ｵﾌｽｸﾘｰﾝﾊﾞｯﾌｧ

	// キャレット関連
	int			m_nCaretType;							// キャレット形状
	int			m_nCaretBehave;							// 画面スクロール時のキャレットの振る舞い
	CPoint		m_caretPos;								// キャレット表示位置
	UINT		m_unCaretBlinkTime;						// キャレット点滅間隔
	int			m_nCaretMoveType;						// キャレット移動タイプ
	int			m_nLogPreCaretX;						// 直前のキャレット論理Ｘ位置

	// IME関連
	HIMC m_hImc;
	BOOL m_bIsImmOpen;									// ＩＭＭ変換中フラグ

	// ｽｸﾛｰﾙ関連
	int m_nMapMode;										// ﾏｯﾋﾟﾝｸﾞﾓｰﾄﾞ
	CSize m_totalLog;									// 論理単位でのﾄｰﾀﾙｻｲｽﾞ
	CSize m_VPageLog;									// 垂直ｽｸﾛｰﾙﾊﾞｰの論理単位での１ﾍﾟｰｼﾞあたりｽｸﾛｰﾙｻｲｽﾞ
	CSize m_VLineLog;									// 垂直ｽｸﾛｰﾙﾊﾞｰの論理単位での１行あたりｽｸﾛｰﾙｻｲｽﾞ
	CSize m_HPageLog;									// 水平ｽｸﾛｰﾙﾊﾞｰの論理単位での１ﾍﾟｰｼﾞあたりｽｸﾛｰﾙｻｲｽﾞ
	CSize m_HLineLog;									// 水平ｽｸﾛｰﾙﾊﾞｰの論理単位での１行あたりｽｸﾛｰﾙｻｲｽﾞ

	// ﾃｷｽﾄ関連
	BOOL		m_bInsertKey;							// Insertキー押下フラグ(TRUE:挿入、FALSE:上書き）
	UINT		m_n1stChar;								// 押下ｷｰ　ﾏﾙﾁﾊﾞｲﾄ文字の１ﾊﾞｲﾄ目
	UINT		m_n2ndChar;								// 押下ｷｰ　ﾏﾙﾁﾊﾞｲﾄ文字の２ﾊﾞｲﾄ目

	// 表示フォント関連
	CFont		m_objDispFont;							// 表示フォント
	CFont		m_objDispFontUnderLine;					// 下線付き表示フォント
	long		m_lfHeight;								// フォントの文字の高さ
	long		m_lfWidth;								// フォントの文字の幅
	long		m_lDispFontPoint;						// 表示フォントのポイント
	CString		m_objDispFontName;						// 表示フォント名

	// カラー関連
	COLORREF	m_FontColor;							// 通常の文字色
	COLORREF	m_BackColor;							// 通常の背景色
	COLORREF	m_InvertFontColor;						// 反転時の文字色
	COLORREF	m_InvertBackColor;						// 反転時の背景色

	// 印刷関連
	BOOL		m_bPrintLineNo;							// 行番号印刷フラグ
	BOOL		m_bPrint2Dangumi;						// ２段組み印刷フラグ
	CLine		m_objPrintFoldLine;						// 折り返し印刷行
	long		m_lPrintFoldLineNo;						// 折り返し印刷行番号
	int			m_nHeaderLayout;						// ヘッダレイアウト位置
	int			m_nFooterLayout;						// フッタレイアウト位置
	CString		m_objStrHeaderFormat;					// ヘッダフォーマット
	CString		m_objStrFooterFormat;					// フッタフォーマット
	BOOL		m_bIsNowPrinting;						// 印刷中フラグ
	CFont		m_objPrintFont;							// 印刷用フォント
	long		m_lPrintLineHeight;						// １行あたりのデバイス単位の高さ
	long		m_lPrintCharWidth;						// １文字あたりのデバイス単位の幅
	long		m_lPrintFontPoint;						// 印字フォントのポイント
	CString		m_objPrintFontName;						// 印字フォント名
	long		m_lLinePerPrintPage;					// １ページあたりの行数

	// 範囲選択関連
	BOOL		m_bIsShiftKeyDown;						// シフトキー押下中フラグ
	BOOL		m_bLButtonDown;							// マウス左ボタン押下中フラグ
	long		m_lSelectStartLine;						// 選択開始行番号
	long		m_lSelectStartOffset;					// 選択開始列番号
	long		m_lSelectEndLine;						// 選択終了行番号
	long		m_lSelectEndOffset;						// 選択終了列番号
	long		m_lPreMoveCaretLine;					// 移動前の行
	long		m_lPreMoveCaretOffset;					// 移動前の列
	long		m_lLButtonDownLine;						// マウス左ボタン押下した行
	long		m_lLButtonDownOffset;					// マウス左ボタン押下した列

	// 行間関連
	long		m_lLineInterval;						// 行間（ピクセル単位）

	// アンダーライン関連
	BOOL		m_bDispUnderLine;						// アンダーライン表示フラグ
	COLORREF	m_UnderLineColor;						// アンダーラインの色

	// マーク関連
	COLORREF	m_MarkLineColor;						// マーク行の色

	// インクリメンタルサーチ関連
	long		m_lIncrimentalSearchStartLine;			// インクリメンタルサーチ開始行
	long		m_lIncrimentalSearchStartOffset;		// インクリメンタルサーチ開始オフセット
	CLine		m_objIncrimentalSearchLine;				// インクリメンタルサーチ文字列

	// ファイル比較間連
	COLORREF	m_DiffDelLineColor;						// 削除行の色
	COLORREF	m_DiffAddLineColor;						// 追加行の色

	// ディレクトリ比較関連
	COLORREF	m_DirDiffLineColor;						// 相違ファイル名の色

	// ビットマップ付きポップアップメニュー
	BCMenu		m_popupmenu;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditorView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);						// ポップアップメニューハンドラ
	afx_msg LRESULT OnMyImeNotify(WPARAM dwCommand, LPARAM dwData);
	void CgAdjustCompositionWindow();
	void MyGetClientRect(CRect*);											// ｸﾗｲｱﾝﾄ領域取得処理
	void MySetScrollSizes(int,SIZE,const SIZE&,const SIZE&,const SIZE&,const SIZE&);
																			// ｽｸﾛｰﾙｻｲｽﾞ設定処理
	void MyScrollWindow(BOOL,int);											// ｳｲﾝﾄﾞｳｽｸﾛｰﾙ処理
	void MyScrollToPosition(CPoint&);										// ｽｸﾛｰﾙ位置設定処理
	void MyTextOut(CDC*,long,long,CLine*,long,long);						// １行描画処理
	int  MyTextOutForPrint(CDC*,long,long,CLine*,long,long,int);			// １行描画（印刷用）処理
	void MyDrawingCRLF(CDC*,long,long,long,long,int,COLORREF,COLORREF);		// 改行マーク描画処理
	void MyDrawingRuler(CDC*,long,long,long,COLORREF,COLORREF);				// ルーラ描画処理
	void MyDrawingZenkakuSpace(CDC*,long,long,long,long,COLORREF,COLORREF);	// 全角スペース描画処理
	void MyDrawingTab(CDC*,long,long,long,long,COLORREF,COLORREF);			// タブコード描画処理
	void MyDrawingUnderLine(CDC*);											// アンダーライン描画処理
	void MyEraseUnderLine(CDC*);											// アンダーライン消去処理
	BOOL IsWithinSelectArea(long,long,long,long,long,long);					// 選択範囲内判定処理
	void MoveCaret(UINT,int);												// キャレット移動処理
	void MySetCaretPos(CPoint&);											// キャレット位置設定
	void MySetCaretToLineOffset(long,long);									// 指定行、列にキャレットを位置づける処理
	void MySetCaretToMatchBrace(BOOL);										// 対応する括弧にキャレットを位置づける
	void GetPosition(long,CPoint*);											// 行末の位置取得
	void GetPosition1(long,CPoint*,CPoint*);								// キャレット未満位置取得
	void GetPosition2(long,CPoint*,CPoint*);								// キャレット以下位置取得
	void GetPosition3(long,long,CPoint*);									// 指定オフセット以下位置取得
	void GetPosition4(long,CPoint*,long);									// 印刷時の行末位置取得
	void GetPosition5(long,CPoint*,ULONG*,CPoint*);
	long GetOffset(long,CPoint*);											// オフセット取得
	long GetPreOffset(long,long);											// 直前オフセット取得
	void AdjustSelectArea();												// 選択範囲調整処理
	BOOL CaretToLineOffset(long*,long*);									// キャレット位置の行番号、列番号取得
	LONG MouseToLineOffset(CPoint&,long*,long*,CPoint*);					// マウス位置の行番号、列番号取得
	void CreateOffScreenBuf();												// オフスクリーンバッファ作成
	void DestroyOffScreenBuf();												// オフスクリーンバッファ破棄
	void UpdateOffBuffer();													// オフスクリーンバッファ更新
	void Jump2LineOffset(long,long);										// 指定行、列へのジャンプ
	void Jump2LineOffsetEx(long,long);										// 指定行、列へのジャンプ
	void RedrawSelectArea(long,long);										// 選択範囲の再描画
	void InvalidateCaretLine();												// キャレット行　１行再描画
	void InvalidateCaretLineEx(long);										// キャレット行　再描画
	void ParsePrintHeaderFormat(CString&,CString&,CPrintInfo*);				// 印刷ヘッダフォーマット解析
	void ParsePrintFooterFormat(CString&,CString&,CPrintInfo*);				// 印刷フッタフォーマット解析
	UINT CountPrintMaxPage(CPrintInfo*,long,long);							// 印刷総ページ数カウント
	void MyPrintOut1Page(CDC*,CPrintInfo*,long,long,long);					// ファイル内容１ページ分出力
	void AutoScroll();														// オートスクロール
	void WordSelectByCtrlClick(CPoint&);									// CTRL+クリックによるワードセレクト


// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CEditorView)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectall();
	afx_msg void OnUpdateEditSelectall(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnUpdateEditDel(CCmdUI* pCmdUI);
	afx_msg void OnEditDel();
	afx_msg void OnEditJump2top();
	afx_msg void OnUpdateEditJump2top(CCmdUI* pCmdUI);
	afx_msg void OnEditJump2bottom();
	afx_msg void OnUpdateEditJump2bottom(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditJump2line(CCmdUI* pCmdUI);
	afx_msg void OnEditJump2line();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnUpdateViewLineno(CCmdUI* pCmdUI);
	afx_msg void OnViewLineno();
	afx_msg void OnUpdateViewCrlf(CCmdUI* pCmdUI);
	afx_msg void OnViewCrlf();
	afx_msg void OnUpdateViewRuler(CCmdUI* pCmdUI);
	afx_msg void OnViewRuler();
	afx_msg void OnParenthesis();
	afx_msg void OnViewTab();
	afx_msg void OnUpdateViewTab(CCmdUI* pCmdUI);
	afx_msg void OnViewZenkakuspace();
	afx_msg void OnUpdateViewZenkakuspace(CCmdUI* pCmdUI);
	afx_msg void OnEditTab2space();
	afx_msg void OnUpdateEditTab2space(CCmdUI* pCmdUI);
	afx_msg void OnEditSpace2tab();
	afx_msg void OnUpdateEditSpace2tab(CCmdUI* pCmdUI);
	afx_msg void OnEditSpaceHan2zen();
	afx_msg void OnUpdateEditSpaceHan2zen(CCmdUI* pCmdUI);
	afx_msg void OnEditSpaceZen2han();
	afx_msg void OnUpdateEditSpaceZen2han(CCmdUI* pCmdUI);
	afx_msg void OnEditKatakanaHan2zen();
	afx_msg void OnUpdateEditKatakanaHan2zen(CCmdUI* pCmdUI);
	afx_msg void OnEditKatakanaZen2han();
	afx_msg void OnUpdateEditKatakanaZen2han(CCmdUI* pCmdUI);
	afx_msg void OnViewUnderline();
	afx_msg void OnUpdateViewUnderline(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewCaretIbeam(CCmdUI* pCmdUI);
	afx_msg void OnViewCaretIbeam();
	afx_msg void OnUpdateViewCaretSquare(CCmdUI* pCmdUI);
	afx_msg void OnViewCaretSquare();
	afx_msg void OnEditAlnumHan2zen();
	afx_msg void OnUpdateEditAlnumHan2zen(CCmdUI* pCmdUI);
	afx_msg void OnEditAlnumZen2han();
	afx_msg void OnUpdateEditAlnumZen2han(CCmdUI* pCmdUI);
	afx_msg void OnEditDeltopspaces();
	afx_msg void OnUpdateEditDeltopspaces(CCmdUI* pCmdUI);
	afx_msg void OnEditDeltab();
	afx_msg void OnUpdateEditDeltab(CCmdUI* pCmdUI);
	afx_msg void OnEditInstab();
	afx_msg void OnUpdateEditInstab(CCmdUI* pCmdUI);
	afx_msg void OnUpdateParenthesis(CCmdUI* pCmdUI);
	afx_msg void OnEditTagjump();
	afx_msg void OnUpdateEditTagjump(CCmdUI* pCmdUI);
	afx_msg void OnEditInscppcomment();
	afx_msg void OnUpdateEditInscppcomment(CCmdUI* pCmdUI);
	afx_msg void OnEditInsvbcomment();
	afx_msg void OnUpdateEditInsvbcomment(CCmdUI* pCmdUI);
	afx_msg void OnEditInsasmcomment();
	afx_msg void OnUpdateEditInsasmcomment(CCmdUI* pCmdUI);
	afx_msg void OnViewRedraw();
	afx_msg void OnUpdateViewRedraw(CCmdUI* pCmdUI);
	afx_msg void OnViewEof();
	afx_msg void OnUpdateViewEof(CCmdUI* pCmdUI);
	afx_msg void OnJumpSetmark();
	afx_msg void OnUpdateJumpSetmark(CCmdUI* pCmdUI);
	afx_msg void OnJumpDelmark();
	afx_msg void OnUpdateJumpDelmark(CCmdUI* pCmdUI);
	afx_msg void OnJumpNextmark();
	afx_msg void OnUpdateJumpNextmark(CCmdUI* pCmdUI);
	afx_msg void OnJumpPrevmark();
	afx_msg void OnUpdateJumpPrevmark(CCmdUI* pCmdUI);
	afx_msg void OnJumpMarklist();
	afx_msg void OnUpdateJumpMarklist(CCmdUI* pCmdUI);
	afx_msg void OnJumpAlldelmark();
	afx_msg void OnUpdateJumpAlldelmark(CCmdUI* pCmdUI);
	afx_msg void OnEditInsday();
	afx_msg void OnUpdateEditInsday(CCmdUI* pCmdUI);
	afx_msg void OnEditInstime();
	afx_msg void OnUpdateEditInstime(CCmdUI* pCmdUI);
	afx_msg void OnEditAl2upper();
	afx_msg void OnUpdateEditAl2upper(CCmdUI* pCmdUI);
	afx_msg void OnEditAl2lower();
	afx_msg void OnUpdateEditAl2lower(CCmdUI* pCmdUI);
	afx_msg void OnWordskip2right();
	afx_msg void OnWordskip2left();
	afx_msg void OnWordselect2right();
	afx_msg void OnWordselect2left();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnEditUudecode();
	afx_msg void OnUpdateEditUudecode(CCmdUI* pCmdUI);
	afx_msg void OnEditBase64decode();
	afx_msg void OnUpdateEditBase64decode(CCmdUI* pCmdUI);
	afx_msg void OnEditQuoteddecode();
	afx_msg void OnUpdateEditQuoteddecode(CCmdUI* pCmdUI);
	afx_msg void OnEditNextdiff();
	afx_msg void OnUpdateEditNextdiff(CCmdUI* pCmdUI);
	afx_msg void OnEditPrevdiff();
	afx_msg void OnUpdateEditPrevdiff(CCmdUI* pCmdUI);
	afx_msg void OnEditJump2topselect();
	afx_msg void OnEditJump2bottomselect();
	afx_msg void OnEditDircmpSyosai();
	afx_msg void OnUpdateEditDircmpSyosai(CCmdUI* pCmdUI);
	afx_msg void OnParenthesis2();
	afx_msg void OnParenthesisSelect();
	afx_msg void OnEditBrowseurl();
	afx_msg void OnUpdateEditBrowseurl(CCmdUI* pCmdUI);
	afx_msg void OnEditExecmailer();
	afx_msg void OnUpdateEditExecmailer(CCmdUI* pCmdUI);
	afx_msg void OnEditInsquote();
	afx_msg void OnUpdateEditInsquote(CCmdUI* pCmdUI);
	afx_msg void OnEditDelcppcomment();
	afx_msg void OnUpdateEditDelcppcomment(CCmdUI* pCmdUI);
	afx_msg void OnEditDelvbcomment();
	afx_msg void OnUpdateEditDelvbcomment(CCmdUI* pCmdUI);
	afx_msg void OnEditDelasmcomment();
	afx_msg void OnUpdateEditDelasmcomment(CCmdUI* pCmdUI);
	afx_msg void OnEditDelquote();
	afx_msg void OnUpdateEditDelquote(CCmdUI* pCmdUI);
	afx_msg void OnChangeInsmode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // EditorView.cpp ファイルがデバッグ環境の時使用されます。
inline CEditorDoc* CEditorView::GetDocument()
   { return (CEditorDoc*)m_pDocument; }
#endif

#endif // !defined(EDITORVIEW_H)


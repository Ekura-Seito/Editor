// EditorDoc.h : CEditorDoc クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(EDITORDOC_H)		//２重インクルード防止
#define EDITORDOC_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CColorManager;
class CLineManager;
class CUndoManager;
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
class CMailSendStatusDlg;

class CEditorDoc : public CDocument
{
	friend class CEditorApp;						//フレンドクラス宣言
	friend class CMainFrame;
	friend class CUndoItemAddCharacter;
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
	friend class CUndoitemDelQuote;
	friend class CUndoItemInsDay;
	friend class CUndoItemInsTime;
	friend class CUndoItemAl2Upper;
	friend class CUndoItemAl2Lower;

protected: // シリアライズ機能のみから作成します。
	CEditorDoc();
	DECLARE_DYNCREATE(CEditorDoc)

//オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CEditorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void SetTitle(LPCTSTR lpszTitle);
	virtual void OnCloseDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void SetFontOfCurrentFileExt(const char*);						// ファイル拡張子に対応するフォント設定
	void SetColorOfCurrentFileExt(const char*);						// ファイル拡張子に対応する色設定
	void GetBytes(long,long*,long*);								// 総バイト数取得
	BOOL IsNeedReload();											// ファイル内容再読み込み必要性チェック
	void ReloadDocument(BOOL);										// ファイル内容再読み込み
	void MyGetFileName(char*);										// ファイル名取得処理
	void MyGetFilePath(char*);										// ファイルパス取得処理
	CString MyGetFileExt();											// ファイル拡張子取得処理
	BOOL IsReadOnly();												// 読み込み専用調査
	BOOL SetReadOnly();												// 読み込み専用設定
	BOOL SetReadOnly2();											// 読み込み専用設定
	BOOL UnsetReadOnly();											// 読み込み専用解除
	BOOL IsEmpty();													// 空ドキュメント調査
	BOOL IsCanUndo();												// ＵＮＤＯ可能調査
	BOOL IsCanRedo();												// ＲＥＤＯ可能調査
	long GetTabSize();												// タブサイズ取得
	void SetTabSize(long);											// タブサイズ設定
	long GetKaigyoKind();											// 改行コード区分取得
	void SetKaigyoKind(long);										// 改行コード区分設定
	long GetLineLength(long);										// 指定行の文字列長取得処理
	long GetMaxLineLength();										// １行の長さの最大値を取得
	long GetLineCount();											// 行数取得処理
	CLine* GetOneLine(long);										// 指定行の文字列取得処理
	void SetOneLine(long,CLine*);									// 指定行の文字列設定処理
	void DeleteOneLine(long);										// 指定行削除処理
	void InsertOneLine(long,CLine*);								// 行挿入処理
	BOOL MyIsDBCSLeadByteEx(ULONG,ULONG);							// ２バイト文字の第一バイトチェック処理
	long GetTabCount(long);											// １行に含まれるタブの数取得処理
	BOOL IsTabCode(ULONG,ULONG);									// タブコードチェック処理
	void AddCharacter(long,long,UINT,UINT,BOOL,CPoint&,CPoint&,BOOL,BOOL);	// 文字登録
	void ProcessReturnCode(long,long,CPoint&,CPoint&,BOOL,BOOL);	// 改行コード処理
	void ProcessDeleteCode(long,long,CPoint&,CPoint&,BOOL,BOOL);	// DELETEコード処理
	void ProcessBackSpaceCode(long,long,CPoint&,CPoint&,BOOL,BOOL);	// BACK SPACEコード処理
	void DeleteSelectedArea(long,long,long,long,BOOL,CPoint&,CPoint&,BOOL,BOOL);// 選択範囲テキスト削除
	CLine* MyGetSelectedText(long,long,long,long);					// 選択範囲テキスト取得
	void ProcessPaste(long,long,long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// 張り付け処理
	void ProcessOleDrop(COleDataObject*,long,long,CPoint&,CPoint&,BOOL,BOOL);// ＯＬＥドロップ処理
	void ProcessReplaceSelectWord(CString&,long,long,long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// 選択範囲置換処理
	void ProcessTabWhenSelected(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// 範囲選択している状態でのＴＡＢキー操作処理
	void ProcessShiftTabWhenSelected(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// 範囲選択している状態でのShift+Tabキー操作処理
	void ProcessDelTopSpaces(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// 範囲選択している領域の先頭空白削除処理
	void ProcessTab2Space(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL,long);	// タブー＞半角スペース変換
	void ProcessSpace2Tab(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL,long);	// 半角スペース－＞タブ変換
	void ProcessSpaceZen2Han(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL,long); // 全角スペース－＞半角スペース変換
	void ProcessSpaceHan2Zen(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL,long); // 半角スペース－＞全角スペース変換
	void ProcessKatakanaHan2Zen(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL,long); // 半角ｶﾀｶﾅ－＞全角カタカナ変換
	void ProcessKatakanaZen2Han(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL,long); // 全角カタカナ－＞半角ｶﾀｶﾅ変換
	void ProcessAlnumHan2Zen(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL,long); // 半角英数字－＞全角英数字変換
	void ProcessAlnumZen2Han(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL,long); // 全角英数字－＞半角英数字変換
	void ProcessInsCppComment(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// 範囲選択ＣＰＰコメント挿入
	void ProcessDelCppComment(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// 範囲選択ＣＰＰコメント削除
	void ProcessInsVbComment(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// 範囲選択ＶＢコメント挿入
	void ProcessDelVbComment(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// 範囲選択ＶＢコメント削除
	void ProcessInsAsmComment(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// 範囲選択アセンブラコメント挿入
	void ProcessDelAsmComment(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// 範囲選択アセンブラコメント削除
	void ProcessInsQuote(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// 範囲選択引用符号挿入
	void ProcessDelQuote(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// 範囲選択引用符号削除
	void ProcessInsDay(long,long,CString&,CPoint&,CPoint&,BOOL,BOOL);// 日付文字列挿入
	void ProcessInsTime(long,long,CString&,CPoint&,CPoint&,BOOL,BOOL);// 時刻文字列挿入
	void ProcessAl2Upper(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// 英字－＞大文字変換
	void ProcessAl2Lower(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);// 英字－＞小文字変換
	void KeyWordColoring(CLine*);									// キーワードカラーリング処理
	void SetFontColor(COLORREF);									// 通常の文字色設定処理
	BOOL GetFlagComment1Clr();										// Ｃ・Ｃ＋＋用コメント有効フラグ取得
	void SetFlagComment1Clr(BOOL);									// Ｃ・Ｃ＋＋用コメント有効フラグ設定
	COLORREF GetCommentColor();										// コメント色取得処理（Ｃ／Ｃ＋＋用）
	void SetCommentColor(COLORREF);									// コメント色設定処理（Ｃ／Ｃ＋＋用）
	BOOL GetFlagComment2Clr();										// ＶＢ用コメント有効フラグ取得
	void SetFlagComment2Clr(BOOL);									// ＶＢ用コメント有効フラグ設定
	COLORREF GetCommentColor2();									// コメント色取得処理（ＶＢ用）
	void SetCommentColor2(COLORREF);								// コメント色設定処理（ＶＢ用）
	BOOL GetFlagComment3Clr();										// アセンブラ用コメント有効フラグ取得
	void SetFlagComment3Clr(BOOL);									// アセンブラ用コメント有効フラグ設定
	COLORREF GetCommentColor3();									// コメント色取得処理（アセンブラ用）
	void SetCommentColor3(COLORREF);								// コメント色設定処理（アセンブラ用）
	BOOL GetFlagComment4Clr();										// 引用符号用コメント有効フラグ取得
	void SetFlagComment4Clr(BOOL);									// 引用符号用コメント有効フラグ設定
	COLORREF GetCommentColor4();									// コメント色取得処理（引用符号用）
	void SetCommentColor4(COLORREF);								// コメント色設定処理（引用符号用）
	BOOL GetFlagUrlColor();											// ＵＲＬ有効フラグ取得
	void SetFlagUrlColor(BOOL);										// ＵＲＬ有効フラグ設定
	COLORREF GetUrlColor();											// ＵＲＬ色取得
	void SetUrlColor(COLORREF);										// ＵＲＬ色設定
	BOOL GetFlagEMailColor();										// ｅ－ｍａｉｌ有効フラグ取得
	void SetFlagEMailColor(BOOL);									// ｅ－ｍａｉｌ有効フラグ設定
	COLORREF GetEMailColor();										// ｅ－ｍａｉｌ色取得
	void SetEMailColor(COLORREF);									// ｅ－ｍａｉｌ色設定
	void DeleteAllKeyWord();
	COLORREF GetKeyNo1Color();										// キーワードＮｏ１の文字色取得処理	
	void SetKeyNo1Color(COLORREF,char*);							// キーワード定義ファイル（ＮＯ１）読み込み＆色設定処理
	COLORREF GetKeyNo2Color();										// キーワードＮｏ２の文字色取得処理	
	void SetKeyNo2Color(COLORREF,char*);							// キーワード定義ファイル（ＮＯ２）読み込み＆色設定処理
	COLORREF GetKeyNo3Color();										// キーワードＮｏ３の文字色取得処理	
	void SetKeyNo3Color(COLORREF,char*);							// キーワード定義ファイル（ＮＯ３）読み込み＆色設定処理
	COLORREF GetKeyNo4Color();										// キーワードＮｏ４の文字色取得処理	
	void SetKeyNo4Color(COLORREF,char*);							// キーワード定義ファイル（ＮＯ４）読み込み＆色設定処理
	void SetConvMode(BOOL);											// 文字コード変換結果表示モード設定処理
	BOOL IsConv();													// 文字コード変換結果表示モード調査
	void AppendConvLine(CLine&);									// 文字コード変換結果１行追加
	void SetEncriptMode(BOOL);										// ファイル暗号化結果表示モード設定処理
	BOOL IsEncript();												// ファイル暗号化結果表示モード調査
	void AppendEncriptLine(CLine&);									// ファイル暗号化結果１行追加
	void SetDecriptMode(BOOL);										// ファイル復号化結果表示モード設定処理
	BOOL IsDecript();												// ファイル復号化結果表示モード調査
	void AppendDecriptLine(CLine&);									// ファイル復号化結果１行追加
	void SetGrepMode(BOOL);											// ＧＲＥＰモード設定処理
	BOOL IsGrep();													// ＧＲＥＰ調査
	void AppendGrepLine(CLine&);									// ＧＲＥＰ結果１行追加
	void AppendHttpLine(CLine&);									// ＨＴＴＰダウンロード結果１行追加
	void SendJumpMsg(long,BOOL);									// 指定行ジャンプ送信
	void SetDumpMode(BOOL);											// ダンプ結果表示モード設定
	BOOL IsDump();													// ダンプ結果表示モード調査
	void AppendDumpLine(CLine&);									// ダンプ結果１行追加
	void GetCRLFCount(long*,long*);									// ＣＲ、ＬＦの数取得処理
	void SetWCountMode(BOOL);										// ワードカウント結果表示モード設定
	BOOL IsWCount();												// ワードカウント結果表示モード調査
	void AppendWCountLine(CLine&);									// ワードカウント結果１行追加
	void SetFSearchMode(BOOL);										// ファイル検索結果表示モード設定
	BOOL IsFSearch();												// ファイル検索結果表示モード調査
	void AppendFSearchLine(CLine&);									// ファイル検索結果１行追加
	void SetEncodeMode(BOOL);										// エンコード結果表示モード設定
	BOOL IsEncode();												// エンコード結果表示モード調査
	void AppendEncodeLine(CLine&);									// エンコード結果１行追加
	void SetBinaryMode(BOOL);										// バイナリ表示モード設定
	BOOL IsBinary();												// バイナリ表示モード調査
	void SetFlagMark(long,BOOL);									// マークフラグ設定
	BOOL GetFlagMark(long);											// マークフラグ取得
	long GetMarkLineCount();										// マーク行数取得
	BOOL GetFlagDelEndSpacesOnSave();								// 保存時行末空白削除フラグ取得
	void SetFlagDelEndSpacesOnSave(BOOL);							// 保存時行末空白削除フラグ設定
	BOOL GetFlagSpace2TabOnSave();									// 保存時空白－＞タブ変換フラグ取得
	void SetFlagSpace2TabOnSave(BOOL);								// 保存時空白－＞タブ変換フラグ設定
	void UuDecode(long,long,long,long);								// ＵＵＤｅｃｏｄｅ
	void Base64Decode(long,long,long,long);							// Ｂａｓｅ６４デコード
	void QuotedPrintableDecode(long,long,long,long);				// Ｑｕｏｔｅｄ－Ｐｒｉｎｔａｂｌｅデコード
	void SetCompareMode(BOOL,char*,char*);							// ファイル比較結果表示モード設定
	BOOL IsCompare();												// ファイル比較結果表示モード調査
	void GetCompareFiles(CString&,CString&);						// ファイル比較　比較ファイル取得
	void AppendCompareLine(CLine*);									// ファイル比較結果１行追加
	void InsertCompareLine(LONG,CLine*);							// ファイル比較結果１行挿入
	int  GetDiffLineKind(long);										// 指定行のファイル比較用、行種別取得
	void SetDirCmpMode(BOOL);										// ディレクトリ比較結果表示モード設定
	BOOL IsDirCmp();												// ディレクトリ比較結果表示モード調査
	void InsertDirCmpLine(LONG,CLine*);								// ディレクトリ比較結果１行挿入
	void SetDirCmpFolder(CString&,CString&);						// ディレクトリ比較用、ディレクトリ名設定
	void GetDirCmpFolder(CString&,CString&);						// ディレクトリ比較用、ディレクトリ名取得
	int  GetDirCmpLineKind(long);									// 指定行のディレクトリ比較用、相違種別取得
	BOOL GetFlagPrintPreview();										// 印刷プレビューフラグ取得
	void SetFlagPrintPreview(BOOL);									// 印刷プレビューフラグ設定
	BOOL IsAboveURL(long,long);										// ＵＲＬ上チェック処理
	BOOL IsAboveEMail(long,long);									// ｅ－ｍａｉｌ上チェック処理

	virtual ~CEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void WaitForMailSendDlgDisp();

private:
	CEditorApp*				m_pApp;							// アプリケーションクラスへのポインタ
	CString					m_strFileName;					// ドキュメントファイル名
	CString					m_strFilePath;					// ドキュメントファイルパス
	CString					m_strFileDir;					// ドキュメントのあるディレクトリパス
	CString					m_strFileExt;					// ファイル拡張子
	DWORD					m_dwFileAttributes;				// ファイル属性
	CTime*					m_pobjLastWrittenTime;			// ファイル最終更新日時
	CLineManager			m_objLineManager;				// テキストドキュメント管理クラス
	CUndoManager			m_objUndoManager;				// Undo,Redo管理クラス
	CColorManager			m_objColorManager;				// キーワードカラー管理クラス
	BOOL					m_bGrepMode;					// ＧＲＥＰモード
	BOOL					m_bConvMode;					// 文字コード変換結果表示モード
	BOOL					m_bDumpMode;					// ダンプ結果表示モード
	BOOL					m_bWCountMode;					// ワードカウント結果表示モード
	BOOL					m_bFSearchMode;					// ファイル検索結果表示モード
	BOOL					m_bBinaryMode;					// バイナリ表示モード
	BOOL					m_bEncriptMode;					// ファイル暗号化結果表示モード
	BOOL					m_bDecriptMode;					// ファイル復号化結果表示モード
	BOOL					m_bEncodeMode;					// エンコード結果表示モード
	BOOL					m_bDelEndSpacesOnSave;			// 保存時の行末空白削除フラグ
	BOOL					m_bSpace2TabOnSave;				// 保存時の空白－＞タブ変換フラグ
	BOOL					m_bCompareMode;					// ファイル比較結果表示モード
	CString					m_objFileCmpFile1;				// ファイル比較用第一ファイルパス
	CString					m_objFileCmpFile2;				// ファイル比較用第二ファイルパス
	BOOL					m_bDirCmpMode;					// ディレクトリ比較結果表示モード
	CString					m_objDirCmpFolder1;				// ディレクトリ比較用第一ディレクトリ名
	CString					m_objDirCmpFolder2;				// ディレクトリ比較用第二ディレクトリ名
	BOOL					m_bIsPrintPreview;				// 印刷プレビュー中フラグ
	CMailSendStatusDlg*		m_pobjMailSendStatusDlg;

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CEditorDoc)
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnViewTab4();
	afx_msg void OnUpdateViewTab4(CCmdUI* pCmdUI);
	afx_msg void OnViewTab8();
	afx_msg void OnUpdateViewTab8(CCmdUI* pCmdUI);
	afx_msg void OnEditClearundobuf();
	afx_msg void OnUpdateEditClearundobuf(CCmdUI* pCmdUI);
	afx_msg void OnFileOpensamename();
	afx_msg void OnUpdateFileOpensamename(CCmdUI* pCmdUI);
	afx_msg void OnFileCopy();
	afx_msg void OnUpdateFileCopy(CCmdUI* pCmdUI);
	afx_msg void OnFileRename();
	afx_msg void OnUpdateFileRename(CCmdUI* pCmdUI);
	afx_msg void OnFileMove();
	afx_msg void OnUpdateFileMove(CCmdUI* pCmdUI);
	afx_msg void OnFileShellopen();
	afx_msg void OnUpdateFileShellopen(CCmdUI* pCmdUI);
	afx_msg void OnEditCode2euc();
	afx_msg void OnUpdateEditCode2euc(CCmdUI* pCmdUI);
	afx_msg void OnEditCode2jis();
	afx_msg void OnUpdateEditCode2jis(CCmdUI* pCmdUI);
	afx_msg void OnEditCode2sjis();
	afx_msg void OnUpdateEditCode2sjis(CCmdUI* pCmdUI);
	afx_msg void OnViewTab2();
	afx_msg void OnUpdateViewTab2(CCmdUI* pCmdUI);
	afx_msg void OnFileSendmail();
	afx_msg void OnUpdateFileSendmail(CCmdUI* pCmdUI);
	afx_msg void OnEditCrlf();
	afx_msg void OnUpdateEditCrlf(CCmdUI* pCmdUI);
	afx_msg void OnEditCr();
	afx_msg void OnUpdateEditCr(CCmdUI* pCmdUI);
	afx_msg void OnEditLf();
	afx_msg void OnUpdateEditLf(CCmdUI* pCmdUI);
	afx_msg void OnEditAllundo();
	afx_msg void OnUpdateEditAllundo(CCmdUI* pCmdUI);
	afx_msg void OnEditAllredo();
	afx_msg void OnUpdateEditAllredo(CCmdUI* pCmdUI);
	afx_msg void OnEditSourcearrange();
	afx_msg void OnUpdateEditSourcearrange(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(EDITORDOC_H)


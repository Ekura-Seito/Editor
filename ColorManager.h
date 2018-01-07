// ColorManager.h: CColorManager クラスのインターフェイス
//

#if !defined(COLORMANAGER_H)		//２重インクルード防止
#define COLORMANAGER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CLine;
class CEditorApp;

class CColorManager
{
public:
	CColorManager();							// コンストラクタ
	virtual ~CColorManager();					// デストラクタ
	void SetFontColor(COLORREF);				// 通常の文字色設定処理
	BOOL GetFlagComment1Clr();					// Ｃ・Ｃ＋＋用コメント有効フラグ取得
	void SetFlagComment1Clr(BOOL);				// Ｃ・Ｃ＋＋用コメント有効フラグ設定
	COLORREF GetCommentColor();					// コメント色取得処理（Ｃ／Ｃ＋＋用）
	void SetCommentColor(COLORREF);				// コメント色設定処理（Ｃ／Ｃ＋＋用）
	BOOL GetFlagComment2Clr();					// ＶＢ用コメント有効フラグ取得
	void SetFlagComment2Clr(BOOL);				// ＶＢ用コメント有効フラグ設定
	COLORREF GetCommentColor2();				// コメント色取得処理（ＶＢ用）
	void SetCommentColor2(COLORREF);			// コメント色設定処理（ＶＢ用）
	BOOL GetFlagComment3Clr();					// アセンブラ用コメント有効フラグ取得
	void SetFlagComment3Clr(BOOL);				// アセンブラ用コメント有効フラグ設定
	COLORREF GetCommentColor3();				// コメント色取得処理（アセンブラ用）
	void SetCommentColor3(COLORREF);			// コメント色設定処理（アセンブラ用）
	BOOL GetFlagComment4Clr();					// 引用符号用コメント有効フラグ取得
	void SetFlagComment4Clr(BOOL);				// 引用符号用コメント有効フラグ設定
	COLORREF GetCommentColor4();				// コメント色取得処理（引用符号用）
	void SetCommentColor4(COLORREF);			// コメント色設定処理（引用符号用）
	BOOL GetFlagUrlColor();						// ＵＲＬ有効フラグ取得
	void SetFlagUrlColor(BOOL);					// ＵＲＬ有効フラグ設定
	COLORREF GetUrlColor();						// ＵＲＬ色取得
	void SetUrlColor(COLORREF);					// ＵＲＬ色設定
	BOOL GetFlagEMailColor();					// ｅ－ｍａｉｌ有効フラグ取得
	void SetFlagEMailColor(BOOL);				// ｅ－ｍａｉｌ有効フラグ設定
	COLORREF GetEMailColor();					// ｅ－ｍａｉｌ色取得
	void SetEMailColor(COLORREF);				// ｅ－ｍａｉｌ色設定
	void KeyWordColoring(CLine*);				// キーワードカラーリング処理
	void DeleteAllKeyWord();
	COLORREF GetKeyNo1Color();					// キーワードＮｏ１の文字色取得処理	
	void SetKeyNo1Color(COLORREF,const char*);	// キーワード定義ファイル（ＮＯ１）読み込み＆色設定処理
	COLORREF GetKeyNo2Color();					// キーワードＮｏ２の文字色取得処理	
	void SetKeyNo2Color(COLORREF,const char*);	// キーワード定義ファイル（ＮＯ２）読み込み＆色設定処理
	COLORREF GetKeyNo3Color();					// キーワードＮｏ３の文字色取得処理	
	void SetKeyNo3Color(COLORREF,const char*);	// キーワード定義ファイル（ＮＯ３）読み込み＆色設定処理
	COLORREF GetKeyNo4Color();					// キーワードＮｏ４の文字色取得処理	
	void SetKeyNo4Color(COLORREF,const char*);	// キーワード定義ファイル（ＮＯ４）読み込み＆色設定処理

private:
	CEditorApp*		m_pApp;						// アプリケーションクラスへのポインタ
	COLORREF		m_FontColor;				// 通常の文字色
	BOOL			m_bComment1Clr;				// Ｃ・Ｃ＋＋コメント有効フラグ
	COLORREF		m_CommentColor;				// コメント色（Ｃ／Ｃ＋＋用）
	BOOL			m_bComment2Clr;				// ＶＢコメント有効フラグ
	COLORREF		m_CommentColor2;			// コメント色（ＶＢ用）
	BOOL			m_bComment3Clr;				// アセンブラコメント有効フラグ
	COLORREF		m_CommentColor3;			// コメント色（アセンブラ用）
	BOOL			m_bComment4Clr;				// 引用符号コメント有効フラグ
	COLORREF		m_CommentColor4;			// コメント色（引用符号用）
	BOOL			m_bUrlClr;					// ＵＲＬ有効フラグ
	COLORREF		m_UrlColor;					// ＵＲＬ色
	BOOL			m_bEMailClr;				// ｅ－ｍａｉｌ有効フラグ
	COLORREF		m_EMailColor;				// ｅ－ｍａｉｌ色
	COLORREF		m_KeyNo1Color;				// キーワードＮｏ１の文字色
	COLORREF		m_KeyNo2Color;				// キーワードＮｏ２の文字色
	COLORREF		m_KeyNo3Color;				// キーワードＮｏ３の文字色
	COLORREF		m_KeyNo4Color;				// キーワードＮｏ４の文字色

	CMapStringToPtr	m_objMapStrToColor;			// キーワード＆色のマップ
};

#endif // !defined(COLORMANAGER_H)


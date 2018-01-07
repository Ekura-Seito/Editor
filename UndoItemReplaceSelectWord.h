// UndoItemReplaceSelectWord.h: CUndoItemReplaceSelectWord クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(UNDOITEMREPLACESELECTWORD_H)		//２重インクルード防止
#define UNDOITEMREPLACESELECTWORD_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CEditorDoc;
class CLine;

class CUndoItemReplaceSelectWord : public CUndoItem
{
public:
	CUndoItemReplaceSelectWord();										// コンストラクタ
	CUndoItemReplaceSelectWord(CEditorDoc*);							// コンストラクタ
	virtual ~CUndoItemReplaceSelectWord();								// デストラクタ

	void	Do();														// 処理実行
	void	Do(CString&,long,long,long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);
	void	doUndo();													// Undo 実行
	void	doRedo();													// Redo 実行
	BOOL	IsCanUndo();												// Undo 可能調査
	BOOL	IsCanRedo();												// Redo 可能調査
	void	setCanUndo(BOOL);											// Undo 可能設定(TRUE:可能,FALSE:不可)
	void	setCanRedo(BOOL);											// Redo 可能設定(TRUE:可能,FALSE:不可)

private:
	CEditorApp*	m_pApp;													// アプリケーションクラスへのポインタ
	BOOL		m_bIsCanUndo;											// Undo 実行可能フラグ
	BOOL		m_bIsCanRedo;											// Redo 実行可能フラグ

	CEditorDoc*	m_pobjEditorDoc;										// ドキュメントクラスへのポインタ

	//操作情報
	long		m_lStartLine;											// 置換開始位置の行番号
	long		m_lStartOffset;											// 置換開始位置の列番号
	long		m_lEndLine;												// 置換終了位置の行番号
	long		m_lEndOffset;											// 置換終了位置の列番号

	long		m_lSelectStartLine;										// 選択開始行番号（置換前）
	long		m_lSelectStartOffset;									// 選択開始列番号（置換前）
	long		m_lSelectEndLine;										// 選択終了行番号（置換前）
	long		m_lSelectEndOffset;										// 選択終了列番号（置換前）

	CLine		m_objSelectWord;										// 選択単語
	CLine		m_objReplaceWord;										// 置換後単語

	BOOL		m_bOriginalModifiedFlag;								// オリジナルのドキュメント修正情報
	CString		m_objOriginalDocTitle;									// オリジナルのドキュメントタイトル
	CPoint		m_objOriginalCaretPos;									// オリジナルのキャレット位置
	CPoint		m_objOriginalScrollPosLog;								// オリジナルのスクロール位置
	BOOL		m_bOriginalDispRuler;									// オリジナルのルーラ表示フラグ
	BOOL		m_bOriginalDispLineNo;									// オリジナルの行番号表示フラグ
};

#endif // !defined(UNDOITEMREPLACESELECTWORD_H)


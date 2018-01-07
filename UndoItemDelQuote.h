// UndoItemDelQuote.h: CUndoItemDelQuote クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(UndoItemDelQuote_H)		//２重インクルード防止
#define UndoItemDelQuote_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorDoc;
class CLine;

class CUndoItemDelQuote : public CUndoItem
{
public:
	CUndoItemDelQuote();								// コンストラクタ
	CUndoItemDelQuote(CEditorDoc*);						// コンストラクタ
	virtual ~CUndoItemDelQuote();						// デストラクタ

	void	Do();											// 処理実行
	void	Do(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);
	void	doUndo();										// Undo 実行
	void	doRedo();										// Redo 実行
	BOOL	IsCanUndo();									// Undo 可能調査
	BOOL	IsCanRedo();									// Redo 可能調査
	void	setCanUndo(BOOL);								// Undo 可能設定(TRUE:可能,FALSE:不可)
	void	setCanRedo(BOOL);								// Redo 可能設定(TRUE:可能,FALSE:不可)

private:
	BOOL		m_bIsCanUndo;								// Undo 実行可能フラグ
	BOOL		m_bIsCanRedo;								// Redo 実行可能フラグ

	CEditorDoc*	m_pobjEditorDoc;							// ドキュメントクラスへのポインタ

	//操作情報
	long		m_lStartLine;								// 引用符号削除処理後の開始位置の行番号
	long		m_lStartOffset;								// 引用符号削除処理後の開始位置の列番号
	long		m_lEndLine;									// 引用符号削除処理後の終了位置の行番号
	long		m_lEndOffset;								// 引用符号削除処理後の終了位置の列番号

	long		m_lSelectStartLine;							// 選択開始行番号（引用符号処理前）
	long		m_lSelectStartOffset;						// 選択開始列番号（引用符号処理前）
	long		m_lSelectEndLine;							// 選択終了行番号（引用符号処理前）
	long		m_lSelectEndOffset;							// 選択終了列番号（引用符号処理前）

	long		m_lSelectLineCnt;							// 選択対象の行数
	CLine**		m_ppobjSelectLine;							// 選択対象テキスト

	BOOL		m_bOriginalModifiedFlag;					// オリジナルのドキュメント修正情報
	CString		m_objOriginalDocTitle;						// オリジナルのドキュメントタイトル
	CPoint		m_objOriginalCaretPos;						// オリジナルのキャレット位置
	CPoint		m_objOriginalScrollPosLog;					// オリジナルのスクロール位置
	BOOL		m_bOriginalDispRuler;						// オリジナルのルーラ表示フラグ
	BOOL		m_bOriginalDispLineNo;						// オリジナルの行番号表示フラグ
};

#endif // !defined(UndoItemDelQuote_H)


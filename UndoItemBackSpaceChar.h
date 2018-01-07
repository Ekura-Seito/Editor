// UndoItemBackSpaceChar.h: CUndoItemBackSpaceChar クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(UNDOITEMBACKSPACECHAR_H)		//２重インクルード防止
#define UNDOITEMBACKSPACECHAR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorDoc;

class CUndoItemBackSpaceChar : public CUndoItem
{
public:
	CUndoItemBackSpaceChar();					// コンストラクタ
	CUndoItemBackSpaceChar(CEditorDoc*);		// コンストラクタ
	virtual ~CUndoItemBackSpaceChar();			// デストラクタ

	void	Do();								// 処理実行
	void	Do(long,long,CPoint&,CPoint&,BOOL,BOOL);
	void	doUndo();							// Undo 実行
	void	doRedo();							// Redo 実行
	BOOL	IsCanUndo();						// Undo 可能調査
	BOOL	IsCanRedo();						// Redo 可能調査
	void	setCanUndo(BOOL);					// Undo 可能設定(TRUE:可能,FALSE:不可)
	void	setCanRedo(BOOL);					// Redo 可能設定(TRUE:可能,FALSE:不可)

private:
	BOOL		m_bIsCanUndo;					// Undo 実行可能フラグ
	BOOL		m_bIsCanRedo;					// Redo 実行可能フラグ

	CEditorDoc*	m_pobjEditorDoc;				// ドキュメントクラスへのポインタ

	//操作情報
	long		m_lCurrentLine;					// 文字 BACK SPACE 対象の行番号
	long		m_lCurrentOffset;				// 文字 BACK SPACE 対象の列番号（ﾊﾞｲﾄ単位）
	long		m_lAfterLine;					// BACK SPACE 後の行番号
	long		m_lAfterOffset;					// BACK SPACE 後の列番号
	char		m_cOriginal1stChar;				// BACK SPACE 位置のオリジナル文字第一ﾊﾞｲﾄ
	char		m_cOriginal2ndChar;				// BACK SPACE 位置のオリジナル文字第二ﾊﾞｲﾄ
	BOOL		m_bOriginalModifiedFlag;		// オリジナルのドキュメント修正情報
	CString		m_objOriginalDocTitle;			// オリジナルのドキュメントタイトル
	CPoint		m_objOriginalCaretPos;			// オリジナルのキャレット位置
	CPoint		m_objAfterCaretPos;				// BACK SPACE後のキャレット位置
	CPoint		m_objPreLineEndPos;				// BACK SPACE対象の前行の行末位置
	CPoint		m_objOriginalScrollPosLog;		// オリジナルのスクロール位置
	BOOL		m_bOriginalDispRuler;			// オリジナルのルーラ表示フラグ
	BOOL		m_bOriginalDispLineNo;			// オリジナルの行番号表示フラグ
};

#endif // !defined(UNDOITEMBACKSPACECHAR_H)

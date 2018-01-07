// UndoItemReturn.h: CUndoItemReturn クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(UNDOITEMRETURN_H)		//２重インクルード防止
#define UNDOITEMRETURN_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorDoc;

class CUndoItemReturn : public CUndoItem
{
public:
	CUndoItemReturn();							// コンストラクタ
	CUndoItemReturn(CEditorDoc*);				// コンストラクタ
	virtual ~CUndoItemReturn();					// デストラクタ

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
	long		m_lCurrentLine;					// RETURN処理時の行番号
	long		m_lCurrentOffset;				// RETURN処理時の列番号（ﾊﾞｲﾄ単位）
	BOOL		m_bOriginalModifiedFlag;		// オリジナルのドキュメント修正情報
	CString		m_objOriginalDocTitle;			// オリジナルのドキュメントタイトル
	CPoint		m_objOriginalCaretPos;			// オリジナルのキャレット位置
	CPoint		m_objOriginalScrollPosLog;		// オリジナルのスクロール位置
	BOOL		m_bOriginalDispRuler;			// オリジナルのルーラ表示フラグ
	BOOL		m_bOriginalDispLineNo;			// オリジナルの行番号表示フラグ
};

#endif // !defined(UNDOITEMRETURN_H)

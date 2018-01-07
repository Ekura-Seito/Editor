// UndoItemInsTime.h: CUndoItemInsTime クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(UndoItemInsTime_H)		//２重インクルード防止
#define UndoItemInsTime_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorDoc;

class CUndoItemInsTime : public CUndoItem
{
public:
	CUndoItemInsTime();										// コンストラクタ
	CUndoItemInsTime(CEditorDoc*);							// コンストラクタ
	virtual ~CUndoItemInsTime();							// デストラクタ

	void	Do();											// 処理実行
	void	Do(long,long,CString&,CPoint&,CPoint&,BOOL,BOOL);
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
	long		m_lCaretLine;								// 時刻挿入位置の行番号
	long		m_lCaretOffset;								// 時刻挿入位置の列番号
	long		m_lSelectStartLine;							// 時刻挿入処理後の選択開始行番号
	long		m_lSelectStartOffset;						// 時刻挿入処理後の選択開始列番号
	long		m_lSelectEndLine;							// 時刻挿入処理後の選択終了行番号
	long		m_lSelectEndOffset;							// 時刻挿入処理後の選択終了列番号

	CString		m_objTimeStr;								// 時刻文字列

	BOOL		m_bOriginalModifiedFlag;					// オリジナルのドキュメント修正情報
	CString		m_objOriginalDocTitle;						// オリジナルのドキュメントタイトル
	CPoint		m_objOriginalCaretPos;						// オリジナルのキャレット位置
	CPoint		m_objOriginalScrollPosLog;					// オリジナルのスクロール位置
	BOOL		m_bOriginalDispRuler;						// オリジナルのルーラ表示フラグ
	BOOL		m_bOriginalDispLineNo;						// オリジナルの行番号表示フラグ
};

#endif // !defined(UndoItemInsTime_H)


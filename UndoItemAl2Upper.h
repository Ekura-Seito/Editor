// UndoItemAl2Upper.h: CUndoItemAl2Upper クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(UndoItemAl2Upper_H)		//２重インクルード防止
#define UndoItemAl2Upper_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorDoc;
class CLine;

class CUndoItemAl2Upper : public CUndoItem
{
public:
	CUndoItemAl2Upper();									// コンストラクタ
	CUndoItemAl2Upper(CEditorDoc*);							// コンストラクタ
	virtual ~CUndoItemAl2Upper();							// デストラクタ

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
	long		m_lSelectStartLine;							// 選択開始行番号
	long		m_lSelectStartOffset;						// 選択開始列番号
	long		m_lSelectEndLine;							// 選択終了行番号
	long		m_lSelectEndOffset;							// 選択終了列番号

	long		m_lSelectLineCnt;							// 選択対象の行数
	CLine**		m_ppobjSelectLine;							// 選択対象テキスト

	BOOL		m_bOriginalModifiedFlag;					// オリジナルのドキュメント修正情報
	CString		m_objOriginalDocTitle;						// オリジナルのドキュメントタイトル
	CPoint		m_objOriginalCaretPos;						// オリジナルのキャレット位置
	CPoint		m_objOriginalScrollPosLog;					// オリジナルのスクロール位置
	BOOL		m_bOriginalDispRuler;						// オリジナルのルーラ表示フラグ
	BOOL		m_bOriginalDispLineNo;						// オリジナルの行番号表示フラグ
};

#endif // !defined(UndoItemAl2Upper_H)


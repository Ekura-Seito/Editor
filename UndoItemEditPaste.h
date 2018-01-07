// UndoItemEditPaste.h: CUndoItemEditPaste クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(UNDOITEMEDITPASTE_H)		//２重インクルード防止
#define UNDOITEMEDITPASTE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CEditorDoc;
class CLine;

class CUndoItemEditPaste : public CUndoItem
{
public:
	CUndoItemEditPaste();										// コンストラクタ
	CUndoItemEditPaste(CEditorDoc*);							// コンストラクタ
	virtual ~CUndoItemEditPaste();								// デストラクタ

	void	Do();												// 処理実行
	void	Do(long,long,long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);
	void	doUndo();											// Undo 実行
	void	doRedo();											// Redo 実行
	BOOL	IsCanUndo();										// Undo 可能調査
	BOOL	IsCanRedo();										// Redo 可能調査
	void	setCanUndo(BOOL);									// Undo 可能設定(TRUE:可能,FALSE:不可)
	void	setCanRedo(BOOL);									// Redo 可能設定(TRUE:可能,FALSE:不可)

private:
	CEditorApp*	m_pApp;											// アプリケーションクラスへのポインタ
	BOOL		m_bIsCanUndo;									// Undo 実行可能フラグ
	BOOL		m_bIsCanRedo;									// Redo 実行可能フラグ

	CEditorDoc*	m_pobjEditorDoc;								// ドキュメントクラスへのポインタ

	//操作情報
	long		m_lStartLine;									// 貼り付け開始位置の行番号
	long		m_lStartOffset;									// 貼り付け開始位置の列番号
	long		m_lEndLine;										// 貼り付け終了位置の行番号
	long		m_lEndOffset;									// 貼り付け終了位置の列番号

	long		m_lSelectStartLine;								// 選択開始行番号（貼り付け前）
	long		m_lSelectStartOffset;							// 選択開始列番号（貼り付け前）
	long		m_lSelectEndLine;								// 選択終了行番号（貼り付け前）
	long		m_lSelectEndOffset;								// 選択終了列番号（貼り付け前）

	CObArray	m_objSelectLineArray;							// 選択テキストの配列
	CObArray	m_objPasteLineArray;							// 貼り付けテキストの配列

	BOOL		m_bOriginalModifiedFlag;						// オリジナルのドキュメント修正情報
	CString		m_objOriginalDocTitle;							// オリジナルのドキュメントタイトル
	CPoint		m_objOriginalCaretPos;							// オリジナルのキャレット位置
	CPoint		m_objOriginalScrollPosLog;						// オリジナルのスクロール位置
	BOOL		m_bOriginalDispRuler;							// オリジナルのルーラ表示フラグ
	BOOL		m_bOriginalDispLineNo;							// オリジナルの行番号表示フラグ
	BOOL		m_bSetCaretToEndOnPaste;						// 貼り付け後にキャレットを最後に移動するか？
};

#endif // !defined(UNDOITEMEDITPASTE_H)

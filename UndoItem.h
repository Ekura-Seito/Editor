// UndoItem.h: CUndoItem クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(UNDOITEM_H)		//２重インクルード防止
#define UNDOITEM_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CUndoItem : public CObject
{
public:
	CUndoItem();							//コンストラクタ
	virtual ~CUndoItem();					//デストラクタ

	virtual void setCanUndo(BOOL) = 0;		//Undo 可能設定(TRUE:可能,FALSE:不可)
	virtual void setCanRedo(BOOL) = 0;		//Redo 可能設定(TRUE:可能,FALSE:不可)
	virtual BOOL IsCanUndo() = 0;			//Undo 可能調査(TRUE:可能,FALSE:不可)
	virtual BOOL IsCanRedo() = 0;			//Redo 可能調査(TRUE:可能,FALSE:不可)
	virtual void Do() = 0;					//処理実行
	virtual void doUndo() = 0;				//Undo 実行
	virtual void doRedo() = 0;				//Redo 実行

protected:
};

#endif // !defined(UNDOITEM_H)

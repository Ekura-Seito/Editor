// UndoManager.h: CUndoManager クラスのインターフェイス
//

#if !defined(UNDOMANAGER_H)		//２重インクルード防止
#define UNDOMANAGER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CUndoItem;

class CUndoManager
{
public:
	CUndoManager();							//コンストラクタ
	virtual ~CUndoManager();				//デストラクタ
	void		Initialize();				//初期化用

	void		addUndoItem(CUndoItem*);	//UndoItemオブジェクトを追加
	BOOL		IsCanUndo();				//Undo 可能調査(TRUE:可能、FALSE:不可)
	BOOL		IsCanRedo();				//Redo 可能調査(TRUE:可能、FALSE:不可)
	BOOL		doUndo();					//Undo 実行
	BOOL		doRedo();					//Redo 実行

private:
	POSITION	m_lCurrentPosition;			//UndoItemオブジェクトを次に入れる位置
	CObList		m_objUndoItemList;			//CUndoItem のリスト

protected:
	void		clearAllItem();				//リストのメンバをすべてクリア
};

#endif // !defined(UNDOMANAGER_H)


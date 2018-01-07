// OperationManager.h: COperationManager クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(OperationManager_H)		//２重インクルード防止
#define OperationManager_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class COperation;
class CEditorView;
class CEditorDoc;

typedef CTypedPtrList<CObList, COperation*> COperationList;

class COperationManager
{
public:
	COperationManager();											// コンストラクタ
	virtual 	~COperationManager();								// デストラクタ

	void		Initialize();										// 初期化
	void		DispOperationLists();								// オペレーション一覧表示
	void		SetOperationFileName(CString&);						// オペレーションファイル名設定
	void		GetOperationFileName(CString&);						// オペレーションファイル名取得
	void		SetOperationName(CString&);							// オペレーション名設定
	void		GetOperationName(CString&);							// オペレーション名取得
	long		GetOperationCount();								// オペレーション数取得
	void		addOperation(COperation*);							// オペレーションオブジェクトを追加
	void		doOperation(CEditorView*,CEditorDoc*);				// オペレーション連続実行
	BOOL		stepDoOperation(CEditorView*,CEditorDoc*,BOOL);		// オペレーションのステップ実行
	void		endStepDoOperation();								// オペレーションのステップ実行終了
	BOOL		checkIsOperationFile(CArchive&);					// オペレーションファイルチェック
	BOOL		Serialize(CArchive&);								// シリアライズ

	void		SearchStartOperation();								// オペレーション順次検索開始
	COperation* SearchGetOperation();								// オペレーション順次検索
	void		SearchEndOperation();								// オペレーション順次検索終了

protected:
	void		clearAllItem();										// リストのメンバをすべてクリア

private:
	CEditorApp*	m_pApp;												// アプリケーションクラスへのポインタ
	CString		m_objOperationFileName;								// オペレーションファイル名
	CString		m_objOperationName;									// オペレーション名
	POSITION	m_lCurrentPosition;									// オペレーションオブジェクトを次に入れる位置
	COperationList m_objOperationList;								// COperation のリスト
};

#endif // !defined(OperationManager_H)


// DirCompare.h : CDirCompare クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(DIRCOMPARE_H)		//２重インクルード防止
#define DIRCOMPARE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CEditorDoc;
class CDirCompareStatusDlg;

class CDirCompare
{
public:
	CDirCompare();									// コンストラクタ
	virtual ~CDirCompare();							// デストラクタ

	BOOL DoCompare(CDirCompareStatusDlg*,CEditorDoc*,CString&,CString&);

protected:
	BOOL MakeFileList(CString&,CObArray*);			//ディレクトリ内ファイル一覧作成
	void AddFileBySort(CString&,CObArray*);
	BOOL SearchLCS();								//最長共通部分（ＬＣＳ : Longest Common Subsequence）探索
	BOOL OutputResult();							//結果出力
	int  CheckIsSameFile(CString*,int*);			//ファイル同一チェック
	LONG MakeNodeNo(LONG,LONG);						//ノード番号取得
	void MakeXY(LONG,LONG*,LONG*);					//
	void SetValuePreIndex(LONG,LONG);				//前ノード設定
	LONG GetValuePreIndex(LONG);					//前ノード取得
	void SetValueFarY(LONG,LONG);
	LONG GetValueFarY(LONG);
	void Clear();

private:
	CEditorApp*		m_pApp;							//アプリケーションクラスへのポインタ
	CDirCompareStatusDlg* m_pobjDirCompareStatusDlg;
	CEditorDoc*		m_pobjCompareDoc;				//ドキュメントクラス
	CObArray 		m_objStringArray1;				//第一ディレクトリのファイル名配列
	CObArray		m_objStringArray2;				//第二ディレクトリのファイル名配列
	long			m_lFileCount1;					//第一ディレクトリ中のファイル数
	long			m_lFileCount2;					//第二ディレクトリ中のファイル数
	map<long,long> 	m_objMapPreIndex;				//前ノードのマップ
	map<long,long> 	m_objMapFarY;					//各対角線の最遠点のｙ座標値のマップ
	CString			m_objDir1;						//第一ディレクトリ名
	CString			m_objDir2;						//第二ディレクトリ名
};

#endif //!defined(DIRCOMPARE_H)


// FileCompare.h : CFileCompare クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(FILECOMPARE_H)		//２重インクルード防止
#define FILECOMPARE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CEditorDoc;
class CFileCompareStatusDlg;

class CFileCompare
{
public:
	CFileCompare();									// コンストラクタ
	virtual ~CFileCompare();						// デストラクタ

	BOOL DoCompare(CFileCompareStatusDlg*,CEditorDoc*,CArchive&,CArchive&);
	void GetDelAddLineCount(long*,long*);

protected:
	BOOL ReadFile(CArchive&,CObArray*);				//ファイル読み込み
	BOOL SearchLCS();								//最長共通部分（ＬＣＳ : Longest Common Subsequence）探索
	BOOL OutputResult();							//結果出力
	LONG MakeNodeNo(LONG,LONG);						//ノード番号取得
	void MakeXY(LONG,LONG*,LONG*);					//
	void SetValuePreIndex(LONG,LONG);				//前ノード設定
	LONG GetValuePreIndex(LONG);					//前ノード取得
	void SetValueFarY(LONG,LONG);
	LONG GetValueFarY(LONG);
	void Clear();

private:
	CEditorApp*		m_pApp;							//アプリケーションクラスへのポインタ
	CFileCompareStatusDlg* m_pobjFileCompareStatusDlg;
	CEditorDoc*		m_pobjCompareDoc;				//ドキュメントクラス
	CObArray 		m_objStringArray1;				//第一ファイルの行データ配列
	CObArray		m_objStringArray2;				//第二ファイルの行データ配列
	long			m_lFileLineCount1;				//第一ファイルの行数
	long			m_lFileLineCount2;				//第二ファイルの行数
	long			m_lDelLineCount;				//削除行の数
	long			m_lAddLineCount;				//追加行の数
	map<long,long> 	m_objMapPreIndex;				//前ノードのマップ
	map<long,long>	m_objMapFarY;					//各対角線の最遠点のｙ座標値のマップ
};

#endif //!defined(FILECOMPARE_H)


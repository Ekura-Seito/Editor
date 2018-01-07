// LineManager.h: CLineManager クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(LINEMANAGER_H)		//２重インクルード防止
#define LINEMANAGER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CLine;

class CLineManager
{
public:
	CLineManager();										// コンストラクタ
	virtual ~CLineManager();							// デストラクタ

	void Initialize();									// 初期化
	void Serialize(CArchive& ar);						// シリアライズ
	void SerializeBinary(CArchive& ar);					// シリアライズ（バイナリモード）
	void GetBytes(long,long*,long*);					// バイト数取得
	long GetTabSize();									// タブサイズ取得
	void SetTabSize(long);								// タブサイズ設定
	long GetLineCount();								// 行数取得処理
	CLine* GetOneLine(long);							// 指定行の文字列取得処理
	void SetOneLine(long,CLine*);						// 指定行の文字列設定処理
	void DeleteOneLine(long);							// 指定行削除処理
	void InsertOneLine(long,CLine*);					// 行挿入処理
	long GetMaxLineLength();							// １行の長さの最大値を取得
	long GetLineLength(long);							// 指定行の文字列長取得処理
	void AddCharacter(long,long,UINT,UINT,BOOL);		// 文字登録
	void ProcessReturnCode(long,long);					// 改行コード処理
	void ProcessDeleteCode(long,long,unsigned char*,unsigned char*);	// DELETEコード処理
	void ProcessBackSpaceCode(long,long,unsigned char*,unsigned char*);	// BACK SPACE処理
	CLine* MyGetSelectedText(long,long,long,long);		// 選択範囲テキスト取得
	void AppendGrepLine(CLine&);						// ＧＲＥＰ結果１行追加
	void AppendHttpLine(CLine&);						// ＨＴＴＰダウンロード結果１行追加
	void AppendConvLine(CLine&);						// 文字コード変換結果１行追加
	void AppendDumpLine(CLine&);						// ダンプ結果１行追加
	void AppendWCountLine(CLine&);						// ワードカウント結果１行追加
	void AppendFSearchLine(CLine&);						// ファイル検索結果１行追加
	void AppendBinaryLine(CLine&);						// バイナリ表示１行追加
	void AppendEncriptLine(CLine&);						// ファイル暗号化結果１行追加
	void AppendDecriptLine(CLine&);						// ファイル復号化結果１行追加
	void AppendEncodeLine(CLine&);						// エンコード結果１行追加
	void AppendCompareLine(CLine*);						// ファイル比較結果１行追加
	void InsertCompareLine(long,CLine*);				// ファイル比較結果１行挿入
	void InsertDirCmpLine(long,CLine*);					// ディレクトリ比較結果１行挿入
	int  GetDiffLineKind(long);							// ファイル比較用行種別取得
	int  GetDirCmpLineKind(long);						// ディレクトリ比較用相違種別取得
	void GetCRLFCount(long*,long*);						// ＣＲ、ＬＦの数取得処理
	long GetKaigyoKind();								// 改行コード区分取得
	void SetKaigyoKind(long);							// 改行コード区分設定
	void SetFlagMark(long,BOOL);						// マークフラグ設定
	BOOL GetFlagMark(long);								// マークフラグ取得
	long GetMarkLineCount();							// マーク行数取得
	void DelEndSpaces(long);							// 指定行の行末空白削除処理
	void ConvSpace2TabOnSave(long,long);				// 保存時用スペース－＞タブ変換処理

private:
	CEditorApp* m_pApp;									// アプリケーションクラスへのポインタ
	long		m_lMarkLineCount;						// マーク行数
	long		m_lCRCount;
	long		m_lLFCount;
	long		m_lKaigyoKind;							// 使用する改行コードの区分
	long		m_lTabSize;								// タブサイズ（４or８）
	long		m_lMaxLineLength;						// １行の長さの最大値
	CObArray 	m_objStringArray;						// ドキュメントの行単位の配列

protected:
	void Clear();										// ドキュメントの行単位の配列を初期化
};

#endif // !defined(LINEMANAGER_H)


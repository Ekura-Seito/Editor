// Line.h: CLine クラスのインターフェイス
//

#if !defined(LINE_H)		//２重インクルード防止
#define LINE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CLine : public CObject
{
	friend class CLine;

public:
	CLine();												// コンストラクタ
	CLine(const unsigned char*);							// コンストラクタ
	virtual ~CLine();										// デストラクタ
	unsigned char operator[](long) const;					// １文字取得
	unsigned char GetAt(long) const;						// １文字取得
	void SetAt(long,unsigned char);							// １文字設定
	long GetLength();										// 文字列長取得
	void PadSpace(long);									// スペースパディング
	void Set(const unsigned char*);							// １行設定
	void Catenate(const unsigned char*);					// 連結
	void Catenate(unsigned char);							// １文字連結
	void Insert(long,const unsigned char*);					// 文字列挿入
	void InsertChar(long,unsigned char,unsigned char);		// 文字挿入
	void InsertChars(long,unsigned char,long);				// 文字指定個数挿入
	void OverWriteChar(long,unsigned char,unsigned char);	// 文字上書き
	void DeleteChar(long,unsigned char*,unsigned char*);	// 文字削除(DELETE)
	void BackSpaceChar(long,unsigned char*,unsigned char*);	// 文字削除(BACK SPACE)
	void Trim(long,long);									// 文字列切り取り
	long GetTabCount();										// タブ数取得
	COLORREF GetColorAt(long);								// 色取得処理
	void SetColorAt(long,COLORREF);							// 色設定処理
	void SetColorAt(long,long,COLORREF);					// 色設定処理
	BOOL GetUnderLineAt(long);								// 下線情報取得処理
	void SetUnderLineAt(long,BOOL);							// 下線情報設定処理
	long DoFind(long,long,CString,BOOL,BOOL,BOOL,BOOL,long*);// 検索実行
	long DoReverseFind(long,long,CString&,BOOL,BOOL,BOOL,BOOL,long*);// 検索実行（逆方向）
	void ParseGrepResult(CString&,long*);					// ＧＲＥＰ結果解釈
	void SaveLength();										// 文字列長保持
	long GetSavedLength();									// 保持しておいた文字列長取得
	void SetFlagMark(BOOL);									// マークフラグ設定
	BOOL GetFlagMark();										// マークフラグ取得
	void DelEndChar();										// 最終文字　１文字削除
	void DelTopSpaces();									// 行頭の空白文字削除
	void DelEndSpaces();									// 行末の空白文字削除
	long GetIndent(long);									// インデント数取得
	int	 Compare(CLine*);									// 文字列比較
	int  Compare(long,const unsigned char*,long);			// 文字列比較
	void SetDiffLineKind(int);								// ファイル比較用、行種別設定
	int  GetDiffLineKind();									// ファイル比較用、行種別取得
	void SetDirCmpLineKind(int);							// ディレクトリ比較用、行種別設定
	int  GetDirCmpLineKind();								// ディレクトリ比較用、行種別取得
	void SetDirCmpFileName(CString&);						// ディレクトリ比較でのファイル名設定
	void GetDirCmpFileName(CString&);						// ディレクトリ比較でのファイル名取得
	long GetPreOffset(long);								// 直前オフセット取得
	BOOL GetUrl(long,CString&);								// キャレット位置のＵＲＬ取得
	BOOL IsAboveURL(long);									// ＵＲＬ上チェック
	BOOL GetEMail(long,CString&);							// キャレット位置のｅ－ｍａｉｌ取得
	BOOL IsAboveEMail(long);								// ｅ－ｍａｉｌ上チェック
	const unsigned char* Data();							// 先頭データのポインタ取得
	const unsigned char* Data(long);
	const unsigned char* String();							// 文字列取得
	const unsigned char* SubString(long);					// 部分文字列取得
	void ResizeColorBuffer(long,unsigned char);
	void ResizeUnderLineBuffer(long,unsigned char);
	void Spaces2One();										// 複数の空白文字を１つに
	void Al2Upper();										// 英字－＞大文字化
	void AlnumZen2Han();									// 英数字－＞半角化
	void KatakanaZen2Han();									// カタカナ－＞半角化
	void SpaceZen2Han();									// 空白－＞半角化

private:
	CEditorApp*	m_pApp;
	CString		m_strLine;									// １行データ
	CString		m_strBuf;
	CObArray	m_Color;									// 色情報
	CObArray	m_bNeedUnderLine;							// 下線情報
	BOOL		m_bMark;									// マークフラグ
	int			m_nDiffLineKind;							// ファイル比較用、行種別
	int			m_nDirCmpLineKind;							// ディレクトリ比較用、行種別
	CString		m_objDirCmpFileName;						// ディレクトリ比較でのファイル名
	long		m_lSavedLineLength;
};

#endif // !defined(LINE_H)


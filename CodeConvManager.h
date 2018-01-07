// CodeConvManager.h : CCodeConvManager クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(CODECONVMANAGER_H)		//２重インクルード防止
#define CODECONVMANAGER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//入力文字コード種別定義
#define INPUT_SJIS		1
#define INPUT_EUC		2
#define INPUT_JIS		3
#define INPUT_ASCII		4
#define INPUT_UNKNOWN	5
#define INPUT_BINARY	6

//文字コード定義
#define CODE_ESC 			0x1b
#define CODE_SI 			0x0f			//Shift In

//コード判定マクロ
#define iskanji1st(c) 	((c) >= 0x81 && (c) <= 0x9f || (c) >= 0xe0 && (c) <= 0xfc)
#define iskanji2nd(c) 	((c) >= 0x40 && (c) <= 0xfc && (c) != 0x7f)
#define iseuc(c) 		((c) >= 0xa1 && (c) <= 0xfe)
#define isjis(c) 		((c) >= 0x21 && (c) <= 0x7f)
#ifndef isHanKana
#define isHanKana(c) 	((c) >= 0xa0 && (c) <= 0xdf)
#endif

//使用する改行コードの区分
#define KAIGYO_CRLF		0
#define KAIGYO_LF		1
#define KAIGYO_CR		2
#define KAIGYO_UNKNOWN	3

//改行コード変換種別
#define KAIGYOCONV_TO_CRLF		"---> CR + LF"
#define KAIGYOCONV_TO_LF		"---> LF"
#define KAIGYOCONV_TO_CR		"---> CR"


class CCodeConvManager
{
public:
	CCodeConvManager();									// コンストラクタ
	virtual ~CCodeConvManager();						// デストラクタ

	UINT DetectCode(const char*);						// ファイルのコード判別
	UINT DetectKaigyoCode(const char*);					// ファイルの改行コード判別

	INT ConvertSjisToEuc(const char*,const char*);		// SJIS -> EUC  コード変換
	INT ConvertSjisToJis(const char*,const char*);		// SJIS -> JIS  コード変換
	INT ConvertEucToSjis(const char*,const char*);		// EUC  -> SJIS コード変換
	INT ConvertEucToJis(const char*,const char*);		// EUC  -> JIS  コード変換
	INT ConvertJisToSjis(const char*,const char*);		// JIS  -> SJIS コード変換
	INT ConvertJisToEuc(const char*,const char*);		// JIS  -> EUC  コード変換

	INT StringKatakanaHan2Zen(CString*);				// 文字列中のカタカナ 半角－＞全角変換

	INT StringSjisToEuc(CString*);						// 文字列のSJIS -> EUC  コード変換
	INT StringSjisToJis(CString*);						// 文字列のSJIS -> JIS  コード変換
	INT StringEucToSjis(CString*);						// 文字列のEUC  -> SJIS コード変換
	INT StringEucToJis(CString*);						// 文字列のEUC  -> JIS  コード変換
	INT StringJisToSjis(CString*);						// 文字列のJIS  -> SJIS コード変換
	INT StringJisToEuc(CString*);						// 文字列のJIS  -> EUC  コード変換

	BOOL IsMaruKana(unsigned int);						// パ、プ、ポ　などの丸付きカタカナの判定
	BOOL IsDakuonKana(unsigned int);					// バ、ガ、ザ　などの濁音付きカタカナの判定
	unsigned int AddMaruToKana(unsigned int);			// 全角カタカナに丸を追加
	unsigned int AddDakuonToKana(unsigned int);			// 全角カタカナに濁音を追加

	BOOL IsZenkakuAlnum(unsigned int);					// 全角英数字判定
	unsigned int AlnumToZen(unsigned int);				// 英数字　半角－＞全角
	unsigned int AlnumToHan(unsigned int);				// 英数字　全角－＞半角

protected:
	BOOL IsBinary(const char*);							// 入力ファイルバイナリチェック
	UINT whatKanji(UCHAR*);								// コード判別
	UINT detectcode(unsigned char*,unsigned int,BOOL*);
	UINT checkforward(unsigned char*,unsigned int);

	unsigned int KanaHan2Zen(unsigned int);				// 半角カナを全角カナ(JIS)に変換する
	unsigned int KanaZen2Han(unsigned int);				// 全角カナ(JIS)を半角カナに変換する
	unsigned int sjis2euc(unsigned int);				// ＳＪＩＳ－＞ＥＵＣ
	unsigned int sjis2jis(unsigned int);				// ＳＪＩＳ－＞ＪＩＳ
	unsigned int euc2sjis(unsigned int);				// ＥＵＣ－＞ＳＪＩＳ
	unsigned int euc2jis(unsigned int);					// ＥＵＣ－＞ＪＩＳ
	unsigned int jis2sjis(unsigned int);				// ＪＩＳ－＞ＳＪＩＳ
	unsigned int jis2euc(unsigned int);					// ＪＩＳ－＞ＥＵＣ
	void set1byte(CArchive&);							// １バイト文字用にエスケープシーケンスを出力
	void set2byte(CArchive&);							// ２バイト文字用にエスケープシーケンスを出力
	void set1byte(CString*);
	void set2byte(CString*);
};

#endif // !defined(CODECONVMANAGER_H)


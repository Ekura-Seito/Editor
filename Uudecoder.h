// Uudecode.h : CUudecoder クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(UUDECODER_H)		//２重インクルード防止
#define UUDECODER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/* Single character decode.  */
#define	DEC(Char) (((Char) - ' ') & 077)

class CUudecoder
{
public:
	CUudecoder();									// コンストラクタ
	virtual ~CUudecoder();							// デストラクタ

	INT  StringDecode(CString*,LPBYTE,long*);		// 文字列のUudecodeデコード
};

#endif // !defined(UUDECODER_H)


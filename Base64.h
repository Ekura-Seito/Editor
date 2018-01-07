// Base64.h : CBase64 クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(BASE64_H)		//２重インクルード防止
#define BASE64_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CBase64
{
public:
	CBase64();									// コンストラクタ
	virtual ~CBase64();							// デストラクタ

	INT	 StringEncode(CString*);				// 文字列のBase64エンコード
	INT  StringDecode(CString*,LPBYTE,long*);	// 文字列のBase64デコード
	CString BytesEncode(LPBYTE,UINT);

protected:
	BYTE B64ToBin(BYTE);
	BYTE BinToB64(BYTE);
};

#endif // !defined(BASE64_H)


// QuotedPrintable.h : CQuotedPrintable クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(QuotedPrintable_H)		//２重インクルード防止
#define QuotedPrintable_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CQuotedPrintable
{
public:
	CQuotedPrintable();								// コンストラクタ
	virtual ~CQuotedPrintable();					// デストラクタ

	INT	 StringEncode(CString*);					// 文字列のエンコード
	INT  StringDecode(CString*,LPBYTE,long*);
	CString BytesEncode(LPBYTE,UINT);

protected:
	BOOL DataCheck(BYTE,int);
};

#endif // !defined(QuotedPrintable_H)


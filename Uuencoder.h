// Uuencoder.h : CUuencoder クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(UUENCODER_H)		//２重インクルード防止
#define UUENCODER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CUuencoder
{
public:
	CUuencoder();									// コンストラクタ
	virtual ~CUuencoder();							// デストラクタ

	CString BytesEncode(LPBYTE,UINT);

private:
	char*	m_pTrans;								// 変換テーブルポインタ
};

#endif // !defined(UUENCODER_H)


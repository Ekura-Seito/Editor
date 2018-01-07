// Uudecoder.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Uudecoder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUudecoder クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUudecoder::CUudecoder()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUudecoder::~CUudecoder()
{
}

/////////////////////////////////////////////////////////////////////////////
// CUudecoder パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列デコード														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CUudecoder::StringDecode
	(	CString*	pobjStrIn,
		LPBYTE		lpbyBufOut,
		long*		plLen
	)
{
	char*	p=NULL;
	int		n=0;
	long	lOut=0;

	(*plLen) = 0;
	p = pobjStrIn->GetBuffer(1024);

	/* N is used to avoid writing out all the characters at the end of the file.  */
	n = DEC(*p);
	if(n <= 0)
		return FALSE;
	for(++p; n > 0; p += 4, n -= 3)
	{
		char ch=0;

		if(n >= 3)
		{
			ch = DEC(p[0]) << 2 | DEC(p[1]) >> 4;
			lpbyBufOut[lOut] = ch;
			lOut++;
			ch = DEC(p[1]) << 4 | DEC(p[2]) >> 2;
			lpbyBufOut[lOut] = ch;
			lOut++;
			ch = DEC(p[2]) << 6 | DEC(p[3]);
			lpbyBufOut[lOut] = ch;
			lOut++;
		}
		else
		{
			if(n >= 1)
			{
				ch = DEC(p[0]) << 2 | DEC(p[1]) >> 4;
				lpbyBufOut[lOut] = ch;
				lOut++;
			}
			if(n >= 2)
			{
				ch = DEC(p[1]) << 4 | DEC(p[2]) >> 2;
				lpbyBufOut[lOut] = ch;
				lOut++;
			}
		}
	}

	(*plLen) = lOut;
	pobjStrIn->ReleaseBuffer();
	return TRUE;
}


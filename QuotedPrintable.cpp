// QuotedPrintable.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "QuotedPrintable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQuotedPrintable クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CQuotedPrintable::CQuotedPrintable()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CQuotedPrintable::~CQuotedPrintable()
{
}

/////////////////////////////////////////////////////////////////////////////
// CQuotedPrintable パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列のエンコード													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CQuotedPrintable::StringEncode
	(	CString*	pobjStrIn
	)
{
	char	szBuf[32];
	BYTE	byB1=0;
	int		nNeedLen=0;
	long	lCnt=0;
	CString	objStr=_T("");

	if(pobjStrIn == NULL)
		return -1;
	if(pobjStrIn->GetLength() == 0)
		return -1;

	for(;;)
	{
		if(lCnt >= pobjStrIn->GetLength())
			break;
		byB1 = pobjStrIn->GetAt(lCnt);
		lCnt++;

		memset(szBuf,0,sizeof(szBuf));
		if( (byB1 >= 0x21 && byB1 <= 0x7e) || byB1 == ' ' )
		{
			if(byB1 == '=')
			{
				//"=3D"にする
				nNeedLen = 3;
				strcpy(szBuf,"=3D");
			}
			else if(byB1 == '.') //メール終端印'.'とまぎらわしいので、"=XX"の形にする
			{
				nNeedLen = 3;
				sprintf(szBuf,"=%02X",byB1);
			}
			else
			{
				//そのまま
				nNeedLen = 1;
				szBuf[0] = byB1;
			}
		}
		else
		{
			//"=XX"の形にする
			nNeedLen = 3;
			sprintf(szBuf,"=%02X",byB1);
		}

		objStr += szBuf;
	}

	//変換後文字列を返す
	(*pobjStrIn) = objStr;
	return 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：バイト列のエンコード													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CString CQuotedPrintable::BytesEncode
	(	LPBYTE	lpbyBufIn,
		UINT	nLen
	)
{
	char	szBuf[32];
	BYTE	byB1=0;
	int		nNeedLen=0;
	long	lCnt=0;
	CString	objStr=_T("");

	if(lpbyBufIn == NULL)
		return _T("");
	if(nLen == 0)
		return _T("");

	for(;;)
	{
		if(lCnt >= (long)nLen)
			break;
		byB1 = lpbyBufIn[lCnt];
		lCnt++;

		memset(szBuf,0,sizeof(szBuf));
		if( (byB1 >= 0x21 && byB1 <= 0x7e) || byB1 == ' ' )
		{
			if(byB1 == '=')
			{
				//"=3D"にする
				nNeedLen = 3;
				strcpy(szBuf,"=3D");
			}
			else if(byB1 == '.') //メール終端印'.'とまぎらわしいので、"=XX"の形にする
			{
				nNeedLen = 3;
				sprintf(szBuf,"=%02X",byB1);
			}
			else
			{
				//そのまま
				nNeedLen = 1;
				szBuf[0] = byB1;
			}
		}
		else
		{
			//"=XX"の形にする
			nNeedLen = 3;
			sprintf(szBuf,"=%02X",byB1);
		}

		objStr += szBuf;
	}

	return objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列のデコード														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CQuotedPrintable::StringDecode
	(	CString*	pobjStrIn,
		LPBYTE		lpbyBufOut,
		long*		plLen
	)
{
	long	lOffset = 0, lOut=0;
	BYTE	byQ1 = 0;					//作成されたバイナリデータ
	int		nQmode = 0; 				//バイナリデータの区切りモード(0-3)
	BYTE	byTemp = 0; 				//Quoted-Printableデコード一時バッファ


	for(lOffset=0;;lOffset++)
	{
		if( lOffset >= pobjStrIn->GetLength() )
			break;

		byQ1 = pobjStrIn->GetAt(lOffset);
		if(DataCheck(byQ1,nQmode) == FALSE)
		{
			(*plLen) = lOut;
			return FALSE;
		}

		if(byQ1 == '=')
		{
			if( lOffset == (pobjStrIn->GetLength() - 1) ) //行末の’＝’はソフト改行をあらわし、
			{											 //改行にはしない。
				break;
			}
			else
			{
				nQmode = 1;
				byTemp = 0;
			}
		}
		else
		{
			if(nQmode == 0)
			{
				lpbyBufOut[lOut] = (BYTE)byQ1;
				lOut++;
			}
			else if(nQmode == 1)
			{
				if(byQ1 >= '0' && byQ1 <= '9')
					byTemp = ((((byQ1 - '0') & 0x0f) << 4) & 0xf0);
				else if(byQ1 >= 'A' && byQ1 <= 'Z')  //'A'を0x0A にする
					byTemp = ((((byQ1 - 0x37) & 0x0f) << 4) & 0xf0);
				else if(byQ1 >= 'a' && byQ1 <= 'z')  //'a'を0x0A にする（堅牢な実装）
					byTemp = ((((byQ1 - 0x57) & 0x0f) << 4) & 0xf0);
				nQmode = 2;
			}
			else if(nQmode == 2)
			{
				if(byQ1 >= '0' && byQ1 <= '9')
					byTemp |= ((byQ1 - '0') & 0x0f);
				else if(byQ1 >= 'A' && byQ1 <= 'Z') //'A'を0x0A にする
					byTemp |= ((byQ1 - 0x37) & 0x0f);
				else if(byQ1 >= 'a' && byQ1 <= 'z') //'a'を0x0A にする（堅牢な実装）
					byTemp |= ((byQ1 - 0x57) & 0x0f);
				lpbyBufOut[lOut] = (BYTE)byTemp;
				lOut++;
				byTemp = 0;
				nQmode = 0;
			}
		}
	}

	(*plLen) = lOut;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CQuotedPrintable プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：データチェック														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CQuotedPrintable::DataCheck
	(	BYTE	byData,
		int		nQmode
	)
{
	if(nQmode == 0)
	{
		if(byData >= 0x21 && byData <= 0x7e)
			return TRUE;
		else if(byData == ' ')
			return TRUE;
		else if(byData == '\t')
			return TRUE;
		else 
			return FALSE;
	}
	else //"=" に続くのは'A'～'Z'あるいは'0'～'9'のみ
	{
		if(byData >= '0' && byData <= '9')
			return TRUE;
		else if(byData >= 'A' && byData <= 'Z')
			return TRUE;
		else if(byData >= 'a' && byData <= 'z') //堅牢な実装
			return TRUE;
		else 
			return FALSE;
	}
}


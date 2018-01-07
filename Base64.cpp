// Base64.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Base64.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBase64 クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CBase64::CBase64()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CBase64::~CBase64()
{
}

/////////////////////////////////////////////////////////////////////////////
// CBase64 パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列のＢＡＳＥ６４エンコード										|*/
/*|	解説	：																  		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CBase64::StringEncode
	(	CString*	pobjStrIn
	)
{
	int		nBmode=0;		//バイナリデータの区切りモード(0-3)
	BYTE	byB1=0;			//符号化するバイナリデータ
	BYTE	byTemp=0;
	BYTE	byWork=0;
	CString	objStr=_T("");
	long	lCnt=0;

	if(pobjStrIn == NULL)
		return -1;
	if(pobjStrIn->GetLength() == 0)
		return -1;

	for(;;)
	{
		if(nBmode >= 0 && nBmode <= 2)
		{
			if(lCnt >= pobjStrIn->GetLength())
				break;
			byB1 = pobjStrIn->GetAt(lCnt);
			lCnt++;
		}

		switch(nBmode)
		{
		case 0:
			byTemp = (0x03 & byB1) << 4;				//下位２Ｂｉｔを保存
			byB1 = (0xfc & byB1) >> 2;					//上位６Ｂｉｔ
			byWork = 0;
			break;
		case 1:
			byWork = (0x0f & byB1) << 2;				//下位４Ｂｉｔを保存
			byB1 = (byTemp | ((0xf0 & byB1) >> 4));		//上位４Ｂｉｔ
			byTemp = byWork;
			byWork = 0;
			break;
		case 2:
			byWork = (0x3f & byB1);						//下位６Ｂｉｔを保存
			byB1 = (byTemp | ((0xc0 & byB1) >> 6));
			byTemp = byWork;
			byWork = 0;
			break;
		case 3:
			byB1 = byTemp;
			byTemp = 0;
			byWork = 0;
			break;
		default:
			break;
		}

		objStr += (TCHAR)BinToB64(byB1);

		nBmode ++;
		if(nBmode > 3)
			nBmode = 0;
	}

	//終端記号の付加
	switch(nBmode)
	{
	case 0:
		break;
	case 1:
		objStr += (TCHAR)BinToB64(byTemp);
		objStr += _T("==");
		break;
	case 2:
		objStr += (TCHAR)BinToB64(byTemp);
		objStr += _T("=");
		break;
	case 3:
		break;
	default:
		break;
	}

	//変換後文字列を返す
	(*pobjStrIn) = objStr;
	return 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：バイト列のＢＡＳＥ６４エンコード										|*/
/*|	解説	：																  		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CString CBase64::BytesEncode
	(	LPBYTE	lpbyBufIn,
		UINT	nLen
	)
{
	int		nBmode=0;		//バイナリデータの区切りモード(0-3)
	BYTE	byB1=0;			//符号化するバイナリデータ
	BYTE	byTemp=0;
	BYTE	byWork=0;
	CString	objStr=_T("");
	long	lCnt=0;

	if(lpbyBufIn == NULL)
		return _T("");
	if(nLen == 0)
		return _T("");

	for(;;)
	{
		if(nBmode >= 0 && nBmode <= 2)
		{
			if(lCnt >= (long)nLen)
				break;
			byB1 = lpbyBufIn[lCnt];
			lCnt++;
		}

		switch(nBmode)
		{
		case 0:
			byTemp = (0x03 & byB1) << 4;				//下位２Ｂｉｔを保存
			byB1 = (0xfc & byB1) >> 2;					//上位６Ｂｉｔ
			byWork = 0;
			break;
		case 1:
			byWork = (0x0f & byB1) << 2;				//下位４Ｂｉｔを保存
			byB1 = (byTemp | ((0xf0 & byB1) >> 4));		//上位４Ｂｉｔ
			byTemp = byWork;
			byWork = 0;
			break;
		case 2:
			byWork = (0x3f & byB1);						//下位６Ｂｉｔを保存
			byB1 = (byTemp | ((0xc0 & byB1) >> 6));
			byTemp = byWork;
			byWork = 0;
			break;
		case 3:
			byB1 = byTemp;
			byTemp = 0;
			byWork = 0;
			break;
		default:
			break;
		}

		objStr += (TCHAR)BinToB64(byB1);

		nBmode ++;
		if(nBmode > 3)
			nBmode = 0;
	}

	//終端記号の付加
	switch(nBmode)
	{
	case 0:
		break;
	case 1:
		objStr += (TCHAR)BinToB64( byTemp );
		objStr += _T("==");
		break;
	case 2:
		objStr += (TCHAR)BinToB64( byTemp );
		objStr += _T("=");
		break;
	case 3:
		break;
	default:
		break;
	}

	//変換後文字列を返す
	return objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列のＢＡＳＥ６４デコード											|*/
/*|	解説	：																  		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CBase64::StringDecode
	(	CString*	pobjStrIn,
		LPBYTE		lpbyBufOut,
		long*		plLen
	)
{
	long	lOffset = 0, lOut=0;
	BYTE	byB1 = 0;					//作成されたバイナリデータ
	int		nBmode = 0; 				//バイナリデータの区切りモード(0-3)
	BYTE	byTemp = 0; 				//Base64デコード一時バッファ


	for(lOffset=0;;lOffset++)
	{
		if( lOffset >= pobjStrIn->GetLength() )
			break;
		if( pobjStrIn->GetAt(lOffset) == '=' )
		{
			(*plLen) = lOut;
			return TRUE;
		}

		//ＡＳＣＩＩデータの数値化(0-63)
		byB1 = B64ToBin( pobjStrIn->GetAt(lOffset) );
		if( !(byB1 >= 0 && byB1 <= 63) )
		{
			(*plLen) = lOut;
			return FALSE;
		}

		//Base64バイナリ変換
		switch(nBmode)
		{
		case 0:
			byTemp = byB1 * 4;							//上位6Bit
			break;
		case 1:
			lpbyBufOut[lOut] = (BYTE)(byTemp | (byB1 / 16));//下位2Bbit
			lOut++;
			byTemp = ((0x0f & byB1) * 16);				//上位4Bit
			break;
		case 2:
			lpbyBufOut[lOut] = (BYTE)(byTemp | (byB1 / 4));//下位4Bit
			lOut++;
			byTemp = ((0x03 & byB1) * 64);				//上位2Bit
			break;
		case 3:
			lpbyBufOut[lOut] = (BYTE)(byTemp | byB1);	//上位6Bit
			lOut++;
			byTemp = 0;
			break;
		default:
			break;
		}
		nBmode++;
		if(nBmode > 3)
			nBmode = 0;
	}

	(*plLen) = lOut;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CBase64 プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＡＳＣＩＩ－＞ＢＩＮＡＲＹ											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BYTE CBase64::B64ToBin(BYTE byB64Ascii)
{
	BYTE	byB1;

	byB1 = 255;

	if(byB64Ascii >= 'A' && byB64Ascii <= 'Z')
		byB1 = byB64Ascii - 'A' + 0;
	if(byB64Ascii >= 'a' && byB64Ascii <= 'z')
		byB1 = byB64Ascii - 'a' + 26;
	if(byB64Ascii >= '0' && byB64Ascii <= '9')
		byB1 = byB64Ascii - '0' + 52;
	if(byB64Ascii == '+')
		byB1 = 62;
	if(byB64Ascii == '/')
		byB1 = 63;

	return byB1;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＢＩＮＡＲＹ－＞ＡＳＣＩＩ											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BYTE CBase64::BinToB64(BYTE byB64Binary)
{
	BYTE	byA1=0;

	if(byB64Binary <= 25)
		byA1 = 'A' + byB64Binary;
	if(byB64Binary > 25 && byB64Binary <= 51)
		byA1 = 'a' + byB64Binary - 26;
	if(byB64Binary > 51 && byB64Binary <= 61)
		byA1 = '0' + byB64Binary - 52;
	if(byB64Binary == 62)
		byA1 = '+';
	if(byB64Binary == 63)
		byA1 = '/';

	return byA1;
}


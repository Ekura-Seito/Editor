// CodeConvManager.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <mbstring.h>
#pragma warning(disable:4786)
#include "CodeConvManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCodeConvManager クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CCodeConvManager::CCodeConvManager()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CCodeConvManager::~CCodeConvManager()
{
}

/////////////////////////////////////////////////////////////////////////////
// CCodeConvManager パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コード判別															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CCodeConvManager::DetectCode
	(	const char*	szInFilePath			//入力ファイルパス
	)
											//関数リターン
											// 1 : Shift Jis
											// 2 : EUC
											// 3 : JIS
											// 4 : ASCII
											// 5 : 不明
											// 6 : Binary
{
	UCHAR	buf[1024];
	UINT	nBytesRead=0;
	UINT	nCheckResult=INPUT_UNKNOWN;
	BOOL	bIsEnd=FALSE;

	if(IsBinary(szInFilePath))
		return INPUT_BINARY;

	//ファイルオープン
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead, NULL ) )
		return INPUT_UNKNOWN;

	for(;;)
	{
		//１０２４バイト読込み
		memset(buf,0,sizeof(buf));
		nBytesRead = objInFile.Read( buf, sizeof(buf) );

		if (nBytesRead == 0) //読み込み終了
			break;

		bIsEnd = FALSE;
		nCheckResult = detectcode(buf, nBytesRead, &bIsEnd);
		if(bIsEnd)
		{
			if( nCheckResult==INPUT_ASCII || nCheckResult==INPUT_UNKNOWN )
				continue;
			else
				break;
		}
		else
			break;
	}

	objInFile.Close();
	return nCheckResult;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：改行コード判別														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CCodeConvManager::DetectKaigyoCode
	(	const char*	szInFilePath			//入力ファイルパス
	)
											//関数リターン
											// 0 : CR+LF
											// 1 : LF
											// 2 : CR
											// 3 : 不明
{
	BYTE	ch=0;
	UCHAR	buf[32];
	long	lCRCount=0;	//CRの数
	long	lLFCount=0;	//LFの数
	UINT	unRet=KAIGYO_UNKNOWN;

	//ファイルオープン
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead, NULL ) )
		return KAIGYO_UNKNOWN;
	CArchive ar( &objInFile, CArchive::load, 32, buf );

	for(;;)//改行候補出現まで読む
	{
		TRY
		{
			ar >> (BYTE&)ch;
		}
		CATCH_ALL(e) //EOF
		{
			return unRet;
		}
		END_CATCH_ALL

		if(!IsDBCSLeadByteEx(0,ch))//１バイト文字
		{
			if(ch=='\r')
			{
				lCRCount++;
				break;
			}
			else if(ch=='\n')
			{
				lLFCount++;
				break;
			}
		}
	}

	//さらに次を読み込み
	TRY
	{
		ar >> (BYTE&)ch;
	}
	CATCH_ALL(e) //EOF
	{
		if(lCRCount>0)
			unRet = KAIGYO_CR;
		else if(lLFCount>0)
			unRet = KAIGYO_LF;
		//クローズ
		ar.Close();
		objInFile.Close();
		return unRet;
	}
	END_CATCH_ALL

	if(IsDBCSLeadByteEx(0,ch)) //２バイト文字
	{
		if(lCRCount>0)
			unRet = KAIGYO_CR;
		else if(lLFCount>0)
			unRet = KAIGYO_LF;
	}
	else
	{
		if(ch=='\r')
		{
			if(lCRCount>0)
				unRet = KAIGYO_CR;
			else if(lLFCount>0)
				unRet = KAIGYO_UNKNOWN;
		}
		else if(ch=='\n')
		{
			if(lCRCount>0)
				unRet = KAIGYO_CRLF;
			else if(lLFCount>0)
				unRet = KAIGYO_LF;
		}
		else
		{
			if(lCRCount>0)
				unRet = KAIGYO_CR;
			else if(lLFCount>0)
				unRet = KAIGYO_LF;
		}
	}

	//クローズ
	ar.Close();
	objInFile.Close();

	return unRet;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＳＪＩＳ－＞ＥＵＣ変換												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::ConvertSjisToEuc
	(	const char*	szInFilePath,		//入力ファイルパス
		const char*	szOutFilePath		//出力ファイルパス
	)
										//関数リターン
										// TRUE : 正常終了
										// -1   : 入力ファイルオープンエラー
										// -2   : 出力ファイルオープンエラー
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	UINT	unConverted=0;

	//入力ファイルオープン
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead, NULL ) )
		return -1;
	CArchive	arIn(&objInFile,CArchive::load);

	//出力ファイルオープン
	CFile	objOutFile;
	if( !objOutFile.Open( szOutFilePath, (CFile::modeCreate|CFile::modeWrite), NULL ) )
	{
		arIn.Close();
		objInFile.Close();
		return -2;
	}
	CArchive	arOut(&objOutFile,CArchive::store);


	TRY
	{
		//まず１バイト読み込み
		arIn >> (BYTE&)by1stChar;

		for(;;) //１文字づつループ
		{
			if( by1stChar < 0x80 )
			{
				//そのまま出力
				arOut << (BYTE)by1stChar;
			}
			else if(iskanji1st(by1stChar)) //シフトＪＩＳ第一バイト
			{
				//次を読み込み
				arIn >> (BYTE&)by2ndChar;
				if(iskanji2nd(by2ndChar))
				{
					//SJIS -> EUC
					unConverted = sjis2euc( ((by1stChar<<8)|by2ndChar) );
					//変換結果出力
					arOut << (BYTE)(((unConverted>>8)&0x00ff));
					arOut << (BYTE)(unConverted&0x00ff);
				}
				else
				{
					//出力
					arOut << (BYTE)by1stChar;
					arOut << (BYTE)by2ndChar;
				}
			}
			else if(isHanKana(by1stChar)) //半角カナコード
			{
				//出力
				arOut << (BYTE)0x8e;
				arOut << (BYTE)by1stChar;
			}
			else
			{
				//出力
				arOut << (BYTE)by1stChar;
			}

			//クリア
			by1stChar=0;
			by2ndChar=0;
			//次を読み込み
			arIn >> (BYTE&)by1stChar;
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL

	//入出力ファイルクローズ
	arIn.Close();
	objInFile.Close();
	arOut.Close();
	objOutFile.Close();

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＳＪＩＳ－＞ＪＩＳ変換												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::ConvertSjisToJis
	(	const char*	szInFilePath,			//入力ファイルパス
		const char*	szOutFilePath			//出力ファイルパス
	)
										//関数リターン
										// TRUE : 正常終了
										// -1   : 入力ファイルオープンエラー
										// -2   : 出力ファイルオープンエラー
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	UINT	unConverted=0;
	char	state, kana;

	//入力ファイルオープン
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead, NULL ) )
		return -1;
	CArchive	arIn(&objInFile,CArchive::load);

	//出力ファイルオープン
	CFile	objOutFile;
	if( !objOutFile.Open( szOutFilePath, (CFile::modeCreate|CFile::modeWrite), NULL ) )
	{
		arIn.Close();
		objInFile.Close();
		return -2;
	}
	CArchive	arOut(&objOutFile,CArchive::store);


	TRY
	{
		//フラグクリア
		state = kana = 0;
		//まず１バイト読み込み
		arIn >> (BYTE&)by1stChar;
	
		for(;;) //１文字づつループ
		{
			if( by1stChar < 0x80 ) //ASCII
			{
				if (state)
				{
					set1byte(arOut);
					state =	0;
				}
				else if	(kana)
				{
					arOut << (BYTE)CODE_SI;
					kana = 0;
				}
				//そのまま出力
				arOut << (BYTE)by1stChar;
			}
			else if(iskanji1st(by1stChar)) //シフトＪＩＳ第一バイト
			{
				//次を読み込み
				arIn >> (BYTE&)by2ndChar;
				if(!state)
				{
					if(kana)
					{
						arOut << (BYTE)CODE_SI;
						kana = 0;
					}
					set2byte(arOut);
					state =	1;
				}
				//SJIS -> JIS
				unConverted = sjis2jis( ((by1stChar<<8)|by2ndChar) );
				//変換結果出力
				arOut << (BYTE)(((unConverted>>8)&0x00ff));
				arOut << (BYTE)(unConverted&0x00ff);
			}
			else if(isHanKana(by1stChar)) //半角カナコード
			{
				if (state)
				{
					set1byte(arOut);
					state =	0;
				}
				//出力
				arOut << (BYTE)by1stChar;
			}
			else
			{
				if (state)
				{
					set1byte(arOut);
					state =	0;
				}
				else if	(kana)
				{
					arOut << (BYTE)CODE_SI;
					kana = 0;
				}
				//出力
				arOut << (BYTE)by1stChar;
			}

			//クリア
			by1stChar=0;
			by2ndChar=0;
			//次を読み込み
			arIn >> (BYTE&)by1stChar;
		}
	}
	CATCH_ALL(e)
	{
		if(state)
		{
			set1byte(arOut);
			state =	0;
		}
	}
	END_CATCH_ALL

	//入出力ファイルクローズ
	arIn.Close();
	objInFile.Close();
	arOut.Close();
	objOutFile.Close();

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＥＵＣ－＞ＳＪＩＳ変換												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::ConvertEucToSjis
	(	const char*	szInFilePath,			//入力ファイルパス
		const char*	szOutFilePath			//出力ファイルパス
	)
										//関数リターン
										// TRUE : 正常終了
										// -1   : 入力ファイルオープンエラー
										// -2   : 出力ファイルオープンエラー
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	UINT	unConverted=0;

	//入力ファイルオープン
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead, NULL ) )
		return -1;
	CArchive	arIn(&objInFile,CArchive::load);

	//出力ファイルオープン
	CFile	objOutFile;
	if( !objOutFile.Open( szOutFilePath, (CFile::modeCreate|CFile::modeWrite), NULL ) )
	{
		arIn.Close();
		objInFile.Close();
		return -2;
	}
	CArchive	arOut(&objOutFile,CArchive::store);


	TRY
	{
		//まず１バイト読み込み
		arIn >> (BYTE&)by1stChar;
	
		for(;;) //１文字づつループ
		{
			if( by1stChar < 0x80 )
			{
				//そのまま出力
				arOut << (BYTE)by1stChar;
			}
			else if(iseuc(by1stChar)) //ＥＵＣ第一バイト
			{
				//次を読み込み
				arIn >> (BYTE&)by2ndChar;
				//EUC -> SJIS
				unConverted = euc2sjis( ((by1stChar<<8)|by2ndChar) );
				//変換結果出力
				arOut << (BYTE)(((unConverted>>8)&0x00ff));
				arOut << (BYTE)(unConverted&0x00ff);
			}
			else if(by1stChar==0x8e)
			{
				//次を読み込み
				arIn >> (BYTE&)by2ndChar;
				//出力
				arOut << (BYTE)by2ndChar;
			}
			else if(by1stChar==0x8f)
			{
				//次を読み込み
				arIn >> (BYTE&)by2ndChar;
				//出力
				arOut << (BYTE)by2ndChar;
				//次を読み込み
				arIn >> (BYTE&)by2ndChar;
				//出力
				arOut << (BYTE)by2ndChar;
			}
			else
			{
				//出力
				arOut << (BYTE)by1stChar;
			}

			//クリア
			by1stChar=0;
			by2ndChar=0;
			//次を読み込み
			arIn >> (BYTE&)by1stChar;
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL

	//入出力ファイルクローズ
	arIn.Close();
	objInFile.Close();
	arOut.Close();
	objOutFile.Close();

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＥＵＣ－＞ＪＩＳ変換													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::ConvertEucToJis
	(	const char*	szInFilePath,			//入力ファイルパス
		const char*	szOutFilePath			//出力ファイルパス
	)
										//関数リターン
										// TRUE : 正常終了
										// -1   : 入力ファイルオープンエラー
										// -2   : 出力ファイルオープンエラー
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	UINT	unConverted=0;
	char	state, kana;

	//入力ファイルオープン
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead, NULL ) )
		return -1;
	CArchive	arIn(&objInFile,CArchive::load);

	//出力ファイルオープン
	CFile	objOutFile;
	if( !objOutFile.Open( szOutFilePath, (CFile::modeCreate|CFile::modeWrite), NULL ) )
	{
		arIn.Close();
		objInFile.Close();
		return -2;
	}
	CArchive	arOut(&objOutFile,CArchive::store);


	TRY
	{
		//フラグクリア
		state = kana = 0;
		//まず１バイト読み込み
		arIn >> (BYTE&)by1stChar;
	
		for(;;) //１文字づつループ
		{
			if( by1stChar < 0x80 )
			{
				if (state)
				{
					set1byte(arOut);
					state =	0;
				}
				else if	(kana)
				{
					arOut << (BYTE)CODE_SI;
					kana = 0;
				}
				//そのまま出力
				arOut << (BYTE)by1stChar;
			}
			else if(iseuc(by1stChar)) //ＥＵＣ第一バイト
			{
				//次を読み込み
				arIn >> (BYTE&)by2ndChar;
				if(!state)
				{
					if(kana)
					{
						arOut << (BYTE)CODE_SI;
						kana = 0;
					}
					set2byte(arOut);
					state =	1;
				}
				//EUC -> JIS
				unConverted = euc2jis( ((by1stChar<<8)|by2ndChar) );
				//変換結果出力
				arOut << (BYTE)(((unConverted>>8)&0x00ff));
				arOut << (BYTE)(unConverted&0x00ff);
			}
			else if(by1stChar==0x8e)
			{
				//次を読み込み
				arIn >> (BYTE&)by2ndChar;
				if (state)
				{
					set1byte(arOut);
					state =	0;
				}
				//出力
				arOut << (BYTE)by2ndChar;
			}
			else if(by1stChar==0x8f)
			{
				if (!state)
				{
					if (kana)
					{
						arOut << (BYTE)CODE_SI;
						kana = 0;
					}
					set2byte(arOut);
					state =	1;
				}
				//次を読み込み
				arIn >> (BYTE&)by2ndChar;
				//出力
				arOut << (BYTE)by2ndChar;
				//次を読み込み
				arIn >> (BYTE&)by2ndChar;
				//出力
				arOut << (BYTE)by2ndChar;
			}
			else
			{
				if(state)
				{
					set1byte(arOut);
					state =	0;
				}
				else if	(kana)
				{
					arOut << (BYTE)CODE_SI;
					kana = 0;
				}
				//出力
				arOut << (BYTE)by1stChar;
			}

			//クリア
			by1stChar=0;
			by2ndChar=0;
			//次を読み込み
			arIn >> (BYTE&)by1stChar;
		}
	}
	CATCH_ALL(e)
	{
		if(state)
		{
			set1byte(arOut);
			state =	0;
		}
	}
	END_CATCH_ALL

	//入出力ファイルクローズ
	arIn.Close();
	objInFile.Close();
	arOut.Close();
	objOutFile.Close();

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＪＩＳ－＞ＳＪＩＳ変換												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::ConvertJisToSjis
	(	const char*	szInFilePath,			//入力ファイルパス
		const char*	szOutFilePath			//出力ファイルパス
	)
										//関数リターン
										// TRUE : 正常終了
										// -1   : 入力ファイルオープンエラー
										// -2   : 出力ファイルオープンエラー
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	BYTE	by3rdChar=0;
	UINT	unConverted=0;
	char	state, kana;

	//入力ファイルオープン
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead, NULL ) )
		return -1;
	CArchive	arIn(&objInFile,CArchive::load);

	//出力ファイルオープン
	CFile	objOutFile;
	if( !objOutFile.Open( szOutFilePath, (CFile::modeCreate|CFile::modeWrite), NULL ) )
	{
		arIn.Close();
		objInFile.Close();
		return -2;
	}
	CArchive	arOut(&objOutFile,CArchive::store);


	TRY
	{
		//フラグクリア
		state = kana = 0;
		//まず１バイト読み込み
		arIn >> (BYTE&)by1stChar;
	
		for(;;) //１文字づつループ
		{
			if( by1stChar == CODE_ESC ) //エスケープコード
			{
				//次を読み込み
				arIn >> (BYTE&)by2ndChar;
				switch(by2ndChar)
				{
				case '$' :
					//さらに次を読み込み
					arIn >> (BYTE&)by3rdChar;
					if(by3rdChar=='B' || by3rdChar=='@')
					{
						state = 1;
					}
					else 
					{
						arOut << (BYTE)CODE_ESC;
						arOut << (BYTE)by2ndChar;
						by1stChar = by3rdChar;
						continue;
					}
					break;
				case '(' :
					//さらに次を読み込み
					arIn >> (BYTE&)by3rdChar;
					if(by3rdChar=='J' || by3rdChar=='B' || by3rdChar=='H')
					{
						state = 0;
					}
					else
					{
						arOut << (BYTE)CODE_ESC;
						arOut << (BYTE)by2ndChar;
						by1stChar = by3rdChar;
						continue;
					}
					break;
				case 'K':
					state = 1;
					break;
				case 'H':
					state = 0;
					break;
				default:
					arOut << (BYTE)CODE_ESC;
					continue;
				}
			}
			else if(by1stChar<=0x20 || by1stChar==0x7f)
			{
				arOut << (BYTE)by1stChar;
			}
			else if(state)
			{
				//次を読み込み
				arIn >> (BYTE&)by2ndChar;
				if(by2ndChar<=0x20)
				{
					arOut << (BYTE)by1stChar;
					by1stChar = by2ndChar;
					continue;
				}
				if(by1stChar<0x80 && isjis(by2ndChar))
				{
					//JIS -> SJIS
					unConverted = jis2sjis( ((by1stChar<<8)|by2ndChar) );
					//変換結果出力
					arOut << (BYTE)(((unConverted>>8)&0x00ff));
					arOut << (BYTE)(unConverted&0x00ff);
				}
				else
				{
					arOut << (BYTE)by1stChar;
					arOut << (BYTE)by2ndChar;
				}
			}
			else
			{
				arOut << (BYTE)by1stChar;
			}

			//クリア
			by1stChar=0;
			by2ndChar=0;
			//次を読み込み
			arIn >> (BYTE&)by1stChar;
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL

	//入出力ファイルクローズ
	arIn.Close();
	objInFile.Close();
	arOut.Close();
	objOutFile.Close();

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＪＩＳ－＞ＥＵＣ変換													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::ConvertJisToEuc
	(	const char*	szInFilePath,			//入力ファイルパス
		const char*	szOutFilePath			//出力ファイルパス
	)
										//関数リターン
										// TRUE : 正常終了
										// -1   : 入力ファイルオープンエラー
										// -2   : 出力ファイルオープンエラー
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	BYTE	by3rdChar=0;
	UINT	unConverted=0;
	char	state, kana;

	//入力ファイルオープン
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead, NULL ) )
		return -1;
	CArchive	arIn(&objInFile,CArchive::load);

	//出力ファイルオープン
	CFile	objOutFile;
	if( !objOutFile.Open( szOutFilePath, (CFile::modeCreate|CFile::modeWrite), NULL ) )
	{
		arIn.Close();
		objInFile.Close();
		return -2;
	}
	CArchive	arOut(&objOutFile,CArchive::store);


	TRY
	{
		//フラグクリア
		state = kana = 0;
		//まず１バイト読み込み
		arIn >> (BYTE&)by1stChar;
	
		for(;;) //１文字づつループ
		{
			if( by1stChar == CODE_ESC ) //エスケープコード
			{
				//次を読み込み
				arIn >> (BYTE&)by2ndChar;
				switch(by2ndChar)
				{
				case '$' :
					//さらに次を読み込み
					arIn >> (BYTE&)by3rdChar;
					if(by3rdChar=='B' || by3rdChar=='@')
					{
						state = 1;
					}
					else 
					{
						arOut << (BYTE)CODE_ESC;
						arOut << (BYTE)by2ndChar;
						by1stChar = by3rdChar;
						continue;
					}
					break;
				case '(' :
					//さらに次を読み込み
					arIn >> (BYTE&)by3rdChar;
					if(by3rdChar=='J' || by3rdChar=='B' || by3rdChar=='H')
					{
						state = 0;
					}
					else
					{
						arOut << (BYTE)CODE_ESC;
						arOut << (BYTE)by2ndChar;
						by1stChar = by3rdChar;
						continue;
					}
					break;
				case 'K':
					state = 1;
					break;
				case 'H':
					state = 0;
					break;
				default:
					arOut << (BYTE)CODE_ESC;
					continue;
				}
			}
			else if(by1stChar<=0x20 || by1stChar==0x7f)
			{
				arOut << (BYTE)by1stChar;
			}
			else if(state)
			{
				//次を読み込み
				arIn >> (BYTE&)by2ndChar;
				if(by2ndChar<=0x20 || by2ndChar==0x7f)
				{
					arOut << (BYTE)by1stChar;
					by1stChar = by2ndChar;
					continue;
				}
				//JIS -> EUC
				unConverted = jis2euc( ((by1stChar<<8)|by2ndChar) );
				//変換結果出力
				arOut << (BYTE)(((unConverted>>8)&0x00ff));
				arOut << (BYTE)(unConverted&0x00ff);
			}
			else
			{
				if(isHanKana(by1stChar)) //半角カナコード
				{
					arOut << (BYTE)0x8e;
				}
				arOut << (BYTE)by1stChar;
			}

			//クリア
			by1stChar=0;
			by2ndChar=0;
			//次を読み込み
			arIn >> (BYTE&)by1stChar;
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL

	//入出力ファイルクローズ
	arIn.Close();
	objInFile.Close();
	arOut.Close();
	objOutFile.Close();

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列中のカタカナ 半角－＞全角変換									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::StringKatakanaHan2Zen
	(	CString*	pobjLineIn
	)
{
	long lCnt=0;
	BYTE byBuf=0, byBuf2=0;
	CString objLine;
	UINT nZenKana=0;

	if(pobjLineIn == NULL)
		return -1;
	if(pobjLineIn->GetLength() == 0)
		return -1;

	for(lCnt=0; lCnt<pobjLineIn->GetLength(); )
	{
		byBuf = pobjLineIn->GetAt(lCnt);

		if(IsDBCSLeadByteEx(0,byBuf))//２バイト文字の第一バイト
		{
			objLine += (UCHAR)byBuf;
			lCnt++;
			byBuf = pobjLineIn->GetAt(lCnt);
			objLine += (UCHAR)byBuf;
			lCnt++;
			continue;
		}
		else //半角
		{
			if( isHanKana(byBuf) ) //半角カタカナ
			{
				//下へ
			}
			else
			{
				objLine += (UCHAR)byBuf;
				lCnt++;
				continue;
			}
		}

		byBuf2 = pobjLineIn->GetAt(lCnt+1);

		if( !IsDBCSLeadByteEx(0,byBuf2) && byBuf2 == 0xde ) // 次が ﾞ
		{
			//全角カタカナ変換＆挿入
			nZenKana = _mbbtombc(byBuf);
			nZenKana = AddDakuonToKana(nZenKana);
			objLine += (UCHAR)((nZenKana>>8)&0x00ff);
			objLine += (UCHAR)(nZenKana&0x00ff);
			lCnt+=2;
		}
		else if( !IsDBCSLeadByteEx(0,byBuf2) && byBuf2 == 0xdf ) // 次が ﾟ
		{
			//全角カタカナ変換＆挿入
			nZenKana = _mbbtombc(byBuf);
			nZenKana = AddMaruToKana(nZenKana);
			objLine += (UCHAR)(((nZenKana>>8)&0x00ff));
			objLine += (UCHAR)(nZenKana&0x00ff);
			lCnt+=2;
		}
		else
		{
			//全角カタカナ変換＆挿入
			nZenKana = _mbbtombc(byBuf);
			objLine += (UCHAR)(((nZenKana>>8)&0x00ff));
			objLine += (UCHAR)(nZenKana&0x00ff);
			lCnt++;
		}
	}

	//変換後文字列を返す
	(*pobjLineIn) = objLine;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列のSJIS->EUCコード変換											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::StringSjisToEuc
	(	CString*	pobjLineIn
	)
{
	long	lCnt=0;
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	UINT	unConverted=0;
	CString	objLine;

	if(pobjLineIn == NULL)
		return -1;
	if(pobjLineIn->GetLength() == 0)
		return -1;

	//まず１バイト読み込み
	lCnt=0;
	by1stChar = pobjLineIn->GetAt(lCnt);
	lCnt++;

	for(;;) //１文字づつループ
	{
		if( by1stChar < 0x80 )
		{
			//そのまま出力
			objLine += (UCHAR)by1stChar;
		}
		else if(iskanji1st(by1stChar)) //シフトＪＩＳ第一バイト
		{
			//次を読み込み
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			if(iskanji2nd(by2ndChar))
			{
				//SJIS -> EUC
				unConverted = sjis2euc( ((by1stChar<<8)|by2ndChar) );
				//変換結果出力
				objLine += (UCHAR)(((unConverted>>8)&0x00ff));
				objLine += (UCHAR)(unConverted&0x00ff);
			}
			else
			{
				//出力
				objLine += (UCHAR)by1stChar;
				objLine += (UCHAR)by2ndChar;
			}
		}
		else if(isHanKana(by1stChar)) //半角カナコード
		{
			//出力
			objLine += (UCHAR)0x8e;
			objLine += (UCHAR)by1stChar;
		}
		else
		{
			//出力
			objLine += (UCHAR)by1stChar;
		}

		//クリア
		by1stChar=0;
		by2ndChar=0;
		//次を読み込み
		if(lCnt >= pobjLineIn->GetLength())
			break;
		by1stChar = pobjLineIn->GetAt(lCnt);
		lCnt++;
	}

	//変換後文字列を返す
	(*pobjLineIn) = objLine;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列のSJIS->JISコード変換											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::StringSjisToJis
	(	CString*	pobjLineIn			//入力文字列
	)
{
	long	lCnt=0;
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	UINT	unConverted=0;
	char	state, kana;
	CString	objLine;

	if(pobjLineIn == NULL)
		return -1;
	if(pobjLineIn->GetLength() == 0)
		return -1;

	//フラグクリア
	state = kana = 0;
	//まず１バイト読み込み
	lCnt=0;
	by1stChar = pobjLineIn->GetAt(lCnt);
	lCnt++;

	for(;;) //１文字づつループ
	{
		if( by1stChar < 0x80 ) //ASCII
		{
			if (state)
			{
				set1byte(&objLine);
				state =	0;
			}
			else if	(kana)
			{
				objLine += (UCHAR)CODE_SI;
				kana = 0;
			}
			//そのまま出力
			objLine += (UCHAR)by1stChar;
		}
		else if(iskanji1st(by1stChar)) //シフトＪＩＳ第一バイト
		{
			//次を読み込み
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			if(!state)
			{
				if(kana)
				{
					objLine += (UCHAR)CODE_SI;
					kana = 0;
				}
				set2byte(&objLine);
				state =	1;
			}
			//SJIS -> JIS
			unConverted = sjis2jis( ((by1stChar<<8)|by2ndChar) );
			//変換結果出力
			objLine += (UCHAR)(((unConverted>>8)&0x00ff));
			objLine += (UCHAR)(unConverted&0x00ff);
		}
		else if(isHanKana(by1stChar)) //半角カナコード
		{
			if (state)
			{
				set1byte(&objLine);
				state =	0;
			}
			//出力
			objLine += (UCHAR)by1stChar;
		}
		else
		{
			if (state)
			{
				set1byte(&objLine);
				state =	0;
			}
			else if	(kana)
			{
				objLine += (UCHAR)CODE_SI;
				kana = 0;
			}
			//出力
			objLine += (UCHAR)by1stChar;
		}

		//クリア
		by1stChar=0;
		by2ndChar=0;
		//次を読み込み
		if(lCnt >= pobjLineIn->GetLength())
			break;
		by1stChar = pobjLineIn->GetAt(lCnt);
		lCnt++;
	}
	if(state)
	{
		set1byte(&objLine);
		state =	0;
	}

	//変換後文字列を返す
	(*pobjLineIn) = objLine;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列のEUC->SJISコード変換											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::StringEucToSjis
	(	CString*	pobjLineIn
	)
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	UINT	unConverted=0;
	long	lCnt=0;
	CString	objLine;

	if(pobjLineIn == NULL)
		return -1;
	if(pobjLineIn->GetLength() == 0)
		return -1;

	//まず１バイト読み込み
	lCnt = 0;
	by1stChar = pobjLineIn->GetAt(lCnt);
	lCnt++;

	for(;;) //１文字づつループ
	{
		if( by1stChar < 0x80 )
		{
			//そのまま出力
			objLine += (UCHAR)by1stChar;
		}
		else if(iseuc(by1stChar)) //ＥＵＣ第一バイト
		{
			//次を読み込み
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			//EUC -> SJIS
			unConverted = euc2sjis( ((by1stChar<<8)|by2ndChar) );
			//変換結果出力
			objLine += (UCHAR)(((unConverted>>8)&0x00ff));
			objLine += (UCHAR)(unConverted&0x00ff);
		}
		else if(by1stChar==0x8e)
		{
			//次を読み込み
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			//出力
			objLine += (UCHAR)by2ndChar;
		}
		else if(by1stChar==0x8f)
		{
			//次を読み込み
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			//出力
			objLine += (UCHAR)by2ndChar;
			//次を読み込み
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			//出力
			objLine += (UCHAR)by2ndChar;
		}
		else
		{
			//出力
			objLine += (UCHAR)by1stChar;
		}

		//クリア
		by1stChar=0;
		by2ndChar=0;
		//次を読み込み
		if(lCnt >= pobjLineIn->GetLength())
			break;
		by1stChar = pobjLineIn->GetAt(lCnt);
		lCnt++;
	}

	//変換後文字列を返す
	(*pobjLineIn) = objLine;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列のEUC->JISコード変換											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::StringEucToJis
	(	CString*	pobjLineIn
	)
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	UINT	unConverted=0;
	char	state, kana;
	long	lCnt=0;
	CString	objLine;

	if(pobjLineIn == NULL)
		return -1;
	if(pobjLineIn->GetLength() == 0)
		return -1;

	//フラグクリア
	state = kana = 0;
	//まず１バイト読み込み
	lCnt = 0;
	by1stChar = pobjLineIn->GetAt(lCnt);
	lCnt++;

	for(;;) //１文字づつループ
	{
		if( by1stChar < 0x80 )
		{
			if (state)
			{
				set1byte(&objLine);
				state =	0;
			}
			else if	(kana)
			{
				objLine += (UCHAR)CODE_SI;
				kana = 0;
			}
			//そのまま出力
			objLine += (UCHAR)by1stChar;
		}
		else if(iseuc(by1stChar)) //ＥＵＣ第一バイト
		{
			//次を読み込み
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			if(!state)
			{
				if(kana)
				{
					objLine += (UCHAR)CODE_SI;
					kana = 0;
				}
				set2byte(&objLine);
				state =	1;
			}
			//EUC -> JIS
			unConverted = euc2jis( ((by1stChar<<8)|by2ndChar) );
			//変換結果出力
			objLine += (UCHAR)(((unConverted>>8)&0x00ff));
			objLine += (UCHAR)(unConverted&0x00ff);
		}
		else if(by1stChar==0x8e)
		{
			//次を読み込み
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			if (state)
			{
				set1byte(&objLine);
				state =	0;
			}
			//出力
			objLine += (UCHAR)by2ndChar;
		}
		else if(by1stChar==0x8f)
		{
			if (!state)
			{
				if (kana)
				{
					objLine += (UCHAR)CODE_SI;
					kana = 0;
				}
				set2byte(&objLine);
				state =	1;
			}
			//次を読み込み
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			//出力
			objLine += (UCHAR)by2ndChar;
			//次を読み込み
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			//出力
			objLine += (UCHAR)by2ndChar;
		}
		else
		{
			if(state)
			{
				set1byte(&objLine);
				state =	0;
			}
			else if	(kana)
			{
				objLine += (UCHAR)CODE_SI;
				kana = 0;
			}
			//出力
			objLine += (UCHAR)by1stChar;
		}

		//クリア
		by1stChar=0;
		by2ndChar=0;
			//次を読み込み
		if(lCnt >= pobjLineIn->GetLength())
			break;
		by1stChar = pobjLineIn->GetAt(lCnt);
		lCnt++;
	}
	if(state)
	{
		set1byte(&objLine);
		state =	0;
	}

	//変換後文字列を返す
	(*pobjLineIn) = objLine;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列のJIS->SJISコード変換											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::StringJisToSjis
	(	CString*	pobjLineIn
	)
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	BYTE	by3rdChar=0;
	UINT	unConverted=0;
	char	state, kana;
	long	lCnt=0;
	CString	objLine;

	if(pobjLineIn == NULL)
		return -1;
	if(pobjLineIn->GetLength() == 0)
		return -1;


	//フラグクリア
	state = kana = 0;
		//まず１バイト読み込み
	lCnt=0;
	by1stChar = pobjLineIn->GetAt(lCnt);
	lCnt++;

	for(;;) //１文字づつループ
	{
		if( by1stChar == CODE_ESC ) //エスケープコード
		{
			//次を読み込み
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			switch(by2ndChar)
			{
			case '$' :
				//さらに次を読み込み
				by3rdChar = pobjLineIn->GetAt(lCnt);
				lCnt++;
				if(by3rdChar=='B' || by3rdChar=='@')
				{
					state = 1;
				}
				else 
				{
					objLine += (UCHAR)CODE_ESC;
					objLine += (UCHAR)by2ndChar;
					by1stChar = by3rdChar;
					continue;
				}
				break;
			case '(' :
				//さらに次を読み込み
				by3rdChar = pobjLineIn->GetAt(lCnt);
				lCnt++;
				if(by3rdChar=='J' || by3rdChar=='B' || by3rdChar=='H')
				{
					state = 0;
				}
				else
				{
					objLine += (UCHAR)CODE_ESC;
					objLine += (UCHAR)by2ndChar;
					by1stChar = by3rdChar;
					continue;
				}
				break;
			case 'K':
				state = 1;
				break;
			case 'H':
				state = 0;
				break;
			default:
				objLine += (UCHAR)CODE_ESC;
				continue;
			}
		}
		else if(by1stChar<=0x20 || by1stChar==0x7f)
		{
			objLine += (UCHAR)by1stChar;
		}
		else if(state)
		{
			//次を読み込み
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			if(by2ndChar<=0x20)
			{
				objLine += (UCHAR)by1stChar;
				by1stChar = by2ndChar;
				continue;
			}
			if(by1stChar<0x80 && isjis(by2ndChar))
			{
				//JIS -> SJIS
				unConverted = jis2sjis( ((by1stChar<<8)|by2ndChar) );
				//変換結果出力
				objLine += (UCHAR)(((unConverted>>8)&0x00ff));
				objLine += (UCHAR)(unConverted&0x00ff);
			}
			else
			{
				objLine += (UCHAR)by1stChar;
				objLine += (UCHAR)by2ndChar;
			}
		}
		else
		{
			objLine += (UCHAR)by1stChar;
		}

		//クリア
		by1stChar=0;
		by2ndChar=0;
		//次を読み込み
		if(lCnt >= pobjLineIn->GetLength())
			break;
		by1stChar = pobjLineIn->GetAt(lCnt);
		lCnt++;
	}

	//変換後文字列を返す
	(*pobjLineIn) = objLine;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列のJIS->EUCコード変換											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::StringJisToEuc
	(	CString*	pobjLineIn
	)
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	BYTE	by3rdChar=0;
	UINT	unConverted=0;
	char	state, kana;
	long	lCnt=0;
	CString	objLine;

	if(pobjLineIn == NULL)
		return -1;
	if(pobjLineIn->GetLength() == 0)
		return -1;


	//フラグクリア
	state = kana = 0;
	//まず１バイト読み込み
	lCnt=0;
	by1stChar = pobjLineIn->GetAt(lCnt);
	lCnt++;

	for(;;) //１文字づつループ
	{
		if( by1stChar == CODE_ESC ) //エスケープコード
		{
			//次を読み込み
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			switch(by2ndChar)
			{
			case '$' :
				//さらに次を読み込み
				by3rdChar = pobjLineIn->GetAt(lCnt);
				lCnt++;
				if(by3rdChar=='B' || by3rdChar=='@')
				{
					state = 1;
				}
				else 
				{
					objLine += (UCHAR)CODE_ESC;
					objLine += (UCHAR)by2ndChar;
					by1stChar = by3rdChar;
					continue;
				}
				break;
			case '(' :
				//さらに次を読み込み
				by3rdChar = pobjLineIn->GetAt(lCnt);
				lCnt++;
				if(by3rdChar=='J' || by3rdChar=='B' || by3rdChar=='H')
				{
					state = 0;
				}
				else
				{
					objLine += (UCHAR)CODE_ESC;
					objLine += (UCHAR)by2ndChar;
					by1stChar = by3rdChar;
					continue;
				}
				break;
			case 'K':
				state = 1;
				break;
			case 'H':
				state = 0;
				break;
			default:
				objLine += (UCHAR)CODE_ESC;
				continue;
			}
		}
		else if(by1stChar<=0x20 || by1stChar==0x7f)
		{
			objLine += (UCHAR)by1stChar;
		}
		else if(state)
		{
			//次を読み込み
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			if(by2ndChar<=0x20 || by2ndChar==0x7f)
			{
				objLine += (UCHAR)by1stChar;
				by1stChar = by2ndChar;
				continue;
			}
			//JIS -> EUC
			unConverted = jis2euc( ((by1stChar<<8)|by2ndChar) );
			//変換結果出力
			objLine += (UCHAR)(((unConverted>>8)&0x00ff));
			objLine += (UCHAR)(unConverted&0x00ff);
		}
		else
		{
			if(isHanKana(by1stChar)) //半角カナコード
			{
				objLine += (UCHAR)0x8e;
			}
			objLine += (UCHAR)by1stChar;
		}

		//クリア
		by1stChar=0;
		by2ndChar=0;
		//次を読み込み
		if(lCnt >= pobjLineIn->GetLength())
			break;
		by1stChar = pobjLineIn->GetAt(lCnt);
		lCnt++;
	}

	//変換後文字列を返す
	(*pobjLineIn) = objLine;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：パ、プ、ポ　などの丸付きカタカナの判定								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CCodeConvManager::IsMaruKana(unsigned int kana)
{
	if(kana==0x8370) 		return TRUE;	// パ
	else if(kana==0x8373) 	return TRUE;	// ピ
	else if(kana==0x8376) 	return TRUE;	// プ
	else if(kana==0x8379) 	return TRUE;	// ペ
	else if(kana==0x837C) 	return TRUE;	// ポ
	else					return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：バ、ガ、ザ　などの濁音付きカタカナの判定								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CCodeConvManager::IsDakuonKana(unsigned int kana)
{
	if(kana==0x8394) 		return TRUE;	// ヴ
	else if(kana==0x834B) 	return TRUE;	// ガ
	else if(kana==0x834D) 	return TRUE;	// ギ
	else if(kana==0x834F) 	return TRUE;	// グ
	else if(kana==0x8351) 	return TRUE;	// ゲ
	else if(kana==0x8353) 	return TRUE;	// ゴ
	else if(kana==0x8355) 	return TRUE;	// ザ
	else if(kana==0x8357) 	return TRUE;	// ジ
	else if(kana==0x8359) 	return TRUE;	// ズ
	else if(kana==0x835B) 	return TRUE;	// ゼ
	else if(kana==0x835D) 	return TRUE;	// ゾ
	else if(kana==0x835F) 	return TRUE;	// ダ
	else if(kana==0x8361) 	return TRUE;	// ヂ
	else if(kana==0x8364) 	return TRUE;	// ヅ
	else if(kana==0x8366) 	return TRUE;	// デ
	else if(kana==0x8368) 	return TRUE;	// ド
	else if(kana==0x836F) 	return TRUE;	// バ
	else if(kana==0x8372) 	return TRUE;	// ビ
	else if(kana==0x8375) 	return TRUE;	// ブ
	else if(kana==0x8378) 	return TRUE;	// ベ
	else if(kana==0x837B) 	return TRUE;	// ボ
	else 					return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：全角カタカナに丸を追加												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::AddMaruToKana(unsigned int kana)
{
	unsigned int unBack = kana;

	//ハ＝836E、パ＝8370などのように２を加えると丸がつく
	kana += 2;

	if(IsMaruKana(kana))
		return kana;
	else
		return unBack;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：全角カタカナに濁音を追加												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::AddDakuonToKana(unsigned int kana)
{
	unsigned int unBack = kana;

	if(kana == 0x8345)
	{
		kana = 0x8394;	//ウ＝0x8345 ----> ヴ＝0x8394
	}
	else
	{
		//カ＝0x834A、ガ＝0x834Bなどのように１を加えると濁音がつく
		kana += 1;
	}

	if(IsDakuonKana(kana))
		return kana;
	else
		return unBack;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：英数字　半角－＞全角													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::AlnumToZen
	(	unsigned int unAlnum
	)
{
	if(unAlnum>='0' && unAlnum<='9')
		return (unAlnum - '0' + 0x824F);
	else if(unAlnum>='a' && unAlnum<='z')
		return (unAlnum - 'a' + 0x8281);
	else if(unAlnum>='A' && unAlnum<='Z')
		return (unAlnum - 'A' + 0x8260);
	else
		return unAlnum;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：英数字　全角－＞半角													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::AlnumToHan
	(	unsigned int unAlnum
	)
{
	if(unAlnum>=0x824F && unAlnum<=0x8258) //０～９
		return (unAlnum - 0x824F + 0x30);
	else if(unAlnum>=0x8281 && unAlnum<=0x829A) //ａ～ｚ
		return (unAlnum - 0x8281 + 'a');
	else if(unAlnum>=0x8260 && unAlnum<=0x8279) //Ａ～Ｚ
		return (unAlnum - 0x8260 + 'A');
	else
		return unAlnum;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：全角英数字判定														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CCodeConvManager::IsZenkakuAlnum
	(	unsigned int unAlnum
	)
{
	if(unAlnum>=0x824F && unAlnum<=0x8258) //０～９
		return TRUE;
	else if(unAlnum>=0x8281 && unAlnum<=0x829A) //ａ～ｚ
		return TRUE;
	else if(unAlnum>=0x8260 && unAlnum<=0x8279) //Ａ～Ｚ
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CCodeConvManager プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：バイナリファイルチェック												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CCodeConvManager::IsBinary
	(	const char*	szInFilePath			//入力ファイルパス
	)
{
	UCHAR	buf[1024];
	UINT	nBytesRead;
	UINT	nCnt, nBinaryCnt;
	char binarytbl[32] = 
	{
		1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  0,  0,  0,  0,
	 //NUL,SOH,STX,ETX,EOT,ENQ,ACK,BEL,BS, HT, LF, VT, FF, CR, SO, SI,
		1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,  1
	 //DLE,DC1,DC2,DC3,DC4,NAC,SYN,ETB,CAN,EM, SUB,ESC,FS, GS, RS, US,
	};

	//入力ファイルから最大１０２４バイト読む
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead|CFile::typeBinary, NULL ) )
		return( FALSE );
	memset(buf,0,sizeof(buf));
	nBytesRead = objInFile.Read( buf, 1024 );

	//バイナリデータ数カウント
	nBinaryCnt = 0;
	for(nCnt=0; nCnt<nBytesRead; nCnt++)
	{
		if((buf[nCnt]<0x20)	&& binarytbl[buf[nCnt]])
			nBinaryCnt++;
	}

	objInFile.Close();

	return ((nBinaryCnt<<5) > nBytesRead);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コード判別															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CCodeConvManager::whatKanji(UCHAR* str)
									//関数リターン
									//・引数strの先頭文字が半角カナの場合、関数値は1
									//・引数strの先頭2文字がシフトJISコードの場合、関数値は2
									//・引数strの先頭2文字がEUC半角カナコードの場合、関数値は4
									//・引数strの先頭2文字がEUC全角コードの場合、関数値は8
									//・引数strの先頭3文字が新JIS(X0208-1983)シフトコードの場合、
									//　関数値は16
									//・引数strの先頭3文字が旧JIS(X0208-1978)シフトコードの場合、
									//　関数値は32
									//・引数strの先頭3文字がJISローマ字(X0201)シフトコードの場合、
									//　関数値は64
									//・引数strの先頭3文字がASCII シフトコードの場合、関数値は128
									//・上のケースが複数個同時に起きる場合は、関数値はそれらの論理和
									//・上のいずれでもない場合は、関数値は0
{
//・半角カナコード
//		0xa0 - 0xdf の間を使う。
//
//・JISコード
//		7ビット系漢字コードであるため、つぎのようなシフトコードを使って、
//		文字セットの切り替えを行い、かな漢字を表現する。
//
//		新JIS X0208-1983	：		ESC $ B
//		旧JIS X0208-1978 	：		ESC $ @
//		JISローマ字 X0201 	：      ESC ( J
//		ASCII 				：      ESC ( B
//
//・Shift JISコード
//		第1バイトとして 0x81 - 0x9f までと 0xe0 - 0xfc まで、
//		第2バイトとして 0x40 - 0xfc まで（0x7fを除く）を使う。
//		この ため、シフトJISは上記の半角コードと混在できる。
//		しかし、制御文字 として使われる ＼（バックスラッシュまたは円記号）
//		などが第2バイト の領域に入っているために問題が生じることがある。
//
//・EUCコード
//		第1、第2バイトとも 0xa1 - 0xfe までであるが、
//		半角の カナ文字については、0x8e を半角カナ文字の前につけ、
//		2バイトで表現 する。
//

	UINT val = 0;
	unsigned char b1, b2, b3;

	b1 = *str++;
	b2 = *str++;
	b3 = *str;
	if(b1 == 0x1b) 								//JISコード
	{
		if(b2 == '$' && b3 == 'B') return 16;
		if(b2 == '$' && b3 == '@') return 32;
		if(b2 == '(' && b3 == 'J') return 64;
		if(b2 == '(' && b3 == 'B') return 128;
		return 0;
	}

	if( b1 >= 0xa0 && b1 <= 0xdf) 				//半角コードの可能性
		val |= 1;
	if( ( ((b1 >= 0x81) && (b1 <= 0x9f)) ||		//Shift Jis の可能性
		  ((b1 >= 0xe0) && (b1 <= 0xfc)) 
		)									&& 
		( (b2 >= 0x40) && 
		  (b2 <= 0xfc) && 
		  (b2 != 0x7f) 
		)
	)
	{
		val |= 2;
	}
	if( (b1 == 0x8e) && (b2 >= 0xa0 && b2 <= 0xdf))	//半角EUC の可能性
		val |= 4;
	if( (b1 >= 0xa1 && b1 <= 0xfe) &&				//全角EUC の可能性
		(b2 >= 0xa1 && b2 <= 0xfe)
	)
	{
		val |= 8;
	}

	return val;				//１になることがある	－＞　半角コードだけでは判定不可
							//３になることはない
							//５になることはない
							//９になることがある	－＞　EUCかもしれない
							//６になることがある	－＞　SJIS EUC混在のため判定不可
							//１０になることがある	－＞　SJIS EUC混在のため判定不可
							//１２なることはない
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＳＪＩＳ－＞ＥＵＣ													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::sjis2euc(unsigned int sjis)
{
	unsigned int hib, lob;

	hib	= (sjis	>> 8) &	0xff;
	lob	= sjis & 0xff;

	hib	-= (hib	<= 0x9f) ? 0x70	: 0xb0;
	hib	<<=	1;
	if (lob	< 0x9f)	{
		lob	-= (lob	< 0x7f)	? 0x1f : 0x20;
		hib--;
	}
	else
		lob	-= 0x7e;

	hib	|= 0x80;
	lob	|= 0x80;
	return (hib	<< 8) |	lob;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＳＪＩＳ－＞ＪＩＳ													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::sjis2jis(unsigned int sjis)
{
	unsigned int hib, lob;

	hib	= (sjis	>> 8) &	0xff;
	lob	= sjis & 0xff;

	hib	-= (hib	<= 0x9f) ? 0x70	: 0xb0;
	hib	<<=	1;
	if (lob	< 0x9f)	{
		lob	-= (lob	< 0x7f)	? 0x1f : 0x20;
		hib--;
	}
	else
		lob	-= 0x7e;

	return (hib	<< 8) |	lob;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＥＵＣ－＞ＳＪＩＳ													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::euc2sjis(unsigned int euc)
{
	unsigned int jis;
	unsigned int hib, lob;

	if ((euc & 0xff00) == 0x8e00)
		jis = KanaHan2Zen(euc & 0xff);
	else
		jis = euc & ~0x8080;

	hib = (jis >> 8) & 0xff;
	lob = jis & 0xff;
	lob += (hib & 1) ? 0x1f : 0x7d;
	if(lob >= 0x7f)
		lob++;
	hib = ((hib - 0x21) >> 1) + 0x81;
	if(hib > 0x9f)
		hib += 0x40;

	return (hib << 8) | lob;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＥＵＣ－＞ＪＩＳ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::euc2jis(unsigned int euc)
{
	unsigned int jis;

	if((euc & 0xff00) == 0x8e00)
		jis = KanaHan2Zen(euc & 0xff);
	else
		jis = euc & ~0x8080;

	return jis;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＪＩＳ－＞ＳＪＩＳ													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::jis2sjis(unsigned int jis)
{
	unsigned int hib, lob;

	hib = (jis >> 8) & 0xff;
	lob = jis & 0xff;
	lob += (hib & 1) ? 0x1f : 0x7d;
	if(lob >= 0x7f) 
		lob++;
	hib = ((hib - 0x21) >> 1) + 0x81;
	if(hib > 0x9f)
		hib += 0x40;

	return (hib << 8) | lob;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＪＩＳ－＞ＥＵＣ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::jis2euc(unsigned int jis)
{
	return jis | 0x8080;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：半角カナを全角カナに変換する											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::KanaHan2Zen(unsigned int hankaku)
{
	static unsigned int z[64] = 
	{
		0x2121,0x2123,0x2156,0x2157,0x2122,0x2126,0x2572,0x2521,
		0x2523,0x2525,0x2527,0x2529,0x2563,0x2565,0x2567,0x2543,
		0x213c,0x2522,0x2524,0x2526,0x2528,0x252a,0x252b,0x252d,
		0x252f,0x2531,0x2533,0x2535,0x2537,0x2539,0x253b,0x253d,
		0x253f,0x2541,0x2544,0x2546,0x2548,0x254a,0x254b,0x254c,
		0x254d,0x254e,0x254f,0x2552,0x2555,0x2558,0x255b,0x255e,
		0x255f,0x2560,0x2561,0x2562,0x2564,0x2566,0x2568,0x2569,
		0x256a,0x256b,0x256c,0x256d,0x256f,0x2573,0x212b,0x212c 
	};

	if(hankaku < 0xa0 || hankaku > 0xdf)
		return 0;

	return z[hankaku - 0xa0];
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：全角カナを半角カナに変換する											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::KanaZen2Han(unsigned int zenkaku)
{
	if(zenkaku == 0x2121)			return 0xa0;
	else if(zenkaku == 0x2123)		return 0xa1;
	else if(zenkaku == 0x2156)		return 0xa2;
	else if(zenkaku == 0x2157)		return 0xa3;
	else if(zenkaku == 0x2122)		return 0xa4;
	else if(zenkaku == 0x2126)		return 0xa5;
	else if(zenkaku == 0x2572)		return 0xa6;
	else if(zenkaku == 0x2521)		return 0xa7;
	else if(zenkaku == 0x2523)		return 0xa8;
	else if(zenkaku == 0x2525)		return 0xa9;
	else if(zenkaku == 0x2527)		return 0xaa;
	else if(zenkaku == 0x2529)		return 0xab;
	else if(zenkaku == 0x2563)		return 0xac;
	else if(zenkaku == 0x2565)		return 0xad;
	else if(zenkaku == 0x2567)		return 0xae;
	else if(zenkaku == 0x2543)		return 0xaf;
	else if(zenkaku == 0x213c)		return 0xb0;
	else if(zenkaku == 0x2522)		return 0xb1;
	else if(zenkaku == 0x2524)		return 0xb2;
	else if(zenkaku == 0x2526)		return 0xb3;
	else if(zenkaku == 0x2528)		return 0xb4;
	else if(zenkaku == 0x252a)		return 0xb5;
	else if(zenkaku == 0x252b)		return 0xb6;
	else if(zenkaku == 0x252d)		return 0xb7;
	else if(zenkaku == 0x252f)		return 0xb8;
	else if(zenkaku == 0x2531)		return 0xb9;
	else if(zenkaku == 0x2533)		return 0xba;
	else if(zenkaku == 0x2535)		return 0xbb;
	else if(zenkaku == 0x2537)		return 0xbc;
	else if(zenkaku == 0x2539)		return 0xbd;
	else if(zenkaku == 0x253b)		return 0xbe;
	else if(zenkaku == 0x253d)		return 0xbf;
	else if(zenkaku == 0x253f)		return 0xc0;
	else if(zenkaku == 0x2541)		return 0xc1;
	else if(zenkaku == 0x2544)		return 0xc2;
	else if(zenkaku == 0x2546)		return 0xc3;
	else if(zenkaku == 0x2548)		return 0xc4;
	else if(zenkaku == 0x254a)		return 0xc5;
	else if(zenkaku == 0x254b)		return 0xc6;
	else if(zenkaku == 0x254c)		return 0xc7;
	else if(zenkaku == 0x254d)		return 0xc8;
	else if(zenkaku == 0x254e)		return 0xc9;
	else if(zenkaku == 0x254f)		return 0xca;
	else if(zenkaku == 0x2552)		return 0xcb;
	else if(zenkaku == 0x2555)		return 0xcc;
	else if(zenkaku == 0x2558)		return 0xcd;
	else if(zenkaku == 0x255b)		return 0xce;
	else if(zenkaku == 0x255e)		return 0xcf;
	else if(zenkaku == 0x255f)		return 0xd0;
	else if(zenkaku == 0x2560)		return 0xd1;
	else if(zenkaku == 0x2561)		return 0xd2;
	else if(zenkaku == 0x2562)		return 0xd3;
	else if(zenkaku == 0x2564)		return 0xd4;
	else if(zenkaku == 0x2566)		return 0xd5;
	else if(zenkaku == 0x2568)		return 0xd6;
	else if(zenkaku == 0x2569)		return 0xd7;
	else if(zenkaku == 0x256a)		return 0xd8;
	else if(zenkaku == 0x256b)		return 0xd9;
	else if(zenkaku == 0x256c)		return 0xda;
	else if(zenkaku == 0x256d)		return 0xdb;
	else if(zenkaku == 0x256f)		return 0xdc;
	else if(zenkaku == 0x2573)		return 0xdd;
	else if(zenkaku == 0x212b)		return 0xde;
	else if(zenkaku == 0x212c)		return 0xdf;
	else							return 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１バイト文字用にエスケープシーケンスを出力							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CCodeConvManager::set1byte
	(	CArchive& arOut
	)
{
	//ASCII コードセット
	arOut << (BYTE)CODE_ESC;
	arOut << (BYTE)'(';
	arOut << (BYTE)'B';
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：２バイト文字用にエスケープシーケンスを出力							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CCodeConvManager::set2byte
	(	CArchive&	arOut
	)
{
	//JIS X 0208-1983(新JIS漢字)
	arOut << (BYTE)CODE_ESC;
	arOut << (BYTE)'$';
	arOut << (BYTE)'B';
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１バイト文字用にエスケープシーケンスを出力							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CCodeConvManager::set1byte
	(	CString*	pobjLine
	)
{
	if(pobjLine == NULL)
		return;

	//ASCII コードセット
	(*pobjLine) += (UCHAR)CODE_ESC;
	(*pobjLine) += (UCHAR)'(';
	(*pobjLine) += (UCHAR)'B';
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：２バイト文字用にエスケープシーケンスを出力							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CCodeConvManager::set2byte
	(	CString*	pobjLine
	)
{
	if(pobjLine == NULL)
		return;

	//JIS X 0208-1983(新JIS漢字)
	(*pobjLine) += (UCHAR)CODE_ESC;
	(*pobjLine) += (UCHAR)'$';
	(*pobjLine) += (UCHAR)'B';
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：scan forward if Shift-JIS code or JIS code exists						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CCodeConvManager::checkforward
	(	unsigned char* p,
		unsigned int size
	)
{
	unsigned char c=0;

    if (size == 0)
        return INPUT_EUC;
    c = *p++;
    while (1) {
        if (c == CODE_ESC) {
            if (--size == 0)
                break;
            if ((c = *p++) == '$') {
                if (--size == 0)
                    break;
                if ((c = *p++) == 'B' || c == '@')
                    return INPUT_JIS;
                else
                    continue;
            }
            else if (c == 'K')
                return INPUT_JIS;
            else
                continue;
        }
        else if (c >= 0x81) {
            if (c == 0x8e) {
                if (--size == 0)
                    break;
                p++;
            }
            else if (c <= 0x9f) {
                if (--size == 0)
                    break;
                c = *p++;
                if (iskanji2nd(c))
                    return INPUT_SJIS;
                else
                    continue;
            }
            else if (c >= 0xa1 && c <= 0xdf) {
                if (--size == 0)
                    break;
                c = *p++;
                if (isHanKana(c))
                    continue;
                else if (iseuc(c))
                    return INPUT_EUC;
                else
                    continue;
            }
            else if (c != 0xa0)
                return INPUT_EUC;
        }
        if (--size == 0)
            break;
        c = *p++;
    }
    return INPUT_EUC;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：detect kanji code														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CCodeConvManager::detectcode
	(	unsigned char* buf, 
		unsigned int size,
		BOOL* pbIsEnd
	)
{
    unsigned char *p=NULL, c=NULL;
    unsigned int count=NULL;
    int unknownstat=NULL;
	(*pbIsEnd) = FALSE;

    count = size;
    if (count == 0)
        return INPUT_ASCII;
    p = buf;
    unknownstat = 0;
    c = *p++;
    while (1) {
        if (c == CODE_ESC) {
            if (--count == 0)
                break;
            if ((c = *p++) == '$') {
                if (--count == 0)
                    break;
                if ((c = *p++) == 'B' || c == '@')
                    return INPUT_JIS;
                else
                    continue;
            }
            else if (c == 'K')
                return INPUT_JIS;
            else
                continue;
        }
        else if (c >= 0x81) {
            if (c == 0x8e) {
                if (--count == 0)
                    break;
                c = *p++;
                if (isHanKana(c))
                    unknownstat |= 2;
                else if (iskanji2nd(c))
                    return INPUT_SJIS;
                else
                    continue;
            }
            else if (c <= 0x9f) {
                if (--count == 0)
                    break;
                c = *p++;
                if (iskanji2nd(c))
                    return INPUT_SJIS;
                else
                    continue;
            }
            else if (c >= 0xa1 && c <= 0xdf || c == 0xfd || c == 0xfe) {
                if (--count == 0)
                    break;
                c = *p++;
                if (iseuc(c))
                    if (isHanKana(c))
                        return checkforward(p, count - 1);
                    else
                        return INPUT_EUC;
                else
                    continue;
            }
            else if (c >= 0xe0 && c <= 0xfc) {
                if (--count == 0)
                    break;
                c = *p++;
                if (iskanji2nd(c))
                    if (iseuc(c))
                        unknownstat |= 1;
                    else
                        return INPUT_SJIS;
                else
                    if (iseuc(c))
                        return INPUT_EUC;
                    else
                        continue;
            }
        }
        if (--count == 0)
            break;
        c = *p++;
    }
	if(count == 0)
		(*pbIsEnd) = TRUE;

    switch (unknownstat) {
    case 1:
    case 3:
        return INPUT_UNKNOWN;
    case 2:
        return INPUT_SJIS;
    default:
        return INPUT_ASCII;
    }
}


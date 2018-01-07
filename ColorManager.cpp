// ColorManager.cpp: CColorManager クラスのインプリメンテーション
//

#include "stdafx.h"
#include <Afxtempl.h>			//For CMap
#include <ctype.h>				//For isalpha()
#include "Editor.h"
#include "Editor_Def.h"
#include "ColorManager.h"
#include "Line.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CColorManager::CColorManager()
{
	m_pApp = (CEditorApp*)AfxGetApp();

	// メンバ変数初期化
	m_FontColor     = RGB(255,255,255);				// 通常の文字色
	m_bComment1Clr  = TRUE;							// Ｃ・Ｃ＋＋コメント有効フラグ
	m_CommentColor  = RGB(0,255,0);					// コメント色（Ｃ／Ｃ＋＋用）
	m_bComment2Clr  = FALSE;						// ＶＢコメント有効フラグ
	m_CommentColor2 = RGB(0,255,0);					// コメント色（ＶＢ用）
	m_bComment3Clr  = FALSE;						// アセンブラコメント有効フラグ
	m_CommentColor3 = RGB(0,255,0);					// コメント色（アセンブラ用）
	m_bComment4Clr  = FALSE;						// 引用符号コメント有効フラグ
	m_CommentColor4 = RGB(0,255,0);					// コメント色（引用符号用）
	m_bUrlClr       = TRUE;							// ＵＲＬ有効フラグ
	m_UrlColor      = RGB(255,0,255);				// ＵＲＬ色
	m_bEMailClr     = TRUE;							// ｅ－ｍａｉｌ有効フラグ
	m_EMailColor    = RGB(255,0,255);				// ｅ－ｍａｉｌ色
	m_KeyNo1Color   = RGB(0,255,255);				// キーワードＮｏ１の文字色
	m_KeyNo2Color   = RGB(0,255,255);				// キーワードＮｏ２の文字色
	m_KeyNo3Color   = RGB(0,255,255);				// キーワードＮｏ３の文字色
	m_KeyNo4Color   = RGB(0,255,255);				// キーワードＮｏ４の文字色
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CColorManager::~CColorManager()
{
	//マップデータクリア
	POSITION	pos;
	CString		objStr;
	COLORREF*	pColor;
	long lCnt = m_objMapStrToColor.GetCount();
	for( pos=m_objMapStrToColor.GetStartPosition(); pos!=0; )
	{
		m_objMapStrToColor.GetNextAssoc(pos,objStr,(void*&)pColor );
		if(pColor != NULL)
			delete pColor;
	}
	m_objMapStrToColor.RemoveAll();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワードカラーリング												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::KeyWordColoring
	(	CLine*	pobjLine
	)
{
	char cBuf=0;
	long lOffset=0;
	long lCnt=0;
	long lDoubleQuateCount=0;		// " の出現回数
	BOOL bOK=FALSE;
	BOOL bIsWithinSlashAstar=FALSE;		// C            の行コメント
	BOOL bIsAppearLess=FALSE;			// '<' 出現ﾌﾗｸﾞ

	for(lCnt=0; lCnt<pobjLine->GetLength(); )//文字列長分ループ
	{
		if(IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)))//２バイト文字の第一バイト
		{
			if(bIsWithinSlashAstar) //  /* ～ */のコメント中
			{
				if(m_bComment1Clr) //Ｃ・Ｃ＋＋コメント有効
				{
					//コメント色セット
					pobjLine->SetColorAt(lCnt,lCnt+2,m_CommentColor);
				}
				else //Ｃ・Ｃ＋＋コメント無効
				{
					//通常文字色セット
					pobjLine->SetColorAt(lCnt,lCnt+2,m_FontColor);
				}
			}
			else
			{
				//通常文字色セット
				pobjLine->SetColorAt(lCnt,lCnt+2,m_FontColor);
			}
			lCnt+=2;
		}
		else //１バイト文字
		{
			if(pobjLine->GetAt(lCnt) == '"') // " の出現回数をカウントアップ
				lDoubleQuateCount++;

			//////////////////////////////////////
			//		コメントチェック			//
			//////////////////////////////////////

			if( (pobjLine->GetAt(lCnt)=='/')     &&		// /* ～　*/ のコメント開始
				(pobjLine->GetAt(lCnt+1)=='*')   &&
				!IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt+1))
			)
			{
				bIsWithinSlashAstar = TRUE;

				if(m_bComment1Clr) //Ｃ・Ｃ＋＋コメント有効
				{
					//コメント色セット
					pobjLine->SetColorAt(lCnt,lCnt+2,m_CommentColor);
				}
				else //Ｃ・Ｃ＋＋コメント無効
				{
					//通常文字色セット
					pobjLine->SetColorAt(lCnt,lCnt+2,m_FontColor);
				}
				lCnt+=2;
				continue;
			}
			else if( (pobjLine->GetAt(lCnt)=='*')     && // /* ～　*/ のコメント終了
					 (pobjLine->GetAt(lCnt+1)=='/')   &&
					 !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt+1))
			)
			{
				bIsWithinSlashAstar = FALSE;

				if(m_bComment1Clr) //Ｃ・Ｃ＋＋コメント有効
				{
					//コメント色セット
					pobjLine->SetColorAt(lCnt,lCnt+2,m_CommentColor);
				}
				else //Ｃ・Ｃ＋＋コメント無効
				{
					//通常文字色セット
					pobjLine->SetColorAt(lCnt,lCnt+2,m_FontColor);
				}
				lCnt+=2;
				continue;
			}

			if( (pobjLine->GetAt(lCnt)=='/')    &&		//ダブルスラッシュのコメント
				(pobjLine->GetAt(lCnt+1)=='/')  &&
				!IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt+1))
			)
			{
				if( m_bComment1Clr != FALSE && 		//Ｃ・Ｃ＋＋コメント有効
					(lDoubleQuateCount % 2) == 0	//かつ""の中ではない
				)
				{
					//行末までコメント色セット
					pobjLine->SetColorAt(lCnt,pobjLine->GetLength(),m_CommentColor);
					return;
				}
				else //Ｃ・Ｃ＋＋コメント無効
				{
					//通常文字色セット
					pobjLine->SetColorAt(lCnt,lCnt+2,m_FontColor);
					lCnt+=2;
					continue;
				}
			}
			if( pobjLine->GetAt(lCnt)=='\'' )			//シングルクオート
			{
				if( m_bComment2Clr != FALSE &&		//ＶＢ行コメント有効
					(lDoubleQuateCount % 2) == 0	//かつ""の中ではない
				)
				{
					//行末までコメント色セット
					pobjLine->SetColorAt(lCnt,pobjLine->GetLength(),m_CommentColor2);
					return;
				}
				else //ＶＢ行コメント無効
				{
					if(m_bComment1Clr!=FALSE && bIsWithinSlashAstar!=FALSE)
					{
						//Ｃ・Ｃ＋＋コメント色セット
						pobjLine->SetColorAt(lCnt,m_CommentColor);
					}
					else
					{
						//通常文字色セット
						pobjLine->SetColorAt(lCnt,m_FontColor);
					}
					lCnt++;
					continue;
				}
			}
			if( pobjLine->GetAt(lCnt)==';' )			//セミコロン
			{
				if( m_bComment3Clr != FALSE &&			//アセンブラコメント有効
					(lDoubleQuateCount % 2) == 0		//かつ""の中ではない
				)
				{
					//行末までコメント色セット
					pobjLine->SetColorAt(lCnt,pobjLine->GetLength(),m_CommentColor3);
					return;
				}
				else //アセンブラコメント無効
				{
					if(m_bComment1Clr!=FALSE && bIsWithinSlashAstar!=FALSE)
					{
						//Ｃ・Ｃ＋＋コメント色セット
						pobjLine->SetColorAt(lCnt,m_CommentColor);
					}
					else
					{
						//通常文字色セット
						pobjLine->SetColorAt(lCnt,m_FontColor);
					}
					lCnt++;
					continue;
				}
			}
			if( lCnt==0 )
			{
				if( pobjLine->GetAt(lCnt)=='>' )			//引用符号
				{
					if(m_bComment4Clr) //引用符号コメント有効
					{
						//行末までコメント色セット
						pobjLine->SetColorAt(lCnt,pobjLine->GetLength(),m_CommentColor4);
						return;
					}
					else //引用符号コメント無効
					{
						if(m_bComment1Clr!=FALSE && bIsWithinSlashAstar!=FALSE)
						{
							//Ｃ・Ｃ＋＋コメント色セット
							pobjLine->SetColorAt(lCnt,m_CommentColor);
						}
						else
						{
							//通常文字色セット
							pobjLine->SetColorAt(lCnt,m_FontColor);
						}
						lCnt++;
						continue;
					}
				}
			}

			if(m_bComment1Clr) //Ｃ・Ｃ＋＋コメント有効
			{
				if(bIsWithinSlashAstar) //　/* ～ */ のコメント中
				{
					//コメント色セット
					pobjLine->SetColorAt(lCnt,m_CommentColor);
					lCnt++;
					continue;
				}
			}

			//////////////////////
			//	ＵＲＬチェック	//
			//////////////////////

			if(m_bUrlClr) //ＵＲＬ有効
			{
				bOK = FALSE;
				long lBackCount=0;
				if(pobjLine->GetAt(lCnt) == ':')
				{
					if((lCnt-strlen("http"))>=0)
					{
						if( !pobjLine->Compare(lCnt-strlen("http"),(const unsigned char*)"http",strlen("http")) &&
							!pobjLine->Compare(lCnt+1,(const unsigned char*)"//",strlen("//"))
						)
						{
							cBuf = pobjLine->GetAt(lCnt+1+strlen("//"));
							if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf))
							{
								bOK = TRUE;
								lBackCount = strlen("http");
							}
						}
					}
					if((lCnt-strlen("https"))>=0)
					{
						if( !pobjLine->Compare(lCnt-strlen("https"),(const unsigned char*)"https",strlen("https")) &&
							!pobjLine->Compare(lCnt+1,(const unsigned char*)"//",strlen("//"))
						)
						{
							cBuf = pobjLine->GetAt(lCnt+1+strlen("//"));
							if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf))
							{
								bOK = TRUE;
								lBackCount = strlen("https");
							}
						}
					}
					if((lCnt-strlen("ftp"))>=0)
					{
						if( !pobjLine->Compare(lCnt-strlen("ftp"),(const unsigned char*)"ftp",strlen("ftp")) &&
							!pobjLine->Compare(lCnt+1,(const unsigned char*)"//",strlen("//"))
						)
						{
							cBuf = pobjLine->GetAt(lCnt+1+strlen("//"));
							if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf))
							{
								bOK = TRUE;
								lBackCount = strlen("ftp");
							}
						}
					}
					if((lCnt-strlen("gopher"))>=0)
					{
						if( !pobjLine->Compare(lCnt-strlen("gopher"),(const unsigned char*)"gopher",strlen("gopher")) &&
							!pobjLine->Compare(lCnt+1,(const unsigned char*)"//",strlen("//"))
						)
						{
							cBuf = pobjLine->GetAt(lCnt+1+strlen("//"));
							if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf))
							{
								bOK = TRUE;
								lBackCount = strlen("gopher");
							}
						}
					}
				}

				if(bOK)
				{
					//前方を探索
					for( ; lBackCount>0; lBackCount-- )
					{
						pobjLine->SetColorAt(lCnt-lBackCount,m_UrlColor);
						pobjLine->SetUnderLineAt(lCnt-lBackCount,TRUE);
					}
					pobjLine->SetColorAt(lCnt,m_UrlColor);
					pobjLine->SetUnderLineAt(lCnt,TRUE);
					//後方を探索
					for(lCnt++; ; )
					{
						if(lCnt >= pobjLine->GetLength())
							break;
						cBuf = pobjLine->GetAt(lCnt);
						if( !isValidURL(cBuf) )
							break;
						pobjLine->SetColorAt(lCnt,m_UrlColor);
						pobjLine->SetUnderLineAt(lCnt,TRUE);
						lCnt++;
					}
					continue;
				}
			}

			//////////////////////////////
			//	ｅ－ｍａｉｌチェック	//
			//////////////////////////////

			if(m_bEMailClr) //ｅ－ｍａｉｌ有効
			{
				bOK = FALSE;
				if(pobjLine->GetAt(lCnt) == '@')
				{
					lOffset = pobjLine->GetPreOffset(lCnt);
					if(lOffset != -1)
					{
						cBuf = pobjLine->GetAt(lOffset);
						if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf)) //前方チェック
						{
							cBuf = pobjLine->GetAt(lCnt+1);
							if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf)) //後方チェック
								bOK = TRUE;
						}
					}
				}

				if(bOK)
				{
					//前方を探索
					for(lOffset=lCnt;;)
					{
						lOffset = pobjLine->GetPreOffset(lOffset);
						if(lOffset < 0)
							break;
						cBuf = pobjLine->GetAt(lOffset);
						if( !isValidEMail(cBuf) )
							break;
						pobjLine->SetColorAt(lOffset,m_EMailColor);
						pobjLine->SetUnderLineAt(lOffset,TRUE);
					}
					pobjLine->SetColorAt(lCnt,m_EMailColor);
					pobjLine->SetUnderLineAt(lCnt,TRUE);
					//後方を探索
					for(lCnt++;;)
					{
						if(lCnt >= pobjLine->GetLength())
							break;
						cBuf = pobjLine->GetAt(lCnt);
						if( !isValidEMail(cBuf) )
							break;
						pobjLine->SetColorAt(lCnt,m_EMailColor);
						pobjLine->SetUnderLineAt(lCnt,TRUE);
						lCnt++;
					}
					continue;
				}
			}

			//////////////////////////////
			//	キーワードチェック		//
			//////////////////////////////

			//キーワード候補以外
			if( (!isAlnum(pobjLine->GetAt(lCnt)))	&&
				(pobjLine->GetAt(lCnt) != '_')		&& 
				(pobjLine->GetAt(lCnt) != '#')		&&
				(pobjLine->GetAt(lCnt) != '<')		&&
				(pobjLine->GetAt(lCnt) != '>')		&&
				(pobjLine->GetAt(lCnt) != '/')
			)
			{
				//通常文字色セット
				pobjLine->SetColorAt(lCnt,m_FontColor);
				lCnt++;
				continue;
			}

			//チェック対象単語きり出し
			CString		objCheckStr = _T("");
			long		lCharCnt=0;
			for(lCharCnt=0 ; ; lCharCnt++)
			{
				cBuf = pobjLine->GetAt(lCnt+lCharCnt);
				if( cBuf==0 )
					break;
				if(IsDBCSLeadByteEx(0,cBuf)) //２バイト文字になったらループ終了
					break;
				if( (!isAlnum(cBuf))	&&
					(cBuf != '_')		&&
					(cBuf != '#')		&&
					(cBuf != '<')		&&
					(cBuf != '>')       &&
					(cBuf != '/')
				) 
				{
					break;
				}
				if( (cBuf == '/') && (pobjLine->GetAt(lCnt+1+lCharCnt) == '*') )
					break;
				if( (cBuf == '/') && (pobjLine->GetAt(lCnt+1+lCharCnt) == '/') )
					break;
				if( cBuf == 0x27 ) //シングルクオート
					break;
				if( cBuf == ';' )
					break;

				if( cBuf == '<' )
				{
					if( objCheckStr.IsEmpty() )
					{
						bIsAppearLess = TRUE;
					}
					else
					{
						break;
					}
				}
				if( (cBuf == '>') && (bIsAppearLess != FALSE) )
				{
					bIsAppearLess = FALSE;	// '<'出現ﾌﾗｸﾞ
					objCheckStr += cBuf;
					break;
				}

				objCheckStr += cBuf;
			}

			//登録済みキーワードと一致するか検索
			COLORREF	color_buf=0;
			COLORREF*	pColor=NULL;
			BOOL bFind = m_objMapStrToColor.Lookup( objCheckStr, (void*&)pColor );
			if( bFind != 0 ) //発見！！
				color_buf = (*pColor);
			else //なし
				color_buf = m_FontColor;			//色を通常の文字色に設定

			//色セット
			pobjLine->SetColorAt(lCnt,lCnt+objCheckStr.GetLength(),color_buf);
			lCnt += objCheckStr.GetLength();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：通常の文字色設定処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetFontColor
	(	COLORREF	color
	)
{
	m_FontColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Ｃ・Ｃ＋＋コメント有効フラグ取得処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CColorManager::GetFlagComment1Clr()
{
	return m_bComment1Clr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Ｃ・Ｃ＋＋コメント有効フラグ設定処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetFlagComment1Clr
	(	BOOL	bComment1Clr
	)
{
	m_bComment1Clr = bComment1Clr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コメント色取得処理（Ｃ／Ｃ＋＋用）									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetCommentColor()
{
	return m_CommentColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コメント色設定処理（Ｃ／Ｃ＋＋用）									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetCommentColor
	(	COLORREF	color
	)
{
	m_CommentColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＶＢコメント有効フラグ取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CColorManager::GetFlagComment2Clr()
{
	return m_bComment2Clr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＶＢコメント有効フラグ設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetFlagComment2Clr
	(	BOOL	bComment2Clr
	)
{
	m_bComment2Clr = bComment2Clr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コメント色取得処理（ＶＢ用）											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetCommentColor2()
{
	return m_CommentColor2;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コメント色設定処理（ＶＢ用）											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetCommentColor2
	(	COLORREF	color
	)
{
	m_CommentColor2 = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：アセンブラコメント有効フラグ取得処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CColorManager::GetFlagComment3Clr()
{
	return m_bComment3Clr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：アセンブラコメント有効フラグ設定処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetFlagComment3Clr
	(	BOOL	bComment3Clr
	)
{
	m_bComment3Clr = bComment3Clr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コメント色取得処理（アセンブラ用）									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetCommentColor3()
{
	return m_CommentColor3;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コメント色設定処理（アセンブラ用）									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetCommentColor3
	(	COLORREF	color
	)
{
	m_CommentColor3 = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：引用符号コメント有効フラグ取得処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CColorManager::GetFlagComment4Clr()
{
	return m_bComment4Clr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：引用符号コメント有効フラグ設定処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetFlagComment4Clr
	(	BOOL	bComment4Clr
	)
{
	m_bComment4Clr = bComment4Clr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コメント色取得処理（引用符号用）										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetCommentColor4()
{
	return m_CommentColor4;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コメント色設定処理（引用符号用）										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetCommentColor4
	(	COLORREF	color
	)
{
	m_CommentColor4 = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＵＲＬ有効フラグ取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CColorManager::GetFlagUrlColor()
{
	return m_bUrlClr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＵＲＬ有効フラグ設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetFlagUrlColor
	(	BOOL	bUrlClr
	)
{
	m_bUrlClr = bUrlClr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＵＲＬ色取得処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetUrlColor()
{
	return m_UrlColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＵＲＬ色設定処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetUrlColor
	(	COLORREF	color
	)
{
	m_UrlColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ｅ－ｍａｉｌ有効フラグ取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CColorManager::GetFlagEMailColor()
{
	return m_bEMailClr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ｅ－ｍａｉｌ有効フラグ設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetFlagEMailColor
	(	BOOL	bEMailClr
	)
{
	m_bEMailClr = bEMailClr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ｅ－ｍａｉｌ色取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetEMailColor()
{
	return m_EMailColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ｅ－ｍａｉｌ色設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetEMailColor
	(	COLORREF	color
	)
{
	m_EMailColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：全キーワードの削除処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::DeleteAllKeyWord()
{
	//マップデータクリア
	POSITION	pos;
	CString		objStr;
	COLORREF*	pColor;
	long lCnt = m_objMapStrToColor.GetCount();
	for( pos=m_objMapStrToColor.GetStartPosition(); pos!=0; )
	{
		m_objMapStrToColor.GetNextAssoc(pos,objStr,(void*&)pColor );
		if(pColor != NULL)
			delete pColor;
	}
	m_objMapStrToColor.RemoveAll();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワードＮｏ１の文字色取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetKeyNo1Color()
{
	return m_KeyNo1Color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワード定義ファイル（ＮＯ１）読み込み＆色設定処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetKeyNo1Color
	(	COLORREF	color,				//色
		const char*	szKeyNo1Fname
	)
{
	long lCRCount=0;
	long lLFCount=0;
	CString	objStrBuf="";
	CFile	objFile;
	CString strPath;
	BOOL bFind=FALSE;

	//メンバ変数へ色を保持
	m_KeyNo1Color = color;

	//キーワードファイルオープン
	if(m_pApp->IsDriveRoot() == FALSE)
		objStrBuf = "\\";
	strPath.Format("%s%s%s",m_pApp->m_strExePath,objStrBuf,szKeyNo1Fname);
	if( !objFile.Open( strPath, CFile::modeRead, NULL ) )
		return;
	//アーカイブ作成
	CArchive ar( &objFile, CArchive::load );

	//１行づつ読み込み
	m_pApp->PrepareMyReadString();
	for(;;)
	{
		objStrBuf.Empty();
		if( m_pApp->MyReadString(ar,objStrBuf,&lCRCount,&lLFCount) == 0 )
		{
			if( objStrBuf.IsEmpty() )
				break;
		}

		if( objStrBuf.Left(2) == _T("//") )// "//"で始まる行を無視
			continue;
		if( objStrBuf.IsEmpty() )
			continue;

		//マップへ登録
		COLORREF*	pColor = NULL;
		BOOL bFind = m_objMapStrToColor.Lookup( objStrBuf, (void*&)pColor );
		if( bFind != 0 ) //発見！！
		{
			(*pColor) = color;					//色変更
		}
		else //なし
		{
			COLORREF*	pColor = new COLORREF;	//色新規登録
			(*pColor) = color;
			m_objMapStrToColor.SetAt(objStrBuf,pColor);
		}
	}

	//アーカイブ＆ファイルクローズ
	ar.Close();
	objFile.Close();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワードＮｏ２の文字色取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetKeyNo2Color()
{
	return m_KeyNo2Color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワード定義ファイル（ＮＯ２）読み込み＆色設定処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetKeyNo2Color
	(	COLORREF	color,				//色
		const char*	szKeyNo2Fname
	)
{
	long lCRCount=0;
	long lLFCount=0;
	CString	objStrBuf="";
	CFile	objFile;
	CString strPath;

	//メンバ変数へ色を保持
	m_KeyNo2Color = color;

	//キーワードファイルオープン
	if(m_pApp->IsDriveRoot() == FALSE)
		objStrBuf = "\\";
	strPath.Format("%s%s%s",m_pApp->m_strExePath,objStrBuf,szKeyNo2Fname);
	if( !objFile.Open( strPath, CFile::modeRead, NULL ) )
		return;
	//アーカイブ作成
	CArchive ar( &objFile, CArchive::load );

	//１行づつ読み込み
	m_pApp->PrepareMyReadString();
	for(;;)
	{
		if( m_pApp->MyReadString(ar,objStrBuf,&lCRCount,&lLFCount) == 0 )
		{
			if( objStrBuf.IsEmpty() )
				break;
		}

		if( objStrBuf.Left(2) == _T("//") ) // "//"で始まる行を無視
			continue;
		if( objStrBuf.IsEmpty() )
			continue;

		//マップへ登録
		COLORREF*	pColor = NULL;
		BOOL bFind = m_objMapStrToColor.Lookup( objStrBuf, (void*&)pColor );
		if( bFind != 0 ) //発見！！
		{
			(*pColor) = color;					//色変更
		}
		else //なし
		{
			COLORREF*	pColor = new COLORREF;	//色新規登録
			(*pColor) = color;
			m_objMapStrToColor.SetAt(objStrBuf,pColor);
		}
	}

	//アーカイブ＆ファイルクローズ
	ar.Close();
	objFile.Close();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワードＮｏ３の文字色取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetKeyNo3Color()
{
	return m_KeyNo3Color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワード定義ファイル（ＮＯ３）読み込み＆色設定処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetKeyNo3Color
	(	COLORREF	color,				//色
		const char*	szKeyNo3Fname
	)
{
	long lCRCount=0;
	long lLFCount=0;
	CString	objStrBuf="";
	CFile	objFile;
	CString strPath;

	//メンバ変数へ色を保持
	m_KeyNo3Color = color;

	//キーワードファイルオープン
	if(m_pApp->IsDriveRoot() == FALSE)
		objStrBuf = "\\";
	strPath.Format("%s%s%s",m_pApp->m_strExePath,objStrBuf,szKeyNo3Fname);
	if( !objFile.Open( strPath, CFile::modeRead, NULL ) )
		return;
	//アーカイブ作成
	CArchive ar( &objFile, CArchive::load );

	//１行づつ読み込み
	m_pApp->PrepareMyReadString();
	for(;;)
	{
		if( m_pApp->MyReadString(ar,objStrBuf,&lCRCount,&lLFCount) == 0 )
		{
			if( objStrBuf.IsEmpty() )
				break;
		}

		if( objStrBuf.Left(2) == _T("//") ) // "//"で始まる行を無視
			continue;
		if( objStrBuf.IsEmpty() )
			continue;

		//マップへ登録
		COLORREF*	pColor = NULL;
		BOOL bFind = m_objMapStrToColor.Lookup( objStrBuf, (void*&)pColor );
		if( bFind != 0 ) //発見！！
		{
			(*pColor) = color;					//色変更
		}
		else //なし
		{
			COLORREF*	pColor = new COLORREF;	//色新規登録
			(*pColor) = color;
			m_objMapStrToColor.SetAt(objStrBuf,pColor);
		}
	}

	//アーカイブ＆ファイルクローズ
	ar.Close();
	objFile.Close();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワードＮｏ４の文字色取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetKeyNo4Color()
{
	return m_KeyNo4Color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワード定義ファイル（ＮＯ４）読み込み＆色設定処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetKeyNo4Color
	(	COLORREF	color,				//色
		const char*	szKeyNo4Fname
	)
{
	long lCRCount=0;
	long lLFCount=0;
	CString	objStrBuf="";
	CFile	objFile;
	CString strPath;

	//メンバ変数へ色を保持
	m_KeyNo4Color = color;

	//キーワードファイルオープン
	if(m_pApp->IsDriveRoot() == FALSE)
		objStrBuf = "\\";
	strPath.Format("%s%s%s",m_pApp->m_strExePath,objStrBuf,szKeyNo4Fname);
	if( !objFile.Open( strPath, CFile::modeRead, NULL ) )
		return;
	//アーカイブ作成
	CArchive ar( &objFile, CArchive::load );

	//１行づつ読み込み
	m_pApp->PrepareMyReadString();
	for(;;)
	{
		if( m_pApp->MyReadString(ar,objStrBuf,&lCRCount,&lLFCount) == 0 )
		{
			if( objStrBuf.IsEmpty() )
				break;
		}

		if( objStrBuf.Left(2) == _T("//") ) // "//"で始まる行を無視
			continue;
		if( objStrBuf.IsEmpty() )
			continue;

		//マップへ登録
		COLORREF*	pColor = NULL;
		BOOL bFind = m_objMapStrToColor.Lookup( objStrBuf, (void*&)pColor );
		if( bFind != 0 ) //発見！！
		{
			(*pColor) = color;					//色変更
		}
		else //なし
		{
			COLORREF*	pColor = new COLORREF;	//色新規登録
			(*pColor) = color;
			m_objMapStrToColor.SetAt(objStrBuf,pColor);
		}
	}

	//アーカイブ＆ファイルクローズ
	ar.Close();
	objFile.Close();
}


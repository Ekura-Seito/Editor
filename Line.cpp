// Line.cpp: CLine クラスのインプリメンテーション
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "Editor.h"
#include "Line.h"
#include "CodeConvManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLine::CLine()
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_strLine = "";
	m_strBuf = "";
	m_bMark = 0;
	m_nDiffLineKind = DIFF_LINEKIND_COMMON;
	m_nDirCmpLineKind = DIRCMP_LINEKIND_NORMAL;
	m_objDirCmpFileName = _T("");
	m_lSavedLineLength = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLine::CLine(const unsigned char* pszLine)
{
	m_pApp = (CEditorApp*)AfxGetApp();
	if(pszLine != NULL)
		m_strLine = pszLine;
	else
		m_strLine = "";
	m_strBuf = "";
	m_bMark = 0;
	m_nDiffLineKind = DIFF_LINEKIND_COMMON;
	m_nDirCmpLineKind = DIRCMP_LINEKIND_NORMAL;
	m_objDirCmpFileName = _T("");
	m_lSavedLineLength = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLine::~CLine()
{
}

/////////////////////////////////////////////////////////////////////////////
//	CLine パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：[ ] 演算子															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned char CLine::operator[](long lOffset) const
{
	if(lOffset < m_strLine.GetLength())
		return m_strLine.GetAt(lOffset);
	else
		return (unsigned char)0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１文字取得															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned char CLine::GetAt(long lOffset) const
{
	if(lOffset < m_strLine.GetLength())
		return m_strLine.GetAt(lOffset);
	else
		return (unsigned char)0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１文字設定															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SetAt
	(	long	lOffset,
		unsigned char	c
	)
{
	if(lOffset < 0 || lOffset >= m_strLine.GetLength())
		return;

	if( c == 0 )
		m_strLine = m_strLine.Left(lOffset);
	else
		m_strLine.SetAt(lOffset,c);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列長取得															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLine::GetLength()
{
	return m_strLine.GetLength();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：スペースパディング													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::PadSpace
	(	long lOffset		//どこまでか
	)
{
	if(lOffset < 0)
		return;
	while(GetLength() <= lOffset)
		m_strLine += ' ';
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１行設定																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::Set
	(	const unsigned char*	pszLine
	)
{
	if(pszLine==NULL)
		return;
	m_strLine = pszLine;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：連結																	|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::Catenate
	(	const unsigned char*	pszSubStr
	)
{
	if(pszSubStr==NULL)
		return;
	CString buf = pszSubStr;
	m_strLine += buf;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１文字連結															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::Catenate
	(	unsigned char	c
	)
{
	m_strLine += c;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列挿入															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::Insert
	(	long	lOffset,					//挿入位置
		const unsigned char*	pszInsStr	//挿入文字列
	)
{
	if(lOffset < 0)
		return;
	if(pszInsStr==NULL)
		return;
	m_strLine.Insert(lOffset,(LPCTSTR)pszInsStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字挿入																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::InsertChar
	(	long			lOffset,		//挿入位置
		unsigned char	c1stChar,		//挿入文字第一バイト
		unsigned char	c2ndChar		//挿入文字第二バイト（１バイト文字のときはNULL）
	)
{
	unsigned char buf[]={c1stChar,c2ndChar,0};
	if(lOffset < 0)
		return;
	m_strLine.Insert(lOffset,(LPCTSTR)buf);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字指定個数挿入														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::InsertChars
	(	long			lOffset,		//挿入位置
		unsigned char	cChar,			//挿入文字
		long			lCnt			//挿入個数
	)
{
	CString	buf="";
	
	for(long i=0; i<lCnt; i++)
		buf += cChar;
	m_strLine.Insert(lOffset,buf);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字上書き															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::OverWriteChar
	(	long			lOffset,		//上書き位置
		unsigned char	c1stChar,		//上書き文字第一バイト
		unsigned char	c2ndChar		//上書き文字第二バイト（１バイト文字のときはNULL）
	)
{
	if(lOffset < 0)
		return;
	
	if((c1stChar != 0) && (c2ndChar == 0))//１ﾊﾞｲﾄの上書き
	{
		if(lOffset >= m_strLine.GetLength())
			PadSpace(lOffset);

		if(IsDBCSLeadByteEx(0,m_strLine.GetAt(lOffset)))//上書き対象が２ﾊﾞｲﾄ文字の第一ﾊﾞｲﾄ
		{
			m_strLine.SetAt(lOffset,c1stChar);
			m_strLine.SetAt(lOffset+1,' '); //２バイト目をスペースで置き換え
		}
		else //上書き対象が１ﾊﾞｲﾄ文字
		{
			m_strLine.SetAt(lOffset,c1stChar);
		}
	}
	else if((c1stChar != 0) && (c2ndChar != 0))//２ﾊﾞｲﾄの上書き
	{
		if((lOffset+1) >= m_strLine.GetLength())
			PadSpace(lOffset+1);

		if(IsDBCSLeadByteEx(0,m_strLine.GetAt(lOffset)))//上書き対象が２ﾊﾞｲﾄ文字の第一ﾊﾞｲﾄ
		{
			//そのまま上書き
			m_strLine.SetAt(lOffset,c1stChar);
			m_strLine.SetAt(lOffset+1,c2ndChar);
		}
		else //上書き対象が１バイト文字
		{
			m_strLine.SetAt(lOffset,c1stChar);
			if(IsDBCSLeadByteEx(0,m_strLine.GetAt(lOffset+1)))//上書き対象の2文字目が２ﾊﾞｲﾄ文字の第一ﾊﾞｲﾄ
			{
				m_strLine.SetAt(lOffset+1,c2ndChar);
				m_strLine.SetAt(lOffset+2,' '); //２文字目をスペースで上書き
			}
			else 
			{
				m_strLine.SetAt(lOffset+1,c2ndChar);
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字削除(DELETE)														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::DeleteChar
	(	long		lOffset,			//文字削除位置(IN)
		unsigned char* pc1stChar,		//削除された文字の第一バイト(OUT)
		unsigned char* pc2ndChar		//削除された文字の第二バイト(OUT)
	)
{
	if(lOffset < 0 || lOffset >= m_strLine.GetLength())
	{
		(*pc1stChar) = 0;
		(*pc2ndChar) = 0;
		return;
	}

	if(IsDBCSLeadByteEx(0,m_strLine.GetAt(lOffset)))
	{
		(*pc1stChar) = m_strLine.GetAt(lOffset);
		(*pc2ndChar) = m_strLine.GetAt(lOffset+1);
		m_strLine.Delete(lOffset,2);
	}
	else
	{
		(*pc1stChar) = m_strLine.GetAt(lOffset);
		(*pc2ndChar) = 0;
		m_strLine.Delete(lOffset,1);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字削除(BACK SPACE)													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::BackSpaceChar
	(	long		lOffset,			//文字削除位置(IN)
		unsigned char* pc1stChar,		//削除された文字の第一バイト(OUT)
		unsigned char* pc2ndChar		//削除された文字の第二バイト(OUT)
	)
{
	if(lOffset < 0 || lOffset > m_strLine.GetLength())
	{
		(*pc1stChar) = 0;
		(*pc2ndChar) = 0;
		return;
	}

	//２バイト文字を考慮
	for(long lCnt=0; ; )
	{
		if(IsDBCSLeadByteEx(0,m_strLine.GetAt(lCnt)))//２バイト文字の第一バイト
		{
			if(lCnt < (lOffset-2))
				lCnt+=2;
			else
			{
				//削除文字通知
				(*pc1stChar) = m_strLine.GetAt(lCnt);
				(*pc2ndChar) = m_strLine.GetAt(lCnt+1);
				m_strLine.Delete(lCnt,2);
				break;
			}
		}
		else //１バイト文字
		{
			if(lCnt < (lOffset-1))
				lCnt+=1;
			else
			{
				//削除文字通知
				(*pc1stChar) = m_strLine.GetAt(lCnt);
				(*pc2ndChar) = 0;
				m_strLine.Delete(lCnt,1);
				break;
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列切り取り処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::Trim
	(	long	lStartOffset,	//切り取り対象開始オフセット（ゼロオリジン）
		long	lEndOffset		//切り取り対象終了オフセット（ゼロオリジン）
	)
{
	if(lStartOffset < 0 || lEndOffset < 0)
		return;
	if( (lStartOffset > m_strLine.GetLength()) || (lEndOffset > m_strLine.GetLength()) )
		return;

	m_strLine.Delete(lStartOffset,(lEndOffset-lStartOffset));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：タブ数取得処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLine::GetTabCount()
{
	int pos=0;
	int nTabCnt=0;
	for(;;)
	{
		pos = m_strLine.Find('\t',pos);
		if(pos == -1)
			break;
		nTabCnt+=1;
		pos+=1;
	}
	return nTabCnt;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：色取得処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CLine::GetColorAt
	(	long	lOffset
	)
{
	if(lOffset < 0 || lOffset >= m_Color.GetSize())
		return 0;

	return (COLORREF)m_Color[lOffset];
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：色設定処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SetColorAt
	(	long		lOffset,
		COLORREF	color
	)
{
	if(lOffset < 0 || lOffset >= m_Color.GetSize())
		return;

	m_Color[lOffset] = (CObject*)color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：色設定処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SetColorAt
	(	long		lSOffset,
		long		lEOffset,
		COLORREF	color
	)
{
	if(lSOffset < 0 || lSOffset >= m_Color.GetSize())
		return;
	if(lEOffset < 0 || lEOffset > m_Color.GetSize())
		return;

	for(long l=lSOffset; l<lEOffset; l++)
		m_Color.SetAtGrow(l,(CObject*)color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：下線情報取得処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CLine::GetUnderLineAt
	(	long	lOffset
	)
{
	if(lOffset < 0 || lOffset >= m_bNeedUnderLine.GetSize())
		return FALSE;

	return (BOOL)m_bNeedUnderLine[lOffset];
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：下線情報設定処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SetUnderLineAt
	(	long		lOffset,
		BOOL		bNeedUnderLine
	)
{
	if(lOffset < 0 || lOffset >= m_bNeedUnderLine.GetSize())
		return;

	m_bNeedUnderLine.SetAtGrow(lOffset,(CObject*)bNeedUnderLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：検索実行処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLine::DoFind							//発見時：開始オフセット、未発見時：－１
	(	long		lStartOffset,			//検索開始オフセット
		long		lEndOffset,				//検索終了オフセット
		CString		objFindStr,				//検索対象文字列
		BOOL		bAllAlphaNum,			//検索対象文字列がすべて英数字か？
		BOOL		bIsMatchCase,			//大文字、小文字を区別するか？
		BOOL		bIsMatchWholeWord,		//単語全体が一致するもの？
		BOOL		bIsUseReglarExpression,	//正規表現を使うか？
		long*		plMatchLength
	)
{
	CString		objMsg, objBuf;
	int			nPos=0, cflags=0, eflags=0, start=0, end=0, ret=0;
	char 		szMsg[256];

	if(lStartOffset < 0 || lStartOffset >= m_strLine.GetLength())
		return -1;
	if(lEndOffset < 0 || lEndOffset > m_strLine.GetLength())
		return -1;

	//検索対象取り出し
	CString strTarget = m_strLine.Mid(lStartOffset,(lEndOffset-lStartOffset+1));
	CString strFind   = objFindStr;

	if(bIsUseReglarExpression) //正規表現を使う場合
	{
		if( m_pApp->m_lpfnRegFunc1==NULL || m_pApp->m_lpfnRegFunc2==NULL ||
			m_pApp->m_lpfnRegFunc3==NULL || m_pApp->m_lpfnRegFunc4==NULL ||
			m_pApp->m_lpfnRegFunc5==NULL )
			return -1;

		cflags = REG_EXTENDED;
		if(bIsMatchCase==FALSE)//大小文字を区別しない
			cflags |= REG_ICASE;
		//正規表現のコンパイルが必要ならばコンパイルするmyIsNeedRegcomp
		if(m_pApp->m_lpfnRegFunc5((const char*)(LPCTSTR)strFind,cflags))
		{
			//regcomp
			ret = m_pApp->m_lpfnRegFunc1((const char*)(LPCTSTR)strFind,cflags);
			if(ret != 0) //error
			{
				memset(szMsg,0,sizeof(szMsg));
				//regerror
				m_pApp->m_lpfnRegFunc3(ret,szMsg,sizeof(szMsg));
				objMsg.LoadString(IDS_REGEX_COMPERR);
				objBuf.Format("%s\r\n%s",objMsg,szMsg);
				m_pApp->MyMessageBox(objBuf,(MB_OK|MB_ICONSTOP),0);
				//regfree
				m_pApp->m_lpfnRegFunc4();
				return -1;
			}
		}

		//検索myRegexec
		if(m_pApp->m_lpfnRegFunc2((const char*)(LPCTSTR)strTarget,eflags,&start,&end)==0)//match!
		{
			(*plMatchLength) = end - start;
			return (lStartOffset + start);
		}
		else
			return -1;
	}
	else //正規表現を使わない場合
	{
		if(bIsMatchCase==FALSE) //大文字、小文字区別なし
		{
			strTarget.MakeUpper();
			strFind.MakeUpper();
		}

		nPos = strTarget.Find(strFind,0);
		if(nPos == -1) //見つからない
			return -1;
		if(!bIsMatchWholeWord || !bAllAlphaNum) //単語単位検索ではない
		{
			(*plMatchLength) = strFind.GetLength();
			return( lStartOffset + nPos );
		}
		else //検索対象文字列がすべて英数字で単語単位検索
		{
			for(;;)
			{
				unsigned char c1 = GetAt(lStartOffset+nPos-1);
				unsigned char c2 = GetAt(lStartOffset+nPos+objFindStr.GetLength());
				if( !isAlnum(c1) && c1!='_' && !isAlnum(c2) && c2!='_' ) //前後が英数字ではない
				{
					(*plMatchLength) = objFindStr.GetLength();
					return( lStartOffset + nPos );
				}
				else //前後が英数字
				{
					//次を検索
					nPos = strTarget.Find(strFind,nPos+1);
					if(nPos == -1)
						break;
				}
			}
			return -1;
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：検索（逆方向）実行処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLine::DoReverseFind					//発見時：開始オフセット、未発見時：－１
	(	long		lStartOffset,			//検索開始オフセット
		long		lEndOffset,				//検索終了オフセット
		CString&	objFindStr,				//検索対象文字列
		BOOL		bAllAlphaNum,			//検索対象文字列がすべて英数字か？
		BOOL		bIsMatchCase,			//大文字、小文字を区別するか？
		BOOL		bIsMatchWholeWord,		//単語全体が一致するもの？
		BOOL		bIsUseReglarExpression,	//正規表現を使うか？
		long*		plMatchLength
	)
{
	long	lStartIndex = lStartOffset;
	long	lPreFoundIndex = -1;
	long	lFoundIndex = -1;

	if(lStartOffset < 0 || lStartOffset >= m_strLine.GetLength())
		return -1;
	if(lEndOffset < 0 || lEndOffset > m_strLine.GetLength())
		return -1;

	for(;;)
	{
		lFoundIndex = DoFind( lStartIndex,
							  lEndOffset,
							  objFindStr,
							  bAllAlphaNum,
							  bIsMatchCase,
							  bIsMatchWholeWord,
							  bIsUseReglarExpression,
							  plMatchLength);
		if( lFoundIndex == -1 )
		{
			return lPreFoundIndex;
		}
		else
		{
			lPreFoundIndex = lFoundIndex;
			lStartIndex = lPreFoundIndex + (*plMatchLength);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＧＲＥＰ結果解釈処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::ParseGrepResult
	(	CString&	objFileName,			//ファイル名(out)
		long*		plLineNo				//行番号(out)
	)
{
	long	lCnt=0;
	objFileName = _T("");
	CString objLineNoStr = _T("");

	for(lCnt=0; lCnt<GetLength(); )
	{
		if( IsDBCSLeadByteEx(0,GetAt(lCnt)) )
		{
			//ファイル名とする
			objFileName += GetAt(lCnt);
			lCnt+=1;
			objFileName += GetAt(lCnt);
			lCnt+=1;
		}
		else
		{
			if( GetAt(lCnt) == '(' ) //行番号始まりを発見
			{
				lCnt+=1;
				break;
			}
			else
			{
				//ファイル名とする
				objFileName += GetAt(lCnt);
				lCnt+=1;
			}
		}
	}

	for( ; lCnt<GetLength(); )
	{
		if( !IsDBCSLeadByteEx(0,GetAt(lCnt)) &&
			isdigit(GetAt(lCnt))
		)
		{
			//行番号とする
			objLineNoStr += GetAt(lCnt);
			lCnt+=1;
		}
		else
		{
			break;
		}
	}

	(*plLineNo) = atoi((char*)(LPCTSTR)objLineNoStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列長保持処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SaveLength()
{
	m_lSavedLineLength = GetLength();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：保持しておいた文字列長取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLine::GetSavedLength()
{
	return m_lSavedLineLength;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：マークフラグ設定処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SetFlagMark
	(	BOOL	bMark
	)
{
	m_bMark = bMark;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：マークフラグ取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CLine::GetFlagMark()
{
	return m_bMark;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：最終文字　１文字削除処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::DelEndChar()
{
	if(GetLength() <= 1)
	{
		Set((const unsigned char*)"");
		return;
	}

	long lCnt=0;
	for(;;)
	{
		if(IsDBCSLeadByteEx(0,m_strLine.GetAt(lCnt)))//２ﾊﾞｲﾄ文字の第一ﾊﾞｲﾄ
		{
			if( (lCnt+2) < GetLength() ) //まだ大丈夫
			{
				lCnt+=2;
			}
			else
			{
				SetAt(lCnt,0);
				return;
			}
		}
		else //１バイト文字
		{
			if( (lCnt+1) < GetLength() ) //まだ大丈夫
			{
				lCnt+=1;
			}
			else
			{
				SetAt(lCnt,0);
				return;
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行末の空白文字削除処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::DelEndSpaces()
{
	m_strLine.TrimRight();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行頭の空白文字削除処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::DelTopSpaces()
{
	m_strLine.TrimLeft();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：インデント数取得														|*/
/*|	解説	：初出する文字まで半角空白にしていくつ分のインデントか調べる			|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLine::GetIndent(long lTabSize)
{
	long lIndent=0;
	long lOffset=0;
	long lMargin=0;
	unsigned char c=0;

	for(lOffset=0; lOffset<GetLength(); lOffset++)
	{
		c = GetAt(lOffset);
		if(IsDBCSLeadByteEx(0,c))//２バイト文字の第一バイト
			break;
		else
		{
			if(c == '\t')//タブの場合マージン分インデントを増やす
			{
				lMargin = lTabSize - (lOffset % lTabSize);
				lIndent += lMargin;
			}
			else if(c == ' ')//半角空白の場合インデントは１増やす
			{
				lIndent++;
			}
			else
				break;
		}
	}

	return lIndent;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列比較															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CLine::Compare
	(	CLine*	pobjLine
	)
{
	if(pobjLine == NULL)
		return 1;
	return m_strLine.Compare(pobjLine->m_strLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列比較															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CLine::Compare(long lOffset, const unsigned char* pszBuf, long len)
{
	if(lOffset < 0)
		return 1;
	if(lOffset >= m_strLine.GetLength() && lOffset != 0)
		return 1;
	if(len < 0)
		return 1;

	CString buf = m_strLine.Mid(lOffset,len);
	return buf.Compare((LPCTSTR)pszBuf);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル比較用、行種別設定											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SetDiffLineKind
	(	int		nDiffLineKind
	)
{
	m_nDiffLineKind = nDiffLineKind;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル比較用、行種別取得											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CLine::GetDiffLineKind()
{
	return m_nDiffLineKind;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ比較用、行種別設定										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SetDirCmpLineKind
	(	int		nDirCmpLineKind
	)
{
	m_nDirCmpLineKind = nDirCmpLineKind;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ比較用、行種別取得										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CLine::GetDirCmpLineKind()
{
	return m_nDirCmpLineKind;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ比較でのファイル名設定									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SetDirCmpFileName
	(	CString&	objStr
	)
{
	m_objDirCmpFileName = objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ比較でのファイル名取得									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::GetDirCmpFileName
	(	CString&	objStr
	)
{
	objStr = m_objDirCmpFileName;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：直前オフセット取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLine::GetPreOffset		//直前オフセット
	(	long	lLimitOffset	//リミットオフセット
	)
{
	if( lLimitOffset == 0 ) //直前はもうない
		return -1;

	long lCnt=0;
	for(;;)
	{
		if(IsDBCSLeadByteEx(0,m_strLine.GetAt(lCnt)))//２バイト文字の第一バイト
		{
			if( (lCnt+2) < lLimitOffset ) //まだ大丈夫
				lCnt+=2;
			else
				return lCnt;
		}
		else //１バイト文字
		{
			if( (lCnt+1) < lLimitOffset ) //まだ大丈夫
				lCnt+=1;
			else
				return lCnt;
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット位置のＵＲＬ取得											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CLine::GetUrl
	(	long		lCaretOffset,		//キャレット位置のオフセット
		CString&	objUrl				//ＵＲＬ返却先
	)
{
	CString objBuf = _T("");
	char cBuf=0;
	long lNow=0, lPos=0;
	long lStart=0, lEnd=0;

	if(lCaretOffset < 0 || lCaretOffset >= m_strLine.GetLength())
		return FALSE;

	for(lNow=0; lNow<GetLength(); )
	{
		if((lPos = m_strLine.Find("http://",lNow)) != -1)
			objBuf = "http://";
		else if((lPos = m_strLine.Find("https://",lNow)) != -1)
			objBuf = "https://";
		else if((lPos = m_strLine.Find("ftp://",lNow)) != -1)
			objBuf = "ftp://";
		else if((lPos = m_strLine.Find("gopher://",lNow)) != -1)
			objBuf = "gopher://";
		else
			break;

		//開始オフセット
		lStart = lPos;
		if(lCaretOffset < lStart)
			return FALSE;

		//終了オフセット
		for(lEnd=lStart+objBuf.GetLength(); lEnd<GetLength(); lEnd+=1)
		{
			cBuf = GetAt(lEnd);
			if( !isValidURL(cBuf) )
			{
				break;
			}
			else
			{
				objBuf += cBuf;
			}
		}
		if(lCaretOffset < lEnd) //キャレット位置のＵＲＬである
		{
			objUrl = objBuf;
			return TRUE;
		}

		//検索開始位置をずらす
		lNow = lEnd;
	}
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＵＲＬ上チェック取得													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CLine::IsAboveURL
	(	long	lOffset
	)
{
	CString objBuf = _T("");
	long lNow=0,lPos=0;
	long lStart=0, lEnd=0;

	if(lOffset < 0 || lOffset >= m_strLine.GetLength())
		return FALSE;

	for(lNow=0; lNow<GetLength(); )
	{
		if((lPos = m_strLine.Find("http://",lNow)) != -1)
			objBuf = "http://";
		else if((lPos = m_strLine.Find("https://",lNow)) != -1)
			objBuf = "https://";
		else if((lPos = m_strLine.Find("ftp://",lNow)) != -1)
			objBuf = "ftp://";
		else if((lPos = m_strLine.Find("gopher://",lNow)) != -1)
			objBuf = "gopher://";
		else
			break;

		//開始オフセット
		lStart = lPos;
		if(lOffset < lStart)
			return FALSE;

		//終了オフセット探索
		for(lEnd=lStart+objBuf.GetLength(); lEnd<GetLength(); lEnd+=1)
		{
			if( !isValidURL(GetAt(lEnd)) )
				break;
		}
		if(lOffset < lEnd)
			return TRUE;

		//検索開始位置をずらす
		lNow = lEnd;
	}
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キャレット位置のｅ－ｍａｉｌ取得										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CLine::GetEMail
	(	long		lOffset,
		CString& 	objEMail		//ｅ－ｍａｉｌ返却先
	)
{
	long lNow=0, lStart=0, lEnd=0;
	long lPos=0;
	BOOL bOK=FALSE;
	char cBuf=0;

	if(lOffset < 0 || lOffset >= m_strLine.GetLength())
		return FALSE;

	for(lNow=0; lNow<GetLength(); )
	{
		bOK = FALSE;
		lStart = lEnd = 0;
		if(GetAt(lNow) == '@')
		{
			lPos = GetPreOffset(lNow);
			if(lPos != -1)
			{
				cBuf = GetAt(lPos);
				if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf)) //前方チェック
				{
					cBuf = GetAt(lNow+1);
					if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf)) //後方チェック
						bOK = TRUE;
				}
			}
		}

		if(bOK)
		{
			//前方を探索
			for(lPos=lNow;;)
			{
				lPos = GetPreOffset(lPos);
				if(lPos < 0)
					break;
				cBuf = GetAt(lPos);
				if( !isValidEMail(cBuf) )
					break;
				lStart = lPos;
			}
			//後方を探索
			for(lNow+=1;;)
			{
				if(lNow >= GetLength())
					break;
				cBuf = GetAt(lNow);
				if( !isValidEMail(cBuf) )
					break;
				lEnd = lNow;
				lNow+=1;
			}

			if(lStart <= lOffset && lOffset <= lEnd)
			{
				objEMail = m_strLine.Mid(lStart,(lEnd-lStart+1));
				return TRUE;
			}
			else
				continue;
		}
		else 
		{
			lNow+=1;
		}
	}
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ｅ－ｍａｉｌ上チェック取得											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CLine::IsAboveEMail
	(	long	lOffset
	)
{
	long lNow=0, lStart=0, lEnd=0;
	long lPos=0;
	BOOL bOK=FALSE;
	char cBuf=0;

	if(lOffset < 0 || lOffset >= m_strLine.GetLength())
		return FALSE;

	for(lNow=0; lNow<GetLength(); )
	{
		bOK = FALSE;
		lStart = lEnd = 0;
		if(GetAt(lNow) == '@')
		{
			lPos = GetPreOffset(lNow);
			if(lPos != -1)
			{
				cBuf = GetAt(lPos);
				if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf)) //前方チェック
				{
					cBuf = GetAt(lNow+1);
					if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf)) //後方チェック
						bOK = TRUE;
				}
			}
		}

		if(bOK)
		{
			//前方を探索
			for(lPos=lNow;;)
			{
				lPos = GetPreOffset(lPos);
				if(lPos < 0)
					break;
				cBuf = GetAt(lPos);
				if( !isValidEMail(cBuf) )
					break;
				lStart = lPos;
			}
			//後方を探索
			for(lNow+=1;;)
			{
				if(lNow >= GetLength())
					break;
				cBuf = GetAt(lNow);
				if( !isValidEMail(cBuf) )
					break;
				lEnd = lNow;
				lNow+=1;
			}

			if(lStart <= lOffset && lOffset <= lEnd)
				return TRUE;
			else
				continue;
		}
		else 
		{
			lNow+=1;
		}
	}
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：データ先頭ポインタ取得												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
const unsigned char* CLine::Data()
{
	return (const unsigned char*)(LPCTSTR)m_strLine;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：データ先頭ポインタ取得												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
const unsigned char* CLine::Data(long lOffset)
{
	if(lOffset < 0 || lOffset >= m_strLine.GetLength())
		return NULL;
	return (const unsigned char*)(LPCTSTR)m_strLine + lOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字列化して取得														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
const unsigned char* CLine::String()
{
	return (const unsigned char*)(LPCTSTR)m_strLine;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：部分文字列取得														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
const unsigned char* CLine::SubString(long lOffset)
{
	if(lOffset < 0 || lOffset >= m_strLine.GetLength())
		return NULL;
	m_strBuf = m_strLine.Mid(lOffset,m_strLine.GetLength()-lOffset);
	return (const unsigned char*)(LPCTSTR)m_strBuf;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：色データ列サイズ変更													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::ResizeColorBuffer(long size, unsigned char c)
{
	m_Color.SetSize(size);
	for(long l=0; l<size; l++)
		m_Color.SetAt(l,(CObject*)0);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：アンダーラインデータ列サイズ変更										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::ResizeUnderLineBuffer(long size, unsigned char c)
{
	m_bNeedUnderLine.SetSize(size);
	for(long l=0; l<size; l++)
		m_bNeedUnderLine.SetAt(l,(CObject*)FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：複数の空白文字を１つに												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::Spaces2One()
{
	long nCnt=0;
	unsigned char c1stChar=0, c2ndChar=0;

	for(nCnt=0; nCnt<GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,GetAt(nCnt)))//２バイト文字の第一バイト
		{
			nCnt+=2;
			continue;
		}
		if(GetAt(nCnt) != '\t' && GetAt(nCnt) != ' ')//タブやスペース以外
		{
			nCnt++;
			continue;
		}

		//連続する空白文字を削除
		for(;;)
		{
			c1stChar=0, c2ndChar=0;
			DeleteChar(nCnt,&c1stChar,&c2ndChar);

			if(IsDBCSLeadByteEx(0,GetAt(nCnt)))
				break;
			else if(GetAt(nCnt) != '\t' && GetAt(nCnt) != ' ')
				break;
		}

		if(nCnt >= GetLength())
			break;

		//半角スペースを１つ挿入
		InsertChar(nCnt,' ',0);
		nCnt++;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：英字－＞大文字化														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::Al2Upper()
{
	for(long nCnt=0; nCnt<GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,GetAt(nCnt)))//２バイト文字の第一バイト
		{
			nCnt+=2;
			continue;
		}
		else //半角
		{
			if( isAlSmall(GetAt(nCnt)) ) //英小文字
			{
				//下へ
			}
			else
			{
				nCnt++;
				continue;
			}
		}

		//英小文字取得
		unsigned char cSmallChar = GetAt(nCnt);
		//英小文字を削除
		unsigned char c1stChar=0, c2ndChar=0;
		DeleteChar(nCnt,&c1stChar,&c2ndChar);
		//大文字変換＆挿入
		unsigned char cBigChar = cSmallChar - 0x20;
		InsertChar(nCnt,cBigChar,0);
		nCnt++;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：英数字－＞半角化														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::AlnumZen2Han()
{
	UINT unChar=0;
	long lEndOffsetAfter=GetLength();
	CCodeConvManager	objCodeConvManager;

	for(long nCnt=0; nCnt<lEndOffsetAfter; )
	{
		if(IsDBCSLeadByteEx(0,GetAt(nCnt)))//２バイト文字の第一バイト
		{
			//文字コード取得
			unChar = ( (GetAt(nCnt) << 8) | GetAt(nCnt+1) );
			if( objCodeConvManager.IsZenkakuAlnum(unChar) == TRUE )//全角英数字
			{
				//下へ
			}
			else
			{
				nCnt+=2;
				continue;
			}
		}
		else //半角
		{
			nCnt++;
			continue;
		}

		//全角英数字取得
		UINT nZenChar = unChar;
		//全角英数字を削除
		unsigned char c1stChar=0, c2ndChar=0;
		DeleteChar(nCnt,&c1stChar,&c2ndChar);
		//半角英数字変換＆挿入
		unsigned char cHanChar=0;
		cHanChar = objCodeConvManager.AlnumToHan(nZenChar);
		InsertChar(nCnt,(cHanChar&0x00ff),0);
		nCnt++;
		//終了オフセットを更新
		lEndOffsetAfter--;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：カタカナ－＞半角化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::KatakanaZen2Han()
{
	UINT unChar=0;
	long lEndOffsetAfter=GetLength();
	CCodeConvManager	objCodeConvManager;

	for(long nCnt=0; nCnt<lEndOffsetAfter; )
	{
		if(IsDBCSLeadByteEx(0,GetAt(nCnt)))//２バイト文字の第一バイト
		{
			//文字コード取得
			unChar = ( (GetAt(nCnt) << 8) | GetAt(nCnt+1) );
			if( _ismbckata(unChar) || //全角カタカナ
				unChar==0x814a     || // ゛
				unChar==0x814b     || // ゜
				unChar==0x815b	      // ー
			)
			{
				//下へ
			}
			else
			{
				nCnt+=2;
				continue;
			}
		}
		else //半角
		{
			nCnt++;
			continue;
		}

		//全角カタカナ取得
		UINT nZenKana = unChar;
		//全角カタカナを削除
		unsigned char c1stChar=0, c2ndChar=0;
		DeleteChar(nCnt,&c1stChar,&c2ndChar);
		//半角カタカナ変換＆挿入
		unsigned char cHanKana=0;
		if(_ismbckata(nZenKana))
		{
			if( objCodeConvManager.IsMaruKana(nZenKana) )
			{
				cHanKana = _mbctombb(nZenKana);
				InsertChar(nCnt,(cHanKana&0x00ff),0);
				nCnt++;
				//ﾟ　を追加挿入
				cHanKana = (char)0xdf;
				InsertChar(nCnt,(cHanKana&0x00ff),0);
				nCnt++;
			}
			else if( objCodeConvManager.IsDakuonKana(nZenKana) )
			{
				cHanKana = _mbctombb(nZenKana);
				InsertChar(nCnt,(cHanKana&0x00ff),0);
				nCnt++;
				//ﾞ　を追加挿入
				cHanKana = (char)0xde;
				InsertChar(nCnt,(cHanKana&0x00ff),0);
				nCnt++;
			}
			else
			{
				cHanKana = _mbctombb(nZenKana);
				InsertChar(nCnt,(cHanKana&0x00ff),0);
				nCnt++;
				//終了オフセットを更新
				lEndOffsetAfter--;
			}
		}
		else if(nZenKana==0x814a) // ゛ --->  ﾞ
		{
			cHanKana = (char)0xde;
			InsertChar(nCnt,(cHanKana&0x00ff),0);
			nCnt++;
			//終了オフセットを更新
			lEndOffsetAfter--;
		}
		else if(nZenKana==0x814b) // ゜ --->  ﾟ
		{
			cHanKana = (char)0xdf;
			InsertChar(nCnt,(cHanKana&0x00ff),0);
			nCnt++;
			//終了オフセットを更新
			lEndOffsetAfter--;
		}
		else if(nZenKana==0x815b) // ー --->  ｰ
		{
			cHanKana = (char)0xb0;
			InsertChar(nCnt,(cHanKana&0x00ff),0);
			nCnt++;
			//終了オフセットを更新
			lEndOffsetAfter--;
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：空白－＞半角化														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SpaceZen2Han()
{
	UINT unChar=0;
	long lEndOffsetAfter=GetLength();

	for(long nCnt=0; nCnt<lEndOffsetAfter; )
	{
		if(IsDBCSLeadByteEx(0,GetAt(nCnt)))//２バイト文字の第一バイト
		{
			//文字コード取得
			unChar = ( (GetAt(nCnt) << 8) | GetAt(nCnt+1) );
			if( unChar == 0x8140 )//全角空白
			{
				//下へ
			}
			else
			{
				nCnt+=2;
				continue;
			}
		}
		else //半角
		{
			nCnt++;
			continue;
		}

		//全角空白を削除
		unsigned char c1stChar=0, c2ndChar=0;
		DeleteChar(nCnt,&c1stChar,&c2ndChar);
		//半角空白挿入
		InsertChar(nCnt,' ',0);
		nCnt++;
		//終了オフセットを更新
		lEndOffsetAfter--;
	}
}

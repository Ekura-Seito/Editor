// LineManager.cpp: CLineManager クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxtempl.h>			//for CTypedPtrList
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "StatusBarDisp.h"		//ステータスバー表示情報構造体
#include "Editor.h"					//アプリケーションクラス
#include "BitmapClient.h"
#include "Operation.h"
#include "OperationManager.h"
#include "MainFrm.h"
#include "Line.h"					//１行クラス
#include "CodeConvManager.h"
#include "LineManager.h"

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
CLineManager::CLineManager()
{
	m_pApp = (CEditorApp*)AfxGetApp();

	m_lMarkLineCount = 0;					// マーク行数
	m_lCRCount = 0;
	m_lLFCount = 0;
	m_lKaigyoKind = KAIGYO_CRLF;
	m_lTabSize = m_pApp->GetProfileInt("CLineManager","TabSize",4);//タブサイズ
	m_lMaxLineLength = 0;					//１行の長さの最大値
	m_objStringArray.SetSize(0);				//初期サイズゼロ
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.07.07 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLineManager::~CLineManager()
{
	//設定をレジストリへ保存
	m_pApp->WriteProfileInt("CLineManager","TabSize",m_lTabSize);

	Clear();
}

/////////////////////////////////////////////////////////////////////////////
// CLineManager クラスのパブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：初期化																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::Initialize()
{
	//行クリア
	Clear();

	m_lMarkLineCount = 0;					// マーク行数
	m_lTabSize = m_pApp->GetProfileInt("CLineManager","TabSize",4);//タブサイズ
	m_lMaxLineLength = 0;					//１行の長さの最大値
	m_objStringArray.SetSize(0);				//初期サイズゼロ
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：シリアライズ															|*/
/*|	解説	：																		|*/
/*|	備考	：CArchive exception: endOfFile.の例外がデバッグウインドウに表示される	|*/
/*|	履歴	：1998.07.07 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::Serialize(CArchive& ar)
{
	long lCRCount=0;
	long lLFCount=0;
	long	lCnt=0;
	long	lMaxOffset=0;

	if(ar.IsStoring()) //保存
	{
		for(lCnt=0; lCnt<m_objStringArray.GetSize(); lCnt++)
		{
			CLine* pobjLine = (CLine*)m_objStringArray.GetAt(lCnt);
			ar.WriteString((const char*)pobjLine->String());

			if(lCnt < m_objStringArray.GetSize() - 1)
			{
				if(m_lKaigyoKind == KAIGYO_CRLF)
					ar.WriteString("\r\n");
				else if(m_lKaigyoKind == KAIGYO_LF)
					ar.WriteString("\n");
				else if(m_lKaigyoKind == KAIGYO_CR)
					ar.WriteString("\r");
				else
					ar.WriteString("\r\n");
			}
		}
	}
	else //読み出し
	{
		//初期化
		Clear();

		int  nNowRet=0;
		int  nBeforeRet=0;
		CLine* pobjLine=NULL;
		m_pApp->PrepareMyReadString();
		for(lCnt=0; ;lCnt++)//１行づつ読み込み
		{
			lCRCount=0;
			lLFCount=0;
			CString objStr = _T("");
			nNowRet = m_pApp->MyReadString(ar,objStr,&lCRCount,&lLFCount);
			if( nNowRet == 0 ) //EOF終了
			{
				if( objStr.IsEmpty() ) //データなし
					break;
			}

			//MyReadString リターン値保持
			nBeforeRet = nNowRet;

			if(m_lCRCount==0 && m_lLFCount==0) //CR, LF の数保存
			{
				m_lCRCount = lCRCount;
				m_lLFCount = lLFCount;

				if(lCRCount==1 && lLFCount==1)
					m_lKaigyoKind=KAIGYO_CRLF;
				else if(lCRCount==0 && lLFCount==1)
					m_lKaigyoKind=KAIGYO_LF;
				else if(lCRCount==1 && lLFCount==0)
					m_lKaigyoKind=KAIGYO_CR;
				else
					m_lKaigyoKind=KAIGYO_CRLF;
			}

			if( lCnt==0 ) //１行目の読み込み
			{
				pobjLine = new CLine((const unsigned char*)(LPCTSTR)objStr);
				m_objStringArray.Add((CObject*)pobjLine);
			}
			else //２行目以降
			{
				//最終行にセット
				pobjLine = (CLine*)m_objStringArray.GetAt(m_objStringArray.GetSize()-1);
				pobjLine->Set((const unsigned char*)(LPCTSTR)objStr);
			}

			//１行の最大オフセット計算
			long lTabCnt = pobjLine->GetTabCount();
			lMaxOffset = objStr.GetLength() - lTabCnt + (lTabCnt*GetTabSize());
			if(m_lMaxLineLength < lMaxOffset)
				m_lMaxLineLength = lMaxOffset;

			if(nNowRet == 1) //改行終了していた場合
			{
				//次の行の分登録
				pobjLine = new CLine();
				m_objStringArray.Add((CObject*)pobjLine);
			}
			else //ＥＯＦ終了していた場合
			{
				break;
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：シリアライズ（バイナリモード）										|*/
/*|	解説	：																		|*/
/*|	備考	：CArchive exception: endOfFile.の例外がデバッグウインドウに表示される	|*/
/*|	履歴	：                   													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::SerializeBinary(CArchive& ar)
{
	long	lCnt=0;
	long	lSumCnt=0;
	UINT	unReaded=0;
	long	lOffset=0;
	BYTE	byBuf[32];
	CLine*  pobjLine=NULL;
	char	szAsciiDataBuf[24];
	char	szAsciiData[128];
	strcpy(szAsciiData,ASCII_DATA);

	if(ar.IsStoring()) //保存は未サポート
		return;

	//初期化
	Clear();

	//書込み禁止ﾓｰﾄﾞｽﾃｰﾀｽﾍﾟｲﾝ設定
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	pWnd->SetReadOnlyMode(TRUE);

	//ゲージ書き込み
	CLine objBinaryLine;
	objBinaryLine.Set((const unsigned char*)"");
	AppendBinaryLine(objBinaryLine);
	objBinaryLine.Set((const unsigned char*)"| Offset | 0001 0203 0405 0607 0809 0A0B 0C0D 0E0F |      ASCII DATA");
	AppendBinaryLine(objBinaryLine);

	objBinaryLine.Set((const unsigned char*)"");
	memset(szAsciiDataBuf,0,sizeof(szAsciiDataBuf));
	lSumCnt=0;
	for(;;)
	{
		TRY
		{
			//１６バイトずつ読み込み
			memset(byBuf,0,sizeof(byBuf));
			unReaded = ar.Read(byBuf,16);
			if(unReaded <= 0)
				break;
		}
		CATCH_ALL(e)
		{
			break;
		}
		END_CATCH_ALL

		for(lOffset=0; lOffset<(signed)unReaded; lOffset++, lSumCnt++)
		{
			if(!(lSumCnt%256))
			{
				if(objBinaryLine.GetLength()!=0)
				{
					//ｱｽｷｰﾃﾞｰﾀ書込み
					objBinaryLine.Catenate((const unsigned char*)"     ");
					objBinaryLine.Catenate((const unsigned char*)szAsciiDataBuf);
					memset(szAsciiDataBuf,0,sizeof(szAsciiDataBuf));
					//いままでのバイナリ表示結果を登録
					AppendBinaryLine(objBinaryLine);
				}
				//セパレータ登録
				CLine objTempLine((const unsigned char*)"|--------|-----------------------------------------|");
				AppendBinaryLine(objTempLine);
			}
			if((lSumCnt!=0)&&(!(lSumCnt%2)))
			{
				objBinaryLine.Catenate((const unsigned char*)" ");
			}
			if((!(lSumCnt%16)))
			{
				if( (lSumCnt%256)!=0 )
				{
					if(objBinaryLine.GetLength()!=0)
					{
						//ｱｽｷｰﾃﾞｰﾀ書込み
						objBinaryLine.Catenate((const unsigned char*)"    ");
						objBinaryLine.Catenate((const unsigned char*)szAsciiDataBuf);
						memset(szAsciiDataBuf,0,sizeof(szAsciiDataBuf));
						//いままでのバイナリ表示結果を登録
						AppendBinaryLine(objBinaryLine);
					}
				}
				//クリア
				objBinaryLine.Set((const unsigned char*)"");
				char szBuf[64];
				sprintf(szBuf,"| %06X | ",lSumCnt);
				objBinaryLine.Set((const unsigned char*)szBuf);
			}
			char szBuf[64];
			sprintf(szBuf,"%02X",byBuf[lOffset]);
			objBinaryLine.Catenate((const unsigned char*)szBuf);

			//////////////////////////
			//	ＡＳＣＩＩでの表示	//
			//////////////////////////
			if(byBuf[lOffset] <= 0x7f)
				szAsciiDataBuf[(lSumCnt%16)] = szAsciiData[byBuf[lOffset]];
			else
				szAsciiDataBuf[(lSumCnt%16)] = '.';
		}
	}
	if(objBinaryLine.GetLength()!=0)
	{
		//ｱｽｷｰﾃﾞｰﾀ書込み
		objBinaryLine.PadSpace(55);
		objBinaryLine.Catenate((const unsigned char*)szAsciiDataBuf);
		memset(szAsciiDataBuf,0,sizeof(szAsciiDataBuf));
		//いままでのバイナリ表示結果を登録
		AppendBinaryLine(objBinaryLine);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：バイト数取得処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::GetBytes
	(	long	lLimitLine,				//指定行
		long*	plLimitBytes,			//指定行までのバイト数返却用バッファ
		long*	plSumBytes				//総バイト数返却用バッファ
	)
{
	long	lCnt=0;
	CLine*	pobjLine=NULL;

	//初期化
	(*plLimitBytes) = 0;
	(*plSumBytes) = 0;

	for(lCnt=0; lCnt<GetLineCount(); lCnt++)
	{
		pobjLine = GetOneLine(lCnt);
		if(pobjLine==NULL)
			continue;

		//総バイト数
		(*plSumBytes) += pobjLine->GetLength();
		if(lCnt < GetLineCount()-1)
		{
			if(m_lKaigyoKind == KAIGYO_CRLF)
				(*plSumBytes) += 2;
			else if(m_lKaigyoKind == KAIGYO_LF)
				(*plSumBytes) += 1;
			else if(m_lKaigyoKind == KAIGYO_CR)
				(*plSumBytes) += 1;
			else
				(*plSumBytes) += 2;
		}

		//指定行までのバイト数
		if(lCnt < lLimitLine)
			(*plLimitBytes) += pobjLine->GetLength();
		if(lCnt < lLimitLine)
		{
			if(m_lKaigyoKind == KAIGYO_CRLF)
				(*plLimitBytes) += 2;
			else if(m_lKaigyoKind == KAIGYO_LF)
				(*plLimitBytes) += 1;
			else if(m_lKaigyoKind == KAIGYO_CR)
				(*plLimitBytes) += 1;
			else
				(*plLimitBytes) += 2;
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行数取得処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLineManager::GetLineCount()
{
	return m_objStringArray.GetSize();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１行取得処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLine* CLineManager::GetOneLine
	(	long		lLineNo		//行番号
	)
{
	if(!(lLineNo < m_objStringArray.GetSize()))	//ない
	{
		return NULL;
	}
	else	//ある
	{
		CLine* pobjLine = (CLine*)m_objStringArray.GetAt(lLineNo);
		//現在の文字列長保持(1999.03.21 Add for StatusBar Pane ID_INDICATOR_BYTE)
		pobjLine->SaveLength();
		return pobjLine;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１行設定処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::SetOneLine
	(	long		lLineNo,		//行番号
		CLine*		pobjLine		//行
	)
{
	if( pobjLine==NULL)
		return;

	//設定行の行末のオフセット計算
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());

	//１行の長さの最大値を更新
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;

	//１行設定
	m_objStringArray.SetAt(lLineNo,(CObject*)pobjLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：指定行削除処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::DeleteOneLine
	(	long	lLineNo			//削除対象行
	)
{
	CLine* pobjLine = NULL;
	pobjLine = (CLine*)m_objStringArray.GetAt(lLineNo);
	if(pobjLine != NULL)
	{
		delete pobjLine;
		m_objStringArray.RemoveAt(lLineNo);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行挿入処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::InsertOneLine
	(	long	lLineNo,
		CLine*	pobjLine
	)
{
	if( pobjLine==NULL)
		return;

	//挿入行の行末のオフセット計算
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());

	//１行の長さの最大値を更新
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;

	//１行挿入
	m_objStringArray.InsertAt(lLineNo,(CObject*)pobjLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１行の長さの最大値を取得処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLineManager::GetMaxLineLength()
{
	return m_lMaxLineLength;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：指定行の文字列長取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLineManager::GetLineLength(long lLineNo)
{
	if(lLineNo >= m_objStringArray.GetSize())
		return 0;

	CLine* pobjLine = (CLine*)m_objStringArray.GetAt(lLineNo);
	return pobjLine->GetLength();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字登録処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AddCharacter
	(	long	lLine,					//文字登録対象の行番号（ｾﾞﾛｵﾘｼﾞﾝ）
		long	lOffset,				//文字登録対象の列番号（ﾊﾞｲﾄ単位、ｾﾞﾛｵﾘｼﾞﾝ）
		UINT	n1stChar,				//登録文字第一ﾊﾞｲﾄ
		UINT	n2ndChar,				//登録文字第二ﾊﾞｲﾄ
		BOOL	bInsertKey				//挿入／上書きﾓｰﾄﾞﾌﾗｸﾞ(TRUE:挿入、FALSE:上書き)
	)
{
	CLine*	pobjLine = NULL;

	//////////////////////////////
	//	登録対象行取得
	//////////////////////////////
	if(lLine >= GetLineCount())//指定された行にﾃﾞｰﾀがまだない
		pobjLine = new CLine();
	else //指定された行にﾃﾞｰﾀが既に有る
		pobjLine = (CLine*)m_objStringArray.GetAt(lLine);

	if(lOffset > pobjLine->GetLength()) //文字登録場所が文字最後尾より後ろ
		bInsertKey = TRUE;//挿入にする

	//////////////////////////////////////
	//	文字登録
	//////////////////////////////////////
	if(bInsertKey)//挿入ﾓｰﾄﾞ
		pobjLine->InsertChar(lOffset,n1stChar,n2ndChar);
	else //上書きﾓｰﾄﾞ
		pobjLine->OverWriteChar(lOffset,n1stChar,n2ndChar);

	//////////////////////////////
	//	行を再登録
	//////////////////////////////
	if(lLine >= GetLineCount())//指定された行にﾃﾞｰﾀがまだない
		m_objStringArray.Add((CObject*)pobjLine);
	else //指定された行にﾃﾞｰﾀが既に有る
		m_objStringArray.SetAt(lLine,(CObject*)pobjLine);

	//////////////////////////////////////////
	//	１行の長さの最大値を更新
	//////////////////////////////////////////

	//行末のオフセット計算
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());

	//１行の長さの最大値を更新
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：改行コード処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::ProcessReturnCode
	(	long	lLine,				//文字登録対象の行番号
		long	lOffset				//文字登録対象の列番号（ﾊﾞｲﾄ単位）
	)
{
	CLine*	pobjLine = NULL;

	if(lLine >= GetLineCount())//指定された行にﾃﾞｰﾀがまだない
	{
		pobjLine = new CLine();
		m_objStringArray.Add((CObject*)pobjLine);
		pobjLine = new CLine();
		m_objStringArray.Add((CObject*)pobjLine);
	}
	else //指定された行にﾃﾞｰﾀがある
	{
		pobjLine = (CLine*)m_objStringArray.GetAt(lLine);
		if(lOffset >= pobjLine->GetLength())//行末での改行
		{
			pobjLine = new CLine();
			//次の行に挿入
			m_objStringArray.InsertAt(lLine+1,(CObject*)pobjLine);
		}
		else //行の途中での改行
		{
			//キャレット位置より後ろを保持
			CLine* pobjNextLine = new CLine();
			pobjNextLine->Set(pobjLine->Data(lOffset));
			//キャレット位置で１行区切る
			pobjLine->SetAt(lOffset,0);
			//改行によって切り出された行を次の行とする
			m_objStringArray.InsertAt(lLine+1,(CObject*)pobjNextLine);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：DELETEコード処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::ProcessDeleteCode
	(	long	lLine,					//文字登録対象の行番号
		long	lOffset,				//文字登録対象の列番号（ﾊﾞｲﾄ単位）
		unsigned char*	pc1stChar,		//DELETEされた第一バイト文字(OUT)
		unsigned char*	pc2ndChar		//DELETEされた第二バイト文字(OUT)
	)
{
	CLine*	pobjLine = NULL;

	if(lLine >= GetLineCount())//指定された行にﾃﾞｰﾀがまだない
	{
		//特に何もしない
		(*pc1stChar) = 0;
		(*pc2ndChar) = 0;
		return;
	}

	//指定された行にﾃﾞｰﾀがある
	pobjLine = (CLine*)m_objStringArray.GetAt(lLine);
	if(lOffset >= pobjLine->GetLength())//行末でのDELETE
	{
		if((lLine+1) >= GetLineCount())//次の行なし
		{
			//特に何もしない
			(*pc1stChar) = 0;
			(*pc2ndChar) = 0;
			return;
		}
		else //次の行あり
		{
			CLine* pobjNextLine = (CLine*)m_objStringArray.GetAt(lLine+1);
			//現在行に次行を連結
			pobjLine->Catenate(pobjNextLine->String());
			//次行を削除
			m_objStringArray.RemoveAt(lLine+1);
			delete pobjNextLine;
			//改行コードのDELETEに相当する
			(*pc1stChar) = '\r';
			(*pc2ndChar) = '\n';

			//行末のオフセット計算
			long lTabCnt = pobjLine->GetTabCount();
			long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
			//１行の長さの最大値を更新
			if(m_lMaxLineLength < lMaxOffset)
				m_lMaxLineLength = lMaxOffset;

			return;
		}
	}
	else //行の途中でのDELETE
	{
		//文字削除
		pobjLine->DeleteChar(lOffset,pc1stChar,pc2ndChar);
		return;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：BACK SPACEコード処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::ProcessBackSpaceCode
	(	long	lLine,				//文字登録対象の行番号
		long	lOffset,			//文字登録対象の列番号（ﾊﾞｲﾄ単位）
		unsigned char*	pc1stChar,	//BACK SPACEされた第一バイト文字(OUT)
		unsigned char*	pc2ndChar	//BACK SPACEされた第二バイト文字(OUT)
	)
{
	CLine*	pobjLine = NULL;

	if(lLine >= GetLineCount())//指定された行にﾃﾞｰﾀがまだない
	{
		//特に何もしない
		(*pc1stChar) = 0;
		(*pc2ndChar) = 0;
		return;
	}

	//指定された行にﾃﾞｰﾀがある
	CLine* pobjPrevLine = NULL;
	pobjLine = (CLine*)m_objStringArray.GetAt(lLine);
	if(lOffset == 0)//行頭でのBACK SPACE
	{
		if(lLine == 0)//前の行なし
		{
			//特に何もしない
			(*pc1stChar) = 0;
			(*pc2ndChar) = 0;
			return;
		}
		else //前の行あり
		{
			pobjPrevLine = (CLine*)m_objStringArray.GetAt(lLine-1);
			//前の行に現在行を連結
			pobjPrevLine->Catenate(pobjLine->String());
			//現在行を削除
			m_objStringArray.RemoveAt(lLine);
			delete pobjLine;
			//改行コードのBACK SPACEに相当する
			(*pc1stChar) = '\r';
			(*pc2ndChar) = '\n';

			//行末のオフセット計算
			long lTabCnt = pobjPrevLine->GetTabCount();
			long lMaxOffset = pobjPrevLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
			//１行の長さの最大値を更新
			if(m_lMaxLineLength < lMaxOffset)
				m_lMaxLineLength = lMaxOffset;

			return;
		}
	}
	else //行の途中でのBACK SPACE
	{
		//文字削除
		pobjLine->BackSpaceChar(lOffset,pc1stChar,pc2ndChar);
		return;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：タブサイズ取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLineManager::GetTabSize()
{
	return m_lTabSize;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：タブサイズ設定処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::SetTabSize(long lTabSize)
{
	m_lTabSize = lTabSize;

	//設定をレジストリへ保存
	m_pApp->WriteProfileInt("CLineManager","TabSize",m_lTabSize);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：選択範囲テキスト取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLine* CLineManager::MyGetSelectedText
	(	long	lSelectStartLine,			//選択開始行番号
		long	lSelectStartOffset,			//選択開始列番号
		long	lSelectEndLine,				//選択終了行番号
		long	lSelectEndOffset			//選択終了列番号
	)
{
	CLine* pobjLine = new CLine();

	if( lSelectStartLine == lSelectEndLine ) //選択行が１行のみ
	{
		//選択対象行取得
		CLine* pobjTempLine = (CLine*)m_objStringArray.GetAt(lSelectStartLine);
		ASSERT(pobjTempLine != NULL);
		pobjLine->Set(pobjTempLine->Data(lSelectStartOffset));
		pobjLine->SetAt((lSelectEndOffset-lSelectStartOffset),0);
		return pobjLine;
	}
	else //選択行が複数行にわたる
	{
		for(long lLineNo=lSelectStartLine; lLineNo<=lSelectEndLine; lLineNo++)
		{
			//選択対象行取得
			CLine* pobjTempLine = (CLine*)GetOneLine(lLineNo);
			if(!pobjTempLine)
				continue;

			if(lLineNo == lSelectStartLine)//選択開始行
			{
				pobjLine->Set(pobjTempLine->Data(lSelectStartOffset));
			}
			else if(lLineNo == lSelectEndLine)//選択終了行
			{
				pobjLine->Catenate((const unsigned char*)"\r\n");
				CLine* pobjTemp2Line = new CLine();
				pobjTemp2Line->Set(pobjTempLine->String());
				pobjTemp2Line->SetAt(lSelectEndOffset,0);
				pobjLine->Catenate(pobjTemp2Line->String());
				delete pobjTemp2Line;
			}
			else //途中行
			{
				pobjLine->Catenate((const unsigned char*)"\r\n");
				pobjLine->Catenate(pobjTempLine->String());
			}
		}

		return pobjLine;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＧＲＥＰ結果１行追加処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendGrepLine
	(	CLine&	objGrepLine				// ＧＲＥＰ結果１行
	)
{
	CLine* pobjLine = new CLine(objGrepLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//１行の最大オフセット計算
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＨＴＴＰダウンロード結果１行追加										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendHttpLine
	(	CLine&	objHttpLine				// 結果１行
	)
{
	CLine* pobjLine = new CLine(objHttpLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//１行の最大オフセット計算
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字コード変換結果１行追加処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendConvLine
	(	CLine&	objConvLine				// 文字コード変換結果１行
	)
{
	CLine* pobjLine = new CLine(objConvLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//１行の最大オフセット計算
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダンプ結果１行追加処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendDumpLine
	(	CLine&	objDumpLine				// ダンプ結果１行
	)
{
	CLine* pobjLine = new CLine(objDumpLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//１行の最大オフセット計算
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ワードカウント結果１行追加処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendWCountLine
	(	CLine&	objWCountLine			// ワードカウント結果１行
	)
{
	CLine* pobjLine = new CLine(objWCountLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//１行の最大オフセット計算
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル検索結果１行追加処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendFSearchLine
	(	CLine&	objFSearchLine			// ファイル検索結果１行
	)
{
	CLine* pobjLine = new CLine(objFSearchLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//１行の最大オフセット計算
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：バイナリ表示１行追加処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendBinaryLine
	(	CLine&	objBinaryLine
	)
{
	CLine* pobjLine = new CLine(objBinaryLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//１行の最大オフセット計算
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル暗号化結果１行追加処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendEncriptLine
	(	CLine&	objEncriptLine
	)
{
	CLine* pobjLine = new CLine(objEncriptLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//１行の最大オフセット計算
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル復号化結果１行追加処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendDecriptLine
	(	CLine&	objDecriptLine
	)
{
	CLine* pobjLine = new CLine(objDecriptLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//１行の最大オフセット計算
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：エンコード結果１行追加処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendEncodeLine
	(	CLine&	objEncodeLine
	)
{
	CLine* pobjLine = new CLine(objEncodeLine.String());
	m_objStringArray.Add((CObject*)pobjLine);

	//１行の最大オフセット計算
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル比較結果１行追加処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::AppendCompareLine
	(	CLine*	pobjCompareLine
	)
{
	CLine* pobjLine = new CLine(pobjCompareLine->String());
	//ファイル比較用、行種別を引き継ぐ
	pobjLine->SetDiffLineKind(pobjCompareLine->GetDiffLineKind());
	m_objStringArray.Add((CObject*)pobjLine);

	//１行の最大オフセット計算
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル比較結果１行挿入処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::InsertCompareLine
	(	long	lLine,
		CLine*	pobjCompareLine
	)
{
	CLine* pobjLine = new CLine(pobjCompareLine->String());
	//ファイル比較用、行種別を引き継ぐ
	pobjLine->SetDiffLineKind(pobjCompareLine->GetDiffLineKind());
	m_objStringArray.InsertAt(lLine,(CObject*)pobjLine);

	//１行の最大オフセット計算
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル比較用行種別取得												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CLineManager::GetDiffLineKind
	(	long	lLine
	)
{
	if(lLine >= m_objStringArray.GetSize())
		return DIFF_LINEKIND_COMMON;

	CLine* pobjLine = NULL;
	pobjLine = (CLine*)m_objStringArray.GetAt(lLine);
	if(pobjLine != NULL)
		return pobjLine->GetDiffLineKind();
	else
		return DIFF_LINEKIND_COMMON;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ比較結果１行挿入処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::InsertDirCmpLine
	(	long	lLine,
		CLine*	pobjDirCmpLine
	)
{
	CLine* pobjLine = new CLine(pobjDirCmpLine->String());
	//ディレクトリ比較用、行種別を引き継ぐ
	pobjLine->SetDirCmpLineKind(pobjDirCmpLine->GetDirCmpLineKind());
	//ディレクトリ比較用、ファイル名を引き継ぐ
	CString objStr=_T("");
	pobjDirCmpLine->GetDirCmpFileName(objStr);
	pobjLine->SetDirCmpFileName(objStr);
	//登録
	m_objStringArray.InsertAt(lLine,(CObject*)pobjLine);

	//１行の最大オフセット計算
	long lTabCnt = pobjLine->GetTabCount();
	long lMaxOffset = pobjLine->GetLength() - lTabCnt + (lTabCnt*GetTabSize());
	if(m_lMaxLineLength < lMaxOffset)
		m_lMaxLineLength = lMaxOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ比較用相違種別取得										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CLineManager::GetDirCmpLineKind
	(	long	lLine
	)
{
	if(lLine >= m_objStringArray.GetSize())
		return DIRCMP_LINEKIND_NORMAL;

	CLine* pobjLine = NULL;
	pobjLine = (CLine*)m_objStringArray.GetAt(lLine);
	if(pobjLine != NULL)
		return pobjLine->GetDirCmpLineKind();
	else
		return DIRCMP_LINEKIND_NORMAL;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＣＲ、ＬＦの数取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::GetCRLFCount
	(	long*	plCRCount,		//CRの数(out)
		long*	plLFCount		//LFの数(out)
	)
{
	(*plCRCount) = m_lCRCount;
	(*plLFCount) = m_lLFCount;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：改行コード区分取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLineManager::GetKaigyoKind()
{
	return m_lKaigyoKind;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：改行コード区分設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::SetKaigyoKind
	(	long	lKaigyoKind
	)
{
	if( m_lKaigyoKind == lKaigyoKind )
		return;

	if( lKaigyoKind==KAIGYO_CRLF || lKaigyoKind==KAIGYO_LF || lKaigyoKind==KAIGYO_CR)
		m_lKaigyoKind = lKaigyoKind;

	if( m_lKaigyoKind==KAIGYO_CRLF )
	{
		m_lCRCount = 1;
		m_lLFCount = 1;
	}
	else if( m_lKaigyoKind==KAIGYO_LF )
	{
		m_lCRCount = 0;
		m_lLFCount = 1;
	}
	else if( m_lKaigyoKind==KAIGYO_CR )
	{
		m_lCRCount = 1;
		m_lLFCount = 0;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：マークフラグ設定処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::SetFlagMark
	(	long	lLine,
		BOOL	bMark
	)
{
	if(lLine >= m_objStringArray.GetSize())
		return;

	CLine* pobjLine = NULL;
	pobjLine = (CLine*)m_objStringArray.GetAt(lLine);
	if(pobjLine != NULL)
	{
		if(bMark)
			m_lMarkLineCount++;
		else
			m_lMarkLineCount--;

		pobjLine->SetFlagMark(bMark);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：マークフラグ取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CLineManager::GetFlagMark
	(	long	lLine
	)
{
	if(lLine >= m_objStringArray.GetSize())
		return FALSE;

	CLine* pobjLine = NULL;
	pobjLine = (CLine*)m_objStringArray.GetAt(lLine);
	if(pobjLine != NULL)
		return pobjLine->GetFlagMark();
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：マーク行数取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLineManager::GetMarkLineCount()
{
	return m_lMarkLineCount;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：指定行の行末空白削除処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::DelEndSpaces
	(	long	lLine					// 行番号
	)
{
	if(lLine < 0)
		return;
	if(lLine >= m_objStringArray.GetSize())
		return;

	CLine* pobjLine = NULL;
	pobjLine = (CLine*)m_objStringArray.GetAt(lLine);
	if(pobjLine != NULL)
	{
		pobjLine->DelEndSpaces();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：保存時用スペース－＞タブ変換処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::ConvSpace2TabOnSave
	(	long	lfWidth,		//フォント横幅
		long	lTabSize		//タブサイズ
	)
{
	long lOffset=0;
	long lCnt=0;
	long lLine=0;
	CLine* pobjLine=NULL;
	long lEndOffsetAfter=0;

	for(lLine=0; lLine<GetLineCount(); lLine++)
	{
		lOffset=0;
		lCnt=0;

		pobjLine = NULL;
		pobjLine = GetOneLine(lLine);
		if( pobjLine == NULL )
			continue;

		lEndOffsetAfter = pobjLine->GetLength();
		for(lCnt=0; lCnt<lEndOffsetAfter; )
		{
			if(IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)))//２バイト文字の第一バイト
			{
				lCnt+=2;
				lOffset += (lfWidth * 2);
				continue;
			}
			if(pobjLine->GetAt(lCnt) == '\t')//タブコード
			{
				//マージン計算
				int nMargin = lTabSize - ( (lOffset/lfWidth) % lTabSize );
				lOffset += (lfWidth * nMargin);
				lCnt++;
				continue;
			}
			else if(pobjLine->GetAt(lCnt) != ' ')//半角スペース以外
			{
				lCnt++;
				lOffset += lfWidth;
				continue;
			}

			//マージンを計算
			int nMargin = lTabSize - ( (lOffset/lfWidth) % lTabSize );
			//スペース数をカウント
			int nSpaceCnt=0;
			for(int i=0; ; )
			{
				if(IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt+i)))
					break;
				else if(pobjLine->GetAt(lCnt+i) == ' ') //半角スペース継続
				{
					i++;
					nSpaceCnt++;
				}
				else
					break;
			}
			//タブに置き換えられる分だけスペースがあるかどうかチェック
			if(nSpaceCnt < nMargin) //ない！！
			{
				lCnt++;
				lOffset += lfWidth;
				continue;
			}

			//マージンの分だけ半角スペースを削除
			unsigned char c1stChar=0, c2ndChar=0;
			for(int i=0; i<nMargin; i++)
				pobjLine->DeleteChar(lCnt,&c1stChar,&c2ndChar);
			//タブを挿入
			pobjLine->InsertChar(lCnt,VK_TAB,0);

			lOffset += (lfWidth * nMargin);
			lCnt++;
			//終了オフセットを更新
			lEndOffsetAfter-=nMargin;
			lEndOffsetAfter++;
		}

		//再設定(1999.03.20 Add for StatusBar Pane ID_INDICATOR_BYTE)
		SetOneLine(lLine,pobjLine);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CLineManager クラスのプロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ドキュメントの行単位のリストを初期化									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：1998.07.07 S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLineManager::Clear()
{
	m_lMarkLineCount = 0;					// マーク行数
	m_lMaxLineLength = 0;					// １行の長さの最大値

	for(int nCnt=0; nCnt<m_objStringArray.GetSize(); nCnt++)
	{
		CLine* pobjLine = (CLine*)m_objStringArray.GetAt(nCnt);
		delete pobjLine;
	}
	m_objStringArray.RemoveAll();
}


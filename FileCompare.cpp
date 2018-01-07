// FileCompare.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include <map>					// for map
using namespace std;
#include "Editor.h"
#include "Editor_Def.h"
#include "Line.h"
#include "LineManager.h"
#include "UndoItem.h"
#include "UndoManager.h"
#include "ColorManager.h"
#include "EditorDoc.h"
#include "StaticFilespec.h"
#include "FileCompareStatusDlg.h"
#include "FileCompare.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileCompare クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFileCompare::CFileCompare()
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_pobjFileCompareStatusDlg = NULL;
	m_pobjCompareDoc = NULL;
	m_objStringArray1.SetSize(0);
	m_objStringArray2.SetSize(0);
	m_lFileLineCount1 = 0;
	m_lFileLineCount2 = 0;
	m_lDelLineCount = 0;
	m_lAddLineCount = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFileCompare::~CFileCompare()
{
	Clear();
}

/////////////////////////////////////////////////////////////////////////////
// CFileCompare パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル比較実行														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileCompare::DoCompare			//ＴＲＵＥ：正常終了、ＦＡＬＳＥ：異常終了
	(	CFileCompareStatusDlg* pobjFileCompareStatusDlg,
		CEditorDoc* pobjCompareDoc,		// ドキュメントクラス
		CArchive&	ar1,				// 比較第一ファイルアーカイブ
		CArchive&	ar2					// 比較第二ファイルアーカイブ
	)
{
	BOOL	bRet=FALSE;
	LONG	lCnt=0;
	CLine*	pobjLine=NULL;
	CString objMsg;

	//パラメータ保持
	m_pobjFileCompareStatusDlg = pobjFileCompareStatusDlg;
	m_pobjCompareDoc = pobjCompareDoc;

	//ファイル読み込み
	objMsg.LoadString(IDS_FILECOMPARE_READ1FILE);
	m_pobjFileCompareStatusDlg->SetStatus(objMsg);
	if( ReadFile(ar1,&m_objStringArray1) == FALSE )
		return FALSE;
	objMsg.LoadString(IDS_FILECOMPARE_READ2FILE);
	m_pobjFileCompareStatusDlg->SetStatus(objMsg);
	if( ReadFile(ar2,&m_objStringArray2) == FALSE )
		return FALSE;
	m_lFileLineCount1 = m_objStringArray1.GetSize();
	m_lFileLineCount2 = m_objStringArray2.GetSize();
	if(m_lFileLineCount1 == 0)
	{
		objMsg.LoadString(IDS_1STZERO_FILECMP);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	if(m_lFileLineCount2 == 0)
	{
		objMsg.LoadString(IDS_2NDZERO_FILECMP);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//最長共通部分（ＬＣＳ : Longest Common Subsequence）探索
	objMsg.LoadString(IDS_FILECOMPARE_COMPFILE);
	m_pobjFileCompareStatusDlg->SetStatus(objMsg);
	if( SearchLCS() == FALSE )
		return FALSE;

	//結果出力
	objMsg.LoadString(IDS_FILECOMPARE_OUTRESULT);
	m_pobjFileCompareStatusDlg->SetStatus(objMsg);
	if( OutputResult() == FALSE )
		return FALSE;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：追加行数、削除行数取得												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileCompare::GetDelAddLineCount
	(	long*	plDelLineCount,
		long*	plAddLineCount
	)
{
	(*plDelLineCount) = m_lDelLineCount;
	(*plAddLineCount) = m_lAddLineCount;
}

/////////////////////////////////////////////////////////////////////////////
// CFileCompare クラスのプロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル読み込み														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileCompare::ReadFile			//ＴＲＵＥ：正常終了、ＦＡＬＳＥ：異常、キャンセル終了
	(	CArchive&	ar,
		CObArray*	pobjObArray
	)
{
	char* 		pPtr=NULL;
	long 		lCRCount=0;
	long 		lLFCount=0;
	long 		lCnt=0;
	int  		nNowRet=0;
	int  		nBeforeRet=0;
	CLine* 		pobjLine=NULL;
	CString 	objStr = _T("");

	m_pApp->PrepareMyReadString();
	for(lCnt=0; ;lCnt++)//１行づつ読み込み
	{
		m_pApp->MessagePump();
		if(m_pobjFileCompareStatusDlg->IsCanceled())
			return FALSE;

		objStr = _T("");
		nNowRet = m_pApp->MyReadString(ar,objStr,&lCRCount,&lLFCount);
		if( nNowRet == 0 ) //EOF終了
		{
			if( objStr.IsEmpty() ) //データなし
				break;
		}

		//MyReadString リターン値保持
		nBeforeRet = nNowRet;

		if( lCnt==0 ) //１行目の読み込み
		{
			pobjLine = new CLine((const unsigned char*)(LPCTSTR)objStr);
			pobjObArray->Add((CObject*)pobjLine);
		}
		else //２行目以降
		{
			//最終行にセット
			pobjLine = (CLine*)pobjObArray->GetAt(pobjObArray->GetSize()-1);
			pobjLine->Set((const unsigned char*)(LPCTSTR)objStr);
		}

		if(nNowRet == 1) //改行終了していた場合
		{
			//次の行の分登録
			pobjLine = new CLine();
			pobjObArray->Add((CObject*)pobjLine);
		}
		else //ＥＯＦ終了していた場合
		{
			break;
		}
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：最長共通部分（ＬＣＳ : Longest Common Subsequence）探索				|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileCompare::SearchLCS()		//ＴＲＵＥ：正常終了、ＦＡＬＳＥ：異常、キャンセル終了
{
	LONG	lPreX=0;
	LONG	lPreY=0;
	LONG	lNowX=0;
	LONG	lNowY=0;
	LONG	lPreNode=0;
	LONG	lNowNode=0;
	LONG	lPreK=0;
	LONG	lX=0;
	LONG	lY=0;
	LONG	lD=0;
	LONG	lK=0;				// 対角線識別番号（y-x : -M～N）
	const LONG lOffset=m_lFileLineCount1;
	CLine*	pobjLine1=NULL;
	CLine*	pobjLine2=NULL;
	CLine	objLine1;
	CLine	objLine2;
	LONG	lFarY_KPLUS1=0;
	LONG	lFarY_KMINUS1=0;
	BOOL	bIsAssumeTabEqual2Space = m_pApp->GetProfileInt("CFileCompare","bIsAssumeTabEqual2Space",FALSE);
	BOOL	bIsAssumeAlphabetBigAndSmall = m_pApp->GetProfileInt("CFileCompare","bIsAssumeAlphabetBigAndSmall",FALSE);
	BOOL	bIsAssumeZenEqual2Han = m_pApp->GetProfileInt("CFileCompare","bIsAssumeZenEqual2Han",FALSE);

	for(lD=0; lD<=m_lFileLineCount1+m_lFileLineCount2; lD++) //  各Ｄについて
	{
		m_pApp->MessagePump();
		if(m_pobjFileCompareStatusDlg->IsCanceled())
			return FALSE;

		for(lK=-lD; lK<=lD; lK+=2) 
		{
			m_pApp->MessagePump();
			if(m_pobjFileCompareStatusDlg->IsCanceled())
				return FALSE;

			//lK-1 or lK+1の最遠点のｘ、ｙ座標
			lFarY_KPLUS1 = GetValueFarY(lK+1+lOffset);
			lFarY_KMINUS1 = GetValueFarY(lK-1+lOffset);
			if( lK==-lD || lK!=lD && lFarY_KMINUS1 < lFarY_KPLUS1 )
			{
				lPreK = lK+1;
				lPreY = lFarY_KPLUS1;
				lPreX = lPreY - lPreK;
				lY = lFarY_KPLUS1;
			}
			else
			{
				lPreK = lK-1;
				lPreY = lFarY_KMINUS1;
				lPreX = lPreY - lPreK;
				lY = lFarY_KMINUS1 + 1;
			}
			//k のｘ、ｙ座標取り出し
			lX = lY - lK;
			lNowX = lX;
			lNowY = lY;

			//ノード番号計算
			lPreNode = MakeNodeNo(lPreX,lPreY);
			lNowNode = MakeNodeNo(lNowX,lNowY);
			//直前ノード番号保持
			if(lPreNode != -1 && lNowNode != -1)
				SetValuePreIndex(lNowNode,lPreNode);

			//対角線があって、更に先に進める場合、値を更新
			for(;;)
			{
				m_pApp->MessagePump();
				if(m_pobjFileCompareStatusDlg->IsCanceled())
					return FALSE;

				if(lX >= m_lFileLineCount1 || lY >= m_lFileLineCount2 )
					break;
				pobjLine1 = (CLine*)m_objStringArray1.GetAt(lX);
				pobjLine2 = (CLine*)m_objStringArray2.GetAt(lY);
				objLine1.Set(pobjLine1->String());
				objLine2.Set(pobjLine2->String());
				if(bIsAssumeZenEqual2Han) //比較オプション：全角と半角を区別しない
				{
					objLine1.AlnumZen2Han();
					objLine1.KatakanaZen2Han();
					objLine1.SpaceZen2Han();
					objLine2.AlnumZen2Han();
					objLine2.KatakanaZen2Han();
					objLine2.SpaceZen2Han();
				}
				if(bIsAssumeTabEqual2Space) //比較オプション：空白文字を区別しない
				{
					objLine1.Spaces2One();
					objLine2.Spaces2One();
				}
				if(bIsAssumeAlphabetBigAndSmall) //比較オプション：英字の大文字小文字を区別しない
				{
					objLine1.Al2Upper();
					objLine2.Al2Upper();
				}
				if( objLine1.Compare(0,objLine2.String(),objLine1.GetLength()) != 0 )
					break;

				lPreNode = MakeNodeNo(lX,lY);
				lX += 1;
				lY += 1;
				lNowNode = MakeNodeNo(lX,lY);
				//直前ノード番号保持
				if(lPreNode != -1 && lNowNode != -1)
					SetValuePreIndex(lNowNode,lPreNode);
			}

			//対角線ｌＫに対応する最遠点のｌＹを保持
			SetValueFarY(lK+lOffset,lY);
#ifdef _DEBUG
//			char	szBuf[128];
//			sprintf(szBuf,"nPreK=%2d (%2d,%2d) ---> k=%2d : (%2d,%2d) ---> (%2d,%2d)",
//					lPreK,lPreX,lPreY,lK,lNowX,lNowY,lX,lY);
//			pApp->ExecuteLogging(szBuf);
#endif
			if( lX>=m_lFileLineCount1 && lY>=m_lFileLineCount2 )
			{
				return TRUE;                         //  found !!!
			}
		}
	}

	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：結果出力																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileCompare::OutputResult()		//ＴＲＵＥ：正常終了、ＦＡＬＳＥ：異常、キャンセル終了
{
	LONG	lPreNode=0;
	LONG	lNodeNo=0;
	LONG	lDiff=0;
	LONG	lEndNode = MakeNodeNo(m_lFileLineCount1,m_lFileLineCount2);
	LONG	lPreX=0;
	LONG	lPreY=0;
	CLine*	pobjLine1=NULL;
	CLine*	pobjLine2=NULL;

	for(lNodeNo=lEndNode; ; )
	{
		m_pApp->MessagePump();
		if(m_pobjFileCompareStatusDlg->IsCanceled())
			return FALSE;

		lPreNode = GetValuePreIndex(lNodeNo);
		if(lPreNode == 0)
			break;

		lDiff = lNodeNo - lPreNode;
		lPreX=lPreY=0;
		MakeXY(lPreNode,&lPreX,&lPreY);

		if(lDiff == 1 && lPreY >= m_lFileLineCount2)
			break;
		else if(lDiff == (m_lFileLineCount2 + 1) && lPreX >= m_lFileLineCount1)
			break;

		if(lDiff == 1)//水平右方向へ移動する（第二ファイルの文字列を出力）
		{
			//追加された行
			pobjLine2 = (CLine*)m_objStringArray2.GetAt(lPreY);
			pobjLine2->SetDiffLineKind(DIFF_LINEKIND_ADDED);
			m_pobjCompareDoc->InsertCompareLine(0,pobjLine2);
			m_lAddLineCount++;
		}
		else if(lDiff == (m_lFileLineCount2 + 1)) //垂直下方向へ移動する（第一ファイルの文字列を出力）
		{
			//削除された行
			pobjLine1 = (CLine*)m_objStringArray1.GetAt(lPreX);
			pobjLine1->SetDiffLineKind(DIFF_LINEKIND_DELETED);
			m_pobjCompareDoc->InsertCompareLine(0,pobjLine1);
			m_lDelLineCount++;
		}
		else //斜め右下方向へ移動する（共通）
		{
			//共通行である
			pobjLine1 = (CLine*)m_objStringArray1.GetAt(lPreX);
			pobjLine1->SetDiffLineKind(DIFF_LINEKIND_COMMON);
			m_pobjCompareDoc->InsertCompareLine(0,pobjLine1);
		}

		lNodeNo = lPreNode;
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ノード番号取得														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LONG CFileCompare::MakeNodeNo
	(	LONG	x, 
		LONG	y
	)
{
	if(x < 0 || y < 0)
		return -1;
	if(x > m_lFileLineCount1)
		return -1;
	if(y > m_lFileLineCount2)
		return -1;

	return 	x*(m_lFileLineCount2+1) + (y+1);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Ｘ，Ｙ番号取得														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileCompare::MakeXY
	(	LONG	lNode,
		LONG*	plX,
		LONG*	plY
	)
{
	LONG lAmari = lNode % (m_lFileLineCount2+1);
	if(lAmari != 0)
		(*plX) = lNode / (m_lFileLineCount2+1);
	else
		(*plX) = lNode / (m_lFileLineCount2+1) -1;

	(*plY) = lNode - (*plX)*(m_lFileLineCount2+1) - 1;
	if( (*plY) < 0 )
		(*plY) = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：前ノード設定															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileCompare::SetValuePreIndex
	(	LONG	x,
		LONG	y
	)
{
	m_objMapPreIndex[x] = y;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：前ノード取得															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LONG CFileCompare::GetValuePreIndex
	(	LONG	x
	)
{
	map<long,long>::iterator it;

	it = m_objMapPreIndex.find(x);
	if( it != m_objMapPreIndex.end() ) //発見！！
		return (*it).second;
	else
		return 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：各対角線の最遠点のｙ座標値設定										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileCompare::SetValueFarY
	(	LONG	x,
		LONG	y
	)
{
	m_objMapFarY[x] = y;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：各対角線の最遠点のｙ座標値取得										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LONG CFileCompare::GetValueFarY
	(	LONG	x
	)
{
	map<long,long>::iterator it;

	it = m_objMapFarY.find(x);
	if( it != m_objMapFarY.end() ) //発見！！
		return (*it).second;
	else
		return 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：クリア																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileCompare::Clear()
{
	int 		nCnt=0;
	CLine*		pobjLine=NULL;

	//第一ファイルの行データ削除
	for(nCnt=0; nCnt<m_objStringArray1.GetSize(); nCnt++)
	{
		pobjLine = (CLine*)m_objStringArray1.GetAt(nCnt);
		if(pobjLine != NULL)
			delete pobjLine;
	}
	m_objStringArray1.RemoveAll();

	//第一ファイルの行データ削除
	for(nCnt=0; nCnt<m_objStringArray2.GetSize(); nCnt++)
	{
		pobjLine = (CLine*)m_objStringArray2.GetAt(nCnt);
		if(pobjLine != NULL)
			delete pobjLine;
	}
	m_objStringArray2.RemoveAll();

	//マップデータクリア
	m_objMapPreIndex.clear();
	m_objMapFarY.clear();
}


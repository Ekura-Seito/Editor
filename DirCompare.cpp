// DirCompare.cpp : インプリメンテーション ファイル
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
#include "DirCompareStatusDlg.h"
#include "DirCompare.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirCompare クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDirCompare::CDirCompare()
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_pobjDirCompareStatusDlg = NULL;
	m_pobjCompareDoc = NULL;
	m_objStringArray1.SetSize(0);
	m_objStringArray2.SetSize(0);
	m_lFileCount1 = 0;
	m_lFileCount2 = 0;
	m_objDir1 = _T("");
	m_objDir2 = _T("");
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDirCompare::~CDirCompare()
{
	Clear();
}

/////////////////////////////////////////////////////////////////////////////
// CDirCompare パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ比較実行													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDirCompare::DoCompare			//ＴＲＵＥ：正常終了、ＦＡＬＳＥ：異常終了
	(	CDirCompareStatusDlg* pobjDirCompareStatusDlg,
		CEditorDoc* pobjCompareDoc,		// ドキュメントクラス
		CString&	objDir1,			// 比較第一ディレクトリ名
		CString&	objDir2				// 比較第二ディレクトリ名
	)
{
	BOOL	bRet=FALSE;
	LONG	lCnt=0;
	CLine*	pobjLine=NULL;
	CString objMsg;

	//パラメータ保持
	m_pobjDirCompareStatusDlg = pobjDirCompareStatusDlg;
	m_pobjCompareDoc = pobjCompareDoc;
	m_objDir1 = objDir1;
	m_objDir2 = objDir2;

	//ディレクトリ内ファイル一覧作成
	objMsg.LoadString(IDS_DIRCOMPARE_MAKE1DIR);
	m_pobjDirCompareStatusDlg->SetStatus(objMsg);
	if( MakeFileList(objDir1,&m_objStringArray1) == FALSE )
		return FALSE;
	objMsg.LoadString(IDS_DIRCOMPARE_MAKE2DIR);
	m_pobjDirCompareStatusDlg->SetStatus(objMsg);
	if( MakeFileList(objDir2,&m_objStringArray2) == FALSE )
		return FALSE;
	m_lFileCount1 = m_objStringArray1.GetSize();
	m_lFileCount2 = m_objStringArray2.GetSize();
	if(m_lFileCount1 == 0)
	{
		objMsg.LoadString(IDS_1STZERO_DIRCMP);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	if(m_lFileCount2 == 0)
	{
		objMsg.LoadString(IDS_2NDZERO_DIRCMP);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}

	//最長共通部分（ＬＣＳ : Longest Common Subsequence）探索
	objMsg.LoadString(IDS_DIRCOMPARE_NOWCOMPARE);
	m_pobjDirCompareStatusDlg->SetStatus(objMsg);
	if( SearchLCS() == FALSE )
		return FALSE;

	//結果出力
	objMsg.LoadString(IDS_DIRCOMPARE_OUTRESULT);
	m_pobjDirCompareStatusDlg->SetStatus(objMsg);
	if( OutputResult() == FALSE )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CFileCompare クラスのプロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ内ファイルリスト作成										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDirCompare::MakeFileList			//ＴＲＵＥ：正常終了、ＦＡＬＳＥ：異常、キャンセル終了
	(	CString&	objDir,				//ディレクトリ名
		CObArray*	pobjObArray
	)
{
	BOOL		bRet=FALSE;
	CString		objFilter=_T("");
	CString		objFileName=_T("");
	CFileFind	objFileFind;

	//ファイル検索キー作成
	if(m_pApp->IsDriveRoot(objDir))
		objFilter.Format("%s*",objDir);
	else
		objFilter.Format("%s\\*",objDir);

	//ファイル検索
	bRet = objFileFind.FindFile(objFilter);
	if(bRet != FALSE)
	{
		while(objFileFind.FindNextFile()!=FALSE)
		{
			m_pApp->MessagePump();
			if(m_pobjDirCompareStatusDlg->IsCanceled())
			{
				objFileFind.Close();
				return FALSE;
			}

			if( objFileFind.IsDots() != FALSE ) // "." or ".."
				continue;
			if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
				continue;

			//ファイル名取得
			objFileName = objFileFind.GetFileName();
			//アルファベット順にリストへ追加
			AddFileBySort(objFileName,pobjObArray);
		}
		//最後の１個
		if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
		{
			//ファイル名取得
			objFileName = objFileFind.GetFileName();
			//アルファベット順にリストへ追加
			AddFileBySort(objFileName,pobjObArray);
		}
	}
	objFileFind.Close();

	if(bRet)
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：アルファベット順にリストへ追加										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompare::AddFileBySort
	(	CString&	objFileName,
		CObArray*	pobjArray
	)
{
	int nIndex=0;
	int nSize=pobjArray->GetSize();
	CString* pobjStrTmp=NULL;
	CString* pobjStrBuf = new CString();
	(*pobjStrBuf) = objFileName;

	for(nIndex=0; nIndex<nSize; nIndex++)
	{
		pobjStrTmp = (CString*)pobjArray->GetAt(nIndex);
		if(pobjStrTmp->CompareNoCase((LPCTSTR)objFileName) <= 0)
			continue;
		else
			break;
	}

	if(nIndex == nSize)
		pobjArray->Add((CObject*)pobjStrBuf); //最後に追加
	else
		pobjArray->InsertAt(nIndex,(CObject*)pobjStrBuf);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：最長共通部分（ＬＣＳ : Longest Common Subsequence）探索				|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDirCompare::SearchLCS()		//ＴＲＵＥ：正常終了、ＦＡＬＳＥ：異常、キャンセル終了
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
	const LONG lOffset=m_lFileCount1;
	CString* pobjStr1=NULL;
	CString* pobjStr2=NULL;
	LONG	lFarY_KPLUS1=0;
	LONG	lFarY_KMINUS1=0;

	for(lD=0; lD<=m_lFileCount1+m_lFileCount2; lD++) //  各Ｄについて
	{
		m_pApp->MessagePump();
		if(m_pobjDirCompareStatusDlg->IsCanceled())
			return FALSE;

		for(lK=-lD; lK<=lD; lK+=2) 
		{
			m_pApp->MessagePump();
			if(m_pobjDirCompareStatusDlg->IsCanceled())
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
				if(m_pobjDirCompareStatusDlg->IsCanceled())
					return FALSE;

				if(lX >= m_lFileCount1 || lY >= m_lFileCount2 )
					break;
				pobjStr1 = (CString*)m_objStringArray1.GetAt(lX);
				pobjStr2 = (CString*)m_objStringArray2.GetAt(lY);
				if( pobjStr1->CompareNoCase((LPCTSTR)(*pobjStr2)) != 0 )
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
			if( lX>=m_lFileCount1 && lY>=m_lFileCount2 )
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
BOOL CDirCompare::OutputResult()		//ＴＲＵＥ：正常終了、ＦＡＬＳＥ：異常、キャンセル終了
{
	LONG	lPreNode=0;
	LONG	lNodeNo=0;
	LONG	lDiff=0;
	LONG	lEndNode = MakeNodeNo(m_lFileCount1,m_lFileCount2);
	LONG	lPreX=0;
	LONG	lPreY=0;
	CString* pobjStr1=NULL;
	CString* pobjStr2=NULL;
	CLine	objLine;
	char	szBuf[256];
	CString objMsg;

	for(lNodeNo=lEndNode; ; )
	{
		m_pApp->MessagePump();
		if(m_pobjDirCompareStatusDlg->IsCanceled())
			return FALSE;

		lPreNode = GetValuePreIndex(lNodeNo);
		if(lPreNode == 0)
			break;

		lDiff = lNodeNo - lPreNode;
		lPreX=lPreY=0;
		MakeXY(lPreNode,&lPreX,&lPreY);

		if(lDiff == 1 && lPreY >= m_lFileCount2)
			break;
		else if(lDiff == (m_lFileCount2 + 1) && lPreX >= m_lFileCount1)
			break;

		if(lDiff == 1)//水平右方向へ移動する（第二ディレクトリのファイル名を出力）
		{
			//追加されたファイル
			pobjStr2 = (CString*)m_objStringArray2.GetAt(lPreY);

			objMsg.LoadString(IDS_DIRCOMPARE_RESULTMSG1);
			sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)(*pobjStr2), (LPCTSTR)m_objDir2);
			objLine.Set((const unsigned char*)szBuf);
			objLine.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
			m_pobjCompareDoc->InsertDirCmpLine(0,&objLine);
			m_pobjDirCompareStatusDlg->IncCompareFileCount();
			m_pobjDirCompareStatusDlg->IncAddFileCount();
		}
		else if(lDiff == (m_lFileCount2 + 1)) //垂直下方向へ移動する（第一ディレクトリのファイル名を出力）
		{
			//削除されたファイル
			pobjStr1 = (CString*)m_objStringArray1.GetAt(lPreX);

			objMsg.LoadString(IDS_DIRCOMPARE_RESULTMSG1);
			sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)(*pobjStr1), (LPCTSTR)m_objDir1);
			objLine.Set((const unsigned char*)szBuf);
			objLine.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
			m_pobjCompareDoc->InsertDirCmpLine(0,&objLine);
			m_pobjDirCompareStatusDlg->IncCompareFileCount();
			m_pobjDirCompareStatusDlg->IncDelFileCount();
		}
		else //斜め右下方向へ移動する（共通）
		{
			//共通ファイル名
			pobjStr1 = (CString*)m_objStringArray1.GetAt(lPreX);

			//同一チェック
			int nDate=0;
			int nRet = CheckIsSameFile(pobjStr1,&nDate);
			if(nRet == 1)
			{
				objMsg.LoadString(IDS_DIRCOMPARE_RESULTMSG4);
				sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)(*pobjStr1));
				objLine.Set((const unsigned char*)szBuf);
				objLine.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
			}
			else if(nRet == 0)
			{
				if(nDate == 1)
				{
					objMsg.LoadString(IDS_DIRCOMPARE_RESULTMSG2);
					sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)(*pobjStr1), (LPCTSTR)m_objDir1);
				}
				else if(nDate == 2)
				{
					objMsg.LoadString(IDS_DIRCOMPARE_RESULTMSG2);
					sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)(*pobjStr1), (LPCTSTR)m_objDir2);
				}
				else if(nDate == 3)
				{
					objMsg.LoadString(IDS_DIRCOMPARE_RESULTMSG3);
					sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)(*pobjStr1));
				}
				objLine.Set((const unsigned char*)szBuf);
				objLine.SetDirCmpFileName((*pobjStr1));
				objLine.SetDirCmpLineKind(DIRCMP_LINEKIND_DIFF);
				m_pobjDirCompareStatusDlg->IncDiffFileCount();
			}
			else if(nRet == -1)
			{
				objMsg.LoadString(IDS_FILEACCESS_ERROR);
				sprintf(szBuf,"  %-30s : %s", (LPCTSTR)(*pobjStr1), (LPCTSTR)objMsg);
				objLine.Set((const unsigned char*)szBuf);
				objLine.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
			}
			m_pobjCompareDoc->InsertDirCmpLine(0,&objLine);
			m_pobjDirCompareStatusDlg->IncCompareFileCount();
		}

		lNodeNo = lPreNode;
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル同一チェック													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CDirCompare::CheckIsSameFile			//１：同一、０：相違あり、－１：異常終了
	(	CString*	pobjFileNameStr,
		int*		pnDate					//１：第一が新しい、２：第二が新しい、３：同じ
	)
{
	CString		objFilePath1=_T("");
	CString		objFilePath2=_T("");
	CFile		objFile1, objFile2;
	CFileStatus objFileStatus1, objFileStatus2;
	char		szBufForAr1[512];
	char		szBufForAr2[512];
	char ch1=0;
	char ch2=0;

	//ファイルパス編集
	if(m_pApp->IsDriveRoot(m_objDir1))
		objFilePath1.Format("%s%s",m_objDir1,(*pobjFileNameStr));
	else
		objFilePath1.Format("%s\\%s",m_objDir1,(*pobjFileNameStr));
	if(m_pApp->IsDriveRoot(m_objDir2))
		objFilePath2.Format("%s%s",m_objDir2,(*pobjFileNameStr));
	else
		objFilePath2.Format("%s\\%s",m_objDir2,(*pobjFileNameStr));

	CFile::GetStatus((LPCTSTR)objFilePath1,objFileStatus1);
	CFile::GetStatus((LPCTSTR)objFilePath2,objFileStatus2);
	//ファイル日付チェック
	if(objFileStatus1.m_mtime > objFileStatus2.m_mtime)
		(*pnDate) = 1;
	else if(objFileStatus1.m_mtime < objFileStatus2.m_mtime)
		(*pnDate) = 2;
	else if(objFileStatus1.m_mtime == objFileStatus2.m_mtime)
		(*pnDate) = 3;
	//ファイルサイズチェック
	if(objFileStatus1.m_size != objFileStatus2.m_size)
		return 0;

	//ファイルオープン
	memset(szBufForAr1,0,sizeof(szBufForAr1));
	memset(szBufForAr2,0,sizeof(szBufForAr2));
	if( !objFile1.Open( objFilePath1, CFile::modeRead|CFile::typeBinary, NULL ) )
		return -1;
	CArchive ar1( &objFile1, CArchive::load, sizeof(szBufForAr1), szBufForAr1 );
	if( !objFile2.Open( objFilePath2, CFile::modeRead|CFile::typeBinary, NULL ) )
	{
		ar1.Close();
		objFile1.Close();
		return -1;
	}
	CArchive ar2( &objFile2, CArchive::load, sizeof(szBufForAr2), szBufForAr2 );

	//１バイトずつチェック
	TRY
	{
		for(;;)
		{
			ar1 >> (BYTE&)ch1;
			ar2 >> (BYTE&)ch2;
			if(ch1 != ch2)
			{
				//相違あり
				ar1.Close();
				objFile1.Close();
				ar2.Close();
				objFile2.Close();
				return 0;
			}
		}
	}
	CATCH_ALL(e)
	{
		//同一
		ar1.Close();
		objFile1.Close();
		ar2.Close();
		objFile2.Close();
		return 1;
	}
	END_CATCH_ALL

	ar1.Close();
	objFile1.Close();
	ar2.Close();
	objFile2.Close();
	return 1;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ノード番号取得														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LONG CDirCompare::MakeNodeNo
	(	LONG	x, 
		LONG	y
	)
{
	if(x < 0 || y < 0)
		return -1;
	if(x > m_lFileCount1)
		return -1;
	if(y > m_lFileCount2)
		return -1;

	return 	x*(m_lFileCount2+1) + (y+1);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Ｘ，Ｙ番号取得														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompare::MakeXY
	(	LONG	lNode,
		LONG*	plX,
		LONG*	plY
	)
{
	LONG lAmari = lNode % (m_lFileCount2+1);
	if(lAmari != 0)
		(*plX) = lNode / (m_lFileCount2+1);
	else
		(*plX) = lNode / (m_lFileCount2+1) -1;

	(*plY) = lNode - (*plX)*(m_lFileCount2+1) - 1;
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
void CDirCompare::SetValuePreIndex
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
LONG CDirCompare::GetValuePreIndex
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
void CDirCompare::SetValueFarY
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
LONG CDirCompare::GetValueFarY
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
void CDirCompare::Clear()
{
	int 		nCnt=0;
	CString*	pobjStr=NULL;

	//第一ディレクトリのファイル名データ削除
	for(nCnt=0; nCnt<m_objStringArray1.GetSize(); nCnt++)
	{
		pobjStr = (CString*)m_objStringArray1.GetAt(nCnt);
		if(pobjStr != NULL)
			delete pobjStr;
	}
	m_objStringArray1.RemoveAll();

	//第二ディレクトリのファイル名データ削除
	for(nCnt=0; nCnt<m_objStringArray2.GetSize(); nCnt++)
	{
		pobjStr = (CString*)m_objStringArray2.GetAt(nCnt);
		if(pobjStr != NULL)
			delete pobjStr;
	}
	m_objStringArray2.RemoveAll();

	//マップデータクリア
	m_objMapPreIndex.clear();
	m_objMapFarY.clear();
}


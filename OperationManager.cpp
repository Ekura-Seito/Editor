// OperationManager.cpp : COperationManager クラスの動作の定義を行います。
//

#include "stdafx.h"
#include <afxtempl.h>			//for CTypedPtrList
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "Editor.h"
#include "Operation.h"
#include "OperationManager.h"
#include "StaticFilespec.h"
#include "ListCtrlEx.h"
#include "AutoResizer.h"
#include "KeyOpeDispDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// COperationManager クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COperationManager::COperationManager()
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_objOperationFileName = _T("");
	m_objOperationName = _T("");
	m_lCurrentPosition = 0;
	clearAllItem();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COperationManager::~COperationManager()
{
	clearAllItem();
}

/////////////////////////////////////////////////////////////////////////////
// COperationManager パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：初期化																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::Initialize()
{
	m_objOperationFileName = _T("");
	m_objOperationName = _T("");
	m_lCurrentPosition = 0;
	clearAllItem();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オペレーション一覧表示												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::DispOperationLists()
{
	//オペレーション一覧ダイアログ表示
	CKeyOpeDispDlg	objKeyOpeDispDlg;
	objKeyOpeDispDlg.Initialize(this);
	objKeyOpeDispDlg.DoModal();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オペレーションファイル名設定											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::SetOperationFileName
	(	CString&	objFName
	)
{
	m_objOperationFileName = objFName;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オペレーションファイル名取得											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::GetOperationFileName
	(	CString&	objFName
	)
{
	objFName = m_objOperationFileName;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オペレーション名設定													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::SetOperationName
	(	CString&	objName
	)
{
	m_objOperationName = objName;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オペレーション名取得													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::GetOperationName
	(	CString&	objName
	)
{
	objName = m_objOperationName;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オペレーション数取得													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long COperationManager::GetOperationCount()
{
	return m_objOperationList.GetCount();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：COperation の登録														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::addOperation
	(	COperation*	pobjOperation
	)
{
	m_objOperationList.AddTail(pobjOperation);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オペレーション連続実行												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::doOperation
	(	CEditorView*	pobjEditorView,		//オペレーション実行対象のＶｉｅｗ
		CEditorDoc*		pobjEditorDoc		//オペレーション実行対象のＤｏｃ
	)
{
	POSITION pos = m_objOperationList.GetHeadPosition();
	COperation*	pobjOperation = NULL;
	for(;;)
	{
		m_pApp->MessagePump();

		if( pos == 0 )
			break;
		pobjOperation = (COperation*)m_objOperationList.GetNext(pos);
		if( pobjOperation == NULL )
			break;
		//オペレーション実行
		pobjOperation->Do(pobjEditorView,pobjEditorDoc);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オペレーションのステップ実行											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL COperationManager::stepDoOperation
	(	CEditorView*	pobjEditorView,		//オペレーション実行対象のＶｉｅｗ
		CEditorDoc*		pobjEditorDoc,		//オペレーション実行対象のＤｏｃ
		BOOL			bIsFirst			//初回フラグ
	)
											//関数リターン
											// TRUE  : つづきあり
											// FALSE : 終了
{
	COperation*	pobjOperation = NULL;

	if(bIsFirst) //ステップ実行の開始
	{
		//先頭オペレーション取得
		m_lCurrentPosition = m_objOperationList.GetHeadPosition();
	}

	if(m_lCurrentPosition != 0)
	{
		pobjOperation = (COperation*)m_objOperationList.GetNext(m_lCurrentPosition);
		//オペレーション実行
		if( pobjOperation != NULL )
			pobjOperation->Do(pobjEditorView,pobjEditorDoc);
	}

	if(m_lCurrentPosition != 0)
		return TRUE; 	//つづきあり
	else
		return FALSE;	//終了
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オペレーションのステップ実行終了										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::endStepDoOperation()
{
	m_lCurrentPosition = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オペレーションファイルチェック										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL COperationManager::checkIsOperationFile
	(	CArchive&	ar
	)
												//関数リターン
												//  TRUE : オペレーションファイルである
												// FALSE : オペレーションファイルでない
{
	int  nRet=0;
	long lCRCount=0;
	long lLFCount=0;

	//マジックワードチェック
	CString objStrBuf = _T("");
	m_pApp->PrepareMyReadString();
	nRet = m_pApp->MyReadString(ar,objStrBuf,&lCRCount,&lLFCount);
	if( nRet == 0 ) //EOF終了
	{
		if( objStrBuf.IsEmpty() ) //データなし
			return FALSE;
	}
	if( objStrBuf.Compare(KEYOPE_MAGICWORD) != 0 )//マジックワード不一致
	{
		return FALSE;
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：シリアライズ															|*/
/*|	解説	：																		|*/
/*|	備考	：本関数を読み込みで呼ぶ場合、あらかじめcheckIsOperationFile関数で		|*/
/*|			  マジックワードチェックをすませておくこと。							|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL COperationManager::Serialize		//TRUE : 正常終了、FALSE : 異常終了
	(	CArchive&	ar
	)
{
	int  nRet=0;
	long lCRCount=0;
	long lLFCount=0;
	char*	pPtr=NULL;
	COperation*	pobjOperation = NULL;
	BOOL bRet=FALSE;

	if(ar.IsStoring()) //保存
	{
		//マジックワード書込み
		ar.WriteString(KEYOPE_MAGICWORD);
		ar.WriteString("\r\n");

		//オペレーション名書込み
		ar.WriteString(KEYOPE_OPENAME_HEADER);
		ar.WriteString((LPCTSTR)m_objOperationName);
		ar.WriteString("\r\n");

		m_objOperationList.Serialize(ar);
		return TRUE;
	}
	else //詠みだし
	{
		//初期化
		Initialize();

		//このSerialize関数をコールする前に
		//マジックワードチェック済みである。

		//オペレーション名取得
		CString objStrBuf = _T("");
		m_pApp->PrepareMyReadString();
		nRet = m_pApp->MyReadString(ar,objStrBuf,&lCRCount,&lLFCount);
		if( nRet == 0 ) //EOF終了
		{
			if( objStrBuf.IsEmpty() ) //データなし
				return FALSE;
		}
		if( objStrBuf.Left(strlen(KEYOPE_OPENAME_HEADER)) != KEYOPE_OPENAME_HEADER )//オペレーション名ヘッダではない
		{
			return FALSE;
		}
		else
		{
			//オペレーション名取得＆設定
			CString objOpeName=_T("");
			if( objStrBuf.GetLength() != (signed)strlen(KEYOPE_OPENAME_HEADER) )
			{
				pPtr = objStrBuf.GetBuffer(objStrBuf.GetLength());
				objOpeName = (char*)(pPtr + strlen(KEYOPE_OPENAME_HEADER));
				objStrBuf.ReleaseBuffer();
			}
			SetOperationName(objOpeName);
		}

		m_objOperationList.Serialize(ar);
		return TRUE;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オペレーション順次検索開始											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::SearchStartOperation()
{
	m_lCurrentPosition = m_objOperationList.GetHeadPosition();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オペレーション順次検索												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COperation* COperationManager::SearchGetOperation()
{
	if(m_lCurrentPosition == 0)
		return NULL;
	else
		return (COperation*)m_objOperationList.GetNext(m_lCurrentPosition);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オペレーション順次検索終了											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::SearchEndOperation()
{
	m_lCurrentPosition = 0;
}

/////////////////////////////////////////////////////////////////////////////
// COperationManager プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：COperation リストのクリア												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperationManager::clearAllItem()
{
	POSITION pos = m_objOperationList.GetHeadPosition();
	POSITION oldPos = pos;
	COperation*	pobjOperation = NULL;
	for(;;)
	{
		if( pos == 0 )
			break;
		pobjOperation = (COperation*)m_objOperationList.GetNext(pos);
		if( pobjOperation == NULL )
			break;
		m_objOperationList.RemoveAt(oldPos);
		delete pobjOperation;
		oldPos = pos;
	}
	m_lCurrentPosition = 0;
}

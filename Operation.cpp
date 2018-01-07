// Operation.cpp: COperation クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxole.h>				//for COleDataSource, COleDropTarget
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "Editor.h"
#include "Operation.h"
#include "Line.h"
#include "BCMenu.h"
#include "EditorView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(COperation, CObject, 0)

/////////////////////////////////////////////////////////////////////////////
// COperation クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COperation::COperation()
{
	m_pApp = (CEditorApp*)AfxGetApp();
	memset(m_szOperationString,0,sizeof(m_szOperationString));
	m_lOperationKind = 0;

	//CView::OnKeyDown関連パラメータ
	m_nChar_OnKeyDown = 0;
	m_nRepCnt_OnKeyDown = 0;
	m_nFlags_OnKeyDown = 0;
	m_nMultValueOfCaretAccelarete = 0;

	//CView::OnKeyUp関連パラメータ
	m_nChar_OnKeyUp = 0;
	m_nRepCnt_OnKeyUp = 0;
	m_nFlags_OnKeyUp = 0;

	//CView::OnChar関連パラメータ
	m_nChar_OnChar = 0;
	m_nRepCnt_OnChar = 0;
	m_nFlags_OnChar = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COperation::~COperation()
{
}

/////////////////////////////////////////////////////////////////////////////
// COperation パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：シリアライズ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperation::Serialize(CArchive& ar)
{
	CString objBuf=_T("");

	CObject::Serialize(ar);
	if (ar.IsStoring())
	{
		objBuf = m_szOperationString;
		ar << objBuf;
		ar << m_lOperationKind;// オペレーション種別

		//CView::OnKeyDown関連パラメータ
		ar << m_nChar_OnKeyDown;
		ar << m_nRepCnt_OnKeyDown;
		ar << m_nFlags_OnKeyDown;
		ar << m_nMultValueOfCaretAccelarete;

		//CView::OnKeyUp関連パラメータ
		ar << m_nChar_OnKeyUp;
		ar << m_nRepCnt_OnKeyUp;
		ar << m_nFlags_OnKeyUp;

		//CView::OnChar関連パラメータ
		ar << m_nChar_OnChar;
		ar << m_nRepCnt_OnChar;
		ar << m_nFlags_OnChar;
	}
	else
	{
		ar >> objBuf;
		strcpy(m_szOperationString,(LPCTSTR)objBuf);
		ar >> m_lOperationKind;// オペレーション種別

		//CView::OnKeyDown関連パラメータ
		ar >> m_nChar_OnKeyDown;
		ar >> m_nRepCnt_OnKeyDown;
		ar >> m_nFlags_OnKeyDown;
		ar >> m_nMultValueOfCaretAccelarete;

		//CView::OnKeyUp関連パラメータ
		ar >> m_nChar_OnKeyUp;
		ar >> m_nRepCnt_OnKeyUp;
		ar >> m_nFlags_OnKeyUp;

		//CView::OnChar関連パラメータ
		ar >> m_nChar_OnChar;
		ar >> m_nRepCnt_OnChar;
		ar >> m_nFlags_OnChar;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オペレーション種別設定												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperation::SetOperationKind
	(	long	lOperationKind
	)
{
	m_lOperationKind = lOperationKind;

	//オペレーションを説明するテキスト設定
	if(m_lOperationKind == KEYOPE_ONKEYDOWN)
		strcpy(m_szOperationString,"ONKEYDOWN　：　");
	else if(m_lOperationKind == KEYOPE_ONKEYUP)
		strcpy(m_szOperationString,"ONKEYUP　　　：　");
	else if(m_lOperationKind == KEYOPE_ONCHAR)
		strcpy(m_szOperationString,"ONCHAR　　　　：　");
	else if(m_lOperationKind == KEYOPE_ONEDITCOPY)
		strcpy(m_szOperationString,"ONEDITCOPY：　");
	else if(m_lOperationKind == KEYOPE_ONEDITCUT)
		strcpy(m_szOperationString,"ONEDITCUT　：　");
	else if(m_lOperationKind == KEYOPE_ONEDITPASTE)
		strcpy(m_szOperationString,"ONEDITPASTE：　");
	else if(m_lOperationKind == KEYOPE_ONEDITDEL)
		strcpy(m_szOperationString,"ONEDITDEL　：　");
	else if(m_lOperationKind == KEYOPE_ONEDITSELECTALL)
		strcpy(m_szOperationString,"ONEDITSELECTALL：　");
	else if(m_lOperationKind == KEYOPE_ONEDITTAB2SPACE)
		strcpy(m_szOperationString,"ONEDITTAB2SPACE：");
	else if(m_lOperationKind == KEYOPE_ONEDITSPACE2TAB)
		strcpy(m_szOperationString,"ONEDITSPACE2TAB：");
	else if(m_lOperationKind == KEYOPE_ONEDITSPACEZEN2HAN)
		strcpy(m_szOperationString,"ONEDITSPACEZEN2HAN：");
	else if(m_lOperationKind == KEYOPE_ONEDITSPACEHAN2ZEN)
		strcpy(m_szOperationString,"ONEDITSPACEHAN2ZEN：");
	else if(m_lOperationKind == KEYOPE_ONEDITKATAKANAHAN2ZEN)
		strcpy(m_szOperationString,"ONEDITKATAKANAHAN2ZEN：");
	else if(m_lOperationKind == KEYOPE_ONEDITKATAKANAZEN2HAN)
		strcpy(m_szOperationString,"ONEDITKATAKANAZEN2HAN：");
	else if(m_lOperationKind == KEYOPE_ONEDITALNUMHAN2ZEN)
		strcpy(m_szOperationString,"ONEDITALNUMHAN2ZEN：");
	else if(m_lOperationKind == KEYOPE_ONEDITALNUMZEN2HAN)
		strcpy(m_szOperationString,"ONEDITALNUMZEN2HAN：");
	else if(m_lOperationKind == KEYOPE_ONEDITDELTOPSPACES)
		strcpy(m_szOperationString,"ONEDITDELTOPSPACES：");
	else if(m_lOperationKind == KEYOPE_ONEDITINSTAB)
		strcpy(m_szOperationString,"ONEDITINSTAB：");
	else if(m_lOperationKind == KEYOPE_ONEDITDELTAB)
		strcpy(m_szOperationString,"ONEDITDELTAB：");
	else if(m_lOperationKind == KEYOPE_ONEDITJUMP2TOP)
		strcpy(m_szOperationString,"ONEDITJUMP2TOP：");
	else if(m_lOperationKind == KEYOPE_ONEDITJUMP2BOTTOM)
		strcpy(m_szOperationString,"ONEDITJUMP2BOTTOM：");
	else if(m_lOperationKind == KEYOPE_ONPARENTHESIS)
		strcpy(m_szOperationString,"ONPARENTHESIS：");
	else if(m_lOperationKind == KEYOPE_ONEDITINSCPPCOMMENT)
		strcpy(m_szOperationString,"ONEDITINSCPPCOMMENT：");
	else if(m_lOperationKind == KEYOPE_ONEDITINSVBCOMMENT)
		strcpy(m_szOperationString,"ONEDITINSVBCOMMENT：");
	else if(m_lOperationKind == KEYOPE_ONEDITINSASMCOMMENT)
		strcpy(m_szOperationString,"ONEDITINSASMCOMMENT：");
	else if(m_lOperationKind == KEYOPE_ONEDITINSDAY)
		strcpy(m_szOperationString,"ONEDITINSDAY：");
	else if(m_lOperationKind == KEYOPE_ONEDITINSTIME)
		strcpy(m_szOperationString,"ONEDITINSTIME：");
	else if(m_lOperationKind == KEYOPE_ONJUMPNEXTMARK)
		strcpy(m_szOperationString,"ONJUMPNEXTMARK：");
	else if(m_lOperationKind == KEYOPE_ONJUMPPREVMARK)
		strcpy(m_szOperationString,"ONJUMPPREVMARK：");
	else if(m_lOperationKind == KEYOPE_ONJUMPMARKLIST)
		strcpy(m_szOperationString,"ONJUMPMARKLIST：");
	else if(m_lOperationKind == KEYOPE_ONEDITAL2UPPER)
		strcpy(m_szOperationString,"ONEDITAL2UPPER：");
	else if(m_lOperationKind == KEYOPE_ONEDITAL2LOWER)
		strcpy(m_szOperationString,"ONEDITAL2LOWER：");
	else if(m_lOperationKind == KEYOPE_ONWORDSKIP2RIGHT)
		strcpy(m_szOperationString,"ONWORDSKIP2RIGHT：");
	else if(m_lOperationKind == KEYOPE_ONWORDSKIP2LEFT)
		strcpy(m_szOperationString,"ONWORDSKIP2LEFT：");
	else if(m_lOperationKind == KEYOPE_ONWORDSELECT2RIGHT)
		strcpy(m_szOperationString,"ONWORDSELECT2RIGHT：");
	else if(m_lOperationKind == KEYOPE_ONWORDSELECT2LEFT)
		strcpy(m_szOperationString,"ONWORDSELECT2LEFT：");
	else if(m_lOperationKind == KEYOPE_ONEDITJUMP2TOPSELECT)
		strcpy(m_szOperationString,"ONEDITJUMP2TOPSELECT：");
	else if(m_lOperationKind == KEYOPE_ONEDITJUMP2BOTTOMSELECT)
		strcpy(m_szOperationString,"ONEDITJUMP2BOTTOMSELECT：");
	else if(m_lOperationKind == KEYOPE_ONPARENTHESISSELECT)
		strcpy(m_szOperationString,"ONPARENTHESISSELECT：");
	else if(m_lOperationKind == KEYOPE_ONEDITINSQUOTE)
		strcpy(m_szOperationString,"ONEDITINSQUOTE");
	else if(m_lOperationKind == KEYOPE_ONEDITDELCPPCOMMENT)
		strcpy(m_szOperationString, "ONEDITDELCPPCOMMENT：");
	else if(m_lOperationKind == KEYOPE_ONEDITDELVBCOMMENT)
		strcpy(m_szOperationString, "ONEDITDELVBCOMMENT：");
	else if(m_lOperationKind == KEYOPE_ONEDITDELASMCOMMENT)
		strcpy(m_szOperationString, "ONEDITDELASMCOMMENT：");
	else if(m_lOperationKind == KEYOPE_ONEDITDELQUOTE)
		strcpy(m_szOperationString, "ONEDITDELQUOTE：");
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：CView::OnKeyDownパラメータ設定										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperation::SetOnKeyDownParameter
	(	UINT	nChar,
		UINT	nRepCnt,
		UINT	nFlags,
		int		nMultValueOfCaretAccelarete
	)
{
	m_nChar_OnKeyDown = nChar;
	m_nRepCnt_OnKeyDown = nRepCnt;
	m_nFlags_OnKeyDown = nFlags;
	m_nMultValueOfCaretAccelarete = nMultValueOfCaretAccelarete;

	//オペレーションを説明するテキスト追加設定
	SetOperationString(nChar,nRepCnt,nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：CView::OnKeyUpパラメータ設定											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperation::SetOnKeyUpParameter
	(	UINT	nChar,
		UINT	nRepCnt,
		UINT	nFlags
	)
{
	m_nChar_OnKeyUp = nChar;
	m_nRepCnt_OnKeyUp = nRepCnt;
	m_nFlags_OnKeyUp = nFlags;

	//オペレーションを説明するテキスト追加設定
	SetOperationString(nChar,nRepCnt,nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：CView::OnCharパラメータ設定											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperation::SetOnCharParameter
	(	UINT	nChar,
		UINT	nRepCnt,
		UINT	nFlags
	)
{
	m_nChar_OnChar = nChar;
	m_nRepCnt_OnChar = nRepCnt;
	m_nFlags_OnChar = nFlags;

	//オペレーションを説明するテキスト追加設定
	SetOperationString(nChar,nRepCnt,nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：処理実行																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperation::Do
	(	CEditorView*	pobjEditorView,		//オペレーション実行対象のＶｉｅｗ
		CEditorDoc*		pobjEditorDoc		//オペレーション実行対象のＤｏｃ
	)
{
	if(m_lOperationKind == KEYOPE_ONKEYDOWN)
	{
		m_pApp->WriteProfileInt("CEditorView","nMultValueOfCaretAccelarete",m_nMultValueOfCaretAccelarete);
		pobjEditorView->OnKeyDown(m_nChar_OnKeyDown,m_nRepCnt_OnKeyDown,m_nFlags_OnKeyDown);
	}
	else if(m_lOperationKind == KEYOPE_ONKEYUP)
		pobjEditorView->OnKeyUp(m_nChar_OnKeyUp,m_nRepCnt_OnKeyUp,m_nFlags_OnKeyUp);
	else if(m_lOperationKind == KEYOPE_ONCHAR)
		pobjEditorView->OnChar(m_nChar_OnChar,m_nRepCnt_OnChar,m_nFlags_OnChar);
	else if(m_lOperationKind == KEYOPE_ONEDITCOPY)
		pobjEditorView->OnEditCopy();
	else if(m_lOperationKind == KEYOPE_ONEDITCUT)
		pobjEditorView->OnEditCut();
	else if(m_lOperationKind == KEYOPE_ONEDITPASTE)
		pobjEditorView->OnEditPaste();
	else if(m_lOperationKind == KEYOPE_ONEDITDEL)
		pobjEditorView->OnEditDel();
	else if(m_lOperationKind == KEYOPE_ONEDITSELECTALL)
		pobjEditorView->OnEditSelectall();
	else if(m_lOperationKind == KEYOPE_ONEDITTAB2SPACE)
		pobjEditorView->OnEditTab2space();
	else if(m_lOperationKind == KEYOPE_ONEDITSPACE2TAB)
		pobjEditorView->OnEditSpace2tab();
	else if(m_lOperationKind == KEYOPE_ONEDITSPACEZEN2HAN)
		pobjEditorView->OnEditSpaceZen2han();
	else if(m_lOperationKind == KEYOPE_ONEDITSPACEHAN2ZEN)
		pobjEditorView->OnEditSpaceHan2zen();
	else if(m_lOperationKind == KEYOPE_ONEDITKATAKANAHAN2ZEN)
		pobjEditorView->OnEditKatakanaHan2zen();
	else if(m_lOperationKind == KEYOPE_ONEDITKATAKANAZEN2HAN)
		pobjEditorView->OnEditKatakanaZen2han();
	else if(m_lOperationKind == KEYOPE_ONEDITALNUMHAN2ZEN)
		pobjEditorView->OnEditAlnumHan2zen();
	else if(m_lOperationKind == KEYOPE_ONEDITALNUMZEN2HAN)
		pobjEditorView->OnEditAlnumZen2han();
	else if(m_lOperationKind == KEYOPE_ONEDITDELTOPSPACES)
		pobjEditorView->OnEditDeltopspaces();
	else if(m_lOperationKind == KEYOPE_ONEDITINSTAB)
		pobjEditorView->OnEditInstab();
	else if(m_lOperationKind == KEYOPE_ONEDITDELTAB)
		pobjEditorView->OnEditDeltab();
	else if(m_lOperationKind == KEYOPE_ONEDITJUMP2TOP)
		pobjEditorView->OnEditJump2top();
	else if(m_lOperationKind == KEYOPE_ONEDITJUMP2BOTTOM)
		pobjEditorView->OnEditJump2bottom();
	else if(m_lOperationKind == KEYOPE_ONPARENTHESIS)
		pobjEditorView->OnParenthesis();
	else if(m_lOperationKind == KEYOPE_ONEDITINSCPPCOMMENT)
		pobjEditorView->OnEditInscppcomment();
	else if(m_lOperationKind == KEYOPE_ONEDITINSVBCOMMENT)
		pobjEditorView->OnEditInsvbcomment();
	else if(m_lOperationKind == KEYOPE_ONEDITINSASMCOMMENT)
		pobjEditorView->OnEditInsasmcomment();
	else if(m_lOperationKind == KEYOPE_ONEDITINSDAY)
		pobjEditorView->OnEditInsday();
	else if(m_lOperationKind == KEYOPE_ONEDITINSTIME)
		pobjEditorView->OnEditInstime();
	else if(m_lOperationKind == KEYOPE_ONJUMPNEXTMARK)
		pobjEditorView->OnJumpNextmark();
	else if(m_lOperationKind == KEYOPE_ONJUMPPREVMARK)
		pobjEditorView->OnJumpPrevmark();
	else if(m_lOperationKind == KEYOPE_ONJUMPMARKLIST)
		pobjEditorView->OnJumpMarklist();
	else if(m_lOperationKind == KEYOPE_ONEDITAL2UPPER)
		pobjEditorView->OnEditAl2upper();
	else if(m_lOperationKind == KEYOPE_ONEDITAL2LOWER)
		pobjEditorView->OnEditAl2lower();
	else if(m_lOperationKind == KEYOPE_ONWORDSKIP2RIGHT)
		pobjEditorView->OnWordskip2right();
	else if(m_lOperationKind == KEYOPE_ONWORDSKIP2LEFT)
		pobjEditorView->OnWordskip2left();
	else if(m_lOperationKind == KEYOPE_ONWORDSELECT2RIGHT)
		pobjEditorView->OnWordselect2right();
	else if(m_lOperationKind == KEYOPE_ONWORDSELECT2LEFT)
		pobjEditorView->OnWordselect2left();
	else if(m_lOperationKind == KEYOPE_ONEDITJUMP2TOPSELECT)
		pobjEditorView->OnEditJump2topselect();
	else if(m_lOperationKind == KEYOPE_ONEDITJUMP2BOTTOMSELECT)
		pobjEditorView->OnEditJump2bottomselect();
	else if(m_lOperationKind == KEYOPE_ONPARENTHESISSELECT)
		pobjEditorView->OnParenthesisSelect();
	else if(m_lOperationKind == KEYOPE_ONEDITINSQUOTE)
		pobjEditorView->OnEditInsquote();
	else if(m_lOperationKind == KEYOPE_ONEDITDELCPPCOMMENT)
		pobjEditorView->OnEditDelcppcomment();
	else if(m_lOperationKind == KEYOPE_ONEDITDELVBCOMMENT)
		pobjEditorView->OnEditDelvbcomment();
	else if(m_lOperationKind == KEYOPE_ONEDITDELASMCOMMENT)
		pobjEditorView->OnEditDelasmcomment();
	else if(m_lOperationKind == KEYOPE_ONEDITDELQUOTE)
		pobjEditorView->OnEditDelquote();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オペレーション内容説明テキスト取得									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperation::GetOperationString
	(	char*	szBuf
	)
{
	strcpy(szBuf,m_szOperationString);
}

/////////////////////////////////////////////////////////////////////////////
// COperation プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オペレーション内容説明テキスト設定									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperation::SetOperationString
	(	UINT	nChar,
		UINT	nRepCnt,
		UINT	nFlags
	)
{
	//オペレーションを説明するテキスト追加設定
	char	szBuf[256];
	sprintf(szBuf,"nChar=%6X(H), nRepCnt=%6X(H), nFlags=%6X(H)",nChar,nRepCnt,nFlags);
	switch(nChar)
	{
	case VK_SHIFT:	//SHIFTキー
		strcat(szBuf,", VK=VK_SHIFT");
		break;
	case VK_INSERT: //INSERTキー
		strcat(szBuf,", VK=VK_INSERT");
		break;
	case VK_LEFT: //左矢印キー
		strcat(szBuf,", VK=VK_LEFT");
		break;
	case VK_RIGHT: //右矢印キー
		strcat(szBuf,", VK=VK_RIGHT");
		break;
	case VK_DOWN: //下矢印キー
		strcat(szBuf,", VK=VK_DOWN");
		break;
	case VK_UP:	//上矢印キー
		strcat(szBuf,", VK=VK_UP");
		break;
	case VK_HOME: //HOMEキー
		strcat(szBuf,", VK=VK_HOME");
		break;
	case VK_END: //ENDキー
		strcat(szBuf,", VK=VK_END");
		break;
	case VK_PRIOR: //PAGE UPキー
		strcat(szBuf,", VK=VK_PRIOR");
		break;
	case VK_NEXT: //PAGE DOWNキー
		strcat(szBuf,", VK=VK_NEXT");
		break;
	case VK_RETURN: //リターンキー
		strcat(szBuf,", VK=VK_RETURN");
		break;
	case VK_DELETE: //DELETEキー
		strcat(szBuf,", VK=VK_DELETE");
		break;
	case VK_BACK: //BACK SPACEキー
		strcat(szBuf,", VK=VK_BACK");
		break;
	case VK_F9: // Ｆ９キー
		strcat(szBuf,", VK=VK_F9");
		break;
	default:
		strcat(szBuf,", VK=OTHER");
		break;
	}

	strcat(m_szOperationString,szBuf);
}


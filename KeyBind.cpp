// KeyBind.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <afxpriv.h>       //SK: makes A2W and other spiffy AFX macros work
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "KeyBind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CKeyBind, CObject, 0)

/////////////////////////////////////////////////////////////////////////////
// CKeyBind

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CKeyBind::CKeyBind()
{
	m_nChar    = 0;
	m_dwMenuID = 0;
	m_bIsCtrl  = FALSE;
	m_bIsShift = FALSE;
	m_bIsAlt   = FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CKeyBind::CKeyBind
	(	CKeyBind&	objBind
	)
{
	m_nChar    = objBind.m_nChar;
	m_dwMenuID = objBind.m_dwMenuID;
	m_bIsCtrl  = objBind.m_bIsCtrl;
	m_bIsShift = objBind.m_bIsShift;
	m_bIsAlt   = objBind.m_bIsAlt;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CKeyBind::CKeyBind
	(	UINT	nChar,
		DWORD	dwMenuID,
		BOOL	bIsCtrl,
		BOOL	bIsShift,
		BOOL	bIsAlt
	)
{
	m_nChar    = nChar;
	m_dwMenuID = dwMenuID;
	m_bIsCtrl  = bIsCtrl;
	m_bIsShift = bIsShift;
	m_bIsAlt   = bIsAlt;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CKeyBind::~CKeyBind()
{
}

/////////////////////////////////////////////////////////////////////////////
// CKeyBind パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：シリアライズ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyBind::Serialize(CArchive& ar)
{
	WORD	wTemp;

	CObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << (UINT)m_nChar;
		ar << (DWORD)m_dwMenuID;
		ar << (WORD)m_bIsCtrl;
		ar << (WORD)m_bIsShift;
		ar << (WORD)m_bIsAlt;
	}
	else
	{
		ar >> m_nChar;
		ar >> m_dwMenuID;
		ar >> wTemp; m_bIsCtrl = (BOOL)wTemp;
		ar >> wTemp; m_bIsShift = (BOOL)wTemp;
		ar >> wTemp; m_bIsAlt = (BOOL)wTemp;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：代入演算子															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyBind::operator=
	(	CKeyBind&	objBind
	)
{
	m_nChar    = objBind.m_nChar;
	m_dwMenuID = objBind.m_dwMenuID;
	m_bIsCtrl  = objBind.m_bIsCtrl;
	m_bIsShift = objBind.m_bIsShift;
	m_bIsAlt   = objBind.m_bIsAlt;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：比較処理																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CKeyBind::Compare
	(	UINT	nChar,
		BOOL	bIsCtrl,
		BOOL	bIsShift,
		BOOL	bIsAlt
	)
{
	if(nChar==0 && !bIsCtrl && !bIsShift && !bIsAlt)
		return FALSE;

	if(m_nChar != nChar)
		return FALSE;
	if(m_bIsCtrl != bIsCtrl)
		return FALSE;
	if(m_bIsShift != bIsShift)
		return FALSE;
	if(m_bIsAlt != bIsAlt)
		return FALSE;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：比較処理																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CKeyBind::Compare
	(	CKeyBind&	objKeyBind
	)
{
	if(m_nChar != objKeyBind.m_nChar)
		return FALSE;
	if(m_bIsCtrl != objKeyBind.m_bIsCtrl)
		return FALSE;
	if(m_bIsShift != objKeyBind.m_bIsShift)
		return FALSE;
	if(m_bIsAlt != objKeyBind.m_bIsAlt)
		return FALSE;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：比較処理																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CKeyBind::Compare
	(	DWORD	dwMenuID
	)
{
	if(m_dwMenuID == dwMenuID)
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：メニューＩＤ更新処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyBind::UpdateMenuID
	(	DWORD	dwMenuID
	)
{
	m_dwMenuID = dwMenuID;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーバインド文字列取得処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyBind::GetKeyBindString(char* szBuf)
{
	if(m_bIsShift)
		strcat(&szBuf[strlen(szBuf)],"Shft+");
	if(m_bIsCtrl)
		strcat(&szBuf[strlen(szBuf)],"Ctl+");
	if(m_bIsAlt)
		strcat(&szBuf[strlen(szBuf)],"Alt+");

	if(m_nChar>=VK_F1 && m_nChar<=VK_F12)
		sprintf(&szBuf[strlen(szBuf)],"F%d",m_nChar-VK_F1+1);
	else if(m_nChar >= 'A' && m_nChar <= 'Z')
		sprintf(&szBuf[strlen(szBuf)],"%c",m_nChar);
	else if(m_nChar == VK_HOME)
		sprintf(&szBuf[strlen(szBuf)],"Home");
	else if(m_nChar == VK_END)
		sprintf(&szBuf[strlen(szBuf)],"End");
	else if(m_nChar == VK_LEFT)
		sprintf(&szBuf[strlen(szBuf)],"Left");
	else if(m_nChar == VK_RIGHT)
		sprintf(&szBuf[strlen(szBuf)],"Right");
	else if(m_nChar == VK_UP)
		sprintf(&szBuf[strlen(szBuf)],"Up");
	else if(m_nChar == VK_DOWN)
		sprintf(&szBuf[strlen(szBuf)],"Down");
	else if(m_nChar == VK_PRIOR)
		sprintf(&szBuf[strlen(szBuf)],"PgUp");
	else if(m_nChar == VK_NEXT)
		sprintf(&szBuf[strlen(szBuf)],"PgDn");
	else if(m_nChar == VK_INSERT)
		sprintf(&szBuf[strlen(szBuf)],"Ins");
	else if(m_nChar == VK_DELETE)
		sprintf(&szBuf[strlen(szBuf)],"Del");
	else if(m_nChar == VK_BACK)
		sprintf(&szBuf[strlen(szBuf)],"Back");
	else if(m_nChar == VK_RETURN)
		sprintf(&szBuf[strlen(szBuf)],"Ret");
	else if(m_nChar >= '0' && m_nChar <= '9')
		sprintf(&szBuf[strlen(szBuf)],"%d", m_nChar-'0');
	else
		sprintf(&szBuf[strlen(szBuf)],"%c", m_nChar);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーバインド文字列取得処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyBind::GetKeyBindString(wchar_t* szBuf)
{
USES_CONVERSION;
	if(m_bIsShift)
		wcscat(&szBuf[wcslen(szBuf)],A2W("Shft+"));
	if(m_bIsCtrl)
		wcscat(&szBuf[wcslen(szBuf)],A2W("Ctl+"));
	if(m_bIsAlt)
		wcscat(&szBuf[wcslen(szBuf)],A2W("Alt+"));

	if(m_nChar>=VK_F1 && m_nChar<=VK_F12)
		swprintf(&szBuf[wcslen(szBuf)],A2W("F%d"),m_nChar-VK_F1+1);
	else if(m_nChar >= 'A' && m_nChar <= 'Z')
		swprintf(&szBuf[wcslen(szBuf)],A2W("%c"),m_nChar);
	else if(m_nChar == VK_HOME)
		swprintf(&szBuf[wcslen(szBuf)],A2W("Home"));
	else if(m_nChar == VK_END)
		swprintf(&szBuf[wcslen(szBuf)],A2W("End"));
	else if(m_nChar == VK_LEFT)
		swprintf(&szBuf[wcslen(szBuf)],A2W("Left"));
	else if(m_nChar == VK_RIGHT)
		swprintf(&szBuf[wcslen(szBuf)],A2W("Right"));
	else if(m_nChar == VK_UP)
		swprintf(&szBuf[wcslen(szBuf)],A2W("Up"));
	else if(m_nChar == VK_DOWN)
		swprintf(&szBuf[wcslen(szBuf)],A2W("Down"));
	else if(m_nChar == VK_PRIOR)
		swprintf(&szBuf[wcslen(szBuf)],A2W("PgUp"));
	else if(m_nChar == VK_NEXT)
		swprintf(&szBuf[wcslen(szBuf)],A2W("PgDn"));
	else if(m_nChar == VK_INSERT)
		swprintf(&szBuf[wcslen(szBuf)],A2W("Ins"));
	else if(m_nChar == VK_DELETE)
		swprintf(&szBuf[wcslen(szBuf)],A2W("Del"));
	else if(m_nChar == VK_BACK)
		swprintf(&szBuf[wcslen(szBuf)],A2W("Back"));
	else if(m_nChar == VK_RETURN)
		swprintf(&szBuf[wcslen(szBuf)],A2W("Ret"));
	else if(m_nChar >= '0' && m_nChar <= '9')
		swprintf(&szBuf[wcslen(szBuf)],A2W("%d"), m_nChar-'0');
	else
		swprintf(&szBuf[wcslen(szBuf)],A2W("%c"), m_nChar);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーバインド文字列取得処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CString CKeyBind::GetKeyBindString()
{
	CString objBuf=_T("");
	char szBuf[64];

	if(m_bIsShift)
		objBuf += "Shft+";
	if(m_bIsCtrl)
		objBuf += "Ctl+";
	if(m_bIsAlt)
		objBuf += "Alt+";

	memset(szBuf,0,sizeof(szBuf));
	if(m_nChar>=VK_F1 && m_nChar<=VK_F12)
		sprintf(szBuf,"F%d",m_nChar-VK_F1+1);
	else if(m_nChar >= 'A' && m_nChar <= 'Z')
		sprintf(szBuf,"%c",m_nChar);
	else if(m_nChar == VK_HOME)
		strcpy(szBuf,"Home");
	else if(m_nChar == VK_END)
		strcpy(szBuf,"End");
	else if(m_nChar == VK_LEFT)
		strcpy(szBuf,"Left");
	else if(m_nChar == VK_RIGHT)
		strcpy(szBuf,"Right");
	else if(m_nChar == VK_UP)
		strcpy(szBuf,"Up");
	else if(m_nChar == VK_DOWN)
		strcpy(szBuf,"Down");
	else if(m_nChar == VK_PRIOR)
		strcpy(szBuf,"PgUp");
	else if(m_nChar == VK_NEXT)
		strcpy(szBuf,"PgDn");
	else if(m_nChar == VK_INSERT)
		strcpy(szBuf,"Ins");
	else if(m_nChar == VK_DELETE)
		strcpy(szBuf,"Del");
	else if(m_nChar == VK_BACK)
		strcpy(szBuf,"Back");
	else if(m_nChar == VK_RETURN)
		strcpy(szBuf,"Ret");
	else if(m_nChar >= '0' && m_nChar <= '9')
		sprintf(szBuf,"%d",m_nChar-'0');
	else
		sprintf(szBuf,"%c",m_nChar);

	objBuf += szBuf;
	return objBuf;
}


// KeyBindManager.cpp : ƒCƒ“ƒvƒŠƒƒ“ƒe[ƒVƒ‡ƒ“ ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include <afxtempl.h>		//for CTypedPtrArray
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "KeyBind.h"
#include "KeyBindManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyBindManager

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	ŠT—v	FƒRƒ“ƒXƒgƒ‰ƒNƒ^														|*/
/*|	‰ğà	F																		|*/
/*|	”õl	F																		|*/
/*|	—š—ğ	F																		|*/
/*|	I/F		F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CKeyBindManager::CKeyBindManager()
{
	m_pApp = (CEditorApp*)AfxGetApp();
	clearAllItem();

	SetDefaultKeyBind();
	ReadKeyBind();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	ŠT—v	FƒfƒXƒgƒ‰ƒNƒ^															|*/
/*|	‰ğà	F																		|*/
/*|	”õl	F																		|*/
/*|	—š—ğ	F																		|*/
/*|	I/F		F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CKeyBindManager::~CKeyBindManager()
{
	WriteKeyBind();
	clearAllItem();
}

/////////////////////////////////////////////////////////////////////////////
// CKeyBindManager ƒpƒuƒŠƒbƒNŠÖ”

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	ŠT—v	FCKeyBind ‚Ìƒtƒ@ƒCƒ‹‚©‚ç‚Ì“Ç‚İ‚İ										|*/
/*|	‰ğà	F																		|*/
/*|	”õl	F																		|*/
/*|	—š—ğ	F																		|*/
/*|	I/F		F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyBindManager::readKeyBind()
{
	CString objFilePath;
	CFile objFile;
	CFileException e;
	CString objMsg,objBuf;
	CFileDialog objFileDialog(TRUE);

	objMsg.LoadString(IDS_KEYBIND_INFILE_TITLE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if(objFileDialog.DoModal() != IDOK)
		return;
	objFilePath = objFileDialog.GetPathName();
	if(objFile.Open(objFilePath,(CFile::modeRead), &e)==FALSE)
	{
		m_pApp->GetFileErrMessage(objMsg,e);
		m_pApp->MyMessageBox((LPCTSTR)objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	CArchive ar(&objFile,CArchive::load);

	//ƒ}ƒWƒbƒNƒ[ƒh“Ç‚İ‚İ•ƒ`ƒFƒbƒN
	ar.ReadString(objBuf);
	if(objBuf.Find(KEYBIND_MAGICWORD) == -1)
	{
		objMsg.LoadString(IDS_NOT_KEYBINDFILE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		ar.Close();
		objFile.Close();
		return;
	}

	//ƒL[ƒŠƒXƒgƒNƒŠƒA
	clearAllItem();
	//ƒVƒŠƒAƒ‰ƒCƒY
	m_objKeyBindList.Serialize(ar);

	ar.Close();
	objFile.Close();

	objMsg.LoadString(IDS_KEYBIND_READEND);
	m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	ŠT—v	FCKeyBind ‚Ìƒtƒ@ƒCƒ‹‚Ö‚Ì‘‚«o‚µ										|*/
/*|	‰ğà	F																		|*/
/*|	”õl	F																		|*/
/*|	—š—ğ	F																		|*/
/*|	I/F		F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyBindManager::writeKeyBind()
{
	CString objFilePath;
	CFile objFile;
	CFileException e;
	CString objMsg,objBuf;
	CFileDialog objFileDialog(FALSE);

	objMsg.LoadString(IDS_KEYBIND_OUTFILE_TITLE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if(objFileDialog.DoModal() != IDOK)
		return;
	objFilePath = objFileDialog.GetPathName();
	if(objFile.Open(objFilePath,(CFile::modeCreate|CFile::modeWrite), &e)==FALSE)
	{
		m_pApp->GetFileErrMessage(objMsg,e);
		m_pApp->MyMessageBox((LPCTSTR)objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	CArchive ar(&objFile,CArchive::store);

	//ƒ}ƒWƒbƒNƒ[ƒh‘‚«‚İ
	objBuf = KEYBIND_MAGICWORD;
	objBuf += "\n";
	ar.WriteString(objBuf);

	//ƒVƒŠƒAƒ‰ƒCƒY
	m_objKeyBindList.Serialize(ar);

	ar.Close();
	objFile.Close();

	objMsg.LoadString(IDS_KEYBIND_WRITEEND);
	m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	ŠT—v	FCKeyBind ‚ÌƒŠƒZƒbƒg													|*/
/*|	‰ğà	F																		|*/
/*|	”õl	F																		|*/
/*|	—š—ğ	F																		|*/
/*|	I/F		F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyBindManager::resetKeyBind()
{
	POSITION pos = m_objKeyBindList.GetHeadPosition();
	CKeyBind*	pobjKeyBind = NULL;
	while (pos != NULL)
	{
		pobjKeyBind = m_objKeyBindList.GetNext(pos);
		pobjKeyBind->m_dwMenuID=0;
	}

	SetDefaultKeyBind();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	ŠT—v	FCKeyBind ‚Ì“o˜^														|*/
/*|	‰ğà	F																		|*/
/*|	”õl	F																		|*/
/*|	—š—ğ	F																		|*/
/*|	I/F		F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyBindManager::addKeyBind
	(	CKeyBind&	objKeyBind
	)
{
	CKeyBind* pBind=NULL;
//	if((pBind=FindKeyBind(objKeyBind.m_dwMenuID)) != NULL) //ƒƒjƒ…[‚h‚cˆê’v
//	{
//		(*pBind) = objKeyBind; //ƒL[Š„‚è“–‚ÄXV
//		return;
//	}
	if(FindKeyBind(objKeyBind)) //ƒL[Š„‚è“–‚Äˆê’v
	{
		UpdateKeyBind(objKeyBind); //ƒƒjƒ…[‚h‚cXV
		return;
	}
	CKeyBind* pobjKeyBind = new CKeyBind(objKeyBind);
	m_objKeyBindList.AddTail(pobjKeyBind);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	ŠT—v	FƒL[ƒoƒCƒ“ƒhŒŸõ														|*/
/*|	‰ğà	F																		|*/
/*|	”õl	F																		|*/
/*|	—š—ğ	F																		|*/
/*|	I/F		F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CKeyBind* CKeyBindManager::FindKeyBind
	(	UINT	nChar,
		BOOL	bIsCtrl,
		BOOL	bIsShift,
		BOOL	bIsAlt
	)
{
	POSITION pos = m_objKeyBindList.GetHeadPosition();
	CKeyBind*	pobjKeyBind = NULL;
	while (pos != NULL)
	{
		pobjKeyBind = m_objKeyBindList.GetNext(pos);
		if( pobjKeyBind == NULL )
			break;
		if(pobjKeyBind->Compare(nChar,bIsCtrl,bIsShift,bIsAlt))//ˆê’v
			return pobjKeyBind;
	}
	return NULL;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	ŠT—v	FƒL[ƒoƒCƒ“ƒhŒŸõ														|*/
/*|	‰ğà	F																		|*/
/*|	”õl	F																		|*/
/*|	—š—ğ	F																		|*/
/*|	I/F		F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CKeyBind* CKeyBindManager::FindKeyBind
	(	CKeyBind&	objKeyBind
	)
{
	POSITION pos = m_objKeyBindList.GetHeadPosition();
	CKeyBind*	pobjKeyBind = NULL;
	while (pos != NULL)
	{
		pobjKeyBind = m_objKeyBindList.GetNext(pos);
		if(pobjKeyBind->Compare(objKeyBind))//ˆê’v
			return pobjKeyBind;
	}
	return NULL;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	ŠT—v	FƒL[ƒoƒCƒ“ƒhŒŸõ														|*/
/*|	‰ğà	F																		|*/
/*|	”õl	F																		|*/
/*|	—š—ğ	F																		|*/
/*|	I/F		F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CKeyBind* CKeyBindManager::FindKeyBind
	(	DWORD	dwMenuID
	)
{
	if(dwMenuID==0)
		return NULL;

	POSITION pos = m_objKeyBindList.GetHeadPosition();
	CKeyBind*	pobjKeyBind = NULL;
	while (pos != NULL)
	{
		pobjKeyBind = m_objKeyBindList.GetNext(pos);
		if( pobjKeyBind == NULL )
			break;
		if(pobjKeyBind->Compare(dwMenuID))//ˆê’v
			return pobjKeyBind;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CKeyBindManager ƒvƒƒeƒNƒgŠÖ”

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	ŠT—v	FƒfƒtƒHƒ‹ƒgƒL[ƒoƒCƒ“ƒhİ’è											|*/
/*|	‰ğà	F																		|*/
/*|	”õl	F																		|*/
/*|	—š—ğ	F																		|*/
/*|	I/F		F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyBindManager::SetDefaultKeyBind()
{
	//////////////////////////
	//ƒtƒ@ƒ“ƒNƒVƒ‡ƒ“ƒL[ŠÖ˜A//
	//////////////////////////

	//F1 -> ID_HELP_CONTENT
	addKeyBind(CKeyBind(VK_F1,ID_HELP_CONTENT,FALSE,FALSE,FALSE));
	//F3 -> ID_EDIT_FINDCONTINUE
	addKeyBind(CKeyBind(VK_F3,ID_EDIT_FINDCONTINUE,FALSE,FALSE,FALSE));
	//Shift + F3 -> ID_EDIT_FINDREVCONTINUE
	addKeyBind(CKeyBind(VK_F3,ID_EDIT_FINDREVCONTINUE,FALSE,TRUE,FALSE));
	//F4 -> ID_EDIT_REPLACE
	addKeyBind(CKeyBind(VK_F4,ID_EDIT_REPLACE,FALSE,FALSE,FALSE));
	//Ctrl + F4 -> ID_WINDOW_CLOSE
	addKeyBind(CKeyBind(VK_F4,ID_WINDOW_CLOSE,TRUE,FALSE,FALSE));
	//Alt + F4 -> ID_APP_EXIT
	addKeyBind(CKeyBind(VK_F4,ID_APP_EXIT,FALSE,FALSE,TRUE));
	//F5 -> ID_VIEW_REDRAW
	addKeyBind(CKeyBind(VK_F5,ID_VIEW_REDRAW,FALSE,FALSE,FALSE));
	//F6 -> ID_NEXT_PANE
	addKeyBind(CKeyBind(VK_F6,ID_NEXT_PANE,FALSE,FALSE,FALSE));
	//Shift + F6 -> ID_PREV_PANE
	addKeyBind(CKeyBind(VK_F6,ID_PREV_PANE,FALSE,TRUE,FALSE));
	//Ctrl + F6 -> ID_WND_NEXT
	addKeyBind(CKeyBind(VK_F6,ID_WND_NEXT,TRUE,FALSE,FALSE));
	//Shift + Ctrl + F6 -> ID_WND_PREV
	addKeyBind(CKeyBind(VK_F6,ID_WND_PREV,TRUE,TRUE,FALSE));

	//////////////////////
	//ƒAƒ‹ƒtƒ@ƒxƒbƒgŠÖ˜A//
	//////////////////////

	//Ctrl + A -> ID_EDIT_SELECTALL
	addKeyBind(CKeyBind('A',ID_EDIT_SELECTALL,TRUE,FALSE,FALSE));
	//Ctrl + C -> ID_EDIT_COPY
	addKeyBind(CKeyBind('C',ID_EDIT_COPY,TRUE,FALSE,FALSE));
	//Ctrl + F -> ID_EDIT_FIND
	addKeyBind(CKeyBind('F',ID_EDIT_FIND,TRUE,FALSE,FALSE));
	//Ctrl + H -> ID_EDIT_REPLACE
	addKeyBind(CKeyBind('H',ID_EDIT_REPLACE,TRUE,FALSE,FALSE));
	//Ctrl + I -> ID_EDIT_INCRIMENTALSEARCH
	addKeyBind(CKeyBind('I',ID_EDIT_INCRIMENTALSEARCH,TRUE,FALSE,FALSE));
	//Ctrl + N -> ID_FILE_NEW
	addKeyBind(CKeyBind('N',ID_FILE_NEW,TRUE,FALSE,FALSE));
	//Ctrl + O -> ID_FILE_OPEN
	addKeyBind(CKeyBind('O',ID_FILE_OPEN,TRUE,FALSE,FALSE));
	//Ctrl + P -> ID_FILE_PRINT
	addKeyBind(CKeyBind('P',ID_FILE_PRINT,TRUE,FALSE,FALSE));
	//Ctrl + S -> ID_FILE_SAVE
	addKeyBind(CKeyBind('S',ID_FILE_SAVE,TRUE,FALSE,FALSE));
	//Ctrl + V -> ID_EDIT_PASTE
	addKeyBind(CKeyBind('V',ID_EDIT_PASTE,TRUE,FALSE,FALSE));
	//Ctrl + X -> ID_EDIT_CUT
	addKeyBind(CKeyBind('X',ID_EDIT_CUT,TRUE,FALSE,FALSE));
	//Ctrl + Y -> ID_EDIT_REDO
	addKeyBind(CKeyBind('Y',ID_EDIT_REDO,TRUE,FALSE,FALSE));
	//Ctrl + Z -> ID_EDIT_UNDO
	addKeyBind(CKeyBind('Z',ID_EDIT_UNDO,TRUE,FALSE,FALSE));

	////////////////
	//‚»‚Ì‘¼‚ÌƒL[//
	////////////////

	//Ctrl + VK_END -> ID_EDIT_JUMP2BOTTOM
	addKeyBind(CKeyBind(VK_END,ID_EDIT_JUMP2BOTTOM,TRUE,FALSE,FALSE));
	//Ctrl + Shift + VK_END -> ID_EDIT_JUMP2BOTTOMSELECT
	addKeyBind(CKeyBind(VK_END,ID_EDIT_JUMP2BOTTOMSELECT,TRUE,TRUE,FALSE));
	//Ctrl + VK_HOME -> ID_EDIT_JUMP2TOP
	addKeyBind(CKeyBind(VK_HOME,ID_EDIT_JUMP2TOP,TRUE,FALSE,FALSE));
	//Ctrl + Shift + VK_HOME -> ID_EDIT_JUMP2TOPSELECT
	addKeyBind(CKeyBind(VK_HOME,ID_EDIT_JUMP2TOPSELECT,TRUE,TRUE,FALSE));
	//Ctrl + VK_LEFT -> ID_WORDSKIP2LEFT
	addKeyBind(CKeyBind(VK_LEFT,ID_WORDSKIP2LEFT,TRUE,FALSE,FALSE));
	//Ctrl + Shift + VK_LEFT -> ID_WORDSELECT2LEFT
	addKeyBind(CKeyBind(VK_LEFT,ID_WORDSELECT2LEFT,TRUE,TRUE,FALSE));
	//Ctrl + VK_RIGHT -> ID_WORDSKIP2RIGHT
	addKeyBind(CKeyBind(VK_RIGHT,ID_WORDSKIP2RIGHT,TRUE,FALSE,FALSE));
	//Ctrl + Shift + VK_RIGHT -> ID_WORDSELECT2RIGHT
	addKeyBind(CKeyBind(VK_RIGHT,ID_WORDSELECT2RIGHT,TRUE,TRUE,FALSE));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	ŠT—v	FƒL[ƒoƒCƒ“ƒh“Ç‚İ‚İ													|*/
/*|	‰ğà	F																		|*/
/*|	”õl	F																		|*/
/*|	—š—ğ	F																		|*/
/*|	I/F		F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyBindManager::ReadKeyBind()
{
	int		i=0;
	UINT	nChar=0;
	DWORD	dwMenuID=0;
	char	szBuf[64];

	//ƒtƒ@ƒ“ƒNƒVƒ‡ƒ“ƒL[i‚e‚P`‚e‚P‚QjŠÖ˜A
	for(i=0; i<12; i++)
	{
		//ƒL[Cü‚È‚µ
		sprintf(szBuf,"F%d",i+1);
		dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
		if(dwMenuID != 0)
			addKeyBind(CKeyBind(VK_F1+i,dwMenuID,FALSE,FALSE,FALSE));
		if((i+1)!=4 || (i+1)==6)//Alt+F4,Alt+F6‚Íœ‚­
		{
			//‚`‚Œ‚”ƒL[Cü‚ ‚è
			sprintf(szBuf,"Alt+F%d",i+1);
			dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
			if(dwMenuID != 0)
				addKeyBind(CKeyBind(VK_F1+i,dwMenuID,FALSE,FALSE,TRUE));
		}
		//‚r‚ˆ‚‰‚†‚”ƒL[Cü‚ ‚è
		sprintf(szBuf,"Shft+F%d",i+1);
		dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
		if(dwMenuID != 0)
			addKeyBind(CKeyBind(VK_F1+i,dwMenuID,FALSE,TRUE,FALSE));
		if((i+1)!=4 && (i+1)!=6) //Ctrl+F4,Ctrl+F6‚Íœ‚­
		{
			//‚b‚”‚’‚ŒƒL[Cü‚ ‚è
			sprintf(szBuf,"Ctl+F%d",i+1);
			dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
			if(dwMenuID != 0)
				addKeyBind(CKeyBind(VK_F1+i,dwMenuID,TRUE,FALSE,FALSE));
		}
		//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚ŒƒL[Cü‚ ‚è
		sprintf(szBuf,"Shft+Ctl+F%d",i+1);
		dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
		if(dwMenuID != 0)
			addKeyBind(CKeyBind(VK_F1+i,dwMenuID,TRUE,TRUE,FALSE));
		//‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è
		sprintf(szBuf,"Ctl+Alt+F%d",i+1);
		dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
		if(dwMenuID != 0)
			addKeyBind(CKeyBind(VK_F1+i,dwMenuID,TRUE,FALSE,TRUE));
		if((i+1)!=6) //Shift+Alt+F6‚Íœ‚­
		{
			//‚r‚ˆ‚‰‚†‚”{‚`‚Œ‚”ƒL[Cü‚ ‚è
			sprintf(szBuf,"Shft+Alt+F%d",i+1);
			dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
			if(dwMenuID != 0)
				addKeyBind(CKeyBind(VK_F1+i,dwMenuID,FALSE,TRUE,TRUE));
		}
		//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è
		sprintf(szBuf,"Shft+Ctl+Alt+F%d",i+1);
		dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
		if(dwMenuID != 0)
			addKeyBind(CKeyBind(VK_F1+i,dwMenuID,TRUE,TRUE,TRUE));
	}

	//ƒAƒ‹ƒtƒ@ƒxƒbƒgŠÖ˜A
	for(nChar='A'; nChar<='Z'; nChar++)
	{
		//‚b‚”‚’‚ŒƒL[Cü‚ ‚è
		sprintf(szBuf,"Ctl+%c",nChar);
		dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
		if(dwMenuID != 0)
			addKeyBind(CKeyBind(nChar,dwMenuID,TRUE,FALSE,FALSE));
		//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚ŒƒL[Cü‚ ‚è
		sprintf(szBuf,"Shft+Ctl+%c",nChar);
		dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
		if(dwMenuID != 0)
			addKeyBind(CKeyBind(nChar,dwMenuID,TRUE,TRUE,FALSE));
	}

	//”šŠÖ˜A
	for(nChar='0'; nChar<='9'; nChar++)
	{
		//‚`‚Œ‚”ƒL[Cü‚ ‚è
		sprintf(szBuf,"Alt+%c",nChar);
		dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
		if(dwMenuID != 0)
			addKeyBind(CKeyBind(nChar,dwMenuID,FALSE,FALSE,TRUE));
		//‚b‚”‚’‚ŒƒL[Cü‚ ‚è
		sprintf(szBuf,"Ctl+%c",nChar);
		dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
		if(dwMenuID != 0)
			addKeyBind(CKeyBind(nChar,dwMenuID,TRUE,FALSE,FALSE));
		//‚b‚”‚’‚Œ{‚r‚ˆ‚‰‚†‚”ƒL[Cü‚ ‚è
		sprintf(szBuf,"Ctl+Shft+%c",nChar);
		dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
		if(dwMenuID != 0)
			addKeyBind(CKeyBind(nChar,dwMenuID,TRUE,TRUE,FALSE));
		//‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è
		sprintf(szBuf,"Ctl+Alt+%c",nChar);
		dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
		if(dwMenuID != 0)
			addKeyBind(CKeyBind(nChar,dwMenuID,TRUE,FALSE,TRUE));
		//‚r‚ˆ‚‰‚†‚”{‚`‚Œ‚”ƒL[Cü‚ ‚è
		sprintf(szBuf,"Shft+Alt+%c",nChar);
		dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
		if(dwMenuID != 0)
			addKeyBind(CKeyBind(nChar,dwMenuID,FALSE,TRUE,TRUE));
		//‚b‚”‚’‚Œ{‚r‚ˆ‚‰‚†‚”{‚`‚Œ‚”ƒL[Cü‚ ‚è
		sprintf(szBuf,"Shft+Ctl+Alt+%c",nChar);
		dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
		if(dwMenuID != 0)
			addKeyBind(CKeyBind(nChar,dwMenuID,TRUE,TRUE,TRUE));
	}

	////////////////////
	//‚»‚Ì‘¼i‚d‚m‚cj//
	////////////////////

	//‚`‚Œ‚”ƒL[Cü‚ ‚è‚d‚m‚c
	strcpy(szBuf,"Alt+End");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_END,dwMenuID,FALSE,FALSE,TRUE));
	//‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚d‚m‚c
	strcpy(szBuf,"Ctl+End");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_END,dwMenuID,TRUE,FALSE,FALSE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚d‚m‚c
	strcpy(szBuf,"Shft+Ctl+End");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_END,dwMenuID,TRUE,TRUE,FALSE));
	//‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚d‚m‚c
	strcpy(szBuf,"Ctl+Alt+End");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_END,dwMenuID,TRUE,FALSE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚`‚Œ‚”ƒL[Cü‚ ‚è‚d‚m‚c
	strcpy(szBuf,"Shft+Alt+End");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_END,dwMenuID,FALSE,TRUE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚d‚m‚c
	strcpy(szBuf,"Shft+Ctl+Alt+End");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_END,dwMenuID,TRUE,TRUE,TRUE));

	//////////////////////
	//‚»‚Ì‘¼i‚g‚n‚l‚dj//
	//////////////////////

	//‚`‚Œ‚”ƒL[Cü‚ ‚è‚g‚n‚l‚d
	strcpy(szBuf,"Alt+Home");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_HOME,dwMenuID,FALSE,FALSE,TRUE));
	//‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚g‚n‚l‚d
	strcpy(szBuf,"Ctl+Home");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_HOME,dwMenuID,TRUE,FALSE,FALSE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚g‚n‚l‚d
	strcpy(szBuf,"Shft+Ctl+Home");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_HOME,dwMenuID,TRUE,TRUE,FALSE));
	//‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚g‚n‚l‚d
	strcpy(szBuf,"Ctl+Alt+Home");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_HOME,dwMenuID,TRUE,FALSE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚`‚Œ‚”ƒL[Cü‚ ‚è‚g‚n‚l‚d
	strcpy(szBuf,"Shft+Alt+Home");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_HOME,dwMenuID,FALSE,TRUE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚g‚n‚l‚d
	strcpy(szBuf,"Shft+Ctl+Alt+Home");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_HOME,dwMenuID,TRUE,TRUE,TRUE));

	//////////////////////
	//‚»‚Ì‘¼i‚k‚d‚e‚sj//
	//////////////////////

	//‚`‚Œ‚”ƒL[Cü‚ ‚è‚k‚d‚e‚s
	strcpy(szBuf,"Alt+Left");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_LEFT,dwMenuID,FALSE,FALSE,TRUE));
	//‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚k‚d‚e‚s
	strcpy(szBuf,"Ctl+Left");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_LEFT,dwMenuID,TRUE,FALSE,FALSE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚k‚d‚e‚s
	strcpy(szBuf,"Shft+Ctl+Left");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_LEFT,dwMenuID,TRUE,TRUE,FALSE));
	//‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚k‚d‚e‚s
	strcpy(szBuf,"Ctl+Alt+Left");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_LEFT,dwMenuID,TRUE,FALSE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚`‚Œ‚”ƒL[Cü‚ ‚è‚k‚d‚e‚s
	strcpy(szBuf,"Shft+Alt+Left");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_LEFT,dwMenuID,FALSE,TRUE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚k‚d‚e‚s
	strcpy(szBuf,"Shft+Ctl+Alt+Left");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_LEFT,dwMenuID,TRUE,TRUE,TRUE));

	////////////////////////
	//‚»‚Ì‘¼i‚q‚h‚f‚g‚sj//
	////////////////////////

	//‚`‚Œ‚”ƒL[Cü‚ ‚è‚q‚h‚f‚g‚s
	strcpy(szBuf,"Alt+Right");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_RIGHT,dwMenuID,FALSE,FALSE,TRUE));
	//‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚q‚h‚f‚g‚s
	strcpy(szBuf,"Ctl+Right");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_RIGHT,dwMenuID,TRUE,FALSE,FALSE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚q‚h‚f‚g‚s
	strcpy(szBuf,"Shft+Ctl+Right");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_RIGHT,dwMenuID,TRUE,TRUE,FALSE));
	//‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚q‚h‚f‚g‚s
	strcpy(szBuf,"Ctl+Alt+Right");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_RIGHT,dwMenuID,TRUE,FALSE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚`‚Œ‚”ƒL[Cü‚ ‚è‚q‚h‚f‚g‚s
	strcpy(szBuf,"Shft+Alt+Right");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_RIGHT,dwMenuID,FALSE,TRUE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚q‚h‚f‚g‚s
	strcpy(szBuf,"Shft+Ctl+Alt+Right");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_RIGHT,dwMenuID,TRUE,TRUE,TRUE));

	//////////////////
	//‚»‚Ì‘¼i‚t‚oj//
	//////////////////

	//‚`‚Œ‚”ƒL[Cü‚ ‚è‚t‚o
	strcpy(szBuf,"Alt+Up");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_UP,dwMenuID,FALSE,FALSE,TRUE));
	//‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚t‚o
	strcpy(szBuf,"Ctl+Up");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_UP,dwMenuID,TRUE,FALSE,FALSE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚t‚o
	strcpy(szBuf,"Shft+Ctl+Up");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_UP,dwMenuID,TRUE,TRUE,FALSE));
	//‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚t‚o
	strcpy(szBuf,"Ctl+Alt+Up");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_UP,dwMenuID,TRUE,FALSE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚`‚Œ‚”ƒL[Cü‚ ‚è‚t‚o
	strcpy(szBuf,"Shft+Alt+Up");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_UP,dwMenuID,FALSE,TRUE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚t‚o
	strcpy(szBuf,"Shft+Ctl+Alt+Up");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_UP,dwMenuID,TRUE,TRUE,TRUE));

	//////////////////////
	//‚»‚Ì‘¼i‚c‚n‚v‚mj//
	//////////////////////

	//‚`‚Œ‚”ƒL[Cü‚ ‚è‚c‚n‚v‚m
	strcpy(szBuf,"Alt+Down");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_DOWN,dwMenuID,FALSE,FALSE,TRUE));
	//‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚c‚n‚v‚m
	strcpy(szBuf,"Ctl+Down");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_DOWN,dwMenuID,TRUE,FALSE,FALSE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚c‚n‚v‚m
	strcpy(szBuf,"Shft+Ctl+Down");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_DOWN,dwMenuID,TRUE,TRUE,FALSE));
	//‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚c‚n‚v‚m
	strcpy(szBuf,"Ctl+Alt+Down");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_DOWN,dwMenuID,TRUE,FALSE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚`‚Œ‚”ƒL[Cü‚ ‚è‚c‚n‚v‚m
	strcpy(szBuf,"Shft+Alt+Down");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_DOWN,dwMenuID,FALSE,TRUE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚c‚n‚v‚m
	strcpy(szBuf,"Shft+Ctl+Alt+Down");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_DOWN,dwMenuID,TRUE,TRUE,TRUE));

	//////////////////////////
	//‚»‚Ì‘¼i‚o‚‚‡‚…‚t‚j//
	//////////////////////////

	//‚`‚Œ‚”ƒL[Cü‚ ‚è‚o‚‚‡‚…‚t‚
	strcpy(szBuf,"Alt+PgUp");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_PRIOR,dwMenuID,FALSE,FALSE,TRUE));
	//‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚o‚‚‡‚…‚t‚
	strcpy(szBuf,"Ctl+PgUp");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_PRIOR,dwMenuID,TRUE,FALSE,FALSE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚o‚‚‡‚…‚t‚
	strcpy(szBuf,"Shft+Ctl+PgUp");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_PRIOR,dwMenuID,TRUE,TRUE,FALSE));
	//‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚o‚‚‡‚…‚t‚
	strcpy(szBuf,"Ctl+Alt+PgUp");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_PRIOR,dwMenuID,TRUE,FALSE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚`‚Œ‚”ƒL[Cü‚ ‚è‚o‚‚‡‚…‚t‚
	strcpy(szBuf,"Shft+Alt+PgUp");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_PRIOR,dwMenuID,FALSE,TRUE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚o‚‚‡‚…‚t‚
	strcpy(szBuf,"Shft+Ctl+Alt+PgUp");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_PRIOR,dwMenuID,TRUE,TRUE,TRUE));

	//////////////////////////////
	//‚»‚Ì‘¼i‚o‚‚‡‚…‚c‚‚—‚j//
	//////////////////////////////

	//‚`‚Œ‚”ƒL[Cü‚ ‚è‚o‚‚‡‚…‚c‚‚—‚
	strcpy(szBuf,"Alt+PgDn");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_NEXT,dwMenuID,FALSE,FALSE,TRUE));
	//‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚o‚‚‡‚…‚c‚‚—‚
	strcpy(szBuf,"Ctl+PgDn");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_NEXT,dwMenuID,TRUE,FALSE,FALSE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚o‚‚‡‚…‚c‚‚—‚
	strcpy(szBuf,"Shft+Ctl+PgDn");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_NEXT,dwMenuID,TRUE,TRUE,FALSE));
	//‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚o‚‚‡‚…‚c‚‚—‚
	strcpy(szBuf,"Ctl+Alt+PgDn");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_NEXT,dwMenuID,TRUE,FALSE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚`‚Œ‚”ƒL[Cü‚ ‚è‚o‚‚‡‚…‚c‚‚—‚
	strcpy(szBuf,"Shft+Alt+PgDn");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_NEXT,dwMenuID,FALSE,TRUE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚o‚‚‡‚…‚c‚‚—‚
	strcpy(szBuf,"Shft+Ctl+Alt+PgDn");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_NEXT,dwMenuID,TRUE,TRUE,TRUE));

	//////////////////////////
	//‚»‚Ì‘¼i‚h‚‚“‚…‚’‚”j//
	//////////////////////////

	//‚`‚Œ‚”ƒL[Cü‚ ‚è‚h‚‚“‚…‚’‚”
	strcpy(szBuf,"Alt+Ins");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_INSERT,dwMenuID,FALSE,FALSE,TRUE));
	//‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚h‚‚“‚…‚’‚”
	strcpy(szBuf,"Ctl+Ins");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_INSERT,dwMenuID,TRUE,FALSE,FALSE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚h‚‚“‚…‚’‚”
	strcpy(szBuf,"Shft+Ctl+Ins");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_INSERT,dwMenuID,TRUE,TRUE,FALSE));
	//‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚h‚‚“‚…‚’‚”
	strcpy(szBuf,"Ctl+Alt+Ins");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_INSERT,dwMenuID,TRUE,FALSE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚`‚Œ‚”ƒL[Cü‚ ‚è‚h‚‚“‚…‚’‚”
	strcpy(szBuf,"Shft+Alt+Ins");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_INSERT,dwMenuID,FALSE,TRUE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚h‚‚“‚…‚’‚”
	strcpy(szBuf,"Shft+Ctl+Alt+Ins");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_INSERT,dwMenuID,TRUE,TRUE,TRUE));

	//////////////////////////
	//‚»‚Ì‘¼i‚c‚…‚Œ‚…‚”‚…j//
	//////////////////////////

	//‚`‚Œ‚”ƒL[Cü‚ ‚è‚c‚…‚Œ‚…‚”‚…
	strcpy(szBuf,"Alt+Del");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_DELETE,dwMenuID,FALSE,FALSE,TRUE));
	//‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚c‚…‚Œ‚…‚”‚…
	strcpy(szBuf,"Ctl+Del");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_DELETE,dwMenuID,TRUE,FALSE,FALSE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚c‚…‚Œ‚…‚”‚…
	strcpy(szBuf,"Shft+Ctl+Del");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_DELETE,dwMenuID,TRUE,TRUE,FALSE));
//	//‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚c‚…‚Œ‚…‚”‚…
//	strcpy(szBuf,"Ctl+Alt+Del");
//	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
//	if(dwMenuID != 0)
//		addKeyBind(CKeyBind(VK_DELETE,dwMenuID,TRUE,FALSE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚`‚Œ‚”ƒL[Cü‚ ‚è‚c‚…‚Œ‚…‚”‚…
	strcpy(szBuf,"Shft+Alt+Del");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_DELETE,dwMenuID,FALSE,TRUE,TRUE));
//	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚c‚…‚Œ‚…‚”‚…
//	strcpy(szBuf,"Shft+Ctl+Alt+Del");
//	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
//	if(dwMenuID != 0)
//		addKeyBind(CKeyBind(VK_DELETE,dwMenuID,TRUE,TRUE,TRUE));

	////////////////////////////////
	//‚»‚Ì‘¼i‚a‚‚ƒ‚‹‚r‚‚‚ƒ‚…j//
	////////////////////////////////

	//‚`‚Œ‚”ƒL[Cü‚ ‚è‚a‚‚ƒ‚‹‚r‚‚‚ƒ‚…
	strcpy(szBuf,"Alt+Back");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_BACK,dwMenuID,FALSE,FALSE,TRUE));
	//‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚a‚‚ƒ‚‹‚r‚‚‚ƒ‚…
	strcpy(szBuf,"Ctl+Back");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_BACK,dwMenuID,TRUE,FALSE,FALSE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚a‚‚ƒ‚‹‚r‚‚‚ƒ‚…
	strcpy(szBuf,"Shft+Ctl+Back");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_BACK,dwMenuID,TRUE,TRUE,FALSE));
	//‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚a‚‚ƒ‚‹‚r‚‚‚ƒ‚…
	strcpy(szBuf,"Ctl+Alt+Back");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_BACK,dwMenuID,TRUE,FALSE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚`‚Œ‚”ƒL[Cü‚ ‚è‚a‚‚ƒ‚‹‚r‚‚‚ƒ‚…
	strcpy(szBuf,"Shft+Alt+Back");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_BACK,dwMenuID,FALSE,TRUE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚a‚‚ƒ‚‹‚r‚‚‚ƒ‚…
	strcpy(szBuf,"Shft+Ctl+Alt+Back");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_BACK,dwMenuID,TRUE,TRUE,TRUE));

	//////////////////////////
	//‚»‚Ì‘¼i‚q‚…‚”‚•‚’‚j//
	//////////////////////////

	//‚`‚Œ‚”ƒL[Cü‚ ‚è‚q‚…‚”‚•‚’‚
	strcpy(szBuf,"Alt+Ret");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_RETURN,dwMenuID,FALSE,FALSE,TRUE));
	//‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚q‚…‚”‚•‚’‚
	strcpy(szBuf,"Ctl+Ret");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_RETURN,dwMenuID,TRUE,FALSE,FALSE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚ŒƒL[Cü‚ ‚è‚q‚…‚”‚•‚’‚
	strcpy(szBuf,"Shft+Ctl+Ret");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_RETURN,dwMenuID,TRUE,TRUE,FALSE));
	//‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚q‚…‚”‚•‚’‚
	strcpy(szBuf,"Ctl+Alt+Ret");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_RETURN,dwMenuID,TRUE,FALSE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚`‚Œ‚”ƒL[Cü‚ ‚è‚q‚…‚”‚•‚’‚
	strcpy(szBuf,"Shft+Alt+Ret");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_RETURN,dwMenuID,FALSE,TRUE,TRUE));
	//‚r‚ˆ‚‰‚†‚”{‚b‚”‚’‚Œ{‚`‚Œ‚”ƒL[Cü‚ ‚è‚q‚…‚”‚•‚’‚
	strcpy(szBuf,"Shft+Ctl+Alt+Ret");
	dwMenuID = m_pApp->GetProfileInt("KeyBindManager",szBuf,0);
	if(dwMenuID != 0)
		addKeyBind(CKeyBind(VK_RETURN,dwMenuID,TRUE,TRUE,TRUE));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	ŠT—v	FƒL[ƒoƒCƒ“ƒh‘‚«‚İ													|*/
/*|	‰ğà	F																		|*/
/*|	”õl	F																		|*/
/*|	—š—ğ	F																		|*/
/*|	I/F		F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyBindManager::WriteKeyBind()
{
	char szBuf[64];
	POSITION pos = m_objKeyBindList.GetHeadPosition();
	CKeyBind*	pobjKeyBind = NULL;
	while (pos != NULL)
	{
		pobjKeyBind = m_objKeyBindList.GetNext(pos);
		if( pobjKeyBind == NULL )
			break;

		//ƒL[•¶š—ñæ“¾
		memset(szBuf,0,sizeof(szBuf));
		pobjKeyBind->GetKeyBindString(szBuf);
		//ƒŒƒWƒXƒgƒŠ‘‚«‚İ
		m_pApp->WriteProfileInt("KeyBindManager",szBuf,pobjKeyBind->m_dwMenuID);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	ŠT—v	FƒL[ƒoƒCƒ“ƒhXV														|*/
/*|	‰ğà	F																		|*/
/*|	”õl	F																		|*/
/*|	—š—ğ	F																		|*/
/*|	I/F		F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyBindManager::UpdateKeyBind
	(	CKeyBind&	objBind
	)
{
	POSITION pos = m_objKeyBindList.GetHeadPosition();
	CKeyBind*	pobjKeyBind = NULL;
	while (pos != NULL)
	{
		pobjKeyBind = m_objKeyBindList.GetNext(pos);
		if( pobjKeyBind == NULL )
			break;
		if(pobjKeyBind->Compare(objBind))//ˆê’v
		{
			pobjKeyBind->UpdateMenuID(objBind.m_dwMenuID);
			break;
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	ŠT—v	FCKeyBind ƒŠƒXƒg‚ÌƒNƒŠƒA												|*/
/*|	‰ğà	F																		|*/
/*|	”õl	F																		|*/
/*|	—š—ğ	F																		|*/
/*|	I/F		F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyBindManager::clearAllItem()
{
	POSITION pos = m_objKeyBindList.GetHeadPosition();
	while (pos != NULL)
		delete m_objKeyBindList.GetNext(pos);
	m_objKeyBindList.RemoveAll();
}


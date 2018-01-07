// Operation.cpp: COperation �N���X�̃C���v�������e�[�V����
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
// COperation �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COperation::COperation()
{
	m_pApp = (CEditorApp*)AfxGetApp();
	memset(m_szOperationString,0,sizeof(m_szOperationString));
	m_lOperationKind = 0;

	//CView::OnKeyDown�֘A�p�����[�^
	m_nChar_OnKeyDown = 0;
	m_nRepCnt_OnKeyDown = 0;
	m_nFlags_OnKeyDown = 0;
	m_nMultValueOfCaretAccelarete = 0;

	//CView::OnKeyUp�֘A�p�����[�^
	m_nChar_OnKeyUp = 0;
	m_nRepCnt_OnKeyUp = 0;
	m_nFlags_OnKeyUp = 0;

	//CView::OnChar�֘A�p�����[�^
	m_nChar_OnChar = 0;
	m_nRepCnt_OnChar = 0;
	m_nFlags_OnChar = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COperation::~COperation()
{
}

/////////////////////////////////////////////////////////////////////////////
// COperation �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�V���A���C�Y															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperation::Serialize(CArchive& ar)
{
	CString objBuf=_T("");

	CObject::Serialize(ar);
	if (ar.IsStoring())
	{
		objBuf = m_szOperationString;
		ar << objBuf;
		ar << m_lOperationKind;// �I�y���[�V�������

		//CView::OnKeyDown�֘A�p�����[�^
		ar << m_nChar_OnKeyDown;
		ar << m_nRepCnt_OnKeyDown;
		ar << m_nFlags_OnKeyDown;
		ar << m_nMultValueOfCaretAccelarete;

		//CView::OnKeyUp�֘A�p�����[�^
		ar << m_nChar_OnKeyUp;
		ar << m_nRepCnt_OnKeyUp;
		ar << m_nFlags_OnKeyUp;

		//CView::OnChar�֘A�p�����[�^
		ar << m_nChar_OnChar;
		ar << m_nRepCnt_OnChar;
		ar << m_nFlags_OnChar;
	}
	else
	{
		ar >> objBuf;
		strcpy(m_szOperationString,(LPCTSTR)objBuf);
		ar >> m_lOperationKind;// �I�y���[�V�������

		//CView::OnKeyDown�֘A�p�����[�^
		ar >> m_nChar_OnKeyDown;
		ar >> m_nRepCnt_OnKeyDown;
		ar >> m_nFlags_OnKeyDown;
		ar >> m_nMultValueOfCaretAccelarete;

		//CView::OnKeyUp�֘A�p�����[�^
		ar >> m_nChar_OnKeyUp;
		ar >> m_nRepCnt_OnKeyUp;
		ar >> m_nFlags_OnKeyUp;

		//CView::OnChar�֘A�p�����[�^
		ar >> m_nChar_OnChar;
		ar >> m_nRepCnt_OnChar;
		ar >> m_nFlags_OnChar;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�y���[�V������ʐݒ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperation::SetOperationKind
	(	long	lOperationKind
	)
{
	m_lOperationKind = lOperationKind;

	//�I�y���[�V�������������e�L�X�g�ݒ�
	if(m_lOperationKind == KEYOPE_ONKEYDOWN)
		strcpy(m_szOperationString,"ONKEYDOWN�@�F�@");
	else if(m_lOperationKind == KEYOPE_ONKEYUP)
		strcpy(m_szOperationString,"ONKEYUP�@�@�@�F�@");
	else if(m_lOperationKind == KEYOPE_ONCHAR)
		strcpy(m_szOperationString,"ONCHAR�@�@�@�@�F�@");
	else if(m_lOperationKind == KEYOPE_ONEDITCOPY)
		strcpy(m_szOperationString,"ONEDITCOPY�F�@");
	else if(m_lOperationKind == KEYOPE_ONEDITCUT)
		strcpy(m_szOperationString,"ONEDITCUT�@�F�@");
	else if(m_lOperationKind == KEYOPE_ONEDITPASTE)
		strcpy(m_szOperationString,"ONEDITPASTE�F�@");
	else if(m_lOperationKind == KEYOPE_ONEDITDEL)
		strcpy(m_szOperationString,"ONEDITDEL�@�F�@");
	else if(m_lOperationKind == KEYOPE_ONEDITSELECTALL)
		strcpy(m_szOperationString,"ONEDITSELECTALL�F�@");
	else if(m_lOperationKind == KEYOPE_ONEDITTAB2SPACE)
		strcpy(m_szOperationString,"ONEDITTAB2SPACE�F");
	else if(m_lOperationKind == KEYOPE_ONEDITSPACE2TAB)
		strcpy(m_szOperationString,"ONEDITSPACE2TAB�F");
	else if(m_lOperationKind == KEYOPE_ONEDITSPACEZEN2HAN)
		strcpy(m_szOperationString,"ONEDITSPACEZEN2HAN�F");
	else if(m_lOperationKind == KEYOPE_ONEDITSPACEHAN2ZEN)
		strcpy(m_szOperationString,"ONEDITSPACEHAN2ZEN�F");
	else if(m_lOperationKind == KEYOPE_ONEDITKATAKANAHAN2ZEN)
		strcpy(m_szOperationString,"ONEDITKATAKANAHAN2ZEN�F");
	else if(m_lOperationKind == KEYOPE_ONEDITKATAKANAZEN2HAN)
		strcpy(m_szOperationString,"ONEDITKATAKANAZEN2HAN�F");
	else if(m_lOperationKind == KEYOPE_ONEDITALNUMHAN2ZEN)
		strcpy(m_szOperationString,"ONEDITALNUMHAN2ZEN�F");
	else if(m_lOperationKind == KEYOPE_ONEDITALNUMZEN2HAN)
		strcpy(m_szOperationString,"ONEDITALNUMZEN2HAN�F");
	else if(m_lOperationKind == KEYOPE_ONEDITDELTOPSPACES)
		strcpy(m_szOperationString,"ONEDITDELTOPSPACES�F");
	else if(m_lOperationKind == KEYOPE_ONEDITINSTAB)
		strcpy(m_szOperationString,"ONEDITINSTAB�F");
	else if(m_lOperationKind == KEYOPE_ONEDITDELTAB)
		strcpy(m_szOperationString,"ONEDITDELTAB�F");
	else if(m_lOperationKind == KEYOPE_ONEDITJUMP2TOP)
		strcpy(m_szOperationString,"ONEDITJUMP2TOP�F");
	else if(m_lOperationKind == KEYOPE_ONEDITJUMP2BOTTOM)
		strcpy(m_szOperationString,"ONEDITJUMP2BOTTOM�F");
	else if(m_lOperationKind == KEYOPE_ONPARENTHESIS)
		strcpy(m_szOperationString,"ONPARENTHESIS�F");
	else if(m_lOperationKind == KEYOPE_ONEDITINSCPPCOMMENT)
		strcpy(m_szOperationString,"ONEDITINSCPPCOMMENT�F");
	else if(m_lOperationKind == KEYOPE_ONEDITINSVBCOMMENT)
		strcpy(m_szOperationString,"ONEDITINSVBCOMMENT�F");
	else if(m_lOperationKind == KEYOPE_ONEDITINSASMCOMMENT)
		strcpy(m_szOperationString,"ONEDITINSASMCOMMENT�F");
	else if(m_lOperationKind == KEYOPE_ONEDITINSDAY)
		strcpy(m_szOperationString,"ONEDITINSDAY�F");
	else if(m_lOperationKind == KEYOPE_ONEDITINSTIME)
		strcpy(m_szOperationString,"ONEDITINSTIME�F");
	else if(m_lOperationKind == KEYOPE_ONJUMPNEXTMARK)
		strcpy(m_szOperationString,"ONJUMPNEXTMARK�F");
	else if(m_lOperationKind == KEYOPE_ONJUMPPREVMARK)
		strcpy(m_szOperationString,"ONJUMPPREVMARK�F");
	else if(m_lOperationKind == KEYOPE_ONJUMPMARKLIST)
		strcpy(m_szOperationString,"ONJUMPMARKLIST�F");
	else if(m_lOperationKind == KEYOPE_ONEDITAL2UPPER)
		strcpy(m_szOperationString,"ONEDITAL2UPPER�F");
	else if(m_lOperationKind == KEYOPE_ONEDITAL2LOWER)
		strcpy(m_szOperationString,"ONEDITAL2LOWER�F");
	else if(m_lOperationKind == KEYOPE_ONWORDSKIP2RIGHT)
		strcpy(m_szOperationString,"ONWORDSKIP2RIGHT�F");
	else if(m_lOperationKind == KEYOPE_ONWORDSKIP2LEFT)
		strcpy(m_szOperationString,"ONWORDSKIP2LEFT�F");
	else if(m_lOperationKind == KEYOPE_ONWORDSELECT2RIGHT)
		strcpy(m_szOperationString,"ONWORDSELECT2RIGHT�F");
	else if(m_lOperationKind == KEYOPE_ONWORDSELECT2LEFT)
		strcpy(m_szOperationString,"ONWORDSELECT2LEFT�F");
	else if(m_lOperationKind == KEYOPE_ONEDITJUMP2TOPSELECT)
		strcpy(m_szOperationString,"ONEDITJUMP2TOPSELECT�F");
	else if(m_lOperationKind == KEYOPE_ONEDITJUMP2BOTTOMSELECT)
		strcpy(m_szOperationString,"ONEDITJUMP2BOTTOMSELECT�F");
	else if(m_lOperationKind == KEYOPE_ONPARENTHESISSELECT)
		strcpy(m_szOperationString,"ONPARENTHESISSELECT�F");
	else if(m_lOperationKind == KEYOPE_ONEDITINSQUOTE)
		strcpy(m_szOperationString,"ONEDITINSQUOTE");
	else if(m_lOperationKind == KEYOPE_ONEDITDELCPPCOMMENT)
		strcpy(m_szOperationString, "ONEDITDELCPPCOMMENT�F");
	else if(m_lOperationKind == KEYOPE_ONEDITDELVBCOMMENT)
		strcpy(m_szOperationString, "ONEDITDELVBCOMMENT�F");
	else if(m_lOperationKind == KEYOPE_ONEDITDELASMCOMMENT)
		strcpy(m_szOperationString, "ONEDITDELASMCOMMENT�F");
	else if(m_lOperationKind == KEYOPE_ONEDITDELQUOTE)
		strcpy(m_szOperationString, "ONEDITDELQUOTE�F");
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FCView::OnKeyDown�p�����[�^�ݒ�										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
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

	//�I�y���[�V�������������e�L�X�g�ǉ��ݒ�
	SetOperationString(nChar,nRepCnt,nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FCView::OnKeyUp�p�����[�^�ݒ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
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

	//�I�y���[�V�������������e�L�X�g�ǉ��ݒ�
	SetOperationString(nChar,nRepCnt,nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FCView::OnChar�p�����[�^�ݒ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
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

	//�I�y���[�V�������������e�L�X�g�ǉ��ݒ�
	SetOperationString(nChar,nRepCnt,nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�������s																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperation::Do
	(	CEditorView*	pobjEditorView,		//�I�y���[�V�������s�Ώۂ̂u������
		CEditorDoc*		pobjEditorDoc		//�I�y���[�V�������s�Ώۂ̂c����
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
/*|	�T�v	�F�I�y���[�V�������e�����e�L�X�g�擾									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperation::GetOperationString
	(	char*	szBuf
	)
{
	strcpy(szBuf,m_szOperationString);
}

/////////////////////////////////////////////////////////////////////////////
// COperation �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�y���[�V�������e�����e�L�X�g�ݒ�									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void COperation::SetOperationString
	(	UINT	nChar,
		UINT	nRepCnt,
		UINT	nFlags
	)
{
	//�I�y���[�V�������������e�L�X�g�ǉ��ݒ�
	char	szBuf[256];
	sprintf(szBuf,"nChar=%6X(H), nRepCnt=%6X(H), nFlags=%6X(H)",nChar,nRepCnt,nFlags);
	switch(nChar)
	{
	case VK_SHIFT:	//SHIFT�L�[
		strcat(szBuf,", VK=VK_SHIFT");
		break;
	case VK_INSERT: //INSERT�L�[
		strcat(szBuf,", VK=VK_INSERT");
		break;
	case VK_LEFT: //�����L�[
		strcat(szBuf,", VK=VK_LEFT");
		break;
	case VK_RIGHT: //�E���L�[
		strcat(szBuf,", VK=VK_RIGHT");
		break;
	case VK_DOWN: //�����L�[
		strcat(szBuf,", VK=VK_DOWN");
		break;
	case VK_UP:	//����L�[
		strcat(szBuf,", VK=VK_UP");
		break;
	case VK_HOME: //HOME�L�[
		strcat(szBuf,", VK=VK_HOME");
		break;
	case VK_END: //END�L�[
		strcat(szBuf,", VK=VK_END");
		break;
	case VK_PRIOR: //PAGE UP�L�[
		strcat(szBuf,", VK=VK_PRIOR");
		break;
	case VK_NEXT: //PAGE DOWN�L�[
		strcat(szBuf,", VK=VK_NEXT");
		break;
	case VK_RETURN: //���^�[���L�[
		strcat(szBuf,", VK=VK_RETURN");
		break;
	case VK_DELETE: //DELETE�L�[
		strcat(szBuf,", VK=VK_DELETE");
		break;
	case VK_BACK: //BACK SPACE�L�[
		strcat(szBuf,", VK=VK_BACK");
		break;
	case VK_F9: // �e�X�L�[
		strcat(szBuf,", VK=VK_F9");
		break;
	default:
		strcat(szBuf,", VK=OTHER");
		break;
	}

	strcat(m_szOperationString,szBuf);
}


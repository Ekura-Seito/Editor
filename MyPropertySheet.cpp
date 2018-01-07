// MyPropertySheet.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <Afxtempl.h>			//�l�e�b�̂b�l�������g�p����
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include <io.h>					//for _access
#include "Editor_Def.h"
#include "StatusBarDisp.h"				//�X�e�[�^�X�o�[�\�����\����
#include "Editor.h"
#include "BitmapClient.h"
#include "Operation.h"
#include "OperationManager.h"
#include "MainFrm.h"
#include "ColorButton.h"
#include "ColorManager.h"
#include "Line.h"
#include "LineManager.h"
#include "UndoItem.h"
#include "UndoManager.h"
#include "EditorDoc.h"
#include "EditorView.h"
#include "MyPropertyPage1.h"
#include "MyPropertyPage2.h"
#include "MyPropertyPage3.h"
#include "MyPropertyPage4.h"
#include "MyPropertyPage5.h"
#include "MyPropertyPage6.h"
#include "MyPropertyPage7.h"
#include "MyPropertyPage8.h"
#include "MyPropertyPage9.h"
#include "MyPropertyPage10.h"
#include "MyPropertyPage11.h"
#include "MyPropertyPage12.h"
#include "MyPropertySheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet

IMPLEMENT_DYNAMIC(CMyPropertySheet, CPropertySheet)

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertySheet::CMyPropertySheet(CWnd* pWndParent)
	 : CPropertySheet(IDS_PROPSHT_CAPTION1, pWndParent)
{
	// ���̈ʒu�ɂ��ׂẴv���p�e�B �y�[�W��ǉ����Ă��������B
	// �����ɕ\������Ă��鏇�ɉ�ʂɕ\������܂��B
	// �f�t�H���g�ł́A�ŏ��̃y�[�W���A�N�e�B�u�ȃy�[�W�ł��B
	// ���̃v���p�e�B �y�[�W���A�N�e�B�u�ɂ�����ɂ́A
	// SetActivePage() ���Ăяo���̂��ЂƂ̕��@�ł��B

	m_pApp = (CEditorApp*)AfxGetApp();
	m_pobjMainFrame = NULL;
	m_pobjEditorActiveDoc = NULL;
	m_pobjEditorActiveView = NULL;

	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
	AddPage(&m_Page4);
	AddPage(&m_Page5);
	AddPage(&m_Page6);
	AddPage(&m_Page7);
	AddPage(&m_Page8);
	AddPage(&m_Page9);
	AddPage(&m_Page10);
	AddPage(&m_Page11);
	AddPage(&m_Page12);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertySheet::~CMyPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CMyPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CMyPropertySheet)
	ON_COMMAND(ID_APPLY_NOW, OnApplyNow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CMyPropertySheet::DoModal() 
{
	return CPropertySheet::DoModal();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�K�p�v�{�^������������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertySheet::OnApplyNow()
{
	Default();
	if( m_Page1.IsChanged() ) //�y�[�W�P�ɕύX����
	{
		m_pApp->SetShowSplash(m_Page1.m_bShowSplash);
		if(m_Page1.m_lSplashTime>=1 && m_Page1.m_lSplashTime<=10)
			m_pApp->WriteProfileInt("CEditorApp","SplashTime",m_Page1.m_lSplashTime);
		m_pApp->WriteProfileInt("CEditorApp","nMaxMRU",m_Page1.m_nMaxMRU);
		m_pApp->SetMaxMRU(m_Page1.m_nMaxMRU);
		m_pApp->WriteProfileInt("CEditorApp","bIsOpenNewFile",m_Page1.m_bIsOpenNewFile);
		m_pApp->SetOpenNewFile(m_Page1.m_bIsOpenNewFile);
		m_pApp->WriteProfileInt("CEditorApp","IsChdirToOpenFile",m_Page1.m_bIsChdirToOpenFile);
		m_pApp->SetChdirToOpenFile(m_Page1.m_bIsChdirToOpenFile);
		m_pApp->WriteProfileInt("CChildFrame","bMaximizeWindow",m_Page1.m_bMaximizeWindow);
		m_pobjMainFrame->SetSaveWindowPlace( m_Page1.m_bIsSaveWindowPlace );
		m_pApp->WriteProfileInt("CEditorDoc","bIsFullTitle",m_Page1.m_bIsFullTitle);
		m_pobjEditorActiveView->SetFlagDispEOFMark(m_Page1.m_bIsDispEOFMark);
		m_pApp->WriteProfileInt("CEditorApp","bIsNoDoubleExecute",m_Page1.m_bIsNoDoubleExecute);
		if(m_Page1.m_bIsExecPasswd!=FALSE && !m_Page1.m_objExecPasswdStr.IsEmpty())
		{
			m_pApp->WriteProfileInt("CEditorApp","bIsExecPasswd",TRUE);
			m_pApp->WriteProfileString("CEditorApp","ExecPasswd",m_Page1.m_objExecPasswdStr);
		}
		else
		{
			m_pApp->WriteProfileInt("CEditorApp","bIsExecPasswd",FALSE);
			m_pApp->WriteProfileString("CEditorApp","ExecPasswd","");
		}
		m_pApp->WriteProfileInt("CEditorApp","bIsEnableShellOpen",m_Page1.m_bIsEnableShellOpen);
		if(m_Page1.m_bIsEnableShellOpen)
			m_pApp->RegisterShellExtDll();
		else
			m_pApp->UnRegisterShellExtDll();
		m_pApp->WriteProfileInt("CEditorApp","bIsMakeShortcutDsktop",m_Page1.m_bIsMakeShotcutDsktop);
		m_pApp->MakeXerxesShortcut(m_Page1.m_bIsMakeShotcutDsktop);
		m_pApp->WriteProfileInt("CEditorView","IsImmAutoOpen",m_Page1.m_bIsImmAutoOpen);

		m_Page1.ClearChangeFlag();
	}
	if( m_Page2.IsChanged() ) //�y�[�W�Q�ɕύX����
	{
		//�t�H���g�ݒ�
		m_pApp->SetFontSettings2AllWindow(m_pobjEditorActiveDoc);

		//�X�e�[�^�X�o�[�t�H���g�ݒ�
		m_pobjMainFrame->SetStatusBarFont(m_Page2.m_stStatusBarLogFont);
		//�^�u�T�C�Y�ݒ�
		if(m_Page2.m_nTabSize == 0)//�Q�^�u
			m_pobjEditorActiveDoc->SetTabSize(2);
		else if(m_Page2.m_nTabSize == 1)//�S�^�u
			m_pobjEditorActiveDoc->SetTabSize(4);
		else					 //�W�^�u
			m_pobjEditorActiveDoc->SetTabSize(8);

		m_Page2.ClearChangeFlag();
	}
	if( m_Page3.IsChanged() ) //�y�[�W�R�ɕύX����
	{
		//�J���[�ݒ�
		m_pApp->SetColorSettings2AllWindow();

		m_pobjMainFrame->SetMDIBkgndColor( m_Page3.m_MDIBkgndColor );
		m_pobjEditorActiveView->SetDiffDelLineColor(m_Page3.m_DiffDelLineColor);
		m_pobjEditorActiveView->SetDiffAddLineColor(m_Page3.m_DiffAddLineColor);
		m_pobjEditorActiveView->SetDirDiffLineColor(m_Page3.m_DirDiffLineColor);

		m_Page3.ClearChangeFlag();
	}
	if( m_Page4.IsChanged() ) //�y�[�W�S�ɕύX����
	{
		//���s�R�[�h�敪�ݒ�
		m_pobjEditorActiveDoc->SetKaigyoKind(m_Page4.m_nKaigyoKind);
		//�I�[�g�Z�[�u�ݒ�
		m_pobjMainFrame->SetFlagAutoSave(m_Page4.m_bIsAutoSave);
		//�I�[�g�Z�[�u�Ԋu
		if(m_Page4.m_lSaveInterval>=1 && m_Page4.m_lSaveInterval<=60)
			m_pobjMainFrame->SetSaveInterval(m_Page4.m_lSaveInterval);
		//�s���󔒍폜�t���O
		m_pobjEditorActiveDoc->SetFlagDelEndSpacesOnSave(m_Page4.m_bDelEndSpacesOnSave);
		//�󔒁|���^�u�ϊ��t���O
		m_pobjEditorActiveDoc->SetFlagSpace2TabOnSave(m_Page4.m_bSpace2TabOnSave);

		m_Page4.ClearChangeFlag();
	}
	if( m_Page5.IsChanged() ) //�y�[�W�T�ɕύX����
	{
		//�s�ԍ�����t���O
		m_pobjEditorActiveView->SetFlagPrintLineNo(m_Page5.m_bPrintLineNo);
		//�Q�i�g�݈���t���O
		m_pobjEditorActiveView->SetFlagPrint2Dangumi(m_Page5.m_bPrint2Dangumi);
		//����p������
		m_pApp->SetPrintOrientation(m_Page5.m_nPrintOrientation);
		//����]��
		CRect objMarginRect;
		objMarginRect.left   = m_Page5.m_nYohakuHidari;
		objMarginRect.top    = m_Page5.m_nYohakuUe;
		objMarginRect.right  = m_Page5.m_nYohakuMigi;
		objMarginRect.bottom = m_Page5.m_nYohakuSita;
		objMarginRect.NormalizeRect();
		m_pApp->SetPrintMargin(objMarginRect);
		//�w�b�_���t�b�^�ʒu
		m_pobjEditorActiveView->SetPrintHeaderLayoutPos(m_Page5.m_nHeaderLayout);
		m_pobjEditorActiveView->SetPrintFooterLayoutPos(m_Page5.m_nFooterLayout);
		//�w�b�_���t�b�^�̃t�H�[�}�b�g
		m_pobjEditorActiveView->SetStrHeaderFormat(m_Page5.m_objStrHeaderFormat);
		m_pobjEditorActiveView->SetStrFooterFormat(m_Page5.m_objStrFooterFormat);

		m_Page5.ClearChangeFlag();
	}
	if( m_Page6.IsChanged() ) //�y�[�W�U�ɕύX����
	{
		//�L�����b�g�`��ݒ�
		if(m_Page6.m_nCaretType == 0)//�c�^�L�����b�g
			m_pobjEditorActiveView->SetCaretType(CARET_IBEAM);
		else if(m_Page6.m_nCaretType == 1)//���^�L�����b�g
			m_pobjEditorActiveView->SetCaretType(CARET_SQUARE);
		//�L�����b�g�_�ŊԊu�ݒ�
		::SetCaretBlinkTime(m_Page6.m_unCaretBlinkTime);
		m_pobjEditorActiveView->HoldCaretBlinkTime(m_Page6.m_unCaretBlinkTime);
		//�L�����b�g�̐U�镑���ݒ�
		if(m_Page6.m_nCaretBehave == 0)//��ʃX�N���[�����ɍs�ʒu�ύX
			m_pobjEditorActiveView->SetCaretBehave(CARET_BEHAVE_MOVE);
		else if(m_Page6.m_nCaretBehave == 1)//��ʃX�N���[�����ɍs�ʒu�ێ�
			m_pobjEditorActiveView->SetCaretBehave(CARET_BEHAVE_STAY);
		//�L�����b�g�ړ��^�C�v�ݒ�
		m_pobjEditorActiveView->SetCaretMoveType(m_Page6.m_nCaretMoveType);
		//�L�����b�g�����X�s�[�h�ݒ�
		m_pApp->WriteProfileInt("CEditorView","nMultValueOfCaretAccelarete",m_Page6.m_nMultValueOfCaretAccelarete+1);
		//�\��t����̃L�����b�g�ʒu
		m_pApp->WriteProfileInt("CEditorDoc","bSetCaretToEndOnPaste",m_Page6.m_nSetCaretToEndOnPaste);

		m_Page6.ClearChangeFlag();
	}
	if( m_Page7.IsChanged() ) //�y�[�W�V�ɕύX����
	{
		m_pobjMainFrame->SetFlagDispLogoBmp( m_Page7.m_bDispLogoBmp );
		m_pobjMainFrame->SetLogoBmpKind(m_Page7.m_nLogoBmpKind);
		m_pobjMainFrame->SetLogoBmpFilePath(m_Page7.m_objBmpFilePath);
		m_pobjMainFrame->SetLogoPosKind(m_Page7.m_nLogoPosKind);
		if(m_Page7.m_unLogoPosX>=0 && m_Page7.m_unLogoPosX<=100)
			m_pobjMainFrame->SetLogoPosX( m_Page7.m_unLogoPosX );
		if(m_Page7.m_unLogoPosY>=0 && m_Page7.m_unLogoPosY<=100)
			m_pobjMainFrame->SetLogoPosY( m_Page7.m_unLogoPosY );

		m_Page7.ClearChangeFlag();
	}
	if( m_Page8.IsChanged() ) //�y�[�W�W�ɕύX����
	{
		m_pobjMainFrame->SetStatusBarDispInfo(&m_Page8.m_stStatusBarDisp);
		m_pobjMainFrame->SetStatusDayStyle(m_Page8.m_lStatusDayStyle);
		m_pobjMainFrame->SetStatusHourStyle(m_Page8.m_lStatusHourStyle);
		m_pobjMainFrame->SetStatusLineStyle(m_Page8.m_lStatusLineStyle);

		m_Page8.ClearChangeFlag();
	}
	if( m_Page9.IsChanged() ) //�y�[�W�X�ɕύX����
	{
		m_Page9.ChangesToRegistry();
		m_Page9.ClearChangeFlag();
	}
	if( m_Page11.IsChanged() ) //�y�[�W�P�P�ɕύX����
	{
		m_Page11.ChangesToRegistry();
		m_Page11.ClearChangeFlag();
	}
	if( m_Page12.IsChanged() ) //�y�[�W�P�Q�ɕύX����
	{
		m_pobjMainFrame->SetHtmlOpenParameter( m_Page12.m_bIsUseProxy,
											   m_Page12.m_objProxyIPAddress,
											   m_Page12.m_bIsDialup,
											   m_Page12.m_objDialupEntry,
											   m_Page12.m_objRasUserName,
											   m_Page12.m_objRasPasswd,
											   m_Page12.m_bIsAutoDisconnect );
	}
}


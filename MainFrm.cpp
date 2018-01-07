// MainFrm.cpp : CMainFrame �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include <Afxtempl.h>			//�l�e�b�̂b�l�������g�p����
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include <io.h>					//for _access()
#pragma warning(disable:4786)
#include <map>					// for map
using namespace std;
//#include "zmouse.h"				//for �C���e���}�E�X
#include "Editor_Def.h"			//���ʂc�d�e�h�m�d
#include "StatusBarDisp.h"		//�X�e�[�^�X�o�[�\�����\����
#include "ColorManager.h"
#include "Editor.h"
#include "ChildFrm.h"
#include "BitmapClient.h"
#include "Operation.h"
#include "OperationManager.h"
#include "MainFrm.h"
#include "ColorButton.h"
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
#include "Line.h"
#include "LineManager.h"
#include "UndoItem.h"
#include "UndoManager.h"
#include "EditorDoc.h"
#include "EditorView.h"
#include "MyComboBoxEx.h"
#include "StaticFilespec.h"
#include "AutoResizer.h"
#include "GrepDlg.h"
#include "GrepStatusDlg.h"
#include "MyComboBoxEx2.h"
#include "CodeConvDlg.h"
#include "CodeConvStatusDlg.h"
#include "KaigyoConvDlg.h"
#include "KaigyoConvStatusDlg.h"
#include "CodeConvManager.h"
#include "StaticFilespec.h"
#include "FilePropertyDlg.h"
#include "WordCountDlg.h"
#include "WCntStatusDlg.h"
#include "FileSearchDlg.h"
#include "FileSearchStatusDlg.h"
#include "Splash.h"
#include "ClipBoardViewDlg.h"
#include "EncriptDlg.h"
#include "EncriptStatusDlg.h"
#include "DecriptDlg.h"
#include "DecriptStatusDlg.h"
//#include "MyFeal.h"
#include "MyDes.h"
#include "Uuencoder.h"
#include "Base64.h"
#include "QuotedPrintable.h"
#include "FileCompareStatusDlg.h"
#include "FileCompareResultDlg.h"
#include "FileCompare.h"
#include "DirCompareDlg.h"
#include "DirCompareStatusDlg.h"
#include "DirCompareResultDlg.h"
#include "DirCompare.h"
#include "DateMaskEdit.h"
#include "TimeMaskEdit.h"
#include "FileTouchDlg.h"
#include "FindDlg.h"
#include "ReplaceDlg.h"
#include "MailListStatusDlg.h"
#include "ListCtrlEx.h"
#include "MailListDlg.h"
#include "Pop.h"
#include "KeyBind.h"
#include "BasicAuthorizationDlg.h"
#include "HtmlOpenDlg.h"
#include "HtmlOpenStatusDlg.h"
#include "HtmlOpenResultDlg.h"
#include "Http.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////�����A�u���p�o�^�ς݃��b�Z�[�W�擾
//static UINT WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);
//�C���e���}�E�X�Ή�
static UINT nMyMouseWheel  = ::RegisterWindowMessage(MSH_MOUSEWHEEL);


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_SETTINGS, OnSettings)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS, OnUpdateSettings)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateEditReplace)
	ON_COMMAND(ID_EDIT_FINDCONTINUE, OnEditFindcontinue)
	ON_COMMAND(ID_EDIT_FINDREVCONTINUE, OnEditFindrevcontinue)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FINDCONTINUE, OnUpdateEditFindcontinue)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FINDREVCONTINUE, OnUpdateEditFindrevcontinue)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_WINDOW_ALLCLOSE, OnUpdateWindowAllclose)
	ON_COMMAND(ID_WINDOW_ALLCLOSE, OnWindowAllclose)
	ON_WM_MEASUREITEM()
	ON_WM_MENUCHAR()
	ON_WM_INITMENUPOPUP()
	ON_UPDATE_COMMAND_UI(ID_EDIT_GREP, OnUpdateEditGrep)
	ON_COMMAND(ID_EDIT_GREP, OnEditGrep)
	ON_WM_ACTIVATE()
	ON_UPDATE_COMMAND_UI(ID_EDIT_CODECONV, OnUpdateEditCodeconv)
	ON_COMMAND(ID_EDIT_CODECONV, OnEditCodeconv)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ASCIIDUMP, OnUpdateEditAsciidump)
	ON_COMMAND(ID_EDIT_ASCIIDUMP, OnEditAsciidump)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SJISDUMP, OnUpdateEditSjisdump)
	ON_COMMAND(ID_EDIT_SJISDUMP, OnEditSjisdump)
	ON_UPDATE_COMMAND_UI(ID_FILE_REREAD, OnUpdateFileReread)
	ON_COMMAND(ID_FILE_REREAD, OnFileReread)
	ON_UPDATE_COMMAND_UI(ID_FILE_PROPERTY, OnUpdateFileProperty)
	ON_COMMAND(ID_FILE_PROPERTY, OnFileProperty)
	ON_COMMAND(ID_EDIT_SETREADONLY, OnEditSetreadonly)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SETREADONLY, OnUpdateEditSetreadonly)
	ON_COMMAND(ID_EDIT_UNSETREADONLY, OnEditUnsetreadonly)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNSETREADONLY, OnUpdateEditUnsetreadonly)
	ON_COMMAND(ID_EDIT_WORDCOUNT, OnEditWordcount)
	ON_UPDATE_COMMAND_UI(ID_EDIT_WORDCOUNT, OnUpdateEditWordcount)
	ON_COMMAND(ID_EDIT_FILESEARCH, OnEditFilesearch)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FILESEARCH, OnUpdateEditFilesearch)
	ON_COMMAND(ID_FILE_SAVECLOSE, OnFileSaveclose)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVECLOSE, OnUpdateFileSaveclose)
	ON_COMMAND(ID_TOOL_OPE_STARTENDRECORD, OnToolOpeStartendrecord)
	ON_UPDATE_COMMAND_UI(ID_TOOL_OPE_STARTENDRECORD, OnUpdateToolOpeStartendrecord)
	ON_COMMAND(ID_TOOL_OPE_DO, OnToolOpeDo)
	ON_UPDATE_COMMAND_UI(ID_TOOL_OPE_DO, OnUpdateToolOpeDo)
	ON_COMMAND(ID_TOOL_OPE_SAVE, OnToolOpeSave)
	ON_UPDATE_COMMAND_UI(ID_TOOL_OPE_SAVE, OnUpdateToolOpeSave)
	ON_COMMAND(ID_TOOL_OPE_OPEN, OnToolOpeOpen)
	ON_UPDATE_COMMAND_UI(ID_TOOL_OPE_OPEN, OnUpdateToolOpeOpen)
	ON_COMMAND(ID_TOOL_OPE_DISP, OnToolOpeDisp)
	ON_UPDATE_COMMAND_UI(ID_TOOL_OPE_DISP, OnUpdateToolOpeDisp)
	ON_COMMAND(ID_TOOL_OPE_STEPDO_NEXT, OnToolOpeStepdoNext)
	ON_UPDATE_COMMAND_UI(ID_TOOL_OPE_STEPDO_NEXT, OnUpdateToolOpeStepdoNext)
	ON_COMMAND(ID_TOOL_OPE_STEPDO_END, OnToolOpeStepdoEnd)
	ON_UPDATE_COMMAND_UI(ID_TOOL_OPE_STEPDO_END, OnUpdateToolOpeStepdoEnd)
	ON_COMMAND(ID_WND_NEXT, OnWndNext)
	ON_UPDATE_COMMAND_UI(ID_WND_NEXT, OnUpdateWndNext)
	ON_COMMAND(ID_WND_PREV, OnWndPrev)
	ON_UPDATE_COMMAND_UI(ID_WND_PREV, OnUpdateWndPrev)
	ON_COMMAND(ID_EDIT_CLIPVIEW, OnEditClipview)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLIPVIEW, OnUpdateEditClipview)
	ON_COMMAND(ID_EDIT_INCRIMENTALSEARCH, OnEditIncrimentalsearch)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INCRIMENTALSEARCH, OnUpdateEditIncrimentalsearch)
	ON_COMMAND(ID_SECURITY_ENCRIPT, OnSecurityEncript)
	ON_UPDATE_COMMAND_UI(ID_SECURITY_ENCRIPT, OnUpdateSecurityEncript)
	ON_COMMAND(ID_SECURITY_DECRIPT, OnSecurityDecript)
	ON_UPDATE_COMMAND_UI(ID_SECURITY_DECRIPT, OnUpdateSecurityDecript)
	ON_COMMAND(ID_EDIT_UUENCODE, OnEditUuencode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UUENCODE, OnUpdateEditUuencode)
	ON_COMMAND(ID_EDIT_BASE64ENCODE, OnEditBase64encode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BASE64ENCODE, OnUpdateEditBase64encode)
	ON_COMMAND(ID_EDIT_QUOTEDENCODE, OnEditQuotedencode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_QUOTEDENCODE, OnUpdateEditQuotedencode)
	ON_COMMAND(ID_EDIT_FILECOMPARE, OnEditFilecompare)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FILECOMPARE, OnUpdateEditFilecompare)
	ON_COMMAND(ID_EDIT_DIRCOMPARE, OnEditDircompare)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DIRCOMPARE, OnUpdateEditDircompare)
	ON_COMMAND(ID_FILE_DELETE, OnFileDelete)
	ON_UPDATE_COMMAND_UI(ID_FILE_DELETE, OnUpdateFileDelete)
	ON_COMMAND(ID_EDIT_FILETOUCH, OnEditFiletouch)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FILETOUCH, OnUpdateEditFiletouch)
	ON_COMMAND(ID_WINDOW_RESTORE, OnWindowRestore)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_RESTORE, OnUpdateWindowRestore)
	ON_COMMAND(ID_WINDOW_MINIMIZE, OnWindowMinimize)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_MINIMIZE, OnUpdateWindowMinimize)
	ON_COMMAND(ID_WINDOW_MAXIMIZE, OnWindowMaximize)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_MAXIMIZE, OnUpdateWindowMaximize)
	ON_COMMAND(ID_VIEW_TOOLBAR_WND, OnViewToolbarWnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_WND, OnUpdateViewToolbarWnd)
	ON_COMMAND(ID_VIEW_TOOLBAR_TOOLS, OnViewToolbarTools)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_TOOLS, OnUpdateViewToolbarTools)
	ON_COMMAND(ID_VIEW_TOOLBAR_FIND, OnViewToolbarFind)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_FIND, OnUpdateViewToolbarFind)
	ON_COMMAND(ID_VIEW_TOOLBAR_EDIT, OnViewToolbarEdit)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_EDIT, OnUpdateViewToolbarEdit)
	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateViewToolbar)
	ON_COMMAND(ID_VIEW_TOOLBAR_JMP, OnViewToolbarJmp)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR_JMP, OnUpdateViewToolbarJmp)
	ON_COMMAND(ID_FILE_MAILLIST, OnFileMaillist)
	ON_UPDATE_COMMAND_UI(ID_FILE_MAILLIST, OnUpdateFileMaillist)
	ON_COMMAND(ID_EDIT_KAIGYOCONV, OnEditKaigyoconv)
	ON_UPDATE_COMMAND_UI(ID_EDIT_KAIGYOCONV, OnUpdateEditKaigyoconv)
	ON_COMMAND(ID_CUSTOMIZE_TOOLBAR, OnCustomizeToolbar)
	ON_UPDATE_COMMAND_UI(ID_CUSTOMIZE_TOOLBAR, OnUpdateCustomizeToolbar)
	ON_COMMAND(ID_CUSTOMIZE_TOOLBAREDIT, OnCustomizeToolbaredit)
	ON_UPDATE_COMMAND_UI(ID_CUSTOMIZE_TOOLBAREDIT, OnUpdateCustomizeToolbaredit)
	ON_COMMAND(ID_CUSTOMIZE_TOOLBARFIND, OnCustomizeToolbarfind)
	ON_UPDATE_COMMAND_UI(ID_CUSTOMIZE_TOOLBARFIND, OnUpdateCustomizeToolbarfind)
	ON_COMMAND(ID_CUSTOMIZE_TOOLBARJMP, OnCustomizeToolbarjmp)
	ON_UPDATE_COMMAND_UI(ID_CUSTOMIZE_TOOLBARJMP, OnUpdateCustomizeToolbarjmp)
	ON_COMMAND(ID_CUSTOMIZE_TOOLBARTOOLS, OnCustomizeToolbartools)
	ON_UPDATE_COMMAND_UI(ID_CUSTOMIZE_TOOLBARTOOLS, OnUpdateCustomizeToolbartools)
	ON_COMMAND(ID_CUSTOMIZE_TOOLBARWND, OnCustomizeToolbarwnd)
	ON_UPDATE_COMMAND_UI(ID_CUSTOMIZE_TOOLBARWND, OnUpdateCustomizeToolbarwnd)
	ON_WM_SIZE()
	ON_COMMAND(ID_WINDOW_CLOSE, OnWindowClose)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_CLOSE, OnUpdateWindowClose)
	ON_COMMAND(ID_HTTP_FILE_OPEN, OnHttpFileOpen)
	ON_COMMAND(ID_EDIT_FCOMP_ALPHABETBIGANDSMALL, OnEditFcompAlphabetbigandsmall)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FCOMP_ALPHABETBIGANDSMALL, OnUpdateEditFcompAlphabetbigandsmall)
	ON_COMMAND(ID_EDIT_FCOMP_TABEQUAL2SPACE, OnEditFcompTabequal2space)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FCOMP_TABEQUAL2SPACE, OnUpdateEditFcompTabequal2space)
	ON_COMMAND(ID_EDIT_FCOMP_ZENEQUAL2HAN, OnEditFcompZenequal2han)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FCOMP_ZENEQUAL2HAN, OnUpdateEditFcompZenequal2han)
	ON_COMMAND(ID_EDIT_FILERECOMPARE, OnEditFilerecompare)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FILERECOMPARE, OnUpdateEditFilerecompare)
	//}}AFX_MSG_MAP
//	// �����A�u���p
//	ON_REGISTERED_MESSAGE(WM_FINDREPLACE,OnFindReplace)
	// �C���e���}�E�X�Ή�
	ON_REGISTERED_MESSAGE(nMyMouseWheel,OnMSHMouseWheel)
END_MESSAGE_MAP()


//static UINT indicators[] =
//{
//	ID_SEPARATOR,           // �X�e�[�^�X ���C�� �C���W�P�[�^
//	ID_INDICATOR_GYORETU,
//	ID_INDICATOR_BYTE,
//	ID_INDICATOR_INS,
//	ID_INDICATOR_READONLY,
//	ID_INDICATOR_TIME,
//	ID_INDICATOR_CHARCODE,
//	ID_INDICATOR_KANA,
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
//};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMainFrame::CMainFrame()
{
	int	nCnt=0;
	int nBuf=0;
	char	szKey[64];

	m_pApp = (CEditorApp*)AfxGetApp();

	//////////////////////////
	//	�����o�ϐ�������	//
	//////////////////////////

	// WEB���HTML�t�@�C�����J���֘A
	m_bIsUseProxyHtml = FALSE;			// �v���L�V�T�[�o�o�R���H
	m_objRequestPathHtml = _T("");		// �v���p�X
	m_objProxyIPAddressHtml = _T("");	// �v���L�V�T�[�o�̂h�o�A�h���X
	m_bIsDialupHtml = FALSE;			// �_�C�A���A�b�v�ڑ����邩�H
	m_objDialupEntryHtml = _T("");		// �g�p����_�C�A���A�b�v�G���g��
	m_objRasUserNameHtml = _T("");		// ���[�U��
	m_objRasPasswdHtml = _T("");		// �p�X���[�h
	m_bIsAutoDisconnectHtml = FALSE;	// �����ؒf���邩�H
	m_pobjHtmlOpenStatusDlg = NULL;		// �󋵃_�C�A���O

	//�����֘A
	m_objFindString = _T("");			// �O�񌟍����̌���������
	m_bIsMatchCaseFind = 0;				// �啶���A��������ʂ̗L��
	m_bIsMatchWholeWordFind = 0;		// �P��S�̂���v���鎖���������H
	m_bIsSearchDownFind = 0;			// ��������
	m_bIsUseRegularExpressionFind = 0;	// ���K�\�����g�p���邩�H

	//�u���֘A
	m_objReplaceString = _T("");		// �u��������
	m_bIsMatchCaseReplace = 0;			// �啶���A��������ʂ̗L��
	m_bIsMatchWholeWordReplace = 0;		// �P��S�̂���v���鎖���������H
	m_bIsNowIncrimentalSearching = 0;	// �C���N�������^���T�[�`���s���t���O
	m_bIsUseRegularExpressionReplace = 0;// ���K�\�����g�p���邩�H

	// �f�q�d�o�֘A
	m_bIsMatchCaseGrep = 0;				// �啶���A��������ʂ̗L��
	m_bIsMatchWholeWordGrep = 0;		// �P��S�̂���v���鎖���������H
	m_bIsSearchSubFolderGrep = 0;		// �T�u�t�H���_���������邩�H
	m_bIsUseRegularExpressionGrep = 0;	// ���K�\�����g�p���邩�H
	m_objFolderGrep = _T("");			// �f�q�d�o���s�t�H���_
	m_objGrepStr = _T("");				// ����������
	m_objFileKindGrep = _T("");			// �����Ώۂ̃t�@�C�����
	m_pobjGrepDoc = NULL;				// �f�q�d�o�p�h�L�������g�N���X
	m_pobjGrepFrameWnd = NULL;			// �f�q�d�o�p�q�E�C���h�E�N���X
	m_pobjGrepStatusDlg = NULL;			// �f�q�d�o�󋵃_�C�A���O

	// �����R�[�h�ϊ��֘A
	m_bIsSearchSubFolderConv = 0;		// �T�u�t�H���_���ϊ����邩�H
	m_bIsOutOtherFileConv = 0;			// �ϊ����ʂ�ʃt�@�C���֏o�͂��邩�H
	m_nConvKind = 0;					// �����R�[�h�ϊ����
	m_objFolderConv = _T("");			// �����R�[�h�ϊ����s�t�H���_
	m_objFileKindConv = _T("");			// �ϊ��Ώۂ̃t�@�C�����
	m_nInCodeConv = 0;					// ���̓t�@�C���̕����R�[�h������
	m_pobjConvDoc = NULL;				// �����R�[�h�ϊ��p�h�L�������g�N���X
	m_pobjConvFrameWnd = NULL;			// �����R�[�h�ϊ��p�E�C���h�E�N���X
	m_pobjConvStatusDlg = NULL;			// �����R�[�h�ϊ��󋵃_�C�A���O

	// ���s�R�[�h�ϊ��֘A
	m_bIsSearchSubFolderKaigyoConv = 0;	// �T�u�t�H���_���ϊ����邩�H
	m_bIsOutOtherFileKaigyoConv = 0;	// �ϊ����ʂ�ʃt�@�C���֏o�͂��邩�H
	m_objKaigyoConvKind = _T("");		// ���s�R�[�h�ϊ����
	m_objFolderKaigyoConv = _T("");		// ���s�R�[�h�ϊ����s�t�H���_
	m_objFileKindKaigyoConv = _T("");	// �ϊ��Ώۂ̃t�@�C�����
	m_pobjKaigyoConvDoc = NULL;			// ���s�R�[�h�ϊ��p�h�L�������g�N���X
	m_pobjKaigyoConvFrameWnd = NULL;	// ���s�R�[�h�ϊ��p�E�C���h�E�N���X
	m_pobjKaigyoConvStatusDlg = NULL;	// ���s�R�[�h�ϊ��󋵃_�C�A���O

	// �t�@�C���Í����֘A
	m_bIsSearchSubFolderEncript = 0;	// �T�u�t�H���_���ϊ����邩�H
	m_bIsOutOtherFileEncript = 0;		// �Í����ʂ�ʃt�@�C���֏o�͂��邩�H
	m_objKeyEncript = _T("");			// �t�@�C���Í����L�[
	m_objFolderEncript = _T("");		// �t�@�C���Í������s�t�H���_
	m_objFileKindEncript = _T("");		// �t�@�C���Í����Ώۂ̃t�@�C�����
	m_pobjEncriptDoc = NULL;			// �t�@�C���Í����p�h�L�������g�N���X
	m_pobjEncriptFrameWnd = NULL;		// �t�@�C���Í����p�E�C���h�E�N���X
	m_pobjEncriptStatusDlg = NULL;		// �t�@�C���Í����󋵃_�C�A���O

	// �t�@�C���������֘A
	m_bIsSearchSubFolderDecript = 0;	// �T�u�t�H���_���ϊ����邩�H
	m_bIsOutOtherFileDecript = 0;		// �������ʂ�ʃt�@�C���֏o�͂��邩�H
	m_objKeyDecript = _T("");			// �t�@�C���������L�[
	m_objFolderDecript = _T("");		// �t�@�C�����������s�t�H���_
	m_objFileKindDecript = _T("");		// �t�@�C���������Ώۂ̃t�@�C�����
	m_pobjDecriptDoc = NULL;			// �t�@�C���������p�h�L�������g�N���X
	m_pobjDecriptFrameWnd = NULL;		// �t�@�C���������p�E�C���h�E�N���X
	m_pobjDecriptStatusDlg = NULL;		// �t�@�C���������󋵃_�C�A���O

	// �_���v�֘A
	m_pobjDumpDoc = NULL;				// �_���v�p�h�L�������g�N���X
	m_pobjDumpFrameWnd = NULL;			// �_���v�p�E�C���h�E�N���X

	// ���[�h�J�E���g�֘A
	m_lByteCountSum = 0;				// �o�C�g�����v
	m_lWordCountSum = 0;				// ���[�h�����v
	m_lLineCountSum = 0;				// �s�����v
	m_lEmptyLineCountSum = 0;			// ��s�����v
	m_lCPPCommentLineCountSum = 0;		// �b�E�b�{�{�R�����g�s�����v
	m_lWCountFileCnt = 0;				// ���[�h�J�E���g�Ώۃt�@�C����
	m_bIsSearchSubFolderWCount = 0;		// �T�u�t�H���_���������邩�H
	m_objFolderWCount = _T("");			// ���[�h�J�E���g���s�t�H���_
	m_objFileKindWCount = _T("");		// ���[�h�J�E���g�Ώۂ̃t�@�C�����
	m_pobjWCountDoc = NULL;				// ���[�h�J�E���g�p�h�L�������g�N���X
	m_pobjWCountFrameWnd = NULL;		// ���[�h�J�E���g�p�q�E�C���h�E�N���X

	// �t�@�C�������֘A
	m_bIsSearchSubFolderFSearch = 0;	// �T�u�t�H���_���ϊ����邩�H
	m_objFolderFSearch = _T("");		// �t�@�C�������ϊ����s�t�H���_
	m_objFileKindFSearch = _T("");		// �����Ώۂ̃t�@�C�����
	m_pobjFSearchDoc = NULL;			// �t�@�C�������p�h�L�������g�N���X
	m_pobjFSearchFrameWnd = NULL;		// �t�@�C�������p�E�C���h�E�N���X
	m_pobjFileSearchStatusDlg = NULL;	// �t�@�C�������󋵃_�C�A���O

	// �t�@�C���^�b�`�֘A
	m_bIsSearchSubFolderTouch = 0;		// �T�u�t�H���_���^�b�`���邩�H
	m_objFolderTouch = _T("");			// �^�b�`���s�t�H���_
	m_objFileKindTouch = _T("");		// �^�b�`�Ώۂ̃t�@�C�����

	// �I�[�g�Z�[�u�֘A
	m_bIsAutoSave = 0;					// �I�[�g�Z�[�u�����邩�H
	m_lSaveInterval = 0;				// �I�[�g�Z�[�u�Ԋu�i���P�ʁj
	m_lTimeUpCount = 0;					// �^�C���A�b�v�J�E���^

	// �L�[����̋L�^�^�Đ��֘A
	m_bIsNowOperationRecording = 0;		// �L�[����L�^���t���O
	m_bIsNowStepDoOperation = 0;		// �L�[����̃X�e�b�v���s���t���O
	m_objOperationManager.Initialize();	// �L�[����}�l�[�W��

	// �X�e�[�^�X�o�[�֘A
	memset(&m_stStatusBarLogFont,0,sizeof(LOGFONT));		// �X�e�[�^�X�o�[��LOGFONT�\����
	memset(&m_stStatusBarDisp,0,sizeof(STATUSBARDISP));	// �X�e�[�^�X�o�[�\�����\����
	m_lStatusDayStyle = 0;				// �X�e�[�^�X�o�[�֕\��������t�\���X�^�C��
	m_lStatusHourStyle = 0;				// �X�e�[�^�X�o�[�֕\�����鎞���\���X�^�C��
	m_lStatusLineStyle = 0;				// �X�e�[�^�X�o�[�֕\������s�A��\���X�^�C��
	m_lSavedGyo = 0;					// �s�ԍ�
	m_lSavedRetu = 0;					// ��ԍ�
	m_lSavedMaxGyo = 0;					// �S�s��

	// �t�@�C����r�ԘA
	m_pobjFileCompareStatusDlg = NULL;	// �t�@�C����r�󋵃_�C�A���O

	// �f�B���N�g����r�֘A
	m_pobjDirCompareStatusDlg = NULL;	// �f�B���N�g����r�󋵃_�C�A���O
	m_objFolder1DirCmp = _T("");		// ����r�t�H���_
	m_objFolder2DirCmp = _T("");		// ����r�t�H���_
	m_pobjDirCmpDoc = NULL;				// �f�B���N�g����r�p�h�L�������g�N���X
	m_pobjDirCmpFrameWnd = NULL;		// �f�B���N�g����r�p�E�C���h�E�N���X

	//////////////////////////////
	//	���W�X�g������ݒ�擾	//
	//////////////////////////////
	m_bIsSaveWindowPlace        = m_pApp->GetProfileInt("CMainFrame","bIsSaveWindowPlace",0);
	m_bIsUseProxyHtml           = m_pApp->GetProfileInt("CMainFrame","bIsUseProxyHtml",0);
	m_objRequestPathHtml        = m_pApp->GetProfileString("CMainFrame","RequestPathHtml","");
	m_objProxyIPAddressHtml     = m_pApp->GetProfileString("CMainFrame","ProxyIPAddress","");
	m_bIsDialupHtml             = m_pApp->GetProfileInt("CMainFrame","bIsDialupHtml",0);
	m_objDialupEntryHtml        = m_pApp->GetProfileString("CMainFrame","DialupEntryHtml","");
	m_objRasUserNameHtml        = m_pApp->GetProfileString("CMainFrame","RasUserNameHtml","");
	m_objRasPasswdHtml          = m_pApp->GetProfileString("CMainFrame","RasPasswdHtml","");
	m_bIsAutoDisconnectHtml     = m_pApp->GetProfileInt("CMainFrame","bIsAutoDisconnectHtml",0);
	m_objFindString             = m_pApp->GetProfileString("CMainFrame","FindString","");
	m_bIsMatchCaseFind          = m_pApp->GetProfileInt("CMainFrame","bIsMatchCaseFind",0);
	m_bIsMatchWholeWordFind     = m_pApp->GetProfileInt("CMainFrame","bIsMatchWholeWordFind",0);
	m_bIsSearchDownFind         = m_pApp->GetProfileInt("CMainFrame","bIsSearchDownFind",TRUE);
	m_bIsUseRegularExpressionFind = m_pApp->GetProfileInt("CMainFrame","bIsUseRegularExpressionFind",0);
	m_objReplaceString          = m_pApp->GetProfileString("CMainFrame","ReplaceString","");
	m_bIsMatchCaseReplace       = m_pApp->GetProfileInt("CMainFrame","bIsMatchCaseReplace",0);
	m_bIsMatchWholeWordReplace  = m_pApp->GetProfileInt("CMainFrame","bIsMatchWholeWordReplace",0);
	m_bIsUseRegularExpressionReplace= m_pApp->GetProfileInt("CMainFrame","bIsUseRegularExpressionReplace",0);
	m_bIsMatchCaseGrep		    = m_pApp->GetProfileInt("CMainFrame","bIsMatchCaseGrep",0);
	m_bIsMatchWholeWordGrep     = m_pApp->GetProfileInt("CMainFrame","bIsMatchWholeWordGrep",0);
	m_bIsSearchSubFolderGrep    = m_pApp->GetProfileInt("CMainFrame","bIsSearchSubFolderGrep",0);
	m_bIsUseRegularExpressionGrep= m_pApp->GetProfileInt("CMainFrame","bIsUseRegularExpressionGrep",0);
	m_objFolderGrep			    = m_pApp->GetProfileString("CMainFrame","FolderGrep","");
	m_objGrepStr			    = m_pApp->GetProfileString("CMainFrame","GrepStr","");
	m_objFileKindGrep		    = m_pApp->GetProfileString("CMainFrame","FileKindGrep","");
	m_bIsSearchSubFolderConv    = m_pApp->GetProfileInt("CMainFrame","bIsSearchSubFolderConv",0);
	m_bIsOutOtherFileConv	    = m_pApp->GetProfileInt("CMainFrame","bIsOutOtherFileConv",1);
	m_nConvKind	  		        = m_pApp->GetProfileInt("CMainFrame","nConvKind",0);
	m_objFolderConv			    = m_pApp->GetProfileString("CMainFrame","FolderConv","");
	m_objFileKindConv		    = m_pApp->GetProfileString("CMainFrame","FileKindConv","");
	m_nInCodeConv  		        = m_pApp->GetProfileInt("CMainFrame","InCodeConv",0);
	m_bIsSearchSubFolderKaigyoConv = m_pApp->GetProfileInt("CMainFrame","bIsSearchSubFolderKaigyoConv",0);
	m_bIsOutOtherFileKaigyoConv	= m_pApp->GetProfileInt("CMainFrame","bIsOutOtherFileKaigyoConv",1);
	m_objKaigyoConvKind		    = m_pApp->GetProfileString("CMainFrame","KaigyoConvKind","");
	m_objFolderKaigyoConv		= m_pApp->GetProfileString("CMainFrame","FolderKaigyoConv","");
	m_objFileKindKaigyoConv		= m_pApp->GetProfileString("CMainFrame","FileKindKaigyoConv","");
	m_bIsSearchSubFolderWCount  = m_pApp->GetProfileInt("CMainFrame","bIsSearchSubFolderWCount",0);
	m_objFolderWCount		    = m_pApp->GetProfileString("CMainFrame","FolderWCount","");
	m_objFileKindWCount		    = m_pApp->GetProfileString("CMainFrame","FileKindWCount","");
	m_bIsAutoSave 			    = m_pApp->GetProfileInt("CMainFrame","bIsAutoSave",0);
	m_lSaveInterval 		    = m_pApp->GetProfileInt("CMainFrame","lSaveInterval",0);
	m_bIsSearchSubFolderFSearch = m_pApp->GetProfileInt("CMainFrame","bIsSearchSubFolderFSearch",0);
	m_objFolderFSearch          = m_pApp->GetProfileString("CMainFrame","FolderFSearch","");
	m_objFileKindFSearch        = m_pApp->GetProfileString("CMainFrame","FileKindFSearch","");
	m_lStatusDayStyle           = m_pApp->GetProfileInt("CMainFrame","StatusDayStyle",STS_DAYSTYLE_YYYYMMDD);
	m_lStatusHourStyle          = m_pApp->GetProfileInt("CMainFrame","StatusHourStyle",STS_HOURSTYLE_HHMM);
	m_lStatusLineStyle          = m_pApp->GetProfileInt("CMainFrame","StatusLineStyle",STS_LINESTYLE_LINE_OFFS_ALL);
	m_bIsSearchSubFolderEncript = m_pApp->GetProfileInt("CMainFrame","bIsSearchSubFolderEncript",0);
	m_bIsOutOtherFileEncript    = m_pApp->GetProfileInt("CMainFrame","bIsOutOtherFileEncript",1);
	m_objFolderEncript          = m_pApp->GetProfileString("CMainFrame","FolderEncript","");
	m_objFileKindEncript        = m_pApp->GetProfileString("CMainFrame","FileKindEncript","");
	m_bIsSearchSubFolderDecript = m_pApp->GetProfileInt("CMainFrame","bIsSearchSubFolderDecript",0);
	m_bIsOutOtherFileDecript    = m_pApp->GetProfileInt("CMainFrame","bIsOutOtherFileDecript",1);
	m_objFolderDecript          = m_pApp->GetProfileString("CMainFrame","FolderDecript","");
	m_objFileKindDecript        = m_pApp->GetProfileString("CMainFrame","FileKindDecript","");
	m_objFolder1DirCmp          = m_pApp->GetProfileString("CMainFrame","Folder1DirCmp","");
	m_objFolder2DirCmp          = m_pApp->GetProfileString("CMainFrame","Folder2DirCmp","");
	m_bIsSearchSubFolderTouch   = m_pApp->GetProfileInt("CMainFrame","bIsSearchSubFolderTouch",0);
	m_objFolderTouch            = m_pApp->GetProfileString("CMainFrame","FolderTouch","");
	m_objFileKindTouch          = m_pApp->GetProfileString("CMainFrame","FileKindTouch","");

	//���݂̃X�e�[�^�X�o�[�\�����擾
	m_stStatusBarDisp.m_nCurrentItemCount=0;
	for(nCnt=0; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		sprintf(szKey,"CurrentItem%d",nCnt);
		nBuf = m_pApp->GetProfileInt("CMainFrame",szKey,0);
		if(nBuf != 0)
		{
			m_stStatusBarDisp.m_nCurrentItems[m_stStatusBarDisp.m_nCurrentItemCount] = nBuf;
			m_stStatusBarDisp.m_nCurrentItemCount++;
		}
	}
	//�ǉ��̃X�e�[�^�X�o�[�\�����擾
	m_stStatusBarDisp.m_nAddItemCount=0;
	for(nCnt=0; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		sprintf(szKey,"AddItem%d",nCnt);
		nBuf = m_pApp->GetProfileInt("CMainFrame",szKey,0);
		if(nBuf != 0)
		{
			m_stStatusBarDisp.m_nAddItems[m_stStatusBarDisp.m_nAddItemCount] = nBuf;
			m_stStatusBarDisp.m_nAddItemCount++;
		}
	}
	//�f�t�H���g�ݒ�
	if(m_stStatusBarDisp.m_nCurrentItemCount==0 && m_stStatusBarDisp.m_nAddItemCount==0)
	{
		m_stStatusBarDisp.m_nCurrentItems[0] = ID_INDICATOR_GYORETU;
		m_stStatusBarDisp.m_nCurrentItems[1] = ID_INDICATOR_BYTE;
		m_stStatusBarDisp.m_nCurrentItems[2] = ID_INDICATOR_CHARCODE;
		m_stStatusBarDisp.m_nCurrentItems[3] = ID_INDICATOR_INS;
		m_stStatusBarDisp.m_nCurrentItems[4] = ID_INDICATOR_READONLY;
		m_stStatusBarDisp.m_nCurrentItems[5] = ID_INDICATOR_TIME;
		m_stStatusBarDisp.m_nCurrentItemCount = 6;
		m_stStatusBarDisp.m_nAddItems[0] = ID_INDICATOR_KANA;
		m_stStatusBarDisp.m_nAddItems[1] = ID_INDICATOR_CAPS;
		m_stStatusBarDisp.m_nAddItems[2] = ID_INDICATOR_NUM;
		m_stStatusBarDisp.m_nAddItems[3] = ID_INDICATOR_SCRL;
		m_stStatusBarDisp.m_nAddItemCount = 4;
	}

	//�����E�u���_�C�A���O�\����
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMainFrame::~CMainFrame()
{
	int	nCnt=0;
	int nBuf=0;
	char	szKey[64];

	//	���W�X�g���֐ݒ�ۑ�
	m_pApp->WriteProfileInt("CMainFrame","bIsSaveWindowPlace",m_bIsSaveWindowPlace);
	m_pApp->WriteProfileInt("CMainFrame","bIsUseProxyHtml",m_bIsUseProxyHtml);
	m_pApp->WriteProfileString("CMainFrame","RequestPathHtml",m_objRequestPathHtml);
	m_pApp->WriteProfileString("CMainFrame","ProxyIPAddress",m_objProxyIPAddressHtml);
	m_pApp->WriteProfileInt("CMainFrame","bIsDialupHtml",m_bIsDialupHtml);
	m_pApp->WriteProfileString("CMainFrame","DialupEntryHtml",m_objDialupEntryHtml);
	m_pApp->WriteProfileString("CMainFrame","RasUserNameHtml",m_objRasUserNameHtml);
	m_pApp->WriteProfileString("CMainFrame","RasPasswdHtml",m_objRasPasswdHtml);
	m_pApp->WriteProfileInt("CMainFrame","bIsAutoDisconnectHtml",m_bIsAutoDisconnectHtml);
	m_pApp->WriteProfileString("CMainFrame","FindString",m_objFindString);
	m_pApp->WriteProfileInt("CMainFrame","bIsMatchCaseFind",m_bIsMatchCaseFind);
	m_pApp->WriteProfileInt("CMainFrame","bIsMatchWholeWordFind",m_bIsMatchWholeWordFind);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchDownFind",m_bIsSearchDownFind);
	m_pApp->WriteProfileInt("CMainFrame","bIsUseRegularExpressionFind",m_bIsUseRegularExpressionFind);
	m_pApp->WriteProfileString("CMainFrame","ReplaceString",m_objReplaceString);
	m_pApp->WriteProfileInt("CMainFrame","bIsMatchCaseReplace",m_bIsMatchCaseReplace);
	m_pApp->WriteProfileInt("CMainFrame","bIsMatchWholeWordReplace",m_bIsMatchWholeWordReplace);
	m_pApp->WriteProfileInt("CMainFrame","bIsUseRegularExpressionReplace",m_bIsUseRegularExpressionReplace);
	m_pApp->WriteProfileInt("CMainFrame","bIsMatchCaseGrep",m_bIsMatchCaseGrep);
	m_pApp->WriteProfileInt("CMainFrame","bIsMatchWholeWordGrep",m_bIsMatchWholeWordGrep);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchSubFolderGrep",m_bIsSearchSubFolderGrep);
	m_pApp->WriteProfileInt("CMainFrame","bIsUseRegularExpressionGrep",m_bIsUseRegularExpressionGrep);
	m_pApp->WriteProfileString("CMainFrame","FolderGrep",m_objFolderGrep);
	m_pApp->WriteProfileString("CMainFrame","FileKindGrep",m_objFileKindGrep);
	m_pApp->WriteProfileString("CMainFrame","GrepStr",m_objGrepStr);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchSubFolderConv",m_bIsSearchSubFolderConv);
	m_pApp->WriteProfileInt("CMainFrame","bIsOutOtherFileConv",m_bIsOutOtherFileConv);
	m_pApp->WriteProfileInt("CMainFrame","nConvKind",m_nConvKind);
	m_pApp->WriteProfileString("CMainFrame","FolderConv",m_objFolderConv);
	m_pApp->WriteProfileString("CMainFrame","FileKindConv",m_objFileKindConv);
	m_pApp->WriteProfileInt("CMainFrame","InCodeConv",m_nInCodeConv);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchSubFolderKaigyoConv",m_bIsSearchSubFolderKaigyoConv);
	m_pApp->WriteProfileInt("CMainFrame","bIsOutOtherFileKaigyoConv",m_bIsOutOtherFileKaigyoConv);
	m_pApp->WriteProfileString("CMainFrame","KaigyoConvKind",m_objKaigyoConvKind);
	m_pApp->WriteProfileString("CMainFrame","FolderKaigyoConv",m_objFolderKaigyoConv);
	m_pApp->WriteProfileString("CMainFrame","FileKindKaigyoConv",m_objFileKindKaigyoConv);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchSubFolderWCount",m_bIsSearchSubFolderWCount);
	m_pApp->WriteProfileString("CMainFrame","FolderWCount",m_objFolderWCount);
	m_pApp->WriteProfileString("CMainFrame","FileKindWCount",m_objFileKindWCount);
	m_pApp->WriteProfileInt("CMainFrame","bIsAutoSave",m_bIsAutoSave);
	m_pApp->WriteProfileInt("CMainFrame","lSaveInterval",m_lSaveInterval);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchSubFolderFSearch",m_bIsSearchSubFolderFSearch);
	m_pApp->WriteProfileString("CMainFrame","FolderFSearch",m_objFolderFSearch);
	m_pApp->WriteProfileString("CMainFrame","FileKindFSearch",m_objFileKindFSearch);
	m_pApp->WriteProfileInt("CMainFrame","StatusDayStyle",m_lStatusDayStyle);
	m_pApp->WriteProfileInt("CMainFrame","StatusHourStyle",m_lStatusHourStyle);
	m_pApp->WriteProfileInt("CMainFrame","StatusLineStyle",m_lStatusLineStyle);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchSubFolderEncript",m_bIsSearchSubFolderEncript);
	m_pApp->WriteProfileInt("CMainFrame","bIsOutOtherFileEncript",m_bIsOutOtherFileEncript);
	m_pApp->WriteProfileString("CMainFrame","FolderEncript",m_objFolderEncript);
	m_pApp->WriteProfileString("CMainFrame","FileKindEncript",m_objFileKindEncript);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchSubFolderDecript",m_bIsSearchSubFolderDecript);
	m_pApp->WriteProfileInt("CMainFrame","bIsOutOtherFileDecript",m_bIsOutOtherFileDecript);
	m_pApp->WriteProfileString("CMainFrame","FolderDecript",m_objFolderDecript);
	m_pApp->WriteProfileString("CMainFrame","FileKindDecript",m_objFileKindDecript);
	m_pApp->WriteProfileString("CMainFrame","Folder1DirCmp",m_objFolder1DirCmp);
	m_pApp->WriteProfileString("CMainFrame","Folder2DirCmp",m_objFolder2DirCmp);
	m_pApp->WriteProfileInt("CMainFrame","bIsSearchSubFolderTouch",m_bIsSearchSubFolderTouch);
	m_pApp->WriteProfileString("CMainFrame","FolderTouch",m_objFolderTouch);
	m_pApp->WriteProfileString("CMainFrame","FileKindTouch",m_objFileKindTouch);

	//���݂̃X�e�[�^�X�o�[�\�����
	for(nCnt=0; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		sprintf(szKey,"CurrentItem%d",nCnt);
		if(m_stStatusBarDisp.m_nCurrentItems[nCnt] != 0)
			m_pApp->WriteProfileInt("CMainFrame",szKey,m_stStatusBarDisp.m_nCurrentItems[nCnt]);
		else
			m_pApp->WriteProfileInt("CMainFrame",szKey,0);
	}
	//�ǉ��̃X�e�[�^�X�o�[�\�����擾
	for(nCnt=0; nCnt<STS_INDICATORS_COUNT; nCnt++)
	{
		sprintf(szKey,"AddItem%d",nCnt);
		if(m_stStatusBarDisp.m_nAddItems[nCnt] != 0)
			m_pApp->WriteProfileInt("CMainFrame",szKey,m_stStatusBarDisp.m_nAddItems[nCnt]);
		else
			m_pApp->WriteProfileInt("CMainFrame",szKey,0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���b�Z�[�W�n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����޳���쐬�����O�ɌĂ΂�鏈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	LPCSTR   lpzsNewName = "XerxesWindowClass";
	WNDCLASS wndcls;

	BOOL bRes = CMDIFrameWnd::PreCreateWindow( cs );   // MDI �ł� CMDIFrameWnd
	HINSTANCE hInst = AfxGetInstanceHandle();

	// see if the class already exists
	if ( !::GetClassInfo( hInst, lpzsNewName, &wndcls ) )
	{
		// get default stuff
		::GetClassInfo( hInst, cs.lpszClass, &wndcls );
		// register a new class
		wndcls.lpszClassName = lpzsNewName;
		wndcls.hIcon = ::LoadIcon(hInst,MAKEINTRESOURCE(IDR_MAINFRAME));
		::RegisterClass( &wndcls );
	}
	cs.lpszClass = lpzsNewName;
	return bRes;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�r�h�y�d����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	//�ŏ������|���c�[���o�[��ԕۑ�
	if(nType == SIZE_MINIMIZED)
		SaveBarState(_T("ToolBar_State2"));

	CMDIFrameWnd::OnSize(nType, cx, cy);

	//�ŏ���������̕��A���|���c�[���o�[��ԕ���
	if(nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
		LoadBarState(_T("ToolBar_State2"));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�̍쐬��v������Ƃ��ɌĂ΂�鏈��							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//�l�c�h�w�i
	if(!m_wndClient.SubclassWindow(m_hWndMDIClient))
	{
		TRACE0("Failed to subclass MDI client window.\n");
		return -1;
	}
	//�c�[���o�[
	if(CreateToolBar() == FALSE)
	{
		TRACE0("Failed to Create ToolBar.\n");
		return -1;
	}
	//�X�e�[�^�X�o�[
	if(!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // �쐬�Ɏ��s
	}
	if(!SetStatusBarDispInfo(&m_stStatusBarDisp))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // �쐬�Ɏ��s
	}

	if( m_bIsSaveWindowPlace != 0 ) //�E�C���h�E�ʒu�ێ�
	{
		//�A�v���P�[�V�����̏�Ԃƈʒu���v���o��
		WINDOWPLACEMENT		wp;
		if(LoadWindowPlacement(&wp))
		{
			SetWindowPlacement(&wp);
		}
	}

	//�X�e�[�^�X�o�[�̃t�H���g�쐬���ݒ�
	memset(&m_stStatusBarLogFont,0,sizeof(LOGFONT));
	m_stStatusBarLogFont.lfHeight  = m_pApp->GetProfileInt("CMainFrame","STSFONT_lfHeight",12);
	m_stStatusBarLogFont.lfCharSet = m_pApp->GetProfileInt("CMainFrame","STSFONT_lfCharSet",DEFAULT_CHARSET);
	m_stStatusBarLogFont.lfItalic  = m_pApp->GetProfileInt("CMainFrame","STSFONT_lfItalic",FALSE);
	strcpy(m_stStatusBarLogFont.lfFaceName,m_pApp->GetProfileString("CMainFrame","STSFONT_lfFaceName","�l�r ����"));
	m_objStatusBarFont.CreateFontIndirect(&m_stStatusBarLogFont);
	m_wndStatusBar.SetFont(&m_objStatusBarFont);

	//���t�A�����p�����^�C�}�[�X�^�[�g
	SetTimer(1,55,NULL);

	// CG: ���̍s�́u�X�v���b�V�� �X�N���[���v�R���|�[�l���g�ɂ��ǉ�����Ă��܂��B
	CSplashWnd::ShowSplashScreen(this);

	//�c�[���o�[��ԕ���
	LoadBarState(_T("ToolBar_State2"));

	return 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�^�C���A�b�v������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == 1 ) //�ŏ��̃^�C�}�[
	{
		//���t�A�����p�����^�C�}�[�I��
		KillTimer(1);
		//���t�A�����p�^�C�}�[�X�^�[�g�i�P�O�b�Ԋu�j
		SetTimer(2,(10*1000),NULL);
	}

	//���t�A�����ݒ�
	SetDateTime();

	if(m_bIsAutoSave) //�I�[�g�Z�[�u���s��
	{
		//�^�C���A�b�v�J�E���^�X�V
		m_lTimeUpCount++;
		if(m_lTimeUpCount >= (m_lSaveInterval*6) ) //�I�[�g�Z�[�u�Ԋu�ɂȂ���
		{
			m_lTimeUpCount=0;
			m_pApp->DoFileAllSave();
		}
	}

	CMDIFrameWnd::OnTimer(nIDEvent);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E���j�������Ƃ��ɌĂ΂�鏈��								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnDestroy() 
{
	::RemoveProp(GetSafeHwnd(),m_pApp->m_pszExeName);

	CMDIFrameWnd::OnDestroy();

	//�X�e�[�^�X�o�[�t�H���g�̐ݒ�ۑ�
	m_pApp->WriteProfileInt("CMainFrame","STSFONT_lfHeight",m_stStatusBarLogFont.lfHeight);
	m_pApp->WriteProfileInt("CMainFrame","STSFONT_lfCharSet",m_stStatusBarLogFont.lfCharSet);
	m_pApp->WriteProfileInt("CMainFrame","STSFONT_lfItalic",m_stStatusBarLogFont.lfItalic);
	m_pApp->WriteProfileString("CMainFrame","STSFONT_lfFaceName",m_stStatusBarLogFont.lfFaceName);

	//���t�A�����p�P���^�C�}�[�I��
	KillTimer(2);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����޳�܂��ͱ��ع���݂��I�����Ȃ���΂Ȃ�Ȃ��Ƃ��ɌĂ΂�鏈��		|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnClose() 
{
	//////////////////////////////////////////
	//	�I�����ɊJ���Ă����t�@�C�����ۑ�	//
	//////////////////////////////////////////
	int nFileCnt=0;
	POSITION pos1 = m_pApp->GetFirstDocTemplatePosition();
	while(pos1 != 0)
	{
		CDocTemplate* pTempl = m_pApp->GetNextDocTemplate(pos1);
		POSITION pos2 = pTempl->GetFirstDocPosition();
		while(pos2 != 0)
		{
			CEditorDoc* pDoc = (CEditorDoc*)pTempl->GetNextDoc(pos2);
			if(pDoc != 0)
			{
				char	szEntry[32];
				sprintf(szEntry,"FnameOnExit%d",nFileCnt);
				m_pApp->WriteProfileString("CEditorApp",szEntry,pDoc->GetPathName());
				nFileCnt++;
			}
		}
	}
	m_pApp->WriteProfileInt("CEditorApp","FCountOnExit",nFileCnt);

	//////////////////////////
	//	�E�C���h�E�ʒu�ێ�	//
	//////////////////////////
	if( m_bIsSaveWindowPlace != 0 ) 
	{
		//�A�v���P�[�V�����̏�Ԃƈʒu���L��������
		WINDOWPLACEMENT		wp;
		if(GetWindowPlacement(&wp))
		{
			if(IsZoomed())
				wp.flags |= WPF_RESTORETOMAXIMIZED;
			SaveWindowPlacement(&wp);
		}
	}

	//�c�[���o�[��ԕێ�
	SaveBarState(_T("ToolBar_State2"));

	CMDIFrameWnd::OnClose();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����޳����è��/��è�ނɂȂ鎞�ɌĂ΂�鏈��							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	if( nState==WA_ACTIVE || nState==WA_CLICKACTIVE )
	{
		//�t�@�C���ύX�Ď�
		m_pApp->CheckFileChange();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �N���X�̐f�f

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�c�[���o�[�̃J�X�^�}�C�Y�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�W���c�[���o�[�̃J�X�^�}�C�Y����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnCustomizeToolbar() 
{
	m_wndToolBar.Customize();
}
void CMainFrame::OnUpdateCustomizeToolbar(CCmdUI* pCmdUI) 
{
	BOOL bVisible = m_wndToolBar.IsWindowVisible();
	BOOL bIsFloating = m_wndToolBar.IsFloating();
	if(bVisible && !bIsFloating)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ҏW�֘A�c�[���o�[�̃J�X�^�}�C�Y����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnCustomizeToolbaredit() 
{
	m_wndToolBarEdit.Customize();
}
void CMainFrame::OnUpdateCustomizeToolbaredit(CCmdUI* pCmdUI) 
{
	BOOL bVisible = m_wndToolBarEdit.IsWindowVisible();
	BOOL bIsFloating = m_wndToolBarEdit.IsFloating();
	if(bVisible && !bIsFloating)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����֘A�c�[���o�[�̃J�X�^�}�C�Y����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnCustomizeToolbarfind() 
{
	m_wndToolBarFind.Customize();
}
void CMainFrame::OnUpdateCustomizeToolbarfind(CCmdUI* pCmdUI) 
{
	BOOL bVisible = m_wndToolBarFind.IsWindowVisible();
	BOOL bIsFloating = m_wndToolBarFind.IsFloating();
	if(bVisible && !bIsFloating)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�W�����v�֘A�c�[���o�[�̃J�X�^�}�C�Y����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnCustomizeToolbarjmp() 
{
	m_wndToolBarJmp.Customize();
}
void CMainFrame::OnUpdateCustomizeToolbarjmp(CCmdUI* pCmdUI) 
{
	BOOL bVisible = m_wndToolBarJmp.IsWindowVisible();
	BOOL bIsFloating = m_wndToolBarJmp.IsFloating();
	if(bVisible && !bIsFloating)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�c�[���֘A�c�[���o�[�̃J�X�^�}�C�Y����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnCustomizeToolbartools() 
{
	m_wndToolBarTools.Customize();
}
void CMainFrame::OnUpdateCustomizeToolbartools(CCmdUI* pCmdUI) 
{
	BOOL bVisible = m_wndToolBarTools.IsWindowVisible();
	BOOL bIsFloating = m_wndToolBarTools.IsFloating();
	if(bVisible && !bIsFloating)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�֘A�c�[���o�[�̃J�X�^�}�C�Y����							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnCustomizeToolbarwnd() 
{
	m_wndToolBarWnd.Customize();
}
void CMainFrame::OnUpdateCustomizeToolbarwnd(CCmdUI* pCmdUI) 
{
	BOOL bVisible = m_wndToolBarWnd.IsWindowVisible();
	BOOL bIsFloating = m_wndToolBarWnd.IsFloating();
	if(bVisible && !bIsFloating)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�C���e���}�E�X�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�z�C�[���X�N���[��������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LRESULT CMainFrame::OnMSHMouseWheel(WPARAM wParam, LPARAM lParam)
{
	//���݂̃A�N�e�B�u�r���[�ƃA�N�e�B�u�h�L�������g���擾
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd==NULL)
		return 0;
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc==NULL)
		return 0;
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView==NULL)
		return 0;

	int nDelta = (int)wParam;
	if(nDelta < 0) //���[�U�����փ}�E�X�z�C�[�������[�e�[�g
	{
		pobjEditorView->OnVScroll(SB_PAGEDOWN,NULL,pobjEditorView->GetScrollBarCtrl(SB_VERT));
	}
	else //���j�^�[�����փ}�E�X�z�C�[�������[�e�[�g
	{
		pobjEditorView->OnVScroll(SB_PAGEUP,NULL,pobjEditorView->GetScrollBarCtrl(SB_VERT));
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�t�@�C���֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���|�v���p�e�B�v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnFileProperty() 
{
	UINT				unCodeKind;
	CFileStatus			objFileStatus;
	CFilePropertyDlg	objFilePropertyDlg;
	CCodeConvManager	objCodeConvManager;
	char	szBuf[256], szFileName[_MAX_PATH], szFilePath[_MAX_PATH];
	CString				objMsg;

	//�A�N�e�B�u�h�L�������g�擾
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd==NULL)
		return;
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc==NULL)
		return;

	//�h�L�������g���擾
	objFilePropertyDlg.m_objTitle = pobjEditorDoc->GetTitle();
	memset(szFileName,0,sizeof(szFileName));
	memset(szFilePath,0,sizeof(szFilePath));
	pobjEditorDoc->MyGetFileName(szFileName);
	objFilePropertyDlg.m_objFileName = szFileName;
	pobjEditorDoc->MyGetFilePath(szFilePath);

	if( _access(szFilePath, 0) != 0 )
	{
		objMsg.LoadString(IDS_CANTACCESS_FILE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	if(strlen(szFilePath))
	{
		CFile::GetStatus(szFilePath,objFileStatus);
		objFilePropertyDlg.m_objCTime = objFileStatus.m_ctime.Format("%Y/%m/%d %H:%M:%S");
		objFilePropertyDlg.m_objMTime = objFileStatus.m_mtime.Format("%Y/%m/%d %H:%M:%S");
		objFilePropertyDlg.m_objATime = objFileStatus.m_atime.Format("%Y/%m/%d");

		long lSumByte = 0;
		long lCaretByte = 0;
		pobjEditorDoc->GetBytes(0,&lCaretByte,&lSumByte);
		long lKB = (long) ((objFileStatus.m_size / 1024) + ((objFileStatus.m_size % 1024) ? 1 : 0));
		sprintf(szBuf,"%ld KB (%ld Byte)",lKB, lSumByte);

		objFilePropertyDlg.m_objSize  = szBuf;
		objFilePropertyDlg.m_objAttribute = _T("");
		if(objFileStatus.m_attribute == 0x00)
			objFilePropertyDlg.m_objAttribute += "normal, ";
		if( (objFileStatus.m_attribute & 0x01) == 0x01 )
			objFilePropertyDlg.m_objAttribute += "ReadOnly, ";
		else
			objFilePropertyDlg.m_objAttribute += "WriteOK, ";
		if( (objFileStatus.m_attribute & 0x02) == 0x02 )
			objFilePropertyDlg.m_objAttribute += "Hidden, ";
		if( (objFileStatus.m_attribute & 0x04) == 0x04 )
			objFilePropertyDlg.m_objAttribute += "System, ";
		if( (objFileStatus.m_attribute & 0x08) == 0x08 )
			objFilePropertyDlg.m_objAttribute += "Volume, ";
		if( (objFileStatus.m_attribute & 0x10) == 0x10 )
			objFilePropertyDlg.m_objAttribute += "Directory, ";
		if( (objFileStatus.m_attribute & 0x20) == 0x20 )
			objFilePropertyDlg.m_objAttribute += "Archive";
		unCodeKind = objCodeConvManager.DetectCode(szFilePath);	
		if(unCodeKind == INPUT_SJIS)
			objFilePropertyDlg.m_objCode = "SJIS";
		else if(unCodeKind == INPUT_EUC)
			objFilePropertyDlg.m_objCode = "EUC";
		else if(unCodeKind == INPUT_JIS)
			objFilePropertyDlg.m_objCode = "JIS";
		else if(unCodeKind == INPUT_ASCII)
			objFilePropertyDlg.m_objCode = "ASCII";
		else if(unCodeKind == INPUT_UNKNOWN)
			objFilePropertyDlg.m_objCode = "UNKNOWN";
		else if(unCodeKind == INPUT_BINARY)
			objFilePropertyDlg.m_objCode = "BINARY";
	}
	if(pobjEditorDoc->GetKaigyoKind() == KAIGYO_CRLF)
		objFilePropertyDlg.m_objKaigyo = "CR+LF";
	else if(pobjEditorDoc->GetKaigyoKind() == KAIGYO_LF)
		objFilePropertyDlg.m_objKaigyo = "LF";
	else if(pobjEditorDoc->GetKaigyoKind() == KAIGYO_CR)
		objFilePropertyDlg.m_objKaigyo = "CR";
	sprintf(szBuf,"%ld",pobjEditorDoc->GetLineCount());
	objFilePropertyDlg.m_objLineCnt  = szBuf;
	sprintf(szBuf,"%ld",pobjEditorDoc->GetTabSize());
	objFilePropertyDlg.m_objTabSize  = szBuf;

	//�t�@�C���v���p�e�B�_�C�A���O�\��
	objFilePropertyDlg.DoModal();
}
void CMainFrame::OnUpdateFileProperty(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�t�@�C�����`�F�b�N
	char	szBuf[_MAX_PATH];
	pobjEditorDoc->MyGetFilePath(szBuf);
	if(!strlen(szBuf))
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pobjEditorDoc->MyGetFileName(szBuf);
	if(!strlen(szBuf))
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C�������A
	//�Í����A�������`�F�b�N
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���|�㏑���ۑ����ĕ���v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnFileSaveclose() 
{
	//�A�N�e�B�u�h�L�������g�擾
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();

	//�㏑���ۑ�
	if( !pobjEditorDoc->IsReadOnly() )
		pobjEditorDoc->OnFileSave();

	//����
	pobjFrameWnd->SendMessage(WM_CLOSE);
}
void CMainFrame::OnUpdateFileSaveclose(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�h�L�������g�G���v�e�B�`�F�b�N
	if( pobjEditorDoc->IsEmpty() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�����݋֎~�`�F�b�N
	if( pobjEditorDoc->IsReadOnly() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C������
	//�Í����A�������`�F�b�N
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���|WEB���HTML�t�@�C�����J���v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnHttpFileOpen() 
{
	long	lRet=0;
	CString objMsg, objTitle, objFilePath;
	CBasicAuthorizationDlg objBasicAuthorizationDlg;
	CHtmlOpenDlg		objHtmlOpenDlg;
	CHtmlOpenResultDlg	objHtmlOpenResultDlg;
	BOOL bRet=FALSE;
	BOOL bIsUseBasicAuth=FALSE;
	CString objRealm = _T("");
	CHttp	objHttp;
	char	szBuf[256],szMsg[512],szRasMsg[512];
	DWORD	dwRasState=0;
	CFile	objFile;


	//�J���Ώۂ̂g�s�l�k�t�@�C�����w�肵�Ă��炤
	objHtmlOpenDlg.m_bIsUseProxy = m_bIsUseProxyHtml;
	objHtmlOpenDlg.m_objRequestPath = m_objRequestPathHtml;
	objHtmlOpenDlg.m_objProxyIPAddress = m_objProxyIPAddressHtml;
	if(objHtmlOpenDlg.DoModal() != IDOK)
		return;
	m_bIsUseProxyHtml = objHtmlOpenDlg.m_bIsUseProxy;
	m_objRequestPathHtml = objHtmlOpenDlg.m_objRequestPath;
	m_objProxyIPAddressHtml = objHtmlOpenDlg.m_objProxyIPAddress;

//	//test
//	objBasicAuthorizationDlg.m_objAuthSite  = objHtmlOpenDlg.m_objSite;
//	objBasicAuthorizationDlg.m_objAuthRealm = objRealm;
//	objBasicAuthorizationDlg.DoModal();
//	return;

	//�󋵃��[�h���X�_�C�A���O�\��
	m_pobjHtmlOpenStatusDlg = new CHtmlOpenStatusDlg();
	m_pobjHtmlOpenStatusDlg->Create(IDD_HTMLOPENSTATUS);
	WaitForHtmlOpenDlgDisp();

	if(m_bIsDialupHtml) //�_�C�A���A�b�v�ڑ�
	{
		if(m_pApp->m_hRasDllInstance != NULL)
		{
			//�֐��A�h���X�擾
			m_pApp->m_lpfnRasFunc3 = (LPFNRASFUNC3)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasConnect");
			m_pApp->m_lpfnRasFunc4 = (LPFNRASFUNC4)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasIsConnected");
			m_pApp->m_lpfnRasFunc5 = (LPFNRASFUNC5)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasDisConnect");
			if( m_pApp->m_lpfnRasFunc3 != NULL && 
				m_pApp->m_lpfnRasFunc4 != NULL && 
				m_pApp->m_lpfnRasFunc5 != NULL)
			{
				//�q�`�r�ڑ�(rasConnect)
				objMsg.LoadString(IDS_RASCONNECT);
				m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
				lRet = m_pApp->m_lpfnRasFunc3((char*)(LPCTSTR)m_objDialupEntryHtml,
									(char*)(LPCTSTR)m_objRasUserNameHtml,
									(char*)(LPCTSTR)m_objRasPasswdHtml,
									"",
									szBuf );
				if(lRet == -6)
				{
					//�q�`�r�ؒf(rasDisconnect)
					objMsg.LoadString(IDS_RASDISCONNECT);
					m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
					m_pApp->m_lpfnRasFunc5();
					objMsg.LoadString(IDS_RASCONNECTERR);
					sprintf(szMsg,(LPCTSTR)objMsg,szBuf);
					m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);

					//�󋵃��[�h���X�_�C�A���O����
					m_pobjHtmlOpenStatusDlg->DestroyWindow();
					delete m_pobjHtmlOpenStatusDlg;
					m_pobjHtmlOpenStatusDlg = NULL;
					return;
				}
				else if(lRet == 1) //���łɃI�[�v���ς�
				{
					objMsg.LoadString(IDS_RASCONNECTEND);
					m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
					//���̂܂܂g�s�l�k�t�@�C���I�[�v����
				}
				else if(lRet == 0) //�q�`�r�ڑ��n�j
				{
					//�ڑ������҂�
					for(;;)
					{
						m_pApp->MessagePump();
						Sleep(10);
						if(m_pobjHtmlOpenStatusDlg->IsCanceled())
						{
							//�q�`�r�ؒf(rasDisconnect)
							objMsg.LoadString(IDS_RASDISCONNECT);
							m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
							m_pApp->m_lpfnRasFunc5();

							//�󋵃��[�h���X�_�C�A���O����
							m_pobjHtmlOpenStatusDlg->DestroyWindow();
							delete m_pobjHtmlOpenStatusDlg;
							m_pobjHtmlOpenStatusDlg = NULL;
							return;
						}

						//��ԃ`�F�b�N(rasIsConnected)
						memset(szRasMsg,0,sizeof(szRasMsg));
						lRet = m_pApp->m_lpfnRasFunc4( &dwRasState, szRasMsg );
						objMsg.LoadString(IDS_RASISCONNECT);
						sprintf(szMsg,(LPCTSTR)objMsg,dwRasState,szRasMsg);
						m_pobjHtmlOpenStatusDlg->SetStatus(szMsg);
						if(lRet) //�ڑ����ꂽ
						{
							objMsg.LoadString(IDS_RASCONNECTEND);
							m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
							break;
						}
						else
						{
							if(dwRasState == 100 || dwRasState == 8193)
							{
								//�q�`�r�ؒf(rasDisconnect)
								m_pApp->m_lpfnRasFunc5();
								objMsg.LoadString(IDS_RAS_CONNECTERR);
								m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);

								//�󋵃��[�h���X�_�C�A���O����
								m_pobjHtmlOpenStatusDlg->DestroyWindow();
								delete m_pobjHtmlOpenStatusDlg;
								m_pobjHtmlOpenStatusDlg = NULL;
								return;
							}
						}
					}
				}
			}
		}
	}

	//�󋵕\��
	if(m_pobjHtmlOpenStatusDlg->IsCanceled())
	{
		if( m_bIsDialupHtml && m_bIsAutoDisconnectHtml && m_pApp->m_lpfnRasFunc5) //�_�C�A���A�b�v�ڑ�
		{
			//�q�`�r�ؒf(rasDisconnect)
			objMsg.LoadString(IDS_RASDISCONNECT);
			m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
			m_pApp->m_lpfnRasFunc5();
		}
		//�󋵃��[�h���X�_�C�A���O����
		m_pobjHtmlOpenStatusDlg->DestroyWindow();
		delete m_pobjHtmlOpenStatusDlg;
		m_pobjHtmlOpenStatusDlg = NULL;
		return;
	}

	//�L���b�V���p�t�@�C���p�X���쐬
	if(m_pApp->IsDriveRoot(m_pApp->m_strCashePath))
		objFilePath.Format("%s%s",m_pApp->m_strCashePath,objHtmlOpenDlg.m_objFileName);
	else
		objFilePath.Format("%s\\%s",m_pApp->m_strCashePath,objHtmlOpenDlg.m_objFileName);
	if(!objFile.Open(objFilePath,(CFile::modeCreate|CFile::modeWrite),NULL))
	{
		objMsg.LoadString(IDS_HTTP_FILEOPENERR);
		m_pApp->MyMessageBox((LPCTSTR)objMsg,(MB_OK|MB_ICONSTOP),0);

		//�󋵃��[�h���X�_�C�A���O����
		m_pobjHtmlOpenStatusDlg->DestroyWindow();
		delete m_pobjHtmlOpenStatusDlg;
		m_pobjHtmlOpenStatusDlg = NULL;

		return;
	}
	CArchive ar( &objFile, CArchive::store );

	//�g�s�l�k�T�[�o�ڑ�
	objMsg.LoadString(IDS_HTTP_CONNECT);
	m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
	bRet = objHttp.Connect(objHtmlOpenDlg.m_objHost,objHtmlOpenDlg.m_bIsUseProxy);
	if(bRet) //OK
	{
		//�󋵕\��
		objMsg.LoadString(IDS_HTTP_GET);
		m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
		if(m_pobjHtmlOpenStatusDlg->IsCanceled())
		{
			if( m_bIsDialupHtml && m_bIsAutoDisconnectHtml && m_pApp->m_lpfnRasFunc5) //�_�C�A���A�b�v�ڑ�
			{
				//�q�`�r�ؒf(rasDisconnect)
				objMsg.LoadString(IDS_RASDISCONNECT);
				m_pobjHtmlOpenStatusDlg->SetStatus(objMsg);
				m_pApp->m_lpfnRasFunc5();
			}

			//�ؒf
			objHttp.Disconnect();

			//�󋵃��[�h���X�_�C�A���O����
			m_pobjHtmlOpenStatusDlg->DestroyWindow();
			delete m_pobjHtmlOpenStatusDlg;
			m_pobjHtmlOpenStatusDlg = NULL;

			//�L���b�V���t�@�C���N���[�Y
			ar.Close();
			objFile.Close();
			return;
		}

		//HTML�t�@�C���擾
		bIsUseBasicAuth = FALSE;
		objRealm = _T("");
		bRet = objHttp.Get(objHtmlOpenDlg.m_objPath,ar,m_pobjHtmlOpenStatusDlg,bIsUseBasicAuth,"","",objRealm);
		if(!bRet && bIsUseBasicAuth) //Basic�F�؂��K�v
		{
			objBasicAuthorizationDlg.m_objAuthSite  = objHtmlOpenDlg.m_objSite;
			objBasicAuthorizationDlg.m_objAuthRealm = objRealm;
			if(objBasicAuthorizationDlg.DoModal() == IDOK)
			{
				//Basic�F�؂��g�p���čĐڑ����Ď擾
				bRet = objHttp.Connect(objHtmlOpenDlg.m_objHost,objHtmlOpenDlg.m_bIsUseProxy);
				if(bRet) //OK
				{
					bRet = objHttp.Get( objHtmlOpenDlg.m_objPath,
										ar,
										m_pobjHtmlOpenStatusDlg,
										bIsUseBasicAuth,
										objBasicAuthorizationDlg.m_objAuthUser,
										objBasicAuthorizationDlg.m_objAuthPasswd,
										objRealm);
				}
			}
		}
	}

	if( m_bIsDialupHtml && m_bIsAutoDisconnectHtml && m_pApp->m_lpfnRasFunc5) //�_�C�A���A�b�v�ڑ�
	{
		//�q�`�r�ؒf(rasDisconnect)
		objMsg.LoadString(IDS_RASDISCONNECT);
		m_pobjMailListStatusDlg->SetStatus(objMsg);
		m_pApp->m_lpfnRasFunc5();
	}

	//�L���b�V���t�@�C���N���[�Y
	ar.Close();
	objFile.Close();
	if(bRet != FALSE)//�I�[�v��
		m_pApp->OpenFile(objFilePath);

	//�󋵃��[�h���X�_�C�A���O����
	m_pobjHtmlOpenStatusDlg->DestroyWindow();
	delete m_pobjHtmlOpenStatusDlg;
	m_pobjHtmlOpenStatusDlg = NULL;

	//�I���ʒm
	if( bRet != FALSE )
		objMsg.LoadString(IDS_HTTPOPEN_END);
	else
		objMsg.LoadString(IDS_HTTPOPEN_CANCEL);
	objHtmlOpenResultDlg.m_objStatus = objMsg;
	objHttp.GetResponseHeader(objHtmlOpenResultDlg.m_objHeader);
	objHtmlOpenResultDlg.DoModal();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���|�ēǂݍ��݁v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnFileReread() 
{
	//�A�N�e�B�u�h�L�������g�擾
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();

	//�}�E�X�J�[�\�������v
	CWaitCursor	objWaitCursor;

	//�ēǂݍ��݁i�_�C�A���O�m�F�Ȃ��j
	pobjEditorDoc->ReloadDocument(FALSE);
}
void CMainFrame::OnUpdateFileReread(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�h�L�������g�G���v�e�B�`�F�b�N
	if( pobjEditorDoc->IsEmpty() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() != FALSE )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C������
	//�Í����A�������`�F�b�N
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���|�폜�v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnFileDelete() 
{
	//���݂̃A�N�e�B�u�r���[�ƃA�N�e�B�u�h�L�������g���擾
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd==NULL)
		return;
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc==NULL)
		return;

	SHFILEOPSTRUCT	stFileOp;
	memset(&stFileOp,0,sizeof(SHFILEOPSTRUCT));
	char	szFrom[_MAX_PATH];
	memset(szFrom,0,sizeof(szFrom));

	//�r���o�߂�\������E�C���h�E�̐e�̃E�C���h�E
	CWnd* pWnd = (CWnd*)AfxGetMainWnd();
	stFileOp.hwnd = pWnd->m_hWnd;
	//�I�y���[�V����
	stFileOp.wFunc = FO_DELETE;
	//��
	pobjEditorDoc->MyGetFilePath(szFrom);
	stFileOp.pFrom = szFrom;
	//����Ɋւ���t���O
	stFileOp.fFlags = FOF_ALLOWUNDO;

	//�t�@�C���폜�������s
	if( ::SHFileOperation(&stFileOp) == 0 ) //OK
	{
		if( _access(szFrom,0) == -1 ) //�{���ɍ폜���ꂽ
		{
			//�E�C���h�E�N���[�Y
			pobjFrameWnd->SendMessage(WM_CLOSE);
			//�l�q�t���X�g����폜
			m_pApp->RemoveRecentFileList(szFrom);
		}
	}
}
void CMainFrame::OnUpdateFileDelete(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd == NULL) || (pobjFrameWnd == this) )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc==NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�t�@�C�����`�F�b�N
	char	szBuf[_MAX_PATH];
	pobjEditorDoc->MyGetFilePath(szBuf);
	if(!strlen(szBuf))
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	pobjEditorDoc->MyGetFileName(szBuf);
	if(!strlen(szBuf))
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C�������A
	//�Í����A�������`�F�b�N
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�ҏW�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|�N���b�v�{�[�h�̒�������v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditClipview() 
{
	//�N���b�v�{�[�h�̒��g��\��
	CClipBoardViewDlg	objClipBoardViewDlg;
	objClipBoardViewDlg.DoModal();
}
void CMainFrame::OnUpdateEditClipview(CCmdUI* pCmdUI) 
{
	//�n�k�d�f�[�^�I�u�W�F�N�g�쐬
	COleDataObject	objOleDataObject;

	//�N���b�v�{�[�h��̃f�[�^�I�u�W�F�N�g�Ɍ��ѕt���܂��B
	BOOL bRet = objOleDataObject.AttachClipboard();
	if( bRet != 0 )
	{
		//�f�[�^��CF_TEXT�`���ŗ��p�ł��邩�𒲍�
		bRet = objOleDataObject.IsDataAvailable(CF_TEXT);
		if( bRet != 0 )
			pCmdUI->Enable(TRUE);
		else
			pCmdUI->Enable(FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|�����݋֎~�ݒ�v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditSetreadonly() 
{
	//�A�N�e�B�u�h�L�������g�擾
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd==NULL)
		return;
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc==NULL)
		return;

	//�����݋֎~�ݒ�
	if(pobjEditorDoc->SetReadOnly())
	{
		//�����݋֎~Ӱ�޽ð���߲ݐݒ�
		SetReadOnlyMode(TRUE);
	}
}
void CMainFrame::OnUpdateEditSetreadonly(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C������
	//�Í����A�������`�F�b�N
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�����݋֎~�`�F�b�N
	if( pobjEditorDoc->IsReadOnly() ) //���łɏ����݋֎~�ɂȂ��Ă���
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|�����݋֎~�����v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditUnsetreadonly() 
{
	//�A�N�e�B�u�h�L�������g�擾
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd==NULL)
		return;
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc==NULL)
		return;

	//�����݋֎~����
	if(pobjEditorDoc->UnsetReadOnly())
	{
		//�����݋֎~Ӱ�޽ð���߲ݐݒ�
		SetReadOnlyMode(FALSE);
	}
}
void CMainFrame::OnUpdateEditUnsetreadonly(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() == TRUE )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C������
	//�Í����A�������`�F�b�N
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�����݋֎~�`�F�b�N
	if( !pobjEditorDoc->IsReadOnly() ) //���łɏ����݋֎~����������Ă���
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�\���֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|�W���c�[���o�[�v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnViewToolbar() 
{
	BOOL bVisible = m_wndToolBar.IsWindowVisible();

	ShowControlBar(&m_wndToolBar, bVisible ? FALSE : TRUE, FALSE );
}
void CMainFrame::OnUpdateViewToolbar(CCmdUI* pCmdUI) 
{
	if(m_wndToolBar.IsWindowVisible())
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|�ҏW�֘A�c�[���o�[�v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnViewToolbarEdit() 
{
	BOOL bVisible = m_wndToolBarEdit.IsWindowVisible();

	ShowControlBar(&m_wndToolBarEdit, bVisible ? FALSE : TRUE, FALSE );
}
void CMainFrame::OnUpdateViewToolbarEdit(CCmdUI* pCmdUI) 
{
	if(m_wndToolBarEdit.IsWindowVisible())
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|�����֘A�c�[���o�[�v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnViewToolbarFind() 
{
	BOOL bVisible = m_wndToolBarFind.IsWindowVisible();

	ShowControlBar(&m_wndToolBarFind, bVisible ? FALSE : TRUE, FALSE );
}
void CMainFrame::OnUpdateViewToolbarFind(CCmdUI* pCmdUI) 
{
	if(m_wndToolBarFind.IsWindowVisible())
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|�W�����v�֘A�c�[���o�[�v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnViewToolbarJmp() 
{
	BOOL bVisible = m_wndToolBarJmp.IsWindowVisible();

	ShowControlBar(&m_wndToolBarJmp, bVisible ? FALSE : TRUE, FALSE );
}
void CMainFrame::OnUpdateViewToolbarJmp(CCmdUI* pCmdUI) 
{
	if(m_wndToolBarJmp.IsWindowVisible())
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|�c�[���֘A�c�[���o�[�v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnViewToolbarTools() 
{
	BOOL bVisible = m_wndToolBarTools.IsWindowVisible();

	ShowControlBar(&m_wndToolBarTools, bVisible ? FALSE : TRUE, FALSE );
}
void CMainFrame::OnUpdateViewToolbarTools(CCmdUI* pCmdUI) 
{
	if(m_wndToolBarTools.IsWindowVisible())
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|�E�C���h�E�֘A�c�[���o�[�v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnViewToolbarWnd() 
{
	BOOL bVisible = m_wndToolBarWnd.IsWindowVisible();

	ShowControlBar(&m_wndToolBarWnd, bVisible ? FALSE : TRUE, FALSE );
}
void CMainFrame::OnUpdateViewToolbarWnd(CCmdUI* pCmdUI) 
{
	if(m_wndToolBarWnd.IsWindowVisible())
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�c�[���֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�c�[���|���[���ꗗ�v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnFileMaillist() 
{
	int				nMailCount=0;
	long			lCnt=0, lRet=0;
	char			szBuf[256],szMsg[512],szRasMsg[512];
	CPop			objPop;
	CMailListDlg	objMailListDlg;
	CString			objStr;

	//���݂̃A�J�E���g�ɑΉ�����A���[���֘A�ݒ���擾
	char			szSection[64];
	int				nAccountNo = m_pApp->GetProfileInt("Mail","CurrentAccountNo",0);
	if(nAccountNo < 0 || nAccountNo > 8)
		nAccountNo = 0;
	sprintf(szSection,"Mail_Account%d",nAccountNo);

	CString objPopServer   = m_pApp->GetProfileString(szSection,"PopServer","");
	CString objPopUser     = m_pApp->GetProfileString(szSection,"PopUser","");
	CString objPopPasswd   = m_pApp->GetProfileString(szSection,"PopPasswd","");
	BOOL bIsDialup         = m_pApp->GetProfileInt(szSection,"bIsDialup",0);
	CString objDialupEntry = m_pApp->GetProfileString(szSection,"DialupEntry","");
	CString objRasUserName = m_pApp->GetProfileString(szSection,"RasUserName","");
	CString objRasPasswd   = m_pApp->GetProfileString(szSection,"RasPasswd","");
	BOOL bAutoDisconnect   = m_pApp->GetProfileInt(szSection,"AutoDisconnect",0);
	int nAuthorizationType = m_pApp->GetProfileInt(szSection,"nAuthorizationType",POP3_AUTHENTICATE_STANDARD);
	DWORD			dwRasState=0;
	CString			objMsg;

	//���O�`�F�b�N
	if(objPopServer.IsEmpty())
	{
		objMsg.LoadString(IDS_POP_NOSERVER);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if(objPopUser.IsEmpty())
	{
		objMsg.LoadString(IDS_POP_NOUSER);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if(objPopPasswd.IsEmpty())
	{
		objMsg.LoadString(IDS_POP_NOPASSWD);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//���[�����X�g�󋵃��[�h���X�_�C�A���O�\��
	m_pobjMailListStatusDlg = new CMailListStatusDlg();
	m_pobjMailListStatusDlg->Create(IDD_MAILLISTSTATUS);
	WaitForMailListDlgDisp();


	if(bIsDialup) //�_�C�A���A�b�v�ڑ�
	{
		if(m_pApp->m_hRasDllInstance != NULL)
		{
			//�֐��A�h���X�擾
			m_pApp->m_lpfnRasFunc3 = (LPFNRASFUNC3)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasConnect");
			m_pApp->m_lpfnRasFunc4 = (LPFNRASFUNC4)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasIsConnected");
			m_pApp->m_lpfnRasFunc5 = (LPFNRASFUNC5)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasDisConnect");
			if( m_pApp->m_lpfnRasFunc3 != NULL && 
				m_pApp->m_lpfnRasFunc4 != NULL && 
				m_pApp->m_lpfnRasFunc5 != NULL)
			{
				//�q�`�r�ڑ�(rasConnect)
				objMsg.LoadString(IDS_RASCONNECT);
				m_pobjMailListStatusDlg->SetStatus(objMsg);
				lRet = m_pApp->m_lpfnRasFunc3((char*)(LPCTSTR)objDialupEntry,
									(char*)(LPCTSTR)objRasUserName,
									(char*)(LPCTSTR)objRasPasswd,
									"",
									szBuf );
				if(lRet == -6)
				{
					//�q�`�r�ؒf(rasDisconnect)
					objMsg.LoadString(IDS_RASDISCONNECT);
					m_pobjMailListStatusDlg->SetStatus(objMsg);
					m_pApp->m_lpfnRasFunc5();
					objMsg.LoadString(IDS_RASCONNECTERR);
					sprintf(szMsg,(LPCTSTR)objMsg,szBuf);
					m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);

					//���[�����X�g�󋵃��[�h���X�_�C�A���O����
					m_pobjMailListStatusDlg->DestroyWindow();
					delete m_pobjMailListStatusDlg;
					m_pobjMailListStatusDlg = NULL;
					return;
				}
				else if(lRet == 1) //���łɃI�[�v���ς�
				{
					objMsg.LoadString(IDS_RASCONNECTEND);
					m_pobjMailListStatusDlg->SetStatus(objMsg);
					//���̂܂܃��[�����X�g�\����
				}
				else if(lRet == 0) //�q�`�r�ڑ��n�j
				{
					//�ڑ������҂�
					for(;;)
					{
						m_pApp->MessagePump();
						Sleep(10);
						if(m_pobjMailListStatusDlg->IsCanceled())
						{
							//�q�`�r�ؒf(rasDisconnect)
							objMsg.LoadString(IDS_RASDISCONNECT);
							m_pobjMailListStatusDlg->SetStatus(objMsg);
							m_pApp->m_lpfnRasFunc5();

							//���[�����X�g�󋵃��[�h���X�_�C�A���O����
							m_pobjMailListStatusDlg->DestroyWindow();
							delete m_pobjMailListStatusDlg;
							m_pobjMailListStatusDlg = NULL;
							return;
						}

						//��ԃ`�F�b�N(rasIsConnected)
						memset(szRasMsg,0,sizeof(szRasMsg));
						lRet = m_pApp->m_lpfnRasFunc4( &dwRasState, szRasMsg );
						objMsg.LoadString(IDS_RASISCONNECT);
						sprintf(szMsg,(LPCTSTR)objMsg,dwRasState,szRasMsg);
						m_pobjMailListStatusDlg->SetStatus(szMsg);
						if(lRet) //�ڑ����ꂽ
						{
							objMsg.LoadString(IDS_RASCONNECTEND);
							m_pobjMailListStatusDlg->SetStatus(objMsg);
							break;
						}
						else
						{
							if(dwRasState == 100 || dwRasState == 8193)
							{
								//�q�`�r�ؒf(rasDisconnect)
								m_pApp->m_lpfnRasFunc5();
								objMsg.LoadString(IDS_RAS_CONNECTERR);
								m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);

								//���[�����X�g�󋵃��[�h���X�_�C�A���O����
								m_pobjMailListStatusDlg->DestroyWindow();
								delete m_pobjMailListStatusDlg;
								m_pobjMailListStatusDlg = NULL;
								return;
							}
						}
					}
				}
			}
		}
	}

	//�󋵕\��
	if(m_pobjMailListStatusDlg->IsCanceled())
	{
		if( bIsDialup && bAutoDisconnect && m_pApp->m_lpfnRasFunc5) //�_�C�A���A�b�v�ڑ�
		{
			//�q�`�r�ؒf(rasDisconnect)
			objMsg.LoadString(IDS_RASDISCONNECT);
			m_pobjMailListStatusDlg->SetStatus(objMsg);
			m_pApp->m_lpfnRasFunc5();
		}
		//���[�����X�g�󋵃��[�h���X�_�C�A���O����
		m_pobjMailListStatusDlg->DestroyWindow();
		delete m_pobjMailListStatusDlg;
		m_pobjMailListStatusDlg = NULL;
		return;
	}

	//���[�����X�g
	objMsg.LoadString(IDS_MAILSERVER_CONNECT);
	m_pobjMailListStatusDlg->SetStatus(objMsg);
	if(objPop.Connect(objPopServer,objPopUser,objPopPasswd,nAuthorizationType))
	{
		//�󋵕\��
		objMsg.LoadString(IDS_MAILLIST_GET);
		m_pobjMailListStatusDlg->SetStatus(objMsg);
		if(m_pobjMailListStatusDlg->IsCanceled())
		{
			if( bIsDialup && bAutoDisconnect && m_pApp->m_lpfnRasFunc5) //�_�C�A���A�b�v�ڑ�
			{
				//�q�`�r�ؒf(rasDisconnect)
				objMsg.LoadString(IDS_RASDISCONNECT);
				m_pobjMailListStatusDlg->SetStatus(objMsg);
				m_pApp->m_lpfnRasFunc5();
			}

			//�ؒf
			objPop.Disconnect();

			//���[�����X�g�󋵃��[�h���X�_�C�A���O����
			m_pobjMailListStatusDlg->DestroyWindow();
			delete m_pobjMailListStatusDlg;
			m_pobjMailListStatusDlg = NULL;
			return;
		}

		nMailCount = objPop.GetMailCount();
		if(nMailCount==0) //�o�n�o�T�[�o���[�����`�F�b�N
		{
			//���[�����[���ʒm
			objMsg.LoadString(IDS_POP_LISTZERO);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
			//�ؒf
			objPop.Disconnect();
		}
		else if(nMailCount > 0)
		{
			//���[�����X�g�_�C�A���O�\��
			objMailListDlg.m_objPopServerStr = objPopServer;
			objMailListDlg.m_objPopUserStr = objPopUser;
			objMailListDlg.m_objPopPasswdStr = objPopPasswd;
			objMailListDlg.m_nMailCount = nMailCount;
			objMailListDlg.m_pPop = &objPop;
			objMailListDlg.DoModal();
		}
	}

	if( bIsDialup && bAutoDisconnect && m_pApp->m_lpfnRasFunc5) //�_�C�A���A�b�v�ڑ�
	{
		//�q�`�r�ؒf(rasDisconnect)
		objMsg.LoadString(IDS_RASDISCONNECT);
		m_pobjMailListStatusDlg->SetStatus(objMsg);
		m_pApp->m_lpfnRasFunc5();
	}

	//���[�����X�g�󋵃��[�h���X�_�C�A���O����
	m_pobjMailListStatusDlg->DestroyWindow();
	delete m_pobjMailListStatusDlg;
	m_pobjMailListStatusDlg = NULL;
}
void CMainFrame::OnUpdateFileMaillist(CCmdUI* pCmdUI) 
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�L�[����̋L�^�J�n/�I���v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnToolOpeStartendrecord() 
{
	int		nRet=0;
	char	szMsg[256];
	CString	objStr = _T("");
	CString objMsg;

	if(m_bIsNowOperationRecording == FALSE) //�L�^�J�n�̏ꍇ
	{
		//�L�[���얢�ۑ��̃`�F�b�N
		if(m_objOperationManager.GetOperationCount() > 0 )
		{
			m_objOperationManager.GetOperationFileName(objStr);
			if(objStr.IsEmpty())
			{
				objMsg.LoadString(IDS_KEYOPE_UNSAVED);
				nRet = m_pApp->MyMessageBox(objMsg,(MB_OKCANCEL|MB_ICONEXCLAMATION),0);
				if(nRet == IDCANCEL)
					return;
			}
		}

		m_objOperationManager.Initialize();
		objMsg.LoadString(IDS_KEYOPE_STARTRECORD);
		if(m_pApp->MyMessageBox(objMsg,(MB_OKCANCEL|MB_ICONINFORMATION),0)==IDCANCEL)
			return;
	}
	else //�L�^�I���̏ꍇ
	{
		objMsg.LoadString(IDS_KEYOPE_RECORDEND);
		sprintf(szMsg,(LPCTSTR)objMsg,m_objOperationManager.GetOperationCount() );
		m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
	//�L�[����L�^���t���O�ؑւ�
	m_bIsNowOperationRecording = (m_bIsNowOperationRecording==TRUE) ? FALSE : TRUE;
}
void CMainFrame::OnUpdateToolOpeStartendrecord(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C������
	//�Í����A�������`�F�b�N
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�����݋֎~�`�F�b�N
	if( pobjEditorDoc->IsReadOnly() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�L�[����̃X�e�b�v���s���`�F�b�N
	if(	IsNowStepDoOperation() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�C���N�������^���T�[�`���s���`�F�b�N
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�L�[����̎��s�v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnToolOpeDo() 
{
	//���݂̃A�N�e�B�u�r���[�ƃA�N�e�B�u�h�L�������g���擾
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();

	//�L�[����̎��s
	m_objOperationManager.doOperation(pobjEditorView,pobjEditorDoc);
}
void CMainFrame::OnUpdateToolOpeDo(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C������
	//�Í����A�������`�F�b�N
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�����݋֎~�`�F�b�N
	if( pobjEditorDoc->IsReadOnly() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�L�[����L�^���`�F�b�N
	if( IsNowOperationRecording() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�L�[����L�^���`�F�b�N
	if( m_objOperationManager.GetOperationCount() == 0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�L�[����̃X�e�b�v���s���`�F�b�N
	if(	IsNowStepDoOperation() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�C���N�������^���T�[�`���s���`�F�b�N
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�L�[����̃X�e�b�v���s�@�|�@���̃L�[����v�I��������				|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnToolOpeStepdoNext() 
{
	BOOL bRet = 0;
	CString objMsg;

	//���݂̃A�N�e�B�u�r���[�ƃA�N�e�B�u�h�L�������g���擾
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();

	//�L�[����̃X�e�b�v���s
	if(m_bIsNowStepDoOperation == FALSE)
		bRet = m_objOperationManager.stepDoOperation(pobjEditorView,pobjEditorDoc,TRUE);//����
	else
		bRet = m_objOperationManager.stepDoOperation(pobjEditorView,pobjEditorDoc,FALSE);//�Q��ڈȍ~

	//�L�[����̃X�e�b�v���s���t���O�I��
	m_bIsNowStepDoOperation = TRUE;

	if(bRet == FALSE) //����ŏI���
	{
		objMsg.LoadString(IDS_KEYOPE_STEPDOEND);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		//�L�[����̃X�e�b�v���s���t���O�I�t
		m_bIsNowStepDoOperation = FALSE;
	}
}
void CMainFrame::OnUpdateToolOpeStepdoNext(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C������
	//�Í����A�������`�F�b�N
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�����݋֎~�`�F�b�N
	if( pobjEditorDoc->IsReadOnly() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�L�[����L�^���`�F�b�N
	if( IsNowOperationRecording() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�L�[����L�^���`�F�b�N
	if( m_objOperationManager.GetOperationCount() == 0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�C���N�������^���T�[�`���s���`�F�b�N
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�L�[����̃X�e�b�v���s�@�|�@�I���v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnToolOpeStepdoEnd() 
{
	//�L�[����̃X�e�b�v���s���t���O�I�t
	m_bIsNowStepDoOperation = FALSE;

	//�L�[����X�e�b�v���s�I��
	m_objOperationManager.endStepDoOperation();

	CString objMsg;
	objMsg.LoadString(IDS_KEYOPE_STEPDOEND2);
	m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
}
void CMainFrame::OnUpdateToolOpeStepdoEnd(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�f�q�d�o�A�_���v�A�R�[�h�ϊ��A���[�h�J�E���g�A�t�@�C������
	//�Í����A�������`�F�b�N
	if( pobjEditorDoc->IsGrep()    ||
		pobjEditorDoc->IsDump()    ||
		pobjEditorDoc->IsConv()    ||
		pobjEditorDoc->IsWCount()  ||
		pobjEditorDoc->IsFSearch() ||
		pobjEditorDoc->IsBinary()  ||
		pobjEditorDoc->IsEncript() ||
		pobjEditorDoc->IsDecript() ||
		pobjEditorDoc->IsEncode()  ||
		pobjEditorDoc->IsCompare() ||
		pobjEditorDoc->IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�����݋֎~�`�F�b�N
	if( pobjEditorDoc->IsReadOnly() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�L�[����L�^���`�F�b�N
	if( IsNowOperationRecording() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�L�[����L�^���`�F�b�N
	if( m_objOperationManager.GetOperationCount() == 0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�L�[����X�e�b�v���s���`�F�b�N
	if( IsNowStepDoOperation() == FALSE)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�C���N�������^���T�[�`���s���`�F�b�N
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�L�[������t�@�C���֕ۑ��v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnToolOpeSave() 
{
	char*	pPtr=NULL;
	char	szFileName[_MAX_FNAME];
	CString objStrFName=_T("");
	CString objMsg;

	//�t�@�C���ۑ��R�����_�C�A���O�\��
	CFileDialog	objFileDialog(FALSE);
	if( objFileDialog.DoModal() == IDOK )
	{
		//�t�@�C�����擾
		objStrFName = objFileDialog.GetPathName();
		strcpy(szFileName,(LPCTSTR)objStrFName);

		//�t�@�C���I�[�v��
		CFile	objFile;
		if( !objFile.Open( szFileName, CFile::modeWrite | CFile::modeCreate, NULL ) )
		{
			objMsg.LoadString(IDS_FOPEN_ERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
		CArchive	ar(&objFile,CArchive::store);

		//�L�[������t�@�C���ۑ�
		m_objOperationManager.Serialize(ar);

		//�L�[����t�@�C�����ݒ�
		m_objOperationManager.SetOperationFileName(objStrFName);

		//�t�@�C���N���[�Y
		ar.Close();
		objFile.Close();
	}
}
void CMainFrame::OnUpdateToolOpeSave(CCmdUI* pCmdUI) 
{
	//�L�[����L�^���`�F�b�N
	if( m_objOperationManager.GetOperationCount() == 0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�L�[����L�^���`�F�b�N
	if( IsNowOperationRecording() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�L�[����X�e�b�v���s���`�F�b�N
	if( IsNowStepDoOperation() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�C���N�������^���T�[�`���s���`�F�b�N
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�L�[������t�@�C������ǂݍ��ށv�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnToolOpeOpen() 
{
	char*	pPtr=NULL;
	char	szFileName[_MAX_FNAME];
	CString objStrFName=_T("");
	CString objMsg;

	//�t�@�C���ǂݍ��݃R�����_�C�A���O�\��
	CFileDialog	objFileDialog(TRUE);
	if( objFileDialog.DoModal() == IDOK )
	{
		//�t�@�C�����擾
		objStrFName = objFileDialog.GetPathName();
		strcpy(szFileName,(LPCTSTR)objStrFName);

		//�t�@�C���I�[�v��
		CFile	objFile;
		if( !objFile.Open( szFileName, CFile::modeRead, NULL ) )
		{
			objMsg.LoadString(IDS_FOPEN_ERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
		CArchive	ar(&objFile,CArchive::load);

		//�L�[����t�@�C���`�F�b�N
		if(m_objOperationManager.checkIsOperationFile(ar) != FALSE)
		{
			//�I�y���[�V����������
			m_objOperationManager.Initialize();

			//�L�[������t�@�C������ǂݍ���
			BOOL bRet = m_objOperationManager.Serialize(ar);
			if(bRet == FALSE)
			{
				objMsg.LoadString(IDS_KEYOPE_FOPENERR);
				m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
				m_objOperationManager.Initialize();
			}
			else
			{
				//�I�y���[�V�����t�@�C�����ݒ�
				m_objOperationManager.SetOperationFileName(objStrFName);

				objMsg.LoadString(IDS_KEYOPE_OPENEND);
				m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
			}
		}
		else //�w�肳�ꂽ�t�@�C���̓L�[����t�@�C���ł͂Ȃ�
		{
			objMsg.LoadString(IDS_NOT_KEYOPEFILE);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		}

		//�t�@�C���N���[�Y
		ar.Close();
		objFile.Close();
	}
}
void CMainFrame::OnUpdateToolOpeOpen(CCmdUI* pCmdUI) 
{
	//�L�[����L�^���`�F�b�N
	if( IsNowOperationRecording() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�L�[����X�e�b�v���s���`�F�b�N
	if( IsNowStepDoOperation() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�C���N�������^���T�[�`���s���`�F�b�N
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�L�[����̓��e�\���v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnToolOpeDisp() 
{
	//�I�y���[�V�����ꗗ�\��
	m_objOperationManager.DispOperationLists();
}
void CMainFrame::OnUpdateToolOpeDisp(CCmdUI* pCmdUI) 
{
	//�L�[����L�^���`�F�b�N
	if( m_objOperationManager.GetOperationCount() == 0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�L�[����L�^���`�F�b�N
	if( IsNowOperationRecording() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�L�[����X�e�b�v���s���`�F�b�N
	if( IsNowStepDoOperation() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�C���N�������^���T�[�`���s���`�F�b�N
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�E�C���h�E�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ŏ����v�I��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnWindowMinimize() 
{
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd != NULL) && (pobjFrameWnd != this) )
	{
		pobjFrameWnd->ShowWindow(SW_SHOWMINIMIZED);

		//�T�E���h
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowMinimize",0) != 0)
		{
			CString strPath;
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowMinimize",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
	}
}
void CMainFrame::OnUpdateWindowMinimize(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd == NULL) || (pobjFrameWnd == this) )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ő剻�v�I��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnWindowMaximize() 
{
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd != NULL) && (pobjFrameWnd != this) )
	{
		pobjFrameWnd->ShowWindow(SW_SHOWMAXIMIZED);

		//�T�E���h
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowMaximize",0) != 0)
		{
			CString strPath;
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowMaximize",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
	}
}
void CMainFrame::OnUpdateWindowMaximize(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd == NULL) || (pobjFrameWnd == this) )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u���Ƃ̃T�C�Y�ɖ߂��v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnWindowRestore() 
{
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd != NULL) && (pobjFrameWnd != this) )
	{
		pobjFrameWnd->ShowWindow(SW_RESTORE);

		//�T�E���h
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowRestore",0) != 0)
		{
			CString strPath;
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowRestore",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
	}
}
void CMainFrame::OnUpdateWindowRestore(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd == NULL) || (pobjFrameWnd == this) )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u���̃E�C���h�E�v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnWndNext() 
{
	MDINext();

	//�T�E���h
	if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowNext",0) != 0)
	{
		CString strPath;
		strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowNext",""));
		if(_access((LPCTSTR)strPath,0) != -1)
			m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
	}
}
void CMainFrame::OnUpdateWndNext(CCmdUI* pCmdUI) 
{
	int	nCount=0;
	CWnd* pWnd = MDIGetActive();
	while(pWnd != NULL)
	{
		if(pWnd != FromHandle(m_hWnd))
			nCount++;
		pWnd = pWnd->GetNextWindow();
	}

	if(nCount <= 1)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�O�̃E�C���h�E�v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnWndPrev() 
{
	CWnd* pWnd = MDIGetActive()->GetNextWindow(GW_HWNDLAST);
	if(pWnd != NULL)
	{
		pWnd->BringWindowToTop();

		//�T�E���h
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowPrev",0) != 0)
		{
			CString strPath;
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowPrev",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
	}
}
void CMainFrame::OnUpdateWndPrev(CCmdUI* pCmdUI) 
{
	int	nCount=0;
	CWnd* pWnd = MDIGetActive();
	while(pWnd != NULL)
	{
		if(pWnd != FromHandle(m_hWnd))
			nCount++;
		pWnd = pWnd->GetNextWindow();
	}

	if(nCount <= 1)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�E�C���h�E�����v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnWindowClose() 
{
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd != NULL) && (pobjFrameWnd != this) )
	{
		pobjFrameWnd->SendMessage(WM_CLOSE);

		//�T�E���h
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowClose",0) != 0)
		{
			CString strPath;
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowClose",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
	}
}
void CMainFrame::OnUpdateWindowClose(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd == NULL) || (pobjFrameWnd == this) )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u���ׂẴE�C���h�E�����v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnWindowAllclose() 
{
	CFrameWnd* pobjFrameWnd = NULL;

	//�T�E���h
	if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowAllClose",0) != 0)
	{
		//Wave�t�@�C���p�X�擾
		CString strPath;
		strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowAllClose",""));
		if(_access((LPCTSTR)strPath,0) != -1)
			m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
	}

	for(;;)
	{
		pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
		if( (pobjFrameWnd == NULL) || (pobjFrameWnd == this) )
			break;

		pobjFrameWnd->SendMessage(WM_CLOSE);
	}
}
void CMainFrame::OnUpdateWindowAllclose(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if( (pobjFrameWnd == NULL) || (pobjFrameWnd == this) )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�����֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�����|������̌����v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFind() 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
		return;
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
		return;
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
		return;
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
		return;

	//�����͈͑I������Ă����ꍇ�ɂ́A���������������Ƃ���
	if(pobjEditorView->IsSelected() && pobjEditorView->GetSelectLineCount()==1)
	{
		long lSelectLineCnt=0;
		CLine* pobjLine=pobjEditorView->MyGetSelectedText(&lSelectLineCnt);
		m_objFindString = pobjLine->String();
		delete pobjLine;
	}

	//�����_�C�A���O�\��
	CFindDlg	objFindDlg;
	objFindDlg.m_pobjMainFrame = this;
	objFindDlg.m_pobjEditorView = pobjEditorView;
	objFindDlg.m_objFindString = m_objFindString;
	objFindDlg.m_bIsMatchWholeWordFind = m_bIsMatchWholeWordFind;
	objFindDlg.m_bIsMatchCaseFind = m_bIsMatchCaseFind;
	objFindDlg.m_bIsSearchDownFind = m_bIsSearchDownFind;
	objFindDlg.m_bIsUseRegularExpression = m_bIsUseRegularExpressionFind;
	objFindDlg.DoModal();
}
void CMainFrame::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�h�L�������g�G���v�e�B�`�F�b�N
	if( pobjEditorDoc->IsEmpty() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�C���N�������^���T�[�`���s���`�F�b�N
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�����|������̒u���v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditReplace() 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
		return;
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
		return;
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
		return;
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
		return;

	//�����͈͑I������Ă����ꍇ�ɂ́A���������������Ƃ���
	if(pobjEditorView->IsSelected() && pobjEditorView->GetSelectLineCount()==1)
	{
		long lSelectLineCnt=0;
		CLine* pobjLine=pobjEditorView->MyGetSelectedText(&lSelectLineCnt);
		m_objFindString = pobjLine->String();
		delete pobjLine;
	}

	//�u���_�C�A���O�\��
	CReplaceDlg		objReplaceDlg;
	objReplaceDlg.m_pobjMainFrame = this;
	objReplaceDlg.m_pobjEditorView = pobjEditorView;
	objReplaceDlg.m_objFindString = m_objFindString;
	objReplaceDlg.m_objReplaceString = m_objReplaceString;
	objReplaceDlg.m_bIsMatchWholeWordReplace = m_bIsMatchWholeWordReplace;
	objReplaceDlg.m_bIsMatchCaseReplace = m_bIsMatchCaseReplace;
	objReplaceDlg.m_bIsUseRegularExpression = m_bIsUseRegularExpressionReplace;
	objReplaceDlg.DoModal();
}
void CMainFrame::OnUpdateEditReplace(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�G���v�e�B�������݋֎~�`�F�b�N
	if( pobjEditorDoc->IsEmpty() || pobjEditorDoc->IsReadOnly() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�C���N�������^���T�[�`���s���`�F�b�N
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u������̌������������s�i�e�R�j�v����������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFindcontinue() 
{
	CString objMsg;

	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
		return;
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
		return;
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
		return;

	if( m_objFindString.IsEmpty() )
	{
		MessageBeep((UINT)-1);
		return;
	}

	//�}�E�X�J�[�\�������v
	CWaitCursor	objWaitCursor;

	//�������s
	BOOL bRet = pobjEditorView->DoFind( m_objFindString,
										m_bIsMatchCaseFind,
										m_bIsMatchWholeWordFind,
										TRUE,
										m_bIsUseRegularExpressionFind);
	if( bRet == FALSE ) // �������I��
	{
		objMsg.LoadString(IDS_FINDENDMSG);
		CString objMsgStr;
		objMsgStr.Format(objMsg,m_objFindString);
		m_pApp->MyMessageBox((LPCTSTR)objMsgStr,(MB_OK|MB_ICONINFORMATION),0);
		//�I��͈͉���
		pobjEditorView->ClearSelectArea();
	}
}
void CMainFrame::OnUpdateEditFindcontinue(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�h�L�������g�G���v�e�B�`�F�b�N
	if( pobjEditorDoc->IsEmpty() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�O�񌟍������񑶍݃`�F�b�N
	if( m_objFindString.IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�C���N�������^���T�[�`���s���`�F�b�N
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u������̌������������s�i�r���������{�e�R�j�v����������				|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFindrevcontinue() 
{
	CString objMsg;

	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
		return;
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
		return;
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
		return;

	if( m_objFindString.IsEmpty() )
	{
		MessageBeep((UINT)-1);
		return;
	}

	//�}�E�X�J�[�\�������v
	CWaitCursor	objWaitCursor;

	//�������s
	BOOL bRet = pobjEditorView->DoFind( m_objFindString,
										m_bIsMatchCaseFind,
										m_bIsMatchWholeWordFind,
										FALSE,
										m_bIsUseRegularExpressionFind);
	if( bRet == FALSE ) // �������I��
	{
		objMsg.LoadString(IDS_FINDENDMSG);
		CString objMsgStr;
		objMsgStr.Format(objMsg,m_objFindString);
		m_pApp->MyMessageBox((LPCTSTR)objMsgStr,(MB_OK|MB_ICONINFORMATION),0);
		//�I��͈͉���
		pobjEditorView->ClearSelectArea();
	}
}
void CMainFrame::OnUpdateEditFindrevcontinue(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�h�L�������g�G���v�e�B�`�F�b�N
	if( pobjEditorDoc->IsEmpty() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�O�񌟍������񑶍݃`�F�b�N
	if( m_objFindString.IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�C���N�������^���T�[�`���s���`�F�b�N
	if( IsNowIncrimentalSearching() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�����|�C���N�������^���T�[�`�v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditIncrimentalsearch() 
{
	CString objMsg;
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();

	//�C���N�������^���T�[�`���s���t���O�؂�ւ�
	m_bIsNowIncrimentalSearching = (m_bIsNowIncrimentalSearching!=FALSE) ? FALSE : TRUE;

	if(m_bIsNowIncrimentalSearching)//�J�n
	{
		objMsg.LoadString(IDS_START_INCRIMENTALSEARCH);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		//�C���N�������^���T�[�`�J�n�s�A�I�t�Z�b�g�N���A
		pobjEditorView->ClearIncrimentalSearchLineOffset();
		//�X�e�[�^�X�y�C���Ƀ��b�Z�[�W�ݒ�
		objMsg.LoadString(IDS_INCREMENTALSEARCH_NOW);
		SetStatusMessage(objMsg);
	}
	else //�I��
	{
		objMsg.LoadString(IDS_END_INCRIMENTALSEARCH);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		//�C���N�������^���T�[�`������������p��
		pobjEditorView->GetIncrimentalSearchString(m_objFindString);
		//�C���N�������^���T�[�`�J�n�s�A�I�t�Z�b�g�N���A
		pobjEditorView->ClearIncrimentalSearchLineOffset();
		//�X�e�[�^�X�y�C���Ƀ��b�Z�[�W�ݒ�
		objMsg.LoadString(AFX_IDS_IDLEMESSAGE);
		SetStatusMessage(objMsg);
	}
}
void CMainFrame::OnUpdateEditIncrimentalsearch(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�G���v�e�B�������݋֎~�`�F�b�N
	if( pobjEditorDoc->IsEmpty() || pobjEditorDoc->IsReadOnly() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�f�q�d�o�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�����|�f�q�d�o�v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditGrep() 
{
	CGrepDlg		objGrepDlg;

	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd != NULL)
	{
		//�A�N�e�B�u�r���[���݃`�F�b�N
		CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
		CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
		if(pobjEditorView != NULL && pobjEditorDoc != NULL)
		{
			//����E����v���r���[���`�F�b�N
			if( pobjEditorDoc->GetFlagPrintPreview() == FALSE )
			{
				//�����͈͑I������Ă����ꍇ�ɂ́A���������������Ƃ���
				if(pobjEditorView->IsSelected() && pobjEditorView->GetSelectLineCount()==1)
				{
					long lSelectLineCnt=0;
					CLine* pobjLine=pobjEditorView->MyGetSelectedText(&lSelectLineCnt);
					m_objGrepStr = pobjLine->String();
					delete pobjLine;
				}
			}
		}
	}

	//�f�q�d�o�����f�t�H���g�ݒ�
	objGrepDlg.m_bIsMatchCaseGrep = m_bIsMatchCaseGrep;				// �啶���A��������ʂ̗L��
	objGrepDlg.m_bIsMatchWholeWordGrep = m_bIsMatchWholeWordGrep;	// �P��S�̂���v���鎖���������H
	objGrepDlg.m_bIsSearchSubFolderGrep = m_bIsSearchSubFolderGrep;	// �T�u�t�H���_���������邩�H
	objGrepDlg.m_bIsUseRegularExpression = m_bIsUseRegularExpressionGrep; // ���K�\�����g�p���邩�H
	objGrepDlg.m_objFolder = m_objFolderGrep;						// �f�q�d�o���s�t�H���_
	objGrepDlg.m_objGrepString = m_objGrepStr;						// ����������
	objGrepDlg.m_objFileKind = m_objFileKindGrep;					// �����Ώۂ̃t�@�C�����

	if( objGrepDlg.DoModal() != IDOK )
		return;

	//�f�q�d�o�����擾���ێ�
	m_bIsMatchCaseGrep = objGrepDlg.m_bIsMatchCaseGrep;				// �啶���A��������ʂ̗L��
	m_bIsMatchWholeWordGrep = objGrepDlg.m_bIsMatchWholeWordGrep;	// �P��S�̂���v���鎖���������H
	m_bIsSearchSubFolderGrep = objGrepDlg.m_bIsSearchSubFolderGrep;	// �T�u�t�H���_���������邩�H
	m_bIsUseRegularExpressionGrep = objGrepDlg.m_bIsUseRegularExpression;	// ���K�\�����g�p���邩�H
	m_objFolderGrep = objGrepDlg.m_objFolder;						// �f�q�d�o���s�t�H���_
	m_objGrepStr = objGrepDlg.m_objGrepString;						// ����������
	m_objFileKindGrep = objGrepDlg.m_objFileKind;					// �����Ώۂ̃t�@�C�����

	//�f�q�d�o���s
	DoGrep();

	//���K�\�����g�����ꍇ�Aregfree���Ăяo���Č�n������
	if(m_bIsUseRegularExpressionGrep && m_pApp->m_lpfnRegFunc4!=NULL)
		m_pApp->m_lpfnRegFunc4();
}
void CMainFrame::OnUpdateEditGrep(CCmdUI* pCmdUI) 
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�����R�[�h�ϊ��֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�c�[���|�����R�[�h�ϊ��v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditCodeconv()
{
	CString			objMsg;
	CCodeConvDlg	objCodeConvDlg;

	// �����R�[�h�ϊ������f�t�H���g�ݒ�
	objCodeConvDlg.m_bIsOutOtherFile = m_bIsOutOtherFileConv;		// �ϊ����ʂ�ʃt�@�C���֏o�͂��邩�H
	objCodeConvDlg.m_bIsSearchSubFolder = m_bIsSearchSubFolderConv;	// �T�u�t�H���_���ϊ����邩�H
	objCodeConvDlg.m_nConvKind = m_nConvKind;						// �����R�[�h�ϊ����
	objCodeConvDlg.m_objFolder = m_objFolderConv;					// �����R�[�h�ϊ����s�t�H���_
	objCodeConvDlg.m_objFileKind = m_objFileKindConv;				// �ϊ��Ώۂ̃t�@�C�����
	objCodeConvDlg.m_nInCode = m_nInCodeConv;						// ���̓t�@�C���̕����R�[�h������

	if( objCodeConvDlg.DoModal() != IDOK )
		return;

	// �����R�[�h�ϊ������擾���ێ�
	m_bIsOutOtherFileConv = objCodeConvDlg.m_bIsOutOtherFile;		// �ϊ����ʂ�ʃt�@�C���֏o�͂��邩�H
	m_bIsSearchSubFolderConv = objCodeConvDlg.m_bIsSearchSubFolder;	// �T�u�t�H���_���ϊ����邩�H
	m_nConvKind = objCodeConvDlg.m_nConvKind;						// �����R�[�h�ϊ����
	m_objFolderConv = objCodeConvDlg.m_objFolder;					// �����R�[�h�ϊ����s�t�H���_
	m_objFileKindConv = objCodeConvDlg.m_objFileKind;				// �ϊ��Ώۂ̃t�@�C�����
	m_nInCodeConv = objCodeConvDlg.m_nInCode;						// ���̓t�@�C���̕����R�[�h������

	// �����R�[�h�ϊ����s
	DoCodeConv();
}
void CMainFrame::OnUpdateEditCodeconv(CCmdUI* pCmdUI)
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i���s�R�[�h�ϊ��֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�c�[���|���s�R�[�h�ϊ��v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditKaigyoconv() 
{
	CKaigyoConvDlg	objKaigyoConvDlg;

	// ���s�R�[�h�ϊ������f�t�H���g�ݒ�
	objKaigyoConvDlg.m_bIsOutOtherFile = m_bIsOutOtherFileKaigyoConv;		// �ϊ����ʂ�ʃt�@�C���֏o�͂��邩�H
	objKaigyoConvDlg.m_bIsSearchSubFolder = m_bIsSearchSubFolderKaigyoConv;	// �T�u�t�H���_���ϊ����邩�H
	objKaigyoConvDlg.m_objConvKind = m_objKaigyoConvKind;					// ���s�R�[�h�ϊ����
	objKaigyoConvDlg.m_objFolder = m_objFolderKaigyoConv;					// ���s�R�[�h�ϊ����s�t�H���_
	objKaigyoConvDlg.m_objFileKind = m_objFileKindKaigyoConv;				// �ϊ��Ώۂ̃t�@�C�����

	if( objKaigyoConvDlg.DoModal() != IDOK )
		return;

	// ���s�R�[�h�ϊ������擾���ێ�
	m_bIsOutOtherFileKaigyoConv = objKaigyoConvDlg.m_bIsOutOtherFile;		// �ϊ����ʂ�ʃt�@�C���֏o�͂��邩�H
	m_bIsSearchSubFolderKaigyoConv = objKaigyoConvDlg.m_bIsSearchSubFolder;	// �T�u�t�H���_���ϊ����邩�H
	m_objKaigyoConvKind = objKaigyoConvDlg.m_objConvKind;					// �����R�[�h�ϊ����
	m_objFolderKaigyoConv = objKaigyoConvDlg.m_objFolder;					// �����R�[�h�ϊ����s�t�H���_
	m_objFileKindKaigyoConv = objKaigyoConvDlg.m_objFileKind;				// �ϊ��Ώۂ̃t�@�C�����

	// ���s�R�[�h�ϊ����s
	DoKaigyoCodeConv();
}
void CMainFrame::OnUpdateEditKaigyoconv(CCmdUI* pCmdUI) 
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i���[�h�J�E���g�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�c�[���|���[�h�J�E���g�v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditWordcount() 
{
	CWordCountDlg	objWordCountDlg;

	// ���[�h�J�E���g�����f�t�H���g�ݒ�
	objWordCountDlg.m_bIsSearchSubFolder = m_bIsSearchSubFolderWCount;	// �T�u�t�H���_���ϊ����邩�H
	objWordCountDlg.m_objFolder = m_objFolderWCount;					// ���[�h�J�E���g���s�t�H���_
	objWordCountDlg.m_objFileKind = m_objFileKindWCount;				// ���[�h�J�E���g�Ώۂ̃t�@�C�����

	if( objWordCountDlg.DoModal() != IDOK )
		return;

	// ���[�h�J�E���g�����擾���ێ�
	m_bIsSearchSubFolderWCount = objWordCountDlg.m_bIsSearchSubFolder;	// �T�u�t�H���_���ϊ����邩�H
	m_objFolderWCount = objWordCountDlg.m_objFolder;					// ���[�h�J�E���g���s�t�H���_
	m_objFileKindWCount = objWordCountDlg.m_objFileKind;				// ���[�h�J�E���g�Ώۂ̃t�@�C�����

	// ���[�h�J�E���g���s
	DoWordCount();
}
void CMainFrame::OnUpdateEditWordcount(CCmdUI* pCmdUI) 
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�t�@�C�������֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�c�[���|�t�@�C�������v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFilesearch() 
{
	CFileSearchDlg	objFileSearchDlg;

	// �t�@�C�����������f�t�H���g�ݒ�
	objFileSearchDlg.m_bIsSearchSubFolder = m_bIsSearchSubFolderFSearch;// �T�u�t�H���_���ϊ����邩�H
	objFileSearchDlg.m_objFolder = m_objFolderFSearch;					// ���[�h�J�E���g���s�t�H���_
	objFileSearchDlg.m_objFileKind = m_objFileKindFSearch;				// ���[�h�J�E���g�Ώۂ̃t�@�C�����

	if( objFileSearchDlg.DoModal() != IDOK )
		return;

	// �t�@�C�����������擾���ێ�
	m_bIsSearchSubFolderFSearch = objFileSearchDlg.m_bIsSearchSubFolder;// �T�u�t�H���_���ϊ����邩�H
	m_objFolderFSearch = objFileSearchDlg.m_objFolder;					// ���[�h�J�E���g���s�t�H���_
	m_objFileKindFSearch = objFileSearchDlg.m_objFileKind;				// ���[�h�J�E���g�Ώۂ̃t�@�C�����

	// �t�@�C���������s
	DoFileSearch();
}
void CMainFrame::OnUpdateEditFilesearch(CCmdUI* pCmdUI) 
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�t�@�C���^�b�`�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�c�[���|�t�@�C���^�b�`�v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFiletouch() 
{
	CFileTouchDlg	objFileTouchDlg;

	// �t�@�C���^�b�`�����f�t�H���g�ݒ�
	objFileTouchDlg.m_bIsSearchSubFolder = m_bIsSearchSubFolderTouch;	// �T�u�t�H���_���^�b�`���邩�H
	objFileTouchDlg.m_objFolder = m_objFolderTouch;						// �^�b�`���s�t�H���_
	objFileTouchDlg.m_objFileKind = m_objFileKindTouch;					// �^�b�`�Ώۂ̃t�@�C�����

	if( objFileTouchDlg.DoModal() != IDOK )
		return;

	// �t�@�C���^�b�`�����擾���ێ�
	m_bIsSearchSubFolderTouch = objFileTouchDlg.m_bIsSearchSubFolder;	// �T�u�t�H���_���^�b�`���邩�H
	m_objFolderTouch = objFileTouchDlg.m_objFolder;						// �^�b�`���s�t�H���_
	m_objFileKindTouch = objFileTouchDlg.m_objFileKind;					// �^�b�`�Ώۂ̃t�@�C�����
	m_objFileTouchTime = objFileTouchDlg.m_mtime;

	// �t�@�C���^�b�`���s
	DoFileTouch();
}
void CMainFrame::OnUpdateEditFiletouch(CCmdUI* pCmdUI) 
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�_���v�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�c�[���|�`�r�b�h�h�R�[�h�\���_���v�v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditAsciidump() 
{
	long	lCRCount=0;
	long	lLFCount=0;
	char	szAsciiDataBuf[24];
	char	szAsciiData[128];
	strcpy(szAsciiData,ASCII_DATA);
	CLine	objHeaderLine;
	CString objMsg;

	//�A�N�e�B�u�t���[���擾
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
		return;
	//�A�N�e�B�u�h�L�������g�擾
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
		return;
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
		return;
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
		return;

	//�_���v���ʏo�͗p�l�c�h�q�E�C���h�E�쐬
	m_pobjDumpDoc = NULL;
	m_pobjDumpFrameWnd = NULL;
	BOOL bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjDumpDoc,&m_pobjDumpFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjDumpDoc->SetFontOfCurrentFileExt("");
	m_pobjDumpDoc->SetColorOfCurrentFileExt("");

	//�_���v���[�h�ݒ�
	SetReadOnlyMode(TRUE);//�����݋֎~Ӱ�޽ð���߲ݐݒ�
	m_pobjDumpDoc->SetDumpMode(TRUE);

	//�^�C�g���ݒ�
	CString	objTitle;
	objMsg.LoadString(IDS_ASCIIDUMP_TITLE);
	objTitle.Format(objMsg,pobjEditorDoc->GetTitle());
	m_pobjDumpDoc->SetTitle(objTitle);

	//�󔒏�����
	objHeaderLine.Set((const unsigned char*)"");
	m_pobjDumpDoc->AppendDumpLine(objHeaderLine);
	//�^�C�g��������
	objHeaderLine.Set((const unsigned char*)(LPCTSTR)objTitle);
	m_pobjDumpDoc->AppendDumpLine(objHeaderLine);
	//�󔒏�����
	objHeaderLine.Set((const unsigned char*)"");
	m_pobjDumpDoc->AppendDumpLine(objHeaderLine);
	//�Q�[�W��������
	objHeaderLine.Set((const unsigned char*)"| Offset | 0001 0203 0405 0607 0809 0A0B 0C0D 0E0F |      ASCII DATA");
	m_pobjDumpDoc->AppendDumpLine(objHeaderLine);


	long lLineCnt=0;		//�s���J�E���^
	long lSumCnt=0;			//�ݐσo�C�g���J�E���^
	long lOffset=0;			//�P�s�������J�E���^
	CLine* pobjLine=NULL;	//�P�s�o�b�t�@
	CLine objDumpLine;		//�_���v���ʂP�s
	memset(szAsciiDataBuf,0,sizeof(szAsciiDataBuf));
	for( ; ; )
	{
		pobjLine = pobjEditorDoc->GetOneLine(lLineCnt);
		if(pobjLine==NULL)
			break;

		//�ꎞ�I�ɉ��s�R�[�h�ǉ�
		CLine	objTempLine;
		if(lLineCnt>0)
		{
			//CR,LF�̐��擾
			long lCnt;
			pobjEditorDoc->GetCRLFCount(&lCRCount,&lLFCount);
			if( lCRCount!=0 || lLFCount!=0 )
			{
				for(lCnt=0; lCnt<lCRCount; lCnt++)
					objTempLine.Catenate((const unsigned char*)"\r");
				for(lCnt=0; lCnt<lLFCount; lCnt++)
					objTempLine.Catenate((const unsigned char*)"\n");
			}
			else
			{
				long lKaigyoKind = pobjEditorDoc->GetKaigyoKind();
				if(lKaigyoKind==KAIGYO_CRLF)
					objTempLine.Catenate((const unsigned char*)"\r\n");
				else if(lKaigyoKind==KAIGYO_LF)
					objTempLine.Catenate((const unsigned char*)"\n");
				else if(lKaigyoKind==KAIGYO_CR)
					objTempLine.Catenate((const unsigned char*)"\r");
				else 
					objTempLine.Catenate((const unsigned char*)"\r\n");
			}
			objTempLine.Catenate(pobjLine->String());
		}
		else
		{
			objTempLine.Set(pobjLine->String());
		}

		for(lOffset=0; lOffset<objTempLine.GetLength(); lOffset++, lSumCnt++) //�P�s���������[�v
		{
			if(!(lSumCnt%256))
			{
				if(objDumpLine.GetLength()!=0)
				{
					//�����ް�������
					objDumpLine.Catenate((const unsigned char*)"     ");
					objDumpLine.Catenate((const unsigned char*)szAsciiDataBuf);
					memset(szAsciiDataBuf,0,sizeof(szAsciiDataBuf));
					//���܂܂ł̃_���v���ʂ�o�^
					m_pobjDumpDoc->AppendDumpLine(objDumpLine);
				}
				//�Z�p���[�^�o�^
				CLine objTempLine((const unsigned char*)"|--------|-----------------------------------------|");
				m_pobjDumpDoc->AppendDumpLine(objTempLine);
			}
			if((lSumCnt!=0)&&(!(lSumCnt%2)))
			{
				objDumpLine.Catenate((const unsigned char*)" ");
			}
			if((!(lSumCnt%16)))
			{
				if( (lSumCnt%256)!=0 )
				{
					if(objDumpLine.GetLength()!=0)
					{
						//�����ް�������
						objDumpLine.Catenate((const unsigned char*)"    ");
						objDumpLine.Catenate((const unsigned char*)szAsciiDataBuf);
						memset(szAsciiDataBuf,0,sizeof(szAsciiDataBuf));
						//���܂܂ł̃_���v���ʂ�o�^
						m_pobjDumpDoc->AppendDumpLine(objDumpLine);
					}
				}
				//�N���A
				objDumpLine.Set((const unsigned char*)"");
				char szBuf[64];
				sprintf(szBuf,"| %06X | ",lSumCnt);
				objDumpLine.Set((const unsigned char*)szBuf);
			}
			char szBuf[64];
			sprintf(szBuf,"%02X",(unsigned int)objTempLine.GetAt(lOffset));
			objDumpLine.Catenate((const unsigned char*)szBuf);

			//////////////////////////
			//	�`�r�b�h�h�ł̕\��	//
			//////////////////////////
			if(objTempLine.GetAt(lOffset) <= 0x7f)
				szAsciiDataBuf[(lSumCnt%16)] = szAsciiData[objTempLine.GetAt(lOffset)];
			else
				szAsciiDataBuf[(lSumCnt%16)] = '.';
		}

		//���̍s��
		lLineCnt++;
	}

	if(objDumpLine.GetLength()!=0)
	{
		//�����ް�������
		objDumpLine.PadSpace(55);
		objDumpLine.Catenate((const unsigned char*)szAsciiDataBuf);
		memset(szAsciiDataBuf,0,sizeof(szAsciiDataBuf));
		//���܂܂ł̃_���v���ʂ�o�^
		m_pobjDumpDoc->AppendDumpLine(objDumpLine);
	}

	m_pobjDumpDoc->UpdateAllViews(NULL,NULL,NULL);
}
void CMainFrame::OnUpdateEditAsciidump(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�h�L�������g�G���v�e�B�`�F�b�N
	if( pobjEditorDoc->IsEmpty() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�c�[���|�r�i�h�r�R�[�h�\���_���v�v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditSjisdump()
{
	long	lCRCount=0;
	long	lLFCount=0;
	char	szSjisDataBuf[24];
	char	szAsciiData[128];
	strcpy(szAsciiData,ASCII_DATA);
	CLine	objHeaderLine;
	CString objMsg;

	//�A�N�e�B�u�t���[���擾
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
		return;
	//�A�N�e�B�u�h�L�������g�擾
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
		return;
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
		return;
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
		return;

	//�_���v���ʏo�͗p�l�c�h�q�E�C���h�E�쐬
	m_pobjDumpDoc = NULL;
	m_pobjDumpFrameWnd = NULL;
	BOOL bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjDumpDoc,&m_pobjDumpFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjDumpDoc->SetFontOfCurrentFileExt("");
	m_pobjDumpDoc->SetColorOfCurrentFileExt("");

	//�_���v���[�h�ݒ�
	SetReadOnlyMode(TRUE);//�����݋֎~Ӱ�޽ð���߲ݐݒ�
	m_pobjDumpDoc->SetDumpMode(TRUE);

	//�^�C�g���ݒ�
	CString	objTitle;
	objMsg.LoadString(IDS_SJISDUMP_TITLE);
	objTitle.Format(objMsg,pobjEditorDoc->GetTitle());
	m_pobjDumpDoc->SetTitle(objTitle);

	//�󔒏�����
	objHeaderLine.Set((const unsigned char*)"");
	m_pobjDumpDoc->AppendDumpLine(objHeaderLine);
	//�^�C�g��������
	objHeaderLine.Set((const unsigned char*)(LPCTSTR)objTitle);
	m_pobjDumpDoc->AppendDumpLine(objHeaderLine);
	//�󔒏�����
	objHeaderLine.Set((const unsigned char*)"");
	m_pobjDumpDoc->AppendDumpLine(objHeaderLine);
	//�Q�[�W��������
	objHeaderLine.Set((const unsigned char*)"| Offset | 0001 0203 0405 0607 0809 0A0B 0C0D 0E0F |      SJIS DATA");
	m_pobjDumpDoc->AppendDumpLine(objHeaderLine);


	BOOL bPreIsDBCSLeadByte=FALSE;
	long lLineCnt=0;		//�s���J�E���^
	long lSumCnt=0;			//�ݐσo�C�g���J�E���^
	long lOffset=0;			//�P�s�������J�E���^
	CLine* pobjLine=NULL;	//�P�s�o�b�t�@
	CLine objDumpLine;		//�_���v���ʂP�s
	memset(szSjisDataBuf,0,sizeof(szSjisDataBuf));
	for( ; ; )
	{
		pobjLine = pobjEditorDoc->GetOneLine(lLineCnt);
		if(pobjLine==NULL)
			break;

		//�ꎞ�I�ɉ��s�R�[�h�ǉ�
		CLine	objTempLine;
		if(lLineCnt>0)
		{
			//CR,LF�̐��擾
			long lCnt;
			pobjEditorDoc->GetCRLFCount(&lCRCount,&lLFCount);
			if( lCRCount!=0 || lLFCount!=0 )
			{
				for(lCnt=0; lCnt<lCRCount; lCnt++)
					objTempLine.Catenate((const unsigned char*)"\r");
				for(lCnt=0; lCnt<lLFCount; lCnt++)
					objTempLine.Catenate((const unsigned char*)"\n");
			}
			else
			{
				long lKaigyoKind = pobjEditorDoc->GetKaigyoKind();
				if(lKaigyoKind==KAIGYO_CRLF)
					objTempLine.Catenate((const unsigned char*)"\r\n");
				else if(lKaigyoKind==KAIGYO_LF)
					objTempLine.Catenate((const unsigned char*)"\n");
				else if(lKaigyoKind==KAIGYO_CR)
					objTempLine.Catenate((const unsigned char*)"\r");
				else
					objTempLine.Catenate((const unsigned char*)"\r\n");
			}
			objTempLine.Catenate(pobjLine->String());
		}
		else
		{
			objTempLine.Set(pobjLine->String());
		}

		for(lOffset=0; lOffset<objTempLine.GetLength(); lOffset++, lSumCnt++) //�P�s���������[�v
		{
			if(!(lSumCnt%256))
			{
				if(objDumpLine.GetLength()!=0)
				{
					//SJIS�ް�������
					objDumpLine.Catenate((const unsigned char*)"     ");
					objDumpLine.Catenate((const unsigned char*)szSjisDataBuf);
					memset(szSjisDataBuf,0,sizeof(szSjisDataBuf));
					//���܂܂ł̃_���v���ʂ�o�^
					m_pobjDumpDoc->AppendDumpLine(objDumpLine);
				}
				//�Z�p���[�^�o�^
				CLine objTempLine((const unsigned char*)"|--------|-----------------------------------------|");
				m_pobjDumpDoc->AppendDumpLine(objTempLine);
			}
			if((lSumCnt!=0)&&(!(lSumCnt%2)))
			{
				objDumpLine.Catenate((const unsigned char*)" ");
			}
			if((!(lSumCnt%16)))
			{
				if( (lSumCnt%256)!=0 )
				{
					if(objDumpLine.GetLength()!=0)
					{
						//SJIS�ް�������
						objDumpLine.Catenate((const unsigned char*)"    ");
						objDumpLine.Catenate((const unsigned char*)szSjisDataBuf);
						memset(szSjisDataBuf,0,sizeof(szSjisDataBuf));
						//���܂܂ł̃_���v���ʂ�o�^
						m_pobjDumpDoc->AppendDumpLine(objDumpLine);
					}
				}
				//�N���A
				objDumpLine.Set((const unsigned char*)"");
				char szBuf[64];
				sprintf(szBuf,"| %06X | ",lSumCnt);
				objDumpLine.Set((const unsigned char*)szBuf);
			}
			char szBuf[64];
			sprintf(szBuf,"%02X",(unsigned int)objTempLine.GetAt(lOffset));
			objDumpLine.Catenate((const unsigned char*)szBuf);

			//////////////////////////////////
			//	�r���������|�i�h�r�ł̕\��	//
			//////////////////////////////////

			if(bPreIsDBCSLeadByte==FALSE)
			{
				if(!IsDBCSLeadByteEx(0,objTempLine.GetAt(lOffset))) //�P�o�C�g����
				{
					if(objTempLine.GetAt(lOffset) <= 0x7f)
						szSjisDataBuf[(lSumCnt%16)] = szAsciiData[objTempLine.GetAt(lOffset)];
					else if(isHanKana(objTempLine.GetAt(lOffset)))
						szSjisDataBuf[(lSumCnt%16)] = objTempLine.GetAt(lOffset);
					else
						szSjisDataBuf[(lSumCnt%16)] = '.';
					bPreIsDBCSLeadByte=FALSE;
				}
				else if(IsDBCSLeadByteEx(0,objTempLine.GetAt(lOffset)))//�Q�o�C�g�����̑��o�C�g
				{
					if((lSumCnt%16)==15) //�P�U�Ԗ�
						szSjisDataBuf[(lSumCnt%16)] = '.';
					else
						szSjisDataBuf[(lSumCnt%16)] = objTempLine.GetAt(lOffset);
					bPreIsDBCSLeadByte=TRUE;
				}
			}
			else //�Q�o�C�g�����̑��o�C�g
			{
				if((lSumCnt%16)==0) //�P�Ԗ�
					szSjisDataBuf[(lSumCnt%16)] = '.';
				else
					szSjisDataBuf[(lSumCnt%16)] = objTempLine.GetAt(lOffset);
				bPreIsDBCSLeadByte=FALSE;
			}
		}

		//���̍s��
		lLineCnt++;
	}

	if(objDumpLine.GetLength()!=0)
	{
		//SJIS�ް�������
		objDumpLine.PadSpace(55);
		objDumpLine.Catenate((const unsigned char*)szSjisDataBuf);
		memset(szSjisDataBuf,0,sizeof(szSjisDataBuf));
		//���܂܂ł̃_���v���ʂ�o�^
		m_pobjDumpDoc->AppendDumpLine(objDumpLine);
	}

	m_pobjDumpDoc->UpdateAllViews(NULL,NULL,NULL);
}
void CMainFrame::OnUpdateEditSjisdump(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�h�L�������g�G���v�e�B�`�F�b�N
	if( pobjEditorDoc->IsEmpty() ) 
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�Í����^�������֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�c�[���|�t�@�C���Í����v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnSecurityEncript() 
{
	CEncriptDlg	objEncriptDlg;

	// �t�@�C���Í��������f�t�H���g�ݒ�
	objEncriptDlg.m_bIsOutOtherFile = m_bIsOutOtherFileEncript;			// �ϊ����ʂ�ʃt�@�C���֏o�͂��邩�H
	objEncriptDlg.m_bIsSearchSubFolder = m_bIsSearchSubFolderEncript;	// �T�u�t�H���_���ϊ����邩�H
	objEncriptDlg.m_objKey = "";										// �t�@�C���Í����L�[
	objEncriptDlg.m_objFolder = m_objFolderEncript;						// �t�@�C���Í������s�t�H���_
	objEncriptDlg.m_objFileKind = m_objFileKindEncript;					// �Í����Ώۂ̃t�@�C�����

	if( objEncriptDlg.DoModal() != IDOK )
		return;

	// �t�@�C���Í��������擾���ێ�
	m_bIsOutOtherFileEncript = objEncriptDlg.m_bIsOutOtherFile;			// �ϊ����ʂ�ʃt�@�C���֏o�͂��邩�H
	m_bIsSearchSubFolderEncript = objEncriptDlg.m_bIsSearchSubFolder;	// �T�u�t�H���_���ϊ����邩�H
	m_objKeyEncript = objEncriptDlg.m_objKey;							// �t�@�C���Í����L�[
	m_objFolderEncript = objEncriptDlg.m_objFolder;						// �t�@�C���Í������s�t�H���_
	m_objFileKindEncript = objEncriptDlg.m_objFileKind;					// �Í����Ώۂ̃t�@�C�����

	// �t�@�C���Í������s
	DoFileEncript();
}
void CMainFrame::OnUpdateSecurityEncript(CCmdUI* pCmdUI) 
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�c�[���|�t�@�C���������v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnSecurityDecript() 
{
	CDecriptDlg	objDecriptDlg;

	// �t�@�C�������������f�t�H���g�ݒ�
	objDecriptDlg.m_bIsOutOtherFile = m_bIsOutOtherFileDecript;			// �ϊ����ʂ�ʃt�@�C���֏o�͂��邩�H
	objDecriptDlg.m_bIsSearchSubFolder = m_bIsSearchSubFolderDecript;	// �T�u�t�H���_���ϊ����邩�H
	objDecriptDlg.m_objKey = "";										// �t�@�C���������L�[
	objDecriptDlg.m_objFolder = m_objFolderDecript;						// �t�@�C�����������s�t�H���_
	objDecriptDlg.m_objFileKind = m_objFileKindDecript;					// �������Ώۂ̃t�@�C�����

	if( objDecriptDlg.DoModal() != IDOK )
		return;

	// �t�@�C�������������擾���ێ�
	m_bIsOutOtherFileDecript = objDecriptDlg.m_bIsOutOtherFile;			// �ϊ����ʂ�ʃt�@�C���֏o�͂��邩�H
	m_bIsSearchSubFolderDecript = objDecriptDlg.m_bIsSearchSubFolder;	// �T�u�t�H���_���ϊ����邩�H
	m_objKeyDecript = objDecriptDlg.m_objKey;							// �t�@�C���������L�[
	m_objFolderDecript = objDecriptDlg.m_objFolder;						// �t�@�C�����������s�t�H���_
	m_objFileKindDecript = objDecriptDlg.m_objFileKind;					// �������Ώۂ̃t�@�C�����

	// �t�@�C�����������s
	DoFileDecript();
}
void CMainFrame::OnUpdateSecurityDecript(CCmdUI* pCmdUI) 
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�G���R�[�h�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�c�[���|�t�t�d�m�b�n�c�d�v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditUuencode() 
{
	CString objMsg;

	//���̓t�@�C�����擾
	char	szBuf[1024];
	char	szFileName[_MAX_FNAME];
	char	szFilePath[_MAX_PATH];
	CString objStrFName, objStrFPath;
	CFileDialog	objFileDialog(TRUE);
	objMsg.LoadString(IDS_UUENCODE_INFILE_TITLE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if( objFileDialog.DoModal() != IDOK )
		return;

	//�t�@�C�����擾
	objStrFPath = objFileDialog.GetPathName();
	strcpy(szFilePath,(LPCTSTR)objStrFPath);
	objStrFName = objFileDialog.GetFileName();
	strcpy(szFileName,(LPCTSTR)objStrFName);

	//���̓t�@�C���I�[�v��
	CFile	objFile;
	if( objFile.Open( szFilePath, CFile::modeRead|CFile::typeBinary ) == NULL )
	{
		/* �t�@�C���I�[�v���G���[ */
		objMsg.LoadString(IDS_FOPEN_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	CArchive	ar(&objFile,CArchive::load);

	//�G���R�[�h���ʏo�͗p�l�c�h�q�E�C���h�E�쐬
	CEditorDoc*	pobjEncodeDoc = NULL;
	CFrameWnd*	pobjEncodeFrameWnd = NULL;
	BOOL bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&pobjEncodeDoc,&pobjEncodeFrameWnd);
	if( bRet == FALSE )
		return;
	pobjEncodeDoc->SetFontOfCurrentFileExt("");
	pobjEncodeDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//�����݋֎~Ӱ�޽ð���߲ݐݒ�
	CString	objTitle;
	objTitle = szFilePath;
	objMsg.LoadString(IDS_UUENCODE_RESULT_TITLE);
	objTitle += objMsg;
	pobjEncodeDoc->SetTitle(objTitle);
	pobjEncodeDoc->SetEncodeMode(TRUE);

	//�^�C�g���ǉ�
	CLine	objEncodeResult;
	objEncodeResult.Set((const unsigned char*)szFilePath);
	objMsg.LoadString(IDS_UUENCODE_RESULT_TITLE);
	objEncodeResult.Catenate((const unsigned char*)(LPCTSTR)objMsg);
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
	objEncodeResult.Set((const unsigned char*)"");
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);

	//begin �s������
	sprintf(szBuf, "begin 666 %s", szFileName);
	objEncodeResult.Set((const unsigned char*)szBuf);
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);

	//�t�t�d�m�b�n�c�d
	CUuencoder	objUuencoder;
	CString objBuf;
	BYTE	byBuf[1024];
	int		nReadLength = 45, nBytesRead=0;
	for(;;)
	{
		try
		{
			memset(byBuf,0,sizeof(byBuf));
			nBytesRead = ar.Read( byBuf, nReadLength );
		}
		catch( CFileException* e )
		{
			delete e;
			break;
		}
		if(nBytesRead == 0)
			break;

		//�G���R�[�h
		objBuf.Empty();
		objBuf = objUuencoder.BytesEncode(byBuf,nBytesRead);
		if(!objBuf.IsEmpty())
		{
			objEncodeResult.Set((const unsigned char*)(LPCTSTR)objBuf);
			pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
		}
	}

	//end �s������
	objEncodeResult.Set((const unsigned char*)"`");
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
	objEncodeResult.Set((const unsigned char*)"end");
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
	objEncodeResult.Set((const unsigned char*)"");
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);

	pobjEncodeDoc->UpdateAllViews(NULL,NULL,NULL);

	//���̓t�@�C���N���[�Y
	ar.Close();
	objFile.Close();
}
void CMainFrame::OnUpdateEditUuencode(CCmdUI* pCmdUI) 
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�c�[���|�a�������U�S�G���R�[�h�v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditBase64encode() 
{
	CString objMsg;

	//���̓t�@�C�����擾
	char	szFileName[_MAX_FNAME];
	char	szFilePath[_MAX_PATH];
	CString objStrFName, objStrFPath;
	CFileDialog	objFileDialog(TRUE);
	objMsg.LoadString(IDS_BASE64_INFILE_TITLE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if( objFileDialog.DoModal() != IDOK )
		return;

	//�t�@�C�����擾
	objStrFPath = objFileDialog.GetPathName();
	strcpy(szFilePath,(LPCTSTR)objStrFPath);
	objStrFName = objFileDialog.GetFileName();
	strcpy(szFileName,(LPCTSTR)objStrFName);

	//���̓t�@�C���I�[�v��
	CFile	objFile;
	if( objFile.Open( szFilePath, CFile::modeRead|CFile::typeBinary ) == NULL )
	{
		/* �t�@�C���I�[�v���G���[ */
		objMsg.LoadString(IDS_FOPEN_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	CArchive	ar(&objFile,CArchive::load);

	//�G���R�[�h���ʏo�͗p�l�c�h�q�E�C���h�E�쐬
	CEditorDoc*	pobjEncodeDoc = NULL;
	CFrameWnd*	pobjEncodeFrameWnd = NULL;
	BOOL bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&pobjEncodeDoc,&pobjEncodeFrameWnd);
	if( bRet == FALSE )
		return;
	pobjEncodeDoc->SetFontOfCurrentFileExt("");
	pobjEncodeDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//�����݋֎~Ӱ�޽ð���߲ݐݒ�
	CString	objTitle;
	objTitle = szFilePath;
	objMsg.LoadString(IDS_BASE64_RESULT_TITLE);
	objTitle += objMsg;
	pobjEncodeDoc->SetTitle(objTitle);
	pobjEncodeDoc->SetEncodeMode(TRUE);

	//�^�C�g���ǉ�
	CLine	objEncodeResult;
	objEncodeResult.Set((const unsigned char*)szFilePath);
	objMsg.LoadString(IDS_BASE64_RESULT_TITLE);
	objEncodeResult.Catenate((const unsigned char*)(LPCTSTR)objMsg);
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
	objEncodeResult.Set((const unsigned char*)"");
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);

	//�a�������U�S�G���R�[�h
	CBase64	objBase64;
	CString objBuf;
	BYTE	byBuf[1024];
	int		nReadLength = 54, nBytesRead=0;
	for(;;)
	{
		try
		{
			memset(byBuf,0,sizeof(byBuf));
			nBytesRead = ar.Read( byBuf, nReadLength );
		}
		catch( CFileException* e )
		{
			delete e;
			break;
		}
		if(nBytesRead == 0)
			break;

		//�G���R�[�h
		objBuf.Empty();
		objBuf = objBase64.BytesEncode(byBuf,nBytesRead);
		if(!objBuf.IsEmpty())
		{
			objEncodeResult.Set((const unsigned char*)(LPCTSTR)objBuf);
			pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
		}
	}

	objEncodeResult.Set((const unsigned char*)"");
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
	pobjEncodeDoc->UpdateAllViews(NULL,NULL,NULL);

	//���̓t�@�C���N���[�Y
	ar.Close();
	objFile.Close();
}
void CMainFrame::OnUpdateEditBase64encode(CCmdUI* pCmdUI) 
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�c�[���|Quoted-Printable�G���R�[�h�v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditQuotedencode() 
{
	CString objMsg;

	//���̓t�@�C�����擾
	char*	pPtr=NULL;
	char	szFileName[_MAX_FNAME];
	char	szFilePath[_MAX_PATH];
	CString objStrFName, objStrFPath;
	CFileDialog	objFileDialog(TRUE);
	objMsg.LoadString(IDS_QUOTED_INFILE_TITLE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if( objFileDialog.DoModal() != IDOK )
		return;

	//�t�@�C�����擾
	objStrFPath = objFileDialog.GetPathName();
	strcpy(szFilePath,(LPCTSTR)objStrFPath);
	objStrFName = objFileDialog.GetFileName();
	strcpy(szFileName,(LPCTSTR)objStrFName);

	//���̓t�@�C���I�[�v��
	CFile	objFile;
	if( objFile.Open( szFilePath, CFile::modeRead|CFile::typeBinary ) == NULL )
	{
		/* �t�@�C���I�[�v���G���[ */
		objMsg.LoadString(IDS_FOPEN_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	CArchive	ar(&objFile,CArchive::load);

	//�G���R�[�h���ʏo�͗p�l�c�h�q�E�C���h�E�쐬
	CEditorDoc*	pobjEncodeDoc = NULL;
	CFrameWnd*	pobjEncodeFrameWnd = NULL;
	BOOL bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&pobjEncodeDoc,&pobjEncodeFrameWnd);
	if( bRet == FALSE )
		return;
	pobjEncodeDoc->SetFontOfCurrentFileExt("");
	pobjEncodeDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//�����݋֎~Ӱ�޽ð���߲ݐݒ�
	CString	objTitle;
	objTitle = szFilePath;
	objMsg.LoadString(IDS_QUOTED_RESULT_TITLE);
	objTitle += objMsg;
	pobjEncodeDoc->SetTitle(objTitle);
	pobjEncodeDoc->SetEncodeMode(TRUE);

	//�^�C�g���ǉ�
	CLine	objEncodeResult;
	objEncodeResult.Set((const unsigned char*)szFilePath);
	objMsg.LoadString(IDS_QUOTED_RESULT_TITLE);
	objEncodeResult.Catenate((const unsigned char*)(LPCTSTR)objMsg);
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
	objEncodeResult.Set((const unsigned char*)"");
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);

	//�p�����������|�o�����������������G���R�[�h
	CQuotedPrintable	objQuotedPrintable;
	CString objBuf=_T("");
	BYTE	byBuf[1024];
	int		nReadLength = 22, nBytesRead=0;
	for(;;)
	{
		try
		{
			memset(byBuf,0,sizeof(byBuf));
			nBytesRead = ar.Read( byBuf, nReadLength );
		}
		catch( CFileException* e )
		{
			delete e;
			break;
		}

		if(!objBuf.IsEmpty())
		{
			if(nBytesRead != 0)
				objBuf += '=';//�\�t�g���s
			objEncodeResult.Set((const unsigned char*)(LPCTSTR)objBuf);
			pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
		}

		if(nBytesRead == 0)
			break;

		//�G���R�[�h
		objBuf.Empty();
		objBuf = objQuotedPrintable.BytesEncode(byBuf,nBytesRead);
	}

	objEncodeResult.Set((const unsigned char*)"");
	pobjEncodeDoc->AppendEncodeLine(objEncodeResult);
	pobjEncodeDoc->UpdateAllViews(NULL,NULL,NULL);

	//���̓t�@�C���N���[�Y
	ar.Close();
	objFile.Close();
}
void CMainFrame::OnUpdateEditQuotedencode(CCmdUI* pCmdUI) 
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�t�@�C����r�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�����|�t�@�C����r�v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFilecompare() 
{
	CString			objMsg;
	CFileDialog		objFileDialog(TRUE);
	CString 		objFilePath1, objFilePath2;

	//���t�@�C���p�X�擾
	objMsg.LoadString(IDS_FILECOMPARE_SEL1FILE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if( objFileDialog.DoModal() != IDOK )
		return;
	objFilePath1 = objFileDialog.GetPathName();

	//���t�@�C���p�X�擾
	objMsg.LoadString(IDS_FILECOMPARE_SEL2FILE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if( objFileDialog.DoModal() != IDOK )
		return;
	objFilePath2 = objFileDialog.GetPathName();

	//�t�@�C����r���s
	DoFileCompare(objFilePath1,objFilePath2,NULL);
}
void CMainFrame::OnUpdateEditFilecompare(CCmdUI* pCmdUI) 
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C����r�ڍ׃I�v�V�����|�^�u�ƃX�y�[�X����ʂ��Ȃ��v�I��������	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFcompTabequal2space() 
{
	BOOL bIsAssumeTabEqual2Space = m_pApp->GetProfileInt("CFileCompare","bIsAssumeTabEqual2Space",FALSE);
	if(bIsAssumeTabEqual2Space)
		m_pApp->WriteProfileInt("CFileCompare","bIsAssumeTabEqual2Space",FALSE);
	else
		m_pApp->WriteProfileInt("CFileCompare","bIsAssumeTabEqual2Space",TRUE);
}
void CMainFrame::OnUpdateEditFcompTabequal2space(CCmdUI* pCmdUI) 
{
	BOOL bIsAssumeTabEqual2Space = m_pApp->GetProfileInt("CFileCompare","bIsAssumeTabEqual2Space",FALSE);
	if(bIsAssumeTabEqual2Space)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C����r�ڍ׃I�v�V�����|�S�p�Ɣ��p����ʂ��Ȃ��v�I��������		|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFcompZenequal2han() 
{
	BOOL bIsAssumeZenEqual2Han = m_pApp->GetProfileInt("CFileCompare","bIsAssumeZenEqual2Han",FALSE);
	if(bIsAssumeZenEqual2Han)
		m_pApp->WriteProfileInt("CFileCompare","bIsAssumeZenEqual2Han",FALSE);
	else
		m_pApp->WriteProfileInt("CFileCompare","bIsAssumeZenEqual2Han",TRUE);
}
void CMainFrame::OnUpdateEditFcompZenequal2han(CCmdUI* pCmdUI) 
{
	BOOL bIsAssumeZenEqual2Han = m_pApp->GetProfileInt("CFileCompare","bIsAssumeZenEqual2Han",FALSE);
	if(bIsAssumeZenEqual2Han)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C����r�ڍ׃I�v�V�����|�p���̑啶������������ʂ��Ȃ��v		|*/
/*|           �I��������															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFcompAlphabetbigandsmall() 
{
	BOOL bIsAssumeAlphabetBigAndSmall = m_pApp->GetProfileInt("CFileCompare","bIsAssumeAlphabetBigAndSmall",FALSE);
	if(bIsAssumeAlphabetBigAndSmall)
		m_pApp->WriteProfileInt("CFileCompare","bIsAssumeAlphabetBigAndSmall",FALSE);
	else
		m_pApp->WriteProfileInt("CFileCompare","bIsAssumeAlphabetBigAndSmall",TRUE);
}
void CMainFrame::OnUpdateEditFcompAlphabetbigandsmall(CCmdUI* pCmdUI) 
{
	BOOL bIsAssumeAlphabetBigAndSmall = m_pApp->GetProfileInt("CFileCompare","bIsAssumeAlphabetBigAndSmall",FALSE);
	if(bIsAssumeAlphabetBigAndSmall)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�����|�t�@�C���Ĕ�r�v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditFilerecompare() 
{
	//�A�N�e�B�u�h�L�������g�擾
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd==NULL)
		return;
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc==NULL)
		return;
	ASSERT_VALID(pobjEditorDoc);

	//�t�@�C����r���ʕ\�����H
	if(pobjEditorDoc->IsCompare() == FALSE)
		return;

	//�t�@�C����r���t�@�C���A���t�@�C���擾
	CString objFilePath1,objFilePath2;
	pobjEditorDoc->GetCompareFiles(objFilePath1,objFilePath2);

	//�\�����̌��ʃf�[�^����������N���A
	for(;;)
	{
		if(pobjEditorDoc->GetLineCount() <= 0)
			break;
		pobjEditorDoc->DeleteOneLine(0);
	}

	//�t�@�C����r���s
	DoFileCompare(objFilePath1,objFilePath2,pobjEditorDoc);
}
void CMainFrame::OnUpdateEditFilerecompare(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�h�L�������g�擾
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd==NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc==NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	ASSERT_VALID(pobjEditorDoc);

	//������`�F�b�N
	if(pobjEditorDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�h�L�������g�ɂP�s���f�[�^���Ȃ��ꍇ�͑I��s�ɂ���
	if( pobjEditorDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�t�@�C����r���ʂ��ǂ���
	if(pobjEditorDoc->IsCompare() == FALSE)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�t�@�C����r���t�@�C���A���t�@�C���擾 �����݃`�F�b�N
	CString objFilePath1,objFilePath2;
	pobjEditorDoc->GetCompareFiles(objFilePath1,objFilePath2);
	if( _access( (LPCTSTR)objFilePath1, 0 ) == -1 || 
		_access( (LPCTSTR)objFilePath2, 0 ) == -1
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�f�B���N�g����r�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�����|�f�B���N�g����r�v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnEditDircompare() 
{
	CDirCompareDlg	objDirCompareDlg;

	// �f�B���N�g����r�����f�t�H���g�ݒ�
	objDirCompareDlg.m_objFolder1 = m_objFolder1DirCmp;					// ��r���t�H���_
	objDirCompareDlg.m_objFolder2 = m_objFolder2DirCmp;					// ��r���t�H���_

	if( objDirCompareDlg.DoModal() != IDOK )
		return;

	// �f�B���N�g����r�����擾���ێ�
	m_objFolder1DirCmp = objDirCompareDlg.m_objFolder1;					// ��r���t�H���_
	m_objFolder2DirCmp = objDirCompareDlg.m_objFolder2;					// ��r���t�H���_

	// �f�B���N�g����r���s
	DoDirCompare();
}
void CMainFrame::OnUpdateEditDircompare(CCmdUI* pCmdUI) 
{
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�ݒ�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ݒ�|�ݒ�v�I��������												|*/
/*|	���	�F																		|*/
/*|	���l	�F�u��O���� (����) �� Editor.exe (COMCTL32.DLL) �ɂ���܂�: 0xC0000005:|*/
/*|			   �����ᔽ�B�v�̃��b�Z�[�W���f�o�b�O�E�C���h�E�ɕ\�������B�������A	|*/
/*|			   CPropertySheet::DoModal()�̃w���v�ɂ��ƁA���̃A�N�Z�X�ᔽ�͖���	|*/
/*|			   ���Ă悢���̂ł��邻�����B											|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnSettings() 
{
	CFrameWnd*   pobjFrameWnd   = (CFrameWnd*)GetActiveFrame();
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();

	//������`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
		return;

	//�ݒ�p�v���p�e�B�V�[�g
	CMyPropertySheet propSheet;
	propSheet.m_pobjMainFrame = this;
	propSheet.m_pobjEditorActiveDoc = pobjEditorDoc;
	propSheet.m_pobjEditorActiveView = pobjEditorView;

	//����̐ݒ�𔽉f
	propSheet.m_Page1.m_bShowSplash         = m_pApp->IsShowSplash();
	propSheet.m_Page1.m_lSplashTime         = m_pApp->GetProfileInt("CEditorApp","SplashTime",2);
	propSheet.m_Page1.m_nMaxMRU             = m_pApp->GetMaxMRU();
	propSheet.m_Page1.m_bIsNoDoubleExecute  = m_pApp->GetProfileInt("CEditorApp","bIsNoDoubleExecute",0);
	propSheet.m_Page1.m_bIsOpenNewFile      = m_pApp->IsOpenNewFile();
	propSheet.m_Page1.m_bIsChdirToOpenFile  = m_pApp->IsChdirToOpenFile();
	propSheet.m_Page1.m_bMaximizeWindow     = m_pApp->GetProfileInt("CChildFrame","bMaximizeWindow",0);
	propSheet.m_Page1.m_bIsSaveWindowPlace  = m_bIsSaveWindowPlace;
	propSheet.m_Page1.m_bIsFullTitle        = m_pApp->GetProfileInt("CEditorDoc","bIsFullTitle",0);
	propSheet.m_Page1.m_bIsDispEOFMark      = m_pApp->GetProfileInt("CEditorView","bIsDispEOFMark",1);
	propSheet.m_Page1.m_bIsExecPasswd       = m_pApp->GetProfileInt("CEditorApp","bIsExecPasswd",0);
	propSheet.m_Page1.m_objExecPasswdStr    = m_pApp->GetProfileString("CEditorApp","ExecPasswd","");
	propSheet.m_Page1.m_bIsEnableShellOpen  = m_pApp->GetProfileInt("CEditorApp","bIsEnableShellOpen",0);
	propSheet.m_Page1.m_bIsMakeShotcutDsktop= m_pApp->GetProfileInt("CEditorApp","bIsMakeShortcutDsktop",0);
	propSheet.m_Page1.m_bIsImmAutoOpen      = m_pApp->GetProfileInt("CEditorView","IsImmAutoOpen",FALSE);
	propSheet.m_Page2.m_objFileExt          = pobjEditorDoc->MyGetFileExt();
	propSheet.m_Page2.m_lDispFontPoint      = pobjEditorView->GetDispFontPoint();
	propSheet.m_Page2.m_objDispFontName     = pobjEditorView->GetDispFontName();
	propSheet.m_Page2.m_lPrintFontPoint     = pobjEditorView->GetPrintFontPoint();
	propSheet.m_Page2.m_objPrintFontName    = pobjEditorView->GetPrintFontName();
	memcpy(&propSheet.m_Page2.m_stStatusBarLogFont,&m_stStatusBarLogFont,sizeof(LOGFONT));
	if( pobjEditorDoc->GetTabSize() == 2 )
		propSheet.m_Page2.m_nTabSize        = 0;
	else if( pobjEditorDoc->GetTabSize() == 4 )
		propSheet.m_Page2.m_nTabSize        = 1;
	else if( pobjEditorDoc->GetTabSize() == 8 )
		propSheet.m_Page2.m_nTabSize        = 2;
	propSheet.m_Page3.m_objFileExt          = pobjEditorDoc->MyGetFileExt();
	propSheet.m_Page3.m_FontColor           = pobjEditorView->GetFontColor();
	propSheet.m_Page3.m_BackColor           = pobjEditorView->GetBackColor();
	propSheet.m_Page3.m_InvertFontColor     = pobjEditorView->GetInvertFontColor();
	propSheet.m_Page3.m_InvertBackColor     = pobjEditorView->GetInvertBackColor();
	propSheet.m_Page3.m_bDispLineNo         = pobjEditorView->GetFlagDispLineNo();
	propSheet.m_Page3.m_LineNoColor         = pobjEditorView->GetLineNoColor();
	propSheet.m_Page3.m_LineNoBackColor     = pobjEditorView->GetLineNoBackColor();
	propSheet.m_Page3.m_KeyNo1Color         = pobjEditorDoc->GetKeyNo1Color();
	propSheet.m_Page3.m_KeyNo2Color         = pobjEditorDoc->GetKeyNo2Color();
	propSheet.m_Page3.m_KeyNo3Color         = pobjEditorDoc->GetKeyNo3Color();
	propSheet.m_Page3.m_KeyNo4Color         = pobjEditorDoc->GetKeyNo4Color();
	propSheet.m_Page3.m_MDIBkgndColor       = m_wndClient.GetMDIBkgndColor();
	propSheet.m_Page3.m_MarkLineColor       = pobjEditorView->GetMarkLineColor();
	propSheet.m_Page3.m_bComment1Clr        = pobjEditorDoc->GetFlagComment1Clr();
	propSheet.m_Page3.m_CommentColor        = pobjEditorDoc->GetCommentColor();
	propSheet.m_Page3.m_bComment2Clr        = pobjEditorDoc->GetFlagComment2Clr();
	propSheet.m_Page3.m_CommentColor2       = pobjEditorDoc->GetCommentColor2();
	propSheet.m_Page3.m_bComment3Clr        = pobjEditorDoc->GetFlagComment3Clr();
	propSheet.m_Page3.m_CommentColor3       = pobjEditorDoc->GetCommentColor3();
	propSheet.m_Page3.m_bComment4Clr        = pobjEditorDoc->GetFlagComment4Clr();
	propSheet.m_Page3.m_CommentColor4       = pobjEditorDoc->GetCommentColor4();
	propSheet.m_Page3.m_bDispRuler          = pobjEditorView->GetFlagDispRuler();
	propSheet.m_Page3.m_RulerColor          = pobjEditorView->GetRulerColor();
	propSheet.m_Page3.m_RulerBackColor      = pobjEditorView->GetRulerBackColor();
	propSheet.m_Page3.m_bDispCRLF           = pobjEditorView->GetFlagDispCRLF();
	propSheet.m_Page3.m_CRLFColor           = pobjEditorView->GetCRLFColor();
	propSheet.m_Page3.m_bDispTab		    = pobjEditorView->GetFlagDispTab();
	propSheet.m_Page3.m_TabColor		    = pobjEditorView->GetTabColor();
	propSheet.m_Page3.m_bDispZenkakuSpace   = pobjEditorView->GetFlagDispZenkakuSpace();
	propSheet.m_Page3.m_ZenkakuSpaceColor   = pobjEditorView->GetZenkakuSpaceColor();
	propSheet.m_Page3.m_bDispUnderLine	    = pobjEditorView->GetFlagDispUnderLine();
	propSheet.m_Page3.m_UnderLineColor	    = pobjEditorView->GetUnderLineColor();
	propSheet.m_Page3.m_bDispUrl            = pobjEditorDoc->GetFlagUrlColor();
	propSheet.m_Page3.m_UrlColor			= pobjEditorDoc->GetUrlColor();
	propSheet.m_Page3.m_bDispEMail			= pobjEditorDoc->GetFlagEMailColor();
	propSheet.m_Page3.m_EMailColor          = pobjEditorDoc->GetEMailColor();
	propSheet.m_Page3.m_DiffDelLineColor    = pobjEditorView->GetDiffDelLineColor();
	propSheet.m_Page3.m_DiffAddLineColor    = pobjEditorView->GetDiffAddLineColor();
	propSheet.m_Page3.m_DirDiffLineColor    = pobjEditorView->GetDirDiffLineColor();
	propSheet.m_Page4.m_nKaigyoKind		    = (int)pobjEditorDoc->GetKaigyoKind();
	propSheet.m_Page4.m_bIsAutoSave		    = m_bIsAutoSave;
	propSheet.m_Page4.m_lSaveInterval	    = m_lSaveInterval;
	propSheet.m_Page4.m_bDelEndSpacesOnSave = pobjEditorDoc->GetFlagDelEndSpacesOnSave();
	propSheet.m_Page4.m_bSpace2TabOnSave    = pobjEditorDoc->GetFlagSpace2TabOnSave();
	propSheet.m_Page5.m_bPrintLineNo        = pobjEditorView->GetFlagPrintLineNo();
	propSheet.m_Page5.m_bPrint2Dangumi      = pobjEditorView->GetFlagPrint2Dangumi();
	propSheet.m_Page5.m_nPrintOrientation   = m_pApp->GetPrintOrientation();
	CRect objMarginRect;
	m_pApp->GetPrintMargin(objMarginRect);
	objMarginRect.NormalizeRect();
	propSheet.m_Page5.m_nYohakuHidari       = objMarginRect.left;
	propSheet.m_Page5.m_nYohakuUe           = objMarginRect.top;
	propSheet.m_Page5.m_nYohakuMigi         = objMarginRect.right;
	propSheet.m_Page5.m_nYohakuSita         = objMarginRect.bottom;
	propSheet.m_Page5.m_nHeaderLayout       = pobjEditorView->GetPrintHeaderLayoutPos();
	propSheet.m_Page5.m_nFooterLayout       = pobjEditorView->GetPrintFooterLayoutPos();
	pobjEditorView->GetStrHeaderFormat(propSheet.m_Page5.m_objStrHeaderFormat);
	pobjEditorView->GetStrFooterFormat(propSheet.m_Page5.m_objStrFooterFormat);
	if( pobjEditorView->GetCaretType() == CARET_IBEAM )
		propSheet.m_Page6.m_nCaretType      = 0;
	else if( pobjEditorView->GetCaretType() == CARET_SQUARE )
		propSheet.m_Page6.m_nCaretType      = 1;
	propSheet.m_Page6.m_unCaretBlinkTime    = ::GetCaretBlinkTime();
	if( pobjEditorView->GetCaretBehave() == CARET_BEHAVE_MOVE )
		propSheet.m_Page6.m_nCaretBehave    = 0;
	else if( pobjEditorView->GetCaretBehave() == CARET_BEHAVE_STAY )
		propSheet.m_Page6.m_nCaretBehave    = 1;
	propSheet.m_Page6.m_nCaretMoveType      = pobjEditorView->GetCaretMoveType();
	int nMultValueOfCaretAccelarete = m_pApp->GetProfileInt("CEditorView","nMultValueOfCaretAccelarete",3);
	if( nMultValueOfCaretAccelarete!=1 && nMultValueOfCaretAccelarete!=2 &&
		nMultValueOfCaretAccelarete!=3 && nMultValueOfCaretAccelarete!=4 &&
		nMultValueOfCaretAccelarete!=5 )
	{
		nMultValueOfCaretAccelarete = 3;
	}
	propSheet.m_Page6.m_nMultValueOfCaretAccelarete = nMultValueOfCaretAccelarete-1;
	int nSetCaretToEndOnPaste = m_pApp->GetProfileInt("CEditorDoc","bSetCaretToEndOnPaste",1);
	if(nSetCaretToEndOnPaste!=0 && nSetCaretToEndOnPaste!=1)
		nSetCaretToEndOnPaste = 1;
	propSheet.m_Page6.m_nSetCaretToEndOnPaste = nSetCaretToEndOnPaste;
	propSheet.m_Page7.m_bDispLogoBmp        = m_wndClient.GetFlagDispLogoBmp();
	propSheet.m_Page7.m_nLogoBmpKind        = m_wndClient.GetLogoBmpKind();
	propSheet.m_Page7.m_objBmpFilePath      = m_wndClient.GetLogoBmpFilePath();
	propSheet.m_Page7.m_nLogoPosKind        = m_wndClient.GetLogoPosKind();
	propSheet.m_Page7.m_unLogoPosX          = m_wndClient.GetLogoPosX();
	propSheet.m_Page7.m_unLogoPosY          = m_wndClient.GetLogoPosY();
	memcpy(&propSheet.m_Page8.m_stStatusBarDisp,&m_stStatusBarDisp,sizeof(STATUSBARDISP));
	propSheet.m_Page8.m_lStatusDayStyle     = m_lStatusDayStyle;
	propSheet.m_Page8.m_lStatusHourStyle    = m_lStatusHourStyle;
	propSheet.m_Page8.m_lStatusLineStyle    = m_lStatusLineStyle;
	propSheet.m_Page12.m_bIsUseProxy        = m_bIsUseProxyHtml;
	propSheet.m_Page12.m_objProxyIPAddress  = m_objProxyIPAddressHtml;
	propSheet.m_Page12.m_bIsDialup          = m_bIsDialupHtml;
	propSheet.m_Page12.m_objDialupEntry     = m_objDialupEntryHtml;
	propSheet.m_Page12.m_objRasUserName     = m_objRasUserNameHtml;
	propSheet.m_Page12.m_objRasPasswd       = m_objRasPasswdHtml;
	propSheet.m_Page12.m_bIsAutoDisconnect  = m_bIsAutoDisconnectHtml;

	//�v���p�e�B�V�[�g�\��
	int nRet = propSheet.DoModal();
	if(nRet == IDOK)
	{
		if( propSheet.m_Page1.IsChanged() ) //�y�[�W�P�ɕύX����
		{
			m_pApp->SetShowSplash(propSheet.m_Page1.m_bShowSplash);
			if(propSheet.m_Page1.m_lSplashTime>=1 && propSheet.m_Page1.m_lSplashTime<=10)
				m_pApp->WriteProfileInt("CEditorApp","SplashTime",propSheet.m_Page1.m_lSplashTime);
			m_pApp->WriteProfileInt("CEditorApp","nMaxMRU",propSheet.m_Page1.m_nMaxMRU);
			m_pApp->SetMaxMRU(propSheet.m_Page1.m_nMaxMRU);
			m_pApp->WriteProfileInt("CEditorApp","bIsOpenNewFile",propSheet.m_Page1.m_bIsOpenNewFile);
			m_pApp->SetOpenNewFile(propSheet.m_Page1.m_bIsOpenNewFile);
			m_pApp->WriteProfileInt("CEditorApp","IsChdirToOpenFile",propSheet.m_Page1.m_bIsChdirToOpenFile);
			m_pApp->SetChdirToOpenFile(propSheet.m_Page1.m_bIsChdirToOpenFile);
			m_pApp->WriteProfileInt("CChildFrame","bMaximizeWindow",propSheet.m_Page1.m_bMaximizeWindow);
			m_bIsSaveWindowPlace = propSheet.m_Page1.m_bIsSaveWindowPlace;
			m_pApp->WriteProfileInt("CEditorDoc","bIsFullTitle",propSheet.m_Page1.m_bIsFullTitle);
			pobjEditorView->SetFlagDispEOFMark(propSheet.m_Page1.m_bIsDispEOFMark);
			m_pApp->WriteProfileInt("CEditorApp","bIsNoDoubleExecute",propSheet.m_Page1.m_bIsNoDoubleExecute);
			if(propSheet.m_Page1.m_bIsExecPasswd!=FALSE && !propSheet.m_Page1.m_objExecPasswdStr.IsEmpty())
			{
				m_pApp->WriteProfileInt("CEditorApp","bIsExecPasswd",TRUE);
				m_pApp->WriteProfileString("CEditorApp","ExecPasswd",propSheet.m_Page1.m_objExecPasswdStr);
			}
			else
			{
				m_pApp->WriteProfileInt("CEditorApp","bIsExecPasswd",FALSE);
				m_pApp->WriteProfileString("CEditorApp","ExecPasswd","");
			}
	  		m_pApp->WriteProfileInt("CEditorApp","bIsEnableShellOpen",propSheet.m_Page1.m_bIsEnableShellOpen);
			if(propSheet.m_Page1.m_bIsEnableShellOpen)
				m_pApp->RegisterShellExtDll();
			else
				m_pApp->UnRegisterShellExtDll();
			m_pApp->WriteProfileInt("CEditorApp","bIsMakeShortcutDsktop",propSheet.m_Page1.m_bIsMakeShotcutDsktop);
			m_pApp->MakeXerxesShortcut(propSheet.m_Page1.m_bIsMakeShotcutDsktop);
			m_pApp->WriteProfileInt("CEditorView","IsImmAutoOpen",propSheet.m_Page1.m_bIsImmAutoOpen);
		}
		if( propSheet.m_Page2.IsChanged() ) //�y�[�W�Q�ɕύX����
		{
			//�t�H���g�ݒ�
			m_pApp->SetFontSettings2AllWindow(pobjEditorDoc);

			//�X�e�[�^�X�o�[�t�H���g�ݒ�
			SetStatusBarFont(propSheet.m_Page2.m_stStatusBarLogFont);
			//�^�u�T�C�Y�ݒ�
			if(propSheet.m_Page2.m_nTabSize == 0)//�Q�^�u
				pobjEditorDoc->SetTabSize(2);
			else if(propSheet.m_Page2.m_nTabSize == 1)//�S�^�u
				pobjEditorDoc->SetTabSize(4);
			else								 //�W�^�u
				pobjEditorDoc->SetTabSize(8);
		}
		if( propSheet.m_Page3.IsChanged() ) //�y�[�W�R�ɕύX����
		{
			//�J���[�ݒ�
			m_pApp->SetColorSettings2AllWindow();

			m_wndClient.SetMDIBkgndColor(propSheet.m_Page3.m_MDIBkgndColor);
			pobjEditorView->SetDiffDelLineColor(propSheet.m_Page3.m_DiffDelLineColor);
			pobjEditorView->SetDiffAddLineColor(propSheet.m_Page3.m_DiffAddLineColor);
			pobjEditorView->SetDirDiffLineColor(propSheet.m_Page3.m_DirDiffLineColor);
		}
		if( propSheet.m_Page4.IsChanged() ) //�y�[�W�S�ɕύX����
		{
			//���s�R�[�h
			pobjEditorDoc->SetKaigyoKind(propSheet.m_Page4.m_nKaigyoKind);
			//�I�[�g�Z�[�u
			m_bIsAutoSave = propSheet.m_Page4.m_bIsAutoSave;
			//�Z�[�u�Ԋu
			if(propSheet.m_Page4.m_lSaveInterval>=1 && propSheet.m_Page4.m_lSaveInterval<=60)
			{
				m_lSaveInterval = propSheet.m_Page4.m_lSaveInterval;
				m_lTimeUpCount = 0;
			}
			//�s���󔒍폜�t���O
			pobjEditorDoc->SetFlagDelEndSpacesOnSave(propSheet.m_Page4.m_bDelEndSpacesOnSave);
			//�󔒁|���^�u�ϊ��t���O
			pobjEditorDoc->SetFlagSpace2TabOnSave(propSheet.m_Page4.m_bSpace2TabOnSave);
		}
		if( propSheet.m_Page5.IsChanged() ) //�y�[�W�T�ɕύX����
		{
			//�s�ԍ�����t���O
			pobjEditorView->SetFlagPrintLineNo(propSheet.m_Page5.m_bPrintLineNo);
			//�Q�i�g�݈���t���O
			pobjEditorView->SetFlagPrint2Dangumi(propSheet.m_Page5.m_bPrint2Dangumi);
			//����p������
			m_pApp->SetPrintOrientation(propSheet.m_Page5.m_nPrintOrientation);
			//����]��
			CRect objMarginRect;
			objMarginRect.left   = propSheet.m_Page5.m_nYohakuHidari;
			objMarginRect.top    = propSheet.m_Page5.m_nYohakuUe;
			objMarginRect.right  = propSheet.m_Page5.m_nYohakuMigi;
			objMarginRect.bottom = propSheet.m_Page5.m_nYohakuSita;
			objMarginRect.NormalizeRect();
			m_pApp->SetPrintMargin(objMarginRect);
			//�w�b�_���t�b�^�ʒu
			pobjEditorView->SetPrintHeaderLayoutPos(propSheet.m_Page5.m_nHeaderLayout);
			pobjEditorView->SetPrintFooterLayoutPos(propSheet.m_Page5.m_nFooterLayout);
			//�w�b�_���t�b�^�̃t�H�[�}�b�g
			pobjEditorView->SetStrHeaderFormat(propSheet.m_Page5.m_objStrHeaderFormat);
			pobjEditorView->SetStrFooterFormat(propSheet.m_Page5.m_objStrFooterFormat);
		}
		if( propSheet.m_Page6.IsChanged() ) //�y�[�W�U�ɕύX����
		{
			//�L�����b�g�`��ݒ�
			if(propSheet.m_Page6.m_nCaretType == 0)//�c�^�L�����b�g
				pobjEditorView->SetCaretType(CARET_IBEAM);
			else if(propSheet.m_Page6.m_nCaretType == 1)//���^�L�����b�g
				pobjEditorView->SetCaretType(CARET_SQUARE);
			//�L�����b�g�_�ŊԊu�ݒ�
			::SetCaretBlinkTime(propSheet.m_Page6.m_unCaretBlinkTime);
			pobjEditorView->HoldCaretBlinkTime(propSheet.m_Page6.m_unCaretBlinkTime);
			//�L�����b�g�̐U�镑���ݒ�
			if(propSheet.m_Page6.m_nCaretBehave == 0)//��ʃX�N���[�����ɍs�ʒu�ύX
				pobjEditorView->SetCaretBehave(CARET_BEHAVE_MOVE);
			else if(propSheet.m_Page6.m_nCaretBehave == 1)//��ʃX�N���[�����ɍs�ʒu�ێ�
				pobjEditorView->SetCaretBehave(CARET_BEHAVE_STAY);
			//�L�����b�g�ړ��^�C�v�ݒ�
			pobjEditorView->SetCaretMoveType(propSheet.m_Page6.m_nCaretMoveType);
			//�L�����b�g�����X�s�[�h�ݒ�
			m_pApp->WriteProfileInt("CEditorView","nMultValueOfCaretAccelarete",propSheet.m_Page6.m_nMultValueOfCaretAccelarete+1);
			//�\��t����̃L�����b�g�ʒu
			m_pApp->WriteProfileInt("CEditorDoc","bSetCaretToEndOnPaste",propSheet.m_Page6.m_nSetCaretToEndOnPaste);
		}
		if( propSheet.m_Page7.IsChanged() ) //�y�[�W�V�ɕύX����
		{
			m_wndClient.SetFlagDispLogoBmp(propSheet.m_Page7.m_bDispLogoBmp);
			m_wndClient.SetLogoBmpKind(propSheet.m_Page7.m_nLogoBmpKind);
	 		m_wndClient.SetLogoBmpFilePath(propSheet.m_Page7.m_objBmpFilePath);
			m_wndClient.SetLogoPosKind(propSheet.m_Page7.m_nLogoPosKind);
			if(propSheet.m_Page7.m_unLogoPosX>=0 && propSheet.m_Page7.m_unLogoPosX<=100)
				m_wndClient.SetLogoPosX(propSheet.m_Page7.m_unLogoPosX);
			if(propSheet.m_Page7.m_unLogoPosY>=0 && propSheet.m_Page7.m_unLogoPosY<=100)
				m_wndClient.SetLogoPosY(propSheet.m_Page7.m_unLogoPosY);
		}
		if( propSheet.m_Page8.IsChanged() ) //�y�[�W�W�ɕύX����
		{
			SetStatusBarDispInfo(&propSheet.m_Page8.m_stStatusBarDisp);
			SetStatusDayStyle(propSheet.m_Page8.m_lStatusDayStyle);
			SetStatusHourStyle(propSheet.m_Page8.m_lStatusHourStyle);
			SetStatusLineStyle(propSheet.m_Page8.m_lStatusLineStyle);
		}
		if( propSheet.m_Page9.IsChanged() ) //�y�[�W�X�ɕύX����
		{
			propSheet.m_Page9.ChangesToRegistry();
		}
		if( propSheet.m_Page11.IsChanged() ) //�y�[�W�P�P�ɕύX����
		{
			propSheet.m_Page11.ChangesToRegistry();
		}
		if( propSheet.m_Page12.IsChanged() ) //�y�[�W�P�Q�ɕύX����
		{
			m_bIsUseProxyHtml       = propSheet.m_Page12.m_bIsUseProxy;
			m_objProxyIPAddressHtml = propSheet.m_Page12.m_objProxyIPAddress;
			m_bIsDialupHtml         = propSheet.m_Page12.m_bIsDialup;
			m_objDialupEntryHtml    = propSheet.m_Page12.m_objDialupEntry;
			m_objRasUserNameHtml    = propSheet.m_Page12.m_objRasUserName;
			m_objRasPasswdHtml      = propSheet.m_Page12.m_objRasPasswd;
			m_bIsAutoDisconnectHtml   = propSheet.m_Page12.m_bIsAutoDisconnect;
		}
	}
}
void CMainFrame::OnUpdateSettings(CCmdUI* pCmdUI) 
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�h�L�������g���݃`�F�b�N
	CEditorDoc*  pobjEditorDoc  = (CEditorDoc*)pobjFrameWnd->GetActiveDocument();
	if(pobjEditorDoc == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	//����E����v���r���[���`�F�b�N
	if( pobjEditorDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame ���j���[�n���h���֐��i�A�C�R���t�����j���[�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�l�d�`�r�t�q�d�h�s�d�l����										|*/
/*|	���	�F																		|*/
/*|	���l	�FThis handler ensure that the popup menu items are drawn correctly		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	BOOL setflag=FALSE;

	if(lpMeasureItemStruct->CtlType==ODT_MENU)
	{
		if(IsMenu((HMENU)lpMeasureItemStruct->itemID))
		{
			CMenu* cmenu=CMenu::FromHandle((HMENU)lpMeasureItemStruct->itemID);
			if(m_menu.IsMenu(cmenu)||m_default.IsMenu(cmenu))
			{
				m_menu.MeasureItem(lpMeasureItemStruct);
				setflag=TRUE;
			}
		}
	}
	if(!setflag)
		CMDIFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�l�d�m�t�b�g�`�q����											|*/
/*|	���	�F																		|*/
/*|	���l	�FThis handler ensures that keyboard shortcuts work						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LRESULT CMainFrame::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
{
	LRESULT lresult;
	if(m_menu.IsMenu(pMenu)||m_default.IsMenu(pMenu))
		lresult=BCMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
	else
		lresult=CMDIFrameWnd::OnMenuChar(nChar, nFlags, pMenu);
	return(lresult);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�l�d�m�t�b�g�`�q����											|*/
/*|	���	�F																		|*/
/*|	���l	�FThis handler updates the menus from time to time						|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CMDIFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	if(!bSysMenu)
	{
		if(m_menu.IsMenu(pPopupMenu)||m_default.IsMenu(pPopupMenu))
			BCMenu::UpdateMenu(pPopupMenu);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�q�E�C���h�E�����邩�H												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsHaveChildWnd()
{
	//�A�N�e�B�u�t���[�����݃`�F�b�N
	CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
	if(pobjFrameWnd == NULL)
		return FALSE;
	//�A�N�e�B�u�r���[���݃`�F�b�N
	CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
	if(pobjEditorView == NULL)
		return FALSE;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����p�����[�^�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetFindParameter
	(	CString&	objFindString,
		BOOL		bIsMatchCaseFind,
		BOOL		bIsMatchWholeWordFind,
		BOOL		bIsSearchDownFind,
		BOOL		bIsUseRegularExpression
	)
{
	m_objFindString         = objFindString;
	m_bIsMatchCaseFind      = bIsMatchCaseFind;
	m_bIsMatchWholeWordFind = bIsMatchWholeWordFind;
	m_bIsSearchDownFind     = bIsSearchDownFind;
	m_bIsUseRegularExpressionFind = bIsUseRegularExpression;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u���p�����[�^�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetReplaceParameter
	(	CString&	objFindString,
		CString&	objReplaceString,
		BOOL		bIsMatchCaseReplace,
		BOOL		bIsMatchWholeWordReplace,
		BOOL		bIsUseRegularExpression
	)
{
	m_objFindString					 = objFindString;
	m_objReplaceString				 = objReplaceString;
	m_bIsMatchCaseReplace			 = bIsMatchCaseReplace;
	m_bIsMatchWholeWordReplace		 = bIsMatchWholeWordReplace;
	m_bIsUseRegularExpressionReplace = bIsUseRegularExpression;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�g�s�l�k�t�@�C���I�[�v���p�����[�^�ݒ菈��							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetHtmlOpenParameter
	(	BOOL		bIsUseProxy,
		CString&	objProxyIPAddress,
		BOOL		bIsDialup,
		CString&	objDialupEntry,
		CString&	objRasUserName,
		CString&	objRasPasswd,
		BOOL		bIsAutoDisconnect
	)
{
	m_bIsUseProxyHtml       = bIsUseProxy;
	m_objProxyIPAddressHtml = objProxyIPAddress;
	m_bIsDialupHtml         = bIsDialup;
	m_objDialupEntryHtml    = objDialupEntry;
	m_objRasUserNameHtml    = objRasUserName;
	m_objRasPasswdHtml      = objRasPasswd;
	m_bIsAutoDisconnectHtml = bIsAutoDisconnect;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�o�[�\�����ڏ��ݒ菈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::SetStatusBarDispInfo
	(	LPSTATUSBARDISP	lpStatusBarDisp
	)
{
	BOOL bRet = FALSE;
	int nCnt = 0;
	UINT* punIndicators = (UINT*)malloc(sizeof(UINT)*(1+lpStatusBarDisp->m_nCurrentItemCount));
	int nIndicatorsCount=0;

	punIndicators[0] = ID_SEPARATOR;
	nIndicatorsCount++;
	for(nCnt=0; nCnt<lpStatusBarDisp->m_nCurrentItemCount; nCnt++)
	{
		punIndicators[nIndicatorsCount] = lpStatusBarDisp->m_nCurrentItems[nCnt];
		nIndicatorsCount++;
	}

	bRet = m_wndStatusBar.SetIndicators(punIndicators,nIndicatorsCount);
	m_wndStatusBar.UpdateWindow();
	free(punIndicators);

	if( lpStatusBarDisp != &m_stStatusBarDisp )
		memcpy(&m_stStatusBarDisp,lpStatusBarDisp,sizeof(STATUSBARDISP));

	return bRet;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ʒu�o�C�g����\�����邩�̃`�F�b�N����						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsDispIndicatorByte()
{
	long lCnt=0;
	for(lCnt=0; lCnt<m_stStatusBarDisp.m_nCurrentItemCount; lCnt++)
	{
		if(m_stStatusBarDisp.m_nCurrentItems[lCnt] == ID_INDICATOR_BYTE)
			return TRUE;
	}
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ʒu�s�A���\�����邩�̃`�F�b�N����						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsDispIndicatorGyoretu()
{
	long lCnt=0;
	for(lCnt=0; lCnt<m_stStatusBarDisp.m_nCurrentItemCount; lCnt++)
	{
		if(m_stStatusBarDisp.m_nCurrentItems[lCnt] == ID_INDICATOR_GYORETU)
			return TRUE;
	}
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ʒu�R�[�h��\�����邩�̃`�F�b�N����						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsDispIndicatorCode()
{
	long lCnt=0;
	for(lCnt=0; lCnt<m_stStatusBarDisp.m_nCurrentItemCount; lCnt++)
	{
		if(m_stStatusBarDisp.m_nCurrentItems[lCnt] == ID_INDICATOR_CHARCODE)
			return TRUE;
	}
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�ʒu�ۑ��t���O�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetSaveWindowPlace
	(	BOOL	bIsSaveWindowPlace
	)
{
	m_bIsSaveWindowPlace = bIsSaveWindowPlace;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�y�C�����b�Z�[�W�ݒ菈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetStatusMessage
	(	char*	pszMsg
	)
{
	//�y�C�������X�V���ăe�L�X�g�ɂ��킹��
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_SEPARATOR);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(pszMsg,strlen(pszMsg));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//�y�C���Ƀ��b�Z�[�W�ݒ�
	m_wndStatusBar.SetPaneText(nIndex,pszMsg);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�y�C�����b�Z�[�W�ݒ菈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetStatusMessage
	(	CString& objMsg
	)
{
	//�y�C�������X�V���ăe�L�X�g�ɂ��킹��
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_SEPARATOR);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent((LPCTSTR)objMsg,objMsg.GetLength());
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//�y�C���Ƀ��b�Z�[�W�ݒ�
	m_wndStatusBar.SetPaneText(nIndex,(LPCTSTR)objMsg);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�y�C���ւ̍s�A��ԍ��ݒ菈��								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetGyoRetu
	(	long	lGyo,		//�s�ԍ�
		long	lRetu,		//��ԍ�
		long	lMaxGyo		//�S�s��
	)
{
	char	buf[256];
	CString objMsg;

	if( (lGyo<0) || (lRetu<0) || (lMaxGyo<0) )
		return;

	//�s�A�񐔕ۑ�
	m_lSavedGyo = lGyo;
	m_lSavedRetu = lRetu;
	m_lSavedMaxGyo = lMaxGyo;

	//�s�A��ԍ��ҏW
	if(m_lStatusLineStyle==STS_LINESTYLE_LINE_OFFS_ALL)
	{
		objMsg.LoadString(IDS_LINESTYLE_LINE_OFFS_ALL);
		sprintf( buf, (LPCTSTR)objMsg, lGyo, lRetu, lMaxGyo );
	}
	else if(m_lStatusLineStyle==STS_LINESTYLE_LINE_OFFS)
	{
		objMsg.LoadString(IDS_LINESTYLE_LINE_OFFS);
		sprintf( buf, (LPCTSTR)objMsg, lGyo, lRetu );
	}
	else if(m_lStatusLineStyle==STS_LINESTYLE_LINE_ALL)
	{
		objMsg.LoadString(IDS_LINESTYLE_LINE_ALL);
		sprintf( buf, (LPCTSTR)objMsg, lGyo, lMaxGyo );
	}
	else if(m_lStatusLineStyle==STS_LINESTYLE_ALL)
	{
		objMsg.LoadString(IDS_LINESTYLE_ALL);
		sprintf( buf, (LPCTSTR)objMsg, lMaxGyo );
	}
	else if(m_lStatusLineStyle==STS_LINESTYLE_LINE)
	{
		objMsg.LoadString(IDS_LINESTYLE_LINE);
		sprintf( buf, (LPCTSTR)objMsg, lGyo );
	}
	else if(m_lStatusLineStyle==STS_LINESTYLE_OFFS)
	{
		objMsg.LoadString(IDS_LINESTYLE_OFFS);
		sprintf( buf, (LPCTSTR)objMsg, lRetu );
	}

	//�y�C�������X�V���ăe�L�X�g�ɂ��킹��
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_GYORETU);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//�y�C���ɍs�A��ԍ��\��
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�y�C���̍s�A��ԍ��N���A����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::ClearGyoRetu()
{
	char	buf[256];

	//�_�~�[
	strcpy( buf, "    " );

	//�y�C�������X�V���ăe�L�X�g�ɂ��킹��
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_GYORETU);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//�y�C���̍s�A��ԍ��N���A
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�y�C���ւ̑}���A�㏑��Ӱ�ސݒ菈��							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetInsMode
	(	BOOL	bInsertKey			//Insert�L�[�����t���O(TRUE:�}���AFALSE:�㏑���j
	)
{
	char	buf[256];
	CString objMsg;

	//�ҏW
	if(bInsertKey)
		objMsg.LoadString(IDS_INSMODE_INSERT);
	else
		objMsg.LoadString(IDS_INSMODE_OVERWRITE);
	strcpy(buf,(LPCTSTR)objMsg);

	//�y�C�������X�V���ăe�L�X�g�ɂ��킹��
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_INS);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//�y�C���ɑ}���A�㏑��Ӱ�ސݒ�
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�y�C���̑}���A�㏑��Ӱ�޸ر����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::ClearInsMode()
{
	char	buf[256];

	//�_�~�[
	strcpy( buf, "    " );

	//�y�C�������X�V���ăe�L�X�g�ɂ��킹��
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_INS);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//�y�C���̑}���A�㏑��Ӱ�޸ر
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�y�C���̏����݋֎~���[�h�ݒ菈��							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetReadOnlyMode
	(	BOOL	bIsReadOnly
	)
{
	char	buf[256];
	CString objMsg;

	//�ð���ްү����
	if( bIsReadOnly )
		objMsg.LoadString(IDS_READONLYMODE_READONLY);
	else
		objMsg.LoadString(IDS_READONLYMODE_WRITEOK);
	strcpy(buf,(LPCTSTR)objMsg);

	//�y�C�������X�V���ăe�L�X�g�ɂ��킹��
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_READONLY);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//�����݋֎~���[�h�ݒ�
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�y�C���̏����݋֎~Ӱ�޸ر����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::ClearReadOnlyMode()
{
	char	buf[256];

	//�_�~�[
	strcpy( buf, "    " );

	//�y�C�������X�V���ăe�L�X�g�ɂ��킹��
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_READONLY);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//�y�C���̑}���A�㏑��Ӱ�޸ر
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�y�C���ւ̓��t�A�����ݒ菈��								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetDateTime()
{
	char	buf[64];
	char*	pPtr=NULL;
	CString	objDayStr=_T("");
	CString objTimeStr=_T("");
	CString objMsg;

	//���ݎ����擾
	CTime	objTimeNow = CTime::GetCurrentTime();

	//���t�\���t�H�[�}�b�g
	if(m_lStatusDayStyle==STS_DAYSTYLE_NONE)
		objDayStr = "";
	else if(m_lStatusDayStyle==STS_DAYSTYLE_YYYYMMDD)
		objDayStr = objTimeNow.Format( "%Y/%m/%d (%a)" );
	else if(m_lStatusDayStyle==STS_DAYSTYLE_YYMMDD)
		objDayStr = objTimeNow.Format( "'%y/%m/%d (%a)" );
	else if(m_lStatusDayStyle==STS_DAYSTYLE_MMDDYYYY)
		objDayStr = objTimeNow.Format( "%m/%d/%Y (%a)" );
	else if(m_lStatusDayStyle==STS_DAYSTYLE_MMDDYY)
		objDayStr = objTimeNow.Format( "%m/%d/'%y (%a)" );
	else if(m_lStatusDayStyle==STS_DAYSTYLE_MMDD)
		objDayStr = objTimeNow.Format( "%m/%d (%a)" );
	else if(m_lStatusDayStyle==STS_DAYSTYLE_DDMMYYYY)
		objDayStr = objTimeNow.Format( "%d/%m/%Y (%a)" );
	else if(m_lStatusDayStyle==STS_DAYSTYLE_DDMMYY)
		objDayStr = objTimeNow.Format( "%d/%m/'%y (%a)" );
	else if(m_lStatusDayStyle==STS_DAYSTYLE_DDMM)
		objDayStr = objTimeNow.Format( "%d/%m (%a)" );
	else if(m_lStatusDayStyle==STS_DAYSTYLE_YYYY_MM_DD)
	{
		objMsg.LoadString(IDS_DAYSTYLE_YYYY_MM_DD);
		objDayStr = objTimeNow.Format( objMsg );
	}
	else if(m_lStatusDayStyle==STS_DAYSTYLE_YY_MM_DD)
	{
		objMsg.LoadString(IDS_DAYSTYLE_YY_MM_DD);
		objDayStr = objTimeNow.Format( objMsg );
	}
	else if(m_lStatusDayStyle==STS_DAYSTYLE_MM_DD)
	{
		objMsg.LoadString(IDS_DAYSTYLE_MM_DD);
		objDayStr = objTimeNow.Format( objMsg );
	}

	//���ԕ\���t�H�[�}�b�g
	if(m_lStatusHourStyle==STS_HOURSTYLE_NONE)
		objTimeStr = "";
	else if(m_lStatusHourStyle==STS_HOURSTYLE_HHMM)
		objTimeStr = objTimeNow.Format( "%H:%M" );
	else if(m_lStatusHourStyle==STS_HOURSTYLE_AMPM_HHMM)
		objTimeStr = objTimeNow.Format( "%p %I:%M" );
	else if(m_lStatusHourStyle==STS_HOURSTYLE_HH_MM)
	{
		objMsg.LoadString(IDS_HOURSTYLE_HH_MM);
		objTimeStr = objTimeNow.Format( objMsg );
	}
	else if(m_lStatusHourStyle==STS_HOURSTYLE_AMPM_HH_MM)
	{
		objMsg.LoadString(IDS_HOURSTYLE_AMPM_HH_MM);
		objTimeStr = objTimeNow.Format( objMsg );
	}

	if(objDayStr.IsEmpty())
	{
		if(objTimeStr.IsEmpty())
			objDayStr = "    ";
		else
			objDayStr = objTimeStr;
	}
	else
	{
		if(objTimeStr.IsEmpty())
			;
		else 
		{
			objDayStr += " ";
			objDayStr += objTimeStr;
		}
	}

	strcpy(buf,(LPCTSTR)objDayStr);

	//�y�C�������X�V���ăe�L�X�g�ɂ��킹��
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_TIME);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//�y�C���Ɍ��ݓ��t�����ݒ�
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�y�C���̓��t�A�����N���A����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::ClearDateTime()
{
	char	buf[256];

	//�_�~�[
	strcpy( buf, "    " );

	//�y�C�������X�V���ăe�L�X�g�ɂ��킹��
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_TIME);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//�y�C���̑}���A�㏑��Ӱ�޸ر
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�y�C���ւ̃L�����b�g�ʒu�R�[�h�ݒ菈��						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetCaretPosCharCode
	(	UCHAR	uc1stChar,				// �L�����b�g�ʒu�R�[�h���o�C�g
		UCHAR	uc2ndChar				// �L�����b�g�ʒu�R�[�h���o�C�g
	)
{
	char	buf[256];

	//�L�����b�g�ʒu�R�[�h�ݒ�
	if(uc2ndChar == 0)
		sprintf( buf, "CODE = %02XH    ", uc1stChar );
	else 
		sprintf( buf, "CODE = %02XH,%02XH", uc1stChar, uc2ndChar );

	//�y�C�������X�V���ăe�L�X�g�ɂ��킹��
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_CHARCODE);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//�y�C���̃L�����b�g�ʒu�R�[�h�ݒ�
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�y�C���ւ̃L�����b�g�ʒu�R�[�h�N���A����					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::ClearCaretPosCharCode()
{
	char	buf[256];

	//�_�~�[
	strcpy( buf, "    " );

	//�y�C�������X�V���ăe�L�X�g�ɂ��킹��
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_CHARCODE);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//�y�C���̃L�����b�g�ʒu�R�[�h�N���A
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�y�C���ւ̃L�����b�g�ʒu�o�C�g���ݒ菈��					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetCaretPosByte
	(	long	lCaretByte,				// �L�����b�g�ʒu�o�C�g��
		long	lSumByte				// ���v�o�C�g��
	)
{
	char	buf[256];

	//�o�C�g���ݒ�
	sprintf( buf, "BYTE=%6d / SUM=%6d", lCaretByte, lSumByte );

	//�y�C�������X�V���ăe�L�X�g�ɂ��킹��
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_BYTE);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//�y�C���̃L�����b�g�ʒu�o�C�g���ݒ�
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�y�C���ւ̃L�����b�g�ʒu�o�C�g���N���A����					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::ClearCaretPosByte()
{
	char	buf[256];

	//�_�~�[
	strcpy( buf, "    " );

	//�y�C�������X�V���ăe�L�X�g�ɂ��킹��
	UINT	nID, nStyle;
	int		nWidth,nIndex=m_wndStatusBar.CommandToIndex(ID_INDICATOR_BYTE);
	if(nIndex == -1)
		return;

	CClientDC	dc(&m_wndStatusBar);
	CFont*		pOldFont=dc.SelectObject(m_wndStatusBar.GetFont());
	CSize		szExtent=dc.GetTextExtent(buf,strlen(buf));
	dc.SelectObject(pOldFont);
	m_wndStatusBar.GetPaneInfo(nIndex,nID,nStyle,nWidth);
	m_wndStatusBar.SetPaneInfo(nIndex,nID,nStyle,szExtent.cx);

	//�y�C���̃L�����b�g�ʒu�o�C�g���N���A
	m_wndStatusBar.SetPaneText(nIndex,buf);
	m_wndStatusBar.UpdateWindow();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�o�[�t�H���g�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetStatusBarFont
	(	LOGFONT		stLogFont
	)
{
	m_objStatusBarFont.DeleteObject();
	m_objStatusBarFont.CreateFontIndirect(&stLogFont);
	m_wndStatusBar.SetFont(&m_objStatusBarFont);
	RecalcLayout();

	memcpy(&m_stStatusBarLogFont,&stLogFont,sizeof(LOGFONT));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�[�g�Z�[�u�t���O�ݒ菈��											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetFlagAutoSave
	(	BOOL	bIsAutoSave
	)
{
	m_bIsAutoSave = bIsAutoSave;

	//�^�C���A�b�v�J�E���^�N���A
	m_lTimeUpCount = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�[�g�Z�[�u�Ԋu�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetSaveInterval
	(	long	lSaveInterval
	)
{
	if(lSaveInterval>=1 && lSaveInterval<=60)
		m_lSaveInterval = lSaveInterval;

	//�^�C���A�b�v�J�E���^�N���A
	m_lTimeUpCount = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�l�c�h�w�i�F�ݒ菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetMDIBkgndColor
	(	COLORREF	color
	)
{
	m_wndClient.SetMDIBkgndColor(color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�l�c�h�@���S�\���t���O�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetFlagDispLogoBmp
	(	BOOL	bDispLogoBmp
	)
{
	m_wndClient.SetFlagDispLogoBmp(bDispLogoBmp);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�l�c�h�@���S�r�b�g�}�b�v��ʐݒ菈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetLogoBmpKind
	(	int		nLogoBmpKind
	)
{
	m_wndClient.SetLogoBmpKind(nLogoBmpKind);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�l�c�h�@���S�\���ʒu��ʐݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetLogoPosKind
	(	int		nLogoPosKind
	)
{
	m_wndClient.SetLogoPosKind(nLogoPosKind);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�l�c�h�@���S�r�b�g�}�b�v�t�@�C���p�X�ݒ菈��							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetLogoBmpFilePath
	(	CString	objLogoBmpFilePath
	)
{
	m_wndClient.SetLogoBmpFilePath(objLogoBmpFilePath);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�l�c�h�@���S�\���ʒu�w�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetLogoPosX
	(	UINT	unLogoPosX
	)
{
	m_wndClient.SetLogoPosX(unLogoPosX);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�l�c�h�@���S�\���ʒu�x�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetLogoPosY
	(	UINT	unLogoPosY
	)
{
	m_wndClient.SetLogoPosY(unLogoPosY);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[����L�^����������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsNowOperationRecording()
{
	return m_bIsNowOperationRecording;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[����X�e�b�v���s����������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsNowStepDoOperation()
{
	return m_bIsNowStepDoOperation;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[����L�^����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::addOperation
	(	COperation*		pobjOperation
	)
{
	m_objOperationManager.addOperation(pobjOperation);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�o�[�֕\��������t�\���X�^�C���擾����						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CMainFrame::GetStatusDayStyle()
{
	return m_lStatusDayStyle;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�o�[�֕\��������t�\���X�^�C���ݒ菈��						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetStatusDayStyle
	(	long	lStatusDayStyle
	)
{
	if(m_lStatusDayStyle == lStatusDayStyle)
		return;
	if(lStatusDayStyle < STS_DAYSTYLE_NONE || lStatusDayStyle > STS_DAYSTYLE_MM_DD)
		return;

	m_lStatusDayStyle = lStatusDayStyle;
	SetDateTime();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�o�[�֕\�����鎞���\���X�^�C���擾����						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CMainFrame::GetStatusHourStyle()
{
	return m_lStatusHourStyle;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�o�[�֕\�����鎞���\���X�^�C���ݒ菈��						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetStatusHourStyle
	(	long	lStatusHourStyle
	)
{
	if(m_lStatusHourStyle == lStatusHourStyle)
		return;
	if(lStatusHourStyle < STS_HOURSTYLE_NONE || lStatusHourStyle > STS_HOURSTYLE_AMPM_HH_MM)
		return;

	m_lStatusHourStyle = lStatusHourStyle;
	SetDateTime();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X�o�[�֕\������s�A��\���X�^�C���ݒ菈��					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetStatusLineStyle
	(	long	lStatusLineStyle
	)
{
	if(m_lStatusLineStyle == lStatusLineStyle)
		return;
	if(lStatusLineStyle < STS_LINESTYLE_LINE_OFFS_ALL || lStatusLineStyle > STS_LINESTYLE_OFFS)
		return;

	m_lStatusLineStyle = lStatusLineStyle;
	SetGyoRetu(m_lSavedGyo,m_lSavedRetu,m_lSavedMaxGyo);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�C���N�������^���T�[�`���s����������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsNowIncrimentalSearching()
{
	return m_bIsNowIncrimentalSearching;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�C���N�������^���T�[�`���s���t���O�ݒ菈��							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SetFlagIncrimentalSearching
	(	BOOL	bIsNowIncrimentalSearching
	)
{
	CString objMsg;

	m_bIsNowIncrimentalSearching = bIsNowIncrimentalSearching;

	if(bIsNowIncrimentalSearching==FALSE)
	{
		//�X�e�[�^�X�y�C���Ƀ��b�Z�[�W�ݒ�
		objMsg.LoadString(AFX_IDS_IDLEMESSAGE);
		SetStatusMessage(objMsg);
		//�C���N�������^���T�[�`������������p��
		CFrameWnd* pobjFrameWnd = (CFrameWnd*)GetActiveFrame();
		if(pobjFrameWnd!=NULL)
		{
			CEditorView* pobjEditorView = (CEditorView*)pobjFrameWnd->GetActiveView();
			if(pobjEditorView!=NULL)
				pobjEditorView->GetIncrimentalSearchString(m_objFindString);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �p�u���b�N�֐��i�A�C�R���t�����j���[�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���j���[����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
HMENU CMainFrame::NewMenu()
{
	static UINT toolbars[]={
		IDR_MAINFRAME,
		IDR_TOOLBAR_EDIT,
		IDR_TOOLBAR_FIND,
		IDR_TOOLBAR_JMP,
		IDR_TOOLBAR_TOOLS,
		IDR_TOOLBAR_WND
	};

	// Load the menu from the resources
	m_menu.LoadMenu(IDR_EDITORTYPE);  // ****replace IDR_MENUTYPE with your menu ID****

	// Use ModifyODMenu to add a bitmap to a menu options.The first parameter
	// is the menu option text string.If it's NULL, keep the current text
	// string.The second option is the ID of the menu option, or the menu
	// option text (can use this for adding bitmaps to popup options) to change.
	// The third option is the resource ID of the bitmap.This can also be a
	// toolbar ID in which case the class searches the toolbar for the
	// appropriate bitmap.Only Bitmap and Toolbar resources are supported.
//	m_menu.ModifyODMenu(NULL,ID_EDIT_INSDAY,IDB_EDIT_INSDAY);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_INSTIME,IDB_EDIT_INSTIME);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_INSASMCOMMENT,IDB_EDIT_INSASMCOMMENT);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_INSVBCOMMENT,IDB_EDIT_INSVBCOMMENT);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_INSCPPCOMMENT,IDB_EDIT_INSCPPCOMMENT);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_INSTAB,IDB_EDIT_INSTAB);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_DELTAB,IDB_EDIT_DELTAB);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_SETREADONLY,IDB_SETREADONLY);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_UNSETREADONLY,IDB_UNSETREADONLY);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_CLIPVIEW,IDB_EDIT_CLIPVIEW);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_FILECOMPARE,IDB_EDIT_FILECOMPARE);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_DIRCOMPARE,IDB_EDIT_DIRCOMPARE);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_FILETOUCH,IDB_EDIT_FILETOUCH);
//	m_menu.ModifyODMenu(NULL,ID_EDIT_BROWSEURL,IDB_EDIT_BROWSEURL);
//	m_menu.ModifyODMenu(NULL,ID_JUMP_SETMARK,IDB_JUMP_SETMARK);
//	m_menu.ModifyODMenu(NULL,ID_JUMP_DELMARK,IDB_JUMP_DELMARK);
//	m_menu.ModifyODMenu(NULL,ID_JUMP_NEXTMARK,IDB_JUMP_NEXTMARK);
//	m_menu.ModifyODMenu(NULL,ID_JUMP_PREVMARK,IDB_JUMP_PREVMARK);
//	m_menu.ModifyODMenu(NULL,ID_WINDOW_NEW,IDB_WINDOW_NEW);
//	m_menu.ModifyODMenu(NULL,ID_WINDOW_TILE_HORZ,IDB_WINDOW_TILE_HORZ);
//	m_menu.ModifyODMenu(NULL,ID_WINDOW_TILE_VERT,IDB_WINDOW_TILE_VERT);
//	m_menu.ModifyODMenu(NULL,ID_WINDOW_CASCADE,IDB_WINDOW_CASCADE);
//	m_menu.ModifyODMenu(NULL,ID_WINDOW_CLOSE,IDB_WINDOW_CLOSE);
//	m_menu.ModifyODMenu(NULL,ID_WINDOW_ALLCLOSE,IDB_WINDOW_ALLCLOSE);
//	m_menu.ModifyODMenu(NULL,ID_WINDOW_SPLIT,IDB_WINDOW_SPLIT);
//	m_menu.ModifyODMenu(NULL,ID_WINDOW_ARRANGE,IDB_WINDOW_ARRANGE);
//	m_menu.ModifyODMenu(NULL,ID_WND_NEXT,IDB_WND_NEXT);
//	m_menu.ModifyODMenu(NULL,ID_WND_PREV,IDB_WND_PREV);
	m_menu.ModifyODMenu(NULL,ID_VIEW_REDRAW,IDB_VIEW_REDRAW);
//	m_menu.ModifyODMenu(NULL,ID_SECURITY_ENCRIPT,IDB_SECURITY_ENCRIPT);
//	m_menu.ModifyODMenu(NULL,ID_SECURITY_DECRIPT,IDB_SECURITY_DECRIPT);

	// Another method for adding bitmaps to menu options is through the
	// LoadToolbars member function.This method allows you to add all
	// the bitmaps in a toolbar object to menu options (if they exist).
	// The first function parameter is an array of toolbar id's.
	// The second is the number of toolbar id's. There is also a
	// function called LoadToolbar that just takes an id.
	m_menu.LoadToolbars(toolbars,6);

	return(m_menu.Detach());
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A�C�R���t�����j���[����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
HMENU CMainFrame::NewDefaultMenu()
{
	m_default.LoadMenu(IDR_MAINFRAME);
	m_default.LoadToolbar(IDR_MAINFRAME);
	return(m_default.Detach());
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �v���e�N�g�֐��i�c�[���o�[�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�c�[���o�[�쐬����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::CreateToolBar()
{
	DWORD	dwStyle = WS_CHILD | WS_VISIBLE | CBRS_SIZE_DYNAMIC | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY;

	//�W���̃c�[���o�[�쐬
	if( !m_wndToolBar.Create(this,dwStyle,ID_VIEW_TOOLBAR,m_wndToolBar_Info,
							 "Software\\S.Ekura\\Xerxes\\CustomizingToolbar","CustomizeInfo") || 
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
		return FALSE;
	//�ҏW�֘A�c�[���o�[�쐬
	if( !m_wndToolBarEdit.Create(this,dwStyle,IDW_TOOLBAR_EDIT,m_wndToolBarEdit_Info,
							 "Software\\S.Ekura\\Xerxes\\CustomizingToolbarEdit","CustomizeInfo") || 
		!m_wndToolBarEdit.LoadToolBar(IDR_TOOLBAR_EDIT))
		return FALSE;
	//�����֘A�c�[���o�[�쐬
	if( !m_wndToolBarFind.Create(this,dwStyle,IDW_TOOLBAR_FIND,m_wndToolBarFind_Info,
							 "Software\\S.Ekura\\Xerxes\\CustomizingToolbarFind","CustomizeInfo") || 
		!m_wndToolBarFind.LoadToolBar(IDR_TOOLBAR_FIND))
		return FALSE;
	//�W�����v�֘A�c�[���o�[�쐬
	if( !m_wndToolBarJmp.Create(this,dwStyle,IDW_TOOLBAR_JMP,m_wndToolBarJmp_Info,
							 "Software\\S.Ekura\\Xerxes\\CustomizingToolbarJmp","CustomizeInfo") || 
		!m_wndToolBarJmp.LoadToolBar(IDR_TOOLBAR_JMP))
		return FALSE;
	//�c�[���֘A�c�[���o�[�쐬
	if( !m_wndToolBarTools.Create(this,dwStyle,IDW_TOOLBAR_TOOLS,m_wndToolBarTools_Info,
							 "Software\\S.Ekura\\Xerxes\\CustomizingToolbarTools","CustomizeInfo") || 
		!m_wndToolBarTools.LoadToolBar(IDR_TOOLBAR_TOOLS))
		return FALSE;
	//�E�C���h�E�֘A�c�[���o�[�쐬
	if( !m_wndToolBarWnd.Create(this,dwStyle,IDW_TOOLBAR_WND,m_wndToolBarWnd_Info,
							 "Software\\S.Ekura\\Xerxes\\CustomizingToolbarWnd","CustomizeInfo") || 
		!m_wndToolBarWnd.LoadToolBar(IDR_TOOLBAR_WND))
		return FALSE;


	//�c�[���o�[�h�b�L���O�\��
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarEdit.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarFind.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarJmp.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarTools.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBarWnd.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);

	//�c�[���o�[�ʒu����
	DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_TOP);
	DockControlBarLeftOf(&m_wndToolBarEdit,&m_wndToolBar);
	DockControlBar(&m_wndToolBarFind);
	DockControlBarLeftOf(&m_wndToolBarJmp,&m_wndToolBarFind);
	DockControlBarLeftOf(&m_wndToolBarTools,&m_wndToolBarJmp);
	DockControlBarLeftOf(&m_wndToolBarWnd,&m_wndToolBarTools);

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�c�[���o�[�z�u����													|*/
/*|	���	�F																		|*/
/*|	���l	�FLeftOf�̃c�[���o�[�̉E�ׂ�Bar��z�u����								|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf)
{
	CRect RC;

	RecalcLayout();
	LeftOf->GetWindowRect(&RC);
	//�E�ւP�I�t�Z�b�g���ړ�
	RC.OffsetRect(1,0);
	DWORD  dw=LeftOf->GetBarStyle();
	UINT n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;
	DockControlBar(Bar,n,&RC);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �v���e�N�g�֐��i�E�C���h�E�ʒu�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�ʒu���[�h����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CMainFrame","WindowPos");
	if(strBuffer.IsEmpty())
		return FALSE;

	int cRead = _stscanf(strBuffer,"%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
					&pwp->flags,
					&pwp->showCmd,
					&pwp->ptMinPosition.x, &pwp->ptMinPosition.y,
					&pwp->ptMaxPosition.x, &pwp->ptMaxPosition.y,
					&pwp->rcNormalPosition.left,  &pwp->rcNormalPosition.top,
					&pwp->rcNormalPosition.right, &pwp->rcNormalPosition.bottom);
	if(cRead != 10)
		return FALSE;
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�ʒu�Z�[�u����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	//�E�C���h�E�̈ʒu��ۑ�����
	CString	strBuffer;
	strBuffer.Format("%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
					pwp->flags,
					pwp->showCmd,
					pwp->ptMinPosition.x, pwp->ptMinPosition.y,
					pwp->ptMaxPosition.x, pwp->ptMaxPosition.y,
					pwp->rcNormalPosition.left,  pwp->rcNormalPosition.top,
					pwp->rcNormalPosition.right, pwp->rcNormalPosition.bottom);
	m_pApp->WriteProfileString("CMainFrame","WindowPos",strBuffer);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �v���e�N�g�֐��i�g�s�l�k�I�[�v���֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�g�s�l�k�I�[�v���󋵃_�C�A���O�\���҂�����							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForHtmlOpenDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjHtmlOpenStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjHtmlOpenStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjHtmlOpenStatusDlg->m_hWnd);
			if(bRet!=0)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �v���e�N�g�֐��i�f�q�d�o�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�q�d�o���s����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoGrep()
{
	CString objMsg;

	//�f�q�d�o���s�f�B���N�g�����݃`�F�b�N
	CFileFind		objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolderGrep);
	if(!bRet)//�Ȃ��I�I
	{
		if(m_pApp->IsDriveRoot(m_objFolderGrep) == FALSE) //���[�g�f�B���N�g���ł��Ȃ�
		{
			objMsg.LoadString(IDS_NOGREPFOLDER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//�f�q�d�o���ʏo�͗p�l�c�h�q�E�C���h�E�쐬
	m_pobjGrepDoc = NULL;
	m_pobjGrepFrameWnd = NULL;
	bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjGrepDoc,&m_pobjGrepFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjGrepDoc->SetFontOfCurrentFileExt("");
	m_pobjGrepDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);
	CString	objTitle;
	objMsg.LoadString(IDS_GERP_TITLE);
	objTitle.Format(objMsg,m_objGrepStr);
	m_pobjGrepDoc->SetTitle(objTitle);
	m_pobjGrepDoc->SetGrepMode(TRUE);

	//�f�q�d�o�󋵃��[�h���X�_�C�A���O�\��
	m_pobjGrepStatusDlg = new CGrepStatusDlg();
	m_pobjGrepStatusDlg->Create(IDD_GREPSTATUS);
	m_pobjGrepStatusDlg->SetGrepStr(m_objGrepStr);
	m_pobjGrepStatusDlg->SetFileKind(m_objFileKindGrep);
	m_pobjGrepStatusDlg->SetFolder(m_objFolderGrep);
	WaitForGrepDlgDisp();

	//�P�f�B���N�g�����f�q�d�o���s
	bRet = DoGrepInOneDirectory(m_objFolderGrep,m_objFileKindGrep,m_objGrepStr);

	//�Y�������^�C�g���ǉ��\��
	long lMatchCount = m_pobjGrepStatusDlg->GetMatchCount();
	char	szBuf[32];
	objMsg.LoadString(IDS_GREP_MATCHCOUNT);
	sprintf(szBuf, (LPCTSTR)objMsg, lMatchCount );
	CString objStr = szBuf;
	objTitle = m_pobjGrepDoc->GetTitle();
	objTitle += objStr;
	m_pobjGrepDoc->SetTitle(objTitle);

	//�f�q�d�o�󋵃��[�h���X�_�C�A���O����
	m_pobjGrepStatusDlg->DestroyWindow();
	delete m_pobjGrepStatusDlg;
	m_pobjGrepStatusDlg = NULL;

	m_pobjGrepDoc->UpdateAllViews(NULL,NULL,NULL);

	//�f�q�d�o�I���ʒm
	if( bRet != FALSE )
	{
		objMsg.LoadString(IDS_GREPEND);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
	else
	{
		objMsg.LoadString(IDS_GREPCANCEL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�f�B���N�g�����f�q�d�o���s����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoGrepInOneDirectory		// TRUE : ����I���AFALSE : �L�����Z���I��
	(	CString		objFolder,				// �f�q�d�o���s���t�H���_
		CString		objFileKinds,			// �f�q�d�o�Ώۃt�@�C�����
		CString		objGrepStr				// �f�q�d�o������
	)
{
	BOOL		bRetStatus = TRUE;
	BOOL		bRet, bRetTmp;
	CString		objFilter;
	CString		objOneKind, objRight, objBuf;
	CFileFind	objFileFind;

	//�f�q�d�o�󋵃_�C�A���O�փt�H���_�ʒm
	if( !IsGrepCanceled() )
		m_pobjGrepStatusDlg->SetFolder(objFolder);

	objRight = objFileKinds;
	for(;;)
	{
		//�E�C���h�E�Y���b�Z�[�W����
		m_pApp->MessagePump();

		//�L�����Z���`�F�b�N
		if( IsGrepCanceled() )
			return FALSE;

		//�f�q�d�o�Ώۃt�@�C����ʐ؂���
		int nIndex = objRight.Find(';');
		if(nIndex != -1)
		{
			objOneKind = objRight.Left(nIndex);
			objBuf = objRight.Right( objRight.GetLength() - nIndex - 1 );
			objRight = objBuf;
		}
		else
		{
			objOneKind = objRight;
		}

		//�t�@�C�������L�[�쐬
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s%s",objFolder,objOneKind);
		else
			objFilter.Format("%s\\%s",objFolder,objOneKind);

		//�f�q�d�o�Ώۃt�@�C������
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//�E�C���h�E�Y���b�Z�[�W����
				m_pApp->MessagePump();

				//�L�����Z���`�F�b�N
				if( IsGrepCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
					continue;

				//�P�t�@�C�����f�q�d�o���s
				bRetTmp = DoGrepInOneFile(objFileFind.GetFileName(),objFileFind.GetFilePath(),objGrepStr);
				if( bRetTmp == FALSE ) //�L�����Z���I�����Ă���
				{
					objFileFind.Close();
					return FALSE;
				}
			}
			//�Ō�̂P��
			if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
			{
				bRetTmp = DoGrepInOneFile(objFileFind.GetFileName(),objFileFind.GetFilePath(),objGrepStr);
				if( bRetTmp == FALSE ) //�L�����Z���I�����Ă���
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();

		if( nIndex == -1 ) // �t�@�C����ʎc��Ȃ�
		{
			break;
		}
	}

	//�E�C���h�E�Y���b�Z�[�W����
	m_pApp->MessagePump();
	//�L�����Z���`�F�b�N
	if( IsGrepCanceled() )
		return FALSE;

	if( m_bIsSearchSubFolderGrep ) //�T�u�t�H���_����������ꍇ
	{
		//�t�@�C�������L�[�쐬�i���C���h�J�[�h�����j
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s*.*",objFolder);
		else
			objFilter.Format("%s\\*.*",objFolder);

		//�T�u�f�B���N�g������
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//�E�C���h�E�Y���b�Z�[�W����
				m_pApp->MessagePump();

				//�L�����Z���`�F�b�N
				if( IsGrepCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}
				
				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
				{
					//�P�f�B���N�g�����f�q�d�o���s�i�������ċA�Ăяo���j
					bRetTmp = DoGrepInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objGrepStr);
					if(bRetTmp == FALSE) //�L�����Z���I�����Ă���
					{
						objFileFind.Close();
						return FALSE;
					}
				}
			}
			//�Ō�̂P��
			if( (objFileFind.IsDots()==FALSE) && (objFileFind.IsDirectory()!=FALSE) )
			{
				//�P�f�B���N�g�����f�q�d�o���s�i�������ċA�Ăяo���j
				bRetTmp = DoGrepInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objGrepStr);
				if(bRetTmp == FALSE) //�L�����Z���I�����Ă���
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�t�@�C�����f�q�d�o���s����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoGrepInOneFile			// TRUE : ����I���AFALSE : �L�����Z���I��
	(	CString		objFileName,			// �f�q�d�o���s���t�@�C��
		CString		objFilePath,			// �f�q�d�o���s���t�@�C���p�X
		CString		objGrepStr				// �f�q�d�o������
	)
{
	char*   pPtr=NULL;
	int		eflags=0, start=0, end=0;
	long	lMatchLength=0;
	long	lCRCount=0;
	long	lLFCount=0;
	long	lOffset=0;
	BOOL	bRetStatus = TRUE;
	CFile	objFile;
	UINT	unCodeKind=0;
	CCodeConvManager	objCodeConvManager;

	//�t�@�C���̕����R�[�h���`�F�b�N
	unCodeKind = objCodeConvManager.DetectCode((LPCTSTR)objFilePath);
	if(unCodeKind == INPUT_UNKNOWN || unCodeKind == INPUT_BINARY)
		return TRUE;

	//�f�q�d�o�󋵃_�C�A���O�փt�H���_�ʒm
	if( !IsGrepCanceled() )
		m_pobjGrepStatusDlg->SetFile(objFileName);

	//�t�@�C���I�[�v��
	if( !objFile.Open( objFilePath, CFile::modeRead, NULL ) )
		return bRetStatus;
	CArchive ar( &objFile, CArchive::load );

	//�����Ώە����񂪑S�ĉp�������琬�邩�`�F�b�N
	BOOL bAllAlphaNum = TRUE;
	for(long lCnt=0; lCnt<objGrepStr.GetLength(); lCnt++)
	{
		if( IsDBCSLeadByteEx(0,objGrepStr.GetAt(lCnt)) )
		{
			bAllAlphaNum = FALSE;
			break;
		}
		else if( (!isAlnum(objGrepStr.GetAt(lCnt))) &&
				 (objGrepStr.GetAt(lCnt) != '_')
		)
		{
			bAllAlphaNum = FALSE;
			break;
		}
	}

	m_pApp->PrepareMyReadString();
	for(long lLineCnt=0; ; lLineCnt++)
	{
		//�E�C���h�E�Y���b�Z�[�W����
		m_pApp->MessagePump();
		//�L�����Z���`�F�b�N
		if( IsGrepCanceled() )
		{
			bRetStatus = FALSE;
			break;
		}

		//�P�s�Âǂݍ���
		CString objStr=_T("");
		if( m_pApp->MyReadString(ar,objStr,&lCRCount,&lLFCount) == 0 )
		{
			if( objStr.IsEmpty() )
				break;
		}

		//�����񌟍�
		CLine	objLine((const unsigned char*)(LPCTSTR)objStr);
		if(m_bIsUseRegularExpressionGrep) //���K�\�����g���ꍇ
		{
			start = end = 0;
			if(m_pApp->m_lpfnRegFunc2!=NULL)
			{
				//myRegexec
				if(m_pApp->m_lpfnRegFunc2((const char*)objLine.String(),eflags,&start,&end)==0)//match!
					lOffset = start;
				else
					lOffset = -1;
			}
			else
				lOffset=-1;
		}
		else
		{
			lOffset = objLine.DoFind( 0,
									  objLine.GetLength(),
									  objGrepStr,
									  bAllAlphaNum,
									  m_bIsMatchCaseGrep,
									  m_bIsMatchWholeWordGrep,
									  FALSE,
									  &lMatchLength);
		}

		if(lOffset != -1) //�����I�I
		{
			CLine	objGrepResult;

			//�����t�@�C���p�X
			objGrepResult.Set((const unsigned char*)(LPCTSTR)objFilePath);

			//�����s�ԍ�
			char	szLine[32];
			sprintf(szLine,"(%d) : ",lLineCnt+1);
			objGrepResult.Catenate((const unsigned char*)szLine);

			//�����s�̓��e
			objGrepResult.Catenate(objLine.String());

			//�f�q�d�o���ʂ��h�L�������g�֓o�^
			if( m_pobjGrepDoc != NULL )
				m_pobjGrepDoc->AppendGrepLine(objGrepResult);

			//�Y�������J�E���g�A�b�v
			if( !IsGrepCanceled() )
				m_pobjGrepStatusDlg->IncMatchCount();
		}
	}

	//�t�@�C���N���[�Y
	ar.Close();
	objFile.Close();

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�q�d�o�󋵃_�C�A���O�\���҂�����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForGrepDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjGrepStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjGrepStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjGrepStatusDlg->m_hWnd);
			if(bRet!=0)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�q�d�o�L�����Z���`�F�b�N����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsGrepCanceled()	//TRUE : �L�����Z���AFALSE : ���L�����Z��
{
	if( m_pobjGrepStatusDlg->IsCanceled() )
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �v���e�N�g�֐��i�����R�[�h�ϊ��֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����R�[�h�ϊ����s����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoCodeConv()
{
	CString objMsg;
	CLine	objConvResult;

	//�����R�[�h�ϊ����s�f�B���N�g�����݃`�F�b�N
	CFileFind		objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolderConv);
	if(!bRet)//�Ȃ��I�I
	{
		if(m_pApp->IsDriveRoot(m_objFolderConv) == FALSE) //���[�g�f�B���N�g���ł��Ȃ�
		{
			objMsg.LoadString(IDS_NOCONVFOLDER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//�����R�[�h�ϊ����ʏo�͗p�l�c�h�q�E�C���h�E�쐬
	m_pobjConvDoc = NULL;
	m_pobjConvFrameWnd = NULL;
	bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjConvDoc,&m_pobjConvFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjConvDoc->SetFontOfCurrentFileExt("");
	m_pobjConvDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//�����݋֎~Ӱ�޽ð���߲ݐݒ�
	CString	objTitle;
	objMsg.LoadString(IDS_CONV_TITLE2);
	m_pobjConvDoc->SetTitle(objTitle);
	m_pobjConvDoc->SetConvMode(TRUE);

	//�����R�[�h�ϊ��󋵃��[�h���X�_�C�A���O�\��
	m_pobjConvStatusDlg = new CCodeConvStatusDlg();
	m_pobjConvStatusDlg->Create(IDD_CONVSTATUS);
	m_pobjConvStatusDlg->SetConvKindStr(m_nConvKind);
	m_pobjConvStatusDlg->SetFileKind(m_objFileKindConv);
	m_pobjConvStatusDlg->SetFolder(m_objFolderConv);
	WaitForConvDlgDisp();

	//�^�C�g���ǉ�
	objMsg.LoadString(IDS_CONV_TITLE2);
	objConvResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjConvDoc->AppendConvLine(objConvResult);

	//�P�f�B���N�g���������R�[�h�ϊ����s
	bRet = DoConvInOneDirectory(m_objFolderConv,m_objFileKindConv,m_nConvKind,m_nInCodeConv);

	//�R�[�h�ϊ��t�@�C�����^�C�g���ǉ��\��
	long lConvFileCount = m_pobjConvStatusDlg->GetConvFileCount();
	char	szBuf[64];
	objMsg.LoadString(IDS_CONV_FILECOUNT);
	sprintf(szBuf, (LPCTSTR)objMsg, lConvFileCount );
	CString objStr = szBuf;
	objTitle = m_pobjConvDoc->GetTitle();
	objTitle += objStr;
	m_pobjConvDoc->SetTitle(objTitle);

	//���L�o��
	objConvResult.Set((const unsigned char*)"");
	m_pobjConvDoc->AppendConvLine(objConvResult);
	objConvResult.Set((const unsigned char*)"");
	m_pobjConvDoc->AppendConvLine(objConvResult);
	objMsg.LoadString(IDS_CONV_MEMO);
	objConvResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjConvDoc->AppendConvLine(objConvResult);
	objConvResult.Set((const unsigned char*)"");
	m_pobjConvDoc->AppendConvLine(objConvResult);

	//�����R�[�h�ϊ��󋵃��[�h���X�_�C�A���O����
	m_pobjConvStatusDlg->DestroyWindow();
	delete m_pobjConvStatusDlg;
	m_pobjConvStatusDlg = NULL;

	m_pobjConvDoc->UpdateAllViews(NULL,NULL,NULL);

	//�����R�[�h�ϊ��I���ʒm
	if( bRet != FALSE )
	{
		if(lConvFileCount>0)
		{
			objMsg.LoadString(IDS_CONVEND);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
		else
		{
			objMsg.LoadString(IDS_CONVZERO);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
	}
	else
	{
		objMsg.LoadString(IDS_CONVCANCEL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�f�B���N�g���������R�[�h�ϊ����s����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoConvInOneDirectory		// TRUE : ����I���AFALSE : �L�����Z���I��
	(	CString		objFolder,				// �����R�[�h�ϊ����s���t�H���_
		CString		objFileKinds,			// �����R�[�h�ϊ��Ώۃt�@�C�����
		int			nConvKind,				// �����R�[�h�ϊ����
		int			nInCode					// ���̓t�@�C���̕����R�[�h������
	)
{
	char		szBuf[512];
	BOOL		bCanceled=FALSE;
	BOOL		bRetStatus = TRUE;
	BOOL		bRet, bRetTmp;
	CString		objFilter;
	CString		objOneKind, objRight, objBuf;
	CFileFind	objFileFind;
	CString		objMsg;

	//�f�B���N�g������ǉ�
	CLine	objConvResult;
	m_pobjConvDoc->AppendConvLine(objConvResult);
	objConvResult.Set((const unsigned char*)"");
	m_pobjConvDoc->AppendConvLine(objConvResult);
	objMsg.LoadString(IDS_CONV_FOLDER_TITLE);
	sprintf(szBuf, (LPCTSTR)objMsg, (LPCTSTR)objFolder);
	objConvResult.Set((const unsigned char*)szBuf);
	m_pobjConvDoc->AppendConvLine(objConvResult);
	objMsg.LoadString(IDS_CONV_LIST_TITLE);
	objConvResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjConvDoc->AppendConvLine(objConvResult);
	objConvResult.Set((const unsigned char*)"-------------------------------------------------------------------------------------------------");
	m_pobjConvDoc->AppendConvLine(objConvResult);


	//�����R�[�h�ϊ��󋵃_�C�A���O�փt�H���_�ʒm
	if( !IsConvCanceled() )
		m_pobjConvStatusDlg->SetFolder(objFolder);

	//////////////////////////////////////////////////
	//	�܂��A�Y������t�@�C���̈ꗗ���쐬����		//
	//////////////////////////////////////////////////

	//�t�@�C�������X�g
	CObList		objFileList;

	objRight = objFileKinds;
	for(;;)
	{
		//�����R�[�h�ϊ��Ώۃt�@�C����ʐ؂���
		int nIndex = objRight.Find(';');
		if(nIndex != -1)
		{
			objOneKind = objRight.Left(nIndex);
			objBuf = objRight.Right( objRight.GetLength() - nIndex - 1 );
			objRight = objBuf;
		}
		else
		{
			objOneKind = objRight;
		}

		//�t�@�C�������L�[�쐬
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s%s",objFolder,objOneKind);
		else
			objFilter.Format("%s\\%s",objFolder,objOneKind);

		//�����R�[�h�ϊ��Ώۃt�@�C������
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
					continue;

				//���������t�@�C���p�X�擾
				CString*	pobjFileStr = new CString;
				(*pobjFileStr) = objFileFind.GetFileName();
				//���X�g�֓o�^
				objFileList.AddTail((CObject*)pobjFileStr);
			}
			if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
			{
				//�Ō�̂P��
				CString*	pobjFileStr = new CString;
				(*pobjFileStr) = objFileFind.GetFileName();
				objFileList.AddTail((CObject*)pobjFileStr);
			}
		}
		objFileFind.Close();

		if( nIndex == -1 ) // �t�@�C����ʎc��Ȃ�
		{
			break;
		}
	}

	//////////////////////////////////////////////////////
	//		�t�@�C�����X�g�����ǂ�Ȃ���R�[�h�ϊ�		//
	//////////////////////////////////////////////////////

	bCanceled=FALSE;
	if(!objFileList.IsEmpty()) //�����t�@�C������
	{
		POSITION	pos = objFileList.GetHeadPosition();
		for( ; pos != 0; ) //���X�g�����A�N�Z�X
		{
			//�E�C���h�E�Y���b�Z�[�W����
			m_pApp->MessagePump();
			//�L�����Z���`�F�b�N
			if( IsConvCanceled() )
				break;

			//�t�@�C�����擾
			CString* pobjFileName = (CString*)objFileList.GetNext(pos);
			if(pobjFileName!=NULL)
			{
				//�t�@�C���p�X�쐬
				CString objFilePath = objFolder + "\\" + (*pobjFileName);
				//�P�t�@�C�������R�[�h�ϊ����s
				bRetTmp = DoConvInOneFile(objFolder,(*pobjFileName),objFilePath,nConvKind,nInCode);
				if( bRetTmp == FALSE ) //�L�����Z���I�����Ă���
				{
					bCanceled=TRUE;
					break;
				}
				else //�����R�[�h�ϊ��t�@�C�����J�E���g�A�b�v
				{
					m_pobjConvStatusDlg->IncConvFileCount();
				}
			}
		}
	}

	//�t�@�C�����X�g�S�폜
	if(!objFileList.IsEmpty())
	{
		POSITION pos;
		for(pos=objFileList.GetHeadPosition(); pos!=0; )
		{
			CString* pobjStr = (CString*)objFileList.GetNext(pos);
			if(pobjStr!=NULL)
				delete pobjStr;
		}
		objFileList.RemoveAll();
	}

	//�E�C���h�E�Y���b�Z�[�W����
	m_pApp->MessagePump();
	//�L�����Z���`�F�b�N
	if(bCanceled)
		return FALSE;
	if( IsConvCanceled() )
		return FALSE;

	if( m_bIsSearchSubFolderConv ) //�T�u�t�H���_����������ꍇ
	{
		//�t�@�C�������L�[�쐬�i���C���h�J�[�h�����j
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s*.*",objFolder);
		else
			objFilter.Format("%s\\*.*",objFolder);

		//�T�u�f�B���N�g������
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//�E�C���h�E�Y���b�Z�[�W����
				m_pApp->MessagePump();
				//�L�����Z���`�F�b�N
				if( IsConvCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
				{
					//�P�f�B���N�g���������R�[�h�ϊ����s�i�������ċA�Ăяo���j
					bRetTmp = DoConvInOneDirectory(objFileFind.GetFilePath(),objFileKinds,nConvKind,nInCode);
					if(bRetTmp == FALSE) //�L�����Z���I�����Ă���
					{
						objFileFind.Close();
						return FALSE;
					}
				}
			}
			//�Ō�̂P��
			if( (objFileFind.IsDots()==FALSE) && (objFileFind.IsDirectory()!=FALSE) )
			{
				//�P�f�B���N�g���������R�[�h�ϊ����s�i�������ċA�Ăяo���j
				bRetTmp = DoConvInOneDirectory(objFileFind.GetFilePath(),objFileKinds,nConvKind,nInCode);
				if(bRetTmp == FALSE) //�L�����Z���I�����Ă���
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�t�@�C�������R�[�h�ϊ����s����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoConvInOneFile			// TRUE : ����I���AFALSE : �L�����Z���I��
	(	CString		objFolder,				// �����R�[�h�ϊ����s���t�H���_
		CString		objInFileName,			// �����R�[�h�ϊ����s���t�@�C��
		CString		objInFilePath,			// �����R�[�h�ϊ����s���t�@�C���p�X
		int			nConvKind,				// �����R�[�h�ϊ����
		int			nInCode					// ���̓t�@�C���̕����R�[�h������
	)
{
	CLine	objConvResult;
	char	szResultMsg[256];
	char	szBuf[512];
	char*	pPtr=NULL;
	BOOL	bRet=TRUE;
	BOOL	bRetStatus=TRUE;
	CFile	objInFile, objOutFile;
	DWORD	dwRet=0;
	INT		nRet=0;
	UINT	unRet=0;
	char	szTempFileName[_MAX_FNAME],szTempPathName[_MAX_PATH+16];
	char	szInFilePath[_MAX_PATH+16],szInFileName[_MAX_PATH],szOutFileName[_MAX_PATH];
	CCodeConvManager	objCodeConvManager;
	CString objMsg;
	UINT nDetectCode=0;

	//////////////////////////////////
	//		���o�̓t�@�C������		//
	//////////////////////////////////

	//�����R�[�h�ϊ��󋵃_�C�A���O�փt�@�C���ʒm
	if( !IsConvCanceled() )
		m_pobjConvStatusDlg->SetFile(objInFileName);

	//���̓t�@�C�����擾
	strcpy(szInFilePath,(LPCTSTR)objInFilePath);
	strcpy(szInFileName,(LPCTSTR)objInFileName);

	//�o�͗p�ꎞ�t�@�C�����擾
	strcpy(szTempPathName,(LPCTSTR)objFolder);
	unRet = ::GetTempFileName( szTempPathName,"Cnv",0,szTempFileName);
	if(unRet==0) //Error
	{
		objMsg.LoadString(IDS_CANTGET_CONVTMPFNAME);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	else
	{
		long lPos=0;
		long lCnt=0;
		for(lCnt=0; lCnt<(signed)strlen(szTempFileName); )
		{
			if(IsDBCSLeadByteEx(0,szTempFileName[lCnt]))
			{
				lCnt+=2;
				continue;
			}
			else
			{
				if(szTempFileName[lCnt]=='\\')
				{
					//'\'�ʒu�ێ�
					lPos = lCnt;
				}
				lCnt++;
			}
		}
		strcpy(szOutFileName,&szTempFileName[lPos+1]);
	}


	//���̓t�@�C���I�[�v��
	if( !objInFile.Open( objInFilePath, CFile::modeRead, NULL ) )
	{
		//�o�͗p�t�@�C���폜
		if( (_access( szTempFileName, 0 )) != -1 )
		{
			::DeleteFile(szTempFileName);
		}
		return FALSE;
	}
	else
	{
		objInFile.Close(); //�����N���[�Y
	}

	//�o�̓t�@�C���I�[�v��
	if( !objOutFile.Open( szTempFileName, (CFile::modeCreate|CFile::modeWrite), NULL ) )
	{
		objMsg.LoadString(IDS_CANTMAKE_CONVTMPFILE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		objInFile.Close();
		return FALSE;
	}
	else
	{
		objOutFile.Close(); //�����N���[�Y
	}

	//////////////////////////////////////
	//		���̓t�@�C���`�F�b�N		//
	//////////////////////////////////////

	//���̓t�@�C�������R�[�h�̔���
	if(nInCode == INCODE_AUTO)
		nDetectCode = objCodeConvManager.DetectCode(szInFilePath);
	else if(nInCode == INCODE_EUC)
		nDetectCode = INPUT_EUC;
	else if(nInCode == INCODE_SJIS)
		nDetectCode = INPUT_SJIS;
	else if(nInCode == INCODE_JIS)
		nDetectCode = INPUT_JIS;
	else
		nDetectCode = objCodeConvManager.DetectCode(szInFilePath);
	
	
	if( nDetectCode == INPUT_BINARY ||		//���̓t�@�C�����o�C�i���t�@�C��
		nDetectCode == INPUT_ASCII  ||		//���̓t�@�C���Ɋ����R�[�h�Ȃ�
		nDetectCode == INPUT_UNKNOWN		//�����R�[�h�s��
	)
	{
		//�ϊ�����
		if( nDetectCode==INPUT_BINARY )
			objMsg.LoadString(IDS_CONV_MSG_BINARY);
		else if( nDetectCode==INPUT_ASCII )
			objMsg.LoadString(IDS_CONV_MSG_ASCII);
		else if( nDetectCode==INPUT_UNKNOWN )
			objMsg.LoadString(IDS_CONV_MSG_UNKNOWN);
		strcpy(szResultMsg, (LPCTSTR)objMsg);

		if(strlen(szInFileName)>30)
			szInFileName[30]=0;
		sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
		objConvResult.Set((const unsigned char*)szBuf);

		//���ʂ��h�L�������g�֓o�^
		m_pobjConvDoc->AppendConvLine(objConvResult);

		//�o�͗p�t�@�C���폜
		if( (_access( szTempFileName, 0 )) != -1 )
		{
			::DeleteFile(szTempFileName);
		}

		//�R�[�h�ϊ��I��
		return TRUE;
	}
	else
	{
		if( (nDetectCode==INPUT_SJIS && nConvKind==CODECONV_TO_SJIS) ||	//���łɕϊ��ς�
			(nDetectCode==INPUT_JIS  && nConvKind==CODECONV_TO_JIS)  ||
			(nDetectCode==INPUT_EUC  && nConvKind==CODECONV_TO_EUC)
		)
		{
			//�ϊ�����
			if( nDetectCode==INPUT_SJIS )
				objMsg.LoadString(IDS_CONV_MSG_SJISEXIST);
			else if( nDetectCode==INPUT_JIS )
				objMsg.LoadString(IDS_CONV_MSG_JISEXIST);
			else if( nDetectCode==INPUT_EUC )
				objMsg.LoadString(IDS_CONV_MSG_EUCEXIST);
			strcpy(szResultMsg, (LPCTSTR)objMsg );

			if(strlen(szInFileName)>30)
				szInFileName[30]=0;
			sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
			objConvResult.Set((const unsigned char*)szBuf);

			//���ʂ��h�L�������g�֓o�^
			m_pobjConvDoc->AppendConvLine(objConvResult);

			//�o�͗p�t�@�C���폜
			if( (_access( szTempFileName, 0 )) != -1 )
			{
				::DeleteFile(szTempFileName);
			}

			//�R�[�h�ϊ��I��
			return TRUE;
		}
	}


	//////////////////////////////////
	//		�����R�[�h�ϊ����s		//
	//////////////////////////////////
	if( nDetectCode==INPUT_SJIS ) //���̓t�@�C���r�i�h�r
	{
		if( nConvKind==CODECONV_TO_EUC ) //SJIS -> EUC
		{
			nRet = objCodeConvManager.ConvertSjisToEuc(szInFilePath,szTempFileName);
			if(nRet==1)
				objMsg.LoadString(IDS_CONV_SJIS2EUC);
			else if(nRet==-1)
				objMsg.LoadString(IDS_INFOPEN_ERR);
			else if(nRet==-2)
				objMsg.LoadString(IDS_OUTFOPEN_ERR);
			strcpy(szResultMsg,(LPCTSTR)objMsg);
		}
		else if( nConvKind==CODECONV_TO_JIS ) //SJIS -> JIS
		{
			nRet = objCodeConvManager.ConvertSjisToJis(szInFilePath,szTempFileName);
			if(nRet==1)
				objMsg.LoadString(IDS_CONV_SJIS2JIS);
			else if(nRet==-1)
				objMsg.LoadString(IDS_INFOPEN_ERR);
			else if(nRet==-2)
				objMsg.LoadString(IDS_OUTFOPEN_ERR);
			strcpy(szResultMsg,(LPCTSTR)objMsg);
		}
	}
	else if( nDetectCode==INPUT_EUC ) //���̓t�@�C���d�t�b
	{
		if( nConvKind==CODECONV_TO_SJIS ) //EUC -> SJIS
		{
			nRet = objCodeConvManager.ConvertEucToSjis(szInFilePath,szTempFileName);
			if(nRet==1)
				objMsg.LoadString(IDS_CONV_EUC2SJIS);
			else if(nRet==-1)
				objMsg.LoadString(IDS_INFOPEN_ERR);
			else if(nRet==-2)
				objMsg.LoadString(IDS_OUTFOPEN_ERR);
			strcpy(szResultMsg,(LPCTSTR)objMsg);
		}
		else if( nConvKind==CODECONV_TO_JIS )	//EUC -> JIS
		{
			nRet = objCodeConvManager.ConvertEucToJis(szInFilePath,szTempFileName);
			if(nRet==1)
				objMsg.LoadString(IDS_CONV_EUC2JIS);
			else if(nRet==-1)
				objMsg.LoadString(IDS_INFOPEN_ERR);
			else if(nRet==-2)
				objMsg.LoadString(IDS_OUTFOPEN_ERR);
			strcpy(szResultMsg,(LPCTSTR)objMsg);
		}
	}
	else if( nDetectCode==INPUT_JIS ) //���̓t�@�C���i�h�r
	{
		if( nConvKind==CODECONV_TO_SJIS )		//JIS -> SJIS
		{
			nRet = objCodeConvManager.ConvertJisToSjis(szInFilePath,szTempFileName);
			if(nRet==1)
				objMsg.LoadString(IDS_CONV_JIS2SJIS);
			else if(nRet==-1)
				objMsg.LoadString(IDS_INFOPEN_ERR);
			else if(nRet==-2)
				objMsg.LoadString(IDS_OUTFOPEN_ERR);
			strcpy(szResultMsg,(LPCTSTR)objMsg);
		}
		else if( nConvKind==CODECONV_TO_EUC ) //JIS -> EUC
		{
			nRet = objCodeConvManager.ConvertJisToEuc(szInFilePath,szTempFileName);
			if(nRet==1)
				objMsg.LoadString(IDS_CONV_JIS2EUC);
			else if(nRet==-1)
				objMsg.LoadString(IDS_INFOPEN_ERR);
			else if(nRet==-2)
				objMsg.LoadString(IDS_OUTFOPEN_ERR);
			strcpy(szResultMsg,(LPCTSTR)objMsg);
		}
	}

	//////////////////////////////////////////
	//		�R�[�h�ϊ����ʂ���ʏo��		//
	//////////////////////////////////////////

	if(m_bIsOutOtherFileConv) //�R�[�h�ϊ����ʂ�ʃt�@�C���o�͂���
	{
		//�ϊ�����
		if(strlen(szInFileName)>30)
			szInFileName[30]=0;
		sprintf(szBuf,"  %-30s %-15s %s", szInFileName,szOutFileName,szResultMsg);
		objConvResult.Set((const unsigned char*)szBuf);
		//���ʂ��h�L�������g�֓o�^
		m_pobjConvDoc->AppendConvLine(objConvResult);
	}
	else //�R�[�h�ϊ����ʂ𓯈�t�@�C���֏o�͂���
	{
		//���̓t�@�C���폜
		bRet = ::DeleteFile(szInFilePath);
		if(bRet!=FALSE) //���̓t�@�C���폜����
		{
			//�o�̓t�@�C��������̓t�@�C�����֕ύX
			nRet = rename(szTempFileName,szInFilePath);
			if(nRet==0) //OK
			{
				//�ϊ�����
				if(strlen(szInFileName)>30)
					szInFileName[30]=0;
				sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"same to left",szResultMsg);
				objConvResult.Set((const unsigned char*)szBuf);
				//���ʂ��h�L�������g�֓o�^
				m_pobjConvDoc->AppendConvLine(objConvResult);
				//�R�[�h�ϊ��I��
				return TRUE;
			}
			else //NG
			{
				//�ϊ�����
				objMsg.LoadString(IDS_CONV_OUT_RENAMEERR);
				strcpy(szResultMsg,(LPCTSTR)objMsg);
				if(strlen(szInFileName)>30)
					szInFileName[30]=0;
				sprintf(szBuf,"  %-30s %-15s %s", szInFileName,szTempFileName,szResultMsg);
				objConvResult.Set((const unsigned char*)szBuf);
				//���ʂ��h�L�������g�֓o�^
				m_pobjConvDoc->AppendConvLine(objConvResult);
				//�R�[�h�ϊ��I��
				return TRUE;
			}
		}
		else //���̓t�@�C���폜���s
		{
			//�ϊ�����
			objMsg.LoadString(IDS_CONV_IN_RENAMEERR);
			strcpy(szResultMsg, (LPCTSTR)objMsg);
			if(strlen(szInFileName)>30)
				szInFileName[30]=0;
			sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
			objConvResult.Set((const unsigned char*)szBuf);
			//���ʂ��h�L�������g�֓o�^
			m_pobjConvDoc->AppendConvLine(objConvResult);
			//�o�͗p�t�@�C���폜
			if( (_access( szTempFileName, 0 )) != -1 )
			{
				::DeleteFile(szTempFileName);
			}
			//�R�[�h�ϊ��I��
			return TRUE;
		}
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����R�[�h�ϊ��󋵃_�C�A���O�\���҂�����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForConvDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjConvStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjConvStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjConvStatusDlg->m_hWnd);
			if(bRet!=NULL)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����R�[�h�ϊ��L�����Z���`�F�b�N����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsConvCanceled()	//TRUE : �L�����Z���AFALSE : ���L�����Z��
{
	if( m_pobjConvStatusDlg->IsCanceled() )
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �v���e�N�g�֐��i���s�R�[�h�ϊ��֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s�R�[�h�ϊ����s����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoKaigyoCodeConv()
{
	CLine	objConvResult;
	CString objMsg;

	//���s�R�[�h�ϊ����s�f�B���N�g�����݃`�F�b�N
	CFileFind		objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolderKaigyoConv);
	if(!bRet)//�Ȃ��I�I
	{
		if(m_pApp->IsDriveRoot(m_objFolderKaigyoConv) == FALSE) //���[�g�f�B���N�g���ł��Ȃ�
		{
			objMsg.LoadString(IDS_NOKAIGYOCONVFOLDER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//���s�R�[�h�ϊ����ʏo�͗p�l�c�h�q�E�C���h�E�쐬
	m_pobjKaigyoConvDoc = NULL;
	m_pobjKaigyoConvFrameWnd = NULL;
	bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjKaigyoConvDoc,&m_pobjKaigyoConvFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjKaigyoConvDoc->SetFontOfCurrentFileExt("");
	m_pobjKaigyoConvDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//�����݋֎~Ӱ�޽ð���߲ݐݒ�
	CString	objTitle;
	objMsg.LoadString(IDS_KAIGYOCONV_TITLE);
	objTitle.Format(objMsg,m_objKaigyoConvKind);
	m_pobjKaigyoConvDoc->SetTitle(objTitle);
	m_pobjKaigyoConvDoc->SetConvMode(TRUE);

	//���s�R�[�h�ϊ��󋵃��[�h���X�_�C�A���O�\��
	m_pobjKaigyoConvStatusDlg = new CKaigyoConvStatusDlg();
	m_pobjKaigyoConvStatusDlg->Create(IDD_KAIGYOCONVSTATUS);
	m_pobjKaigyoConvStatusDlg->SetConvKindStr(m_objKaigyoConvKind);
	m_pobjKaigyoConvStatusDlg->SetFileKind(m_objFileKindKaigyoConv);
	m_pobjKaigyoConvStatusDlg->SetFolder(m_objFolderKaigyoConv);
	WaitForKaigyoConvDlgDisp();

	//�^�C�g���ǉ�
	objMsg.LoadString(IDS_KAIGYOCONV_TITLE2);
	objConvResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);

	//�P�f�B���N�g�������s�R�[�h�ϊ����s
	bRet = DoKaigyoConvInOneDirectory(m_objFolderKaigyoConv,m_objFileKindKaigyoConv,m_objKaigyoConvKind);

	//�R�[�h�ϊ��t�@�C�����^�C�g���ǉ��\��
	long lConvFileCount = m_pobjKaigyoConvStatusDlg->GetConvFileCount();
	char	szBuf[64];
	objMsg.LoadString(IDS_KAIGYOCONV_FILECOUNT);
	sprintf(szBuf, (LPCTSTR)objMsg, lConvFileCount );
	CString objStr = szBuf;
	objTitle = m_pobjKaigyoConvDoc->GetTitle();
	objTitle += objStr;
	m_pobjKaigyoConvDoc->SetTitle(objTitle);

	//���L�o��
	objConvResult.Set((const unsigned char*)"");
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
	objConvResult.Set((const unsigned char*)"");
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
	objMsg.LoadString(IDS_KAIGYOCONV_MEMO);
	objConvResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
	objConvResult.Set((const unsigned char*)"");
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);

	//���s�R�[�h�ϊ��󋵃��[�h���X�_�C�A���O����
	m_pobjKaigyoConvStatusDlg->DestroyWindow();
	delete m_pobjKaigyoConvStatusDlg;
	m_pobjKaigyoConvStatusDlg = NULL;

	m_pobjKaigyoConvDoc->UpdateAllViews(NULL,NULL,NULL);

	//���s�R�[�h�ϊ��I���ʒm
	if( bRet != FALSE )
	{
		if(lConvFileCount>0)
		{
			objMsg.LoadString(IDS_KAIGYOCONVEND);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
		else
		{
			objMsg.LoadString(IDS_KAIGYOCONVZERO);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
	}
	else
	{
		objMsg.LoadString(IDS_KAIGYOCONVCANCEL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�f�B���N�g�������s�R�[�h�ϊ����s����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoKaigyoConvInOneDirectory	// TRUE : ����I���AFALSE : �L�����Z���I��
	(	CString		objFolder,				// ���s�R�[�h�ϊ����s���t�H���_
		CString		objFileKinds,			// ���s�R�[�h�ϊ��Ώۃt�@�C�����
		CString		objConvKind				// ���s�R�[�h�ϊ����
	)
{
	char		szBuf[512];
	BOOL		bCanceled=FALSE;
	BOOL		bRetStatus = TRUE;
	BOOL		bRet, bRetTmp;
	CString		objFilter;
	CString		objOneKind, objRight, objBuf;
	CFileFind	objFileFind;
	CString		objMsg;

	//�f�B���N�g������ǉ�
	CLine	objConvResult;
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
	objConvResult.Set((const unsigned char*)"");
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
	objMsg.LoadString(IDS_KAIGYOCONV_FOLDER_TITLE);
	sprintf(szBuf, (LPCTSTR)objMsg, (LPCTSTR)objFolder);
	objConvResult.Set((const unsigned char*)szBuf);
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
	objMsg.LoadString(IDS_KAIGYOCONV_LIST_TITLE);
	objConvResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
	objConvResult.Set((const unsigned char*)"-------------------------------------------------------------------------------------------------");
	m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);


	//���s�R�[�h�ϊ��󋵃_�C�A���O�փt�H���_�ʒm
	if( !IsKaigyoConvCanceled() )
		m_pobjKaigyoConvStatusDlg->SetFolder(objFolder);

	//////////////////////////////////////////////////
	//	�܂��A�Y������t�@�C���̈ꗗ���쐬����		//
	//////////////////////////////////////////////////

	//�t�@�C�������X�g
	CObList		objFileList;

	objRight = objFileKinds;
	for(;;)
	{
		//���s�R�[�h�ϊ��Ώۃt�@�C����ʐ؂���
		int nIndex = objRight.Find(';');
		if(nIndex != -1)
		{
			objOneKind = objRight.Left(nIndex);
			objBuf = objRight.Right( objRight.GetLength() - nIndex - 1 );
			objRight = objBuf;
		}
		else
		{
			objOneKind = objRight;
		}

		//�t�@�C�������L�[�쐬
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s%s",objFolder,objOneKind);
		else
			objFilter.Format("%s\\%s",objFolder,objOneKind);

		//���s�R�[�h�ϊ��Ώۃt�@�C������
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
					continue;

				//���������t�@�C���p�X�擾
				CString*	pobjFileStr = new CString;
				(*pobjFileStr) = objFileFind.GetFileName();
				//���X�g�֓o�^
				objFileList.AddTail((CObject*)pobjFileStr);
			}
			if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
			{
				//�Ō�̂P��
				CString*	pobjFileStr = new CString;
				(*pobjFileStr) = objFileFind.GetFileName();
				objFileList.AddTail((CObject*)pobjFileStr);
			}
		}
		objFileFind.Close();

		if( nIndex == -1 ) // �t�@�C����ʎc��Ȃ�
		{
			break;
		}
	}

	//////////////////////////////////////////////////////
	//		�t�@�C�����X�g�����ǂ�Ȃ���R�[�h�ϊ�		//
	//////////////////////////////////////////////////////

	bCanceled=FALSE;
	if(!objFileList.IsEmpty()) //�����t�@�C������
	{
		POSITION	pos = objFileList.GetHeadPosition();
		for( ; pos != 0; ) //���X�g�����A�N�Z�X
		{
			//�E�C���h�E�Y���b�Z�[�W����
			m_pApp->MessagePump();
			//�L�����Z���`�F�b�N
			if( IsKaigyoConvCanceled() )
				break;

			//�t�@�C�����擾
			CString* pobjFileName = (CString*)objFileList.GetNext(pos);
			if(pobjFileName!=NULL)
			{
				//�t�@�C���p�X�쐬
				CString objFilePath = objFolder + "\\" + (*pobjFileName);
				//�P�t�@�C�����s�R�[�h�ϊ����s
				bRetTmp = DoKaigyoConvInOneFile(objFolder,(*pobjFileName),objFilePath,objConvKind);
				if( bRetTmp == FALSE ) //�L�����Z���I�����Ă���
				{
					bCanceled=TRUE;
					break;
				}
				else //���s�R�[�h�ϊ��t�@�C�����J�E���g�A�b�v
				{
					m_pobjKaigyoConvStatusDlg->IncConvFileCount();
				}
			}
		}
	}

	//�t�@�C�����X�g�S�폜
	if(!objFileList.IsEmpty())
	{
		POSITION pos;
		for(pos=objFileList.GetHeadPosition(); pos!=0; )
		{
			CString* pobjStr = (CString*)objFileList.GetNext(pos);
			if(pobjStr!=NULL)
				delete pobjStr;
		}
		objFileList.RemoveAll();
	}

	//�E�C���h�E�Y���b�Z�[�W����
	m_pApp->MessagePump();
	//�L�����Z���`�F�b�N
	if(bCanceled)
		return FALSE;
	if( IsKaigyoConvCanceled() )
		return FALSE;

	if( m_bIsSearchSubFolderKaigyoConv ) //�T�u�t�H���_����������ꍇ
	{
		//�t�@�C�������L�[�쐬�i���C���h�J�[�h�����j
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s*.*",objFolder);
		else
			objFilter.Format("%s\\*.*",objFolder);

		//�T�u�f�B���N�g������
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//�E�C���h�E�Y���b�Z�[�W����
				m_pApp->MessagePump();
				//�L�����Z���`�F�b�N
				if( IsKaigyoConvCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
				{
					//�P�f�B���N�g�������s�R�[�h�ϊ����s�i�������ċA�Ăяo���j
					bRetTmp = DoKaigyoConvInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objConvKind);
					if(bRetTmp == FALSE) //�L�����Z���I�����Ă���
					{
						objFileFind.Close();
						return FALSE;
					}
				}
			}
			//�Ō�̂P��
			if( (objFileFind.IsDots()==FALSE) && (objFileFind.IsDirectory()!=FALSE) )
			{
				//�P�f�B���N�g�������s�R�[�h�ϊ����s�i�������ċA�Ăяo���j
				bRetTmp = DoKaigyoConvInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objConvKind);
				if(bRetTmp == FALSE) //�L�����Z���I�����Ă���
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�t�@�C�������R�[�h�ϊ����s����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoKaigyoConvInOneFile		// TRUE : ����I���AFALSE : �L�����Z���I��
	(	CString		objFolder,				// ���s�R�[�h�ϊ����s���t�H���_
		CString		objInFileName,			// ���s�R�[�h�ϊ����s���t�@�C��
		CString		objInFilePath,			// ���s�R�[�h�ϊ����s���t�@�C���p�X
		CString		objConvKind				// ���s�R�[�h�ϊ����
	)
{
	CLine	objConvResult;
	char	szResultMsg[256];
	char	szBuf[512];
	char	buf[16], buf2[16];
	char*	pPtr=NULL;
	BOOL	bRet=TRUE;
	CFile	objInFile, objOutFile;
	DWORD	dwRet=0;
	INT		nRet=0;
	UINT	unRet=0;
	char	szTempFileName[_MAX_FNAME],szTempPathName[_MAX_PATH+16];
	char	szInFilePath[_MAX_PATH+16],szInFileName[_MAX_PATH],szOutFileName[_MAX_PATH];
	CCodeConvManager	objCodeConvManager;
	BYTE	byBuf=0;
	long	lCRCount=0,lLFCount=0;
	CString objMsg;

	//////////////////////////////////
	//		���o�̓t�@�C������		//
	//////////////////////////////////

	//���s�R�[�h�ϊ��󋵃_�C�A���O�փt�@�C���ʒm
	if( !IsKaigyoConvCanceled() )
		m_pobjKaigyoConvStatusDlg->SetFile(objInFileName);

	//���̓t�@�C�����擾
	strcpy(szInFilePath,(LPCTSTR)objInFilePath);
	strcpy(szInFileName,(LPCTSTR)objInFileName);

	//�o�͗p�ꎞ�t�@�C�����擾
	strcpy(szTempPathName,(LPCTSTR)objFolder);
	unRet = ::GetTempFileName( szTempPathName,"Cnv",0,szTempFileName);
	if(unRet==0) //Error
	{
		objMsg.LoadString(IDS_CANTGET_KAIGYOCONVTMPFNAME);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	else
	{
		long lPos=0;
		long lCnt=0;
		for(lCnt=0; lCnt<(signed)strlen(szTempFileName); )
		{
			if(IsDBCSLeadByteEx(0,szTempFileName[lCnt]))
			{
				lCnt+=2;
				continue;
			}
			else
			{
				if(szTempFileName[lCnt]=='\\')
				{
					//'\'�ʒu�ێ�
					lPos = lCnt;
				}
				lCnt++;
			}
		}
		strcpy(szOutFileName,&szTempFileName[lPos+1]);
	}


	//���̓t�@�C���I�[�v��
	if( !objInFile.Open( objInFilePath, CFile::modeRead, NULL ) )
	{
		//�o�͗p�t�@�C���폜
		if( (_access( szTempFileName, 0 )) != -1 )
		{
			::DeleteFile(szTempFileName);
		}
		return FALSE;
	}
	else
	{
		objInFile.Close();
	}

	//�o�̓t�@�C���I�[�v��
	if( !objOutFile.Open( szTempFileName, (CFile::modeCreate|CFile::modeWrite), NULL ) )
	{
		objMsg.LoadString(IDS_CANTMAKE_KAIGYOCONVTMPFILE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		objInFile.Close();
		return FALSE;
	}
	else
	{
		objOutFile.Close();
	}

	//////////////////////////////////////
	//		���̓t�@�C���`�F�b�N		//
	//////////////////////////////////////
	//���̓t�@�C�������R�[�h�̔���
	UINT nDetectCode = objCodeConvManager.DetectCode(szInFilePath);
	if( nDetectCode == INPUT_BINARY ||		//���̓t�@�C�����o�C�i���t�@�C��
		nDetectCode == INPUT_UNKNOWN		//�����R�[�h�s��
	)
	{
		//�ϊ�����
		if( nDetectCode==INPUT_BINARY )
			objMsg.LoadString(IDS_KAIGYOCONV_MSG_BINARY);
		else if( nDetectCode==INPUT_UNKNOWN )
			objMsg.LoadString(IDS_KAIGYOCONV_MSG_UNKNOWN);
		strcpy(szResultMsg,(LPCTSTR)objMsg);

		if(strlen(szInFileName)>30)
			szInFileName[30]=0;
		sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
		objConvResult.Set((const unsigned char*)szBuf);

		//���ʂ��h�L�������g�֓o�^
		m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);

		//�o�͗p�t�@�C���폜
		if( (_access( szTempFileName, 0 )) != -1 )
		{
			::DeleteFile(szTempFileName);
		}

		//�R�[�h�ϊ��I��
		return TRUE;
	}

	//���̓t�@�C�����s�R�[�h�̔���
	nDetectCode = objCodeConvManager.DetectKaigyoCode(szInFilePath);
	if( nDetectCode == KAIGYO_UNKNOWN )	//���s�R�[�h�s��
	{
		//�ϊ�����
		objMsg.LoadString(IDS_KAIGYOCONV_MSG_UNKNOWN);
		strcpy(szResultMsg,(LPCTSTR)objMsg);

		if(strlen(szInFileName)>30)
			szInFileName[30]=0;
		sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
		objConvResult.Set((const unsigned char*)szBuf);

		//���ʂ��h�L�������g�֓o�^
		m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);

		//�o�͗p�t�@�C���폜
		if( (_access( szTempFileName, 0 )) != -1 )
		{
			::DeleteFile(szTempFileName);
		}

		//�R�[�h�ϊ��I��
		return TRUE;
	}
	else
	{
		if( (nDetectCode==KAIGYO_CRLF && objConvKind==KAIGYOCONV_TO_CRLF) ||	//���łɕϊ��ς�
			(nDetectCode==KAIGYO_LF   && objConvKind==KAIGYOCONV_TO_LF)   ||
			(nDetectCode==KAIGYO_CR   && objConvKind==KAIGYOCONV_TO_CR)
		)
		{
			//�ϊ�����
			if( nDetectCode==KAIGYO_CRLF )
				objMsg.LoadString(IDS_KAIGYOCONV_MSG_CRLFEXIST);
			else if( nDetectCode==KAIGYO_LF )
				objMsg.LoadString(IDS_KAIGYOCONV_MSG_LFEXIST);
			else if( nDetectCode==KAIGYO_CR )
				objMsg.LoadString(IDS_KAIGYOCONV_MSG_CREXIST);
			strcpy(szResultMsg,(LPCTSTR)objMsg);

			if(strlen(szInFileName)>30)
				szInFileName[30]=0;
			sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
			objConvResult.Set((const unsigned char*)szBuf);

			//���ʂ��h�L�������g�֓o�^
			m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);

			//�o�͗p�t�@�C���폜
			if( (_access( szTempFileName, 0 )) != -1 )
			{
				::DeleteFile(szTempFileName);
			}

			//�R�[�h�ϊ��I��
			return TRUE;
		}
	}


	//////////////////////////////////
	//		���s�R�[�h�ϊ����s		//
	//////////////////////////////////

	//���̓t�@�C���I�[�v��
	objInFile.Open( objInFilePath, CFile::modeRead, NULL );
	CArchive	arIn( &objInFile, CArchive::load, 16, buf );
	//�o�̓t�@�C���I�[�v��
	objOutFile.Open( szTempFileName, (CFile::modeCreate|CFile::modeWrite), NULL );
	CArchive	arOut( &objOutFile, CArchive::store, 16, buf2 );

	byBuf=0;
	lCRCount=lLFCount=0;
	for(;;)
	{
		TRY
		{
			arIn >> (BYTE&)byBuf;
		}
		CATCH_ALL(e) //EOF
		{
			break;
		}
		END_CATCH_ALL

		if(IsDBCSLeadByteEx(0,byBuf)) //�Q�o�C�g����
		{
			if(lCRCount!=0 || lLFCount!=0)
			{
				if(objConvKind==KAIGYOCONV_TO_CRLF)
				{
					arOut << (BYTE)'\r';
					arOut << (BYTE)'\n';
				}
				else if(objConvKind==KAIGYOCONV_TO_LF)
					arOut << (BYTE)'\n';
				else if(objConvKind==KAIGYOCONV_TO_CR)
					arOut << (BYTE)'\r';

				lCRCount=lLFCount=0;
			}

			arOut << (BYTE)byBuf;
			arIn  >> (BYTE&)byBuf;
			arOut << (BYTE)byBuf;
		}
		else
		{
			if(byBuf=='\r')
			{
				if( (nDetectCode==KAIGYO_CRLF && lCRCount==1 && lLFCount==1) ||
					(nDetectCode==KAIGYO_CR   && lCRCount==1)
				)
				{
					if(objConvKind==KAIGYOCONV_TO_CRLF)
					{
						arOut << (BYTE)'\r';
						arOut << (BYTE)'\n';
					}
					else if(objConvKind==KAIGYOCONV_TO_LF)
						arOut << (BYTE)'\n';
					else if(objConvKind==KAIGYOCONV_TO_CR)
						arOut << (BYTE)'\r';

					lCRCount=lLFCount=0;
				}
				lCRCount++;
			}
			else if(byBuf=='\n')
			{
				if(nDetectCode==KAIGYO_LF && lLFCount==1)
				{
					if(objConvKind==KAIGYOCONV_TO_CRLF)
					{
						arOut << (BYTE)'\r';
						arOut << (BYTE)'\n';
					}
					else if(objConvKind==KAIGYOCONV_TO_LF)
						arOut << (BYTE)'\n';
					else if(objConvKind==KAIGYOCONV_TO_CR)
						arOut << (BYTE)'\r';

					lCRCount=lLFCount=0;
				}
				lLFCount++;
			}
			else
			{
				if(lCRCount!=0 || lLFCount!=0)
				{
					if(objConvKind==KAIGYOCONV_TO_CRLF)
					{
						arOut << (BYTE)'\r';
						arOut << (BYTE)'\n';
					}
					else if(objConvKind==KAIGYOCONV_TO_LF)
						arOut << (BYTE)'\n';
					else if(objConvKind==KAIGYOCONV_TO_CR)
						arOut << (BYTE)'\r';

					lCRCount=lLFCount=0;
				}

				arOut << (BYTE)byBuf;
			}
		}
	}

	if(lCRCount!=0 || lLFCount!=0)
	{
		if(objConvKind==KAIGYOCONV_TO_CRLF)
		{
			arOut << (BYTE)'\r';
			arOut << (BYTE)'\n';
		}
		else if(objConvKind==KAIGYOCONV_TO_LF)
			arOut << (BYTE)'\n';
		else if(objConvKind==KAIGYOCONV_TO_CR)
			arOut << (BYTE)'\r';

		lCRCount=lLFCount=0;
	}

	//�N���[�Y
	arIn.Close();
	objInFile.Close();
	arOut.Close();
	objOutFile.Close();

	//�ϊ�����
	memset(szResultMsg,0,sizeof(szResultMsg));
	if(nDetectCode==KAIGYO_CRLF)
	{
		if(objConvKind==KAIGYOCONV_TO_LF)
			objMsg.LoadString(IDS_KAIGYOCONV_CRLF2LF);
		else if(objConvKind==KAIGYOCONV_TO_CR)
			objMsg.LoadString(IDS_KAIGYOCONV_CRLF2CR);
		strcpy(szResultMsg,(LPCTSTR)objMsg);
	}
	else if(nDetectCode==KAIGYO_LF)
	{
		if(objConvKind==KAIGYOCONV_TO_CRLF)
			objMsg.LoadString(IDS_KAIGYOCONV_LF2CRLF);
		else if(objConvKind==KAIGYOCONV_TO_CR)
			objMsg.LoadString(IDS_KAIGYOCONV_LF2CR);
		strcpy(szResultMsg,(LPCTSTR)objMsg);
	}
	else if(nDetectCode==KAIGYO_CR)
	{
		if(objConvKind==KAIGYOCONV_TO_CRLF)
			objMsg.LoadString(IDS_KAIGYOCONV_CR2CRLF);
		else if(objConvKind==KAIGYOCONV_TO_LF)
			objMsg.LoadString(IDS_KAIGYOCONV_CR2LF);
		strcpy(szResultMsg,(LPCTSTR)objMsg);
	}

	//////////////////////////////////////////
	//		�R�[�h�ϊ����ʂ���ʏo��		//
	//////////////////////////////////////////

	if(m_bIsOutOtherFileKaigyoConv) //�R�[�h�ϊ����ʂ�ʃt�@�C���o�͂���
	{
		//�ϊ�����
		if(strlen(szInFileName)>30)
			szInFileName[30]=0;
		sprintf(szBuf,"  %-30s %-15s %s", szInFileName,szOutFileName,szResultMsg);
		objConvResult.Set((const unsigned char*)szBuf);
		//���ʂ��h�L�������g�֓o�^
		m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
	}
	else //�R�[�h�ϊ����ʂ𓯈�t�@�C���֏o�͂���
	{
		//���̓t�@�C���폜
		bRet = ::DeleteFile(szInFilePath);
		if(bRet!=FALSE) //���̓t�@�C���폜����
		{
			//�o�̓t�@�C��������̓t�@�C�����֕ύX
			nRet = rename(szTempFileName,szInFilePath);
			if(nRet==0) //OK
			{
				//�ϊ�����
				if(strlen(szInFileName)>30)
					szInFileName[30]=0;
				sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"same to left",szResultMsg);
				objConvResult.Set((const unsigned char*)szBuf);
				//���ʂ��h�L�������g�֓o�^
				m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
				//�R�[�h�ϊ��I��
				return TRUE;
			}
			else //NG
			{
				//�ϊ�����
				objMsg.LoadString(IDS_KAIGYOCONV_OUT_RENAMEERR);
				strcpy(szResultMsg, (LPCTSTR)objMsg);
				if(strlen(szInFileName)>30)
					szInFileName[30]=0;
				sprintf(szBuf,"  %-30s %-15s %s", szInFileName,szTempFileName,szResultMsg);
				objConvResult.Set((const unsigned char*)szBuf);
				//���ʂ��h�L�������g�֓o�^
				m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
				//�R�[�h�ϊ��I��
				return TRUE;
			}
		}
		else //���̓t�@�C���폜���s
		{
			//�ϊ�����
			objMsg.LoadString(IDS_KAIGYOCONV_IN_RENAMEERR);
			strcpy(szResultMsg, (LPCTSTR)objMsg );
			if(strlen(szInFileName)>30)
				szInFileName[30]=0;
			sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
			objConvResult.Set((const unsigned char*)szBuf);
			//���ʂ��h�L�������g�֓o�^
			m_pobjKaigyoConvDoc->AppendConvLine(objConvResult);
			//�o�͗p�t�@�C���폜
			if( (_access( szTempFileName, 0 )) != -1 )
			{
				::DeleteFile(szTempFileName);
			}
			//�R�[�h�ϊ��I��
			return TRUE;
		}
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s�R�[�h�ϊ��󋵃_�C�A���O�\���҂�����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForKaigyoConvDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjKaigyoConvStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjKaigyoConvStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjKaigyoConvStatusDlg->m_hWnd);
			if(bRet!=NULL)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s�R�[�h�ϊ��L�����Z���`�F�b�N����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsKaigyoConvCanceled()	//TRUE : �L�����Z���AFALSE : ���L�����Z��
{
	if( m_pobjKaigyoConvStatusDlg->IsCanceled() )
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �v���e�N�g�֐��i���[�h�J�E���g�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[�h�J�E���g���s����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoWordCount()
{
	char	szBuf[256];
	CLine	objWCountResult;
	CString objMsg;

	//���[�h�J�E���g���s�f�B���N�g�����݃`�F�b�N
	CFileFind		objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolderWCount);
	if(!bRet)//�Ȃ��I�I
	{
		if(m_pApp->IsDriveRoot(m_objFolderWCount) == FALSE)//���[�g�f�B���N�g���ł��Ȃ�
		{
			objMsg.LoadString(IDS_NOWCOUNTFOLDER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//���[�h�J�E���g���ʏo�͗p�l�c�h�q�E�C���h�E�쐬
	m_pobjWCountDoc = NULL;
	m_pobjWCountFrameWnd = NULL;
	bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjWCountDoc,&m_pobjWCountFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjWCountDoc->SetFontOfCurrentFileExt("");
	m_pobjWCountDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//�����݋֎~���[�h�X�e�[�^�X�y�C���ݒ�
	CString	objTitle;
	objTitle.LoadString(IDS_WCOUNT_TITLE);
	m_pobjWCountDoc->SetTitle(objTitle);
	m_pobjWCountDoc->SetWCountMode(TRUE);

	//���[�h�J�E���g�󋵃��[�h���X�_�C�A���O�\��
	m_pobjWCntStatusDlg = new CWCntStatusDlg();
	m_pobjWCntStatusDlg->Create(IDD_WCNTSTATUS);
	m_pobjWCntStatusDlg->SetFileKind(m_objFileKindWCount);
	m_pobjWCntStatusDlg->SetFolder(m_objFolderWCount);
	WaitForWCntDlgDisp();

	//�J�E���^�N���A
	m_lByteCountSum = 0;				// �o�C�g�����v
	m_lWordCountSum = 0;				// ���[�h�����v
	m_lLineCountSum = 0;				// �s�����v
	m_lEmptyLineCountSum = 0;		// ��s�����v
	m_lCPPCommentLineCountSum = 0;	// �b�E�b�{�{�R�����g�s�����v
	m_lWCountFileCnt = 0;			// ���[�h�J�E���g�Ώۃt�@�C����

	//�^�C�g���ǉ�
	objMsg.LoadString(IDS_WCOUNT_TITLE);
	objWCountResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);

	//�P�f�B���N�g�������[�h�J�E���g���s
	bRet = DoWCountInOneDirectory(m_objFolderWCount,m_objFileKindWCount);

	//���[�h�J�E���g�Ώۃt�@�C�����^�C�g���ǉ��\��
	objMsg.LoadString(IDS_WCOUNT_FILECOUNT);
	sprintf(szBuf, (LPCTSTR)objMsg, m_lWCountFileCnt );
	CString objStr = szBuf;
	objTitle = m_pobjWCountDoc->GetTitle();
	objTitle += objStr;
	m_pobjWCountDoc->SetTitle(objTitle);

	//�o�C�g���A���[�h���A�s���A��s���A�b�E�b�{�{�R�����g�s�݌v��ǉ�
	objWCountResult.Set((const unsigned char*)"");
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objWCountResult.Set((const unsigned char*)"");
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objWCountResult.Set((const unsigned char*)"-----------------------------------------------------------------------------------------------");
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objMsg.LoadString(IDS_WCOUNT_LIST_SUM);
	sprintf(szBuf,(LPCTSTR)objMsg,m_lByteCountSum,m_lWordCountSum,m_lLineCountSum,m_lEmptyLineCountSum,m_lCPPCommentLineCountSum);
	objWCountResult.Set((const unsigned char*)szBuf);
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objWCountResult.Set((const unsigned char*)"");
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objMsg.LoadString(IDS_WCOUNT_MEMO);
	objWCountResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objMsg.LoadString(IDS_WCOUNT_MEMO2);
	objWCountResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objMsg.LoadString(IDS_WCOUNT_MEMO3);
	objWCountResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objMsg.LoadString(IDS_WCOUNT_MEMO4);
	objWCountResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objWCountResult.Set((const unsigned char*)"");
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objWCountResult.Set((const unsigned char*)"");
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);

	//���[�h�J�E���g�󋵃��[�h���X�_�C�A���O����
	m_pobjWCntStatusDlg->DestroyWindow();
	delete m_pobjWCntStatusDlg;
	m_pobjWCntStatusDlg = NULL;

	//���ʕ\��
	m_pobjWCountDoc->UpdateAllViews(NULL,NULL,NULL);

	//���[�h�J�E���g�I���ʒm
	if( bRet != FALSE )
	{
		if(m_lWCountFileCnt==0)
		{
			objMsg.LoadString(IDS_WCOUNTZERO);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
		else
		{
			objMsg.LoadString(IDS_WCOUNTEND);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
	}
	else
	{
		objMsg.LoadString(IDS_WCOUNTCANCEL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�f�B���N�g�������[�h�J�E���g���s����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoWCountInOneDirectory		// TRUE : ����I���AFALSE : �L�����Z���I��
	(	CString		objFolder,				// ���[�h�J�E���g���s���t�H���_
		CString		objFileKinds			// ���[�h�J�E���g�Ώۃt�@�C�����
	)
{
	char		szBuf[512];
	BOOL		bRetStatus = TRUE;
	BOOL		bRet, bRetTmp;
	CString		objFilter, objMsg;
	CString		objOneKind, objRight, objBuf;
	CFileFind	objFileFind;

	//���[�h�J�E���g�󋵃_�C�A���O�փt�H���_�ʒm
	if( !IsWCntCanceled() )
		m_pobjWCntStatusDlg->SetFolder(objFolder);

	//�f�B���N�g������ǉ�
	CLine	objWCountResult;
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objWCountResult.Set((const unsigned char*)"");
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objMsg.LoadString(IDS_WCOUNT_FOLDER_TITLE);
	sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)objFolder);
	objWCountResult.Set((const unsigned char*)szBuf);
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objMsg.LoadString(IDS_WCOUNT_LIST_TITLE);
	objWCountResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);
	objWCountResult.Set((const unsigned char*)"-----------------------------------------------------------------------------------------------");
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);


	objRight = objFileKinds;
	for(;;)
	{
		//�E�C���h�E�Y���b�Z�[�W����
		m_pApp->MessagePump();
		//�L�����Z���`�F�b�N
		if( IsWCntCanceled() )
			return FALSE;

		//���[�h�J�E���g�Ώۃt�@�C����ʐ؂���
		int nIndex = objRight.Find(';');
		if(nIndex != -1)
		{
			objOneKind = objRight.Left(nIndex);
			objBuf = objRight.Right( objRight.GetLength() - nIndex - 1 );
			objRight = objBuf;
		}
		else
		{
			objOneKind = objRight;
		}

		//�t�@�C�������L�[�쐬
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s%s",objFolder,objOneKind);
		else
			objFilter.Format("%s\\%s",objFolder,objOneKind);

		//���[�h�J�E���g�Ώۃt�@�C������
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//�E�C���h�E�Y���b�Z�[�W����
				m_pApp->MessagePump();
				//�L�����Z���`�F�b�N
				if( IsWCntCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
					continue;

				//�t�@�C�����\��
				CString objStr1 = objFileFind.GetFileName();
				m_pobjWCntStatusDlg->SetFile(objStr1);

				//�P�t�@�C�������[�h�J�E���g���s
				bRetTmp = DoWCountInOneFile(objFileFind.GetFileName(),objFileFind.GetFilePath());
				if( bRetTmp == FALSE ) //�L�����Z���I�����Ă���
				{
					objFileFind.Close();
					return FALSE;
				}
				else
				{
					//���[�h�J�E���g�Ώۃt�@�C�����J�E���g�A�b�v
					m_lWCountFileCnt++;
				}
			}
			if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
			{
				//�t�@�C�����\��
				CString objStr1 = objFileFind.GetFileName();
				m_pobjWCntStatusDlg->SetFile(objStr1);

				//�Ō�̂P��
				bRetTmp = DoWCountInOneFile(objFileFind.GetFileName(),objFileFind.GetFilePath());
				if( bRetTmp == FALSE ) //�L�����Z���I�����Ă���
				{
					objFileFind.Close();
					return FALSE;
				}
				else
				{
					//���[�h�J�E���g�Ώۃt�@�C�����J�E���g�A�b�v
					m_lWCountFileCnt++;
				}
			}
		}
		objFileFind.Close();

		if( nIndex == -1 ) // �t�@�C����ʎc��Ȃ�
		{
			break;
		}
	}

	//�E�C���h�E�Y���b�Z�[�W����
	m_pApp->MessagePump();
	//�L�����Z���`�F�b�N
	if( IsWCntCanceled() )
		return FALSE;

	if( m_bIsSearchSubFolderWCount ) //�T�u�t�H���_����������ꍇ
	{
		//�t�@�C�������L�[�쐬�i���C���h�J�[�h�����j
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s*.*",objFolder);
		else
			objFilter.Format("%s\\*.*",objFolder);

		//�T�u�f�B���N�g������
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//�E�C���h�E�Y���b�Z�[�W����
				m_pApp->MessagePump();
				//�L�����Z���`�F�b�N
				if( IsWCntCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
				{
					//�P�f�B���N�g�������[�h�J�E���g���s�i�������ċA�Ăяo���j
					bRetTmp = DoWCountInOneDirectory(objFileFind.GetFilePath(),objFileKinds);
					if(bRetTmp == FALSE) //�L�����Z���I�����Ă���
					{
						objFileFind.Close();
						return FALSE;
					}
				}
			}
			//�Ō�̂P��
			if( (objFileFind.IsDots()==FALSE) && (objFileFind.IsDirectory()!=FALSE) )
			{
				//�P�f�B���N�g�������[�h�J�E���g���s�i�������ċA�Ăяo���j
				bRetTmp = DoWCountInOneDirectory(objFileFind.GetFilePath(),objFileKinds);
				if(bRetTmp == FALSE) //�L�����Z���I�����Ă���
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�t�@�C�������[�h�J�E���g���s����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoWCountInOneFile			// TRUE : ����I���AFALSE : �L�����Z���I��
	(	CString		objFileName,			// ���[�h�J�E���g���s���t�@�C��
		CString		objFilePath				// ���[�h�J�E���g���s���t�@�C���p�X
	)
{
	long	lCnt=0;
	long	lCRCount=0;
	long	lLFCount=0;
	long	lByteCount=0;
	long	lWordCount=0;
	long	lLineCount=0;
	long	lEmptyLineCountSum=0;
	long	lCPPCommentLineCountSum=0;
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	BOOL	bRetStatus = TRUE;
	BOOL	bSpaceFlg=FALSE;
	CFile	objFile;
	CCodeConvManager	objCodeConvManager;

	char	szBuf[MAX_BUF];
	char	szFilePath[_MAX_PATH];
	char	szFileName[_MAX_PATH];
	strcpy(szFilePath,(LPCTSTR)objFilePath);
	strcpy(szFileName,(LPCTSTR)objFileName);

	//�o�C�i���t�@�C���`�F�b�N
	UINT nDetectCode = objCodeConvManager.DetectCode(szFilePath);
	if( nDetectCode==INPUT_BINARY )
	{
		CLine	objWCountResult;
		if(strlen(szFileName)>35)
			szFileName[35]=0;
		sprintf(szBuf,"  %-35s        ---        ---        ---        ---        ---  (Binary File)",szFileName);
		objWCountResult.Set((const unsigned char*)szBuf);
		m_pobjWCountDoc->AppendWCountLine(objWCountResult);
		return bRetStatus;
	}

	//�t�@�C���I�[�v��
	if( !objFile.Open( objFilePath, CFile::modeRead, NULL ) )
		return bRetStatus;
	CArchive ar( &objFile, CArchive::load );

	//////////////////////////
	//	���[�h�J�E���g�J�n	//
	//////////////////////////

	int  nNowRet=0;
	int  nBeforeRet=0;
	m_pApp->PrepareMyReadString();
	for(lLineCount=0; ;lLineCount++)//�P�s�Âǂݍ���
	{
		lCRCount=0;
		lLFCount=0;
		CString objStrBuf=_T("");
		nNowRet = m_pApp->MyReadString(ar,objStrBuf,&lCRCount,&lLFCount);
		if( nNowRet == 0 ) //EOF�I��
		{
			if( objStrBuf.IsEmpty() ) //�f�[�^�Ȃ�
			{
				if( nNowRet != nBeforeRet )
					lLineCount++;
				break;
			}
		}

		//MyReadString ���^�[���l�ێ�
		nBeforeRet = nNowRet;

		//�o�C�g���J�E���g
		lByteCount += objStrBuf.GetLength();
		lByteCount += lCRCount;
		lByteCount += lLFCount;

		//���[�h���J�E���g
		bSpaceFlg=FALSE;
		for(lCnt=0; lCnt<objStrBuf.GetLength(); )
		{
			if( IsDBCSLeadByteEx(0,objStrBuf.GetAt(lCnt)) )//�Q�o�C�g�����̑��o�C�g
			{
				bSpaceFlg=FALSE;
				lCnt+=2;
			}
			else //�P�o�C�g����
			{
				if( objStrBuf.GetAt(lCnt)==' ' || objStrBuf.GetAt(lCnt)=='\t' )
				{
					if(bSpaceFlg==FALSE) //�󔒕����V�K�o��
					{
						bSpaceFlg=TRUE;
						lWordCount++;
					}
				}
				else if(lCnt < objStrBuf.GetLength() - 1 && objStrBuf.GetAt(lCnt+1)==0)
				{
					if(bSpaceFlg==FALSE) //�󔒕����V�K�o��
					{
						bSpaceFlg=TRUE;
						lWordCount++;
					}
				}
				else //�󔒕����ȊO
				{
					bSpaceFlg=FALSE;
				}
				lCnt++;
			}
		}

		//�󔒍s���J�E���g
		if( objStrBuf.IsEmpty() )
			lEmptyLineCountSum++;
		else
		{
			BOOL bFindFlg=FALSE;
			for(lCnt=0; lCnt<objStrBuf.GetLength(); lCnt++)
			{
				if( IsDBCSLeadByteEx(0,objStrBuf.GetAt(lCnt)) )
				{
					bFindFlg=TRUE;
					break;
				}
				else
				{
					if(objStrBuf.GetAt(lCnt)==' ' || objStrBuf.GetAt(lCnt)=='\t')
						continue;
					else
					{
						bFindFlg=TRUE;
						break;
					}
				}
			}
			if(bFindFlg==FALSE) //�󔒕����݂̂�����
				lEmptyLineCountSum++;
		}

		//�b�E�b�{�{�R�����g�s���J�E���g
		for(lCnt=0; lCnt<objStrBuf.GetLength(); lCnt++)
		{
			if( IsDBCSLeadByteEx(0,objStrBuf.GetAt(lCnt)) )//�Q�o�C�g�����̑��o�C�g
			{
				break;
			}
			else
			{
				if(objStrBuf.GetAt(lCnt)==' ' || objStrBuf.GetAt(lCnt)=='\t') //�󔒕����̏ꍇ�ǂݔ�΂�
					continue;
				if( objStrBuf.GetAt(lCnt)=='/' )
				{
					if(lCnt < objStrBuf.GetLength()-1 )
					{
						if( objStrBuf.GetAt(lCnt+1)=='/' && 
							!IsDBCSLeadByteEx(0,objStrBuf.GetAt(lCnt+1)) )
						{
							lCPPCommentLineCountSum++;
							break;
						}
						else if( objStrBuf.GetAt(lCnt+1)=='*' && 
								 !IsDBCSLeadByteEx(0,objStrBuf.GetAt(lCnt+1)) )
						{
							lCPPCommentLineCountSum++;
							break;
						}
						else
							break;
					}
					else
						break;
				}
				else
					break;
			}
		}
	}

	//�t�@�C���N���[�Y
	ar.Close();
	objFile.Close();

	//�o�C�g���A���[�h���A�s���A�󔒍s���A�b�E�b�{�{�R�����g�s���J�E���g���ʂ��P�s�\��
	CLine	objWCountResult;
	if(strlen(szFileName)>35)
		szFileName[35]=0;
	sprintf(szBuf,"  %-35s  %9ld  %9ld  %9ld  %9ld  %9ld",
		szFileName,lByteCount,lWordCount,lLineCount,lEmptyLineCountSum,lCPPCommentLineCountSum);
	objWCountResult.Set((const unsigned char*)szBuf);
	m_pobjWCountDoc->AppendWCountLine(objWCountResult);

	//�o�C�g���A���[�h���A�s���A�󔒍s���A�b�E�b�{�{�R�����g�s����݌v
	m_lByteCountSum += lByteCount;
	m_lWordCountSum += lWordCount;
	m_lLineCountSum += lLineCount;
	m_lEmptyLineCountSum += lEmptyLineCountSum;
	m_lCPPCommentLineCountSum += lCPPCommentLineCountSum;

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[�h�J�E���g�󋵃_�C�A���O�\���҂�����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForWCntDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjWCntStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjWCntStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjWCntStatusDlg->m_hWnd);
			if(bRet!=0)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[�h�J�E���g�L�����Z���`�F�b�N����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsWCntCanceled()	//TRUE : �L�����Z���AFALSE : ���L�����Z��
{
	if( m_pobjWCntStatusDlg->IsCanceled() )
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �v���e�N�g�֐��i�t�@�C�������֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���������s����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoFileSearch()
{
	CString objMsg;

	//�t�@�C���������s�f�B���N�g�����݃`�F�b�N
	CFileFind		objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolderFSearch);
	if(!bRet)//�Ȃ��I�I
	{
		if(m_pApp->IsDriveRoot(m_objFolderFSearch) == FALSE) //���[�g�f�B���N�g���ł��Ȃ�
		{
			objMsg.LoadString(IDS_NOFSEARCHFOLDER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//�t�@�C���������ʏo�͗p�l�c�h�q�E�C���h�E�쐬
	m_pobjFSearchDoc = NULL;
	m_pobjFSearchFrameWnd = NULL;
	bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjFSearchDoc,&m_pobjFSearchFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjFSearchDoc->SetFontOfCurrentFileExt("");
	m_pobjFSearchDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);
	CString	objTitle;
	objMsg.LoadString(IDS_FSEARCH_TITLE);
	objTitle.Format( objMsg, m_objFileKindFSearch );
	m_pobjFSearchDoc->SetTitle(objTitle);
	m_pobjFSearchDoc->SetFSearchMode(TRUE);

	//�t�@�C�������󋵃��[�h���X�_�C�A���O�\��
	m_pobjFileSearchStatusDlg = new CFileSearchStatusDlg();
	m_pobjFileSearchStatusDlg->Create(IDD_FILESEARCHSTATUS);
	m_pobjFileSearchStatusDlg->SetFileKind(m_objFileKindFSearch);
	m_pobjFileSearchStatusDlg->SetFolder(m_objFolderFSearch);
	WaitForFSearchDlgDisp();

	//�P�f�B���N�g�����t�@�C���������s
	bRet = DoFSearchInOneDirectory(m_objFolderFSearch,m_objFileKindFSearch);

	//�Y�������^�C�g���ǉ��\��
	long lMatchCount = m_pobjFileSearchStatusDlg->GetMatchCount();
	char	szBuf[32];
	objMsg.LoadString(IDS_FSEARCH_FILECOUNT);
	sprintf(szBuf, (LPCTSTR)objMsg,lMatchCount );
	CString objStr = szBuf;
	objTitle = m_pobjFSearchDoc->GetTitle();
	objTitle += objStr;
	m_pobjFSearchDoc->SetTitle(objTitle);

	//�t�@�C�������󋵃��[�h���X�_�C�A���O����
	m_pobjFileSearchStatusDlg->DestroyWindow();
	delete m_pobjFileSearchStatusDlg;
	m_pobjFileSearchStatusDlg = NULL;

	m_pobjFSearchDoc->UpdateAllViews(NULL,NULL,NULL);

	//�t�@�C�������I���ʒm
	if( bRet != FALSE )
	{
		objMsg.LoadString(IDS_FSEARCHEND);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
	else
	{
		objMsg.LoadString(IDS_FSEARCHCANCEL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�f�B���N�g�����t�@�C���������s����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoFSearchInOneDirectory	// TRUE : ����I���AFALSE : �L�����Z���I��
	(	CString		objFolder,				// �������s���t�H���_
		CString		objFileKinds			// �����Ώۃt�@�C�����
	)
{
	BOOL		bRetStatus = TRUE;
	BOOL		bRet, bRetTmp;
	CString		objFilter;
	CString		objOneKind, objRight, objBuf;
	CFileFind	objFileFind;

	//�t�@�C�������󋵃_�C�A���O�փt�H���_�ʒm
	if( !IsFSearchCanceled() )
		m_pobjFileSearchStatusDlg->SetFolder(objFolder);

	objRight = objFileKinds;
	for(;;)
	{
		//�E�C���h�E�Y���b�Z�[�W����
		m_pApp->MessagePump();
		//�L�����Z���`�F�b�N
		if( IsFSearchCanceled() )
			return FALSE;

		//�����Ώۃt�@�C����ʐ؂���
		int nIndex = objRight.Find(';');
		if(nIndex != -1)
		{
			objOneKind = objRight.Left(nIndex);
			objBuf = objRight.Right( objRight.GetLength() - nIndex - 1 );
			objRight = objBuf;
		}
		else
		{
			objOneKind = objRight;
		}

		//�t�@�C�������L�[�쐬
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s%s",objFolder,objOneKind);
		else
			objFilter.Format("%s\\%s",objFolder,objOneKind);

		//�Ώۃt�@�C������
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//�E�C���h�E�Y���b�Z�[�W����
				m_pApp->MessagePump();
				//�L�����Z���`�F�b�N
				if( IsFSearchCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
					continue;

				//�t�@�C������
				m_pobjFileSearchStatusDlg->IncMatchCount();
				CString objStr1 = objFileFind.GetFileName();
				m_pobjFileSearchStatusDlg->SetFile(objStr1);
				//�\��
				CLine	objFSearchResult;
				CString objStr2 = objFileFind.GetFilePath();
				objFSearchResult.Set((const unsigned char*)(LPCTSTR)objStr2);
				m_pobjFSearchDoc->AppendFSearchLine(objFSearchResult);
			}
			//�Ō�̂P��
			if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
			{
				//�t�@�C������
				m_pobjFileSearchStatusDlg->IncMatchCount();
				CString objStr1 = objFileFind.GetFileName();
				m_pobjFileSearchStatusDlg->SetFile(objStr1);
				//�\��
				CLine	objFSearchResult;
				CString objStr2 = objFileFind.GetFilePath();
				objFSearchResult.Set((const unsigned char*)(LPCTSTR)objStr2);
				m_pobjFSearchDoc->AppendFSearchLine(objFSearchResult);
			}
		}
		objFileFind.Close();

		if( nIndex == -1 ) // �t�@�C����ʎc��Ȃ�
		{
			break;
		}
	}

	//�E�C���h�E�Y���b�Z�[�W����
	m_pApp->MessagePump();
	//�L�����Z���`�F�b�N
	if( IsFSearchCanceled() )
		return FALSE;

	if( m_bIsSearchSubFolderFSearch ) //�T�u�t�H���_����������ꍇ
	{
		//�t�@�C�������L�[�쐬�i���C���h�J�[�h�����j
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s*.*",objFolder);
		else
			objFilter.Format("%s\\*.*",objFolder);

		//�T�u�f�B���N�g������
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//�E�C���h�E�Y���b�Z�[�W����
				m_pApp->MessagePump();
				//�L�����Z���`�F�b�N
				if( IsFSearchCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}
				
				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
				{
					//�P�f�B���N�g�����t�@�C���������s�i�������ċA�Ăяo���j
					bRetTmp = DoFSearchInOneDirectory(objFileFind.GetFilePath(),objFileKinds);
					if(bRetTmp == FALSE) //�L�����Z���I�����Ă���
					{
						objFileFind.Close();
						return FALSE;
					}
				}
			}
			//�Ō�̂P��
			if( (objFileFind.IsDots()==FALSE) && (objFileFind.IsDirectory()!=FALSE) )
			{
				//�P�f�B���N�g�����t�@�C���������s�i�������ċA�Ăяo���j
				bRetTmp = DoFSearchInOneDirectory(objFileFind.GetFilePath(),objFileKinds);
				if(bRetTmp == FALSE) //�L�����Z���I�����Ă���
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C�������󋵃_�C�A���O�\���҂�����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForFSearchDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjFileSearchStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjFileSearchStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjFileSearchStatusDlg->m_hWnd);
			if(bRet!=NULL)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C�������L�����Z���`�F�b�N����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsFSearchCanceled()	//TRUE : �L�����Z���AFALSE : ���L�����Z��
{
	if( m_pobjFileSearchStatusDlg->IsCanceled() )
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �v���e�N�g�֐��i�t�@�C���Í����֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���Í������s����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoFileEncript()
{
	CLine	objEncriptResult;
	CString objMsg;

	//�t�@�C���Í������s�f�B���N�g�����݃`�F�b�N
	CFileFind		objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolderEncript);
	if(!bRet)//�Ȃ��I�I
	{
		if(m_pApp->IsDriveRoot(m_objFolderEncript) == FALSE) //���[�g�f�B���N�g���ł��Ȃ�
		{
			objMsg.LoadString(IDS_NOENCRIPTFOLDER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//�t�@�C���Í������ʏo�͗p�l�c�h�q�E�C���h�E�쐬
	m_pobjEncriptDoc = NULL;
	m_pobjEncriptFrameWnd = NULL;
	bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjEncriptDoc,&m_pobjEncriptFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjEncriptDoc->SetFontOfCurrentFileExt("");
	m_pobjEncriptDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//�����݋֎~Ӱ�޽ð���߲ݐݒ�
	CString	objTitle;
	objTitle.LoadString(IDS_ENCRYPT_TITLE);
	m_pobjEncriptDoc->SetTitle(objTitle);
	m_pobjEncriptDoc->SetEncriptMode(TRUE);

	//�t�@�C���Í����󋵃��[�h���X�_�C�A���O�\��
	m_pobjEncriptStatusDlg = new CEncriptStatusDlg();
	m_pobjEncriptStatusDlg->Create(IDD_ENCRIPTSTATUS);
	m_pobjEncriptStatusDlg->SetFileKind(m_objFileKindEncript);
	m_pobjEncriptStatusDlg->SetFolder(m_objFolderEncript);
	WaitForEncriptDlgDisp();

	//�^�C�g���ǉ�
	objMsg.LoadString(IDS_ENCRYPT_TITLE);
	objEncriptResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);

	//�P�f�B���N�g�����t�@�C���Í������s
	bRet = DoEncriptInOneDirectory(m_objFolderEncript,m_objFileKindEncript,m_objKeyEncript);

	//�Í����Ώۃt�@�C�����^�C�g���ǉ��\��
	long lEncriptFileCount = m_pobjEncriptStatusDlg->GetEncriptFileCount();
	char	szBuf[64];
	objMsg.LoadString(IDS_ENCRYPT_FILECOUNT);
	sprintf(szBuf,(LPCTSTR)objMsg,lEncriptFileCount);
	CString objStr = szBuf;
	objTitle = m_pobjEncriptDoc->GetTitle();
	objTitle += objStr;
	m_pobjEncriptDoc->SetTitle(objTitle);

	//���L�o��
	objEncriptResult.Set((const unsigned char*)"");
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
	objEncriptResult.Set((const unsigned char*)"");
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
	objMsg.LoadString(IDS_ENCRYPT_MEMO);
	objEncriptResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
	objEncriptResult.Set((const unsigned char*)"");
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);

	//�t�@�C���Í����󋵃��[�h���X�_�C�A���O����
	m_pobjEncriptStatusDlg->DestroyWindow();
	delete m_pobjEncriptStatusDlg;
	m_pobjEncriptStatusDlg = NULL;

	m_pobjEncriptDoc->UpdateAllViews(NULL,NULL,NULL);

	//�t�@�C���Í����I���ʒm
	if( bRet != FALSE )
	{
		if(lEncriptFileCount>0)
		{
			objMsg.LoadString(IDS_ENCRIPTEND);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
		else
		{
			objMsg.LoadString(IDS_ENCRIPTZERO);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
	}
	else
	{
		objMsg.LoadString(IDS_ENCRIPTCANCEL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�f�B���N�g�����t�@�C���Í������s����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoEncriptInOneDirectory	// TRUE : ����I���AFALSE : �L�����Z���I��
	(	CString		objFolder,				// �t�@�C���Í������s���t�H���_
		CString		objFileKinds,			// �t�@�C���Í����Ώۃt�@�C�����
		CString		objKey					// �t�@�C���Í����L�[
	)
{
	char		szBuf[512];
	BOOL		bCanceled=FALSE;
	BOOL		bRetStatus = TRUE;
	BOOL		bRet, bRetTmp;
	CString		objFilter, objMsg;
	CString		objOneKind, objRight, objBuf;
	CFileFind	objFileFind;

	//�f�B���N�g������ǉ�
	CLine	objEncriptResult;
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
	objEncriptResult.Set((const unsigned char*)"");
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
	objMsg.LoadString(IDS_ENCRYPT_FOLDER_TITLE);
	sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)objFolder);
	objEncriptResult.Set((const unsigned char*)szBuf);
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
	objMsg.LoadString(IDS_ENCRYPT_LIST_TITLE);
	objEncriptResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
	objEncriptResult.Set((const unsigned char*)"-------------------------------------------------------------------------------------------------");
	m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);


	//�t�@�C���Í����󋵃_�C�A���O�փt�H���_�ʒm
	if( !IsEncriptCanceled() )
		m_pobjEncriptStatusDlg->SetFolder(objFolder);

	//////////////////////////////////////////////////
	//	�܂��A�Y������t�@�C���̈ꗗ���쐬����		//
	//////////////////////////////////////////////////

	//�t�@�C�������X�g
	CObList		objFileList;

	objRight = objFileKinds;
	for(;;)
	{
		//�Í��Ώۃt�@�C����ʐ؂���
		int nIndex = objRight.Find(';');
		if(nIndex != -1)
		{
			objOneKind = objRight.Left(nIndex);
			objBuf = objRight.Right( objRight.GetLength() - nIndex - 1 );
			objRight = objBuf;
		}
		else
		{
			objOneKind = objRight;
		}

		//�t�@�C�������L�[�쐬
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s%s",objFolder,objOneKind);
		else
			objFilter.Format("%s\\%s",objFolder,objOneKind);

		//�Ώۃt�@�C������
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
					continue;

				//���������t�@�C���p�X�擾
				CString*	pobjFileStr = new CString;
				(*pobjFileStr) = objFileFind.GetFileName();
				//���X�g�֓o�^
				objFileList.AddTail((CObject*)pobjFileStr);
			}
			if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
			{
				//�Ō�̂P��
				CString*	pobjFileStr = new CString;
				(*pobjFileStr) = objFileFind.GetFileName();
				objFileList.AddTail((CObject*)pobjFileStr);
			}
		}
		objFileFind.Close();

		if( nIndex == -1 ) // �t�@�C����ʎc��Ȃ�
		{
			break;
		}
	}

	//////////////////////////////////////////////////
	//		�t�@�C�����X�g�����ǂ�Ȃ���Í���		//
	//////////////////////////////////////////////////

	bCanceled=FALSE;
	if(!objFileList.IsEmpty()) //�����t�@�C������
	{
		POSITION	pos = objFileList.GetHeadPosition();
		for( ; pos != 0; ) //���X�g�����A�N�Z�X
		{
			//�E�C���h�E�Y���b�Z�[�W����
			m_pApp->MessagePump();
			//�L�����Z���`�F�b�N
			if( IsEncriptCanceled() )
				break;

			//�t�@�C�����擾
			CString* pobjFileName = (CString*)objFileList.GetNext(pos);
			if(pobjFileName!=NULL)
			{
				//�t�@�C���p�X�쐬
				CString objFilePath = objFolder + "\\" + (*pobjFileName);
				//�P�t�@�C�� �Í������s
				bRetTmp = DoEncriptInOneFile(objFolder,(*pobjFileName),objFilePath,objKey);
				if( bRetTmp == FALSE ) //�L�����Z���I�����Ă���
				{
					bCanceled=TRUE;
					break;
				}
				else //�Í����t�@�C�����J�E���g�A�b�v
				{
					m_pobjEncriptStatusDlg->IncEncriptFileCount();
				}
			}
		}
	}

	//�t�@�C�����X�g�S�폜
	if(!objFileList.IsEmpty())
	{
		POSITION pos;
		for(pos=objFileList.GetHeadPosition(); pos!=0; )
		{
			CString* pobjStr = (CString*)objFileList.GetNext(pos);
			if(pobjStr!=NULL)
				delete pobjStr;
		}
		objFileList.RemoveAll();
	}

	//�E�C���h�E�Y���b�Z�[�W����
	m_pApp->MessagePump();
	//�L�����Z���`�F�b�N
	if(bCanceled)
		return FALSE;
	if( IsEncriptCanceled() )
		return FALSE;

	if( m_bIsSearchSubFolderEncript ) //�T�u�t�H���_����������ꍇ
	{
		//�t�@�C�������L�[�쐬�i���C���h�J�[�h�����j
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s*.*",objFolder);
		else
			objFilter.Format("%s\\*.*",objFolder);

		//�T�u�f�B���N�g������
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//�E�C���h�E�Y���b�Z�[�W����
				m_pApp->MessagePump();
				//�L�����Z���`�F�b�N
				if( IsEncriptCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
				{
					//�P�f�B���N�g�����t�@�C���Í������s�i�������ċA�Ăяo���j
					bRetTmp = DoEncriptInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objKey);
					if(bRetTmp == FALSE) //�L�����Z���I�����Ă���
					{
						objFileFind.Close();
						return FALSE;
					}
				}
			}
			//�Ō�̂P��
			if( (objFileFind.IsDots()==FALSE) && (objFileFind.IsDirectory()!=FALSE) )
			{
				//�P�f�B���N�g�����t�@�C���Í������s�i�������ċA�Ăяo���j
				bRetTmp = DoEncriptInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objKey);
				if(bRetTmp == FALSE) //�L�����Z���I�����Ă���
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�t�@�C���Í������s����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoEncriptInOneFile			// TRUE : ����I���AFALSE : �L�����Z���I��
	(	CString		objFolder,				// �Í������s���t�H���_
		CString		objInFileName,			// �Í������s���t�@�C��
		CString		objInFilePath,			// �Í������s���t�@�C���p�X
		CString		objKey					// �Í����L�[
	)
{
	int		nCnt=0;
	CLine	objEncriptResult;
	char	szResultMsg[256];
	char	szBuf[512];
	char*	pPtr=NULL;
	BOOL	bRet=TRUE;
	BOOL	bRetStatus=TRUE;
	CFile	objInFile, objOutFile;
	DWORD	dwRet=0;
	INT		nRet=0;
	UINT	unRet=0;
	char	szTempFileName[_MAX_FNAME],szTempPathName[_MAX_PATH+16];
	char	szInFilePath[_MAX_PATH+16],szInFileName[_MAX_PATH],szOutFileName[_MAX_PATH];
	char	szKeyString[64];
	CDes	objDes;
	CString objMsg;

	//////////////////////////////
	//		�Í����L�[����		//
	//////////////////////////////

	//�Í����L�[�𕶎��|���o�C�i���֕ϊ�
	memset(szKeyString,0,sizeof(szKeyString));
	for(nCnt=0; nCnt<objKey.GetLength()/2; nCnt++)
	{
		int nHigh=0,nLow=0;
		BYTE byBuf = objKey.GetAt(2*nCnt);
		if( byBuf >= '0' && byBuf <= '9')
			nHigh = byBuf - '0';
		else if( byBuf >= 'a' && byBuf <= 'f' )
			nHigh = byBuf - 'a' + 10;
		else if( byBuf >= 'A' && byBuf <= 'F' )
			nHigh = byBuf - 'A' + 10;

		byBuf = objKey.GetAt(2*nCnt+1);
		if( byBuf >= '0' && byBuf <= '9')
			nLow = byBuf - '0';
		else if( byBuf >= 'a' && byBuf <= 'f' )
			nLow = byBuf - 'a' + 10;
		else if( byBuf >= 'A' && byBuf <= 'F' )
			nLow = byBuf - 'A' + 10;

		szKeyString[nCnt] = nHigh*16 + nLow;
	}

	//////////////////////////////////
	//		���o�̓t�@�C������		//
	//////////////////////////////////

	//�t�@�C���Í����󋵃_�C�A���O�փt�@�C���ʒm
	if( !IsEncriptCanceled() )
		m_pobjEncriptStatusDlg->SetFile(objInFileName);

	//���̓t�@�C�����擾
	strcpy(szInFilePath,(LPCTSTR)objInFilePath);
	strcpy(szInFileName,(LPCTSTR)objInFileName);

	//�o�͗p�ꎞ�t�@�C�����擾
	strcpy(szTempPathName,(LPCTSTR)objFolder);
	unRet = ::GetTempFileName( szTempPathName,"Enc",0,szTempFileName);
	if(unRet==0) //Error
	{
		objMsg.LoadString(IDS_CANTGET_ENCRIPTTMPFNAME);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	else
	{
		long lCnt=0;
		long lPos=0;
		for(lCnt=0; lCnt<(signed)strlen(szTempFileName); )
		{
			if(IsDBCSLeadByteEx(0,szTempFileName[lCnt]))
			{
				lCnt+=2;
				continue;
			}
			else
			{
				if(szTempFileName[lCnt]=='\\')
				{
					//'\'�ʒu�ێ�
					lPos = lCnt;
				}
				lCnt++;
			}
		}
		strcpy(szOutFileName,&szTempFileName[lPos+1]);
	}


	//���̓t�@�C���I�[�v��
	if( !objInFile.Open( objInFilePath, CFile::modeRead, NULL ) ) //Error
	{
		//�o�͗p�t�@�C���폜
		if( (_access( szTempFileName, 0 )) != -1 )
		{
			::DeleteFile(szTempFileName);
		}
		return FALSE;
	}
	else
	{
		objInFile.Close(); //�����N���[�Y
	}

	//�o�̓t�@�C���I�[�v��
	if( !objOutFile.Open( szTempFileName, (CFile::modeCreate|CFile::modeWrite), NULL ) ) //Error
	{
		objMsg.LoadString(IDS_CANTMAKE_ENCRIPTTMPFILE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		objInFile.Close();
		return FALSE;
	}
	else
	{
		objOutFile.Close(); //�����N���[�Y
	}

	//////////////////////////////////
	//		�t�@�C���Í������s		//
	//////////////////////////////////
	int	argc = 0;
	char**	argv=NULL;

	//�������̈�m��
	argc = 6;
	argv = (char**)malloc(sizeof(char*)*6);
	for(nCnt=0; nCnt<6; nCnt++)
		argv[nCnt] = (char*)malloc(sizeof(char)*1024);

	//DES �Í����p�����[�^�ݒ�
	strcpy(argv[0],"des");
	strcpy(argv[1],"-e");
	strcpy(argv[2],"-k");
	memcpy(argv[3],szKeyString,sizeof(szKeyString));
	strcpy(argv[4],szInFilePath);
	strcpy(argv[5],szTempFileName);

	//DES �Í���
	nRet = objDes.main(argc,argv);
	if(nRet==0)
		objMsg.LoadString(IDS_ENCRYPT_RESULTMSG);
	else if(nRet==-1)
		objMsg.LoadString(IDS_ENCRYPT_PARAMERR);
	else if(nRet==-2)
		objMsg.LoadString(IDS_ENCRYPT_FACCESSERR);
	strcpy(szResultMsg,(LPCTSTR)objMsg);

	//�������̈�J��
	for(nCnt=0; nCnt<6; nCnt++)
		free(argv[nCnt]);
	free(argv);

	//////////////////////////////////////////////
	//		�t�@�C���Í������ʂ���ʏo��		//
	//////////////////////////////////////////////

	if(m_bIsOutOtherFileEncript) //�Í������ʂ�ʃt�@�C���o�͂���
	{
		//�ϊ�����
		if(strlen(szInFileName)>30)
			szInFileName[30]=0;
		sprintf(szBuf,"  %-30s %-15s %s", szInFileName,szOutFileName,szResultMsg);
		objEncriptResult.Set((const unsigned char*)szBuf);
		//���ʂ��h�L�������g�֓o�^
		m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
	}
	else //�Í������ʂ𓯈�t�@�C���֏o�͂���
	{
		//���̓t�@�C���폜
		bRet = ::DeleteFile(szInFilePath);
		if(bRet != FALSE) //���̓t�@�C���폜����
		{
			//�o�̓t�@�C��������̓t�@�C�����֕ύX
			nRet = ::rename(szTempFileName,szInFilePath);
			if(nRet==0) //OK
			{
				//�ϊ�����
				if(strlen(szInFileName)>30)
					szInFileName[30]=0;
				sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"same to left",szResultMsg);
				objEncriptResult.Set((const unsigned char*)szBuf);
				//���ʂ��h�L�������g�֓o�^
				m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
				//�t�@�C���Í����I��
				return TRUE;
			}
			else //NG
			{
				//�ϊ�����
				objMsg.LoadString(IDS_ENCRYPT_OUT_RENAMEERR);
				strcpy(szResultMsg,(LPCTSTR)objMsg);
				if(strlen(szInFileName)>30)
					szInFileName[30]=0;
				sprintf(szBuf,"  %-30s %-15s %s", szInFileName,szTempFileName,szResultMsg);
				objEncriptResult.Set((const unsigned char*)szBuf);
				//���ʂ��h�L�������g�֓o�^
				m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
				//�t�@�C���Í����I��
				return TRUE;
			}
		}
		else //���̓t�@�C���폜���s
		{
			//�ϊ�����
			objMsg.LoadString(IDS_ENCRYPT_IN_RENAMEERR);
			strcpy(szResultMsg,(LPCTSTR)objMsg);
			if(strlen(szInFileName)>30)
				szInFileName[30]=0;
			sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
			objEncriptResult.Set((const unsigned char*)szBuf);
			//���ʂ��h�L�������g�֓o�^
			m_pobjEncriptDoc->AppendEncriptLine(objEncriptResult);
			//�o�͗p�t�@�C���폜
			if( (_access( szTempFileName, 0 )) != -1 )
			{
				::DeleteFile(szTempFileName);
			}
			//�t�@�C���Í����I��
			return TRUE;
		}
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���Í����󋵃_�C�A���O�\���҂�����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForEncriptDlgDisp()
{
	BOOL	bRet=FALSE;

	for(;;)
	{
		if( m_pobjEncriptStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjEncriptStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjEncriptStatusDlg->m_hWnd);
			if(bRet!=NULL)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���Í����L�����Z���`�F�b�N����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsEncriptCanceled()	//TRUE : �L�����Z���AFALSE : ���L�����Z��
{
	if( m_pobjEncriptStatusDlg->IsCanceled() )
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �v���e�N�g�֐��i�t�@�C���������֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C�����������s����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoFileDecript()
{
	CLine	objDecriptResult;
	CString objMsg;

	//�t�@�C�����������s�f�B���N�g�����݃`�F�b�N
	CFileFind		objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolderDecript);
	if(!bRet)//�Ȃ��I�I
	{
		if(m_pApp->IsDriveRoot(m_objFolderDecript) == FALSE) //���[�g�f�B���N�g���ł��Ȃ�
		{
			objMsg.LoadString(IDS_NODECRIPTFOLDER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//�t�@�C�����������ʏo�͗p�l�c�h�q�E�C���h�E�쐬
	m_pobjDecriptDoc = NULL;
	m_pobjDecriptFrameWnd = NULL;
	bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjDecriptDoc,&m_pobjDecriptFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjDecriptDoc->SetFontOfCurrentFileExt("");
	m_pobjDecriptDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//�����݋֎~Ӱ�޽ð���߲ݐݒ�
	CString	objTitle;
	objTitle.LoadString(IDS_DECRYPT_TITLE);
	m_pobjDecriptDoc->SetTitle(objTitle);
	m_pobjDecriptDoc->SetDecriptMode(TRUE);

	//�t�@�C���������󋵃��[�h���X�_�C�A���O�\��
	m_pobjDecriptStatusDlg = new CDecriptStatusDlg();
	m_pobjDecriptStatusDlg->Create(IDD_DECRIPTSTATUS);
	m_pobjDecriptStatusDlg->SetFileKind(m_objFileKindDecript);
	m_pobjDecriptStatusDlg->SetFolder(m_objFolderDecript);
	WaitForDecriptDlgDisp();

	//�^�C�g���ǉ�
	objMsg.LoadString(IDS_DECRYPT_TITLE);
	objDecriptResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);

	//�P�f�B���N�g�����t�@�C�����������s
	bRet = DoDecriptInOneDirectory(m_objFolderDecript,m_objFileKindDecript,m_objKeyDecript);

	//�������Ώۃt�@�C�����^�C�g���ǉ��\��
	long lDecriptFileCount = m_pobjDecriptStatusDlg->GetDecriptFileCount();
	char	szBuf[64];
	objMsg.LoadString(IDS_DECRYPT_FILECOUNT);
	sprintf(szBuf,(LPCTSTR)objMsg,lDecriptFileCount);
	CString objStr = szBuf;
	objTitle = m_pobjDecriptDoc->GetTitle();
	objTitle += objStr;
	m_pobjDecriptDoc->SetTitle(objTitle);

	//���L�o��
	objDecriptResult.Set((const unsigned char*)"");
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
	objDecriptResult.Set((const unsigned char*)"");
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
	objMsg.LoadString(IDS_DECRYPT_MEMO);
	objDecriptResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
	objDecriptResult.Set((const unsigned char*)"");
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);

	//�t�@�C���������󋵃��[�h���X�_�C�A���O����
	m_pobjDecriptStatusDlg->DestroyWindow();
	delete m_pobjDecriptStatusDlg;
	m_pobjDecriptStatusDlg = NULL;

	m_pobjDecriptDoc->UpdateAllViews(NULL,NULL,NULL);

	//�t�@�C���������I���ʒm
	if( bRet != FALSE )
	{
		if(lDecriptFileCount>0)
		{
			objMsg.LoadString(IDS_DECRIPTEND);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
		else
		{
			objMsg.LoadString(IDS_DECRIPTZERO);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
	}
	else
	{
		objMsg.LoadString(IDS_DECRIPTCANCEL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�f�B���N�g�����t�@�C�����������s����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoDecriptInOneDirectory	// TRUE : ����I���AFALSE : �L�����Z���I��
	(	CString		objFolder,				// �t�@�C�����������s���t�H���_
		CString		objFileKinds,			// �t�@�C���������Ώۃt�@�C�����
		CString		objKey					// �t�@�C���������L�[
	)
{
	char		szBuf[512];
	BOOL		bCanceled=FALSE;
	BOOL		bRetStatus = TRUE;
	BOOL		bRet, bRetTmp;
	CString		objFilter, objMsg;
	CString		objOneKind, objRight, objBuf;
	CFileFind	objFileFind;

	//�f�B���N�g������ǉ�
	CLine	objDecriptResult;
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
	objDecriptResult.Set((const unsigned char*)"");
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
	objMsg.LoadString(IDS_DECRYPT_FOLDER_TITLE);
	sprintf(szBuf,(LPCTSTR)objMsg, (LPCTSTR)objFolder);
	objDecriptResult.Set((const unsigned char*)szBuf);
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
	objMsg.LoadString(IDS_DECRYPT_LIST_TITLE);
	objDecriptResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
	objDecriptResult.Set((const unsigned char*)"-------------------------------------------------------------------------------------------------");
	m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);


	//�t�@�C���������󋵃_�C�A���O�փt�H���_�ʒm
	if( !IsDecriptCanceled() )
		m_pobjDecriptStatusDlg->SetFolder(objFolder);

	//////////////////////////////////////////////////
	//	�܂��A�Y������t�@�C���̈ꗗ���쐬����		//
	//////////////////////////////////////////////////

	//�t�@�C�������X�g
	CObList		objFileList;

	objRight = objFileKinds;
	for(;;)
	{
		//�Í��Ώۃt�@�C����ʐ؂���
		int nIndex = objRight.Find(';');
		if(nIndex != -1)
		{
			objOneKind = objRight.Left(nIndex);
			objBuf = objRight.Right( objRight.GetLength() - nIndex - 1 );
			objRight = objBuf;
		}
		else
		{
			objOneKind = objRight;
		}

		//�t�@�C�������L�[�쐬
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s%s",objFolder,objOneKind);
		else
			objFilter.Format("%s\\%s",objFolder,objOneKind);

		//�Ώۃt�@�C������
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
					continue;

				//���������t�@�C���p�X�擾
				CString*	pobjFileStr = new CString;
				(*pobjFileStr) = objFileFind.GetFileName();
				//���X�g�֓o�^
				objFileList.AddTail((CObject*)pobjFileStr);
			}
			if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
			{
				//�Ō�̂P��
				CString*	pobjFileStr = new CString;
				(*pobjFileStr) = objFileFind.GetFileName();
				objFileList.AddTail((CObject*)pobjFileStr);
			}
		}
		objFileFind.Close();

		if( nIndex == -1 ) // �t�@�C����ʎc��Ȃ�
		{
			break;
		}
	}

	//////////////////////////////////////////////////
	//		�t�@�C�����X�g�����ǂ�Ȃ��畜����		//
	//////////////////////////////////////////////////

	bCanceled=FALSE;
	if(!objFileList.IsEmpty()) //�����t�@�C������
	{
		POSITION	pos = objFileList.GetHeadPosition();
		for( ; pos != 0; ) //���X�g�����A�N�Z�X
		{
			//�E�C���h�E�Y���b�Z�[�W����
			m_pApp->MessagePump();
			//�L�����Z���`�F�b�N
			if( IsDecriptCanceled() )
				break;

			//�t�@�C�����擾
			CString* pobjFileName = (CString*)objFileList.GetNext(pos);
			if(pobjFileName!=NULL)
			{
				//�t�@�C���p�X�쐬
				CString objFilePath = objFolder + "\\" + (*pobjFileName);
				//�P�t�@�C�� ���������s
				bRetTmp = DoDecriptInOneFile(objFolder,(*pobjFileName),objFilePath,objKey);
				if( bRetTmp == FALSE ) //�L�����Z���I�����Ă���
				{
					bCanceled=TRUE;
					break;
				}
				else //�������t�@�C�����J�E���g�A�b�v
				{
					m_pobjDecriptStatusDlg->IncDecriptFileCount();
				}
			}
		}
	}

	//�t�@�C�����X�g�S�폜
	if(!objFileList.IsEmpty())
	{
		POSITION pos;
		for(pos=objFileList.GetHeadPosition(); pos!=0; )
		{
			CString* pobjStr = (CString*)objFileList.GetNext(pos);
			if(pobjStr!=NULL)
				delete pobjStr;
		}
		objFileList.RemoveAll();
	}

	//�E�C���h�E�Y���b�Z�[�W����
	m_pApp->MessagePump();
	//�L�����Z���`�F�b�N
	if(bCanceled)
		return FALSE;
	if( IsDecriptCanceled() )
		return FALSE;

	if( m_bIsSearchSubFolderDecript ) //�T�u�t�H���_����������ꍇ
	{
		//�t�@�C�������L�[�쐬�i���C���h�J�[�h�����j
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s*.*",objFolder);
		else
			objFilter.Format("%s\\*.*",objFolder);

		//�T�u�f�B���N�g������
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//�E�C���h�E�Y���b�Z�[�W����
				m_pApp->MessagePump();
				//�L�����Z���`�F�b�N
				if( IsDecriptCanceled() )
				{
					objFileFind.Close();
					return FALSE;
				}
				
				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
				{
					//�P�f�B���N�g�����t�@�C�����������s�i�������ċA�Ăяo���j
					bRetTmp = DoDecriptInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objKey);
					if(bRetTmp == FALSE) //�L�����Z���I�����Ă���
					{
						objFileFind.Close();
						return FALSE;
					}
				}
			}
			//�Ō�̂P��
			if( (objFileFind.IsDots()==FALSE) && (objFileFind.IsDirectory()!=FALSE) )
			{
				//�P�f�B���N�g�����t�@�C�����������s�i�������ċA�Ăяo���j
				bRetTmp = DoDecriptInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objKey);
				if(bRetTmp == FALSE) //�L�����Z���I�����Ă���
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�t�@�C�����������s����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoDecriptInOneFile			// TRUE : ����I���AFALSE : �L�����Z���I��
	(	CString		objFolder,				// ���������s���t�H���_
		CString		objInFileName,			// ���������s���t�@�C��
		CString		objInFilePath,			// ���������s���t�@�C���p�X
		CString		objKey					// �������L�[
	)
{
	int		nCnt=0;
	CLine	objDecriptResult;
	char	szResultMsg[256];
	char	szBuf[512];
	char*	pPtr=NULL;
	BOOL	bRet=TRUE;
	BOOL	bRetStatus=TRUE;
	CFile	objInFile, objOutFile;
	DWORD	dwRet=0;
	INT		nRet=0;
	UINT	unRet=0;
	char	szTempFileName[_MAX_FNAME],szTempPathName[_MAX_PATH+16];
	char	szInFilePath[_MAX_PATH+16],szInFileName[_MAX_PATH],szOutFileName[_MAX_PATH];
	char	szKeyString[64];
	CDes	objDes;
	CString objMsg;

	//////////////////////////////
	//		�������L�[����		//
	//////////////////////////////

	//�������L�[�𕶎��|���o�C�i���֕ϊ�
	memset(szKeyString,0,sizeof(szKeyString));
	for(nCnt=0; nCnt<objKey.GetLength()/2; nCnt++)
	{
		int nHigh=0,nLow=0;
		BYTE byBuf = objKey.GetAt(2*nCnt);
		if( byBuf >= '0' && byBuf <= '9')
			nHigh = byBuf - '0';
		else if( byBuf >= 'a' && byBuf <= 'f' )
			nHigh = byBuf - 'a' + 10;
		else if( byBuf >= 'A' && byBuf <= 'F' )
			nHigh = byBuf - 'A' + 10;

		byBuf = objKey.GetAt(2*nCnt+1);
		if( byBuf >= '0' && byBuf <= '9')
			nLow = byBuf - '0';
		else if( byBuf >= 'a' && byBuf <= 'f' )
			nLow = byBuf - 'a' + 10;
		else if( byBuf >= 'A' && byBuf <= 'F' )
			nLow = byBuf - 'A' + 10;

		szKeyString[nCnt] = nHigh*16 + nLow;
	}

	//////////////////////////////////
	//		���o�̓t�@�C������		//
	//////////////////////////////////

	//�t�@�C���Í����󋵃_�C�A���O�փt�@�C���ʒm
	if( !IsDecriptCanceled() )
		m_pobjDecriptStatusDlg->SetFile(objInFileName);

	//���̓t�@�C�����擾
	strcpy(szInFilePath,(LPCTSTR)objInFilePath);
	strcpy(szInFileName,(LPCTSTR)objInFileName);

	//�o�͗p�ꎞ�t�@�C�����擾
	strcpy(szTempPathName,(LPCTSTR)objFolder);
	unRet = ::GetTempFileName( szTempPathName,"Dec",0,szTempFileName);
	if(unRet==0) //Error
	{
		objMsg.LoadString(IDS_CANTGET_DECRIPTTMPFNAME);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return FALSE;
	}
	else
	{
		long lCnt=0;
		long lPos=0;
		for(lCnt=0; lCnt<(signed)strlen(szTempFileName); )
		{
			if(IsDBCSLeadByteEx(0,szTempFileName[lCnt]))
			{
				lCnt+=2;
				continue;
			}
			else
			{
				if(szTempFileName[lCnt]=='\\')
				{
					//'\'�ʒu�ێ�
					lPos = lCnt;
				}
				lCnt++;
			}
		}
		strcpy(szOutFileName,&szTempFileName[lPos+1]);
	}


	//���̓t�@�C���I�[�v��
	if( !objInFile.Open( objInFilePath, CFile::modeRead, NULL ) ) //Error
	{
		//�o�͗p�t�@�C���폜
		if( (_access( szTempFileName, 0 )) != -1 )
		{
			::DeleteFile(szTempFileName);
		}
		return FALSE;
	}
	else
	{
		objInFile.Close(); //�����N���[�Y
	}

	//�o�̓t�@�C���I�[�v��
	if( !objOutFile.Open( szTempFileName, (CFile::modeCreate|CFile::modeWrite), NULL ) ) //Error
	{
		objMsg.LoadString(IDS_CANTMAKE_DECRIPTTMPFILE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		objInFile.Close();
		return FALSE;
	}
	else
	{
		objOutFile.Close(); //�����N���[�Y
	}

	//////////////////////////////////
	//		�t�@�C�����������s		//
	//////////////////////////////////
	int	argc = 0;
	char**	argv=NULL;

	//�������̈�m��
	argc = 6;
	argv = (char**)malloc(sizeof(char*)*6);
	for(nCnt=0; nCnt<6; nCnt++)
		argv[nCnt] = (char*)malloc(sizeof(char)*1024);

	//DES �������p�����[�^�ݒ�
	strcpy(argv[0],"des");
	strcpy(argv[1],"-d");
	strcpy(argv[2],"-k");
	memcpy(argv[3],szKeyString,sizeof(szKeyString));
	strcpy(argv[4],szInFilePath);
	strcpy(argv[5],szTempFileName);

	//DES ������
	nRet = objDes.main(argc,argv);
	if(nRet==0)
		objMsg.LoadString(IDS_DECRYPT_RESULTMSG);
	else if(nRet==-1)
		objMsg.LoadString(IDS_DECRYPT_PARAMERR);
	else if(nRet==-2)
		objMsg.LoadString(IDS_DECRYPT_FACCESSERR);
	else if(nRet==-3)
		objMsg.LoadString(IDS_DECRYPT_FCOLLUPT);
	strcpy(szResultMsg,(LPCTSTR)objMsg);

	//�������̈�J��
	for(nCnt=0; nCnt<6; nCnt++)
		free(argv[nCnt]);
	free(argv);

	//////////////////////////////////////////////
	//		�t�@�C�����������ʂ���ʏo��		//
	//////////////////////////////////////////////

	if(m_bIsOutOtherFileDecript) //���������ʂ�ʃt�@�C���o�͂���
	{
		//�ϊ�����
		if(strlen(szInFileName)>30)
			szInFileName[30]=0;
		sprintf(szBuf,"  %-30s %-15s %s", szInFileName,szOutFileName,szResultMsg);
		objDecriptResult.Set((const unsigned char*)szBuf);
		//���ʂ��h�L�������g�֓o�^
		m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
	}
	else //���������ʂ𓯈�t�@�C���֏o�͂���
	{
		//���̓t�@�C���폜
		bRet = ::DeleteFile(szInFilePath);
		if(bRet != FALSE) //���̓t�@�C���폜����
		{
			//�o�̓t�@�C��������̓t�@�C�����֕ύX
			nRet = ::rename(szTempFileName,szInFilePath);
			if(nRet==0) //OK
			{
				//�ϊ�����
				if(strlen(szInFileName)>30)
					szInFileName[30]=0;
				sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"same to left",szResultMsg);
				objDecriptResult.Set((const unsigned char*)szBuf);
				//���ʂ��h�L�������g�֓o�^
				m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
				//�t�@�C���������I��
				return TRUE;
			}
			else //NG
			{
				//�ϊ�����
				objMsg.LoadString(IDS_DECRYPT_OUT_RENAMEERR);
				strcpy(szResultMsg,(LPCTSTR)objMsg);
				if(strlen(szInFileName)>30)
					szInFileName[30]=0;
				sprintf(szBuf,"  %-30s %-15s %s", szInFileName,szTempFileName,szResultMsg);
				objDecriptResult.Set((const unsigned char*)szBuf);
				//���ʂ��h�L�������g�֓o�^
				m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
				//�t�@�C���������I��
				return TRUE;
			}
		}
		else //���̓t�@�C���폜���s
		{
			//�ϊ�����
			objMsg.LoadString(IDS_DECRYPT_IN_RENAMEERR);
			strcpy(szResultMsg,(LPCTSTR)objMsg);
			if(strlen(szInFileName)>30)
				szInFileName[30]=0;
			sprintf(szBuf,"  %-30s %-15s %s", szInFileName,"---",szResultMsg);
			objDecriptResult.Set((const unsigned char*)szBuf);
			//���ʂ��h�L�������g�֓o�^
			m_pobjDecriptDoc->AppendDecriptLine(objDecriptResult);
			//�o�͗p�t�@�C���폜
			if( (_access( szTempFileName, 0 )) != -1 )
			{
				::DeleteFile(szTempFileName);
			}
			//�t�@�C���������I��
			return TRUE;
		}
	}

	return bRetStatus;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���������󋵃_�C�A���O�\���҂�����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForDecriptDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjDecriptStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjDecriptStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjDecriptStatusDlg->m_hWnd);
			if(bRet!=NULL)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���������L�����Z���`�F�b�N����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::IsDecriptCanceled()	//TRUE : �L�����Z���AFALSE : ���L�����Z��
{
	if( m_pobjDecriptStatusDlg->IsCanceled() )
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �v���e�N�g�֐��i�t�@�C����r�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r���s����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoFileCompare
	(	CString&	objFilePath1,		//���t�@�C���p�X
		CString&	objFilePath2,		//���t�@�C���p�X
		CEditorDoc* pDoc				//�h�L�������g�N���X�ւ̃|�C���^
	)
{
	clock_t			startClock, endClock;
	ULONG			ulDuration=0;
	char			szBuf[512];
	char			szBufForAr1[512], szBufForAr2[512];
	UINT			unCodeKind=0;
	char*			pPtr=NULL;
	char			szFilePath1[_MAX_PATH],  szFilePath2[_MAX_PATH];
	CCodeConvManager	objCodeConvManager;
	CFile			objFile1, objFile2;
	BOOL 			bRet=FALSE;
	CString			objTitle=_T("");
	long			lDelLineCount=0;
	long			lAddLineCount=0;
	BOOL			bIsCanceled=FALSE;
	CString			objMsg;
	CEditorDoc*	pobjCompareDoc=NULL;
	CFrameWnd*	pobjCompareFrameWnd=NULL;

	//�t�@�C���I�[�v��
	strcpy(szFilePath1,(LPCTSTR)objFilePath1);
	if(objCodeConvManager.DetectCode(szFilePath1) == INPUT_BINARY) //�o�C�i���t�@�C���`�F�b�N
	{
		objMsg.LoadString(IDS_FILECOMPARE_BINARY);
		sprintf(szBuf,(LPCTSTR)objMsg,szFilePath1);
		m_pApp->MyMessageBox(szBuf,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if( objFile1.Open( szFilePath1, CFile::modeRead ) == NULL ) //���t�@�C���I�[�v��
	{
		objMsg.LoadString(IDS_FOPEN_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	CArchive	ar1(&objFile1,CArchive::load,512,szBufForAr1);

	strcpy(szFilePath2,(LPCTSTR)objFilePath2);
	if(objCodeConvManager.DetectCode(szFilePath2) == INPUT_BINARY) //�o�C�i���t�@�C���`�F�b�N
	{
		objMsg.LoadString(IDS_FILECOMPARE_BINARY);
		sprintf(szBuf,(LPCTSTR)objMsg,szFilePath2);
		m_pApp->MyMessageBox(szBuf,(MB_OK|MB_ICONSTOP),0);
		ar1.Close();
		objFile1.Close();
		return;
	}
	if( objFile2.Open( szFilePath2, CFile::modeRead ) == NULL ) //���t�@�C���I�[�v��
	{
		objMsg.LoadString(IDS_FOPEN_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		ar1.Close();
		objFile1.Close();
		return;
	}
	CArchive	ar2(&objFile2,CArchive::load,512,szBufForAr2);

	if(pDoc == NULL)
	{
		//�t�@�C����r���ʏo�͗p�l�c�h�q�E�C���h�E�쐬
		bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&pobjCompareDoc,&pobjCompareFrameWnd);
		if( bRet == FALSE )
		{
			ar1.Close();
			ar2.Close();
			objFile1.Close();
			objFile2.Close();
			return;
		}
	}
	else
	{
		pobjCompareDoc = pDoc;
	}
	pobjCompareDoc->SetFontOfCurrentFileExt("");
	pobjCompareDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//�����݋֎~Ӱ�޽ð���߲ݐݒ�
	pobjCompareDoc->SetCompareMode(TRUE,szFilePath1,szFilePath2);
	//�^�C�g���\��
	objMsg.LoadString(IDS_FILECOMPARE_TITLE);
	objTitle.Format(objMsg,szFilePath1,objFilePath2);
	pobjCompareDoc->SetTitle(objTitle);

	//�t�@�C����r�󋵃��[�h���X�_�C�A���O�\��
	m_pobjFileCompareStatusDlg = new CFileCompareStatusDlg();
	m_pobjFileCompareStatusDlg->m_objFile1Name = szFilePath1;
	m_pobjFileCompareStatusDlg->m_objFile2Name = szFilePath2;
	m_pobjFileCompareStatusDlg->Create(IDD_FILECOMPARE);
	WaitForFileCompareStatusDlgDisp();

	//�^�X�N�X�^�[�g���Ԍv��
	startClock = clock();

	//�t�@�C����r
	CFileCompare	objFileCompare;
	bRet = objFileCompare.DoCompare( m_pobjFileCompareStatusDlg,	// �t�@�C����r�󋵃_�C�A���O
									 pobjCompareDoc,				// �h�L�������g�N���X
									 ar1,							// ���t�@�C���A�[�J�C�u
									 ar2);							// ���t�@�C���A�[�J�C�u

	//�t�@�C����r�󋵃��[�h���X�_�C�A���O����
	bIsCanceled = m_pobjFileCompareStatusDlg->IsCanceled();
	m_pobjFileCompareStatusDlg->DestroyWindow();
	delete m_pobjFileCompareStatusDlg;
	m_pobjFileCompareStatusDlg = NULL;

	pobjCompareDoc->UpdateAllViews(NULL,NULL,NULL);

	//�^�X�N�G���h���Ԍv��
	endClock = clock();
	ulDuration = (ULONG)(endClock - startClock);
	ulDuration /= (ULONG)CLOCKS_PER_SEC;

	//���ʕ\��
	CFileCompareResultDlg	objFileCompareResultDlg;
	objFileCompareResultDlg.m_objFile1Name = szFilePath1;
	objFileCompareResultDlg.m_objFile2Name = szFilePath2;
	objFileCompare.GetDelAddLineCount(&lDelLineCount,&lAddLineCount);
	sprintf(szBuf,"%6d",lDelLineCount);
	objFileCompareResultDlg.m_objDelLineCountStr = szBuf;
	sprintf(szBuf,"%6d",lAddLineCount);
	objFileCompareResultDlg.m_objAddLineCountStr = szBuf;
	if(bRet==FALSE)
	{
		if(bIsCanceled)
			objMsg.LoadString(IDS_FILECOMPARECANCEL);
		else
			objMsg.LoadString(IDS_FILECOMPARENG);
		objFileCompareResultDlg.m_objStatusStr = objMsg;
	}
	else 
	{
		if(lDelLineCount==0 && lAddLineCount==0)
			objMsg.LoadString(IDS_FILECOMPARE_END);
		else
			objMsg.LoadString(IDS_FILECOMPARE_END2);
		objFileCompareResultDlg.m_objStatusStr = objMsg;
	}
	objMsg.LoadString(IDS_TIME);
	sprintf(szBuf,(LPCTSTR)objMsg,ulDuration);
	objFileCompareResultDlg.m_objTimeStr = szBuf;
	objFileCompareResultDlg.DoModal();

	//�t�@�C���N���[�Y
	ar1.Close();
	ar2.Close();
	objFile1.Close();
	objFile2.Close();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r�󋵃_�C�A���O�\���҂�									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForFileCompareStatusDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjFileCompareStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjFileCompareStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjFileCompareStatusDlg->m_hWnd);
			if(bRet!=NULL)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �v���e�N�g�֐��i�f�B���N�g����r�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r���s����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoDirCompare()
{
	clock_t	startClock, endClock;
	ULONG	ulDuration=0;
	char	szBuf[512];
	CLine	objCmpResult;
	CString objMsg;

	//���s�f�B���N�g�����݃`�F�b�N
	CFileFind objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolder1DirCmp);
	if(!bRet)//�Ȃ��I�I
	{
		if(m_pApp->IsDriveRoot(m_objFolder1DirCmp) == FALSE) //���[�g�f�B���N�g���ł��Ȃ�
		{
			objMsg.LoadString(IDS_NO1STDIR_CMP);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}
	bRet = objFileFind.FindFile(m_objFolder2DirCmp);
	if(!bRet)//�Ȃ��I�I
	{
		if(m_pApp->IsDriveRoot(m_objFolder2DirCmp) == FALSE) //���[�g�f�B���N�g���ł��Ȃ�
		{
			objMsg.LoadString(IDS_NO2NDDIR_CMP);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//�f�B���N�g����r���ʏo�͗p�l�c�h�q�E�C���h�E�쐬
	m_pobjDirCmpDoc = NULL;
	m_pobjDirCmpFrameWnd = NULL;
	bRet = m_pApp->CreateNewMDIChdWnd((CDocument**)&m_pobjDirCmpDoc,&m_pobjDirCmpFrameWnd);
	if( bRet == FALSE )
		return;
	m_pobjDirCmpDoc->SetFontOfCurrentFileExt("");
	m_pobjDirCmpDoc->SetColorOfCurrentFileExt("");

	SetReadOnlyMode(TRUE);//�����݋֎~Ӱ�޽ð���߲ݐݒ�
	CString	objTitle;
	objMsg.LoadString(IDS_DIRCOMPARE_TITLE);
	objTitle.Format(objMsg, m_objFolder1DirCmp, m_objFolder2DirCmp);
	m_pobjDirCmpDoc->SetTitle(objTitle);
	m_pobjDirCmpDoc->SetDirCmpMode(TRUE);

	//�f�B���N�g�����o�^
	m_pobjDirCmpDoc->SetDirCmpFolder(m_objFolder1DirCmp,m_objFolder2DirCmp);

	//�f�B���N�g����r�󋵃��[�h���X�_�C�A���O�\��
	m_pobjDirCompareStatusDlg = new CDirCompareStatusDlg();
	m_pobjDirCompareStatusDlg->Create(IDD_DIRCOMPARESTATUS);
	WaitForDirCompareStatusDlgDisp();
	m_pobjDirCompareStatusDlg->SetDir1(m_objFolder1DirCmp);
	m_pobjDirCompareStatusDlg->SetDir2(m_objFolder2DirCmp);

	//��s
	objCmpResult.Set((const unsigned char*)"");
	objCmpResult.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
	m_pobjDirCmpDoc->InsertDirCmpLine(0,&objCmpResult);

	//�^�X�N�X�^�[�g���Ԍv��
	startClock = clock();

	//�P�f�B���N�g����r���s
	bRet = DoDirCmpInOneDirectory();

	//�^�C�g���ǉ�
	objCmpResult.Set((const unsigned char*)"-------------------------------------------------------------------------------------------------");
	objCmpResult.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
	m_pobjDirCmpDoc->InsertDirCmpLine(0,&objCmpResult);
	objMsg.LoadString(IDS_DIRCOMPARE_LIST_TITLE);
	objCmpResult.Set( (const unsigned char*)(LPCTSTR)objMsg );
	objCmpResult.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
	m_pobjDirCmpDoc->InsertDirCmpLine(0,&objCmpResult);
	objMsg.LoadString(IDS_DIRCOMPARE_2NDDIR);
	sprintf(szBuf,(LPCTSTR)objMsg,(LPCTSTR)m_objFolder2DirCmp);
	objCmpResult.Set((const unsigned char*)szBuf);
	objCmpResult.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
	m_pobjDirCmpDoc->InsertDirCmpLine(0,&objCmpResult);
	objMsg.LoadString(IDS_DIRCOMPARE_1STDIR);
	sprintf(szBuf,(LPCTSTR)objMsg,(LPCTSTR)m_objFolder1DirCmp);
	objCmpResult.Set((const unsigned char*)szBuf);
	objCmpResult.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
	m_pobjDirCmpDoc->InsertDirCmpLine(0,&objCmpResult);
	objCmpResult.Set((const unsigned char*)"");
	objCmpResult.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
	m_pobjDirCmpDoc->InsertDirCmpLine(0,&objCmpResult);
	objMsg.LoadString(IDS_DIRCOMPARE_TITLE2);
	objCmpResult.Set((const unsigned char*)(LPCTSTR)objMsg);
	objCmpResult.SetDirCmpLineKind(DIRCMP_LINEKIND_NORMAL);
	m_pobjDirCmpDoc->InsertDirCmpLine(0,&objCmpResult);

	//�f�B���N�g����r�󋵃��[�h���X�_�C�A���O����
	BOOL bIsCanceled       = m_pobjDirCompareStatusDlg->IsCanceled();
	long lCompareFileCount = m_pobjDirCompareStatusDlg->GetCompareFileCount();
	long lAddFileCount     = m_pobjDirCompareStatusDlg->GetAddFileCount();
	long lDelFileCount     = m_pobjDirCompareStatusDlg->GetDelFileCount();
	long lDiffFileCount    = m_pobjDirCompareStatusDlg->GetDiffFileCount();
	m_pobjDirCompareStatusDlg->DestroyWindow();
	delete m_pobjDirCompareStatusDlg;
	m_pobjDirCompareStatusDlg = NULL;

	m_pobjDirCmpDoc->UpdateAllViews(NULL,NULL,NULL);

	//�^�X�N�G���h���Ԍv��
	endClock = clock();
	ulDuration = (ULONG)(endClock - startClock);
	ulDuration /= (ULONG)CLOCKS_PER_SEC;

	//�f�B���N�g����r�I���ʒm
	CDirCompareResultDlg	objDirCompareResultDlg;
	objDirCompareResultDlg.m_objDir1Name = m_objFolder1DirCmp;
	objDirCompareResultDlg.m_objDir2Name = m_objFolder2DirCmp;
	objDirCompareResultDlg.m_lCompareFileCount = lCompareFileCount;
	objDirCompareResultDlg.m_lAddFileCount = lAddFileCount;
	objDirCompareResultDlg.m_lDelFileCount = lDelFileCount;
	objDirCompareResultDlg.m_lDiffFileCount = lDiffFileCount;
	objDirCompareResultDlg.m_lTime = (long)ulDuration;
	if(bRet==FALSE)
	{
		if(bIsCanceled)
			objMsg.LoadString(IDS_DIRCOMPARECANCEL);
		else
			objMsg.LoadString(IDS_DIRCOMPARENG);
		objDirCompareResultDlg.m_objStatusStr = objMsg;
	}
	else 
	{
		objMsg.LoadString(IDS_DIRCOMAPRE_END);
		objDirCompareResultDlg.m_objStatusStr = objMsg;
	}
	objDirCompareResultDlg.DoModal();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�f�B���N�g����r���s												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoDirCmpInOneDirectory()
{
	//�f�B���N�g����r���s
	CDirCompare	objDirCompare;
	return objDirCompare.DoCompare( m_pobjDirCompareStatusDlg,
									m_pobjDirCmpDoc,
									m_objFolder1DirCmp,
							 		m_objFolder2DirCmp );
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r�󋵃_�C�A���O�\���҂�								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForDirCompareStatusDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjDirCompareStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjDirCompareStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjDirCompareStatusDlg->m_hWnd);
			if(bRet!=NULL)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �v���e�N�g�֐��i�t�@�C���^�b�`�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���^�b�`���s����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::DoFileTouch()
{
	CString objMsg;

	//�t�@�C���^�b�`���s�f�B���N�g�����݃`�F�b�N
	CFileFind		objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolderTouch);
	if(!bRet)//�Ȃ��I�I
	{
		if(m_pApp->IsDriveRoot(m_objFolderTouch) == FALSE) //���[�g�f�B���N�g���ł��Ȃ�
		{
			objMsg.LoadString(IDS_NOFSEARCHFOLDER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//�P�f�B���N�g�����t�@�C���^�b�`���s
	bRet = DoFileTouchInOneDirectory(m_objFolderTouch,m_objFileKindTouch,m_objFileTouchTime);

	//�t�@�C���^�b�`�I���ʒm
	if( bRet != FALSE )
	{
		objMsg.LoadString(IDS_FTOUCHEND);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
	else
	{
		objMsg.LoadString(IDS_FTOUCHCANCEL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�f�B���N�g�����t�@�C���^�b�`���s����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMainFrame::DoFileTouchInOneDirectory	// TRUE : ����I���AFALSE : �L�����Z���I��
	(	CString		objFolder,				// �^�b�`���s���t�H���_
		CString		objFileKinds,			// �^�b�`�Ώۃt�@�C�����
		CTime		objTouchTime			// �^�b�`����
	)
{
	BOOL		bRetStatus = TRUE;
	BOOL		bRet, bRetTmp;
	CString		objFilter;
	CString		objOneKind, objRight, objBuf;
	CString		objFilePath;
	CFileFind	objFileFind;
	CFileStatus objFileStatus;

	objRight = objFileKinds;
	for(;;)
	{
		//�E�C���h�E�Y���b�Z�[�W����
		m_pApp->MessagePump();

		//�����Ώۃt�@�C����ʐ؂���
		int nIndex = objRight.Find(';');
		if(nIndex != -1)
		{
			objOneKind = objRight.Left(nIndex);
			objBuf = objRight.Right( objRight.GetLength() - nIndex - 1 );
			objRight = objBuf;
		}
		else
		{
			objOneKind = objRight;
		}

		//�t�@�C�������L�[�쐬
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s%s",objFolder,objOneKind);
		else
			objFilter.Format("%s\\%s",objFolder,objOneKind);

		//�Ώۃt�@�C������
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//�E�C���h�E�Y���b�Z�[�W����
				m_pApp->MessagePump();

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
					continue;

				//�t�@�C���^�b�`
				objFilePath = objFileFind.GetFilePath();
				CFile::GetStatus((LPCTSTR)objFilePath,objFileStatus);
				objFileStatus.m_mtime = objTouchTime;
				CFile::SetStatus((LPCTSTR)objFilePath,objFileStatus);
			}
			//�Ō�̂P��
			if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
			{
				//�t�@�C���^�b�`
				objFilePath = objFileFind.GetFilePath();
				CFile::GetStatus((LPCTSTR)objFilePath,objFileStatus);
				objFileStatus.m_mtime = objTouchTime;
				CFile::SetStatus((LPCTSTR)objFilePath,objFileStatus);
			}
		}
		objFileFind.Close();

		if( nIndex == -1 ) // �t�@�C����ʎc��Ȃ�
		{
			break;
		}
	}

	//�E�C���h�E�Y���b�Z�[�W����
	m_pApp->MessagePump();

	if( m_bIsSearchSubFolderTouch ) //�T�u�t�H���_����������ꍇ
	{
		//�t�@�C�������L�[�쐬�i���C���h�J�[�h�����j
		if(m_pApp->IsDriveRoot(objFolder))
			objFilter.Format("%s*.*",objFolder);
		else
			objFilter.Format("%s\\*.*",objFolder);

		//�T�u�f�B���N�g������
		bRet = objFileFind.FindFile(objFilter);
		if(bRet != FALSE)
		{
			while(objFileFind.FindNextFile()!=FALSE)
			{
				//�E�C���h�E�Y���b�Z�[�W����
				m_pApp->MessagePump();

				if( objFileFind.IsDots() != FALSE ) // "." or ".."
					continue;
				if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
				{
					//�P�f�B���N�g�����t�@�C���������s�i�������ċA�Ăяo���j
					bRetTmp = DoFileTouchInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objTouchTime);
					if(bRetTmp == FALSE) //�L�����Z���I�����Ă���
					{
						objFileFind.Close();
						return FALSE;
					}
				}
			}
			//�Ō�̂P��
			if( (objFileFind.IsDots()==FALSE) && (objFileFind.IsDirectory()!=FALSE) )
			{
				//�P�f�B���N�g�����t�@�C���������s�i�������ċA�Ăяo���j
				bRetTmp = DoFileTouchInOneDirectory(objFileFind.GetFilePath(),objFileKinds,objTouchTime);
				if(bRetTmp == FALSE) //�L�����Z���I�����Ă���
				{
					objFileFind.Close();
					return FALSE;
				}
			}
		}
		objFileFind.Close();
	}

	return bRetStatus;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame �v���e�N�g�֐��i���[�����X�g�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[�����X�g�󋵃_�C�A���O�\���҂�									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMainFrame::WaitForMailListDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjMailListStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjMailListStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjMailListStatusDlg->m_hWnd);
			if(bRet!=0)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}


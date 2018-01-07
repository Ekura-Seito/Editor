// EditorView.cpp : CEditorView �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include <Afxtempl.h>			//�l�e�b�̂b�l�������g�p����
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include <afxadv.h>				//for CSharedFile
#include <mbstring.h>			//for OnLButtonDblClk
#include <io.h>					//for _access
#include "Editor_Def.h"
#include "StatusBarDisp.h"		//�X�e�[�^�X�o�[�\�����\����
#include "ColorManager.h"
#include "CodeConvManager.h"
#include "BitmapClient.h"
#include "Operation.h"
#include "OperationManager.h"
#include "MainFrm.h"
#include "Editor.h"
#include "Line.h"
#include "LineManager.h"
#include "UndoItem.h"
#include "UndoManager.h"
#include "EditorDoc.h"
#include "EditorView.h"
#include "LineJumpDlg.h"
#include "AutoResizer.h"
#include "MarkLineListDlg.h"
#include "KeyBind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorView

IMPLEMENT_DYNCREATE(CEditorView, CView)

BEGIN_MESSAGE_MAP(CEditorView, CView)
	ON_WM_CONTEXTMENU()
	ON_MESSAGE(WM_IME_NOTIFY, OnMyImeNotify)
	//{{AFX_MSG_MAP(CEditorView)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_SYSKEYDOWN()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectall)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECTALL, OnUpdateEditSelectall)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(ID_EDIT_DEL, OnUpdateEditDel)
	ON_COMMAND(ID_EDIT_DEL, OnEditDel)
	ON_COMMAND(ID_EDIT_JUMP2TOP, OnEditJump2top)
	ON_UPDATE_COMMAND_UI(ID_EDIT_JUMP2TOP, OnUpdateEditJump2top)
	ON_COMMAND(ID_EDIT_JUMP2BOTTOM, OnEditJump2bottom)
	ON_UPDATE_COMMAND_UI(ID_EDIT_JUMP2BOTTOM, OnUpdateEditJump2bottom)
	ON_UPDATE_COMMAND_UI(ID_EDIT_JUMP2LINE, OnUpdateEditJump2line)
	ON_COMMAND(ID_EDIT_JUMP2LINE, OnEditJump2line)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_VIEW_LINENO, OnUpdateViewLineno)
	ON_COMMAND(ID_VIEW_LINENO, OnViewLineno)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CRLF, OnUpdateViewCrlf)
	ON_COMMAND(ID_VIEW_CRLF, OnViewCrlf)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RULER, OnUpdateViewRuler)
	ON_COMMAND(ID_VIEW_RULER, OnViewRuler)
	ON_COMMAND(ID_PARENTHESIS, OnParenthesis)
	ON_COMMAND(ID_VIEW_TAB, OnViewTab)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TAB, OnUpdateViewTab)
	ON_COMMAND(ID_VIEW_ZENKAKUSPACE, OnViewZenkakuspace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZENKAKUSPACE, OnUpdateViewZenkakuspace)
	ON_COMMAND(ID_EDIT_TAB2SPACE, OnEditTab2space)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TAB2SPACE, OnUpdateEditTab2space)
	ON_COMMAND(ID_EDIT_SPACE2TAB, OnEditSpace2tab)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SPACE2TAB, OnUpdateEditSpace2tab)
	ON_COMMAND(ID_EDIT_SPACE_HAN2ZEN, OnEditSpaceHan2zen)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SPACE_HAN2ZEN, OnUpdateEditSpaceHan2zen)
	ON_COMMAND(ID_EDIT_SPACE_ZEN2HAN, OnEditSpaceZen2han)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SPACE_ZEN2HAN, OnUpdateEditSpaceZen2han)
	ON_COMMAND(ID_EDIT_KATAKANA_HAN2ZEN, OnEditKatakanaHan2zen)
	ON_UPDATE_COMMAND_UI(ID_EDIT_KATAKANA_HAN2ZEN, OnUpdateEditKatakanaHan2zen)
	ON_COMMAND(ID_EDIT_KATAKANA_ZEN2HAN, OnEditKatakanaZen2han)
	ON_UPDATE_COMMAND_UI(ID_EDIT_KATAKANA_ZEN2HAN, OnUpdateEditKatakanaZen2han)
	ON_COMMAND(ID_VIEW_UNDERLINE, OnViewUnderline)
	ON_UPDATE_COMMAND_UI(ID_VIEW_UNDERLINE, OnUpdateViewUnderline)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CARET_IBEAM, OnUpdateViewCaretIbeam)
	ON_COMMAND(ID_VIEW_CARET_IBEAM, OnViewCaretIbeam)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CARET_SQUARE, OnUpdateViewCaretSquare)
	ON_COMMAND(ID_VIEW_CARET_SQUARE, OnViewCaretSquare)
	ON_COMMAND(ID_EDIT_ALNUM_HAN2ZEN, OnEditAlnumHan2zen)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ALNUM_HAN2ZEN, OnUpdateEditAlnumHan2zen)
	ON_COMMAND(ID_EDIT_ALNUM_ZEN2HAN, OnEditAlnumZen2han)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ALNUM_ZEN2HAN, OnUpdateEditAlnumZen2han)
	ON_COMMAND(ID_EDIT_DELTOPSPACES, OnEditDeltopspaces)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELTOPSPACES, OnUpdateEditDeltopspaces)
	ON_COMMAND(ID_EDIT_DELTAB, OnEditDeltab)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELTAB, OnUpdateEditDeltab)
	ON_COMMAND(ID_EDIT_INSTAB, OnEditInstab)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSTAB, OnUpdateEditInstab)
	ON_UPDATE_COMMAND_UI(ID_PARENTHESIS, OnUpdateParenthesis)
	ON_COMMAND(ID_EDIT_TAGJUMP, OnEditTagjump)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TAGJUMP, OnUpdateEditTagjump)
	ON_COMMAND(ID_EDIT_INSCPPCOMMENT, OnEditInscppcomment)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSCPPCOMMENT, OnUpdateEditInscppcomment)
	ON_COMMAND(ID_EDIT_INSVBCOMMENT, OnEditInsvbcomment)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSVBCOMMENT, OnUpdateEditInsvbcomment)
	ON_COMMAND(ID_EDIT_INSASMCOMMENT, OnEditInsasmcomment)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSASMCOMMENT, OnUpdateEditInsasmcomment)
	ON_COMMAND(ID_VIEW_REDRAW, OnViewRedraw)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REDRAW, OnUpdateViewRedraw)
	ON_COMMAND(ID_VIEW_EOF, OnViewEof)
	ON_UPDATE_COMMAND_UI(ID_VIEW_EOF, OnUpdateViewEof)
	ON_COMMAND(ID_JUMP_SETMARK, OnJumpSetmark)
	ON_UPDATE_COMMAND_UI(ID_JUMP_SETMARK, OnUpdateJumpSetmark)
	ON_COMMAND(ID_JUMP_DELMARK, OnJumpDelmark)
	ON_UPDATE_COMMAND_UI(ID_JUMP_DELMARK, OnUpdateJumpDelmark)
	ON_COMMAND(ID_JUMP_NEXTMARK, OnJumpNextmark)
	ON_UPDATE_COMMAND_UI(ID_JUMP_NEXTMARK, OnUpdateJumpNextmark)
	ON_COMMAND(ID_JUMP_PREVMARK, OnJumpPrevmark)
	ON_UPDATE_COMMAND_UI(ID_JUMP_PREVMARK, OnUpdateJumpPrevmark)
	ON_COMMAND(ID_JUMP_MARKLIST, OnJumpMarklist)
	ON_UPDATE_COMMAND_UI(ID_JUMP_MARKLIST, OnUpdateJumpMarklist)
	ON_COMMAND(ID_JUMP_ALLDELMARK, OnJumpAlldelmark)
	ON_UPDATE_COMMAND_UI(ID_JUMP_ALLDELMARK, OnUpdateJumpAlldelmark)
	ON_COMMAND(ID_EDIT_INSDAY, OnEditInsday)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSDAY, OnUpdateEditInsday)
	ON_COMMAND(ID_EDIT_INSTIME, OnEditInstime)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSTIME, OnUpdateEditInstime)
	ON_COMMAND(ID_EDIT_AL2UPPER, OnEditAl2upper)
	ON_UPDATE_COMMAND_UI(ID_EDIT_AL2UPPER, OnUpdateEditAl2upper)
	ON_COMMAND(ID_EDIT_AL2LOWER, OnEditAl2lower)
	ON_UPDATE_COMMAND_UI(ID_EDIT_AL2LOWER, OnUpdateEditAl2lower)
	ON_COMMAND(ID_WORDSKIP2RIGHT, OnWordskip2right)
	ON_COMMAND(ID_WORDSKIP2LEFT, OnWordskip2left)
	ON_COMMAND(ID_WORDSELECT2RIGHT, OnWordselect2right)
	ON_COMMAND(ID_WORDSELECT2LEFT, OnWordselect2left)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_EDIT_UUDECODE, OnEditUudecode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UUDECODE, OnUpdateEditUudecode)
	ON_COMMAND(ID_EDIT_BASE64DECODE, OnEditBase64decode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BASE64DECODE, OnUpdateEditBase64decode)
	ON_COMMAND(ID_EDIT_QUOTEDDECODE, OnEditQuoteddecode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_QUOTEDDECODE, OnUpdateEditQuoteddecode)
	ON_COMMAND(ID_EDIT_NEXTDIFF, OnEditNextdiff)
	ON_UPDATE_COMMAND_UI(ID_EDIT_NEXTDIFF, OnUpdateEditNextdiff)
	ON_COMMAND(ID_EDIT_PREVDIFF, OnEditPrevdiff)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PREVDIFF, OnUpdateEditPrevdiff)
	ON_COMMAND(ID_EDIT_JUMP2TOPSELECT, OnEditJump2topselect)
	ON_COMMAND(ID_EDIT_JUMP2BOTTOMSELECT, OnEditJump2bottomselect)
	ON_COMMAND(ID_EDIT_DIRCMP_SYOSAI, OnEditDircmpSyosai)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DIRCMP_SYOSAI, OnUpdateEditDircmpSyosai)
	ON_COMMAND(ID_PARENTHESIS_SELECT, OnParenthesisSelect)
	ON_COMMAND(ID_EDIT_BROWSEURL, OnEditBrowseurl)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BROWSEURL, OnUpdateEditBrowseurl)
	ON_COMMAND(ID_EDIT_EXECMAILER, OnEditExecmailer)
	ON_UPDATE_COMMAND_UI(ID_EDIT_EXECMAILER, OnUpdateEditExecmailer)
	ON_COMMAND(ID_EDIT_INSQUOTE, OnEditInsquote)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSQUOTE, OnUpdateEditInsquote)
	ON_COMMAND(ID_EDIT_DELCPPCOMMENT, OnEditDelcppcomment)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELCPPCOMMENT, OnUpdateEditDelcppcomment)
	ON_COMMAND(ID_EDIT_DELVBCOMMENT, OnEditDelvbcomment)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELVBCOMMENT, OnUpdateEditDelvbcomment)
	ON_COMMAND(ID_EDIT_DELASMCOMMENT, OnEditDelasmcomment)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELASMCOMMENT, OnUpdateEditDelasmcomment)
	ON_COMMAND(ID_EDIT_DELQUOTE, OnEditDelquote)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELQUOTE, OnUpdateEditDelquote)
	ON_COMMAND(ID_CHANGE_INSMODE, OnChangeInsmode)
	//}}AFX_MSG_MAP
	// �W������R�}���h
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorView �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CEditorView::CEditorView()
{
	CString objMsg;
	m_pApp = (CEditorApp*)AfxGetApp();

	////////////////////////////////////////
	//	�����o�ϐ�������
	////////////////////////////////////////

	// �`��֘A
	m_pobjInvalidRegion = NULL;			// ������ذ�ޮ�
	m_pobjInvalidRect = NULL;			// �����ȋ�`
	m_bUseClipRgn = 0;				// �N���b�v���ꂽ���[�W�����̂ݕ`��
	m_bNoUseOffBuffer = 0;			// �̽�ذ��ޯ̧�s�g�p�׸�

	// �S�p�X�y�[�X�֘A
	m_bDispZenkakuSpace = 0;			// �S�p�X�y�[�X�`��t���O
	m_ZenkakuSpaceColor = 0;			// �S�p�X�y�[�X�̐F

	// �^�u�֘A
	m_bDispTab = 0;					// �^�u�R�[�h�`��t���O
	m_TabColor = 0;					// �^�u�R�[�h�̐F

	// ���[���֘A
	m_bDispRuler = 0;				// ���[���\���t���O
	m_RulerColor = 0;				// ���[���̐F
	m_RulerBackColor = 0;			// ���[���̔w�i�̐F
	memset(m_szRuler,0,sizeof(m_szRuler));
	m_RulerPos = CPoint(0,0);			// ���[���\�����̃��[�������ʒu

	// �s�ԍ��֘A
	m_bDispLineNo = 0;				// �s�ԍ��\���t���O
	m_LineNoColor = 0;				// �s�ԍ��̐F
	m_LineNoBackColor = 0;			// �s�ԍ��̔w�i�̐F
	memset(m_szLineNoFormat,0,sizeof(m_szLineNoFormat));
	m_lLineNoWidth = 0;				// �s�ԍ��\����
	m_LineNoPos = CPoint(0,0);			// �s�ԍ��\�����̍s���ʒu

	// ���s�}�[�N�֘A
	m_bDispCRLF = 0;					// ���s�}�[�N�\���t���O
	m_CRLFColor = 0;					// ���s�}�[�N�̐F

	// �I�t�X�N���[���o�b�t�@�֘A
	m_nOffBufferWidth = 0;			// �̽�ذ��ޯ̧�̕�

	// �L�����b�g�֘A
	m_nCaretType = 0;				// �L�����b�g�`��
	m_nCaretBehave = 0;				// ��ʃX�N���[�����̃L�����b�g�̐U�镑��
	m_caretPos = CPoint(0, 0);			// �L�����b�g�\���ʒu
	m_unCaretBlinkTime = 0;			// �L�����b�g�_�ŊԊu
	m_nCaretMoveType = 0;				// �L�����b�g�ړ��^�C�v
	m_nLogPreCaretX = 0;				// ���O�̃L�����b�g�_���w�ʒu

	// IME�֘A
	m_hImc = NULL;
	m_bIsImmOpen = 0;				// �h�l�l�ϊ����t���O

	// ��۰ي֘A
	m_nMapMode = 0;					// ϯ��ݸ�Ӱ��
	m_totalLog.cx = 0;				// �_���P�ʂł�İ�ٻ���
	m_totalLog.cy = 0;
	m_VPageLog.cx = 0;				// ������۰��ް�̘_���P�ʂł̂P�߰�ނ����轸۰ٻ���
	m_VPageLog.cy = 0;
	m_VLineLog.cx = 0;				// ������۰��ް�̘_���P�ʂł̂P�s�����轸۰ٻ���
	m_VLineLog.cy = 0;
	m_HPageLog.cx = 0;				// ������۰��ް�̘_���P�ʂł̂P�߰�ނ����轸۰ٻ���
	m_HPageLog.cy = 0;
	m_HLineLog.cx = 0;				// ������۰��ް�̘_���P�ʂł̂P�s�����轸۰ٻ���
	m_HLineLog.cy = 0;

	// ÷�Ċ֘A
	m_bInsertKey = TRUE;				// Insert�L�[�����t���O�i��̫�Ăő}��Ӱ�ށj
	m_n1stChar = 0;					// �������@����޲ĕ����ޯ̧
	m_n2ndChar = 0;					// �������@����޲ĕ����ޯ̧

	// �\���t�H���g�֘A
	m_lfHeight = 0;					// �t�H���g�̕����̍���
	m_lfWidth = 0;					// �t�H���g�̕����̕�
	m_lDispFontPoint = 0;			// �\���t�H���g�̃|�C���g
	m_objDispFontName = _T("");			// �\���t�H���g��
	m_FontColor = 0;					// �����F

	// �J���[�֘A
	m_FontColor = 0;					// �ʏ�̕����F
	m_BackColor = 0;					// �ʏ�̔w�i�F
	m_InvertFontColor = 0;			// ���]���̕����F
	m_InvertBackColor = 0;			// ���]���̔w�i�F

	// ����֘A
	m_bPrintLineNo = 0;				// �s�ԍ�����t���O
	m_bPrint2Dangumi = 0;			// �Q�i�g�݈���t���O
	//m_objPrintFoldLine;				// �܂�Ԃ�����s
	m_lPrintFoldLineNo = 0;			// �܂�Ԃ�����s�ԍ�
	m_nHeaderLayout = 0;				// �w�b�_���C�A�E�g�ʒu
	m_nFooterLayout = 0;				// �t�b�^���C�A�E�g�ʒu
	m_objStrHeaderFormat = _T("");		// �w�b�_�t�H�[�}�b�g
	m_objStrFooterFormat = _T("");		// �t�b�^�t�H�[�}�b�g
	m_lPrintLineHeight = 0;			// �P�s������̃f�o�C�X�P�ʂ̍���
	m_lPrintCharWidth = 0;			// �P����������̃f�o�C�X�P�ʂ̕�
	m_lPrintFontPoint = 0;			// �󎚃t�H���g�̃|�C���g
	m_objPrintFontName = _T("");		// �󎚃t�H���g��
	m_lLinePerPrintPage = 0;			// �P�y�[�W������̍s��

	// �͈͑I���֘A
	m_bIsShiftKeyDown = FALSE;			// �V�t�g�L�[�������t���O
	m_bLButtonDown = 0;				// �}�E�X���{�^���������t���O
	m_lSelectStartLine = -1;			// �I���J�n�s�ԍ�
	m_lSelectStartOffset = -1;			// �I���J�n��ԍ�
	m_lSelectEndLine = -1;				// �I���I���s�ԍ�
	m_lSelectEndOffset = -1;			// �I���I����ԍ�
	m_lPreMoveCaretLine = 0;			// �ړ��O�̍s
	m_lPreMoveCaretOffset = 0;		// �ړ��O�̗�
	m_lLButtonDownLine = 0;			// �}�E�X���{�^�����������s
	m_lLButtonDownOffset = 0;		// �}�E�X���{�^������������

	// �s�Ԋ֘A
	m_lLineInterval = 0;				// �s�ԁi�s�N�Z���P�ʁj

	// �A���_�[���C���֘A
	m_bDispUnderLine = 0;			// �A���_�[���C���\���t���O
	m_UnderLineColor = 0;			// �A���_�[���C���̐F

	// �}�[�N�֘A
	m_MarkLineColor = 0;			// �}�[�N�s�̐F

	// �C���N�������^���T�[�`�֘A
	m_lIncrimentalSearchStartLine=0;	// �C���N�������^���T�[�`�J�n�s
	m_lIncrimentalSearchStartOffset=0;// �C���N�������^���T�[�`�J�n�I�t�Z�b�g
	m_objIncrimentalSearchLine.Set((const unsigned char*)"");	// �C���N�������^���T�[�`������

	// �t�@�C����r�ԘA
	m_DiffDelLineColor = 0;			// �폜�s�̐F
	m_DiffAddLineColor = 0;			// �ǉ��s�̐F

	// �f�B���N�g����r�֘A
	m_DirDiffLineColor = 0;			// ����t�@�C�����̐F

	//	���W�X�g������ݒ�擾
	m_FontColor			 = m_pApp->GetProfileInt("CEditorView","FontColor",RGB(255,255,255));
	m_BackColor			 = m_pApp->GetProfileInt("CEditorView","BackColor",RGB(0,0,0));
	m_InvertFontColor	 = m_pApp->GetProfileInt("CEditorView","InvertFontColor",RGB(0,0,0));
	m_InvertBackColor	 = m_pApp->GetProfileInt("CEditorView","InvertBackColor",RGB(255,255,255));
	m_bDispRuler		 = m_pApp->GetProfileInt("CEditorView","bDispRuler",TRUE);
	m_RulerColor		 = m_pApp->GetProfileInt("CEditorView","RulerColor",RGB(0,0,0));
	m_RulerBackColor	 = m_pApp->GetProfileInt("CEditorView","RulerBackColor",RGB(192,192,192));
	strcpy(m_szRuler,m_pApp->GetProfileString("CEditorView","RulerFormat","----+----"));
	m_bDispCRLF			 = m_pApp->GetProfileInt("CEditorView","bDispCRLF",TRUE);
	m_CRLFColor			 = m_pApp->GetProfileInt("CEditorView","CRLFColor",RGB(255,255,0));
	m_bDispLineNo		 = m_pApp->GetProfileInt("CEditorView","bDispLineNo",TRUE);
	m_LineNoColor		 = m_pApp->GetProfileInt("CEditorView","LineNoColor",RGB(0,255,255));
	m_LineNoBackColor	 = m_pApp->GetProfileInt("CEditorView","LineNoBackColor",RGB(0,0,0));
	objMsg.LoadString(IDS_LINENO_FORMAT);
	strcpy(m_szLineNoFormat,(LPCTSTR)objMsg);
	m_bDispZenkakuSpace  = m_pApp->GetProfileInt("CEditorView","bDispZenkakuSpace",FALSE);
	m_ZenkakuSpaceColor  = m_pApp->GetProfileInt("CEditorView","ZenkakuSpaceColor",RGB(255,255,255));
	m_bDispTab           = m_pApp->GetProfileInt("CEditorView","bDispTab",FALSE);
	m_TabColor           = m_pApp->GetProfileInt("CEditorView","TabColor",RGB(255,255,255));
	m_lDispFontPoint	 = m_pApp->GetProfileInt("CEditorView","DispFontPoint",120);
	objMsg.LoadString(IDS_FONTNAME_DISP);
	m_objDispFontName	 = m_pApp->GetProfileString("CEditorView","DispFontName",objMsg);
	m_lPrintFontPoint	 = m_pApp->GetProfileInt("CEditorView","PrintFontPoint",80);
	objMsg.LoadString(IDS_FONTNAME_PRINT);
	m_objPrintFontName	 = m_pApp->GetProfileString("CEditorView","PrintFontName",objMsg);
	m_bPrintLineNo       = m_pApp->GetProfileInt("CEditorView","bPrintLineNo",1);
	m_bPrint2Dangumi     = m_pApp->GetProfileInt("CEditorView","bPrint2Dangumi",0);
	m_nHeaderLayout      = m_pApp->GetProfileInt("CEditorView","nHeaderLayout",0);
	m_nFooterLayout      = m_pApp->GetProfileInt("CEditorView","nFooterLayout",1);
	m_objStrHeaderFormat = m_pApp->GetProfileString("CEditorView","StrHeaderFormat",
								"FILE : &f  DATE : &d &t  PAGE : &p / &P");
	m_objStrFooterFormat = m_pApp->GetProfileString("CEditorView","StrFooterFormat","-- &p --");
//	m_lLineInterval		 = m_pApp->GetProfileInt("CEditorView","lLineInterval",2);
	m_lLineInterval = 2;
	m_bDispUnderLine	 = m_pApp->GetProfileInt("CEditorView","bDispUnderLine",TRUE);
	m_UnderLineColor	 = m_pApp->GetProfileInt("CEditorView","UnderLineColor",RGB(255,255,0));
	m_nCaretType 		 = m_pApp->GetProfileInt("CEditorView","CaretType",CARET_IBEAM);
	if((m_nCaretType != CARET_IBEAM) && (m_nCaretType != CARET_SQUARE))
		m_nCaretType = CARET_IBEAM;
	m_nCaretBehave       = m_pApp->GetProfileInt("CEditorView","CaretBehave",CARET_BEHAVE_MOVE);
	if((m_nCaretBehave != CARET_BEHAVE_MOVE) && (m_nCaretBehave != CARET_BEHAVE_STAY))
		m_nCaretBehave = CARET_BEHAVE_MOVE;
	m_unCaretBlinkTime   = m_pApp->GetProfileInt("CEditorView","CaretBlinkTime",0);
	if(m_unCaretBlinkTime==0)
		m_unCaretBlinkTime = ::GetCaretBlinkTime();
	m_nCaretMoveType     = m_pApp->GetProfileInt("CEditorView","CaretMoveType",CARET_MOVE_WORDTYPE);
	if(m_nCaretMoveType != CARET_MOVE_WORDTYPE && m_nCaretMoveType != CARET_MOVE_NOTEPADTYPE)
		m_nCaretMoveType = CARET_MOVE_WORDTYPE;
	m_MarkLineColor      = m_pApp->GetProfileInt("CEditorView","MarkLineColor",RGB(128,0,128));
	m_DiffDelLineColor   = (COLORREF)m_pApp->GetProfileInt("CEditorView","DiffDelLineColor",RGB(255,0,0));
	m_DiffAddLineColor   = (COLORREF)m_pApp->GetProfileInt("CEditorView","DiffAddLineColor",RGB(0,0,255));
	m_DirDiffLineColor   = (COLORREF)m_pApp->GetProfileInt("CEditorView","DirDiffLineColor",RGB(0,128,0));

	// CG: ���̍s�� �uIME �T�|�[�g�v �R���|�[�l���g�ɂ��ǉ�����Ă��܂��B
	m_hImc = ::ImmCreateContext();
	CgAdjustCompositionWindow();

	//�r�b�g�}�b�v�t���|�b�v�A�b�v���j���[
	static UINT toolbars[]={
		IDR_MAINFRAME,
		IDR_TOOLBAR_EDIT,
		IDR_TOOLBAR_FIND,
		IDR_TOOLBAR_JMP,
		IDR_TOOLBAR_TOOLS,
		IDR_TOOLBAR_WND
	};
	VERIFY(m_popupmenu.LoadMenu(CG_IDR_POPUP_EDITOR_VIEW));
	// Another method for adding bitmaps to menu options is through the
	// LoadToolbars member function.This method allows you to add all
	// the bitmaps in a toolbar object to menu options (if they exist).
	// The first function parameter is an array of toolbar id's.
	// The second is the number of toolbar id's. There is also a
	// function called LoadToolbar that just takes an id.
	m_popupmenu.LoadToolbars(toolbars,6);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CEditorView::~CEditorView()
{
	//�ݒ�����W�X�g���֕ۑ�
	m_pApp->WriteProfileString("CEditorView","RulerFormat",m_szRuler);
	m_pApp->WriteProfileString("CEditorView","LineNoFormat",m_szLineNoFormat);
	m_pApp->WriteProfileInt("CEditorView","DispFontPoint",m_lDispFontPoint);
	m_pApp->WriteProfileString("CEditorView","DispFontName",m_objDispFontName);
	m_pApp->WriteProfileInt("CEditorView","PrintFontPoint",m_lPrintFontPoint);
	m_pApp->WriteProfileString("CEditorView","PrintFontName",m_objPrintFontName);
	m_pApp->WriteProfileInt("CEditorView","bPrintLineNo",m_bPrintLineNo);
	m_pApp->WriteProfileInt("CEditorView","bPrint2Dangumi",m_bPrint2Dangumi);
	m_pApp->WriteProfileInt("CEditorView","nHeaderLayout",m_nHeaderLayout);
	m_pApp->WriteProfileInt("CEditorView","nFooterLayout",m_nFooterLayout);
	m_pApp->WriteProfileString("CEditorView","StrHeaderFormat",m_objStrHeaderFormat);
	m_pApp->WriteProfileString("CEditorView","StrFooterFormat",m_objStrFooterFormat);
	m_pApp->WriteProfileInt("CEditorView","lLineInterval",m_lLineInterval);
	m_pApp->WriteProfileInt("CEditorView","CaretType",m_nCaretType);
	m_pApp->WriteProfileInt("CEditorView","CaretBehave",m_nCaretBehave);
	m_pApp->WriteProfileInt("CEditorView","CaretBlinkTime",m_unCaretBlinkTime);
	m_pApp->WriteProfileInt("CEditorView","CaretMoveType",m_nCaretMoveType);
	m_pApp->WriteProfileInt("CEditorView","DiffDelLineColor",m_DiffDelLineColor);
	m_pApp->WriteProfileInt("CEditorView","DiffAddLineColor",m_DiffAddLineColor);
	m_pApp->WriteProfileInt("CEditorView","DirDiffLineColor",m_DirDiffLineColor);

	if(m_pobjInvalidRegion)
	{
		delete m_pobjInvalidRegion;
		m_pobjInvalidRegion = NULL;
	}
	if(m_pobjInvalidRect)
	{
		delete m_pobjInvalidRect;
		m_pobjInvalidRect = NULL;
	}
	m_objDispFont.DeleteObject();
	m_objDispFontUnderLine.DeleteObject();

	// CG: ���̍s�� �uIME �T�|�[�g�v�R���|�[�l���g�ɂ��ǉ�����Ă��܂��B
	::ImmAssociateContext(m_hWnd, NULL);
	::ImmDestroyContext(m_hImc);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView �N���X�̕`��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�d�q�`�r�d�a�j�f�m�c����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;	//������h�~
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�o�C�X�R���e�L�X�g�̑�����������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(pDC);

	//ϯ��ݸ�Ӱ�ސݒ�
	pDC->SetMapMode(m_nMapMode);

//	//�ޭ��߰Ă̌��_�ݒ�
//	//����ɂ��AOnDraw���Ř_�����W(0,0)�ɕ`�悷��ƁA
//	//�f�o�C�X���W(0-GetScrollPosLogX(),0)�ɕ`�悳���
//	CPoint ptVpOrg(0, 0);
//	if(!pDC->IsPrinting())//����ȊO
//	{
//		//�w�����̃X�N���[�����}�C�i�X�����փV�t�g
//		ptVpOrg.x = -1 * GetScrollPosLogX();
//	}
//	pDC->SetViewportOrg(ptVpOrg);

	//�ޭ��߰Ă̌��_�ݒ�
	CPoint ptVpOrg(0, 0);
	pDC->SetViewportOrg(ptVpOrg);

	//�h�����n���h�����R�[��
	CView::OnPrepareDC(pDC, pInfo);

	if(pDC->IsPrinting())//�����
	{
		//������[�v�I���^�C�~���O�Ǘ�
		pInfo->m_bContinuePrinting = (pInfo->m_nCurPage <= pInfo->GetMaxPage());
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�`�揈��																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnDraw(CDC* pDC)
{
	if(pDC->IsPrinting())	//����p�̃R�[�h�́AOnPrint() �ɋL�q
		return;

	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	// OnDraw() �֐����ĂԑO�Ɍ��݂̃X�N���[���ʒu��
	// �e����^����r���[�|�[�g�̌��_�𒲐߂��Ă��܂��B

	if( m_bUseClipRgn )//�����̈�̂ݕ`�悷��ꍇ
	{
		//���݂̖����̈�擾
		CRgn	rgnClipBox;
		rgnClipBox.CreateRectRgnIndirect((*m_pobjInvalidRect));

		//���݂̃N���b�v�̈�擾
		CRect	rcClip;
		pDC->GetClipBox(rcClip);
		rcClip.NormalizeRect();
		if( rcClip.IsRectEmpty()==FALSE )
		{
			if( rcClip.EqualRect(&clientRect)==FALSE )
			{
				//�̈�ǉ�
				CRgn	rgn;
				rgn.CreateRectRgnIndirect(rcClip);
				rgnClipBox.CombineRgn(&rgnClipBox,&rgn,RGN_OR);
			}
		}

		//�w�i�F�u���V�쐬
		CBrush	objBackBrush;
		objBackBrush.CreateSolidBrush(m_BackColor);
		CBrush	objRulerBackBrush;
		objRulerBackBrush.CreateSolidBrush(m_RulerBackColor);

		//�ő啝�v�Z
		ULONG ulRight = 0;
		if( ((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth) > clientRect.Width() )
			ulRight = (ULONG)((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth);
		else
			ulRight = clientRect.Width();

		//�s�ԍ��\���t�B�[���h�̕�
		if( m_bDispLineNo ) //�s�ԍ���\������
			m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		else
			m_lLineNoWidth = 0;

		//�`��J�n�s�ƕ`��I���s���v�Z
		ULONG	lineStart, lineStop;	//�`��s�ԍ�
		lineStart = (ULONG)((ULONG)GetScrollPosLogY() / (ULONG)(m_lfHeight + m_lLineInterval) );
		lineStop  = lineStart + (ULONG)((ULONG)clientRect.Height() / (ULONG)(m_lfHeight + m_lLineInterval) ) + 1;

		//�t�H���g�ݒ�
		CFont* oldFont = pDC->SelectObject(&m_objDispFont);

		//�P�s�Â`��
		ULONG ulCnt = 0;
		if( m_bDispRuler ) //���[����\������
		{
			CRect	lineRect(0,0,ulRight,(m_lfHeight+m_lLineInterval));
			lineRect.NormalizeRect();
			if( rgnClipBox.RectInRegion(lineRect) ) //�����̈�ɓ����Ă���
			{
				pDC->SelectClipRgn(&rgnClipBox);
				pDC->FillRect(&lineRect,&objRulerBackBrush);
				//���[���`��
				MyDrawingRuler(pDC,0,0,ulRight,m_RulerColor,m_RulerBackColor);
			}
			ulCnt++;
			lineStop--;
		}
		ULONG ulLineNo=0;
		for ( ulLineNo=lineStart; ulLineNo<lineStop; ulLineNo++)
		{
			CRect	lineRect(0,(ulCnt*(m_lfHeight+m_lLineInterval)),ulRight,((ulCnt+1)*(m_lfHeight+m_lLineInterval)));
			lineRect.NormalizeRect();
			if( rgnClipBox.RectInRegion(lineRect) ) //�����̈�ɓ����Ă���
			{
				pDC->SelectClipRgn(&rgnClipBox);

				//�P�s�`��
				pDC->FillRect(&lineRect,&objBackBrush);
				CLine*  pobjLine = pDoc->GetOneLine(ulLineNo);
				MyTextOut(pDC,m_lLineNoWidth,(ulCnt*(m_lfHeight+m_lLineInterval)),pobjLine,m_lfWidth,ulLineNo);
			}
			ulCnt++;
		}
		if(m_bDispUnderLine) //�L�����b�g�s�A���_�[���C���`��
		{
			MyDrawingUnderLine(pDC);
		}

		//�t�H���g�����Ƃɖ߂�
		pDC->SelectObject(oldFont);

		//�����̈�`��t���O�𗎂Ƃ��Ă���
		m_bUseClipRgn = FALSE;
	}
	else if( m_bNoUseOffBuffer ) //�I�t�X�N���[���o�b�t�@���g�p���Ȃ��ŕ`�悷��ꍇ
	{
		//�w�i�F�u���V�쐬
		CBrush	objBackBrush;
		objBackBrush.CreateSolidBrush(m_BackColor);
		CBrush	objRulerBackBrush;
		objRulerBackBrush.CreateSolidBrush(m_RulerBackColor);

		//�ő啝�v�Z
		ULONG ulRight = 0;
		if( ((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth) > clientRect.Width() )
			ulRight = (ULONG)((pDoc->GetMaxLineLength() * 1.2)*m_lfWidth);
		else
			ulRight = clientRect.Width();

		//�s�ԍ��\���t�B�[���h�̕�
		if( m_bDispLineNo ) //�s�ԍ���\������
			m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		else
			m_lLineNoWidth = 0;

		//�`��J�n�s�ƕ`��I���s���v�Z
		ULONG	lineStart, lineStop;	//�`��s�ԍ�
		lineStart = (ULONG)((ULONG)GetScrollPosLogY() / (ULONG)(m_lfHeight+m_lLineInterval));
		lineStop  = lineStart + (ULONG)((ULONG)clientRect.Height() / (ULONG)(m_lfHeight+m_lLineInterval)) + 1;

		//�t�H���g�ݒ�
		CFont* oldFont = pDC->SelectObject(&m_objDispFont);

		//�P�s�Â`��
		ULONG ulCnt = 0;
		if( m_bDispRuler ) //���[����\������
		{
			CRect	lineRect(0,0,ulRight,(m_lfHeight+m_lLineInterval));
			lineRect.NormalizeRect();
			pDC->FillRect(&lineRect,&objRulerBackBrush);
			MyDrawingRuler(pDC,0,0,ulRight,m_RulerColor,m_RulerBackColor);
			ulCnt++;
			lineStop--;
		}
		ULONG ulLineNo;
		for ( ulLineNo=lineStart; ulLineNo<lineStop; ulLineNo++)
		{
			//�w�i�F�œh��Ԃ�
			CRect	lineRect(0,(ulCnt*(m_lfHeight+m_lLineInterval)),ulRight,((ulCnt+1)*(m_lfHeight+m_lLineInterval)));
			lineRect.NormalizeRect();

			pDC->FillRect(&lineRect,&objBackBrush);
			CLine*  pobjLine = pDoc->GetOneLine(ulLineNo);
			MyTextOut(pDC,m_lLineNoWidth,(ulCnt*(m_lfHeight+m_lLineInterval)),pobjLine,m_lfWidth,ulLineNo);

			ulCnt++;
		}
		if( m_bDispUnderLine )//�A���_�[���C����`�悷��ꍇ
		{
			MyDrawingUnderLine(pDC);
		}

		//�t�H���g�����Ƃɖ߂�
		pDC->SelectObject(oldFont);

		//�I�t�X�N���[���o�b�t�@�s�g�p�t���O�𗎂Ƃ��Ă���
		m_bNoUseOffBuffer = FALSE;
	}
	else
	{
		//�I�t�X�N���[���o�b�t�@�X�V
		UpdateOffBuffer();

		//�I�t�X�N���[���o�b�t�@����ʂ֓]��
		CRect clientRect;
		GetClientRect(&clientRect);
		clientRect.NormalizeRect();
		pDC->BitBlt( 0,						//�]����l�p�`�̍�����̘_�� x ���W
					 0,						//�]����l�p�`�̍�����̘_�� y ���W
					 m_nOffBufferWidth,		//�]����l�p�`�Ɠ]�����r�b�g�}�b�v�̕� (�_���P��)
					 clientRect.Height(),	//�]����l�p�`�Ɠ]�����r�b�g�}�b�v�̍��� (�_���P��)
					 &m_objOffBufferDC,		//�̽�ذ��ޯ̧��DC
					 0,						//�]�����r�b�g�}�b�v�̍�����̘_�� x ���W
					 0,						//�]�����r�b�g�}�b�v�̍�����̘_�� y ���W
					 SRCCOPY);				//���s׽����ڰ��݁i�]�����r�b�g�}�b�v��]����r�b�g�}�b�v�ɃR�s�[�j
	}

	m_bUseClipRgn = 0;				// �N���b�v���ꂽ���[�W�����̂ݕ`��
	m_bNoUseOffBuffer = 0;			// �I�t�X�N���[���o�b�t�@�s�g�p�t���O
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView �N���X�̈��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����������															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������t���O�I��
	if(pInfo->m_bPreview)
	{
		pDoc->SetFlagPrintPreview(TRUE);
	}

	//�f�t�H���g�̈�������i����_�C�A���O�I�[�v���j
	return DoPreparePrinting(pInfo);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����T�C�N������������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	//�v�����^�c�b�擾
	CDC* pobjPrinterDC = CDC::FromHandle(pDC->m_hAttribDC);

	//����p�t�H���g�쐬
	m_objPrintFont.CreatePointFont(m_lPrintFontPoint,m_objPrintFontName,pobjPrinterDC);

	//�P�y�[�W������̍s���͂܂��s��
	m_lLinePerPrintPage = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����T�C�N���I������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	//����p�t�H���g�j��
	m_objPrintFont.DeleteObject();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����v���r���[�I������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������t���O�I�t
	pDoc->SetFlagPrintPreview(FALSE);

	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�y�[�W�P�ʂ̈������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	long  lHeadLen=0;
	long  lMargin=0;
	long  lFootLen=0;
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//////////////////////////////////////
	//	����]���}�[�W������
	//////////////////////////////////////
	CRect objMarginRect;
	m_pApp->GetPrintMargin(objMarginRect);
	objMarginRect.NormalizeRect();
	int nHorzRes = pDC->GetDeviceCaps(LOGPIXELSX);
	int nVertRes = pDC->GetDeviceCaps(LOGPIXELSY);
	pInfo->m_rectDraw.left   += ::MulDiv(nHorzRes,objMarginRect.left,  1000);
	pInfo->m_rectDraw.top    += ::MulDiv(nVertRes,objMarginRect.top,   1000);
	pInfo->m_rectDraw.right  -= ::MulDiv(nHorzRes,objMarginRect.right, 1000);
	pInfo->m_rectDraw.bottom -= ::MulDiv(nVertRes,objMarginRect.bottom,1000);

	//////////////////////////////////////
	//	�󎚃t�H���g�I��
	//////////////////////////////////////
	CFont* pOldFont = (CFont*)pDC->SelectObject(&m_objPrintFont);

	//////////////////////////////////////
	//	�e�s�̍������v�Z
	//////////////////////////////////////
	TEXTMETRIC	tm;
	pDC->GetTextMetrics(&tm);
	m_lPrintLineHeight = tm.tmHeight + tm.tmExternalLeading;
	m_lPrintCharWidth  = tm.tmAveCharWidth;

	//�P�s�Ɉ󎚂ł��镶����
	long lCharCnt = pInfo->m_rectDraw.Width() / m_lPrintCharWidth;

	if(m_lLinePerPrintPage == 0)
	{
		//�P�y�[�W������̍s���v�Z
		m_lLinePerPrintPage = pInfo->m_rectDraw.Height() / m_lPrintLineHeight;
		m_lLinePerPrintPage -= 2; //�w�b�_�Q�s�����Ђ�
		m_lLinePerPrintPage -= 2; //�t�b�^�Q�s�����Ђ�

		//���y�[�W���v�Z
		UINT unMaxPage = CountPrintMaxPage(pInfo,m_lLinePerPrintPage,m_lPrintCharWidth);
		pInfo->SetMaxPage( unMaxPage );
	}

	//////////////////////////////////////
	//	�y�[�W�w�b�_��
	//////////////////////////////////////
	CString	objHeader = _T("");
	ParsePrintHeaderFormat(m_objStrHeaderFormat,objHeader,pInfo);
	if(!objHeader.IsEmpty())
	{
		if(m_nHeaderLayout==PRINT_HEADER_LEFT) //�w�b�_���悹
		{
			//�w�b�_���悹��
			pDC->TextOut(pInfo->m_rectDraw.left,pInfo->m_rectDraw.top,objHeader);
		}
		else if(m_nHeaderLayout==PRINT_HEADER_CENTER) //�w�b�_����
		{
			//�w�b�_�̕�����
			lHeadLen = objHeader.GetLength();
			//�}�[�W��
			lMargin = (lCharCnt - lHeadLen) / 2;
			if(lMargin<0)
				lMargin = 0;
			//�w�b�_������
			lMargin *= m_lPrintCharWidth;
			lMargin += pInfo->m_rectDraw.left;
			pDC->TextOut(lMargin,pInfo->m_rectDraw.top,objHeader);
		}
		else if(m_nHeaderLayout==PRINT_HEADER_RIGHT) //�w�b�_�E�悹
		{
			//�w�b�_�̕�����
			lHeadLen = objHeader.GetLength();
			//�}�[�W��
			lMargin = lCharCnt - lHeadLen;
			if(lMargin<0)
				lMargin = 0;
			//�w�b�_�E�悹��
			lMargin *= m_lPrintCharWidth;
			lMargin += pInfo->m_rectDraw.left;
			pDC->TextOut(lMargin,pInfo->m_rectDraw.top,objHeader);
		}
	}

	/////////////////////////////////////////
	//	�t�@�C���̓��e�P�y�[�W���o��
	/////////////////////////////////////////
	MyPrintOut1Page(pDC,pInfo,m_lLinePerPrintPage,m_lPrintLineHeight,m_lPrintCharWidth);

	//////////////////////////////////////
	//	�y�[�W�t�b�^��
	//////////////////////////////////////
	CString	objFooter = _T("");
	ParsePrintFooterFormat(m_objStrFooterFormat,objFooter,pInfo);
	if(!objFooter.IsEmpty())
	{
		if(m_nFooterLayout==PRINT_FOOTER_LEFT) //�t�b�^���悹
		{
			//�t�b�^���悹��
			pDC->TextOut( pInfo->m_rectDraw.left,
						  ((m_lLinePerPrintPage+4)*m_lPrintLineHeight) + pInfo->m_rectDraw.top,
						  objFooter);
		}
		else if(m_nFooterLayout==PRINT_FOOTER_CENTER) //�t�b�^����
		{
			//�t�b�^�̕�����
			lFootLen = objFooter.GetLength();
			//�}�[�W��
			lMargin = (lCharCnt - lFootLen) / 2;
			if(lMargin<0)
				lMargin = 0;
			//�t�b�^������
			lMargin *= m_lPrintCharWidth;
			lMargin += pInfo->m_rectDraw.left;
			pDC->TextOut( lMargin,
						  ((m_lLinePerPrintPage+4)*m_lPrintLineHeight) + pInfo->m_rectDraw.top,
						  objFooter);
		}
		else if(m_nFooterLayout==PRINT_FOOTER_RIGHT) //�t�b�^�E�悹
		{
			//�t�b�^�̕�����
			lFootLen = objFooter.GetLength();
			//�}�[�W��
			lMargin = lCharCnt - lFootLen;
			if(lMargin<0)
				lMargin = 0;
			//�t�b�^�E�悹��
			lMargin *= m_lPrintCharWidth;
			lMargin += pInfo->m_rectDraw.left;
			pDC->TextOut( lMargin,
						  ((m_lLinePerPrintPage+4)*m_lPrintLineHeight) + pInfo->m_rectDraw.top,
						  objFooter);
		}
	}

	///////////////////////////////////
	//	�t�H���g��߂�
	pDC->SelectObject(pOldFont);

//	CView::OnPrint(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView �N���X�̐f�f

#ifdef _DEBUG
void CEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEditorDoc* CEditorView::GetDocument() // ��f�o�b�O �o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEditorDoc)));
	return (CEditorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�h���b�O���h���b�v�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�h���b�O���h���b�v���A�E�C���h�E�ɍŏ��Ɉړ��������̏���				|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
DROPEFFECT CEditorView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	LONG	lCurrentLine=0;
	LONG	lCurrentOffset=0;
	CPoint	tempPos(0,0);
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() ||
		pWnd->IsNowStepDoOperation() 
	)
	{
		//�}�E�X����𖳎�
		return CView::OnDragEnter(pDataObject, dwKeyState, point);
	}

	if( pDataObject->IsDataAvailable(CF_TEXT) ) //�e�L�X�g�`��
	{
		//�h���b�v��̃r���[���A�N�e�B�u�ɂ���
		CSplitterWnd *spltWnd = (CSplitterWnd *)GetParent();
		spltWnd->SetActivePane(0, 0, this);
		((CMDIChildWnd *)spltWnd->GetParent())->MDIActivate();

		//�}�E�X�ʒu�@�|���@�s�A��ԍ��ϊ�
		lCurrentLine=0;
		lCurrentOffset=0;
		LONG lRet = MouseToLineOffset( point, &lCurrentLine, &lCurrentOffset, &tempPos );
		if(	lRet ==	0 || lRet == 2 )
		{
			//���N���X�̃n���h���R�[��
			return CView::OnDragEnter(pDataObject, dwKeyState, point);
		}

		//�L�����b�g�ʒu�����݂̃}�E�X�ʒu�ɐݒ�
		MySetCaretPos(tempPos);

		//�R���g���[���̏�Ԃɂ��A�R�s�[���ǂ�����ʒm
		return (dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
	}

	return CView::OnDragEnter(pDataObject, dwKeyState, point);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�h���b�O���h���b�v���A�E�C���h�E�O�Ɉړ��������̏���					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnDragLeave()
{
	//CF_TEXT �ł͓��ɂ��邱�Ƃ͖���
	CView::OnDragLeave();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�h���b�O���h���b�v���A�E�C���h�E�̊O�Ƀh���b�O���ꂽ���̏���			|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
DROPEFFECT CEditorView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	LONG	lCurrentLine=0;
	LONG	lCurrentOffset=0;
	CPoint	tempPos(0,0);
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() ||
		pWnd->IsNowStepDoOperation() 
	)
	{
		//�}�E�X����𖳎�
		return CView::OnDragOver(pDataObject, dwKeyState, point);
	}

	if( pDataObject->IsDataAvailable(CF_TEXT) ) //�e�L�X�g�`��
	{
		//�h���b�v��̃r���[���A�N�e�B�u�ɂ���
		CSplitterWnd *spltWnd = (CSplitterWnd *)GetParent();
		spltWnd->SetActivePane(0, 0, this);
		((CMDIChildWnd *)spltWnd->GetParent())->MDIActivate();

		//�}�E�X�ʒu�@�|���@�s�A��ԍ��ϊ�
		lCurrentLine=0;
		lCurrentOffset=0;
		LONG lRet = MouseToLineOffset( point, &lCurrentLine, &lCurrentOffset, &tempPos );
		if(lRet	== 0 ||	lRet ==	2)
		{
			//���N���X�̃n���h���R�[��
			return CView::OnDragOver(pDataObject, dwKeyState, point);
		}

		//�L�����b�g�ʒu�����݂̃}�E�X�ʒu�ɐݒ�
		MySetCaretPos(tempPos);

		//�R���g���[���̏�Ԃɂ��A�R�s�[���ǂ�����ʒm
		return (dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
	}
	return CView::OnDragOver(pDataObject, dwKeyState, point);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�h���b�v���삪�s���鎞�̏���										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return CView::OnDrop(pDataObject, dropEffect, point);
	}

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() ||
		pWnd->IsNowStepDoOperation() 
	)
	{
		//�}�E�X����𖳎�
		MessageBeep((UINT)-1);
		return CView::OnDrop(pDataObject, dropEffect, point);
	}

	if( pDataObject->IsDataAvailable(CF_TEXT) ) //�e�L�X�g�`��
	{
		//�h���b�v��̃r���[���A�N�e�B�u�ɂ���
		CSplitterWnd *spltWnd = (CSplitterWnd *)GetParent();
		spltWnd->SetActivePane(0, 0, this);
		((CMDIChildWnd *)spltWnd->GetParent())->MDIActivate();

		//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
		if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
			return CView::OnDrop(pDataObject, dropEffect, point);

		//�͈͑I���ʒu����
		AdjustSelectArea();

		//�h���b�v����
		CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
		pDoc->ProcessOleDrop( pDataObject,				// �n�k�d�f�[�^
							  lCurrentLine, 			// �L�����b�g�ʒu�̍s
							  lCurrentOffset,			// �L�����b�g�ʒu�̃I�t�Z�b�g
							  m_caretPos,				// �L�����b�g�ʒu
							  objScrollPosLog,			// �X�N���[���ʒu
							  m_bDispRuler,				// ���[���\���t���O
							  m_bDispLineNo );			// �s�ԍ��\���t���O

		//�ĕ`��
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);

		return dropEffect;
	}

	return CView::OnDrop(pDataObject, dropEffect, point);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�\���֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|���t���b�V���v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewRedraw()
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	CRect	clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	InvalidateRect(clientRect,TRUE);
}
void CEditorView::OnUpdateViewRedraw(CCmdUI* pCmdUI)
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|�s�ԍ���\���v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewLineno()
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//�t���O���]
	m_bDispLineNo = (m_bDispLineNo!=FALSE) ? FALSE : TRUE;

	//�g���q�ʐݒ�ۑ�
	CString objKey;
	CString objFileExt = pDoc->MyGetFileExt();
	if(objFileExt.IsEmpty())
		objKey = "FileExt_none";
	if(!objFileExt.CompareNoCase("txt"))
		objKey = "FileExt_txt";
	else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
			!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
		objKey = "FileExt_htm html shtml shtm";
	else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
		objKey = "FileExt_c cpp";
	else if(!objFileExt.CompareNoCase("h"))
		objKey = "FileExt_h";
	else if(!objFileExt.CompareNoCase("asm"))
		objKey = "FileExt_asm";
	else if(!objFileExt.CompareNoCase("asp"))
		objKey = "FileExt_asp";
	else if(!objFileExt.CompareNoCase("frm"))
		objKey = "FileExt_frm";
	else if(!objFileExt.CompareNoCase("bas"))
		objKey = "FileExt_bas";
	else if(!objFileExt.CompareNoCase("pc"))
		objKey = "FileExt_pc";
	else if(!objFileExt.CompareNoCase("log"))
		objKey = "FileExt_log";
	else if(!objFileExt.CompareNoCase("dat"))
		objKey = "FileExt_dat";
	else if(!objFileExt.CompareNoCase("bat"))
		objKey = "FileExt_bat";
	else if(!objFileExt.CompareNoCase("ini"))
		objKey = "FileExt_ini";
	else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
		objKey = "FileExt_mak mk";
	else if(!objFileExt.CompareNoCase("java"))
		objKey = "FileExt_java";
	else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
		objKey = "FileExt_y l";
	else if(!objFileExt.CompareNoCase("wrl"))
		objKey = "FileExt_wrl";
	else
		objKey = "FileExt_other";
	m_pApp->WriteProfileInt(objKey,"bDispLineNo",m_bDispLineNo);
	m_pApp->SetFlag2AllWindow(objFileExt,
							  m_bDispLineNo,
							  m_bDispRuler,
							  m_bDispCRLF,
							  m_bDispTab,
							  m_bDispZenkakuSpace,
							  m_bDispUnderLine);

	if( m_bDispLineNo )
	{
		//�L�����b�g�ʒu
		m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		m_LineNoPos = CPoint(m_lLineNoWidth,0);
		CPoint objPoint = m_caretPos + m_LineNoPos;
		MySetCaretPos(objPoint);
	}
	else
	{
		//�L�����b�g�ʒu
		CPoint objPoint = m_caretPos - m_LineNoPos;
		m_lLineNoWidth = 0;
		m_LineNoPos = CPoint(0,0);
		MySetCaretPos(objPoint);
	}

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateViewLineno(CCmdUI* pCmdUI)
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_bDispLineNo)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|���[����\���v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewRuler()
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//�t���O���]
	m_bDispRuler = (m_bDispRuler!=FALSE) ? FALSE : TRUE;

	//�g���q�ʐݒ�ۑ�
	CString objKey;
	CString objFileExt = pDoc->MyGetFileExt();
	if(objFileExt.IsEmpty())
		objKey = "FileExt_none";
	if(!objFileExt.CompareNoCase("txt"))
		objKey = "FileExt_txt";
	else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
			!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
		objKey = "FileExt_htm html shtml shtm";
	else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
		objKey = "FileExt_c cpp";
	else if(!objFileExt.CompareNoCase("h"))
		objKey = "FileExt_h";
	else if(!objFileExt.CompareNoCase("asm"))
		objKey = "FileExt_asm";
	else if(!objFileExt.CompareNoCase("asp"))
		objKey = "FileExt_asp";
	else if(!objFileExt.CompareNoCase("frm"))
		objKey = "FileExt_frm";
	else if(!objFileExt.CompareNoCase("bas"))
		objKey = "FileExt_bas";
	else if(!objFileExt.CompareNoCase("pc"))
		objKey = "FileExt_pc";
	else if(!objFileExt.CompareNoCase("log"))
		objKey = "FileExt_log";
	else if(!objFileExt.CompareNoCase("dat"))
		objKey = "FileExt_dat";
	else if(!objFileExt.CompareNoCase("bat"))
		objKey = "FileExt_bat";
	else if(!objFileExt.CompareNoCase("ini"))
		objKey = "FileExt_ini";
	else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
		objKey = "FileExt_mak mk";
	else if(!objFileExt.CompareNoCase("java"))
		objKey = "FileExt_java";
	else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
		objKey = "FileExt_y l";
	else if(!objFileExt.CompareNoCase("wrl"))
		objKey = "FileExt_wrl";
	else
		objKey = "FileExt_other";
	m_pApp->WriteProfileInt(objKey,"bDispRuler",m_bDispRuler);
	m_pApp->SetFlag2AllWindow(objFileExt,
							  m_bDispLineNo,
							  m_bDispRuler,
							  m_bDispCRLF,
							  m_bDispTab,
							  m_bDispZenkakuSpace,
							  m_bDispUnderLine);

	if( m_bDispRuler )
	{
		//�L�����b�g�ʒu
		m_RulerPos = CPoint(0,(m_lfHeight+m_lLineInterval));
		CPoint objPoint = m_caretPos + m_RulerPos;
		MySetCaretPos(objPoint);
	}
	else
	{
		//�L�����b�g�ʒu
		CPoint objPoint = m_caretPos - m_RulerPos;
		m_RulerPos = CPoint(0,0);
		MySetCaretPos(objPoint);
	}

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateViewRuler(CCmdUI* pCmdUI)
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_bDispRuler)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|���s�}�[�N��\���v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewCrlf()
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//�t���O���]
	m_bDispCRLF = (m_bDispCRLF!=FALSE) ? FALSE : TRUE;

	//�g���q�ʐݒ�ۑ�
	CString objKey;
	CString objFileExt = pDoc->MyGetFileExt();
	if(objFileExt.IsEmpty())
		objKey = "FileExt_none";
	if(!objFileExt.CompareNoCase("txt"))
		objKey = "FileExt_txt";
	else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
			!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
		objKey = "FileExt_htm html shtml shtm";
	else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
		objKey = "FileExt_c cpp";
	else if(!objFileExt.CompareNoCase("h"))
		objKey = "FileExt_h";
	else if(!objFileExt.CompareNoCase("asm"))
		objKey = "FileExt_asm";
	else if(!objFileExt.CompareNoCase("asp"))
		objKey = "FileExt_asp";
	else if(!objFileExt.CompareNoCase("frm"))
		objKey = "FileExt_frm";
	else if(!objFileExt.CompareNoCase("bas"))
		objKey = "FileExt_bas";
	else if(!objFileExt.CompareNoCase("pc"))
		objKey = "FileExt_pc";
	else if(!objFileExt.CompareNoCase("log"))
		objKey = "FileExt_log";
	else if(!objFileExt.CompareNoCase("dat"))
		objKey = "FileExt_dat";
	else if(!objFileExt.CompareNoCase("bat"))
		objKey = "FileExt_bat";
	else if(!objFileExt.CompareNoCase("ini"))
		objKey = "FileExt_ini";
	else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
		objKey = "FileExt_mak mk";
	else if(!objFileExt.CompareNoCase("java"))
		objKey = "FileExt_java";
	else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
		objKey = "FileExt_y l";
	else if(!objFileExt.CompareNoCase("wrl"))
		objKey = "FileExt_wrl";
	else
		objKey = "FileExt_other";
	m_pApp->WriteProfileInt(objKey,"bDispCRLF",m_bDispCRLF);
	m_pApp->SetFlag2AllWindow(objFileExt,
							  m_bDispLineNo,
							  m_bDispRuler,
							  m_bDispCRLF,
							  m_bDispTab,
							  m_bDispZenkakuSpace,
							  m_bDispUnderLine);

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateViewCrlf(CCmdUI* pCmdUI)
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_bDispCRLF)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|�^�u�L����\���v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewTab()
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//�t���O���]
	m_bDispTab = (m_bDispTab!=FALSE) ? FALSE : TRUE;

		//�g���q�ʐݒ�ۑ�
	CString objKey;
	CString objFileExt = pDoc->MyGetFileExt();
	if(objFileExt.IsEmpty())
		objKey = "FileExt_none";
	if(!objFileExt.CompareNoCase("txt"))
		objKey = "FileExt_txt";
	else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
			!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
		objKey = "FileExt_htm html shtml shtm";
	else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
		objKey = "FileExt_c cpp";
	else if(!objFileExt.CompareNoCase("h"))
		objKey = "FileExt_h";
	else if(!objFileExt.CompareNoCase("asm"))
		objKey = "FileExt_asm";
	else if(!objFileExt.CompareNoCase("asp"))
		objKey = "FileExt_asp";
	else if(!objFileExt.CompareNoCase("frm"))
		objKey = "FileExt_frm";
	else if(!objFileExt.CompareNoCase("bas"))
		objKey = "FileExt_bas";
	else if(!objFileExt.CompareNoCase("pc"))
		objKey = "FileExt_pc";
	else if(!objFileExt.CompareNoCase("log"))
		objKey = "FileExt_log";
	else if(!objFileExt.CompareNoCase("dat"))
		objKey = "FileExt_dat";
	else if(!objFileExt.CompareNoCase("bat"))
		objKey = "FileExt_bat";
	else if(!objFileExt.CompareNoCase("ini"))
		objKey = "FileExt_ini";
	else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
		objKey = "FileExt_mak mk";
	else if(!objFileExt.CompareNoCase("java"))
		objKey = "FileExt_java";
	else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
		objKey = "FileExt_y l";
	else if(!objFileExt.CompareNoCase("wrl"))
		objKey = "FileExt_wrl";
	else
		objKey = "FileExt_other";
	m_pApp->WriteProfileInt(objKey,"bDispTab",m_bDispTab);
	m_pApp->SetFlag2AllWindow(objFileExt,
							  m_bDispLineNo,
							  m_bDispRuler,
							  m_bDispCRLF,
							  m_bDispTab,
							  m_bDispZenkakuSpace,
							  m_bDispUnderLine);

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateViewTab(CCmdUI* pCmdUI)
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_bDispTab)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|�S�p�X�y�[�X��\���v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewZenkakuspace()
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//�t���O���]
	m_bDispZenkakuSpace = (m_bDispZenkakuSpace!=FALSE) ? FALSE : TRUE;

		//�g���q�ʐݒ�ۑ�
	CString objKey;
	CString objFileExt = pDoc->MyGetFileExt();
	if(objFileExt.IsEmpty())
		objKey = "FileExt_none";
	if(!objFileExt.CompareNoCase("txt"))
		objKey = "FileExt_txt";
	else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
			!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
		objKey = "FileExt_htm html shtml shtm";
	else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
		objKey = "FileExt_c cpp";
	else if(!objFileExt.CompareNoCase("h"))
		objKey = "FileExt_h";
	else if(!objFileExt.CompareNoCase("asm"))
		objKey = "FileExt_asm";
	else if(!objFileExt.CompareNoCase("asp"))
		objKey = "FileExt_asp";
	else if(!objFileExt.CompareNoCase("frm"))
		objKey = "FileExt_frm";
	else if(!objFileExt.CompareNoCase("bas"))
		objKey = "FileExt_bas";
	else if(!objFileExt.CompareNoCase("pc"))
		objKey = "FileExt_pc";
	else if(!objFileExt.CompareNoCase("log"))
		objKey = "FileExt_log";
	else if(!objFileExt.CompareNoCase("dat"))
		objKey = "FileExt_dat";
	else if(!objFileExt.CompareNoCase("bat"))
		objKey = "FileExt_bat";
	else if(!objFileExt.CompareNoCase("ini"))
		objKey = "FileExt_ini";
	else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
		objKey = "FileExt_mak mk";
	else if(!objFileExt.CompareNoCase("java"))
		objKey = "FileExt_java";
	else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
		objKey = "FileExt_y l";
	else if(!objFileExt.CompareNoCase("wrl"))
		objKey = "FileExt_wrl";
	else
		objKey = "FileExt_other";
	m_pApp->WriteProfileInt(objKey,"bDispZenkakuSpace",m_bDispZenkakuSpace);
	m_pApp->SetFlag2AllWindow(objFileExt,
							  m_bDispLineNo,
							  m_bDispRuler,
							  m_bDispCRLF,
							  m_bDispTab,
							  m_bDispZenkakuSpace,
							  m_bDispUnderLine);

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateViewZenkakuspace(CCmdUI* pCmdUI)
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_bDispZenkakuSpace)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|�A���_�[���C����\���v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewUnderline()
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//�t���O���]
	m_bDispUnderLine = (m_bDispUnderLine!=FALSE) ? FALSE : TRUE;

	//�g���q�ʐݒ�ۑ�
	CString objKey;
	CString objFileExt = pDoc->MyGetFileExt();
	if(objFileExt.IsEmpty())
		objKey = "FileExt_none";
	if(!objFileExt.CompareNoCase("txt"))
		objKey = "FileExt_txt";
	else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
			!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
		objKey = "FileExt_htm html shtml shtm";
	else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
		objKey = "FileExt_c cpp";
	else if(!objFileExt.CompareNoCase("h"))
		objKey = "FileExt_h";
	else if(!objFileExt.CompareNoCase("asm"))
		objKey = "FileExt_asm";
	else if(!objFileExt.CompareNoCase("asp"))
		objKey = "FileExt_asp";
	else if(!objFileExt.CompareNoCase("frm"))
		objKey = "FileExt_frm";
	else if(!objFileExt.CompareNoCase("bas"))
		objKey = "FileExt_bas";
	else if(!objFileExt.CompareNoCase("pc"))
		objKey = "FileExt_pc";
	else if(!objFileExt.CompareNoCase("log"))
		objKey = "FileExt_log";
	else if(!objFileExt.CompareNoCase("dat"))
		objKey = "FileExt_dat";
	else if(!objFileExt.CompareNoCase("bat"))
		objKey = "FileExt_bat";
	else if(!objFileExt.CompareNoCase("ini"))
		objKey = "FileExt_ini";
	else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
		objKey = "FileExt_mak mk";
	else if(!objFileExt.CompareNoCase("java"))
		objKey = "FileExt_java";
	else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
		objKey = "FileExt_y l";
	else if(!objFileExt.CompareNoCase("wrl"))
		objKey = "FileExt_wrl";
	else
		objKey = "FileExt_other";
	m_pApp->WriteProfileInt(objKey,"bDispUnderLine",m_bDispUnderLine);
	m_pApp->SetFlag2AllWindow(objFileExt,
							  m_bDispLineNo,
							  m_bDispRuler,
							  m_bDispCRLF,
							  m_bDispTab,
							  m_bDispZenkakuSpace,
							  m_bDispUnderLine);

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateViewUnderline(CCmdUI* pCmdUI)
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_bDispUnderLine)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|�d�n�e�}�[�N��\���v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewEof()
{
	BOOL bFlg = m_pApp->GetProfileInt("CEditorView","bIsDispEOFMark",1);
	bFlg = (bFlg==1) ? 0 : 1;
	m_pApp->WriteProfileInt("CEditorView","bIsDispEOFMark",bFlg);

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateViewEof(CCmdUI* pCmdUI)
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	BOOL bFlg = m_pApp->GetProfileInt("CEditorView","bIsDispEOFMark",1);
	if(bFlg == 1)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|�L�����b�g�`��F�c�_�v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewCaretIbeam()
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(m_nCaretType==CARET_IBEAM) //���łɏc�_�ɂȂ��Ă���
		return;
	if(!m_bInsertKey)
		return;

	m_nCaretType = CARET_IBEAM;

	HideCaret();								//�L�����b�g��\��
	CreateSolidCaret(m_lfWidth/4, m_lfHeight); 	//�c�_�L�����b�g�쐬
	ShowCaret();								//�L�����b�g�\��
	MySetCaretPos(m_caretPos);					//�L�����b�g�ʒu�ݒ�
}
void CEditorView::OnUpdateViewCaretIbeam(CCmdUI* pCmdUI)
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_nCaretType==CARET_IBEAM)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�\���|�L�����b�g�`��F���^�v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnViewCaretSquare()
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(m_nCaretType==CARET_SQUARE) //���łɔ��^�ɂȂ��Ă���
		return;
	if(!m_bInsertKey)
		return;

	m_nCaretType = CARET_SQUARE;

	HideCaret();								//�L�����b�g��\��
	CreateSolidCaret(m_lfWidth, m_lfHeight/2); 	//���^�L�����b�g�쐬
	ShowCaret();								//�L�����b�g�\��
	MySetCaretPos(m_caretPos);					//�L�����b�g�ʒu�ݒ�
}
void CEditorView::OnUpdateViewCaretSquare(CCmdUI* pCmdUI)
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_nCaretType==CARET_SQUARE)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�R�s�[�A�؂���A�\��t���֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|�R�s�[�v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditCopy()
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITCOPY);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�n�k�d�f�[�^�\�[�X�쐬
	COleDataSource*	pobjOleDataSource = new COleDataSource();

	//�R�s�[����f�[�^�쐬
	AdjustSelectArea();
	CSharedFile objShareFile(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
	if(m_lSelectStartLine == m_lSelectEndLine) //�P�s
	{
		CLine* pobjLine = (CLine*)pDoc->GetOneLine(m_lSelectStartLine);
		objShareFile.Write( pobjLine->Data(m_lSelectStartOffset),
							(m_lSelectEndOffset-m_lSelectStartOffset) );
	}
	else //�����s
	{
		for(ULONG lLineNo=m_lSelectStartLine; lLineNo<=(ULONG)m_lSelectEndLine; lLineNo++)
		{
			CLine* pobjLine = (CLine*)pDoc->GetOneLine(lLineNo);
			if(!pobjLine)
				continue;
			if(lLineNo == (ULONG)m_lSelectStartLine)//�I���J�n�s
			{
				objShareFile.Write(	pobjLine->Data(m_lSelectStartOffset),
									(pobjLine->GetLength()-m_lSelectStartOffset) );
			}
			else if(lLineNo == (ULONG)m_lSelectEndLine)//�I���I���s
			{
				objShareFile.Write( "\r\n", 2 );
				objShareFile.Write( pobjLine->String(), m_lSelectEndOffset );
			}
			else //�r���s
			{
				objShareFile.Write( "\r\n", 2 );
				objShareFile.Write( pobjLine->String(), pobjLine->GetLength() );
			}
		}
	}

	//�N���b�v�{�[�h��
	pobjOleDataSource->CacheGlobalData(CF_TEXT, objShareFile.Detach());
	pobjOleDataSource->SetClipboard();
}
void CEditorView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|�؂���v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditCut()
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITCUT);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�͈͑I���ʒu����
	AdjustSelectArea();
	//�I��͈̓e�L�X�g�폜
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->DeleteSelectedArea(m_lSelectStartLine,		//�I���J�n�s
							 m_lSelectStartOffset,		//�I���J�n�I�t�Z�b�g
							 m_lSelectEndLine,			//�I���I���s
							 m_lSelectEndOffset,		//�I���I���I�t�Z�b�g
							 TRUE,						//�N���b�v�{�[�h�փR�s�[����
							 m_caretPos,				//�L�����b�g�ʒu
							 objScrollPosLog,			//�X�N���[���ʒu
							 m_bDispRuler,
							 m_bDispLineNo );

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|�\��t���v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditPaste()
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITPASTE);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//�͈͑I���ʒu����
	AdjustSelectArea();

	//����t������
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessPaste(	lCurrentLine,			//�L�����b�g�ʒu�̍s
						lCurrentOffset,			//�L�����b�g�ʒu�̃I�t�Z�b�g
						m_lSelectStartLine,		//�I���J�n�s
						m_lSelectStartOffset,	//�I���J�n�I�t�Z�b�g
						m_lSelectEndLine,		//�I���I���s
						m_lSelectEndOffset,		//�I���I���I�t�Z�b�g
						m_caretPos,				//�L�����b�g�ʒu
						objScrollPosLog,		//�X�N���[���ʒu
						m_bDispRuler,
						m_bDispLineNo );

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

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
/*|	�T�v	�F�u�ҏW�|�폜�v�I��������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditDel()
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITDEL);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//�͈͑I���ʒu����
	//�I��͈̓e�L�X�g�폜
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->DeleteSelectedArea(m_lSelectStartLine,
							 m_lSelectStartOffset,
							 m_lSelectEndLine,
							 m_lSelectEndOffset,
							 FALSE,
							 m_caretPos,
							 objScrollPosLog,
							 m_bDispRuler,
							 m_bDispLineNo );

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateEditDel(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|���ׂđI���v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditSelectall()
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITSELECTALL);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�ŏI�s�փW�����v
	OnEditJump2bottom();

	//�I���J�n�ʒu�ƑI���I���ʒu��ݒ肷��
	m_lSelectStartLine = 0;
	m_lSelectStartOffset = 0;
	m_lSelectEndLine = pDoc->GetLineCount() - 1;
	CLine* pobjTempLine = pDoc->GetOneLine( m_lSelectEndLine );
	if( pobjTempLine != NULL )
		m_lSelectEndOffset = pobjTempLine->GetLength();
	else
		m_lSelectEndOffset = 0;

	//�L�����b�g�ʒu�ݒ�
	MySetCaretToLineOffset(m_lSelectEndLine,m_lSelectEndOffset);

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateEditSelectall(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�h�L�������g�ɂP�s���f�[�^���Ȃ��ꍇ�͑I��s�ɂ���
	if( pDoc->IsEmpty() )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|�t�q�k���u���E�Y�v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditBrowseurl() 
{
	CString objMsg;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//�P�s�擾
	CLine* pobjLine = (CLine*)pDoc->GetOneLine(lCurrentLine);
	if(pobjLine == NULL)
		return;

	//�L�����b�g�ʒu�̂t�q�k�擾
	CString objUrl=_T("");
	if(pobjLine->GetUrl(lCurrentOffset,objUrl) == FALSE)
	{
		objMsg.LoadString(IDS_NOTABOVEURL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//�t�q�k���v�d�a�u���E�U�ŕ\��
	m_pApp->BrowseUrl(objUrl);
}
void CEditorView::OnUpdateEditBrowseurl(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�h�L�������g�ɂP�s���f�[�^���Ȃ��ꍇ�͑I��s�ɂ���
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�t�q�k��`�F�b�N
	if(pDoc->IsAboveURL(lCurrentLine,lCurrentOffset))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�|���[���[���N���v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditExecmailer() 
{
	CString objMsg;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//�P�s�擾
	CLine* pobjLine = (CLine*)pDoc->GetOneLine(lCurrentLine);
	if(pobjLine == NULL)
		return;

	//�L�����b�g�ʒu�̂��|���������擾
	CString objEMail=_T("");
	if(pobjLine->GetEMail(lCurrentOffset,objEMail) == FALSE)
	{
		objMsg.LoadString(IDS_NOTABOVEEMAIL);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//���[���[���N��
	m_pApp->ExecMailer(objEMail);
}
void CEditorView::OnUpdateEditExecmailer(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�h�L�������g�ɂP�s���f�[�^���Ȃ��ꍇ�͑I��s�ɂ���
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//���|����������`�F�b�N
	if(pDoc->IsAboveEMail(lCurrentLine,lCurrentOffset))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i���t�����}���֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@���t�}���v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditInsday()
{
	CString objMsg;

	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITINSDAY);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//���t�\���X�^�C���擾
	long lStatusDayStyle = pWnd->GetStatusDayStyle();
	CTime	objTimeNow = CTime::GetCurrentTime();
	CString	objDayStr=_T("");
	if(lStatusDayStyle==STS_DAYSTYLE_NONE)
	{
		objMsg.LoadString(IDS_DAYSTYLE_NONE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	else if(lStatusDayStyle==STS_DAYSTYLE_YYYYMMDD)
		objDayStr = objTimeNow.Format( "%Y/%m/%d (%a)" );
	else if(lStatusDayStyle==STS_DAYSTYLE_YYMMDD)
		objDayStr = objTimeNow.Format( "'%y/%m/%d (%a)" );
	else if(lStatusDayStyle==STS_DAYSTYLE_MMDDYYYY)
		objDayStr = objTimeNow.Format( "%m/%d/%Y (%a)" );
	else if(lStatusDayStyle==STS_DAYSTYLE_MMDDYY)
		objDayStr = objTimeNow.Format( "%m/%d/'%y (%a)" );
	else if(lStatusDayStyle==STS_DAYSTYLE_MMDD)
		objDayStr = objTimeNow.Format( "%m/%d (%a)" );
	else if(lStatusDayStyle==STS_DAYSTYLE_DDMMYYYY)
		objDayStr = objTimeNow.Format( "%d/%m/%Y (%a)" );
	else if(lStatusDayStyle==STS_DAYSTYLE_DDMMYY)
		objDayStr = objTimeNow.Format( "%d/%m/'%y (%a)" );
	else if(lStatusDayStyle==STS_DAYSTYLE_DDMM)
		objDayStr = objTimeNow.Format( "%d/%m (%a)" );
	else if(lStatusDayStyle==STS_DAYSTYLE_YYYY_MM_DD)
	{
		objMsg.LoadString(IDS_DAYSTYLE_YYYY_MM_DD);
		objDayStr = objTimeNow.Format( (LPCTSTR)objMsg );
	}
	else if(lStatusDayStyle==STS_DAYSTYLE_YY_MM_DD)
	{
		objMsg.LoadString(IDS_DAYSTYLE_YY_MM_DD);
		objDayStr = objTimeNow.Format( (LPCTSTR)objMsg );
	}
	else if(lStatusDayStyle==STS_DAYSTYLE_MM_DD)
	{
		objMsg.LoadString(IDS_DAYSTYLE_MM_DD);
		objDayStr = objTimeNow.Format( (LPCTSTR)objMsg );
	}
	//���t���L�����b�g�ʒu�֑}��
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessInsDay( lCurrentLine,			//�L�����b�g�ʒu�s
						 lCurrentOffset,		//�L�����b�g�ʒu�I�t�Z�b�g
						 objDayStr,				//���t������
						 m_caretPos,			//�L�����b�g�ʒu
						 objScrollPosLog,		//�X�N���[���ʒu
						 m_bDispRuler,			//���[���\���t���O
						 m_bDispLineNo );		//�s�ԍ��\���t���O

	//�L�����b�g�s�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	InvalidateCaretLine();
}
void CEditorView::OnUpdateEditInsday(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@�����}���v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditInstime()
{
	CString objMsg;

	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITINSTIME);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�����\���X�^�C���擾
	long lStatusHourStyle = pWnd->GetStatusHourStyle();
	CTime	objTimeNow = CTime::GetCurrentTime();
	CString objTimeStr=_T("");
	if(lStatusHourStyle==STS_HOURSTYLE_NONE)
	{
		objMsg.LoadString(IDS_HOURSTYLE_NONE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	else if(lStatusHourStyle==STS_HOURSTYLE_HHMM)
		objTimeStr = objTimeNow.Format( "%H:%M" );
	else if(lStatusHourStyle==STS_HOURSTYLE_AMPM_HHMM)
		objTimeStr = objTimeNow.Format( "%p %I:%M" );
	else if(lStatusHourStyle==STS_HOURSTYLE_HH_MM)
		objTimeStr = objTimeNow.Format( "%H��%M��" );
	else if(lStatusHourStyle==STS_HOURSTYLE_AMPM_HH_MM)
		objTimeStr = objTimeNow.Format( "%p %I��%M��" );

	//�������L�����b�g�ʒu�֑}��
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessInsTime(lCurrentLine,			//�L�����b�g�ʒu�s
						 lCurrentOffset,		//�L�����b�g�ʒu�I�t�Z�b�g
						 objTimeStr,			//����������
						 m_caretPos,			//�L�����b�g�ʒu
						 objScrollPosLog,		//�X�N���[���ʒu
						 m_bDispRuler,			//���[���\���t���O
						 m_bDispLineNo );		//�s�ԍ��\���t���O

	//�L�����b�g�s�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	InvalidateCaretLine();
}
void CEditorView::OnUpdateEditInstime(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@�}���^�㏑�����[�h�ؑւ��v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnChangeInsmode() 
{
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();

	m_bInsertKey = !m_bInsertKey;	//�}���^�㏑�����[�h�؊���
	if(m_bInsertKey)
	{
		if(m_nCaretType==CARET_IBEAM)
			CreateSolidCaret(m_lfWidth/4, m_lfHeight); //�}���L�����b�g�쐬
		else if(m_nCaretType==CARET_SQUARE)
			CreateSolidCaret(m_lfWidth, m_lfHeight/2); //�}���L�����b�g�쐬
	}
	else
	{
		CreateSolidCaret(m_lfWidth, m_lfHeight); //�㏑���L�����b�g�쐬
	}
	ShowCaret();					//�L�����b�g�\��
	MySetCaretPos(m_caretPos);		//�L�����b�g�ʒu�ݒ�
	//�X�e�[�^�X�o�[�y�C���ɑ}���A�㏑��Ӱ�ސݒ�
	pWnd->SetInsMode(m_bInsertKey);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�����ϊ��֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@�^�u�|�����p�X�y�[�X�ϊ��v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditTab2space()
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITTAB2SPACE);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�͈͑I���ʒu����
	AdjustSelectArea();
	//�^�u�|�����p�X�y�[�X�ϊ�
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessTab2Space( m_lSelectStartLine,		//�I���J�n�s
							m_lSelectStartOffset,	//�I���J�n�I�t�Z�b�g
							m_lSelectEndLine,		//�I���I���s
							m_lSelectEndOffset,		//�I���I���I�t�Z�b�g
							m_caretPos,				//�L�����b�g�ʒu
							objScrollPosLog,		//�X�N���[���ʒu
							m_bDispRuler,			//���[���\���t���O
							m_bDispLineNo,			//�s�ԍ��\���t���O
							m_lfWidth );			//�t�H���g��

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditTab2space(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@���p�X�y�[�X�|���^�u�ϊ��v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditSpace2tab()
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITSPACE2TAB);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�͈͑I���ʒu����
	AdjustSelectArea();
	//���p�X�y�[�X�|���^�u�ϊ�
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessSpace2Tab( m_lSelectStartLine,		//�I���J�n�s
							m_lSelectStartOffset,	//�I���J�n�I�t�Z�b�g
							m_lSelectEndLine,		//�I���I���s
							m_lSelectEndOffset,		//�I���I���I�t�Z�b�g
							m_caretPos,				//�L�����b�g�ʒu
							objScrollPosLog,		//�X�N���[���ʒu
							m_bDispRuler,			//���[���\���t���O
							m_bDispLineNo,			//�s�ԍ��\���t���O
							m_lfWidth );			//�t�H���g��

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditSpace2tab(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@�S�p�X�y�[�X�|�����p�X�y�[�X�ϊ��v�I��������				|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditSpaceZen2han()
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITSPACEZEN2HAN);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�͈͑I���ʒu����
	AdjustSelectArea();
	//�S�p�X�y�[�X�|�����p�X�y�[�X�ϊ�
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessSpaceZen2Han( m_lSelectStartLine,	//�I���J�n�s
							m_lSelectStartOffset,	//�I���J�n�I�t�Z�b�g
							m_lSelectEndLine,		//�I���I���s
							m_lSelectEndOffset,		//�I���I���I�t�Z�b�g
							m_caretPos,				//�L�����b�g�ʒu
							objScrollPosLog,		//�X�N���[���ʒu
							m_bDispRuler,			//���[���\���t���O
							m_bDispLineNo,			//�s�ԍ��\���t���O
							m_lfWidth );			//�t�H���g��

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditSpaceZen2han(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@���p�X�y�[�X�|���S�p�X�y�[�X�ϊ��v�I��������				|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditSpaceHan2zen()
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITSPACEHAN2ZEN);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�͈͑I���ʒu����
	AdjustSelectArea();
	//���p�X�y�[�X�|���S�p�X�y�[�X�ϊ�
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessSpaceHan2Zen( m_lSelectStartLine,	//�I���J�n�s
							m_lSelectStartOffset,	//�I���J�n�I�t�Z�b�g
							m_lSelectEndLine,		//�I���I���s
							m_lSelectEndOffset,		//�I���I���I�t�Z�b�g
							m_caretPos,				//�L�����b�g�ʒu
							objScrollPosLog,		//�X�N���[���ʒu
							m_bDispRuler,			//���[���\���t���O
							m_bDispLineNo,			//�s�ԍ��\���t���O
							m_lfWidth );			//�t�H���g��

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditSpaceHan2zen(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@���p���Ł|���S�p�J�^�J�i�ϊ��v�I��������					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditKatakanaHan2zen()
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITKATAKANAHAN2ZEN);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�͈͑I���ʒu����
	AdjustSelectArea();
	//���p���Ł|���S�p�J�^�J�i�ϊ�
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessKatakanaHan2Zen( m_lSelectStartLine,	//�I���J�n�s
							m_lSelectStartOffset,	//�I���J�n�I�t�Z�b�g
							m_lSelectEndLine,		//�I���I���s
							m_lSelectEndOffset,		//�I���I���I�t�Z�b�g
							m_caretPos,				//�L�����b�g�ʒu
							objScrollPosLog,		//�X�N���[���ʒu
							m_bDispRuler,			//���[���\���t���O
							m_bDispLineNo,			//�s�ԍ��\���t���O
							m_lfWidth );			//�t�H���g��

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditKatakanaHan2zen(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@�S�p�J�^�J�i�|�����p���ŕϊ��v�I��������					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditKatakanaZen2han()
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITKATAKANAZEN2HAN);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�͈͑I���ʒu����
	AdjustSelectArea();
	//�S�p�J�^�J�i�|�����p���ŕϊ�
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessKatakanaZen2Han( m_lSelectStartLine,	//�I���J�n�s
							m_lSelectStartOffset,	//�I���J�n�I�t�Z�b�g
							m_lSelectEndLine,		//�I���I���s
							m_lSelectEndOffset,		//�I���I���I�t�Z�b�g
							m_caretPos,				//�L�����b�g�ʒu
							objScrollPosLog,		//�X�N���[���ʒu
							m_bDispRuler,			//���[���\���t���O
							m_bDispLineNo,			//�s�ԍ��\���t���O
							m_lfWidth );			//�t�H���g��

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditKatakanaZen2han(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@���p�p�����|���S�p�p�����ϊ��v�I��������					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditAlnumHan2zen()
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITALNUMHAN2ZEN);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�͈͑I���ʒu����
	AdjustSelectArea();
	//���p�p�����|���S�p�p�����ϊ�
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessAlnumHan2Zen( m_lSelectStartLine,	//�I���J�n�s
							m_lSelectStartOffset,	//�I���J�n�I�t�Z�b�g
							m_lSelectEndLine,		//�I���I���s
							m_lSelectEndOffset,		//�I���I���I�t�Z�b�g
							m_caretPos,				//�L�����b�g�ʒu
							objScrollPosLog,		//�X�N���[���ʒu
							m_bDispRuler,			//���[���\���t���O
							m_bDispLineNo,			//�s�ԍ��\���t���O
							m_lfWidth );			//�t�H���g��

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditAlnumHan2zen(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@�S�p�p�����|�����p�p�����ϊ��v�I��������					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditAlnumZen2han()
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITALNUMZEN2HAN);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�͈͑I���ʒu����
	AdjustSelectArea();
	//�S�p�p�����|�����p�p�����ϊ�
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessAlnumZen2Han( m_lSelectStartLine,	//�I���J�n�s
							m_lSelectStartOffset,	//�I���J�n�I�t�Z�b�g
							m_lSelectEndLine,		//�I���I���s
							m_lSelectEndOffset,		//�I���I���I�t�Z�b�g
							m_caretPos,				//�L�����b�g�ʒu
							objScrollPosLog,		//�X�N���[���ʒu
							m_bDispRuler,			//���[���\���t���O
							m_bDispLineNo,			//�s�ԍ��\���t���O
							m_lfWidth );			//�t�H���g��

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditAlnumZen2han(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@�p���|���啶���v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditAl2upper()
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITAL2UPPER);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�͈͑I���ʒu����
	AdjustSelectArea();
	//�p���|���啶���ϊ�
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessAl2Upper(  m_lSelectStartLine,	    //�I���J�n�s
							m_lSelectStartOffset,	//�I���J�n�I�t�Z�b�g
							m_lSelectEndLine,		//�I���I���s
							m_lSelectEndOffset,		//�I���I���I�t�Z�b�g
							m_caretPos,				//�L�����b�g�ʒu
							objScrollPosLog,		//�X�N���[���ʒu
							m_bDispRuler,			//���[���\���t���O
							m_bDispLineNo );		//�s�ԍ��\���t���O

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditAl2upper(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@�p���|���������v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditAl2lower()
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITAL2LOWER);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�͈͑I���ʒu����
	AdjustSelectArea();
	//�p���|���������ϊ�
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessAl2Lower(  m_lSelectStartLine,	    //�I���J�n�s
							m_lSelectStartOffset,	//�I���J�n�I�t�Z�b�g
							m_lSelectEndLine,		//�I���I���s
							m_lSelectEndOffset,		//�I���I���I�t�Z�b�g
							m_caretPos,				//�L�����b�g�ʒu
							objScrollPosLog,		//�X�N���[���ʒu
							m_bDispRuler,			//���[���\���t���O
							m_bDispLineNo );		//�s�ԍ��\���t���O

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditAl2lower(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@�擪�̃X�y�[�X�A�^�u�폜�v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditDeltopspaces()
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITDELTOPSPACES);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�͈͑I���ʒu����
	AdjustSelectArea();
	//�擪�̃X�y�[�X�A�^�u�폜
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessDelTopSpaces( m_lSelectStartLine,
							  m_lSelectStartOffset,
							  m_lSelectEndLine,
							  m_lSelectEndOffset,
							  m_caretPos,
							  objScrollPosLog,
							  m_bDispRuler,
							  m_bDispLineNo );

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditDeltopspaces(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@�^�u�}���v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditInstab()
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITINSTAB);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//�͈͑I���ʒu����
	//�͈͑I�����Ă����Ԃł̂s�`�a�L�[����
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessTabWhenSelected( m_lSelectStartLine,
								  m_lSelectStartOffset,
								  m_lSelectEndLine,
								  m_lSelectEndOffset,
								  m_caretPos,
								  objScrollPosLog,
								  m_bDispRuler,
								  m_bDispLineNo );

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditInstab(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@�^�u�폜�v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditDeltab()
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITDELTAB);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//�͈͑I���ʒu����
	//�͈͑I�����Ă����Ԃł̂r���������{�s�`�a�L�[����
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessShiftTabWhenSelected( m_lSelectStartLine,
									  m_lSelectStartOffset,
									  m_lSelectEndLine,
									  m_lSelectEndOffset,
									  m_caretPos,
									  objScrollPosLog,
									  m_bDispRuler,
									  m_bDispLineNo );

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditDeltab(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@�b�^�b�{�{�s�R�����g�}���v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditInscppcomment()
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITINSCPPCOMMENT);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//�͈͑I���ʒu����
	//�͈͑I�����Ă����Ԃł̂b�^�b�{�{�R�����g�}��
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessInsCppComment( m_lSelectStartLine,
								m_lSelectStartOffset,
								m_lSelectEndLine,
								m_lSelectEndOffset,
								m_caretPos,
								objScrollPosLog,
								m_bDispRuler,
								m_bDispLineNo );

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditInscppcomment(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@�b�^�b�{�{�s�R�����g�폜�v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditDelcppcomment() 
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITDELCPPCOMMENT);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//�͈͑I���ʒu����
	//�͈͑I�����Ă����Ԃł̂b�^�b�{�{�R�����g�폜
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessDelCppComment( m_lSelectStartLine,
								m_lSelectStartOffset,
								m_lSelectEndLine,
								m_lSelectEndOffset,
								m_caretPos,
								objScrollPosLog,
								m_bDispRuler,
								m_bDispLineNo );

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditDelcppcomment(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@�u�a�s�R�����g�}���v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditInsvbcomment()
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITINSVBCOMMENT);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//�͈͑I���ʒu����
	//�͈͑I�����Ă����Ԃł̂u�a�R�����g�}��
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessInsVbComment(  m_lSelectStartLine,
								m_lSelectStartOffset,
								m_lSelectEndLine,
								m_lSelectEndOffset,
								m_caretPos,
								objScrollPosLog,
								m_bDispRuler,
								m_bDispLineNo );

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditInsvbcomment(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@�u�a�s�R�����g�폜�v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditDelvbcomment() 
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITDELVBCOMMENT);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//�͈͑I���ʒu����
	//�͈͑I�����Ă����Ԃł̂u�a�R�����g�폜
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessDelVbComment(  m_lSelectStartLine,
								m_lSelectStartOffset,
								m_lSelectEndLine,
								m_lSelectEndOffset,
								m_caretPos,
								objScrollPosLog,
								m_bDispRuler,
								m_bDispLineNo );

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditDelvbcomment(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@�A�Z���u���s�R�����g�}���v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditInsasmcomment()
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITINSASMCOMMENT);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//�͈͑I���ʒu����
	//�͈͑I�����Ă����Ԃł̃A�Z���u���R�����g�}��
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessInsAsmComment( m_lSelectStartLine,
								m_lSelectStartOffset,
								m_lSelectEndLine,
								m_lSelectEndOffset,
								m_caretPos,
								objScrollPosLog,
								m_bDispRuler,
								m_bDispLineNo );

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditInsasmcomment(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@�A�Z���u���s�R�����g�폜�v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditDelasmcomment() 
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITDELASMCOMMENT);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//�͈͑I���ʒu����
	//�͈͑I�����Ă����Ԃł̃A�Z���u���R�����g�폜
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessDelAsmComment( m_lSelectStartLine,
								m_lSelectStartOffset,
								m_lSelectEndLine,
								m_lSelectEndOffset,
								m_caretPos,
								objScrollPosLog,
								m_bDispRuler,
								m_bDispLineNo );

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}

void CEditorView::OnUpdateEditDelasmcomment(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@���p�����}���v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditInsquote() 
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITINSQUOTE);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//�͈͑I���ʒu����
	//�͈͑I�����Ă����Ԃł̈��p�����}��
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessInsQuote( m_lSelectStartLine,
						   m_lSelectStartOffset,
						   m_lSelectEndLine,
						   m_lSelectEndOffset,
						   m_caretPos,
						   objScrollPosLog,
						   m_bDispRuler,
						   m_bDispLineNo );

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditInsquote(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ҏW�@�|�@���p�����폜�v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditDelquote() 
{
	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;
	if(!IsSelected())
		return;

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		MessageBeep((UINT)-1);
		return;
	}

	//���b�Z�[�W�|���v
	m_pApp->MessagePump();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITDELQUOTE);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	AdjustSelectArea();//�͈͑I���ʒu����
	//�͈͑I�����Ă����Ԃł̈��p�����폜
	CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
	pDoc->ProcessDelQuote( m_lSelectStartLine,
						   m_lSelectStartOffset,
						   m_lSelectEndLine,
						   m_lSelectEndOffset,
						   m_caretPos,
						   objScrollPosLog,
						   m_bDispRuler,
						   m_bDispLineNo );

	//�I��̈�̂ݍĕ`��
	OnUpdate(NULL,NULL,NULL);
	AdjustSelectArea();
	long lRedrawLineCnt=0;
	if(m_lSelectStartLine == lCurrentLine)
		lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
	else if(m_lSelectEndLine == lCurrentLine)
		lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
	InvalidateCaretLineEx(lRedrawLineCnt);
}
void CEditorView::OnUpdateEditDelquote(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�����݋֎~�̏ꍇ�͖������ɂ���
	if(pDoc->IsReadOnly())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�͈͑I������Ă���ꍇ�ɂ������j���[�I���ł���悤�ɂ���
	if( IsSelected() )
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�W�����v�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�W�����v�|�擪�s�փW�����v�v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditJump2top()
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITJUMP2TOP);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�I���J�n�ʒu�ƑI���I���ʒu��ݒ肷��
	m_lSelectStartLine = 0;
	m_lSelectStartOffset = 0;
	m_lSelectEndLine = 0;
	CLine* pobjLine = pDoc->GetOneLine( m_lSelectEndLine );
	if( pobjLine != NULL )
		m_lSelectEndOffset = pobjLine->GetLength();
	else
		m_lSelectEndOffset = 0;

	m_bUseClipRgn = 0;
	m_bNoUseOffBuffer = 0;
	MyScrollToPosition(CPoint(0,0));
	MySetCaretPos(CPoint(m_LineNoPos.x,m_RulerPos.y));

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateEditJump2top(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�h�L�������g�ɂP�s���f�[�^���Ȃ��ꍇ�͑I��s�ɂ���
	if( pDoc->IsEmpty() )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FCtrl+Shift+Home�L�[�i�擪�s�֑I���W�����v�j����������					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditJump2topselect() 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	long lCurrentLine = 0;
	long lCurrentOffset = 0;

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
		return;
	if(pDoc->IsEmpty())
		return;

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITJUMP2TOPSELECT);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�W�����v�O�L�����b�g�ʒu��I���J�n�ʒu�ɂ���
	lCurrentLine=0;
	lCurrentOffset=0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	m_lSelectStartLine = lCurrentLine;
	m_lSelectStartOffset = lCurrentOffset;
	m_lSelectEndLine = 0;
	m_lSelectEndOffset = 0;

	//�W�����v
	m_bUseClipRgn = 0;
	m_bNoUseOffBuffer = 0;
	MyScrollToPosition(CPoint(0,0));
	MySetCaretPos(CPoint(m_LineNoPos.x,m_RulerPos.y));

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�W�����v�|�ŏI�s�փW�����v�v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditJump2bottom()
{
	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITJUMP2BOTTOM);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	CRect	clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();

	long lLineCnt = clientRect.Height() / (m_lfHeight+m_lLineInterval);	//�N���C�A���g�̈�ɕ\���ł���s��
	long lMaxLine = pDoc->GetLineCount();								//�h�L�������g�̑��s��

	//�I���J�n�ʒu�ƑI���I���ʒu��ݒ肷��
	m_lSelectStartLine = lMaxLine-1;
	m_lSelectStartOffset = 0;
	m_lSelectEndLine = lMaxLine-1;
	CLine* pobjLine = pDoc->GetOneLine( m_lSelectEndLine );
	if( pobjLine != NULL )
		m_lSelectEndOffset = pobjLine->GetLength();
	else
		m_lSelectEndOffset = 0;

	m_bUseClipRgn = 0;
	m_bNoUseOffBuffer = 0;
	if( lLineCnt < lMaxLine )
	{
		long lY = (lMaxLine - lLineCnt) * (m_lfHeight+m_lLineInterval);
		MyScrollToPosition(CPoint(0,lY));//�_�����W�Ŏw��
		long lcaretY = (lLineCnt-1)*(m_lfHeight+m_lLineInterval) + m_RulerPos.y;
		MySetCaretPos(CPoint(m_LineNoPos.x,lcaretY)); //�f�o�C�X���W�Ŏw��
	}
	else
	{
		MyScrollToPosition(CPoint(0,0));
		long lcaretY = (lMaxLine-1)*(m_lfHeight+m_lLineInterval) + m_RulerPos.y;
		MySetCaretPos(CPoint(m_LineNoPos.x,lcaretY)); //�f�o�C�X���W�Ŏw��
	}

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateEditJump2bottom(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�h�L�������g�ɂP�s���f�[�^���Ȃ��ꍇ�͑I��s�ɂ���
	if( pDoc->IsEmpty() )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FCtrl+Shift+End�L�[�i�ŏI�s�֑I���W�����v�j����������					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditJump2bottomselect() 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	long lCurrentLine = 0;
	long lCurrentOffset = 0;

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
		return;
	if(pDoc->IsEmpty())
		return;

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONEDITJUMP2BOTTOMSELECT);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	CRect	clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();

	long lLineCnt = clientRect.Height() / (m_lfHeight+m_lLineInterval);	//�N���C�A���g�̈�ɕ\���ł���s��
	long lMaxLine = pDoc->GetLineCount();								//�h�L�������g�̑��s��

	//�W�����v�O�L�����b�g�ʒu��I���J�n�ʒu�ɂ���
	lCurrentLine=0;
	lCurrentOffset=0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	m_lSelectStartLine = lCurrentLine;
	m_lSelectStartOffset = lCurrentOffset;

	//�I���I���ʒu��ݒ肷��
	m_lSelectEndLine = lMaxLine-1;
	CLine* pobjLine = pDoc->GetOneLine( m_lSelectEndLine );
	if( pobjLine != NULL )
		m_lSelectEndOffset = pobjLine->GetLength();
	else
		m_lSelectEndOffset = 0;

	m_bUseClipRgn = 0;
	m_bNoUseOffBuffer = 0;
	if( lLineCnt < lMaxLine )
	{
		long lY = (lMaxLine - lLineCnt) * (m_lfHeight+m_lLineInterval);
		MyScrollToPosition(CPoint(0,lY));//�_�����W�Ŏw��
		long lcaretY = (lLineCnt-1)*(m_lfHeight+m_lLineInterval) + m_RulerPos.y;
		MySetCaretPos(CPoint(m_LineNoPos.x,lcaretY)); //�f�o�C�X���W�Ŏw��
	}
	else
	{
		MyScrollToPosition(CPoint(0,0));
		long lcaretY = (lMaxLine-1)*(m_lfHeight+m_lLineInterval) + m_RulerPos.y;
		MySetCaretPos(CPoint(m_LineNoPos.x,lcaretY)); //�f�o�C�X���W�Ŏw��
	}

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�W�����v�|�w��s�փW�����v�v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditJump2line()
{
	CString objMsg;

	//������`�F�b�N
	CEditorDoc* pDoc = GetDocument();
	if(pDoc->GetFlagPrintPreview())
		return;

	CRect	clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();

	//�N���C�A���g�̈�ɕ\���ł���s��
	long lLineCnt = clientRect.Height() / (m_lfHeight+m_lLineInterval);
	//�h�L�������g�̑��s��
	long lMaxLine = pDoc->GetLineCount();
	//�L�����b�g�ʒu�s�ԍ�
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//�W�����v��s�ԍ����̓_�C�A���O�\��
	CLineJumpDlg	objLineJumpDlg;
	objLineJumpDlg.m_lLineNo = lCurrentLine+1;
	objLineJumpDlg.m_lMaxNo = lMaxLine;
	if( objLineJumpDlg.DoModal() != IDOK )
		return;
	if( (objLineJumpDlg.m_lLineNo<=0) ||
		(objLineJumpDlg.m_lLineNo>(UINT)objLineJumpDlg.m_lMaxNo)
	)
	{
		MessageBeep((UINT)-1);
		objMsg.LoadString(IDS_OUTOFJUMPRANGE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//�I���J�n�ʒu�ƑI���I���ʒu��ݒ肷��
	m_lSelectStartLine = objLineJumpDlg.m_lLineNo-1;
	m_lSelectStartOffset = 0;
	m_lSelectEndLine = objLineJumpDlg.m_lLineNo-1;
	CLine* pobjLine = pDoc->GetOneLine( m_lSelectEndLine );
	if( pobjLine != NULL )
		m_lSelectEndOffset = pobjLine->GetLength();
	else
		m_lSelectEndOffset = 0;

	m_bUseClipRgn = 0;
	m_bNoUseOffBuffer = 0;

	long lDlgNo = objLineJumpDlg.m_lLineNo;
	if( lLineCnt < lMaxLine )
	{
		if( (lDlgNo + lLineCnt) <= lMaxLine )
		{
			long lY = (lDlgNo - 1) * (m_lfHeight+m_lLineInterval);
			MyScrollToPosition(CPoint(0,lY));	//�_�����W�Ŏw��
			MySetCaretPos(CPoint(m_LineNoPos.x,m_RulerPos.y));	//�f�o�C�X���W�Ŏw��
		}
		else
		{
			long lY = (lMaxLine - lLineCnt) * (m_lfHeight+m_lLineInterval);
			MyScrollToPosition(CPoint(0,lY));	//�_�����W�Ŏw��
			long lTopLine = lMaxLine - lLineCnt;
			long lcaretY = (lDlgNo - 1 - lTopLine) * (m_lfHeight+m_lLineInterval) + m_RulerPos.y;
			MySetCaretPos(CPoint(m_LineNoPos.x,lcaretY)); //�f�o�C�X���W�Ŏw��
		}
	}
	else
	{
		MyScrollToPosition(CPoint(0,0)); //�_�����W�Ŏw��
		long lcaretY = (lDlgNo - 1) * (m_lfHeight+m_lLineInterval) + m_RulerPos.y;
		MySetCaretPos(CPoint(m_LineNoPos.x,lcaretY)); //�f�o�C�X���W�Ŏw��
	}

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateEditJump2line(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�h�L�������g�ɂP�s���f�[�^���Ȃ��ꍇ�͑I��s�ɂ���
	if( pDoc->IsEmpty() )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�W�����v�|�^�O�W�����v�v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditTagjump()
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CString objMsg;

	if( !pDoc->IsGrep() )
		return;

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	// �L�����b�g�ʒu�̍s�擾
	CLine*	pobjLine = NULL;
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
		return;

	//�f�q�d�o���ʉ���
	CString		objGrepFileName;
	long		lLineNo = 0;
	pobjLine->ParseGrepResult(objGrepFileName,&lLineNo);

	//�f�q�d�o�t�@�C���I�[�v��
	if( (_access( (LPCTSTR)objGrepFileName, 0 )) != -1 )
	{
		m_pApp->OpenDocumentFile((LPCTSTR)objGrepFileName);
		//�w��s�ԍ��ւ̃W�����v�w��
		m_pApp->SendJumpMsg((LPCTSTR)objGrepFileName,lLineNo);
	}
	else
	{
		char	szMsg[512];
		objMsg.LoadString(IDS_GREPFILE_NOTFOUND);
		sprintf(szMsg,(LPCTSTR)objMsg,(LPCTSTR)objGrepFileName);
		m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);
		objGrepFileName.ReleaseBuffer();
	}
}
void CEditorView::OnUpdateEditTagjump(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�f�q�d�o���ʕ\�����łȂ�
	if( !pDoc->IsGrep() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�h�L�������g�ɂP�s���f�[�^���Ȃ��ꍇ�͑I��s�ɂ���
	if( pDoc->IsEmpty() )
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�W�����v�|���ݍs���}�[�N�v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnJumpSetmark()
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//���ݍs���}�[�N
	pDoc->SetFlagMark(lCurrentLine,TRUE);

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	InvalidateCaretLine();//�L�����b�g�̂���P�s���ĕ`��
}
void CEditorView::OnUpdateJumpSetmark(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�h�L�������g�ɂP�s���f�[�^���Ȃ��ꍇ�͑I��s�ɂ���
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	BOOL bMarked = pDoc->GetFlagMark(lCurrentLine);
	if(bMarked) //�}�[�N�ς�
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�W�����v�|���ݍs�̃}�[�N�����v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnJumpDelmark()
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	//���ݍs�̃}�[�N����
	pDoc->SetFlagMark(lCurrentLine,FALSE);

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	InvalidateCaretLine();//�L�����b�g�̂���P�s���ĕ`��
}
void CEditorView::OnUpdateJumpDelmark(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�h�L�������g�ɂP�s���f�[�^���Ȃ��ꍇ�͑I��s�ɂ���
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	BOOL bMarked = pDoc->GetFlagMark(lCurrentLine);
	if(bMarked==FALSE) //�}�[�N���Ă��Ȃ�
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�W�����v�|���ׂẴ}�[�N�����v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnJumpAlldelmark()
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	ULONG lLineCnt=0;

	for(lLineCnt=0; lLineCnt<(ULONG)pDoc->GetLineCount(); lLineCnt++)
	{
		if(pDoc->GetFlagMark(lLineCnt))
		{
			pDoc->SetFlagMark(lLineCnt,FALSE);
		}
	}

	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	Invalidate(FALSE);
}
void CEditorView::OnUpdateJumpAlldelmark(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�h�L�������g�ɂP�s���f�[�^���Ȃ��ꍇ�͑I��s�ɂ���
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�}�[�N�ςݍs�������邩
	if(pDoc->GetMarkLineCount() < 1)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�W�����v�|���̃}�[�N�փW�����v�v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnJumpNextmark()
{
	CString objMsg;

	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	LONG	lLineCnt=0;
	long 	lMaxLine = pDoc->GetLineCount();

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONJUMPNEXTMARK);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	BOOL bFind=FALSE;
	for(lLineCnt=lCurrentLine+1; lLineCnt<pDoc->GetLineCount(); lLineCnt++)
	{
		if(pDoc->GetFlagMark(lLineCnt))
		{
			bFind=TRUE;
			break;
		}
	}
	if(bFind)
	{
		//���̃}�[�N�s�փW�����v
		Jump2LineOffset(lLineCnt,0);
		//�ĕ`��
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);
	}
	else
	{
		//�}�[�N�ς݂̍s�������Ȃ�
		objMsg.LoadString(IDS_NOMARKLINE_NEXT);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
	}
}
void CEditorView::OnUpdateJumpNextmark(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�h�L�������g�ɂP�s���f�[�^���Ȃ��ꍇ�͑I��s�ɂ���
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�}�[�N�ςݍs�������邩
	if(pDoc->GetMarkLineCount() < 1)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�W�����v�|�O�̃}�[�N�փW�����v�v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnJumpPrevmark()
{
	CString objMsg;
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONJUMPPREVMARK);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	LONG	lLineCnt=0;
	long 	lMaxLine = pDoc->GetLineCount();

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	BOOL bFind=FALSE;
	for(lLineCnt=lCurrentLine-1; lLineCnt>=0; lLineCnt--)
	{
		if(pDoc->GetFlagMark(lLineCnt))
		{
			bFind=TRUE;
			break;
		}
	}
	if(bFind)
	{
		//�O�̃}�[�N�s�փW�����v
		Jump2LineOffset(lLineCnt,0);
		//�ĕ`��
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);
	}
	else
	{
		//�}�[�N�ς݂̍s�������Ȃ�
		objMsg.LoadString(IDS_NOMARKLINE_PREV);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
	}
}
void CEditorView::OnUpdateJumpPrevmark(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�h�L�������g�ɂP�s���f�[�^���Ȃ��ꍇ�͑I��s�ɂ���
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�}�[�N�ςݍs�������邩
	if(pDoc->GetMarkLineCount() < 1)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�[�N�ʒu�ꗗ�\������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnJumpMarklist()
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() )
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONJUMPMARKLIST);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�}�[�N�ʒu�ꗗ�\��
	CMarkLineListDlg	objMarkLineListDlg;
	objMarkLineListDlg.Init(this,pDoc);
	int nRet = objMarkLineListDlg.DoModal();
	if(nRet == IDCANCEL)
		return;

	if(objMarkLineListDlg.m_lSelectedLineNo != 0)
	{
		//�}�[�N�s�փW�����v
		Jump2LineOffset(objMarkLineListDlg.m_lSelectedLineNo-1,0);
		//�ĕ`��
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);
	}
}
void CEditorView::OnUpdateJumpMarklist(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�h�L�������g�ɂP�s���f�[�^���Ȃ��ꍇ�͑I��s�ɂ���
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�}�[�N�ςݍs�������邩
	if(pDoc->GetMarkLineCount() < 1)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�|�b�v�A�b�v���j���[�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�{�b�v�A�b�v���j���[�\���n���h��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnContextMenu(CWnd*, CPoint point)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (point.x == -1 && point.y == -1)
	{
		//�L�[�X�g���[�N�̔���
		CRect rect;
		GetClientRect(rect);
		rect.NormalizeRect();
		ClientToScreen(rect);
		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CMenu* pPopup = m_popupmenu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	//////////////////////////////////////////////
	//		���j���[���ڑI���\�^�s�ݒ�		//
	//////////////////////////////////////////////

	//�u���ɖ߂��v���j���[�̐ݒ�
	if( pDoc->IsCanUndo() )
		pPopup->EnableMenuItem( ID_EDIT_UNDO, (MF_BYCOMMAND|MF_ENABLED) );
	else
		pPopup->EnableMenuItem( ID_EDIT_UNDO, (MF_BYCOMMAND|MF_GRAYED) );

	//�u�J��Ԃ��v���j���[�̐ݒ�
	if( pDoc->IsCanRedo() )
		pPopup->EnableMenuItem( ID_EDIT_REDO, (MF_BYCOMMAND|MF_ENABLED) );
	else
		pPopup->EnableMenuItem( ID_EDIT_REDO, (MF_BYCOMMAND|MF_GRAYED) );

	//�uUndo/Redo�o�b�t�@�N���A�v���j���[�̐ݒ�
	if( pDoc->IsCanUndo() || pDoc->IsCanRedo() )
		pPopup->EnableMenuItem( ID_EDIT_CLEARUNDOBUF, (MF_BYCOMMAND|MF_ENABLED) );
	else
		pPopup->EnableMenuItem( ID_EDIT_CLEARUNDOBUF, (MF_BYCOMMAND|MF_GRAYED) );

	//�u�؂���v���j���[�̐ݒ�
	if( IsSelected() && !pDoc->IsReadOnly() )
		pPopup->EnableMenuItem( ID_EDIT_CUT, (MF_BYCOMMAND|MF_ENABLED) );
	else
		pPopup->EnableMenuItem( ID_EDIT_CUT, (MF_BYCOMMAND|MF_GRAYED) );

	//�u�R�s�[�v���j���[�̐ݒ�
	if( IsSelected() )
		pPopup->EnableMenuItem( ID_EDIT_COPY, (MF_BYCOMMAND|MF_ENABLED) );
	else
		pPopup->EnableMenuItem( ID_EDIT_COPY, (MF_BYCOMMAND|MF_GRAYED) );

	//�u�\��t���v���j���[�̐ݒ�
	if( pDoc->IsReadOnly() )
	{
		pPopup->EnableMenuItem( ID_EDIT_PASTE, (MF_BYCOMMAND|MF_GRAYED) );
	}
	else
	{
		COleDataObject	objOleDataObject;
		BOOL bRet = objOleDataObject.AttachClipboard();
		if( bRet != 0 )
		{
			bRet = objOleDataObject.IsDataAvailable(CF_TEXT);
			if( bRet != 0 )
				pPopup->EnableMenuItem( ID_EDIT_PASTE, (MF_BYCOMMAND|MF_ENABLED) );
			else
				pPopup->EnableMenuItem( ID_EDIT_PASTE, (MF_BYCOMMAND|MF_GRAYED) );
		}
		else
			pPopup->EnableMenuItem( ID_EDIT_PASTE, (MF_BYCOMMAND|MF_GRAYED) );
	}

	//�u�폜�v���j���[�̐ݒ�
	if( IsSelected() && !pDoc->IsReadOnly() )
		pPopup->EnableMenuItem( ID_EDIT_DEL, (MF_BYCOMMAND|MF_ENABLED) );
	else
		pPopup->EnableMenuItem( ID_EDIT_DEL, (MF_BYCOMMAND|MF_GRAYED) );

	//�u���ׂđI���v���j���[�̐ݒ�
	if( pDoc->IsEmpty() )
		pPopup->EnableMenuItem( ID_EDIT_SELECTALL, (MF_BYCOMMAND|MF_ENABLED) );
	else
		pPopup->EnableMenuItem( ID_EDIT_SELECTALL, (MF_BYCOMMAND|MF_GRAYED) );

	//�u�N���b�v�{�[�h�̒��g������v���j���[�̐ݒ�
	COleDataObject	objOleDataObject;
	BOOL bRet = objOleDataObject.AttachClipboard();
	if( bRet != 0 )
	{
		bRet = objOleDataObject.IsDataAvailable(CF_TEXT);
		if( bRet != 0 )
			pPopup->EnableMenuItem( ID_EDIT_CLIPVIEW, (MF_BYCOMMAND|MF_ENABLED) );
		else
			pPopup->EnableMenuItem( ID_EDIT_CLIPVIEW, (MF_BYCOMMAND|MF_GRAYED) );
	}
	else
		pPopup->EnableMenuItem( ID_EDIT_CLIPVIEW, (MF_BYCOMMAND|MF_GRAYED) );

	//�u�t�q�k���u���E�Y�v���j���[�̐ݒ�
	pPopup->EnableMenuItem( ID_EDIT_BROWSEURL, (MF_BYCOMMAND|MF_ENABLED) );

	if( !pDoc->IsGrep()   && !pDoc->IsDump()    && !pDoc->IsConv()     &&
		!pDoc->IsWCount() && !pDoc->IsFSearch() && !pDoc->IsReadOnly() && 
		!pDoc->IsBinary() && !pDoc->IsEncript() && !pDoc->IsDecript()  &&
		!pDoc->IsEncode() && !pDoc->IsCompare() && !pDoc->IsDirCmp()
	)
	{
		//�u�����݋֎~�ɂ���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_SETREADONLY, (MF_BYCOMMAND|MF_ENABLED) );
		//�u�����݋֎~����������v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_UNSETREADONLY, (MF_BYCOMMAND|MF_ENABLED) );
	}
	else
	{
		//�u�����݋֎~�ɂ���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_SETREADONLY, (MF_BYCOMMAND|MF_GRAYED) );
		//�u�����݋֎~����������v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_UNSETREADONLY, (MF_BYCOMMAND|MF_GRAYED) );
	}


	if( !pDoc->IsReadOnly() )
	{
		//�u���t�}���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_INSDAY, (MF_BYCOMMAND|MF_ENABLED) );
		//�u�����}���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_INSTIME, (MF_BYCOMMAND|MF_ENABLED) );
	}
	else
	{
		//�u���t�}���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_INSDAY, (MF_BYCOMMAND|MF_GRAYED) );
		//�u�����}���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_INSTIME, (MF_BYCOMMAND|MF_GRAYED) );
	}

	if( IsSelected() && !pDoc->IsReadOnly() )
	{
		//�u�^�u�|�����p�X�y�[�X�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_TAB2SPACE, (MF_BYCOMMAND|MF_ENABLED) );
		//�u���p�X�y�[�X�|���^�u�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_SPACE2TAB, (MF_BYCOMMAND|MF_ENABLED) );
		//�u�S�p�X�y�[�X�|�����p�X�y�[�X�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_SPACE_ZEN2HAN, (MF_BYCOMMAND|MF_ENABLED) );
		//�u���p�X�y�[�X�|���S�p�X�y�[�X�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_SPACE_HAN2ZEN, (MF_BYCOMMAND|MF_ENABLED) );
		//�u���p���Ł|���S�p�J�^�J�i�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_KATAKANA_HAN2ZEN, (MF_BYCOMMAND|MF_ENABLED) );
		//�u�S�p�J�^�J�i�|�����p���Łv���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_KATAKANA_ZEN2HAN, (MF_BYCOMMAND|MF_ENABLED) );
		//�u���p�p�����|���S�p�p�����v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_ALNUM_HAN2ZEN, (MF_BYCOMMAND|MF_ENABLED) );
		//�u�S�p�p�����|�����p�p�����v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_ALNUM_ZEN2HAN, (MF_BYCOMMAND|MF_ENABLED) );
		//�u�p���|���啶���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_AL2UPPER, (MF_BYCOMMAND|MF_ENABLED) );
		//�u�p���|���������v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_AL2LOWER, (MF_BYCOMMAND|MF_ENABLED) );
		//�u�擪�̃X�y�[�X�A�^�u�폜�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_DELTOPSPACES, (MF_BYCOMMAND|MF_ENABLED) );
		//�u�^�u�}���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_INSTAB, (MF_BYCOMMAND|MF_ENABLED) );
		//�u�^�u�폜�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_DELTAB, (MF_BYCOMMAND|MF_ENABLED) );
		//�u�b�^�b�{�{�R�����g�}���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_INSCPPCOMMENT, (MF_BYCOMMAND|MF_ENABLED) );
		//�u�b�^�b�{�{�R�����g�폜�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_DELCPPCOMMENT, (MF_BYCOMMAND|MF_ENABLED) );
		//�u�u�a�R�����g�}���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_INSVBCOMMENT, (MF_BYCOMMAND|MF_ENABLED) );
		//�u�u�a�R�����g�폜�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_DELVBCOMMENT, (MF_BYCOMMAND|MF_ENABLED) );
		//�u�A�Z���u���R�����g�}���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_INSASMCOMMENT, (MF_BYCOMMAND|MF_ENABLED) );
		//�u�A�Z���u���R�����g�폜�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_DELASMCOMMENT, (MF_BYCOMMAND|MF_ENABLED) );
		//�u���p�����}���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_INSQUOTE, (MF_BYCOMMAND|MF_ENABLED) );
		//�u���p�����폜�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_DELQUOTE, (MF_BYCOMMAND|MF_ENABLED) );
	}
	else
	{
		//�u�^�u�|�����p�X�y�[�X�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_TAB2SPACE, (MF_BYCOMMAND|MF_GRAYED) );
		//�u���p�X�y�[�X�|���^�u�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_SPACE2TAB, (MF_BYCOMMAND|MF_GRAYED) );
		//�u�S�p�X�y�[�X�|�����p�X�y�[�X�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_SPACE_ZEN2HAN, (MF_BYCOMMAND|MF_GRAYED) );
		//�u���p�X�y�[�X�|���S�p�X�y�[�X�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_SPACE_HAN2ZEN, (MF_BYCOMMAND|MF_GRAYED) );
		//�u���p���Ł|���S�p�J�^�J�i�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_KATAKANA_HAN2ZEN, (MF_BYCOMMAND|MF_GRAYED) );
		//�u�S�p�J�^�J�i�|�����p���Łv���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_KATAKANA_ZEN2HAN, (MF_BYCOMMAND|MF_GRAYED) );
		//�u���p�p�����|���S�p�p�����v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_ALNUM_HAN2ZEN, (MF_BYCOMMAND|MF_GRAYED) );
		//�u�S�p�p�����|�����p�p�����v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_ALNUM_ZEN2HAN, (MF_BYCOMMAND|MF_GRAYED) );
		//�u�p���|���啶���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_AL2UPPER, (MF_BYCOMMAND|MF_GRAYED) );
		//�u�p���|���������v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_AL2LOWER, (MF_BYCOMMAND|MF_GRAYED) );
		//�u�擪�̃X�y�[�X�A�^�u�폜�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_DELTOPSPACES, (MF_BYCOMMAND|MF_GRAYED) );
		//�u�^�u�}���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_INSTAB, (MF_BYCOMMAND|MF_GRAYED) );
		//�u�^�u�폜�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_DELTAB, (MF_BYCOMMAND|MF_GRAYED) );
		//�u�b�^�b�{�{�R�����g�}���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_INSCPPCOMMENT, (MF_BYCOMMAND|MF_GRAYED) );
		//�u�b�^�b�{�{�R�����g�폜�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_DELCPPCOMMENT, (MF_BYCOMMAND|MF_GRAYED) );
		//�u�u�a�R�����g�}���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_INSVBCOMMENT, (MF_BYCOMMAND|MF_GRAYED) );
		//�u�u�a�R�����g�폜�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_DELVBCOMMENT, (MF_BYCOMMAND|MF_GRAYED) );
		//�u�A�Z���u���R�����g�}���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_INSASMCOMMENT, (MF_BYCOMMAND|MF_GRAYED) );
		//�u�A�Z���u���R�����g�폜�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_DELASMCOMMENT, (MF_BYCOMMAND|MF_GRAYED) );
		//�u���p�����}���v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_INSQUOTE, (MF_BYCOMMAND|MF_GRAYED) );
		//�u���p�����폜�v���j���[�̐ݒ�
		pPopup->EnableMenuItem( ID_EDIT_DELQUOTE, (MF_BYCOMMAND|MF_GRAYED) );
	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�E�C���h�E�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ޭ�����è�ށ^��è�ނɂȂ�Ƃ��ɂ�т�����鏈��					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();

	if (bActivate) //��A�N�e�B�u�[���A�N�e�B�u
	{
		if(m_bInsertKey)
		{
			if(m_nCaretType==CARET_IBEAM)
				CreateSolidCaret(m_lfWidth/4, m_lfHeight); //�}���L�����b�g�쐬
			else if(m_nCaretType==CARET_SQUARE)
				CreateSolidCaret(m_lfWidth, m_lfHeight/2); //�}���L�����b�g�쐬
		}
		else
		{
			CreateSolidCaret(m_lfWidth, m_lfHeight); //�㏑���L�����b�g�쐬
		}
		//�L�����b�g�\��
		ShowCaret();
		//�L�����b�g�_�ŊԊu��ݒ�
		::SetCaretBlinkTime(m_unCaretBlinkTime);
		//�L�����b�g�ʒu�ݒ�
		MySetCaretPos(m_caretPos);
		//�X�e�[�^�X�o�[�y�C���ݒ�
		pWnd->SetInsMode(m_bInsertKey);
		pWnd->SetReadOnlyMode(pDoc->IsReadOnly());
	}
	else //�A�N�e�B�u�[����A�N�e�B�u
	{
		//�L�����b�g�_�ŊԊu���I���W�i���l�ɖ߂�
		::SetCaretBlinkTime(m_pApp->m_unOrgCaretBlinkTime);
		//�L�����b�g��\��
		HideCaret();
		//�X�e�[�^�X�o�[�y�C���N���A
		pWnd->ClearGyoRetu();
		pWnd->ClearCaretPosByte();
		pWnd->ClearInsMode();
		pWnd->ClearReadOnlyMode();
		pWnd->ClearCaretPosCharCode();
	}

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E���쐬����O�ɌĂяo����鏈��								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B
	return CView::PreCreateWindow(cs);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�̍쐬��v�����鎞�ɌĂяo����鏈��						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//�̽�ذ��ޯ̧�쐬
	CreateOffScreenBuf();

	//�r���[�E�C���h�E���h���b�v�^�[�Q�b�g�Ƃ��ēo�^
	if( m_objOleDropTarget.Register(this) )
		return 0;
	else
		return -1;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E���j������鎞�ɌĂяo����鏈��							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnDestroy()
{
	CView::OnDestroy();

	//�̽�ذ��ޯ̧�j��
	DestroyOffScreenBuf();
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�X�V�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ŏ����ޭ����޷���Ăɱ������ꂽ��ɌĂяo����鏈��					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnInitialUpdate()
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//�\���t�H���g�쐬
	CClientDC	dc(this);
	m_objDispFont.DeleteObject();
	m_objDispFont.CreatePointFont(m_lDispFontPoint,m_objDispFontName,&dc);
	//�����t���\���t�H���g�쐬
	LOGFONT logfont;
	memset(&logfont,0,sizeof(LOGFONT));
	m_objDispFont.GetLogFont(&logfont);
	logfont.lfUnderline = TRUE;
	m_objDispFontUnderLine.DeleteObject();
	m_objDispFontUnderLine.CreateFontIndirect(&logfont);

	//�����̍����ƕ��v�Z
	CFont* oldFont = dc.SelectObject(&m_objDispFont);
	TEXTMETRIC	tm;
	dc.GetTextMetrics(&tm);
	m_lfHeight = tm.tmHeight + tm.tmExternalLeading;
	m_lfWidth  = tm.tmAveCharWidth + tm.tmOverhang;
	dc.SelectObject(oldFont);

	//�X�e�[�^�X�o�[�y�C���ɑ}���A�㏑��Ӱ�ސݒ�
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	pWnd->SetInsMode(m_bInsertKey);

	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	// CG: ���̍s�� �uIME �T�|�[�g�v �R���|�[�l���g�ɂ��ǉ�����Ă��܂��B
	::ImmAssociateContext(m_hWnd, m_hImc);
	if(m_pApp->GetProfileInt("CEditorView","IsImmAutoOpen",FALSE))
	{
		::ImmSetOpenStatus(m_hImc,TRUE);
		this->PostMessage(WM_IME_NOTIFY,IMN_SETOPENSTATUS,0);
	}

	//�͈͑I������
	m_lSelectStartLine   = -1;
	m_lSelectStartOffset = -1;
	m_lSelectEndLine     = -1;
	m_lSelectEndOffset   = -1;

	//�s�ԍ��\���֘A������
	if( m_bDispLineNo )
	{
		m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		m_LineNoPos = CPoint(m_lLineNoWidth,0);
	}
	else
	{
		m_lLineNoWidth = 0;
		m_LineNoPos = CPoint(0,0);
	}

	//���[���\���֘A������
	if( m_bDispRuler )
		m_RulerPos = CPoint(0,(m_lfHeight+m_lLineInterval));
	else
		m_RulerPos = CPoint(0,0);

	//�L�����b�g�ʒu�����ݒ�
	m_caretPos = CPoint(m_LineNoPos.x,m_RulerPos.y);

	//���N���X�̃n���h���R�[��
	CView::OnInitialUpdate();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�޷���Ă������ްĂ��ꂽ�Ƃ��ɌĂяo����鏈��							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//////////////////////////////////////
	//		�̽�ذ��ޯ̧�쐬���Ȃ���	//
	//////////////////////////////////////

	DestroyOffScreenBuf();
	CreateOffScreenBuf();

	//////////////////////////////////
	//		�����̍����ƕ��v�Z		//
	//////////////////////////////////

	CClientDC	dc(this);
	CFont* oldFont = dc.SelectObject(&m_objDispFont);
	TEXTMETRIC	tm;
	dc.GetTextMetrics(&tm);
	m_lfHeight = tm.tmHeight + tm.tmExternalLeading;
	m_lfWidth  = tm.tmAveCharWidth + tm.tmOverhang;
	dc.SelectObject(oldFont);
	if( (m_lfWidth==0) || (m_lfHeight==0) )
	{
		//�[���X�N���[���T�C�Y�ݒ�
		MySetScrollSizes(MM_TEXT,		// ϯ��ݸ�Ӱ��
						CSize(0,0),		// İ�ٻ���
						CSize(0,0),		// ������۰��ް���ĸد��ł̽�۰ٻ���
						CSize(0,0),		// ������۰��ް���د��ł̽�۰ٻ���
						CSize(0,0),		// ������۰��ް���ĸد��ł̽�۰ٻ���
						CSize(0,0)		// ������۰��ް���د��ł̽�۰ٻ���
		);
		//���N���X�̃n���h���R�[��
		CView::OnUpdate(pSender,lHint,pHint);
		return;
	}

	//�s�ԍ��\���֘A������
	if( m_bDispLineNo )
	{
		m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		m_LineNoPos = CPoint(m_lLineNoWidth,0);
	}
	else
	{
		m_lLineNoWidth = 0;
		m_LineNoPos = CPoint(0,0);
	}

	//���[���\���֘A������
	if( m_bDispRuler )
		m_RulerPos = CPoint(0,(m_lfHeight+m_lLineInterval));
	else
		m_RulerPos = CPoint(0,0);

	//////////////////////////////////////
	//	�N���C�A���g�̈�̍����ƕ�		//
	//////////////////////////////////////

	CRect clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//////////////////////////////////////
	//	�N���C�A���g�̈�ɕ\���ł���	//
	//	�s���ƕ����������߂�			//
	//////////////////////////////////////

	long lCanDispLineCnt = nHeight / (m_lfHeight+m_lLineInterval);	// �N���C�A���g�̈�ɕ\���ł���s��
	long lCanDispCharCnt = nWidth  / m_lfWidth;						// �N���C�A���g�̈�ɕ\���ł��镶����

	//////////////////////////////////////
	//		�X�N���[���T�C�Y�v�Z		//
	//////////////////////////////////////

	//�g�[�^���T�C�Y�v�Z
	CSize sizeTotal(0,0);
	if( pDoc->GetMaxLineLength() > 0 )
	{
		sizeTotal.cx = (pDoc->GetMaxLineLength() + 1) * m_lfWidth;
		sizeTotal.cx += clientRect.Width()  % m_lfWidth;//���܂�����Z
	}
	else
	{
		sizeTotal.cx = 0;
	}
	if( pDoc->GetLineCount() > 0 )
	{
		sizeTotal.cy = (pDoc->GetLineCount() + 1) * (m_lfHeight+m_lLineInterval);
		sizeTotal.cy += clientRect.Height() % (m_lfHeight+m_lLineInterval);	//���܂�����Z
	}
	else
	{
		sizeTotal.cy = 0;
	}

	//�y�[�W�T�C�Y�v�Z
	CSize sizeVPage;						// ������۰��ް���ĸد��ł̽�۰ٻ���
	CSize sizeHPage;						// ������۰��ް���ĸد��ł̽�۰ٻ���
	if( pDoc->GetLineCount() > 10 )
		sizeVPage = CSize(0,((m_lfHeight+m_lLineInterval)*10));
	else
		sizeVPage = CSize(0,((m_lfHeight+m_lLineInterval)*pDoc->GetLineCount()));
	if( pDoc->GetMaxLineLength() > 5 )
		sizeHPage = CSize((m_lfWidth*5),0);
	else
		sizeHPage = CSize((m_lfWidth*pDoc->GetMaxLineLength()),0);

	//�s�T�C�Y�v�Z
	CSize sizeVLine;						// ������۰��ް���د��ł̽�۰ٻ���
	CSize sizeHLine;						// ������۰��ް���د��ł̽�۰ٻ���
	if( pDoc->GetLineCount() > 0 )
		sizeVLine = CSize(0,(m_lfHeight+m_lLineInterval));
	else
		sizeVLine = CSize(0,0);
	if( pDoc->GetMaxLineLength() > 0 )
		sizeHLine = CSize(m_lfWidth,0);
	else
		sizeHLine = CSize(0,0);

	//////////////////////////////////////
	//		�X�N���[���T�C�Y�ݒ�		//
	//////////////////////////////////////

	MySetScrollSizes(MM_TEXT,		// ϯ��ݸ�Ӱ��
					sizeTotal,		// İ�ٻ���
					sizeVPage,		// ������۰��ް���ĸد��ł̽�۰ٻ���
					sizeVLine,		// ������۰��ް���د��ł̽�۰ٻ���
					sizeHPage,		// ������۰��ް���ĸد��ł̽�۰ٻ���
					sizeHLine		// ������۰��ް���د��ł̽�۰ٻ���
	);

	//���N���X�̃n���h���R�[��
	CView::OnUpdate(pSender,lHint,pHint);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�r�h�y�d����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnSize(UINT nType, int cx, int cy)
{
	//���N���X�̃n���h���R�[��
	CView::OnSize(nType, cx, cy);

	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//////////////////////////////////////
	//		�̽�ذ��ޯ̧�쐬���Ȃ���	//
	//////////////////////////////////////

	DestroyOffScreenBuf();
	CreateOffScreenBuf();

	//////////////////////////////////
	//		�����̍����ƕ��v�Z		//
	//////////////////////////////////

	CClientDC	dc(this);
	CFont* oldFont = dc.SelectObject(&m_objDispFont);
	TEXTMETRIC	tm;
	dc.GetTextMetrics(&tm);
	m_lfHeight = tm.tmHeight + tm.tmExternalLeading;
	m_lfWidth  = tm.tmAveCharWidth + tm.tmOverhang;
	dc.SelectObject(oldFont);
	if( (m_lfWidth==0) || (m_lfHeight==0) )
	{
		//�[���X�N���[���T�C�Y�ݒ�
		MySetScrollSizes(MM_TEXT,		// ϯ��ݸ�Ӱ��
						CSize(0,0),		// İ�ٻ���
						CSize(0,0),		// ������۰��ް���ĸد��ł̽�۰ٻ���
						CSize(0,0),		// ������۰��ް���د��ł̽�۰ٻ���
						CSize(0,0),		// ������۰��ް���ĸد��ł̽�۰ٻ���
						CSize(0,0)		// ������۰��ް���د��ł̽�۰ٻ���
		);
		return;
	}

	//�s�ԍ��\���֘A������
	if( m_bDispLineNo )
	{
		m_lLineNoWidth = strlen(m_szLineNoFormat) * m_lfWidth;
		m_LineNoPos = CPoint(m_lLineNoWidth,0);
	}
	else
	{
		m_lLineNoWidth = 0;
		m_LineNoPos = CPoint(0,0);
	}

	//���[���\���֘A������
	if( m_bDispRuler )
		m_RulerPos = CPoint(0,(m_lfHeight+m_lLineInterval));
	else
		m_RulerPos = CPoint(0,0);

	//////////////////////////////////////
	//	�N���C�A���g�̈�̍����ƕ�		//
	//////////////////////////////////////

	CRect	clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//////////////////////////////////////
	//	�N���C�A���g�̈�ɕ\���ł���	//
	//	�s���ƕ����������߂�			//
	//////////////////////////////////////

	long lCanDispLineCnt = nHeight / (m_lfHeight+m_lLineInterval);	//�N���C�A���g�̈�ɕ\���ł���s��
	long lCanDispCharCnt = nWidth  / m_lfWidth;						//�N���C�A���g�̈�ɕ\���ł��镶����

	//////////////////////////////////////
	//		�X�N���[���T�C�Y�v�Z		//
	//////////////////////////////////////

	//�g�[�^���T�C�Y�v�Z
	CSize sizeTotal(0,0);
	if( pDoc->GetMaxLineLength() > 0 )
	{
		sizeTotal.cx = (pDoc->GetMaxLineLength() + 1) * m_lfWidth;
		sizeTotal.cx += clientRect.Width()  % m_lfWidth;//���܂�����Z
	}
	else
	{
		sizeTotal.cx = 0;
	}
	if( pDoc->GetLineCount() > 0 )
	{
		sizeTotal.cy = (pDoc->GetLineCount() + 1) * (m_lfHeight+m_lLineInterval);
		sizeTotal.cy += clientRect.Height() % (m_lfHeight+m_lLineInterval);	//���܂�����Z
	}
	else
	{
		sizeTotal.cy = 0;
	}

	//�y�[�W�T�C�Y�v�Z
	CSize sizeVPage;						// ������۰��ް���ĸد��ł̽�۰ٻ���
	CSize sizeHPage;						// ������۰��ް���ĸد��ł̽�۰ٻ���
	if( pDoc->GetLineCount() > 10 )
		sizeVPage = CSize(0,((m_lfHeight+m_lLineInterval)*10));
	else
		sizeVPage = CSize(0,((m_lfHeight+m_lLineInterval)*pDoc->GetLineCount()));
	if( pDoc->GetMaxLineLength() > 5 )
		sizeHPage = CSize((m_lfWidth*5),0);
	else
		sizeHPage = CSize((m_lfWidth*pDoc->GetMaxLineLength()),0);

	//�s�T�C�Y�v�Z
	CSize sizeVLine;						// ������۰��ް���د��ł̽�۰ٻ���
	CSize sizeHLine;						// ������۰��ް���د��ł̽�۰ٻ���
	if( pDoc->GetLineCount() > 0 )
		sizeVLine = CSize(0,(m_lfHeight+m_lLineInterval));
	else
		sizeVLine = CSize(0,0);
	if( pDoc->GetMaxLineLength() > 0 )
		sizeHLine = CSize(m_lfWidth,0);
	else
		sizeHLine = CSize(0,0);

	//////////////////////////////////////
	//		�X�N���[���T�C�Y�ݒ�		//
	//////////////////////////////////////

	MySetScrollSizes(MM_TEXT,		// ϯ��ݸ�Ӱ��
					sizeTotal,		// İ�ٻ���
					sizeVPage,		// ������۰��ް���ĸد��ł̽�۰ٻ���
					sizeVLine,		// ������۰��ް���د��ł̽�۰ٻ���
					sizeHPage,		// ������۰��ް���ĸد��ł̽�۰ٻ���
					sizeHLine		// ������۰��ް���د��ł̽�۰ٻ���
	);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�X�N���[���֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����X�N���[���o�[�@�X�N���[������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int	nAmount=0;	//�X�N���[����

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() ||
		pWnd->IsNowStepDoOperation() 
	)
	{
		//�}�E�X����𖳎�
		CView::OnVScroll(nSBCode, nPos, pScrollBar);
		return;
	}

	if(m_bDispUnderLine) //�A���_�[���C����`�悷��ꍇ
	{
		//�X�N���[������O�ɃA���_�[���C�����������Ă���
		CClientDC dc(this);
		CPoint ptVpOrg(0, 0);
		ptVpOrg.x = -1 * GetScrollPosLogX();
		dc.SetViewportOrg(ptVpOrg);
		MyEraseUnderLine(&dc);
	}

	//�ײ��ė̈�
	CRect	clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	if(m_lfHeight==0)
		return;
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );

	// ��۰��ް��ү���ނ������ł����ꍇ
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;

	// ������۰��ް�ȊO
	if (pScrollBar != GetScrollBarCtrl(SB_VERT))
		return;

	//��۰��ް�擾
	CScrollBar* pobjVScrollBar = GetScrollBarCtrl(SB_VERT);

	//��۰��߼޼�ݎ擾
	SCROLLINFO	vinfo;
	pobjVScrollBar->GetScrollInfo(&vinfo,SIF_ALL);
	int yOrig = vinfo.nPos; //���݈ʒu�擾
	switch(nSBCode)
	{
	case SB_TOP:			// ��ԏ�ɗ���
		vinfo.nPos = 0;
		break;
	case SB_BOTTOM:			// ��ԉ��ɗ���
		vinfo.nPos = m_totalLog.cy;
		break;
	case SB_LINEUP:			// ����د�
		vinfo.nPos -= m_VLineLog.cy;
		if(vinfo.nPos < 0)
			vinfo.nPos = 0;
		break;
	case SB_LINEDOWN:		// �����د�
		if( (vinfo.nPos + nHeight + m_VLineLog.cy) < m_totalLog.cy )
			vinfo.nPos += m_VLineLog.cy;
		break;
	case SB_PAGEUP:			// �㽸۰��ް���ĸد�
		vinfo.nPos -= m_VPageLog.cy;
		if(vinfo.nPos < 0)
			vinfo.nPos = 0;
		break;
	case SB_PAGEDOWN:		// ����۰��ް���ĸد�
		if( (vinfo.nPos + nHeight + m_VPageLog.cy) < m_totalLog.cy )
		{
			vinfo.nPos += m_VPageLog.cy;
		}
		else
		{
			while( (vinfo.nPos + nHeight + m_VLineLog.cy) < m_totalLog.cy )
				vinfo.nPos += m_VLineLog.cy;
		}
		break;
	case SB_THUMBTRACK:		// ��۰��ް����ׯ��
		if(m_VLineLog.cy == 0)
			return;
		vinfo.nTrackPos -= (vinfo.nTrackPos % m_VLineLog.cy);
		if( (vinfo.nTrackPos + nHeight) < m_totalLog.cy )
		{
			vinfo.nPos = vinfo.nTrackPos;
		}
		else
		{
			while( (vinfo.nPos + nHeight + m_VLineLog.cy) < m_totalLog.cy )
				vinfo.nPos += m_VLineLog.cy;
		}
		break;
	}

	//��۰��ް���݈ʒu�X�V
	AdjustVScrollPos(&vinfo);
	pobjVScrollBar->SetScrollInfo(&vinfo,TRUE);

	//�ĕ`��
	if( yOrig != vinfo.nPos )
	{
		//����޳��۰�
		nAmount = -(vinfo.nPos - yOrig);
		MyScrollWindow(2,nAmount);
	}

	//��گĈʒu���Đݒ�
	if(m_nCaretBehave == CARET_BEHAVE_MOVE) //��ʃX�N���[�����ɍs��ʒu��ύX����ꍇ
	{
		CPoint objPos = m_caretPos;
		objPos.x = m_LineNoPos.x;
		MySetCaretPos(objPos);
	}
	else if(m_nCaretBehave == CARET_BEHAVE_STAY) //��ʃX�N���[�����ɍs��ʒu��ێ�����ꍇ
	{
		CPoint objPos = m_caretPos;
		objPos.y += nAmount; //���֍s���Ƃ����͌����B��֍s���Ƃ����͑���
		if(objPos.y < m_RulerPos.y) //��֍s���߂���
		{
			objPos.x = m_LineNoPos.x;
			objPos.y = m_RulerPos.y;
		}
		if(objPos.y > (nHeight - (m_lfHeight+m_lLineInterval)) ) //���֍s���߂���
		{
			objPos.x = m_LineNoPos.x;
			objPos.y = (nHeight - (m_lfHeight+m_lLineInterval));
		}
		MySetCaretPos(objPos);
	}

	//�h��������׺��
	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����X�N���[���o�[�@�X�N���[������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nAmount=0;		//�X�N���[����

	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording()  ||
		pWnd->IsNowStepDoOperation() 
	)
	{
		//�}�E�X����𖳎�
		CView::OnHScroll(nSBCode, nPos, pScrollBar);
		return;
	}

	if(m_bDispUnderLine) //�A���_�[���C����`�悷��ꍇ
	{
		//�X�N���[������O�ɃA���_�[���C�����������Ă���
		CClientDC dc(this);
		CPoint ptVpOrg(0, 0);
		ptVpOrg.x = -1 * GetScrollPosLogX();
		dc.SetViewportOrg(ptVpOrg);
		MyEraseUnderLine(&dc);
	}

	//�ײ��ė̈�
	CRect	clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nWidth = clientRect.Width();
	if(m_lfWidth==0)
		return;
	nWidth -= ( clientRect.Width() % m_lfWidth );

	// ��۰��ް��ү���ނ������ł����ꍇ
	if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
		return;

	// ������۰��ް�ȊO
	if (pScrollBar != GetScrollBarCtrl(SB_HORZ))
		return;

	//��۰��ް�擾
	CScrollBar* pobjHScrollBar = GetScrollBarCtrl(SB_HORZ);
	SCROLLINFO	hinfo;
	pobjHScrollBar->GetScrollInfo(&hinfo,SIF_ALL);
	int xOrig = hinfo.nPos; //���݈ʒu�擾
	switch(nSBCode)
	{
	case SB_TOP:			// ��ԍ��ɗ���
		hinfo.nPos = 0;
		break;
	case SB_BOTTOM:			// ��ԉE�ɗ���
		hinfo.nPos = m_totalLog.cx;
		break;
	case SB_LINEUP:			// �����د�
		hinfo.nPos -= m_HLineLog.cx;
		if(hinfo.nPos < 0)
			hinfo.nPos = 0;
		break;
	case SB_LINEDOWN:		// �E���د�
		if( (hinfo.nPos + nWidth + m_HLineLog.cx) < m_totalLog.cx )
			hinfo.nPos += m_HLineLog.cx;
		break;
	case SB_PAGEUP:			// ����۰��ް���ĸد�
		hinfo.nPos -= m_HPageLog.cx;
		if(hinfo.nPos < 0)
			hinfo.nPos = 0;
		break;
	case SB_PAGEDOWN:		// �E��۰��ް���ĸد�
		if( (hinfo.nPos + nWidth + m_HPageLog.cx) < m_totalLog.cx )
		{
			hinfo.nPos += m_HPageLog.cx;
		}
		else
		{
			while( (hinfo.nPos + nWidth + m_HLineLog.cx) < m_totalLog.cx )
				hinfo.nPos += m_HLineLog.cx;
		}
		break;
	case SB_THUMBTRACK:		// ��۰��ް����ׯ��
		if(m_HLineLog.cx==0)
			return;
		hinfo.nTrackPos -= (hinfo.nTrackPos % m_HLineLog.cx);
		if( (hinfo.nTrackPos + nWidth) < m_totalLog.cx )
		{
			hinfo.nPos = hinfo.nTrackPos;
		}
		else
		{
			while( (hinfo.nPos + nWidth + m_HLineLog.cx) < m_totalLog.cx )
				hinfo.nPos += m_HLineLog.cx;
		}
		break;
	}

	//��۰��ް���݈ʒu�X�V
	AdjustHScrollPos(&hinfo);
	pobjHScrollBar->SetScrollInfo(&hinfo,TRUE);

	//�ĕ`��
	if( xOrig != hinfo.nPos )
	{
		//����޳��۰�
		nAmount = -(hinfo.nPos - xOrig);
		MyScrollWindow(1,nAmount);
	}

	//��گĈʒu
	if(m_nCaretBehave == CARET_BEHAVE_MOVE) //��ʃX�N���[�����ɍs��ʒu��ύX����ꍇ
	{
		CPoint objPos = m_caretPos;
		objPos.x = m_LineNoPos.x;
		MySetCaretPos(objPos);
	}
	else if(m_nCaretBehave == CARET_BEHAVE_STAY) //��ʃX�N���[�����ɍs��ʒu��ێ�����ꍇ
	{
		CPoint objPos = m_caretPos;
		objPos.x += nAmount; //�E�֍s���Ƃ����͌����B���֍s���Ƃ����͑���
		if(objPos.x < m_LineNoPos.x) //�E�֍s���߂���
		{
			objPos.x = m_LineNoPos.x;
		}
		if(objPos.x > (nWidth-m_lfWidth) ) //���֍s���߂���
		{
			objPos.x = (nWidth-m_lfWidth);
		}
		MySetCaretPos(objPos);
	}

	//�h��������׺��
	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�h�l�d�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FIME ���b�Z�[�W WM_IME_NOTIFY�̃��b�Z�[�W�n���h������					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
LRESULT CEditorView::OnMyImeNotify(WPARAM dwCommand, LPARAM dwData)
{
	//dwCommand �ɂ́A�ȉ��̎�ނ�����B
	//		IMN_CHANGECANDIDATE,		IMN_CLOSECANDIDATE
	//		IMN_CLOSESTATUSWINDOW,		IMN_GUIDELINE
	//		IMN_OPENCANDIDATE,			IMN_PRIVATE
	//		IMN_SETCANDIDATEPOS,		IMN_SETCOMPOSITIONFONT
	//		IMN_SETCOMPOSITIONWINDOW,	IMN_SETCONVERSIONMODE
	//		IMN_SETOPENSTATUS,			IMN_SETSENTENCEMODE
	//		IMN_SETSTATUSWINDOWPOS
	//���ׂĂ݂�ƈȉ��̃��b�Z�[�W�����ł���
	//		�A�v���P�[�V�����N�����A�I����
	//			IMN_SETOPENSTATUS
	//		Alt + ���p�L�[������
	//			IMN_SETOPENSTATUS �� IMN_SETSTATUSWINDOWPOS
	//����Ĉȉ��̂悤�ɂ���
#if 0
	WPARAM wPram1  = IMN_CHANGECANDIDATE;		//8
	WPARAM wPram2  = IMN_CLOSECANDIDATE;		//4
	WPARAM wPram3  = IMN_CLOSESTATUSWINDOW;		//1
	WPARAM wPram4  = IMN_GUIDELINE;				//13
	WPARAM wPram5  = IMN_OPENCANDIDATE;			//5
	WPARAM wPram6  = IMN_PRIVATE;				//14
	WPARAM wPram7  = IMN_SETCANDIDATEPOS;		//9
	WPARAM wPram8  = IMN_SETCOMPOSITIONFONT;	//10
	WPARAM wPram9  = IMN_SETCOMPOSITIONWINDOW;	//11
	WPARAM wPram10 = IMN_SETCONVERSIONMODE;		//6
	WPARAM wPram11 = IMN_SETOPENSTATUS;			//8
	WPARAM wPram12 = IMN_SETSENTENCEMODE;		//7
	WPARAM wPram13 = IMN_SETSTATUSWINDOWPOS;	//12
#endif

	if(dwCommand==IMN_SETOPENSTATUS) //�`�k�s�{���p�L�[����
	{
		LRESULT ret = DefWindowProc(WM_IME_NOTIFY, dwCommand, dwData);
		if( m_bIsImmOpen == FALSE ) // IME  CLOSE -> OPEN
		{
			//�L�����b�g�ʒu�ɕϊ��E�C���h�E��\��
			CgAdjustCompositionWindow();

			//�ϊ��E�C���h�E���t�H���g�ݒ�
			LOGFONT	logfont;
			ZeroMemory((PVOID)&logfont,sizeof(LOGFONT));
			m_objDispFont.GetLogFont(&logfont);
			::ImmSetCompositionFont(m_hImc, &logfont);
		}
		//�h�l�l�I�[�v���t���O�Z�b�g
		m_bIsImmOpen = !m_bIsImmOpen;
		return ret;
	}
	else if( (dwCommand==IMN_OPENCANDIDATE)  || //�X�y�[�X�L�[�����ɂ��A�ϊ��Ώۊ����ꗗ�̃E�C���h�E(CandidateWindow)
			 (dwCommand==IMN_CLOSECANDIDATE) ||	//���\������Ȃ��悤�ɂ���B
			 (dwCommand==3)
	)
	{
		return 0;
	}
	else
	{
		// CG: ���̊֐��� �uIME �T�|�[�g�v �R���|�[�l���g�ɂ��ǉ�����Ă��܂��B
  		LRESULT ret = DefWindowProc(WM_IME_NOTIFY, dwCommand, dwData);
		return ret;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���݂̃L�����b�g�̈ʒu�ɕϊ��E�B���h�E���ړ����鏈��					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::CgAdjustCompositionWindow()
{
	// CG: ���̊֐��� �uIME �T�|�[�g�v �R���|�[�l���g�ɂ��ǉ�����Ă��܂��B
	// ���݂̃L�����b�g�ʒu�ɕϊ��E�B���h�E���ړ�����K�v������ꍇ�ɁA���̊֐����Ăяo���Ă��������B
	COMPOSITIONFORM cf;
	if (m_hImc)
	{
		cf.dwStyle = CFS_POINT;
		cf.ptCurrentPos = m_caretPos;
		::ImmSetCompositionWindow(m_hImc, &cf);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�}�E�X����֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�E�X���{�^���_�E���C�x���g����										|*/
/*|	���	�F																		|*/
/*|	���l	�F������View����޳����ϳ��د����ꂽ���ɗ���								|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnLButtonDown
	(	UINT nFlags,
		CPoint point		//�N���C�A���g���W�n�ɂ�����}�E�X�N���b�N�ʒu
	)
{
	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() ||
		pWnd->IsNowStepDoOperation() 
	)
	{
		//�}�E�X����𖳎�
		CView::OnLButtonDown(nFlags, point);
		return;
	}

	long	lCurrentLine=0;
	long	lCurrentOffset=0;
	CPoint	tempPos(0,0);
	CEditorDoc* pDoc = (CEditorDoc*)GetDocument();
	CRect	clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	//�}�E�X�ʒu�@�|���@�s�A��ԍ��ϊ�
	LONG lRet =	MouseToLineOffset( point, &lCurrentLine, &lCurrentOffset, &tempPos );
	if(lRet	== 0) //Error
	{
		//���N���X�̃n���h���R�[��
		CView::OnLButtonDown(nFlags, point);
		return;
	}

	if(lRet	== 2) //�}�E�X�ʒu���s�ԍ��̈��
	{
		ClearSelectArea();

		//�L�����b�g�ʒu�����݂̃}�E�X�ʒu�s�̐擪�Ɉʒu�Â�
		MySetCaretToLineOffset(lCurrentLine,0);

		// �I���J�n�ʒu���L�����Ă����i�I���J�n�I�t�Z�b�g�͍s���j
		m_lSelectStartLine = lCurrentLine;
		m_lSelectStartOffset = 0;
		m_lSelectEndLine = -1;
		m_lSelectEndOffset = -1;
		m_lPreMoveCaretLine	= -1;
		m_lPreMoveCaretOffset =	-1;
		m_lLButtonDownLine = lCurrentLine;
		m_lLButtonDownOffset = 0;

		//�}�E�X���{�^�����������L��
		m_bLButtonDown = TRUE;

		//�}�E�X�L���v�`���J�n
		//�ȍ~�Aϳ����{����޳�̗̈�O�ɏo�Ă����Ă��{����޳�ɓ͂�����
		SetCapture();

		//���N���X�̃n���h���R�[��
		CView::OnLButtonDown(nFlags, point);

		//�I�[�g�X�N���[��
		AutoScroll();
		return;
	}
	else //�}�E�X�ʒu���s�ԍ��̈�⃋�[���̈�ȊO
	{
		//�L�����b�g�_���w�ʒu���L��
		m_nLogPreCaretX = tempPos.x - m_LineNoPos.x + GetScrollPosLogX();

		//�L�����b�g�ʒu�����݂̃}�E�X�ʒu�ɐݒ�
		MySetCaretPos(tempPos);

		BOOL bWithin = IsWithinSelectArea( m_lSelectStartLine,
										   m_lSelectStartOffset,
										   m_lSelectEndLine,
										   m_lSelectEndOffset,
										   lCurrentLine,
										   lCurrentOffset );
		if(IsSelected()	&& bWithin) //�I��̈�����N���b�N�����ꍇ
		{
			//////////////////////////////////////
			//	�n�k�d�h���b�O���h���b�v�Ή�	//
			//////////////////////////////////////

			//�h���b�v�\�[�X�̑I���s�A����L�����Ă���
			long	lSelectStartLine   = m_lSelectStartLine;
			long	lSelectStartOffset = m_lSelectStartOffset;
			long	lSelectEndLine	   = m_lSelectEndLine;
			long	lSelectEndOffset   = m_lSelectEndOffset;

			//�h���b�O�f�[�^����
			AdjustSelectArea();
			CSharedFile	objShareFile(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
			if(m_lSelectStartLine == m_lSelectEndLine) //�P�s
			{
				CLine* pobjLine	= (CLine*)pDoc->GetOneLine(m_lSelectStartLine);
				objShareFile.Write(	pobjLine->Data(m_lSelectStartOffset),
									(m_lSelectEndOffset-m_lSelectStartOffset) );
			}
			else //�����s
			{
				for(ULONG lLineNo=m_lSelectStartLine; lLineNo<=(ULONG)m_lSelectEndLine; lLineNo++)
				{
					CLine* pobjLine	= (CLine*)pDoc->GetOneLine(lLineNo);
					if(!pobjLine)
						continue;
					if(lLineNo == (ULONG)m_lSelectStartLine)//�I���J�n�s
					{
						objShareFile.Write(	pobjLine->Data(m_lSelectStartOffset),
											(pobjLine->GetLength()-m_lSelectStartOffset) );
					}
					else if(lLineNo	== (ULONG)m_lSelectEndLine)//�I���I���s
					{
						objShareFile.Write(	"\r\n",	2 );
						objShareFile.Write(	pobjLine->String(), m_lSelectEndOffset );
					}
					else //�r���s
					{
						objShareFile.Write(	"\r\n",	2 );
						objShareFile.Write(	pobjLine->String(), pobjLine->GetLength() );
					}
				}
			}

			//�h���b�O�J�n
			COleDataSource	objOleDataSource;
			objOleDataSource.CacheGlobalData(CF_TEXT, objShareFile.Detach());
			DROPEFFECT result =	objOleDataSource.DoDragDrop();
			if(result==DROPEFFECT_MOVE)	//MOVE
			{
				//�h���b�v�\�[�X�̑I��͈͂��폜
				CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
				pDoc->DeleteSelectedArea(
							lSelectStartLine,			// �I���J�n�s�ԍ�
							lSelectStartOffset,			// �I���J�n��ԍ�
							lSelectEndLine,				// �I���I���s�ԍ�
							lSelectEndOffset,			// �I���I����ԍ�
							FALSE,						// �N���b�v�{�[�h�փR�s�[���邩�H(TRUE:����AFALSE:���Ȃ��j
							m_caretPos,					// �L�����b�g�ʒu
							objScrollPosLog,			// �X�N���[���ʒu
							m_bDispRuler,				// ���[���\���t���O
							m_bDispLineNo );			// �s�ԍ��\���t���O
			}

			//�I��͈͉���
			ClearSelectArea();

			//���N���X�̃n���h���R�[��
			CView::OnLButtonDown(nFlags, point);
			return;
		}
		else //�I��̈�O���N���b�N�����ꍇ
		{
			// �I���J�n�ʒu���L�����Ă���
			m_lSelectStartLine = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine = -1;
			m_lSelectEndOffset = -1;
			m_lPreMoveCaretLine	= -1;
			m_lPreMoveCaretOffset =	-1;
			m_lLButtonDownLine = lCurrentLine;
			m_lLButtonDownOffset = lCurrentOffset;

			//�}�E�X���{�^�����������L��
			m_bLButtonDown = TRUE;

			//�}�E�X�L���v�`���J�n
			//�ȍ~�Aϳ����{����޳�̗̈�O�ɏo�Ă����Ă��{����޳�ɓ͂�����
			SetCapture();

			//���N���X�̃n���h���R�[��
			CView::OnLButtonDown(nFlags, point);

			//�I�[�g�X�N���[��
			AutoScroll();
			return;
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�E�X�J�[�\���ړ��C�x���g����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	//���N���X�̃n���h���R�[��
	CView::OnMouseMove(nFlags, point);
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�E�X���{�^���A�b�v�C�x���g����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() ||
		pWnd->IsNowStepDoOperation() 
	)
	{
		//�}�E�X����𖳎�
		CView::OnLButtonUp(nFlags, point);
		return;
	}

	long lCurrentLine=0;
	long lCurrentOffset=0;
	CEditorDoc* pDoc = (CEditorDoc*)GetDocument();
	CPoint	tempPos(0,0);
	CRect	clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	m_bLButtonDown = FALSE;	//�}�E�X���{�^���A�b�v

	if( GetCapture() == this )
	{
		ReleaseCapture();		//�}�E�X�L���v�`���I��

		if( (clientRect.PtInRect(point) == FALSE) || //�}�E�X���N���C�A���g�̈�O�ɏo��
			(point.x < 0)						  ||
			(point.y < 0)
		)
		{
			//���N���X�̃n���h���R�[��
			CView::OnLButtonUp(nFlags, point);
			return;
		}
		else
		{
			//�}�E�X�ʒu�@�|���@�s�A��ԍ��ϊ�
			LONG lRet =	MouseToLineOffset( point, &lCurrentLine, &lCurrentOffset, &tempPos );
			if(lRet	== 0)//���s
			{
				//���N���X�̃n���h���R�[��
				CView::OnLButtonDown(nFlags, point);
				return;
			}
			else
			{
				if(lRet	== 1)//����
				{
					//�L�����b�g�ʒu�ݒ�
					MySetCaretPos(tempPos);
				}
				else if(lRet ==	2)//�s�ԍ��G���A��
				{
					//�L�����b�g���}�E�X�ʒu�s�̍Ō�Ɉʒu�Â�
					CLine* pobjLine	= pDoc->GetOneLine(lCurrentLine);
					if(pobjLine	!= NULL)
						lCurrentOffset = pobjLine->GetLength();
					MySetCaretToLineOffset(lCurrentLine,lCurrentOffset);
				}

				if(	(m_lLButtonDownLine	!= lCurrentLine) ||	(m_lLButtonDownOffset != lCurrentOffset) )
				{
					//�I��͈͍ĕ`��
					RedrawSelectArea(m_lPreMoveCaretLine,m_lPreMoveCaretOffset);
				}
				else //�}�E�X�A�b�v�ʒu���I���J�n�ʒu�Ɠ���
				{
					ClearSelectArea();
					m_pApp->MessagePump();
					if(::GetAsyncKeyState(VK_CONTROL)<0 && lRet==1)//CTRL���������}�[�W���G���A�łȂ�
					{
						//CTRL+�N���b�N�ɂ�郏�[�h�Z���N�g
						WordSelectByCtrlClick(point);
					}
				}
			}
		}
	}

	//���N���X�̃n���h���R�[��
	CView::OnLButtonUp(nFlags, point);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�E�X���{�^���_�u���N���b�N�C�x���g����								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if( pWnd->IsNowOperationRecording() ||
		pWnd->IsNowStepDoOperation() 
	)
	{
		//�}�E�X����𖳎�
		CView::OnLButtonDblClk(nFlags, point);
		return;
	}

	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	long	lCnt=0;
	long	lCurrentLine=0;
	long	lCurrentOffset=0;
	CPoint	prePos(0,0);
	CPoint	nowPos(0,0);
	CPoint	mousePos(0,0);
	CLine*	pobjLine = NULL;
	CString objMsg;

	//�}�E�X�ʒu�@�|���@�s�A��ԍ��ϊ�
	LONG lRet =	MouseToLineOffset( point, &lCurrentLine, &lCurrentOffset, &mousePos	);
	if(lRet	== 0 ||	lRet ==	2)
	{
		//���N���X�̃n���h���R�[��
		CView::OnLButtonDblClk(nFlags, point);
		return;
	}

	//�}�E�X�ʒu�̍s�擾
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
	{
		//���N���X�̃n���h���R�[��
		CView::OnLButtonDblClk(nFlags, point);
		return;
	}

	if( pDoc->IsGrep() ) //�f�q�d�o���ʕ\�����ł���
	{
		CString		objGrepFileName;
		long		lLineNo = 0;

		//�f�q�d�o���ʉ���
		pobjLine->ParseGrepResult(objGrepFileName,&lLineNo);

		//�f�q�d�o�t�@�C���I�[�v��
		if( (_access( (LPCTSTR)objGrepFileName, 0 )) != -1 )
		{
			m_pApp->OpenDocumentFile((LPCTSTR)objGrepFileName);

			//�w��s�ԍ��ւ̃W�����v�w��
			m_pApp->SendJumpMsg((LPCTSTR)objGrepFileName,lLineNo);

			//���N���X�̃n���h���R�[��
			CView::OnLButtonDblClk(nFlags, point);
			return;
		}
		else
		{
			char	szMsg[512];
			objMsg.LoadString(IDS_GREPFILE_NOTFOUND);
			sprintf(szMsg,(LPCTSTR)objMsg,(LPCTSTR)objGrepFileName);
			m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);
			objGrepFileName.ReleaseBuffer();

			//���N���X�̃n���h���R�[��
			CView::OnLButtonDblClk(nFlags, point);
			return;
		}
	}
	else if( pDoc->IsFSearch() ) //�t�@�C���������ʕ\�����ł���
	{
		char		szFileName[_MAX_PATH];
		CCodeConvManager	objCodeConvManager;

		//�����t�@�C���I�[�v��
		strcpy(szFileName,(const char*)pobjLine->String());
		if( (_access( szFileName, 0 )) == -1 )
		{
			char	szMsg[512];
			objMsg.LoadString(IDS_FINDFILE_NOTFOUND);
			sprintf(szMsg,(LPCTSTR)objMsg,szFileName);
			m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);
			CView::OnLButtonDblClk(nFlags, point);
			return;
		}
		else
		{
			//�������ʃt�@�C���I�[�v��
			m_pApp->OpenDocumentFile(szFileName);
			CView::OnLButtonDblClk(nFlags, point);
			return;
		}
	}
	else if( pDoc->IsDirCmp() && 	//�f�B���N�g����r�\�����@�����@����t�@�C���ł���
			 pDoc->GetDirCmpLineKind(lCurrentLine)==DIRCMP_LINEKIND_DIFF 
	)
	{
		//��r�f�B���N�g�����擾
		CString objDir1="",objDir2="",objFileName="";
		pDoc->GetDirCmpFolder(objDir1,objDir2);
		//����t�@�C�����擾
		CLine* pobjLine = pDoc->GetOneLine(lCurrentLine);
		if(pobjLine == NULL)
			return;
		pobjLine->GetDirCmpFileName(objFileName);
		//�p�X�쐬
		if(m_pApp->IsDriveRoot(objDir1))
			objDir1 += objFileName;
		else
			objDir1 += "\\" + objFileName;
		if(m_pApp->IsDriveRoot(objDir2))
			objDir2 += objFileName;
		else
			objDir2 += "\\" + objFileName;

		pWnd->DoFileCompare(objDir1,objDir2,NULL);
		return;
	}
	else if(pobjLine->IsAboveURL(lCurrentOffset))//�t�q�k��ł���
	{
		if(pDoc->GetFlagUrlColor()) 
		{
			//�L�����b�g�ʒu�̂t�q�k�擾
			CString objUrl=_T("");
			if(pobjLine->GetUrl(lCurrentOffset,objUrl))
			{
				//�t�q�k���v�d�a�u���E�U�ŕ\��
				m_pApp->BrowseUrl(objUrl);
				return;
			}
		}
	}
	else if(pobjLine->IsAboveEMail(lCurrentOffset))//���|����������ł���
	{
		if(pDoc->GetFlagEMailColor()) 
		{
			//�L�����b�g�ʒu�̂��|���������擾
			CString objEMail=_T("");
			if(pobjLine->GetEMail(lCurrentOffset,objEMail))
			{
				//���[���[���N��
				m_pApp->ExecMailer(objEMail);
				return;
			}
		}
	}

	//�܂��I������������
	if(IsSelected())
		ClearSelectArea();

	//////////////////////////////
	//		�I��͈͐ݒ�		//
	//////////////////////////////
	if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //����ٸد��ʒu�����p
	{
		if( isHanKana(pobjLine->GetAt(lCurrentOffset)) )//���p����
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// �I��͈͂̌�������
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//�I��͈͂̑O������

			//�}�E�X�ʒu��_�����W�֕ϊ�����
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//�P�����O�̈ʒu�擾
				if( ((nowPos.x-prePos.x)==m_lfWidth) &&
					isHanKana(pobjLine->GetAt(lCnt-1))
				)
				{
					m_lSelectStartOffset--;
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt--;
				if( nowPos.x == 0 )
					break;
			}
		}
		else if( ( (pobjLine->GetAt(lCurrentOffset)>='A' && pobjLine->GetAt(lCurrentOffset)<='Z') ||
			  	 (pobjLine->GetAt(lCurrentOffset)>='a' && pobjLine->GetAt(lCurrentOffset)<='z') ||
			     (pobjLine->GetAt(lCurrentOffset)>='0' && pobjLine->GetAt(lCurrentOffset)<='9') ||
			     (pobjLine->GetAt(lCurrentOffset)=='_')
			     ) &&
				 !isHanKana(pobjLine->GetAt(lCurrentOffset))
		)
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// �I��͈͂̌�������
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) 						||
					( (pobjLine->GetAt(lCnt)<'A' || pobjLine->GetAt(lCnt)>'Z') &&
					  (pobjLine->GetAt(lCnt)<'a' || pobjLine->GetAt(lCnt)>'z') &&
					  (pobjLine->GetAt(lCnt)<'0' || pobjLine->GetAt(lCnt)>'9') &&
					  (pobjLine->GetAt(lCnt)!='_')
					)																||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//�I��͈͂̑O������

			//�}�E�X�ʒu��_�����W�֕ϊ�����
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//�P�����O�̈ʒu�擾
				if( ( (nowPos.x-prePos.x)==m_lfWidth) &&
					( (pobjLine->GetAt(lCnt-1)>='A' && pobjLine->GetAt(lCnt-1)<='Z') ||
			  		  (pobjLine->GetAt(lCnt-1)>='a' && pobjLine->GetAt(lCnt-1)<='z') ||
			  		  (pobjLine->GetAt(lCnt-1)>='0' && pobjLine->GetAt(lCnt-1)<='9') ||
			  		  (pobjLine->GetAt(lCnt-1)=='_')
					) &&
					!isHanKana(pobjLine->GetAt(lCnt-1))
				)
				{
					m_lSelectStartOffset--;
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt--;
				if( nowPos.x == 0 )
					break;
			}
		}
		else if( !isspace(pobjLine->GetAt(lCurrentOffset)) ) //����ٸد��ʒu�����p�p�����ł����p���łł��߰��ł��Ȃ�
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// �I��͈͂̌�������
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) 								||
					( (pobjLine->GetAt(lCnt)>='A' && pobjLine->GetAt(lCnt)<='Z') ||
			  		  (pobjLine->GetAt(lCnt)>='a' && pobjLine->GetAt(lCnt)<='z') ||
			  		  (pobjLine->GetAt(lCnt)>='0' && pobjLine->GetAt(lCnt)<='9') ||
			  		  (pobjLine->GetAt(lCnt)=='_')
					) 																		||
					isHanKana(pobjLine->GetAt(lCnt))										||
					isspace(pobjLine->GetAt(lCnt))
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//�I��͈͂̑O������

			//�}�E�X�ʒu��_�����W�֕ϊ�����
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//�P�����O�̈ʒu�擾
				if( ((nowPos.x-prePos.x)==m_lfWidth)  									&&
					( (pobjLine->GetAt(lCnt-1)<'A' || pobjLine->GetAt(lCnt-1)>'Z') &&
					  (pobjLine->GetAt(lCnt-1)<'a' || pobjLine->GetAt(lCnt-1)>'z') &&
					  (pobjLine->GetAt(lCnt-1)<'0' || pobjLine->GetAt(lCnt-1)>'9') &&
					  (pobjLine->GetAt(lCnt-1)!='_')
					)																	&&
					!isHanKana(pobjLine->GetAt(lCnt-1))									&&
					!isspace(pobjLine->GetAt(lCnt-1))
				)
				{
					m_lSelectStartOffset--;
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt--;
				if( nowPos.x == 0 )
					break;
			}
		}
	}
	else if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //����ٸد��ʒu���S�p����
	{
		//�����R�[�h�擾
		UINT unChar = ( (pobjLine->GetAt(lCurrentOffset) << 8) | (pobjLine->GetAt(lCurrentOffset+1)) );
		if( _ismbchira( unChar ) ) //����ٸد��ʒu��������޲ĂЂ炪��
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// �I��͈͂̌�������
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!_ismbchira( unCharTmp )
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//�I��͈͂̑O������

			//�}�E�X�ʒu��_�����W�֕ϊ�����
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//�P�����O�̈ʒu�擾
				if( (nowPos.x-prePos.x)==(m_lfWidth*2) )
				{
					//�����R�[�h�擾
					UINT unCharTmp = ( (pobjLine->GetAt(lCnt-2) << 8) | (pobjLine->GetAt(lCnt-1)) );
					if( _ismbchira( unCharTmp ) ) //�Ђ炪�Ȍp��
					{
						m_lSelectStartOffset -= 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt -= 2;
				if( nowPos.x == 0 )
					break;
			}
		}
		else if( (_ismbckata(unChar)) ||
				 (unChar==0x815B)			//����ٸد��ʒu��������޲Ă������Ȃ����h�[�h
		)
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// �I��͈͂̌�������
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbckata( unCharTmp ) && (unCharTmp!=0x815B) )
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//�I��͈͂̑O������

			//�}�E�X�ʒu��_�����W�֕ϊ�����
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//�P�����O�̈ʒu�擾
				if( (nowPos.x-prePos.x)==(m_lfWidth*2) )
				{
					//�����R�[�h�擾
					UINT unCharTmp = ( (pobjLine->GetAt(lCnt-2) << 8) | (pobjLine->GetAt(lCnt-1)) );
					if( _ismbckata( unCharTmp ) || (unCharTmp==0x815B) ) //�J�^�J�i�p��
					{
						m_lSelectStartOffset -= 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt -= 2;
				if( nowPos.x == 0 )
					break;
			}
		}
		else if( _ismbcl1(unChar) || _ismbcl2(unChar) ) //����ٸد��ʒu��������޲Ċ���
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// �I��͈͂̌�������
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbcl1( unCharTmp ) && !_ismbcl2( unCharTmp ) )
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//�I��͈͂̑O������

			//�}�E�X�ʒu��_�����W�֕ϊ�����
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//�P�����O�̈ʒu�擾
				if( (nowPos.x-prePos.x)==(m_lfWidth*2) )
				{
					//�����R�[�h�擾
					UINT unCharTmp = ( (pobjLine->GetAt(lCnt-2) << 8) | (pobjLine->GetAt(lCnt-1)) );
					if( _ismbcl1(unCharTmp) || _ismbcl2(unCharTmp) ) //�����p��
					{
						m_lSelectStartOffset -= 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt -= 2;
				if( nowPos.x == 0 )
					break;
			}
		}
		else //���̑��̂Q�o�C�g����
		{
			m_lSelectStartLine   = lCurrentLine;
			m_lSelectStartOffset = lCurrentOffset;
			m_lSelectEndLine     = lCurrentLine;
			m_lSelectEndOffset   = lCurrentOffset;

			// �I��͈͂̌�������
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( _ismbchira( unCharTmp ) ||
					  _ismbckata(unCharTmp)   ||
					  (unCharTmp==0x815B)     ||
					  _ismbcl1( unCharTmp )   ||
					  _ismbcl2( unCharTmp )
					)
				)
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					m_lSelectEndOffset = lCnt;
					break;
				}
				else
				{
					m_lSelectEndOffset = lCnt;
				}
			}

			//�I��͈͂̑O������

			//�}�E�X�ʒu��_�����W�֕ϊ�����
			nowPos = mousePos;
			nowPos -= m_LineNoPos;
			nowPos -= m_RulerPos;
			nowPos.x += GetScrollPosLogX();
			nowPos.y += GetScrollPosLogY();
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&nowPos,&prePos);//�P�����O�̈ʒu�擾
				if( (nowPos.x-prePos.x)==(m_lfWidth*2) )
				{
					//�����R�[�h�擾
					UINT unCharTmp = ( (pobjLine->GetAt(lCnt-2) << 8) | (pobjLine->GetAt(lCnt-1)) );
					if(	( !_ismbchira( unCharTmp ) &&
					      !_ismbckata(unCharTmp)   &&
					      (unCharTmp!=0x815B)      &&
					      !_ismbcl1( unCharTmp )   &&
					      !_ismbcl2( unCharTmp )
					    )
					)
					{
						m_lSelectStartOffset -= 2;
					}
					else
					{
						break;
					}
				}
				else
				{
					break;
				}

				nowPos = prePos;
				lCnt -= 2;
				if( nowPos.x == 0 )
					break;
			}
		}
	}

	//�L�����b�g�ʒu�ݒ�
	MySetCaretToLineOffset(m_lSelectEndLine,m_lSelectEndOffset);

	//�}�E�X�ʒu�̍s���ĕ`��
	InvalidateCaretLine();

	//���N���X�̃n���h���R�[��
	CView::OnLButtonDblClk(nFlags, point);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�r�d�s�b�t�q�r�n�q������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect	clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();

	//�}�E�X�ʒu�擾
	POINT	stCursorPos;
	::GetCursorPos(&stCursorPos);
	ScreenToClient(&stCursorPos);

	if( !(stCursorPos.x >= m_LineNoPos.x) || !(stCursorPos.x < clientRect.Width()) || //�������̓G���A�O
		!(stCursorPos.y >= m_RulerPos.y)  || !(stCursorPos.y < clientRect.Height())
	)
	{
		return CView::OnSetCursor(pWnd, nHitTest, message);
	}

	//�������̓G���A���@���@�I��
	if(IsSelected())
		return CView::OnSetCursor(pWnd, nHitTest, message);

	//�}�E�X�ʒu�@�|���@�s�A��ԍ��ϊ�
	long lLine=0;
	long lOffset=0;
	CPoint tempPos;
	CPoint objCursorPos(stCursorPos.x,stCursorPos.y);
	long lRet =	MouseToLineOffset( objCursorPos, &lLine, &lOffset, &tempPos );//1:OK
	if(lRet != 1)
	{
		//�W���h�r�[���J�[�\��
		::SetCursor(::LoadCursor(NULL, IDC_IBEAM));
		return TRUE;
	}

	//�t�q�k��`�F�b�N
	if(pDoc->GetFlagUrlColor())
	{
		if(pDoc->IsAboveURL(lLine,lOffset))
		{
			//�n���h�J�[�\��
			::SetCursor(m_pApp->LoadCursor(IDC_HAND));
			return TRUE;
		}
	}

	//���|����������`�F�b�N
	if(pDoc->GetFlagEMailColor())
	{
		if(pDoc->IsAboveEMail(lLine,lOffset))
		{
			//�n���h�J�[�\��
			::SetCursor(m_pApp->LoadCursor(IDC_HAND));
			return TRUE;
		}
	}

	//�W���h�r�[���J�[�\��
	::SetCursor(::LoadCursor(NULL, IDC_IBEAM));
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�t�B���^�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���b�Z�[�W�t�B���^����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorView::PreTranslateMessage(MSG* pMsg) 
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	UINT nChar = LOWORD(pMsg->wParam);
	CKeyBind* pobjKeyBind=NULL;
	BOOL bIsCtrl  = (::GetAsyncKeyState(VK_CONTROL) < 0 ? TRUE : FALSE);
	BOOL bIsShift = (::GetAsyncKeyState(VK_SHIFT)   < 0 ? TRUE : FALSE);
	BOOL bIsAlt   = (::GetAsyncKeyState(VK_MENU)    < 0 ? TRUE : FALSE);
	if(pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_KEYDOWN)
	{
		if(bIsAlt && nChar==VK_F6)
			int a=0;
		if(nChar >= VK_F1 && nChar <= VK_F12) //�t�@���N�V�����L�[
		{
			//�L�[�o�C���h����
			if((pobjKeyBind = m_pApp->FindKeyBind(nChar,bIsCtrl,bIsShift,bIsAlt)) != NULL)
			{
				pWnd->PostMessage(WM_COMMAND,pobjKeyBind->m_dwMenuID,0);
				return TRUE;
			}
			else
				return CView::PreTranslateMessage(pMsg);
		}
		if(bIsCtrl || bIsAlt)
		{
			//�L�[�o�C���h����
			if((pobjKeyBind = m_pApp->FindKeyBind(nChar,bIsCtrl,bIsShift,bIsAlt)) != NULL)
			{
				pWnd->PostMessage(WM_COMMAND,pobjKeyBind->m_dwMenuID,0);
				return TRUE;
			}
			else
				return CView::PreTranslateMessage(pMsg);
		}
		else if(bIsShift)
		{
			if(nChar==VK_INSERT || nChar==VK_DELETE || nChar==VK_BACK || nChar==VK_RETURN)
			{
				//�L�[�o�C���h����
				if((pobjKeyBind = m_pApp->FindKeyBind(nChar,bIsCtrl,bIsShift,bIsAlt)) != NULL)
				{
					pWnd->PostMessage(WM_COMMAND,pobjKeyBind->m_dwMenuID,0);
					return TRUE;
				}
				else
					return CView::PreTranslateMessage(pMsg);
			}
			else
				return CView::PreTranslateMessage(pMsg);
		}
		else
			return CView::PreTranslateMessage(pMsg);
	}
	else
		return CView::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�������͊֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�r�x�r�j�d�x�c�n�v�m����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CKeyBind* pobjKeyBind=NULL;
	BOOL bIsCtrl  = (::GetAsyncKeyState(VK_CONTROL) < 0 ? TRUE : FALSE);
	BOOL bIsShift = (::GetAsyncKeyState(VK_SHIFT)   < 0 ? TRUE : FALSE);
	BOOL bIsAlt   = (::GetAsyncKeyState(VK_MENU)    < 0 ? TRUE : FALSE);
	//�L�[�o�C���h����
	if((pobjKeyBind = m_pApp->FindKeyBind(nChar,bIsCtrl,bIsShift,bIsAlt)) != NULL)
		pWnd->PostMessage(WM_COMMAND,pobjKeyBind->m_dwMenuID,0);
	CView::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�j�d�x�c�n�v�m����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nCnt=0;
	int nMultValueOfCaretAccelarete = m_pApp->GetProfileInt("CEditorView","nMultValueOfCaretAccelarete",3);
	if( nMultValueOfCaretAccelarete!=1 && nMultValueOfCaretAccelarete!=2 &&
		nMultValueOfCaretAccelarete!=3 && nMultValueOfCaretAccelarete!=4 &&
		nMultValueOfCaretAccelarete!=5 )
	{
		nMultValueOfCaretAccelarete = 3;
	}
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(nChar == VK_SHIFT)
		m_bIsShiftKeyDown = TRUE;

	TRACE("nChar=%x,nRepCnt=%d,nFlags=%x\r\n",nChar,nRepCnt,nFlags);

	// �C���N�������^���T�[�`���s���`�F�b�N
	if(pWnd->IsNowIncrimentalSearching())
	{
		//�h�����n���h���R�[��
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	// �L�[����̋L�^���`�F�b�N
	if(pWnd->IsNowOperationRecording())
	{
		if( (nChar==VK_SHIFT) || (nChar==VK_INSERT) || (nChar==VK_LEFT)   || (nChar==VK_RIGHT)  ||
			(nChar==VK_DOWN)  || (nChar==VK_UP)     || (nChar==VK_HOME)   || (nChar==VK_END)    ||
			(nChar==VK_PRIOR) || (nChar==VK_NEXT)   || (nChar==VK_RETURN) || (nChar==VK_DELETE) ||
			(nChar==VK_BACK)
		)
		{
			//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
			COperation* pobjOperation = new COperation();
			pobjOperation->SetOperationKind(KEYOPE_ONKEYDOWN);
			pobjOperation->SetOnKeyDownParameter(nChar,nRepCnt,nFlags,nMultValueOfCaretAccelarete);
			//�L�^
			pWnd->addOperation(pobjOperation);
		}
	}

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	long lLineLength    = pDoc->GetLineLength(lCurrentLine);
	long lMaxLine       = pDoc->GetLineCount();

	if( (nChar == VK_LEFT) || (nChar == VK_RIGHT) || (nChar == VK_DOWN) || //�L�����b�g�ړ��L�[
		(nChar == VK_UP)   || (nChar == VK_HOME)  || (nChar == VK_END)  ||
		(nChar == VK_PRIOR)|| (nChar == VK_NEXT)
	)
	{
		//�L�����b�g�ړ�����܂��̈ʒu���L��
		m_lPreMoveCaretLine   = lCurrentLine;
		m_lPreMoveCaretOffset = lCurrentOffset;

		if(m_bIsShiftKeyDown)//SHIFT�L�[������
		{
			if( (m_lSelectStartLine == -1) || (m_lSelectStartOffset == -1) )
			{
				//�I���J�n�ʒu���L��
				m_lSelectStartLine   = lCurrentLine;
				m_lSelectStartOffset = lCurrentOffset;
				m_lSelectEndLine     = -1;
				m_lSelectEndOffset   = -1;
			}
		}
	}

	//////////////////////////
	//�����L�[�ɂ�鏈������//
	//////////////////////////
	if(nChar == VK_INSERT) //INSERT�L�[
	{
		//�}���E�㏑�����[�h�ؑւ�
		OnChangeInsmode();
	}
	else if(nChar == VK_LEFT) //�����L�[
	{
		if((nFlags&BIT_14)!=0)//�L�[�A������
		{
			for(nCnt=0; nCnt<nMultValueOfCaretAccelarete; nCnt++)
				MoveCaret(VK_LEFT,1);
		}
		else
			MoveCaret(VK_LEFT,1);
	}
	else if(nChar == VK_RIGHT) //�E���L�[
	{
		if((nFlags&BIT_14)!=0)//�L�[�A������
		{
			for(nCnt=0; nCnt<nMultValueOfCaretAccelarete; nCnt++)
				MoveCaret(VK_RIGHT,1);
		}
		else
			MoveCaret(VK_RIGHT,1);
	}
	else if(nChar == VK_DOWN) //�����L�[
	{
		if((nFlags&BIT_14)!=0)//�L�[�A������
		{
			for(nCnt=0; nCnt<nMultValueOfCaretAccelarete; nCnt++)
				MoveCaret(VK_DOWN,(int)nRepCnt);
		}
		else
			MoveCaret(VK_DOWN,(int)nRepCnt);
	}
	else if(nChar == VK_UP)	//����L�[
	{
		if((nFlags&BIT_14)!=0)//�L�[�A������
		{
			for(nCnt=0; nCnt<nMultValueOfCaretAccelarete; nCnt++)
				MoveCaret(VK_UP,(int)nRepCnt);
		}
		else
			MoveCaret(VK_UP,(int)nRepCnt);
	}
	else if(nChar == VK_HOME) //HOME�L�[
		MoveCaret(VK_HOME,0);
	else if(nChar == VK_END) //END�L�[
		MoveCaret(VK_END,0);
	else if(nChar == VK_PRIOR) //PAGE UP�L�[
	{
		if((nFlags&BIT_14)!=0)//�L�[�A������
		{
			for(nCnt=0; nCnt<nMultValueOfCaretAccelarete; nCnt++)
				MoveCaret(VK_PRIOR,(int)nRepCnt);
		}
		else
			MoveCaret(VK_PRIOR,(int)nRepCnt);
	}
	else if(nChar == VK_NEXT) //PAGE DOWN�L�[
	{
		if((nFlags&BIT_14)!=0)//�L�[�A������
		{
			for(nCnt=0; nCnt<nMultValueOfCaretAccelarete; nCnt++)
				MoveCaret(VK_NEXT,(int)nRepCnt);
		}
		else
			MoveCaret(VK_NEXT,(int)nRepCnt);
	}
	else if(nChar == VK_RETURN) //���^�[���L�[
	{
		if( pDoc->IsReadOnly() )
		{
			MessageBeep((UINT)-1);
		}
		else
		{
			//���ݺ��ޏ���
			CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
			pDoc->ProcessReturnCode( lCurrentLine,
									 lCurrentOffset,
									 m_caretPos,
									 objScrollPosLog,
									 m_bDispRuler,
									 m_bDispLineNo );
			OnUpdate(NULL,NULL,NULL);
			MoveCaret(VK_DOWN,1);
			MoveCaret(VK_HOME,0);
			Invalidate(FALSE);
		}
	}
	else if(nChar == VK_DELETE) //DELETE�L�[
	{
		if( pDoc->IsReadOnly() )
		{
			MessageBeep((UINT)-1);
		}
		else
		{
			if( IsSelected() )
			{
				AdjustSelectArea();//�͈͑I���ʒu����
				//�I��͈̓e�L�X�g�폜
				CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
				pDoc->DeleteSelectedArea(m_lSelectStartLine,
										 m_lSelectStartOffset,
										 m_lSelectEndLine,
										 m_lSelectEndOffset,
										 FALSE,
										 m_caretPos,
										 objScrollPosLog,
										 m_bDispRuler,
										 m_bDispLineNo );
				OnUpdate(NULL,NULL,NULL);
				Invalidate(FALSE);
			}
			else //�͈͑I�����Ă��Ȃ�
			{
				//DELETE�R�[�h����
				CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
				pDoc->ProcessDeleteCode( lCurrentLine,
										 lCurrentOffset,
										 m_caretPos,
										 objScrollPosLog,
										 m_bDispRuler,
										 m_bDispLineNo );
			}
		}
	}
	else if(nChar == VK_BACK) //BACK SPACE�L�[
	{
		if( pDoc->IsReadOnly() )
		{
			MessageBeep((UINT)-1);
		}
		else
		{
			if( IsSelected() )
			{
				AdjustSelectArea();//�͈͑I���ʒu����
				//�I��͈̓e�L�X�g�폜
				CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
				pDoc->DeleteSelectedArea(m_lSelectStartLine,
										 m_lSelectStartOffset,
										 m_lSelectEndLine,
										 m_lSelectEndOffset,
										 FALSE,
										 m_caretPos,
										 objScrollPosLog,
										 m_bDispRuler,
										 m_bDispLineNo );
				OnUpdate(NULL,NULL,NULL);
				Invalidate(FALSE);
			}
			else //�͈͑I�����Ă��Ȃ�
			{
				//BACK SPACE�R�[�h����
				CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
				pDoc->ProcessBackSpaceCode( lCurrentLine,
											lCurrentOffset,
											m_caretPos,
											objScrollPosLog,
											m_bDispRuler,
											m_bDispLineNo );
			}
		}
	}

	//�h�����n���h���R�[��
	CView::OnKeyDown(nChar, nRepCnt, nFlags);

	if( (nChar == VK_LEFT) || (nChar == VK_RIGHT) || (nChar == VK_DOWN) || //�L�����b�g�ړ��L�[
		(nChar == VK_UP)   || (nChar == VK_HOME)  || (nChar == VK_END)  ||
		(nChar == VK_PRIOR)|| (nChar == VK_NEXT)
	)
	{
		if(m_bIsShiftKeyDown)//SHIFT�L�[������
		{
			// �I��͈͂̍ĕ`�揈��
			RedrawSelectArea(m_lPreMoveCaretLine,m_lPreMoveCaretOffset);
		}
		else //SHIFT�L�[�������Ă��Ȃ�
		{
			//�͈͑I������
			ClearSelectArea();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�j�d�x�t�o����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();

	// �L�[����̋L�^���`�F�b�N
	if(pWnd->IsNowOperationRecording())
	{
		if(nChar==VK_SHIFT)
		{
			//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
			COperation* pobjOperation = new COperation();
			pobjOperation->SetOperationKind(KEYOPE_ONKEYUP);
			pobjOperation->SetOnKeyUpParameter(nChar,nRepCnt,nFlags);
			//�L�^
			pWnd->addOperation(pobjOperation);
		}
	}

	if(nChar == VK_SHIFT)
		m_bIsShiftKeyDown = FALSE;

	// ���N���X�̃n���h���R�[��
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�b�g�`�q����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect	clientRect;
	GetClientRect(&clientRect);
	clientRect.NormalizeRect();
	CString objMsg;

	//�����݋֎~�`�F�b�N
	if( pDoc->IsReadOnly() )
	{
		if(!pDoc->IsDirCmp())
			MessageBeep((UINT)-1);
		CView::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	// �C���N�������^���T�[�`���s���`�F�b�N
	if(pWnd->IsNowIncrimentalSearching())
	{
		if(m_n1stChar == 0) //�����܂��ێ����Ă��Ȃ�
		{
			if(IsDBCSLeadByteEx(0,nChar))//����޲ĕ����̂P������
			{
				//�ޯ̧�֕ێ�
				m_n1stChar = nChar;
				CView::OnChar(nChar, nRepCnt, nFlags);
				return;
			}
			else //�ݸ���޲ĕ���
			{
				if( (nChar==VK_RETURN) || (nChar == VK_TAB) )
				{
					//����
					CView::OnChar(nChar, nRepCnt, nFlags);
					return;
				}
				else if(nChar==VK_BACK)
				{
					//���̂P�����폜
					m_objIncrimentalSearchLine.DelEndChar();
					if(m_objIncrimentalSearchLine.GetLength()==0)
					{
						//�����֖߂�
						m_lIncrimentalSearchStartLine   = 0;
						m_lIncrimentalSearchStartOffset = 0;
						ClearSelectArea();
						Jump2LineOffsetEx(0,0);
						CView::OnChar(nChar, nRepCnt, nFlags);
						return;
					}
					else
					{
						//���o�[�X�C���N�������^���T�[�`���s
						CString objStr = m_objIncrimentalSearchLine.String();
						BOOL bRet = DoIncrimentalSearch( m_lIncrimentalSearchStartLine,
														 m_lIncrimentalSearchStartOffset,
														 objStr,
														 FALSE );
						if(bRet != FALSE)//�����I��
						{
							//���񌟍��J�n�s�A�I�t�Z�b�g�ێ�
							m_lIncrimentalSearchStartLine   = m_lSelectStartLine;
							m_lIncrimentalSearchStartOffset = m_lSelectStartOffset;
						}
						//�I��
						CView::OnChar(nChar,nRepCnt,nFlags);
						return;
					}
				}
				else if(nChar==VK_ESCAPE)
				{
					//�C���N�������^���T�[�`���I������
					objMsg.LoadString(IDS_END_INCRIMENTALSEARCH);
					m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
					m_lIncrimentalSearchStartLine = 0;
					m_lIncrimentalSearchStartOffset = 0;
					pWnd->SetFlagIncrimentalSearching(FALSE);
					m_n1stChar = 0;
					m_n2ndChar = 0;
					CView::OnChar(nChar, nRepCnt, nFlags);
					return;
				}
				else
				{
					m_objIncrimentalSearchLine.Catenate((UCHAR)nChar);
					m_n1stChar = 0;
					m_n2ndChar = 0;
				}
			}
		}
		else //����޲Ă�ێ��ς�
		{
			char szBuf[4];
			szBuf[0] = m_n1stChar;
			szBuf[1] = nChar;
			szBuf[2] = 0;
			m_objIncrimentalSearchLine.Catenate((const unsigned char*)szBuf);
			m_n1stChar = 0;
			m_n2ndChar = 0;
		}

		//�C���N�������^���T�[�`���s
		CString objStr = m_objIncrimentalSearchLine.String();
		BOOL bRet = DoIncrimentalSearch( m_lIncrimentalSearchStartLine,
										 m_lIncrimentalSearchStartOffset,
										 objStr,
										 TRUE );
		if(bRet != FALSE)//�����I��
		{
			//���񌟍��J�n�s�A�I�t�Z�b�g�ێ�
			m_lIncrimentalSearchStartLine   = m_lSelectStartLine;
			m_lIncrimentalSearchStartOffset = m_lSelectStartOffset;
		}
		else //�������I��
		{
			char szBuf[512];
			objMsg.LoadString(IDS_INCREMENTALSEARCH_END);
			sprintf(szBuf,(LPCTSTR)objMsg,(LPCTSTR)objStr);
			m_pApp->MyMessageBox((LPCTSTR)objMsg,(MB_OK|MB_ICONINFORMATION),0);
			m_lIncrimentalSearchStartLine = 0;
			m_lIncrimentalSearchStartOffset = 0;
			pWnd->SetFlagIncrimentalSearching(FALSE);
		}

		//�I��
		CView::OnChar(nChar,nRepCnt,nFlags);
		return;
	}

	// �L�[����̋L�^���`�F�b�N
	if(pWnd->IsNowOperationRecording())
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONCHAR);
		pobjOperation->SetOnCharParameter(nChar,nRepCnt,nFlags);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	// �͈͑I���ς݃`�F�b�N
	if(IsSelected())
	{
		if(!IsDBCSLeadByteEx(0,nChar) && (nChar==VK_TAB)) //���p�^�u
		{
			//�������Ȃ�
		}
		else if(!IsDBCSLeadByteEx(0,nChar) && (nChar==VK_ESCAPE)) //ESCAPE
		{
			//�I��͈͉���
			ClearSelectArea();
		}
		else if(!IsDBCSLeadByteEx(0,nChar) && !isprint(nChar) ) //
		{
			//�������Ȃ�
		}
		else
		{
			AdjustSelectArea();//�͈͑I���ʒu����
			//�I��͈̓e�L�X�g�폜
			CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
			pDoc->DeleteSelectedArea(m_lSelectStartLine,
									 m_lSelectStartOffset,
									 m_lSelectEndLine,
									 m_lSelectEndOffset,
									 FALSE,
									 m_caretPos,
									 objScrollPosLog,
									 m_bDispRuler,
									 m_bDispLineNo );
		}
	}

	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	long lLineLength    = pDoc->GetLineLength(lCurrentLine);
	long lMaxLine       = pDoc->GetLineCount();


	if(m_n1stChar == 0) //�����܂��ێ����Ă��Ȃ�
	{
		if(IsDBCSLeadByteEx(0,nChar))//����޲ĕ����̂P������
		{
			//�ޯ̧�֕ێ�
			m_n1stChar = nChar;
			CView::OnChar(nChar, nRepCnt, nFlags);
			return;
		}
		else //�ݸ���޲ĕ���
		{
			if( (nChar==VK_BACK) ||	(nChar==VK_RETURN) || (nChar == VK_ESCAPE) )
			{
				//����
				CView::OnChar(nChar, nRepCnt, nFlags);
				return;
			}
			else
			{
				if( (nChar==VK_TAB) && IsSelected() )
				{
					if(::GetAsyncKeyState(VK_SHIFT)<0) //Shift Key ������
					{
						AdjustSelectArea();//�͈͑I���ʒu����
						//�͈͑I�����Ă����Ԃł̂r���������{�s�`�a�L�[����
						CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
						pDoc->ProcessShiftTabWhenSelected( m_lSelectStartLine,
														  m_lSelectStartOffset,
														  m_lSelectEndLine,
														  m_lSelectEndOffset,
														  m_caretPos,
														  objScrollPosLog,
														  m_bDispRuler,
														  m_bDispLineNo );
					}
					else
					{
						AdjustSelectArea();//�͈͑I���ʒu����
						//�͈͑I�����Ă����Ԃł̂s�`�a�L�[����
						CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
						pDoc->ProcessTabWhenSelected( m_lSelectStartLine,
													  m_lSelectStartOffset,
													  m_lSelectEndLine,
													  m_lSelectEndOffset,
													  m_caretPos,
													  objScrollPosLog,
													  m_bDispRuler,
													  m_bDispLineNo );
					}
				}
				else
				{
					//�����o�^
					CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
					pDoc->AddCharacter(	lCurrentLine,
										lCurrentOffset,
										nChar,
										NULL,
										m_bInsertKey,
										m_caretPos,
										objScrollPosLog,
										m_bDispRuler,
										m_bDispLineNo );
				}
				m_n1stChar = 0;
				m_n2ndChar = 0;
			}
		}
	}
	else //����޲Ă�ێ��ς�
	{
		//�����o�^
		CPoint objScrollPosLog(GetScrollPosLogX(),GetScrollPosLogY());
		pDoc->AddCharacter( lCurrentLine,
							lCurrentOffset,
							m_n1stChar,
							nChar,
							m_bInsertKey,
							m_caretPos,
							objScrollPosLog,
							m_bDispRuler,
							m_bDispLineNo );
		m_n1stChar = 0;
		m_n2ndChar = 0;
	}


	//�ĕ`��
	OnUpdate(NULL,NULL,NULL);
	if( (nChar==VK_TAB) && IsSelected() ) //�͈͑I�����ꂽ�̈�̃^�u
	{
		//�I��̈�̂ݍĕ`��
		AdjustSelectArea();
		long lRedrawLineCnt=0;
		if(m_lSelectStartLine == lCurrentLine)
			lRedrawLineCnt = m_lSelectEndLine - m_lSelectStartLine; //�v���X�l
		else if(m_lSelectEndLine == lCurrentLine)
			lRedrawLineCnt = -1 * (m_lSelectEndLine - m_lSelectStartLine); //�}�C�i�X�l
		InvalidateCaretLineEx(lRedrawLineCnt);
	}
	else
	{
		InvalidateCaretLine();//�L�����b�g�̂���P�s���ĕ`��
		//�L�����b�g�ړ�
		MoveCaret(VK_RIGHT,1);
	}

	//�h�����n���h���R�[��
	CView::OnChar(nChar, nRepCnt, nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FCtrl + ] ����������													|*/
/*|	���	�F�Ή����銇�ʂփL�����b�g�ړ�											|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnParenthesis()
{
	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if(pWnd->IsNowOperationRecording())
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONPARENTHESIS);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�Ή����銇�ʂւ̃L�����b�g�ʒu�Â�
	MySetCaretToMatchBrace(FALSE);
}
void CEditorView::OnUpdateParenthesis(CCmdUI* pCmdUI)
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	// �L�����b�g�ʒu�̍s�擾
	CLine*	pobjLine = NULL;
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	// �L�����b�g�ʒu�̕����擾
	UCHAR unCurrentChar = pobjLine->GetAt(lCurrentOffset);
	if( IsDBCSLeadByteEx(0,unCurrentChar) )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if( unCurrentChar==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	if( unCurrentChar!='(' &&
		unCurrentChar!=')' &&
		unCurrentChar!='{' &&
		unCurrentChar!='}' &&
		unCurrentChar!='[' &&
		unCurrentChar!=']' &&
		unCurrentChar!='<' &&
		unCurrentChar!='>'
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FCtrl + Shift + E ����������											|*/
/*|	���	�F�Ή����銇�ʂփL�����b�g�ړ����I��									|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnParenthesisSelect()
{
	// �L�[����̋L�^���`�F�b�N
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	if(pWnd->IsNowOperationRecording())
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONPARENTHESISSELECT);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	//�Ή����銇�ʂւ̃L�����b�g�ʒu�Â�
	MySetCaretToMatchBrace(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�L�����b�g�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�b�������{�E���L�[����������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnWordskip2right() 
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// �L�[����̋L�^���`�F�b�N
	if(pWnd->IsNowOperationRecording())
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONWORDSKIP2RIGHT);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	CPoint	objCaretPos = m_caretPos;
	//�L�����b�g�ʒu��_�����W�֕ϊ�����
	CPoint	logicalPoint = objCaretPos;
	logicalPoint -= m_LineNoPos;
	logicalPoint -= m_RulerPos;
	logicalPoint.x += GetScrollPosLogX();
	logicalPoint.y += GetScrollPosLogY();

	CRect clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	if( (m_lfWidth==0) || (m_lfHeight==0) )
		return;
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	long	lCnt=0;
	long	lEndOffset = lCurrentOffset;
	CLine*	pobjLine = NULL;
	long lMaxLine       = pDoc->GetLineCount();

	//�L�����b�g�ʒu�̍s�擾
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
		return;

	//�܂��I������������
	if(IsSelected())
		ClearSelectArea();

	//////////////////////////////////////
	//		�L�����b�g�ړ���ݒ�		//
	//////////////////////////////////////
	if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) && //�L�����b�g�ʒu�����p
		pobjLine->GetAt(lCurrentOffset) != 0
	) 
	{
		int nMargin=0;
		if( isHanKana(pobjLine->GetAt(lCurrentOffset)) )//���p����
		{
			// �L�����b�g�E�����ړ��������
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
				}
			}
		}
		else if( ( isAlnum(pobjLine->GetAt(lCurrentOffset)) || (pobjLine->GetAt(lCurrentOffset)=='_')
				 ) &&
				 !isHanKana(pobjLine->GetAt(lCurrentOffset))
		)
		{
			// �L�����b�g�E�����ړ��������
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt))	||
					( !isAlnum(pobjLine->GetAt(lCnt)) &&
					  (pobjLine->GetAt(lCnt)!='_')
					)											||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
				}
			}
		}
		else
		{
			// �L�����b�g�E�����ړ��������
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) 		||
					isAlnum(pobjLine->GetAt(lCnt))					||
					(pobjLine->GetAt(lCnt)=='_')					||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
				}
			}
		}

		if( logicalPoint.x + nMargin > nWidth ) //�\���͈͂̉E���֍s�����ꍇ
		{
			//�E�����X�N���[��
			MyScrollToPosition(CPoint(GetScrollPosLogX()+nMargin, GetScrollPosLogY()));
		}
		//�L�����b�g�ʒu�ݒ�
		MySetCaretToLineOffset(lCurrentLine,lEndOffset);
	}
	else if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //�L�����b�g�ʒu���S�p����
	{
		int nMargin=0;

		//�����R�[�h�擾
		UINT unChar = ( (pobjLine->GetAt(lCurrentOffset) << 8) | (pobjLine->GetAt(lCurrentOffset+1)) );
		if( _ismbchira( unChar ) ) //�L�����b�g�ʒu��������޲ĂЂ炪��
		{
			// �L�����b�g�E�����ړ��������
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!_ismbchira( unCharTmp )
				)
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
				}
			}
		}
		else if( (_ismbckata(unChar)) ||
				 (unChar==0x815B)			//�L�����b�g�ʒu��������޲Ă������Ȃ����h�[�h
		)
		{
			// �L�����b�g�E�����ړ��������
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbckata( unCharTmp ) && (unCharTmp!=0x815B) )
				)
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
				}
			}
		}
		else if( _ismbcl1(unChar) || _ismbcl2(unChar) ) //�L�����b�g�ʒu��������޲Ċ���
		{
			// �L�����b�g�E�����ړ��������
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbcl1( unCharTmp ) && !_ismbcl2( unCharTmp ) )
				)
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
				}
			}
		}
		else //���̑��̂Q�o�C�g����
		{
			// �L�����b�g�E�����ړ��������
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( _ismbchira( unCharTmp ) ||
					  _ismbckata(unCharTmp)   ||
					  (unCharTmp==0x815B)     ||
					  _ismbcl1( unCharTmp )   ||
					  _ismbcl2( unCharTmp )
					)
				)
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
				}
			}
		}

		if( logicalPoint.x + nMargin > nWidth ) //�\���͈͂̉E���֍s�����ꍇ
		{
			//�E�����X�N���[��
			MyScrollToPosition(CPoint(GetScrollPosLogX()+nMargin, GetScrollPosLogY()));
		}
		//�L�����b�g�ʒu�ݒ�
		MySetCaretToLineOffset(lCurrentLine,lEndOffset);
	}
	else //�L�����b�g�ʒu���s��
	{
		if(lCurrentLine+1 < lMaxLine) //�܂����֍s����
		{
			//MyScrollToPosition���P��̂݃R�[�����邽��
			CPoint	objCaretPos = m_caretPos;
			CPoint	destPos(GetScrollPosLogX(),GetScrollPosLogY());
			if(GetScrollPosLogX() > 0)
			{
				//���X�N���[��
				destPos.x = 0;
			}
			//�L�����b�g�����̍s��
			if((objCaretPos.y+(m_lfHeight+m_lLineInterval)*2) > nHeight)//�\���͈͂̉����ɍs�����ꍇ
			{
				//�P�s���������X�N���[��
				destPos.y += (m_lfHeight+m_lLineInterval);
			}

			if( destPos.x != GetScrollPosLogX() ||
				destPos.y != GetScrollPosLogY()
			)
			{
				MyScrollToPosition( destPos );
			}

			lCurrentLine++;
			lEndOffset=0;
			//�L�����b�g�ʒu�ݒ�
			MySetCaretToLineOffset(lCurrentLine,lEndOffset);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�b�������{�����L�[����������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnWordskip2left() 
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// �L�[����̋L�^���`�F�b�N
	if(pWnd->IsNowOperationRecording())
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONWORDSKIP2LEFT);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	CPoint	objCaretPos = m_caretPos;
	//�L�����b�g�ʒu��_�����W�֕ϊ�����
	CPoint	logicalPoint = objCaretPos;
	logicalPoint -= m_LineNoPos;
	logicalPoint -= m_RulerPos;
	logicalPoint.x += GetScrollPosLogX();
	logicalPoint.y += GetScrollPosLogY();

	CRect clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	if( (m_lfWidth==0) || (m_lfHeight==0) )
		return;
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	long	lCnt=0;

	//�ЂƂO��
	lCurrentOffset = GetPreOffset(lCurrentLine,lCurrentOffset);
	CLine*	pobjLine = NULL;

	//�L�����b�g�ʒu�̍s�擾
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
		return;

	//�܂��I������������
	if(IsSelected())
		ClearSelectArea();

	//////////////////////////////////////
	//		�L�����b�g�ړ���ݒ�		//
	//////////////////////////////////////
	if(lCurrentOffset == -1)
	{
		if(lCurrentLine > 0)//�܂���֍s����
		{
			//MyScrollToPotision���P��̃R�[���ɂ��邽�߂̏���
			CPoint	destPos(GetScrollPosLogX(),GetScrollPosLogY());
			//�L�����b�g�𕶎���̍Ō����
			CPoint	tempLogPos(0,logicalPoint.y-(m_lfHeight+m_lLineInterval));
			GetPosition(lCurrentLine-1,&tempLogPos);//�s���̈ʒu�擾
			if(tempLogPos.x > nWidth)//�\���͈͂̉E���֍s�����ꍇ
			{
				//�E�����X�N���[��
				long lLogX = tempLogPos.x - nWidth + 1;
				destPos.x = lLogX;
			}
			//�_�����W�|���f�o�C�X���W
			objCaretPos = tempLogPos;
			objCaretPos -= destPos;/*m_ScrollPosLog;*/
			objCaretPos += m_RulerPos;
			objCaretPos += m_LineNoPos;
			if(objCaretPos.y < m_RulerPos.y )//�\���͈͂̏㑤�ɍs�����ꍇ
			{
				objCaretPos.y = m_RulerPos.y;
				//�P�s��������X�N���[��
				destPos.y -= (m_lfHeight+m_lLineInterval);
			}
			if( destPos.x != GetScrollPosLogX() ||
				destPos.y != GetScrollPosLogY()
			)
			{
				MyScrollToPosition(destPos);
			}
		}

		//�L�����b�g�ʒu�ݒ�
		MySetCaretPos(objCaretPos);		//�L�����b�g�ʒu�ݒ�i�N���C�A���g���W�n�j
	}
	else if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //�L�����b�g�ʒu�����p
	{
		CPoint tempPos(0,logicalPoint.y), tempPos2=logicalPoint;
		if( isHanKana(pobjLine->GetAt(lCurrentOffset)) )//���p����
		{
			// �L�����b�g�������ړ��������
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // �O�ֈʒu�Â�
				if(lCnt == -1)
					break;

				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					break;
				}
			}
		}
		else if( ( isAlnum(pobjLine->GetAt(lCurrentOffset)) || (pobjLine->GetAt(lCurrentOffset)=='_')
				 ) &&
				 !isHanKana(pobjLine->GetAt(lCurrentOffset))
		)
		{
			// �L�����b�g�������ړ��������
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // �O�ֈʒu�Â�
				if(lCnt == -1)
					break;

				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt))	||
					( !isAlnum(pobjLine->GetAt(lCnt)) &&
					  (pobjLine->GetAt(lCnt)!='_')
					)											||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					break;
				}
			}
		}
		else
		{
			// �L�����b�g�������ړ��������
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // �O�ֈʒu�Â�
				if(lCnt == -1)
					break;

				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) 		||
					isAlnum(pobjLine->GetAt(lCnt))					||
					(pobjLine->GetAt(lCnt)=='_')					||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					break;
				}
			}
		}

		objCaretPos = tempPos;
		//�_�����W�|���f�o�C�X���W
		objCaretPos.x -= GetScrollPosLogX();
		objCaretPos.y -= GetScrollPosLogY();
		objCaretPos += m_RulerPos;
		objCaretPos += m_LineNoPos;
		if(objCaretPos.x < m_LineNoPos.x)//�\���͈͂̍����֍s�����ꍇ
		{
			//�������X�N���[��
			long lLogX = objCaretPos.x - m_LineNoPos.x - m_RulerPos.x + GetScrollPosLogX();
			MyScrollToPosition(CPoint(lLogX, GetScrollPosLogY()));
			objCaretPos.x = m_LineNoPos.x;
		}

		MySetCaretPos(objCaretPos);		//�L�����b�g�ʒu�ݒ�i�N���C�A���g���W�n�j
	}
	else if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //�L�����b�g�ʒu���S�p����
	{
		CPoint tempPos(0,logicalPoint.y), tempPos2=logicalPoint;

		//�����R�[�h�擾
		UINT unChar = ( (pobjLine->GetAt(lCurrentOffset) << 8) | (pobjLine->GetAt(lCurrentOffset+1)) );
		if( _ismbchira( unChar ) ) //�L�����b�g�ʒu��������޲ĂЂ炪��
		{
			// �L�����b�g�������ړ��������
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // �O�ֈʒu�Â�
				if(lCnt == -1)
					break;

				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!_ismbchira( unCharTmp )
				)
				{
					break;
				}
			}
		}
		else if( (_ismbckata(unChar)) ||
				 (unChar==0x815B)			//�L�����b�g�ʒu��������޲Ă������Ȃ����h�[�h
		)
		{
			// �L�����b�g�������ړ��������
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // �O�ֈʒu�Â�
				if(lCnt == -1)
					break;

				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbckata( unCharTmp ) && (unCharTmp!=0x815B) )
				)
				{
					break;
				}
			}
		}
		else if( _ismbcl1(unChar) || _ismbcl2(unChar) ) //�L�����b�g�ʒu��������޲Ċ���
		{
			//�L�����b�g�������ړ��������
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // �O�ֈʒu�Â�
				if(lCnt == -1)
					break;

				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbcl1( unCharTmp ) && !_ismbcl2( unCharTmp ) )
				)
				{
					break;
				}
			}
		}
		else //���̑��̂Q�o�C�g����
		{
			//�L�����b�g�������ړ��������
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // �O�ֈʒu�Â�
				if(lCnt == -1)
					break;

				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( _ismbchira( unCharTmp ) ||
					  _ismbckata(unCharTmp)   ||
					  (unCharTmp==0x815B)     ||
					  _ismbcl1( unCharTmp )   ||
					  _ismbcl2( unCharTmp )
					)
				)
				{
					break;
				}
			}
		}

		objCaretPos = tempPos;
		//�_�����W�|���f�o�C�X���W
		objCaretPos.x -= GetScrollPosLogX();
		objCaretPos.y -= GetScrollPosLogY();
		objCaretPos += m_RulerPos;
		objCaretPos += m_LineNoPos;
		if(objCaretPos.x < m_LineNoPos.x)//�\���͈͂̍����֍s�����ꍇ
		{
			//�������X�N���[��
			long lLogX = objCaretPos.x - m_LineNoPos.x - m_RulerPos.x + GetScrollPosLogX();
			MyScrollToPosition(CPoint(lLogX, GetScrollPosLogY()));
			objCaretPos.x = m_LineNoPos.x;
		}

		MySetCaretPos(objCaretPos);		//�L�����b�g�ʒu�ݒ�i�N���C�A���g���W�n�j
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�b�������{�r���������{�E���L�[����������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnWordselect2right() 
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// �L�[����̋L�^���`�F�b�N
	if(pWnd->IsNowOperationRecording())
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONWORDSELECT2RIGHT);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	CPoint	objCaretPos = m_caretPos;
	//�L�����b�g�ʒu��_�����W�֕ϊ�����
	CPoint	logicalPoint = objCaretPos;
	logicalPoint -= m_LineNoPos;
	logicalPoint -= m_RulerPos;
	logicalPoint.x += GetScrollPosLogX();
	logicalPoint.y += GetScrollPosLogY();

	CRect clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	if( (m_lfWidth==0) || (m_lfHeight==0) )
		return;
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	long	lCnt=0;
	long	lEndOffset = lCurrentOffset;
	CLine*	pobjLine = NULL;
	long lMaxLine       = pDoc->GetLineCount();

	//�L�����b�g�ړ�����܂��̈ʒu���L��
	m_lPreMoveCaretLine   = lCurrentLine;
	m_lPreMoveCaretOffset = lCurrentOffset;
	if( (m_lSelectStartLine == -1) || (m_lSelectStartOffset == -1) )
	{
		//�I���J�n�ʒu���L��
		m_lSelectStartLine   = lCurrentLine;
		m_lSelectStartOffset = lCurrentOffset;
		m_lSelectEndLine     = -1;
		m_lSelectEndOffset   = -1;
	}

	//�L�����b�g�ʒu�̍s�擾
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
		return;

	//////////////////////////////////////
	//		�L�����b�g�ړ���ݒ�		//
	//////////////////////////////////////
	if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) && //�L�����b�g�ʒu�����p
		pobjLine->GetAt(lCurrentOffset) != 0
	)
	{
		int nMargin=0;
		if( isHanKana(pobjLine->GetAt(lCurrentOffset)) )//���p����
		{
			// �L�����b�g�E�����ړ��������
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
				}
			}
		}
		else if( ( isAlnum(pobjLine->GetAt(lCurrentOffset)) || (pobjLine->GetAt(lCurrentOffset)=='_')
				 ) &&
				 !isHanKana(pobjLine->GetAt(lCurrentOffset))
		)
		{
			// �L�����b�g�E�����ړ��������
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt))	||
					( !isAlnum(pobjLine->GetAt(lCnt)) &&
					  (pobjLine->GetAt(lCnt)!='_')
					)											||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
				}
			}
		}
		else
		{
			// �L�����b�g�E�����ړ��������
			for(lCnt=lCurrentOffset; ; lCnt++)
			{
				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) 		||
					isAlnum(pobjLine->GetAt(lCnt))					||
					(pobjLine->GetAt(lCnt)=='_')					||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += m_lfWidth;
				}
			}
		}

		if( logicalPoint.x + nMargin > nWidth ) //�\���͈͂̉E���֍s�����ꍇ
		{
			//�E�����X�N���[��
			MyScrollToPosition(CPoint(GetScrollPosLogX()+nMargin, GetScrollPosLogY()));
		}
		//�L�����b�g�ʒu�ݒ�
		MySetCaretToLineOffset(lCurrentLine,lEndOffset);
	}
	else if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //�L�����b�g�ʒu���S�p����
	{
		int nMargin=0;

		//�����R�[�h�擾
		UINT unChar = ( (pobjLine->GetAt(lCurrentOffset) << 8) | (pobjLine->GetAt(lCurrentOffset+1)) );
		if( _ismbchira( unChar ) ) //�L�����b�g�ʒu��������޲ĂЂ炪��
		{
			// �L�����b�g�E�����ړ��������
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!_ismbchira( unCharTmp )
				)
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
				}
			}
		}
		else if( (_ismbckata(unChar)) ||
				 (unChar==0x815B)			//�L�����b�g�ʒu��������޲Ă������Ȃ����h�[�h
		)
		{
			// �L�����b�g�E�����ړ��������
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbckata( unCharTmp ) && (unCharTmp!=0x815B) )
				)
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
				}
			}
		}
		else if( _ismbcl1(unChar) || _ismbcl2(unChar) ) //�L�����b�g�ʒu��������޲Ċ���
		{
			// �L�����b�g�E�����ړ��������
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbcl1( unCharTmp ) && !_ismbcl2( unCharTmp ) )
				)
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
				}
			}
		}
		else //���̑��̂Q�o�C�g����
		{
			// �L�����b�g�E�����ړ��������
			for(lCnt=lCurrentOffset; ; lCnt+=2)
			{
				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( _ismbchira( unCharTmp ) ||
					  _ismbckata(unCharTmp)   ||
					  (unCharTmp==0x815B)     ||
					  _ismbcl1( unCharTmp )   ||
					  _ismbcl2( unCharTmp )
					)
				)
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else if( pobjLine->GetAt(lCnt)==0 )
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
					break;
				}
				else
				{
					lEndOffset = lCnt;
					nMargin += (m_lfWidth * 2);
				}
			}
		}

		if( logicalPoint.x + nMargin > nWidth ) //�\���͈͂̉E���֍s�����ꍇ
		{
			//�E�����X�N���[��
			MyScrollToPosition(CPoint(GetScrollPosLogX()+nMargin, GetScrollPosLogY()));
		}
		//�L�����b�g�ʒu�ݒ�
		MySetCaretToLineOffset(lCurrentLine,lEndOffset);
	}
	else //�L�����b�g�ʒu���s��
	{
		if(lCurrentLine+1 < lMaxLine) //�܂����֍s����
		{
			//MyScrollToPosition���P��̂݃R�[�����邽��
			CPoint	objCaretPos = m_caretPos;
			CPoint	destPos(GetScrollPosLogX(),GetScrollPosLogY());
			if(GetScrollPosLogX() > 0)
			{
				//���X�N���[��
				destPos.x = 0;
			}
			//�L�����b�g�����̍s��
			if((objCaretPos.y+(m_lfHeight+m_lLineInterval)*2) > nHeight)//�\���͈͂̉����ɍs�����ꍇ
			{
				//�P�s���������X�N���[��
				destPos.y += (m_lfHeight+m_lLineInterval);
			}

			if( destPos.x != GetScrollPosLogX() ||
				destPos.y != GetScrollPosLogY()
			)
			{
				MyScrollToPosition( destPos );
			}

			lCurrentLine++;
			lEndOffset=0;
			//�L�����b�g�ʒu�ݒ�
			MySetCaretToLineOffset(lCurrentLine,lEndOffset);
		}
	}

	// �I��͈͂̍ĕ`�揈��
	RedrawSelectArea(m_lPreMoveCaretLine,m_lPreMoveCaretOffset);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�b�������{�r���������{�����L�[����������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnWordselect2left() 
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// �L�[����̋L�^���`�F�b�N
	if(pWnd->IsNowOperationRecording())
	{
		//�L�[�I�y���[�V�����I�u�W�F�N�g�쐬
		COperation* pobjOperation = new COperation();
		pobjOperation->SetOperationKind(KEYOPE_ONWORDSELECT2LEFT);
		//�L�^
		pWnd->addOperation(pobjOperation);
	}

	CPoint	objCaretPos = m_caretPos;
	//�L�����b�g�ʒu��_�����W�֕ϊ�����
	CPoint	logicalPoint = objCaretPos;
	logicalPoint -= m_LineNoPos;
	logicalPoint -= m_RulerPos;
	logicalPoint.x += GetScrollPosLogX();
	logicalPoint.y += GetScrollPosLogY();

	CRect clientRect;
	MyGetClientRect(&clientRect);
	clientRect.NormalizeRect();
	int nHeight = clientRect.Height();
	int nWidth = clientRect.Width();
	if( (m_lfWidth==0) || (m_lfHeight==0) )
		return;
	nHeight -= ( clientRect.Height() % (m_lfHeight+m_lLineInterval) );
	nWidth -= ( clientRect.Width() % m_lfWidth );

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;
	long	lCnt=0;
	CLine*	pobjLine = NULL;

	//�L�����b�g�ړ�����܂��̈ʒu���L��
	m_lPreMoveCaretLine   = lCurrentLine;
	m_lPreMoveCaretOffset = lCurrentOffset;
	if( (m_lSelectStartLine == -1) || (m_lSelectStartOffset == -1) )
	{
		//�I���J�n�ʒu���L��
		m_lSelectStartLine   = lCurrentLine;
		m_lSelectStartOffset = lCurrentOffset;
		m_lSelectEndLine     = -1;
		m_lSelectEndOffset   = -1;
	}

	//�ЂƂO��
	lCurrentOffset = GetPreOffset(lCurrentLine,lCurrentOffset);

	//�L�����b�g�ʒu�̍s�擾
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
		return;

	//////////////////////////////////////
	//		�L�����b�g�ړ���ݒ�		//
	//////////////////////////////////////
	if(lCurrentOffset == -1)
	{
		if(lCurrentLine > 0)//�܂���֍s����
		{
			//MyScrollToPotision���P��̃R�[���ɂ��邽�߂̏���
			CPoint	destPos(GetScrollPosLogX(),GetScrollPosLogY());
			//�L�����b�g�𕶎���̍Ō����
			CPoint	tempLogPos(0,logicalPoint.y-(m_lfHeight+m_lLineInterval));
			GetPosition(lCurrentLine-1,&tempLogPos);//�s���̈ʒu�擾
			if(tempLogPos.x > nWidth)//�\���͈͂̉E���֍s�����ꍇ
			{
				//�E�����X�N���[��
				long lLogX = tempLogPos.x - nWidth + 1;
				destPos.x = lLogX;
			}
			//�_�����W�|���f�o�C�X���W
			objCaretPos = tempLogPos;
			objCaretPos -= destPos;/*m_ScrollPosLog;*/
			objCaretPos += m_RulerPos;
			objCaretPos += m_LineNoPos;
			if(objCaretPos.y < m_RulerPos.y )//�\���͈͂̏㑤�ɍs�����ꍇ
			{
				objCaretPos.y = m_RulerPos.y;
				//�P�s��������X�N���[��
				destPos.y -= (m_lfHeight+m_lLineInterval);
			}
			if( destPos.x != GetScrollPosLogX() ||
				destPos.y != GetScrollPosLogY()
			)
			{
				MyScrollToPosition(destPos);
			}
		}

		//�L�����b�g�ʒu�ݒ�
		MySetCaretPos(objCaretPos);		//�L�����b�g�ʒu�ݒ�i�N���C�A���g���W�n�j
	}
	else if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //�L�����b�g�ʒu�����p
	{
		CPoint tempPos(0,logicalPoint.y), tempPos2=logicalPoint;
		if( isHanKana(pobjLine->GetAt(lCurrentOffset)) )//���p����
		{
			// �L�����b�g�������ړ��������
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // �O�ֈʒu�Â�
				if(lCnt == -1)
					break;

				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					break;
				}
			}
		}
		else if( ( isAlnum(pobjLine->GetAt(lCurrentOffset)) || (pobjLine->GetAt(lCurrentOffset)=='_')
				 ) &&
				 !isHanKana(pobjLine->GetAt(lCurrentOffset))
		)
		{
			// �L�����b�g�������ړ��������
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // �O�ֈʒu�Â�
				if(lCnt == -1)
					break;

				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt))	||
					( !isAlnum(pobjLine->GetAt(lCnt)) &&
					  (pobjLine->GetAt(lCnt)!='_')
					)											||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					break;
				}
			}
		}
		else
		{
			// �L�����b�g�������ړ��������
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // �O�ֈʒu�Â�
				if(lCnt == -1)
					break;

				if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) 		||
					isAlnum(pobjLine->GetAt(lCnt))					||
					(pobjLine->GetAt(lCnt)=='_')					||
					isHanKana(pobjLine->GetAt(lCnt))
				)
				{
					break;
				}
			}
		}

		objCaretPos = tempPos;
		//�_�����W�|���f�o�C�X���W
		objCaretPos.x -= GetScrollPosLogX();
		objCaretPos.y -= GetScrollPosLogY();
		objCaretPos += m_RulerPos;
		objCaretPos += m_LineNoPos;
		if(objCaretPos.x < m_LineNoPos.x)//�\���͈͂̍����֍s�����ꍇ
		{
			//�������X�N���[��
			long lLogX = objCaretPos.x - m_LineNoPos.x - m_RulerPos.x + GetScrollPosLogX();
			MyScrollToPosition(CPoint(lLogX, GetScrollPosLogY()));
			objCaretPos.x = m_LineNoPos.x;
		}

		MySetCaretPos(objCaretPos);		//�L�����b�g�ʒu�ݒ�i�N���C�A���g���W�n�j
	}
	else if( IsDBCSLeadByteEx(0,pobjLine->GetAt(lCurrentOffset)) ) //�L�����b�g�ʒu���S�p����
	{
		CPoint tempPos(0,logicalPoint.y), tempPos2=logicalPoint;

		//�����R�[�h�擾
		UINT unChar = ( (pobjLine->GetAt(lCurrentOffset) << 8) | (pobjLine->GetAt(lCurrentOffset+1)) );
		if( _ismbchira( unChar ) ) //�L�����b�g�ʒu��������޲ĂЂ炪��
		{
			// �L�����b�g�������ړ��������
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // �O�ֈʒu�Â�
				if(lCnt == -1)
					break;

				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					!_ismbchira( unCharTmp )
				)
				{
					break;
				}
			}
		}
		else if( (_ismbckata(unChar)) ||
				 (unChar==0x815B)			//�L�����b�g�ʒu��������޲Ă������Ȃ����h�[�h
		)
		{
			// �L�����b�g�������ړ��������
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // �O�ֈʒu�Â�
				if(lCnt == -1)
					break;

				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbckata( unCharTmp ) && (unCharTmp!=0x815B) )
				)
				{
					break;
				}
			}
		}
		else if( _ismbcl1(unChar) || _ismbcl2(unChar) ) //�L�����b�g�ʒu��������޲Ċ���
		{
			// �L�����b�g�������ړ��������
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // �O�ֈʒu�Â�
				if(lCnt == -1)
					break;

				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( !_ismbcl1( unCharTmp ) && !_ismbcl2( unCharTmp ) )
				)
				{
					break;
				}
			}
		}
		else //���̑��̂Q�o�C�g����
		{
			// �L�����b�g�������ړ��������
			for(lCnt=lCurrentOffset; ; )
			{
				GetPosition1(lCurrentLine,&tempPos2,&tempPos);
				tempPos2 = tempPos;
				lCnt = GetPreOffset(lCurrentLine,lCnt); // �O�ֈʒu�Â�
				if(lCnt == -1)
					break;

				//�����R�[�h�擾
				UINT unCharTmp = ( (pobjLine->GetAt(lCnt) << 8) | (pobjLine->GetAt(lCnt+1)) );
				if( !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)) ||
					( _ismbchira( unCharTmp ) ||
					  _ismbckata(unCharTmp)   ||
					  (unCharTmp==0x815B)     ||
					  _ismbcl1( unCharTmp )   ||
					  _ismbcl2( unCharTmp )
					)
				)
				{
					break;
				}
			}
		}

		objCaretPos = tempPos;
		//�_�����W�|���f�o�C�X���W
		objCaretPos.x -= GetScrollPosLogX();
		objCaretPos.y -= GetScrollPosLogY();
		objCaretPos += m_RulerPos;
		objCaretPos += m_LineNoPos;
		if(objCaretPos.x < m_LineNoPos.x)//�\���͈͂̍����֍s�����ꍇ
		{
			//�������X�N���[��
			long lLogX = objCaretPos.x - m_LineNoPos.x - m_RulerPos.x + GetScrollPosLogX();
			MyScrollToPosition(CPoint(lLogX, GetScrollPosLogY()));
			objCaretPos.x = m_LineNoPos.x;
		}

		MySetCaretPos(objCaretPos);		//�L�����b�g�ʒu�ݒ�i�N���C�A���g���W�n�j
	}

	// �I��͈͂̍ĕ`�揈��
	RedrawSelectArea(m_lPreMoveCaretLine,m_lPreMoveCaretOffset);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�c�[���֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�t�c�����������I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditUudecode() 
{
	CEditorDoc* pDoc = (CEditorDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	/* �I��̈���t�t�c���������� */
	pDoc->UuDecode( m_lSelectStartLine,
					m_lSelectStartOffset,
					m_lSelectEndLine,
					m_lSelectEndOffset );
}
void CEditorView::OnUpdateEditUudecode(CCmdUI* pCmdUI) 
{
	if(IsSelected())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�a�������U�S�f�R�[�h�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditBase64decode() 
{
	CEditorDoc* pDoc = (CEditorDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	/* �I��̈���a�������U�S�f�R�[�h */
	pDoc->Base64Decode( m_lSelectStartLine,
						m_lSelectStartOffset,
						m_lSelectEndLine,
						m_lSelectEndOffset );
}
void CEditorView::OnUpdateEditBase64decode(CCmdUI* pCmdUI) 
{
	if(IsSelected())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�p�����������|�o�����������������f�R�[�h�I��������					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditQuoteddecode() 
{
	CEditorDoc* pDoc = (CEditorDoc*)GetDocument();
	ASSERT_VALID(pDoc);

	//�I��̈���p�����������|�o�����������������f�R�[�h
	pDoc->QuotedPrintableDecode( m_lSelectStartLine,
								 m_lSelectStartOffset,
								 m_lSelectEndLine,
								 m_lSelectEndOffset );
}
void CEditorView::OnUpdateEditQuoteddecode(CCmdUI* pCmdUI) 
{
	if(IsSelected())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�t�@�C����r�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u���̑�����փW�����v�v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditNextdiff() 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	LONG	lLineCnt=0;
	int		nDiffLineKindNow = DIFF_LINEKIND_COMMON;
	BOOL 	bFind=FALSE;
	CString objMsg;

	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	// ���ݍs�̃t�@�C����r��ʎ擾
	nDiffLineKindNow = pDoc->GetDiffLineKind(lCurrentLine);
	if(nDiffLineKindNow == DIFF_LINEKIND_COMMON)
	{
		//���̑�����̍s�ԍ�������
		bFind = FALSE;
		for(lLineCnt=lCurrentLine+1; lLineCnt<pDoc->GetLineCount(); lLineCnt++)
		{
			if( pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_DELETED ||
				pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_ADDED
			)
			{
				bFind=TRUE;
				break;
			}
		}
	}
	else
	{
		//���̋��ʌ��̍s�ԍ�������
		bFind = FALSE;
		for(lLineCnt=lCurrentLine+1; lLineCnt<pDoc->GetLineCount(); lLineCnt++)
		{
			if( pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_COMMON )
			{
				bFind=TRUE;
				break;
			}
		}
		if(bFind != FALSE)
		{
			bFind = FALSE;
			lLineCnt++;
			//���̑�����̍s�ԍ�������
			for( ; lLineCnt<pDoc->GetLineCount(); lLineCnt++)
			{
				if( pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_DELETED ||
					pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_ADDED
				)
				{
					bFind=TRUE;
					break;
				}
			}
		}
	}

	if(bFind != FALSE)
	{
		//���̑�����փW�����v
		Jump2LineOffset(lLineCnt,0);
		//�ĕ`��
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);
	}
	else
	{
		//�����Ȃ�
		objMsg.LoadString(IDS_NOMORE_NEXTDIFF);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
	}
}
void CEditorView::OnUpdateEditNextdiff(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�h�L�������g�ɂP�s���f�[�^���Ȃ��ꍇ�͑I��s�ɂ���
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�t�@�C����r���ʂ��ǂ���
	if(pDoc->IsCompare() == FALSE)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�O�̑�����փW�����v�v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditPrevdiff() 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	LONG	lLineCnt=0;
	int		nDiffLineKindNow = DIFF_LINEKIND_COMMON;
	BOOL 	bFind=FALSE;
	CString objMsg;

	// ���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	// ���ݍs�̃t�@�C����r��ʎ擾
	nDiffLineKindNow = pDoc->GetDiffLineKind(lCurrentLine);
	if( nDiffLineKindNow == DIFF_LINEKIND_DELETED || nDiffLineKindNow == DIFF_LINEKIND_ADDED )
	{
		//�O�̋��ʌ��̍s�܂ňړ�
		for(lLineCnt=lCurrentLine-1; lLineCnt>=0; lLineCnt--)
		{
			if( pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_COMMON )
			{
				lCurrentLine = lLineCnt;
				nDiffLineKindNow = DIFF_LINEKIND_COMMON;
				break;
			}
		}
	}
	if(nDiffLineKindNow == DIFF_LINEKIND_COMMON)
	{
		//�O�̑�����̍s�ԍ�������
		bFind = FALSE;
		for(lLineCnt=lCurrentLine-1; lLineCnt>=0; lLineCnt--)
		{
			if( pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_DELETED ||
				pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_ADDED
			)
			{
				bFind=TRUE;
				break;
			}
		}
		if(bFind != FALSE)
		{
			bFind = FALSE;
			lLineCnt--;
			for( ; lLineCnt>=-1; lLineCnt--)
			{
				if( lLineCnt <= -1 )
				{
					lLineCnt = 0;
					bFind = TRUE;
					break;
				}
				if( pDoc->GetDiffLineKind(lLineCnt) == DIFF_LINEKIND_COMMON )
				{
					lLineCnt++;
					bFind = TRUE;
					break;
				}
			}
		}
	}

	if(bFind != FALSE)
	{
		//�O�̑�����փW�����v
		Jump2LineOffset(lLineCnt,0);
		//�ĕ`��
		OnUpdate(NULL,NULL,NULL);
		Invalidate(FALSE);
	}
	else
	{
		//�����Ȃ�
		objMsg.LoadString(IDS_NOMORE_PREVDIFF);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
	}
}
void CEditorView::OnUpdateEditPrevdiff(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�h�L�������g�ɂP�s���f�[�^���Ȃ��ꍇ�͑I��s�ɂ���
	if( pDoc->IsEmpty() )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�t�@�C����r���ʂ��ǂ���
	if(pDoc->IsCompare() == FALSE)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�f�B���N�g����r�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u����t�@�C��������ɏڍׂɔ�r�v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorView::OnEditDircmpSyosai() 
{
	CMainFrame*	pWnd = (CMainFrame*)AfxGetMainWnd();
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
		return;

	// �L�����b�g�ʒu�̍s�擾
	CLine*	pobjLine = NULL;
	pobjLine = pDoc->GetOneLine(lCurrentLine);
	if( pobjLine == NULL )
		return;

	//��r�f�B���N�g�����擾
	CString objDir1,objDir2,objFileName;
	pDoc->GetDirCmpFolder(objDir1,objDir2);
	//����t�@�C�����擾
	pobjLine->GetDirCmpFileName(objFileName);
	//�p�X�쐬
	if(m_pApp->IsDriveRoot(objDir1))
		objDir1 += objFileName;
	else
		objDir1 += "\\" + objFileName;
	if(m_pApp->IsDriveRoot(objDir2))
		objDir2 += objFileName;
	else
		objDir2 += "\\" + objFileName;

	pWnd->DoFileCompare(objDir1,objDir2,NULL);
}
void CEditorView::OnUpdateEditDircmpSyosai(CCmdUI* pCmdUI) 
{
	CEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//������`�F�b�N
	if(pDoc->GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//�f�B���N�g����r���ʃ`�F�b�N
	if(pDoc->IsDirCmp() == FALSE)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//	���݂̃L�����b�g�ʒu�̍s�A��ԍ��擾
	long lCurrentLine = 0;
	long lCurrentOffset = 0;
	if( CaretToLineOffset(&lCurrentLine,&lCurrentOffset) == FALSE )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	// �L�����b�g�ʒu�̕����擾
	if(pDoc->GetDirCmpLineKind(lCurrentLine)==DIRCMP_LINEKIND_DIFF) //�f�B���N�g����r���ʑ���t�@�C��
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}


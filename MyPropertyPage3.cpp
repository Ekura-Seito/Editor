// MyPropertyPage3.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "ColorButton.h"
#include "MyPropertyPage3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage3 �v���p�e�B �y�[�W

IMPLEMENT_DYNCREATE(CMyPropertyPage3, CPropertyPage)

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage3::CMyPropertyPage3() : CPropertyPage(CMyPropertyPage3::IDD)
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsChanged = FALSE;								//�ύX�����t���O
	m_objFileExt = _T("");

	//{{AFX_DATA_INIT(CMyPropertyPage3)
	m_bComment4Clr = FALSE;
	//}}AFX_DATA_INIT

	//�f�t�H���g�J���[������
	m_FontColor         = GetSysColor( COLOR_BTNFACE );		// �ʏ�̕����F
	m_BackColor         = GetSysColor( COLOR_BTNFACE );		// �ʏ�̔w�i�F
	m_InvertFontColor   = GetSysColor( COLOR_BTNFACE );		// ���]���̕����F
	m_InvertBackColor   = GetSysColor( COLOR_BTNFACE );		// ���]���̔w�i�F
	m_KeyNo1Color       = GetSysColor( COLOR_BTNFACE );		// �L�[���[�h�m���P�̕����F
	m_KeyNo2Color       = GetSysColor( COLOR_BTNFACE );		// �L�[���[�h�m���Q�̕����F
	m_KeyNo3Color       = GetSysColor( COLOR_BTNFACE );		// �L�[���[�h�m���R�̕����F
	m_KeyNo4Color       = GetSysColor( COLOR_BTNFACE );		// �L�[���[�h�m���S�̕����F
	m_MarkLineColor		= RGB(128,0,128);					// �}�[�N�s�F
	m_CommentColor      = GetSysColor( COLOR_BTNFACE );		// �R�����g�̕����F�i�b�^�b�{�{�p�j
	m_CommentColor2     = GetSysColor( COLOR_BTNFACE );		// �R�����g�̕����F�i�u�a�p�j
	m_CommentColor3     = GetSysColor( COLOR_BTNFACE );		// �R�����g�̕����F�i�A�Z���u���p�j
	m_CommentColor4     = GetSysColor( COLOR_BTNFACE );		// �R�����g�̕����F�i���p�����p�j
	m_LineNoColor       = GetSysColor( COLOR_BTNFACE );		// �s�ԍ��̐F
	m_LineNoBackColor   = GetSysColor( COLOR_BTNFACE );		// �s�ԍ��̔w�i�̐F
	m_RulerColor        = GetSysColor( COLOR_BTNFACE );		// ���[���̐F
	m_RulerBackColor    = GetSysColor( COLOR_BTNFACE );		// ���[���̔w�i�̐F
	m_CRLFColor         = GetSysColor( COLOR_BTNFACE );		// ���s�}�[�N�̐F
	m_TabColor		    = GetSysColor( COLOR_BTNFACE );		// �^�u�}�[�N�̐F
	m_ZenkakuSpaceColor = GetSysColor( COLOR_BTNFACE );		// �S�p�X�y�[�X�̐F
	m_UnderLineColor    = GetSysColor( COLOR_BTNFACE );		// �A���_�[���C���̐F
	m_UrlColor          = GetSysColor( COLOR_BTNFACE );		// �t�q�k�̐F
	m_EMailColor        = GetSysColor( COLOR_BTNFACE );		// ���|���������̐F
	m_MDIBkgndColor     = RGB(128,128,128);					// �l�c�h�w�i�F
	m_DiffDelLineColor  = GetSysColor( COLOR_BTNFACE );		// �t�@�C����r�p�A�폜�s�F
	m_DiffAddLineColor  = GetSysColor( COLOR_BTNFACE );		// �t�@�C����r�p�A�ǉ��s�F
	m_DirDiffLineColor  = GetSysColor( COLOR_BTNFACE );		// �f�B���N�g����r�p�A����t�@�C���F
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage3::~CMyPropertyPage3()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage3 �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ύX������������														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage3::IsChanged() 
{
	return m_bIsChanged;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ύX�����t���O�N���A����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::ClearChangeFlag()
{
	m_bIsChanged = FALSE;
	SetModified(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�c�c�w�f�[�^����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage3)
	DDX_Control(pDX, IDC_BUTTON_COMMENTCLR4, m_objCommentClrBtn4);
	DDX_Control(pDX, IDC_COMBO_FILEEXT, m_objFileExtCombo);
	DDX_Control(pDX, IDC_BUTTON_TXTCLR,        m_objTxtClrBtn);
	DDX_Control(pDX, IDC_BUTTON_INVTXTCLR,     m_objInvTxtClrBtn);
	DDX_Control(pDX, IDC_BUTTON_INVBKCLR,      m_objInvBkClrBtn);
	DDX_Control(pDX, IDC_BUTTON_BKCLR,         m_objBkClrBtn);
	DDX_Control(pDX, IDC_BUTTON_KEYWORD1CLR,   m_objKeyNo1ClrBtn);
	DDX_Control(pDX, IDC_BUTTON_KEYWORD2CLR,   m_objKeyNo2ClrBtn);
	DDX_Control(pDX, IDC_BUTTON_KEYWORD3CLR,   m_objKeyNo3ClrBtn);
	DDX_Control(pDX, IDC_BUTTON_KEYWORD4CLR,   m_objKeyNo4ClrBtn);
	DDX_Control(pDX, IDC_BUTTON_MARKLINE,      m_objMarkLineClrBtn);
	DDX_Check(pDX,   IDC_CHECK_CPPCOMMENT,     m_bComment1Clr);
	DDX_Control(pDX, IDC_BUTTON_COMMENTCLR,    m_objCommentClrBtn);
	DDX_Check(pDX,   IDC_CHECK_VBCOMMENT,      m_bComment2Clr);
	DDX_Control(pDX, IDC_BUTTON_COMMENTCLR2,   m_objCommentClrBtn2);
	DDX_Check(pDX,   IDC_CHECK_ASMCOMMENT,     m_bComment3Clr);
	DDX_Control(pDX, IDC_BUTTON_COMMENTCLR3,   m_objCommentClrBtn3);
	DDX_Check(pDX,   IDC_CHECK_DISPLINENO,     m_bDispLineNo);
	DDX_Control(pDX, IDC_BUTTON_LINENO,        m_objLineNoClrBtn);
	DDX_Control(pDX, IDC_BUTTON_LINENOBACK,    m_objLineNoBackClrBtn);
	DDX_Check(pDX,   IDC_CHECK_DISPRULER,      m_bDispRuler);
	DDX_Control(pDX, IDC_BUTTON_RULER,         m_objRulerClrBtn);
	DDX_Control(pDX, IDC_BUTTON_RULERBACK,     m_objRulerBackClrBtn);
	DDX_Check(pDX,   IDC_CHECK_DISPCRLF,       m_bDispCRLF);
	DDX_Control(pDX, IDC_BUTTON_CRLFCLR,       m_objCRLFClrBtn);
	DDX_Check(pDX,   IDC_CHECK_DISPTAB,        m_bDispTab);
	DDX_Control(pDX, IDC_BUTTON_TABCLR,        m_objTabClrBtn);
	DDX_Check(pDX,   IDC_CHECK_DISPSPACE,      m_bDispZenkakuSpace);
	DDX_Control(pDX, IDC_BUTTON_SPACECLR,      m_objZenkakuSpaceClrBtn);
	DDX_Check(pDX,   IDC_CHECK_DISPUNDERLINE,  m_bDispUnderLine);
	DDX_Control(pDX, IDC_BUTTON_UNDERLINE,     m_objUnderLineClrBtn);
	DDX_Check(pDX,   IDC_CHECK_DISPURL,        m_bDispUrl);
	DDX_Control(pDX, IDC_BUTTON_URLCLR,        m_objUrlClrBtn);
	DDX_Check(pDX,   IDC_CHECK_DISPEMAIL,      m_bDispEMail);
	DDX_Control(pDX, IDC_BUTTON_EMAILCLR,      m_objEMailClrBtn);
	DDX_Control(pDX, IDC_BUTTON_MDIBKGND,      m_objMDIBkgndClrBtn);
	DDX_Control(pDX, IDC_BUTTON_DIFFDELLINECLR,m_objDiffDelLineClrBtn);
	DDX_Control(pDX, IDC_BUTTON_DIFFADDLINECLR,m_objDiffAddLineClrBtn);
	DDX_Control(pDX, IDC_BUTTON_DIRCMPDIFFFILECLR, m_objDirDiffLineClrBtn);
	DDX_Check(pDX, IDC_CHECK_QUOTE, m_bComment4Clr);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyPropertyPage3, CPropertyPage)
	//{{AFX_MSG_MAP(CMyPropertyPage3)
	ON_BN_CLICKED(IDC_BUTTON_TXTCLR,        OnButtonTxtclr)
	ON_BN_CLICKED(IDC_BUTTON_BKCLR,         OnButtonBkclr)
	ON_BN_CLICKED(IDC_BUTTON_INVBKCLR,      OnButtonInvbkclr)
	ON_BN_CLICKED(IDC_BUTTON_INVTXTCLR,     OnButtonInvtxtclr)
	ON_BN_CLICKED(IDC_BUTTON_KEYWORD1CLR,   OnButtonKeyword1clr)
	ON_BN_CLICKED(IDC_BUTTON_KEYWORD2CLR,   OnButtonKeyword2clr)
	ON_BN_CLICKED(IDC_BUTTON_KEYWORD3CLR,   OnButtonKeyword3clr)
	ON_BN_CLICKED(IDC_BUTTON_KEYWORD4CLR,   OnButtonKeyword4clr)
	ON_BN_CLICKED(IDC_BUTTON_MARKLINE,      OnButtonMarklineclr)
	ON_BN_CLICKED(IDC_CHECK_CPPCOMMENT,     OnChkcppcomment)
	ON_BN_CLICKED(IDC_BUTTON_COMMENTCLR,    OnButtonCommentclr)
	ON_BN_CLICKED(IDC_CHECK_VBCOMMENT,      OnChkvbcomment)
	ON_BN_CLICKED(IDC_BUTTON_COMMENTCLR2,   OnButtonCommentclr2)
	ON_BN_CLICKED(IDC_CHECK_ASMCOMMENT,     OnChkasmcomment)
	ON_BN_CLICKED(IDC_BUTTON_COMMENTCLR3,   OnButtonCommentclr3)
	ON_BN_CLICKED(IDC_CHECK_DISPLINENO,     OnChkDisplineno)
	ON_BN_CLICKED(IDC_BUTTON_LINENO,        OnButtonlinenoclr)
	ON_BN_CLICKED(IDC_BUTTON_LINENOBACK,    OnButtonlinenobackclr)
	ON_BN_CLICKED(IDC_CHECK_DISPRULER,      OnChkDispruler)
	ON_BN_CLICKED(IDC_BUTTON_RULER,         OnButtonrulerclr)
	ON_BN_CLICKED(IDC_BUTTON_RULERBACK,     OnButtonrulerbackclr)
	ON_BN_CLICKED(IDC_CHECK_DISPCRLF,       OnChkDispcrlf)
	ON_BN_CLICKED(IDC_BUTTON_CRLFCLR,       OnButtonCrlfclr)
	ON_BN_CLICKED(IDC_CHECK_DISPTAB,        OnChkDisptab)
	ON_BN_CLICKED(IDC_BUTTON_TABCLR,        OnButtonTabclr)
	ON_BN_CLICKED(IDC_CHECK_DISPSPACE,      OnChkDispzenkakuspace)
	ON_BN_CLICKED(IDC_BUTTON_SPACECLR,      OnButtonZenkakuSpaceclr)
	ON_BN_CLICKED(IDC_CHECK_DISPUNDERLINE,	OnChkDispunderline)
	ON_BN_CLICKED(IDC_BUTTON_UNDERLINE,		OnButtonUnderLineclr)
	ON_BN_CLICKED(IDC_CHECK_DISPURL,        OnChkDispurl)
	ON_BN_CLICKED(IDC_BUTTON_URLCLR,        OnButtonUrlclr)
	ON_BN_CLICKED(IDC_CHECK_DISPEMAIL,      OnChkDispemail)
	ON_BN_CLICKED(IDC_BUTTON_EMAILCLR,      OnButtonEmailclr)
	ON_BN_CLICKED(IDC_BUTTON_MDIBKGND,      OnButtonMDIBkgndclr)
	ON_BN_CLICKED(IDC_BUTTON_DIFFDELLINECLR,OnButtonDiffDelLineclr)
	ON_BN_CLICKED(IDC_BUTTON_DIFFADDLINECLR,OnButtonDiffAddLineclr)
	ON_BN_CLICKED(IDC_BUTTON_DIRCMPDIFFFILECLR, OnButtonDirDifflineclr)
	ON_CBN_SELCHANGE(IDC_COMBO_FILEEXT, OnSelchangeComboFileext)
	ON_BN_CLICKED(IDC_CHECK_QUOTE, OnCheckQuote)
	ON_BN_CLICKED(IDC_BUTTON_COMMENTCLR4, OnButtonCommentclr4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage3 ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O����������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage3::OnInitDialog() 
{
	CString	objStrBuf, objKey;
	CFile	objFile;
	char	szFullPath[_MAX_PATH];
	char	szKeyNo1Fname[_MAX_PATH],szKeyNo2Fname[_MAX_PATH];
	char	szKeyNo3Fname[_MAX_PATH],szKeyNo4Fname[_MAX_PATH];

	//���N���X�n���h���R�[��
	CPropertyPage::OnInitDialog();

	//�t�@�C���g���q�R���{�{�b�N�X�ݒ�
	m_objFileExtCombo.AddString("none");
	m_objFileExtCombo.AddString("txt");
	m_objFileExtCombo.AddString("htm html shtml shtm");
	m_objFileExtCombo.AddString("c cpp");
	m_objFileExtCombo.AddString("h");
	m_objFileExtCombo.AddString("asm");
	m_objFileExtCombo.AddString("asp");
	m_objFileExtCombo.AddString("frm");
	m_objFileExtCombo.AddString("bas");
	m_objFileExtCombo.AddString("pc");
	m_objFileExtCombo.AddString("log");
	m_objFileExtCombo.AddString("dat");
	m_objFileExtCombo.AddString("bat");
	m_objFileExtCombo.AddString("ini");
	m_objFileExtCombo.AddString("mak mk");
	m_objFileExtCombo.AddString("java");
	m_objFileExtCombo.AddString("y l");
	m_objFileExtCombo.AddString("wrl");
	m_objFileExtCombo.AddString("other");

	m_objFileExt.MakeLower();
	if(m_objFileExt.IsEmpty())
	{
		objKey = "FileExt_none";
		m_objFileExtCombo.SetCurSel(0);
		strcpy(szKeyNo1Fname,"keyword_no1_none.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_none.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_none.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_none.txt");
	}
	else if(!m_objFileExt.CompareNoCase("txt"))
	{
		objKey = "FileExt_txt";
		m_objFileExtCombo.SetCurSel(1);
		strcpy(szKeyNo1Fname,"keyword_no1_txt.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_txt.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_txt.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_txt.txt");
	}
	else if(!m_objFileExt.CompareNoCase("htm")   || !m_objFileExt.CompareNoCase("html") ||
			!m_objFileExt.CompareNoCase("shtml") || !m_objFileExt.CompareNoCase("shtm")
	)
	{
		objKey = "FileExt_htm html shtml shtm";
		m_objFileExtCombo.SetCurSel(2);
		strcpy(szKeyNo1Fname,"keyword_no1_htm html shtml shtm.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_htm html shtml shtm.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_htm html shtml shtm.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_htm html shtml shtm.txt");
	}
	else if(!m_objFileExt.CompareNoCase("c") || !m_objFileExt.CompareNoCase("cpp"))
	{
		objKey = "FileExt_c cpp";
		m_objFileExtCombo.SetCurSel(3);
		strcpy(szKeyNo1Fname,"keyword_no1_c cpp.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_c cpp.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_c cpp.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_c cpp.txt");
	}
	else if(!m_objFileExt.CompareNoCase("h"))
	{
		objKey = "FileExt_h";
		m_objFileExtCombo.SetCurSel(4);
		strcpy(szKeyNo1Fname,"keyword_no1_h.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_h.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_h.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_h.txt");
	}
	else if(!m_objFileExt.CompareNoCase("asm"))
	{
		objKey = "FileExt_asm";
		m_objFileExtCombo.SetCurSel(5);
		strcpy(szKeyNo1Fname,"keyword_no1_asm.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_asm.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_asm.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_asm.txt");
	}
	else if(!m_objFileExt.CompareNoCase("asp"))
	{
		objKey = "FileExt_asp";
		m_objFileExtCombo.SetCurSel(6);
		strcpy(szKeyNo1Fname,"keyword_no1_asp.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_asp.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_asp.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_asp.txt");
	}
	else if(!m_objFileExt.CompareNoCase("frm"))
	{
		objKey = "FileExt_frm";
		m_objFileExtCombo.SetCurSel(7);
		strcpy(szKeyNo1Fname,"keyword_no1_frm.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_frm.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_frm.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_frm.txt");
	}
	else if(!m_objFileExt.CompareNoCase("bas"))
	{
		objKey = "FileExt_bas";
		m_objFileExtCombo.SetCurSel(8);
		strcpy(szKeyNo1Fname,"keyword_no1_bas.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_bas.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_bas.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_bas.txt");
	}
	else if(!m_objFileExt.CompareNoCase("pc"))
	{
		objKey = "FileExt_pc";
		m_objFileExtCombo.SetCurSel(9);
		strcpy(szKeyNo1Fname,"keyword_no1_pc.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_pc.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_pc.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_pc.txt");
	}
	else if(!m_objFileExt.CompareNoCase("log"))
	{
		objKey = "FileExt_log";
		m_objFileExtCombo.SetCurSel(10);
		strcpy(szKeyNo1Fname,"keyword_no1_log.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_log.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_log.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_log.txt");
	}
	else if(!m_objFileExt.CompareNoCase("dat"))
	{
		objKey = "FileExt_dat";
		m_objFileExtCombo.SetCurSel(11);
		strcpy(szKeyNo1Fname,"keyword_no1_dat.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_dat.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_dat.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_dat.txt");
	}
	else if(!m_objFileExt.CompareNoCase("bat"))
	{
		objKey = "FileExt_bat";
		m_objFileExtCombo.SetCurSel(12);
		strcpy(szKeyNo1Fname,"keyword_no1_bat.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_bat.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_bat.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_bat.txt");
	}
	else if(!m_objFileExt.CompareNoCase("ini"))
	{
		objKey = "FileExt_ini";
		m_objFileExtCombo.SetCurSel(13);
		strcpy(szKeyNo1Fname,"keyword_no1_ini.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_ini.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_ini.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_ini.txt");
	}
	else if(!m_objFileExt.CompareNoCase("mak") || !m_objFileExt.CompareNoCase("mk"))
	{
		objKey = "FileExt_mak mk";
		m_objFileExtCombo.SetCurSel(14);
		strcpy(szKeyNo1Fname,"keyword_no1_mak mk.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_mak mk.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_mak mk.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_mak mk.txt");
	}
	else if(!m_objFileExt.CompareNoCase("java"))
	{
		objKey = "FileExt_java";
		m_objFileExtCombo.SetCurSel(15);
		strcpy(szKeyNo1Fname,"keyword_no1_java.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_java.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_java.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_java.txt");
	}
	else if(!m_objFileExt.CompareNoCase("y") || !m_objFileExt.CompareNoCase("l"))
	{
		objKey = "FileExt_y l";
		m_objFileExtCombo.SetCurSel(16);
		strcpy(szKeyNo1Fname,"keyword_no1_y l.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_y l.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_y l.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_y l.txt");
	}
	else if(!m_objFileExt.CompareNoCase("wrl"))
	{
		objKey = "FileExt_wrl";
		m_objFileExtCombo.SetCurSel(17);
		strcpy(szKeyNo1Fname,"keyword_no1_wrl.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_wrl.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_wrl.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_wrl.txt");
	}
	else
	{
		objKey = "FileExt_other";
		m_objFileExtCombo.SetCurSel(18);
		strcpy(szKeyNo1Fname,"keyword_no1_other.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_other.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_other.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_other.txt");
	}

	//�t�@�C���g���q�ɑΉ�����ݒ�擾
	m_FontColor       = m_pApp->GetProfileInt(objKey,"FontColor",m_FontColor);
	m_BackColor       = m_pApp->GetProfileInt(objKey,"BackColor",m_BackColor);
	m_InvertFontColor = m_pApp->GetProfileInt(objKey,"InvertFontColor",m_InvertFontColor);
	m_InvertBackColor = m_pApp->GetProfileInt(objKey,"InvertBackColor",m_InvertBackColor);
	m_KeyNo1Color     = m_pApp->GetProfileInt(objKey,"KeyNo1Color",m_KeyNo1Color);
	m_KeyNo2Color     = m_pApp->GetProfileInt(objKey,"KeyNo2Color",m_KeyNo2Color);
	m_KeyNo3Color     = m_pApp->GetProfileInt(objKey,"KeyNo3Color",m_KeyNo3Color);
	m_KeyNo4Color     = m_pApp->GetProfileInt(objKey,"KeyNo4Color",m_KeyNo4Color);
	m_MarkLineColor   = m_pApp->GetProfileInt(objKey,"MarkLineColor",m_MarkLineColor);
	m_bComment1Clr    = m_pApp->GetProfileInt(objKey,"bComment1Clr",m_bComment1Clr);
	m_CommentColor    = m_pApp->GetProfileInt(objKey,"CommentColor",m_CommentColor);
	m_bComment2Clr    = m_pApp->GetProfileInt(objKey,"bComment2Clr",m_bComment2Clr);
	m_CommentColor2   = m_pApp->GetProfileInt(objKey,"CommentColor2",m_CommentColor2);
	m_bComment3Clr    = m_pApp->GetProfileInt(objKey,"bComment3Clr",m_bComment3Clr);
	m_CommentColor3   = m_pApp->GetProfileInt(objKey,"CommentColor3",m_CommentColor3);
	m_bComment4Clr    = m_pApp->GetProfileInt(objKey,"bComment4Clr",m_bComment4Clr);
	m_CommentColor4   = m_pApp->GetProfileInt(objKey,"CommentColor4",m_CommentColor4);
	m_bDispLineNo     = m_pApp->GetProfileInt(objKey,"bDispLineNo",m_bDispLineNo);
	m_LineNoColor     = m_pApp->GetProfileInt(objKey,"LineNoColor",m_LineNoColor);
	m_LineNoBackColor = m_pApp->GetProfileInt(objKey,"LineNoBackColor",m_LineNoBackColor);
	m_bDispRuler	  = m_pApp->GetProfileInt(objKey,"bDispRuler",m_bDispRuler);
	m_RulerColor	  = m_pApp->GetProfileInt(objKey,"RulerColor",m_RulerColor);
	m_RulerBackColor  = m_pApp->GetProfileInt(objKey,"RulerBackColor",m_RulerBackColor);
	m_bDispCRLF		  = m_pApp->GetProfileInt(objKey,"bDispCRLF",m_bDispCRLF);
	m_CRLFColor		  = m_pApp->GetProfileInt(objKey,"CRLFColor",m_CRLFColor);
	m_bDispTab		  = m_pApp->GetProfileInt(objKey,"bDispTab",m_bDispTab);
	m_TabColor		  = m_pApp->GetProfileInt(objKey,"TabColor",m_TabColor);
	m_bDispZenkakuSpace = m_pApp->GetProfileInt(objKey,"bDispZenkakuSpace",m_bDispZenkakuSpace);
	m_ZenkakuSpaceColor = m_pApp->GetProfileInt(objKey,"ZenkakuSpaceColor",m_ZenkakuSpaceColor);
	m_bDispUnderLine  = m_pApp->GetProfileInt(objKey,"bDispUnderLine",m_bDispUnderLine);
	m_UnderLineColor  = m_pApp->GetProfileInt(objKey,"UnderLineColor",m_UnderLineColor);
	m_bDispUrl		  = m_pApp->GetProfileInt(objKey,"bDispUrl",m_bDispUrl);
	m_UrlColor		  = m_pApp->GetProfileInt(objKey,"UrlColor",m_UrlColor);
	m_bDispEMail	  = m_pApp->GetProfileInt(objKey,"bDispEMail",m_bDispEMail);
	m_EMailColor	  = m_pApp->GetProfileInt(objKey,"EMailColor",m_EMailColor);

	//�t�@�C���g���q�ɑΉ�����ݒ�ۑ�
	m_pApp->WriteProfileInt(objKey,"FontColor",m_FontColor);
	m_pApp->WriteProfileInt(objKey,"BackColor",m_BackColor);
	m_pApp->WriteProfileInt(objKey,"InvertFontColor",m_InvertFontColor);
	m_pApp->WriteProfileInt(objKey,"InvertBackColor",m_InvertBackColor);
	m_pApp->WriteProfileInt(objKey,"KeyNo1Color",m_KeyNo1Color);
	m_pApp->WriteProfileInt(objKey,"KeyNo2Color",m_KeyNo2Color);
	m_pApp->WriteProfileInt(objKey,"KeyNo3Color",m_KeyNo3Color);
	m_pApp->WriteProfileInt(objKey,"KeyNo4Color",m_KeyNo4Color);
	m_pApp->WriteProfileInt(objKey,"MarkLineColor",m_MarkLineColor);
	m_pApp->WriteProfileInt(objKey,"bComment1Clr",m_bComment1Clr);
	m_pApp->WriteProfileInt(objKey,"CommentColor",m_CommentColor);
	m_pApp->WriteProfileInt(objKey,"bComment2Clr",m_bComment2Clr);
	m_pApp->WriteProfileInt(objKey,"CommentColor2",m_CommentColor2);
	m_pApp->WriteProfileInt(objKey,"bComment3Clr",m_bComment3Clr);
	m_pApp->WriteProfileInt(objKey,"CommentColor3",m_CommentColor3);
	m_pApp->WriteProfileInt(objKey,"bComment4Clr",m_bComment4Clr);
	m_pApp->WriteProfileInt(objKey,"CommentColor4",m_CommentColor4);
	m_pApp->WriteProfileInt(objKey,"bDispLineNo",m_bDispLineNo);
	m_pApp->WriteProfileInt(objKey,"LineNoColor",m_LineNoColor);
	m_pApp->WriteProfileInt(objKey,"LineNoBackColor",m_LineNoBackColor);
	m_pApp->WriteProfileInt(objKey,"bDispRuler",m_bDispRuler);
	m_pApp->WriteProfileInt(objKey,"RulerColor",m_RulerColor);
	m_pApp->WriteProfileInt(objKey,"RulerBackColor",m_RulerBackColor);
	m_pApp->WriteProfileInt(objKey,"bDispCRLF",m_bDispCRLF);
	m_pApp->WriteProfileInt(objKey,"CRLFColor",m_CRLFColor);
	m_pApp->WriteProfileInt(objKey,"bDispTab",m_bDispTab);
	m_pApp->WriteProfileInt(objKey,"TabColor",m_TabColor);
	m_pApp->WriteProfileInt(objKey,"bDispZenkakuSpace",m_bDispZenkakuSpace);
	m_pApp->WriteProfileInt(objKey,"ZenkakuSpaceColor",m_ZenkakuSpaceColor);
	m_pApp->WriteProfileInt(objKey,"bDispUnderLine",m_bDispUnderLine);
	m_pApp->WriteProfileInt(objKey,"UnderLineColor",m_UnderLineColor);
	m_pApp->WriteProfileInt(objKey,"bDispUrl",m_bDispUrl);
	m_pApp->WriteProfileInt(objKey,"UrlColor",m_UrlColor);
	m_pApp->WriteProfileInt(objKey,"bDispEMail",m_bDispEMail);
	m_pApp->WriteProfileInt(objKey,"EMailColor",m_EMailColor);
	m_pApp->WriteProfileString(objKey,"Keyword_no1_filename",szKeyNo1Fname);
	m_pApp->WriteProfileString(objKey,"Keyword_no2_filename",szKeyNo2Fname);
	m_pApp->WriteProfileString(objKey,"Keyword_no3_filename",szKeyNo3Fname);
	m_pApp->WriteProfileString(objKey,"Keyword_no4_filename",szKeyNo4Fname);

	//�{�^���w�i�F�A�f�t�H���g�ݒ�
	m_objTxtClrBtn.SetBackColor(m_FontColor,FALSE);
	m_objBkClrBtn.SetBackColor(m_BackColor,FALSE);
	m_objInvTxtClrBtn.SetBackColor(m_InvertFontColor,FALSE);
	m_objInvBkClrBtn.SetBackColor(m_InvertBackColor,FALSE);
	m_objKeyNo1ClrBtn.SetBackColor(m_KeyNo1Color,FALSE);
	m_objKeyNo2ClrBtn.SetBackColor(m_KeyNo2Color,FALSE);
	m_objKeyNo3ClrBtn.SetBackColor(m_KeyNo3Color,FALSE);
	m_objKeyNo4ClrBtn.SetBackColor(m_KeyNo4Color,FALSE);
	m_objMarkLineClrBtn.SetBackColor(m_MarkLineColor,FALSE);
	m_objCommentClrBtn.SetBackColor(m_CommentColor,FALSE);
	m_objCommentClrBtn2.SetBackColor(m_CommentColor2,FALSE);
	m_objCommentClrBtn3.SetBackColor(m_CommentColor3,FALSE);
	m_objCommentClrBtn4.SetBackColor(m_CommentColor4,FALSE);
	m_objLineNoClrBtn.SetBackColor(m_LineNoColor,FALSE);
	m_objLineNoBackClrBtn.SetBackColor(m_LineNoBackColor,FALSE);
	m_objRulerClrBtn.SetBackColor(m_RulerColor,FALSE);
	m_objRulerBackClrBtn.SetBackColor(m_RulerBackColor,FALSE);
	m_objCRLFClrBtn.SetBackColor(m_CRLFColor,FALSE);
	m_objTabClrBtn.SetBackColor(m_TabColor,FALSE);
	m_objZenkakuSpaceClrBtn.SetBackColor(m_ZenkakuSpaceColor,FALSE);
	m_objUnderLineClrBtn.SetBackColor(m_UnderLineColor,FALSE);
	m_objUrlClrBtn.SetBackColor(m_UrlColor,FALSE);
	m_objEMailClrBtn.SetBackColor(m_EMailColor,FALSE);
	m_objMDIBkgndClrBtn.SetBackColor(m_MDIBkgndColor,FALSE);
	m_objDiffDelLineClrBtn.SetBackColor(m_DiffDelLineColor,FALSE);
	m_objDiffAddLineClrBtn.SetBackColor(m_DiffAddLineColor,FALSE);
	m_objDirDiffLineClrBtn.SetBackColor(m_DirDiffLineColor,FALSE);

	//�L�[���[�h�m���P�F�I���{�^���L���^�����ݒ�
	strcpy( szFullPath, m_pApp->m_strExePath );
	if(m_pApp->IsDriveRoot() == FALSE)
		strcat( szFullPath, "\\" );
	strcat( szFullPath, szKeyNo1Fname );
	if( !objFile.Open( szFullPath, CFile::modeRead, NULL ) ) //��`�t�@�C���Ȃ�
	{
		m_objKeyNo1ClrBtn.EnableWindow(FALSE);
	}
	else
	{
		objFile.Close();
		m_objKeyNo1ClrBtn.EnableWindow(TRUE);
	}

	//�L�[���[�h�m���Q�F�I���{�^���L���^�����ݒ�
	strcpy( szFullPath, m_pApp->m_strExePath );
	if(m_pApp->IsDriveRoot() == FALSE)
		strcat( szFullPath, "\\" );
	strcat( szFullPath, szKeyNo2Fname );
	if( !objFile.Open( szFullPath, CFile::modeRead, NULL ) ) //��`�t�@�C���Ȃ�
	{
		m_objKeyNo2ClrBtn.EnableWindow(FALSE);
	}
	else
	{
		objFile.Close();
		m_objKeyNo2ClrBtn.EnableWindow(TRUE);
	}

	//�L�[���[�h�m���R�F�I���{�^���L���^�����ݒ�
	strcpy( szFullPath, m_pApp->m_strExePath );
	if(m_pApp->IsDriveRoot() == FALSE)
		strcat( szFullPath, "\\" );
	strcat( szFullPath, szKeyNo3Fname );
	if( !objFile.Open( szFullPath, CFile::modeRead, NULL ) ) //��`�t�@�C���Ȃ�
	{
		m_objKeyNo3ClrBtn.EnableWindow(FALSE);
	}
	else
	{
		objFile.Close();
		m_objKeyNo3ClrBtn.EnableWindow(TRUE);
	}

	//�L�[���[�h�m���S�F�I���{�^���L���^�����ݒ�
	strcpy( szFullPath, m_pApp->m_strExePath );
	if(m_pApp->IsDriveRoot() == FALSE)
		strcat( szFullPath, "\\" );
	strcat( szFullPath, szKeyNo4Fname );
	if( !objFile.Open( szFullPath, CFile::modeRead, NULL ) ) //��`�t�@�C���Ȃ�
	{
		m_objKeyNo4ClrBtn.EnableWindow(FALSE);
	}
	else
	{
		objFile.Close();
		m_objKeyNo4ClrBtn.EnableWindow(TRUE);
	}

	//�b�E�b�{�{�s�R�����g�F�I���{�^���L���^�����ݒ�
	if( m_bComment1Clr )
		m_objCommentClrBtn.EnableWindow(TRUE);
	else
		m_objCommentClrBtn.EnableWindow(FALSE);

	//�u�a�s�R�����g�F�I���{�^���L���^�����ݒ�
	if( m_bComment2Clr )
		m_objCommentClrBtn2.EnableWindow(TRUE);
	else
		m_objCommentClrBtn2.EnableWindow(FALSE);

	//�A�Z���u���s�R�����g�F�I���{�^���L���^�����ݒ�
	if( m_bComment3Clr )
		m_objCommentClrBtn3.EnableWindow(TRUE);
	else
		m_objCommentClrBtn3.EnableWindow(FALSE);

	//���p�����s�R�����g�F�I���{�^���L���^�����ݒ�
	if( m_bComment4Clr )
		m_objCommentClrBtn4.EnableWindow(TRUE);
	else
		m_objCommentClrBtn4.EnableWindow(FALSE);

	//�s�ԍ��̐F�I���{�^���L���^�����ݒ�
	if( m_bDispLineNo )
	{
		m_objLineNoClrBtn.EnableWindow(TRUE);
		m_objLineNoBackClrBtn.EnableWindow(TRUE);
	}
	else
	{
		m_objLineNoClrBtn.EnableWindow(FALSE);
		m_objLineNoBackClrBtn.EnableWindow(FALSE);
	}

	//���[���̐F�I���{�^���L���^�����ݒ�
	if( m_bDispRuler )
	{
		m_objRulerClrBtn.EnableWindow(TRUE);
		m_objRulerBackClrBtn.EnableWindow(TRUE);
	}
	else
	{
		m_objRulerClrBtn.EnableWindow(FALSE);
		m_objRulerBackClrBtn.EnableWindow(FALSE);
	}

	//���s�}�[�N�̐F�I���{�^���L���^�����ݒ�
	if( m_bDispCRLF )
		m_objCRLFClrBtn.EnableWindow(TRUE);
	else
		m_objCRLFClrBtn.EnableWindow(FALSE);

	//�^�u�}�[�N�̐F�I���{�^���L���^�����ݒ�
	if( m_bDispTab )
		m_objTabClrBtn.EnableWindow(TRUE);
	else
		m_objTabClrBtn.EnableWindow(FALSE);

	//�S�p�X�y�[�X�̐F�I���{�^���L���^�����ݒ�
	if( m_bDispZenkakuSpace )
		m_objZenkakuSpaceClrBtn.EnableWindow(TRUE);
	else
		m_objZenkakuSpaceClrBtn.EnableWindow(FALSE);

	//�A���_�[���C���̐F�I���{�^���L���^�����ݒ�
	if( m_bDispUnderLine )
		m_objUnderLineClrBtn.EnableWindow(TRUE);
	else
		m_objUnderLineClrBtn.EnableWindow(FALSE);

	//�t�q�k�̐F�I���{�^���L���^�����ݒ�
	if( m_bDispUrl )
		m_objUrlClrBtn.EnableWindow(TRUE);
	else
		m_objUrlClrBtn.EnableWindow(FALSE);

	//���|���������̐F�I���{�^���L���^�����ݒ�
	if( m_bDispEMail )
		m_objEMailClrBtn.EnableWindow(TRUE);
	else
		m_objEMailClrBtn.EnableWindow(FALSE);

	//�����o�ϐ��|���R���g���[��
	UpdateData(FALSE);

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C���g���q�R���{�{�b�N�X�I��ύX������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnSelchangeComboFileext() 
{
	CFile	objFile;
	char	szFullPath[_MAX_PATH];
	char	szKeyNo1Fname[_MAX_PATH],szKeyNo2Fname[_MAX_PATH];
	char	szKeyNo3Fname[_MAX_PATH],szKeyNo4Fname[_MAX_PATH];
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�g���q�ɑΉ�����F�f�[�^�ݒ�擾
	m_FontColor         = m_pApp->GetProfileInt(objStr,"FontColor",RGB(255,255,255));
	m_BackColor         = m_pApp->GetProfileInt(objStr,"BackColor",RGB(0,0,0));
	m_InvertFontColor   = m_pApp->GetProfileInt(objStr,"InvertFontColor",RGB(0,0,0));
	m_InvertBackColor   = m_pApp->GetProfileInt(objStr,"InvertBackColor",RGB(255,255,255));
	m_KeyNo1Color       = m_pApp->GetProfileInt(objStr,"KeyNo1Color",RGB(0,255,255));
	m_KeyNo2Color       = m_pApp->GetProfileInt(objStr,"KeyNo2Color",RGB(0,255,255));
	m_KeyNo3Color       = m_pApp->GetProfileInt(objStr,"KeyNo3Color",RGB(0,255,255));
	m_KeyNo4Color       = m_pApp->GetProfileInt(objStr,"KeyNo4Color",RGB(0,255,255));
	m_MarkLineColor     = m_pApp->GetProfileInt(objStr,"MarkLineColor",RGB(128,0,128));
	m_bComment1Clr      = m_pApp->GetProfileInt(objStr,"bComment1Clr",1);
	m_CommentColor      = m_pApp->GetProfileInt(objStr,"CommentColor",RGB(0,255,0));
	m_bComment2Clr      = m_pApp->GetProfileInt(objStr,"bComment2Clr",0);
	m_CommentColor2     = m_pApp->GetProfileInt(objStr,"CommentColor2",RGB(0,255,0));
	m_bComment3Clr      = m_pApp->GetProfileInt(objStr,"bComment3Clr",0);
	m_CommentColor3     = m_pApp->GetProfileInt(objStr,"CommentColor3",RGB(0,255,0));
	m_bComment4Clr      = m_pApp->GetProfileInt(objStr,"bComment4Clr",0);
	m_CommentColor4     = m_pApp->GetProfileInt(objStr,"CommentColor4",RGB(0,255,0));
	m_bDispLineNo       = m_pApp->GetProfileInt(objStr,"bDispLineNo",1);
	m_LineNoColor       = m_pApp->GetProfileInt(objStr,"LineNoColor",RGB(0,255,255));
	m_LineNoBackColor   = m_pApp->GetProfileInt(objStr,"LineNoBackColor",RGB(0,0,0));
	m_bDispRuler	    = m_pApp->GetProfileInt(objStr,"bDispRuler",1);
	m_RulerColor	    = m_pApp->GetProfileInt(objStr,"RulerColor",RGB(0,0,0));
	m_RulerBackColor    = m_pApp->GetProfileInt(objStr,"RulerBackColor",RGB(192,192,192));
	m_bDispCRLF		    = m_pApp->GetProfileInt(objStr,"bDispCRLF",1);
	m_CRLFColor		    = m_pApp->GetProfileInt(objStr,"CRLFColor",RGB(255,255,0));
	m_bDispTab		    = m_pApp->GetProfileInt(objStr,"bDispTab",0);
	m_TabColor		    = m_pApp->GetProfileInt(objStr,"TabColor",RGB(255,255,255));
	m_bDispZenkakuSpace = m_pApp->GetProfileInt(objStr,"bDispZenkakuSpace",0);
	m_ZenkakuSpaceColor = m_pApp->GetProfileInt(objStr,"ZenkakuSpaceColor",RGB(255,255,255));
	m_bDispUnderLine    = m_pApp->GetProfileInt(objStr,"bDispUnderLine",1);
	m_UnderLineColor    = m_pApp->GetProfileInt(objStr,"UnderLineColor",RGB(255,255,0));
	m_bDispUrl		    = m_pApp->GetProfileInt(objStr,"bDispUrl",1);
	m_UrlColor		    = m_pApp->GetProfileInt(objStr,"UrlColor",RGB(255,0,255));
	m_bDispEMail	    = m_pApp->GetProfileInt(objStr,"bDispEMail",1);
	m_EMailColor	    = m_pApp->GetProfileInt(objStr,"EMailColor",RGB(255,0,255));

	//�_�C�A���O��̃{�^���F�ύX
	m_objTxtClrBtn.SetBackColor(m_FontColor,TRUE);
	m_objBkClrBtn.SetBackColor(m_BackColor,TRUE);
	m_objInvTxtClrBtn.SetBackColor(m_InvertFontColor,TRUE);
	m_objInvBkClrBtn.SetBackColor(m_InvertBackColor,TRUE);
	m_objKeyNo1ClrBtn.SetBackColor(m_KeyNo1Color,TRUE);
	m_objKeyNo2ClrBtn.SetBackColor(m_KeyNo2Color,TRUE);
	m_objKeyNo3ClrBtn.SetBackColor(m_KeyNo3Color,TRUE);
	m_objKeyNo4ClrBtn.SetBackColor(m_KeyNo4Color,TRUE);
	m_objMarkLineClrBtn.SetBackColor(m_MarkLineColor,TRUE);
	m_objCommentClrBtn.SetBackColor(m_CommentColor,TRUE);
	m_objCommentClrBtn2.SetBackColor(m_CommentColor2,TRUE);
	m_objCommentClrBtn3.SetBackColor(m_CommentColor3,TRUE);
	m_objCommentClrBtn4.SetBackColor(m_CommentColor4,TRUE);
	m_objLineNoClrBtn.SetBackColor(m_LineNoColor,TRUE);
	m_objLineNoBackClrBtn.SetBackColor(m_LineNoBackColor,TRUE);
	m_objRulerClrBtn.SetBackColor(m_RulerColor,TRUE);
	m_objRulerBackClrBtn.SetBackColor(m_RulerBackColor,TRUE);
	m_objCRLFClrBtn.SetBackColor(m_CRLFColor,TRUE);
	m_objTabClrBtn.SetBackColor(m_TabColor,TRUE);
	m_objZenkakuSpaceClrBtn.SetBackColor(m_ZenkakuSpaceColor,TRUE);
	m_objUnderLineClrBtn.SetBackColor(m_UnderLineColor,TRUE);
	m_objUrlClrBtn.SetBackColor(m_UrlColor,TRUE);
	m_objEMailClrBtn.SetBackColor(m_EMailColor,TRUE);

	//�L�[���[�h��`�t�@�C�����擾
	sprintf(szKeyNo1Fname,"keyword_no1_%s.txt",(LPCTSTR)objStr2);
	sprintf(szKeyNo2Fname,"keyword_no2_%s.txt",(LPCTSTR)objStr2);
	sprintf(szKeyNo3Fname,"keyword_no3_%s.txt",(LPCTSTR)objStr2);
	sprintf(szKeyNo4Fname,"keyword_no4_%s.txt",(LPCTSTR)objStr2);
	

	//�L�[���[�h�m���P�F�I���{�^���L���^�����ݒ�
	strcpy( szFullPath, m_pApp->m_strExePath );
	if(m_pApp->IsDriveRoot() == FALSE)
		strcat( szFullPath, "\\" );
	strcat( szFullPath, szKeyNo1Fname );
	if( !objFile.Open( szFullPath, CFile::modeRead, NULL ) ) //��`�t�@�C���Ȃ�
	{
		m_objKeyNo1ClrBtn.EnableWindow(FALSE);
	}
	else
	{
		objFile.Close();
		m_objKeyNo1ClrBtn.EnableWindow(TRUE);
	}

	//�L�[���[�h�m���Q�F�I���{�^���L���^�����ݒ�
	strcpy( szFullPath, m_pApp->m_strExePath );
	if(m_pApp->IsDriveRoot() == FALSE)
		strcat( szFullPath, "\\" );
	strcat( szFullPath, szKeyNo2Fname );
	if( !objFile.Open( szFullPath, CFile::modeRead, NULL ) ) //��`�t�@�C���Ȃ�
	{
		m_objKeyNo2ClrBtn.EnableWindow(FALSE);
	}
	else
	{
		objFile.Close();
		m_objKeyNo2ClrBtn.EnableWindow(TRUE);
	}

	//�L�[���[�h�m���R�F�I���{�^���L���^�����ݒ�
	strcpy( szFullPath, m_pApp->m_strExePath );
	if(m_pApp->IsDriveRoot() == FALSE)
		strcat( szFullPath, "\\" );
	strcat( szFullPath, szKeyNo3Fname );
	if( !objFile.Open( szFullPath, CFile::modeRead, NULL ) ) //��`�t�@�C���Ȃ�
	{
		m_objKeyNo3ClrBtn.EnableWindow(FALSE);
	}
	else
	{
		objFile.Close();
		m_objKeyNo3ClrBtn.EnableWindow(TRUE);
	}

	//�L�[���[�h�m���S�F�I���{�^���L���^�����ݒ�
	strcpy( szFullPath, m_pApp->m_strExePath );
	if(m_pApp->IsDriveRoot() == FALSE)
		strcat( szFullPath, "\\" );
	strcat( szFullPath, szKeyNo4Fname );
	if( !objFile.Open( szFullPath, CFile::modeRead, NULL ) ) //��`�t�@�C���Ȃ�
	{
		m_objKeyNo4ClrBtn.EnableWindow(FALSE);
	}
	else
	{
		objFile.Close();
		m_objKeyNo4ClrBtn.EnableWindow(TRUE);
	}

	//�b�E�b�{�{�s�R�����g�F�I���{�^���L���^�����ݒ�
	if( m_bComment1Clr )
		m_objCommentClrBtn.EnableWindow(TRUE);
	else
		m_objCommentClrBtn.EnableWindow(FALSE);

	//�u�a�s�R�����g�F�I���{�^���L���^�����ݒ�
	if( m_bComment2Clr )
		m_objCommentClrBtn2.EnableWindow(TRUE);
	else
		m_objCommentClrBtn2.EnableWindow(FALSE);

	//�A�Z���u���s�R�����g�F�I���{�^���L���^�����ݒ�
	if( m_bComment3Clr )
		m_objCommentClrBtn3.EnableWindow(TRUE);
	else
		m_objCommentClrBtn3.EnableWindow(FALSE);

	//���p�����s�R�����g�F�I���{�^���L���^�����ݒ�
	if( m_bComment4Clr )
		m_objCommentClrBtn4.EnableWindow(TRUE);
	else
		m_objCommentClrBtn4.EnableWindow(FALSE);

	//�s�ԍ��̐F�I���{�^���L���^�����ݒ�
	if( m_bDispLineNo )
	{
		m_objLineNoClrBtn.EnableWindow(TRUE);
		m_objLineNoBackClrBtn.EnableWindow(TRUE);
	}
	else
	{
		m_objLineNoClrBtn.EnableWindow(FALSE);
		m_objLineNoBackClrBtn.EnableWindow(FALSE);
	}

	//���[���̐F�I���{�^���L���^�����ݒ�
	if( m_bDispRuler )
	{
		m_objRulerClrBtn.EnableWindow(TRUE);
		m_objRulerBackClrBtn.EnableWindow(TRUE);
	}
	else
	{
		m_objRulerClrBtn.EnableWindow(FALSE);
		m_objRulerBackClrBtn.EnableWindow(FALSE);
	}

	//���s�}�[�N�̐F�I���{�^���L���^�����ݒ�
	if( m_bDispCRLF )
		m_objCRLFClrBtn.EnableWindow(TRUE);
	else
		m_objCRLFClrBtn.EnableWindow(FALSE);

	//�^�u�}�[�N�̐F�I���{�^���L���^�����ݒ�
	if( m_bDispTab )
		m_objTabClrBtn.EnableWindow(TRUE);
	else
		m_objTabClrBtn.EnableWindow(FALSE);

	//�S�p�X�y�[�X�̐F�I���{�^���L���^�����ݒ�
	if( m_bDispZenkakuSpace )
		m_objZenkakuSpaceClrBtn.EnableWindow(TRUE);
	else
		m_objZenkakuSpaceClrBtn.EnableWindow(FALSE);

	//�A���_�[���C���̐F�I���{�^���L���^�����ݒ�
	if( m_bDispUnderLine )
		m_objUnderLineClrBtn.EnableWindow(TRUE);
	else
		m_objUnderLineClrBtn.EnableWindow(FALSE);

	//�t�q�k�̐F�I���{�^���L���^�����ݒ�
	if( m_bDispUrl )
		m_objUrlClrBtn.EnableWindow(TRUE);
	else
		m_objUrlClrBtn.EnableWindow(FALSE);

	//���|���������̐F�I���{�^���L���^�����ݒ�
	if( m_bDispEMail )
		m_objEMailClrBtn.EnableWindow(TRUE);
	else
		m_objEMailClrBtn.EnableWindow(FALSE);

	//�����o�ϐ��|���R���g���[��
	UpdateData(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����F�ݒ�{�^������������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonTxtclr() 
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_FontColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_FontColor != objColorDialog.GetColor() )
		{
			m_FontColor = objColorDialog.GetColor();
			m_objTxtClrBtn.SetBackColor(m_FontColor,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"FontColor",m_FontColor);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���]�����F�ݒ�{�^������������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonInvtxtclr() 
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_InvertFontColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_InvertFontColor != objColorDialog.GetColor() )
		{
			m_InvertFontColor = objColorDialog.GetColor();
			m_objInvTxtClrBtn.SetBackColor(m_InvertFontColor,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"InvertFontColor",m_InvertFontColor);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w�i�F�ݒ�{�^������������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonBkclr() 
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_BackColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_BackColor != objColorDialog.GetColor() )
		{
			m_BackColor = objColorDialog.GetColor();
			m_objBkClrBtn.SetBackColor(m_BackColor,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"BackColor",m_BackColor);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���]�w�i�F�ݒ�{�^������������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonInvbkclr() 
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_InvertBackColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_InvertBackColor != objColorDialog.GetColor() )
		{
			m_InvertBackColor = objColorDialog.GetColor();
			m_objInvBkClrBtn.SetBackColor(m_InvertBackColor,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"InvertBackColor",m_InvertBackColor);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h�m���P�����F�ݒ�{�^������������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonKeyword1clr()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_KeyNo1Color);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_KeyNo1Color != objColorDialog.GetColor() )
		{
			m_KeyNo1Color = objColorDialog.GetColor();
			m_objKeyNo1ClrBtn.SetBackColor(m_KeyNo1Color,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"KeyNo1Color",m_KeyNo1Color);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h�m���Q�����F�ݒ�{�^������������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonKeyword2clr()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_KeyNo2Color);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_KeyNo2Color != objColorDialog.GetColor() )
		{
			m_KeyNo2Color = objColorDialog.GetColor();
			m_objKeyNo2ClrBtn.SetBackColor(m_KeyNo2Color,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"KeyNo2Color",m_KeyNo2Color);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h�m���R�����F�ݒ�{�^������������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonKeyword3clr()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_KeyNo3Color);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_KeyNo3Color != objColorDialog.GetColor() )
		{
			m_KeyNo3Color = objColorDialog.GetColor();
			m_objKeyNo3ClrBtn.SetBackColor(m_KeyNo3Color,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"KeyNo3Color",m_KeyNo3Color);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h�m���S�����F�ݒ�{�^������������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonKeyword4clr()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_KeyNo4Color);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_KeyNo4Color != objColorDialog.GetColor() )
		{
			m_KeyNo4Color = objColorDialog.GetColor();
			m_objKeyNo4ClrBtn.SetBackColor(m_KeyNo4Color,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"KeyNo4Color",m_KeyNo4Color);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�[�N�s�F�ݒ�{�^������������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonMarklineclr()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_MarkLineColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_MarkLineColor != objColorDialog.GetColor() )
		{
			m_MarkLineColor = objColorDialog.GetColor();
			m_objMarkLineClrBtn.SetBackColor(m_MarkLineColor,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"MarkLineColor",m_MarkLineColor);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�b�E�b�{�{�s�R�����g�L���`�F�b�N������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnChkcppcomment()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	m_bComment1Clr = (m_bComment1Clr!=FALSE) ? FALSE : TRUE;

	//�ݒ�ۑ�
	m_pApp->WriteProfileInt(objStr,"bComment1Clr",m_bComment1Clr);

	m_bIsChanged = TRUE;
	SetModified();

	//�b�E�b�{�{�s�R�����g�̐F�I���{�^���L���^�����ݒ�
	if( m_bComment1Clr )
		m_objCommentClrBtn.EnableWindow(TRUE);
	else
		m_objCommentClrBtn.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�i�b�^�b�{�{�j�����F�ݒ�{�^������������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonCommentclr()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_CommentColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_CommentColor != objColorDialog.GetColor() )
		{
			m_CommentColor = objColorDialog.GetColor();
			m_objCommentClrBtn.SetBackColor(m_CommentColor,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"CommentColor",m_CommentColor);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�a�s�R�����g�L���`�F�b�N������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnChkvbcomment()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	m_bComment2Clr = (m_bComment2Clr!=FALSE) ? FALSE : TRUE;

	//�ݒ�ۑ�
	m_pApp->WriteProfileInt(objStr,"bComment2Clr",m_bComment2Clr);

	m_bIsChanged = TRUE;
	SetModified();

	//�u�a�s�R�����g�̐F�I���{�^���L���^�����ݒ�
	if( m_bComment2Clr )
		m_objCommentClrBtn2.EnableWindow(TRUE);
	else
		m_objCommentClrBtn2.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�i�u�a�j�����F�ݒ�{�^������������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonCommentclr2()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_CommentColor2);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_CommentColor2 != objColorDialog.GetColor() )
		{
			m_CommentColor2 = objColorDialog.GetColor();
			m_objCommentClrBtn2.SetBackColor(m_CommentColor2,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"CommentColor2",m_CommentColor2);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A�Z���u���s�R�����g�L���`�F�b�N������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnChkasmcomment()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	m_bComment3Clr = (m_bComment3Clr!=FALSE) ? FALSE : TRUE;

	//�ݒ�ۑ�
	m_pApp->WriteProfileInt(objStr,"bComment3Clr",m_bComment3Clr);

	m_bIsChanged = TRUE;
	SetModified();

	//�A�Z���u���s�R�����g�̐F�I���{�^���L���^�����ݒ�
	if( m_bComment3Clr )
		m_objCommentClrBtn3.EnableWindow(TRUE);
	else
		m_objCommentClrBtn3.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�i�A�Z���u���j�����F�ݒ�{�^������������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonCommentclr3()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_CommentColor3);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_CommentColor3 != objColorDialog.GetColor() )
		{
			m_CommentColor3 = objColorDialog.GetColor();
			m_objCommentClrBtn3.SetBackColor(m_CommentColor3,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"CommentColor3",m_CommentColor3);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���p�����L���`�F�b�N������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnCheckQuote() 
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	m_bComment4Clr = (m_bComment4Clr!=FALSE) ? FALSE : TRUE;

	//�ݒ�ۑ�
	m_pApp->WriteProfileInt(objStr,"bComment4Clr",m_bComment4Clr);

	m_bIsChanged = TRUE;
	SetModified();

	//���p�����̐F�I���{�^���L���^�����ݒ�
	if( m_bComment4Clr )
		m_objCommentClrBtn4.EnableWindow(TRUE);
	else
		m_objCommentClrBtn4.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���p���������F�ݒ�{�^������������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonCommentclr4() 
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_CommentColor4);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_CommentColor4 != objColorDialog.GetColor() )
		{
			m_CommentColor4 = objColorDialog.GetColor();
			m_objCommentClrBtn4.SetBackColor(m_CommentColor4,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"CommentColor4",m_CommentColor4);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s�ԍ��\���`�F�b�N������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnChkDisplineno()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	m_bDispLineNo = (m_bDispLineNo!=FALSE) ? FALSE : TRUE;

	//�ݒ�ۑ�
	m_pApp->WriteProfileInt(objStr,"bDispLineNo",m_bDispLineNo);

	m_bIsChanged = TRUE;
	SetModified();

	//�s�ԍ��̐F�I���{�^���L���^�����ݒ�
	if( m_bDispLineNo )
	{
		m_objLineNoClrBtn.EnableWindow(TRUE);
		m_objLineNoBackClrBtn.EnableWindow(TRUE);
	}
	else
	{
		m_objLineNoClrBtn.EnableWindow(FALSE);
		m_objLineNoBackClrBtn.EnableWindow(FALSE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s�ԍ��̐F�ݒ�{�^������������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonlinenoclr()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_LineNoColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_LineNoColor != objColorDialog.GetColor() )
		{
			m_LineNoColor = objColorDialog.GetColor();
			m_objLineNoClrBtn.SetBackColor(m_LineNoColor,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"LineNoColor",m_LineNoColor);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s�ԍ��̔w�i�̐F�ݒ�{�^������������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonlinenobackclr()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_LineNoBackColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_LineNoBackColor != objColorDialog.GetColor() )
		{
			m_LineNoBackColor = objColorDialog.GetColor();
			m_objLineNoBackClrBtn.SetBackColor(m_LineNoBackColor,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"LineNoBackColor",m_LineNoBackColor);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[���\���`�F�b�N������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnChkDispruler()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	m_bDispRuler = (m_bDispRuler!=FALSE) ? FALSE : TRUE;

	//�ݒ�ۑ�
	m_pApp->WriteProfileInt(objStr,"bDispRuler",m_bDispRuler);

	m_bIsChanged = TRUE;
	SetModified();

	//���[���̐F�I���{�^���L���^�����ݒ�
	if( m_bDispRuler )
	{
		m_objRulerClrBtn.EnableWindow(TRUE);
		m_objRulerBackClrBtn.EnableWindow(TRUE);
	}
	else
	{
		m_objRulerClrBtn.EnableWindow(FALSE);
		m_objRulerBackClrBtn.EnableWindow(FALSE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[���F�ݒ�{�^������������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonrulerclr()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_RulerColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_RulerColor != objColorDialog.GetColor() )
		{
			m_RulerColor = objColorDialog.GetColor();
			m_objRulerClrBtn.SetBackColor(m_RulerColor,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"RulerColor",m_RulerColor);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[���̔w�i�̐F�ݒ�{�^������������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonrulerbackclr()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_RulerBackColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_RulerBackColor != objColorDialog.GetColor() )
		{
			m_RulerBackColor = objColorDialog.GetColor();
			m_objRulerBackClrBtn.SetBackColor(m_RulerBackColor,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"RulerBackColor",m_RulerBackColor);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s�R�[�h�\���`�F�b�N������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnChkDispcrlf()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	m_bDispCRLF = (m_bDispCRLF!=FALSE) ? FALSE : TRUE;

	//�ݒ�ۑ�
	m_pApp->WriteProfileInt(objStr,"bDispCRLF",m_bDispCRLF);

	m_bIsChanged = TRUE;
	SetModified();

	//���s�}�[�N�̐F�I���{�^���L���^�����ݒ�
	if( m_bDispCRLF )
		m_objCRLFClrBtn.EnableWindow(TRUE);
	else
		m_objCRLFClrBtn.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s�R�[�h�F�ݒ�{�^������������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonCrlfclr()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_CRLFColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_CRLFColor != objColorDialog.GetColor() )
		{
			m_CRLFColor = objColorDialog.GetColor();
			m_objCRLFClrBtn.SetBackColor(m_CRLFColor,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"CRLFColor",m_CRLFColor);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�^�u�R�[�h�\���`�F�b�N������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnChkDisptab()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	m_bDispTab = (m_bDispTab!=FALSE) ? FALSE : TRUE;

	//�ݒ�ۑ�
	m_pApp->WriteProfileInt(objStr,"bDispTab",m_bDispTab);

	m_bIsChanged = TRUE;
	SetModified();

	//�^�u�}�[�N�̐F�I���{�^���L���^�����ݒ�
	if( m_bDispTab )
		m_objTabClrBtn.EnableWindow(TRUE);
	else
		m_objTabClrBtn.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�^�u�R�[�h�F�ݒ�{�^������������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonTabclr()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_TabColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_TabColor != objColorDialog.GetColor() )
		{
			m_TabColor = objColorDialog.GetColor();
			m_objTabClrBtn.SetBackColor(m_TabColor,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"TabColor",m_TabColor);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�S�p�X�y�[�X�\���`�F�b�N������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnChkDispzenkakuspace()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	m_bDispZenkakuSpace = (m_bDispZenkakuSpace!=FALSE) ? FALSE : TRUE;

	//�ݒ�ۑ�
	m_pApp->WriteProfileInt(objStr,"bDispZenkakuSpace",m_bDispZenkakuSpace);

	m_bIsChanged = TRUE;
	SetModified();

	//�S�p�X�y�[�X�̐F�I���{�^���L���^�����ݒ�
	if( m_bDispZenkakuSpace )
		m_objZenkakuSpaceClrBtn.EnableWindow(TRUE);
	else
		m_objZenkakuSpaceClrBtn.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�S�p�X�y�[�X�F�ݒ�{�^������������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonZenkakuSpaceclr()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_ZenkakuSpaceColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_ZenkakuSpaceColor != objColorDialog.GetColor() )
		{
			m_ZenkakuSpaceColor = objColorDialog.GetColor();
			m_objZenkakuSpaceClrBtn.SetBackColor(m_ZenkakuSpaceColor,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"ZenkakuSpaceColor",m_ZenkakuSpaceColor);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A���_�[���C���\���`�F�b�N������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnChkDispunderline()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	m_bDispUnderLine = (m_bDispUnderLine!=FALSE) ? FALSE : TRUE;

	//�ݒ�ۑ�
	m_pApp->WriteProfileInt(objStr,"bDispUnderLine",m_bDispUnderLine);

	m_bIsChanged = TRUE;
	SetModified();

	//�A���_�[���C���̐F�I���{�^���L���^�����ݒ�
	if( m_bDispUnderLine )
		m_objUnderLineClrBtn.EnableWindow(TRUE);
	else
		m_objUnderLineClrBtn.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A���_�[���C���F�ݒ�{�^������������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonUnderLineclr()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_UnderLineColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_UnderLineColor != objColorDialog.GetColor() )
		{
			m_UnderLineColor = objColorDialog.GetColor();
			m_objUnderLineClrBtn.SetBackColor(m_UnderLineColor,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"UnderLineColor",m_UnderLineColor);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�q�k�\���`�F�b�N������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnChkDispurl()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	m_bDispUrl = (m_bDispUrl!=FALSE) ? FALSE : TRUE;

	//�ݒ�ۑ�
	m_pApp->WriteProfileInt(objStr,"bDispUrl",m_bDispUrl);

	m_bIsChanged = TRUE;
	SetModified();

	//�t�q�k�̐F�I���{�^���L���^�����ݒ�
	if( m_bDispUrl )
		m_objUrlClrBtn.EnableWindow(TRUE);
	else
		m_objUrlClrBtn.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�q�k�F�ݒ�{�^������������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonUrlclr()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_UrlColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_UrlColor != objColorDialog.GetColor() )
		{
			m_UrlColor = objColorDialog.GetColor();
			m_objUrlClrBtn.SetBackColor(m_UrlColor,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"UrlColor",m_UrlColor);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���|���������\���`�F�b�N������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnChkDispemail()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	m_bDispEMail = (m_bDispEMail!=FALSE) ? FALSE : TRUE;

	//�ݒ�ۑ�
	m_pApp->WriteProfileInt(objStr,"bDispEMail",m_bDispEMail);

	m_bIsChanged = TRUE;
	SetModified();

	//���|���������̐F�I���{�^���L���^�����ݒ�
	if( m_bDispEMail )
		m_objEMailClrBtn.EnableWindow(TRUE);
	else
		m_objEMailClrBtn.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���|���������F�ݒ�{�^������������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonEmailclr()
{
	CString objStr=_T("FileExt_"),objStr2;
	int nIndex = m_objFileExtCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	//�t�@�C���g���q�R���{�{�b�N�X������擾
	m_objFileExtCombo.GetLBText(nIndex,objStr2);
	objStr += objStr2;

	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_EMailColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_EMailColor != objColorDialog.GetColor() )
		{
			m_EMailColor = objColorDialog.GetColor();
			m_objEMailClrBtn.SetBackColor(m_EMailColor,TRUE);

			//�ݒ�ۑ�
			m_pApp->WriteProfileInt(objStr,"EMailColor",m_EMailColor);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�l�c�h�w�i�F�ݒ�{�^������������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonMDIBkgndclr()
{
	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_MDIBkgndColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_MDIBkgndColor != objColorDialog.GetColor() )
		{
			m_MDIBkgndColor = objColorDialog.GetColor();
			m_objMDIBkgndClrBtn.SetBackColor(m_MDIBkgndColor,TRUE);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r�p�A�폜�s�F�ݒ�{�^������������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonDiffDelLineclr()
{
	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_DiffDelLineColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_DiffDelLineColor != objColorDialog.GetColor() )
		{
			m_DiffDelLineColor = objColorDialog.GetColor();
			m_objDiffDelLineClrBtn.SetBackColor(m_DiffDelLineColor,TRUE);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r�p�A�ǉ��s�F�ݒ�{�^������������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonDiffAddLineclr()
{
	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_DiffAddLineColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_DiffAddLineColor != objColorDialog.GetColor() )
		{
			m_DiffAddLineColor = objColorDialog.GetColor();
			m_objDiffAddLineClrBtn.SetBackColor(m_DiffAddLineColor,TRUE);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r�p�A����t�@�C���F�ݒ�{�^������������				|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage3::OnButtonDirDifflineclr()
{
	//�F�I���R�����_�C�A���O�\��
	CColorDialog	objColorDialog(m_DirDiffLineColor);
	int nRet = objColorDialog.DoModal();
	if(nRet == IDOK)
	{
		if( m_DirDiffLineColor != objColorDialog.GetColor() )
		{
			m_DirDiffLineColor = objColorDialog.GetColor();
			m_objDirDiffLineClrBtn.SetBackColor(m_DirDiffLineColor,TRUE);

			m_bIsChanged = TRUE;
			SetModified();
		}
	}
}



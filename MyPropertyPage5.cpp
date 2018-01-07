// MyPropertyPage5.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "MyPropertyPage5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMyPropertyPage5, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage5 �v���p�e�B �y�[�W

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage5::CMyPropertyPage5() : CPropertyPage(CMyPropertyPage5::IDD)
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsChanged = FALSE;								//�ύX�����t���O

	//{{AFX_DATA_INIT(CMyPropertyPage5)
	m_bPrintLineNo = FALSE;
	m_bPrint2Dangumi = FALSE;
	m_nPrintOrientation = -1;
	m_objStrHeaderFormat = _T("");
	m_nHeaderLayout = -1;
	m_objStrFooterFormat = _T("");
	m_nFooterLayout = -1;
	m_nYohakuHidari = 0;
	m_nYohakuMigi = 0;
	m_nYohakuSita = 0;
	m_nYohakuUe = 0;
	//}}AFX_DATA_INIT
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage5::~CMyPropertyPage5()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage5 �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ύX������������														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage5::IsChanged() 
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
void CMyPropertyPage5::ClearChangeFlag()
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
void CMyPropertyPage5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage5)
	DDX_Check(pDX, IDC_CHECK_LINENO, m_bPrintLineNo);
	DDX_Check(pDX, IDC_CHECK_2DANGUMI, m_bPrint2Dangumi);
	DDX_Radio(pDX, IDC_RADIO_PRINTORIENTATION_TATE, m_nPrintOrientation);
	DDX_Text(pDX, IDC_EDIT_HEADERFORMAT, m_objStrHeaderFormat);
	DDX_Radio(pDX, IDC_RADIO_HEADER_LEFT, m_nHeaderLayout);
	DDX_Text(pDX, IDC_EDIT_FOOTERFORMAT, m_objStrFooterFormat);
	DDX_Radio(pDX, IDC_RADIO_FOOTER_LEFT, m_nFooterLayout);
	DDX_Text(pDX, IDC_EDIT_YOHAKU_HIDARI, m_nYohakuHidari);
	DDX_Text(pDX, IDC_EDIT_YOHAKU_MIGI, m_nYohakuMigi);
	DDX_Text(pDX, IDC_EDIT_YOHAKU_SITA, m_nYohakuSita);
	DDX_Text(pDX, IDC_EDIT_YOHAKU_UE, m_nYohakuUe);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyPropertyPage5, CPropertyPage)
	//{{AFX_MSG_MAP(CMyPropertyPage5)
	ON_BN_CLICKED(IDC_CHECK_LINENO, OnCheckLineno)
	ON_BN_CLICKED(IDC_BUTTON_LAYOUTHELP, OnButtonLayouthelp)
	ON_BN_CLICKED(IDC_RADIO_FOOTER_LEFT, OnRadioFooterLeft)
	ON_BN_CLICKED(IDC_RADIO_FOOTER_CENTER, OnRadioFooterCenter)
	ON_BN_CLICKED(IDC_RADIO_FOOTER_RIGHT, OnRadioFooterRight)
	ON_BN_CLICKED(IDC_RADIO_HEADER_LEFT, OnRadioHeaderLeft)
	ON_BN_CLICKED(IDC_RADIO_HEADER_CENTER, OnRadioHeaderCenter)
	ON_BN_CLICKED(IDC_RADIO_HEADER_RIGHT, OnRadioHeaderRight)
	ON_EN_CHANGE(IDC_EDIT_FOOTERFORMAT, OnChangeEditFooterformat)
	ON_EN_CHANGE(IDC_EDIT_HEADERFORMAT, OnChangeEditHeaderformat)
	ON_BN_CLICKED(IDC_CHECK_2DANGUMI, OnCheck2dangumi)
	ON_BN_CLICKED(IDC_RADIO_PRINTORIENTATION_TATE, OnRadioPrintorientationTate)
	ON_BN_CLICKED(IDC_RADIO_PRINTORIENTATION_YOKO, OnRadioPrintorientationYoko)
	ON_EN_CHANGE(IDC_EDIT_YOHAKU_HIDARI, OnChangeEditYohakuHidari)
	ON_EN_CHANGE(IDC_EDIT_YOHAKU_MIGI, OnChangeEditYohakuMigi)
	ON_EN_CHANGE(IDC_EDIT_YOHAKU_SITA, OnChangeEditYohakuSita)
	ON_EN_CHANGE(IDC_EDIT_YOHAKU_UE, OnChangeEditYohakuUe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage5 ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O����������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage5::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	//�t�H���g�ݒ�
	CWnd* pWnd=NULL;
	pWnd = GetDlgItem(IDC_EDIT_HEADERFORMAT);
	pWnd->SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	pWnd = GetDlgItem(IDC_EDIT_FOOTERFORMAT);
	pWnd->SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�s�ԍ�����v�I��������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage5::OnCheckLineno()
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�Q�i�g�݈���v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage5::OnCheck2dangumi()
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u��������@�c�v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage5::OnRadioPrintorientationTate() 
{
	if(m_nPrintOrientation != PRINTORIENTATION_TATE)
	{
		m_nPrintOrientation = PRINTORIENTATION_TATE;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u��������@���v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage5::OnRadioPrintorientationYoko() 
{
	if(m_nPrintOrientation != PRINTORIENTATION_YOKO)
	{
		m_nPrintOrientation = PRINTORIENTATION_YOKO;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u���C�A�E�g�̃w���v�\���v�{�^������������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage5::OnButtonLayouthelp() 
{
	//�w�b�_�ƃt�b�^�̃��C�A�E�g�w���v�_�C�A���O��\��
	CString objMsg;
	objMsg.LoadString(IDS_PRINTLAYOUTHELP);
	m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�w�b�_���C�A�E�g�@���悹�v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage5::OnRadioHeaderLeft() 
{
	if(m_nHeaderLayout != PRINT_HEADER_LEFT)
	{
		m_nHeaderLayout = PRINT_HEADER_LEFT;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�w�b�_���C�A�E�g�@�����v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage5::OnRadioHeaderCenter() 
{
	if(m_nHeaderLayout != PRINT_HEADER_CENTER)
	{
		m_nHeaderLayout = PRINT_HEADER_CENTER;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�w�b�_���C�A�E�g�@�E�悹�v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage5::OnRadioHeaderRight() 
{
	if(m_nHeaderLayout != PRINT_HEADER_RIGHT)
	{
		m_nHeaderLayout = PRINT_HEADER_RIGHT;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�b�^���C�A�E�g�@���悹�v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage5::OnRadioFooterLeft() 
{
	if(m_nFooterLayout != PRINT_FOOTER_LEFT)
	{
		m_nFooterLayout = PRINT_FOOTER_LEFT;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�b�^���C�A�E�g�@�����v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage5::OnRadioFooterCenter() 
{
	if(m_nFooterLayout != PRINT_FOOTER_CENTER)
	{
		m_nFooterLayout = PRINT_FOOTER_CENTER;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�b�^���C�A�E�g�@�E�悹�v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage5::OnRadioFooterRight() 
{
	if(m_nFooterLayout != PRINT_FOOTER_RIGHT)
	{
		m_nFooterLayout = PRINT_FOOTER_RIGHT;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�w�b�_�t�H�[�}�b�g �G�f�B�b�g�{�b�N�X�ύX������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage5::OnChangeEditHeaderformat() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�b�^�t�H�[�}�b�g �G�f�B�b�g�{�b�N�X�ύX������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage5::OnChangeEditFooterformat() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����]���� �G�f�B�b�g�{�b�N�X�ύX������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage5::OnChangeEditYohakuHidari() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����]���E �G�f�B�b�g�{�b�N�X�ύX������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage5::OnChangeEditYohakuMigi() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����]���� �G�f�B�b�g�{�b�N�X�ύX������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage5::OnChangeEditYohakuSita() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����]���� �G�f�B�b�g�{�b�N�X�ύX������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage5::OnChangeEditYohakuUe() 
{
	m_bIsChanged = TRUE;
	SetModified();
}


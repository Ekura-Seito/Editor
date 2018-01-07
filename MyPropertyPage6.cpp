// MyPropertyPage6.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "MyPropertyPage6.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMyPropertyPage6, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage6 �v���p�e�B �y�[�W

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage6::CMyPropertyPage6() : CPropertyPage(CMyPropertyPage6::IDD)
{
	m_bIsChanged = FALSE;								// �ύX�����t���O

	//{{AFX_DATA_INIT(CMyPropertyPage6)
	m_nCaretType = -1;
	m_unCaretBlinkTime = 0;
	m_nCaretBehave = -1;
	m_nCaretMoveType = -1;
	m_nMultValueOfCaretAccelarete = -1;
	m_nSetCaretToEndOnPaste = -1;
	//}}AFX_DATA_INIT
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage6::~CMyPropertyPage6()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage6 �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ύX������������														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage6::IsChanged() 
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
void CMyPropertyPage6::ClearChangeFlag()
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
void CMyPropertyPage6::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage6)
	DDX_Radio(pDX, IDC_RADIO_CARET_IBEAM, m_nCaretType);
	DDX_Text(pDX, IDC_EDIT_BLINKTIME, m_unCaretBlinkTime);
	DDV_MinMaxUInt(pDX, m_unCaretBlinkTime, 200, 800);
	DDX_Radio(pDX, IDC_RADIO_CARETMOVE, m_nCaretBehave);
	DDX_Radio(pDX, IDC_RADIO_WORDTYPE, m_nCaretMoveType);
	DDX_Radio(pDX, IDC_RADIO_ACCEL_1, m_nMultValueOfCaretAccelarete);
	DDX_Radio(pDX, IDC_RADIO_PASTETOP, m_nSetCaretToEndOnPaste);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyPropertyPage6, CPropertyPage)
	//{{AFX_MSG_MAP(CMyPropertyPage6)
	ON_BN_CLICKED(IDC_RADIO_CARET_IBEAM, OnRadioCaretIbeam)
	ON_BN_CLICKED(IDC_RADIO_CARET_SQUARE, OnRadioCaretSquare)
	ON_EN_CHANGE(IDC_EDIT_BLINKTIME, OnChangeEditBlinktime)
	ON_BN_CLICKED(IDC_RADIO_CARETMOVE, OnRadioCaretmove)
	ON_BN_CLICKED(IDC_RADIO_CARETSTAY, OnRadioCaretstay)
	ON_BN_CLICKED(IDC_RADIO_WORDTYPE, OnRadioWordtype)
	ON_BN_CLICKED(IDC_RADIO_NOTEPADTYPE, OnRadioNotepadtype)
	ON_BN_CLICKED(IDC_RADIO_ACCEL_1, OnRadioAccel1)
	ON_BN_CLICKED(IDC_RADIO_ACCEL_2, OnRadioAccel2)
	ON_BN_CLICKED(IDC_RADIO_ACCEL_3, OnRadioAccel3)
	ON_BN_CLICKED(IDC_RADIO_ACCEL_4, OnRadioAccel4)
	ON_BN_CLICKED(IDC_RADIO_ACCEL_5, OnRadioAccel5)
	ON_BN_CLICKED(IDC_RADIO_PASTETOP, OnRadioPastetop)
	ON_BN_CLICKED(IDC_RADIO_PASTEEND, OnRadioPasteend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage6 ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\������������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage6::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	//�t�H���g�ݒ�
	CWnd* pWnd = GetDlgItem(IDC_EDIT_BLINKTIME);
	pWnd->SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�`��F�c�_�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioCaretIbeam() 
{
	if(m_nCaretType != 0)
	{
		m_nCaretType = 0;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�`��F���^�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioCaretSquare() 
{
	if(m_nCaretType != 1)
	{
		m_nCaretType = 1;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�_�ŊԊu�G�f�B�b�g�{�b�N�X�ύX������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnChangeEditBlinktime() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�N���[�����̃L�����b�g����F�u�s�ʒu��ύX�v�I��������				|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioCaretmove() 
{
	if(m_nCaretBehave != 0)
	{
		m_nCaretBehave = 0;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�N���[�����̃L�����b�g����F�u�s�ʒu��ێ��v�I��������				|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioCaretstay() 
{
	if(m_nCaretBehave != 1)
	{
		m_nCaretBehave = 1;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ړ����̓���F�u���[�v�����v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioWordtype() 
{
	if(m_nCaretMoveType != 0)
	{
		m_nCaretMoveType = 0;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ړ����̓���F�u���������v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioNotepadtype() 
{
	if(m_nCaretMoveType != 1)
	{
		m_nCaretMoveType = 1;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ړ����̉����F�u�P�{���v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioAccel1() 
{
	if(m_nMultValueOfCaretAccelarete != 0)
	{
		m_nMultValueOfCaretAccelarete = 0;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ړ����̉����F�u�Q�{���v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioAccel2() 
{
	if(m_nMultValueOfCaretAccelarete != 1)
	{
		m_nMultValueOfCaretAccelarete = 1;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ړ����̉����F�u�R�{���v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioAccel3() 
{
	if(m_nMultValueOfCaretAccelarete != 2)
	{
		m_nMultValueOfCaretAccelarete = 2;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ړ����̉����F�u�S�{���v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioAccel4() 
{
	if(m_nMultValueOfCaretAccelarete != 3)
	{
		m_nMultValueOfCaretAccelarete = 3;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ړ����̉����F�u�T�{���v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioAccel5() 
{
	if(m_nMultValueOfCaretAccelarete != 4)
	{
		m_nMultValueOfCaretAccelarete = 4;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�\��t����̃L�����b�g�ʒu�F�u�擪�v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioPastetop() 
{
	if(m_nSetCaretToEndOnPaste != 0)
	{
		m_nSetCaretToEndOnPaste = 0;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�\��t����̃L�����b�g�ʒu�F�u�Ō�v�I��������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage6::OnRadioPasteend() 
{
	if(m_nSetCaretToEndOnPaste != 1)
	{
		m_nSetCaretToEndOnPaste = 1;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}


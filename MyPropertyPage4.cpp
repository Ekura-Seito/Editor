// MyPropertyPage4.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "Line.h"					//�P�s�N���X
#include "LineManager.h"
#include "CodeConvManager.h"
#include "MyPropertyPage4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMyPropertyPage4, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage4 �v���p�e�B �y�[�W

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage4::CMyPropertyPage4() : CPropertyPage(CMyPropertyPage4::IDD)
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsChanged = FALSE;								// �ύX�����t���O

	//{{AFX_DATA_INIT(CMyPropertyPage4)
	m_nKaigyoKind = -1;
	m_bIsAutoSave = FALSE;
	m_bDelEndSpacesOnSave = FALSE;
	m_bSpace2TabOnSave = FALSE;
	//}}AFX_DATA_INIT
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage4::~CMyPropertyPage4()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage4 �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ύX������������														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage4::IsChanged() 
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
void CMyPropertyPage4::ClearChangeFlag()
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
void CMyPropertyPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage4)
	DDX_Control(pDX, IDC_EDIT_SAVEINTERVAL, m_objSaveIntervalEdit);
	DDX_Radio(pDX, IDC_RADIO_CRLF, m_nKaigyoKind);
	DDX_Check(pDX, IDC_CHECK_AUTOSAVE, m_bIsAutoSave);
	DDX_Check(pDX, IDC_CHECK_DELENDSPACES, m_bDelEndSpacesOnSave);
	DDX_Check(pDX, IDC_CHECK_SPACE2TAB, m_bSpace2TabOnSave);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPropertyPage4, CPropertyPage)
	//{{AFX_MSG_MAP(CMyPropertyPage4)
	ON_BN_CLICKED(IDC_RADIO_CRLF, OnRadioCrlf)
	ON_BN_CLICKED(IDC_RADIO_LF, OnRadioLf)
	ON_BN_CLICKED(IDC_RADIO_CR, OnRadioCr)
	ON_BN_CLICKED(IDC_CHECK_AUTOSAVE, OnCheckAutosave)
	ON_EN_CHANGE(IDC_EDIT_SAVEINTERVAL, OnChangeEditSaveinterval)
	ON_BN_CLICKED(IDC_CHECK_DELENDSPACES, OnCheckDelendspacesonsave)
	ON_BN_CLICKED(IDC_CHECK_SPACE2TAB, OnCheckSpace2tab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage4 ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O����������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage4::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	//�t�H���g�ݒ�
	m_objSaveIntervalEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//�I�[�g�Z�[�u�Ԋu�G�f�B�b�g�L���^�����ݒ�
	if( m_bIsAutoSave )
		m_objSaveIntervalEdit.EnableWindow(TRUE);
	else
		m_objSaveIntervalEdit.EnableWindow(FALSE);

	//�I�[�g�Z�[�u�Ԋu�G�f�B�b�g������
	char	szTemp[32];
	sprintf(szTemp,"%d",m_lSaveInterval);
	m_objSaveIntervalEdit.SetWindowText(szTemp);
	UpdateData(FALSE);

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�b�q�^�k�e�I��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage4::OnRadioCrlf() 
{
	if(m_nKaigyoKind != KAIGYO_CRLF)
	{
		m_nKaigyoKind = KAIGYO_CRLF;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�k�e�I��������														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage4::OnRadioLf() 
{
	if(m_nKaigyoKind != KAIGYO_LF)
	{
		m_nKaigyoKind = KAIGYO_LF;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�b�q�I��������														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage4::OnRadioCr() 
{
	if(m_nKaigyoKind != KAIGYO_CR)
	{
		m_nKaigyoKind = KAIGYO_CR;

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�I�[�g�Z�[�u���s���v�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage4::OnCheckAutosave() 
{
	m_bIsAutoSave = (m_bIsAutoSave!=FALSE) ? FALSE : TRUE;

	m_bIsChanged = TRUE;
	SetModified();

	//�I�[�g�Z�[�u�Ԋu�G�f�B�b�g�L���^�����ݒ�
	if( m_bIsAutoSave )
		m_objSaveIntervalEdit.EnableWindow(TRUE);
	else
		m_objSaveIntervalEdit.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�[�g�Z�[�u�Ԋu�G�f�B�b�g�{�b�N�X�ύX������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage4::OnChangeEditSaveinterval() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ۑ����ɍs���̋󔒍폜�v�I��������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage4::OnCheckDelendspacesonsave() 
{
	m_bDelEndSpacesOnSave = (m_bDelEndSpacesOnSave!=FALSE) ? FALSE : TRUE;

	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�ۑ����ɋ󔒂��^�u�֕ϊ��v�I��������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage4::OnCheckSpace2tab() 
{
	m_bSpace2TabOnSave = (m_bSpace2TabOnSave!=FALSE) ? FALSE : TRUE;

	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�n�j�v�{�^������������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage4::OnOK() 
{
	CString objMsg;

	//�R���g���[���|�������o�ϐ��f�[�^��荞��
	UpdateData(TRUE);

	if(m_bIsAutoSave)
	{
		CString objStr;
		m_objSaveIntervalEdit.GetWindowText(objStr);
		m_lSaveInterval = atoi((char*)(LPCTSTR)objStr);
	
		//�I�[�g�Z�[�u�Ԋu�Ó����`�F�b�N
		if(m_lSaveInterval < 1 || m_lSaveInterval > 60)
		{
			objMsg.LoadString(IDS_AUTOSAVETIME_ERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONEXCLAMATION),0);
		}
	}

	CPropertyPage::OnOK();
}


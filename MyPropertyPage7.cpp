// MyPropertyPage7.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "MyPropertyPage7.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMyPropertyPage7, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage7 �v���p�e�B �y�[�W

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage7::CMyPropertyPage7() : CPropertyPage(CMyPropertyPage7::IDD)
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_unLogoPosX = 0;
	m_unLogoPosY = 0;
	m_bIsChanged = FALSE;								// �ύX�����t���O
	m_objBmpFilePath = _T("");

	//{{AFX_DATA_INIT(CMyPropertyPage7)
	m_bDispLogoBmp = FALSE;
	m_nLogoBmpKind = -1;
	m_nLogoPosKind = -1;
	//}}AFX_DATA_INIT
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage7::~CMyPropertyPage7()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage7 �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ύX������������														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage7::IsChanged() 
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
void CMyPropertyPage7::ClearChangeFlag()
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
void CMyPropertyPage7::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage7)
	DDX_Control(pDX, IDC_REF, m_objRefBtn);
	DDX_Check(pDX, IDC_CHECK_LOGODISP, m_bDispLogoBmp);
	DDX_Radio(pDX, IDC_RADIO_XERXES, m_nLogoBmpKind);
	DDX_Control(pDX, IDC_EDIT_BMPFILEPATH, m_objBmpFilePathEdit);
	DDX_Control(pDX, IDC_EDIT_LOGOPOSX, m_objLogoPosXEdit);
	DDX_Control(pDX, IDC_EDIT_LOGOPOSY, m_objLogoPosYEdit);
	DDX_Radio(pDX, IDC_RADIO_TILE, m_nLogoPosKind);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPropertyPage7, CPropertyPage)
	//{{AFX_MSG_MAP(CMyPropertyPage7)
	ON_BN_CLICKED(IDC_REF, OnRef)
	ON_BN_CLICKED(IDC_CHECK_LOGODISP, OnCheckLogodisp)
	ON_BN_CLICKED(IDC_RADIO_XERXES, OnRadioXerxesLogo)
	ON_BN_CLICKED(IDC_RADIO_FAVARITE, OnRadioFavariteLogo)
	ON_EN_CHANGE(IDC_EDIT_BMPFILEPATH, OnChangeEditBmpFilePath)
	ON_EN_CHANGE(IDC_EDIT_LOGOPOSX, OnChangeEditLogoposx)
	ON_EN_CHANGE(IDC_EDIT_LOGOPOSY, OnChangeEditLogoposy)
	ON_BN_CLICKED(IDC_RADIO_TILE, OnRadioTile)
	ON_BN_CLICKED(IDC_RADIO_POSSITEI, OnRadioPossitei)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage7 ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O����������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage7::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	//�t�H���g�ݒ�
	m_objBmpFilePathEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	m_objLogoPosXEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	m_objLogoPosYEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	if( m_bDispLogoBmp )
	{
		//���S�r�b�g�}�b�v�敪�@�L���^�����ݒ�
		GetDlgItem(IDC_RADIO_XERXES)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_FAVARITE)->EnableWindow(TRUE);
		if( m_nLogoBmpKind == LOGO_XERXES )
		{
			m_objRefBtn.EnableWindow(FALSE);
			m_objBmpFilePathEdit.EnableWindow(FALSE);
		}
		else if( m_nLogoBmpKind == LOGO_FAVARITE )
		{
			m_objRefBtn.EnableWindow(TRUE);
			m_objBmpFilePathEdit.EnableWindow(TRUE);
		}

		//���S�\���敪�@�L���^�����ݒ�
		GetDlgItem(IDC_RADIO_TILE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_POSSITEI)->EnableWindow(TRUE);
		if( m_nLogoPosKind == LOGO_POS_TILE )
		{
			m_objLogoPosXEdit.EnableWindow(FALSE);
			m_objLogoPosYEdit.EnableWindow(FALSE);
		}
		else if( m_nLogoPosKind == LOGO_POS_SITEI )
		{
			m_objLogoPosXEdit.EnableWindow(TRUE);
			m_objLogoPosYEdit.EnableWindow(TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_RADIO_XERXES)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FAVARITE)->EnableWindow(FALSE);
		m_objRefBtn.EnableWindow(FALSE);
		m_objBmpFilePathEdit.EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_POSSITEI)->EnableWindow(FALSE);
		m_objLogoPosXEdit.EnableWindow(FALSE);
		m_objLogoPosYEdit.EnableWindow(FALSE);
	}

	//�a�l�o�t�@�C���p�X�G�f�B�b�g������
	m_objBmpFilePathEdit.SetWindowText(m_objBmpFilePath);
	//���S�ʒu�G�f�B�b�g������
	char	szTemp[32];
	sprintf(szTemp,"%d",m_unLogoPosX);
	m_objLogoPosXEdit.SetWindowText(szTemp);
	sprintf(szTemp,"%d",m_unLogoPosY);
	m_objLogoPosYEdit.SetWindowText(szTemp);

	UpdateData(FALSE);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u���S��\���v�I��������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnCheckLogodisp() 
{
	m_bDispLogoBmp = (m_bDispLogoBmp!=FALSE) ? FALSE : TRUE;

	m_bIsChanged = TRUE;
	SetModified();

	//���S�ʒu�G�f�B�b�g�L���^�����ݒ�
	if( m_bDispLogoBmp )
	{
		//���S�r�b�g�}�b�v�敪�@�L���^�����ݒ�
		GetDlgItem(IDC_RADIO_XERXES)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_FAVARITE)->EnableWindow(TRUE);
		if( m_nLogoBmpKind == LOGO_XERXES )
		{
			m_objRefBtn.EnableWindow(FALSE);
			m_objBmpFilePathEdit.EnableWindow(FALSE);
		}
		else if( m_nLogoBmpKind == LOGO_FAVARITE )
		{
			m_objRefBtn.EnableWindow(TRUE);
			m_objBmpFilePathEdit.EnableWindow(TRUE);
		}

		//���S�\���敪�@�L���^�����ݒ�
		GetDlgItem(IDC_RADIO_TILE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_POSSITEI)->EnableWindow(TRUE);
		if( m_nLogoPosKind == LOGO_POS_TILE )
		{
			m_objLogoPosXEdit.EnableWindow(FALSE);
			m_objLogoPosYEdit.EnableWindow(FALSE);
		}
		else if( m_nLogoPosKind == LOGO_POS_SITEI )
		{
			m_objLogoPosXEdit.EnableWindow(TRUE);
			m_objLogoPosYEdit.EnableWindow(TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_RADIO_XERXES)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FAVARITE)->EnableWindow(FALSE);
		m_objRefBtn.EnableWindow(FALSE);
		m_objBmpFilePathEdit.EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_POSSITEI)->EnableWindow(FALSE);
		m_objLogoPosXEdit.EnableWindow(FALSE);
		m_objLogoPosYEdit.EnableWindow(FALSE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�t�H���g��Xerxes���S�I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnRadioXerxesLogo() 
{
	if(m_nLogoBmpKind != LOGO_XERXES)
	{
		m_nLogoBmpKind = LOGO_XERXES;
		m_objRefBtn.EnableWindow(FALSE);
		m_objBmpFilePathEdit.EnableWindow(FALSE);

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�D���Ȃa�l�o���S�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnRadioFavariteLogo() 
{
	if(m_nLogoBmpKind != LOGO_FAVARITE)
	{
		m_nLogoBmpKind = LOGO_FAVARITE;
		m_objRefBtn.EnableWindow(TRUE);
		m_objBmpFilePathEdit.EnableWindow(TRUE);

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�a�l�o�t�@�C���p�X�G�f�B�b�g�{�b�N�X�ύX������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnChangeEditBmpFilePath() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�Q�ƃ{�^������������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnRef() 
{
	CString	objFilter = _T("BMP file (*.bmp)|*.bmp|");
	DWORD dwFlags = ( OFN_HIDEREADONLY     | 
					  OFN_OVERWRITEPROMPT  |
					  OFN_LONGNAMES        |
					  OFN_FILEMUSTEXIST );
	CFileDialog	objFileDialog(	TRUE,				//OPEN
								NULL,				//�f�t�H���g�t�@�C���g���q
								NULL,				//�f�t�H���g�t�@�C����
								dwFlags,			//�I�[�v���t���O
								objFilter,			//�t�B���^
								NULL );				//�e�E�C���h�E�n���h��
	if( objFileDialog.DoModal() == IDOK )
	{
		m_objBmpFilePathEdit.SetWindowText(objFileDialog.GetPathName());
		m_objBmpFilePathEdit.UpdateData(TRUE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���S���^�C���\������I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnRadioTile() 
{
	if(m_nLogoPosKind != LOGO_POS_TILE)
	{
		m_nLogoPosKind = LOGO_POS_TILE;
		m_objLogoPosXEdit.EnableWindow(FALSE);
		m_objLogoPosYEdit.EnableWindow(FALSE);

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���S�\���ʒu���w�肷��I��������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnRadioPossitei() 
{
	if(m_nLogoPosKind != LOGO_POS_SITEI)
	{
		m_nLogoPosKind = LOGO_POS_SITEI;
		m_objLogoPosXEdit.EnableWindow(TRUE);
		m_objLogoPosYEdit.EnableWindow(TRUE);

		//�ύX���ꂽ
		m_bIsChanged = TRUE;
		SetModified();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���S�ʒu�w�G�f�B�b�g�{�b�N�X�ύX������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnChangeEditLogoposx() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���S�ʒu�x�G�f�B�b�g�{�b�N�X�ύX������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage7::OnChangeEditLogoposy() 
{
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
void CMyPropertyPage7::OnOK() 
{
	CString		objStr=_T("");
	char*		pPtr=NULL;
	CString		objMsg;

	//�R���g���[���|�������o�ϐ��f�[�^��荞��
	UpdateData(TRUE);

	//�a�l�o�t�@�C���p�X�擾
	m_objBmpFilePathEdit.GetWindowText(m_objBmpFilePath);

	//���S�ʒu�w�̑Ó����`�F�b�N
	m_objLogoPosXEdit.GetWindowText(objStr);
	m_unLogoPosX = (UINT)atoi((char*)(LPCTSTR)objStr);
	if(m_unLogoPosX < 0 || m_unLogoPosX > 100)
	{
		objMsg.LoadString(IDS_LOGOX_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONEXCLAMATION),0);
	}

	//���S�ʒu�x�̑Ó����`�F�b�N
	m_objLogoPosYEdit.GetWindowText(objStr);
	m_unLogoPosY = (UINT)atoi((char*)(LPCTSTR)objStr);
	if(m_unLogoPosY < 0 || m_unLogoPosY > 100)
	{
		objMsg.LoadString(IDS_LOGOY_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONEXCLAMATION),0);
	}

	CPropertyPage::OnOK();
}


// MyPropertyPage1.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "Editor.h"
#include "MyPropertyPage1.h"
#include "ExecPasswdDlg.h"
#include "Mymd5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMyPropertyPage1, CPropertyPage)

BEGIN_MESSAGE_MAP(CMyPropertyPage1, CPropertyPage)
	//{{AFX_MSG_MAP(CMyPropertyPage1)
	ON_BN_CLICKED(IDC_CHECK_CHDIRTOOPENFILE, OnCheckChdirtoopenfile)
	ON_BN_CLICKED(IDC_CHECK_MAXCLDWND, OnCheckMaxcldwnd)
	ON_BN_CLICKED(IDC_CHECK_OPENNEWFILE, OnCheckOpennewfile)
	ON_BN_CLICKED(IDC_CHECK_SAVEWINDOWPLACE, OnCheckSavewindowplace)
	ON_EN_CHANGE(IDC_EDIT_MAXMRU, OnChangeEditMaxmru)
	ON_BN_CLICKED(IDC_CHECK_FULLTITLE, OnCheckFulltitle)
	ON_BN_CLICKED(IDC_CHECK_DISPEOFMARK, OnCheckDispeofmark)
	ON_BN_CLICKED(IDC_CHECK_NODOUBLEEXECUTE, OnCheckNodoubleexecute)
	ON_BN_CLICKED(IDC_CHECK_SHOWSPLASH, OnCheckShowsplash)
	ON_EN_CHANGE(IDC_EDIT_SPLASHTIME, OnChangeEditSplashtime)
	ON_BN_CLICKED(IDC_CHECK_EXECPASSWD, OnCheckExecpasswd)
	ON_BN_CLICKED(IDC_SETPASSWDBTN, OnSetpasswdbtn)
	ON_BN_CLICKED(IDC_CHECK_ENABLESHELLOPEN, OnCheckEnableshellopen)
	ON_BN_CLICKED(IDC_CHECK_MAKESHORTCUTDSKTOP, OnCheckMakeshortcutdsktop)
	ON_BN_CLICKED(IDC_CHECK_IMMAUTOOPEN, OnCheckImmautoopen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage1 �v���p�e�B �y�[�W

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage1::CMyPropertyPage1() : CPropertyPage(CMyPropertyPage1::IDD)
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsChanged = FALSE;								//�ύX�����t���O
	m_lSplashTime = 0;
	m_objExecPasswdStr = _T("");

	//{{AFX_DATA_INIT(CMyPropertyPage1)
	m_bShowSplash = FALSE;
	m_bIsOpenNewFile = FALSE;
	m_bIsChdirToOpenFile = FALSE;
	m_bMaximizeWindow = FALSE;
	m_bIsSaveWindowPlace = FALSE;
	m_nMaxMRU = 0;
	m_bIsFullTitle = FALSE;
	m_bIsDispEOFMark = FALSE;
	m_bIsNoDoubleExecute = FALSE;
	m_bIsExecPasswd = FALSE;
	m_bIsEnableShellOpen = FALSE;
	m_bIsMakeShotcutDsktop = FALSE;
	m_bIsImmAutoOpen = FALSE;
	//}}AFX_DATA_INIT
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage1::~CMyPropertyPage1()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�c�c�w�f�[�^����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage1)
	DDX_Check(pDX, IDC_CHECK_SHOWSPLASH, m_bShowSplash);
	DDX_Control(pDX, IDC_EDIT_SPLASHTIME, m_objSplashTimeEdit);
	DDX_Check(pDX, IDC_CHECK_OPENNEWFILE, m_bIsOpenNewFile);
	DDX_Check(pDX, IDC_CHECK_CHDIRTOOPENFILE, m_bIsChdirToOpenFile);
	DDX_Check(pDX, IDC_CHECK_MAXCLDWND, m_bMaximizeWindow);
	DDX_Check(pDX, IDC_CHECK_SAVEWINDOWPLACE, m_bIsSaveWindowPlace);
	DDX_Text(pDX, IDC_EDIT_MAXMRU, m_nMaxMRU);
	DDV_MinMaxInt(pDX, m_nMaxMRU, 4, 16);
	DDX_Check(pDX, IDC_CHECK_FULLTITLE, m_bIsFullTitle);
	DDX_Check(pDX, IDC_CHECK_DISPEOFMARK, m_bIsDispEOFMark);
	DDX_Check(pDX, IDC_CHECK_NODOUBLEEXECUTE, m_bIsNoDoubleExecute);
	DDX_Check(pDX, IDC_CHECK_EXECPASSWD, m_bIsExecPasswd);
	DDX_Control(pDX, IDC_SETPASSWDBTN, m_objSetPasswdBtn);
	DDX_Check(pDX, IDC_CHECK_ENABLESHELLOPEN, m_bIsEnableShellOpen);
	DDX_Check(pDX, IDC_CHECK_MAKESHORTCUTDSKTOP, m_bIsMakeShotcutDsktop);
	DDX_Check(pDX, IDC_CHECK_IMMAUTOOPEN, m_bIsImmAutoOpen);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage1 �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ύX������������														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage1::IsChanged() 
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
void CMyPropertyPage1::ClearChangeFlag()
{
	m_bIsChanged = FALSE;
	SetModified(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage1 ���b�Z�[�W�n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\������������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage1::OnInitDialog() 
{
	//�K��N���X�̃n���h���R�[��
	CPropertyPage::OnInitDialog();

	//�t�H���g�ݒ�
	m_objSplashTimeEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	CWnd* pWnd = GetDlgItem(IDC_EDIT_MAXMRU);
	pWnd->SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//�X�v���b�V���X�N���[���\�����ԃG�f�B�b�g�L���^�����ݒ�
	if( m_bShowSplash )
		m_objSplashTimeEdit.EnableWindow(TRUE);
	else
		m_objSplashTimeEdit.EnableWindow(FALSE);

	//�X�v���b�V���X�N���[���\�����ԃG�f�B�b�g������
	char	szTemp[32];
	sprintf(szTemp,"%d",m_lSplashTime);
	m_objSplashTimeEdit.SetWindowText(szTemp);
	m_objSplashTimeEdit.UpdateData(TRUE);

	//�N���p�X���[�h�ݒ�{�^���L���^�����ݒ�
	if( m_bIsExecPasswd )
		m_objSetPasswdBtn.EnableWindow(TRUE);
	else
		m_objSetPasswdBtn.EnableWindow(FALSE);

	//�����o�ϐ��|���R���g���[��
	UpdateData(FALSE);

	//�ύX�����t���O�N���A
	m_bIsChanged = FALSE;

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�X�v���b�V���X�N���[���\���v�`�F�b�N������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckShowsplash() 
{
	m_bShowSplash = (m_bShowSplash!=FALSE) ? FALSE : TRUE;

	m_bIsChanged = TRUE;
	SetModified();

	//�X�v���b�V���X�N���[���\�����ԃG�f�B�b�g�L���^�����ݒ�
	if( m_bShowSplash )
		m_objSplashTimeEdit.EnableWindow(TRUE);
	else
		m_objSplashTimeEdit.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�v���b�V���X�N���[���\�����ԃG�f�B�b�g�ύX������					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnChangeEditSplashtime() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�����N���h�~�v�`�F�b�N������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckNodoubleexecute() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�I�[�v���t�@�C���f�B���N�g���ړ��v�`�F�b�N������					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckChdirtoopenfile() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�q�E�C���h�E�쐬���ő剻�v�`�F�b�N������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckMaxcldwnd() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�N�����V�K�q�E�C���h�E�I�[�v���v�`�F�b�N������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckOpennewfile() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u���C���t���[���E�C���h�E�ʒu�T�C�Y�ۑ��v�`�F�b�N������				|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckSavewindowplace() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�t�@�C���^�C�g���t���p�X�\���v�`�F�b�N������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckFulltitle() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�d�n�e�\���v�`�F�b�N������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckDispeofmark() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�N�����Ƀp�X���[�h�`�F�b�N����v�`�F�b�N������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckExecpasswd() 
{
	m_bIsExecPasswd = (m_bIsExecPasswd!=FALSE) ? FALSE : TRUE;

	m_bIsChanged = TRUE;
	SetModified();

	//�N���p�X���[�h�ݒ�{�^���L���^�����ݒ�
	if( m_bIsExecPasswd )
		m_objSetPasswdBtn.EnableWindow(TRUE);
	else
		m_objSetPasswdBtn.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�N�����p�X���[�h�ݒ�{�^������������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnSetpasswdbtn() 
{
	CString		objNewPasswd, objNewPasswdMD5, objOldPasswdMD5;
	char		szNewPasswd[1024], szNewPasswdMD5[1024];

	//�p�X���[�h�ݒ�_�C�A���O�\��
	CExecPasswdDlg	objExecPasswdDlg;
	if(objExecPasswdDlg.DoModal() != IDOK)
		return;

	//�V�p�X���[�h�擾
	objNewPasswd.Empty();
	objNewPasswd = objExecPasswdDlg.m_objExecPasswdStr;
	memset(szNewPasswd,0,sizeof(szNewPasswd));
	strcpy(szNewPasswd,(LPCTSTR)objNewPasswd);

	//�V�p�X���[�h�̂l�c�T���擾
	CMd5	objMd5;
	memset(szNewPasswdMD5,0,sizeof(szNewPasswdMD5));
	objMd5.MD5String(szNewPasswd,szNewPasswdMD5);
	objNewPasswdMD5 = szNewPasswdMD5;

	//�p�X���[�h���ς�������ǂ������`�F�b�N
	objOldPasswdMD5 = m_pApp->GetProfileString("CEditorApp","ExecPasswd","");
	if(objOldPasswdMD5.Compare(objNewPasswdMD5) != 0) //�Ⴄ
	{
		m_bIsChanged = TRUE;
		SetModified();

		//�l�c�T�ϊ���̐V�p�X���[�h�ێ�
		m_objExecPasswdStr = objNewPasswdMD5;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�G�N�X�v���[������J����悤�ɂ���v�`�F�b�N������					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckEnableshellopen() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�f�X�N�g�b�v���Xerxes�ւ̃V���[�g�J�b�g�쐬�v�`�F�b�N������		|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckMakeshortcutdsktop() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ŐV�t�@�C�����G�f�B�b�g�{�b�N�X�ύX������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnChangeEditMaxmru() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�uIMM�����I�[�v���v�`�F�b�N������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage1::OnCheckImmautoopen() 
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
void CMyPropertyPage1::OnOK() 
{
	CString objMsg;

	//�R���g���[���|�������o�ϐ��f�[�^��荞��
	UpdateData(TRUE);

	if(m_bShowSplash)
	{
		CString objStr;
		m_objSplashTimeEdit.GetWindowText(objStr);
		m_lSplashTime = atoi((char*)(LPCTSTR)objStr);

		//�X�v���b�V���X�N���[���\�����Ԃ̑Ó����`�F�b�N
		if(m_lSplashTime < 1 || m_lSplashTime > 10)
		{
			objMsg.LoadString(IDS_SPLASHTIME_ERR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONEXCLAMATION),0);
		}
	}
	if(m_bIsExecPasswd)
	{
		if(m_objExecPasswdStr.IsEmpty())
		{
			objMsg.LoadString(IDS_SET_PASSWD);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		}
	}

	CPropertyPage::OnOK();
}



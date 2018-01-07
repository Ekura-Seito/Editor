// MyPropertyPage12.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "MyPropertyPage12.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMyPropertyPage12, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage12 �v���p�e�B �y�[�W

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage12::CMyPropertyPage12() : CPropertyPage(CMyPropertyPage12::IDD)
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsChanged = FALSE;								// �ύX�����t���O

	//{{AFX_DATA_INIT(CMyPropertyPage12)
	m_bIsUseProxy = FALSE;
	m_bIsDialup = FALSE;
	m_bIsAutoDisconnect = FALSE;
	//}}AFX_DATA_INIT

	m_objProxyIPAddress = _T("");
	m_objDialupEntry = _T("");
	m_objRasUserName = _T("");
	m_objRasPasswd = _T("");
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage12::~CMyPropertyPage12()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�c�c�w�f�[�^����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage12)
	DDX_Check(pDX, IDC_CHECK_USEPROXY, m_bIsUseProxy);
	DDX_Control(pDX, IDC_EDIT_PROXYIPADDRESS, m_objEditProxyIPAddress);
	DDX_Check(pDX, IDC_CHECK_DIALUP, m_bIsDialup);
	DDX_Control(pDX, IDC_COMBO_DIALUPENTRY, m_objComboDialupEntry);
	DDX_Control(pDX, IDC_EDIT_RASUSERNAME, m_objEditRasUserName);
	DDX_Control(pDX, IDC_EDIT_RASPASSWD, m_objEditRasPasswd);
	DDX_Check(pDX, IDC_CHECK_AUTODISCONNECT, m_bIsAutoDisconnect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPropertyPage12, CPropertyPage)
	//{{AFX_MSG_MAP(CMyPropertyPage12)
	ON_BN_CLICKED(IDC_CHECK_USEPROXY, OnCheckUseproxy)
	ON_EN_CHANGE(IDC_EDIT_PROXYIPADDRESS, OnChangeEditProxyipaddress)
	ON_BN_CLICKED(IDC_CHECK_DIALUP, OnCheckDialup)
	ON_CBN_SELCHANGE(IDC_COMBO_DIALUPENTRY, OnSelchangeComboDialupentry)
	ON_EN_CHANGE(IDC_EDIT_RASUSERNAME, OnChangeEditRasusername)
	ON_EN_CHANGE(IDC_EDIT_RASPASSWD, OnChangeEditRaspasswd)
	ON_BN_CLICKED(IDC_CHECK_AUTODISCONNECT, OnCheckAutoDisconnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage12 �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ύX������������														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage12::IsChanged() 
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
void CMyPropertyPage12::ClearChangeFlag()
{
	m_bIsChanged = FALSE;
	SetModified(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage12 ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage12::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	//�f�t�H���g�f�[�^�ݒ�
	m_objEditProxyIPAddress.SetWindowText(m_objProxyIPAddress);
	m_objEditRasUserName.SetWindowText(m_objRasUserName);
	m_objEditRasPasswd.SetWindowText(m_objRasPasswd);

	//�_�C�A���A�b�v�G���g���R���{�{�b�N�X�ݒ�
	SetDialupEntryCombo();

	//�v���L�V�T�[�o���g���ꍇ�ɂ̓v���L�V�T�[�o�A�h���X����L����
	if(m_bIsUseProxy)
		m_objEditProxyIPAddress.EnableWindow(TRUE);
	else
		m_objEditProxyIPAddress.EnableWindow(FALSE);

	//�����o�ϐ��|���R���g���[��
	UpdateData(FALSE);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�v���L�V�T�[�o���g���v�`�F�b�N������								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::OnCheckUseproxy() 
{
	//�R���g���[���|�������o�ϐ��f�[�^��荞��
	UpdateData(TRUE);

	m_bIsChanged = TRUE;
	SetModified();

	if(m_bIsUseProxy)
		m_objEditProxyIPAddress.EnableWindow(TRUE);
	else
		m_objEditProxyIPAddress.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v���L�V�T�[�o�̃A�h���X�ύX������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::OnChangeEditProxyipaddress() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u���f�����g�p���ă_�C�A���A�b�v�v�I��������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::OnCheckDialup() 
{
	//�R���g���[���|�������o�ϐ�
	UpdateData(TRUE);

	m_bIsChanged = TRUE;
	SetModified();

	if(!m_bIsDialup)
	{
		m_objComboDialupEntry.EnableWindow(FALSE);
		m_objEditRasUserName.EnableWindow(FALSE);
		m_objEditRasPasswd.EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_AUTODISCONNECT)->EnableWindow(FALSE);
	}
	else
	{
		m_objComboDialupEntry.EnableWindow(TRUE);
		m_objEditRasUserName.EnableWindow(TRUE);
		m_objEditRasPasswd.EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_AUTODISCONNECT)->EnableWindow(TRUE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���A�b�v�G���g���R���{�{�b�N�X�I��ύX������					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::OnSelchangeComboDialupentry() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�q�`�r���[�U���G�f�B�b�g�{�b�N�X�ύX������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::OnChangeEditRasusername() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�q�`�r�p�X���[�h�G�f�B�b�g�{�b�N�X�ύX������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::OnChangeEditRaspasswd() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�I���������ؒf�v�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::OnCheckAutoDisconnect() 
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
void CMyPropertyPage12::OnOK() 
{
	int nIndex=0;
	CString objMsg;

	//�R���g���[���|�������o�ϐ��f�[�^��荞��
	UpdateData(TRUE);

	//�f�[�^�擾
	m_objEditProxyIPAddress.GetWindowText(m_objProxyIPAddress);
	nIndex = m_objComboDialupEntry.GetCurSel();
	if(nIndex == CB_ERR)
		return;
	m_objComboDialupEntry.GetLBText(nIndex,m_objDialupEntry);
	m_objEditRasUserName.GetWindowText(m_objRasUserName);
	m_objEditRasPasswd.GetWindowText(m_objRasPasswd);

	//�u�����N�`�F�b�N
	if(m_bIsUseProxy && m_objProxyIPAddress.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_PROXYIPADDRESS);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if(m_bIsDialup)
	{
		if(m_objRasUserName.IsEmpty())
		{
			objMsg.LoadString(IDS_RAS_NOUSERNAME);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	CPropertyPage::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage12 �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���A�b�v�G���g���R���{�{�b�N�X�ݒ�								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::SetDialupEntryCombo()
{
	int				nCnt=0;
	LONG			lRet=0;
	BOOL			bFind=FALSE;
	LPRASENTRYNAME	lpRasEntry=NULL;
	DWORD			cEntry=0;
	DWORD			dwRet=0,dwNeedSize=0;
	UINT			ndx=0;

	//�_�C�A���A�b�v�l�b�g���[�N�C���X�g�[���ς݃`�F�b�N
	if(m_pApp->m_hRasDllInstance == NULL)
	{
		GetDlgItem(IDC_CHECK_DIALUP)->EnableWindow(FALSE);
		m_objComboDialupEntry.EnableWindow(FALSE);
		m_objEditRasUserName.EnableWindow(FALSE);
		m_objEditRasPasswd.EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_AUTODISCONNECT)->EnableWindow(FALSE);
		m_bIsDialup = FALSE;
		m_bIsAutoDisconnect = FALSE;
		return;
	}

	//�֐��A�h���X�擾
	m_pApp->m_lpfnRasFunc1 = (LPFNRASFUNC1)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasGetEntryNeedSize");
	m_pApp->m_lpfnRasFunc2 = (LPFNRASFUNC2)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasGetRasEntry");
	if( m_pApp->m_lpfnRasFunc1 == NULL || m_pApp->m_lpfnRasFunc2 == NULL)
	{
		GetDlgItem(IDC_CHECK_DIALUP)->EnableWindow(FALSE);
		m_objComboDialupEntry.EnableWindow(FALSE);
		m_objEditRasUserName.EnableWindow(FALSE);
		m_objEditRasPasswd.EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_AUTODISCONNECT)->EnableWindow(FALSE);
		m_bIsDialup = FALSE;
		m_bIsAutoDisconnect = FALSE;
		return;
	}

	//�K�v�T�C�Y�擾
	dwNeedSize = m_pApp->m_lpfnRasFunc1();
	lpRasEntry = (LPRASENTRYNAME)malloc((UINT)dwNeedSize);
	memset(lpRasEntry,0,dwNeedSize);

	//�q�`�r�ڑ��G���g���擾(rasGetRasEntry)
	lRet = m_pApp->m_lpfnRasFunc2( lpRasEntry, &cEntry );
	if( lRet != 0 ) //NG
	{
		if(lpRasEntry)
			free(lpRasEntry);

		GetDlgItem(IDC_CHECK_DIALUP)->EnableWindow(FALSE);
		m_objComboDialupEntry.EnableWindow(FALSE);
		m_objEditRasUserName.EnableWindow(FALSE);
		m_objEditRasPasswd.EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_AUTODISCONNECT)->EnableWindow(FALSE);
		m_bIsDialup = FALSE;
		m_bIsAutoDisconnect = FALSE;
		return;
	}

	//�ڑ������R���{�{�b�N�X�֐ݒ�
	for(ndx=0; ndx<cEntry; ndx++)
		m_objComboDialupEntry.AddString(lpRasEntry[ndx].szEntryName);

	bFind = FALSE;
	for(ndx=0; ndx<cEntry; ndx++)
	{
		if(!m_objDialupEntry.CompareNoCase(lpRasEntry[ndx].szEntryName))
		{
			m_objComboDialupEntry.SetCurSel(ndx);
			bFind = TRUE;
			break;
		}
	}
	if(bFind == FALSE)
	{
		m_objDialupEntry = lpRasEntry[0].szEntryName;
		m_objComboDialupEntry.SetCurSel(0);
	}

	//�q�`�r�G���g���������J��
	free(lpRasEntry);
}


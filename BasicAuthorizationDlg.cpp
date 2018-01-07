// BasicAuthorizationDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "Editor_Def.h"
#include "AutoResizer.h"
#include "BasicAuthorizationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBasicAuthorizationDlg �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CBasicAuthorizationDlg::CBasicAuthorizationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBasicAuthorizationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBasicAuthorizationDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_objAuthSite = _T("");
	m_objAuthRealm = _T("");
	m_objAuthUser = _T("");
	m_objAuthPasswd = _T("");
	m_bIsSaveThisPassword = FALSE;
	m_hIcon = m_pApp->LoadIcon(IDR_MAINFRAME);
	m_hIconS = (HICON)::LoadImage(m_pApp->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME),
			IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�c�c�w�f�[�^����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBasicAuthorizationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBasicAuthorizationDlg)
	DDX_Control(pDX, IDC_EDIT_AUTH_USER, m_objAuthUserEdit);
	DDX_Control(pDX, IDC_EDIT_AUTH_PASSWD, m_objAuthPasswdEdit);
	DDX_Check(pDX, IDC_CHECK_SAVETHISPASSWD, m_bIsSaveThisPassword);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBasicAuthorizationDlg, CDialog)
	//{{AFX_MSG_MAP(CBasicAuthorizationDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBasicAuthorizationDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CBasicAuthorizationDlg::OnInitDialog()
{
	CString objMsg,objKey;
	CDialog::OnInitDialog();

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//�I�[�g���T�C�Y
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_AUTH_SITE,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_AUTH_REALM,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_EDIT_AUTH_USER,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_EDIT_AUTH_PASSWD,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_CHECK_SAVETHISPASSWD,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDOK,CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDCANCEL,CP_MOVE_HORIZONTAL);

	//�E�C���h�E�ʒu����
	WINDOWPLACEMENT		wp;
	if(LoadWindowPlacement(&wp))
		SetWindowPlacement(&wp);

	//�I�[�g���T�C�Y�N��
	m_objAutoResizer.MoveControls();

	//���[�U�h�c�A�p�X���[�h�ۑ��l����
	objKey.Format("%s_%s_bIsSaveThisPassword",m_objAuthSite,m_objAuthRealm);
	m_bIsSaveThisPassword = m_pApp->GetProfileInt("CBasicAuthorizationDlg",objKey,FALSE);
	if(m_bIsSaveThisPassword)
	{
		objKey.Format("%s_%s_userid",m_objAuthSite,m_objAuthRealm);
		m_objAuthUser = m_pApp->GetProfileString("CBasicAuthorizationDlg",objKey,"");
		objKey.Format("%s_%s_passwd",m_objAuthSite,m_objAuthRealm);
		m_objAuthPasswd = m_pApp->GetProfileString("CBasicAuthorizationDlg",objKey,"");
	}
	else
	{
		m_objAuthUser = "";
		m_objAuthPasswd = "";
	}

	//�T�C�g�A�̈���A���[�U�h�c�A�p�X���[�h��\��
	SetDlgItemText(IDC_AUTH_SITE, m_objAuthSite);
	SetDlgItemText(IDC_AUTH_REALM, m_objAuthRealm);
	m_objAuthUserEdit.SetWindowText(m_objAuthUser);
	m_objAuthUserEdit.UpdateData(TRUE);
	m_objAuthPasswdEdit.SetWindowText(m_objAuthPasswd);
	m_objAuthPasswdEdit.UpdateData(TRUE);

	//�����o�ϐ��|���R���g���[��
	UpdateData(FALSE);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�r�h�y�d����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBasicAuthorizationDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	//�I�[�g���T�C�Y�N��
	m_objAutoResizer.MoveControls();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�n�j�{�^������������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBasicAuthorizationDlg::OnOK() 
{
	CString objMsg,objKey;

	//�R���g���[���|�������o�ϐ��f�[�^��荞��
	UpdateData(TRUE);

	//�f�[�^�擾
	m_objAuthUserEdit.GetWindowText(m_objAuthUser);
	m_objAuthPasswdEdit.GetWindowText(m_objAuthPasswd);

	//�u�����N�`�F�b�N
	if(m_objAuthUser.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_USERID);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if(m_objAuthPasswd.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_PASSWORD);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//���[�U�h�c�ƃp�X���[�h��ۑ�
	if(m_bIsSaveThisPassword)
	{
		objKey.Format("%s_%s_bIsSaveThisPassword",m_objAuthSite,m_objAuthRealm);
		m_pApp->WriteProfileInt("CBasicAuthorizationDlg",objKey,m_bIsSaveThisPassword);
		objKey.Format("%s_%s_userid",m_objAuthSite,m_objAuthRealm);
		m_pApp->WriteProfileString("CBasicAuthorizationDlg",objKey,m_objAuthUser);
		objKey.Format("%s_%s_passwd",m_objAuthSite,m_objAuthRealm);
		m_pApp->WriteProfileString("CBasicAuthorizationDlg",objKey,m_objAuthPasswd);
	}
	else
	{
		objKey.Format("%s_%s_bIsSaveThisPassword",m_objAuthSite,m_objAuthRealm);
		m_pApp->WriteProfileInt("CBasicAuthorizationDlg",objKey,m_bIsSaveThisPassword);
		objKey.Format("%s_%s_userid",m_objAuthSite,m_objAuthRealm);
		m_pApp->WriteProfileString("CBasicAuthorizationDlg",objKey,"");
		objKey.Format("%s_%s_passwd",m_objAuthSite,m_objAuthRealm);
		m_pApp->WriteProfileString("CBasicAuthorizationDlg",objKey,"");
	}

	//�E�C���h�E�̏�Ԃƈʒu���L��������
	WINDOWPLACEMENT		wp;
	if(GetWindowPlacement(&wp))
	{
		if(IsZoomed())
			wp.flags |= WPF_RESTORETOMAXIMIZED;
		SaveWindowPlacement(&wp);
	}

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CBasicAuthorizationDlg �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�ʒu���[�h����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CBasicAuthorizationDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CBasicAuthorizationDlg","WindowPos");
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
void CBasicAuthorizationDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CBasicAuthorizationDlg","WindowPos",strBuffer);
}


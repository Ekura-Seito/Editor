// CheckExecPasswdDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "Editor_Def.h"
#include "CheckExecPasswdDlg.h"
#include "Mymd5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckExecPasswdDlg �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CCheckExecPasswdDlg::CCheckExecPasswdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckExecPasswdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckExecPasswdDlg)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT

	m_bIsPasswdOK = FALSE;
	m_pApp = (CEditorApp*)AfxGetApp();
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
void CCheckExecPasswdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckExecPasswdDlg)
	DDX_Control(pDX, IDC_EDIT_EXECPASSWD, m_objExecPasswdEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckExecPasswdDlg, CDialog)
	//{{AFX_MSG_MAP(CCheckExecPasswdDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckExecPasswdDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CCheckExecPasswdDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//�t�H���g�ݒ�
	m_objExecPasswdEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//�f�t�H���g�f�[�^�ݒ�
	m_objExecPasswdEdit.SetWindowText(_T(""));
	m_objExecPasswdEdit.UpdateData(TRUE);

	m_bIsPasswdOK = FALSE;

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//�����o�ϐ��|���R���g���[��
	UpdateData(FALSE);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�n�j�{�^������������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CCheckExecPasswdDlg::OnOK() 
{
	CString	objTypedPasswd, objTypedPasswdMD5, objOldPasswdMD5;
	char	szTypedPasswd[1024], szTypedPasswdMD5[1024];

	//�R���g���[���|�������o�ϐ��f�[�^��荞��
	UpdateData(TRUE);

	//�f�[�^�擾
	objTypedPasswd.Empty();
	m_objExecPasswdEdit.GetWindowText(objTypedPasswd);
	memset(szTypedPasswd,0,sizeof(szTypedPasswd));
	strcpy(szTypedPasswd,(LPCTSTR)objTypedPasswd);

	//���͂��ꂽ�p�X���[�h�̂l�c�T���擾
	CMd5	objMd5;
	memset(szTypedPasswdMD5,0,sizeof(szTypedPasswdMD5));
	objMd5.MD5String(szTypedPasswd,szTypedPasswdMD5);
	objTypedPasswdMD5 = szTypedPasswdMD5;

	//�p�X���[�h�`�F�b�N
	objOldPasswdMD5 = m_pApp->GetProfileString("CEditorApp","ExecPasswd","");
	if(objOldPasswdMD5.Compare(objTypedPasswdMD5) != 0) //�Ⴄ
	{
		CString objMsg;
		objMsg.LoadString(IDS_PASSWORD_NG);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		m_bIsPasswdOK = FALSE;
		return;
	}
	else
	{
		m_bIsPasswdOK = TRUE;
		CDialog::OnOK();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�b�`�m�b�d�k�{�^������������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CCheckExecPasswdDlg::OnCancel() 
{
	m_bIsPasswdOK = FALSE;

	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// CCheckExecPasswdDlg �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�p�X���[�h�`�F�b�N���ʎ擾											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CCheckExecPasswdDlg::IsPasswdOK()
{
	return m_bIsPasswdOK;
}


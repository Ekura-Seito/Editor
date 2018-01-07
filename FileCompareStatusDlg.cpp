// FileCompareStatusDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "StaticFilespec.h"
#include "FileCompareStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileCompareStatusDlg �_�C�A���O

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFileCompareStatusDlg::CFileCompareStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileCompareStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileCompareStatusDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsCanceled = FALSE;
	m_objFile1Name = _T("");
	m_objFile2Name = _T("");
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
void CFileCompareStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileCompareStatusDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileCompareStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CFileCompareStatusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileCompareStatusDlg �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����Z���`�F�b�N													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileCompareStatusDlg::IsCanceled()
{
	return m_bIsCanceled;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X���b�Z�[�W�ݒ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileCompareStatusDlg::SetStatus
	(	char*	pszMsg
	)
{
	CString objStr = pszMsg;
	SetDlgItemText(IDC_STATUS, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�e�[�^�X���b�Z�[�W�ݒ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileCompareStatusDlg::SetStatus
	(	CString& objStr
	)
{
	SetDlgItemText(IDC_STATUS, objStr);
}

/////////////////////////////////////////////////////////////////////////////
// CFileCompareStatusDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileCompareStatusDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//���t�@�C�����\��
	m_wndStaticFileName1.SubclassDlgItem(IDC_FILE1, this);
	m_wndStaticFileName1.SetPath(TRUE);
	m_wndStaticFileName1.SetText(m_objFile1Name);
	//���t�@�C�����\��
	m_wndStaticFileName2.SubclassDlgItem(IDC_FILE2, this);
	m_wndStaticFileName2.SetPath(TRUE);
	m_wndStaticFileName2.SetText(m_objFile2Name);

	//�����o�ϐ��|���R���g���[��
	UpdateData(FALSE);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����Z���{�^������������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileCompareStatusDlg::OnCancel() 
{
	m_bIsCanceled = TRUE;
	CDialog::OnCancel();
}


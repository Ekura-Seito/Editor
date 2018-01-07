// FileCompareResultDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "StaticFilespec.h"
#include "FileCompareResultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileCompareResultDlg �_�C�A���O

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFileCompareResultDlg::CFileCompareResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileCompareResultDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileCompareResultDlg)
	m_objAddLineCountStr = _T("");
	m_objDelLineCountStr = _T("");
	m_objStatusStr = _T("");
	m_objTimeStr = _T("");
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
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
void CFileCompareResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileCompareResultDlg)
	DDX_Text(pDX, IDC_ADDLINE_CNT, m_objAddLineCountStr);
	DDX_Text(pDX, IDC_DELLINE_CNT, m_objDelLineCountStr);
	DDX_Text(pDX, IDC_COMPARE_STATUS, m_objStatusStr);
	DDX_Text(pDX, IDC_TIME, m_objTimeStr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileCompareResultDlg, CDialog)
	//{{AFX_MSG_MAP(CFileCompareResultDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileCompareResultDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileCompareResultDlg::OnInitDialog() 
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
	m_wndStaticFileName1.SubclassDlgItem(IDC_FILE1_NAME, this);
	m_wndStaticFileName1.SetPath(TRUE);
	SetDlgItemText(IDC_FILE1_NAME, m_objFile1Name);
	//���t�@�C�����\��
	m_wndStaticFileName2.SubclassDlgItem(IDC_FILE2_NAME, this);
	m_wndStaticFileName2.SetPath(TRUE);
	SetDlgItemText(IDC_FILE2_NAME, m_objFile2Name);

	//�����o�ϐ��|���R���g���[��
	UpdateData(FALSE);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}


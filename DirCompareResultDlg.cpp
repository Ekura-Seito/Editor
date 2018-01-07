// DirCompareResultDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "Editor_Def.h"
#include "StaticFilespec.h"
#include "DirCompareResultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirCompareResultDlg �_�C�A���O

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDirCompareResultDlg::CDirCompareResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDirCompareResultDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDirCompareResultDlg)
	m_objStatusStr = _T("");
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_objDir1Name = _T("");
	m_objDir2Name = _T("");
	m_lCompareFileCount = 0;
	m_lAddFileCount = 0;
	m_lDelFileCount = 0;
	m_lDiffFileCount = 0;
	m_lTime = 0;
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
void CDirCompareResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDirCompareResultDlg)
	DDX_Text(pDX, IDC_COMPARE_STATUS, m_objStatusStr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDirCompareResultDlg, CDialog)
	//{{AFX_MSG_MAP(CDirCompareResultDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirCompareResultDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDirCompareResultDlg::OnInitDialog() 
{
	CString objStr = _T("");
	CString objMsg;

	CDialog::OnInitDialog();

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//���f�B���N�g�����\��
	m_wndStaticDirName1.SubclassDlgItem(IDC_DIR1_NAME, this);
	m_wndStaticDirName1.SetPath(TRUE);
	SetDlgItemText(IDC_DIR1_NAME, m_objDir1Name);
	//���f�B���N�g�����\��
	m_wndStaticDirName2.SubclassDlgItem(IDC_DIR2_NAME, this);
	m_wndStaticDirName2.SetPath(TRUE);
	SetDlgItemText(IDC_DIR2_NAME, m_objDir2Name);

	//��r�t�@�C������
	objStr.Format("%6ld",m_lCompareFileCount);
	SetDlgItemText(IDC_COMPAREFILE_CNT,objStr);
	//�폜�t�@�C����
	objStr.Format("%6ld",m_lAddFileCount);
	SetDlgItemText(IDC_ADDFILE_CNT,objStr);
	//�ǉ��t�@�C����
	objStr.Format("%6ld",m_lDelFileCount);
	SetDlgItemText(IDC_DELFILE_CNT,objStr);
	//����t�@�C����
	objStr.Format("%6ld",m_lDiffFileCount);
	SetDlgItemText(IDC_DIFFILE_CNT,objStr);

	//���v����
	objMsg.LoadString(IDS_TIME);
	objStr.Format((LPCTSTR)objMsg, m_lTime);
	SetDlgItemText(IDC_TIME,objStr);

	//�����o�ϐ��|���R���g���[��
	UpdateData(FALSE);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}


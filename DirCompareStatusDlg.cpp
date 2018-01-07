// DirCompareStatusDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "StaticFilespec.h"
#include "DirCompareStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirCompareStatusDlg �_�C�A���O

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDirCompareStatusDlg::CDirCompareStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDirCompareStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDirCompareStatusDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsCanceled = FALSE;
	m_lCompareFileCount = 0;
	m_lAddFileCount = 0;
	m_lDelFileCount = 0;
	m_lDiffFileCount = 0;
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
void CDirCompareStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDirCompareStatusDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDirCompareStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CDirCompareStatusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirCompareStatusDlg �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����Z���`�F�b�N													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDirCompareStatusDlg::IsCanceled()
{
	return m_bIsCanceled;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F��r���f�B���N�g���ݒ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareStatusDlg::SetDir1
	(	CString&	objDir
	)
{
	m_wndStaticDirName1.SetText(objDir);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F��r���f�B���N�g���ݒ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareStatusDlg::SetDir2
	(	CString&	objDir
	)
{
	m_wndStaticDirName2.SetText(objDir);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F��r�󋵐ݒ�															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareStatusDlg::SetStatus
	(	char*	szMsg
	)
{
	CString objStr = szMsg;
	SetDlgItemText(IDC_STATUS, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F��r�󋵐ݒ�															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareStatusDlg::SetStatus
	(	CString& objStr
	)
{
	SetDlgItemText(IDC_STATUS, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F��r�t�@�C�����J�E���g�A�b�v											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareStatusDlg::IncCompareFileCount()
{
	m_lCompareFileCount++;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F��r�t�@�C�����擾													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CDirCompareStatusDlg::GetCompareFileCount()
{
	return m_lCompareFileCount;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ǉ��t�@�C�����J�E���g�A�b�v											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareStatusDlg::IncAddFileCount()
{
	m_lAddFileCount++;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ǉ��t�@�C�����擾													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CDirCompareStatusDlg::GetAddFileCount()
{
	return m_lAddFileCount;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�폜�t�@�C�����J�E���g�A�b�v											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareStatusDlg::IncDelFileCount()
{
	m_lDelFileCount++;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�폜�t�@�C�����擾													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CDirCompareStatusDlg::GetDelFileCount()
{
	return m_lDelFileCount;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����t�@�C�����J�E���g�A�b�v											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareStatusDlg::IncDiffFileCount()
{
	m_lDiffFileCount++;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����t�@�C�����擾													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CDirCompareStatusDlg::GetDiffFileCount()
{
	return m_lDiffFileCount;
}

/////////////////////////////////////////////////////////////////////////////
// CDirCompareStatusDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDirCompareStatusDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//���f�B���N�g��
	m_wndStaticDirName1.SubclassDlgItem(IDC_DIR1, this);
	m_wndStaticDirName1.SetPath(TRUE);
	//���f�B���N�g��
	m_wndStaticDirName2.SubclassDlgItem(IDC_DIR2, this);
	m_wndStaticDirName2.SetPath(TRUE);

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
void CDirCompareStatusDlg::OnCancel() 
{
	m_bIsCanceled = TRUE;
	CDialog::OnCancel();
}


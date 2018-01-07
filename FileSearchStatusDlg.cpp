// FileSearchStatusDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "StaticFilespec.h"
#include "FileSearchStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileSearchStatusDlg �_�C�A���O

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFileSearchStatusDlg::CFileSearchStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileSearchStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileSearchStatusDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsCanceled = 0;
	m_lMatchCount = 0;
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
void CFileSearchStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileSearchStatusDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileSearchStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CFileSearchStatusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileSearchStatusDlg �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����Z���`�F�b�N													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileSearchStatusDlg::IsCanceled()
{
	return m_bIsCanceled;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����ʃ��b�Z�[�W�ݒ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileSearchStatusDlg::SetFileKind
	(	CString&	objStr
	)
{
	SetDlgItemText(IDC_FILEKIND,objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����t�H���_���b�Z�[�W�ݒ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileSearchStatusDlg::SetFolder
	(	CString&		objStr
	)
{
	m_wndStaticFolder.SetText(objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����t�@�C�����b�Z�[�W�ݒ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileSearchStatusDlg::SetFile
	(	CString&	objStr
	)
{
	SetDlgItemText(IDC_FILE, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�Y�������J�E���g�A�b�v����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileSearchStatusDlg::IncMatchCount()
{
	m_lMatchCount++;
	CString	objStr;
	objStr.Format("%ld",m_lMatchCount);
	SetDlgItemText(IDC_MATCHCOUNT, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�Y�������擾����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CFileSearchStatusDlg::GetMatchCount()
{
	return m_lMatchCount;
}

/////////////////////////////////////////////////////////////////////////////
// CFileSearchStatusDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileSearchStatusDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//�t�H���_
	m_wndStaticFolder.SubclassDlgItem(IDC_FOLDER, this);
	m_wndStaticFolder.SetPath(TRUE);

	//�Y������������
	CString	objStr;
	objStr.Format("%ld",m_lMatchCount);
	SetDlgItemText(IDC_MATCHCOUNT, objStr);

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
void CFileSearchStatusDlg::OnCancel() 
{
	m_bIsCanceled = TRUE;
	CDialog::OnCancel();
}


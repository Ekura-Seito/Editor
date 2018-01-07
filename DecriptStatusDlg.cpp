// DecriptStatusDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "StaticFilespec.h"
#include "DecriptStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDecriptStatusDlg �_�C�A���O

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDecriptStatusDlg::CDecriptStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDecriptStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDecriptStatusDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsCanceled = 0;
	m_lDecriptFileCount = 0;
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
void CDecriptStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDecriptStatusDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDecriptStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CDecriptStatusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDecriptStatusDlg �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����Z���`�F�b�N													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDecriptStatusDlg::IsCanceled()
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
void CDecriptStatusDlg::SetFileKind
	(	CString&	objStr
	)
{
	SetDlgItemText(IDC_FILEKIND,objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�H���_���b�Z�[�W�ݒ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDecriptStatusDlg::SetFolder
	(	CString&		objStr
	)
{
	m_wndStaticFolder.SetText(objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C�����b�Z�[�W�ݒ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDecriptStatusDlg::SetFile
	(	CString&	objStr
	)
{
	SetDlgItemText(IDC_FILE, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�������t�@�C�������J�E���g�A�b�v����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDecriptStatusDlg::IncDecriptFileCount()
{
	m_lDecriptFileCount++;
	CString	objStr;
	objStr.Format("%ld",m_lDecriptFileCount);
	SetDlgItemText(IDC_FILECOUNT, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�������t�@�C�������擾												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CDecriptStatusDlg::GetDecriptFileCount()
{
	return m_lDecriptFileCount;
}

/////////////////////////////////////////////////////////////////////////////
// CDecriptStatusDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDecriptStatusDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//�������t�@�C������������
	CString	objStr;
	objStr.Format("%ld",m_lDecriptFileCount);
	SetDlgItemText(IDC_FILECOUNT, objStr);

	//�t�H���_�\���T�u�N���X��
	m_wndStaticFolder.SubclassDlgItem(IDC_FOLDER, this);
	m_wndStaticFolder.SetPath(TRUE);

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
void CDecriptStatusDlg::OnCancel() 
{
	m_bIsCanceled = TRUE;
	CDialog::OnCancel();
}


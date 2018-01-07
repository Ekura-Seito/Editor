// LineJumpDlg.cpp : CLineJumpDlg �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "Editor.h"
#include "LineJumpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineJumpDlg �_�C�A���O

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLineJumpDlg::CLineJumpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineJumpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLineJumpDlg)
	m_lLineNo = 0;
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_hIcon = m_pApp->LoadIcon(IDR_MAINFRAME);
	m_hIconS = (HICON)::LoadImage(m_pApp->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME),
			IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
}


void CLineJumpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineJumpDlg)
	DDX_Text(pDX, IDC_EDIT_LINENO, m_lLineNo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineJumpDlg, CDialog)
	//{{AFX_MSG_MAP(CLineJumpDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineJumpDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\������������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CLineJumpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//�X�^�e�B�b�N�e�L�X�g�ݒ�
	CString objStr;
	objStr.Format("( %d �` %d )", 1, m_lMaxNo );
	SetDlgItemText(IDC_LINENO, objStr);

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
void CLineJumpDlg::OnOK() 
{
	CDialog::OnOK();
}


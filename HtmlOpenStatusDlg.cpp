// HtmlOpenStatusDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "HtmlOpenStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHtmlOpenStatusDlg �_�C�A���O

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CHtmlOpenStatusDlg::CHtmlOpenStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHtmlOpenStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHtmlOpenStatusDlg)
	//}}AFX_DATA_INIT

	m_bIsCanceled = FALSE;
	m_pApp = (CEditorApp*)AfxGetApp();
	m_hIcon = m_pApp->LoadIcon(IDR_MAINFRAME);
	m_hIconS = (HICON)::LoadImage(m_pApp->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME),
			IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	m_nProgressLower = 0;
	m_nProgressUpper = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�c�c�w�f�[�^����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CHtmlOpenStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHtmlOpenStatusDlg)
	DDX_Control(pDX, IDC_PROGRESS_BYTE, m_objProgressBar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHtmlOpenStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CHtmlOpenStatusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHtmlOpenStatusDlg �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����Z���`�F�b�N													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CHtmlOpenStatusDlg::IsCanceled()
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
void CHtmlOpenStatusDlg::SetStatus
	(	char*	pszMsg
	)
{
	if(IsCanceled())
		return;

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
void CHtmlOpenStatusDlg::SetStatus
	(	CString& objStr
	)
{
	if(IsCanceled())
		return;
	SetDlgItemText(IDC_STATUS, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v���O���X�o�[�͈͐ݒ菈��											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CHtmlOpenStatusDlg::SetRange
	(	int nLower,		//�͈͂̉���
		int nUpper		//�͈͂̏��
	)
{
	if(IsCanceled())
		return;

	//�v���O���X�o�[�͈̔͐ݒ�
	m_objProgressBar.SetRange(nLower,nUpper);
	m_nProgressLower = nLower;
	m_nProgressUpper = nUpper;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v���O���X�o�[�ʒu�ݒ菈��											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CHtmlOpenStatusDlg::SetPos
	(	int nPos		//�ʒu�iSetRange�Őݒ肵���͈͓��ł��邱�Ɓj
	)
{
	if(IsCanceled())
		return;
	if(nPos < m_nProgressLower || nPos > m_nProgressUpper)
		return;

	//�v���O���X�o�[�̌��ݒl�ݒ�
	m_objProgressBar.SetPos(nPos);

	//�o�C�g�����ݒ�
	CString objBuf;
	float percent=0;
	if(m_nProgressUpper != 0)
		percent = (float)((nPos * 100) / m_nProgressUpper);
	objBuf.Format("%5d / %5d ( %3.1f %% )", nPos, m_nProgressUpper, percent);

	SetDlgItemText(IDC_BYTE, objBuf);
}

/////////////////////////////////////////////////////////////////////////////
// CHtmlOpenStatusDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CHtmlOpenStatusDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	SetRange(0,0);
	SetPos(0);
	SetDlgItemText(IDC_BYTE, "");

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
void CHtmlOpenStatusDlg::OnCancel() 
{
	m_bIsCanceled = TRUE;
	CDialog::OnCancel();
}


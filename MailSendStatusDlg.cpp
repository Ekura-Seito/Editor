// MailSendStatusDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "MailSendStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailSendStatusDlg �_�C�A���O

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMailSendStatusDlg::CMailSendStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMailSendStatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMailSendStatusDlg)
	//}}AFX_DATA_INIT

	m_bIsCanceled = FALSE;
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
void CMailSendStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailSendStatusDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMailSendStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CMailSendStatusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailSendStatusDlg �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����Z���`�F�b�N													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMailSendStatusDlg::IsCanceled()
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
void CMailSendStatusDlg::SetStatus
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
void CMailSendStatusDlg::SetStatus
	(	CString&	objStr
	)
{
	if(IsCanceled())
		return;
	SetDlgItemText(IDC_STATUS, objStr);
}

/////////////////////////////////////////////////////////////////////////////
// CMailSendStatusDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMailSendStatusDlg::OnInitDialog() 
{
	int nCurrentAccountNo=0;
	CString objMsg;
	char szSection[64];
	CWnd*	pWnd=NULL;

	CDialog::OnInitDialog();

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	nCurrentAccountNo = m_pApp->GetProfileInt("Mail","CurrentAccountNo",0);
	if(nCurrentAccountNo < 0 || nCurrentAccountNo > 8)
		nCurrentAccountNo = 0;
	sprintf(szSection,"Mail_Account%d",nCurrentAccountNo);

	//�A�J�E���g���ݒ�
	objMsg.LoadString(IDS_MAILACCOUNT_0 + nCurrentAccountNo);
	pWnd = GetDlgItem(IDC_MSENDACCOUNT);
	if(pWnd != NULL)
		pWnd->SetWindowText(objMsg);

	//�r�l�s�o�T�[�o���ݒ�
	objMsg = m_pApp->GetProfileString(szSection,"SmtpServer","");
	pWnd = GetDlgItem(IDC_MSENDSERVER);
	if(pWnd != NULL)
		pWnd->SetWindowText(objMsg);

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
void CMailSendStatusDlg::OnCancel() 
{
	m_bIsCanceled = TRUE;
	CDialog::OnCancel();
}

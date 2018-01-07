// FileTouchDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <afxdisp.h>	//For COleDateTime
#pragma warning(disable:4786)
#include "editor.h"
#include "Editor_Def.h"
#include "TimeMaskEdit.h"
#include "DateMaskEdit.h"
#include "AutoResizer.h"
#include "FileTouchDlg.h"
#include "DirDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileTouchDlg �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFileTouchDlg::CFileTouchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileTouchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileTouchDlg)
	m_bIsSearchSubFolder = FALSE;
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_objFolder = _T("");					// �t�@�C���������s�t�H���_
	m_objFileKind = _T("");					// �����Ώۂ̃t�@�C�����
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
void CFileTouchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileTouchDlg)
	DDX_Control(pDX, IDC_EDIT_TIME, m_objEditTime);
	DDX_Control(pDX, IDC_EDIT_DATE, m_objEditDate);
	DDX_Control(pDX, IDC_EDIT_FILEKIND, m_objEditFileKind);
	DDX_Control(pDX, IDC_EDIT_FOLDER, m_objEditFolder);
	DDX_Check(pDX, IDC_CHECK_SUBFOLDER, m_bIsSearchSubFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileTouchDlg, CDialog)
	//{{AFX_MSG_MAP(CFileTouchDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileTouchDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileTouchDlg::OnInitDialog() 
{
	CString objMsg;
	CDialog::OnInitDialog();

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//�f�t�H���g�f�[�^�ݒ�
	m_objEditFolder.SetWindowText(m_objFolder);
	m_objEditFolder.UpdateData(TRUE);
	m_objEditFileKind.SetWindowText(m_objFileKind);
	m_objEditFileKind.UpdateData(TRUE);

	//���t���̓G���A�Ǝ������̓G���A������
	m_objEditDate.Init();
	m_objEditTime.Init();

	//�c�[���q���g
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	objMsg.LoadString(IDS_BROWSE);
	m_tooltip.AddTool(GetDlgItem(IDC_BROWSE), objMsg);
	objMsg.LoadString(IDS_FILEKIND);
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_FILEKIND), objMsg);

	//�I�[�g���T�C�Y
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_EDIT_TIME,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_EDIT_FILEKIND,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_EDIT_FOLDER,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDOK,CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDCANCEL,CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_BROWSE,CP_MOVE_HORIZONTAL);

	//�E�C���h�E�ʒu����
	WINDOWPLACEMENT		wp;
	if(LoadWindowPlacement(&wp))
		SetWindowPlacement(&wp);

	//�I�[�g���T�C�Y�N��
	m_objAutoResizer.MoveControls();

	//�����o�ϐ��|���R���g���[��
	UpdateData(FALSE);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�r�h�y�d����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileTouchDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	//�I�[�g���T�C�Y�N��
	m_objAutoResizer.MoveControls();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���b�Z�[�W����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileTouchDlg::PreTranslateMessage(MSG* pMsg)
{
	m_tooltip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�H���_�u���E�Y�{�^������������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileTouchDlg::OnBrowse() 
{
	CDirDialog		objDirDialog;
	objDirDialog.m_strTitle = "  ";
	if( objDirDialog.DoBrowse() != FALSE )
	{
		m_objEditFolder.SetWindowText(objDirDialog.m_strPath);
		m_objEditFolder.UpdateData(TRUE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�n�j�{�^������������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileTouchDlg::OnOK() 
{
	CString objMsg;
	//�R���g���[���|�������o�ϐ��f�[�^��荞��
	UpdateData(TRUE);

	//�f�[�^�擾
	m_objEditFolder.GetWindowText(m_objFolder);
	m_objEditFileKind.GetWindowText(m_objFileKind);

	//�u�����N�`�F�b�N
	if(m_objFolder.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_FOLDER);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if(m_objFileKind.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_FILEKIND);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//���t���̓G���A�Ǝ������̓G���A����f�[�^�擾
	int	nYear   = m_objEditDate.GetYear();
	int nMonth  = m_objEditDate.GetMonth();
	int nDay    = m_objEditDate.GetDay();
	int nHour   = m_objEditTime.GetHour();
	int nMinute = m_objEditTime.GetMinute();
	int nSecond = m_objEditTime.GetSecond();

	//	���t�Ó����`�F�b�N
	COleDateTime	objOleDateTime(nYear,nMonth,nDay,nHour,nMinute,nSecond);
	if(objOleDateTime.GetStatus() != COleDateTime::valid)//�s��
	{
		objMsg.LoadString(IDS_FILETOUCH_INVALIDDATE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	m_mtime = CTime(nYear,nMonth,nDay,nHour,nMinute,nSecond);

	//�E�C���h�E�̏�Ԃƈʒu���L��������
	WINDOWPLACEMENT		wp;
	if(GetWindowPlacement(&wp))
	{
		if(IsZoomed())
			wp.flags |= WPF_RESTORETOMAXIMIZED;
		SaveWindowPlacement(&wp);
	}

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CFileTouchDlg �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�ʒu���[�h����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileTouchDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CTouchDlg","WindowPos");
	if(strBuffer.IsEmpty())
		return FALSE;

	int cRead = _stscanf(strBuffer,"%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
					&pwp->flags,
					&pwp->showCmd,
					&pwp->ptMinPosition.x, &pwp->ptMinPosition.y,
					&pwp->ptMaxPosition.x, &pwp->ptMaxPosition.y,
					&pwp->rcNormalPosition.left,  &pwp->rcNormalPosition.top,
					&pwp->rcNormalPosition.right, &pwp->rcNormalPosition.bottom);
	if(cRead != 10)
		return FALSE;
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�ʒu�Z�[�u����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileTouchDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	//�E�C���h�E�̈ʒu��ۑ�����
	CString	strBuffer;
	strBuffer.Format("%i:%i:%i:%i:%i:%i:%i:%i:%i:%i",
					pwp->flags,
					pwp->showCmd,
					pwp->ptMinPosition.x, pwp->ptMinPosition.y,
					pwp->ptMaxPosition.x, pwp->ptMaxPosition.y,
					pwp->rcNormalPosition.left,  pwp->rcNormalPosition.top,
					pwp->rcNormalPosition.right, pwp->rcNormalPosition.bottom);
	m_pApp->WriteProfileString("CTouchDlg","WindowPos",strBuffer);
}


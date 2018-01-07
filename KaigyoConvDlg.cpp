// KaigyoConvDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "CodeConvManager.h"
#include "AutoResizer.h"
#include "KaigyoConvDlg.h"
#include "DirDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKaigyoConvDlg �_�C�A���O

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CKaigyoConvDlg::CKaigyoConvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKaigyoConvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKaigyoConvDlg)
	m_bIsOutOtherFile = FALSE;
	m_bIsSearchSubFolder = FALSE;
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_objFolder = _T("");					// ���s�R�[�h�ϊ����s�t�H���_
	m_objConvKind = _T("");					// �ϊ����
	m_objFileKind = _T("");					// �ϊ��Ώۂ̃t�@�C�����
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
void CKaigyoConvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKaigyoConvDlg)
	DDX_Control(pDX, IDC_COMBO_CONVKIND, m_objComboConvKind);
	DDX_CBString(pDX, IDC_COMBO_CONVKIND, m_objConvKind);
	DDX_Control(pDX, IDC_EDIT_FOLDER, m_objEditFolder);
	DDX_Control(pDX, IDC_EDIT_FILEKIND, m_objEditFileKind);
	DDX_Check(pDX, IDC_CHECK_OUTOTHERFILE, m_bIsOutOtherFile);
	DDX_Check(pDX, IDC_CHECK_SUBFOLDER, m_bIsSearchSubFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKaigyoConvDlg, CDialog)
	//{{AFX_MSG_MAP(CKaigyoConvDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKaigyoConvDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CKaigyoConvDlg::OnInitDialog() 
{
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

	//�ϊ���ʃR���{�{�b�N�X�֐ݒ�
	m_objComboConvKind.AddString(KAIGYOCONV_TO_CRLF);
	m_objComboConvKind.AddString(KAIGYOCONV_TO_LF);
	m_objComboConvKind.AddString(KAIGYOCONV_TO_CR);
	if(m_objConvKind.IsEmpty())
	{
		m_objComboConvKind.SetCurSel(0);
	}
	else
	{
		if(m_objConvKind==KAIGYOCONV_TO_CRLF)
			m_objComboConvKind.SetCurSel(0);
		else if(m_objConvKind==KAIGYOCONV_TO_LF)
			m_objComboConvKind.SetCurSel(1);
		else if(m_objConvKind==KAIGYOCONV_TO_CR)
			m_objComboConvKind.SetCurSel(2);
	}

	//�c�[���q���g
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	m_tooltip.AddTool(GetDlgItem(IDC_BROWSE), IDS_BROWSE);
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_FILEKIND), IDS_FILEKIND);

	//�I�[�g���T�C�Y
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_COMBO_CONVKIND,CP_RESIZE_HORIZONTAL);
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
void CKaigyoConvDlg::OnSize(UINT nType, int cx, int cy) 
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
BOOL CKaigyoConvDlg::PreTranslateMessage(MSG* pMsg)
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
void CKaigyoConvDlg::OnBrowse() 
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
void CKaigyoConvDlg::OnOK() 
{
	CString objMsg;

	//�R���g���[���|�������o�ϐ��f�[�^��荞��
	UpdateData(TRUE);

	//�R���{�{�b�N�X�I���e�L�X�g�擾
	int nIndex = m_objComboConvKind.GetCurSel();
	if( (nIndex==CB_ERR) ||
		(nIndex < 0)     ||
		(nIndex > 2)
	)
	{
		objMsg.LoadString(IDS_KAIGYOCONVKIND_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	else
	{
		if(nIndex==0)
			m_objConvKind = KAIGYOCONV_TO_CRLF;
		else if(nIndex==1)
			m_objConvKind = KAIGYOCONV_TO_LF;
		else if(nIndex==2)
			m_objConvKind = KAIGYOCONV_TO_CR;
	}

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
// CKaigyoConvDlg �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�ʒu���[�h����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CKaigyoConvDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CKaigyoConvDlg","WindowPos");
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
void CKaigyoConvDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CKaigyoConvDlg","WindowPos",strBuffer);
}


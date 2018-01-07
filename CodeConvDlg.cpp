// CodeConvDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "AutoResizer.h"
#include "CodeConvDlg.h"
#include "DirDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCodeConvDlg �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CCodeConvDlg::CCodeConvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCodeConvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCodeConvDlg)
	m_bIsOutOtherFile = FALSE;
	m_bIsSearchSubFolder = FALSE;
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_objFolder = _T("");					// �����R�[�h�ϊ����s�t�H���_
	m_nConvKind = 0;						// �ϊ����
	m_objFileKind = _T("");					// �ϊ��Ώۂ̃t�@�C�����
	m_nInCode = 0;							// ���̓t�@�C���̕����R�[�h
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
void CCodeConvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCodeConvDlg)
	DDX_Control(pDX, IDC_COMBO_CONVKIND, m_objComboConvKind);
	DDX_Control(pDX, IDC_EDIT_FOLDER, m_objEditFolder);
	DDX_Control(pDX, IDC_EDIT_FILEKIND, m_objEditFileKind);
	DDX_Check(pDX, IDC_CHECK_OUTOTHERFILE, m_bIsOutOtherFile);
	DDX_Check(pDX, IDC_CHECK_SUBFOLDER, m_bIsSearchSubFolder);
	DDX_Control(pDX, IDC_COMBO_INCODE, m_objComboInCode);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCodeConvDlg, CDialog)
	//{{AFX_MSG_MAP(CCodeConvDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCodeConvDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CCodeConvDlg::OnInitDialog() 
{
	CString objMsg, objBuf;
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
	objMsg.LoadString(IDS_CODECONV_TO_EUC);
	m_objComboConvKind.AddString((LPCTSTR)objMsg);
	objMsg.LoadString(IDS_CODECONV_TO_SJIS);
	m_objComboConvKind.AddString((LPCTSTR)objMsg);
	objMsg.LoadString(IDS_CODECONV_TO_JIS);
	m_objComboConvKind.AddString((LPCTSTR)objMsg);
	if(m_nConvKind < 0 || m_nConvKind > 2)
		m_objComboConvKind.SetCurSel(0);
	else
		m_objComboConvKind.SetCurSel(m_nConvKind);

	//���̓t�@�C���̕����R�[�h�̔��ʂɂ���
	objMsg.LoadString(IDS_INCODE_AUTO);
	m_objComboInCode.AddString((LPCTSTR)objMsg);
	objMsg.LoadString(IDS_INCODE_EUC);
	m_objComboInCode.AddString((LPCTSTR)objMsg);
	objMsg.LoadString(IDS_INCODE_SJIS);
	m_objComboInCode.AddString((LPCTSTR)objMsg);
	objMsg.LoadString(IDS_INCODE_JIS);
	m_objComboInCode.AddString((LPCTSTR)objMsg);
	if(m_nInCode < 0 || m_nInCode > 3)
		m_objComboInCode.SetCurSel(0);
	else
		m_objComboInCode.SetCurSel(m_nInCode);

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
	m_objAutoResizer.AddControl(IDC_COMBO_CONVKIND,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_EDIT_FILEKIND,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_EDIT_FOLDER,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDOK,CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDCANCEL,CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_BROWSE,CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_COMBO_INCODE,CP_RESIZE_HORIZONTAL);

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
void CCodeConvDlg::OnSize(UINT nType, int cx, int cy) 
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
BOOL CCodeConvDlg::PreTranslateMessage(MSG* pMsg)
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
void CCodeConvDlg::OnBrowse() 
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
void CCodeConvDlg::OnOK() 
{
	CString objMsg;
	int nIndex=0;

	//�R���g���[���|�������o�ϐ��f�[�^��荞��
	UpdateData(TRUE);

	//�ϊ���ʃR���{�{�b�N�X�I���e�L�X�g�擾
	nIndex = m_objComboConvKind.GetCurSel();
	if( (nIndex==CB_ERR) ||
		(nIndex < 0)     ||
		(nIndex > 2)
	)
	{
		objMsg.LoadString(IDS_CONVKIND_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	else
	{
		if(nIndex==0)
			m_nConvKind = CODECONV_TO_EUC;
		else if(nIndex==1)
			m_nConvKind = CODECONV_TO_SJIS;
		else if(nIndex==2)
			m_nConvKind = CODECONV_TO_JIS;
	}

	//���̓t�@�C���̕����R�[�h�̔��ʃR���{�{�b�N�X�I���e�L�X�g�擾
	nIndex = m_objComboInCode.GetCurSel();
	if( (nIndex==CB_ERR) ||
		(nIndex < 0)     ||
		(nIndex > 3)
	)
	{
		objMsg.LoadString(IDS_INCODEKIND_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	else
	{
		if(nIndex==0)
			m_nInCode = INCODE_AUTO;
		else if(nIndex==1)
			m_nInCode = INCODE_EUC;
		else if(nIndex==2)
			m_nInCode = INCODE_SJIS;
		else if(nIndex==3)
			m_nInCode = INCODE_JIS;
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
// CCodeConvDlg �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�ʒu���[�h����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CCodeConvDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CCodeConvDlg","WindowPos");
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
void CCodeConvDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CCodeConvDlg","WindowPos",strBuffer);
}


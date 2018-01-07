// FilePropertyDlg.cpp : CFilePropertyDlg �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "StaticFilespec.h"
#include "AutoResizer.h"
#include "FilePropertyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilePropertyDlg �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFilePropertyDlg::CFilePropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFilePropertyDlg::IDD, pParent)
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_objTitle = _T("");

	//{{AFX_DATA_INIT(CFilePropertyDlg)
	m_objFileName = _T("");
	m_objCTime = _T("");
	m_objMTime = _T("");
	m_objATime = _T("");
	m_objSize = _T("");
	m_objAttribute = _T("");
	m_objKaigyo = _T("");
	m_objCode = _T("");
	m_objLineCnt = _T("");
	m_objTabSize = _T("");
	//}}AFX_DATA_INIT
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
void CFilePropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilePropertyDlg)
	DDX_Text(pDX, IDC_FILENAME, m_objFileName);
	DDX_Text(pDX, IDC_CTIME, m_objCTime);
	DDX_Text(pDX, IDC_MTIME, m_objMTime);
	DDX_Text(pDX, IDC_ATIME, m_objATime);
	DDX_Text(pDX, IDC_LOGSIZE, m_objSize);
	DDX_Text(pDX, IDC_ATTR, m_objAttribute);
	DDX_Text(pDX, IDC_KAIGYO, m_objKaigyo);
	DDX_Text(pDX, IDC_CODE, m_objCode);
	DDX_Text(pDX, IDC_LINECNT, m_objLineCnt);
	DDX_Text(pDX, IDC_TABSIZE, m_objTabSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFilePropertyDlg, CDialog)
	//{{AFX_MSG_MAP(CFilePropertyDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFilePropertyDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\������������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFilePropertyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//�^�C�g���\��
	m_wndStaticFileTitle.SubclassDlgItem(IDC_TITLE, this);
	m_wndStaticFileTitle.SetPath(TRUE);
	SetDlgItemText(IDC_TITLE, m_objTitle);

	//�I�[�g���T�C�Y
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_TITLE,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_FILENAME,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_CTIME,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_MTIME,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_ATIME,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_LOGSIZE,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_ATTR,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_KAIGYO,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_CODE,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_LINECNT,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_TABSIZE,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDOK,CP_MOVE_HORIZONTAL|CP_MOVE_VERTICAL);

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
void CFilePropertyDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	//�I�[�g���T�C�Y�N��
	m_objAutoResizer.MoveControls();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�n�j�{�^������������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFilePropertyDlg::OnOK() 
{
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
// CFilePropertyDlg �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�ʒu���[�h����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFilePropertyDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CFilePropertyDlg","WindowPos");
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
void CFilePropertyDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CFilePropertyDlg","WindowPos",strBuffer);
}


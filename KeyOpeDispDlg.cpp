// KeyOpeDispDlg.cpp : CKeyOpeDispDlg �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include <afxtempl.h>			//for CTypedPtrList
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "Editor.h"
#include "Operation.h"
#include "OperationManager.h"
#include "StaticFilespec.h"
#include "ListCtrlEx.h"
#include "AutoResizer.h"
#include "KeyOpeDispDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyOpeListDlg �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CKeyOpeDispDlg::CKeyOpeDispDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyOpeDispDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyOpeDispDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_pobjOperationManager = NULL;
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
void CKeyOpeDispDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyOpeDispDlg)
	DDX_Control(pDX, IDC_EDIT_OPERATIONNAME, m_objOperatinNameEdit);
	DDX_Control(pDX, IDC_KEYOPE_LIST, m_objKeyOpeList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKeyOpeDispDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyOpeDispDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyOpeDispDlg �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F������																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyOpeDispDlg::Initialize
	(	COperationManager*	pobjOperationManager
	)
{
	m_pobjOperationManager = pobjOperationManager;
}

/////////////////////////////////////////////////////////////////////////////
// CKeyOpeDispDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�h�m�h�s�c�h�`�k�n�f����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CKeyOpeDispDlg::OnInitDialog() 
{
	long	lCnt=0;
	COperation* pobjOperation = NULL;
	char	szNo[16];
	char	szBuf[256];

	CDialog::OnInitDialog();

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//���O���̃t�H���g�ݒ�
	m_objOperatinNameEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//���O���G�f�B�b�g������
	CString objOperationName = _T("");
	m_pobjOperationManager->GetOperationName(objOperationName);
	m_objOperatinNameEdit.SetWindowText(objOperationName);
	UpdateData(FALSE);

	//�L�[����t�@�C����������
	CString objKeyOpeFname = _T("");
	m_pobjOperationManager->GetOperationFileName(objKeyOpeFname);
	if(objKeyOpeFname.IsEmpty())
		objKeyOpeFname = "unsaved";
	m_wndStaticFileTitle.SubclassDlgItem(IDC_KEYOPE_FNAME, this);
	m_wndStaticFileTitle.SetPath(TRUE);
	SetDlgItemText(IDC_KEYOPE_FNAME, objKeyOpeFname);

	//���X�g�w�b�_�ݒ�
	m_objKeyOpeList.InsertColumn(0, "No.", LVCFMT_LEFT, 36);
	m_objKeyOpeList.InsertColumn(1, "Kind", LVCFMT_LEFT, 512-32);

	//���|�[�g�r���[
	m_objKeyOpeList.ModifyStyle( LVS_TYPEMASK, LVS_REPORT);

	if(m_pobjOperationManager != NULL)
	{
		//�I�y���[�V������������
		m_pobjOperationManager->SearchStartOperation();
		for(lCnt=0;;lCnt++)
		{
			pobjOperation = m_pobjOperationManager->SearchGetOperation();
			if(pobjOperation == NULL)
				break;

			sprintf(szNo,"%02d",(lCnt+1));
			m_objKeyOpeList.InsertItem(lCnt,szNo);

			//�I�y���[�V�������e�����e�L�X�g�擾
			memset(szBuf,0,sizeof(szBuf));
			pobjOperation->GetOperationString(szBuf);

			//���X�g�֕\��
			m_objKeyOpeList.SetItemText(lCnt,1,szBuf);
		}
		m_pobjOperationManager->SearchEndOperation();
	}

	//�I�[�g���T�C�Y
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_EDIT_OPERATIONNAME,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_KEYOPE_FNAME,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_KEYOPE_LIST,CP_RESIZE_HORIZONTAL|CP_RESIZE_VERTICAL);
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
void CKeyOpeDispDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	//�I�[�g���T�C�Y�N��
	m_objAutoResizer.MoveControls();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�n�j�v�{�^������������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CKeyOpeDispDlg::OnOK() 
{
	//�R���g���[���|�������o�ϐ��f�[�^��荞��
	UpdateData(TRUE);

	//���O���擾
	CString objOperationName=_T("");
	m_objOperatinNameEdit.GetWindowText(objOperationName);
	m_pobjOperationManager->SetOperationName(objOperationName);

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
// CKeyOpeDispDlg �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�ʒu���[�h����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CKeyOpeDispDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CKeyOpeDispDlg","WindowPos");
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
void CKeyOpeDispDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CKeyOpeDispDlg","WindowPos",strBuffer);
}


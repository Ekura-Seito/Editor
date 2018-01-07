// FileMoveDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "StaticFilespec.h"
#include "AutoResizer.h"
#include "FileMoveDlg.h"
#include "DirDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileMoveDlg �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFileMoveDlg::CFileMoveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileMoveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileMoveDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	memset(m_szFromDir,0,sizeof(m_szFromDir));
	memset(m_szFromName,0,sizeof(m_szFromName));
	memset(m_szFromPath,0,sizeof(m_szFromPath));
	m_objFolder = _T("");
	memset(m_szToPath,0,sizeof(m_szToPath));
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
void CFileMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileMoveDlg)
	DDX_Control(pDX, IDC_EDIT_FOLDER, m_objEditFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileMoveDlg, CDialog)
	//{{AFX_MSG_MAP(CFileMoveDlg)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileMoveDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileMoveDlg::OnInitDialog() 
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

	//�t�H���g�ݒ�
	m_objEditFolder.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//�t�@�C�����ƃf�B���N�g���擾
	strcpy(m_szFromDir,m_szFromPath);
	long lPos=0;
	long lCnt=0;
	for(lCnt=0; lCnt<(signed)strlen(m_szFromDir); )
	{
		if(IsDBCSLeadByteEx(0,m_szFromDir[lCnt]))//�Q�޲ĕ����̑���޲�
		{
			lCnt+=2;
		}
		else
		{
			if(m_szFromDir[lCnt]=='\\')
			{
				//'\'�ʒu�ێ�
				lPos = lCnt;
			}
			lCnt++;
		}
	}
	strcpy(m_szFromName,&m_szFromDir[lPos+1]);
	m_szFromDir[lPos+1] = 0;

	//���t�@�C���p�X�\��
	m_wndStaticFileName.SubclassDlgItem(IDC_FILE, this);
	m_wndStaticFileName.SetPath(TRUE);
	CString objStr = m_szFromPath;
	SetDlgItemText(IDC_FILE, objStr);

	//�ړ���t�H���_�f�t�H���g�ݒ�
	m_objEditFolder.SetWindowText(m_objFolder);
	m_objEditFolder.UpdateData(TRUE);

	//�c�[���q���g
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	objMsg.LoadString(IDS_BROWSE);
	m_tooltip.AddTool(GetDlgItem(IDC_BROWSE), objMsg);

	//�I�[�g���T�C�Y
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_FILE,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_EDIT_FOLDER,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_BROWSE,CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDOK,CP_MOVE_VERTICAL|CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDCANCEL,CP_MOVE_VERTICAL|CP_MOVE_HORIZONTAL);

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
void CFileMoveDlg::OnSize(UINT nType, int cx, int cy) 
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
BOOL CFileMoveDlg::PreTranslateMessage(MSG* pMsg)
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
void CFileMoveDlg::OnBrowse() 
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
void CFileMoveDlg::OnOK() 
{
	CString objMsg;
	//�R���g���[���|�������o�ϐ��f�[�^��荞��
	UpdateData(TRUE);

	//�f�[�^�擾
	m_objEditFolder.GetWindowText(m_objFolder);

	//�u�����N�`�F�b�N
	if(m_objFolder.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_FOLDER);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	//�f�B���N�g���Ó����`�F�b�N
	CFileFind		objFileFind;
	BOOL bRet = objFileFind.FindFile(m_objFolder);
	if(!bRet)//�Ȃ��I�I
	{
		if(m_pApp->IsDriveRoot(m_objFolder) == FALSE) //���[�g�f�B���N�g���ł��Ȃ�
		{
			objMsg.LoadString(IDS_CANTACCESS_FOLDERMV);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}
	else //����
	{
		objFileFind.FindNextFile();
		if(objFileFind.IsDirectory() == FALSE) //�f�B���N�g���ł͂Ȃ�
		{
			objMsg.LoadString(IDS_INPUT_EXISTFOLDER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	//�ړ���̃t�@�C���p�X��ҏW
	if(m_pApp->IsDriveRoot(m_objFolder) == FALSE) //���[�g�f�B���N�g���ł͂Ȃ�
		sprintf(m_szToPath,"%s\\%s",(LPCTSTR)m_objFolder, m_szFromName);
	else
		sprintf(m_szToPath,"%s%s",(LPCTSTR)m_objFolder, m_szFromName);

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
// CFileMoveDlg �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�ʒu���[�h����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileMoveDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CFileMoveDlg","WindowPos");
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
void CFileMoveDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CFileMoveDlg","WindowPos",strBuffer);
}


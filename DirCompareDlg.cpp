// DirCompareDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "Editor_Def.h"
#include "AutoResizer.h"
#include "MyComboBoxEx.h"
#include "DirCompareDlg.h"
#include "DirDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirCompareDlg �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDirCompareDlg::CDirCompareDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDirCompareDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDirCompareDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_objFolder1 = _T("");
	m_objFolder2 = _T("");
	for(int nCnt=0; nCnt<20; nCnt++)
	{
		m_objPastFolder1[nCnt] = _T("");
		m_objPastFolder2[nCnt] = _T("");
	}
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
void CDirCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDirCompareDlg)
	DDX_Control(pDX, IDC_COMBO_FOLDER1, m_objComboFolder1);
	DDX_CBString(pDX, IDC_COMBO_FOLDER1, m_objFolder1);
	DDX_Control(pDX, IDC_COMBO_FOLDER2, m_objComboFolder2);
	DDX_CBString(pDX, IDC_COMBO_FOLDER2, m_objFolder2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDirCompareDlg, CDialog)
	//{{AFX_MSG_MAP(CDirCompareDlg)
	ON_BN_CLICKED(IDC_BROWSE1, OnBrowse1)
	ON_BN_CLICKED(IDC_BROWSE2, OnBrowse2)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirCompareDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDirCompareDlg::OnInitDialog() 
{
	CString	objStr,objMsg;
	char	szKey[32];
	int nIndex=0;
	int nCnt=0,nCnt2=0;
	BOOL bFind=FALSE;

	CDialog::OnInitDialog();

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

//	//�t�H���g�ݒ�
//	m_objComboFolder1.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
//	m_objComboFolder2.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//�f�B���N�g���R���{�{�b�N�X�啶�����������ꎋ�ݒ�
	m_objComboFolder1.SetFlagCompareNoCase(TRUE);
	m_objComboFolder2.SetFlagCompareNoCase(TRUE);

	//���f�B���N�g���R���{�{�b�N�X�։ߋ��f�[�^�ݒ�
	objStr.Empty();
	memset(szKey,0,sizeof(szKey));
	nIndex=0;
	nCnt=0;nCnt2=0;
	bFind=FALSE;
	if(!m_objFolder1.IsEmpty())
	{
		m_objFolder1.MakeLower();
		m_objPastFolder1[nIndex] = m_objFolder1;
		m_objComboFolder1.AddString(m_objFolder1);
		nIndex++;
	}
	for(nCnt=0; nCnt<20; nCnt++)
	{
		if(nIndex >= 20)
			break;
		sprintf(szKey,"Folder1_%d",nCnt);
		objStr = m_pApp->GetProfileString("CDirCompareDlg",szKey,"");
		if( !objStr.IsEmpty() )
		{
			for(bFind=FALSE,nCnt2=0; nCnt2<nIndex; nCnt2++)
			{
				if(m_objPastFolder1[nCnt2].CompareNoCase(objStr) == 0)
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind == FALSE)
			{
				objStr.MakeLower();
				m_objPastFolder1[nIndex] = objStr;
				m_objComboFolder1.AddString(objStr);
				nIndex++;
			}
		}
	}
	m_objComboFolder1.SetCurSel(0);

	//���f�B���N�g���R���{�{�b�N�X�։ߋ��f�[�^�ݒ�
	objStr.Empty();
	memset(szKey,0,sizeof(szKey));
	nIndex=0;
	nCnt=0;nCnt2=0;
	bFind=FALSE;
	if(!m_objFolder2.IsEmpty())
	{
		m_objFolder2.MakeLower();
		m_objPastFolder2[nIndex] = m_objFolder2;
		m_objComboFolder2.AddString(m_objFolder2);
		nIndex++;
	}
	for(nCnt=0; nCnt<20; nCnt++)
	{
		if(nIndex >= 20)
			break;
		sprintf(szKey,"Folder2_%d",nCnt);
		objStr = m_pApp->GetProfileString("CDirCompareDlg",szKey,"");
		if( !objStr.IsEmpty() )
		{
			for(bFind=FALSE,nCnt2=0; nCnt2<nIndex; nCnt2++)
			{
				if(m_objPastFolder2[nCnt2].CompareNoCase(objStr) == 0)
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind == FALSE)
			{
				objStr.MakeLower();
				m_objPastFolder2[nIndex] = objStr;
				m_objComboFolder2.AddString(objStr);
				nIndex++;
			}
		}
	}
	m_objComboFolder2.SetCurSel(0);

	//�c�[���q���g
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	objMsg.LoadString(IDS_BROWSE);
	m_tooltip.AddTool(GetDlgItem(IDC_BROWSE1), objMsg);
	m_tooltip.AddTool(GetDlgItem(IDC_BROWSE2), objMsg);

	//�I�[�g���T�C�Y
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_COMBO_FOLDER1,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_COMBO_FOLDER2,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_BROWSE1,CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_BROWSE2,CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDOK,CP_MOVE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDCANCEL,CP_MOVE_HORIZONTAL);

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
void CDirCompareDlg::OnSize(UINT nType, int cx, int cy) 
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
BOOL CDirCompareDlg::PreTranslateMessage(MSG* pMsg)
{
	m_tooltip.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���f�B���N�g���p�t�H���_�I���{�^������������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareDlg::OnBrowse1() 
{
	CDirDialog		objDirDialog;
	objDirDialog.m_strTitle = "  ";
	if( objDirDialog.DoBrowse() != FALSE )
	{
		m_objFolder1 = objDirDialog.m_strPath;
		m_objFolder1.MakeLower();
//		m_objComboFolder1.UpdateData(TRUE);

		//�����o�ϐ��|���R���g���[��
		UpdateData(FALSE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���f�B���N�g���p�t�H���_�I���{�^������������						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareDlg::OnBrowse2() 
{
	CDirDialog		objDirDialog;
	objDirDialog.m_strTitle = "  ";
	if( objDirDialog.DoBrowse() != FALSE )
	{
		m_objFolder2 = objDirDialog.m_strPath;
		m_objFolder2.MakeLower();
//		m_objComboFolder2.UpdateData(TRUE);

		//�����o�ϐ��|���R���g���[��
		UpdateData(FALSE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�n�j�{�^������������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDirCompareDlg::OnOK() 
{
	char	szKey[32];
	int nIndex=0;
	int nCnt=0,nCnt2=0;
	BOOL bFind=FALSE;
	CString objMsg;

	//�R���g���[���|�������o�ϐ��f�[�^��荞��
	UpdateData(TRUE);

	//�u�����N�`�F�b�N
	if(m_objFolder1.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT1STDIR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if(m_objFolder2.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT2NDDIR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//����f�B���N�g���`�F�b�N
	if(m_objFolder1.CompareNoCase(m_objFolder2) == 0)
	{
		objMsg.LoadString(IDS_INPUTDIFFDIR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//���f�B���N�g�������f�[�^�X�V
	memset(szKey,0,sizeof(szKey));
	m_objFolder1.MakeLower();
	m_pApp->WriteProfileString("CDirCompareDlg","Folder1_0",m_objFolder1);
	nIndex=0;
	nCnt=0;nCnt2=0;
	bFind=FALSE;
	for(nIndex=0; nIndex<20; nIndex++)
	{
		sprintf(szKey,"Folder1_%d",nCnt+1);
		if( !m_objPastFolder1[nIndex].IsEmpty() &&
			m_objFolder1 != m_objPastFolder1[nIndex]
		)
		{
			for(bFind=FALSE,nCnt2=0; nCnt2<nIndex; nCnt2++)
			{
				if(m_objPastFolder1[nCnt2].Compare(m_objPastFolder1[nIndex]) == 0)
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind==FALSE)
			{
				m_objPastFolder1[nIndex].MakeLower();
				m_pApp->WriteProfileString("CDirCompareDlg",szKey,m_objPastFolder1[nIndex]);
				nCnt++;
			}
		}
	}

	//���f�B���N�g�������f�[�^�X�V
	memset(szKey,0,sizeof(szKey));
	m_objFolder2.MakeLower();
	m_pApp->WriteProfileString("CDirCompareDlg","Folder2_0",m_objFolder2);
	nIndex=0;
	nCnt=0;nCnt2=0;
	bFind=FALSE;
	for(nIndex=0; nIndex<20; nIndex++)
	{
		sprintf(szKey,"Folder2_%d",nCnt+1);
		if( !m_objPastFolder2[nIndex].IsEmpty() &&
			m_objFolder2 != m_objPastFolder2[nIndex]
		)
		{
			for(bFind=FALSE,nCnt2=0; nCnt2<nIndex; nCnt2++)
			{
				if(m_objPastFolder2[nCnt2].Compare(m_objPastFolder2[nIndex]) == 0)
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind==FALSE)
			{
				m_objPastFolder2[nIndex].MakeLower();
				m_pApp->WriteProfileString("CDirCompareDlg",szKey,m_objPastFolder2[nIndex]);
				nCnt++;
			}
		}
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
// CDirCompareDlg �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�ʒu���[�h����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDirCompareDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CDirCompareDlg","WindowPos");
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
void CDirCompareDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CDirCompareDlg","WindowPos",strBuffer);
}


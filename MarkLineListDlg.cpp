// MarkLineListDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include "editor.h"
#include "Editor_Def.h"
#include "Line.h"
#include "LineManager.h"
#include "UndoItem.h"
#include "UndoManager.h"
#include "ColorManager.h"
#include "EditorDoc.h"
#include "BCMenu.h"
#include "EditorView.h"
#include "AutoResizer.h"
#include "MarkLineListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarkLineListDlg �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMarkLineListDlg::CMarkLineListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMarkLineListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarkLineListDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_lSelectedLineNo = 0;
	m_pobjEditorView = NULL;
	m_pobjEditorDoc = NULL;
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
void CMarkLineListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarkLineListDlg)
	DDX_Control(pDX, IDC_LIST_MARKLINE, m_objMarkLineList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarkLineListDlg, CDialog)
	//{{AFX_MSG_MAP(CMarkLineListDlg)
	ON_LBN_DBLCLK(IDC_LIST_MARKLINE, OnDblclkListMarkline)
	ON_BN_CLICKED(IDC_JUMPBTN, OnJumpbtn)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarkLineListDlg �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����������															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMarkLineListDlg::Init
	(	CEditorView*	pobjEditorView,
		CEditorDoc*		pobjEditorDoc
	)
{
	//�����o�ϐ��֕ێ�
	m_pobjEditorView = pobjEditorView;
	m_pobjEditorDoc = pobjEditorDoc;
}

/////////////////////////////////////////////////////////////////////////////
// CMarkLineListDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O����������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMarkLineListDlg::OnInitDialog() 
{
	long	lCnt=0;
	CLine*	pobjLine=NULL;
	char	szBuf[MAX_BUF];

	CDialog::OnInitDialog();

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//�t�H���g�ݒ�
	m_objMarkLineList.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	m_lSelectedLineNo = 0;
	for(lCnt=0; lCnt<m_pobjEditorDoc->GetLineCount(); lCnt++)
	{
		pobjLine = m_pobjEditorDoc->GetOneLine(lCnt);
		if(pobjLine!=NULL)
		{
			if(pobjLine->GetFlagMark())
			{
				sprintf(szBuf,"%06d: %s",(lCnt+1),pobjLine->String());
				for(long i=0; i<(signed)strlen(szBuf); i++)
				{
					if(!IsDBCSLeadByteEx(0,szBuf[i]) && szBuf[i]==VK_TAB)
						szBuf[i] = ' ';
				}
				m_objMarkLineList.AddString(szBuf);
			}
		}
	}

	//�I�[�g���T�C�Y
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_LIST_MARKLINE,CP_RESIZE_HORIZONTAL|CP_RESIZE_VERTICAL);
	m_objAutoResizer.AddControl(IDC_JUMPBTN,CP_MOVE_HORIZONTAL|CP_MOVE_VERTICAL);
	m_objAutoResizer.AddControl(IDCANCEL,CP_MOVE_HORIZONTAL|CP_MOVE_VERTICAL);

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
void CMarkLineListDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	//�I�[�g���T�C�Y�N��
	m_objAutoResizer.MoveControls();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�[�N�s�ꗗ�_�u���N���b�N������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMarkLineListDlg::OnDblclkListMarkline() 
{
	//�R���g���[���|�������o�ϐ��f�[�^��荞��
	UpdateData(TRUE);

	int nIndex = m_objMarkLineList.GetCurSel();
	if(nIndex == LB_ERR || nIndex < 0)
		return;

	//�I�����ꂽ������擾
	char	szBuf[MAX_BUF];
	memset(szBuf,0,sizeof(szBuf));
	m_objMarkLineList.GetText(nIndex,szBuf);

	//�s�ԍ��擾
	char	szBuf2[32];
	memset(szBuf2,0,sizeof(szBuf2));
	for(long lCnt=0;lCnt<sizeof(szBuf2);lCnt++)
	{
		if(szBuf[lCnt]==':')
			break;
		szBuf2[lCnt] = szBuf[lCnt];
	}
	m_lSelectedLineNo = atol(szBuf2);

	//�E�C���h�E�̏�Ԃƈʒu���L��������
	WINDOWPLACEMENT		wp;
	if(GetWindowPlacement(&wp))
	{
		if(IsZoomed())
			wp.flags |= WPF_RESTORETOMAXIMIZED;
		SaveWindowPlacement(&wp);
	}

	//�_�C�A���O�I��
	EndDialog(IDOK);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�W�����v�v�{�^������������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMarkLineListDlg::OnJumpbtn() 
{
	CString objMsg;

	//�R���g���[���|�������o�ϐ��f�[�^��荞��
	UpdateData(TRUE);

	int nIndex = m_objMarkLineList.GetCurSel();
	if(nIndex == LB_ERR || nIndex < 0)
	{
		objMsg.LoadString(IDS_NOTSEL_MARKLINE);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//�I�����ꂽ������擾
	char	szBuf[MAX_BUF];
	memset(szBuf,0,sizeof(szBuf));
	m_objMarkLineList.GetText(nIndex,szBuf);

	//�s�ԍ��擾
	char	szBuf2[32];
	memset(szBuf2,0,sizeof(szBuf2));
	for(long lCnt=0;lCnt<sizeof(szBuf2);lCnt++)
	{
		if(szBuf[lCnt]==':')
			break;
		szBuf2[lCnt] = szBuf[lCnt];
	}
	m_lSelectedLineNo = atol(szBuf2);

	//�E�C���h�E�̏�Ԃƈʒu���L��������
	WINDOWPLACEMENT		wp;
	if(GetWindowPlacement(&wp))
	{
		if(IsZoomed())
			wp.flags |= WPF_RESTORETOMAXIMIZED;
		SaveWindowPlacement(&wp);
	}

	//�_�C�A���O�I��
	EndDialog(IDOK);
}

/////////////////////////////////////////////////////////////////////////////
// CMarkLineListDlg �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�ʒu���[�h����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMarkLineListDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CMarkLineListDlg","WindowPos");
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
void CMarkLineListDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CMarkLineListDlg","WindowPos",strBuffer);
}


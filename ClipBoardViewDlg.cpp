// ClipBoardViewDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <afxole.h>				//for COleDataSource, COleDropTarget
#pragma warning(disable:4786)
#include "editor.h"
#include "Editor_Def.h"
#include "AutoResizer.h"
#include "ClipBoardViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CClipBoardViewDlg::CClipBoardViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClipBoardViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClipBoardViewDlg)
	//}}AFX_DATA_INIT

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
void CClipBoardViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClipBoardViewDlg)
	DDX_Control(pDX, IDC_EDIT_CLIP_CONTENTS, m_objClipContentsEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClipBoardViewDlg, CDialog)
	//{{AFX_MSG_MAP(CClipBoardViewDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClipBoardViewDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\������������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CClipBoardViewDlg::OnInitDialog() 
{
	//���N���X�̃n���h�����R�[��
	CDialog::OnInitDialog();

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//�t�H���g�ݒ�
	m_objClipContentsEdit.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//�N���b�v�{�[�h����ǂ�
	CString	objClipContents = _T("");
	ReadFromClipBoard(objClipContents);

	//�_�C�A���O�֕\��
	m_objClipContentsEdit.SetWindowText(objClipContents);

	//�I�[�g���T�C�Y
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_EDIT_CLIP_CONTENTS,CP_RESIZE_HORIZONTAL|CP_RESIZE_VERTICAL);
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
void CClipBoardViewDlg::OnSize(UINT nType, int cx, int cy) 
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
void CClipBoardViewDlg::OnOK() 
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
// CClipBoardViewDlg �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�N���b�v�{�[�h����ǂޏ���											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CClipBoardViewDlg::ReadFromClipBoard
	(	CString&	objStr
	)
{
	long lCnt=0;
	int  nRet=0;
	long lCRCount=0;
	long lLFCount=0;
	long lSizeSum=0;

	//�n�k�d�f�[�^�I�u�W�F�N�g�쐬
	COleDataObject	objOleDataObject;
	//�N���b�v�{�[�h��̃f�[�^�I�u�W�F�N�g�Ɍ��ѕt���܂��B
	objOleDataObject.AttachClipboard();
	CFile* pobjFile = objOleDataObject.GetFileData(CF_TEXT);

	//�N���b�v�{�[�h���f�[�^�ǂݏo��
	CArchive ar(pobjFile, CArchive::load);
	m_pApp->PrepareMyReadString();
	for(;;)//�P�s�Âǂݍ���
	{
		lCRCount=0;
		lLFCount=0;
		CString objStrBuf = _T("");
		nRet = m_pApp->MyReadString(ar,objStrBuf,&lCRCount,&lLFCount);
		if( nRet == 0 ) //EOF�I��
		{
			if( objStrBuf.IsEmpty() )
				break;
		}

		if((lSizeSum+objStrBuf.GetLength()) >= 64*1020)
		{
			CString objMsg;
			objMsg.LoadString(IDS_NOTALLDISP_CLIP);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
			break;
		}
		else
		{
			lSizeSum += objStrBuf.GetLength();
		}

		//�A��
		objStr += objStrBuf;
		for(lCnt=0; lCnt<lCRCount; lCnt++)
			objStr += "\r";
		for(lCnt=0; lCnt<lLFCount; lCnt++)
			objStr += "\n";
	}

	//�N���b�v�{�[�h�N���[�Y
	ar.Abort();
	pobjFile->Close();
	delete pobjFile;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�ʒu���[�h����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CClipBoardViewDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CClipBoardViewDlg","WindowPos");
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
void CClipBoardViewDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CClipBoardViewDlg","WindowPos",strBuffer);
}


// ChildFrm.cpp : CChildFrame �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include <io.h>				// for _access
#include "Editor.h"
#include "Editor_Def.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CChildFrame::CChildFrame()
{
	m_bMaximizeWindow = 0;
	m_pApp = (CEditorApp*)AfxGetApp();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CChildFrame::~CChildFrame()
{
//	//�ݒ�ۑ�
//	m_pApp->WriteProfileInt("CChildFrame","bMaximizeWindow",m_bMaximizeWindow);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FOnCreate()���s���Ƀt���[�����[�N����Ăяo����鏈��					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CChildFrame::OnCreateClient( LPCREATESTRUCT /*lpcs*/,	CCreateContext* pContext)
{
	//�X�v���b�^�E�C���h�E�쐬
	return m_wndSplitter.Create( this,
		2, 2,                 // TODO: �s�Ɨ�̐��𒲐����Ă��������B
		CSize( 10, 10 ),      // TODO: �ŏ��̋��T�C�Y�𒲐����Ă��������B
		pContext );
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�쐬�O�ɌĂяo����鏈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs �̐ݒ���s���āAWindow �N���X�܂���
	//       �X�^�C����ύX���Ă��������B

	//�ݒ�擾
	m_bMaximizeWindow = m_pApp->GetProfileInt("CChildFrame","bMaximizeWindow",0);
	if( m_bMaximizeWindow != 0 )
	{
		//�l�c�h�̎q�E�B���h�E���͂��߂���ő剻���ĕ\��
		cs.style |= (WS_MAXIMIZE | WS_VISIBLE);
	}
	else
	{
		cs.style |= WS_VISIBLE;
		cs.style &= ~(WS_MAXIMIZE);
	}
	return CMDIChildWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame �N���X�̐f�f

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame �N���X�̃��b�Z�[�W�n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���b�Z�[�W�t�B���^����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CChildFrame::PreTranslateMessage(MSG* pMsg) 
{
	return CMDIChildWnd::PreTranslateMessage(pMsg);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�V�X�e���R�}���h�I��������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CChildFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	UINT nItemID = (nID & 0xFFF0);
	CString strPath;

	switch (nItemID)
	{
	case SC_MAXIMIZE:
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowMaximize",0) != 0)
		{
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowMaximize",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
		break;
	case SC_MINIMIZE:
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowMinimize",0) != 0)
		{
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowMinimize",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
		break;
	case SC_RESTORE:
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowRestore",0) != 0)
		{
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowRestore",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
		break;
	case SC_NEXTWINDOW:
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowNext",0) != 0)
		{
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowNext",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
		break;
	case SC_PREVWINDOW:
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowPrev",0) != 0)
		{
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowPrev",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
		break;
	case SC_CLOSE:
		if(m_pApp->GetProfileInt("Sound","IsPlayOnWindowClose",0) != 0)
		{
			strPath.Format("%s\\%s",m_pApp->m_strMediaDirPath,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowClose",""));
			if(_access((LPCTSTR)strPath,0) != -1)
				m_pApp->MyPlaySound((LPCTSTR)strPath,FALSE);
		}
		break;
	default:
		break;
	}

	CMDIChildWnd::OnSysCommand(nID, lParam);
}


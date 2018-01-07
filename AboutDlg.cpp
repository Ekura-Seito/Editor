// AboutDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "Editor.h"
#include "Hyperlink.h"
#include "Label.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_brush.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	m_hIcon = m_pApp->LoadIcon(IDR_MAINFRAME);
	m_hIconLaugh = m_pApp->LoadIcon(IDR_LAUGHICON);
	DWORD dwErr = GetLastError();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_APPICON, m_objAppIcon);
	DDX_Control(pDX, IDC_HYPERLINK, m_hyperlink);
	DDX_Control(pDX, IDC_SOFTNAME, m_objSoftName);
	DDX_Control(pDX, IDC_CREDIT, m_objCredit);
	DDX_Control(pDX, IDC_KAISYA, m_Kaisya);
	DDX_Control(pDX, IDC_USER, m_User);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�o�[�W�������_�C�A���O����������									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CAboutDlg::OnInitDialog()
{
	CString objMsg;
	objMsg.LoadString(IDS_FONTNAME_ABOUTDLG);

	CDialog::OnInitDialog();

	//�\�t�g����
	m_objSoftName.SetTextColor(GetSysColor(COLOR_BTNTEXT));
	m_objSoftName.SetBkColor(GetSysColor(COLOR_BTNFACE));
	m_objSoftName.SetFontBold(TRUE);
	m_objSoftName.SetFontName(objMsg);
	m_objSoftName.SetFontSize(18);
	m_objSoftName.SetFontItalic(TRUE);

	//�N���W�b�g���
	m_objCredit.SetTextColor(GetSysColor(COLOR_BTNTEXT));
	m_objCredit.SetBkColor(GetSysColor(COLOR_BTNFACE));
	m_objCredit.SetFontName(objMsg);
	m_objCredit.SetFontSize(12);

	//�������
	m_Kaisya.SetText(m_objKaisya);
	m_Kaisya.SetTextColor(GetSysColor(COLOR_BTNTEXT));
	m_Kaisya.SetBkColor(GetSysColor(COLOR_BTNFACE));
	m_Kaisya.SetFontName(objMsg);
	m_Kaisya.SetFontSize(12);

	//���[�U���
	m_User.SetText(m_objUser);
	m_User.SetTextColor(GetSysColor(COLOR_BTNTEXT));
	m_User.SetBkColor(GetSysColor(COLOR_BTNFACE));
	m_User.SetFontName(objMsg);
	m_User.SetFontSize(12);

	//�n�C�p�[�����N�R���g���[��������
	m_hyperlink.SetURL("http://www.asahi-net.or.jp/~ga5s-ekr/index.htm");
	DWORD dwRemove=0L, dwAdd=0L;
	dwAdd = CHyperLink::StyleUnderline;
	m_hyperlink.ModifyLinkStyle(dwRemove, dwAdd);
	dwAdd = CHyperLink::StyleUseHover;
	m_hyperlink.ModifyLinkStyle(dwRemove, dwAdd);

	//�����o�ϐ��|���R���g���[��
	UpdateData(FALSE);
	ShowWindow(TRUE);       // SHOW_OPENWINDOW
	UpdateWindow();

	//wav file play
	m_pApp->MyPlaySound(IDSOUND_ABOUTDLG,FALSE);

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�b�s�k�b�n�k�n�q����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if(GetDlgItem(IDC_HYPERLINK) == pWnd)
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	else if(GetDlgItem(IDC_APPICON) == pWnd)
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	else if(GetDlgItem(IDC_SOFTNAME) == pWnd)
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	else if(GetDlgItem(IDC_CREDIT) == pWnd)
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	else if(GetDlgItem(IDC_KAISYA) == pWnd)
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	else if(GetDlgItem(IDC_USER) == pWnd)
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	else
		return (HBRUSH)m_brush;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�l�n�t�r�d�l�n�u�d����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CAboutDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rect;
	m_objAppIcon.GetWindowRect(rect);
	ScreenToClient(rect);
	if(!rect.PtInRect(point))
	{
		ReleaseCapture();
		if(m_objAppIcon.GetIcon() != m_hIcon)
			m_objAppIcon.SetIcon(m_hIcon);
	}
	else
	{
		SetCapture();
		if(m_objAppIcon.GetIcon() != m_hIconLaugh)
			m_objAppIcon.SetIcon(m_hIconLaugh);
	}

	CDialog::OnMouseMove(nFlags, point);
}

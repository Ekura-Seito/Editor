// MailListDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <io.h>				//for _access
#pragma warning(disable:4786)
#include "editor_def.h"
#include "editor.h"
#include "ListCtrlEx.h"
#include "AutoResizer.h"
#include "MailListDlg.h"
#include "Pop.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMailListDlg �_�C�A���O

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMailListDlg::CMailListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMailListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMailListDlg)
	m_objPopServerStr = _T("");
	m_objPopUserStr = _T("");
	//}}AFX_DATA_INIT

	m_pPop = NULL;
	m_pApp = (CEditorApp*)AfxGetApp();
	m_nMailCount = 0;
	m_objPopPasswdStr = _T("");
	//�C���[�W���X�g�쐬
	m_AttachImageList.Create(IDB_MAILATTACH, 16, 1, RGB(255, 255, 255));
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
void CMailListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMailListDlg)
	DDX_Control(pDX, IDC_LIST_MAIL, m_objListMail);
	DDX_Text(pDX, IDC_POPSERVER, m_objPopServerStr);
	DDX_Text(pDX, IDC_POPUSER, m_objPopUserStr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMailListDlg, CDialog)
	//{{AFX_MSG_MAP(CMailListDlg)
	ON_BN_CLICKED(IDC_RECVBTN, OnRecvbtn)
	ON_BN_CLICKED(IDC_DELBTN, OnDelbtn)
	ON_BN_CLICKED(IDC_REFRESHBTN, OnRefreshbtn)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMailListDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMailListDlg::OnInitDialog() 
{
	//���N���X�̃n���h���R�[��
	CDialog::OnInitDialog();

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//�C���[�W���X�g�Z�b�g
	m_objListMail.SetImageList(&m_AttachImageList, LVSIL_SMALL);

	//���X�g�w�b�_
	m_objListMail.InsertColumn(0, "��", LVCFMT_LEFT, 45);
	m_objListMail.InsertColumn(1, "Subject", LVCFMT_LEFT, 180);
	m_objListMail.InsertColumn(2, "From", LVCFMT_LEFT, 160);
	m_objListMail.InsertColumn(3, "Date", LVCFMT_LEFT, 170);
	m_objListMail.InsertColumn(4, "Size", LVCFMT_RIGHT, 60);

	//���[�����X�g�쐬
	DispMailList();

	//���|�[�g�r���[
	m_objListMail.ModifyStyle( LVS_TYPEMASK, LVS_REPORT);

	//��M�ς݃��[���ۑ���f�B���N�g�����݃`�F�b�N
	if( _access(m_pApp->m_strRecvMailPath,0) == -1 )
		GetDlgItem(IDC_RECVBTN)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_RECVBTN)->EnableWindow(TRUE);

	//�I�[�g���T�C�Y
	CRect rect;
	GetClientRect(rect);
	m_objAutoResizer.SetParent(this,rect.Width(),rect.Height());
	m_objAutoResizer.AddControl(IDC_POPSERVER,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_POPUSER,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_MAILCOUNT,CP_RESIZE_HORIZONTAL);
	m_objAutoResizer.AddControl(IDC_LIST_MAIL,CP_RESIZE_HORIZONTAL|CP_RESIZE_VERTICAL);
	m_objAutoResizer.AddControl(IDC_RECVBTN,CP_MOVE_HORIZONTAL|CP_MOVE_VERTICAL);
	m_objAutoResizer.AddControl(IDC_DELBTN,CP_MOVE_HORIZONTAL|CP_MOVE_VERTICAL);
	m_objAutoResizer.AddControl(IDC_REFRESHBTN,CP_MOVE_HORIZONTAL|CP_MOVE_VERTICAL);
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
void CMailListDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	//�I�[�g���T�C�Y�N��
	m_objAutoResizer.MoveControls();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u���[����M�v�{�^������������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMailListDlg::OnRecvbtn() 
{
	int 	nCnt=0,nSelectCnt=0,nRecvMailCnt=0;
	LV_ITEM lvi;
	BOOL	bRet=FALSE;
	time_t	t_now=0;
	struct tm* ptm=NULL;
	char	szMailPath[_MAX_PATH],szAttachPath[_MAX_PATH],szBuf[1024];
	CString objMsg;

	//�R���g���[���|�������o�ϐ�
	UpdateData(TRUE);

	//���ݎ����擾
	t_now = time(NULL);
	ptm = localtime(&t_now);

	for(nCnt=0; nCnt<m_nMailCount; nCnt++)
	{
		//�I����ԃ`�F�b�N
		memset(&lvi,0,sizeof(LV_ITEM));
		lvi.mask = LVIF_STATE;
		lvi.iItem = nCnt;
		lvi.iSubItem = 0;
		lvi.stateMask = 0xFFFF;
		m_objListMail.GetItem(&lvi);
		if(lvi.state & LVIS_SELECTED)
		{
			//���[���ۑ���p�X�ҏW
			nSelectCnt++;
			sprintf(szMailPath,"%s\\Mail%04d%02d%02d_%02d%02d%02d_%d.txt",
				(LPCTSTR)m_pApp->m_strRecvMailPath,
				ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,
				ptm->tm_hour,ptm->tm_min,ptm->tm_sec,nSelectCnt);

			//�Y�t�t�@�C���ۑ���p�X�ҏW
			sprintf(szAttachPath,"%s\\%04d%02d%02d_%02d%02d%02d_%d",
				(LPCTSTR)m_pApp->m_strRecvMailAttachPath,
				ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,
				ptm->tm_hour,ptm->tm_min,ptm->tm_sec,nSelectCnt);

			//�P���[����M���I�[�v��
			bRet = m_pPop->RecvOneMail(nCnt+1,szMailPath,szAttachPath);
			if(bRet)
			{
				nRecvMailCnt++;
				if( (_access( szMailPath, 0 )) != -1 )
					m_pApp->OpenDocumentFile(szMailPath);
			}
			else
			{
				objMsg.LoadString(IDS_MAILRECVERR);
				sprintf(szBuf,(LPCTSTR)objMsg,nCnt+1);
				m_pApp->MyMessageBox(szBuf,(MB_OK|MB_ICONSTOP),0);
			}
		}
	}

	if(nSelectCnt == 0)
	{
		objMsg.LoadString(IDS_POP_SELECTLIST);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONEXCLAMATION),0);
	}
	else
	{
		//���[�����X�g�č쐬
		DispMailList();

		//��M�I���ʒm
		if(nRecvMailCnt>0)
		{
			objMsg.LoadString(IDS_MAILRECVEND);
			sprintf(szBuf,(LPCTSTR)objMsg,nRecvMailCnt);
			m_pApp->MyMessageBox(szBuf,(MB_OK|MB_ICONINFORMATION),0);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u���[���폜�v�{�^������������										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMailListDlg::OnDelbtn() 
{
	int 	nCnt=0,nSelectCnt=0;
	LV_ITEM lvi;
	BOOL	bRet=FALSE;
	CString objMsg;

	//�R���g���[���|�������o�ϐ�
	UpdateData(TRUE);

	for(nCnt=0; nCnt<m_nMailCount; nCnt++)
	{
		//�I����ԃ`�F�b�N
		memset(&lvi,0,sizeof(LV_ITEM));
		lvi.mask = LVIF_STATE;
		lvi.iItem = nCnt;
		lvi.iSubItem = 0;
		lvi.stateMask = 0xFFFF;
		m_objListMail.GetItem(&lvi);
		if(lvi.state & LVIS_SELECTED)
		{
			nSelectCnt++;
			//�P���[���폜
			bRet = m_pPop->DelOneMail(nCnt+1);
		}
	}

	if(nSelectCnt == 0)
	{
		objMsg.LoadString(IDS_POP_SELECTLIST);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONEXCLAMATION),0);
	}
	else
	{
		//���[�����X�g�č쐬
		DispMailList();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�\���X�V�{�^������������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMailListDlg::OnRefreshbtn() 
{
	CWaitCursor	objWaitCursor;

	char			szSection[64];
	int				nAccountNo = m_pApp->GetProfileInt("Mail","CurrentAccountNo",0);
	if(nAccountNo < 0 || nAccountNo > 8)
		nAccountNo = 0;
	sprintf(szSection,"Mail_Account%d",nAccountNo);

	int nAuthorizationType = m_pApp->GetProfileInt(szSection,"nAuthorizationType",POP3_AUTHENTICATE_STANDARD);

	m_pPop->Disconnect();
	BOOL bRet = m_pPop->Connect( m_objPopServerStr,
								 m_objPopUserStr,
								 m_objPopPasswdStr,
								 nAuthorizationType );
	if(bRet)
	{
		int nMailCount = m_pPop->GetMailCount();
		if(nMailCount >= 0)
			m_nMailCount = nMailCount;

		//���[�����X�g�č쐬
		DispMailList();
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�n�j�{�^������������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMailListDlg::OnOK() 
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
// CMailListDlg �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���[�����X�g�쐬														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMailListDlg::DispMailList()
{
	int	nCnt=0,nIndex=0,nMailCount=0,nSize=0;
	CString objStr, objMsg;
	char	szNumber[32],szSubject[1024],szFrom[1024],szDate[1024];
	BOOL	bRet=FALSE,bRet2=FALSE,bIsMultiPart=FALSE;
	LV_ITEM lvi;

	//���X�g���܂��S���N���A
	m_objListMail.DeleteAllItems();

	//���[�����X�g�쐬
	for(nIndex=0,nCnt=0; nCnt<m_nMailCount; nCnt++)
	{
		//�����A���o�l�A���t�����擾
		memset(szSubject,0,sizeof(szSubject));
		memset(szFrom,0,sizeof(szFrom));
		memset(szDate,0,sizeof(szDate));
		bRet  = m_pPop->GetMailInfo(nCnt+1,szSubject,szFrom,szDate,&bIsMultiPart);
		//�T�C�Y���擾
		bRet2 = m_pPop->GetMailSize(nCnt+1,&nSize);
		if(bRet || bRet2)
		{
			sprintf(szNumber,"%02d",nIndex+1);

			memset(&lvi,0,sizeof(LV_ITEM));
			lvi.mask = LVIF_TEXT | LVIF_IMAGE;
			lvi.iItem = nIndex;
			lvi.iSubItem = 0;
			lvi.pszText = szNumber;
			lvi.iImage = (bIsMultiPart ? 1 : 0);
			m_objListMail.InsertItem(&lvi);

			m_objListMail.SetItemText(nIndex,1,szSubject);
			m_objListMail.SetItemText(nIndex,2,szFrom);
			m_objListMail.SetItemText(nIndex,3,szDate);
			sprintf(szNumber,"%7d",nSize);
			m_objListMail.SetItemText(nIndex,4,szNumber);

			nIndex++;
		}
	}

	//���[������
	nMailCount = m_pPop->GetMailCount();
	if(nMailCount >= 0)
		m_nMailCount = nMailCount;
	objMsg.LoadString(IDS_MAILCOUNT);
	objStr.Format(objMsg, m_nMailCount );
	SetDlgItemText(IDC_MAILCOUNT, objStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�E�C���h�E�ʒu���[�h����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMailListDlg::LoadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
	CString strBuffer = m_pApp->GetProfileString("CMailListDlg","WindowPos");
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
void CMailListDlg::SaveWindowPlacement(LPWINDOWPLACEMENT pwp)
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
	m_pApp->WriteProfileString("CMailListDlg","WindowPos",strBuffer);
}


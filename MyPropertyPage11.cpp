// MyPropertyPage11.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <io.h>					// for _access
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "MyPropertyPage11.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage11 �v���p�e�B �y�[�W

IMPLEMENT_DYNCREATE(CMyPropertyPage11, CPropertyPage)

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage11::CMyPropertyPage11() : CPropertyPage(CMyPropertyPage11::IDD)
{
	//{{AFX_DATA_INIT(CMyPropertyPage11)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsChanged = FALSE;								//�ύX�����t���O
	memset(m_stSoundDef,0,sizeof(SOUND_DEF)*13);
	memset(m_hRootItems,0,sizeof(HTREEITEM)*4);
	//�C���[�W���X�g�쐬
	m_ImageList.Create(IDB_SOUNDICONS, 16, 1, RGB(255, 255, 255));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage11::~CMyPropertyPage11()
{
}

void CMyPropertyPage11::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage11)
	DDX_Control(pDX, IDC_COMBO_WAVEFILES, m_objWaveFilesCombo);
	DDX_Control(pDX, IDC_TREE_SOUNDS, m_objSoundsTreeCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPropertyPage11, CPropertyPage)
	//{{AFX_MSG_MAP(CMyPropertyPage11)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_CBN_SELCHANGE(IDC_COMBO_WAVEFILES, OnSelchangeComboWavefiles)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_SOUNDS, OnSelchangedTreeSounds)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage11 �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ύX������������														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage11::IsChanged() 
{
	return m_bIsChanged;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ύX�����t���O�N���A����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage11::ClearChangeFlag()
{
	m_bIsChanged = FALSE;
	SetModified(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ύX���e�����W�X�g���֔��f�����鏈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage11::ChangesToRegistry()
{
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnXerxesStart",         m_stSoundDef[0].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnXerxesStart",       m_stSoundDef[0].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnXerxesEnd",           m_stSoundDef[1].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnXerxesEnd",         m_stSoundDef[1].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnMessageInfo",         m_stSoundDef[2].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnMessageInfo",       m_stSoundDef[2].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnMessageStop",         m_stSoundDef[3].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnMessageStop",       m_stSoundDef[3].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnMessageQuestion",     m_stSoundDef[4].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnMessageQuestion",   m_stSoundDef[4].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnMessageExclamation",  m_stSoundDef[5].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnMessageExclamation",m_stSoundDef[5].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnWindowMinimize",      m_stSoundDef[6].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnWindowMinimize",    m_stSoundDef[6].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnWindowMaximize",      m_stSoundDef[7].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnWindowMaximize",    m_stSoundDef[7].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnWindowRestore",       m_stSoundDef[8].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnWindowRestore",     m_stSoundDef[8].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnWindowNext",          m_stSoundDef[9].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnWindowNext",        m_stSoundDef[9].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnWindowPrev",          m_stSoundDef[10].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnWindowPrev",        m_stSoundDef[10].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnWindowClose",         m_stSoundDef[11].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnWindowClose",       m_stSoundDef[11].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnWindowAllClose",      m_stSoundDef[12].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnWindowAllClose",    m_stSoundDef[12].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnFileNew",             m_stSoundDef[13].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnFileNew",           m_stSoundDef[13].szWaveFileName);
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage11 ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage11::OnInitDialog() 
{
	int nCnt=0;
	CString objFilter, *pobjStr=NULL;
	BOOL		bRet=FALSE;
	CFileFind	objFileFind;
	CString		objMsg;


	CPropertyPage::OnInitDialog();

	//�C���[�W���X�g�Z�b�g
	m_objSoundsTreeCtrl.SetImageList(&m_ImageList, TVSIL_NORMAL);

	//SOUND_DEF�\���̏�����
	m_stSoundDef[0].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnXerxesStart",0);
	strcpy(m_stSoundDef[0].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnXerxesStart",""));
	m_stSoundDef[1].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnXerxesEnd",0);
	strcpy(m_stSoundDef[1].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnXerxesEnd",""));
	m_stSoundDef[2].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnMessageInfo",0);
	strcpy(m_stSoundDef[2].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnMessageInfo",""));
	m_stSoundDef[3].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnMessageStop",0);
	strcpy(m_stSoundDef[3].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnMessageStop",""));
	m_stSoundDef[4].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnMessageQuestion",0);
	strcpy(m_stSoundDef[4].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnMessageQuestion",""));
	m_stSoundDef[5].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnMessageExclamation",0);
	strcpy(m_stSoundDef[5].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnMessageExclamation",""));
	m_stSoundDef[6].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnWindowMinimize",0);
	strcpy(m_stSoundDef[6].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowMinimize",""));
	m_stSoundDef[7].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnWindowMaximize",0);
	strcpy(m_stSoundDef[7].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowMaximize",""));
	m_stSoundDef[8].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnWindowRestore",0);
	strcpy(m_stSoundDef[8].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowRestore",""));
	m_stSoundDef[9].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnWindowNext",0);
	strcpy(m_stSoundDef[9].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowNext",""));
	m_stSoundDef[10].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnWindowPrev",0);
	strcpy(m_stSoundDef[10].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowPrev",""));
	m_stSoundDef[11].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnWindowClose",0);
	strcpy(m_stSoundDef[11].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowClose",""));
	m_stSoundDef[12].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnWindowAllClose",0);
	strcpy(m_stSoundDef[12].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowAllClose",""));
	m_stSoundDef[13].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnFileNew",0);
	strcpy(m_stSoundDef[13].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnFileNew",""));

	//���[�g�A�C�e���}��
	objMsg.LoadString(IDS_SOUND_XERXES_STARTEND);
	m_hRootItems[0] = m_objSoundsTreeCtrl.InsertItem(objMsg,2,3,TVI_ROOT,TVI_LAST);
	if(m_hRootItems[0] != NULL)
	{
		//�uXerxes�̋N���v�A�C�e���}��
		objMsg.LoadString(IDS_SOUND_XERXES_START);
		if(m_stSoundDef[0].bIsPlay != 0)
			m_stSoundDef[0].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[0],TVI_LAST);
		else
			m_stSoundDef[0].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[0],TVI_LAST);
		//�uXerxes�̏I���v�A�C�e���}��
		objMsg.LoadString(IDS_SOUND_XERXES_END);
		if(m_stSoundDef[1].bIsPlay != 0)
			m_stSoundDef[1].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[0],TVI_LAST);
		else
			m_stSoundDef[1].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[0],TVI_LAST);

		m_objSoundsTreeCtrl.Expand(m_hRootItems[0],TVE_EXPAND);
	}

	//���[�g�A�C�e���}��
	objMsg.LoadString(IDS_SOUND_MESSAGE);
	m_hRootItems[1] = m_objSoundsTreeCtrl.InsertItem(objMsg,2,3,TVI_ROOT,TVI_LAST);
	if(m_hRootItems[1] != NULL)
	{
		//�u���b�Z�[�W�i���j�v�A�C�e���}��
		objMsg.LoadString(IDS_SOUND_MESSAGE_INFO);
		if(m_stSoundDef[2].bIsPlay != 0)
			m_stSoundDef[2].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[1],TVI_LAST);
		else
			m_stSoundDef[2].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[1],TVI_LAST);
		//�u���b�Z�[�W�i�x���j�v�A�C�e���}��
		objMsg.LoadString(IDS_SOUND_MESSAGE_STOP);
		if(m_stSoundDef[3].bIsPlay != 0)
			m_stSoundDef[3].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[1],TVI_LAST);
		else
			m_stSoundDef[3].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[1],TVI_LAST);
		//�u���b�Z�[�W�i�₢���킹�j�v�A�C�e���}��
		objMsg.LoadString(IDS_SOUND_MESSAGE_QUESTION);
		if(m_stSoundDef[4].bIsPlay != 0)
			m_stSoundDef[4].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[1],TVI_LAST);
		else
			m_stSoundDef[4].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[1],TVI_LAST);
		//�u���b�Z�[�W�i���Ӂj�v�A�C�e���}��
		objMsg.LoadString(IDS_SOUND_MESSAGE_EXCLAIM);
		if(m_stSoundDef[5].bIsPlay != 0)
			m_stSoundDef[5].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[1],TVI_LAST);
		else
			m_stSoundDef[5].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[1],TVI_LAST);

		m_objSoundsTreeCtrl.Expand(m_hRootItems[1],TVE_EXPAND);
	}

	//���[�g�A�C�e���}��
	objMsg.LoadString(IDS_SOUND_WINDOW);
	m_hRootItems[2] = m_objSoundsTreeCtrl.InsertItem(objMsg,2,3,TVI_ROOT,TVI_LAST);
	if(m_hRootItems[2] != NULL)
	{
		//�u�E�C���h�E�i�ŏ����j�v�A�C�e���}��
		objMsg.LoadString(IDS_SOUND_WINDOW_MINIMIZE);
		if(m_stSoundDef[6].bIsPlay != 0)
			m_stSoundDef[6].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[2],TVI_LAST);
		else
			m_stSoundDef[6].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[2],TVI_LAST);
		//�u�E�C���h�E�i�ő剻�j�v�A�C�e���}��
		objMsg.LoadString(IDS_SOUND_WINDOW_MAXIMIZE);
		if(m_stSoundDef[7].bIsPlay != 0)
			m_stSoundDef[7].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[2],TVI_LAST);
		else
			m_stSoundDef[7].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[2],TVI_LAST);
		//�u�E�C���h�E�i���Ƃ̃T�C�Y�ɖ߂��j�v�A�C�e���}��
		objMsg.LoadString(IDS_SOUND_WINDOW_RESTORE);
		if(m_stSoundDef[8].bIsPlay != 0)
			m_stSoundDef[8].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[2],TVI_LAST);
		else
			m_stSoundDef[8].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[2],TVI_LAST);
		//�u�E�C���h�E�i���̃E�C���h�E�j�v�A�C�e���}��
		objMsg.LoadString(IDS_SOUND_WINDOW_NEXT);
		if(m_stSoundDef[9].bIsPlay != 0)
			m_stSoundDef[9].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[2],TVI_LAST);
		else
			m_stSoundDef[9].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[2],TVI_LAST);
		//�u�E�C���h�E�i�O�̃E�C���h�E�j�v�A�C�e���}��
		objMsg.LoadString(IDS_SOUND_WINDOW_PREV);
		if(m_stSoundDef[10].bIsPlay != 0)
			m_stSoundDef[10].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[2],TVI_LAST);
		else
			m_stSoundDef[10].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[2],TVI_LAST);
		//�u�E�C���h�E�i����j�v�A�C�e���}��
		objMsg.LoadString(IDS_SOUND_WINDOW_CLOSE);
		if(m_stSoundDef[11].bIsPlay != 0)
			m_stSoundDef[11].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[2],TVI_LAST);
		else
			m_stSoundDef[11].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[2],TVI_LAST);
		//�u�E�C���h�E�i���ׂĕ���j�v�A�C�e���}��
		objMsg.LoadString(IDS_SOUND_WINDOW_ALLCLOSE);
		if(m_stSoundDef[12].bIsPlay != 0)
			m_stSoundDef[12].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[2],TVI_LAST);
		else
			m_stSoundDef[12].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[2],TVI_LAST);

		m_objSoundsTreeCtrl.Expand(m_hRootItems[2],TVE_EXPAND);
	}

	//���[�g�A�C�e���}��
	objMsg.LoadString(IDS_SOUND_FILE);
	m_hRootItems[3] = m_objSoundsTreeCtrl.InsertItem(objMsg,2,3,TVI_ROOT,TVI_LAST);
	if(m_hRootItems[3] != NULL)
	{
		//�u�V�K�쐬�v�A�C�e���}��
		objMsg.LoadString(IDS_SOUND_FILE_NEW);
		if(m_stSoundDef[13].bIsPlay != 0)
			m_stSoundDef[13].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[3],TVI_LAST);
		else
			m_stSoundDef[13].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[3],TVI_LAST);

		m_objSoundsTreeCtrl.Expand(m_hRootItems[3],TVE_EXPAND);
	}
	if(m_hRootItems[0] != NULL)
		m_objSoundsTreeCtrl.Select(m_hRootItems[0],TVGN_FIRSTVISIBLE);

	//Wave File�R���{�{�b�N�X�ݒ�
	objMsg.LoadString(IDS_SOUND_NONE);
	m_objWaveFilesCombo.AddString(objMsg);
	objFilter.Format("%s%s",m_pApp->m_strMediaDirPath,"\\*.wav");
	CObArray objStringArray;
	objStringArray.SetSize(0);
	MakeFileList(objFilter,&objStringArray);
	for(nCnt=0; nCnt<objStringArray.GetSize(); nCnt++)
	{
		pobjStr = (CString*)objStringArray.GetAt(nCnt);
		m_objWaveFilesCombo.AddString((*pobjStr));
	}
	m_objWaveFilesCombo.SetCurSel(0);
	m_objWaveFilesCombo.EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(FALSE);

	//�����o�ϐ��|���R���g���[��
	UpdateData(FALSE);

	//�ύX�����t���O�N���A
	m_bIsChanged = FALSE;

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�T�E���h�c���[�I��ύX������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage11::OnSelchangedTreeSounds(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TV_ITEM item;
	char szBuf[256],szFileName[_MAX_FNAME];
	BOOL bRet=FALSE, bFind=FALSE;
	int	nIndex=0, nCnt=0;
	CString objMsg;

	m_bIsChanged = TRUE;
	SetModified();

	//���I���A�C�e�����擾
	memset(szBuf,0,sizeof(szBuf));
	memset(&item,0,sizeof(TV_ITEM));
	item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE;
	item.hItem = pNMTreeView->itemOld.hItem;
	item.pszText = szBuf;
	item.cchTextMax = 256;
	bRet = m_objSoundsTreeCtrl.GetItem(&item);
	if(!bRet)
	{
		*pResult = 0;
		return;
	}

	for(bFind=FALSE,nCnt=0; nCnt<4; nCnt++)
	{
		if(item.hItem == m_hRootItems[nCnt])
		{
			bFind=TRUE;
			break;
		}
	}
	if(bFind)
	{
		//���I���A�C�e�����m�[�h�ł������ꍇ�͉������Ȃ�
		;
	}
	else
	{
		//���ݑI�𒆂̂v�`�u�d�t�@�C�����擾���ē����\���̂֋L�^
		nIndex = m_objWaveFilesCombo.GetCurSel();
		if(nIndex == CB_ERR)
			return;
		else if(nIndex == 0) //�Ȃ�
		{
			//�A�C�e���C���[�W�ݒ�
			m_objSoundsTreeCtrl.SetItemImage(pNMTreeView->itemOld.hItem,0,0);

			//����炳�Ȃ��ɐݒ�
			for(nCnt=0; nCnt<14; nCnt++)
			{
				if(item.hItem == m_stSoundDef[nCnt].hItem)
				{
					m_stSoundDef[nCnt].bIsPlay = 0;
					memset(m_stSoundDef[nCnt].szWaveFileName,0,_MAX_FNAME);
					break;
				}
			}
		}
		else
		{
			//�A�C�e���C���[�W�ݒ�
			m_objSoundsTreeCtrl.SetItemImage(pNMTreeView->itemOld.hItem,1,1);

			memset(szFileName,0,sizeof(szFileName));
			m_objWaveFilesCombo.GetLBText(nIndex,szFileName);

			//�v�`�u�d�t�@�C�����ݒ�
			for(nCnt=0; nCnt<14; nCnt++)
			{
				if(item.hItem == m_stSoundDef[nCnt].hItem)
				{
					m_stSoundDef[nCnt].bIsPlay = 1;
					strcpy(m_stSoundDef[nCnt].szWaveFileName,szFileName);
					break;
				}
			}
		}
	}

	//�V�I���A�C�e�����擾
	memset(szBuf,0,sizeof(szBuf));
	memset(&item,0,sizeof(TV_ITEM));
	item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE;
	item.hItem = pNMTreeView->itemNew.hItem;
	item.pszText = szBuf;
	item.cchTextMax = 256;
	bRet = m_objSoundsTreeCtrl.GetItem(&item);
	if(!bRet)
	{
		*pResult = 0;
		return;
	}

	//�I������Ă���A�C�e�����m�[�h�̏ꍇ�͉������Ȃ�
	for(bFind=FALSE,nCnt=0; nCnt<4; nCnt++)
	{
		if(item.hItem == m_hRootItems[nCnt])
		{
			bFind=TRUE;
			break;
		}
	}
	if(bFind)
	{
		m_objWaveFilesCombo.SetCurSel(0);
		m_objWaveFilesCombo.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(FALSE);
		*pResult = 0;
		return;
	}
	else
	{
		//�I������Ă���A�C�e���ɑΉ�����v�`�u�d�t�@�C�����R���{�{�b�N�X��
		memset(szFileName,0,sizeof(szFileName));
		for(nCnt=0; nCnt<14; nCnt++)
		{
			if(item.hItem == m_stSoundDef[nCnt].hItem)
			{
				strcpy(szFileName,m_stSoundDef[nCnt].szWaveFileName);
				break;
			}
		}

		if(strlen(szFileName) != 0)
		{
			m_objWaveFilesCombo.SelectString(-1,szFileName);
			GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(TRUE);
		}
		else
		{
			m_objWaveFilesCombo.SetCurSel(0);
			GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(FALSE);
		}
		m_objWaveFilesCombo.EnableWindow(TRUE);
	}

	*pResult = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�`�u�d�t�@�C���R���{�{�b�N�X�I��ύX������							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage11::OnSelchangeComboWavefiles() 
{
	int nIndex=0, nCnt=0;
	HTREEITEM hItem=NULL;
	TV_ITEM item;
	char szBuf[256],szFileName[_MAX_FNAME];

	//���ݑI�𒆂̃c���[�A�C�e���擾
	hItem = m_objSoundsTreeCtrl.GetSelectedItem();
	if(hItem == NULL)
		return;
	memset(szBuf,0,sizeof(szBuf));
	memset(&item,0,sizeof(TV_ITEM));
	item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE;
	item.hItem = hItem;
	item.pszText = szBuf;
	item.cchTextMax = 256;
	if(m_objSoundsTreeCtrl.GetItem(&item) == FALSE)
		return;
	//�I������Ă���A�C�e�����m�[�h�̏ꍇ�͉������Ȃ�
	for(nCnt=0; nCnt<4; nCnt++)
	{
		if(item.hItem == m_hRootItems[nCnt])
			return;
	}

	m_bIsChanged = TRUE;
	SetModified();

	//���ݑI�𒆂̂v�`�u�d�t�@�C�����擾
	nIndex = m_objWaveFilesCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;
	else if(nIndex == 0) //�u�v�`�u�d�t�@�C���Ȃ��v�ɂȂ���
	{
		//�A�C�e���C���[�W�ݒ�
		m_objSoundsTreeCtrl.SetItemImage(hItem,0,0);

		//�e�X�g�{�^��������
		GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(FALSE);

		//����炳�Ȃ��ɐݒ�
		for(nCnt=0; nCnt<14; nCnt++)
		{
			if(item.hItem == m_stSoundDef[nCnt].hItem)
			{
				m_stSoundDef[nCnt].bIsPlay = 0;
				memset(m_stSoundDef[nCnt].szWaveFileName,0,_MAX_FNAME);
				break;
			}
		}
	}
	else
	{
		//�A�C�e���C���[�W�ݒ�
		m_objSoundsTreeCtrl.SetItemImage(hItem,1,1);

		//�e�X�g�{�^���L����
		GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(TRUE);

		memset(szFileName,0,sizeof(szFileName));
		m_objWaveFilesCombo.GetLBText(nIndex,szFileName);

		//�v�`�u�d�t�@�C�����ݒ�
		for(nCnt=0; nCnt<14; nCnt++)
		{
			if(item.hItem == m_stSoundDef[nCnt].hItem)
			{
				m_stSoundDef[nCnt].bIsPlay = 1;
				strcpy(m_stSoundDef[nCnt].szWaveFileName,szFileName);
				break;
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�e�X�g�v�{�^������������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage11::OnButtonTest() 
{
	int	nIndex=0;
	char szBuf[256],szFilePath[_MAX_PATH];

	//���ݑI�𒆂̂v�`�u�d�t�@�C�����擾
	nIndex = m_objWaveFilesCombo.GetCurSel();
	if(nIndex == CB_ERR || nIndex == 0)
		return;
	memset(szBuf,0,sizeof(szBuf));
	if(m_objWaveFilesCombo.GetLBText(nIndex,szBuf) == CB_ERR)
		return;
	sprintf(szFilePath,"%s\\%s", (LPCTSTR)m_pApp->m_strMediaDirPath,szBuf);
	if(_access(szFilePath,0) == -1)
		return;

	//�v�`�u�d�t�@�C���e�X�g
	CWaitCursor objCursor;
	m_pApp->MyPlaySound(szFilePath,TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�n�j�v�{�^������������												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage11::OnOK() 
{
	//�R���g���[���|�������o�ϐ��f�[�^��荞��
	UpdateData(TRUE);
	CPropertyPage::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage11 �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g�����t�@�C�����X�g�쐬										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage11::MakeFileList		//�s�q�t�d�F����I���A�e�`�k�r�d�F�ُ�A�L�����Z���I��
	(	CString&	objFilter,
		CObArray*	pobjObArray
	)
{
	BOOL		bRet=FALSE;
	CString		objFilePath=_T("");
	CFileFind	objFileFind;

	//�t�@�C������
	bRet = objFileFind.FindFile(objFilter);
	if(bRet != FALSE)
	{
		while(objFileFind.FindNextFile()!=FALSE)
		{
			if( objFileFind.IsDots() != FALSE ) // "." or ".."
				continue;
			if( objFileFind.IsDirectory() != FALSE ) //�f�B���N�g��
				continue;

			objFilePath = objFileFind.GetFileName();
			AddFileBySort(objFilePath,pobjObArray);
		}
		//�Ō�̂P��
		if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
		{
			objFilePath = objFileFind.GetFileName();
			AddFileBySort(objFilePath,pobjObArray);
		}
	}
	objFileFind.Close();

	if(bRet)
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A���t�@�x�b�g���Ƀ��X�g�֒ǉ�										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F                  													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage11::AddFileBySort
	(	CString&	objFileName,
		CObArray*	pobjArray
	)
{
	int nIndex=0;
	int nSize=pobjArray->GetSize();
	CString* pobjStrTmp=NULL;
	CString* pobjStrBuf = new CString();
	(*pobjStrBuf) = objFileName;

	for(nIndex=0; nIndex<nSize; nIndex++)
	{
		pobjStrTmp = (CString*)pobjArray->GetAt(nIndex);
		if(pobjStrTmp->CompareNoCase((LPCTSTR)objFileName) <= 0)
			continue;
		else
			break;
	}

	if(nIndex == nSize)
		pobjArray->Add((CObject*)pobjStrBuf); //�Ō�ɒǉ�
	else
		pobjArray->InsertAt(nIndex,(CObject*)pobjStrBuf);
}


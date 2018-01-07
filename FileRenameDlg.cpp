// FileRenameDlg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "FileRenameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileRenameDlg �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFileRenameDlg::CFileRenameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileRenameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileRenameDlg)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	memset(m_szFromDir,0,sizeof(m_szFromDir));
	memset(m_szFromName,0,sizeof(m_szFromName));
	memset(m_szFromPath,0,sizeof(m_szFromPath));
	m_objToFile = _T("");
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
void CFileRenameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileRenameDlg)
	DDX_Control(pDX, IDC_EDIT_TOFILE, m_objEditToFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileRenameDlg, CDialog)
	//{{AFX_MSG_MAP(CFileRenameDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileRenameDlg ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�_�C�A���O�\��������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CFileRenameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//�A�C�R���ݒ�
	SetIcon(m_hIcon, TRUE);
	if ( ::GetSystemMetrics(SM_CYSMICON) < 32 ) {
		SetIcon(m_hIconS, FALSE);
	} else {
		SetIcon(m_hIcon, FALSE);
	}

	//�t�H���g�ݒ�
	m_objEditToFile.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

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

	//���t�@�C�����\��
	CString objStr = m_szFromName;
	SetDlgItemText(IDC_FROMFILE, objStr);

	//�f�t�H���g�f�[�^�ݒ�
	m_objEditToFile.SetWindowText(m_objToFile);
	m_objEditToFile.UpdateData(TRUE);

	//�����o�ϐ��|���R���g���[��
	UpdateData(FALSE);

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�n�j�{�^������������													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileRenameDlg::OnOK() 
{
	CString objMsg;
	//�R���g���[���|�������o�ϐ��f�[�^��荞��
	UpdateData(TRUE);

	//�f�[�^�擾
	m_objEditToFile.GetWindowText(m_objToFile);

	//�u�����N�`�F�b�N
	if(m_objToFile.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_AFTERNAME);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	//���O�d���`�F�b�N
	if(m_objToFile.CompareNoCase(m_szFromName) == 0)
	{
		objMsg.LoadString(IDS_INPUT_DIFFNAME);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	//�ύX��t�@�C�����Ó����`�F�b�N
	for(long lCnt=0; lCnt<m_objToFile.GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,m_objToFile.GetAt(lCnt)))//�Q�޲ĕ����̑���޲�
		{
			lCnt+=2;
		}
		else
		{
			if( m_objToFile.GetAt(lCnt) == '\\' )
			{
				objMsg.LoadString(IDS_INVALIDCHAR_AFTERNAME);
				m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
				return;
			}
			lCnt++;
		}
	}

	//�ύX��̃t�@�C���p�X��ҏW
	sprintf(m_szToPath,"%s%s",m_szFromDir,(LPCTSTR)m_objToFile);

	CDialog::OnOK();
}


// DateMaskEdit.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <afxdisp.h>	//For COleDateTime
#pragma warning(disable:4786)
#include "editor.h"
#include "Editor_Def.h"
#include "DateMaskEdit.h"

#define DELIMITER_CHARACTER		'/'		//�f���~�^����

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDateMaskEdit

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDateMaskEdit::CDateMaskEdit()
{
	m_bMaskKeyInProgress = FALSE;

	//���t�t�H�[�}�b�g������
	m_objDateFormatString = _T("YYYY/MM/DD");
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDateMaskEdit::~CDateMaskEdit()
{
}


BEGIN_MESSAGE_MAP(CDateMaskEdit, CEdit)
	//{{AFX_MSG_MAP(CDateMaskEdit)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDateMaskEdit �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����������@���t�ݒ�													|*/
/*|	���	�F																		|*/
/*|	���l	�F�{�����o�֐��́AOnInitDialog()����ĂԂ���							|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDateMaskEdit::Init()
{
	//����޳�쐬�ς݁H
	ASSERT(::IsWindow(m_hWnd));

	//���ݎ����擾���ݒ�
	CTime objTime;
	objTime = CTime::GetCurrentTime();
	m_objDateString = objTime.Format("%Y/%m/%d");
	SetWindowText(m_objDateString);
	UpdateData(TRUE);

	//�����o�ϐ��֕ێ�
	m_nYear = (UINT)objTime.GetYear();
	m_nMonth = (UINT)objTime.GetMonth();
	m_nDay = (UINT)objTime.GetDay();

	//�t�H���g�ݒ�
	SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�N�ݒ菈��															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDateMaskEdit::SetYear(UINT nYear)
{
	if(!(nYear < 9999))
		return;

	m_nYear = nYear;
	m_objDateString.Format("%04d/%02d/%02d",m_nYear,m_nMonth,m_nDay);
	SetWindowText(m_objDateString);
	UpdateData(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���ݒ菈��															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDateMaskEdit::SetMonth(UINT nMonth)
{
	if(!(nMonth >= 1 && nMonth <= 12))
		return;

	m_nMonth = nMonth;
	m_objDateString.Format("%04d/%02d/%02d",m_nYear,m_nMonth,m_nDay);
	SetWindowText(m_objDateString);
	UpdateData(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���ݒ菈��															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDateMaskEdit::SetDay(UINT nDay)
{
	if(!(nDay >= 1 && nDay <= 31))
		return;

	m_nDay = nDay;
	m_objDateString.Format("%04d/%02d/%02d",m_nYear,m_nMonth,m_nDay);
	SetWindowText(m_objDateString);
	UpdateData(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�N�擾����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CDateMaskEdit::GetYear()
{
	return m_nYear;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���擾����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CDateMaskEdit::GetMonth()
{
	return m_nMonth;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���擾����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CDateMaskEdit::GetDay()
{
	return m_nDay;
}

/////////////////////////////////////////////////////////////////////////////
// CDateMaskEdit ���b�Z�[�W �n���h��
	
/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�j�d�x�c�n�t�m���b�Z�[�W����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDateMaskEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch(nChar) //DELETE���AINSERT���ABACK SPACE���𖳎�
	{
    case VK_DELETE:
    case VK_INSERT: 
	case VK_BACK:
	case VK_CONTROL:
		  return;
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�b�g�`�q���b�Z�[�W����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDateMaskEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int startPos, endPos;

	switch(nChar) //DELETE���AINSERT���ABACK SPACE���𖳎�
	{
    case VK_DELETE:
    case VK_INSERT: 
	case VK_BACK:
	case VK_CONTROL:
	case 0x01: //Ctrl+A
	case 0x03: //Ctrl+C
	case 0x16: //Ctrl+V
	case 0x18: //Ctrl+X
		MessageBeep((UINT)-1);
		return;
	}

	///////////////////////////////////////////
	//	�t�H�[�}�b�g�ɍ������`�F�b�N
	if(!m_bMaskKeyInProgress)//��������� '/' �p��WM_CHARү���ޔ��s���łȂ�
	{
		//�������ꂽ�������t�H�[�}�b�g�ɂ������`�F�b�N
		if(!CheckChar(nChar))
		{
			MessageBeep((UINT)-1);//�t�H�[�}�b�g�ɍ���Ȃ�
			return;
		}
	}

	////////////////////////////////////////////
	//	�}�����[�h�ŕ����㏑��
	if (isprint(nChar))//�󎚉\
	{
		GetSel(startPos, endPos);//�������ڂŃL�[�������ꂽ���擾
		SetSel(startPos, endPos+1);
		Clear();//�G�f�B�b�g�̑I������
		m_objDateString.SetAt(endPos, nChar);//���������ւ�
	}

	//�h�����N���X�̃n���h�����R�[��
	CEdit::OnChar(nChar, nRepCnt, nFlags);

	/////////////////////////////////////////////
	//	�f���~�^�����������I�ɃX�L�b�v
	if(!m_bMaskKeyInProgress)//��������� '/' �p��WM_CHARү���ޔ��s���łȂ�
	{
		GetSel(startPos, endPos);
		if(endPos < m_objDateFormatString.GetLength())
		{
			UINT c = m_objDateFormatString.GetAt(endPos);
			if (c == DELIMITER_CHARACTER)//���̕������f���~�^
			{
				//�����I�ɃX�L�b�v����悤�Ɏ�����WM_CHAR���b�Z�[�W�𔭍s����
				SendChar(c);
			}
		}
	}

	/////////////////////////////////////////
	//	�N�A���A���X�V
	char* pPtr = m_objDateString.GetBuffer(36);
	char	szBuf[36];
	strncpy(szBuf,pPtr,4);//�N
	szBuf[4] = NULL;
	m_nYear = atoi(szBuf);
	strncpy(szBuf,(pPtr+5),2);//��
	szBuf[2] = NULL;
	m_nMonth = atoi(szBuf);
	strncpy(szBuf,(pPtr+8),2);//��
	szBuf[2] = NULL;
	m_nDay = atoi(szBuf);
	m_objDateString.ReleaseBuffer();
}

///////////////////////////////////////////////////////////////////
//	�v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�H�[�}�b�g�`�F�b�N����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDateMaskEdit::CheckChar(UINT nChar)//�s�q�t�d�F�n�j�A�e�`�k�r�d�F�m�f
{
	UINT	c;
	int		startPos, endPos;
	char	szBuf[36];
	char	*pPtr;

	///////////////////////////////////////////
	//	�����L�[�`�F�b�N
	if(!isprint(nChar))//�R���g���[���L�[�͂n�j
		return TRUE;

	GetSel(startPos,endPos);
	if(endPos >= m_objDateFormatString.GetLength())//�t�H�[�}�b�g�������蒷���Ȃ�΂͂���
		return FALSE;

	c = m_objDateFormatString.GetAt(endPos);
	if (c == DELIMITER_CHARACTER)	//�f���~�^�ʒu�ł̓f���~�^�����ȊO�_��
		if(nChar != c)
			return FALSE;
	else							//�f���~�^�ʒu�ȊO�ł́A�����ȊO�_��
		if(!isdigit(nChar))
			return FALSE;

	///////////////////////////////////////////
	//	�C���㕶���擾
	CString objStr = m_objDateString;
	GetSel(startPos, endPos);
	objStr.SetAt(endPos,nChar);

	//�N�A���A�����o��
	pPtr = objStr.GetBuffer(36);

	strncpy(szBuf,pPtr,4);//�N
	szBuf[4] = NULL;
	int		nYear = atoi(szBuf);

	strncpy(szBuf,(pPtr+5),2);//��
	szBuf[2] = NULL;
	int		nMonth = atoi(szBuf);

	strncpy(szBuf,(pPtr+8),2);//��
	szBuf[2] = NULL;
	int		nDay = atoi(szBuf);

	objStr.ReleaseBuffer();

//���t�Ó����`�F�b�N��CFileTouchDlg::OnOK�ɂčs��
//	////////////////////////////////////////
//	//	���t�Ó����`�F�b�N
//	COleDateTime	objOleDateTime(nYear,nMonth,nDay,0,0,0);
//	if(objOleDateTime.GetStatus() == COleDateTime::valid)//����
//		return TRUE;
//	else
//		return FALSE;
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�b�g�`�q���b�Z�[�W���s����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDateMaskEdit::SendChar(UINT nChar)
{
	m_bMaskKeyInProgress = TRUE;//������
#ifdef WIN32
	AfxCallWndProc(this, m_hWnd, WM_CHAR, nChar, 1);
#else
	SendMessage(WM_CHAR, nChar, 1);
#endif
	m_bMaskKeyInProgress = FALSE;//�I��
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���b�Z�[�W�O����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CDateMaskEdit::PreTranslateMessage(MSG* pMsg) 
{
//	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
//	if( pMsg->message == WM_KEYDOWN ) 
//	{
//		if( pMsg->wParam == 'V' ) 
//		{
//			if( GetAsyncKeyState(VK_CONTROL) < 0 ) //Ctrl+V�𖳎�
//				return TRUE;
//		}
//	}
	return CEdit::PreTranslateMessage(pMsg);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�b�n�m�s�d�w�s�l�d�m�t���b�Z�[�W���s����						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDateMaskEdit::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	//�������Ȃ����ƂŁA�}�E�X�E�N���b�N�ɂ��R���e�L�X�g
	//���j���[���\������Ȃ��悤�ɂ���
}


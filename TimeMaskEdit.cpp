// TimeMaskEdit.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <afxdisp.h>	//For COleDateTime
#pragma warning(disable:4786)
#include "editor.h"
#include "Editor_Def.h"
#include "TimeMaskEdit.h"

#define DELIMITER_CHARACTER		':'		//�f���~�^����

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimeMaskEdit

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CTimeMaskEdit::CTimeMaskEdit()
{
	m_bMaskKeyInProgress = FALSE;

	//���ԃt�H�[�}�b�g������
	m_objTimeFormatString = _T("HH:MM:SS");
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CTimeMaskEdit::~CTimeMaskEdit()
{
}


BEGIN_MESSAGE_MAP(CTimeMaskEdit, CEdit)
	//{{AFX_MSG_MAP(CTimeMaskEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDateMaskEdit �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����������@�����ݒ�													|*/
/*|	���	�F																		|*/
/*|	���l	�F�{�����o�֐��́AOnInitDialog()����ĂԂ���							|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CTimeMaskEdit::Init()
{
	//����޳�쐬�ς݁H
	ASSERT(::IsWindow(m_hWnd));

	//���ݎ����擾���ݒ�
	CTime objTime;
	objTime = CTime::GetCurrentTime();
	m_objTimeString = objTime.Format("%H:%M:%S");
	SetWindowText(m_objTimeString);
	UpdateData(TRUE);

	//�����o�ϐ��֕ێ�
	m_nHour   = (UINT)objTime.GetHour();
	m_nMinute = (UINT)objTime.GetMinute();
	m_nSecond = (UINT)objTime.GetSecond();

	//�t�H���g�ݒ�
	SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���Ԑݒ菈��															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CTimeMaskEdit::SetHour(UINT nHour)
{
	if(!(nHour <= 23))
		return;

	m_nHour = nHour;
	m_objTimeString.Format("%02d:%02d:%02d",m_nHour,m_nMinute,m_nSecond);
	SetWindowText(m_objTimeString);
	UpdateData(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���ݒ菈��															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CTimeMaskEdit::SetMinute(UINT nMinute)
{
	if(!(nMinute <= 59))
		return;

	m_nMinute = nMinute;
	m_objTimeString.Format("%02d:%02d:%02d",m_nHour,m_nMinute,m_nSecond);
	SetWindowText(m_objTimeString);
	UpdateData(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�b�ݒ菈��															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CTimeMaskEdit::SetSecond(UINT nSecond)
{
	if(!(nSecond <= 59))
		return;

	m_nSecond = nSecond;
	m_objTimeString.Format("%02d:%02d:%02d",m_nHour,m_nMinute,m_nSecond);
	SetWindowText(m_objTimeString);
	UpdateData(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���Ԏ擾����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CTimeMaskEdit::GetHour()
{
	return m_nHour;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���擾����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CTimeMaskEdit::GetMinute()
{
	return m_nMinute;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�b�擾����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CTimeMaskEdit::GetSecond()
{
	return m_nSecond;
}


/////////////////////////////////////////////////////////////////////////////
// CTimeMaskEdit ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�j�d�x�c�n�t�m���b�Z�[�W����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F1998.06.20 S.Ekura													|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CTimeMaskEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
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
void CTimeMaskEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
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
	if(!m_bMaskKeyInProgress)//��������� ':' �p��WM_CHARү���ޔ��s���łȂ�
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
		m_objTimeString.SetAt(endPos, nChar);//���������ւ�
	}

	//�h�����N���X�̃n���h�����R�[��
	CEdit::OnChar(nChar, nRepCnt, nFlags);

	/////////////////////////////////////////////
	//	�f���~�^�����������I�ɃX�L�b�v
	if(!m_bMaskKeyInProgress)//��������� ':' �p��WM_CHARү���ޔ��s���łȂ�
	{
		GetSel(startPos, endPos);
		if(endPos < m_objTimeFormatString.GetLength())
		{
			UINT c = m_objTimeFormatString.GetAt(endPos);
			if (c == DELIMITER_CHARACTER)//���̕������f���~�^
			{
				//�����I�ɃX�L�b�v����悤�Ɏ�����WM_CHAR���b�Z�[�W�𔭍s����
				SendChar(c);
			}
		}
	}

	////////////////////////////////////////////
	//	���ԁA���A�b�X�V
	char* pPtr = m_objTimeString.GetBuffer(36);
	char	szBuf[36];
	strncpy(szBuf,pPtr,2);//����
	szBuf[2] = NULL;
	m_nHour = atoi(szBuf);
	strncpy(szBuf,(pPtr+3),2);//��
	szBuf[2] = NULL;
	m_nMinute = atoi(szBuf);
	strncpy(szBuf,(pPtr+6),2);//�b
	szBuf[2] = NULL;
	m_nSecond = atoi(szBuf);
	m_objTimeString.ReleaseBuffer();
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
BOOL CTimeMaskEdit::CheckChar(UINT nChar)	//�s�q�t�d�F�n�j�A�e�`�k�r�d�F�m�f
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
	if(endPos >= m_objTimeFormatString.GetLength())//�t�H�[�}�b�g�������蒷���Ȃ�΂͂���
		return FALSE;

	c = m_objTimeFormatString.GetAt(endPos);
	if (c == DELIMITER_CHARACTER)	//�f���~�^�ʒu�ł̓f���~�^�����ȊO�_��
		if(nChar != c)
			return FALSE;
	else							//�f���~�^�ʒu�ȊO�ł́A�����ȊO�_��
		if(!isdigit(nChar))
			return FALSE;

	///////////////////////////////////////
	//	�C���㕶���擾
	CString objStr = m_objTimeString;
	GetSel(startPos, endPos);
	objStr.SetAt(endPos,nChar);

	//���ԁA���A�b���o��
	pPtr = objStr.GetBuffer(36);

	strncpy(szBuf,pPtr,2);//����
	szBuf[2] = NULL;
	int		nHour = atoi(szBuf);

	strncpy(szBuf,(pPtr+3),2);//��
	szBuf[2] = NULL;
	int		nMinute = atoi(szBuf);

	strncpy(szBuf,(pPtr+6),2);//�b
	szBuf[2] = NULL;
	int		nSecond = atoi(szBuf);

	objStr.ReleaseBuffer();

//���t�Ó����`�F�b�N��CFileTouchDlg::OnOK�ɂčs��
//	///////////////////////////////////////////
//	//	�����Ó����`�F�b�N
//	COleDateTime	objOleDateTime(1998,6,20,nHour,nMinute,nSecond);//���t�̓_�~�[
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
void CTimeMaskEdit::SendChar(UINT nChar)
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
BOOL CTimeMaskEdit::PreTranslateMessage(MSG* pMsg) 
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
void CTimeMaskEdit::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	//�������Ȃ����ƂŁA�}�E�X�E�N���b�N�ɂ��R���e�L�X�g
	//���j���[���\������Ȃ��悤�ɂ���
}


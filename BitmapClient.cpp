
#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "Editor_Def.h"
#include "BitmapClient.h"
#include "KeyBind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBitmapClient �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CBitmapClient::CBitmapClient()
{
	m_pApp = (CEditorApp*)AfxGetApp();

	//�����o�ϐ��N���A
	m_bDispLogoBmp = 0;
	m_nLogoBmpKind = 0;
	m_objBmpFilePath = _T("");
	m_nLogoPosKind = 0;
	m_unLogoPosX = 0;
	m_unLogoPosY = 0;
	m_bFavariteLogoLoadOK = FALSE;

	//�ݒ�擾
	m_MDIBkgndColor  = (COLORREF)m_pApp->GetProfileInt("CBitmapClient","MDIBkgndColor",RGB(128,128,128));
	m_bDispLogoBmp   = m_pApp->GetProfileInt("CBitmapClient","bDispLogoBmp",1);
	m_nLogoBmpKind   = m_pApp->GetProfileInt("CBitmapClient","nLogoBmpKind",LOGO_XERXES);
	m_objBmpFilePath = m_pApp->GetProfileString("CBitmapClient","LogoBmpFilePath","");
	m_nLogoPosKind   = m_pApp->GetProfileInt("CBitmapClient","nLogoPosKind",LOGO_POS_SITEI);
	m_unLogoPosX     = (UINT)m_pApp->GetProfileInt("CBitmapClient","LogoPosX",100);
	if(m_unLogoPosX > 100)
		m_unLogoPosX = 100;
	m_unLogoPosY     = (UINT)m_pApp->GetProfileInt("CBitmapClient","LogoPosY",100);
	if(m_unLogoPosY > 100)
		m_unLogoPosY = 100;

	//Xerxes���S�r�b�g�}�b�v�����[�h
	m_objXerxrsLogoBmp.LoadBitmap(IDB_LOGO);
	//�D���ȃ��S�r�b�g�}�b�v�����[�h
	m_bFavariteLogoLoadOK = LoadBMPImage(m_objBmpFilePath, m_objFavariteLogoBmp, NULL );
	if(m_nLogoBmpKind == LOGO_FAVARITE && m_bFavariteLogoLoadOK != TRUE)
	{
		CString objMsg;
		objMsg.LoadString(IDS_LOGOBMPLOAD_NG);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CBitmapClient::~CBitmapClient()
{
	//�r�b�g�}�b�v�j��
	if(m_bFavariteLogoLoadOK != FALSE)
		m_objFavariteLogoBmp.DeleteObject();

	//�ݒ�ۑ�
	m_pApp->WriteProfileInt("CBitmapClient","MDIBkgndColor",m_MDIBkgndColor);
	m_pApp->WriteProfileInt("CBitmapClient","bDispLogoBmp",m_bDispLogoBmp);
	m_pApp->WriteProfileInt("CBitmapClient","nLogoBmpKind",m_nLogoBmpKind);
	m_pApp->WriteProfileString("CBitmapClient","LogoBmpFilePath",m_objBmpFilePath);
	m_pApp->WriteProfileInt("CBitmapClient","nLogoPosKind",m_nLogoPosKind);
	m_pApp->WriteProfileInt("CBitmapClient","LogoPosX",m_unLogoPosX);
	m_pApp->WriteProfileInt("CBitmapClient","LogoPosY",m_unLogoPosY);
}


BEGIN_MESSAGE_MAP(CBitmapClient, CWnd)
	//{{AFX_MSG_MAP(CBitmapClient)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SYSKEYDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBitmapClient �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�l�c�h�w�i�F�擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CBitmapClient::GetMDIBkgndColor()
{
	return m_MDIBkgndColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�l�c�h�w�i�F�ݒ菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBitmapClient::SetMDIBkgndColor
	(	COLORREF	color
	)
{
	if(m_MDIBkgndColor != color)
	{
		m_MDIBkgndColor = color;
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW|RDW_ALLCHILDREN);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���S�r�b�g�}�b�v�\���t���O�擾����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CBitmapClient::GetFlagDispLogoBmp()
{
	return m_bDispLogoBmp;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���S�r�b�g�}�b�v�\���t���O�ݒ菈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBitmapClient::SetFlagDispLogoBmp
	(	BOOL	bDispLogoBmp
	)
{
	if(m_bDispLogoBmp != bDispLogoBmp)
	{
		m_bDispLogoBmp = bDispLogoBmp;
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW|RDW_ALLCHILDREN);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���S�r�b�g�}�b�v��ʎ擾����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CBitmapClient::GetLogoBmpKind()
{
	return	m_nLogoBmpKind;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���S�r�b�g�}�b�v��ʐݒ菈��											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBitmapClient::SetLogoBmpKind
	(	int		nLogoBmpKind
	)
{
	if(m_nLogoBmpKind != nLogoBmpKind)
	{
		m_nLogoBmpKind = nLogoBmpKind;
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW|RDW_ALLCHILDREN);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�r�b�g�}�b�v�t�@�C���p�X�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CString CBitmapClient::GetLogoBmpFilePath()
{
	return m_objBmpFilePath;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�r�b�g�}�b�v�t�@�C���p�X�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBitmapClient::SetLogoBmpFilePath
	(	CString	objBmpFilePath
	)
{
	if(m_objBmpFilePath.Compare(objBmpFilePath) != 0)
	{
		m_objBmpFilePath = objBmpFilePath;

		//�܂����܂ł̃r�b�g�}�b�v�j��
		if(m_bFavariteLogoLoadOK != FALSE)
			m_objFavariteLogoBmp.DeleteObject();
		//�V�����r�b�g�}�b�v�����[�h
		m_bFavariteLogoLoadOK = LoadBMPImage(m_objBmpFilePath, m_objFavariteLogoBmp, NULL );
		if(m_bFavariteLogoLoadOK == FALSE)
		{
			CString objMsg;
			objMsg.LoadString(IDS_LOGOBMPLOAD_NG);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		}
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW|RDW_ALLCHILDREN);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���S�\���ʒu��ʎ擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CBitmapClient::GetLogoPosKind()
{
	return	m_nLogoPosKind;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���S�\���ʒu��ʐݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBitmapClient::SetLogoPosKind
	(	int		nLogoPosKind
	)
{
	if(m_nLogoPosKind != nLogoPosKind)
	{
		m_nLogoPosKind = nLogoPosKind;
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW|RDW_ALLCHILDREN);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���S�\���ʒu�w�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CBitmapClient::GetLogoPosX()
{
	return m_unLogoPosX;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���S�\���ʒu�w�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBitmapClient::SetLogoPosX
	(	UINT		unLogoPosX
	)
{
	if(m_unLogoPosX != unLogoPosX)
	{
		m_unLogoPosX = unLogoPosX;
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW|RDW_ALLCHILDREN);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���S�\���ʒu�x�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CBitmapClient::GetLogoPosY()
{
	return m_unLogoPosY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���S�\���ʒu�x�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBitmapClient::SetLogoPosY
	(	UINT		unLogoPosY
	)
{
	if(m_unLogoPosY != unLogoPosY)
	{
		m_unLogoPosY = unLogoPosY;
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW|RDW_ALLCHILDREN);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView ���b�Z�[�W�n���h���i�t�B���^�֘A�j

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���b�Z�[�W�t�B���^����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CBitmapClient::PreTranslateMessage(MSG* pMsg) 
{
	UINT nChar = LOWORD(pMsg->wParam);
	CKeyBind* pobjKeyBind=NULL;
	BOOL bIsCtrl  = (::GetAsyncKeyState(VK_CONTROL) < 0 ? TRUE : FALSE);
	BOOL bIsShift = (::GetAsyncKeyState(VK_SHIFT)   < 0 ? TRUE : FALSE);
	BOOL bIsAlt   = (::GetAsyncKeyState(VK_MENU)    < 0 ? TRUE : FALSE);
	if(pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_KEYDOWN)
	{
		if(nChar >= VK_F1 && nChar <= VK_F12) //�t�@���N�V�����L�[
		{
			//�L�[�o�C���h����
			if((pobjKeyBind = m_pApp->FindKeyBind(nChar,bIsCtrl,bIsShift,bIsAlt)) != NULL)
			{
				AfxGetMainWnd()->PostMessage(WM_COMMAND,pobjKeyBind->m_dwMenuID,0);
				return TRUE;
			}
			else
				return CWnd::PreTranslateMessage(pMsg);
		}
		if(bIsCtrl || bIsAlt)
		{
			//�L�[�o�C���h����
			if((pobjKeyBind = m_pApp->FindKeyBind(nChar,bIsCtrl,bIsShift,bIsAlt)) != NULL)
			{
				AfxGetMainWnd()->PostMessage(WM_COMMAND,pobjKeyBind->m_dwMenuID,0);
				return TRUE;
			}
			else
				return CWnd::PreTranslateMessage(pMsg);
		}
		else if(bIsShift)
		{
			if(nChar==VK_INSERT || nChar==VK_DELETE || nChar==VK_BACK || nChar==VK_RETURN)
			{
				//�L�[�o�C���h����
				if((pobjKeyBind = m_pApp->FindKeyBind(nChar,bIsCtrl,bIsShift,bIsAlt)) != NULL)
				{
					AfxGetMainWnd()->PostMessage(WM_COMMAND,pobjKeyBind->m_dwMenuID,0);
					return TRUE;
				}
				else
					return CWnd::PreTranslateMessage(pMsg);
			}
			else
				return CWnd::PreTranslateMessage(pMsg);
		}
		else
			return CWnd::PreTranslateMessage(pMsg);
	}
	else
		return CWnd::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CBitmapClient ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�r�x�r�j�d�x�c�n�v�m����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBitmapClient::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CKeyBind* pobjKeyBind=NULL;
	BOOL bIsCtrl  = (::GetAsyncKeyState(VK_CONTROL) < 0 ? TRUE : FALSE);
	BOOL bIsShift = (::GetAsyncKeyState(VK_SHIFT)   < 0 ? TRUE : FALSE);
	BOOL bIsAlt   = (::GetAsyncKeyState(VK_MENU)    < 0 ? TRUE : FALSE);
	//�L�[�o�C���h����
	if((pobjKeyBind = m_pApp->FindKeyBind(nChar,bIsCtrl,bIsShift,bIsAlt)) != NULL)
		AfxGetMainWnd()->PostMessage(WM_COMMAND,pobjKeyBind->m_dwMenuID,0);
	
	CWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�j�d�x�c�n�v�m����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBitmapClient::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CKeyBind* pobjKeyBind=NULL;
	BOOL bIsCtrl  = (::GetAsyncKeyState(VK_CONTROL) < 0 ? TRUE : FALSE);
	BOOL bIsShift = (::GetAsyncKeyState(VK_SHIFT)   < 0 ? TRUE : FALSE);
	BOOL bIsAlt   = (::GetAsyncKeyState(VK_MENU)    < 0 ? TRUE : FALSE);
	//�L�[�o�C���h����
	if((pobjKeyBind = m_pApp->FindKeyBind(nChar,bIsCtrl,bIsShift,bIsAlt)) != NULL)
		AfxGetMainWnd()->PostMessage(WM_COMMAND,pobjKeyBind->m_dwMenuID,0);
	
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�r�h�y�d����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBitmapClient::OnSize(UINT nType, int cx, int cy) 
{
	//���N���X�̃n���h���R�[��
	CWnd::OnSize(nType, cx, cy);
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW|RDW_ALLCHILDREN);
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�v�l�Q�d�q�`�r�d�a�j�f�m�c����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CBitmapClient::OnEraseBkgnd(CDC* pDC) 
{
	//���N���X�̃n���h���R�[��
	CWnd::OnEraseBkgnd(pDC);

	UINT	unX=0, unY=0;
	BITMAP	bm;
	CDC		dcMem;
	CRect	rect;
	GetClientRect(rect);
	rect.NormalizeRect();
	CBrush	objBkgndBrush;
	objBkgndBrush.CreateSolidBrush(m_MDIBkgndColor);

	//�l�c�h�w�i��h��Ԃ�
	pDC->FillRect(&rect,&objBkgndBrush);
	if(m_bDispLogoBmp == FALSE)
		return TRUE;

	//////////////////////////
	//	���S�a�l�o�̕`��	//
	//////////////////////////

	if(m_nLogoBmpKind == LOGO_XERXES)
		m_objXerxrsLogoBmp.GetObject(sizeof(bm),(LPVOID)&bm);
	else if(m_nLogoBmpKind == LOGO_FAVARITE && m_bFavariteLogoLoadOK != FALSE)
		m_objFavariteLogoBmp.GetObject(sizeof(bm),(LPVOID)&bm);
	else
		return TRUE;

	dcMem.CreateCompatibleDC(pDC);
	CBitmap* pOldBMP;
	if(m_nLogoBmpKind == LOGO_XERXES)
		pOldBMP = (CBitmap*)dcMem.SelectObject(&m_objXerxrsLogoBmp);
	else if(m_nLogoBmpKind == LOGO_FAVARITE)
		pOldBMP = (CBitmap*)dcMem.SelectObject(&m_objFavariteLogoBmp);

	if(m_nLogoPosKind == LOGO_POS_TILE) //�^�C���\��
	{
		for(unX=0; unX<(UINT)rect.right; unX+=bm.bmWidth)
		{
			for(unY=0; unY<(UINT)rect.bottom; unY+=bm.bmHeight)
			{
				pDC->BitBlt( unX,				//�]����l�p�`�̍�����̘_�� x ���W
							 unY,				//�]����l�p�`�̍�����̘_�� y ���W
							 bm.bmWidth,		//�]����l�p�`�Ɠ]�����r�b�g�}�b�v�̕� (�_���P��) 
							 bm.bmHeight,		//�]����l�p�`�Ɠ]�����r�b�g�}�b�v�̍��� (�_���P��) 
							 &dcMem,			//�̽�ذ��ޯ̧��DC
							 0,					//�]�����r�b�g�}�b�v�̍�����̘_�� x ���W
							 0,					//�]�����r�b�g�}�b�v�̍�����̘_�� y ���W
							 SRCCOPY);			//���s׽����ڰ��݁i�]�����r�b�g�}�b�v��]����r�b�g�}�b�v�ɃR�s�[�j
			}
		}
	}
	else if(m_nLogoPosKind == LOGO_POS_SITEI) //�ʒu�w��\��
	{
		unX = (UINT)((rect.right - bm.bmWidth)   * ((float)m_unLogoPosX / 100));
		unY = (UINT)((rect.bottom - bm.bmHeight) * ((float)m_unLogoPosY / 100));
		pDC->BitBlt( unX,				//�]����l�p�`�̍�����̘_�� x ���W
					 unY,				//�]����l�p�`�̍�����̘_�� y ���W
					 bm.bmWidth,		//�]����l�p�`�Ɠ]�����r�b�g�}�b�v�̕� (�_���P��) 
					 bm.bmHeight,		//�]����l�p�`�Ɠ]�����r�b�g�}�b�v�̍��� (�_���P��) 
					 &dcMem,			//�̽�ذ��ޯ̧��DC
					 0,					//�]�����r�b�g�}�b�v�̍�����̘_�� x ���W
					 0,					//�]�����r�b�g�}�b�v�̍�����̘_�� y ���W
					 SRCCOPY);			//���s׽����ڰ��݁i�]�����r�b�g�}�b�v��]����r�b�g�}�b�v�ɃR�s�[�j
	}

	dcMem.SelectObject(pOldBMP);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CBitmapClient �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FBMP�t�@�C�������[�h����GDI�r�b�g�}�b�v�I�u�W�F�N�g�Ƙ_���p���b�g�����|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CBitmapClient::LoadBMPImage
	(	LPCTSTR		sBMPFile,			//BMP�t�@�C���̃t���p�X��
		CBitmap&	bitmap,				//�������������r�b�g�}�b�v�I�u�W�F�N�g
		CPalette*	pPal				//�_���p���b�g��ێ�����BNULL�ł��B
	)
{
	CFile file;
	if( !file.Open( sBMPFile, CFile::modeRead|CFile::typeBinary) )
		return FALSE;

	BITMAPFILEHEADER bmfHeader;

	// �t�@�C���w�b�_��ǂ�
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
	{
		file.Close();
		return FALSE;
	}

	// �t�@�C���^�C�v�� 'BM' �łȂ���΂Ȃ�Ȃ�
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
	{
		file.Close();
		return FALSE;
	}

	// �t�@�C���̎c��̒����𒲂ׂāA���������m�ۂ���
	DWORD nPackedDIBLen = (DWORD)(file.GetLength() - sizeof(BITMAPFILEHEADER));
	HGLOBAL hDIB = ::GlobalAlloc(GMEM_FIXED, nPackedDIBLen);
	if (hDIB == 0)
	{
		file.Close();
		return FALSE;
	}

	// �r�b�g�}�b�v�t�@�C���̎c���ǂݍ���
	if (file.Read((LPSTR)hDIB, nPackedDIBLen) != nPackedDIBLen )
	{
		::GlobalFree(hDIB);
		file.Close();
		return FALSE;
	}


	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;

	// bmiHeader.biClrUsed���O�Ȃ�A
	// �摜�̃r�b�g�����琄�������F����ݒ肵�Ă���
	int nColors = bmiHeader.biClrUsed ? bmiHeader.biClrUsed : 1 << bmiHeader.biBitCount;

	LPVOID lpDIBBits;
	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + bmInfo.bmiHeader.biClrUsed) + 
			((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);

	// �_���p���b�g�����
	if( pPal != NULL )
	{
		// �p���b�g�̐���
		if( nColors <= 256 )
		{
			UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
			LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

			pLP->palVersion = 0x300;
			pLP->palNumEntries = nColors;

			for( int i=0; i < nColors; i++)
			{
				pLP->palPalEntry[i].peRed = bmInfo.bmiColors[i].rgbRed;
				pLP->palPalEntry[i].peGreen = bmInfo.bmiColors[i].rgbGreen;
				pLP->palPalEntry[i].peBlue = bmInfo.bmiColors[i].rgbBlue;
				pLP->palPalEntry[i].peFlags = 0;
			}

			pPal->CreatePalette( pLP );

			delete[] pLP;
		}
	}

	CClientDC dc(NULL);
	CPalette* pOldPalette = NULL;
	if( pPal )
	{
		pOldPalette = dc.SelectPalette( pPal, FALSE );
		dc.RealizePalette();
	}

	//DIB����DDB���쐬
	//When you no longer need the bitmap, call the DeleteObject function to delete it. 
	HBITMAP hBmp = ::CreateDIBitmap( dc.m_hDC,		// �f�o�C�X�R���e�L�X�g
									 &bmiHeader,	// �r�b�g�}�b�v�T�C�Y�ƃt�H�[�}�b�g�f�[�^�ւ̃|�C���^
									 CBM_INIT,		// �������t���O
									 lpDIBBits,		// �������f�[�^�ւ̃|�C���^
									 &bmInfo,		// �J���[�t�H�[�}�b�g�f�[�^�ւ̃|�C���^
									 DIB_RGB_COLORS);// �J���[�f�[�^�̎g����
	bitmap.Attach( hBmp );

	if( pOldPalette )
		dc.SelectPalette( pOldPalette, FALSE );

	::GlobalFree(hDIB);
	return TRUE;
}


// ColorButton.cpp : ColorButton �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "ColorButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ColorButton �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CColorButton::CColorButton()
{
	m_colorBack = GetSysColor( COLOR_BTNFACE );
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CColorButton::~CColorButton()
{
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////
//	�p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�{�^���w�i�F�ݒ菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorButton::SetBackColor
	(	COLORREF color,			//�w�i�F
		BOOL	 bUpdate		//�X�V�t���O�iTRUE:�X�V����AFALSE:�X�V���Ȃ��j
	)
{
	m_colorBack = color;
	if(bUpdate != FALSE )
		Invalidate();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�{�^���w�i�F�擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorButton::GetBackColor()
{
	return m_colorBack;
}

/////////////////////////////////////////////////////////////////////////////
// CColorButton ���b�Z�[�W �n���h��

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�I�[�i�`��R���g���[���̍��ڂ�`�悷��Ƃ��ɂ�т�����鏈��			|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rect;
	rect.CopyRect(&lpDrawItemStruct->rcItem);
	rect.NormalizeRect();

	///////////////////////////////////////////
	//	�{�^���̊O�g�A�w�i�̕`��
	DrawBorders(pDC,rect,lpDrawItemStruct->itemState);

	///////////////////////////////////////////
	//	�{�^���̃L���v�V�����̕`��
	DrawText(pDC,rect,lpDrawItemStruct->itemState);
}

////////////////////////////////////////////////////////////////
//	�v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�g�`�揈��															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorButton::DrawBorders
	(	CDC* pDC,			//�f�o�C�X�R���e�L�X�g
		CRect& rect,		//�`��̈�
		UINT state			//�{�^���I�����
	)
{
	CBrush	objBrush;
	if( (state & ODS_DISABLED) )//�g�p�֎~���
		objBrush.CreateSolidBrush( GetSysColor(COLOR_BTNFACE) );
	else
		objBrush.CreateSolidBrush(m_colorBack);
	CBrush * pOldBrush = pDC->SelectObject(&objBrush);
	pDC->RoundRect(rect.left+1, rect.top+1, rect.Width()-1, rect.Height()-1, 2, 2);
	pDC->SelectObject(pOldBrush);

	if( (state & ODS_SELECTED) )//�{�^���I�����
		pDC->Draw3dRect(rect,GetSysColor(COLOR_3DDKSHADOW),GetSysColor(COLOR_3DHILIGHT));
	else
		pDC->Draw3dRect(rect,GetSysColor(COLOR_3DHILIGHT),GetSysColor(COLOR_3DDKSHADOW));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�e�L�X�g�`�揈��														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorButton::DrawText
	(	CDC* pDC,			//�f�o�C�X�R���e�L�X�g
		CRect&	rect,		//�`��̈�
		UINT state			//�{�^���I�����
	)
{
	unsigned char cRed=0,cGreen=0,cBlue=0;

	//�t�H���g�ݒ�
	LOGFONT	logfont;
	ZeroMemory((PVOID)&logfont,sizeof (LOGFONT));
	CFont * pFont = GetFont();
	pFont->GetLogFont(&logfont);
	if( (state & ODS_DISABLED) )//�g�p�֎~���
	{
		logfont.lfStrikeOut = TRUE;	//���������t��
	}
	CFont  dispFont;
	dispFont.CreateFontIndirect(&logfont);
	pFont = pDC->SelectObject (&dispFont);

	//�����F�ݒ�
	int iOldBkMode = pDC->GetBkMode();
	COLORREF    colorOldText = pDC->GetTextColor();
	pDC->SetBkMode( TRANSPARENT );
	cRed   = (unsigned char)((m_colorBack & 0xff0000) >> 16);
	cGreen = (unsigned char)((m_colorBack & 0x00ff00) >> 8);
	cBlue  = (unsigned char)(m_colorBack & 0x0000ff);
	if( (cRed+cGreen+cBlue) <= 256 ) //�w�i�����ɋ߂�
	{
		if( (state & ODS_DISABLED) )
			pDC->SetTextColor( RGB(0,0,0) );
		else
			pDC->SetTextColor( RGB((255-cRed),(255-cGreen),(255-cBlue)) );
	}
	else
	{
		pDC->SetTextColor( RGB(0,0,0) );
	}

	//�L���v�V�����`��
	CString objCaption;
	GetWindowText(objCaption);
	pDC->DrawText(objCaption,rect,(DT_CENTER|DT_VCENTER|DT_SINGLELINE));

	//�t�H�[�J�X�����鎞�A�_���g��`��
	if( (state & ODS_FOCUS) )
	{
		CPen	objPenWaku;
		CPen*	pobjOldPen;
		objPenWaku.CreatePen(PS_DOT,1,pDC->GetTextColor());
		pobjOldPen = pDC->SelectObject(&objPenWaku);
		pDC->MoveTo(rect.left+3,rect.top+3);			//����
		pDC->LineTo(rect.left+3,rect.Height()-3);		//����
		pDC->LineTo(rect.Width()-3,rect.Height()-3);	//�E��
		pDC->LineTo(rect.Width()-3,rect.top+3);			//�E��
		pDC->LineTo(rect.left+3,rect.top+3);			//���̈ʒu
		pDC->SelectObject(pobjOldPen);
	}

	//���ɖ߂�
	pDC->SetBkMode( iOldBkMode );
	pDC->SetTextColor( colorOldText );
	pDC->SelectObject(pFont);
	dispFont.DeleteObject();
}



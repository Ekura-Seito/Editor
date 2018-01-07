// ColorButton.cpp : ColorButton クラスの動作の定義を行います。
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
// ColorButton クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CColorButton::CColorButton()
{
	m_colorBack = GetSysColor( COLOR_BTNFACE );
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CColorButton::~CColorButton()
{
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////
//	パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ボタン背景色設定処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorButton::SetBackColor
	(	COLORREF color,			//背景色
		BOOL	 bUpdate		//更新フラグ（TRUE:更新する、FALSE:更新しない）
	)
{
	m_colorBack = color;
	if(bUpdate != FALSE )
		Invalidate();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ボタン背景色取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorButton::GetBackColor()
{
	return m_colorBack;
}

/////////////////////////////////////////////////////////////////////////////
// CColorButton メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：オーナ描画コントロールの項目を描画するときによびだされる処理			|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rect;
	rect.CopyRect(&lpDrawItemStruct->rcItem);
	rect.NormalizeRect();

	///////////////////////////////////////////
	//	ボタンの外枠、背景の描画
	DrawBorders(pDC,rect,lpDrawItemStruct->itemState);

	///////////////////////////////////////////
	//	ボタンのキャプションの描画
	DrawText(pDC,rect,lpDrawItemStruct->itemState);
}

////////////////////////////////////////////////////////////////
//	プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：枠描画処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorButton::DrawBorders
	(	CDC* pDC,			//デバイスコンテキスト
		CRect& rect,		//描画領域
		UINT state			//ボタン選択状態
	)
{
	CBrush	objBrush;
	if( (state & ODS_DISABLED) )//使用禁止状態
		objBrush.CreateSolidBrush( GetSysColor(COLOR_BTNFACE) );
	else
		objBrush.CreateSolidBrush(m_colorBack);
	CBrush * pOldBrush = pDC->SelectObject(&objBrush);
	pDC->RoundRect(rect.left+1, rect.top+1, rect.Width()-1, rect.Height()-1, 2, 2);
	pDC->SelectObject(pOldBrush);

	if( (state & ODS_SELECTED) )//ボタン選択状態
		pDC->Draw3dRect(rect,GetSysColor(COLOR_3DDKSHADOW),GetSysColor(COLOR_3DHILIGHT));
	else
		pDC->Draw3dRect(rect,GetSysColor(COLOR_3DHILIGHT),GetSysColor(COLOR_3DDKSHADOW));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：テキスト描画処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorButton::DrawText
	(	CDC* pDC,			//デバイスコンテキスト
		CRect&	rect,		//描画領域
		UINT state			//ボタン選択状態
	)
{
	unsigned char cRed=0,cGreen=0,cBlue=0;

	//フォント設定
	LOGFONT	logfont;
	ZeroMemory((PVOID)&logfont,sizeof (LOGFONT));
	CFont * pFont = GetFont();
	pFont->GetLogFont(&logfont);
	if( (state & ODS_DISABLED) )//使用禁止状態
	{
		logfont.lfStrikeOut = TRUE;	//取り消し線付き
	}
	CFont  dispFont;
	dispFont.CreateFontIndirect(&logfont);
	pFont = pDC->SelectObject (&dispFont);

	//文字色設定
	int iOldBkMode = pDC->GetBkMode();
	COLORREF    colorOldText = pDC->GetTextColor();
	pDC->SetBkMode( TRANSPARENT );
	cRed   = (unsigned char)((m_colorBack & 0xff0000) >> 16);
	cGreen = (unsigned char)((m_colorBack & 0x00ff00) >> 8);
	cBlue  = (unsigned char)(m_colorBack & 0x0000ff);
	if( (cRed+cGreen+cBlue) <= 256 ) //背景が黒に近い
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

	//キャプション描画
	CString objCaption;
	GetWindowText(objCaption);
	pDC->DrawText(objCaption,rect,(DT_CENTER|DT_VCENTER|DT_SINGLELINE));

	//フォーカスがある時、点線枠を描画
	if( (state & ODS_FOCUS) )
	{
		CPen	objPenWaku;
		CPen*	pobjOldPen;
		objPenWaku.CreatePen(PS_DOT,1,pDC->GetTextColor());
		pobjOldPen = pDC->SelectObject(&objPenWaku);
		pDC->MoveTo(rect.left+3,rect.top+3);			//左上
		pDC->LineTo(rect.left+3,rect.Height()-3);		//左下
		pDC->LineTo(rect.Width()-3,rect.Height()-3);	//右下
		pDC->LineTo(rect.Width()-3,rect.top+3);			//右上
		pDC->LineTo(rect.left+3,rect.top+3);			//元の位置
		pDC->SelectObject(pobjOldPen);
	}

	//元に戻す
	pDC->SetBkMode( iOldBkMode );
	pDC->SetTextColor( colorOldText );
	pDC->SelectObject(pFont);
	dispFont.DeleteObject();
}



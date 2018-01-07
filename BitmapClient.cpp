
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
// CBitmapClient クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CBitmapClient::CBitmapClient()
{
	m_pApp = (CEditorApp*)AfxGetApp();

	//メンバ変数クリア
	m_bDispLogoBmp = 0;
	m_nLogoBmpKind = 0;
	m_objBmpFilePath = _T("");
	m_nLogoPosKind = 0;
	m_unLogoPosX = 0;
	m_unLogoPosY = 0;
	m_bFavariteLogoLoadOK = FALSE;

	//設定取得
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

	//Xerxesロゴビットマップをロード
	m_objXerxrsLogoBmp.LoadBitmap(IDB_LOGO);
	//好きなロゴビットマップをロード
	m_bFavariteLogoLoadOK = LoadBMPImage(m_objBmpFilePath, m_objFavariteLogoBmp, NULL );
	if(m_nLogoBmpKind == LOGO_FAVARITE && m_bFavariteLogoLoadOK != TRUE)
	{
		CString objMsg;
		objMsg.LoadString(IDS_LOGOBMPLOAD_NG);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CBitmapClient::~CBitmapClient()
{
	//ビットマップ破棄
	if(m_bFavariteLogoLoadOK != FALSE)
		m_objFavariteLogoBmp.DeleteObject();

	//設定保存
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
// CBitmapClient パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＭＤＩ背景色取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CBitmapClient::GetMDIBkgndColor()
{
	return m_MDIBkgndColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＭＤＩ背景色設定処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：ロゴビットマップ表示フラグ取得処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CBitmapClient::GetFlagDispLogoBmp()
{
	return m_bDispLogoBmp;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ロゴビットマップ表示フラグ設定処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：ロゴビットマップ種別取得処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CBitmapClient::GetLogoBmpKind()
{
	return	m_nLogoBmpKind;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ロゴビットマップ種別設定処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：ビットマップファイルパス取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CString CBitmapClient::GetLogoBmpFilePath()
{
	return m_objBmpFilePath;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ビットマップファイルパス設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBitmapClient::SetLogoBmpFilePath
	(	CString	objBmpFilePath
	)
{
	if(m_objBmpFilePath.Compare(objBmpFilePath) != 0)
	{
		m_objBmpFilePath = objBmpFilePath;

		//まず今までのビットマップ破棄
		if(m_bFavariteLogoLoadOK != FALSE)
			m_objFavariteLogoBmp.DeleteObject();
		//新しいビットマップをロード
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
/*|	概要	：ロゴ表示位置種別取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CBitmapClient::GetLogoPosKind()
{
	return	m_nLogoPosKind;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ロゴ表示位置種別設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：ロゴ表示位置Ｘ取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CBitmapClient::GetLogoPosX()
{
	return m_unLogoPosX;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ロゴ表示位置Ｘ設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
/*|	概要	：ロゴ表示位置Ｙ取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CBitmapClient::GetLogoPosY()
{
	return m_unLogoPosY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ロゴ表示位置Ｙ設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
// CEditorView メッセージハンドラ（フィルタ関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：メッセージフィルタ処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
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
		if(nChar >= VK_F1 && nChar <= VK_F12) //ファンクションキー
		{
			//キーバインド検索
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
			//キーバインド検索
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
				//キーバインド検索
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
// CBitmapClient メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＳＹＳＫＥＹＤＯＷＮ処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBitmapClient::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CKeyBind* pobjKeyBind=NULL;
	BOOL bIsCtrl  = (::GetAsyncKeyState(VK_CONTROL) < 0 ? TRUE : FALSE);
	BOOL bIsShift = (::GetAsyncKeyState(VK_SHIFT)   < 0 ? TRUE : FALSE);
	BOOL bIsAlt   = (::GetAsyncKeyState(VK_MENU)    < 0 ? TRUE : FALSE);
	//キーバインド検索
	if((pobjKeyBind = m_pApp->FindKeyBind(nChar,bIsCtrl,bIsShift,bIsAlt)) != NULL)
		AfxGetMainWnd()->PostMessage(WM_COMMAND,pobjKeyBind->m_dwMenuID,0);
	
	CWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＫＥＹＤＯＷＮ処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBitmapClient::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CKeyBind* pobjKeyBind=NULL;
	BOOL bIsCtrl  = (::GetAsyncKeyState(VK_CONTROL) < 0 ? TRUE : FALSE);
	BOOL bIsShift = (::GetAsyncKeyState(VK_SHIFT)   < 0 ? TRUE : FALSE);
	BOOL bIsAlt   = (::GetAsyncKeyState(VK_MENU)    < 0 ? TRUE : FALSE);
	//キーバインド検索
	if((pobjKeyBind = m_pApp->FindKeyBind(nChar,bIsCtrl,bIsShift,bIsAlt)) != NULL)
		AfxGetMainWnd()->PostMessage(WM_COMMAND,pobjKeyBind->m_dwMenuID,0);
	
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＳＩＺＥ処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CBitmapClient::OnSize(UINT nType, int cx, int cy) 
{
	//基底クラスのハンドラコール
	CWnd::OnSize(nType, cx, cy);
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW|RDW_ALLCHILDREN);
	return;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＭ＿ＥＲＡＳＥＢＫＧＮＤ処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CBitmapClient::OnEraseBkgnd(CDC* pDC) 
{
	//基底クラスのハンドラコール
	CWnd::OnEraseBkgnd(pDC);

	UINT	unX=0, unY=0;
	BITMAP	bm;
	CDC		dcMem;
	CRect	rect;
	GetClientRect(rect);
	rect.NormalizeRect();
	CBrush	objBkgndBrush;
	objBkgndBrush.CreateSolidBrush(m_MDIBkgndColor);

	//ＭＤＩ背景を塗りつぶし
	pDC->FillRect(&rect,&objBkgndBrush);
	if(m_bDispLogoBmp == FALSE)
		return TRUE;

	//////////////////////////
	//	ロゴＢＭＰの描画	//
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

	if(m_nLogoPosKind == LOGO_POS_TILE) //タイル表示
	{
		for(unX=0; unX<(UINT)rect.right; unX+=bm.bmWidth)
		{
			for(unY=0; unY<(UINT)rect.bottom; unY+=bm.bmHeight)
			{
				pDC->BitBlt( unX,				//転送先四角形の左上隅の論理 x 座標
							 unY,				//転送先四角形の左上隅の論理 y 座標
							 bm.bmWidth,		//転送先四角形と転送元ビットマップの幅 (論理単位) 
							 bm.bmHeight,		//転送先四角形と転送元ビットマップの高さ (論理単位) 
							 &dcMem,			//ｵﾌｽｸﾘｰﾝﾊﾞｯﾌｧのDC
							 0,					//転送元ビットマップの左上隅の論理 x 座標
							 0,					//転送元ビットマップの左上隅の論理 y 座標
							 SRCCOPY);			//実行ﾗｽﾀｵﾍﾟﾚｰｼｮﾝ（転送元ビットマップを転送先ビットマップにコピー）
			}
		}
	}
	else if(m_nLogoPosKind == LOGO_POS_SITEI) //位置指定表示
	{
		unX = (UINT)((rect.right - bm.bmWidth)   * ((float)m_unLogoPosX / 100));
		unY = (UINT)((rect.bottom - bm.bmHeight) * ((float)m_unLogoPosY / 100));
		pDC->BitBlt( unX,				//転送先四角形の左上隅の論理 x 座標
					 unY,				//転送先四角形の左上隅の論理 y 座標
					 bm.bmWidth,		//転送先四角形と転送元ビットマップの幅 (論理単位) 
					 bm.bmHeight,		//転送先四角形と転送元ビットマップの高さ (論理単位) 
					 &dcMem,			//ｵﾌｽｸﾘｰﾝﾊﾞｯﾌｧのDC
					 0,					//転送元ビットマップの左上隅の論理 x 座標
					 0,					//転送元ビットマップの左上隅の論理 y 座標
					 SRCCOPY);			//実行ﾗｽﾀｵﾍﾟﾚｰｼｮﾝ（転送元ビットマップを転送先ビットマップにコピー）
	}

	dcMem.SelectObject(pOldBMP);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CBitmapClient プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：BMPファイルをロードしてGDIビットマップオブジェクトと論理パレットを作る|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CBitmapClient::LoadBMPImage
	(	LPCTSTR		sBMPFile,			//BMPファイルのフルパス名
		CBitmap&	bitmap,				//初期化したいビットマップオブジェクト
		CPalette*	pPal				//論理パレットを保持する。NULLでも可。
	)
{
	CFile file;
	if( !file.Open( sBMPFile, CFile::modeRead|CFile::typeBinary) )
		return FALSE;

	BITMAPFILEHEADER bmfHeader;

	// ファイルヘッダを読む
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
	{
		file.Close();
		return FALSE;
	}

	// ファイルタイプは 'BM' でなければならない
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
	{
		file.Close();
		return FALSE;
	}

	// ファイルの残りの長さを調べて、メモリを確保する
	DWORD nPackedDIBLen = (DWORD)(file.GetLength() - sizeof(BITMAPFILEHEADER));
	HGLOBAL hDIB = ::GlobalAlloc(GMEM_FIXED, nPackedDIBLen);
	if (hDIB == 0)
	{
		file.Close();
		return FALSE;
	}

	// ビットマップファイルの残りを読み込む
	if (file.Read((LPSTR)hDIB, nPackedDIBLen) != nPackedDIBLen )
	{
		::GlobalFree(hDIB);
		file.Close();
		return FALSE;
	}


	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;

	// bmiHeader.biClrUsedが０なら、
	// 画像のビット数から推測した色数を設定しておく
	int nColors = bmiHeader.biClrUsed ? bmiHeader.biClrUsed : 1 << bmiHeader.biBitCount;

	LPVOID lpDIBBits;
	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + bmInfo.bmiHeader.biClrUsed) + 
			((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);

	// 論理パレットを作る
	if( pPal != NULL )
	{
		// パレットの生成
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

	//DIBからDDBを作成
	//When you no longer need the bitmap, call the DeleteObject function to delete it. 
	HBITMAP hBmp = ::CreateDIBitmap( dc.m_hDC,		// デバイスコンテキスト
									 &bmiHeader,	// ビットマップサイズとフォーマットデータへのポインタ
									 CBM_INIT,		// 初期化フラグ
									 lpDIBBits,		// 初期化データへのポインタ
									 &bmInfo,		// カラーフォーマットデータへのポインタ
									 DIB_RGB_COLORS);// カラーデータの使い方
	bitmap.Attach( hBmp );

	if( pOldPalette )
		dc.SelectPalette( pOldPalette, FALSE );

	::GlobalFree(hDIB);
	return TRUE;
}


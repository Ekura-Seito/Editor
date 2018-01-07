// BitmapClient.h : CBitmapClient クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(BITMAPCLIENT_H)		//２重インクルード防止
#define BITMAPCLIENT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CBitmapClient : public CWnd
{
public:
	CBitmapClient();								// コンストラクタ
	virtual ~CBitmapClient();						// デストラクタ

	COLORREF	GetMDIBkgndColor();					// ＭＤＩ背景色取得
	void		SetMDIBkgndColor(COLORREF);			// ＭＤＩ背景色設定
	BOOL		GetFlagDispLogoBmp();				// ロゴビットマップ表示フラグ取得
	void		SetFlagDispLogoBmp(BOOL);			// ロゴビットマップ表示フラグ設定
	int			GetLogoBmpKind();					// ロゴビットマップ種別取得
	void		SetLogoBmpKind(int);				// ロゴビットマップ種別設定
	CString		GetLogoBmpFilePath();				// ビットマップファイルパス取得処理
	void		SetLogoBmpFilePath(CString);		// ビットマップファイルパス設定処理
	int			GetLogoPosKind();					// ロゴ表示位置種別取得
	void		SetLogoPosKind(int);				// ロゴ表示位置種別設定
	UINT		GetLogoPosX();						// ロゴ表示位置Ｘ取得
	void		SetLogoPosX(UINT);					// ロゴ表示位置Ｘ設定
	UINT		GetLogoPosY();						// ロゴ表示位置Ｙ取得
	void		SetLogoPosY(UINT);					// ロゴ表示位置Ｙ設定

private:
	CEditorApp* m_pApp;								// アプリケーションクラスへのポインタ
	BOOL		m_bDispLogoBmp;						// ロゴビットマップ表示フラグ
	int			m_nLogoBmpKind;						// ロゴビットマップ種別
	CString 	m_objBmpFilePath;					// ロゴビットマップファイルパス
	int			m_nLogoPosKind;						// ロゴ表示位置種別
	UINT		m_unLogoPosX;						// ロゴビットマップ表示位置Ｘ
	UINT		m_unLogoPosY;						// ロゴビットマップ表示位置Ｙ
	CBitmap		m_objXerxrsLogoBmp;					// Xerxesロゴビットマップ
	BOOL		m_bFavariteLogoLoadOK;				// 
	CBitmap		m_objFavariteLogoBmp;				// 好きなロゴビットマップ
	COLORREF	m_MDIBkgndColor;					// ＭＤＩ背景色

public:
// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CBitmapClient)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CBitmapClient)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadBMPImage(LPCTSTR,CBitmap&,CPalette*);
};

#endif // !defined(BITMAPCLIENT_H)


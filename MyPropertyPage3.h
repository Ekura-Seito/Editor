// MyPropertyPage3.h : CMyPropertyPage3 クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MYPROPERTYPAGE3_H)		//２重インクルード防止
#define MYPROPERTYPAGE3_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CColorButton;

class CMyPropertyPage3 : public CPropertyPage
{
	DECLARE_DYNCREATE(CMyPropertyPage3)

public:
	CMyPropertyPage3();					//コンストラクタ
	~CMyPropertyPage3();				//デストラクタ

	BOOL IsChanged();					// 変更発生調査
	void ClearChangeFlag();				// 

	CString		m_objFileExt;			// ファイル拡張子
	COLORREF	m_FontColor;			// 通常の文字色
	COLORREF	m_BackColor;			// 通常の背景色
	COLORREF	m_InvertFontColor;		// 反転時の文字色
	COLORREF	m_InvertBackColor;		// 反転時の背景色
	COLORREF	m_KeyNo1Color;			// キーワードＮｏ１の文字色
	COLORREF	m_KeyNo2Color;			// キーワードＮｏ２の文字色
	COLORREF	m_KeyNo3Color;			// キーワードＮｏ３の文字色
	COLORREF	m_KeyNo4Color;			// キーワードＮｏ４の文字色
	COLORREF	m_MarkLineColor;		// マーク行色
	COLORREF	m_CommentColor;			// コメントの文字色（Ｃ／Ｃ＋＋用）
	COLORREF	m_CommentColor2;		// コメントの文字色（ＶＢ用）
	COLORREF	m_CommentColor3;		// コメントの文字色（アセンブラ用）
	COLORREF	m_CommentColor4;		// コメントの文字色（引用符号用）
	COLORREF	m_LineNoColor;			// 行番号の色
	COLORREF	m_LineNoBackColor;		// 行番号の背景の色
	COLORREF	m_RulerColor;			// ルーラの色
	COLORREF	m_RulerBackColor;		// ルーラの背景の色
	COLORREF	m_CRLFColor;			// 改行マークの色
	COLORREF	m_TabColor;				// タブマークの色
	COLORREF	m_ZenkakuSpaceColor;	// 全角スペースの色
	COLORREF	m_UnderLineColor;		// アンダーラインの色
	COLORREF	m_UrlColor;				// ＵＲＬ色
	COLORREF	m_EMailColor;			// ｅ－ｍａｉｌ色
	COLORREF	m_MDIBkgndColor;		// ＭＤＩ背景色
	COLORREF	m_DiffDelLineColor;		// ファイル比較用、削除行色
	COLORREF	m_DiffAddLineColor;		// ファイル比較用、追加行色
	COLORREF	m_DirDiffLineColor;		// ディレクトリ比較用、相違ファイル色

	//{{AFX_DATA(CMyPropertyPage3)
	enum { IDD = IDD_PROPPAGE3 };
	CComboBox	m_objFileExtCombo;
	CColorButton	m_objTxtClrBtn;
	CColorButton	m_objInvTxtClrBtn;
	CColorButton	m_objInvBkClrBtn;
	CColorButton	m_objBkClrBtn;
	CColorButton	m_objKeyNo1ClrBtn;
	CColorButton	m_objKeyNo2ClrBtn;
	CColorButton	m_objKeyNo3ClrBtn;
	CColorButton	m_objKeyNo4ClrBtn;
	CColorButton	m_objMarkLineClrBtn;
	BOOL			m_bComment1Clr;
	CColorButton	m_objCommentClrBtn;
	BOOL			m_bComment2Clr;
	CColorButton	m_objCommentClrBtn2;
	BOOL			m_bComment3Clr;
	CColorButton	m_objCommentClrBtn3;
	BOOL			m_bComment4Clr;
	CColorButton	m_objCommentClrBtn4;
	BOOL			m_bDispLineNo;
	CColorButton	m_objLineNoClrBtn;
	CColorButton	m_objLineNoBackClrBtn;
	BOOL			m_bDispRuler;
	CColorButton	m_objRulerClrBtn;
	CColorButton	m_objRulerBackClrBtn;
	BOOL			m_bDispCRLF;
	CColorButton	m_objCRLFClrBtn;
	BOOL			m_bDispTab;
	CColorButton	m_objTabClrBtn;
	BOOL			m_bDispZenkakuSpace;
	CColorButton	m_objZenkakuSpaceClrBtn;
	BOOL			m_bDispUnderLine;
	CColorButton	m_objUnderLineClrBtn;
	BOOL			m_bDispUrl;
	CColorButton	m_objUrlClrBtn;
	BOOL			m_bDispEMail;
	CColorButton	m_objEMailClrBtn;
	CColorButton	m_objMDIBkgndClrBtn;
	CColorButton	m_objDiffDelLineClrBtn;
	CColorButton	m_objDiffAddLineClrBtn;
	CColorButton	m_objDirDiffLineClrBtn;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMyPropertyPage3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMyPropertyPage3)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonTxtclr();
	afx_msg void OnButtonBkclr();
	afx_msg void OnButtonInvbkclr();
	afx_msg void OnButtonInvtxtclr();
	afx_msg void OnButtonKeyword1clr();
	afx_msg void OnButtonKeyword2clr();
	afx_msg void OnButtonKeyword3clr();
	afx_msg void OnButtonKeyword4clr();
	afx_msg void OnButtonMarklineclr();
	afx_msg void OnChkcppcomment();
	afx_msg void OnButtonCommentclr();
	afx_msg void OnChkvbcomment();
	afx_msg void OnButtonCommentclr2();
	afx_msg void OnChkasmcomment();
	afx_msg void OnButtonCommentclr3();
	afx_msg void OnChkDisplineno();
	afx_msg void OnButtonlinenoclr();
	afx_msg void OnButtonlinenobackclr();
	afx_msg void OnChkDispruler();
	afx_msg void OnButtonrulerclr();
	afx_msg void OnButtonrulerbackclr();
	afx_msg void OnChkDispcrlf();
	afx_msg void OnButtonCrlfclr();
	afx_msg void OnChkDisptab();
	afx_msg void OnButtonTabclr();
	afx_msg void OnChkDispzenkakuspace();
	afx_msg void OnButtonZenkakuSpaceclr();
	afx_msg void OnChkDispunderline();
	afx_msg void OnButtonUnderLineclr();
	afx_msg void OnChkDispurl();
	afx_msg void OnButtonUrlclr();
	afx_msg void OnChkDispemail();
	afx_msg void OnButtonEmailclr();
	afx_msg void OnButtonMDIBkgndclr();
	afx_msg void OnButtonDiffDelLineclr();
	afx_msg void OnButtonDiffAddLineclr();
	afx_msg void OnButtonDirDifflineclr();
	afx_msg void OnSelchangeComboFileext();
	afx_msg void OnCheckQuote();
	afx_msg void OnButtonCommentclr4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;									//アプリケーションクラスへのポインタ
	BOOL		m_bIsChanged;							//変更発生フラグ
};

#endif // !defined(MYPROPERTYPAGE3_H)


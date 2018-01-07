// ColorButton.h : CColorButton クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(COLORBUTTON_H)		//２重インクルード防止
#define COLORBUTTON_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CColorButton : public CButton
{
public:
	CColorButton();									//コンストラクタ
	void SetBackColor(COLORREF,BOOL);				//ボタンの背景色設定
	COLORREF GetBackColor();						//ボタンの背景色取得

protected:
	void DrawBorders(CDC*,CRect&,UINT);				//ボタンの枠描画
	void DrawText(CDC*,CRect&,UINT);				//ボタンテキスト描画

private:
	COLORREF		m_colorBack;					//ボタンの背景色
	

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CColorButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CColorButton();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CColorButton)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(COLORBUTTON_H)

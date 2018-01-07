// DateMaskEdit.h : ヘッダー ファイル
//

#if !defined(DATEMASKEDIT_H)		//２重インクルード防止
#define DATEMASKEDIT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDateMaskEdit : public CEdit
{
// コンストラクション
public:
	CDateMaskEdit();			//コンストラクタ
	void Init();				//初期化

	void SetYear(UINT);			//年設定処理
	void SetMonth(UINT);		//月設定処理
	void SetDay(UINT);			//日設定処理
	UINT GetYear();				//年取得処理
	UINT GetMonth();			//月取得処理
	UINT GetDay();				//日取得処理

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CDateMaskEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CDateMaskEdit();

	// 生成されたメッセージ マップ関数
protected:
	CString m_objDateFormatString;		//日付フォーマット
	CString m_objDateString;			//表示中の日付文字列
	UINT m_nYear;						//年
	UINT m_nMonth;						//月
	UINT m_nDay;						//日
	BOOL m_bMaskKeyInProgress;			//ﾃﾞﾘﾐﾀ '/' 用 WM_CHAR発行中フラグ

	BOOL CheckChar(UINT nChar);			//入力文字のﾌｫｰﾏｯﾄﾁｪｯｸ処理
	void SendChar(UINT nChar);			//ﾃﾞﾘﾐﾀ '/' 用 WM_CHAR発行処理

	//{{AFX_MSG(CDateMaskEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif //!defined(DATEMASKEDIT_H)


// TimeMaskEdit.h : ヘッダー ファイル
//

#if !defined(TIMEMASKEDIT_H)		//２重インクルード防止
#define TIMEMASKEDIT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTimeMaskEdit : public CEdit
{
// コンストラクション
public:
	CTimeMaskEdit();
	void Init();				//初期化

	void SetHour(UINT);			//時間設定処理
	void SetMinute(UINT);		//分設定処理
	void SetSecond(UINT);		//秒設定処理
	UINT GetHour();				//時間取得処理
	UINT GetMinute();			//分取得処理
	UINT GetSecond();			//秒取得処理

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CTimeMaskEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CTimeMaskEdit();

	// 生成されたメッセージ マップ関数
protected:
	CString m_objTimeFormatString;		//時間フォーマット
	CString m_objTimeString;			//表示中の時間文字列
	UINT m_nHour;						//時刻
	UINT m_nMinute;						//分
	UINT m_nSecond;						//秒
	BOOL m_bMaskKeyInProgress;			//ﾃﾞﾘﾐﾀ ':' 用 WM_CHAR発行中フラグ

	BOOL CheckChar(UINT nChar);			//入力文字のﾌｫｰﾏｯﾄﾁｪｯｸ処理
	void SendChar(UINT nChar);			//ﾃﾞﾘﾐﾀ '/' 用 WM_CHAR発行処理

	//{{AFX_MSG(CTimeMaskEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif //!defined(TIMEMASKEDIT_H)


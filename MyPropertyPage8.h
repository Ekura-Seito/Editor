// MyPropertyPage8.h : CMyPropertyPage8 クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MYPROPERTYPAGE8_H)		//２重インクルード防止
#define MYPROPERTYPAGE8_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMyPropertyPage8 : public CPropertyPage
{
	DECLARE_DYNCREATE(CMyPropertyPage8)

// コンストラクション
public:
	CMyPropertyPage8();
	~CMyPropertyPage8();

	BOOL IsChanged();									// 変更発生調査
	void ClearChangeFlag();								// 

	STATUSBARDISP	m_stStatusBarDisp;					// ステータスバー表示情報構造体
	long			m_lStatusDayStyle;					// ステータスバーへ表示する日付表示スタイル
	long			m_lStatusHourStyle;					// ステータスバーへ表示する時刻表示スタイル
	long			m_lStatusLineStyle;					// ステータスバーへ表示する行、列表示スタイル

// ダイアログ データ
	//{{AFX_DATA(CMyPropertyPage8)
	enum { IDD = IDD_PROPPAGE8 };
	CComboBox	m_objLineStyleCombo;
	CComboBox	m_objHourStyleCombo;
	CComboBox	m_objDayStyleCombo;
	CListBox	m_objCurrentItemListBox;
	CListBox	m_objAddItemListBox;
	CButton	m_objDelButton;
	CButton	m_objAddButton;
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CMyPropertyPage8)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CMyPropertyPage8)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnSelchangeComboDaystyle();
	afx_msg void OnSelchangeComboHourstyle();
	afx_msg void OnSelchangeComboLinestyle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL	m_bIsChanged;								// 変更発生フラグ
};

#endif // !defined(MYPROPERTYPAGE8_H)


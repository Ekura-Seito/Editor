// KeyBind.h : ヘッダー ファイル
//

#if !defined(KEYBIND_H)
#define KEYBIND_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CKeyBind : public CObject
{
protected:
	DECLARE_SERIAL(CKeyBind);

public:
	CKeyBind();
	CKeyBind(CKeyBind&);
	CKeyBind(UINT,DWORD,BOOL,BOOL,BOOL);
	virtual ~CKeyBind();
	virtual void Serialize(CArchive&);

	void	operator=(CKeyBind&);
	BOOL	Compare(UINT,BOOL,BOOL,BOOL);
	BOOL	Compare(CKeyBind&);
	BOOL	Compare(DWORD);
	void	UpdateMenuID(DWORD);
	void	GetKeyBindString(char*);
	void	GetKeyBindString(wchar_t*);
	CString GetKeyBindString();

	UINT	m_nChar;				//キーコード
	DWORD	m_dwMenuID;				//メニューＩＤ
	BOOL	m_bIsCtrl;				//ＣＴＲＬキー
	BOOL	m_bIsShift;				//Ｓｈｉｆｔキー
	BOOL	m_bIsAlt;				//Ａｌｔキー
};

#endif // !defined(KEYBIND_H)


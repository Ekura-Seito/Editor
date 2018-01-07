// KeyBindManager.h : ヘッダー ファイル
//

#if !defined(KEYBINDMANAGER_H)
#define KEYBINDMANAGER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CKeyBind;

//キーバインドのリスト
typedef CTypedPtrList<CObList, CKeyBind*> CKeyBindList;

class CKeyBindManager
{
public:
	CKeyBindManager();
	virtual ~CKeyBindManager();

	void		readKeyBind();
	void		writeKeyBind();
	void		resetKeyBind();
	void		addKeyBind(CKeyBind&);
	CKeyBind*	FindKeyBind(UINT,BOOL,BOOL,BOOL);
	CKeyBind*	FindKeyBind(CKeyBind&);
	CKeyBind*	FindKeyBind(DWORD);

protected:
	void		SetDefaultKeyBind();
	void		ReadKeyBind();
	void		WriteKeyBind();
	void		UpdateKeyBind(CKeyBind&);
	void		clearAllItem();				// リストのメンバをすべてクリア

private:
	CEditorApp*	m_pApp;						// アプリケーションクラスへのポインタ
	CKeyBindList	m_objKeyBindList;		// CKeyBind のリスト
};

#endif // !defined(KEYBINDMANAGER_H)


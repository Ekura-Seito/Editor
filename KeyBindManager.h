// KeyBindManager.h : �w�b�_�[ �t�@�C��
//

#if !defined(KEYBINDMANAGER_H)
#define KEYBINDMANAGER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CKeyBind;

//�L�[�o�C���h�̃��X�g
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
	void		clearAllItem();				// ���X�g�̃����o�����ׂăN���A

private:
	CEditorApp*	m_pApp;						// �A�v���P�[�V�����N���X�ւ̃|�C���^
	CKeyBindList	m_objKeyBindList;		// CKeyBind �̃��X�g
};

#endif // !defined(KEYBINDMANAGER_H)


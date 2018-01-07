// KeyBind.h : �w�b�_�[ �t�@�C��
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

	UINT	m_nChar;				//�L�[�R�[�h
	DWORD	m_dwMenuID;				//���j���[�h�c
	BOOL	m_bIsCtrl;				//�b�s�q�k�L�[
	BOOL	m_bIsShift;				//�r���������L�[
	BOOL	m_bIsAlt;				//�`�����L�[
};

#endif // !defined(KEYBIND_H)


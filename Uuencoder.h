// Uuencoder.h : CUuencoder �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(UUENCODER_H)		//�Q�d�C���N���[�h�h�~
#define UUENCODER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CUuencoder
{
public:
	CUuencoder();									// �R���X�g���N�^
	virtual ~CUuencoder();							// �f�X�g���N�^

	CString BytesEncode(LPBYTE,UINT);

private:
	char*	m_pTrans;								// �ϊ��e�[�u���|�C���^
};

#endif // !defined(UUENCODER_H)


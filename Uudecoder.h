// Uudecode.h : CUudecoder �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(UUDECODER_H)		//�Q�d�C���N���[�h�h�~
#define UUDECODER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/* Single character decode.  */
#define	DEC(Char) (((Char) - ' ') & 077)

class CUudecoder
{
public:
	CUudecoder();									// �R���X�g���N�^
	virtual ~CUudecoder();							// �f�X�g���N�^

	INT  StringDecode(CString*,LPBYTE,long*);		// �������Uudecode�f�R�[�h
};

#endif // !defined(UUDECODER_H)


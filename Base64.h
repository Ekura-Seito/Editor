// Base64.h : CBase64 �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(BASE64_H)		//�Q�d�C���N���[�h�h�~
#define BASE64_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CBase64
{
public:
	CBase64();									// �R���X�g���N�^
	virtual ~CBase64();							// �f�X�g���N�^

	INT	 StringEncode(CString*);				// �������Base64�G���R�[�h
	INT  StringDecode(CString*,LPBYTE,long*);	// �������Base64�f�R�[�h
	CString BytesEncode(LPBYTE,UINT);

protected:
	BYTE B64ToBin(BYTE);
	BYTE BinToB64(BYTE);
};

#endif // !defined(BASE64_H)


// QuotedPrintable.h : CQuotedPrintable �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(QuotedPrintable_H)		//�Q�d�C���N���[�h�h�~
#define QuotedPrintable_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CQuotedPrintable
{
public:
	CQuotedPrintable();								// �R���X�g���N�^
	virtual ~CQuotedPrintable();					// �f�X�g���N�^

	INT	 StringEncode(CString*);					// ������̃G���R�[�h
	INT  StringDecode(CString*,LPBYTE,long*);
	CString BytesEncode(LPBYTE,UINT);

protected:
	BOOL DataCheck(BYTE,int);
};

#endif // !defined(QuotedPrintable_H)


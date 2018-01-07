// MyDes.h : CDes �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(DES_H)		//�Q�d�C���N���[�h�h�~
#define DES_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDes
{
public:
	CDes();								// �R���X�g���N�^
	virtual ~CDes();					// �f�X�g���N�^

	int main(int argc, char *argv[]);
	int crypt(char,char*,char*,int,char*);

protected:
	int ecb_crypt(char*,char*,unsigned,unsigned);
	int cbc_crypt(char*,char*,unsigned,unsigned,char*);
	void des_setparity(char*);
	void des_setkey(char*);
	void des_crypt(char*,int);
	void set_random(char*,int);
	void killarg(char*);
};

#endif // !defined(DES_H)


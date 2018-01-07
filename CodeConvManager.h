// CodeConvManager.h : CCodeConvManager �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(CODECONVMANAGER_H)		//�Q�d�C���N���[�h�h�~
#define CODECONVMANAGER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//���͕����R�[�h��ʒ�`
#define INPUT_SJIS		1
#define INPUT_EUC		2
#define INPUT_JIS		3
#define INPUT_ASCII		4
#define INPUT_UNKNOWN	5
#define INPUT_BINARY	6

//�����R�[�h��`
#define CODE_ESC 			0x1b
#define CODE_SI 			0x0f			//Shift In

//�R�[�h����}�N��
#define iskanji1st(c) 	((c) >= 0x81 && (c) <= 0x9f || (c) >= 0xe0 && (c) <= 0xfc)
#define iskanji2nd(c) 	((c) >= 0x40 && (c) <= 0xfc && (c) != 0x7f)
#define iseuc(c) 		((c) >= 0xa1 && (c) <= 0xfe)
#define isjis(c) 		((c) >= 0x21 && (c) <= 0x7f)
#ifndef isHanKana
#define isHanKana(c) 	((c) >= 0xa0 && (c) <= 0xdf)
#endif

//�g�p������s�R�[�h�̋敪
#define KAIGYO_CRLF		0
#define KAIGYO_LF		1
#define KAIGYO_CR		2
#define KAIGYO_UNKNOWN	3

//���s�R�[�h�ϊ����
#define KAIGYOCONV_TO_CRLF		"---> CR + LF"
#define KAIGYOCONV_TO_LF		"---> LF"
#define KAIGYOCONV_TO_CR		"---> CR"


class CCodeConvManager
{
public:
	CCodeConvManager();									// �R���X�g���N�^
	virtual ~CCodeConvManager();						// �f�X�g���N�^

	UINT DetectCode(const char*);						// �t�@�C���̃R�[�h����
	UINT DetectKaigyoCode(const char*);					// �t�@�C���̉��s�R�[�h����

	INT ConvertSjisToEuc(const char*,const char*);		// SJIS -> EUC  �R�[�h�ϊ�
	INT ConvertSjisToJis(const char*,const char*);		// SJIS -> JIS  �R�[�h�ϊ�
	INT ConvertEucToSjis(const char*,const char*);		// EUC  -> SJIS �R�[�h�ϊ�
	INT ConvertEucToJis(const char*,const char*);		// EUC  -> JIS  �R�[�h�ϊ�
	INT ConvertJisToSjis(const char*,const char*);		// JIS  -> SJIS �R�[�h�ϊ�
	INT ConvertJisToEuc(const char*,const char*);		// JIS  -> EUC  �R�[�h�ϊ�

	INT StringKatakanaHan2Zen(CString*);				// �����񒆂̃J�^�J�i ���p�|���S�p�ϊ�

	INT StringSjisToEuc(CString*);						// �������SJIS -> EUC  �R�[�h�ϊ�
	INT StringSjisToJis(CString*);						// �������SJIS -> JIS  �R�[�h�ϊ�
	INT StringEucToSjis(CString*);						// �������EUC  -> SJIS �R�[�h�ϊ�
	INT StringEucToJis(CString*);						// �������EUC  -> JIS  �R�[�h�ϊ�
	INT StringJisToSjis(CString*);						// �������JIS  -> SJIS �R�[�h�ϊ�
	INT StringJisToEuc(CString*);						// �������JIS  -> EUC  �R�[�h�ϊ�

	BOOL IsMaruKana(unsigned int);						// �p�A�v�A�|�@�Ȃǂ̊ەt���J�^�J�i�̔���
	BOOL IsDakuonKana(unsigned int);					// �o�A�K�A�U�@�Ȃǂ̑����t���J�^�J�i�̔���
	unsigned int AddMaruToKana(unsigned int);			// �S�p�J�^�J�i�Ɋۂ�ǉ�
	unsigned int AddDakuonToKana(unsigned int);			// �S�p�J�^�J�i�ɑ�����ǉ�

	BOOL IsZenkakuAlnum(unsigned int);					// �S�p�p��������
	unsigned int AlnumToZen(unsigned int);				// �p�����@���p�|���S�p
	unsigned int AlnumToHan(unsigned int);				// �p�����@�S�p�|�����p

protected:
	BOOL IsBinary(const char*);							// ���̓t�@�C���o�C�i���`�F�b�N
	UINT whatKanji(UCHAR*);								// �R�[�h����
	UINT detectcode(unsigned char*,unsigned int,BOOL*);
	UINT checkforward(unsigned char*,unsigned int);

	unsigned int KanaHan2Zen(unsigned int);				// ���p�J�i��S�p�J�i(JIS)�ɕϊ�����
	unsigned int KanaZen2Han(unsigned int);				// �S�p�J�i(JIS)�𔼊p�J�i�ɕϊ�����
	unsigned int sjis2euc(unsigned int);				// �r�i�h�r�|���d�t�b
	unsigned int sjis2jis(unsigned int);				// �r�i�h�r�|���i�h�r
	unsigned int euc2sjis(unsigned int);				// �d�t�b�|���r�i�h�r
	unsigned int euc2jis(unsigned int);					// �d�t�b�|���i�h�r
	unsigned int jis2sjis(unsigned int);				// �i�h�r�|���r�i�h�r
	unsigned int jis2euc(unsigned int);					// �i�h�r�|���d�t�b
	void set1byte(CArchive&);							// �P�o�C�g�����p�ɃG�X�P�[�v�V�[�P���X���o��
	void set2byte(CArchive&);							// �Q�o�C�g�����p�ɃG�X�P�[�v�V�[�P���X���o��
	void set1byte(CString*);
	void set2byte(CString*);
};

#endif // !defined(CODECONVMANAGER_H)


// Editor_Def.h �{�A�v���P�[�V�������ʂ̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(EDITOR_DEF_H)		//�Q�d�C���N���[�h�h�~
#define EDITOR_DEF_H

#ifndef NULL
#define NULL 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

//////////////
//	�r�b�g	//
//////////////
#define BIT_0	0x0001
#define BIT_1	0x0002
#define BIT_2	0x0004
#define BIT_3	0x0008
#define BIT_4	0x0010
#define BIT_5	0x0020
#define BIT_6	0x0040
#define BIT_7	0x0080
#define BIT_8	0x0100
#define BIT_9	0x0200
#define BIT_10	0x0400
#define BIT_11	0x0800
#define BIT_12	0x1000
#define BIT_13	0x2000
#define BIT_14	0x4000
#define BIT_15	0x8000

//////////////////////
//	���C�Z���X�L�[	//
//////////////////////
#define LISENCEKEY_COUNT	40
#ifdef MAIN
const char *g_szLisenceKeys[] = 
{
	"2dc47e2d", "7bba4079", "33d5cc50", "c45cc1df", "91721cac",
	"8450d9b9", "4ac15ac7", "de4d359a", "7e9fdd10", "4d565094",
	"591e1a69", "367f4ca5", "18ade61d", "477456fd", "8fb2e393",
	"56d410bc", "5a156a49", "730435b2", "afb7bdb8", "8cc76512",
	"a254511e", "0578d132", "53225519", "7a650110", "6d1d4bd1",
	"f3f2321c", "dbfbe323", "dd445ee6", "a76e199c", "a2053503",
	"f7c8c8ce", "83b1a7f8", "fbce0455", "7ab9be79", "04030405",
	"5ca2c707", "bfbeb94f", "715b36ea", "2909069c", "bd1a39e8"
};
#else
extern const char *g_szLisenceKeys[];
#endif

//////////////////////
//	�����R�[�h�֘A	//
//////////////////////

//�����R�[�h�ϊ����
#define CODECONV_TO_EUC			0
#define CODECONV_TO_SJIS		1
#define CODECONV_TO_JIS			2

//���̓t�@�C���̕����R�[�h����
#define INCODE_AUTO				0
#define INCODE_EUC				1
#define INCODE_SJIS				2
#define INCODE_JIS				3

//�����R�[�h��`
#define CODE_CR 			0x0d
#define CODE_LF 			0x0a
#define CODE_EOF			0x1a
#define CODE_SQUOTE			0x27			//�V���O���N�I�[�g
#define CODE_SEMICOLON		0x3b			//�Z�~�R����
#define CODE_QUOTE			0x3e			// '>'

//�R�[�h����}�N��
#define isHanKana(c) 	((c) >= 0xa0 && (c) <= 0xdf)
#define isAlnum(c)		(((c) >= '0' && (c) <='9') || ((c) >= 'a' && (c) <='z') || ((c) >= 'A' && (c) <='Z'))
#define isAlSmall(c)	((c) >= 'a' && (c) <= 'z')
#define isAlBig(c)		((c) >= 'A' && (c) <= 'Z')
#define isValidURL(c)   ( !IsDBCSLeadByteEx(0,(c)) &&									\
						  (c) != '('  && (c) != ')' && (c) != '<'  && (c) != '>'  &&	\
						  (c) != ','  && (c) != ';' && (c) != '\\' && (c) != '"'  &&	\
						  (c) != '['  && (c) != ']' && (c) != ' '  && (c) != '\t' &&	\
						  (c) != '\''													\
						)
#define isValidEMail(c)	( !IsDBCSLeadByteEx(0,(c))   &&									\
						  (c) != '('  && (c) != ')'  && (c) != '<' && (c) != '>'  &&	\
						  (c) != ','  && (c) != ';'  && (c) != ':' && (c) != '\\' &&	\
						  (c) != '"'  && (c) != '['  && (c) != ']' && (c) != ' '  &&	\
						  (c) != '\t' && (c) != '\'' && (c) != '@' && (c) != '/'		\
						)

//////////////////
//	����֘A	//
//////////////////

//����p���̕���
#define PRINTORIENTATION_TATE	0
#define PRINTORIENTATION_YOKO	1

//����p�@�w�b�_�̈ʒu
#define PRINT_HEADER_LEFT		0
#define PRINT_HEADER_CENTER		1
#define PRINT_HEADER_RIGHT		2

//����p�@�t�b�^�̈ʒu
#define PRINT_FOOTER_LEFT		0
#define PRINT_FOOTER_CENTER		1
#define PRINT_FOOTER_RIGHT		2

//////////////////////
//	�L�����b�g�֘A	//
//////////////////////

//�L�����b�g�`��̒�`
#define CARET_IBEAM				1
#define CARET_SQUARE			2

//�X�N���[�����̃L�����b�g����̒�`
#define CARET_BEHAVE_MOVE		1
#define CARET_BEHAVE_STAY		2

//�L�����b�g�ړ��^�C�v
#define CARET_MOVE_WORDTYPE		0
#define CARET_MOVE_NOTEPADTYPE	1

//////////////////////
//	�L�[����֘A	//
//////////////////////

//�L�[����t�@�C���̃}�W�b�N���[�h��`
#define KEYOPE_MAGICWORD		"This is Key Operation File Ver1.1"

//�L�[����t�@�C���̃I�y���[�V�������w�b�_��`
#define KEYOPE_OPENAME_HEADER	"Operation Name = "

//�L�[����̎�ʒ�`
#define KEYOPE_ONKEYDOWN				1
#define KEYOPE_ONKEYUP					2
#define KEYOPE_ONCHAR					3
#define KEYOPE_ONEDITCOPY				4
#define KEYOPE_ONEDITCUT				5
#define KEYOPE_ONEDITPASTE				6
#define KEYOPE_ONEDITDEL				7
#define KEYOPE_ONEDITSELECTALL			8
#define KEYOPE_ONEDITTAB2SPACE  		9
#define KEYOPE_ONEDITSPACE2TAB			10
#define KEYOPE_ONEDITSPACEZEN2HAN		11
#define KEYOPE_ONEDITSPACEHAN2ZEN		12
#define KEYOPE_ONEDITKATAKANAHAN2ZEN	13
#define KEYOPE_ONEDITKATAKANAZEN2HAN	14
#define KEYOPE_ONEDITALNUMHAN2ZEN		15
#define KEYOPE_ONEDITALNUMZEN2HAN		16
#define KEYOPE_ONEDITDELTOPSPACES		17
#define KEYOPE_ONEDITINSTAB				18
#define KEYOPE_ONEDITDELTAB				19
#define KEYOPE_ONEDITJUMP2TOP			20
#define KEYOPE_ONEDITJUMP2BOTTOM		21
#define KEYOPE_ONPARENTHESIS			22
#define KEYOPE_ONEDITINSCPPCOMMENT		23
#define KEYOPE_ONEDITINSVBCOMMENT		24
#define KEYOPE_ONEDITINSASMCOMMENT		25
#define KEYOPE_ONEDITINSDAY				26
#define KEYOPE_ONEDITINSTIME			27
#define KEYOPE_ONJUMPNEXTMARK			28
#define KEYOPE_ONJUMPPREVMARK			29
#define KEYOPE_ONJUMPMARKLIST			30
#define KEYOPE_ONEDITAL2UPPER			31
#define KEYOPE_ONEDITAL2LOWER			32
#define KEYOPE_ONWORDSKIP2RIGHT			33
#define KEYOPE_ONWORDSKIP2LEFT			34
#define KEYOPE_ONWORDSELECT2RIGHT		35
#define KEYOPE_ONWORDSELECT2LEFT		36
#define KEYOPE_ONEDITJUMP2TOPSELECT		37
#define KEYOPE_ONEDITJUMP2BOTTOMSELECT	38
#define KEYOPE_ONPARENTHESIS2			39
#define KEYOPE_ONPARENTHESISSELECT		40
#define KEYOPE_ONEDITINSQUOTE			41
#define KEYOPE_ONEDITDELCPPCOMMENT		42
#define KEYOPE_ONEDITDELVBCOMMENT		43
#define KEYOPE_ONEDITDELASMCOMMENT		44
#define KEYOPE_ONEDITDELQUOTE			45

//////////////////////////
//	�X�e�[�^�X�o�[�֘A	//
//////////////////////////

//�X�e�[�^�X�o�[�\�����ڐ�
#define STS_INDICATORS_COUNT			10

//�X�e�[�^�X�o�[�֕\��������t�\���X�^�C��
#define STS_DAYSTYLE_NONE				0
#define STS_DAYSTYLE_YYYYMMDD			1
#define STS_DAYSTYLE_YYMMDD				2
#define STS_DAYSTYLE_MMDDYYYY			3
#define STS_DAYSTYLE_MMDDYY				4
#define STS_DAYSTYLE_MMDD				5
#define STS_DAYSTYLE_DDMMYYYY			6
#define STS_DAYSTYLE_DDMMYY				7
#define STS_DAYSTYLE_DDMM				8
#define STS_DAYSTYLE_YYYY_MM_DD			9
#define STS_DAYSTYLE_YY_MM_DD			10
#define STS_DAYSTYLE_MM_DD				11

//�X�e�[�^�X�o�[�֕\�����鎞���\���X�^�C��
#define STS_HOURSTYLE_NONE				0
#define STS_HOURSTYLE_HHMM				1
#define STS_HOURSTYLE_AMPM_HHMM			2
#define STS_HOURSTYLE_HH_MM				3
#define STS_HOURSTYLE_AMPM_HH_MM		4

//�X�e�[�^�X�o�[�֕\������s�A��\���X�^�C��
#define STS_LINESTYLE_LINE_OFFS_ALL		0
#define STS_LINESTYLE_LINE_OFFS			1
#define STS_LINESTYLE_LINE_ALL			2
#define STS_LINESTYLE_ALL				3
#define STS_LINESTYLE_LINE				4
#define STS_LINESTYLE_OFFS				5

//////////////////////////
//	�w���v�g�s�b�N�h�c	//
//////////////////////////

//�u�͂��߂Ɂv�̍���
#define HELPTOPIC_GENERAL				2				//�T�v
#define HELPTOPIC_FEATURE				3				//����
#define HELPTOPIC_ABOUTSOFTNAME			25				//�\�t�g���̂ɂ���
#define HELPTOPIC_INSTALL				4				//�C���X�g�[�����@
#define HELPTOPIC_UNINSTALL				5				//�A���C���X�g�[�����@
#define HELPTOPIC_TARGETMACHINE			6				//����@��ƊJ����
//�u���j���[�v�̍���
#define HELPTOPIC_FILEMENU				8				//�u�t�@�C���v���j���[
#define HELPTOPIC_EDITMENU				9				//�u�ҏW�v���j���[
#define HELPTOPIC_FINDMENU				10				//�u�����v���j���[
#define HELPTOPIC_JUMPMENU				11				//�u�W�����v�v���j���[
#define HELPTOPIC_VIEWMENU				12				//�u�\���v���j���[
#define HELPTOPIC_TOOLMENU				13				//�u�c�[���v���j���[
#define HELPTOPIC_SETMENU				14				//�u�ݒ�v���j���[
#define HELPTOPIC_WINDOWMENU			15				//�u�E�C���h�E�v���j���[
#define HELPTOPIC_HELPMENU				16				//�u�w���v�v���j���[
//�u�Z�p���v�̍���
#define HELPTOPIC_CMDLINE				18				//�N�����R�}���h���C��
#define HELPTOPIC_KEYWORDCOLOR			19				//�L�[���[�h�J���[�����O
#define HELPTOPIC_KEYOPERATION			20				//�L�[����̋L�^
#define HELPTOPIC_LINEMARKING			26				//�s�̃}�[�L���O
#define HELPTOPIC_DRAGANDDROP			27				//�h���b�N���h���b�v
#define HELPTOPIC_INCREMENTALSEARCH		28				//�C���N�������^���T�[�`
#define HELPTOPIC_UUDECODE				29				//�t�t�c�d�b�n�c�d
#define HELPTOPIC_BASE64DECODE			30				//�a�������U�S�f�R�[�h
#define HELPTOPIC_QUOTEDPRINTABLE		31				//�p�����������|�o�����������������f�R�[�h
#define HELPTOPIC_DES					33				//�c�d�r�ɂ��Í����^������
#define HELPTOPIC_FILECOMPARE			52				//�t�@�C����r
#define HELPTOPIC_SHELLEXTENSION		32				//�V�F���G�N�X�e���V����
#define HELPTOPIC_DIRCOMPARE			49				//�f�B���N�g����r
#define HELPTOPIC_FILEOPERATION			60				//�t�@�C������
#define HELPTOPIC_URLEMAIL				68				//�t�q�k�A���[���A�h���X
#define HELPTOPIC_KEYBIND				69				//�L�[���蓖��
#define HELPTOPIC_MAIL					76				//���[������M
#define HELPTOPIC_TOOLBAR				24				//�c�[���o�[�̃J�X�^�}�C�Y
//�u�₢���킹���v�̍���
#define HELPTOPIC_CONTACT				22				//�A����
#define HELPTOPIC_COPYRIGHT				23				//���쌠���ɂ���

//////////////////////////
//	�t�@�C����r�ԘA	//
//////////////////////////

#define DIFF_RESULTDISPKIND_ONE			1				//�t�@�C����r���ʂ��P�E�C���h�E�ɕ\��
#define DIFF_RESULTDISPKIND_TWO			2				//�t�@�C����r���ʂ��Q�E�C���h�E�ɕ\��

#define DIFF_LINEKIND_COMMON			0				//���ʍs
#define DIFF_LINEKIND_DELETED			1				//�폜�s
#define DIFF_LINEKIND_ADDED				2				//�ǉ��s

#define DIFF_MAX_WEIGHT					255				//�L���O���t�̍ő�̏d��
#define DIFF_START_NODENO				1				//�X�^�[�g�n�_�̐ߔԍ�

//////////////////////////////
//	�f�B���N�g����r�ԘA	//
//////////////////////////////

#define DIRCMP_LINEKIND_NORMAL			0
#define DIRCMP_LINEKIND_DIFF			1

//////////////////
//	�c�c�d�ԘA	//
//////////////////

#define DDE_SERVICENAME					"Xerxes_DDEServer"
#define DDE_TOPICNAME					"CONTEXT_FILEOPEN"

//////////////////////////////
//	�l�c�h�w�i�̃��S�ԘA	//
//////////////////////////////

//���S�a�l�o�敪
#define LOGO_XERXES		0
#define LOGO_FAVARITE	1

//���S�\���敪
#define LOGO_POS_TILE	0
#define LOGO_POS_SITEI	1

//////////////////////
//	���[���֌W		//
//////////////////////

#define SMTP_ENCODE_BASE64		0			//Base64 �G���R�[�h
#define SMTP_ENCODE_QUOTED		1			//Quoted-Printable�G���R�[�h
#define SMTP_ENCODE_UUENCODE	2			//uuencode

#define SMTP_CONNECT_LAN		0			//�k�`�m�ڑ�
#define SMTP_CONNECT_DIALUP		1			//�_�C�A���A�b�v�ڑ�

#define POP3_AUTHENTICATE_STANDARD	0		//USER,PASS�R�}���h�ɂ�郆�[�U�F��
#define POP3_AUTHENTICATE_APOP		1		//APOP�R�}���h�ɂ�郆�[�U�F��

#define MAIL_ACCOUNT_MAX			8		//�A�J�E���g���ő�l
typedef struct _MAIL_SETTINGS				//�A�J�E���g�ʐݒ���ێ��p�\����
{
	char	m_szMailAddress[64];
	char 	m_szSmtpServer[64];
	char	m_szPopServer[64];
	char	m_szPopUserName[64];
	char	m_szPopPasswd[64];
	BOOL	m_bIsDialup;
	char	m_szDialupEntry[64];
	char	m_szRasUserName[64];
	char	m_szRasPasswd[64];
	BOOL	m_bAutoDisconnect;
	int		m_nEncodeType;
	BOOL	m_bIsAutoSetSubject;
	BOOL	m_bIsSendMessageID;
	BOOL	m_bIsSendDate;
	char 	m_szSignature[1024];
	int		m_nAuthorizationType;
} MAIL_SETTINGS, *LPMAIL_SETTINGS;

//////////////////////////////////
//	�L�[�o�C���h�֌W�\���̒�`	//
//////////////////////////////////
typedef struct _KEY
{
	UINT	nChar;
	char	szKeyName[64];
	BOOL	bIsCtrl;
	BOOL	bIsShift;
	BOOL	bIsAlt;
} KEY, *LPKEY;

typedef struct _KEY_FUNC
{
	char	szFuncName[64];
	DWORD	dwMenuID;
	char	szMenuName[128];
	char	szMenuExplain[256];
} KEY_FUNC, *LPKEY_FUNC;

//�L�[�o�C���h�t�@�C���̃}�W�b�N���[�h��`
#define KEYBIND_MAGICWORD		"This is Key Bind File"

//////////////////////
//	�T�E���h�֘A	//
//////////////////////
typedef struct _SOUND_DEF
{
	BOOL		bIsPlay;
	char		szWaveFileName[_MAX_FNAME];
	HTREEITEM	hItem;
} SOUND_DEF, *LPSOUND_DEF;

//////////////////////
//	���K�\���֘A	//
//////////////////////

//POSIX regcomp�Ɏw�肷�铮��t���O
#ifndef REG_EXTENDED
#define REG_EXTENDED 1
#endif
#ifndef REG_ICASE
#define REG_ICASE (REG_EXTENDED << 1)
#endif
#ifndef REG_NEWLINE
#define REG_NEWLINE (REG_ICASE << 1)
#endif
#ifndef REG_NOSUB
#define REG_NOSUB (REG_NEWLINE << 1)
#endif

//////////////
//	���̑�	//
//////////////

//�_���v�p��`
#define ASCII_DATA "................................ !\"#$%&'()*+'-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_'abcdefghijklmnopqrstuvwxyz{|}~ "

//�P�s�̍ő啶����
#define MAX_BUF	2048


#endif // !defined(EDITOR_DEF_H)


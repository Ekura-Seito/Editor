// Myfeal.cpp : インプリメンテーション ファイル
//
//		FEAL(Fast Data Encipherment Algorithm) Program
//

#include "stdafx.h"
#include <stdio.h>
#include <memory.h>
#include <io.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys\stat.h>
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "Myfeal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeal 定義

//#define		S0(x)		*(rot0 + (0xff & (x)))
//#define		S1(x)		*(rot1 + (0xff & (x)))
#define		S0(x)		(g_rot0[ (0xff & (x)) ])
#define		S1(x)		(g_rot0[ ((0xff & (x)) + 1) ])

/////////////////////////////////////////////////////////////////////////////
// CFeal グローバル変数

const UINT g_rot0[257] = {
	  0,  4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60,
	 64, 68, 72, 76, 80, 84, 88, 92, 96,100,104,108,112,116,120,124,
	128,132,136,140,144,148,152,156,160,164,168,172,176,180,184,188,
	192,196,200,204,208,212,216,220,224,228,232,236,240,244,248,252,
	  1,  5,  9, 13, 17, 21, 25, 29, 33, 37, 41, 45, 49, 53, 57, 61,
	 65, 69, 73, 77, 81, 85, 89, 93, 97,101,105,109,113,117,121,125,
	129,133,137,141,145,149,153,157,161,165,169,173,177,181,185,189,
	193,197,201,205,209,213,217,221,225,229,233,237,241,245,249,253,
	  2,  6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46, 50, 54, 58, 62,
	 66, 70, 74, 78, 82, 86, 90, 94, 98,102,106,110,114,118,122,126,
	130,134,138,142,146,150,154,158,162,166,170,174,178,182,186,190,
	194,198,202,206,210,214,218,222,226,230,234,238,242,246,250,254,
	  3,  7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59, 63,
	 67, 71, 75, 79, 83, 87, 91, 95, 99,103,107,111,115,119,123,127,
	131,135,139,143,147,151,155,159,163,167,171,175,179,183,187,191,
	195,199,203,207,211,215,219,223,227,231,235,239,243,247,251,255, 0 };

/////////////////////////////////////////////////////////////////////////////
// CFeal クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFeal::CFeal()
{
	m_nRotNo=0;
	m_nIniNo=0;
	m_nXorNo=0;

	m_fp=NULL;
	m_f1=m_f2=NULL;
	memset(m_buffer,0,sizeof(m_buffer));
	memset(m_bufo,0,sizeof(m_bufo));
	m_mem=m_mem1=m_nbuf=m_nnbuf=m_count=m_lshift=m_rshift=0;

	m_edflag=-1,m_exflag=0,m_mode=1;
	m_blen=8,m_blena=0,m_llen=64,m_nn=8;
	memset(m_initial,0,sizeof(m_initial));

	m_initial4 = m_initial + 4;
	memset(m_x,(char)0xff,sizeof(m_x));
	m_xb1 = m_xb2 = m_set = m_set4 = 0;
	memset(m_ks,0,sizeof(m_ks));

	memset(m_key,0,sizeof(m_key));
	m_key_nn=0;
	m_kn0=m_kn1=m_kn2=m_kn3=m_kn4=m_kn5=m_kn6=m_kn7=0;
	m_kn8=m_kn9=m_kn10=m_kn11=m_kn12=m_kn13=m_kn14=m_kn15=0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFeal::~CFeal()
{
}

/////////////////////////////////////////////////////////////////////////////
// CFeal パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：メイン																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CFeal::main
	(	int		argc,
		char*	argv[]
	)
												//関数リターン値
												//  0 : 正常終了
												// -1 : パラメータエラー
												// -2 : ファイルアクセスエラー
{
	int fi = 0, fk = 0;

	//1999.03.10 Add
	m_edflag = -1;
	//1999.08.06 Add
	m_fp = NULL;
	m_f1 = NULL;
	m_f2 = NULL;

	if(argc < 2 || *argv[1] != '-')	
	{
		//パラメータエラー
		return -1;
	}
	int k = 1;
	do
	{
		switch(tolower(*(argv[k] + 1)))
		{
		case 'n':	m_nn = atoi(argv[k] + 2);
					if((m_nn < 4) || ((m_nn % 2) != 0))	
					{
						//パラメータエラー
						return -1;
					}
					break;
		case 'x':	m_exflag = 1;
					break;
		case 'm':	m_mode = atoi(argv[k] + 2);
					if(m_mode < 1 || m_mode > 4)
					{
						//パラメータエラー
						return -1;
					}
					break;
		case 'k':	fk = k;
					break;
		case 'f':	fi = k;
					break;
		case 'b':	m_blen = atoi(argv[k] + 2);
					if(m_blen < 1 || m_blen > 8)
					{
						//パラメータエラー
						return -1;
					}
					break;
		case 'l':	m_llen = atoi(argv[k] + 2);
					if(m_llen < 1 || m_llen > 64)
					{
						//パラメータエラー
						return -1;
					}
					break;
		case 'd':	if(m_edflag == 1)	
					{
						//パラメータエラー
						return -1;
					}
					m_edflag = 0;
					break;
		case 'e':	if(m_edflag == 0)	
					{
						//パラメータエラー
						return -1;
					}
					m_edflag = 1;
					break;
		case 'h':	m_fp = fopen("feal.man", "r");
					if(m_fp == NULL)	
					{
						//パラメータエラー
						return -1;
					}
					while(fgets(m_buffer, 256, m_fp) != NULL)
						fprintf(stderr, "%s", m_buffer);
					fclose(m_fp);
					return 0;
		case '?':	
					return 0;
		default:	
					//パラメータエラー
					return -1;
		}
	} while(*argv[++k] == '-');

	if((m_mode == 4 && m_blen * 8 > m_llen) || m_edflag == -1 || (k + 1) >= argc)
	{
		//パラメータエラー
		return -1;
	}
	if(m_mode <= 2 && m_blen != 8)
	{
		m_blen = 8;
	}
	m_blena = 8 - m_blen;


	int k1 = k++;
	int k2 = k;

	if(fk != 0)
	{
		if(*(argv[fk] + 2) == '=')	
			key_set(argv[fk] + 3);
		else if(*(argv[fk] + 2) == ':')
		{
			int i = (m_exflag)? 16: 8;
			char *p = argv[fk] + 3;
			UINT *q = m_ks;
			while(*p != '\0' && i-- != 0)	
				*q++ = *p++;
		}
		else
		{
			char keybuf[32];
			m_f1 = open(argv[fk] + 2, O_RDONLY | O_BINARY);
			if(m_f1 == -1)
			{
				//ファイルアクセスエラー
				return -2;
			}
			read(m_f1, keybuf, (m_exflag)? 32: 16);
			key_set(keybuf);
			close(m_f1);
		}
	}

	if(m_mode >= 2 && fi != 0)
	{
		m_f1 = open(argv[fi] + 2, O_RDONLY | O_BINARY);
		if(m_f1 == -1)
		{
			//ファイルアクセスエラー
			return -2;
		}
		read(m_f1, m_initial, 16);
		initial_set();
		close(m_f1);
	}

	if(m_mode >= 2)
	{
		switch(m_blen)
		{
		case 1:	
			m_nXorNo = 1;
			break;
		case 2:	
			m_nXorNo = 2;
			break;
		case 3:	
			m_nXorNo = 3;
			break;
		case 4:	
			m_nXorNo = 4;
			break;
		case 5:	
			m_nXorNo = 5;
			break;
		case 6:	
			m_nXorNo = 6;
			break;
		case 7:	
			m_nXorNo = 7;
			break;
		case 8:	
			m_nXorNo = 8;
			break;
		}

		if(m_mode == 4)
		{
			m_xb1 = m_x + m_blen;
			m_xb2 = m_x + m_blena;
		}
		if(m_mode >= 3)
		{
			m_set = (m_mode == 3)? m_bufo: m_x;
			if(m_llen == 32)
			{
				m_nIniNo = 1;
				m_nRotNo = 1;
			}
			else if(m_llen == 64)
			{
				m_nIniNo = 2;
				m_nRotNo = 2;
			}
			else if(m_llen < 32)
			{
				m_nIniNo = 3;
				m_nRotNo = 3;
				m_lshift = m_llen;
				m_rshift = 32 - m_lshift;
			}
			else
			{
				m_nIniNo = 4;
				m_nRotNo = 4;

				m_lshift = m_llen - 32;
				m_rshift = 32 - m_lshift;
				m_set4 = m_set + 4;
			}
		}
	}

	m_f1 = open(argv[k1], O_RDONLY | O_BINARY);
	if(m_f1 == -1)
	{
		//ファイルアクセスエラー
		return -2;
	}
	m_fp = fopen(argv[k2], "rb");
	if(m_fp != NULL)
	{
		fclose(m_fp);
		::DeleteFile(argv[k2]);
	}
	m_f2 = open(argv[k2], O_WRONLY | O_CREAT | O_BINARY, S_IREAD | S_IWRITE);


	m_nnbuf = NBUF / m_blen;
	m_mem = m_nbuf = m_nnbuf * m_blen;

	makekey((m_mode <= 2)? m_edflag: 1);

	switch(m_mode)
	{
	case 1:	ecb();		break;
	case 2:	cbc();		break;
	case 3:	ofb();		break;
	case 4:	cfb();		break;
	}
	close(m_f1);
	close(m_f2);
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CFeal プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字－＞ＨＥＸ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CFeal::hexcode(char c)
{
	if(c >= '0' && c <= '9')	return (c - '0');
	else if(c >= 'A' && c <= 'F')	return (c - 'A' + 10);
	else if(c >= 'a' && c <= 'f')	return (c - 'a' + 10);
	else return (c & 0x0f);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：初期設定																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::initial_set(void)
{
	char *p = m_initial, *q=NULL;

//	register int i = 16;
	int i = 16;
	while(*p != '\0' && i-- != 0)	*p++ = hexcode(*p);

	p = q = m_initial;
	i = 8;
	do
	{
		*q++ = (*p * 16) | *(p + 1);
		p += 2;
	} while(--i);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーセット															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::key_set(char *buf)
{
	char *p = buf;
//	register int i = (m_exflag)? 32: 16;
	int i = (m_exflag)? 32: 16;
	while(*p != '\0' && i-- != 0)	*p++ = hexcode(*p);

	UINT *q = m_ks;
	p = buf;
	i = (m_exflag)? 16: 8;
	do
	{
		*q++ = (*p * 16) | *(p + 1);
		p += 2;
	} while(--i);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＣＨＡＲ－＞ＬＯＮＧ													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
ULONG CFeal::char_to_long(char *a)
{
	FEAL_UNION m;
	memset(&m,0,sizeof(FEAL_UNION));
	char *mb0 = m.b, *mb1 = m.b + 1, *mb2 = m.b + 2, *mb3 = m.b + 3;

	*mb3 = a[0];
	*mb2 = a[1];
	*mb1 = a[2];
	*mb0 = a[3];
	return m.a;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＬＯＮＧ－＞ＣＨＡＲ													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::long_to_char(ULONG m1, ULONG m2)
{
	FEAL_UNION m;
	memset(&m,0,sizeof(FEAL_UNION));
	char *mb0 = m.b, *mb1 = m.b + 1, *mb2 = m.b + 2, *mb3 = m.b + 3;

	m.a = m1;
	m_initial[0] = *mb3;
	m_initial[1] = *mb2;
	m_initial[2] = *mb1;
	m_initial[3] = *mb0;
	m.a = m2;
	m_initial[4] = *mb3;
	m_initial[5] = *mb2;
	m_initial[6] = *mb1;
	m_initial[7] = *mb0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ローテーション初期化１												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::rot_initial1(void)
{
	memcpy(m_initial, m_initial4, 4);
	memcpy(m_initial4, m_set, 4);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ローテーション初期化２												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::rot_initial2(void)
{
	memcpy(m_initial, m_set, 8);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ローテーション初期化３												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::rot_initial3(void)
{
	ULONG m2 = char_to_long(m_initial4);
	long_to_char((char_to_long(m_initial) << m_lshift) | (m2 >> m_rshift),
				 (m2 << m_lshift) | (char_to_long(m_set) >> m_rshift));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ローテーション初期化４												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::rot_initial4(void)
{
	ULONG s1 = char_to_long(m_set);
	long_to_char((char_to_long(m_initial4) << m_lshift) | (s1 >> m_rshift),
				 (s1 << m_lshift) | (char_to_long(m_set4) >> m_rshift));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：初期シフト１															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::ini_shift1(void)
{
	memcpy(m_initial4, m_initial, 4);
	memset(m_initial, 0, 4);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：初期シフト２															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::ini_shift2(void)
{
	memset(m_initial, 0, 8);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：初期シフト３															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::ini_shift3(void)
{
	FEAL_UNION m;
	memset(&m,0,sizeof(FEAL_UNION));
	char *mb0 = m.b, *mb1 = m.b + 1, *mb2 = m.b + 2, *mb3 = m.b + 3;

	memset(m_initial, 0, 4);
	m.a = char_to_long(m_initial) >> m_rshift;
	m_initial[4] = *mb3;
	m_initial[5] = *mb2;
	m_initial[6] = *mb1;
	m_initial[7] = *mb0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：初期シフト４															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::ini_shift4(void)
{
	ULONG m1 = char_to_long(m_initial);
	long_to_char((m1 >> m_rshift),
				 ((m1 << m_lshift) | (char_to_long(m_initial4) >> m_rshift)));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＸＯＲ１																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::xor1(char *in, char *out)
{
	out[0] ^= in[0];
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＸＯＲ２																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::xor2(char *in, char *out)
{
	out[0] ^= in[0];
	out[1] ^= in[1];
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＸＯＲ３																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::xor3(char *in, char *out)
{
	out[0] ^= in[0];
	out[1] ^= in[1];
	out[2] ^= in[2];
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＸＯＲ４																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::xor4(char *in, char *out)
{
	out[0] ^= in[0];
	out[1] ^= in[1];
	out[2] ^= in[2];
	out[3] ^= in[3];
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＸＯＲ５																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::xor5(char *in, char *out)
{
	out[0] ^= in[0];
	out[1] ^= in[1];
	out[2] ^= in[2];
	out[3] ^= in[3];
	out[4] ^= in[4];
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＸＯＲ６																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::xor6(char *in, char *out)
{
	out[0] ^= in[0];
	out[1] ^= in[1];
	out[2] ^= in[2];
	out[3] ^= in[3];
	out[4] ^= in[4];
	out[5] ^= in[5];
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＸＯＲ７																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::xor7(char *in, char *out)
{
	out[0] ^= in[0];
	out[1] ^= in[1];
	out[2] ^= in[2];
	out[3] ^= in[3];
	out[4] ^= in[4];
	out[5] ^= in[5];
	out[6] ^= in[6];
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＸＯＲ８																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::xor8(char *in, char *out)
{
	out[0] ^= in[0];
	out[1] ^= in[1];
	out[2] ^= in[2];
	out[3] ^= in[3];
	out[4] ^= in[4];
	out[5] ^= in[5];
	out[6] ^= in[6];
	out[7] ^= in[7];
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キー作成																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::makekey(int flag)
{
	UINT a0 = 0;
	UINT a1 = 0;
	UINT a2 = 0;
	UINT a3 = 0;
	UINT a4 = m_ks[0];
	UINT a5 = m_ks[1];
	UINT a6 = m_ks[2];
	UINT a7 = m_ks[3];
	UINT a8 = m_ks[4];
	UINT a9 = m_ks[5];
	UINT a10 = m_ks[6];
	UINT a11 = m_ks[7];

	UINT s0  = m_ks[8] ^ m_ks[12];
	UINT s1  = m_ks[9] ^ m_ks[13];
	UINT s2  = m_ks[10] ^ m_ks[14];
	UINT s3  = m_ks[11] ^ m_ks[15];
	UINT s4  = m_ks[8];
	UINT s5  = m_ks[9];
	UINT s6  = m_ks[10];
	UINT s7  = m_ks[11];
	UINT s8  = m_ks[12];
	UINT s9  = m_ks[13];
	UINT s10 = m_ks[14];
	UINT s11 = m_ks[15];

//	register int m = m_nn / 2 + 4;
	int m = m_nn / 2 + 4;
	UINT *p = m_key;
	UINT p1=0,p2=0;
	do
	{
		p1 = *(p + 1) = S1((a4 ^ a5) + (a6 ^ a7 ^ a0 ^ a8 ^ s0));
		p2 = *(p + 2) = S0((a6 ^ a7) + (p1 ^ a1 ^ a9 ^ s1));
		a0 = *p       = S0(a4 + ((a1 = p1) ^ a2 ^ a10 ^ s2));
		a3 = *(p + 3) = S1(a7 + ((a2 = p2) ^ a3 ^ a11 ^ s3));
		if(--m == 0)	break;

		p1 = *(p + 5) = S1((a8  ^ a9 ) + (a10 ^ a11 ^ a4 ^ a0 ^ s4));
		p2 = *(p + 6) = S0((a10 ^ a11) + (p1 ^ a5 ^ a1 ^ s5));
		a4 = *(p + 4) = S0(a8  + ((a5 = p1) ^ a6 ^ a2 ^ s6));
		a7 = *(p + 7) = S1(a11 + ((a6 = p2) ^ a7 ^ a3 ^ s7));
		if(--m == 0)	break;

		p1  = *(p + 9)  = S1((a0 ^ a1) + (a2 ^ a3 ^ a8 ^ a4 ^ s8));
		p2  = *(p + 10) = S0((a2 ^ a3) + (p1 ^ a9 ^ a5 ^ s9));
		a8  = *(p + 8)  = S0(a0 + ((a9  = p1) ^ a10 ^ a6 ^ s10));
		a11 = *(p + 11) = S1(a3 + ((a10 = p2) ^ a11 ^ a7 ^ s11));
		p += 12;
	} while(--m);

	if(flag == 0)
	{
		UINT *q = m_key + m_nn * 2 - 2;
		p = m_key;
		m = m_nn / 2;
		do
		{
			UINT work = *p;
			*p   = *q;
			*q   = work;
			work     = *(p + 1);
			*(p + 1) = *(q + 1);
			*(q + 1) = work;
			p += 2;
			q -= 2;
		} while(--m);
	}

	m_key_nn = m_key + m_nn * 2;

	p = (flag)? m_key_nn: m_key_nn + 8;
	m_kn0 = *p;
	m_kn1 = *(p + 1); 
	m_kn2 = *(p + 2); 
	m_kn3 = *(p + 3); 
	m_kn4 = *(p + 4); 
	m_kn5 = *(p + 5); 
	m_kn6 = *(p + 6); 
	m_kn7 = *(p + 7); 

	p = (flag)? m_key_nn + 8: m_key_nn;
	m_kn8  = *p;  
	m_kn9  = *(p + 1); 
	m_kn10 = *(p + 2); 
	m_kn11 = *(p + 3); 
	m_kn12 = *(p + 4); 
	m_kn13 = *(p + 5); 
	m_kn14 = *(p + 6); 
	m_kn15 = *(p + 7);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＦＥＡＬ																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::feal(char *in, char *out)
{
	UINT l0 = in[0] ^ m_kn0;
	UINT l1 = in[1] ^ m_kn1;
	UINT l2 = in[2] ^ m_kn2;
	UINT l3 = in[3] ^ m_kn3;
	UINT r0 = in[4] ^ m_kn4 ^ l0;
	UINT r1 = in[5] ^ m_kn5 ^ l1;
	UINT r2 = in[6] ^ m_kn6 ^ l2;
	UINT r3 = in[7] ^ m_kn7 ^ l3;

	UINT w1=0,w2=0;
	UINT *t = m_key;
	do
	{
		l1 ^= (w1 = S1((r0 ^ r1 ^ *t) + (w2 = r3 ^ r2 ^ *(t + 1))));
		l2 ^= (w2 = S0(w1 + w2));
		l0 ^= S0(r0 + w1);
		l3 ^= S1(r3 + w2);
		r1 ^= (w1 = S1((l0 ^ l1 ^ *(t + 2)) + (w2 = l3 ^ l2 ^ *(t + 3))));
		r2 ^= (w2 = S0(w1 + w2));
		r0 ^= S0(l0 + w1);
		r3 ^= S1(l3 + w2);
	} while((t += 4) != m_key_nn);

	out[0] = r0 ^ m_kn8;
	out[1] = r1 ^ m_kn9;
	out[2] = r2 ^ m_kn10;
	out[3] = r3 ^ m_kn11;
	out[4] = l0 ^ r0 ^ m_kn12;
	out[5] = l1 ^ r1 ^ m_kn13;
	out[6] = l2 ^ r2 ^ m_kn14;
	out[7] = l3 ^ r3 ^ m_kn15;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＥＣＢ																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::ecb(void)
{
	while((m_count = read(m_f1, m_buffer, m_nbuf)) != 0)
	{
		m_mem1 = m_nnbuf;
		char *p = m_buffer;
		if(m_count != m_nbuf)
		{
			m_mem1 = (m_count + 7) / 8;
			m_mem = m_mem1 * 8;
			memset(m_buffer + m_count, 0, m_mem - m_count);
		}
		do
		{
			feal(p, p);
			p += 8;
		} while(--m_mem1);
		write(m_f2, m_buffer, m_mem);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＣＢＣ																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::cbc(void)
{
	while((m_count = read(m_f1, m_buffer, m_nbuf)) != 0)
	{
		m_mem1 = m_nnbuf;
		char *p = m_buffer;
		if(m_count != m_nbuf)
		{
			m_mem1 = (m_count + 7) / 8;
			m_mem = m_mem1 * 8;
			memset(m_buffer + m_count, 0, m_mem - m_count);
		}
		if(m_edflag)
		{
			do
			{
				xor8(m_initial, p);
				feal(p, m_initial);
				memcpy(p, m_initial, 8);
				p += 8;
			} while(--m_mem1);
		}
		else
		{
			do
			{
				feal(p, m_bufo);
				xor8(m_initial, m_bufo);
				memcpy(m_initial, p, 8);
				memcpy(p, m_bufo, 8);
				p += 8;
			} while(--m_mem1);
		}
		write(m_f2, m_buffer, m_mem);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＯＦＢ																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::ofb(void)
{
	while((m_count = read(m_f1, m_buffer, m_nbuf)) != 0)
	{
		m_mem1 = m_nnbuf;
		char *p = m_buffer;
		if(m_count != m_nbuf)
		{
			m_mem1 = (m_count + m_blen - 1) / m_blen;
			m_mem = m_mem1 * m_blen;
			memset(m_buffer + m_count, 0, m_mem - m_count);
		}
		do
		{
			feal(m_initial, m_bufo);

			if(m_nXorNo == 1)
				xor1(m_bufo, p);
			else if(m_nXorNo == 2)
				xor2(m_bufo, p);
			else if(m_nXorNo == 3)
				xor3(m_bufo, p);
			else if(m_nXorNo == 4)
				xor4(m_bufo, p);
			else if(m_nXorNo == 5)
				xor5(m_bufo, p);
			else if(m_nXorNo == 6)
				xor6(m_bufo, p);
			else if(m_nXorNo == 7)
				xor7(m_bufo, p);
			else if(m_nXorNo == 8)
				xor8(m_bufo, p);

			if(m_nRotNo == 1)
				rot_initial1();
			else if(m_nRotNo == 2)
				rot_initial2();
			else if(m_nRotNo == 3)
				rot_initial3();
			else if(m_nRotNo == 4)
				rot_initial4();

			p += m_blen;
		} while(--m_mem1);
		write(m_f2, m_buffer, m_mem);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＣＦＢ																|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFeal::cfb(void)
{
	if(m_nIniNo == 1)
		ini_shift1();
	else if(m_nIniNo == 2)
		ini_shift2();
	else if(m_nIniNo == 3)
		ini_shift3();
	else if(m_nIniNo == 4)
		ini_shift4();

	while((m_count = read(m_f1, m_buffer, m_nbuf)) != 0)
	{
		m_mem1 = m_nnbuf;
		char *p = m_buffer;
		if(m_count != m_nbuf)
		{
			m_mem1 = (m_count + m_blen - 1) / m_blen;
			m_mem = m_mem1 * m_blen;
			memset(m_buffer + m_count, 0, m_mem - m_count);
		}
		if(m_edflag)
		{
			do
			{
				feal(m_initial, m_bufo);

				if(m_nXorNo == 1)
					xor1(m_bufo, p);
				else if(m_nXorNo == 2)
					xor2(m_bufo, p);
				else if(m_nXorNo == 3)
					xor3(m_bufo, p);
				else if(m_nXorNo == 4)
					xor4(m_bufo, p);
				else if(m_nXorNo == 5)
					xor5(m_bufo, p);
				else if(m_nXorNo == 6)
					xor6(m_bufo, p);
				else if(m_nXorNo == 7)
					xor7(m_bufo, p);
				else if(m_nXorNo == 8)
					xor8(m_bufo, p);

				memcpy(m_x, m_xb1, m_blena);
				memcpy(m_xb2, p, m_blen);

				if(m_nRotNo == 1)
					rot_initial1();
				else if(m_nRotNo == 2)
					rot_initial2();
				else if(m_nRotNo == 3)
					rot_initial3();
				else if(m_nRotNo == 4)
					rot_initial4();

				p += m_blen;
			} while(--m_mem1);
		}
		else
		{
			do
			{
				feal(m_initial, m_bufo);
				memcpy(m_x, m_xb1, m_blena);
				memcpy(m_xb2, p, m_blen);

				if(m_nRotNo == 1)
					rot_initial1();
				else if(m_nRotNo == 2)
					rot_initial2();
				else if(m_nRotNo == 3)
					rot_initial3();
				else if(m_nRotNo == 4)
					rot_initial4();

				if(m_nXorNo == 1)
					xor1(m_bufo, p);
				else if(m_nXorNo == 2)
					xor2(m_bufo, p);
				else if(m_nXorNo == 3)
					xor3(m_bufo, p);
				else if(m_nXorNo == 4)
					xor4(m_bufo, p);
				else if(m_nXorNo == 5)
					xor5(m_bufo, p);
				else if(m_nXorNo == 6)
					xor6(m_bufo, p);
				else if(m_nXorNo == 7)
					xor7(m_bufo, p);
				else if(m_nXorNo == 8)
					xor8(m_bufo, p);

				p += m_blen;
			} while(--m_mem1);
		}
		write(m_f2, m_buffer, m_mem);
	}
}


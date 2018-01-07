// Myfeal.h : CFeal クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(FEAL_H)		//２重インクルード防止
#define FEAL_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define NBUF  51200		// 50kB
#define NBUF8 6400		// = NBUF / 8
#define NMAX  128		// maximum value of N

//共用体
typedef union
{
	ULONG a;
	char b[4];
}FEAL_UNION;


class CFeal
{
public:
	CFeal();							// コンストラクタ
	virtual ~CFeal();					// デストラクタ

	int main(int argc, char *argv[]);

protected:
	ULONG char_to_long(char *a);
	void long_to_char(ULONG m1, ULONG m2);
	int hexcode(char c);
	void initial_set(void);
	void key_set(char *buf);
	void rot_initial1(void);
	void rot_initial2(void);
	void rot_initial3(void);
	void rot_initial4(void);
	void ini_shift1(void);
	void ini_shift2(void);
	void ini_shift3(void);
	void ini_shift4(void);
	void xor1(char *in, char *out);
	void xor2(char *in, char *out);
	void xor3(char *in, char *out);
	void xor4(char *in, char *out);
	void xor5(char *in, char *out);
	void xor6(char *in, char *out);
	void xor7(char *in, char *out);
	void xor8(char *in, char *out);
	void makekey(int flag);
	void feal(char *in, char *out);
	void ecb(void);
	void cbc(void);
	void ofb(void);
	void cfb(void);

private:
	int m_nRotNo;
	int m_nIniNo;
	int m_nXorNo;

	FILE*	m_fp;
	int 	m_f1, m_f2;
	char 	m_buffer[NBUF], m_bufo[8];
	unsigned int m_mem, m_mem1, m_nbuf, m_nnbuf, m_count, m_lshift, m_rshift;

	int 	m_edflag, m_exflag, m_mode;
	int 	m_blen, m_blena, m_llen, m_nn;
	char 	m_initial[16];

	char*	m_initial4;
	char	m_x[8];
	char 	*m_xb1, *m_xb2, *m_set, *m_set4;
	UINT 	m_ks[16];

	UINT 	m_key[NMAX * 2 + 16];
	UINT 	*m_key_nn;
	UINT 	m_kn0, m_kn1, m_kn2, m_kn3, m_kn4, m_kn5, m_kn6, m_kn7;
	UINT 	m_kn8, m_kn9, m_kn10, m_kn11, m_kn12, m_kn13, m_kn14, m_kn15;
};

#endif // !defined(FEAL_H)								//２重インクルード防止


// MyDes.h : CDes クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(DES_H)		//２重インクルード防止
#define DES_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDes
{
public:
	CDes();								// コンストラクタ
	virtual ~CDes();					// デストラクタ

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


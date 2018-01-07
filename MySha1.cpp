// MySha1.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor.h"
#include "Editor_Def.h"
#include "MySha1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

byte const SHADERprefix[] = 
{
	0x30, /* Universal, Constructed, Sequence */
	0x21, /* Length 33 (bytes following) */
	0x30, /* Universal, Constructed, Sequence */
	0x09, /* Length 9 */
	0x06, /* Universal, Primitive, object-identifier */
	0x05, /* Length 8 */
	43, /* 43 = ISO(1)*40 + 3 */
	14,
	3,
	2,
	26,
	0x05, /* Universal, Primitive, NULL */
	0x00, /* Length 0 */
	0x04, /* Universal, Primitive, Octet string */
	0x14 /* Length 20 */
		 /* 20 SHA.1 digest bytes go here */
};

//struct PgpHash const HashSHA = 
//{
//	"SHA1", 
//	PGP_HASH_SHA,
//	SHADERprefix, 
//	sizeof(SHADERprefix),
//	SHA_HASHBYTES,
//	sizeof(struct SHAContext),
//	sizeof(struct aaa{char _a; struct SHAContext _b;}) - sizeof(struct SHAContext),
//	shaInit, 
//	shaUpdate, 
//	shaFinal
//};


/////////////////////////////////////////////////////////////////////////////
// CSha1 クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CSha1::CSha1()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CSha1::~CSha1()
{
}

/////////////////////////////////////////////////////////////////////////////
// CSha1 パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	関数名	：SHA1String															|*/
/*|	概要	：文字列のＳＨＡ１ハッシュ値取得										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	Author	：	1999.08.20	S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSha1::SHA1String(char* szIn, int nLen, char* szOut)
{
	struct SHAContext sha;
	byte const *hash;
	long i;

	shaInit(&sha);
	shaUpdate(&sha, (byte *)szIn, nLen);
	hash = shaFinal(&sha);

	for(i = 0; i < SHA_HASHBYTES; i++)
		sprintf(szOut+2*i, "%02X", hash[i]);
}

/////////////////////////////////////////////////////////////////////////////
// CSha1 プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	関数名	：shaInit																|*/
/*|	概要	：Initialize the SHA values												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	Author	：	1999.08.20	S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSha1::shaInit(void *priv)
{
	struct SHAContext *ctx = (struct SHAContext *)priv;

	/* Set the h-vars to their initial values */
	ctx->iv[0] = 0x67452301;
	ctx->iv[1] = 0xEFCDAB89;
	ctx->iv[2] = 0x98BADCFE;
	ctx->iv[3] = 0x10325476;
	ctx->iv[4] = 0xC3D2E1F0;

	/* Initialise bit count */
#ifdef HAVE64
	ctx->bytes = 0;
#else
	ctx->bytesHi = 0;
	ctx->bytesLo = 0;
#endif
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	関数名	：shaUpdate																|*/
/*|	概要	：Update SHA for a block of data										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	Author	：	1999.08.20	S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSha1::shaUpdate(void *priv, byte const *buf, size_t len)
{
	struct SHAContext *ctx = (struct SHAContext *)priv;
	unsigned i;

	/* Update bitcount */
#ifdef HAVE64
	i = (unsigned)ctx->bytes % SHA_BLOCKBYTES;
	ctx->bytes += len;
#else
	word32 t = ctx->bytesLo;
	if ( ( ctx->bytesLo = t + len ) < t )
		ctx->bytesHi++; /* Carry from low to high */
	i = (unsigned)t % SHA_BLOCKBYTES; /* Bytes already in ctx->key */
#endif

	/* i is always less than SHA_BLOCKBYTES. */
	if (SHA_BLOCKBYTES-i > len) {
		 memcpy((byte *)ctx->key + i, buf, len);
		 return;
	}

	if (i) { /* First chunk is an odd size */
		memcpy((byte *)ctx->key + i, buf, SHA_BLOCKBYTES - i);
		shaByteSwap(ctx->key, (byte *)ctx->key, SHA_BLOCKWORDS);
		shaTransform(ctx);
		buf += SHA_BLOCKBYTES-i;
		len -= SHA_BLOCKBYTES-i;
	}

	/* Process data in 64-byte chunks */
	while (len >= SHA_BLOCKBYTES) {
		shaByteSwap(ctx->key, buf, SHA_BLOCKWORDS);
		shaTransform(ctx);
		buf += SHA_BLOCKBYTES;
		len -= SHA_BLOCKBYTES;
	}

	/* Handle any remaining bytes of data. */
	if (len)
		memcpy(ctx->key, buf, len);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	関数名	：shaFinal																|*/
/*|	概要	：Final wrapup															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	Author	：	1999.08.20	S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
byte const * CSha1::shaFinal(void *priv)
{
	struct SHAContext *ctx = (struct SHAContext *)priv;
	byte *digest;
#if HAVE64
	unsigned i = (unsigned)ctx->bytes % SHA_BLOCKBYTES;
#else
	unsigned i = (unsigned)ctx->bytesLo % SHA_BLOCKBYTES;
#endif
	byte *p = (byte *)ctx->key + i; /* First unused byte */
	word32 t;

	/* Set the first char of padding to 0x80. There is always room. */
	*p++ = 0x80;

	/* Bytes of padding needed to make 64 bytes (0..63) */
	i = SHA_BLOCKBYTES - 1 - i;

	if (i < 8) { /* Padding forces an extra block */
		memset(p, 0, i);
		shaByteSwap(ctx->key, (byte *)ctx->key, 16);
		shaTransform(ctx);
		p = (byte *)ctx->key;
		i = 64;
	}
	memset(p, 0, i-8);
	shaByteSwap(ctx->key, (byte *)ctx->key, 14);

	/* Append length in bits and transform */
#if HAVE64
	ctx->key[14] = (word32)(ctx->bytes >> 29);
	ctx->key[15] = (word32)ctx->bytes << 3;
#else
	ctx->key[14] = ctx->bytesHi << 3 | ctx->bytesLo >> 29;
	ctx->key[15] = ctx->bytesLo << 3;
#endif
	shaTransform(ctx);

	digest = (byte *)ctx->iv;
	for (i = 0; i < SHA_HASHWORDS; i++) {
		t = ctx->iv[i];
		digest[0] = (byte)(t >> 24);
		digest[1] = (byte)(t >> 16);
		digest[2] = (byte)(t >> 8);
		digest[3] = (byte)t;
		digest += 4;
	}
	/* In case it's sensitive */
	/* XXX memset(ctx, 0, sizeof(ctx)); */
	 return (byte const *)ctx->iv;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	関数名	：shaByteSwap															|*/
/*|	概要	：Shuffle the bytes into big-endian order within words, as per the SHA spec.|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	Author	：	1999.08.20	S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSha1::shaByteSwap(word32 *dest, byte const *src, unsigned words)
{
	do 
	{
		*dest++ = (word32)((unsigned)src[0] << 8 | src[1]) << 16 | ((unsigned)src[2] << 8 | src[3]);
		src += 4;
	} while (--words);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	関数名	：shaTransform															|*/
/*|	概要	：Perform the SHA transformation										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	Author	：	1999.08.20	S.Ekura													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CSha1::shaTransform(struct SHAContext *sha)
{
	register word32 A, B, C, D, E;
#if SHA_VERSION
	register word32 t;
#endif

	/* Set up first buffer */
	A = sha->iv[0];
	B = sha->iv[1];
	C = sha->iv[2];
	D = sha->iv[3];
	E = sha->iv[4];

	/* Heavy mangling, in 4 sub-rounds of 20 interations each. */
	subRound( A, B, C, D, E, f1, K2, sha->key[ 0] );
	subRound( E, A, B, C, D, f1, K2, sha->key[ 1] );
	subRound( D, E, A, B, C, f1, K2, sha->key[ 2] );
	subRound( C, D, E, A, B, f1, K2, sha->key[ 3] );
	subRound( B, C, D, E, A, f1, K2, sha->key[ 4] );
	subRound( A, B, C, D, E, f1, K2, sha->key[ 5] );
	subRound( E, A, B, C, D, f1, K2, sha->key[ 6] );
	subRound( D, E, A, B, C, f1, K2, sha->key[ 7] );
	subRound( C, D, E, A, B, f1, K2, sha->key[ 8] );
	subRound( B, C, D, E, A, f1, K2, sha->key[ 9] );
	subRound( A, B, C, D, E, f1, K2, sha->key[10] );
	subRound( E, A, B, C, D, f1, K2, sha->key[11] );
	subRound( D, E, A, B, C, f1, K2, sha->key[12] );
	subRound( C, D, E, A, B, f1, K2, sha->key[13] );
	subRound( B, C, D, E, A, f1, K2, sha->key[14] );
	subRound( A, B, C, D, E, f1, K2, sha->key[15] );
	subRound( E, A, B, C, D, f1, K2, expand(sha->key, 16) );
	subRound( D, E, A, B, C, f1, K2, expand(sha->key, 17) );
	subRound( C, D, E, A, B, f1, K2, expand(sha->key, 18) );
	subRound( B, C, D, E, A, f1, K2, expand(sha->key, 19) );

	subRound( A, B, C, D, E, f2, K3, expand(sha->key, 20) );
	subRound( E, A, B, C, D, f2, K3, expand(sha->key, 21) );
	subRound( D, E, A, B, C, f2, K3, expand(sha->key, 22) );
	subRound( C, D, E, A, B, f2, K3, expand(sha->key, 23) );
	subRound( B, C, D, E, A, f2, K3, expand(sha->key, 24) );
	subRound( A, B, C, D, E, f2, K3, expand(sha->key, 25) );
	subRound( E, A, B, C, D, f2, K3, expand(sha->key, 26) );
	subRound( D, E, A, B, C, f2, K3, expand(sha->key, 27) );
	subRound( C, D, E, A, B, f2, K3, expand(sha->key, 28) );
	subRound( B, C, D, E, A, f2, K3, expand(sha->key, 29) );
	subRound( A, B, C, D, E, f2, K3, expand(sha->key, 30) );
	subRound( E, A, B, C, D, f2, K3, expand(sha->key, 31) );
	subRound( D, E, A, B, C, f2, K3, expand(sha->key, 32) );
	subRound( C, D, E, A, B, f2, K3, expand(sha->key, 33) );
	subRound( B, C, D, E, A, f2, K3, expand(sha->key, 34) );
	subRound( A, B, C, D, E, f2, K3, expand(sha->key, 35) );
	subRound( E, A, B, C, D, f2, K3, expand(sha->key, 36) );
	subRound( D, E, A, B, C, f2, K3, expand(sha->key, 37) );
	subRound( C, D, E, A, B, f2, K3, expand(sha->key, 38) );
	subRound( B, C, D, E, A, f2, K3, expand(sha->key, 39) );

	subRound( A, B, C, D, E, f3, K5, expand(sha->key, 40) );
	subRound( E, A, B, C, D, f3, K5, expand(sha->key, 41) );
	subRound( D, E, A, B, C, f3, K5, expand(sha->key, 42) );
	subRound( C, D, E, A, B, f3, K5, expand(sha->key, 43) );
	subRound( B, C, D, E, A, f3, K5, expand(sha->key, 44) );
	subRound( A, B, C, D, E, f3, K5, expand(sha->key, 45) );
	subRound( E, A, B, C, D, f3, K5, expand(sha->key, 46) );
	subRound( D, E, A, B, C, f3, K5, expand(sha->key, 47) );
	subRound( C, D, E, A, B, f3, K5, expand(sha->key, 48) );
	subRound( B, C, D, E, A, f3, K5, expand(sha->key, 49) );
	subRound( A, B, C, D, E, f3, K5, expand(sha->key, 50) );
	subRound( E, A, B, C, D, f3, K5, expand(sha->key, 51) );
	subRound( D, E, A, B, C, f3, K5, expand(sha->key, 52) );
	subRound( C, D, E, A, B, f3, K5, expand(sha->key, 53) );
	subRound( B, C, D, E, A, f3, K5, expand(sha->key, 54) );
	subRound( A, B, C, D, E, f3, K5, expand(sha->key, 55) );
	subRound( E, A, B, C, D, f3, K5, expand(sha->key, 56) );
	subRound( D, E, A, B, C, f3, K5, expand(sha->key, 57) );
	subRound( C, D, E, A, B, f3, K5, expand(sha->key, 58) );
	subRound( B, C, D, E, A, f3, K5, expand(sha->key, 59) );

	subRound( A, B, C, D, E, f4, K10, expand(sha->key, 60) );
	subRound( E, A, B, C, D, f4, K10, expand(sha->key, 61) );
	subRound( D, E, A, B, C, f4, K10, expand(sha->key, 62) );
	subRound( C, D, E, A, B, f4, K10, expand(sha->key, 63) );
	subRound( B, C, D, E, A, f4, K10, expand(sha->key, 64) );
	subRound( A, B, C, D, E, f4, K10, expand(sha->key, 65) );
	subRound( E, A, B, C, D, f4, K10, expand(sha->key, 66) );
	subRound( D, E, A, B, C, f4, K10, expand(sha->key, 67) );
	subRound( C, D, E, A, B, f4, K10, expand(sha->key, 68) );
	subRound( B, C, D, E, A, f4, K10, expand(sha->key, 69) );
	subRound( A, B, C, D, E, f4, K10, expand(sha->key, 70) );
	subRound( E, A, B, C, D, f4, K10, expand(sha->key, 71) );
	subRound( D, E, A, B, C, f4, K10, expand(sha->key, 72) );
	subRound( C, D, E, A, B, f4, K10, expand(sha->key, 73) );
	subRound( B, C, D, E, A, f4, K10, expand(sha->key, 74) );
	subRound( A, B, C, D, E, f4, K10, expand(sha->key, 75) );
	subRound( E, A, B, C, D, f4, K10, expand(sha->key, 76) );
	subRound( D, E, A, B, C, f4, K10, expandx(sha->key, 77) );
	subRound( C, D, E, A, B, f4, K10, expandx(sha->key, 78) );
	subRound( B, C, D, E, A, f4, K10, expandx(sha->key, 79) );

	/* Build message digest */
	sha->iv[0] += A;
	sha->iv[1] += B;
	sha->iv[2] += C;
	sha->iv[3] += D;
	sha->iv[4] += E;
}


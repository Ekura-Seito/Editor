// MySha1.h : CSha1 クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(SHA1_H)		//２重インクルード防止
#define SHA1_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//Contents of pgpTypes.h
typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef char			VoidAlign;
typedef uchar		 	Boolean;
#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif
#ifndef 	alignof		// Lookie here! An ANSI-compliant alignment finder!
#define 	alignof(type) (sizeof(struct{type _x; char _y;}) - sizeof(type))
#endif

//Contents of pgpUsuals.h
typedef unsigned char byte;
typedef signed char int8;
typedef unsigned short word16;
typedef short int16;
typedef uint word32;
typedef int int32;
#define wipe(x) memset(x, 0, sizeof(*(x)))
#ifndef min
#define min(x,y) (((x)<(y)) ? (x) : (y))
#endif
#ifndef max
#define max(x,y) (((x)>(y)) ? (x) : (y))
#endif
typedef int PgpVersion;
#define PGPVERSION_2	2	 /* 2.0 through 2.5 */
#define PGPVERSION_2_6	3	/* 2.6.x */
#define PGPVERSION_3 4 /* 3.0 */
//extern char const pgpLibVersionString[];
#define IVLEN	10		//The PGP Library Cipher IV Length
#define PGP_LITERAL_TEXT		'\164' /* Ascii 't' */
#define PGP_LITERAL_BINARY		'\142' /* Ascii 'b' */

//Contents of pgpHash.h
struct PgpHash //A static description of a hash
{
	char const *name;
	byte type;
	byte const *DERprefix; /* DER-encoded prefix */
	unsigned DERprefixsize; /* DER-encoded prefix length */
	unsigned hashsize; /* Bytes of output */
	unsigned context_size;
	unsigned context_align;
	void (*init) (void *priv);
	void (*update) (void *priv, byte const *buf, size_t len);
	byte const * (*final) (void *priv);
};
#ifndef TYPE_PGPHASH
#define TYPE_PGPHASH 1
typedef struct PgpHash PgpHash;
#endif
struct PgpHashContext //A way to keep the two together
{
	struct PgpHash const *hash;
	void *priv;
};
#ifndef TYPE_PGPHASHCONTEXT
#define TYPE_PGPHASHCONTEXT 1
typedef struct PgpHashContext PgpHashContext;
#endif
/* Macros to invoke on a struct HashContext */
#define pgpHashInit(hc) (hc)->hash->init((hc)->priv)
#define pgpHashUpdate(hc, buf, len) (hc)->hash->update((hc)->priv, buf, len)
#define pgpHashFinal(hc) (hc)->hash->final((hc)->priv)
//struct PgpHashContext *pgpHashCreate(struct PgpHash const *h);
//void   pgpHashDestroy(struct PgpHashContext *hc);
//struct PgpHashContext *pgpHashClone(struct PgpHashContext const *hc);
//void   pgpHashCopy(struct PgpHashContext *dest,struct PgpHashContext const *src);
//struct PgpHash const *pgpHashByNumber(byte type);
//struct PgpHash const *pgpHashByName(char const *name, size_t namelen);
//int    pgpHashListCreate(byte const *buf, struct PgpHashContext **hashes, unsigned len);
//void   pgpHashListDestroy(struct PgpHashContext *hashes, unsigned len);
//struct PgpHashContext *pgpHashListFind( struct PgpHashContext *hashes, unsigned len, struct PgpHash const *h);
//Hash prefixes and lengths for argument lists (preprocessor evil)
#define PGP_HASH_MD5 1
#define PGP_HASH_SHA 2
#define PGP_HASH_RIPEMD160 3


extern struct PgpHash const HashSHA;

//Define to 1 for FIPS 180.1 version (with extra rotate in prescheduling),
//0 for FIPS 180 version (with the mysterious "weakness" that the NSA
//isn't talking about).
#define SHA_VERSION 1
#define SHA_BLOCKBYTES 64
#define SHA_BLOCKWORDS 16
#define SHA_HASHBYTES 20
#define SHA_HASHWORDS 5

struct SHAContext 
{
	word32 key[SHA_BLOCKWORDS];
	word32 iv[SHA_HASHWORDS];
#if HAVE64
	word64 bytes;
#else
	word32 bytesHi, bytesLo;
#endif
};

//The SHA f()-functions
/* f1 is a bit-select function. If (x) then y else z */
/*#define f1(x,y,z) ( (x & y) | (~x & z) )  // Rounds 0-19 */
#define f1(x,y,z) ( z ^ (x & (y ^ z) ) )  /* Rounds 0-19 */
#define f2(x,y,z) ( x ^ y ^ z )   /* Rounds 20-39 */
/* f3 is a majority function */
/*#define f3(x,y,z) ( (x & y) | (y & z) | (z & x) ) // Rounds 40-59 */
/*#define f3(x,y,z) ( (x & y) | (z & (x | y) ) ) // Rounds 40-59 */
#define f3(x,y,z) ( (x & y) + (z & (x ^ y) ) ) /* Rounds 40-59 */
#define f4(x,y,z) ( x ^ y ^ z )   /* Rounds 60-79 */

/* The SHA Mysterious Constants. */
#define K2 0x5A827999L /* Rounds 0-19 - floor(sqrt(2) * 2^30) */
#define K3 0x6ED9EBA1L /* Rounds 20-39 - floor(sqrt(3) * 2^30) */
#define K5 0x8F1BBCDCL /* Rounds 40-59 - floor(sqrt(5) * 2^30) */
#define K10 0xCA62C1D6L /* Rounds 60-79 - floor(sqrt(10) * 2^30) */
/* I wonder why not use K7=0xA953FD4E, K11=0xD443949F or K13=0xE6C15A23 */

/* 32-bit rotate left - kludged with shifts */
#define ROTL(n,X) ( (X << n) | (X >> (32-n)) )

//The initial expanding function
#if SHA_VERSION /* FIPS 180.1 */
#define expandx(W,i) (t = W[i&15] ^ W[(i-14)&15] ^ W[(i-8)&15] ^ W[(i-3)&15], \
ROTL(1, t))
#define expand(W,i) (W[i&15] = expandx(W,i))
#else /* Old FIPS 180 */
#define expandx(W,i) (W[i&15] ^ W[(i-14)&15] ^ W[(i-8)&15] ^ W[(i-3)&15])
#define expand(W,i) (W[i&15] ^= W[(i-14)&15] ^ W[(i-8)&15] ^ W[(i-3)&15])
#endif

//The prototype SHA sub-round
#define subRound(a, b, c, d, e, f, k, data) \
 ( e += ROTL(5,a) + f(b, c, d) + k + data, b = ROTL(30, b) )


class CSha1
{
public:
	CSha1();								// コンストラクタ
	virtual ~CSha1();						// デストラクタ

	void SHA1String(char*, int, char*);

protected:
	void shaInit(void*);
	void shaUpdate(void*, byte const *, size_t);
	byte const *shaFinal(void*);
	void shaTransform(struct SHAContext *);
	void shaByteSwap(word32 *, byte const *, unsigned);
};

#endif //!defined(SHA1_H)


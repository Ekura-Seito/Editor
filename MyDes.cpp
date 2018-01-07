// MyDes.cpp : �C���v�������e�[�V���� �t�@�C��
//
//		DES(Data Encryption Standard) Program
//

#include "stdafx.h"
//#include <windows.h>
//#include <windowsx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <io.h>
#include "MyDes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDes ��`

#ifndef O_BINARY
#define O_BINARY 0
#endif /* O_BINARY */

#define	ror4(x)	((x) >> 4 | (x) << 28)
#define	rol1(x)	((x) << 1 | (x) >> 31)
#define DES_DIRMASK	001
#define DES_ENCRYPT	(0 * DES_DIRMASK)
#define DES_DECRYPT	(1 * DES_DIRMASK)
#define DES_DEVMASK	002
#define DES_HW		(0 * DES_DEVMASK)
#define DES_SW		(1 * DES_DEVMASK)
#define DESERR_NONE	0
#define DESERR_NOHWDEVICE 1
#define DESERR_HWERROR	2
#define DESERR_BADPARAM	3
#define	DES_FAILED(err)	((err) > DESERR_NOHWDEVICE)
#define BLKSIZE 8192
#define	YES	1
#define	NO	0
#define	C	28
#define	D	0

unsigned long Psboxes[8][64] = {
	{
	0x00808200, 0x00000000, 0x00008000, 0x00808202,
	0x00808002, 0x00008202, 0x00000002, 0x00008000,
	0x00000200, 0x00808200, 0x00808202, 0x00000200,
	0x00800202, 0x00808002, 0x00800000, 0x00000002,
	0x00000202, 0x00800200, 0x00800200, 0x00008200,
	0x00008200, 0x00808000, 0x00808000, 0x00800202,
	0x00008002, 0x00800002, 0x00800002, 0x00008002,
	0x00000000, 0x00000202, 0x00008202, 0x00800000,
	0x00008000, 0x00808202, 0x00000002, 0x00808000,
	0x00808200, 0x00800000, 0x00800000, 0x00000200,
	0x00808002, 0x00008000, 0x00008200, 0x00800002,
	0x00000200, 0x00000002, 0x00800202, 0x00008202,
	0x00808202, 0x00008002, 0x00808000, 0x00800202,
	0x00800002, 0x00000202, 0x00008202, 0x00808200,
	0x00000202, 0x00800200, 0x00800200, 0x00000000,
	0x00008002, 0x00008200, 0x00000000, 0x00808002,
	},
	{
	0x40084010, 0x40004000, 0x00004000, 0x00084010,
	0x00080000, 0x00000010, 0x40080010, 0x40004010,
	0x40000010, 0x40084010, 0x40084000, 0x40000000,
	0x40004000, 0x00080000, 0x00000010, 0x40080010,
	0x00084000, 0x00080010, 0x40004010, 0x00000000,
	0x40000000, 0x00004000, 0x00084010, 0x40080000,
	0x00080010, 0x40000010, 0x00000000, 0x00084000,
	0x00004010, 0x40084000, 0x40080000, 0x00004010,
	0x00000000, 0x00084010, 0x40080010, 0x00080000,
	0x40004010, 0x40080000, 0x40084000, 0x00004000,
	0x40080000, 0x40004000, 0x00000010, 0x40084010,
	0x00084010, 0x00000010, 0x00004000, 0x40000000,
	0x00004010, 0x40084000, 0x00080000, 0x40000010,
	0x00080010, 0x40004010, 0x40000010, 0x00080010,
	0x00084000, 0x00000000, 0x40004000, 0x00004010,
	0x40000000, 0x40080010, 0x40084010, 0x00084000,
	},
	{
	0x00000104, 0x04010100, 0x00000000, 0x04010004,
	0x04000100, 0x00000000, 0x00010104, 0x04000100,
	0x00010004, 0x04000004, 0x04000004, 0x00010000,
	0x04010104, 0x00010004, 0x04010000, 0x00000104,
	0x04000000, 0x00000004, 0x04010100, 0x00000100,
	0x00010100, 0x04010000, 0x04010004, 0x00010104,
	0x04000104, 0x00010100, 0x00010000, 0x04000104,
	0x00000004, 0x04010104, 0x00000100, 0x04000000,
	0x04010100, 0x04000000, 0x00010004, 0x00000104,
	0x00010000, 0x04010100, 0x04000100, 0x00000000,
	0x00000100, 0x00010004, 0x04010104, 0x04000100,
	0x04000004, 0x00000100, 0x00000000, 0x04010004,
	0x04000104, 0x00010000, 0x04000000, 0x04010104,
	0x00000004, 0x00010104, 0x00010100, 0x04000004,
	0x04010000, 0x04000104, 0x00000104, 0x04010000,
	0x00010104, 0x00000004, 0x04010004, 0x00010100,
	},
	{
	0x80401000, 0x80001040, 0x80001040, 0x00000040,
	0x00401040, 0x80400040, 0x80400000, 0x80001000,
	0x00000000, 0x00401000, 0x00401000, 0x80401040,
	0x80000040, 0x00000000, 0x00400040, 0x80400000,
	0x80000000, 0x00001000, 0x00400000, 0x80401000,
	0x00000040, 0x00400000, 0x80001000, 0x00001040,
	0x80400040, 0x80000000, 0x00001040, 0x00400040,
	0x00001000, 0x00401040, 0x80401040, 0x80000040,
	0x00400040, 0x80400000, 0x00401000, 0x80401040,
	0x80000040, 0x00000000, 0x00000000, 0x00401000,
	0x00001040, 0x00400040, 0x80400040, 0x80000000,
	0x80401000, 0x80001040, 0x80001040, 0x00000040,
	0x80401040, 0x80000040, 0x80000000, 0x00001000,
	0x80400000, 0x80001000, 0x00401040, 0x80400040,
	0x80001000, 0x00001040, 0x00400000, 0x80401000,
	0x00000040, 0x00400000, 0x00001000, 0x00401040,
	},
	{
	0x00000080, 0x01040080, 0x01040000, 0x21000080,
	0x00040000, 0x00000080, 0x20000000, 0x01040000,
	0x20040080, 0x00040000, 0x01000080, 0x20040080,
	0x21000080, 0x21040000, 0x00040080, 0x20000000,
	0x01000000, 0x20040000, 0x20040000, 0x00000000,
	0x20000080, 0x21040080, 0x21040080, 0x01000080,
	0x21040000, 0x20000080, 0x00000000, 0x21000000,
	0x01040080, 0x01000000, 0x21000000, 0x00040080,
	0x00040000, 0x21000080, 0x00000080, 0x01000000,
	0x20000000, 0x01040000, 0x21000080, 0x20040080,
	0x01000080, 0x20000000, 0x21040000, 0x01040080,
	0x20040080, 0x00000080, 0x01000000, 0x21040000,
	0x21040080, 0x00040080, 0x21000000, 0x21040080,
	0x01040000, 0x00000000, 0x20040000, 0x21000000,
	0x00040080, 0x01000080, 0x20000080, 0x00040000,
	0x00000000, 0x20040000, 0x01040080, 0x20000080,
	},
	{
	0x10000008, 0x10200000, 0x00002000, 0x10202008,
	0x10200000, 0x00000008, 0x10202008, 0x00200000,
	0x10002000, 0x00202008, 0x00200000, 0x10000008,
	0x00200008, 0x10002000, 0x10000000, 0x00002008,
	0x00000000, 0x00200008, 0x10002008, 0x00002000,
	0x00202000, 0x10002008, 0x00000008, 0x10200008,
	0x10200008, 0x00000000, 0x00202008, 0x10202000,
	0x00002008, 0x00202000, 0x10202000, 0x10000000,
	0x10002000, 0x00000008, 0x10200008, 0x00202000,
	0x10202008, 0x00200000, 0x00002008, 0x10000008,
	0x00200000, 0x10002000, 0x10000000, 0x00002008,
	0x10000008, 0x10202008, 0x00202000, 0x10200000,
	0x00202008, 0x10202000, 0x00000000, 0x10200008,
	0x00000008, 0x00002000, 0x10200000, 0x00202008,
	0x00002000, 0x00200008, 0x10002008, 0x00000000,
	0x10202000, 0x10000000, 0x00200008, 0x10002008,
	},
	{
	0x00100000, 0x02100001, 0x02000401, 0x00000000,
	0x00000400, 0x02000401, 0x00100401, 0x02100400,
	0x02100401, 0x00100000, 0x00000000, 0x02000001,
	0x00000001, 0x02000000, 0x02100001, 0x00000401,
	0x02000400, 0x00100401, 0x00100001, 0x02000400,
	0x02000001, 0x02100000, 0x02100400, 0x00100001,
	0x02100000, 0x00000400, 0x00000401, 0x02100401,
	0x00100400, 0x00000001, 0x02000000, 0x00100400,
	0x02000000, 0x00100400, 0x00100000, 0x02000401,
	0x02000401, 0x02100001, 0x02100001, 0x00000001,
	0x00100001, 0x02000000, 0x02000400, 0x00100000,
	0x02100400, 0x00000401, 0x00100401, 0x02100400,
	0x00000401, 0x02000001, 0x02100401, 0x02100000,
	0x00100400, 0x00000000, 0x00000001, 0x02100401,
	0x00000000, 0x00100401, 0x02100000, 0x00000400,
	0x02000001, 0x02000400, 0x00000400, 0x00100001,
	},
	{
	0x08000820, 0x00000800, 0x00020000, 0x08020820,
	0x08000000, 0x08000820, 0x00000020, 0x08000000,
	0x00020020, 0x08020000, 0x08020820, 0x00020800,
	0x08020800, 0x00020820, 0x00000800, 0x00000020,
	0x08020000, 0x08000020, 0x08000800, 0x00000820,
	0x00020800, 0x00020020, 0x08020020, 0x08020800,
	0x00000820, 0x00000000, 0x00000000, 0x08020020,
	0x08000020, 0x08000800, 0x00020820, 0x00020000,
	0x00020820, 0x00020000, 0x08020800, 0x00000800,
	0x00000020, 0x08020020, 0x00000800, 0x00020820,
	0x08000800, 0x00000020, 0x08000020, 0x08020000,
	0x08020020, 0x08000000, 0x00020000, 0x08000820,
	0x00000000, 0x08020820, 0x00020020, 0x08000020,
	0x08020000, 0x08000800, 0x08000820, 0x00000000,
	0x08020820, 0x00020800, 0x00020800, 0x00000820,
	0x00000820, 0x00020020, 0x08000000, 0x08020800,
	},
};
#define ps(i, j)	Psboxes[i][j]

/////////////////////////////////////////////////////////////////////////////
// CDes �O���[�o���ϐ�

unsigned long Keys[16][2];
unsigned long Ip_l[] = {
	0x00000000,
	0x00000080,
	0x00000000,
	0x00000080,
	0x00008000,
	0x00008080,
	0x00008000,
	0x00008080,
	0x00000000,
	0x00000080,
	0x00000000,
	0x00000080,
	0x00008000,
	0x00008080,
	0x00008000,
	0x00008080,
	0x00800000,
	0x00800080,
	0x00800000,
	0x00800080,
	0x00808000,
	0x00808080,
	0x00808000,
	0x00808080,
	0x00800000,
	0x00800080,
	0x00800000,
	0x00800080,
	0x00808000,
	0x00808080,
	0x00808000,
	0x00808080,
	0x00000000,
	0x00000080,
	0x00000000,
	0x00000080,
	0x00008000,
	0x00008080,
	0x00008000,
	0x00008080,
	0x00000000,
	0x00000080,
	0x00000000,
	0x00000080,
	0x00008000,
	0x00008080,
	0x00008000,
	0x00008080,
	0x00800000,
	0x00800080,
	0x00800000,
	0x00800080,
	0x00808000,
	0x00808080,
	0x00808000,
	0x00808080,
	0x00800000,
	0x00800080,
	0x00800000,
	0x00800080,
	0x00808000,
	0x00808080,
	0x00808000,
	0x00808080,
	0x80000000,
	0x80000080,
	0x80000000,
	0x80000080,
	0x80008000,
	0x80008080,
	0x80008000,
	0x80008080,
	0x80000000,
	0x80000080,
	0x80000000,
	0x80000080,
	0x80008000,
	0x80008080,
	0x80008000,
	0x80008080,
	0x80800000,
	0x80800080,
	0x80800000,
	0x80800080,
	0x80808000,
	0x80808080,
	0x80808000,
	0x80808080,
	0x80800000,
	0x80800080,
	0x80800000,
	0x80800080,
	0x80808000,
	0x80808080,
	0x80808000,
	0x80808080,
	0x80000000,
	0x80000080,
	0x80000000,
	0x80000080,
	0x80008000,
	0x80008080,
	0x80008000,
	0x80008080,
	0x80000000,
	0x80000080,
	0x80000000,
	0x80000080,
	0x80008000,
	0x80008080,
	0x80008000,
	0x80008080,
	0x80800000,
	0x80800080,
	0x80800000,
	0x80800080,
	0x80808000,
	0x80808080,
	0x80808000,
	0x80808080,
	0x80800000,
	0x80800080,
	0x80800000,
	0x80800080,
	0x80808000,
	0x80808080,
	0x80808000,
	0x80808080,
	0x00000000,
	0x00000080,
	0x00000000,
	0x00000080,
	0x00008000,
	0x00008080,
	0x00008000,
	0x00008080,
	0x00000000,
	0x00000080,
	0x00000000,
	0x00000080,
	0x00008000,
	0x00008080,
	0x00008000,
	0x00008080,
	0x00800000,
	0x00800080,
	0x00800000,
	0x00800080,
	0x00808000,
	0x00808080,
	0x00808000,
	0x00808080,
	0x00800000,
	0x00800080,
	0x00800000,
	0x00800080,
	0x00808000,
	0x00808080,
	0x00808000,
	0x00808080,
	0x00000000,
	0x00000080,
	0x00000000,
	0x00000080,
	0x00008000,
	0x00008080,
	0x00008000,
	0x00008080,
	0x00000000,
	0x00000080,
	0x00000000,
	0x00000080,
	0x00008000,
	0x00008080,
	0x00008000,
	0x00008080,
	0x00800000,
	0x00800080,
	0x00800000,
	0x00800080,
	0x00808000,
	0x00808080,
	0x00808000,
	0x00808080,
	0x00800000,
	0x00800080,
	0x00800000,
	0x00800080,
	0x00808000,
	0x00808080,
	0x00808000,
	0x00808080,
	0x80000000,
	0x80000080,
	0x80000000,
	0x80000080,
	0x80008000,
	0x80008080,
	0x80008000,
	0x80008080,
	0x80000000,
	0x80000080,
	0x80000000,
	0x80000080,
	0x80008000,
	0x80008080,
	0x80008000,
	0x80008080,
	0x80800000,
	0x80800080,
	0x80800000,
	0x80800080,
	0x80808000,
	0x80808080,
	0x80808000,
	0x80808080,
	0x80800000,
	0x80800080,
	0x80800000,
	0x80800080,
	0x80808000,
	0x80808080,
	0x80808000,
	0x80808080,
	0x80000000,
	0x80000080,
	0x80000000,
	0x80000080,
	0x80008000,
	0x80008080,
	0x80008000,
	0x80008080,
	0x80000000,
	0x80000080,
	0x80000000,
	0x80000080,
	0x80008000,
	0x80008080,
	0x80008000,
	0x80008080,
	0x80800000,
	0x80800080,
	0x80800000,
	0x80800080,
	0x80808000,
	0x80808080,
	0x80808000,
	0x80808080,
	0x80800000,
	0x80800080,
	0x80800000,
	0x80800080,
	0x80808000,
	0x80808080,
	0x80808000,
	0x80808080,
};
unsigned long Ip_r[] = {
	0x00000000,
	0x00000000,
	0x00000080,
	0x00000080,
	0x00000000,
	0x00000000,
	0x00000080,
	0x00000080,
	0x00008000,
	0x00008000,
	0x00008080,
	0x00008080,
	0x00008000,
	0x00008000,
	0x00008080,
	0x00008080,
	0x00000000,
	0x00000000,
	0x00000080,
	0x00000080,
	0x00000000,
	0x00000000,
	0x00000080,
	0x00000080,
	0x00008000,
	0x00008000,
	0x00008080,
	0x00008080,
	0x00008000,
	0x00008000,
	0x00008080,
	0x00008080,
	0x00800000,
	0x00800000,
	0x00800080,
	0x00800080,
	0x00800000,
	0x00800000,
	0x00800080,
	0x00800080,
	0x00808000,
	0x00808000,
	0x00808080,
	0x00808080,
	0x00808000,
	0x00808000,
	0x00808080,
	0x00808080,
	0x00800000,
	0x00800000,
	0x00800080,
	0x00800080,
	0x00800000,
	0x00800000,
	0x00800080,
	0x00800080,
	0x00808000,
	0x00808000,
	0x00808080,
	0x00808080,
	0x00808000,
	0x00808000,
	0x00808080,
	0x00808080,
	0x00000000,
	0x00000000,
	0x00000080,
	0x00000080,
	0x00000000,
	0x00000000,
	0x00000080,
	0x00000080,
	0x00008000,
	0x00008000,
	0x00008080,
	0x00008080,
	0x00008000,
	0x00008000,
	0x00008080,
	0x00008080,
	0x00000000,
	0x00000000,
	0x00000080,
	0x00000080,
	0x00000000,
	0x00000000,
	0x00000080,
	0x00000080,
	0x00008000,
	0x00008000,
	0x00008080,
	0x00008080,
	0x00008000,
	0x00008000,
	0x00008080,
	0x00008080,
	0x00800000,
	0x00800000,
	0x00800080,
	0x00800080,
	0x00800000,
	0x00800000,
	0x00800080,
	0x00800080,
	0x00808000,
	0x00808000,
	0x00808080,
	0x00808080,
	0x00808000,
	0x00808000,
	0x00808080,
	0x00808080,
	0x00800000,
	0x00800000,
	0x00800080,
	0x00800080,
	0x00800000,
	0x00800000,
	0x00800080,
	0x00800080,
	0x00808000,
	0x00808000,
	0x00808080,
	0x00808080,
	0x00808000,
	0x00808000,
	0x00808080,
	0x00808080,
	0x80000000,
	0x80000000,
	0x80000080,
	0x80000080,
	0x80000000,
	0x80000000,
	0x80000080,
	0x80000080,
	0x80008000,
	0x80008000,
	0x80008080,
	0x80008080,
	0x80008000,
	0x80008000,
	0x80008080,
	0x80008080,
	0x80000000,
	0x80000000,
	0x80000080,
	0x80000080,
	0x80000000,
	0x80000000,
	0x80000080,
	0x80000080,
	0x80008000,
	0x80008000,
	0x80008080,
	0x80008080,
	0x80008000,
	0x80008000,
	0x80008080,
	0x80008080,
	0x80800000,
	0x80800000,
	0x80800080,
	0x80800080,
	0x80800000,
	0x80800000,
	0x80800080,
	0x80800080,
	0x80808000,
	0x80808000,
	0x80808080,
	0x80808080,
	0x80808000,
	0x80808000,
	0x80808080,
	0x80808080,
	0x80800000,
	0x80800000,
	0x80800080,
	0x80800080,
	0x80800000,
	0x80800000,
	0x80800080,
	0x80800080,
	0x80808000,
	0x80808000,
	0x80808080,
	0x80808080,
	0x80808000,
	0x80808000,
	0x80808080,
	0x80808080,
	0x80000000,
	0x80000000,
	0x80000080,
	0x80000080,
	0x80000000,
	0x80000000,
	0x80000080,
	0x80000080,
	0x80008000,
	0x80008000,
	0x80008080,
	0x80008080,
	0x80008000,
	0x80008000,
	0x80008080,
	0x80008080,
	0x80000000,
	0x80000000,
	0x80000080,
	0x80000080,
	0x80000000,
	0x80000000,
	0x80000080,
	0x80000080,
	0x80008000,
	0x80008000,
	0x80008080,
	0x80008080,
	0x80008000,
	0x80008000,
	0x80008080,
	0x80008080,
	0x80800000,
	0x80800000,
	0x80800080,
	0x80800080,
	0x80800000,
	0x80800000,
	0x80800080,
	0x80800080,
	0x80808000,
	0x80808000,
	0x80808080,
	0x80808080,
	0x80808000,
	0x80808000,
	0x80808080,
	0x80808080,
	0x80800000,
	0x80800000,
	0x80800080,
	0x80800080,
	0x80800000,
	0x80800000,
	0x80800080,
	0x80800080,
	0x80808000,
	0x80808000,
	0x80808080,
	0x80808080,
	0x80808000,
	0x80808000,
	0x80808080,
	0x80808080,
};
unsigned long Ip_inv_l[] = {
	0x00000000,
	0x80000000,
	0x00800000,
	0x80800000,
	0x00008000,
	0x80008000,
	0x00808000,
	0x80808000,
	0x00000080,
	0x80000080,
	0x00800080,
	0x80800080,
	0x00008080,
	0x80008080,
	0x00808080,
	0x80808080,
	0x00000000,
	0x80000000,
	0x00800000,
	0x80800000,
	0x00008000,
	0x80008000,
	0x00808000,
	0x80808000,
	0x00000080,
	0x80000080,
	0x00800080,
	0x80800080,
	0x00008080,
	0x80008080,
	0x00808080,
	0x80808080,
	0x00000000,
	0x80000000,
	0x00800000,
	0x80800000,
	0x00008000,
	0x80008000,
	0x00808000,
	0x80808000,
	0x00000080,
	0x80000080,
	0x00800080,
	0x80800080,
	0x00008080,
	0x80008080,
	0x00808080,
	0x80808080,
	0x00000000,
	0x80000000,
	0x00800000,
	0x80800000,
	0x00008000,
	0x80008000,
	0x00808000,
	0x80808000,
	0x00000080,
	0x80000080,
	0x00800080,
	0x80800080,
	0x00008080,
	0x80008080,
	0x00808080,
	0x80808080,
	0x00000000,
	0x80000000,
	0x00800000,
	0x80800000,
	0x00008000,
	0x80008000,
	0x00808000,
	0x80808000,
	0x00000080,
	0x80000080,
	0x00800080,
	0x80800080,
	0x00008080,
	0x80008080,
	0x00808080,
	0x80808080,
	0x00000000,
	0x80000000,
	0x00800000,
	0x80800000,
	0x00008000,
	0x80008000,
	0x00808000,
	0x80808000,
	0x00000080,
	0x80000080,
	0x00800080,
	0x80800080,
	0x00008080,
	0x80008080,
	0x00808080,
	0x80808080,
	0x00000000,
	0x80000000,
	0x00800000,
	0x80800000,
	0x00008000,
	0x80008000,
	0x00808000,
	0x80808000,
	0x00000080,
	0x80000080,
	0x00800080,
	0x80800080,
	0x00008080,
	0x80008080,
	0x00808080,
	0x80808080,
	0x00000000,
	0x80000000,
	0x00800000,
	0x80800000,
	0x00008000,
	0x80008000,
	0x00808000,
	0x80808000,
	0x00000080,
	0x80000080,
	0x00800080,
	0x80800080,
	0x00008080,
	0x80008080,
	0x00808080,
	0x80808080,
	0x00000000,
	0x80000000,
	0x00800000,
	0x80800000,
	0x00008000,
	0x80008000,
	0x00808000,
	0x80808000,
	0x00000080,
	0x80000080,
	0x00800080,
	0x80800080,
	0x00008080,
	0x80008080,
	0x00808080,
	0x80808080,
	0x00000000,
	0x80000000,
	0x00800000,
	0x80800000,
	0x00008000,
	0x80008000,
	0x00808000,
	0x80808000,
	0x00000080,
	0x80000080,
	0x00800080,
	0x80800080,
	0x00008080,
	0x80008080,
	0x00808080,
	0x80808080,
	0x00000000,
	0x80000000,
	0x00800000,
	0x80800000,
	0x00008000,
	0x80008000,
	0x00808000,
	0x80808000,
	0x00000080,
	0x80000080,
	0x00800080,
	0x80800080,
	0x00008080,
	0x80008080,
	0x00808080,
	0x80808080,
	0x00000000,
	0x80000000,
	0x00800000,
	0x80800000,
	0x00008000,
	0x80008000,
	0x00808000,
	0x80808000,
	0x00000080,
	0x80000080,
	0x00800080,
	0x80800080,
	0x00008080,
	0x80008080,
	0x00808080,
	0x80808080,
	0x00000000,
	0x80000000,
	0x00800000,
	0x80800000,
	0x00008000,
	0x80008000,
	0x00808000,
	0x80808000,
	0x00000080,
	0x80000080,
	0x00800080,
	0x80800080,
	0x00008080,
	0x80008080,
	0x00808080,
	0x80808080,
	0x00000000,
	0x80000000,
	0x00800000,
	0x80800000,
	0x00008000,
	0x80008000,
	0x00808000,
	0x80808000,
	0x00000080,
	0x80000080,
	0x00800080,
	0x80800080,
	0x00008080,
	0x80008080,
	0x00808080,
	0x80808080,
	0x00000000,
	0x80000000,
	0x00800000,
	0x80800000,
	0x00008000,
	0x80008000,
	0x00808000,
	0x80808000,
	0x00000080,
	0x80000080,
	0x00800080,
	0x80800080,
	0x00008080,
	0x80008080,
	0x00808080,
	0x80808080,
	0x00000000,
	0x80000000,
	0x00800000,
	0x80800000,
	0x00008000,
	0x80008000,
	0x00808000,
	0x80808000,
	0x00000080,
	0x80000080,
	0x00800080,
	0x80800080,
	0x00008080,
	0x80008080,
	0x00808080,
	0x80808080,
};
unsigned long Ip_inv_r[] = {
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x80000000,
	0x80000000,
	0x80000000,
	0x80000000,
	0x80000000,
	0x80000000,
	0x80000000,
	0x80000000,
	0x80000000,
	0x80000000,
	0x80000000,
	0x80000000,
	0x80000000,
	0x80000000,
	0x80000000,
	0x80000000,
	0x00800000,
	0x00800000,
	0x00800000,
	0x00800000,
	0x00800000,
	0x00800000,
	0x00800000,
	0x00800000,
	0x00800000,
	0x00800000,
	0x00800000,
	0x00800000,
	0x00800000,
	0x00800000,
	0x00800000,
	0x00800000,
	0x80800000,
	0x80800000,
	0x80800000,
	0x80800000,
	0x80800000,
	0x80800000,
	0x80800000,
	0x80800000,
	0x80800000,
	0x80800000,
	0x80800000,
	0x80800000,
	0x80800000,
	0x80800000,
	0x80800000,
	0x80800000,
	0x00008000,
	0x00008000,
	0x00008000,
	0x00008000,
	0x00008000,
	0x00008000,
	0x00008000,
	0x00008000,
	0x00008000,
	0x00008000,
	0x00008000,
	0x00008000,
	0x00008000,
	0x00008000,
	0x00008000,
	0x00008000,
	0x80008000,
	0x80008000,
	0x80008000,
	0x80008000,
	0x80008000,
	0x80008000,
	0x80008000,
	0x80008000,
	0x80008000,
	0x80008000,
	0x80008000,
	0x80008000,
	0x80008000,
	0x80008000,
	0x80008000,
	0x80008000,
	0x00808000,
	0x00808000,
	0x00808000,
	0x00808000,
	0x00808000,
	0x00808000,
	0x00808000,
	0x00808000,
	0x00808000,
	0x00808000,
	0x00808000,
	0x00808000,
	0x00808000,
	0x00808000,
	0x00808000,
	0x00808000,
	0x80808000,
	0x80808000,
	0x80808000,
	0x80808000,
	0x80808000,
	0x80808000,
	0x80808000,
	0x80808000,
	0x80808000,
	0x80808000,
	0x80808000,
	0x80808000,
	0x80808000,
	0x80808000,
	0x80808000,
	0x80808000,
	0x00000080,
	0x00000080,
	0x00000080,
	0x00000080,
	0x00000080,
	0x00000080,
	0x00000080,
	0x00000080,
	0x00000080,
	0x00000080,
	0x00000080,
	0x00000080,
	0x00000080,
	0x00000080,
	0x00000080,
	0x00000080,
	0x80000080,
	0x80000080,
	0x80000080,
	0x80000080,
	0x80000080,
	0x80000080,
	0x80000080,
	0x80000080,
	0x80000080,
	0x80000080,
	0x80000080,
	0x80000080,
	0x80000080,
	0x80000080,
	0x80000080,
	0x80000080,
	0x00800080,
	0x00800080,
	0x00800080,
	0x00800080,
	0x00800080,
	0x00800080,
	0x00800080,
	0x00800080,
	0x00800080,
	0x00800080,
	0x00800080,
	0x00800080,
	0x00800080,
	0x00800080,
	0x00800080,
	0x00800080,
	0x80800080,
	0x80800080,
	0x80800080,
	0x80800080,
	0x80800080,
	0x80800080,
	0x80800080,
	0x80800080,
	0x80800080,
	0x80800080,
	0x80800080,
	0x80800080,
	0x80800080,
	0x80800080,
	0x80800080,
	0x80800080,
	0x00008080,
	0x00008080,
	0x00008080,
	0x00008080,
	0x00008080,
	0x00008080,
	0x00008080,
	0x00008080,
	0x00008080,
	0x00008080,
	0x00008080,
	0x00008080,
	0x00008080,
	0x00008080,
	0x00008080,
	0x00008080,
	0x80008080,
	0x80008080,
	0x80008080,
	0x80008080,
	0x80008080,
	0x80008080,
	0x80008080,
	0x80008080,
	0x80008080,
	0x80008080,
	0x80008080,
	0x80008080,
	0x80008080,
	0x80008080,
	0x80008080,
	0x80008080,
	0x00808080,
	0x00808080,
	0x00808080,
	0x00808080,
	0x00808080,
	0x00808080,
	0x00808080,
	0x00808080,
	0x00808080,
	0x00808080,
	0x00808080,
	0x00808080,
	0x00808080,
	0x00808080,
	0x00808080,
	0x00808080,
	0x80808080,
	0x80808080,
	0x80808080,
	0x80808080,
	0x80808080,
	0x80808080,
	0x80808080,
	0x80808080,
	0x80808080,
	0x80808080,
	0x80808080,
	0x80808080,
	0x80808080,
	0x80808080,
	0x80808080,
	0x80808080,
};

/////////////////////////////////////////////////////////////////////////////
// CDes �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDes::CDes()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CDes::~CDes()
{
}

/////////////////////////////////////////////////////////////////////////////
// CDes �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���C��																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CDes::main
	(	int argc,
		char **argv
	)
{
	unsigned mode = (unsigned)-1;
	int ecb_flag = 0;
	int key_given = 0;
	int n, left;
	char key[8], ivec[8];
	static char buff[2*BLKSIZE];
	char *filename, *p;
	int ifd, ofd;

	memset(key, 0, 8);
	while (++argv, --argc > 0 && *argv && **argv == '-' && argv[0][1]) {
		for (p = *argv + 1; *p; p++) {
			switch (*p) {
			case 'e':
				mode = DES_ENCRYPT;
				break;
			case 'd':
				mode = DES_DECRYPT;
				break;
			case 'b':
				ecb_flag = 1;
				break;
			case 'k': /* read key */
				if (--argc <= 0 || *++argv == NULL) {
//					fprintf(stderr, "Missing key after -k\n");
					return -1;
				}
				strncpy(key, *argv, 8);
				killarg(*argv);
				key_given = 1;
				break;
			case 'f': /* -f and -s options are ignored */
			case 's': /* Because we have no DES chip */
				break;
			default:
//				fprintf(stderr, "Bad option: %s\n", *argv);
				return -1;
			}
		}
	}
	if (argc > 2) {
		return -1;
	}
	if (mode == -1) {
//		fprintf(stderr, "No -e or -d flag\n");
		return -1;
	}
		
	if (argc >= 1 && strcmp(*argv, "-") != 0) {
		filename = *argv;
		if ((ifd = open(filename, O_RDONLY | O_BINARY)) < 0)
			return -2;
	} else {
		filename = NULL;
		ifd = 0;
	}
	if (argc >= 2 && strcmp(argv[1], "-") != 0) {
		if ((ofd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0666)) < 0)
			return -2;
	} else {
		ofd = 1;
	}

	if (!key_given)
		return -1;

	des_setparity(key);
	memset(ivec, 0, 8);
	left = 0;
	while ((n = read(ifd, buff + left, sizeof(buff) - left)) > 0) {
		left += n;
		if (left > BLKSIZE) {
			if (ecb_flag)
				ecb_crypt(key, buff, BLKSIZE, mode);
			else
				cbc_crypt(key, buff, BLKSIZE, mode, ivec);
			write(ofd, buff, BLKSIZE);
			left -= BLKSIZE;
			memcpy(buff, buff + BLKSIZE, left);
		}
	}
	/* last block */
	n = (left & ~7) + 8;
	set_random(buff + left, n - left);
	buff[n - 1] = left & 7;
	if (ecb_flag)
		ecb_crypt(key, buff, n, mode);
	else
		cbc_crypt(key, buff, n, mode, ivec);
	if ((mode & DES_DIRMASK) == DES_DECRYPT) {
		if (left < 8 || (left & 7) >= 8 || buff[left - 1] >= 8) {
			return -3;
		}
		n = (left - 8) + buff[left - 1];
	}
	write(ofd, buff, n);

	close(ofd);
	close(ifd);
	return 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�Í����E������														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CDes::crypt
	(	char  type,		//'e' or 'd'
		char* inkey,
		char* in,
		int	  inlen,
		char* out
	)
{
	unsigned mode = (unsigned)-1;
	int ecb_flag = 0;
	int key_given = 0;
	int n, left;
	char key[8], ivec[8];
	static char buff[2*BLKSIZE];
//	char *filename, *p;
//	int ifd, ofd;

	memset(key, 0, 8);
//2000.07.19 Add Start
	int ipos=0,opos=0;
	mode = (type == 'e') ? DES_ENCRYPT : DES_DECRYPT;
	strncpy(key,inkey,8);
	key_given = 1;
//2000.07.19 Add End


//2000.07.19 Cut Start
//	while (++argv, --argc > 0 && *argv && **argv == '-' && argv[0][1]) {
//		for (p = *argv + 1; *p; p++) {
//			switch (*p) {
//			case 'e':
//				mode = DES_ENCRYPT;
//				break;
//			case 'd':
//				mode = DES_DECRYPT;
//				break;
//			case 'b':
//				ecb_flag = 1;
//				break;
//			case 'k': /* read key */
//				if (--argc <= 0 || *++argv == NULL) {
////					fprintf(stderr, "Missing key after -k\n");
//					return -1;
//				}
//				strncpy(key, *argv, 8);
//				killarg(*argv);
//				key_given = 1;
//				break;
//			case 'f': /* -f and -s options are ignored */
//			case 's': /* Because we have no DES chip */
//				break;
//			default:
////				fprintf(stderr, "Bad option: %s\n", *argv);
//				return -1;
//			}
//		}
//	}
//	if (argc > 2) {
//		return -1;
//	}
//	if (mode == -1) {
////		fprintf(stderr, "No -e or -d flag\n");
//		return -1;
//	}
//		
//	if (argc >= 1 && strcmp(*argv, "-") != 0) {
//		filename = *argv;
//		if ((ifd = open(filename, O_RDONLY | O_BINARY)) < 0)
//			return -2;
//	} else {
//		filename = NULL;
//		ifd = 0;
//	}
//	if (argc >= 2 && strcmp(argv[1], "-") != 0) {
//		if ((ofd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0666)) < 0)
//			return -2;
//	} else {
//		ofd = 1;
//	}
//
//	if (!key_given)
//		return -1;
//2000.07.19 Cut End


	des_setparity(key);
	memset(ivec, 0, 8);
	left = 0;

//2000.07.19 Add Start
	while(TRUE) {
		if( (sizeof(buff) - left) <= (unsigned int)inlen )
			n = sizeof(buff) - left;
		else
			n = inlen;

		if(n <= 0)
			break;
		inlen -= n;

		memcpy(buff + left, &in[ipos], n);
		ipos += n;
		left += n;
		if (left > BLKSIZE) {
			if (ecb_flag)
				ecb_crypt(key, buff, BLKSIZE, mode);
			else
				cbc_crypt(key, buff, BLKSIZE, mode, ivec);
			memcpy(&out[opos], buff, BLKSIZE);
			opos += BLKSIZE;
			left -= BLKSIZE;
			memcpy(buff, buff + BLKSIZE, left);
		}
	}
//2000.07.19 Add End

//2000.07.19 Cut Start
//	while ((n = read(ifd, buff + left, sizeof(buff) - left)) > 0) {
//		left += n;
//		if (left > BLKSIZE) {
//			if (ecb_flag)
//				ecb_crypt(key, buff, BLKSIZE, mode);
//			else
//				cbc_crypt(key, buff, BLKSIZE, mode, ivec);
//			write(ofd, buff, BLKSIZE);
//			left -= BLKSIZE;
//			memcpy(buff, buff + BLKSIZE, left);
//		}
//	}
//2000.07.19 Cut End

	/* last block */
	n = (left & ~7) + 8;
	set_random(buff + left, n - left);
	buff[n - 1] = left & 7;
	if (ecb_flag)
		ecb_crypt(key, buff, n, mode);
	else
		cbc_crypt(key, buff, n, mode, ivec);
	if ((mode & DES_DIRMASK) == DES_DECRYPT) {
		if (left < 8 || (left & 7) >= 8 || buff[left - 1] >= 8) {
			return -3;
		}
		n = (left - 8) + buff[left - 1];
	}
//2000.07.19 Cut Start
//	write(ofd, buff, n);
//	close(ofd);
//	close(ifd);
//2000.07.19 Cut End

//2000.07.19 Add Start
	memcpy(&out[opos], buff, n);
//2000.07.19 Add End

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDes �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FFill random 'n' byte sequence at address 'block'						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDes::set_random
	(	char *block,
		int n
	)
{
	unsigned long now = (unsigned long)time(NULL);
	unsigned long x;

	for (x = now; n; n--) {
		*block++ = (char)x;
		if ((x >>= 8) == 0)
			x = now;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�FRubout key argument to prevent being peeped by ps(1)					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDes::killarg
	(	char *arg
	)
{
	while (*arg)
		*arg++ = 'X';
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�p���e�B�Z�b�g														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDes::des_setparity
	(	char *key
	)
{
	int n;

	for (n = 8; n > 0; n--) {
		char c;

		c = *key &= 0x7f;
		c ^= c << 4;
		c ^= c << 2;
		c ^= c << 1;
		*key++ ^= ~c & 0x80;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[�Z�b�g															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDes::des_setkey
	(	char *key
	)
{
	char nshifts[]
		= { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };
	unsigned char pc1[] = {
		C + 20, C + 12, C + 4, D + 0, D + 4, D + 12, D + 20,
		C + 21, C + 13, C + 5, D + 1, D + 5, D + 13, D + 21,
		C + 22, C + 14, C + 6, D + 2, D + 6, D + 14, D + 22,
		C + 23, C + 15, C + 7, D + 3, D + 7, D + 15, D + 23,
		C + 24, C + 16, C + 8, C + 0, D + 8, D + 16, D + 24,
		C + 25, C + 17, C + 9, C + 1, D + 9, D + 17, D + 25,
		C + 26, C + 18, C +10, C + 2, D +10, D + 18, D + 26,
		C + 27, C + 19, C +11, C + 3, D +11, D + 19, D + 27
	};
	unsigned char pc2_c[] = {
		14, 11, 17,  4, 27, 23,
		25,  0, 13, 22,  7, 18,
		 5,  9, 16, 24,  2, 20,
		12, 21,  1,  8, 15, 26
	};
	unsigned char pc2_d[] = {
		15,  4, 25, 19,  9,  1,
		26, 16,  5, 11, 23,  8,
		12,  7, 17,  0, 22,  3,
		10, 14,  6, 20, 27, 24,
	};
	unsigned long c, d;
	unsigned char b, *p;
	int i, j;

	/* PC-1 (Permuted Choice 1) */
	c = d = 0;
	p = pc1;
	for (i = 0; i < 8; i++) {
		b = *key++;
		for (j = 0; j < 7; j++) {
			if (b & 0x80) {
				if (*p >= C)
					c |= 1L << (*p - C);
				else
					d |= 1L << *p;
			}
			p++;
			b <<= 1;
		}
	}

	for (i = 0; i < 16; i++) {
		unsigned char *k, mask, w[8];
		unsigned int n = nshifts[i];
		
		c = (c << n | c >> (28 - n)) & 0x0fffffff;
		d = (d << n | d >> (28 - n)) & 0x0fffffff;
		/* PC-2 (Permuted Choice 2) */
		k = w;
		p = pc2_c;
		for (j = 0; j < 4; j++) {
			*k = 0;
			for (mask = 0x20; mask > 0; mask >>= 1) {
				if ((c >> *p++) & 1)
					*k |= mask;
			}
			k++;
		}
		p = pc2_d;
		for (j = 0; j < 4; j++) {
			*k = 0;
			for (mask = 0x20; mask > 0; mask >>= 1) {
				if ((d >> *p++) & 1)
					*k |= mask;
			}
			k++;
		}

		Keys[i][0] = ((unsigned long)w[0] << 24) | ((unsigned long)w[2] << 16)
			   | ((unsigned long)w[4] << 8) | (unsigned long)w[6];
		Keys[i][1] = ((unsigned long)w[1] << 24) | ((unsigned long)w[3] << 16)
			   | ((unsigned long)w[5] << 8) | (unsigned long)w[7];

	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�Í����E������														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CDes::des_crypt
	(	char *block,
		int dflag
	)
{
	unsigned long l, r, l2, r2;
	unsigned char *s;
	int i;

	/* Initial Transpose */
	s = (unsigned char *)block;
	l = (((((((Ip_l[s[0]] >> 1
		  | Ip_l[s[1]]) >> 1
		 | Ip_l[s[2]]) >> 1
		| Ip_l[s[3]]) >> 1
	       | Ip_l[s[4]]) >> 1
	      | Ip_l[s[5]]) >> 1
	     | Ip_l[s[6]]) >> 1
	     | Ip_l[s[7]]);
	r = (((((((Ip_r[s[0]] >> 1
		  | Ip_r[s[1]]) >> 1
		 | Ip_r[s[2]]) >> 1
		| Ip_r[s[3]]) >> 1
	       | Ip_r[s[4]]) >> 1
	      | Ip_r[s[5]]) >> 1
	     | Ip_r[s[6]]) >> 1
	     | Ip_r[s[7]]);

	if (dflag == 0) {
		for (i = 0; i < 16; i++) {
			unsigned long v, x1, x2;

			/* E-transpose, Xor, S-box and P-transpose */
			x1 = rol1(r);
			x2 = ror4(x1) ^ Keys[i][0];
			x1 ^= Keys[i][1];
			v = l	^ ps(7, x1 & 0x3f)
				^ ps(5, (x1 >> 8) & 0x3f)
				^ ps(3, (x1 >> 16) & 0x3f)
				^ ps(1, (x1 >> 24) & 0x3f)
				^ ps(6, x2 & 0x3f)
				^ ps(4, (x2 >> 8) & 0x3f)
				^ ps(2, (x2 >> 16) & 0x3f)
				^ ps(0, (x2 >> 24) & 0x3f);
			l = r;
			r = v;
		}
	} else {
		for (i = 16; --i >= 0; ) {
			unsigned long v, x1, x2;

			/* E-transpose, Xor, S-box and P-transpose */
			x1 = rol1(r);
			x2 = ror4(x1) ^ Keys[i][0];
			x1 ^= Keys[i][1];
			v = l	^ ps(7, x1 & 0x3f)
				^ ps(5, (x1 >> 8) & 0x3f)
				^ ps(3, (x1 >> 16) & 0x3f)
				^ ps(1, (x1 >> 24) & 0x3f)
				^ ps(6, x2 & 0x3f)
				^ ps(4, (x2 >> 8) & 0x3f)
				^ ps(2, (x2 >> 16) & 0x3f)
				^ ps(0, (x2 >> 24) & 0x3f);
			l = r;
			r = v;
		}
	}
	/* Final transpose */
	l2 = (((((((Ip_inv_l[r & 0xff] >> 1
		   | Ip_inv_l[l & 0xff]) >> 1
		  | Ip_inv_l[r>>8 & 0xff]) >> 1
		 | Ip_inv_l[l>>8 & 0xff]) >> 1
		| Ip_inv_l[r>>16 & 0xff]) >> 1
	       | Ip_inv_l[l>>16 & 0xff]) >> 1
	      | Ip_inv_l[r>>24 & 0xff]) >> 1
	     | Ip_inv_l[l>>24 & 0xff]);
	r2 = (((((((Ip_inv_r[r & 0xff] >> 1
		   | Ip_inv_r[l & 0xff]) >> 1
		  | Ip_inv_r[r>>8 & 0xff]) >> 1
		 | Ip_inv_r[l>>8 & 0xff]) >> 1
		| Ip_inv_r[r>>16 & 0xff]) >> 1
	       | Ip_inv_r[l>>16 & 0xff]) >> 1
	      | Ip_inv_r[r>>24 & 0xff]) >> 1
	     | Ip_inv_r[l>>24 & 0xff]);
	*block++ = (char)(l2 >> 24);
	*block++ = (char)(l2 >> 16);
	*block++ = (char)(l2 >> 8);
	*block++ = (char)(l2);
	*block++ = (char)(r2 >> 24);
	*block++ = (char)(r2 >> 16);
	*block++ = (char)(r2 >> 8);
	*block++ = (char)(r2);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�d�b�a���[�h�Í����E������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CDes::ecb_crypt
	(	char *key,
		char *buf,
		unsigned len,
		unsigned mode
	)
{
	des_setkey(key);
	for (len = (len + 7) & ~7; len != 0; buf += 8, len -= 8)
		des_crypt(buf, mode & DES_DIRMASK);
	return ((mode & DES_DEVMASK) == DES_SW ? DESERR_NONE
					       : DESERR_NOHWDEVICE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�b�a�b���[�h�Í����E������											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CDes::cbc_crypt
	(	char *key,
		char *buf,
		unsigned len,
		unsigned mode,
		char *ivec
	)
{
	char nvec[8];

	des_setkey(key);
	switch (mode & DES_DIRMASK) {
	case DES_ENCRYPT:
		for (len = (len + 7) & ~7; len != 0; len -= 8) {
			buf[0] ^= ivec[0];
			buf[1] ^= ivec[1];
			buf[2] ^= ivec[2];
			buf[3] ^= ivec[3];
			buf[4] ^= ivec[4];
			buf[5] ^= ivec[5];
			buf[6] ^= ivec[6];
			buf[7] ^= ivec[7];
			des_crypt(buf, DES_ENCRYPT);
			ivec[0] = *buf++;
			ivec[1] = *buf++;
			ivec[2] = *buf++;
			ivec[3] = *buf++;
			ivec[4] = *buf++;
			ivec[5] = *buf++;
			ivec[6] = *buf++;
			ivec[7] = *buf++;
		} 
		break;
	case DES_DECRYPT:
		for (len = (len + 7) & ~7; len != 0; len -= 8) {
			nvec[0] = buf[0];
			nvec[1] = buf[1];
			nvec[2] = buf[2];
			nvec[3] = buf[3];
			nvec[4] = buf[4];
			nvec[5] = buf[5];
			nvec[6] = buf[6];
			nvec[7] = buf[7];
			des_crypt(buf, DES_DECRYPT);
			*buf++ ^= ivec[0];
			*buf++ ^= ivec[1];
			*buf++ ^= ivec[2];
			*buf++ ^= ivec[3];
			*buf++ ^= ivec[4];
			*buf++ ^= ivec[5];
			*buf++ ^= ivec[6];
			*buf++ ^= ivec[7];
			ivec[0] = nvec[0];
			ivec[1] = nvec[1];
			ivec[2] = nvec[2];
			ivec[3] = nvec[3];
			ivec[4] = nvec[4];
			ivec[5] = nvec[5];
			ivec[6] = nvec[6];
			ivec[7] = nvec[7];
		}
		break;
	}
	return ((mode & DES_DEVMASK) == DES_SW ? DESERR_NONE
					       : DESERR_NOHWDEVICE);
}

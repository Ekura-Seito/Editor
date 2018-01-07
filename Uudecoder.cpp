// Uudecoder.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "Uudecoder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUudecoder �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUudecoder::CUudecoder()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CUudecoder::~CUudecoder()
{
}

/////////////////////////////////////////////////////////////////////////////
// CUudecoder �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F������f�R�[�h														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CUudecoder::StringDecode
	(	CString*	pobjStrIn,
		LPBYTE		lpbyBufOut,
		long*		plLen
	)
{
	char*	p=NULL;
	int		n=0;
	long	lOut=0;

	(*plLen) = 0;
	p = pobjStrIn->GetBuffer(1024);

	/* N is used to avoid writing out all the characters at the end of the file.  */
	n = DEC(*p);
	if(n <= 0)
		return FALSE;
	for(++p; n > 0; p += 4, n -= 3)
	{
		char ch=0;

		if(n >= 3)
		{
			ch = DEC(p[0]) << 2 | DEC(p[1]) >> 4;
			lpbyBufOut[lOut] = ch;
			lOut++;
			ch = DEC(p[1]) << 4 | DEC(p[2]) >> 2;
			lpbyBufOut[lOut] = ch;
			lOut++;
			ch = DEC(p[2]) << 6 | DEC(p[3]);
			lpbyBufOut[lOut] = ch;
			lOut++;
		}
		else
		{
			if(n >= 1)
			{
				ch = DEC(p[0]) << 2 | DEC(p[1]) >> 4;
				lpbyBufOut[lOut] = ch;
				lOut++;
			}
			if(n >= 2)
			{
				ch = DEC(p[1]) << 4 | DEC(p[2]) >> 2;
				lpbyBufOut[lOut] = ch;
				lOut++;
			}
		}
	}

	(*plLen) = lOut;
	pobjStrIn->ReleaseBuffer();
	return TRUE;
}


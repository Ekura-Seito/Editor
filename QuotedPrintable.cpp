// QuotedPrintable.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "QuotedPrintable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQuotedPrintable �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CQuotedPrintable::CQuotedPrintable()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CQuotedPrintable::~CQuotedPrintable()
{
}

/////////////////////////////////////////////////////////////////////////////
// CQuotedPrintable �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F������̃G���R�[�h													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CQuotedPrintable::StringEncode
	(	CString*	pobjStrIn
	)
{
	char	szBuf[32];
	BYTE	byB1=0;
	int		nNeedLen=0;
	long	lCnt=0;
	CString	objStr=_T("");

	if(pobjStrIn == NULL)
		return -1;
	if(pobjStrIn->GetLength() == 0)
		return -1;

	for(;;)
	{
		if(lCnt >= pobjStrIn->GetLength())
			break;
		byB1 = pobjStrIn->GetAt(lCnt);
		lCnt++;

		memset(szBuf,0,sizeof(szBuf));
		if( (byB1 >= 0x21 && byB1 <= 0x7e) || byB1 == ' ' )
		{
			if(byB1 == '=')
			{
				//"=3D"�ɂ���
				nNeedLen = 3;
				strcpy(szBuf,"=3D");
			}
			else if(byB1 == '.') //���[���I�[��'.'�Ƃ܂���킵���̂ŁA"=XX"�̌`�ɂ���
			{
				nNeedLen = 3;
				sprintf(szBuf,"=%02X",byB1);
			}
			else
			{
				//���̂܂�
				nNeedLen = 1;
				szBuf[0] = byB1;
			}
		}
		else
		{
			//"=XX"�̌`�ɂ���
			nNeedLen = 3;
			sprintf(szBuf,"=%02X",byB1);
		}

		objStr += szBuf;
	}

	//�ϊ��㕶�����Ԃ�
	(*pobjStrIn) = objStr;
	return 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�o�C�g��̃G���R�[�h													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CString CQuotedPrintable::BytesEncode
	(	LPBYTE	lpbyBufIn,
		UINT	nLen
	)
{
	char	szBuf[32];
	BYTE	byB1=0;
	int		nNeedLen=0;
	long	lCnt=0;
	CString	objStr=_T("");

	if(lpbyBufIn == NULL)
		return _T("");
	if(nLen == 0)
		return _T("");

	for(;;)
	{
		if(lCnt >= (long)nLen)
			break;
		byB1 = lpbyBufIn[lCnt];
		lCnt++;

		memset(szBuf,0,sizeof(szBuf));
		if( (byB1 >= 0x21 && byB1 <= 0x7e) || byB1 == ' ' )
		{
			if(byB1 == '=')
			{
				//"=3D"�ɂ���
				nNeedLen = 3;
				strcpy(szBuf,"=3D");
			}
			else if(byB1 == '.') //���[���I�[��'.'�Ƃ܂���킵���̂ŁA"=XX"�̌`�ɂ���
			{
				nNeedLen = 3;
				sprintf(szBuf,"=%02X",byB1);
			}
			else
			{
				//���̂܂�
				nNeedLen = 1;
				szBuf[0] = byB1;
			}
		}
		else
		{
			//"=XX"�̌`�ɂ���
			nNeedLen = 3;
			sprintf(szBuf,"=%02X",byB1);
		}

		objStr += szBuf;
	}

	return objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F������̃f�R�[�h														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CQuotedPrintable::StringDecode
	(	CString*	pobjStrIn,
		LPBYTE		lpbyBufOut,
		long*		plLen
	)
{
	long	lOffset = 0, lOut=0;
	BYTE	byQ1 = 0;					//�쐬���ꂽ�o�C�i���f�[�^
	int		nQmode = 0; 				//�o�C�i���f�[�^�̋�؂胂�[�h(0-3)
	BYTE	byTemp = 0; 				//Quoted-Printable�f�R�[�h�ꎞ�o�b�t�@


	for(lOffset=0;;lOffset++)
	{
		if( lOffset >= pobjStrIn->GetLength() )
			break;

		byQ1 = pobjStrIn->GetAt(lOffset);
		if(DataCheck(byQ1,nQmode) == FALSE)
		{
			(*plLen) = lOut;
			return FALSE;
		}

		if(byQ1 == '=')
		{
			if( lOffset == (pobjStrIn->GetLength() - 1) ) //�s���́f���f�̓\�t�g���s������킵�A
			{											 //���s�ɂ͂��Ȃ��B
				break;
			}
			else
			{
				nQmode = 1;
				byTemp = 0;
			}
		}
		else
		{
			if(nQmode == 0)
			{
				lpbyBufOut[lOut] = (BYTE)byQ1;
				lOut++;
			}
			else if(nQmode == 1)
			{
				if(byQ1 >= '0' && byQ1 <= '9')
					byTemp = ((((byQ1 - '0') & 0x0f) << 4) & 0xf0);
				else if(byQ1 >= 'A' && byQ1 <= 'Z')  //'A'��0x0A �ɂ���
					byTemp = ((((byQ1 - 0x37) & 0x0f) << 4) & 0xf0);
				else if(byQ1 >= 'a' && byQ1 <= 'z')  //'a'��0x0A �ɂ���i���S�Ȏ����j
					byTemp = ((((byQ1 - 0x57) & 0x0f) << 4) & 0xf0);
				nQmode = 2;
			}
			else if(nQmode == 2)
			{
				if(byQ1 >= '0' && byQ1 <= '9')
					byTemp |= ((byQ1 - '0') & 0x0f);
				else if(byQ1 >= 'A' && byQ1 <= 'Z') //'A'��0x0A �ɂ���
					byTemp |= ((byQ1 - 0x37) & 0x0f);
				else if(byQ1 >= 'a' && byQ1 <= 'z') //'a'��0x0A �ɂ���i���S�Ȏ����j
					byTemp |= ((byQ1 - 0x57) & 0x0f);
				lpbyBufOut[lOut] = (BYTE)byTemp;
				lOut++;
				byTemp = 0;
				nQmode = 0;
			}
		}
	}

	(*plLen) = lOut;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CQuotedPrintable �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�[�^�`�F�b�N														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CQuotedPrintable::DataCheck
	(	BYTE	byData,
		int		nQmode
	)
{
	if(nQmode == 0)
	{
		if(byData >= 0x21 && byData <= 0x7e)
			return TRUE;
		else if(byData == ' ')
			return TRUE;
		else if(byData == '\t')
			return TRUE;
		else 
			return FALSE;
	}
	else //"=" �ɑ����̂�'A'�`'Z'���邢��'0'�`'9'�̂�
	{
		if(byData >= '0' && byData <= '9')
			return TRUE;
		else if(byData >= 'A' && byData <= 'Z')
			return TRUE;
		else if(byData >= 'a' && byData <= 'z') //���S�Ȏ���
			return TRUE;
		else 
			return FALSE;
	}
}


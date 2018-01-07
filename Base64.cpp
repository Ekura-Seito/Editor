// Base64.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Base64.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBase64 �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CBase64::CBase64()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CBase64::~CBase64()
{
}

/////////////////////////////////////////////////////////////////////////////
// CBase64 �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F������̂a�`�r�d�U�S�G���R�[�h										|*/
/*|	���	�F																  		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CBase64::StringEncode
	(	CString*	pobjStrIn
	)
{
	int		nBmode=0;		//�o�C�i���f�[�^�̋�؂胂�[�h(0-3)
	BYTE	byB1=0;			//����������o�C�i���f�[�^
	BYTE	byTemp=0;
	BYTE	byWork=0;
	CString	objStr=_T("");
	long	lCnt=0;

	if(pobjStrIn == NULL)
		return -1;
	if(pobjStrIn->GetLength() == 0)
		return -1;

	for(;;)
	{
		if(nBmode >= 0 && nBmode <= 2)
		{
			if(lCnt >= pobjStrIn->GetLength())
				break;
			byB1 = pobjStrIn->GetAt(lCnt);
			lCnt++;
		}

		switch(nBmode)
		{
		case 0:
			byTemp = (0x03 & byB1) << 4;				//���ʂQ�a������ۑ�
			byB1 = (0xfc & byB1) >> 2;					//��ʂU�a����
			byWork = 0;
			break;
		case 1:
			byWork = (0x0f & byB1) << 2;				//���ʂS�a������ۑ�
			byB1 = (byTemp | ((0xf0 & byB1) >> 4));		//��ʂS�a����
			byTemp = byWork;
			byWork = 0;
			break;
		case 2:
			byWork = (0x3f & byB1);						//���ʂU�a������ۑ�
			byB1 = (byTemp | ((0xc0 & byB1) >> 6));
			byTemp = byWork;
			byWork = 0;
			break;
		case 3:
			byB1 = byTemp;
			byTemp = 0;
			byWork = 0;
			break;
		default:
			break;
		}

		objStr += (TCHAR)BinToB64(byB1);

		nBmode ++;
		if(nBmode > 3)
			nBmode = 0;
	}

	//�I�[�L���̕t��
	switch(nBmode)
	{
	case 0:
		break;
	case 1:
		objStr += (TCHAR)BinToB64(byTemp);
		objStr += _T("==");
		break;
	case 2:
		objStr += (TCHAR)BinToB64(byTemp);
		objStr += _T("=");
		break;
	case 3:
		break;
	default:
		break;
	}

	//�ϊ��㕶�����Ԃ�
	(*pobjStrIn) = objStr;
	return 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�o�C�g��̂a�`�r�d�U�S�G���R�[�h										|*/
/*|	���	�F																  		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CString CBase64::BytesEncode
	(	LPBYTE	lpbyBufIn,
		UINT	nLen
	)
{
	int		nBmode=0;		//�o�C�i���f�[�^�̋�؂胂�[�h(0-3)
	BYTE	byB1=0;			//����������o�C�i���f�[�^
	BYTE	byTemp=0;
	BYTE	byWork=0;
	CString	objStr=_T("");
	long	lCnt=0;

	if(lpbyBufIn == NULL)
		return _T("");
	if(nLen == 0)
		return _T("");

	for(;;)
	{
		if(nBmode >= 0 && nBmode <= 2)
		{
			if(lCnt >= (long)nLen)
				break;
			byB1 = lpbyBufIn[lCnt];
			lCnt++;
		}

		switch(nBmode)
		{
		case 0:
			byTemp = (0x03 & byB1) << 4;				//���ʂQ�a������ۑ�
			byB1 = (0xfc & byB1) >> 2;					//��ʂU�a����
			byWork = 0;
			break;
		case 1:
			byWork = (0x0f & byB1) << 2;				//���ʂS�a������ۑ�
			byB1 = (byTemp | ((0xf0 & byB1) >> 4));		//��ʂS�a����
			byTemp = byWork;
			byWork = 0;
			break;
		case 2:
			byWork = (0x3f & byB1);						//���ʂU�a������ۑ�
			byB1 = (byTemp | ((0xc0 & byB1) >> 6));
			byTemp = byWork;
			byWork = 0;
			break;
		case 3:
			byB1 = byTemp;
			byTemp = 0;
			byWork = 0;
			break;
		default:
			break;
		}

		objStr += (TCHAR)BinToB64(byB1);

		nBmode ++;
		if(nBmode > 3)
			nBmode = 0;
	}

	//�I�[�L���̕t��
	switch(nBmode)
	{
	case 0:
		break;
	case 1:
		objStr += (TCHAR)BinToB64( byTemp );
		objStr += _T("==");
		break;
	case 2:
		objStr += (TCHAR)BinToB64( byTemp );
		objStr += _T("=");
		break;
	case 3:
		break;
	default:
		break;
	}

	//�ϊ��㕶�����Ԃ�
	return objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F������̂a�`�r�d�U�S�f�R�[�h											|*/
/*|	���	�F																  		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CBase64::StringDecode
	(	CString*	pobjStrIn,
		LPBYTE		lpbyBufOut,
		long*		plLen
	)
{
	long	lOffset = 0, lOut=0;
	BYTE	byB1 = 0;					//�쐬���ꂽ�o�C�i���f�[�^
	int		nBmode = 0; 				//�o�C�i���f�[�^�̋�؂胂�[�h(0-3)
	BYTE	byTemp = 0; 				//Base64�f�R�[�h�ꎞ�o�b�t�@


	for(lOffset=0;;lOffset++)
	{
		if( lOffset >= pobjStrIn->GetLength() )
			break;
		if( pobjStrIn->GetAt(lOffset) == '=' )
		{
			(*plLen) = lOut;
			return TRUE;
		}

		//�`�r�b�h�h�f�[�^�̐��l��(0-63)
		byB1 = B64ToBin( pobjStrIn->GetAt(lOffset) );
		if( !(byB1 >= 0 && byB1 <= 63) )
		{
			(*plLen) = lOut;
			return FALSE;
		}

		//Base64�o�C�i���ϊ�
		switch(nBmode)
		{
		case 0:
			byTemp = byB1 * 4;							//���6Bit
			break;
		case 1:
			lpbyBufOut[lOut] = (BYTE)(byTemp | (byB1 / 16));//����2Bbit
			lOut++;
			byTemp = ((0x0f & byB1) * 16);				//���4Bit
			break;
		case 2:
			lpbyBufOut[lOut] = (BYTE)(byTemp | (byB1 / 4));//����4Bit
			lOut++;
			byTemp = ((0x03 & byB1) * 64);				//���2Bit
			break;
		case 3:
			lpbyBufOut[lOut] = (BYTE)(byTemp | byB1);	//���6Bit
			lOut++;
			byTemp = 0;
			break;
		default:
			break;
		}
		nBmode++;
		if(nBmode > 3)
			nBmode = 0;
	}

	(*plLen) = lOut;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CBase64 �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�`�r�b�h�h�|���a�h�m�`�q�x											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BYTE CBase64::B64ToBin(BYTE byB64Ascii)
{
	BYTE	byB1;

	byB1 = 255;

	if(byB64Ascii >= 'A' && byB64Ascii <= 'Z')
		byB1 = byB64Ascii - 'A' + 0;
	if(byB64Ascii >= 'a' && byB64Ascii <= 'z')
		byB1 = byB64Ascii - 'a' + 26;
	if(byB64Ascii >= '0' && byB64Ascii <= '9')
		byB1 = byB64Ascii - '0' + 52;
	if(byB64Ascii == '+')
		byB1 = 62;
	if(byB64Ascii == '/')
		byB1 = 63;

	return byB1;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�a�h�m�`�q�x�|���`�r�b�h�h											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BYTE CBase64::BinToB64(BYTE byB64Binary)
{
	BYTE	byA1=0;

	if(byB64Binary <= 25)
		byA1 = 'A' + byB64Binary;
	if(byB64Binary > 25 && byB64Binary <= 51)
		byA1 = 'a' + byB64Binary - 26;
	if(byB64Binary > 51 && byB64Binary <= 61)
		byA1 = '0' + byB64Binary - 52;
	if(byB64Binary == 62)
		byA1 = '+';
	if(byB64Binary == 63)
		byA1 = '/';

	return byA1;
}


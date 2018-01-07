// CodeConvManager.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <mbstring.h>
#pragma warning(disable:4786)
#include "CodeConvManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCodeConvManager �N���X�̍\�z/����

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CCodeConvManager::CCodeConvManager()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CCodeConvManager::~CCodeConvManager()
{
}

/////////////////////////////////////////////////////////////////////////////
// CCodeConvManager �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�[�h����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CCodeConvManager::DetectCode
	(	const char*	szInFilePath			//���̓t�@�C���p�X
	)
											//�֐����^�[��
											// 1 : Shift Jis
											// 2 : EUC
											// 3 : JIS
											// 4 : ASCII
											// 5 : �s��
											// 6 : Binary
{
	UCHAR	buf[1024];
	UINT	nBytesRead=0;
	UINT	nCheckResult=INPUT_UNKNOWN;
	BOOL	bIsEnd=FALSE;

	if(IsBinary(szInFilePath))
		return INPUT_BINARY;

	//�t�@�C���I�[�v��
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead, NULL ) )
		return INPUT_UNKNOWN;

	for(;;)
	{
		//�P�O�Q�S�o�C�g�Ǎ���
		memset(buf,0,sizeof(buf));
		nBytesRead = objInFile.Read( buf, sizeof(buf) );

		if (nBytesRead == 0) //�ǂݍ��ݏI��
			break;

		bIsEnd = FALSE;
		nCheckResult = detectcode(buf, nBytesRead, &bIsEnd);
		if(bIsEnd)
		{
			if( nCheckResult==INPUT_ASCII || nCheckResult==INPUT_UNKNOWN )
				continue;
			else
				break;
		}
		else
			break;
	}

	objInFile.Close();
	return nCheckResult;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���s�R�[�h����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CCodeConvManager::DetectKaigyoCode
	(	const char*	szInFilePath			//���̓t�@�C���p�X
	)
											//�֐����^�[��
											// 0 : CR+LF
											// 1 : LF
											// 2 : CR
											// 3 : �s��
{
	BYTE	ch=0;
	UCHAR	buf[32];
	long	lCRCount=0;	//CR�̐�
	long	lLFCount=0;	//LF�̐�
	UINT	unRet=KAIGYO_UNKNOWN;

	//�t�@�C���I�[�v��
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead, NULL ) )
		return KAIGYO_UNKNOWN;
	CArchive ar( &objInFile, CArchive::load, 32, buf );

	for(;;)//���s���o���܂œǂ�
	{
		TRY
		{
			ar >> (BYTE&)ch;
		}
		CATCH_ALL(e) //EOF
		{
			return unRet;
		}
		END_CATCH_ALL

		if(!IsDBCSLeadByteEx(0,ch))//�P�o�C�g����
		{
			if(ch=='\r')
			{
				lCRCount++;
				break;
			}
			else if(ch=='\n')
			{
				lLFCount++;
				break;
			}
		}
	}

	//����Ɏ���ǂݍ���
	TRY
	{
		ar >> (BYTE&)ch;
	}
	CATCH_ALL(e) //EOF
	{
		if(lCRCount>0)
			unRet = KAIGYO_CR;
		else if(lLFCount>0)
			unRet = KAIGYO_LF;
		//�N���[�Y
		ar.Close();
		objInFile.Close();
		return unRet;
	}
	END_CATCH_ALL

	if(IsDBCSLeadByteEx(0,ch)) //�Q�o�C�g����
	{
		if(lCRCount>0)
			unRet = KAIGYO_CR;
		else if(lLFCount>0)
			unRet = KAIGYO_LF;
	}
	else
	{
		if(ch=='\r')
		{
			if(lCRCount>0)
				unRet = KAIGYO_CR;
			else if(lLFCount>0)
				unRet = KAIGYO_UNKNOWN;
		}
		else if(ch=='\n')
		{
			if(lCRCount>0)
				unRet = KAIGYO_CRLF;
			else if(lLFCount>0)
				unRet = KAIGYO_LF;
		}
		else
		{
			if(lCRCount>0)
				unRet = KAIGYO_CR;
			else if(lLFCount>0)
				unRet = KAIGYO_LF;
		}
	}

	//�N���[�Y
	ar.Close();
	objInFile.Close();

	return unRet;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�r�i�h�r�|���d�t�b�ϊ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::ConvertSjisToEuc
	(	const char*	szInFilePath,		//���̓t�@�C���p�X
		const char*	szOutFilePath		//�o�̓t�@�C���p�X
	)
										//�֐����^�[��
										// TRUE : ����I��
										// -1   : ���̓t�@�C���I�[�v���G���[
										// -2   : �o�̓t�@�C���I�[�v���G���[
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	UINT	unConverted=0;

	//���̓t�@�C���I�[�v��
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead, NULL ) )
		return -1;
	CArchive	arIn(&objInFile,CArchive::load);

	//�o�̓t�@�C���I�[�v��
	CFile	objOutFile;
	if( !objOutFile.Open( szOutFilePath, (CFile::modeCreate|CFile::modeWrite), NULL ) )
	{
		arIn.Close();
		objInFile.Close();
		return -2;
	}
	CArchive	arOut(&objOutFile,CArchive::store);


	TRY
	{
		//�܂��P�o�C�g�ǂݍ���
		arIn >> (BYTE&)by1stChar;

		for(;;) //�P�����Â��[�v
		{
			if( by1stChar < 0x80 )
			{
				//���̂܂܏o��
				arOut << (BYTE)by1stChar;
			}
			else if(iskanji1st(by1stChar)) //�V�t�g�i�h�r���o�C�g
			{
				//����ǂݍ���
				arIn >> (BYTE&)by2ndChar;
				if(iskanji2nd(by2ndChar))
				{
					//SJIS -> EUC
					unConverted = sjis2euc( ((by1stChar<<8)|by2ndChar) );
					//�ϊ����ʏo��
					arOut << (BYTE)(((unConverted>>8)&0x00ff));
					arOut << (BYTE)(unConverted&0x00ff);
				}
				else
				{
					//�o��
					arOut << (BYTE)by1stChar;
					arOut << (BYTE)by2ndChar;
				}
			}
			else if(isHanKana(by1stChar)) //���p�J�i�R�[�h
			{
				//�o��
				arOut << (BYTE)0x8e;
				arOut << (BYTE)by1stChar;
			}
			else
			{
				//�o��
				arOut << (BYTE)by1stChar;
			}

			//�N���A
			by1stChar=0;
			by2ndChar=0;
			//����ǂݍ���
			arIn >> (BYTE&)by1stChar;
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL

	//���o�̓t�@�C���N���[�Y
	arIn.Close();
	objInFile.Close();
	arOut.Close();
	objOutFile.Close();

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�r�i�h�r�|���i�h�r�ϊ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::ConvertSjisToJis
	(	const char*	szInFilePath,			//���̓t�@�C���p�X
		const char*	szOutFilePath			//�o�̓t�@�C���p�X
	)
										//�֐����^�[��
										// TRUE : ����I��
										// -1   : ���̓t�@�C���I�[�v���G���[
										// -2   : �o�̓t�@�C���I�[�v���G���[
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	UINT	unConverted=0;
	char	state, kana;

	//���̓t�@�C���I�[�v��
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead, NULL ) )
		return -1;
	CArchive	arIn(&objInFile,CArchive::load);

	//�o�̓t�@�C���I�[�v��
	CFile	objOutFile;
	if( !objOutFile.Open( szOutFilePath, (CFile::modeCreate|CFile::modeWrite), NULL ) )
	{
		arIn.Close();
		objInFile.Close();
		return -2;
	}
	CArchive	arOut(&objOutFile,CArchive::store);


	TRY
	{
		//�t���O�N���A
		state = kana = 0;
		//�܂��P�o�C�g�ǂݍ���
		arIn >> (BYTE&)by1stChar;
	
		for(;;) //�P�����Â��[�v
		{
			if( by1stChar < 0x80 ) //ASCII
			{
				if (state)
				{
					set1byte(arOut);
					state =	0;
				}
				else if	(kana)
				{
					arOut << (BYTE)CODE_SI;
					kana = 0;
				}
				//���̂܂܏o��
				arOut << (BYTE)by1stChar;
			}
			else if(iskanji1st(by1stChar)) //�V�t�g�i�h�r���o�C�g
			{
				//����ǂݍ���
				arIn >> (BYTE&)by2ndChar;
				if(!state)
				{
					if(kana)
					{
						arOut << (BYTE)CODE_SI;
						kana = 0;
					}
					set2byte(arOut);
					state =	1;
				}
				//SJIS -> JIS
				unConverted = sjis2jis( ((by1stChar<<8)|by2ndChar) );
				//�ϊ����ʏo��
				arOut << (BYTE)(((unConverted>>8)&0x00ff));
				arOut << (BYTE)(unConverted&0x00ff);
			}
			else if(isHanKana(by1stChar)) //���p�J�i�R�[�h
			{
				if (state)
				{
					set1byte(arOut);
					state =	0;
				}
				//�o��
				arOut << (BYTE)by1stChar;
			}
			else
			{
				if (state)
				{
					set1byte(arOut);
					state =	0;
				}
				else if	(kana)
				{
					arOut << (BYTE)CODE_SI;
					kana = 0;
				}
				//�o��
				arOut << (BYTE)by1stChar;
			}

			//�N���A
			by1stChar=0;
			by2ndChar=0;
			//����ǂݍ���
			arIn >> (BYTE&)by1stChar;
		}
	}
	CATCH_ALL(e)
	{
		if(state)
		{
			set1byte(arOut);
			state =	0;
		}
	}
	END_CATCH_ALL

	//���o�̓t�@�C���N���[�Y
	arIn.Close();
	objInFile.Close();
	arOut.Close();
	objOutFile.Close();

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�d�t�b�|���r�i�h�r�ϊ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::ConvertEucToSjis
	(	const char*	szInFilePath,			//���̓t�@�C���p�X
		const char*	szOutFilePath			//�o�̓t�@�C���p�X
	)
										//�֐����^�[��
										// TRUE : ����I��
										// -1   : ���̓t�@�C���I�[�v���G���[
										// -2   : �o�̓t�@�C���I�[�v���G���[
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	UINT	unConverted=0;

	//���̓t�@�C���I�[�v��
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead, NULL ) )
		return -1;
	CArchive	arIn(&objInFile,CArchive::load);

	//�o�̓t�@�C���I�[�v��
	CFile	objOutFile;
	if( !objOutFile.Open( szOutFilePath, (CFile::modeCreate|CFile::modeWrite), NULL ) )
	{
		arIn.Close();
		objInFile.Close();
		return -2;
	}
	CArchive	arOut(&objOutFile,CArchive::store);


	TRY
	{
		//�܂��P�o�C�g�ǂݍ���
		arIn >> (BYTE&)by1stChar;
	
		for(;;) //�P�����Â��[�v
		{
			if( by1stChar < 0x80 )
			{
				//���̂܂܏o��
				arOut << (BYTE)by1stChar;
			}
			else if(iseuc(by1stChar)) //�d�t�b���o�C�g
			{
				//����ǂݍ���
				arIn >> (BYTE&)by2ndChar;
				//EUC -> SJIS
				unConverted = euc2sjis( ((by1stChar<<8)|by2ndChar) );
				//�ϊ����ʏo��
				arOut << (BYTE)(((unConverted>>8)&0x00ff));
				arOut << (BYTE)(unConverted&0x00ff);
			}
			else if(by1stChar==0x8e)
			{
				//����ǂݍ���
				arIn >> (BYTE&)by2ndChar;
				//�o��
				arOut << (BYTE)by2ndChar;
			}
			else if(by1stChar==0x8f)
			{
				//����ǂݍ���
				arIn >> (BYTE&)by2ndChar;
				//�o��
				arOut << (BYTE)by2ndChar;
				//����ǂݍ���
				arIn >> (BYTE&)by2ndChar;
				//�o��
				arOut << (BYTE)by2ndChar;
			}
			else
			{
				//�o��
				arOut << (BYTE)by1stChar;
			}

			//�N���A
			by1stChar=0;
			by2ndChar=0;
			//����ǂݍ���
			arIn >> (BYTE&)by1stChar;
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL

	//���o�̓t�@�C���N���[�Y
	arIn.Close();
	objInFile.Close();
	arOut.Close();
	objOutFile.Close();

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�d�t�b�|���i�h�r�ϊ�													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::ConvertEucToJis
	(	const char*	szInFilePath,			//���̓t�@�C���p�X
		const char*	szOutFilePath			//�o�̓t�@�C���p�X
	)
										//�֐����^�[��
										// TRUE : ����I��
										// -1   : ���̓t�@�C���I�[�v���G���[
										// -2   : �o�̓t�@�C���I�[�v���G���[
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	UINT	unConverted=0;
	char	state, kana;

	//���̓t�@�C���I�[�v��
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead, NULL ) )
		return -1;
	CArchive	arIn(&objInFile,CArchive::load);

	//�o�̓t�@�C���I�[�v��
	CFile	objOutFile;
	if( !objOutFile.Open( szOutFilePath, (CFile::modeCreate|CFile::modeWrite), NULL ) )
	{
		arIn.Close();
		objInFile.Close();
		return -2;
	}
	CArchive	arOut(&objOutFile,CArchive::store);


	TRY
	{
		//�t���O�N���A
		state = kana = 0;
		//�܂��P�o�C�g�ǂݍ���
		arIn >> (BYTE&)by1stChar;
	
		for(;;) //�P�����Â��[�v
		{
			if( by1stChar < 0x80 )
			{
				if (state)
				{
					set1byte(arOut);
					state =	0;
				}
				else if	(kana)
				{
					arOut << (BYTE)CODE_SI;
					kana = 0;
				}
				//���̂܂܏o��
				arOut << (BYTE)by1stChar;
			}
			else if(iseuc(by1stChar)) //�d�t�b���o�C�g
			{
				//����ǂݍ���
				arIn >> (BYTE&)by2ndChar;
				if(!state)
				{
					if(kana)
					{
						arOut << (BYTE)CODE_SI;
						kana = 0;
					}
					set2byte(arOut);
					state =	1;
				}
				//EUC -> JIS
				unConverted = euc2jis( ((by1stChar<<8)|by2ndChar) );
				//�ϊ����ʏo��
				arOut << (BYTE)(((unConverted>>8)&0x00ff));
				arOut << (BYTE)(unConverted&0x00ff);
			}
			else if(by1stChar==0x8e)
			{
				//����ǂݍ���
				arIn >> (BYTE&)by2ndChar;
				if (state)
				{
					set1byte(arOut);
					state =	0;
				}
				//�o��
				arOut << (BYTE)by2ndChar;
			}
			else if(by1stChar==0x8f)
			{
				if (!state)
				{
					if (kana)
					{
						arOut << (BYTE)CODE_SI;
						kana = 0;
					}
					set2byte(arOut);
					state =	1;
				}
				//����ǂݍ���
				arIn >> (BYTE&)by2ndChar;
				//�o��
				arOut << (BYTE)by2ndChar;
				//����ǂݍ���
				arIn >> (BYTE&)by2ndChar;
				//�o��
				arOut << (BYTE)by2ndChar;
			}
			else
			{
				if(state)
				{
					set1byte(arOut);
					state =	0;
				}
				else if	(kana)
				{
					arOut << (BYTE)CODE_SI;
					kana = 0;
				}
				//�o��
				arOut << (BYTE)by1stChar;
			}

			//�N���A
			by1stChar=0;
			by2ndChar=0;
			//����ǂݍ���
			arIn >> (BYTE&)by1stChar;
		}
	}
	CATCH_ALL(e)
	{
		if(state)
		{
			set1byte(arOut);
			state =	0;
		}
	}
	END_CATCH_ALL

	//���o�̓t�@�C���N���[�Y
	arIn.Close();
	objInFile.Close();
	arOut.Close();
	objOutFile.Close();

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�i�h�r�|���r�i�h�r�ϊ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::ConvertJisToSjis
	(	const char*	szInFilePath,			//���̓t�@�C���p�X
		const char*	szOutFilePath			//�o�̓t�@�C���p�X
	)
										//�֐����^�[��
										// TRUE : ����I��
										// -1   : ���̓t�@�C���I�[�v���G���[
										// -2   : �o�̓t�@�C���I�[�v���G���[
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	BYTE	by3rdChar=0;
	UINT	unConverted=0;
	char	state, kana;

	//���̓t�@�C���I�[�v��
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead, NULL ) )
		return -1;
	CArchive	arIn(&objInFile,CArchive::load);

	//�o�̓t�@�C���I�[�v��
	CFile	objOutFile;
	if( !objOutFile.Open( szOutFilePath, (CFile::modeCreate|CFile::modeWrite), NULL ) )
	{
		arIn.Close();
		objInFile.Close();
		return -2;
	}
	CArchive	arOut(&objOutFile,CArchive::store);


	TRY
	{
		//�t���O�N���A
		state = kana = 0;
		//�܂��P�o�C�g�ǂݍ���
		arIn >> (BYTE&)by1stChar;
	
		for(;;) //�P�����Â��[�v
		{
			if( by1stChar == CODE_ESC ) //�G�X�P�[�v�R�[�h
			{
				//����ǂݍ���
				arIn >> (BYTE&)by2ndChar;
				switch(by2ndChar)
				{
				case '$' :
					//����Ɏ���ǂݍ���
					arIn >> (BYTE&)by3rdChar;
					if(by3rdChar=='B' || by3rdChar=='@')
					{
						state = 1;
					}
					else 
					{
						arOut << (BYTE)CODE_ESC;
						arOut << (BYTE)by2ndChar;
						by1stChar = by3rdChar;
						continue;
					}
					break;
				case '(' :
					//����Ɏ���ǂݍ���
					arIn >> (BYTE&)by3rdChar;
					if(by3rdChar=='J' || by3rdChar=='B' || by3rdChar=='H')
					{
						state = 0;
					}
					else
					{
						arOut << (BYTE)CODE_ESC;
						arOut << (BYTE)by2ndChar;
						by1stChar = by3rdChar;
						continue;
					}
					break;
				case 'K':
					state = 1;
					break;
				case 'H':
					state = 0;
					break;
				default:
					arOut << (BYTE)CODE_ESC;
					continue;
				}
			}
			else if(by1stChar<=0x20 || by1stChar==0x7f)
			{
				arOut << (BYTE)by1stChar;
			}
			else if(state)
			{
				//����ǂݍ���
				arIn >> (BYTE&)by2ndChar;
				if(by2ndChar<=0x20)
				{
					arOut << (BYTE)by1stChar;
					by1stChar = by2ndChar;
					continue;
				}
				if(by1stChar<0x80 && isjis(by2ndChar))
				{
					//JIS -> SJIS
					unConverted = jis2sjis( ((by1stChar<<8)|by2ndChar) );
					//�ϊ����ʏo��
					arOut << (BYTE)(((unConverted>>8)&0x00ff));
					arOut << (BYTE)(unConverted&0x00ff);
				}
				else
				{
					arOut << (BYTE)by1stChar;
					arOut << (BYTE)by2ndChar;
				}
			}
			else
			{
				arOut << (BYTE)by1stChar;
			}

			//�N���A
			by1stChar=0;
			by2ndChar=0;
			//����ǂݍ���
			arIn >> (BYTE&)by1stChar;
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL

	//���o�̓t�@�C���N���[�Y
	arIn.Close();
	objInFile.Close();
	arOut.Close();
	objOutFile.Close();

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�i�h�r�|���d�t�b�ϊ�													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::ConvertJisToEuc
	(	const char*	szInFilePath,			//���̓t�@�C���p�X
		const char*	szOutFilePath			//�o�̓t�@�C���p�X
	)
										//�֐����^�[��
										// TRUE : ����I��
										// -1   : ���̓t�@�C���I�[�v���G���[
										// -2   : �o�̓t�@�C���I�[�v���G���[
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	BYTE	by3rdChar=0;
	UINT	unConverted=0;
	char	state, kana;

	//���̓t�@�C���I�[�v��
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead, NULL ) )
		return -1;
	CArchive	arIn(&objInFile,CArchive::load);

	//�o�̓t�@�C���I�[�v��
	CFile	objOutFile;
	if( !objOutFile.Open( szOutFilePath, (CFile::modeCreate|CFile::modeWrite), NULL ) )
	{
		arIn.Close();
		objInFile.Close();
		return -2;
	}
	CArchive	arOut(&objOutFile,CArchive::store);


	TRY
	{
		//�t���O�N���A
		state = kana = 0;
		//�܂��P�o�C�g�ǂݍ���
		arIn >> (BYTE&)by1stChar;
	
		for(;;) //�P�����Â��[�v
		{
			if( by1stChar == CODE_ESC ) //�G�X�P�[�v�R�[�h
			{
				//����ǂݍ���
				arIn >> (BYTE&)by2ndChar;
				switch(by2ndChar)
				{
				case '$' :
					//����Ɏ���ǂݍ���
					arIn >> (BYTE&)by3rdChar;
					if(by3rdChar=='B' || by3rdChar=='@')
					{
						state = 1;
					}
					else 
					{
						arOut << (BYTE)CODE_ESC;
						arOut << (BYTE)by2ndChar;
						by1stChar = by3rdChar;
						continue;
					}
					break;
				case '(' :
					//����Ɏ���ǂݍ���
					arIn >> (BYTE&)by3rdChar;
					if(by3rdChar=='J' || by3rdChar=='B' || by3rdChar=='H')
					{
						state = 0;
					}
					else
					{
						arOut << (BYTE)CODE_ESC;
						arOut << (BYTE)by2ndChar;
						by1stChar = by3rdChar;
						continue;
					}
					break;
				case 'K':
					state = 1;
					break;
				case 'H':
					state = 0;
					break;
				default:
					arOut << (BYTE)CODE_ESC;
					continue;
				}
			}
			else if(by1stChar<=0x20 || by1stChar==0x7f)
			{
				arOut << (BYTE)by1stChar;
			}
			else if(state)
			{
				//����ǂݍ���
				arIn >> (BYTE&)by2ndChar;
				if(by2ndChar<=0x20 || by2ndChar==0x7f)
				{
					arOut << (BYTE)by1stChar;
					by1stChar = by2ndChar;
					continue;
				}
				//JIS -> EUC
				unConverted = jis2euc( ((by1stChar<<8)|by2ndChar) );
				//�ϊ����ʏo��
				arOut << (BYTE)(((unConverted>>8)&0x00ff));
				arOut << (BYTE)(unConverted&0x00ff);
			}
			else
			{
				if(isHanKana(by1stChar)) //���p�J�i�R�[�h
				{
					arOut << (BYTE)0x8e;
				}
				arOut << (BYTE)by1stChar;
			}

			//�N���A
			by1stChar=0;
			by2ndChar=0;
			//����ǂݍ���
			arIn >> (BYTE&)by1stChar;
		}
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL

	//���o�̓t�@�C���N���[�Y
	arIn.Close();
	objInFile.Close();
	arOut.Close();
	objOutFile.Close();

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����񒆂̃J�^�J�i ���p�|���S�p�ϊ�									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::StringKatakanaHan2Zen
	(	CString*	pobjLineIn
	)
{
	long lCnt=0;
	BYTE byBuf=0, byBuf2=0;
	CString objLine;
	UINT nZenKana=0;

	if(pobjLineIn == NULL)
		return -1;
	if(pobjLineIn->GetLength() == 0)
		return -1;

	for(lCnt=0; lCnt<pobjLineIn->GetLength(); )
	{
		byBuf = pobjLineIn->GetAt(lCnt);

		if(IsDBCSLeadByteEx(0,byBuf))//�Q�o�C�g�����̑��o�C�g
		{
			objLine += (UCHAR)byBuf;
			lCnt++;
			byBuf = pobjLineIn->GetAt(lCnt);
			objLine += (UCHAR)byBuf;
			lCnt++;
			continue;
		}
		else //���p
		{
			if( isHanKana(byBuf) ) //���p�J�^�J�i
			{
				//����
			}
			else
			{
				objLine += (UCHAR)byBuf;
				lCnt++;
				continue;
			}
		}

		byBuf2 = pobjLineIn->GetAt(lCnt+1);

		if( !IsDBCSLeadByteEx(0,byBuf2) && byBuf2 == 0xde ) // ���� �
		{
			//�S�p�J�^�J�i�ϊ����}��
			nZenKana = _mbbtombc(byBuf);
			nZenKana = AddDakuonToKana(nZenKana);
			objLine += (UCHAR)((nZenKana>>8)&0x00ff);
			objLine += (UCHAR)(nZenKana&0x00ff);
			lCnt+=2;
		}
		else if( !IsDBCSLeadByteEx(0,byBuf2) && byBuf2 == 0xdf ) // ���� �
		{
			//�S�p�J�^�J�i�ϊ����}��
			nZenKana = _mbbtombc(byBuf);
			nZenKana = AddMaruToKana(nZenKana);
			objLine += (UCHAR)(((nZenKana>>8)&0x00ff));
			objLine += (UCHAR)(nZenKana&0x00ff);
			lCnt+=2;
		}
		else
		{
			//�S�p�J�^�J�i�ϊ����}��
			nZenKana = _mbbtombc(byBuf);
			objLine += (UCHAR)(((nZenKana>>8)&0x00ff));
			objLine += (UCHAR)(nZenKana&0x00ff);
			lCnt++;
		}
	}

	//�ϊ��㕶�����Ԃ�
	(*pobjLineIn) = objLine;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�������SJIS->EUC�R�[�h�ϊ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::StringSjisToEuc
	(	CString*	pobjLineIn
	)
{
	long	lCnt=0;
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	UINT	unConverted=0;
	CString	objLine;

	if(pobjLineIn == NULL)
		return -1;
	if(pobjLineIn->GetLength() == 0)
		return -1;

	//�܂��P�o�C�g�ǂݍ���
	lCnt=0;
	by1stChar = pobjLineIn->GetAt(lCnt);
	lCnt++;

	for(;;) //�P�����Â��[�v
	{
		if( by1stChar < 0x80 )
		{
			//���̂܂܏o��
			objLine += (UCHAR)by1stChar;
		}
		else if(iskanji1st(by1stChar)) //�V�t�g�i�h�r���o�C�g
		{
			//����ǂݍ���
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			if(iskanji2nd(by2ndChar))
			{
				//SJIS -> EUC
				unConverted = sjis2euc( ((by1stChar<<8)|by2ndChar) );
				//�ϊ����ʏo��
				objLine += (UCHAR)(((unConverted>>8)&0x00ff));
				objLine += (UCHAR)(unConverted&0x00ff);
			}
			else
			{
				//�o��
				objLine += (UCHAR)by1stChar;
				objLine += (UCHAR)by2ndChar;
			}
		}
		else if(isHanKana(by1stChar)) //���p�J�i�R�[�h
		{
			//�o��
			objLine += (UCHAR)0x8e;
			objLine += (UCHAR)by1stChar;
		}
		else
		{
			//�o��
			objLine += (UCHAR)by1stChar;
		}

		//�N���A
		by1stChar=0;
		by2ndChar=0;
		//����ǂݍ���
		if(lCnt >= pobjLineIn->GetLength())
			break;
		by1stChar = pobjLineIn->GetAt(lCnt);
		lCnt++;
	}

	//�ϊ��㕶�����Ԃ�
	(*pobjLineIn) = objLine;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�������SJIS->JIS�R�[�h�ϊ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::StringSjisToJis
	(	CString*	pobjLineIn			//���͕�����
	)
{
	long	lCnt=0;
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	UINT	unConverted=0;
	char	state, kana;
	CString	objLine;

	if(pobjLineIn == NULL)
		return -1;
	if(pobjLineIn->GetLength() == 0)
		return -1;

	//�t���O�N���A
	state = kana = 0;
	//�܂��P�o�C�g�ǂݍ���
	lCnt=0;
	by1stChar = pobjLineIn->GetAt(lCnt);
	lCnt++;

	for(;;) //�P�����Â��[�v
	{
		if( by1stChar < 0x80 ) //ASCII
		{
			if (state)
			{
				set1byte(&objLine);
				state =	0;
			}
			else if	(kana)
			{
				objLine += (UCHAR)CODE_SI;
				kana = 0;
			}
			//���̂܂܏o��
			objLine += (UCHAR)by1stChar;
		}
		else if(iskanji1st(by1stChar)) //�V�t�g�i�h�r���o�C�g
		{
			//����ǂݍ���
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			if(!state)
			{
				if(kana)
				{
					objLine += (UCHAR)CODE_SI;
					kana = 0;
				}
				set2byte(&objLine);
				state =	1;
			}
			//SJIS -> JIS
			unConverted = sjis2jis( ((by1stChar<<8)|by2ndChar) );
			//�ϊ����ʏo��
			objLine += (UCHAR)(((unConverted>>8)&0x00ff));
			objLine += (UCHAR)(unConverted&0x00ff);
		}
		else if(isHanKana(by1stChar)) //���p�J�i�R�[�h
		{
			if (state)
			{
				set1byte(&objLine);
				state =	0;
			}
			//�o��
			objLine += (UCHAR)by1stChar;
		}
		else
		{
			if (state)
			{
				set1byte(&objLine);
				state =	0;
			}
			else if	(kana)
			{
				objLine += (UCHAR)CODE_SI;
				kana = 0;
			}
			//�o��
			objLine += (UCHAR)by1stChar;
		}

		//�N���A
		by1stChar=0;
		by2ndChar=0;
		//����ǂݍ���
		if(lCnt >= pobjLineIn->GetLength())
			break;
		by1stChar = pobjLineIn->GetAt(lCnt);
		lCnt++;
	}
	if(state)
	{
		set1byte(&objLine);
		state =	0;
	}

	//�ϊ��㕶�����Ԃ�
	(*pobjLineIn) = objLine;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�������EUC->SJIS�R�[�h�ϊ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::StringEucToSjis
	(	CString*	pobjLineIn
	)
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	UINT	unConverted=0;
	long	lCnt=0;
	CString	objLine;

	if(pobjLineIn == NULL)
		return -1;
	if(pobjLineIn->GetLength() == 0)
		return -1;

	//�܂��P�o�C�g�ǂݍ���
	lCnt = 0;
	by1stChar = pobjLineIn->GetAt(lCnt);
	lCnt++;

	for(;;) //�P�����Â��[�v
	{
		if( by1stChar < 0x80 )
		{
			//���̂܂܏o��
			objLine += (UCHAR)by1stChar;
		}
		else if(iseuc(by1stChar)) //�d�t�b���o�C�g
		{
			//����ǂݍ���
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			//EUC -> SJIS
			unConverted = euc2sjis( ((by1stChar<<8)|by2ndChar) );
			//�ϊ����ʏo��
			objLine += (UCHAR)(((unConverted>>8)&0x00ff));
			objLine += (UCHAR)(unConverted&0x00ff);
		}
		else if(by1stChar==0x8e)
		{
			//����ǂݍ���
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			//�o��
			objLine += (UCHAR)by2ndChar;
		}
		else if(by1stChar==0x8f)
		{
			//����ǂݍ���
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			//�o��
			objLine += (UCHAR)by2ndChar;
			//����ǂݍ���
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			//�o��
			objLine += (UCHAR)by2ndChar;
		}
		else
		{
			//�o��
			objLine += (UCHAR)by1stChar;
		}

		//�N���A
		by1stChar=0;
		by2ndChar=0;
		//����ǂݍ���
		if(lCnt >= pobjLineIn->GetLength())
			break;
		by1stChar = pobjLineIn->GetAt(lCnt);
		lCnt++;
	}

	//�ϊ��㕶�����Ԃ�
	(*pobjLineIn) = objLine;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�������EUC->JIS�R�[�h�ϊ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::StringEucToJis
	(	CString*	pobjLineIn
	)
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	UINT	unConverted=0;
	char	state, kana;
	long	lCnt=0;
	CString	objLine;

	if(pobjLineIn == NULL)
		return -1;
	if(pobjLineIn->GetLength() == 0)
		return -1;

	//�t���O�N���A
	state = kana = 0;
	//�܂��P�o�C�g�ǂݍ���
	lCnt = 0;
	by1stChar = pobjLineIn->GetAt(lCnt);
	lCnt++;

	for(;;) //�P�����Â��[�v
	{
		if( by1stChar < 0x80 )
		{
			if (state)
			{
				set1byte(&objLine);
				state =	0;
			}
			else if	(kana)
			{
				objLine += (UCHAR)CODE_SI;
				kana = 0;
			}
			//���̂܂܏o��
			objLine += (UCHAR)by1stChar;
		}
		else if(iseuc(by1stChar)) //�d�t�b���o�C�g
		{
			//����ǂݍ���
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			if(!state)
			{
				if(kana)
				{
					objLine += (UCHAR)CODE_SI;
					kana = 0;
				}
				set2byte(&objLine);
				state =	1;
			}
			//EUC -> JIS
			unConverted = euc2jis( ((by1stChar<<8)|by2ndChar) );
			//�ϊ����ʏo��
			objLine += (UCHAR)(((unConverted>>8)&0x00ff));
			objLine += (UCHAR)(unConverted&0x00ff);
		}
		else if(by1stChar==0x8e)
		{
			//����ǂݍ���
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			if (state)
			{
				set1byte(&objLine);
				state =	0;
			}
			//�o��
			objLine += (UCHAR)by2ndChar;
		}
		else if(by1stChar==0x8f)
		{
			if (!state)
			{
				if (kana)
				{
					objLine += (UCHAR)CODE_SI;
					kana = 0;
				}
				set2byte(&objLine);
				state =	1;
			}
			//����ǂݍ���
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			//�o��
			objLine += (UCHAR)by2ndChar;
			//����ǂݍ���
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			//�o��
			objLine += (UCHAR)by2ndChar;
		}
		else
		{
			if(state)
			{
				set1byte(&objLine);
				state =	0;
			}
			else if	(kana)
			{
				objLine += (UCHAR)CODE_SI;
				kana = 0;
			}
			//�o��
			objLine += (UCHAR)by1stChar;
		}

		//�N���A
		by1stChar=0;
		by2ndChar=0;
			//����ǂݍ���
		if(lCnt >= pobjLineIn->GetLength())
			break;
		by1stChar = pobjLineIn->GetAt(lCnt);
		lCnt++;
	}
	if(state)
	{
		set1byte(&objLine);
		state =	0;
	}

	//�ϊ��㕶�����Ԃ�
	(*pobjLineIn) = objLine;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�������JIS->SJIS�R�[�h�ϊ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::StringJisToSjis
	(	CString*	pobjLineIn
	)
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	BYTE	by3rdChar=0;
	UINT	unConverted=0;
	char	state, kana;
	long	lCnt=0;
	CString	objLine;

	if(pobjLineIn == NULL)
		return -1;
	if(pobjLineIn->GetLength() == 0)
		return -1;


	//�t���O�N���A
	state = kana = 0;
		//�܂��P�o�C�g�ǂݍ���
	lCnt=0;
	by1stChar = pobjLineIn->GetAt(lCnt);
	lCnt++;

	for(;;) //�P�����Â��[�v
	{
		if( by1stChar == CODE_ESC ) //�G�X�P�[�v�R�[�h
		{
			//����ǂݍ���
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			switch(by2ndChar)
			{
			case '$' :
				//����Ɏ���ǂݍ���
				by3rdChar = pobjLineIn->GetAt(lCnt);
				lCnt++;
				if(by3rdChar=='B' || by3rdChar=='@')
				{
					state = 1;
				}
				else 
				{
					objLine += (UCHAR)CODE_ESC;
					objLine += (UCHAR)by2ndChar;
					by1stChar = by3rdChar;
					continue;
				}
				break;
			case '(' :
				//����Ɏ���ǂݍ���
				by3rdChar = pobjLineIn->GetAt(lCnt);
				lCnt++;
				if(by3rdChar=='J' || by3rdChar=='B' || by3rdChar=='H')
				{
					state = 0;
				}
				else
				{
					objLine += (UCHAR)CODE_ESC;
					objLine += (UCHAR)by2ndChar;
					by1stChar = by3rdChar;
					continue;
				}
				break;
			case 'K':
				state = 1;
				break;
			case 'H':
				state = 0;
				break;
			default:
				objLine += (UCHAR)CODE_ESC;
				continue;
			}
		}
		else if(by1stChar<=0x20 || by1stChar==0x7f)
		{
			objLine += (UCHAR)by1stChar;
		}
		else if(state)
		{
			//����ǂݍ���
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			if(by2ndChar<=0x20)
			{
				objLine += (UCHAR)by1stChar;
				by1stChar = by2ndChar;
				continue;
			}
			if(by1stChar<0x80 && isjis(by2ndChar))
			{
				//JIS -> SJIS
				unConverted = jis2sjis( ((by1stChar<<8)|by2ndChar) );
				//�ϊ����ʏo��
				objLine += (UCHAR)(((unConverted>>8)&0x00ff));
				objLine += (UCHAR)(unConverted&0x00ff);
			}
			else
			{
				objLine += (UCHAR)by1stChar;
				objLine += (UCHAR)by2ndChar;
			}
		}
		else
		{
			objLine += (UCHAR)by1stChar;
		}

		//�N���A
		by1stChar=0;
		by2ndChar=0;
		//����ǂݍ���
		if(lCnt >= pobjLineIn->GetLength())
			break;
		by1stChar = pobjLineIn->GetAt(lCnt);
		lCnt++;
	}

	//�ϊ��㕶�����Ԃ�
	(*pobjLineIn) = objLine;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�������JIS->EUC�R�[�h�ϊ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
INT CCodeConvManager::StringJisToEuc
	(	CString*	pobjLineIn
	)
{
	BYTE	by1stChar=0;
	BYTE	by2ndChar=0;
	BYTE	by3rdChar=0;
	UINT	unConverted=0;
	char	state, kana;
	long	lCnt=0;
	CString	objLine;

	if(pobjLineIn == NULL)
		return -1;
	if(pobjLineIn->GetLength() == 0)
		return -1;


	//�t���O�N���A
	state = kana = 0;
	//�܂��P�o�C�g�ǂݍ���
	lCnt=0;
	by1stChar = pobjLineIn->GetAt(lCnt);
	lCnt++;

	for(;;) //�P�����Â��[�v
	{
		if( by1stChar == CODE_ESC ) //�G�X�P�[�v�R�[�h
		{
			//����ǂݍ���
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			switch(by2ndChar)
			{
			case '$' :
				//����Ɏ���ǂݍ���
				by3rdChar = pobjLineIn->GetAt(lCnt);
				lCnt++;
				if(by3rdChar=='B' || by3rdChar=='@')
				{
					state = 1;
				}
				else 
				{
					objLine += (UCHAR)CODE_ESC;
					objLine += (UCHAR)by2ndChar;
					by1stChar = by3rdChar;
					continue;
				}
				break;
			case '(' :
				//����Ɏ���ǂݍ���
				by3rdChar = pobjLineIn->GetAt(lCnt);
				lCnt++;
				if(by3rdChar=='J' || by3rdChar=='B' || by3rdChar=='H')
				{
					state = 0;
				}
				else
				{
					objLine += (UCHAR)CODE_ESC;
					objLine += (UCHAR)by2ndChar;
					by1stChar = by3rdChar;
					continue;
				}
				break;
			case 'K':
				state = 1;
				break;
			case 'H':
				state = 0;
				break;
			default:
				objLine += (UCHAR)CODE_ESC;
				continue;
			}
		}
		else if(by1stChar<=0x20 || by1stChar==0x7f)
		{
			objLine += (UCHAR)by1stChar;
		}
		else if(state)
		{
			//����ǂݍ���
			by2ndChar = pobjLineIn->GetAt(lCnt);
			lCnt++;
			if(by2ndChar<=0x20 || by2ndChar==0x7f)
			{
				objLine += (UCHAR)by1stChar;
				by1stChar = by2ndChar;
				continue;
			}
			//JIS -> EUC
			unConverted = jis2euc( ((by1stChar<<8)|by2ndChar) );
			//�ϊ����ʏo��
			objLine += (UCHAR)(((unConverted>>8)&0x00ff));
			objLine += (UCHAR)(unConverted&0x00ff);
		}
		else
		{
			if(isHanKana(by1stChar)) //���p�J�i�R�[�h
			{
				objLine += (UCHAR)0x8e;
			}
			objLine += (UCHAR)by1stChar;
		}

		//�N���A
		by1stChar=0;
		by2ndChar=0;
		//����ǂݍ���
		if(lCnt >= pobjLineIn->GetLength())
			break;
		by1stChar = pobjLineIn->GetAt(lCnt);
		lCnt++;
	}

	//�ϊ��㕶�����Ԃ�
	(*pobjLineIn) = objLine;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�p�A�v�A�|�@�Ȃǂ̊ەt���J�^�J�i�̔���								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CCodeConvManager::IsMaruKana(unsigned int kana)
{
	if(kana==0x8370) 		return TRUE;	// �p
	else if(kana==0x8373) 	return TRUE;	// �s
	else if(kana==0x8376) 	return TRUE;	// �v
	else if(kana==0x8379) 	return TRUE;	// �y
	else if(kana==0x837C) 	return TRUE;	// �|
	else					return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�o�A�K�A�U�@�Ȃǂ̑����t���J�^�J�i�̔���								|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CCodeConvManager::IsDakuonKana(unsigned int kana)
{
	if(kana==0x8394) 		return TRUE;	// ��
	else if(kana==0x834B) 	return TRUE;	// �K
	else if(kana==0x834D) 	return TRUE;	// �M
	else if(kana==0x834F) 	return TRUE;	// �O
	else if(kana==0x8351) 	return TRUE;	// �Q
	else if(kana==0x8353) 	return TRUE;	// �S
	else if(kana==0x8355) 	return TRUE;	// �U
	else if(kana==0x8357) 	return TRUE;	// �W
	else if(kana==0x8359) 	return TRUE;	// �Y
	else if(kana==0x835B) 	return TRUE;	// �[
	else if(kana==0x835D) 	return TRUE;	// �]
	else if(kana==0x835F) 	return TRUE;	// �_
	else if(kana==0x8361) 	return TRUE;	// �a
	else if(kana==0x8364) 	return TRUE;	// �d
	else if(kana==0x8366) 	return TRUE;	// �f
	else if(kana==0x8368) 	return TRUE;	// �h
	else if(kana==0x836F) 	return TRUE;	// �o
	else if(kana==0x8372) 	return TRUE;	// �r
	else if(kana==0x8375) 	return TRUE;	// �u
	else if(kana==0x8378) 	return TRUE;	// �x
	else if(kana==0x837B) 	return TRUE;	// �{
	else 					return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�S�p�J�^�J�i�Ɋۂ�ǉ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::AddMaruToKana(unsigned int kana)
{
	unsigned int unBack = kana;

	//�n��836E�A�p��8370�Ȃǂ̂悤�ɂQ��������Ɗۂ���
	kana += 2;

	if(IsMaruKana(kana))
		return kana;
	else
		return unBack;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�S�p�J�^�J�i�ɑ�����ǉ�												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::AddDakuonToKana(unsigned int kana)
{
	unsigned int unBack = kana;

	if(kana == 0x8345)
	{
		kana = 0x8394;	//�E��0x8345 ----> ����0x8394
	}
	else
	{
		//�J��0x834A�A�K��0x834B�Ȃǂ̂悤�ɂP��������Ƒ�������
		kana += 1;
	}

	if(IsDakuonKana(kana))
		return kana;
	else
		return unBack;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�p�����@���p�|���S�p													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::AlnumToZen
	(	unsigned int unAlnum
	)
{
	if(unAlnum>='0' && unAlnum<='9')
		return (unAlnum - '0' + 0x824F);
	else if(unAlnum>='a' && unAlnum<='z')
		return (unAlnum - 'a' + 0x8281);
	else if(unAlnum>='A' && unAlnum<='Z')
		return (unAlnum - 'A' + 0x8260);
	else
		return unAlnum;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�p�����@�S�p�|�����p													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::AlnumToHan
	(	unsigned int unAlnum
	)
{
	if(unAlnum>=0x824F && unAlnum<=0x8258) //�O�`�X
		return (unAlnum - 0x824F + 0x30);
	else if(unAlnum>=0x8281 && unAlnum<=0x829A) //���`��
		return (unAlnum - 0x8281 + 'a');
	else if(unAlnum>=0x8260 && unAlnum<=0x8279) //�`�`�y
		return (unAlnum - 0x8260 + 'A');
	else
		return unAlnum;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�S�p�p��������														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CCodeConvManager::IsZenkakuAlnum
	(	unsigned int unAlnum
	)
{
	if(unAlnum>=0x824F && unAlnum<=0x8258) //�O�`�X
		return TRUE;
	else if(unAlnum>=0x8281 && unAlnum<=0x829A) //���`��
		return TRUE;
	else if(unAlnum>=0x8260 && unAlnum<=0x8279) //�`�`�y
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CCodeConvManager �v���e�N�g�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�o�C�i���t�@�C���`�F�b�N												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CCodeConvManager::IsBinary
	(	const char*	szInFilePath			//���̓t�@�C���p�X
	)
{
	UCHAR	buf[1024];
	UINT	nBytesRead;
	UINT	nCnt, nBinaryCnt;
	char binarytbl[32] = 
	{
		1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  1,  0,  0,  0,  0,
	 //NUL,SOH,STX,ETX,EOT,ENQ,ACK,BEL,BS, HT, LF, VT, FF, CR, SO, SI,
		1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1,  1
	 //DLE,DC1,DC2,DC3,DC4,NAC,SYN,ETB,CAN,EM, SUB,ESC,FS, GS, RS, US,
	};

	//���̓t�@�C������ő�P�O�Q�S�o�C�g�ǂ�
	CFile	objInFile;
	if( !objInFile.Open( szInFilePath, CFile::modeRead|CFile::typeBinary, NULL ) )
		return( FALSE );
	memset(buf,0,sizeof(buf));
	nBytesRead = objInFile.Read( buf, 1024 );

	//�o�C�i���f�[�^���J�E���g
	nBinaryCnt = 0;
	for(nCnt=0; nCnt<nBytesRead; nCnt++)
	{
		if((buf[nCnt]<0x20)	&& binarytbl[buf[nCnt]])
			nBinaryCnt++;
	}

	objInFile.Close();

	return ((nBinaryCnt<<5) > nBytesRead);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�[�h����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CCodeConvManager::whatKanji(UCHAR* str)
									//�֐����^�[��
									//�E����str�̐擪���������p�J�i�̏ꍇ�A�֐��l��1
									//�E����str�̐擪2�������V�t�gJIS�R�[�h�̏ꍇ�A�֐��l��2
									//�E����str�̐擪2������EUC���p�J�i�R�[�h�̏ꍇ�A�֐��l��4
									//�E����str�̐擪2������EUC�S�p�R�[�h�̏ꍇ�A�֐��l��8
									//�E����str�̐擪3�������VJIS(X0208-1983)�V�t�g�R�[�h�̏ꍇ�A
									//�@�֐��l��16
									//�E����str�̐擪3��������JIS(X0208-1978)�V�t�g�R�[�h�̏ꍇ�A
									//�@�֐��l��32
									//�E����str�̐擪3������JIS���[�}��(X0201)�V�t�g�R�[�h�̏ꍇ�A
									//�@�֐��l��64
									//�E����str�̐擪3������ASCII �V�t�g�R�[�h�̏ꍇ�A�֐��l��128
									//�E��̃P�[�X�����������ɋN����ꍇ�́A�֐��l�͂����̘_���a
									//�E��̂�����ł��Ȃ��ꍇ�́A�֐��l��0
{
//�E���p�J�i�R�[�h
//		0xa0 - 0xdf �̊Ԃ��g���B
//
//�EJIS�R�[�h
//		7�r�b�g�n�����R�[�h�ł��邽�߁A���̂悤�ȃV�t�g�R�[�h���g���āA
//		�����Z�b�g�̐؂�ւ����s���A���Ȋ�����\������B
//
//		�VJIS X0208-1983	�F		ESC $ B
//		��JIS X0208-1978 	�F		ESC $ @
//		JIS���[�}�� X0201 	�F      ESC ( J
//		ASCII 				�F      ESC ( B
//
//�EShift JIS�R�[�h
//		��1�o�C�g�Ƃ��� 0x81 - 0x9f �܂ł� 0xe0 - 0xfc �܂ŁA
//		��2�o�C�g�Ƃ��� 0x40 - 0xfc �܂Łi0x7f�������j���g���B
//		���� ���߁A�V�t�gJIS�͏�L�̔��p�R�[�h�ƍ��݂ł���B
//		�������A���䕶�� �Ƃ��Ďg���� �_�i�o�b�N�X���b�V���܂��͉~�L���j
//		�Ȃǂ���2�o�C�g �̗̈�ɓ����Ă��邽�߂ɖ�肪�����邱�Ƃ�����B
//
//�EEUC�R�[�h
//		��1�A��2�o�C�g�Ƃ� 0xa1 - 0xfe �܂łł��邪�A
//		���p�� �J�i�����ɂ��ẮA0x8e �𔼊p�J�i�����̑O�ɂ��A
//		2�o�C�g�ŕ\�� ����B
//

	UINT val = 0;
	unsigned char b1, b2, b3;

	b1 = *str++;
	b2 = *str++;
	b3 = *str;
	if(b1 == 0x1b) 								//JIS�R�[�h
	{
		if(b2 == '$' && b3 == 'B') return 16;
		if(b2 == '$' && b3 == '@') return 32;
		if(b2 == '(' && b3 == 'J') return 64;
		if(b2 == '(' && b3 == 'B') return 128;
		return 0;
	}

	if( b1 >= 0xa0 && b1 <= 0xdf) 				//���p�R�[�h�̉\��
		val |= 1;
	if( ( ((b1 >= 0x81) && (b1 <= 0x9f)) ||		//Shift Jis �̉\��
		  ((b1 >= 0xe0) && (b1 <= 0xfc)) 
		)									&& 
		( (b2 >= 0x40) && 
		  (b2 <= 0xfc) && 
		  (b2 != 0x7f) 
		)
	)
	{
		val |= 2;
	}
	if( (b1 == 0x8e) && (b2 >= 0xa0 && b2 <= 0xdf))	//���pEUC �̉\��
		val |= 4;
	if( (b1 >= 0xa1 && b1 <= 0xfe) &&				//�S�pEUC �̉\��
		(b2 >= 0xa1 && b2 <= 0xfe)
	)
	{
		val |= 8;
	}

	return val;				//�P�ɂȂ邱�Ƃ�����	�|���@���p�R�[�h�����ł͔���s��
							//�R�ɂȂ邱�Ƃ͂Ȃ�
							//�T�ɂȂ邱�Ƃ͂Ȃ�
							//�X�ɂȂ邱�Ƃ�����	�|���@EUC��������Ȃ�
							//�U�ɂȂ邱�Ƃ�����	�|���@SJIS EUC���݂̂��ߔ���s��
							//�P�O�ɂȂ邱�Ƃ�����	�|���@SJIS EUC���݂̂��ߔ���s��
							//�P�Q�Ȃ邱�Ƃ͂Ȃ�
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�r�i�h�r�|���d�t�b													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::sjis2euc(unsigned int sjis)
{
	unsigned int hib, lob;

	hib	= (sjis	>> 8) &	0xff;
	lob	= sjis & 0xff;

	hib	-= (hib	<= 0x9f) ? 0x70	: 0xb0;
	hib	<<=	1;
	if (lob	< 0x9f)	{
		lob	-= (lob	< 0x7f)	? 0x1f : 0x20;
		hib--;
	}
	else
		lob	-= 0x7e;

	hib	|= 0x80;
	lob	|= 0x80;
	return (hib	<< 8) |	lob;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�r�i�h�r�|���i�h�r													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::sjis2jis(unsigned int sjis)
{
	unsigned int hib, lob;

	hib	= (sjis	>> 8) &	0xff;
	lob	= sjis & 0xff;

	hib	-= (hib	<= 0x9f) ? 0x70	: 0xb0;
	hib	<<=	1;
	if (lob	< 0x9f)	{
		lob	-= (lob	< 0x7f)	? 0x1f : 0x20;
		hib--;
	}
	else
		lob	-= 0x7e;

	return (hib	<< 8) |	lob;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�d�t�b�|���r�i�h�r													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::euc2sjis(unsigned int euc)
{
	unsigned int jis;
	unsigned int hib, lob;

	if ((euc & 0xff00) == 0x8e00)
		jis = KanaHan2Zen(euc & 0xff);
	else
		jis = euc & ~0x8080;

	hib = (jis >> 8) & 0xff;
	lob = jis & 0xff;
	lob += (hib & 1) ? 0x1f : 0x7d;
	if(lob >= 0x7f)
		lob++;
	hib = ((hib - 0x21) >> 1) + 0x81;
	if(hib > 0x9f)
		hib += 0x40;

	return (hib << 8) | lob;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�d�t�b�|���i�h�r														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::euc2jis(unsigned int euc)
{
	unsigned int jis;

	if((euc & 0xff00) == 0x8e00)
		jis = KanaHan2Zen(euc & 0xff);
	else
		jis = euc & ~0x8080;

	return jis;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�i�h�r�|���r�i�h�r													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::jis2sjis(unsigned int jis)
{
	unsigned int hib, lob;

	hib = (jis >> 8) & 0xff;
	lob = jis & 0xff;
	lob += (hib & 1) ? 0x1f : 0x7d;
	if(lob >= 0x7f) 
		lob++;
	hib = ((hib - 0x21) >> 1) + 0x81;
	if(hib > 0x9f)
		hib += 0x40;

	return (hib << 8) | lob;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�i�h�r�|���d�t�b														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::jis2euc(unsigned int jis)
{
	return jis | 0x8080;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���p�J�i��S�p�J�i�ɕϊ�����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::KanaHan2Zen(unsigned int hankaku)
{
	static unsigned int z[64] = 
	{
		0x2121,0x2123,0x2156,0x2157,0x2122,0x2126,0x2572,0x2521,
		0x2523,0x2525,0x2527,0x2529,0x2563,0x2565,0x2567,0x2543,
		0x213c,0x2522,0x2524,0x2526,0x2528,0x252a,0x252b,0x252d,
		0x252f,0x2531,0x2533,0x2535,0x2537,0x2539,0x253b,0x253d,
		0x253f,0x2541,0x2544,0x2546,0x2548,0x254a,0x254b,0x254c,
		0x254d,0x254e,0x254f,0x2552,0x2555,0x2558,0x255b,0x255e,
		0x255f,0x2560,0x2561,0x2562,0x2564,0x2566,0x2568,0x2569,
		0x256a,0x256b,0x256c,0x256d,0x256f,0x2573,0x212b,0x212c 
	};

	if(hankaku < 0xa0 || hankaku > 0xdf)
		return 0;

	return z[hankaku - 0xa0];
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�S�p�J�i�𔼊p�J�i�ɕϊ�����											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned int CCodeConvManager::KanaZen2Han(unsigned int zenkaku)
{
	if(zenkaku == 0x2121)			return 0xa0;
	else if(zenkaku == 0x2123)		return 0xa1;
	else if(zenkaku == 0x2156)		return 0xa2;
	else if(zenkaku == 0x2157)		return 0xa3;
	else if(zenkaku == 0x2122)		return 0xa4;
	else if(zenkaku == 0x2126)		return 0xa5;
	else if(zenkaku == 0x2572)		return 0xa6;
	else if(zenkaku == 0x2521)		return 0xa7;
	else if(zenkaku == 0x2523)		return 0xa8;
	else if(zenkaku == 0x2525)		return 0xa9;
	else if(zenkaku == 0x2527)		return 0xaa;
	else if(zenkaku == 0x2529)		return 0xab;
	else if(zenkaku == 0x2563)		return 0xac;
	else if(zenkaku == 0x2565)		return 0xad;
	else if(zenkaku == 0x2567)		return 0xae;
	else if(zenkaku == 0x2543)		return 0xaf;
	else if(zenkaku == 0x213c)		return 0xb0;
	else if(zenkaku == 0x2522)		return 0xb1;
	else if(zenkaku == 0x2524)		return 0xb2;
	else if(zenkaku == 0x2526)		return 0xb3;
	else if(zenkaku == 0x2528)		return 0xb4;
	else if(zenkaku == 0x252a)		return 0xb5;
	else if(zenkaku == 0x252b)		return 0xb6;
	else if(zenkaku == 0x252d)		return 0xb7;
	else if(zenkaku == 0x252f)		return 0xb8;
	else if(zenkaku == 0x2531)		return 0xb9;
	else if(zenkaku == 0x2533)		return 0xba;
	else if(zenkaku == 0x2535)		return 0xbb;
	else if(zenkaku == 0x2537)		return 0xbc;
	else if(zenkaku == 0x2539)		return 0xbd;
	else if(zenkaku == 0x253b)		return 0xbe;
	else if(zenkaku == 0x253d)		return 0xbf;
	else if(zenkaku == 0x253f)		return 0xc0;
	else if(zenkaku == 0x2541)		return 0xc1;
	else if(zenkaku == 0x2544)		return 0xc2;
	else if(zenkaku == 0x2546)		return 0xc3;
	else if(zenkaku == 0x2548)		return 0xc4;
	else if(zenkaku == 0x254a)		return 0xc5;
	else if(zenkaku == 0x254b)		return 0xc6;
	else if(zenkaku == 0x254c)		return 0xc7;
	else if(zenkaku == 0x254d)		return 0xc8;
	else if(zenkaku == 0x254e)		return 0xc9;
	else if(zenkaku == 0x254f)		return 0xca;
	else if(zenkaku == 0x2552)		return 0xcb;
	else if(zenkaku == 0x2555)		return 0xcc;
	else if(zenkaku == 0x2558)		return 0xcd;
	else if(zenkaku == 0x255b)		return 0xce;
	else if(zenkaku == 0x255e)		return 0xcf;
	else if(zenkaku == 0x255f)		return 0xd0;
	else if(zenkaku == 0x2560)		return 0xd1;
	else if(zenkaku == 0x2561)		return 0xd2;
	else if(zenkaku == 0x2562)		return 0xd3;
	else if(zenkaku == 0x2564)		return 0xd4;
	else if(zenkaku == 0x2566)		return 0xd5;
	else if(zenkaku == 0x2568)		return 0xd6;
	else if(zenkaku == 0x2569)		return 0xd7;
	else if(zenkaku == 0x256a)		return 0xd8;
	else if(zenkaku == 0x256b)		return 0xd9;
	else if(zenkaku == 0x256c)		return 0xda;
	else if(zenkaku == 0x256d)		return 0xdb;
	else if(zenkaku == 0x256f)		return 0xdc;
	else if(zenkaku == 0x2573)		return 0xdd;
	else if(zenkaku == 0x212b)		return 0xde;
	else if(zenkaku == 0x212c)		return 0xdf;
	else							return 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�o�C�g�����p�ɃG�X�P�[�v�V�[�P���X���o��							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CCodeConvManager::set1byte
	(	CArchive& arOut
	)
{
	//ASCII �R�[�h�Z�b�g
	arOut << (BYTE)CODE_ESC;
	arOut << (BYTE)'(';
	arOut << (BYTE)'B';
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�Q�o�C�g�����p�ɃG�X�P�[�v�V�[�P���X���o��							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CCodeConvManager::set2byte
	(	CArchive&	arOut
	)
{
	//JIS X 0208-1983(�VJIS����)
	arOut << (BYTE)CODE_ESC;
	arOut << (BYTE)'$';
	arOut << (BYTE)'B';
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�o�C�g�����p�ɃG�X�P�[�v�V�[�P���X���o��							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CCodeConvManager::set1byte
	(	CString*	pobjLine
	)
{
	if(pobjLine == NULL)
		return;

	//ASCII �R�[�h�Z�b�g
	(*pobjLine) += (UCHAR)CODE_ESC;
	(*pobjLine) += (UCHAR)'(';
	(*pobjLine) += (UCHAR)'B';
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�Q�o�C�g�����p�ɃG�X�P�[�v�V�[�P���X���o��							|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CCodeConvManager::set2byte
	(	CString*	pobjLine
	)
{
	if(pobjLine == NULL)
		return;

	//JIS X 0208-1983(�VJIS����)
	(*pobjLine) += (UCHAR)CODE_ESC;
	(*pobjLine) += (UCHAR)'$';
	(*pobjLine) += (UCHAR)'B';
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�Fscan forward if Shift-JIS code or JIS code exists						|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CCodeConvManager::checkforward
	(	unsigned char* p,
		unsigned int size
	)
{
	unsigned char c=0;

    if (size == 0)
        return INPUT_EUC;
    c = *p++;
    while (1) {
        if (c == CODE_ESC) {
            if (--size == 0)
                break;
            if ((c = *p++) == '$') {
                if (--size == 0)
                    break;
                if ((c = *p++) == 'B' || c == '@')
                    return INPUT_JIS;
                else
                    continue;
            }
            else if (c == 'K')
                return INPUT_JIS;
            else
                continue;
        }
        else if (c >= 0x81) {
            if (c == 0x8e) {
                if (--size == 0)
                    break;
                p++;
            }
            else if (c <= 0x9f) {
                if (--size == 0)
                    break;
                c = *p++;
                if (iskanji2nd(c))
                    return INPUT_SJIS;
                else
                    continue;
            }
            else if (c >= 0xa1 && c <= 0xdf) {
                if (--size == 0)
                    break;
                c = *p++;
                if (isHanKana(c))
                    continue;
                else if (iseuc(c))
                    return INPUT_EUC;
                else
                    continue;
            }
            else if (c != 0xa0)
                return INPUT_EUC;
        }
        if (--size == 0)
            break;
        c = *p++;
    }
    return INPUT_EUC;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�Fdetect kanji code														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
UINT CCodeConvManager::detectcode
	(	unsigned char* buf, 
		unsigned int size,
		BOOL* pbIsEnd
	)
{
    unsigned char *p=NULL, c=NULL;
    unsigned int count=NULL;
    int unknownstat=NULL;
	(*pbIsEnd) = FALSE;

    count = size;
    if (count == 0)
        return INPUT_ASCII;
    p = buf;
    unknownstat = 0;
    c = *p++;
    while (1) {
        if (c == CODE_ESC) {
            if (--count == 0)
                break;
            if ((c = *p++) == '$') {
                if (--count == 0)
                    break;
                if ((c = *p++) == 'B' || c == '@')
                    return INPUT_JIS;
                else
                    continue;
            }
            else if (c == 'K')
                return INPUT_JIS;
            else
                continue;
        }
        else if (c >= 0x81) {
            if (c == 0x8e) {
                if (--count == 0)
                    break;
                c = *p++;
                if (isHanKana(c))
                    unknownstat |= 2;
                else if (iskanji2nd(c))
                    return INPUT_SJIS;
                else
                    continue;
            }
            else if (c <= 0x9f) {
                if (--count == 0)
                    break;
                c = *p++;
                if (iskanji2nd(c))
                    return INPUT_SJIS;
                else
                    continue;
            }
            else if (c >= 0xa1 && c <= 0xdf || c == 0xfd || c == 0xfe) {
                if (--count == 0)
                    break;
                c = *p++;
                if (iseuc(c))
                    if (isHanKana(c))
                        return checkforward(p, count - 1);
                    else
                        return INPUT_EUC;
                else
                    continue;
            }
            else if (c >= 0xe0 && c <= 0xfc) {
                if (--count == 0)
                    break;
                c = *p++;
                if (iskanji2nd(c))
                    if (iseuc(c))
                        unknownstat |= 1;
                    else
                        return INPUT_SJIS;
                else
                    if (iseuc(c))
                        return INPUT_EUC;
                    else
                        continue;
            }
        }
        if (--count == 0)
            break;
        c = *p++;
    }
	if(count == 0)
		(*pbIsEnd) = TRUE;

    switch (unknownstat) {
    case 1:
    case 3:
        return INPUT_UNKNOWN;
    case 2:
        return INPUT_SJIS;
    default:
        return INPUT_ASCII;
    }
}


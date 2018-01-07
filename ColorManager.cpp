// ColorManager.cpp: CColorManager �N���X�̃C���v�������e�[�V����
//

#include "stdafx.h"
#include <Afxtempl.h>			//For CMap
#include <ctype.h>				//For isalpha()
#include "Editor.h"
#include "Editor_Def.h"
#include "ColorManager.h"
#include "Line.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CColorManager::CColorManager()
{
	m_pApp = (CEditorApp*)AfxGetApp();

	// �����o�ϐ�������
	m_FontColor     = RGB(255,255,255);				// �ʏ�̕����F
	m_bComment1Clr  = TRUE;							// �b�E�b�{�{�R�����g�L���t���O
	m_CommentColor  = RGB(0,255,0);					// �R�����g�F�i�b�^�b�{�{�p�j
	m_bComment2Clr  = FALSE;						// �u�a�R�����g�L���t���O
	m_CommentColor2 = RGB(0,255,0);					// �R�����g�F�i�u�a�p�j
	m_bComment3Clr  = FALSE;						// �A�Z���u���R�����g�L���t���O
	m_CommentColor3 = RGB(0,255,0);					// �R�����g�F�i�A�Z���u���p�j
	m_bComment4Clr  = FALSE;						// ���p�����R�����g�L���t���O
	m_CommentColor4 = RGB(0,255,0);					// �R�����g�F�i���p�����p�j
	m_bUrlClr       = TRUE;							// �t�q�k�L���t���O
	m_UrlColor      = RGB(255,0,255);				// �t�q�k�F
	m_bEMailClr     = TRUE;							// ���|���������L���t���O
	m_EMailColor    = RGB(255,0,255);				// ���|���������F
	m_KeyNo1Color   = RGB(0,255,255);				// �L�[���[�h�m���P�̕����F
	m_KeyNo2Color   = RGB(0,255,255);				// �L�[���[�h�m���Q�̕����F
	m_KeyNo3Color   = RGB(0,255,255);				// �L�[���[�h�m���R�̕����F
	m_KeyNo4Color   = RGB(0,255,255);				// �L�[���[�h�m���S�̕����F
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CColorManager::~CColorManager()
{
	//�}�b�v�f�[�^�N���A
	POSITION	pos;
	CString		objStr;
	COLORREF*	pColor;
	long lCnt = m_objMapStrToColor.GetCount();
	for( pos=m_objMapStrToColor.GetStartPosition(); pos!=0; )
	{
		m_objMapStrToColor.GetNextAssoc(pos,objStr,(void*&)pColor );
		if(pColor != NULL)
			delete pColor;
	}
	m_objMapStrToColor.RemoveAll();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h�J���[�����O												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::KeyWordColoring
	(	CLine*	pobjLine
	)
{
	char cBuf=0;
	long lOffset=0;
	long lCnt=0;
	long lDoubleQuateCount=0;		// " �̏o����
	BOOL bOK=FALSE;
	BOOL bIsWithinSlashAstar=FALSE;		// C            �̍s�R�����g
	BOOL bIsAppearLess=FALSE;			// '<' �o���׸�

	for(lCnt=0; lCnt<pobjLine->GetLength(); )//�����񒷕����[�v
	{
		if(IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt)))//�Q�o�C�g�����̑��o�C�g
		{
			if(bIsWithinSlashAstar) //  /* �` */�̃R�����g��
			{
				if(m_bComment1Clr) //�b�E�b�{�{�R�����g�L��
				{
					//�R�����g�F�Z�b�g
					pobjLine->SetColorAt(lCnt,lCnt+2,m_CommentColor);
				}
				else //�b�E�b�{�{�R�����g����
				{
					//�ʏ핶���F�Z�b�g
					pobjLine->SetColorAt(lCnt,lCnt+2,m_FontColor);
				}
			}
			else
			{
				//�ʏ핶���F�Z�b�g
				pobjLine->SetColorAt(lCnt,lCnt+2,m_FontColor);
			}
			lCnt+=2;
		}
		else //�P�o�C�g����
		{
			if(pobjLine->GetAt(lCnt) == '"') // " �̏o���񐔂��J�E���g�A�b�v
				lDoubleQuateCount++;

			//////////////////////////////////////
			//		�R�����g�`�F�b�N			//
			//////////////////////////////////////

			if( (pobjLine->GetAt(lCnt)=='/')     &&		// /* �`�@*/ �̃R�����g�J�n
				(pobjLine->GetAt(lCnt+1)=='*')   &&
				!IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt+1))
			)
			{
				bIsWithinSlashAstar = TRUE;

				if(m_bComment1Clr) //�b�E�b�{�{�R�����g�L��
				{
					//�R�����g�F�Z�b�g
					pobjLine->SetColorAt(lCnt,lCnt+2,m_CommentColor);
				}
				else //�b�E�b�{�{�R�����g����
				{
					//�ʏ핶���F�Z�b�g
					pobjLine->SetColorAt(lCnt,lCnt+2,m_FontColor);
				}
				lCnt+=2;
				continue;
			}
			else if( (pobjLine->GetAt(lCnt)=='*')     && // /* �`�@*/ �̃R�����g�I��
					 (pobjLine->GetAt(lCnt+1)=='/')   &&
					 !IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt+1))
			)
			{
				bIsWithinSlashAstar = FALSE;

				if(m_bComment1Clr) //�b�E�b�{�{�R�����g�L��
				{
					//�R�����g�F�Z�b�g
					pobjLine->SetColorAt(lCnt,lCnt+2,m_CommentColor);
				}
				else //�b�E�b�{�{�R�����g����
				{
					//�ʏ핶���F�Z�b�g
					pobjLine->SetColorAt(lCnt,lCnt+2,m_FontColor);
				}
				lCnt+=2;
				continue;
			}

			if( (pobjLine->GetAt(lCnt)=='/')    &&		//�_�u���X���b�V���̃R�����g
				(pobjLine->GetAt(lCnt+1)=='/')  &&
				!IsDBCSLeadByteEx(0,pobjLine->GetAt(lCnt+1))
			)
			{
				if( m_bComment1Clr != FALSE && 		//�b�E�b�{�{�R�����g�L��
					(lDoubleQuateCount % 2) == 0	//����""�̒��ł͂Ȃ�
				)
				{
					//�s���܂ŃR�����g�F�Z�b�g
					pobjLine->SetColorAt(lCnt,pobjLine->GetLength(),m_CommentColor);
					return;
				}
				else //�b�E�b�{�{�R�����g����
				{
					//�ʏ핶���F�Z�b�g
					pobjLine->SetColorAt(lCnt,lCnt+2,m_FontColor);
					lCnt+=2;
					continue;
				}
			}
			if( pobjLine->GetAt(lCnt)=='\'' )			//�V���O���N�I�[�g
			{
				if( m_bComment2Clr != FALSE &&		//�u�a�s�R�����g�L��
					(lDoubleQuateCount % 2) == 0	//����""�̒��ł͂Ȃ�
				)
				{
					//�s���܂ŃR�����g�F�Z�b�g
					pobjLine->SetColorAt(lCnt,pobjLine->GetLength(),m_CommentColor2);
					return;
				}
				else //�u�a�s�R�����g����
				{
					if(m_bComment1Clr!=FALSE && bIsWithinSlashAstar!=FALSE)
					{
						//�b�E�b�{�{�R�����g�F�Z�b�g
						pobjLine->SetColorAt(lCnt,m_CommentColor);
					}
					else
					{
						//�ʏ핶���F�Z�b�g
						pobjLine->SetColorAt(lCnt,m_FontColor);
					}
					lCnt++;
					continue;
				}
			}
			if( pobjLine->GetAt(lCnt)==';' )			//�Z�~�R����
			{
				if( m_bComment3Clr != FALSE &&			//�A�Z���u���R�����g�L��
					(lDoubleQuateCount % 2) == 0		//����""�̒��ł͂Ȃ�
				)
				{
					//�s���܂ŃR�����g�F�Z�b�g
					pobjLine->SetColorAt(lCnt,pobjLine->GetLength(),m_CommentColor3);
					return;
				}
				else //�A�Z���u���R�����g����
				{
					if(m_bComment1Clr!=FALSE && bIsWithinSlashAstar!=FALSE)
					{
						//�b�E�b�{�{�R�����g�F�Z�b�g
						pobjLine->SetColorAt(lCnt,m_CommentColor);
					}
					else
					{
						//�ʏ핶���F�Z�b�g
						pobjLine->SetColorAt(lCnt,m_FontColor);
					}
					lCnt++;
					continue;
				}
			}
			if( lCnt==0 )
			{
				if( pobjLine->GetAt(lCnt)=='>' )			//���p����
				{
					if(m_bComment4Clr) //���p�����R�����g�L��
					{
						//�s���܂ŃR�����g�F�Z�b�g
						pobjLine->SetColorAt(lCnt,pobjLine->GetLength(),m_CommentColor4);
						return;
					}
					else //���p�����R�����g����
					{
						if(m_bComment1Clr!=FALSE && bIsWithinSlashAstar!=FALSE)
						{
							//�b�E�b�{�{�R�����g�F�Z�b�g
							pobjLine->SetColorAt(lCnt,m_CommentColor);
						}
						else
						{
							//�ʏ핶���F�Z�b�g
							pobjLine->SetColorAt(lCnt,m_FontColor);
						}
						lCnt++;
						continue;
					}
				}
			}

			if(m_bComment1Clr) //�b�E�b�{�{�R�����g�L��
			{
				if(bIsWithinSlashAstar) //�@/* �` */ �̃R�����g��
				{
					//�R�����g�F�Z�b�g
					pobjLine->SetColorAt(lCnt,m_CommentColor);
					lCnt++;
					continue;
				}
			}

			//////////////////////
			//	�t�q�k�`�F�b�N	//
			//////////////////////

			if(m_bUrlClr) //�t�q�k�L��
			{
				bOK = FALSE;
				long lBackCount=0;
				if(pobjLine->GetAt(lCnt) == ':')
				{
					if((lCnt-strlen("http"))>=0)
					{
						if( !pobjLine->Compare(lCnt-strlen("http"),(const unsigned char*)"http",strlen("http")) &&
							!pobjLine->Compare(lCnt+1,(const unsigned char*)"//",strlen("//"))
						)
						{
							cBuf = pobjLine->GetAt(lCnt+1+strlen("//"));
							if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf))
							{
								bOK = TRUE;
								lBackCount = strlen("http");
							}
						}
					}
					if((lCnt-strlen("https"))>=0)
					{
						if( !pobjLine->Compare(lCnt-strlen("https"),(const unsigned char*)"https",strlen("https")) &&
							!pobjLine->Compare(lCnt+1,(const unsigned char*)"//",strlen("//"))
						)
						{
							cBuf = pobjLine->GetAt(lCnt+1+strlen("//"));
							if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf))
							{
								bOK = TRUE;
								lBackCount = strlen("https");
							}
						}
					}
					if((lCnt-strlen("ftp"))>=0)
					{
						if( !pobjLine->Compare(lCnt-strlen("ftp"),(const unsigned char*)"ftp",strlen("ftp")) &&
							!pobjLine->Compare(lCnt+1,(const unsigned char*)"//",strlen("//"))
						)
						{
							cBuf = pobjLine->GetAt(lCnt+1+strlen("//"));
							if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf))
							{
								bOK = TRUE;
								lBackCount = strlen("ftp");
							}
						}
					}
					if((lCnt-strlen("gopher"))>=0)
					{
						if( !pobjLine->Compare(lCnt-strlen("gopher"),(const unsigned char*)"gopher",strlen("gopher")) &&
							!pobjLine->Compare(lCnt+1,(const unsigned char*)"//",strlen("//"))
						)
						{
							cBuf = pobjLine->GetAt(lCnt+1+strlen("//"));
							if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf))
							{
								bOK = TRUE;
								lBackCount = strlen("gopher");
							}
						}
					}
				}

				if(bOK)
				{
					//�O����T��
					for( ; lBackCount>0; lBackCount-- )
					{
						pobjLine->SetColorAt(lCnt-lBackCount,m_UrlColor);
						pobjLine->SetUnderLineAt(lCnt-lBackCount,TRUE);
					}
					pobjLine->SetColorAt(lCnt,m_UrlColor);
					pobjLine->SetUnderLineAt(lCnt,TRUE);
					//�����T��
					for(lCnt++; ; )
					{
						if(lCnt >= pobjLine->GetLength())
							break;
						cBuf = pobjLine->GetAt(lCnt);
						if( !isValidURL(cBuf) )
							break;
						pobjLine->SetColorAt(lCnt,m_UrlColor);
						pobjLine->SetUnderLineAt(lCnt,TRUE);
						lCnt++;
					}
					continue;
				}
			}

			//////////////////////////////
			//	���|���������`�F�b�N	//
			//////////////////////////////

			if(m_bEMailClr) //���|���������L��
			{
				bOK = FALSE;
				if(pobjLine->GetAt(lCnt) == '@')
				{
					lOffset = pobjLine->GetPreOffset(lCnt);
					if(lOffset != -1)
					{
						cBuf = pobjLine->GetAt(lOffset);
						if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf)) //�O���`�F�b�N
						{
							cBuf = pobjLine->GetAt(lCnt+1);
							if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf)) //����`�F�b�N
								bOK = TRUE;
						}
					}
				}

				if(bOK)
				{
					//�O����T��
					for(lOffset=lCnt;;)
					{
						lOffset = pobjLine->GetPreOffset(lOffset);
						if(lOffset < 0)
							break;
						cBuf = pobjLine->GetAt(lOffset);
						if( !isValidEMail(cBuf) )
							break;
						pobjLine->SetColorAt(lOffset,m_EMailColor);
						pobjLine->SetUnderLineAt(lOffset,TRUE);
					}
					pobjLine->SetColorAt(lCnt,m_EMailColor);
					pobjLine->SetUnderLineAt(lCnt,TRUE);
					//�����T��
					for(lCnt++;;)
					{
						if(lCnt >= pobjLine->GetLength())
							break;
						cBuf = pobjLine->GetAt(lCnt);
						if( !isValidEMail(cBuf) )
							break;
						pobjLine->SetColorAt(lCnt,m_EMailColor);
						pobjLine->SetUnderLineAt(lCnt,TRUE);
						lCnt++;
					}
					continue;
				}
			}

			//////////////////////////////
			//	�L�[���[�h�`�F�b�N		//
			//////////////////////////////

			//�L�[���[�h���ȊO
			if( (!isAlnum(pobjLine->GetAt(lCnt)))	&&
				(pobjLine->GetAt(lCnt) != '_')		&& 
				(pobjLine->GetAt(lCnt) != '#')		&&
				(pobjLine->GetAt(lCnt) != '<')		&&
				(pobjLine->GetAt(lCnt) != '>')		&&
				(pobjLine->GetAt(lCnt) != '/')
			)
			{
				//�ʏ핶���F�Z�b�g
				pobjLine->SetColorAt(lCnt,m_FontColor);
				lCnt++;
				continue;
			}

			//�`�F�b�N�ΏےP�ꂫ��o��
			CString		objCheckStr = _T("");
			long		lCharCnt=0;
			for(lCharCnt=0 ; ; lCharCnt++)
			{
				cBuf = pobjLine->GetAt(lCnt+lCharCnt);
				if( cBuf==0 )
					break;
				if(IsDBCSLeadByteEx(0,cBuf)) //�Q�o�C�g�����ɂȂ����烋�[�v�I��
					break;
				if( (!isAlnum(cBuf))	&&
					(cBuf != '_')		&&
					(cBuf != '#')		&&
					(cBuf != '<')		&&
					(cBuf != '>')       &&
					(cBuf != '/')
				) 
				{
					break;
				}
				if( (cBuf == '/') && (pobjLine->GetAt(lCnt+1+lCharCnt) == '*') )
					break;
				if( (cBuf == '/') && (pobjLine->GetAt(lCnt+1+lCharCnt) == '/') )
					break;
				if( cBuf == 0x27 ) //�V���O���N�I�[�g
					break;
				if( cBuf == ';' )
					break;

				if( cBuf == '<' )
				{
					if( objCheckStr.IsEmpty() )
					{
						bIsAppearLess = TRUE;
					}
					else
					{
						break;
					}
				}
				if( (cBuf == '>') && (bIsAppearLess != FALSE) )
				{
					bIsAppearLess = FALSE;	// '<'�o���׸�
					objCheckStr += cBuf;
					break;
				}

				objCheckStr += cBuf;
			}

			//�o�^�ς݃L�[���[�h�ƈ�v���邩����
			COLORREF	color_buf=0;
			COLORREF*	pColor=NULL;
			BOOL bFind = m_objMapStrToColor.Lookup( objCheckStr, (void*&)pColor );
			if( bFind != 0 ) //�����I�I
				color_buf = (*pColor);
			else //�Ȃ�
				color_buf = m_FontColor;			//�F��ʏ�̕����F�ɐݒ�

			//�F�Z�b�g
			pobjLine->SetColorAt(lCnt,lCnt+objCheckStr.GetLength(),color_buf);
			lCnt += objCheckStr.GetLength();
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ʏ�̕����F�ݒ菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetFontColor
	(	COLORREF	color
	)
{
	m_FontColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�b�E�b�{�{�R�����g�L���t���O�擾����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CColorManager::GetFlagComment1Clr()
{
	return m_bComment1Clr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�b�E�b�{�{�R�����g�L���t���O�ݒ菈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetFlagComment1Clr
	(	BOOL	bComment1Clr
	)
{
	m_bComment1Clr = bComment1Clr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�F�擾�����i�b�^�b�{�{�p�j									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetCommentColor()
{
	return m_CommentColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�F�ݒ菈���i�b�^�b�{�{�p�j									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetCommentColor
	(	COLORREF	color
	)
{
	m_CommentColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�a�R�����g�L���t���O�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CColorManager::GetFlagComment2Clr()
{
	return m_bComment2Clr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�u�a�R�����g�L���t���O�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetFlagComment2Clr
	(	BOOL	bComment2Clr
	)
{
	m_bComment2Clr = bComment2Clr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�F�擾�����i�u�a�p�j											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetCommentColor2()
{
	return m_CommentColor2;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�F�ݒ菈���i�u�a�p�j											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetCommentColor2
	(	COLORREF	color
	)
{
	m_CommentColor2 = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A�Z���u���R�����g�L���t���O�擾����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CColorManager::GetFlagComment3Clr()
{
	return m_bComment3Clr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A�Z���u���R�����g�L���t���O�ݒ菈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetFlagComment3Clr
	(	BOOL	bComment3Clr
	)
{
	m_bComment3Clr = bComment3Clr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�F�擾�����i�A�Z���u���p�j									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetCommentColor3()
{
	return m_CommentColor3;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�F�ݒ菈���i�A�Z���u���p�j									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetCommentColor3
	(	COLORREF	color
	)
{
	m_CommentColor3 = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���p�����R�����g�L���t���O�擾����									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CColorManager::GetFlagComment4Clr()
{
	return m_bComment4Clr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���p�����R�����g�L���t���O�ݒ菈��									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetFlagComment4Clr
	(	BOOL	bComment4Clr
	)
{
	m_bComment4Clr = bComment4Clr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�F�擾�����i���p�����p�j										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetCommentColor4()
{
	return m_CommentColor4;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R�����g�F�ݒ菈���i���p�����p�j										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetCommentColor4
	(	COLORREF	color
	)
{
	m_CommentColor4 = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�q�k�L���t���O�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CColorManager::GetFlagUrlColor()
{
	return m_bUrlClr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�q�k�L���t���O�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetFlagUrlColor
	(	BOOL	bUrlClr
	)
{
	m_bUrlClr = bUrlClr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�q�k�F�擾����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetUrlColor()
{
	return m_UrlColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�q�k�F�ݒ菈��														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetUrlColor
	(	COLORREF	color
	)
{
	m_UrlColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���|���������L���t���O�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CColorManager::GetFlagEMailColor()
{
	return m_bEMailClr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���|���������L���t���O�ݒ菈��										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetFlagEMailColor
	(	BOOL	bEMailClr
	)
{
	m_bEMailClr = bEMailClr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���|���������F�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetEMailColor()
{
	return m_EMailColor;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���|���������F�ݒ菈��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetEMailColor
	(	COLORREF	color
	)
{
	m_EMailColor = color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�S�L�[���[�h�̍폜����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::DeleteAllKeyWord()
{
	//�}�b�v�f�[�^�N���A
	POSITION	pos;
	CString		objStr;
	COLORREF*	pColor;
	long lCnt = m_objMapStrToColor.GetCount();
	for( pos=m_objMapStrToColor.GetStartPosition(); pos!=0; )
	{
		m_objMapStrToColor.GetNextAssoc(pos,objStr,(void*&)pColor );
		if(pColor != NULL)
			delete pColor;
	}
	m_objMapStrToColor.RemoveAll();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h�m���P�̕����F�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetKeyNo1Color()
{
	return m_KeyNo1Color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h��`�t�@�C���i�m�n�P�j�ǂݍ��݁��F�ݒ菈��					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetKeyNo1Color
	(	COLORREF	color,				//�F
		const char*	szKeyNo1Fname
	)
{
	long lCRCount=0;
	long lLFCount=0;
	CString	objStrBuf="";
	CFile	objFile;
	CString strPath;
	BOOL bFind=FALSE;

	//�����o�ϐ��֐F��ێ�
	m_KeyNo1Color = color;

	//�L�[���[�h�t�@�C���I�[�v��
	if(m_pApp->IsDriveRoot() == FALSE)
		objStrBuf = "\\";
	strPath.Format("%s%s%s",m_pApp->m_strExePath,objStrBuf,szKeyNo1Fname);
	if( !objFile.Open( strPath, CFile::modeRead, NULL ) )
		return;
	//�A�[�J�C�u�쐬
	CArchive ar( &objFile, CArchive::load );

	//�P�s�Âǂݍ���
	m_pApp->PrepareMyReadString();
	for(;;)
	{
		objStrBuf.Empty();
		if( m_pApp->MyReadString(ar,objStrBuf,&lCRCount,&lLFCount) == 0 )
		{
			if( objStrBuf.IsEmpty() )
				break;
		}

		if( objStrBuf.Left(2) == _T("//") )// "//"�Ŏn�܂�s�𖳎�
			continue;
		if( objStrBuf.IsEmpty() )
			continue;

		//�}�b�v�֓o�^
		COLORREF*	pColor = NULL;
		BOOL bFind = m_objMapStrToColor.Lookup( objStrBuf, (void*&)pColor );
		if( bFind != 0 ) //�����I�I
		{
			(*pColor) = color;					//�F�ύX
		}
		else //�Ȃ�
		{
			COLORREF*	pColor = new COLORREF;	//�F�V�K�o�^
			(*pColor) = color;
			m_objMapStrToColor.SetAt(objStrBuf,pColor);
		}
	}

	//�A�[�J�C�u���t�@�C���N���[�Y
	ar.Close();
	objFile.Close();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h�m���Q�̕����F�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetKeyNo2Color()
{
	return m_KeyNo2Color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h��`�t�@�C���i�m�n�Q�j�ǂݍ��݁��F�ݒ菈��					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetKeyNo2Color
	(	COLORREF	color,				//�F
		const char*	szKeyNo2Fname
	)
{
	long lCRCount=0;
	long lLFCount=0;
	CString	objStrBuf="";
	CFile	objFile;
	CString strPath;

	//�����o�ϐ��֐F��ێ�
	m_KeyNo2Color = color;

	//�L�[���[�h�t�@�C���I�[�v��
	if(m_pApp->IsDriveRoot() == FALSE)
		objStrBuf = "\\";
	strPath.Format("%s%s%s",m_pApp->m_strExePath,objStrBuf,szKeyNo2Fname);
	if( !objFile.Open( strPath, CFile::modeRead, NULL ) )
		return;
	//�A�[�J�C�u�쐬
	CArchive ar( &objFile, CArchive::load );

	//�P�s�Âǂݍ���
	m_pApp->PrepareMyReadString();
	for(;;)
	{
		if( m_pApp->MyReadString(ar,objStrBuf,&lCRCount,&lLFCount) == 0 )
		{
			if( objStrBuf.IsEmpty() )
				break;
		}

		if( objStrBuf.Left(2) == _T("//") ) // "//"�Ŏn�܂�s�𖳎�
			continue;
		if( objStrBuf.IsEmpty() )
			continue;

		//�}�b�v�֓o�^
		COLORREF*	pColor = NULL;
		BOOL bFind = m_objMapStrToColor.Lookup( objStrBuf, (void*&)pColor );
		if( bFind != 0 ) //�����I�I
		{
			(*pColor) = color;					//�F�ύX
		}
		else //�Ȃ�
		{
			COLORREF*	pColor = new COLORREF;	//�F�V�K�o�^
			(*pColor) = color;
			m_objMapStrToColor.SetAt(objStrBuf,pColor);
		}
	}

	//�A�[�J�C�u���t�@�C���N���[�Y
	ar.Close();
	objFile.Close();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h�m���R�̕����F�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetKeyNo3Color()
{
	return m_KeyNo3Color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h��`�t�@�C���i�m�n�R�j�ǂݍ��݁��F�ݒ菈��					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetKeyNo3Color
	(	COLORREF	color,				//�F
		const char*	szKeyNo3Fname
	)
{
	long lCRCount=0;
	long lLFCount=0;
	CString	objStrBuf="";
	CFile	objFile;
	CString strPath;

	//�����o�ϐ��֐F��ێ�
	m_KeyNo3Color = color;

	//�L�[���[�h�t�@�C���I�[�v��
	if(m_pApp->IsDriveRoot() == FALSE)
		objStrBuf = "\\";
	strPath.Format("%s%s%s",m_pApp->m_strExePath,objStrBuf,szKeyNo3Fname);
	if( !objFile.Open( strPath, CFile::modeRead, NULL ) )
		return;
	//�A�[�J�C�u�쐬
	CArchive ar( &objFile, CArchive::load );

	//�P�s�Âǂݍ���
	m_pApp->PrepareMyReadString();
	for(;;)
	{
		if( m_pApp->MyReadString(ar,objStrBuf,&lCRCount,&lLFCount) == 0 )
		{
			if( objStrBuf.IsEmpty() )
				break;
		}

		if( objStrBuf.Left(2) == _T("//") ) // "//"�Ŏn�܂�s�𖳎�
			continue;
		if( objStrBuf.IsEmpty() )
			continue;

		//�}�b�v�֓o�^
		COLORREF*	pColor = NULL;
		BOOL bFind = m_objMapStrToColor.Lookup( objStrBuf, (void*&)pColor );
		if( bFind != 0 ) //�����I�I
		{
			(*pColor) = color;					//�F�ύX
		}
		else //�Ȃ�
		{
			COLORREF*	pColor = new COLORREF;	//�F�V�K�o�^
			(*pColor) = color;
			m_objMapStrToColor.SetAt(objStrBuf,pColor);
		}
	}

	//�A�[�J�C�u���t�@�C���N���[�Y
	ar.Close();
	objFile.Close();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h�m���S�̕����F�擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CColorManager::GetKeyNo4Color()
{
	return m_KeyNo4Color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�[���[�h��`�t�@�C���i�m�n�S�j�ǂݍ��݁��F�ݒ菈��					|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CColorManager::SetKeyNo4Color
	(	COLORREF	color,				//�F
		const char*	szKeyNo4Fname
	)
{
	long lCRCount=0;
	long lLFCount=0;
	CString	objStrBuf="";
	CFile	objFile;
	CString strPath;

	//�����o�ϐ��֐F��ێ�
	m_KeyNo4Color = color;

	//�L�[���[�h�t�@�C���I�[�v��
	if(m_pApp->IsDriveRoot() == FALSE)
		objStrBuf = "\\";
	strPath.Format("%s%s%s",m_pApp->m_strExePath,objStrBuf,szKeyNo4Fname);
	if( !objFile.Open( strPath, CFile::modeRead, NULL ) )
		return;
	//�A�[�J�C�u�쐬
	CArchive ar( &objFile, CArchive::load );

	//�P�s�Âǂݍ���
	m_pApp->PrepareMyReadString();
	for(;;)
	{
		if( m_pApp->MyReadString(ar,objStrBuf,&lCRCount,&lLFCount) == 0 )
		{
			if( objStrBuf.IsEmpty() )
				break;
		}

		if( objStrBuf.Left(2) == _T("//") ) // "//"�Ŏn�܂�s�𖳎�
			continue;
		if( objStrBuf.IsEmpty() )
			continue;

		//�}�b�v�֓o�^
		COLORREF*	pColor = NULL;
		BOOL bFind = m_objMapStrToColor.Lookup( objStrBuf, (void*&)pColor );
		if( bFind != 0 ) //�����I�I
		{
			(*pColor) = color;					//�F�ύX
		}
		else //�Ȃ�
		{
			COLORREF*	pColor = new COLORREF;	//�F�V�K�o�^
			(*pColor) = color;
			m_objMapStrToColor.SetAt(objStrBuf,pColor);
		}
	}

	//�A�[�J�C�u���t�@�C���N���[�Y
	ar.Close();
	objFile.Close();
}


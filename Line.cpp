// Line.cpp: CLine �N���X�̃C���v�������e�[�V����
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "Editor_Def.h"
#include "Editor.h"
#include "Line.h"
#include "CodeConvManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLine::CLine()
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_strLine = "";
	m_strBuf = "";
	m_bMark = 0;
	m_nDiffLineKind = DIFF_LINEKIND_COMMON;
	m_nDirCmpLineKind = DIRCMP_LINEKIND_NORMAL;
	m_objDirCmpFileName = _T("");
	m_lSavedLineLength = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�R���X�g���N�^														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLine::CLine(const unsigned char* pszLine)
{
	m_pApp = (CEditorApp*)AfxGetApp();
	if(pszLine != NULL)
		m_strLine = pszLine;
	else
		m_strLine = "";
	m_strBuf = "";
	m_bMark = 0;
	m_nDiffLineKind = DIFF_LINEKIND_COMMON;
	m_nDirCmpLineKind = DIRCMP_LINEKIND_NORMAL;
	m_objDirCmpFileName = _T("");
	m_lSavedLineLength = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�X�g���N�^															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLine::~CLine()
{
}

/////////////////////////////////////////////////////////////////////////////
//	CLine �p�u���b�N�֐�

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F[ ] ���Z�q															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned char CLine::operator[](long lOffset) const
{
	if(lOffset < m_strLine.GetLength())
		return m_strLine.GetAt(lOffset);
	else
		return (unsigned char)0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�����擾															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
unsigned char CLine::GetAt(long lOffset) const
{
	if(lOffset < m_strLine.GetLength())
		return m_strLine.GetAt(lOffset);
	else
		return (unsigned char)0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�����ݒ�															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SetAt
	(	long	lOffset,
		unsigned char	c
	)
{
	if(lOffset < 0 || lOffset >= m_strLine.GetLength())
		return;

	if( c == 0 )
		m_strLine = m_strLine.Left(lOffset);
	else
		m_strLine.SetAt(lOffset,c);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����񒷎擾															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLine::GetLength()
{
	return m_strLine.GetLength();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�X�y�[�X�p�f�B���O													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::PadSpace
	(	long lOffset		//�ǂ��܂ł�
	)
{
	if(lOffset < 0)
		return;
	while(GetLength() <= lOffset)
		m_strLine += ' ';
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�s�ݒ�																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::Set
	(	const unsigned char*	pszLine
	)
{
	if(pszLine==NULL)
		return;
	m_strLine = pszLine;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A��																	|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::Catenate
	(	const unsigned char*	pszSubStr
	)
{
	if(pszSubStr==NULL)
		return;
	CString buf = pszSubStr;
	m_strLine += buf;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�P�����A��															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::Catenate
	(	unsigned char	c
	)
{
	m_strLine += c;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F������}��															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::Insert
	(	long	lOffset,					//�}���ʒu
		const unsigned char*	pszInsStr	//�}��������
	)
{
	if(lOffset < 0)
		return;
	if(pszInsStr==NULL)
		return;
	m_strLine.Insert(lOffset,(LPCTSTR)pszInsStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����}��																|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::InsertChar
	(	long			lOffset,		//�}���ʒu
		unsigned char	c1stChar,		//�}���������o�C�g
		unsigned char	c2ndChar		//�}���������o�C�g�i�P�o�C�g�����̂Ƃ���NULL�j
	)
{
	unsigned char buf[]={c1stChar,c2ndChar,0};
	if(lOffset < 0)
		return;
	m_strLine.Insert(lOffset,(LPCTSTR)buf);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����w����}��														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::InsertChars
	(	long			lOffset,		//�}���ʒu
		unsigned char	cChar,			//�}������
		long			lCnt			//�}����
	)
{
	CString	buf="";
	
	for(long i=0; i<lCnt; i++)
		buf += cChar;
	m_strLine.Insert(lOffset,buf);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����㏑��															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::OverWriteChar
	(	long			lOffset,		//�㏑���ʒu
		unsigned char	c1stChar,		//�㏑���������o�C�g
		unsigned char	c2ndChar		//�㏑���������o�C�g�i�P�o�C�g�����̂Ƃ���NULL�j
	)
{
	if(lOffset < 0)
		return;
	
	if((c1stChar != 0) && (c2ndChar == 0))//�P�޲Ă̏㏑��
	{
		if(lOffset >= m_strLine.GetLength())
			PadSpace(lOffset);

		if(IsDBCSLeadByteEx(0,m_strLine.GetAt(lOffset)))//�㏑���Ώۂ��Q�޲ĕ����̑���޲�
		{
			m_strLine.SetAt(lOffset,c1stChar);
			m_strLine.SetAt(lOffset+1,' '); //�Q�o�C�g�ڂ��X�y�[�X�Œu������
		}
		else //�㏑���Ώۂ��P�޲ĕ���
		{
			m_strLine.SetAt(lOffset,c1stChar);
		}
	}
	else if((c1stChar != 0) && (c2ndChar != 0))//�Q�޲Ă̏㏑��
	{
		if((lOffset+1) >= m_strLine.GetLength())
			PadSpace(lOffset+1);

		if(IsDBCSLeadByteEx(0,m_strLine.GetAt(lOffset)))//�㏑���Ώۂ��Q�޲ĕ����̑���޲�
		{
			//���̂܂܏㏑��
			m_strLine.SetAt(lOffset,c1stChar);
			m_strLine.SetAt(lOffset+1,c2ndChar);
		}
		else //�㏑���Ώۂ��P�o�C�g����
		{
			m_strLine.SetAt(lOffset,c1stChar);
			if(IsDBCSLeadByteEx(0,m_strLine.GetAt(lOffset+1)))//�㏑���Ώۂ�2�����ڂ��Q�޲ĕ����̑���޲�
			{
				m_strLine.SetAt(lOffset+1,c2ndChar);
				m_strLine.SetAt(lOffset+2,' '); //�Q�����ڂ��X�y�[�X�ŏ㏑��
			}
			else 
			{
				m_strLine.SetAt(lOffset+1,c2ndChar);
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����폜(DELETE)														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::DeleteChar
	(	long		lOffset,			//�����폜�ʒu(IN)
		unsigned char* pc1stChar,		//�폜���ꂽ�����̑��o�C�g(OUT)
		unsigned char* pc2ndChar		//�폜���ꂽ�����̑��o�C�g(OUT)
	)
{
	if(lOffset < 0 || lOffset >= m_strLine.GetLength())
	{
		(*pc1stChar) = 0;
		(*pc2ndChar) = 0;
		return;
	}

	if(IsDBCSLeadByteEx(0,m_strLine.GetAt(lOffset)))
	{
		(*pc1stChar) = m_strLine.GetAt(lOffset);
		(*pc2ndChar) = m_strLine.GetAt(lOffset+1);
		m_strLine.Delete(lOffset,2);
	}
	else
	{
		(*pc1stChar) = m_strLine.GetAt(lOffset);
		(*pc2ndChar) = 0;
		m_strLine.Delete(lOffset,1);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����폜(BACK SPACE)													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::BackSpaceChar
	(	long		lOffset,			//�����폜�ʒu(IN)
		unsigned char* pc1stChar,		//�폜���ꂽ�����̑��o�C�g(OUT)
		unsigned char* pc2ndChar		//�폜���ꂽ�����̑��o�C�g(OUT)
	)
{
	if(lOffset < 0 || lOffset > m_strLine.GetLength())
	{
		(*pc1stChar) = 0;
		(*pc2ndChar) = 0;
		return;
	}

	//�Q�o�C�g�������l��
	for(long lCnt=0; ; )
	{
		if(IsDBCSLeadByteEx(0,m_strLine.GetAt(lCnt)))//�Q�o�C�g�����̑��o�C�g
		{
			if(lCnt < (lOffset-2))
				lCnt+=2;
			else
			{
				//�폜�����ʒm
				(*pc1stChar) = m_strLine.GetAt(lCnt);
				(*pc2ndChar) = m_strLine.GetAt(lCnt+1);
				m_strLine.Delete(lCnt,2);
				break;
			}
		}
		else //�P�o�C�g����
		{
			if(lCnt < (lOffset-1))
				lCnt+=1;
			else
			{
				//�폜�����ʒm
				(*pc1stChar) = m_strLine.GetAt(lCnt);
				(*pc2ndChar) = 0;
				m_strLine.Delete(lCnt,1);
				break;
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F������؂��菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::Trim
	(	long	lStartOffset,	//�؂���ΏۊJ�n�I�t�Z�b�g�i�[���I���W���j
		long	lEndOffset		//�؂���ΏۏI���I�t�Z�b�g�i�[���I���W���j
	)
{
	if(lStartOffset < 0 || lEndOffset < 0)
		return;
	if( (lStartOffset > m_strLine.GetLength()) || (lEndOffset > m_strLine.GetLength()) )
		return;

	m_strLine.Delete(lStartOffset,(lEndOffset-lStartOffset));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�^�u���擾����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLine::GetTabCount()
{
	int pos=0;
	int nTabCnt=0;
	for(;;)
	{
		pos = m_strLine.Find('\t',pos);
		if(pos == -1)
			break;
		nTabCnt+=1;
		pos+=1;
	}
	return nTabCnt;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�F�擾����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CLine::GetColorAt
	(	long	lOffset
	)
{
	if(lOffset < 0 || lOffset >= m_Color.GetSize())
		return 0;

	return (COLORREF)m_Color[lOffset];
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�F�ݒ菈��															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SetColorAt
	(	long		lOffset,
		COLORREF	color
	)
{
	if(lOffset < 0 || lOffset >= m_Color.GetSize())
		return;

	m_Color[lOffset] = (CObject*)color;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�F�ݒ菈��															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SetColorAt
	(	long		lSOffset,
		long		lEOffset,
		COLORREF	color
	)
{
	if(lSOffset < 0 || lSOffset >= m_Color.GetSize())
		return;
	if(lEOffset < 0 || lEOffset > m_Color.GetSize())
		return;

	for(long l=lSOffset; l<lEOffset; l++)
		m_Color.SetAtGrow(l,(CObject*)color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�������擾����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CLine::GetUnderLineAt
	(	long	lOffset
	)
{
	if(lOffset < 0 || lOffset >= m_bNeedUnderLine.GetSize())
		return FALSE;

	return (BOOL)m_bNeedUnderLine[lOffset];
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�������ݒ菈��														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SetUnderLineAt
	(	long		lOffset,
		BOOL		bNeedUnderLine
	)
{
	if(lOffset < 0 || lOffset >= m_bNeedUnderLine.GetSize())
		return;

	m_bNeedUnderLine.SetAtGrow(lOffset,(CObject*)bNeedUnderLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�������s����															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLine::DoFind							//�������F�J�n�I�t�Z�b�g�A���������F�|�P
	(	long		lStartOffset,			//�����J�n�I�t�Z�b�g
		long		lEndOffset,				//�����I���I�t�Z�b�g
		CString		objFindStr,				//�����Ώە�����
		BOOL		bAllAlphaNum,			//�����Ώە����񂪂��ׂĉp�������H
		BOOL		bIsMatchCase,			//�啶���A����������ʂ��邩�H
		BOOL		bIsMatchWholeWord,		//�P��S�̂���v������́H
		BOOL		bIsUseReglarExpression,	//���K�\�����g�����H
		long*		plMatchLength
	)
{
	CString		objMsg, objBuf;
	int			nPos=0, cflags=0, eflags=0, start=0, end=0, ret=0;
	char 		szMsg[256];

	if(lStartOffset < 0 || lStartOffset >= m_strLine.GetLength())
		return -1;
	if(lEndOffset < 0 || lEndOffset > m_strLine.GetLength())
		return -1;

	//�����Ώێ��o��
	CString strTarget = m_strLine.Mid(lStartOffset,(lEndOffset-lStartOffset+1));
	CString strFind   = objFindStr;

	if(bIsUseReglarExpression) //���K�\�����g���ꍇ
	{
		if( m_pApp->m_lpfnRegFunc1==NULL || m_pApp->m_lpfnRegFunc2==NULL ||
			m_pApp->m_lpfnRegFunc3==NULL || m_pApp->m_lpfnRegFunc4==NULL ||
			m_pApp->m_lpfnRegFunc5==NULL )
			return -1;

		cflags = REG_EXTENDED;
		if(bIsMatchCase==FALSE)//�召��������ʂ��Ȃ�
			cflags |= REG_ICASE;
		//���K�\���̃R���p�C�����K�v�Ȃ�΃R���p�C������myIsNeedRegcomp
		if(m_pApp->m_lpfnRegFunc5((const char*)(LPCTSTR)strFind,cflags))
		{
			//regcomp
			ret = m_pApp->m_lpfnRegFunc1((const char*)(LPCTSTR)strFind,cflags);
			if(ret != 0) //error
			{
				memset(szMsg,0,sizeof(szMsg));
				//regerror
				m_pApp->m_lpfnRegFunc3(ret,szMsg,sizeof(szMsg));
				objMsg.LoadString(IDS_REGEX_COMPERR);
				objBuf.Format("%s\r\n%s",objMsg,szMsg);
				m_pApp->MyMessageBox(objBuf,(MB_OK|MB_ICONSTOP),0);
				//regfree
				m_pApp->m_lpfnRegFunc4();
				return -1;
			}
		}

		//����myRegexec
		if(m_pApp->m_lpfnRegFunc2((const char*)(LPCTSTR)strTarget,eflags,&start,&end)==0)//match!
		{
			(*plMatchLength) = end - start;
			return (lStartOffset + start);
		}
		else
			return -1;
	}
	else //���K�\�����g��Ȃ��ꍇ
	{
		if(bIsMatchCase==FALSE) //�啶���A��������ʂȂ�
		{
			strTarget.MakeUpper();
			strFind.MakeUpper();
		}

		nPos = strTarget.Find(strFind,0);
		if(nPos == -1) //������Ȃ�
			return -1;
		if(!bIsMatchWholeWord || !bAllAlphaNum) //�P��P�ʌ����ł͂Ȃ�
		{
			(*plMatchLength) = strFind.GetLength();
			return( lStartOffset + nPos );
		}
		else //�����Ώە����񂪂��ׂĉp�����ŒP��P�ʌ���
		{
			for(;;)
			{
				unsigned char c1 = GetAt(lStartOffset+nPos-1);
				unsigned char c2 = GetAt(lStartOffset+nPos+objFindStr.GetLength());
				if( !isAlnum(c1) && c1!='_' && !isAlnum(c2) && c2!='_' ) //�O�オ�p�����ł͂Ȃ�
				{
					(*plMatchLength) = objFindStr.GetLength();
					return( lStartOffset + nPos );
				}
				else //�O�オ�p����
				{
					//��������
					nPos = strTarget.Find(strFind,nPos+1);
					if(nPos == -1)
						break;
				}
			}
			return -1;
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����i�t�����j���s����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLine::DoReverseFind					//�������F�J�n�I�t�Z�b�g�A���������F�|�P
	(	long		lStartOffset,			//�����J�n�I�t�Z�b�g
		long		lEndOffset,				//�����I���I�t�Z�b�g
		CString&	objFindStr,				//�����Ώە�����
		BOOL		bAllAlphaNum,			//�����Ώە����񂪂��ׂĉp�������H
		BOOL		bIsMatchCase,			//�啶���A����������ʂ��邩�H
		BOOL		bIsMatchWholeWord,		//�P��S�̂���v������́H
		BOOL		bIsUseReglarExpression,	//���K�\�����g�����H
		long*		plMatchLength
	)
{
	long	lStartIndex = lStartOffset;
	long	lPreFoundIndex = -1;
	long	lFoundIndex = -1;

	if(lStartOffset < 0 || lStartOffset >= m_strLine.GetLength())
		return -1;
	if(lEndOffset < 0 || lEndOffset > m_strLine.GetLength())
		return -1;

	for(;;)
	{
		lFoundIndex = DoFind( lStartIndex,
							  lEndOffset,
							  objFindStr,
							  bAllAlphaNum,
							  bIsMatchCase,
							  bIsMatchWholeWord,
							  bIsUseReglarExpression,
							  plMatchLength);
		if( lFoundIndex == -1 )
		{
			return lPreFoundIndex;
		}
		else
		{
			lPreFoundIndex = lFoundIndex;
			lStartIndex = lPreFoundIndex + (*plMatchLength);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�q�d�o���ʉ��ߏ���													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::ParseGrepResult
	(	CString&	objFileName,			//�t�@�C����(out)
		long*		plLineNo				//�s�ԍ�(out)
	)
{
	long	lCnt=0;
	objFileName = _T("");
	CString objLineNoStr = _T("");

	for(lCnt=0; lCnt<GetLength(); )
	{
		if( IsDBCSLeadByteEx(0,GetAt(lCnt)) )
		{
			//�t�@�C�����Ƃ���
			objFileName += GetAt(lCnt);
			lCnt+=1;
			objFileName += GetAt(lCnt);
			lCnt+=1;
		}
		else
		{
			if( GetAt(lCnt) == '(' ) //�s�ԍ��n�܂�𔭌�
			{
				lCnt+=1;
				break;
			}
			else
			{
				//�t�@�C�����Ƃ���
				objFileName += GetAt(lCnt);
				lCnt+=1;
			}
		}
	}

	for( ; lCnt<GetLength(); )
	{
		if( !IsDBCSLeadByteEx(0,GetAt(lCnt)) &&
			isdigit(GetAt(lCnt))
		)
		{
			//�s�ԍ��Ƃ���
			objLineNoStr += GetAt(lCnt);
			lCnt+=1;
		}
		else
		{
			break;
		}
	}

	(*plLineNo) = atoi((char*)(LPCTSTR)objLineNoStr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����񒷕ێ�����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SaveLength()
{
	m_lSavedLineLength = GetLength();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ێ����Ă����������񒷎擾����										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLine::GetSavedLength()
{
	return m_lSavedLineLength;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�[�N�t���O�ݒ菈��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SetFlagMark
	(	BOOL	bMark
	)
{
	m_bMark = bMark;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�}�[�N�t���O�擾����													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CLine::GetFlagMark()
{
	return m_bMark;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�ŏI�����@�P�����폜����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::DelEndChar()
{
	if(GetLength() <= 1)
	{
		Set((const unsigned char*)"");
		return;
	}

	long lCnt=0;
	for(;;)
	{
		if(IsDBCSLeadByteEx(0,m_strLine.GetAt(lCnt)))//�Q�޲ĕ����̑���޲�
		{
			if( (lCnt+2) < GetLength() ) //�܂����v
			{
				lCnt+=2;
			}
			else
			{
				SetAt(lCnt,0);
				return;
			}
		}
		else //�P�o�C�g����
		{
			if( (lCnt+1) < GetLength() ) //�܂����v
			{
				lCnt+=1;
			}
			else
			{
				SetAt(lCnt,0);
				return;
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s���̋󔒕����폜����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::DelEndSpaces()
{
	m_strLine.TrimRight();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�s���̋󔒕����폜����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::DelTopSpaces()
{
	m_strLine.TrimLeft();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�C���f���g���擾														|*/
/*|	���	�F���o���镶���܂Ŕ��p�󔒂ɂ��Ă������̃C���f���g�����ׂ�			|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLine::GetIndent(long lTabSize)
{
	long lIndent=0;
	long lOffset=0;
	long lMargin=0;
	unsigned char c=0;

	for(lOffset=0; lOffset<GetLength(); lOffset++)
	{
		c = GetAt(lOffset);
		if(IsDBCSLeadByteEx(0,c))//�Q�o�C�g�����̑��o�C�g
			break;
		else
		{
			if(c == '\t')//�^�u�̏ꍇ�}�[�W�����C���f���g�𑝂₷
			{
				lMargin = lTabSize - (lOffset % lTabSize);
				lIndent += lMargin;
			}
			else if(c == ' ')//���p�󔒂̏ꍇ�C���f���g�͂P���₷
			{
				lIndent++;
			}
			else
				break;
		}
	}

	return lIndent;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�������r															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CLine::Compare
	(	CLine*	pobjLine
	)
{
	if(pobjLine == NULL)
		return 1;
	return m_strLine.Compare(pobjLine->m_strLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�������r															|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CLine::Compare(long lOffset, const unsigned char* pszBuf, long len)
{
	if(lOffset < 0)
		return 1;
	if(lOffset >= m_strLine.GetLength() && lOffset != 0)
		return 1;
	if(len < 0)
		return 1;

	CString buf = m_strLine.Mid(lOffset,len);
	return buf.Compare((LPCTSTR)pszBuf);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r�p�A�s��ʐݒ�											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SetDiffLineKind
	(	int		nDiffLineKind
	)
{
	m_nDiffLineKind = nDiffLineKind;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�@�C����r�p�A�s��ʎ擾											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CLine::GetDiffLineKind()
{
	return m_nDiffLineKind;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r�p�A�s��ʐݒ�										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SetDirCmpLineKind
	(	int		nDirCmpLineKind
	)
{
	m_nDirCmpLineKind = nDirCmpLineKind;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r�p�A�s��ʎ擾										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CLine::GetDirCmpLineKind()
{
	return m_nDirCmpLineKind;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r�ł̃t�@�C�����ݒ�									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SetDirCmpFileName
	(	CString&	objStr
	)
{
	m_objDirCmpFileName = objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�B���N�g����r�ł̃t�@�C�����擾									|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::GetDirCmpFileName
	(	CString&	objStr
	)
{
	objStr = m_objDirCmpFileName;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���O�I�t�Z�b�g�擾����												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CLine::GetPreOffset		//���O�I�t�Z�b�g
	(	long	lLimitOffset	//���~�b�g�I�t�Z�b�g
	)
{
	if( lLimitOffset == 0 ) //���O�͂����Ȃ�
		return -1;

	long lCnt=0;
	for(;;)
	{
		if(IsDBCSLeadByteEx(0,m_strLine.GetAt(lCnt)))//�Q�o�C�g�����̑��o�C�g
		{
			if( (lCnt+2) < lLimitOffset ) //�܂����v
				lCnt+=2;
			else
				return lCnt;
		}
		else //�P�o�C�g����
		{
			if( (lCnt+1) < lLimitOffset ) //�܂����v
				lCnt+=1;
			else
				return lCnt;
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ʒu�̂t�q�k�擾											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CLine::GetUrl
	(	long		lCaretOffset,		//�L�����b�g�ʒu�̃I�t�Z�b�g
		CString&	objUrl				//�t�q�k�ԋp��
	)
{
	CString objBuf = _T("");
	char cBuf=0;
	long lNow=0, lPos=0;
	long lStart=0, lEnd=0;

	if(lCaretOffset < 0 || lCaretOffset >= m_strLine.GetLength())
		return FALSE;

	for(lNow=0; lNow<GetLength(); )
	{
		if((lPos = m_strLine.Find("http://",lNow)) != -1)
			objBuf = "http://";
		else if((lPos = m_strLine.Find("https://",lNow)) != -1)
			objBuf = "https://";
		else if((lPos = m_strLine.Find("ftp://",lNow)) != -1)
			objBuf = "ftp://";
		else if((lPos = m_strLine.Find("gopher://",lNow)) != -1)
			objBuf = "gopher://";
		else
			break;

		//�J�n�I�t�Z�b�g
		lStart = lPos;
		if(lCaretOffset < lStart)
			return FALSE;

		//�I���I�t�Z�b�g
		for(lEnd=lStart+objBuf.GetLength(); lEnd<GetLength(); lEnd+=1)
		{
			cBuf = GetAt(lEnd);
			if( !isValidURL(cBuf) )
			{
				break;
			}
			else
			{
				objBuf += cBuf;
			}
		}
		if(lCaretOffset < lEnd) //�L�����b�g�ʒu�̂t�q�k�ł���
		{
			objUrl = objBuf;
			return TRUE;
		}

		//�����J�n�ʒu�����炷
		lNow = lEnd;
	}
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�t�q�k��`�F�b�N�擾													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CLine::IsAboveURL
	(	long	lOffset
	)
{
	CString objBuf = _T("");
	long lNow=0,lPos=0;
	long lStart=0, lEnd=0;

	if(lOffset < 0 || lOffset >= m_strLine.GetLength())
		return FALSE;

	for(lNow=0; lNow<GetLength(); )
	{
		if((lPos = m_strLine.Find("http://",lNow)) != -1)
			objBuf = "http://";
		else if((lPos = m_strLine.Find("https://",lNow)) != -1)
			objBuf = "https://";
		else if((lPos = m_strLine.Find("ftp://",lNow)) != -1)
			objBuf = "ftp://";
		else if((lPos = m_strLine.Find("gopher://",lNow)) != -1)
			objBuf = "gopher://";
		else
			break;

		//�J�n�I�t�Z�b�g
		lStart = lPos;
		if(lOffset < lStart)
			return FALSE;

		//�I���I�t�Z�b�g�T��
		for(lEnd=lStart+objBuf.GetLength(); lEnd<GetLength(); lEnd+=1)
		{
			if( !isValidURL(GetAt(lEnd)) )
				break;
		}
		if(lOffset < lEnd)
			return TRUE;

		//�����J�n�ʒu�����炷
		lNow = lEnd;
	}
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�L�����b�g�ʒu�̂��|���������擾										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CLine::GetEMail
	(	long		lOffset,
		CString& 	objEMail		//���|���������ԋp��
	)
{
	long lNow=0, lStart=0, lEnd=0;
	long lPos=0;
	BOOL bOK=FALSE;
	char cBuf=0;

	if(lOffset < 0 || lOffset >= m_strLine.GetLength())
		return FALSE;

	for(lNow=0; lNow<GetLength(); )
	{
		bOK = FALSE;
		lStart = lEnd = 0;
		if(GetAt(lNow) == '@')
		{
			lPos = GetPreOffset(lNow);
			if(lPos != -1)
			{
				cBuf = GetAt(lPos);
				if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf)) //�O���`�F�b�N
				{
					cBuf = GetAt(lNow+1);
					if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf)) //����`�F�b�N
						bOK = TRUE;
				}
			}
		}

		if(bOK)
		{
			//�O����T��
			for(lPos=lNow;;)
			{
				lPos = GetPreOffset(lPos);
				if(lPos < 0)
					break;
				cBuf = GetAt(lPos);
				if( !isValidEMail(cBuf) )
					break;
				lStart = lPos;
			}
			//�����T��
			for(lNow+=1;;)
			{
				if(lNow >= GetLength())
					break;
				cBuf = GetAt(lNow);
				if( !isValidEMail(cBuf) )
					break;
				lEnd = lNow;
				lNow+=1;
			}

			if(lStart <= lOffset && lOffset <= lEnd)
			{
				objEMail = m_strLine.Mid(lStart,(lEnd-lStart+1));
				return TRUE;
			}
			else
				continue;
		}
		else 
		{
			lNow+=1;
		}
	}
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F���|����������`�F�b�N�擾											|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CLine::IsAboveEMail
	(	long	lOffset
	)
{
	long lNow=0, lStart=0, lEnd=0;
	long lPos=0;
	BOOL bOK=FALSE;
	char cBuf=0;

	if(lOffset < 0 || lOffset >= m_strLine.GetLength())
		return FALSE;

	for(lNow=0; lNow<GetLength(); )
	{
		bOK = FALSE;
		lStart = lEnd = 0;
		if(GetAt(lNow) == '@')
		{
			lPos = GetPreOffset(lNow);
			if(lPos != -1)
			{
				cBuf = GetAt(lPos);
				if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf)) //�O���`�F�b�N
				{
					cBuf = GetAt(lNow+1);
					if(!IsDBCSLeadByteEx(0,cBuf) && isAlnum(cBuf)) //����`�F�b�N
						bOK = TRUE;
				}
			}
		}

		if(bOK)
		{
			//�O����T��
			for(lPos=lNow;;)
			{
				lPos = GetPreOffset(lPos);
				if(lPos < 0)
					break;
				cBuf = GetAt(lPos);
				if( !isValidEMail(cBuf) )
					break;
				lStart = lPos;
			}
			//�����T��
			for(lNow+=1;;)
			{
				if(lNow >= GetLength())
					break;
				cBuf = GetAt(lNow);
				if( !isValidEMail(cBuf) )
					break;
				lEnd = lNow;
				lNow+=1;
			}

			if(lStart <= lOffset && lOffset <= lEnd)
				return TRUE;
			else
				continue;
		}
		else 
		{
			lNow+=1;
		}
	}
	return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�[�^�擪�|�C���^�擾												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
const unsigned char* CLine::Data()
{
	return (const unsigned char*)(LPCTSTR)m_strLine;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�f�[�^�擪�|�C���^�擾												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
const unsigned char* CLine::Data(long lOffset)
{
	if(lOffset < 0 || lOffset >= m_strLine.GetLength())
		return NULL;
	return (const unsigned char*)(LPCTSTR)m_strLine + lOffset;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����񉻂��Ď擾														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
const unsigned char* CLine::String()
{
	return (const unsigned char*)(LPCTSTR)m_strLine;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F����������擾														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
const unsigned char* CLine::SubString(long lOffset)
{
	if(lOffset < 0 || lOffset >= m_strLine.GetLength())
		return NULL;
	m_strBuf = m_strLine.Mid(lOffset,m_strLine.GetLength()-lOffset);
	return (const unsigned char*)(LPCTSTR)m_strBuf;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�F�f�[�^��T�C�Y�ύX													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::ResizeColorBuffer(long size, unsigned char c)
{
	m_Color.SetSize(size);
	for(long l=0; l<size; l++)
		m_Color.SetAt(l,(CObject*)0);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�A���_�[���C���f�[�^��T�C�Y�ύX										|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::ResizeUnderLineBuffer(long size, unsigned char c)
{
	m_bNeedUnderLine.SetSize(size);
	for(long l=0; l<size; l++)
		m_bNeedUnderLine.SetAt(l,(CObject*)FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�����̋󔒕������P��												|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::Spaces2One()
{
	long nCnt=0;
	unsigned char c1stChar=0, c2ndChar=0;

	for(nCnt=0; nCnt<GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,GetAt(nCnt)))//�Q�o�C�g�����̑��o�C�g
		{
			nCnt+=2;
			continue;
		}
		if(GetAt(nCnt) != '\t' && GetAt(nCnt) != ' ')//�^�u��X�y�[�X�ȊO
		{
			nCnt++;
			continue;
		}

		//�A������󔒕������폜
		for(;;)
		{
			c1stChar=0, c2ndChar=0;
			DeleteChar(nCnt,&c1stChar,&c2ndChar);

			if(IsDBCSLeadByteEx(0,GetAt(nCnt)))
				break;
			else if(GetAt(nCnt) != '\t' && GetAt(nCnt) != ' ')
				break;
		}

		if(nCnt >= GetLength())
			break;

		//���p�X�y�[�X���P�}��
		InsertChar(nCnt,' ',0);
		nCnt++;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�p���|���啶����														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::Al2Upper()
{
	for(long nCnt=0; nCnt<GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,GetAt(nCnt)))//�Q�o�C�g�����̑��o�C�g
		{
			nCnt+=2;
			continue;
		}
		else //���p
		{
			if( isAlSmall(GetAt(nCnt)) ) //�p������
			{
				//����
			}
			else
			{
				nCnt++;
				continue;
			}
		}

		//�p�������擾
		unsigned char cSmallChar = GetAt(nCnt);
		//�p���������폜
		unsigned char c1stChar=0, c2ndChar=0;
		DeleteChar(nCnt,&c1stChar,&c2ndChar);
		//�啶���ϊ����}��
		unsigned char cBigChar = cSmallChar - 0x20;
		InsertChar(nCnt,cBigChar,0);
		nCnt++;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�p�����|�����p��														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::AlnumZen2Han()
{
	UINT unChar=0;
	long lEndOffsetAfter=GetLength();
	CCodeConvManager	objCodeConvManager;

	for(long nCnt=0; nCnt<lEndOffsetAfter; )
	{
		if(IsDBCSLeadByteEx(0,GetAt(nCnt)))//�Q�o�C�g�����̑��o�C�g
		{
			//�����R�[�h�擾
			unChar = ( (GetAt(nCnt) << 8) | GetAt(nCnt+1) );
			if( objCodeConvManager.IsZenkakuAlnum(unChar) == TRUE )//�S�p�p����
			{
				//����
			}
			else
			{
				nCnt+=2;
				continue;
			}
		}
		else //���p
		{
			nCnt++;
			continue;
		}

		//�S�p�p�����擾
		UINT nZenChar = unChar;
		//�S�p�p�������폜
		unsigned char c1stChar=0, c2ndChar=0;
		DeleteChar(nCnt,&c1stChar,&c2ndChar);
		//���p�p�����ϊ����}��
		unsigned char cHanChar=0;
		cHanChar = objCodeConvManager.AlnumToHan(nZenChar);
		InsertChar(nCnt,(cHanChar&0x00ff),0);
		nCnt++;
		//�I���I�t�Z�b�g���X�V
		lEndOffsetAfter--;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�J�^�J�i�|�����p��													|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::KatakanaZen2Han()
{
	UINT unChar=0;
	long lEndOffsetAfter=GetLength();
	CCodeConvManager	objCodeConvManager;

	for(long nCnt=0; nCnt<lEndOffsetAfter; )
	{
		if(IsDBCSLeadByteEx(0,GetAt(nCnt)))//�Q�o�C�g�����̑��o�C�g
		{
			//�����R�[�h�擾
			unChar = ( (GetAt(nCnt) << 8) | GetAt(nCnt+1) );
			if( _ismbckata(unChar) || //�S�p�J�^�J�i
				unChar==0x814a     || // �J
				unChar==0x814b     || // �K
				unChar==0x815b	      // �[
			)
			{
				//����
			}
			else
			{
				nCnt+=2;
				continue;
			}
		}
		else //���p
		{
			nCnt++;
			continue;
		}

		//�S�p�J�^�J�i�擾
		UINT nZenKana = unChar;
		//�S�p�J�^�J�i���폜
		unsigned char c1stChar=0, c2ndChar=0;
		DeleteChar(nCnt,&c1stChar,&c2ndChar);
		//���p�J�^�J�i�ϊ����}��
		unsigned char cHanKana=0;
		if(_ismbckata(nZenKana))
		{
			if( objCodeConvManager.IsMaruKana(nZenKana) )
			{
				cHanKana = _mbctombb(nZenKana);
				InsertChar(nCnt,(cHanKana&0x00ff),0);
				nCnt++;
				//߁@��ǉ��}��
				cHanKana = (char)0xdf;
				InsertChar(nCnt,(cHanKana&0x00ff),0);
				nCnt++;
			}
			else if( objCodeConvManager.IsDakuonKana(nZenKana) )
			{
				cHanKana = _mbctombb(nZenKana);
				InsertChar(nCnt,(cHanKana&0x00ff),0);
				nCnt++;
				//ށ@��ǉ��}��
				cHanKana = (char)0xde;
				InsertChar(nCnt,(cHanKana&0x00ff),0);
				nCnt++;
			}
			else
			{
				cHanKana = _mbctombb(nZenKana);
				InsertChar(nCnt,(cHanKana&0x00ff),0);
				nCnt++;
				//�I���I�t�Z�b�g���X�V
				lEndOffsetAfter--;
			}
		}
		else if(nZenKana==0x814a) // �J --->  �
		{
			cHanKana = (char)0xde;
			InsertChar(nCnt,(cHanKana&0x00ff),0);
			nCnt++;
			//�I���I�t�Z�b�g���X�V
			lEndOffsetAfter--;
		}
		else if(nZenKana==0x814b) // �K --->  �
		{
			cHanKana = (char)0xdf;
			InsertChar(nCnt,(cHanKana&0x00ff),0);
			nCnt++;
			//�I���I�t�Z�b�g���X�V
			lEndOffsetAfter--;
		}
		else if(nZenKana==0x815b) // �[ --->  �
		{
			cHanKana = (char)0xb0;
			InsertChar(nCnt,(cHanKana&0x00ff),0);
			nCnt++;
			//�I���I�t�Z�b�g���X�V
			lEndOffsetAfter--;
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	�T�v	�F�󔒁|�����p��														|*/
/*|	���	�F																		|*/
/*|	���l	�F																		|*/
/*|	����	�F																		|*/
/*|	I/F		�F																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CLine::SpaceZen2Han()
{
	UINT unChar=0;
	long lEndOffsetAfter=GetLength();

	for(long nCnt=0; nCnt<lEndOffsetAfter; )
	{
		if(IsDBCSLeadByteEx(0,GetAt(nCnt)))//�Q�o�C�g�����̑��o�C�g
		{
			//�����R�[�h�擾
			unChar = ( (GetAt(nCnt) << 8) | GetAt(nCnt+1) );
			if( unChar == 0x8140 )//�S�p��
			{
				//����
			}
			else
			{
				nCnt+=2;
				continue;
			}
		}
		else //���p
		{
			nCnt++;
			continue;
		}

		//�S�p�󔒂��폜
		unsigned char c1stChar=0, c2ndChar=0;
		DeleteChar(nCnt,&c1stChar,&c2ndChar);
		//���p�󔒑}��
		InsertChar(nCnt,' ',0);
		nCnt++;
		//�I���I�t�Z�b�g���X�V
		lEndOffsetAfter--;
	}
}

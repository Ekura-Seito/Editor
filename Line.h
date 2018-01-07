// Line.h: CLine �N���X�̃C���^�[�t�F�C�X
//

#if !defined(LINE_H)		//�Q�d�C���N���[�h�h�~
#define LINE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CLine : public CObject
{
	friend class CLine;

public:
	CLine();												// �R���X�g���N�^
	CLine(const unsigned char*);							// �R���X�g���N�^
	virtual ~CLine();										// �f�X�g���N�^
	unsigned char operator[](long) const;					// �P�����擾
	unsigned char GetAt(long) const;						// �P�����擾
	void SetAt(long,unsigned char);							// �P�����ݒ�
	long GetLength();										// �����񒷎擾
	void PadSpace(long);									// �X�y�[�X�p�f�B���O
	void Set(const unsigned char*);							// �P�s�ݒ�
	void Catenate(const unsigned char*);					// �A��
	void Catenate(unsigned char);							// �P�����A��
	void Insert(long,const unsigned char*);					// ������}��
	void InsertChar(long,unsigned char,unsigned char);		// �����}��
	void InsertChars(long,unsigned char,long);				// �����w����}��
	void OverWriteChar(long,unsigned char,unsigned char);	// �����㏑��
	void DeleteChar(long,unsigned char*,unsigned char*);	// �����폜(DELETE)
	void BackSpaceChar(long,unsigned char*,unsigned char*);	// �����폜(BACK SPACE)
	void Trim(long,long);									// ������؂���
	long GetTabCount();										// �^�u���擾
	COLORREF GetColorAt(long);								// �F�擾����
	void SetColorAt(long,COLORREF);							// �F�ݒ菈��
	void SetColorAt(long,long,COLORREF);					// �F�ݒ菈��
	BOOL GetUnderLineAt(long);								// �������擾����
	void SetUnderLineAt(long,BOOL);							// �������ݒ菈��
	long DoFind(long,long,CString,BOOL,BOOL,BOOL,BOOL,long*);// �������s
	long DoReverseFind(long,long,CString&,BOOL,BOOL,BOOL,BOOL,long*);// �������s�i�t�����j
	void ParseGrepResult(CString&,long*);					// �f�q�d�o���ʉ���
	void SaveLength();										// �����񒷕ێ�
	long GetSavedLength();									// �ێ����Ă����������񒷎擾
	void SetFlagMark(BOOL);									// �}�[�N�t���O�ݒ�
	BOOL GetFlagMark();										// �}�[�N�t���O�擾
	void DelEndChar();										// �ŏI�����@�P�����폜
	void DelTopSpaces();									// �s���̋󔒕����폜
	void DelEndSpaces();									// �s���̋󔒕����폜
	long GetIndent(long);									// �C���f���g���擾
	int	 Compare(CLine*);									// �������r
	int  Compare(long,const unsigned char*,long);			// �������r
	void SetDiffLineKind(int);								// �t�@�C����r�p�A�s��ʐݒ�
	int  GetDiffLineKind();									// �t�@�C����r�p�A�s��ʎ擾
	void SetDirCmpLineKind(int);							// �f�B���N�g����r�p�A�s��ʐݒ�
	int  GetDirCmpLineKind();								// �f�B���N�g����r�p�A�s��ʎ擾
	void SetDirCmpFileName(CString&);						// �f�B���N�g����r�ł̃t�@�C�����ݒ�
	void GetDirCmpFileName(CString&);						// �f�B���N�g����r�ł̃t�@�C�����擾
	long GetPreOffset(long);								// ���O�I�t�Z�b�g�擾
	BOOL GetUrl(long,CString&);								// �L�����b�g�ʒu�̂t�q�k�擾
	BOOL IsAboveURL(long);									// �t�q�k��`�F�b�N
	BOOL GetEMail(long,CString&);							// �L�����b�g�ʒu�̂��|���������擾
	BOOL IsAboveEMail(long);								// ���|����������`�F�b�N
	const unsigned char* Data();							// �擪�f�[�^�̃|�C���^�擾
	const unsigned char* Data(long);
	const unsigned char* String();							// ������擾
	const unsigned char* SubString(long);					// ����������擾
	void ResizeColorBuffer(long,unsigned char);
	void ResizeUnderLineBuffer(long,unsigned char);
	void Spaces2One();										// �����̋󔒕������P��
	void Al2Upper();										// �p���|���啶����
	void AlnumZen2Han();									// �p�����|�����p��
	void KatakanaZen2Han();									// �J�^�J�i�|�����p��
	void SpaceZen2Han();									// �󔒁|�����p��

private:
	CEditorApp*	m_pApp;
	CString		m_strLine;									// �P�s�f�[�^
	CString		m_strBuf;
	CObArray	m_Color;									// �F���
	CObArray	m_bNeedUnderLine;							// �������
	BOOL		m_bMark;									// �}�[�N�t���O
	int			m_nDiffLineKind;							// �t�@�C����r�p�A�s���
	int			m_nDirCmpLineKind;							// �f�B���N�g����r�p�A�s���
	CString		m_objDirCmpFileName;						// �f�B���N�g����r�ł̃t�@�C����
	long		m_lSavedLineLength;
};

#endif // !defined(LINE_H)


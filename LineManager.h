// LineManager.h: CLineManager �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(LINEMANAGER_H)		//�Q�d�C���N���[�h�h�~
#define LINEMANAGER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CLine;

class CLineManager
{
public:
	CLineManager();										// �R���X�g���N�^
	virtual ~CLineManager();							// �f�X�g���N�^

	void Initialize();									// ������
	void Serialize(CArchive& ar);						// �V���A���C�Y
	void SerializeBinary(CArchive& ar);					// �V���A���C�Y�i�o�C�i�����[�h�j
	void GetBytes(long,long*,long*);					// �o�C�g���擾
	long GetTabSize();									// �^�u�T�C�Y�擾
	void SetTabSize(long);								// �^�u�T�C�Y�ݒ�
	long GetLineCount();								// �s���擾����
	CLine* GetOneLine(long);							// �w��s�̕�����擾����
	void SetOneLine(long,CLine*);						// �w��s�̕�����ݒ菈��
	void DeleteOneLine(long);							// �w��s�폜����
	void InsertOneLine(long,CLine*);					// �s�}������
	long GetMaxLineLength();							// �P�s�̒����̍ő�l���擾
	long GetLineLength(long);							// �w��s�̕����񒷎擾����
	void AddCharacter(long,long,UINT,UINT,BOOL);		// �����o�^
	void ProcessReturnCode(long,long);					// ���s�R�[�h����
	void ProcessDeleteCode(long,long,unsigned char*,unsigned char*);	// DELETE�R�[�h����
	void ProcessBackSpaceCode(long,long,unsigned char*,unsigned char*);	// BACK SPACE����
	CLine* MyGetSelectedText(long,long,long,long);		// �I��͈̓e�L�X�g�擾
	void AppendGrepLine(CLine&);						// �f�q�d�o���ʂP�s�ǉ�
	void AppendHttpLine(CLine&);						// �g�s�s�o�_�E�����[�h���ʂP�s�ǉ�
	void AppendConvLine(CLine&);						// �����R�[�h�ϊ����ʂP�s�ǉ�
	void AppendDumpLine(CLine&);						// �_���v���ʂP�s�ǉ�
	void AppendWCountLine(CLine&);						// ���[�h�J�E���g���ʂP�s�ǉ�
	void AppendFSearchLine(CLine&);						// �t�@�C���������ʂP�s�ǉ�
	void AppendBinaryLine(CLine&);						// �o�C�i���\���P�s�ǉ�
	void AppendEncriptLine(CLine&);						// �t�@�C���Í������ʂP�s�ǉ�
	void AppendDecriptLine(CLine&);						// �t�@�C�����������ʂP�s�ǉ�
	void AppendEncodeLine(CLine&);						// �G���R�[�h���ʂP�s�ǉ�
	void AppendCompareLine(CLine*);						// �t�@�C����r���ʂP�s�ǉ�
	void InsertCompareLine(long,CLine*);				// �t�@�C����r���ʂP�s�}��
	void InsertDirCmpLine(long,CLine*);					// �f�B���N�g����r���ʂP�s�}��
	int  GetDiffLineKind(long);							// �t�@�C����r�p�s��ʎ擾
	int  GetDirCmpLineKind(long);						// �f�B���N�g����r�p�����ʎ擾
	void GetCRLFCount(long*,long*);						// �b�q�A�k�e�̐��擾����
	long GetKaigyoKind();								// ���s�R�[�h�敪�擾
	void SetKaigyoKind(long);							// ���s�R�[�h�敪�ݒ�
	void SetFlagMark(long,BOOL);						// �}�[�N�t���O�ݒ�
	BOOL GetFlagMark(long);								// �}�[�N�t���O�擾
	long GetMarkLineCount();							// �}�[�N�s���擾
	void DelEndSpaces(long);							// �w��s�̍s���󔒍폜����
	void ConvSpace2TabOnSave(long,long);				// �ۑ����p�X�y�[�X�|���^�u�ϊ�����

private:
	CEditorApp* m_pApp;									// �A�v���P�[�V�����N���X�ւ̃|�C���^
	long		m_lMarkLineCount;						// �}�[�N�s��
	long		m_lCRCount;
	long		m_lLFCount;
	long		m_lKaigyoKind;							// �g�p������s�R�[�h�̋敪
	long		m_lTabSize;								// �^�u�T�C�Y�i�Sor�W�j
	long		m_lMaxLineLength;						// �P�s�̒����̍ő�l
	CObArray 	m_objStringArray;						// �h�L�������g�̍s�P�ʂ̔z��

protected:
	void Clear();										// �h�L�������g�̍s�P�ʂ̔z���������
};

#endif // !defined(LINEMANAGER_H)


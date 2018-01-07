// FileCompare.h : CFileCompare �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(FILECOMPARE_H)		//�Q�d�C���N���[�h�h�~
#define FILECOMPARE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CEditorDoc;
class CFileCompareStatusDlg;

class CFileCompare
{
public:
	CFileCompare();									// �R���X�g���N�^
	virtual ~CFileCompare();						// �f�X�g���N�^

	BOOL DoCompare(CFileCompareStatusDlg*,CEditorDoc*,CArchive&,CArchive&);
	void GetDelAddLineCount(long*,long*);

protected:
	BOOL ReadFile(CArchive&,CObArray*);				//�t�@�C���ǂݍ���
	BOOL SearchLCS();								//�Œ����ʕ����i�k�b�r : Longest Common Subsequence�j�T��
	BOOL OutputResult();							//���ʏo��
	LONG MakeNodeNo(LONG,LONG);						//�m�[�h�ԍ��擾
	void MakeXY(LONG,LONG*,LONG*);					//
	void SetValuePreIndex(LONG,LONG);				//�O�m�[�h�ݒ�
	LONG GetValuePreIndex(LONG);					//�O�m�[�h�擾
	void SetValueFarY(LONG,LONG);
	LONG GetValueFarY(LONG);
	void Clear();

private:
	CEditorApp*		m_pApp;							//�A�v���P�[�V�����N���X�ւ̃|�C���^
	CFileCompareStatusDlg* m_pobjFileCompareStatusDlg;
	CEditorDoc*		m_pobjCompareDoc;				//�h�L�������g�N���X
	CObArray 		m_objStringArray1;				//���t�@�C���̍s�f�[�^�z��
	CObArray		m_objStringArray2;				//���t�@�C���̍s�f�[�^�z��
	long			m_lFileLineCount1;				//���t�@�C���̍s��
	long			m_lFileLineCount2;				//���t�@�C���̍s��
	long			m_lDelLineCount;				//�폜�s�̐�
	long			m_lAddLineCount;				//�ǉ��s�̐�
	map<long,long> 	m_objMapPreIndex;				//�O�m�[�h�̃}�b�v
	map<long,long>	m_objMapFarY;					//�e�Ίp���̍ŉ��_�̂����W�l�̃}�b�v
};

#endif //!defined(FILECOMPARE_H)


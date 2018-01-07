// DirCompare.h : CDirCompare �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(DIRCOMPARE_H)		//�Q�d�C���N���[�h�h�~
#define DIRCOMPARE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CEditorDoc;
class CDirCompareStatusDlg;

class CDirCompare
{
public:
	CDirCompare();									// �R���X�g���N�^
	virtual ~CDirCompare();							// �f�X�g���N�^

	BOOL DoCompare(CDirCompareStatusDlg*,CEditorDoc*,CString&,CString&);

protected:
	BOOL MakeFileList(CString&,CObArray*);			//�f�B���N�g�����t�@�C���ꗗ�쐬
	void AddFileBySort(CString&,CObArray*);
	BOOL SearchLCS();								//�Œ����ʕ����i�k�b�r : Longest Common Subsequence�j�T��
	BOOL OutputResult();							//���ʏo��
	int  CheckIsSameFile(CString*,int*);			//�t�@�C������`�F�b�N
	LONG MakeNodeNo(LONG,LONG);						//�m�[�h�ԍ��擾
	void MakeXY(LONG,LONG*,LONG*);					//
	void SetValuePreIndex(LONG,LONG);				//�O�m�[�h�ݒ�
	LONG GetValuePreIndex(LONG);					//�O�m�[�h�擾
	void SetValueFarY(LONG,LONG);
	LONG GetValueFarY(LONG);
	void Clear();

private:
	CEditorApp*		m_pApp;							//�A�v���P�[�V�����N���X�ւ̃|�C���^
	CDirCompareStatusDlg* m_pobjDirCompareStatusDlg;
	CEditorDoc*		m_pobjCompareDoc;				//�h�L�������g�N���X
	CObArray 		m_objStringArray1;				//���f�B���N�g���̃t�@�C�����z��
	CObArray		m_objStringArray2;				//���f�B���N�g���̃t�@�C�����z��
	long			m_lFileCount1;					//���f�B���N�g�����̃t�@�C����
	long			m_lFileCount2;					//���f�B���N�g�����̃t�@�C����
	map<long,long> 	m_objMapPreIndex;				//�O�m�[�h�̃}�b�v
	map<long,long> 	m_objMapFarY;					//�e�Ίp���̍ŉ��_�̂����W�l�̃}�b�v
	CString			m_objDir1;						//���f�B���N�g����
	CString			m_objDir2;						//���f�B���N�g����
};

#endif //!defined(DIRCOMPARE_H)


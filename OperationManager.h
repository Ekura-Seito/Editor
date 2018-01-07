// OperationManager.h: COperationManager �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(OperationManager_H)		//�Q�d�C���N���[�h�h�~
#define OperationManager_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class COperation;
class CEditorView;
class CEditorDoc;

typedef CTypedPtrList<CObList, COperation*> COperationList;

class COperationManager
{
public:
	COperationManager();											// �R���X�g���N�^
	virtual 	~COperationManager();								// �f�X�g���N�^

	void		Initialize();										// ������
	void		DispOperationLists();								// �I�y���[�V�����ꗗ�\��
	void		SetOperationFileName(CString&);						// �I�y���[�V�����t�@�C�����ݒ�
	void		GetOperationFileName(CString&);						// �I�y���[�V�����t�@�C�����擾
	void		SetOperationName(CString&);							// �I�y���[�V�������ݒ�
	void		GetOperationName(CString&);							// �I�y���[�V�������擾
	long		GetOperationCount();								// �I�y���[�V�������擾
	void		addOperation(COperation*);							// �I�y���[�V�����I�u�W�F�N�g��ǉ�
	void		doOperation(CEditorView*,CEditorDoc*);				// �I�y���[�V�����A�����s
	BOOL		stepDoOperation(CEditorView*,CEditorDoc*,BOOL);		// �I�y���[�V�����̃X�e�b�v���s
	void		endStepDoOperation();								// �I�y���[�V�����̃X�e�b�v���s�I��
	BOOL		checkIsOperationFile(CArchive&);					// �I�y���[�V�����t�@�C���`�F�b�N
	BOOL		Serialize(CArchive&);								// �V���A���C�Y

	void		SearchStartOperation();								// �I�y���[�V�������������J�n
	COperation* SearchGetOperation();								// �I�y���[�V������������
	void		SearchEndOperation();								// �I�y���[�V�������������I��

protected:
	void		clearAllItem();										// ���X�g�̃����o�����ׂăN���A

private:
	CEditorApp*	m_pApp;												// �A�v���P�[�V�����N���X�ւ̃|�C���^
	CString		m_objOperationFileName;								// �I�y���[�V�����t�@�C����
	CString		m_objOperationName;									// �I�y���[�V������
	POSITION	m_lCurrentPosition;									// �I�y���[�V�����I�u�W�F�N�g�����ɓ����ʒu
	COperationList m_objOperationList;								// COperation �̃��X�g
};

#endif // !defined(OperationManager_H)


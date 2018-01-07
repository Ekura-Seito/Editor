// UndoItemAddCharacter.h: CUndoItemAddCharacter �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(UNDOITEMADDCHARACTER_H)		//�Q�d�C���N���[�h�h�~
#define UNDOITEMADDCHARACTER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorDoc;

class CUndoItemAddCharacter : public CUndoItem
{
public:
	CUndoItemAddCharacter();								// �R���X�g���N�^
	CUndoItemAddCharacter(CEditorDoc*);						// �R���X�g���N�^
	virtual ~CUndoItemAddCharacter();						// �f�X�g���N�^

	void	Do();											// �������s
	void	Do(long,long,UINT,UINT,BOOL,CPoint&,CPoint&,BOOL,BOOL);
	void	doUndo();										// Undo ���s
	void	doRedo();										// Redo ���s
	BOOL	IsCanUndo();									// Undo �\����
	BOOL	IsCanRedo();									// Redo �\����
	void	setCanUndo(BOOL);								// Undo �\�ݒ�(TRUE:�\,FALSE:�s��)
	void	setCanRedo(BOOL);								// Redo �\�ݒ�(TRUE:�\,FALSE:�s��)

private:
	BOOL		m_bIsCanUndo;								// Undo ���s�\�t���O
	BOOL		m_bIsCanRedo;								// Redo ���s�\�t���O

	CEditorDoc*	m_pobjEditorDoc;							// �h�L�������g�N���X�ւ̃|�C���^

	//������
	long		m_lCurrentLine;								// �����o�^�Ώۂ̍s�ԍ�
	long		m_lCurrentOffset;							// �����o�^�Ώۂ̗�ԍ��i�޲ĒP�ʁj
	UINT		m_n1stChar;									// �o�^��������޲�
	UINT		m_n2ndChar;									// �o�^��������޲�
	BOOL		m_bInsertKey;								// �}���^�㏑��Ӱ���׸�
	UINT		m_nOriginal1stChar;							// �o�^�ʒu�̃I���W�i����������޲�
	UINT		m_nOriginal2ndChar;							// �o�^�ʒu�̃I���W�i����������޲�
	UINT		m_nOriginal3rdChar;							// �o�^�ʒu�̃I���W�i��������O�޲�
	BOOL		m_bOriginalModifiedFlag;					// �I���W�i���̃h�L�������g�C�����
	CString		m_objOriginalDocTitle;						// �I���W�i���̃h�L�������g�^�C�g��
	CPoint		m_objOriginalCaretPos;						// �I���W�i���̃L�����b�g�ʒu
	CPoint		m_objOriginalScrollPosLog;					// �I���W�i���̃X�N���[���ʒu
	BOOL		m_bOriginalDispRuler;						// �I���W�i���̃��[���\���t���O
	BOOL		m_bOriginalDispLineNo;						// �I���W�i���̍s�ԍ��\���t���O
};

#endif // !defined(UNDOITEMADDCHARACTER_H)

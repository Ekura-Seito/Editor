// UndoItemReturn.h: CUndoItemReturn �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(UNDOITEMRETURN_H)		//�Q�d�C���N���[�h�h�~
#define UNDOITEMRETURN_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorDoc;

class CUndoItemReturn : public CUndoItem
{
public:
	CUndoItemReturn();							// �R���X�g���N�^
	CUndoItemReturn(CEditorDoc*);				// �R���X�g���N�^
	virtual ~CUndoItemReturn();					// �f�X�g���N�^

	void	Do();								// �������s
	void	Do(long,long,CPoint&,CPoint&,BOOL,BOOL);
	void	doUndo();							// Undo ���s
	void	doRedo();							// Redo ���s
	BOOL	IsCanUndo();						// Undo �\����
	BOOL	IsCanRedo();						// Redo �\����
	void	setCanUndo(BOOL);					// Undo �\�ݒ�(TRUE:�\,FALSE:�s��)
	void	setCanRedo(BOOL);					// Redo �\�ݒ�(TRUE:�\,FALSE:�s��)

private:
	BOOL		m_bIsCanUndo;					// Undo ���s�\�t���O
	BOOL		m_bIsCanRedo;					// Redo ���s�\�t���O

	CEditorDoc*	m_pobjEditorDoc;				// �h�L�������g�N���X�ւ̃|�C���^

	//������
	long		m_lCurrentLine;					// RETURN�������̍s�ԍ�
	long		m_lCurrentOffset;				// RETURN�������̗�ԍ��i�޲ĒP�ʁj
	BOOL		m_bOriginalModifiedFlag;		// �I���W�i���̃h�L�������g�C�����
	CString		m_objOriginalDocTitle;			// �I���W�i���̃h�L�������g�^�C�g��
	CPoint		m_objOriginalCaretPos;			// �I���W�i���̃L�����b�g�ʒu
	CPoint		m_objOriginalScrollPosLog;		// �I���W�i���̃X�N���[���ʒu
	BOOL		m_bOriginalDispRuler;			// �I���W�i���̃��[���\���t���O
	BOOL		m_bOriginalDispLineNo;			// �I���W�i���̍s�ԍ��\���t���O
};

#endif // !defined(UNDOITEMRETURN_H)

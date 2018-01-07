// UndoItemBackSpaceChar.h: CUndoItemBackSpaceChar �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(UNDOITEMBACKSPACECHAR_H)		//�Q�d�C���N���[�h�h�~
#define UNDOITEMBACKSPACECHAR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorDoc;

class CUndoItemBackSpaceChar : public CUndoItem
{
public:
	CUndoItemBackSpaceChar();					// �R���X�g���N�^
	CUndoItemBackSpaceChar(CEditorDoc*);		// �R���X�g���N�^
	virtual ~CUndoItemBackSpaceChar();			// �f�X�g���N�^

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
	long		m_lCurrentLine;					// ���� BACK SPACE �Ώۂ̍s�ԍ�
	long		m_lCurrentOffset;				// ���� BACK SPACE �Ώۂ̗�ԍ��i�޲ĒP�ʁj
	long		m_lAfterLine;					// BACK SPACE ��̍s�ԍ�
	long		m_lAfterOffset;					// BACK SPACE ��̗�ԍ�
	char		m_cOriginal1stChar;				// BACK SPACE �ʒu�̃I���W�i����������޲�
	char		m_cOriginal2ndChar;				// BACK SPACE �ʒu�̃I���W�i����������޲�
	BOOL		m_bOriginalModifiedFlag;		// �I���W�i���̃h�L�������g�C�����
	CString		m_objOriginalDocTitle;			// �I���W�i���̃h�L�������g�^�C�g��
	CPoint		m_objOriginalCaretPos;			// �I���W�i���̃L�����b�g�ʒu
	CPoint		m_objAfterCaretPos;				// BACK SPACE��̃L�����b�g�ʒu
	CPoint		m_objPreLineEndPos;				// BACK SPACE�Ώۂ̑O�s�̍s���ʒu
	CPoint		m_objOriginalScrollPosLog;		// �I���W�i���̃X�N���[���ʒu
	BOOL		m_bOriginalDispRuler;			// �I���W�i���̃��[���\���t���O
	BOOL		m_bOriginalDispLineNo;			// �I���W�i���̍s�ԍ��\���t���O
};

#endif // !defined(UNDOITEMBACKSPACECHAR_H)

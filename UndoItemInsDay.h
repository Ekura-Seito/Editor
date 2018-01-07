// UndoItemInsDay.h: CUndoItemInsDay �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(UndoItemInsDay_H)		//�Q�d�C���N���[�h�h�~
#define UndoItemInsDay_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorDoc;

class CUndoItemInsDay : public CUndoItem
{
public:
	CUndoItemInsDay();										// �R���X�g���N�^
	CUndoItemInsDay(CEditorDoc*);							// �R���X�g���N�^
	virtual ~CUndoItemInsDay();								// �f�X�g���N�^

	void	Do();											// �������s
	void	Do(long,long,CString&,CPoint&,CPoint&,BOOL,BOOL);
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
	long		m_lCaretLine;								// ���t�}���ʒu�̍s�ԍ�
	long		m_lCaretOffset;								// ���t�}���ʒu�̗�ԍ�
	long		m_lSelectStartLine;							// ���t�}��������̑I���J�n�s�ԍ�
	long		m_lSelectStartOffset;						// ���t�}��������̑I���J�n��ԍ�
	long		m_lSelectEndLine;							// ���t�}��������̑I���I���s�ԍ�
	long		m_lSelectEndOffset;							// ���t�}��������̑I���I����ԍ�

	CString		m_objDayStr;								// ���t������

	BOOL		m_bOriginalModifiedFlag;					// �I���W�i���̃h�L�������g�C�����
	CString		m_objOriginalDocTitle;						// �I���W�i���̃h�L�������g�^�C�g��
	CPoint		m_objOriginalCaretPos;						// �I���W�i���̃L�����b�g�ʒu
	CPoint		m_objOriginalScrollPosLog;					// �I���W�i���̃X�N���[���ʒu
	BOOL		m_bOriginalDispRuler;						// �I���W�i���̃��[���\���t���O
	BOOL		m_bOriginalDispLineNo;						// �I���W�i���̍s�ԍ��\���t���O
};

#endif // !defined(UndoItemInsDay_H)


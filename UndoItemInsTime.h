// UndoItemInsTime.h: CUndoItemInsTime �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(UndoItemInsTime_H)		//�Q�d�C���N���[�h�h�~
#define UndoItemInsTime_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorDoc;

class CUndoItemInsTime : public CUndoItem
{
public:
	CUndoItemInsTime();										// �R���X�g���N�^
	CUndoItemInsTime(CEditorDoc*);							// �R���X�g���N�^
	virtual ~CUndoItemInsTime();							// �f�X�g���N�^

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
	long		m_lCaretLine;								// �����}���ʒu�̍s�ԍ�
	long		m_lCaretOffset;								// �����}���ʒu�̗�ԍ�
	long		m_lSelectStartLine;							// �����}��������̑I���J�n�s�ԍ�
	long		m_lSelectStartOffset;						// �����}��������̑I���J�n��ԍ�
	long		m_lSelectEndLine;							// �����}��������̑I���I���s�ԍ�
	long		m_lSelectEndOffset;							// �����}��������̑I���I����ԍ�

	CString		m_objTimeStr;								// ����������

	BOOL		m_bOriginalModifiedFlag;					// �I���W�i���̃h�L�������g�C�����
	CString		m_objOriginalDocTitle;						// �I���W�i���̃h�L�������g�^�C�g��
	CPoint		m_objOriginalCaretPos;						// �I���W�i���̃L�����b�g�ʒu
	CPoint		m_objOriginalScrollPosLog;					// �I���W�i���̃X�N���[���ʒu
	BOOL		m_bOriginalDispRuler;						// �I���W�i���̃��[���\���t���O
	BOOL		m_bOriginalDispLineNo;						// �I���W�i���̍s�ԍ��\���t���O
};

#endif // !defined(UndoItemInsTime_H)


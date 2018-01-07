// UndoItemDelAsmComment.h: CUndoItemDelAsmComment �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(UndoItemDelAsmComment_H)		//�Q�d�C���N���[�h�h�~
#define UndoItemDelAsmComment_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorDoc;
class CLine;

class CUndoItemDelAsmComment : public CUndoItem
{
public:
	CUndoItemDelAsmComment();								// �R���X�g���N�^
	CUndoItemDelAsmComment(CEditorDoc*);					// �R���X�g���N�^
	virtual ~CUndoItemDelAsmComment();						// �f�X�g���N�^

	void	Do();											// �������s
	void	Do(long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);
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
	long		m_lStartLine;								// �`�r�l�R�����g�폜������̊J�n�ʒu�̍s�ԍ�
	long		m_lStartOffset;								// �`�r�l�R�����g�폜������̊J�n�ʒu�̗�ԍ�
	long		m_lEndLine;									// �`�r�l�R�����g�폜������̏I���ʒu�̍s�ԍ�
	long		m_lEndOffset;								// �`�r�l�R�����g�폜������̏I���ʒu�̗�ԍ�

	long		m_lSelectStartLine;							// �I���J�n�s�ԍ��i�`�r�l�R�����g�����O�j
	long		m_lSelectStartOffset;						// �I���J�n��ԍ��i�`�r�l�R�����g�����O�j
	long		m_lSelectEndLine;							// �I���I���s�ԍ��i�`�r�l�R�����g�����O�j
	long		m_lSelectEndOffset;							// �I���I����ԍ��i�`�r�l�R�����g�����O�j

	long		m_lSelectLineCnt;							// �I��Ώۂ̍s��
	CLine**		m_ppobjSelectLine;							// �I��Ώۃe�L�X�g

	BOOL		m_bOriginalModifiedFlag;					// �I���W�i���̃h�L�������g�C�����
	CString		m_objOriginalDocTitle;						// �I���W�i���̃h�L�������g�^�C�g��
	CPoint		m_objOriginalCaretPos;						// �I���W�i���̃L�����b�g�ʒu
	CPoint		m_objOriginalScrollPosLog;					// �I���W�i���̃X�N���[���ʒu
	BOOL		m_bOriginalDispRuler;						// �I���W�i���̃��[���\���t���O
	BOOL		m_bOriginalDispLineNo;						// �I���W�i���̍s�ԍ��\���t���O
};

#endif // !defined(UndoItemDelAsmComment_H)


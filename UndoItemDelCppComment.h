// UndoItemDelCppComment.h: CUndoItemDelCppComment �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(UndoItemDelCppComment_H)		//�Q�d�C���N���[�h�h�~
#define UndoItemDelCppComment_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorDoc;
class CLine;

class CUndoItemDelCppComment : public CUndoItem
{
public:
	CUndoItemDelCppComment();								// �R���X�g���N�^
	CUndoItemDelCppComment(CEditorDoc*);					// �R���X�g���N�^
	virtual ~CUndoItemDelCppComment();						// �f�X�g���N�^

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
	long		m_lStartLine;								// �b�o�o�R�����g�폜������̊J�n�ʒu�̍s�ԍ�
	long		m_lStartOffset;								// �b�o�o�R�����g�폜������̊J�n�ʒu�̗�ԍ�
	long		m_lEndLine;									// �b�o�o�R�����g�폜������̏I���ʒu�̍s�ԍ�
	long		m_lEndOffset;								// �b�o�o�R�����g�폜������̏I���ʒu�̗�ԍ�

	long		m_lSelectStartLine;							// �I���J�n�s�ԍ��i�b�o�o�R�����g�����O�j
	long		m_lSelectStartOffset;						// �I���J�n��ԍ��i�b�o�o�R�����g�����O�j
	long		m_lSelectEndLine;							// �I���I���s�ԍ��i�b�o�o�R�����g�����O�j
	long		m_lSelectEndOffset;							// �I���I����ԍ��i�b�o�o�R�����g�����O�j

	long		m_lSelectLineCnt;							// �I��Ώۂ̍s��
	CLine**		m_ppobjSelectLine;							// �I��Ώۃe�L�X�g

	BOOL		m_bOriginalModifiedFlag;					// �I���W�i���̃h�L�������g�C�����
	CString		m_objOriginalDocTitle;						// �I���W�i���̃h�L�������g�^�C�g��
	CPoint		m_objOriginalCaretPos;						// �I���W�i���̃L�����b�g�ʒu
	CPoint		m_objOriginalScrollPosLog;					// �I���W�i���̃X�N���[���ʒu
	BOOL		m_bOriginalDispRuler;						// �I���W�i���̃��[���\���t���O
	BOOL		m_bOriginalDispLineNo;						// �I���W�i���̍s�ԍ��\���t���O
};

#endif // !defined(UndoItemDelCppComment_H)


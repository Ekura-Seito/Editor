// UndoItemReplaceSelectWord.h: CUndoItemReplaceSelectWord �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(UNDOITEMREPLACESELECTWORD_H)		//�Q�d�C���N���[�h�h�~
#define UNDOITEMREPLACESELECTWORD_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CEditorDoc;
class CLine;

class CUndoItemReplaceSelectWord : public CUndoItem
{
public:
	CUndoItemReplaceSelectWord();										// �R���X�g���N�^
	CUndoItemReplaceSelectWord(CEditorDoc*);							// �R���X�g���N�^
	virtual ~CUndoItemReplaceSelectWord();								// �f�X�g���N�^

	void	Do();														// �������s
	void	Do(CString&,long,long,long,long,long,long,CPoint&,CPoint&,BOOL,BOOL);
	void	doUndo();													// Undo ���s
	void	doRedo();													// Redo ���s
	BOOL	IsCanUndo();												// Undo �\����
	BOOL	IsCanRedo();												// Redo �\����
	void	setCanUndo(BOOL);											// Undo �\�ݒ�(TRUE:�\,FALSE:�s��)
	void	setCanRedo(BOOL);											// Redo �\�ݒ�(TRUE:�\,FALSE:�s��)

private:
	CEditorApp*	m_pApp;													// �A�v���P�[�V�����N���X�ւ̃|�C���^
	BOOL		m_bIsCanUndo;											// Undo ���s�\�t���O
	BOOL		m_bIsCanRedo;											// Redo ���s�\�t���O

	CEditorDoc*	m_pobjEditorDoc;										// �h�L�������g�N���X�ւ̃|�C���^

	//������
	long		m_lStartLine;											// �u���J�n�ʒu�̍s�ԍ�
	long		m_lStartOffset;											// �u���J�n�ʒu�̗�ԍ�
	long		m_lEndLine;												// �u���I���ʒu�̍s�ԍ�
	long		m_lEndOffset;											// �u���I���ʒu�̗�ԍ�

	long		m_lSelectStartLine;										// �I���J�n�s�ԍ��i�u���O�j
	long		m_lSelectStartOffset;									// �I���J�n��ԍ��i�u���O�j
	long		m_lSelectEndLine;										// �I���I���s�ԍ��i�u���O�j
	long		m_lSelectEndOffset;										// �I���I����ԍ��i�u���O�j

	CLine		m_objSelectWord;										// �I��P��
	CLine		m_objReplaceWord;										// �u����P��

	BOOL		m_bOriginalModifiedFlag;								// �I���W�i���̃h�L�������g�C�����
	CString		m_objOriginalDocTitle;									// �I���W�i���̃h�L�������g�^�C�g��
	CPoint		m_objOriginalCaretPos;									// �I���W�i���̃L�����b�g�ʒu
	CPoint		m_objOriginalScrollPosLog;								// �I���W�i���̃X�N���[���ʒu
	BOOL		m_bOriginalDispRuler;									// �I���W�i���̃��[���\���t���O
	BOOL		m_bOriginalDispLineNo;									// �I���W�i���̍s�ԍ��\���t���O
};

#endif // !defined(UNDOITEMREPLACESELECTWORD_H)


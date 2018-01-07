// UndoItemOleDrop.h: CUndoItemOleDrop �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(UndoItemOleDrop_H)		//�Q�d�C���N���[�h�h�~
#define UndoItemOleDrop_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CEditorDoc;
class CLine;

class CUndoItemOleDrop : public CUndoItem
{
public:
	CUndoItemOleDrop();											// �R���X�g���N�^
	CUndoItemOleDrop(CEditorDoc*);								// �R���X�g���N�^
	virtual ~CUndoItemOleDrop();								// �f�X�g���N�^

	void	Do();												// �������s
	void	Do(COleDataObject*,long,long,CPoint&,CPoint&,BOOL,BOOL);
	void	doUndo();											// Undo ���s
	void	doRedo();											// Redo ���s
	BOOL	IsCanUndo();										// Undo �\����
	BOOL	IsCanRedo();										// Redo �\����
	void	setCanUndo(BOOL);									// Undo �\�ݒ�(TRUE:�\,FALSE:�s��)
	void	setCanRedo(BOOL);									// Redo �\�ݒ�(TRUE:�\,FALSE:�s��)

private:
	CEditorApp*	m_pApp;											// �A�v���P�[�V�����N���X�ւ̃|�C���^
	BOOL		m_bIsCanUndo;									// Undo ���s�\�t���O
	BOOL		m_bIsCanRedo;									// Redo ���s�\�t���O

	CEditorDoc*	m_pobjEditorDoc;								// �h�L�������g�N���X�ւ̃|�C���^

	//������
	long		m_lStartLine;									// �\��t���J�n�ʒu�̍s�ԍ�
	long		m_lStartOffset;									// �\��t���J�n�ʒu�̗�ԍ�
	long		m_lEndLine;										// �\��t���I���ʒu�̍s�ԍ�
	long		m_lEndOffset;									// �\��t���I���ʒu�̗�ԍ�

	CObArray	m_objDropLineArray;								// �h���b�v�e�L�X�g�̔z��

	BOOL		m_bOriginalModifiedFlag;						// �I���W�i���̃h�L�������g�C�����
	CString		m_objOriginalDocTitle;							// �I���W�i���̃h�L�������g�^�C�g��
	CPoint		m_objOriginalCaretPos;							// �I���W�i���̃L�����b�g�ʒu
	CPoint		m_objOriginalScrollPosLog;						// �I���W�i���̃X�N���[���ʒu
	BOOL		m_bOriginalDispRuler;							// �I���W�i���̃��[���\���t���O
	BOOL		m_bOriginalDispLineNo;							// �I���W�i���̍s�ԍ��\���t���O
};

#endif // !defined(UndoItemOleDrop_H)


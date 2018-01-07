// UndoManager.h: CUndoManager �N���X�̃C���^�[�t�F�C�X
//

#if !defined(UNDOMANAGER_H)		//�Q�d�C���N���[�h�h�~
#define UNDOMANAGER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CUndoItem;

class CUndoManager
{
public:
	CUndoManager();							//�R���X�g���N�^
	virtual ~CUndoManager();				//�f�X�g���N�^
	void		Initialize();				//�������p

	void		addUndoItem(CUndoItem*);	//UndoItem�I�u�W�F�N�g��ǉ�
	BOOL		IsCanUndo();				//Undo �\����(TRUE:�\�AFALSE:�s��)
	BOOL		IsCanRedo();				//Redo �\����(TRUE:�\�AFALSE:�s��)
	BOOL		doUndo();					//Undo ���s
	BOOL		doRedo();					//Redo ���s

private:
	POSITION	m_lCurrentPosition;			//UndoItem�I�u�W�F�N�g�����ɓ����ʒu
	CObList		m_objUndoItemList;			//CUndoItem �̃��X�g

protected:
	void		clearAllItem();				//���X�g�̃����o�����ׂăN���A
};

#endif // !defined(UNDOMANAGER_H)


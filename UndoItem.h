// UndoItem.h: CUndoItem �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(UNDOITEM_H)		//�Q�d�C���N���[�h�h�~
#define UNDOITEM_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CUndoItem : public CObject
{
public:
	CUndoItem();							//�R���X�g���N�^
	virtual ~CUndoItem();					//�f�X�g���N�^

	virtual void setCanUndo(BOOL) = 0;		//Undo �\�ݒ�(TRUE:�\,FALSE:�s��)
	virtual void setCanRedo(BOOL) = 0;		//Redo �\�ݒ�(TRUE:�\,FALSE:�s��)
	virtual BOOL IsCanUndo() = 0;			//Undo �\����(TRUE:�\,FALSE:�s��)
	virtual BOOL IsCanRedo() = 0;			//Redo �\����(TRUE:�\,FALSE:�s��)
	virtual void Do() = 0;					//�������s
	virtual void doUndo() = 0;				//Undo ���s
	virtual void doRedo() = 0;				//Redo ���s

protected:
};

#endif // !defined(UNDOITEM_H)

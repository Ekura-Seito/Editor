// Operation.h: COperation �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(Operation_H)		//�Q�d�C���N���[�h�h�~
#define Operation_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CEditorView;
class CEditorDoc;

class COperation : public CObject
{
protected:
	DECLARE_SERIAL(COperation);

public:
	COperation();									// �R���X�g���N�^
	virtual ~COperation();							// �f�X�g���N�^
	virtual void Serialize(CArchive& ar);

	void SetOperationKind(long);					// �I�y���[�V������ʐݒ�
	void SetOnKeyDownParameter(UINT,UINT,UINT,int);	// CView::OnKeyDown�p�����[�^�ݒ�
	void SetOnKeyUpParameter(UINT,UINT,UINT);		// CView::OnKeyUp�p�����[�^�ݒ�
	void SetOnCharParameter(UINT,UINT,UINT);		// CView::OnChar�p�����[�^�ݒ�

	void Do(CEditorView*,CEditorDoc*);				// �I�y���[�V�������s
	int  MySerialize(CArchive&);					// �V���A���C�Y
	void GetOperationString(char*);					// �I�y���[�V�������e�����e�L�X�g�擾

protected:
	void SetOperationString(UINT,UINT,UINT);		// �I�y���[�V�������e�����e�L�X�g�ݒ�

private:
	CEditorApp*	m_pApp;								// �A�v���P�[�V�����N���X�ւ̃|�C���^
	char	m_szOperationString[256];				// �I�y���[�V�������e���������e�L�X�g
	long	m_lOperationKind;						// �I�y���[�V�������

	//CView::OnKeyDown�֘A�p�����[�^
	UINT	m_nChar_OnKeyDown;
	UINT	m_nRepCnt_OnKeyDown;
	UINT	m_nFlags_OnKeyDown;
	int		m_nMultValueOfCaretAccelarete;

	//CView::OnKeyUp�֘A�p�����[�^
	UINT	m_nChar_OnKeyUp;
	UINT	m_nRepCnt_OnKeyUp;
	UINT	m_nFlags_OnKeyUp;

	//CView::OnChar�֘A�p�����[�^
	UINT	m_nChar_OnChar;
	UINT	m_nRepCnt_OnChar;
	UINT	m_nFlags_OnChar;
};

#endif // !defined(Operation_H)


// ColorButton.h : CColorButton �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(COLORBUTTON_H)		//�Q�d�C���N���[�h�h�~
#define COLORBUTTON_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CColorButton : public CButton
{
public:
	CColorButton();									//�R���X�g���N�^
	void SetBackColor(COLORREF,BOOL);				//�{�^���̔w�i�F�ݒ�
	COLORREF GetBackColor();						//�{�^���̔w�i�F�擾

protected:
	void DrawBorders(CDC*,CRect&,UINT);				//�{�^���̘g�`��
	void DrawText(CDC*,CRect&,UINT);				//�{�^���e�L�X�g�`��

private:
	COLORREF		m_colorBack;					//�{�^���̔w�i�F
	

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CColorButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CColorButton();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CColorButton)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(COLORBUTTON_H)

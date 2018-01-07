// MyPropertyPage8.h : CMyPropertyPage8 �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MYPROPERTYPAGE8_H)		//�Q�d�C���N���[�h�h�~
#define MYPROPERTYPAGE8_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMyPropertyPage8 : public CPropertyPage
{
	DECLARE_DYNCREATE(CMyPropertyPage8)

// �R���X�g���N�V����
public:
	CMyPropertyPage8();
	~CMyPropertyPage8();

	BOOL IsChanged();									// �ύX��������
	void ClearChangeFlag();								// 

	STATUSBARDISP	m_stStatusBarDisp;					// �X�e�[�^�X�o�[�\�����\����
	long			m_lStatusDayStyle;					// �X�e�[�^�X�o�[�֕\��������t�\���X�^�C��
	long			m_lStatusHourStyle;					// �X�e�[�^�X�o�[�֕\�����鎞���\���X�^�C��
	long			m_lStatusLineStyle;					// �X�e�[�^�X�o�[�֕\������s�A��\���X�^�C��

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CMyPropertyPage8)
	enum { IDD = IDD_PROPPAGE8 };
	CComboBox	m_objLineStyleCombo;
	CComboBox	m_objHourStyleCombo;
	CComboBox	m_objDayStyleCombo;
	CListBox	m_objCurrentItemListBox;
	CListBox	m_objAddItemListBox;
	CButton	m_objDelButton;
	CButton	m_objAddButton;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CMyPropertyPage8)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CMyPropertyPage8)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnSelchangeComboDaystyle();
	afx_msg void OnSelchangeComboHourstyle();
	afx_msg void OnSelchangeComboLinestyle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL	m_bIsChanged;								// �ύX�����t���O
};

#endif // !defined(MYPROPERTYPAGE8_H)


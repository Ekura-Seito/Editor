// DateMaskEdit.h : �w�b�_�[ �t�@�C��
//

#if !defined(DATEMASKEDIT_H)		//�Q�d�C���N���[�h�h�~
#define DATEMASKEDIT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDateMaskEdit : public CEdit
{
// �R���X�g���N�V����
public:
	CDateMaskEdit();			//�R���X�g���N�^
	void Init();				//������

	void SetYear(UINT);			//�N�ݒ菈��
	void SetMonth(UINT);		//���ݒ菈��
	void SetDay(UINT);			//���ݒ菈��
	UINT GetYear();				//�N�擾����
	UINT GetMonth();			//���擾����
	UINT GetDay();				//���擾����

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CDateMaskEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CDateMaskEdit();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	CString m_objDateFormatString;		//���t�t�H�[�}�b�g
	CString m_objDateString;			//�\�����̓��t������
	UINT m_nYear;						//�N
	UINT m_nMonth;						//��
	UINT m_nDay;						//��
	BOOL m_bMaskKeyInProgress;			//����� '/' �p WM_CHAR���s���t���O

	BOOL CheckChar(UINT nChar);			//���͕�����̫�ϯ���������
	void SendChar(UINT nChar);			//����� '/' �p WM_CHAR���s����

	//{{AFX_MSG(CDateMaskEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif //!defined(DATEMASKEDIT_H)


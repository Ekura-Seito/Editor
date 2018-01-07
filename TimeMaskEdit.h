// TimeMaskEdit.h : �w�b�_�[ �t�@�C��
//

#if !defined(TIMEMASKEDIT_H)		//�Q�d�C���N���[�h�h�~
#define TIMEMASKEDIT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTimeMaskEdit : public CEdit
{
// �R���X�g���N�V����
public:
	CTimeMaskEdit();
	void Init();				//������

	void SetHour(UINT);			//���Ԑݒ菈��
	void SetMinute(UINT);		//���ݒ菈��
	void SetSecond(UINT);		//�b�ݒ菈��
	UINT GetHour();				//���Ԏ擾����
	UINT GetMinute();			//���擾����
	UINT GetSecond();			//�b�擾����

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CTimeMaskEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CTimeMaskEdit();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	CString m_objTimeFormatString;		//���ԃt�H�[�}�b�g
	CString m_objTimeString;			//�\�����̎��ԕ�����
	UINT m_nHour;						//����
	UINT m_nMinute;						//��
	UINT m_nSecond;						//�b
	BOOL m_bMaskKeyInProgress;			//����� ':' �p WM_CHAR���s���t���O

	BOOL CheckChar(UINT nChar);			//���͕�����̫�ϯ���������
	void SendChar(UINT nChar);			//����� '/' �p WM_CHAR���s����

	//{{AFX_MSG(CTimeMaskEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif //!defined(TIMEMASKEDIT_H)


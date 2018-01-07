// BitmapClient.h : CBitmapClient �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(BITMAPCLIENT_H)		//�Q�d�C���N���[�h�h�~
#define BITMAPCLIENT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CBitmapClient : public CWnd
{
public:
	CBitmapClient();								// �R���X�g���N�^
	virtual ~CBitmapClient();						// �f�X�g���N�^

	COLORREF	GetMDIBkgndColor();					// �l�c�h�w�i�F�擾
	void		SetMDIBkgndColor(COLORREF);			// �l�c�h�w�i�F�ݒ�
	BOOL		GetFlagDispLogoBmp();				// ���S�r�b�g�}�b�v�\���t���O�擾
	void		SetFlagDispLogoBmp(BOOL);			// ���S�r�b�g�}�b�v�\���t���O�ݒ�
	int			GetLogoBmpKind();					// ���S�r�b�g�}�b�v��ʎ擾
	void		SetLogoBmpKind(int);				// ���S�r�b�g�}�b�v��ʐݒ�
	CString		GetLogoBmpFilePath();				// �r�b�g�}�b�v�t�@�C���p�X�擾����
	void		SetLogoBmpFilePath(CString);		// �r�b�g�}�b�v�t�@�C���p�X�ݒ菈��
	int			GetLogoPosKind();					// ���S�\���ʒu��ʎ擾
	void		SetLogoPosKind(int);				// ���S�\���ʒu��ʐݒ�
	UINT		GetLogoPosX();						// ���S�\���ʒu�w�擾
	void		SetLogoPosX(UINT);					// ���S�\���ʒu�w�ݒ�
	UINT		GetLogoPosY();						// ���S�\���ʒu�x�擾
	void		SetLogoPosY(UINT);					// ���S�\���ʒu�x�ݒ�

private:
	CEditorApp* m_pApp;								// �A�v���P�[�V�����N���X�ւ̃|�C���^
	BOOL		m_bDispLogoBmp;						// ���S�r�b�g�}�b�v�\���t���O
	int			m_nLogoBmpKind;						// ���S�r�b�g�}�b�v���
	CString 	m_objBmpFilePath;					// ���S�r�b�g�}�b�v�t�@�C���p�X
	int			m_nLogoPosKind;						// ���S�\���ʒu���
	UINT		m_unLogoPosX;						// ���S�r�b�g�}�b�v�\���ʒu�w
	UINT		m_unLogoPosY;						// ���S�r�b�g�}�b�v�\���ʒu�x
	CBitmap		m_objXerxrsLogoBmp;					// Xerxes���S�r�b�g�}�b�v
	BOOL		m_bFavariteLogoLoadOK;				// 
	CBitmap		m_objFavariteLogoBmp;				// �D���ȃ��S�r�b�g�}�b�v
	COLORREF	m_MDIBkgndColor;					// �l�c�h�w�i�F

public:
// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CBitmapClient)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CBitmapClient)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadBMPImage(LPCTSTR,CBitmap&,CPalette*);
};

#endif // !defined(BITMAPCLIENT_H)


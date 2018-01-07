// CG: ���̃t�@�C���́u�X�v���b�V�� �X�N���[���v�R���|�[�l���g�ɂ��ǉ�����Ă��܂��B

#ifndef _SPLASH_SCRN_
#define _SPLASH_SCRN_

class CEditorApp;

class CSplashWnd : public CWnd
{
// �\�z
protected:
	CSplashWnd();

// �A�g���r���[�g:
public:
	CBitmap m_bitmap;

// �I�y���[�V����
public:
	static void EnableSplashScreen(BOOL bEnable = TRUE);
	static void ShowSplashScreen(CWnd* pParentWnd = NULL);
	static void PreTranslateAppMessage(MSG* pMsg);

// �I�[�o�[���C�h
	// ClassWizard �́A���̈ʒu�ɉ��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CSplashWnd)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	~CSplashWnd();
	virtual void PostNcDestroy();

protected:
	BOOL Create(CWnd* pParentWnd = NULL);
	void HideSplashScreen();
	static BOOL c_bShowSplashWnd;
	static CSplashWnd* c_pSplashWnd;

// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CSplashWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;							// �A�v���P�[�V�����N���X�ւ̃|�C���^
};

#endif //!defined(_SPLASH_SCRN_)


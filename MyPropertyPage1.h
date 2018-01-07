// MyPropertyPage1.h : CMyPropertyPage1 �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MYPROPERTYPAGE1_H)		//�Q�d�C���N���[�h�h�~
#define MYPROPERTYPAGE1_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CMyPropertyPage1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CMyPropertyPage1)

// �\�z
public:
	CMyPropertyPage1();									// �R���X�g���N�^
	~CMyPropertyPage1();								// �f�X�g���N�^

	BOOL IsChanged();									// �ύX��������
	void ClearChangeFlag();								// 

	long	m_lSplashTime;								// �X�v���b�V���X�N���[���\������
	CString	m_objExecPasswdStr;							// �N���p�X���[�h

	//{{AFX_DATA(CMyPropertyPage1)
	enum { IDD = IDD_PROPPAGE1 };
	BOOL	m_bShowSplash;
	CEdit	m_objSplashTimeEdit;
	BOOL	m_bIsOpenNewFile;
	BOOL	m_bIsChdirToOpenFile;
	BOOL	m_bMaximizeWindow;
	BOOL	m_bIsSaveWindowPlace;
	int		m_nMaxMRU;
	BOOL	m_bIsFullTitle;
	BOOL	m_bIsDispEOFMark;
	BOOL	m_bIsNoDoubleExecute;
	BOOL	m_bIsExecPasswd;
	CButton	m_objSetPasswdBtn;
	BOOL	m_bIsEnableShellOpen;
	BOOL	m_bIsMakeShotcutDsktop;
	BOOL	m_bIsImmAutoOpen;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMyPropertyPage1)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMyPropertyPage1)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckChdirtoopenfile();
	afx_msg void OnCheckMaxcldwnd();
	afx_msg void OnCheckOpennewfile();
	afx_msg void OnCheckSavewindowplace();
	afx_msg void OnChangeEditMaxmru();
	afx_msg void OnCheckFulltitle();
	afx_msg void OnCheckDispeofmark();
	afx_msg void OnCheckNodoubleexecute();
	afx_msg void OnCheckShowsplash();
	afx_msg void OnChangeEditSplashtime();
	afx_msg void OnCheckExecpasswd();
	afx_msg void OnSetpasswdbtn();
	afx_msg void OnCheckEnableshellopen();
	afx_msg void OnCheckMakeshortcutdsktop();
	afx_msg void OnCheckImmautoopen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;									//�A�v���P�[�V�����N���X�ւ̃|�C���^
	BOOL		m_bIsChanged;							//�ύX�����t���O
};

#endif // !defined(MYPROPERTYPAGE1_H)


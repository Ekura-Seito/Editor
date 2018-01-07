// MyPropertyPage7.h : CMyPropertyPage7 �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MYPROPERTYPAGE7_H)		//�Q�d�C���N���[�h�h�~
#define MYPROPERTYPAGE7_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CMyPropertyPage7 : public CPropertyPage
{
	DECLARE_DYNCREATE(CMyPropertyPage7)

public:
	CMyPropertyPage7();
	~CMyPropertyPage7();

	BOOL IsChanged();									// �ύX��������
	void ClearChangeFlag();								// 

	UINT	m_unLogoPosX;
	UINT	m_unLogoPosY;
	CString m_objBmpFilePath;

	//{{AFX_DATA(CMyPropertyPage7)
	enum { IDD = IDD_PROPPAGE7 };
	CButton	m_objRefBtn;
	BOOL	m_bDispLogoBmp;
	int		m_nLogoBmpKind;
	CEdit	m_objBmpFilePathEdit;
	CEdit	m_objLogoPosXEdit;
	CEdit	m_objLogoPosYEdit;
	int		m_nLogoPosKind;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMyPropertyPage7)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMyPropertyPage7)
	virtual BOOL OnInitDialog();
	afx_msg void OnRef();
	afx_msg void OnCheckLogodisp();
	afx_msg void OnRadioXerxesLogo();
	afx_msg void OnRadioFavariteLogo();
	afx_msg void OnChangeEditBmpFilePath();
	afx_msg void OnChangeEditLogoposx();
	afx_msg void OnChangeEditLogoposy();
	afx_msg void OnRadioTile();
	afx_msg void OnRadioPossitei();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*		m_pApp;
	BOOL	m_bIsChanged;								// �ύX�����t���O
};

#endif // !defined(MYPROPERTYPAGE7_H)


// MyPropertyPage2.h : CMyPropertyPage2 �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MYPROPERTYPAGE2_H)		//�Q�d�C���N���[�h�h�~
#define MYPROPERTYPAGE2_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CMyPropertyPage2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CMyPropertyPage2)

public:
	CMyPropertyPage2();
	~CMyPropertyPage2();

	BOOL IsChanged();						// �ύX��������
	void ClearChangeFlag();					// 

	CString		m_objFileExt;				// �t�@�C���g���q
	LOGFONT		m_stStatusBarLogFont;		// �ð���ް��LOGFONT�\����
	long		m_lDispFontPoint;			// �\���t�H���g�̃|�C���g
	CString		m_objDispFontName;			// �\���t�H���g��
	long		m_lPrintFontPoint;			// �󎚃t�H���g�̃|�C���g
	CString		m_objPrintFontName;			// �󎚃t�H���g��

	//{{AFX_DATA(CMyPropertyPage2)
	enum { IDD = IDD_PROPPAGE2 };
	CComboBox	m_objDispFontNameCombo;
	CComboBox	m_objDispFontSizeCombo;
	CComboBox	m_objPrintFontNameCombo;
	CComboBox	m_objPrintFontSizeCombo;
	CComboBox	m_objFileExtCombo;
	int		m_nTabSize;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMyPropertyPage2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	friend int CALLBACK EnumFontFamiliesCallBack(ENUMLOGFONT*,NEWTEXTMETRIC*,int,LPARAM);

	//{{AFX_MSG(CMyPropertyPage2)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonStsbarfont();
	afx_msg void OnRadio4tab();
	afx_msg void OnRadio8tab();
	afx_msg void OnRadio2tab();
	afx_msg void OnSelchangeComboDispfontname();
	afx_msg void OnSelchangeComboDispfontsize();
	afx_msg void OnSelchangeComboFileext();
	afx_msg void OnSelchangeComboPrintfontname();
	afx_msg void OnSelchangeComboPrintfontsize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp* m_pApp;
	BOOL	m_bIsChanged;								//�ύX�����t���O
};

#endif // !defined(MYPROPERTYPAGE2_H)

